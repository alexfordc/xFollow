#ifndef TcpServer_H
#define TcpServer_H

#include <winsock2.h>
#include <thread>

class CTcpServer
{
public:
	CTcpServer();
	~CTcpServer();

public:
	bool init();
	bool start();
	bool stop();
	bool close();

private:
	SOCKET           m_server;
	SOCKET           sClient;

	int  run();
	std::thread*     m_thread;
};

#endif // TcpServer_H
