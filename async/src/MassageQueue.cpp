#include "MassageQueue.h"
#include <chrono>

namespace msg {

	void MassageQueue::push(std::vector <std::string>& massage)
	{
		std::unique_lock<std::mutex> lg(_mut);
		_finish = false;
		_queue.push(massage);
	}

	Command MassageQueue::front()
	{
		std::unique_lock<std::mutex> lg(_mut);
		return _queue.front();
	}

	void MassageQueue::pop()
	{
		std::unique_lock<std::mutex> lg(_mut);
		_queue.pop();
	}

	bool MassageQueue::empty() const { return _queue.empty(); }

	void MassageQueue::print(Command& c)const {
		for (const auto& cmd : c)
		{
			std::cout << cmd << " ";
		}
		std::cout << std::endl;
	}

}