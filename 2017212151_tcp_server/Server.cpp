#include "Server.h"
#include <QTableWidget>
#include <Winsock2.h>
#include <stdio.h>
#include <tchar.h>
#include <QDebug>
#include <QPushButton>
#include <QTableWidgetItem>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
Server::Server(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	row = 0;
	
	tableWidget = ui.tableWidget;
	connect(ui.startButton,&QPushButton::clicked,this,&Server::startButton);
	connect(ui.stopButton, &QPushButton::clicked, this, &Server::stopButton);
	connect(&thread01, &MyThread::sendTime, this, &Server::setTime);
	connect(&thread01, &MyThread::sendIPAddress, this, &Server::setIPAddress);
	connect(&thread01, &MyThread::sendContent, this, &Server::setContent);
	connect(&thread01, &MyThread::sendLocalIP, this, &Server::setLocalIP);
	connect(ui.quitButton, &QPushButton::clicked, this, &Server::quitButton);
	qDebug() << "Waiting Start Button clicked!";
	
}

void Server::startButton()
{

	thread01.start();
	thread01.init();
	ui.startButton->setEnabled(false);
	ui.stopButton->setEnabled(true);

}

int Server::stopButton()
{
	qDebug() << "Service is closing,please open service after a second";
	thread01.stop();
	ui.startButton->setEnabled(true);
	ui.stopButton->setEnabled(false);
	thread01.quit();
	return 0;
}


void Server::setTime(QString time)
{
	tableWidget->setItem(row, 0, new QTableWidgetItem(time));

}

void Server::setIPAddress(QString IPAddress)
{
	tableWidget->setItem(row, 1, new QTableWidgetItem(IPAddress));

}

void Server::setContent(QString content)
{
	tableWidget->setItem(row, 2, new QTableWidgetItem(content));
	row ++;
}

void Server::setLocalIP(QString localIP)
{
	ui.lineEdit->setText(localIP);
}
void Server::quitButton()
{
	close();   
}

