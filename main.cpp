//STL
#include <iostream>
//Boost
#include <boost/asio.hpp>

#include <libasync.h>

namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;


class Session
	: public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket)
		: _socket(std::move(socket))
	{
	}

	void start()
	{
		do_read();
	}

private:
	void do_read()
	{
		auto self(shared_from_this());
		_socket.async_read_some(boost::asio::buffer(_data, _max_length),
			[this, self](boost::system::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					do_write(length);
				}
			});
	}

	void do_write(std::size_t length)
	{
		auto self(shared_from_this());
		boost::asio::async_write(_socket, boost::asio::buffer(_data, length),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
			{
				if (!ec)
				{
					do_read();
				}
			});
	}

	tcp::socket _socket;
	enum { _max_length = 1024 };
	char _data[_max_length];
};

class Server
{
public:
	Server(boost::asio::io_context& context, short port)
		: _acceptor(context, tcp::endpoint(tcp::v4(), port)),
		_socket(context)
	{
		do_accept();
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
};


bool isDig(char* arg)
{
	std::string num = arg;
	auto isdig = [](char ch) {return std::isdigit(ch); };
	return std::all_of(num.cbegin(), num.cend(), isdig);
}

int main(int argc, char* argv[])
{
	int bulkSize;
	short port;

	if (argc == 3)
	{
		if (!isDig(argv[1]) || !isDig(argv[2]))
		{
			std::cout << "Not a val" << std::endl;
			exit(1);
		}
		port = atoi(argv[1]);
		bulkSize = atoi(argv[2]);
	}
	else
	{
		std::cout << "Something is wrong" << std::endl;
		exit(1);
	}

	asio::io_context context;
	Server server(context, port);
	context.run();

	return 0;
}
