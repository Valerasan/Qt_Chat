#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QString>


class Server : public QTcpServer
{

    Q_OBJECT

public:
    Server();
    QTcpSocket* socket;

private:
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    void SendToClient(QString str);

public slots:
    void incomingConnection(qintptr socketDescription);
    void slotReadyRead();
};

#endif // SERVER_H
