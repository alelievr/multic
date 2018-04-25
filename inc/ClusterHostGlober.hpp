#pragma once

#include <iostream>
#include <string>
#include <list>

#pragma clang diagnostic ignored "-Wc++98-compat"

class		ClusterHostGlober
{
	private:
		std::list< std::string >	_ips;

		std::list< std::string >	_GlobSingle(const std::string & s) noexcept;
		bool						_ContainsGlobSymobol(const std::string & s) noexcept;

		std::list< std::string >	_Glob(const std::string & host) noexcept;
		std::string					_Resolve(const std::string & host) noexcept;

	public:
		ClusterHostGlober(void) = default;
		ClusterHostGlober(const ClusterHostGlober&) = delete;

		virtual ~ClusterHostGlober(void);

		void	LoadHosts(const std::string & hosts) noexcept;

		ClusterHostGlober &	operator=(ClusterHostGlober const & src) = delete;

		std::list< std::string >	GetIps(void) const;
};

std::ostream &	operator<<(std::ostream & o, ClusterHostGlober const & r);
