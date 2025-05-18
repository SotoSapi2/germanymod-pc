#include "TaskScheduler.hpp"
#include <cassert>

Task::Task(std::function<void()>& task) : m_task(task)
{}

Task::~Task()
{
	assert(GetStatus() != RUNNING && "Trying deconstruct task while its still running.");

	if (m_thread != nullptr)
	{
		if (m_thread->joinable())
		{
			m_thread->join();
		}

		delete m_thread;
	}
}

void Task::Run()
{
	assert(GetStatus() != RUNNING && "Task is already running.");

	if (m_thread != nullptr)
	{
		delete m_thread;
	}

	m_thread = new std::thread([&]
	{
		try
		{
			m_taskStatus.store(TaskStatus::RUNNING);
			m_task();
		}
		catch (std::exception& ex)
		{}

		m_taskStatus.store(TaskStatus::FINISHED);
		m_conditionVar.notify_all();
	});
}

void Task::Wait(long timeout)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	if (timeout != 0)
	{
		m_conditionVar.wait_for(
			lock, 
			std::chrono::milliseconds(timeout), 
			[&] { return m_taskStatus.load() == TaskStatus::FINISHED; }
		);
	}
	else
	{
		m_conditionVar.wait(
			lock, 
			[&] { return m_taskStatus.load() == TaskStatus::FINISHED; }
		);
	}
}

void Task::Join()
{
	assert(m_thread != nullptr && "Thread is not started yet.");
	assert(m_thread->joinable() && "Thread cannot be joined.");
	m_thread->join();
}

TaskStatus Task::GetStatus()
{
	return m_taskStatus.load();
}