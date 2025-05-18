#pragma once
#include <functional>
#include <thread>
#include <mutex>
#include <mutex>

enum TaskStatus
{
	WAITING_FOR_ACTIAVTION,
	RUNNING,
	FINISHED
};

class Task
{
	private:
	std::function<void()> m_task;
	std::thread* m_thread = nullptr;
	std::mutex m_mutex;
	std::condition_variable m_conditionVar;
	std::atomic<TaskStatus> m_taskStatus = TaskStatus::WAITING_FOR_ACTIAVTION;

	void UpdateStatus(TaskStatus status);

	public:
	Task(std::function<void()>& task);

	~Task();

	void Run();

	void Wait(long timeout = 0);

	void Join();

	TaskStatus GetStatus();
};

namespace TaskScheduler
{
	Task* RunAsync(std::function<void()> task);
	void INIT();
}