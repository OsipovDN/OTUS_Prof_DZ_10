#pragma once
//Boost
#include <boost/asio.hpp>

#include <Session.h>

namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;


class Server
{
	static asio::io_context context;
public:
	Server(short port, std::size_t size)
		: _acceptor(context, tcp::endpoint(tcp::v4(), port)),
		_socket(context),
		_bulkSize(size)
	{
		do_accept();
	}
	void run()
	{
		context.run();
	}
private:
	void do_accept()
	{
		_acceptor.async_accept(_socket,
			[this](boost::system::error_code ec)
			{
				if (!ec)
				{
					std::make_shared<Session>(std::move(_socket))->start();
				}

				do_accept();
			});
	}

	tcp::acceptor _acceptor;
	tcp::socket _socket;

	std::size_t _bulkSize;
};