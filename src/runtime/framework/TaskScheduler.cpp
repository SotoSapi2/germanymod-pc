#include "TaskScheduler.hpp"
#include <Logger.hpp>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

namespace TaskScheduler
{
	bool initialized = false;
	std::vector<Task*> tasks;
	std::thread* taskCollectorThread;
	std::mutex tasksMutex;

	void CollectorTask()
	{
		while (true)
		{
			{
				std::lock_guard<std::mutex> lock(tasksMutex);
				tasks.erase(std::remove_if(tasks.begin(), tasks.end(), [](Task* task)
				{
					if (task->GetStatus() == TaskStatus::FINISHED)
					{
						delete task;
						return true;
					}
					return false;
				}), tasks.end());
			}

			std::this_thread::sleep_for(std::chrono::seconds(10));
		}
	}

	Task* RunAsync(std::function<void()> task)
	{
		if (!initialized)
		{
			throw std::runtime_error("Trying to run task when TaskScheduler is not initialized.");
		}

		Task* taskToBeRun = new Task(task);
		{
			std::lock_guard<std::mutex> lock(tasksMutex);
			tasks.push_back(taskToBeRun);
		}

		taskToBeRun->Run();

		return taskToBeRun;
	}

	void INIT()
	{
		if (initialized)
		{
			throw std::runtime_error("TaskScheduler is already initialized.");
		}

		initialized = true;
		taskCollectorThread = new std::thread(CollectorTask);
	}
}