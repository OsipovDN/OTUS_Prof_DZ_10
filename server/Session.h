#pragma once
#include <libasync.h>

namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;


class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket, std::size_t size)
		: _socket(std::move(socket)),
		_bulkSize(size)
	{
		if (_handle == nullptr)
		{
			std::cout << "Create handle\n";
			_handle = async::connect(_bulkSize);
		}
		std::cout << __FUNCTION__ << std::endl;
	}

	void start()
	{
		do_read();
	}

	~Session()
	{
		std::cout << __FUNCTION__ << std::endl;
		async::disconnect(_handle);
	}

private:
	void do_read()
	{

		_socket.async_read_some(boost::asio::buffer(_data, 1024),
			[self = shared_from_this()](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				auto msg = std::string{ self->_data, length };
				msg += "EOF\n";
				//std::cout << "message: " << msg;
				async::receive(self->_handle, msg.c_str(), msg.size());
				self->do_read();
			}
		});
	}

	tcp::socket _socket;
	char _data[1024];

	std::size_t _bulkSize;
	async::handle_t _handle = nullptr;
};