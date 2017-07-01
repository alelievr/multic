#ifndef TARGETRESOLVER_HPP
# define TARGETRESOLVER_HPP
# include <iostream>
# include <string>

# pragma clang diagnostic ignored "-Wc++98-compat-pedantic"

class		TargetResolver
{
	private:

		struct	GlobMatch
		{
			int     min;
			int     max;
			int     value;
		};

		static char *		_GlobTarget(const char *target);

	public:
		TargetResolver(void) = delete;
		TargetResolver(const TargetResolver &) = delete;
		virtual ~TargetResolver(void) = delete;

		TargetResolver &	operator=(TargetResolver const & src) = delete;

		static bool			Resolve(const char *target, char *ipbuff, bool & error);
};

#endif
