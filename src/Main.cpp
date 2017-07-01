#include "Main.hpp"
#include "TargetResolver.hpp"
#include <unistd.h>

Main::Main(int ac, char **av) : _flags(0), _programName(av[0])
{
	char		ip[0xF0];
	bool		error;

	_options(&ac, &av);

	av--;
	while (*++av)
	{
		while (TargetResolver::Resolve(*av, ip, error))
		{
			if (error)
				continue ;
			_targetIps.push_back(ip);
		}
	}

	std::cout << "ip targets:" << std::endl;
	for (const std::string & cip : _targetIps)
		std::cout << cip << std::endl; 
}

Main::~Main(void) {}

void		Main::_options(int *argc, char ***argv)
{
	int		ch;

	if (*argc <= 1)
		_usage();
	while ((ch = getopt(*argc, *argv, "f:x")) != -1) {
		switch (ch) {
			case 'x':
				_flags |= ShowExpanded;
				break ;
			case '?':
			default:
				_usage();
		}
	}
	*argc -= optind;
	*argv += optind;
}

void		Main::_usage(void)
{
	std::cout << "usage: " << _programName << " hostname1 ip2 10.11.[1-9].[1.8] ..." << std::endl;
	exit(-1);
}

int			Main::Run(void)
{
	return (0);
}

int		main(int ac, char **av)
{
	return (Main(ac, av).Run());
}
