#include "Connection.hpp"
#include "multic.h"

Connection::Connection(std::string a0) : _ip(a0)
{
	std::cout << "Default constructor of Connection called" << std::endl;
	this->_fd = 0;
}

Connection::Connection(Connection const & src)
{
	*this = src;
	std::cout << "Copy constructor of Connection called" << std::endl;
}

Connection::~Connection(void)
{
	std::cout << "Destructor of Connection called" << std::endl;
}

void		Connection::Connect(const std::string & ip) noexcept
{
	
}

void		Connection::Connect(void) noexcept
{
	
}

void		Connection::Listen(void) noexcept
{
	
}


Connection &	Connection::operator=(Connection const & src)
{
	std::cout << "Assignment operator of Connection called" << std::endl;

	if (this != &src) {
		this->_ip = src._ip;
		this->_fd = src._fd;
	}
	return (*this);
}

std::string		Connection::GetIp(void) const { return (this->_ip); }

int				Connection::GetFd(void) const { return (this->_fd); }

std::ostream &	operator<<(std::ostream & o, Connection const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
