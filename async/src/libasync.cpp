#include <sstream>
#include <memory>
#include <thread>
#include <algorithm>

#include "CommandController.h"
#include "MassageQueue.h"
#include "Printers.h"
#include "libasync.h"

namespace async {

	class HandlerKeeper {
	public:
		HandlerKeeper():_printerPtr(nullptr),_queuePtr(nullptr){};
		void setPrinter(std::shared_ptr<IPrinter> printerPtr) { _printerPtr = printerPtr; }
		void setQueue(std::shared_ptr<IQueue> queuePtr) { _queuePtr = queuePtr; }
		std::shared_ptr<IPrinter> getPrinter() {return _printerPtr; }
		std::shared_ptr<IQueue> getQueue() { return _queuePtr; }
		void remove() 
		{
			_printerPtr = nullptr;
			_queuePtr = nullptr;
		}

	private:
		std::shared_ptr<IPrinter> _printerPtr;
		std::shared_ptr<IQueue> _queuePtr;
	};

	HandlerKeeper helper;

	handle_t connect(std::size_t bulk) {

		helper.setQueue (std::make_shared<msg::MassageQueue>());
		helper.setPrinter(std::make_shared<Printer>(helper.getQueue(), 2));

		return std::make_unique<Controller::CommandController>(helper.getQueue(), bulk).release();
	}

	void receive(handle_t handler, const char* data, std::size_t size) {

		auto controller = static_cast<Controller::CommandController*>(handler);
		if (handler == nullptr)
			return;

		std::istringstream stream(std::string(data, size));

		std::string temp;
		while (stream >> temp)
		{
			controller->addCommand(temp);
		}
		controller = nullptr;
	}

	void disconnect(handle_t handler) {
		receive(std::move(handler), "EOF", 3);

		auto controller = static_cast<Controller::CommandController*>(handler);
		controller->finish();

	}

}
