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

#ifndef _tcp_client_h
#define _tcp_client_h

#include <QTcpSocket>
#include <QHostAddress>

class TcpClient : public QObject
{
    Q_OBJECT
public:
  TcpClient(QHostAddress host, int port);
  
  bool isConnected() { return is_connected; }
  QString readLine();
  void sendData(const QString &msg);

signals:
  void receivedData(const QString &msg);
  void errorMsg(const QString &msg);
  void tcp_connected();
  void tcp_disconnected();

private slots:
    void readData();
    void error(QAbstractSocket::SocketError);
    void set_connected();
    void set_disconnected();
public slots:
    void connect();
    void disconnect();
    void quitting();

private:
    void clearInput();
    QTcpSocket *tcpSocket;
    bool is_connected;
    QHostAddress host;
    int port;
};

#endif
