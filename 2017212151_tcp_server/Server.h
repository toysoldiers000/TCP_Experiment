#pragma once

#include <QtWidgets/QWidget>
#include "ui_Server.h"
#include "MyThread.h"
class Server : public QWidget
{
	Q_OBJECT

public:
	Server(QWidget *parent = Q_NULLPTR);

public slots:
	void startButton();
	int stopButton();
	void setTime(QString time);
	void setIPAddress(QString IPAddress);
	void setContent(QString content);
	void setLocalIP(QString localIP);
	void quitButton();
 private:
	Ui::ServerClass ui;
	MyThread thread01;
	int row;
	QTableWidget *tableWidget=NULL;
};
