#include "ReverseShell.hpp"
#include "Connection.hpp"
#include <unistd.h>
#include <signal.h>
#include <util.h>
#include <list>

ReverseShell::ReverseShell(void)
{
	Connection			c("127.0.0.1");
	std::list< std::pair< pid_t, int > >	clientPids;

	c.Bind();

	c.Listen();

	while (42)
	{
		int serverSocket = c.Accept();
		std::cout << "Accept !" << std::endl;
		pid_t	pid;

		if ((pid = fork()) == 0)
		{
			_RunShell(serverSocket);
			exit(0);
		}

		clientPids.push_back(std::pair< pid_t, int >(pid, serverSocket));

		//TODO: check if there are dead processes in the pid list and remove them + close their socket
	}
}

ReverseShell::~ReverseShell(void)
{
	std::cout << "Destructor of ReverseShell called" << std::endl;
}

void		ReverseShell::_RunShell(int serverSocket) noexcept
{
	char	cmd[0xF00];
	pid_t	shellPid;
	int		shellFd;
	int		status;

	printf("server: %i\n", serverSocket);

	if ((shellPid = forkpty(&shellFd, NULL, NULL, NULL)) == 0)
	{
		setsid();
		exit(execl("/bin/zsh", "zsh", NULL));
	}

	while (42)
	{
		int r = read(serverSocket, cmd, sizeof(cmd));

		if (r <= 0)
		{
			std::cout << "read return: " << r << std::endl;

			if (r != 0)
				perror("read");

			close(serverSocket);

			kill(shellPid, SIGKILL);
			waitpid(shellPid, &status, 0);
			exit(-1);
		}

		cmd[r] = 0;

		write(shellFd, cmd, r);
	}
}

std::ostream &	operator<<(std::ostream & o, ReverseShell const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
