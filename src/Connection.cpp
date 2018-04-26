#include "Connection.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_CONNECTION_QUEUE	42

Connection::Connection(std::string a0) : _ip(a0), _connected(false)
{
	this->_socket = 0;

	_Init();
}

Connection::Connection(Connection const & src)
{
	*this = src;

	_Init();
}

void		Connection::_Init(void) noexcept
{
	struct protoent *	proto;
	int					activate = 1;

	proto = getprotobyname("tcp");
	if ((_socket = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0)
		perror("_socket"), exit(-1);

	endprotoent();

	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &activate, sizeof(int)) < 0)
		perror("setsockopt"), exit(-1);

	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, &activate, sizeof(int)) < 0)
		perror("setsockopt"), exit(-1);
}

void		Connection::Connect(const std::string & ip) noexcept
{
	struct sockaddr_in	sin;

	sin.sin_family = AF_INET;
	sin.sin_port = htons(static_cast< uint16_t >(REVERSE_SHELL_PORT));

    if (inet_pton(AF_INET, ip.c_str(), &sin.sin_addr) <= 0) 
    {
		std::cout << "Invalid address/ Address not supported" << std::endl;
        exit(-1);
    }
	
	if (connect(_socket, reinterpret_cast< const struct sockaddr * >(&sin), sizeof(sin)) == -1)
		perror("connect"), exit(-1);

	_connected = true;
}

void		Connection::Bind(int port) noexcept
{
	struct sockaddr_in	sin;

	sin.sin_family = AF_INET;
	sin.sin_port = htons(static_cast< uint16_t >(port));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(_socket, reinterpret_cast< const struct sockaddr * >(&sin), sizeof(sin)) == -1)
		perror("bind"), exit(-1);
}

void		Connection::Connect(void) noexcept
{
	if (_ip.empty())
		Connect("127.0.0.1");
	else
		Connect(_ip);
}

void		Connection::Listen(void) noexcept
{
	if (listen(_socket, MAX_CONNECTION_QUEUE) < 0)
		perror("listen"), exit(-1);
}

int			Connection::Accept(void) noexcept
{
	int					connectionSocket;
	struct sockaddr_in	clientAddr;
	socklen_t			size = sizeof(clientAddr);

	if ((connectionSocket = accept(_socket, reinterpret_cast< struct sockaddr * >(&clientAddr), &size)) < 0)
	{
		perror("accept");
		return -1;
	}

	return connectionSocket;
}

bool			Connection::Send(const std::string & message) const noexcept
{
	if (!_connected)
		return false;

	return write(_socket, message.c_str(), message.length()) != -1;
}

Connection &	Connection::operator=(Connection const & src)
{
	if (this != &src) {
		this->_ip = src._ip;
		this->_socket = src._socket;
		this->_connected = src._connected;
	}
	return (*this);
}

std::string		Connection::GetIp(void) const { return (this->_ip); }

int				Connection::GetFd(void) const { return (this->_socket); }

std::ostream &	operator<<(std::ostream & o, Connection const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
