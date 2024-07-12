#pragma once


class IController {
public:

	virtual void addCommand(const std::string& s) = 0;
	virtual void finish() = 0;
	virtual ~IController()=default;
};