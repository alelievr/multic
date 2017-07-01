#ifndef MAIN_HPP
# define MAIN_HPP
# include <iostream>
# include <string>
# include <vector>

# pragma clang diagnostic ignored "-Wc++98-compat-pedantic"

class		Main
{
	private:

		enum Options
		{
			ShowExpanded	= 0x001,
		};

		int							_flags;
		int							:32;
		std::string					_programName;
		std::vector< std::string >	_targetIps;

		void	_options(int *argc, char ***argv);
		void	_usage(void) __attribute__((noreturn));

	public:
		Main(void) = delete;
		Main(int ac, char **av);
		Main(const Main &) = delete;
		virtual ~Main(void);

		Main &	operator=(Main const & src) = delete;

		int			Run(void);
};

#endif
