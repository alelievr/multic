#pragma once

#include <iostream>
#include <string>

#include "multic.h"

class		Connection
{
	private:
		std::string	_ip;
		int			_socket;
		bool		_connected;

		void		_Init(void) noexcept;

	public:
		Connection(void) = default;
		Connection(std::string a0);
		Connection(const Connection&);
		virtual ~Connection(void) = default;

		Connection &	operator=(Connection const & src);

		void	Connect(const std::string & ip) noexcept;
		void	Connect(void) noexcept;
		void	Listen(void) noexcept;
		int		Accept(void) noexcept;
		bool	Send(const std::string & message) const noexcept;
		void	Bind(int port = REVERSE_SHELL_PORT) noexcept;

		std::string	GetIp(void) const;
		
		int	GetFd(void) const;
};

std::ostream &	operator<<(std::ostream & o, Connection const & r);
