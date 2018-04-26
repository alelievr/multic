#include "ClusterShell.hpp"

#include <unistd.h>
#include <errno.h>

ClusterShell::ClusterShell(const std::list< std::string > & ips)
{
	Connect(ips);
}

void		ClusterShell::_InitializeConnections(const std::list< std::string > & ips, fd_set * active_fds) noexcept
{
	FD_ZERO(active_fds);

	FD_SET(STDIN_FILENO, active_fds);

	for (const std::string & ip : ips)
	{
		Connection c = Connection(ip);

		c.Connect();

		clientConnections.push_back(c);

		if (c.GetFd() == -1)
			std::cout << "An error occured while connecting to " << ip << " with code " << strerror(errno) << std::endl;
	}
}

void		ClusterShell::Connect(const std::list< std::string > & ips) noexcept
{
	fd_set					read_fds;
	fd_set					active_fds;

	_InitializeConnections(ips, &active_fds);

	while (42)
	{
		read_fds = active_fds;
		if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) < 0)
			std::cout << std::string("select: ") + strerror(errno) << std::endl, exit(-1);

		for (int i = 0; i < FD_SETSIZE; i++)
			if (FD_ISSET(i, &read_fds))
			{
				if (i == STDIN_FILENO)
					_ReadFromShell();
			}
	}
}

void		ClusterShell::_ReadFromShell(void) noexcept
{
	char	cmd[0xF00];

	long r = read(0, cmd, sizeof(cmd));

	if (r == -1)
		std::cout << "An error occured with the terminal" << std::endl, exit(-1);

	cmd[r] = 0;

	std::cout << "Sending command [" << cmd << "]" << std::endl;

	for (const Connection & c : clientConnections)
		c.Send(cmd);
}

std::ostream &	operator<<(std::ostream & o, ClusterShell const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
