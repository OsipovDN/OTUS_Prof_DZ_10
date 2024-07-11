#pragma once

#include <IPrinter.h>
#include <queue>
#include <thread>
#include <condition_variable>

#include <iostream>
#include <queue>
#include <memory>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>

#include <MassageQueue.h>

class Printer :public IPrinter {
	using Command = std::vector <std::string>;
private:

	std::shared_ptr<IQueue> _tasks;
	std::queue <Command> _completeTasks;

	bool _quite;
	std::mutex _mut;
	std::condition_variable _conditionOutFile;
	std::vector<std::thread> _workers;

	std::string getNameFile();

public:

	Printer(std::shared_ptr<IQueue> q,std::size_t thr_count);
	~Printer();

	void printToStream() override;
	void printToCOut() override;
	void print(Command& c)const;
};