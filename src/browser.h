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

#ifndef _wdsbrowser_h
#define _wdsbrowser_h

#include <QWidget>

#include <QVector>
#include <QMap>
#include <QDateTime>
#include "wdsentry.h"
#include "tcpclient.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QTabWidget;
class QGroupBox;
class QFile;
class QGridLayout;
class WdsPickConfig;
class WdsPickContext;

class WdsBrowser : public QWidget
{
Q_OBJECT
public:
  WdsBrowser(WdsPickConfig *config, WdsPickContext *context, QWidget *provider, QTabWidget *tab, QWidget *parent = 0);
  bool sthToSave();
  void saveLog();

private slots:
  void update_selection(QVector<WdsEntry> &new_selection);
  void nextSel();
  void prevSel();
  void gotoSel();
  void set_connected();
  void set_disconnected();
  void displayMsg(const QString &);
  //void displayError(const QString &);
  //void sendCmd();
  void saveComment();
  void update_time();
public slots:
  void connect_cdc();
  void disconnect_cdc();
  void quit();

private:
  QTabWidget *parentTab;
  QDateTime utc_time;
  QTime sid_time;
  double obs_long, obs_lat;
  struct Field { QLabel *label; QLabel *desc; };
  QMap<QString,Field> fields;
  TcpClient *cdcClient;
  void add_field(QMap<QString,Field> &fields, QString lbl, int width, QGridLayout *layout, int ncols, bool first_in_layout = false);
  void display_mesures(QString id);
  QGroupBox* createSelectionGroup();
  QGroupBox* createTimeGroup();
  QGroupBox* createAdcGroup();
  QGroupBox* createCdcGroup();
  QGroupBox* createLogGroup();
  QTextEdit *mesInfo;
  QPushButton *nextButton;
  QPushButton *prevButton;
  QPushButton *gotoButton;
  //QPushButton *sendCmdButton;
  QPushButton *connectButton;
  QPushButton *disconnectButton;
  QLineEdit *cdcCmdTxt;
  QTextEdit *cdcLog;
  QLabel *cdcStatus;
  QLineEdit *logComment;
  QPushButton *saveCommentButton;
  QVector<WdsEntry> selection;
  int current_sel;
  void display_selected();
  WdsPickContext *context;
  WdsPickConfig *config;
  QFile *logFile;
  QTextStream logStream;
};

#endif
