#pragma once

#include <iostream>
#include <string>
#include <list>

#include <sys/select.h>

#include "Connection.hpp"

class		ClusterShell
{
	private:
		std::list< Connection >	clientConnections;

		void	_InitializeConnections(const std::list< std::string > & ips, fd_set * active_fds) noexcept;
		void	_ReadFromShell(void) noexcept;

	public:
		ClusterShell(void) = default;
		ClusterShell(const std::list< std::string > & ips);
		ClusterShell(const ClusterShell&) = delete;
		virtual ~ClusterShell(void) = default;

		ClusterShell &	operator=(ClusterShell const & src) = delete;

		void	Connect(const std::list< std::string > & ips) noexcept;
};

std::ostream &	operator<<(std::ostream & o, ClusterShell const & r);
