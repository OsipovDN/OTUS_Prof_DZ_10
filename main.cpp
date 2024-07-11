//STL
#include <iostream>
#include <algorithm>

//Boost
#include <boost/asio.hpp>

#include "Server.h"

namespace asio = boost::asio;

bool isDig(char* arg)
{
	std::string num = arg;
	auto isdig = [](char ch) {return std::isdigit(ch); };
	return std::all_of(num.cbegin(), num.cend(), isdig);
}

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3 || !isDig(argv[1]) || !isDig(argv[2]))
		{
			std::cout << "Something is wrong" << std::endl;
			return -1;
		}
		auto port = static_cast<unsigned short>(atoi(argv[1]));
		std::size_t bulkSize = static_cast<std::size_t>(atoi(argv[2]));

		asio::io_context context;
		Server server(context, port, bulkSize);
		context.run();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception: " << ex.what() << std::endl;
	}

	return 0;
}

