#pragma once
//Boost
#include <boost/asio.hpp>

#include <Session.h>

namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;

class Server
{
public:
	Server(asio::io_context& cont, short port, std::size_t size)
		: _acceptor(cont, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port)),
		_socket(cont),
		_bulkSize(size)
	{
		std::cout << __FUNCTION__ << std::endl;
		do_accept();
	}
	~Server() {
		std::cout << __FUNCTION__ << std::endl;
	}

private:
	void do_accept()
	{
		_acceptor.async_accept(_socket,
			[this](boost::system::error_code ec)
			{
				if (!ec)
				{
					std::cout << "Connection\n";
					auto session = std::make_shared<Session>(std::move(_socket), _bulkSize);
					_session.push_back(session);
					session->start();
				}
				do_accept();
			});
	}

	tcp::acceptor _acceptor;
	tcp::socket _socket;
	std::vector<std::shared_ptr <Session>> _session;

	std::size_t _bulkSize;
};