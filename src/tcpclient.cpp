/**********************************************************************/
/*                                                                    */
/*            This file is part of the WdsPick software               */
/*                                                                    */
/*  Copyright (c) 2018-present, Jocelyn SEROT.  All rights reserved.  */
/*                                                                    */
/*  This source code is licensed under the license found in the       */
/*  LICENSE file in the root directory of this source tree.           */
/*                                                                    */
/**********************************************************************/

#include "tcpclient.h"

TcpClient::TcpClient(QHostAddress host_, int port_)
{
  tcpSocket = new QTcpSocket();
  QObject::connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
  QObject::connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
  QObject::connect(tcpSocket, SIGNAL(connected()), this, SLOT(set_connected()));
  QObject::connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(set_disconnected()));
  host = host_;
  port = port_;
  is_connected = false;
  qDebug() << "TcpClient: created socket " << host.toString() << ":" << port;
}

void TcpClient::connect()
{
  if ( ! isConnected() ) {
    qDebug() << "TcpClient: trying to connect to" << host.toString() << ":" << port;
    tcpSocket->connectToHost(host, port);
    }
  else
    qDebug() << "TcpClient: already connected to" << host.toString() << ":" << port;
}

void TcpClient::disconnect()
{
  qDebug() << "TcpClient: disconnect";
  if ( isConnected() ) {
    sendData("quit");
    //tcpSocket->close();
    tcpSocket->disconnectFromHost();
    qDebug() << "TcpClient: disconnect. done";
    }
}

void TcpClient::readData()
{
    QTextStream in(tcpSocket);

    if ( tcpSocket->canReadLine() )  {
      QString s = tcpSocket->readLine();
      emit receivedData(s);
    }
}

void TcpClient::clearInput()
{
    QTextStream in(tcpSocket);

    while ( tcpSocket->canReadLine() )  {
      QString s = tcpSocket->readLine();
    }
}

void TcpClient::sendData(const QString &msg)
{
  QTextStream out(tcpSocket);
  QString data = msg + "\r\n";

  out << data << endl;
}

void TcpClient::set_connected()
{
  qDebug() << "TcpClient: set_connected() !";
  is_connected = true;
  emit tcp_connected();
}

void TcpClient::set_disconnected()
{
  qDebug() << "TcpClient: set_disconnected";
  is_connected = false;
  emit tcp_disconnected();
}


void TcpClient::error(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
      emit errorMsg("Host not found");
      break;
    case QAbstractSocket::ConnectionRefusedError:
      emit errorMsg("Connection refused by peer");
      break;
    default:
      emit errorMsg("Error: " + tcpSocket->errorString());
      break;
    }
}

void TcpClient::quitting()
{
  qDebug() << "TcpClient: quitting";
  if ( isConnected() ) {
    disconnect();
  }
}
