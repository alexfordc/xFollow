#include "TcpServer.h"


CTcpServer::CTcpServer()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		//return -1;
		// @note ������ʼ���϶��ɹ�
	}
}

CTcpServer::~CTcpServer()
{
	WSACleanup();
}

bool CTcpServer::init()
{
	int retVal = 0;

	m_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_server)
	{
		return false;
	}
	//����SocketΪ������ģʽ
	u_long mode = 1;
	retVal = ioctlsocket(m_server, FIONBIO, (u_long FAR*)&mode);
	if (retVal == SOCKET_ERROR)
	{
		return false;
	}
	//���÷�����Socket��ַ
	sockaddr_in addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(9990);
	addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	retVal = bind(m_server, (const struct sockaddr*)&addrServ, sizeof(sockaddr_in));
	if (retVal == SOCKET_ERROR)
	{
		closesocket(m_server);
		return false;
	}

	return true;
}

bool CTcpServer::start()
{
	int retVal = 0;
	//����
	retVal = listen(m_server, 2);
	if (retVal == SOCKET_ERROR)
	{
		closesocket(m_server);
		return false;
	}

	m_thread = new std::thread(std::bind(&CTcpServer::run, this));
	return m_thread != nullptr;
}

bool CTcpServer::stop()
{

}

bool CTcpServer::close()
{
}

//////////////////////////////////////////////////////////////////////////
int CTcpServer::run()
{
	//���ܿͻ�����
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	//ѭ���ȴ�
	while (true)
	{
		sClient = accept(m_server,(sockaddr FAR*)&addrClient,&addrClientlen);
		if (INVALID_SOCKET == sClient)
		{
			int err = WSAGetLastError();
			if (err == WSAEWOULDBLOCK)
			{
				Sleep(100);
				continue;
			}
			else
			{
				closesocket(m_server);
				return -1;
			}		
		}
		break;
	}
	return 0;
}
