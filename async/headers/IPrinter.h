#pragma once
#include <vector>
#include <string>

class IPrinter {
public:

	virtual void printToStream() = 0;
	virtual void printToCOut() = 0;
	virtual ~IPrinter()=default;
};




