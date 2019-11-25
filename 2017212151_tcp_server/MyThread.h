#pragma once

#include <QThread>
#include <Winsock2.h>
#include <stdio.h>
#include <tchar.h>
#include <QDebug>
#include <QString>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

class MyThread : public QThread
{
	Q_OBJECT

public:
	MyThread(QObject *parent = 0);

	~MyThread();

	void stop();

	QString time,IPAddress,content,localIP;
	char* MyThread::RC4(char* C, char* key);
	void init();

protected:
	void run();
private:

	WORD wVersionRequested;
	WSADATA wsaData;
	char PCname[100] = { "" };
	char *IPaddress = NULL;
	struct hostent FAR * lpHostEnt = NULL;
	LPSTR lpAddr = NULL;
	SOCKET servsock, clisock;
	sockaddr_in tcpaddr, cliaddr;
	char buff[256];
	char buffer[1024] = "\0";
	char key[256] = "password";


	volatile bool stopped;
signals:
	void sendTime(QString);
signals:
	void sendIPAddress(QString);
signals:
	void sendContent(QString);
	void sendLocalIP(QString);




};
