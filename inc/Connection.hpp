#ifndef CONNECTION_HPP
# define CONNECTION_HPP
# include <iostream>
# include <string>

class		Connection
{
	private:
		std::string	_ip;
		int	_fd;


	public:
		Connection(void) = default;
		Connection(std::string a0);
		Connection(const Connection&);
		virtual ~Connection(void);

		Connection &	operator=(Connection const & src);

		void	Connect(const std::string & ip) noexcept;

		void	Connect(void) noexcept;

		void	Listen(void) noexcept;

		std::string	GetIp(void) const;
		
		int	GetFd(void) const;
};

std::ostream &	operator<<(std::ostream & o, Connection const & r);

#endif
