#include "MyThread.h"
#include <QDebug>
#include <QDateTime>
#include <QByteArray>

//WORD wVersionRequested;
//WSADATA wsaData;
//char PCname[100] = { "" };
//char *IPaddress = NULL;
//struct hostent FAR * lpHostEnt = NULL;
//LPSTR lpAddr = NULL;
//SOCKET servsock, clisock;
//sockaddr_in tcpaddr, cliaddr;
//char buff[256];
//char buffer[1024] = "\0";



MyThread::MyThread(QObject *parent)
	: QThread(parent)
{
	stopped = false;

}

MyThread::~MyThread()
{

}

void MyThread::run()
{

	servsock = 0;
	wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0)//初始化 ws2_32.dll 动态库
	{
		qDebug()<<"WSAStartup() failed!\n";//Winsock 初始化错误
		return ;
	}
	if (wsaData.wVersion != wVersionRequested)
	{
		qDebug() << "The version of Winsock is not suited!\n";//Winsock 版本不匹配
		WSACleanup();//结束对 ws2_32.dll 的调用
		return ;
	}
	//说明 ws2_32.dll 正确加载
	qDebug() << "Load ws2_32.dll successfully!\n";
	//获取本机 IP 地址

	gethostname(PCname, sizeof(PCname));
	qDebug() << "Local Hostname is %s.\n", PCname;
	lpHostEnt = gethostbyname(PCname);
	if (lpHostEnt == NULL)
	{
		//产生错误
		qDebug() << "gethostbyname failed!\n";
		return ;
	}
	//获取 IP
	lpAddr = lpHostEnt->h_addr_list[0];
	if (lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr, lpAddr, 4);
		//转换为标准格式
		IPaddress = inet_ntoa(inAddr);//将一个 32 位数字表示的 IP 地址转换成点分十进制IP 地址字符串
		if (sizeof(IPaddress) == 0)
			qDebug() << "get host IP failed!\n";
		else
		{
			printf("Local HostIP is %s.\n", IPaddress);
			localIP = QString::fromLocal8Bit(IPaddress);
			emit sendLocalIP(localIP);
		}
			
	}

	//创建套接字
	
	qDebug() << "Create Socket...\n";
	/**************************************************************/
	servsock = socket(AF_INET, SOCK_STREAM, 0);
	/**************************************************************/
	int servport = 5555;
	int iSockErr = 0;
	//定义服务器地址结构

	int clilen = 0;
	memset(&tcpaddr, 0, sizeof(tcpaddr));
	tcpaddr.sin_family = AF_INET;
	tcpaddr.sin_port = htons(servport);
	//将一个点分十进制 IP 地址字符串转换成 32 位数字表示的 IP 地址
	tcpaddr.sin_addr.s_addr = inet_addr(IPaddress);//"127.0.0.1"//INADDR_ANY
	//绑定套接字到服务器地址结构
	qDebug() << "Binding...\n";
	/**************************************************************/
	
		iSockErr = bind(servsock, (sockaddr*)&tcpaddr, sizeof(tcpaddr));
	
	/**************************************************************/
		if (iSockErr == SOCKET_ERROR)
		{
			qDebug() << "Binding failed:%d\n", WSAGetLastError();//根据不同的错误类型进行不同的处理
			this->stop();
		}
	
	//函数调用成功，进行其他处理
	//监听套接字
	qDebug() << "Listening...\n";

	iSockErr = listen(servsock, 5);
	if (iSockErr != 0)
	{
		qDebug() << "Listen failed:%d\n", WSAGetLastError();
		return ;
	}
	int len = 0;
	//等待连接请求
	qDebug() << "Waitting TCP Request...\n";
	len = sizeof(cliaddr);
	/**************************************************************/
	if (stopped == false)
	{
		clisock = accept(servsock, (sockaddr*)&cliaddr, &len);
	}

	/**************************************************************/
	qDebug() << "Accept TCP Client:%s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port);
	sprintf_s(buff, "Welcome you %s", inet_ntoa(cliaddr.sin_addr));
	//发送欢迎词
	/**************************************************************/
	send(clisock, buff, sizeof(buff), 0);
	/**************************************************************/


	while (!stopped)
	{
		
		memset(buffer, 0, sizeof(buffer));
		qDebug() << "waiting for message from others-------------\n";
		if (recv(clisock, buffer, sizeof(buffer), 0) != SOCKET_ERROR)
		{
			qDebug() << "Received datagram from TCP Client \n"<<inet_ntoa(cliaddr.sin_addr)<<buffer;

			QDateTime current_date_time = QDateTime::currentDateTime();
			time = current_date_time.toString("yyyy-MM-dd   hh:mm:ss");
			//发射信号time
			emit sendTime(time);

			IPAddress = QString::fromLocal8Bit(inet_ntoa(cliaddr.sin_addr));
			emit sendIPAddress(IPAddress);
			this->RC4(buffer, key);
			content = QString::fromLocal8Bit(buffer);
			emit sendContent(content);


			////给 cilent 发信息
			send(clisock, buffer, sizeof(buffer), 0);
		}
		else
			break;
		Sleep(500);

		
		
	}
	stopped = false;

	qDebug() << "Service is shutdown,you are able to open service again";
}

void MyThread::stop()
{
	shutdown(clisock, 2);
	shutdown(servsock, 2);
	closesocket(clisock);
	closesocket(servsock);
	WSACleanup();
	stopped = true;
	
	//return;
}

void MyThread::init()
{
	stopped = false;

}

char* MyThread::RC4(char* C, char* key)
{
	int S[256];
	int T[256];

	int  count = 0;
	count = strlen(key);

	for (int i = 0; i < 256; i++)
	{
		S[i] = i;
		int tmp = i % count;
		T[i] = key[tmp];
	}

	int j = 0;

	for (int i = 0; i < 256; i++)
	{
		j = (j + S[i] + T[i]) % 256;
		int tmp;
		tmp = S[j];
		S[j] = S[i];
		S[i] = tmp;
	}

	int length = 0;
	length = strlen(C);

	int i;
	i = 0, j = 0;

	for (int p = 0; p < length; p++)
	{

		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		int tmp;
		tmp = S[j];
		S[j] = S[i];
		S[i] = tmp;

		int k = S[(S[i] + S[j]) % 256];
		C[p] = C[p] ^ k;
	}

	return C;
}