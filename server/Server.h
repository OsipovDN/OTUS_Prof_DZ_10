#pragma once
//Boost
#include <boost/asio.hpp>

#include <Session.h>

namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;

asio::io_context context;
class Server
{
public:
	Server(short port, std::size_t size)
		: _acceptor(context, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port)),
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
		_acceptor.async_accept(
			[this](boost::system::error_code ec, tcp::socket socket)
			{
				if (!ec)
				{
					std::make_shared<Session>(std::move(socket), _bulkSize)->start();
				}
				do_accept();
			});
	}

	tcp::acceptor _acceptor;

	std::size_t _bulkSize;
};