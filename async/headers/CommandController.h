#pragma once
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include <IController.h>
#include <IQueue.h>

using PullBlock = std::vector<std::string>;

class PackageSender;

namespace Controller
{
	class CommandController: public IController {
	private:
		std::shared_ptr <IQueue> _msgQueue;

		PullBlock _statPull;
		PullBlock _dynamPull;
		PullBlock _buf;
		size_t _scopeBlockCount;
		bool _isOpen;

		bool isScope(const std::string& str);
		void addStatBlock(const std::string& str);
		void addDynBlock(PullBlock& obj);

	public:

		CommandController(std::shared_ptr<IQueue> q,std::size_t count);
		~CommandController() { std::cout << "Controller dtor!" << std::endl; };
		virtual void addCommand(std::string &s) override;

	};
}