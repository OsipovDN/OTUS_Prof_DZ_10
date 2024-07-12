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
		~CommandController() override { std::cout << __FUNCTION__ << std::endl; };
		void addCommand(const std::string &s) override;
		void finish()override
		{ 
			_msgQueue->finish(); 
		}
	};
}