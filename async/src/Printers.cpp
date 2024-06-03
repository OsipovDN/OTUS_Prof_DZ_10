#include "Printers.h"
#include <chrono>

Printer::Printer(std::shared_ptr<IQueue> q, size_t thr_count) :
	_tasks(q),
	_quite(false)
{
	_workers.emplace_back(&Printer::printToCOut, this);
	for (size_t i = 0; i < thr_count; ++i)
	{
		_workers.emplace_back(&Printer::printToStream, this);
	}
}

Printer::~Printer()
{
	std::cout << "dtor" << std::endl;
	{
		std::unique_lock<std::mutex> lock(_mut);
		_quite = true;
	}
	_conditionOutFile.notify_all();

	for (auto& worker : _workers) {
		worker.join();
	}
}

std::string Printer::getNameFile()
{
	static int i = 0;
	std::stringstream ss;
	ss << std::this_thread::get_id();
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string name = "bulk" + ss.str() + std::to_string(time+i++) + ".log";
	return name;
}

void Printer::printToStream()
{
	while (true)
	{	
		std::unique_lock<std::mutex> lock(_mut);
		_conditionOutFile.wait(lock, [this]()->bool
			{
				return !_completeTasks.empty() || _quite;
			});
		if (_quite && _completeTasks.empty())
		{
			return;
		}
		std::string name = getNameFile();
		std::ofstream file(name);
		if (!file.is_open())
		{
			std::cout << "file is not open!" << std::endl;
		}
		else
		{
			auto msg = _completeTasks.front();
			_completeTasks.pop();
			lock.unlock();

			file << "bulk: ";
			std::for_each(msg.cbegin(), msg.cend() - 1, [&file](const std::string& str) {
				file << str << ",";
				});
			file << *(msg.cend() - 1) << std::endl;
			file.close();
		}
	}
}

void Printer::printToCOut()
{
	while (true)
	{
		if (_tasks->isFinish() && _tasks->empty())
			return;
		auto cmd = _tasks->front();
		_tasks->pop();
		_completeTasks.push(cmd);
		_conditionOutFile.notify_one();
		std::cout << "bulk: ";
		std::for_each(cmd.cbegin(), cmd.cend() - 1, [](const std::string& str) {
			std::cout << str << ",";
			});
		std::cout << *(cmd.cend() - 1) << std::endl;
	}
}
void Printer::print(Command& c)const {
	std::cout << "from Printers: ";
	for (auto cmd : c)
	{
		std::cout << cmd << " ";
	}
	std::cout << std::endl;
}
