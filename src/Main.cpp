#include "Main.hpp"
#include "ClusterHostGlober.hpp"
#include "ReverseShell.hpp"
#include "ClusterShell.hpp"
#include <unistd.h>

Main::Main(int ac, char **av) : _flags(0), _programName(av[0])
{
	ClusterHostGlober		clusterIps;

	_Options(&ac, &av);

	///Local mode
	if (ac == 0)
	{
		ReverseShell();
	}
	else
	{
		av--;
		while (*++av)
			clusterIps.LoadHosts(*av);

		std::cout << "ip targets:" << std::endl;
		for (const std::string & cip : clusterIps.GetIps())
			std::cout << cip << std::endl; 
	
		ClusterShell().Connect(clusterIps.GetIps());
	}
}

Main::~Main(void) {}

void		Main::_Options(int *argc, char ***argv)
{
	int		ch;

	while ((ch = getopt(*argc, *argv, "h")) != -1) {
		switch (ch) {
			case 'x':
				_flags |= ShowExpanded;
				break ;
			case '?':
			case 'h':
			default:
				_Usage();
		}
	}
	*argc -= optind;
	*argv += optind;
}

void		Main::_Usage(void)
{
	std::cout << "reverse shell:" << std::endl;
	std::cout << "\t$ " << _programName << " '10.11.[1-9].[1-8]'" << std::endl;
	std::cout << "\t$ " << _programName << " 'e1r10p[1-23]'" << std::endl;
	std::cout << "client" << std::endl;
	std::cout << "\t$ " << _programName << std::endl;
	exit(-1);
}

int		main(int ac, char **av)
{
	Main(ac, av);

	return (0);
}
