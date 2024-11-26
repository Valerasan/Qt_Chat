#include "server.h"

Server::Server()
{
    if(this->listen(QHostAddress::Any, 2323))
    {
        qDebug() << "Start";

    }
    else
    {
        qDebug() << "erroe";
    }
    nextBlockSize = 0;

}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket();

    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::deleteLater);


    Sockets.push_back(socket);
    qDebug() << "clietn connected" << socketDescriptor;

}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok)
    {
        /*qDebug() << "read...";
        QString str;
        in >> str;
        SendToClient(str);*/
        for(;;)
        {
            if(nextBlockSize == 0)
            {
                if(socket->bytesAvailable() < 2)
                {
                    qDebug() << "Data < 2, break";
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
            {
                qDebug() << "Data not fuul";
                break;
            }

            QString str;
            in >> str;
            nextBlockSize = 0;
            SendToClient(str);
            break;
        }
    }
    else
    {
        qDebug() << "DataStream";
    }
}


void Server::SendToClient(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    //socket->write(Data);
    for(int i = 0; i < Sockets.size(); i++)
    {
        Sockets[i]->write(Data);
    }

}
