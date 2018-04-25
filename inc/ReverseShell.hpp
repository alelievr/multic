#ifndef REVERSESHELL_HPP
# define REVERSESHELL_HPP
# include <iostream>
# include <string>

class		ReverseShell
{
	private:
		
		void	_RunShell(int serverSocket) noexcept;

	public:
		ReverseShell(void);
		ReverseShell(const ReverseShell &) = delete;
		virtual ~ReverseShell(void);

		ReverseShell &	operator=(ReverseShell const & src) = delete;
};

std::ostream &	operator<<(std::ostream & o, ReverseShell const & r);

#endif
