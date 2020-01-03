#pragma once

#include "pch.h"

class ThreadPool {
private:
	std::vector<std::thread> m_workers;
	std::queue<std::function<void()>> m_tasks;

	std::mutex m_queueMutex;
	std::condition_variable m_condition;
	bool stop;

public:
	void init(size_t size) {
		for (size_t i = 0; i < size; i++) {
			m_workers.emplace_back(
				[this]
				{
					for (;;) {
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(this->m_queueMutex);
							this->m_condition.wait(lock,
								[this] { return this->stop || !this->m_tasks.empty(); });
							if (this->stop && this->m_tasks.empty())
								return;
							task = std::move(this->m_tasks.front());
							this->m_tasks.pop();
						}
						task();
					}
				}
				);
		}
	}

	ThreadPool() :
		stop(false) {}
	~ThreadPool() {

		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			stop = true;
		}
		m_condition.notify_all();
		for (std::thread& worker : m_workers)
			worker.join();
	}

	template<class F, class...Args>
	auto enqueue(F&& f, Args&&... args)
		->std::future<typename std::result_of<F(Args...)>::type> {
		using return_type = typename std::result_of<F(Args...)>::type;

		auto task = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);

		std::future<return_type> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			if (stop)
				throw std::runtime_error("Enqueue on stopped ThreadPool");
			m_tasks.emplace([task]() { (*task)(); });
		}
		m_condition.notify_one();
		return res;
	}
};
