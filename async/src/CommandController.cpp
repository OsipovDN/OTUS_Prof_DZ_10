#include "CommandController.h"

namespace Controller
{
	CommandController::CommandController(std::shared_ptr<IQueue> q, std::size_t count) :_scopeBlockCount(0), _isOpen(false)
	{
		std::cout << __FUNCTION__ << std::endl;

		_msgQueue = q;
		_statPull.reserve(count);
	}

	bool CommandController::isScope(const std::string& str)
	{
		if (str == "{") {
			_isOpen = !_isOpen;
			_scopeBlockCount++;
			return true;
		}
		else if (str == "}") {
			_isOpen = !_isOpen;
			_scopeBlockCount--;
			return true;
		}
		else
			return false;
	}

	void CommandController::addStatBlock(const std::string& str)
	{
		if (_statPull.size() != _statPull.capacity())
			_statPull.emplace_back(str);

		if (_statPull.size() == _statPull.capacity()) {
			_msgQueue->push(_statPull);
			_statPull.clear();
		}
	}

	void CommandController::addDynBlock(const PullBlock& obj)
	{
		for (const auto& it : obj)
			_dynamPull.emplace_back(it);
	}

	void CommandController::addCommand(const std::string& cmd)
	{
		if (isScope(cmd))
		{
			if (_statPull.size() != 0 && _isOpen)
			{
				_msgQueue->push(_statPull);
				_statPull.clear();
			}
			else if (_scopeBlockCount == 0 && !_isOpen)
			{
				addDynBlock(_buf);
				_buf.clear();
				_msgQueue->push(_dynamPull);
				_dynamPull.clear();
			}
		}
		else if (cmd != "EOF")
		{
			if (_scopeBlockCount == 0 && _dynamPull.size() == 0)
				addStatBlock(cmd);
			else
				_buf.emplace_back(cmd);
		}
		else
		{
			if (_statPull.size() != 0)
			{
				_msgQueue->push(_statPull);
				_statPull.clear();
			}
			//_msgQueue->finish();
		}

	}

}