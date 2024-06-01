#pragma once


class IController {
public:
	virtual void addCommand(std::string& s) = 0;
	virtual ~IController()=default;
};