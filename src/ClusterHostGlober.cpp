#include "ClusterHostGlober.hpp"

#include <regex>

#include <netdb.h>
#include <arpa/inet.h>

ClusterHostGlober::~ClusterHostGlober(void) {}

const std::regex	braketGlobRegex = std::regex("\\[(\\d+)-(\\d+)\\]");
const std::regex	starGlobRegex = std::regex("(\\*)");
const std::regex	ipRegex = std::regex("\\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");

std::list< std::string >    ClusterHostGlober::_GlobSingle(const std::string & s) noexcept
{
	std::list< std::string >	result;
	std::smatch					matches;

	if (std::regex_search(s, matches, braketGlobRegex))
	{
		int from = std::atoi(matches[1].str().c_str());
		int to = std::atoi(matches[2].str().c_str());

		if (from > to)
			std::cout << "Glob exception, a > b !" << std::endl, exit(-1);

		for (int i = from; i <= to; i++)
			result.push_back(std::regex_replace(s, braketGlobRegex, std::to_string(i), std::regex_constants::format_first_only));
	}
	else if (std::regex_search(s, matches, starGlobRegex))
	{
		for (int i = 1; i <= 24; i++)
			result.push_back(std::regex_replace(s, starGlobRegex, std::to_string(i), std::regex_constants::format_first_only));
	}
	else
		result.push_back(s);

	return result;
}

bool                        ClusterHostGlober::_ContainsGlobSymobol(const std::string & s) noexcept
{
	return std::regex_search(s, braketGlobRegex) || std::regex_search(s, starGlobRegex);
}

std::list< std::string >	ClusterHostGlober::_Glob(const std::string & host) noexcept
{
	std::list< std::string > hostsA = _GlobSingle(host);

	while (_ContainsGlobSymobol(hostsA.front()))
	{
		std::list< std::string > hostsB;

		for (const std::string & h : hostsA)
		{
			auto globs = _GlobSingle(h);
			std::cout << "h " << h << " | " << globs.front() << std::endl;
			hostsB.splice(hostsB.end(), globs);
		}

		hostsA = hostsB;
	}

	return hostsA;
}

void						ClusterHostGlober::LoadHosts(const std::string & host) noexcept
{
	for (const std::string & h : _Glob(host))
	{
		const std::string ip = _Resolve(h);

		if (std::regex_match(ip, ipRegex))
			_ips.push_back(ip);
	}
}

std::string					ClusterHostGlober::_Resolve(const std::string & host) noexcept
{
	hostent *	hostname = gethostbyname(host.c_str());

	if (hostname)
	    return std::string(inet_ntoa(**(in_addr**)hostname->h_addr_list));
	else
		return {};
}


std::list< std::string >	ClusterHostGlober::GetIps(void) const { return (this->_ips); }

std::ostream &	operator<<(std::ostream & o, ClusterHostGlober const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
