#include <iostream>
#include <string>
#include <vector>

#include <trctrl/Socket.h>

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Usage: trmod <control_unix_socket> {commands}\n";
		return 1;
	}

	int argi = 1;

	char* control_unix_socket = argv[argi++];

	Socket sock;
	if (sock.bind(control_unix_socket) < 0)
	{
		std::cerr << "Could not connect to control_unix_socket!\n";
		return 2;
	}

	std::vector<char> buffer(1000);

	while (argi < argc)
	{
		std::string cmd = argv[argi++];

		if (cmd == "!" || cmd == "?")
		{
			if (sock.readmsg(buffer) < 0)
			{
				break;
			}

			// TODO: insert time
			int time = 0;

			if (cmd == "!")
			{
				std::cout << time << ": " << std::string(buffer.begin(), buffer.end()) << "\n";
			}
		}
		else
		{
			buffer.clear();
			buffer.insert(buffer.begin(), cmd.begin(), cmd.end());

			if (sock.writemsg(buffer) < 0)
			{
				break;
			}
		}
	}

	std::string stop = "stop";
	buffer.clear();
	buffer.insert(buffer.begin(), stop.begin(), stop.end());

	sock.writemsg(buffer);

	sock.close();
}
