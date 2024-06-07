#pragma once

#include <mutex>
#include <deque>
#include <vector>

namespace net
{
	template<typename T>
	class tsqueue
	{
	public:
		tsqueue() = default;
		tsqueue(const tsqueue<T>&) = delete;
		virtual ~tsqueue()
		{
			clear();
		}

		const T& front()
		{
			std::scoped_lock(mtxQueue);
			return deqQueue.front();
		}

		const T& back()
		{
			std::scoped_lock lock(mtxQueue);
			return deqQueue.back();
		}

		T pop_front()
		{
			std::scoped_lock lock(mtxQueue);
			auto t = std::move(deqQueue.front());
			deqQueue.pop_front();
			return t;
		}

		void push_back(const T& item)
		{
			std::scoped_lock(mtxQueue);
			deqQueue.emplace_back(std::move(item));

			std::unique_lock<std::mutex> ul(mtxBlocking);
			cvBlocking.notify_one();
		}

		void push_front(const T& item)
		{
			std::scoped_lock(mtxQueue);
			deqQueue.emplace_front(std::move(item));

			std::unique_lock<std::mutex> ul(mtxBlocking);
			cvBlocking.notify_one();

		}

		bool empty()
		{
			std::scoped_lock lock(mtxQueue);
			return deqQueue.empty();
		}

		size_t count()
		{
			std::scoped_lock lock(mtxQueue);
			return deqQueue.size();
		}

		void clear()
		{
			std::scoped_lock lock(mtxQueue);
			deqQueue.clear();
		}

		void wait()
		{
			while (empty())
			{
				std::unique_lock<std::mutex> ul(mtxBlocking);
				cvBlocking.wait(ul);
			}
		}

	protected:
		std::mutex mtxQueue;
		std::deque<T> deqQueue;
		std::condition_variable cvBlocking;
		std::mutex mtxBlocking;
	};
}