#pragma once

class IQueue
{
public:

	virtual void push(std::vector <std::string>& massage) = 0;
	virtual void pop() = 0;
	virtual std::vector <std::string> front() = 0;
	virtual bool empty() const = 0;
	virtual bool isFinish()const = 0;
	virtual void finish() = 0;
	virtual ~IQueue() = default;
};