#include <sstream>
#include <memory>
#include <thread>
#include <algorithm>

#include "CommandController.h"
#include "MassageQueue.h"
#include "Printers.h"
#include "libasync.h"

namespace async {

	handle_t connect(std::size_t bulk) {

		static auto msgSender = std::make_shared<msg::MassageQueue>();
		static auto filePrinter = std::make_shared<Printer>(msgSender, 2);

		return std::make_unique<Controller::CommandController>(msgSender, bulk).release();
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
