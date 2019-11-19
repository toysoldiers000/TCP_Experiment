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


protected:
	void run();
private:
	volatile bool stopped;
signals:
	void sendTime(QString);
signals:
	void sendIPAddress(QString);
signals:
	void sendContent(QString);
	void sendLocalIP(QString);




};
