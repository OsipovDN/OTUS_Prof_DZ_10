#include <sstream>
#include <memory>
#include <thread>
#include <algorithm>

#include "CommandController.h"
#include "MassageQueue.h"
#include "Printers.h"
#include "libasync.h"

namespace async {

	class Handler {
	public:
		Handler():_prt(nullptr), _que(nullptr){};
		void setPrinter(std::shared_ptr<IPrinter> p) { _prt = p; }
		void setQueue(std::shared_ptr<IQueue> q) { _que = q; }
		std::shared_ptr<IPrinter> getPrinter() {return _prt; }
		std::shared_ptr<IQueue> getQueue() { return _que; }
		void remove() 
		{
			_prt = nullptr;
			_que = nullptr;
		}

	private:
		std::shared_ptr<IPrinter> _prt;
		std::shared_ptr<IQueue> _que;
	};

	Handler h;

	handle_t connect(std::size_t bulk) {

		h.setQueue (std::make_shared<msg::MassageQueue>());
		h.setPrinter(std::make_shared<Printer>(h.getQueue(), 2));

		return std::make_unique<Controller::CommandController>(h.getQueue(), bulk).release();
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
		h.remove();
	}

}
