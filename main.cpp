//STL
#include <iostream>
#include <algorithm>

#include <libasync.h>
#include <Server.h>

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
		port = (unsigned short)strtoul(argv[1], NULL, 0);
		bulkSize = atoi(argv[2]);
	}
	else
	{
		std::cout << "Something is wrong" << std::endl;
		exit(1);
	}

	
	Server server(port, bulkSize);
	server.run();

	return 0;
}
