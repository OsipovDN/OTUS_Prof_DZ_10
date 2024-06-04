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
	}

	void start()
	{
		do_read();
	}

	~Session()
	{
		async::disconnect(_handle);
	}

private:
	void do_read()
	{
		auto self(shared_from_this());
		_socket.async_read_some(boost::asio::buffer(_data, _maxLength),
			[this, self](boost::system::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					auto msg = std::string{ _data, length };
					if (_handle == nullptr)
					{
						_handle = async::connect(_bulkSize);
					}
					async::receive(_handle, msg.c_str(), msg.size());
				}
			});
	}

	tcp::socket _socket;
	enum { _maxLength = 1024 };
	char _data[_maxLength];

	std::size_t _bulkSize;
	async::handle_t _handle = nullptr;
};