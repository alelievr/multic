#ifndef REVERSESHELL_HPP
# define REVERSESHELL_HPP
# include <iostream>
# include <string>

class		ReverseShell
{
	private:
		
		void	_RunShell(int serverSocket) noexcept;
		void	_ReadFromServer(int shellPid, int shellFd, int serverScoket) noexcept;
		void	_ReadFromShell(int shellFd) noexcept;

	public:
		ReverseShell(void);
		ReverseShell(const ReverseShell &) = delete;
		virtual ~ReverseShell(void) = default;

		ReverseShell &	operator=(ReverseShell const & src) = delete;
};

std::ostream &	operator<<(std::ostream & o, ReverseShell const & r);

#endif
