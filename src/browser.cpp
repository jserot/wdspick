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

#include <QtGui>
#include <QTextStream>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QVector>
#include <QMap>
#include <QDateTime>
#include <QGridLayout>
#include <QGroupBox>
#include <QTabWidget>
#include <QScrollBar>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>

#include "browser.h"
#include "wdsentry.h"
#include "config.h"
#include "context.h"
#include "astrometry.h"

typedef struct {
  QLabel *label;
  QLabel *desc;
} Field;

WdsBrowser::WdsBrowser(WdsPickConfig *config, WdsPickContext *context, QWidget *provider, QTabWidget *tab, QWidget *parent)
  : QWidget(parent), cdcClient(NULL), context(context), config(config)
{
  obs_long = config->obs_long;
  obs_lat = config->obs_lat;
  QDate date = QDate::currentDate();
  QString log_file_name = config->rsrc_path + config->log_file_prefix + date.toString("yyyy-MM-dd") + ".log";
  logFile = new QFile(log_file_name);
  if ( !logFile->open(QIODevice::Append | QIODevice::Text) )
    QMessageBox::critical(this, "WDS Picker:", tr("Cannot open log file %1!\n").arg(log_file_name));
  logStream.setDevice(logFile); 
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(createTimeGroup());
  layout->addWidget(createSelectionGroup());
  layout->addWidget(createAdcGroup());
  layout->addWidget(createCdcGroup());
  layout->addWidget(createLogGroup());
  setLayout(layout);
  current_sel = -1;
  parentTab = tab;
  nextButton->setEnabled(false);
  prevButton->setEnabled(false);
  gotoButton->setEnabled(false);
  connect(provider, SIGNAL(extracted(QVector<WdsEntry>&)), this, SLOT(update_selection(QVector<WdsEntry>&)));
  connect(nextButton, SIGNAL(clicked()), this, SLOT(nextSel()));
  connect(prevButton, SIGNAL(clicked()), this, SLOT(prevSel()));
  connect(gotoButton, SIGNAL(clicked()), this, SLOT(gotoSel()));
  //connect(cdcCmdTxt, SIGNAL(returnPressed()), this, SLOT(sendCmd()));
  //connect(sendCmdButton, SIGNAL(clicked()), this, SLOT(sendCmd()));
  connect(connectButton, SIGNAL(clicked()), this, SLOT(connect_cdc()));
  connect(disconnectButton, SIGNAL(clicked()), this, SLOT(disconnect_cdc()));
  connectButton->setEnabled(true);
  disconnectButton->setEnabled(false);
  connect(saveCommentButton, SIGNAL(clicked()), this, SLOT(saveComment()));
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update_time()));
  timer->start(10000); // every 10s
}

void WdsBrowser::add_field(
  QMap<QString,Field> &fields,
  QString lbl,
  int width,
  QGridLayout *layout,
  int ncols,
  bool first_in_layout)
{
  static int row=0, col=0;
  Field f;
  f.label = new QLabel(lbl);
  f.desc = new QLabel;
  f.desc->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  f.desc->setFixedWidth(width);
  fields.insert(lbl,f);
  row = first_in_layout ? 0 : col==ncols-1 ? row+1 : row;
  col = first_in_layout ? 0 : col==ncols-1 ? 0 : col+1;
  layout->addWidget(f.label, row, col*2);
  layout->addWidget(f.desc, row, col*2+1);
}

void WdsBrowser::update_time(void)
{
  utc_time = QDateTime::currentDateTimeUtc();
  fields["Date:"].desc->setText(utc_time.toString("dd/MM/yyyy"));
  fields["UTC:"].desc->setText(utc_time.toString("hh:mm:ss"));
  sid_time = get_sideral_time(utc_time,obs_long);
  fields["LST:"].desc->setText(sid_time.toString("hh:mm:ss"));
}

int compute_field_width(const char *templ)
{
  QFontMetrics metrics(QApplication::font());
  return metrics.width(templ);
}

QGroupBox *WdsBrowser::createTimeGroup()
{
  int large_field_width = compute_field_width("88/88/8888");
  QGroupBox *box = new QGroupBox(tr("Time"));
  QGridLayout *layout = new QGridLayout;

  add_field(fields, tr("Date:"), large_field_width, layout, 3, true);
  add_field(fields, tr("UTC:"), large_field_width, layout, 3);
  add_field(fields, tr("LST:"), large_field_width, layout, 3);

  box->setLayout(layout);
  return box;
}

QGroupBox *WdsBrowser::createSelectionGroup()
{
  int default_field_width = compute_field_width("88888");
  int large_field_width = compute_field_width("88888888888");

  QGroupBox *box = new QGroupBox(tr("Selection"));
  QGridLayout *layout = new QGridLayout;

  add_field(fields, tr("CNT:"), large_field_width, layout, 2, true);
  add_field(fields, tr("Notes:"), large_field_width, layout, 2);
  add_field(fields, tr("ID:"), large_field_width, layout, 2);
  add_field(fields, tr("WDS:"), large_field_width, layout, 2);
  add_field(fields, tr("m1:"), default_field_width, layout, 2);
  add_field(fields, tr("m2:"), default_field_width, layout, 2);
  add_field(fields, tr("date2:"), default_field_width, layout, 2);
  add_field(fields, tr("nobs:"), default_field_width, layout, 2);
  add_field(fields, tr("sep:"), default_field_width, layout, 2);
  add_field(fields, tr("pa:"), default_field_width, layout, 2);
  add_field(fields, tr("ogrd:"), default_field_width, layout, 2);
  add_field(fields, tr("oref:"), large_field_width, layout, 2);
  add_field(fields, tr("osep:"), default_field_width, layout, 2);
  add_field(fields, tr("opa:"), default_field_width, layout, 2);

  mesInfo = new QTextEdit();
  mesInfo->setReadOnly(true);
  mesInfo->setMaximumHeight(64);
  layout->addWidget(mesInfo, 8, 0, 1, 4);

  nextButton = new QPushButton(tr("Next"));
  layout->addWidget(nextButton, 9, 0);
  prevButton = new QPushButton(tr("Prev"));
  layout->addWidget(prevButton, 9, 1);
  gotoButton = new QPushButton(tr("Goto"));
  layout->addWidget(gotoButton, 9, 2, 1, 2);

  box->setLayout(layout);
  return box;
}

QGroupBox *WdsBrowser::createAdcGroup()
{
  int default_field_width = compute_field_width("888888");

  QGroupBox *box = new QGroupBox(tr("ADC"));
  QGridLayout *layout = new QGridLayout;

  add_field(fields, tr("Az:"), default_field_width, layout, 4, true);
  add_field(fields, tr("Alt:"), default_field_width, layout, 4);
  add_field(fields, tr("z:"), default_field_width, layout, 4);
  add_field(fields, tr("ADC:"), default_field_width, layout, 4);

  box->setLayout(layout);
  return box;
}

QGroupBox *WdsBrowser::createCdcGroup()
{
  QGroupBox *box = new QGroupBox("CdC connection");
  QGridLayout *layout = new QGridLayout;

  cdcCmdTxt = new QLineEdit;
  //sendCmdButton = new QPushButton("Send");
  //sendCmdButton->setEnabled(false);
  //layout->addWidget(cdcCmdTxt,0,0);
  //layout->addWidget(sendCmdButton,0,1);
  connectButton = new QPushButton("Connect");
  connectButton->setEnabled(true);
  disconnectButton = new QPushButton("Disconnect");
  disconnectButton->setEnabled(false);
  cdcStatus = new QLabel("Not connected");
  layout->addWidget(connectButton, 0, 0);
  layout->addWidget(disconnectButton, 0, 1);
  layout->addWidget(cdcStatus, 0, 2);
  cdcLog = new QTextEdit();
  cdcLog->setReadOnly(true);
  cdcLog->setMaximumHeight(32);
  layout->addWidget(cdcLog, 1, 0, 1, 3);
  box->setLayout(layout);
  return box;
}

QGroupBox *WdsBrowser::createLogGroup()
{
  QGroupBox *box = new QGroupBox("Log");
  QGridLayout *layout = new QGridLayout;

  logComment = new QLineEdit;
  saveCommentButton = new QPushButton("Save");
  layout->addWidget(logComment,0,0);
  layout->addWidget(saveCommentButton,0,1);
  box->setLayout(layout);
  return box;
}

void WdsBrowser::update_selection(QVector<WdsEntry> &new_selection)
// Called when the extract tab emits the "extracted" signal
{
  selection = new_selection;
  current_sel = 0;
  display_selected();
  parentTab->setCurrentWidget(this); // Show us !
}

t_eqcoord wds2eqc(QString wds)
{
  t_eqcoord r;
  r.ra = (wds.mid(0,2).toDouble() + wds.mid(2,2).toDouble()/60.0 + wds.mid(4,1).toInt()/600.0)*15.0;
  r.dec = wds.mid(6,2).toDouble() + wds.mid(8,2).toDouble()/60.0;
  if ( wds.at(5) == '-' ) r.dec = -r.dec;
  return r;
}

void WdsBrowser::display_selected()
{
  WdsEntry e = selection[current_sel];
  QString idx = QString::number(current_sel+1) + "/" + QString::number(selection.count());
  fields["CNT:"].desc->setText(idx);   
  QString sp = e.sptype.trimmed();
  fields["Notes:"].desc->setText(e.notes+(sp.isEmpty()?"":" ["+sp+"]"));
  fields["ID:"].desc->setText(e.disc + e.comp);
  fields["WDS:"].desc->setText(e.wds);
  fields["m1:"].desc->setText(QString::number(e.m1));
  fields["m2:"].desc->setText(QString::number(e.m2));
  fields["date2:"].desc->setText(QString::number(e.last_obs));
  fields["nobs:"].desc->setText(QString::number(e.nb_obs));
  fields["sep:"].desc->setText(QString::number(e.sep));
  fields["pa:"].desc->setText(QString::number(e.pa));
  if ( e.has_orb ) {
    fields["ogrd:"].desc->setText(QString::number(e.orb.grade));
    fields["oref:"].desc->setText(e.orb.ref);
    fields["osep:"].desc->setText(QString::number(e.orb.sep[0]));  // TOFIX : this should depend on the current year
    fields["opa:"].desc->setText(QString::number(e.orb.pa[0]));  // TOFIX : this should depend on the current year
    }
  else {
    fields["ogrd:"].desc->setText("");
    fields["oref:"].desc->setText("");
    fields["osep:"].desc->setText("");
    fields["opa:"].desc->setText("");
    }
  display_mesures(e.disc+e.comp);
  t_eqcoord eq = wds2eqc(e.wds);
  t_azcoord az = eq2az_coord(sid_time, obs_lat, eq); 
  fields["Az:"].desc->setText(QString::number(az.az,'f',0));
  fields["Alt:"].desc->setText(QString::number(az.h,'f',0));
  int z = (int)(90-az.h);
  fields["z:"].desc->setText(QString::number(z));
  if ( !context->adctab.isEmpty() ) 
    fields["ADC:"].desc->setText(context->adctab.contains(z) ? QString::number(context->adctab[z],'f',1) : "??");
  nextButton->setEnabled(true);
  prevButton->setEnabled(true);
  gotoButton->setEnabled(true);
}

void WdsBrowser::display_mesures(QString id)
{
   mesInfo->clear();
   QStringList ms = context->mesures[id];
   for ( int i=0; i<ms.size(); i++ ) 
     mesInfo->insertPlainText("* " + ms.at(i) +"\n");
}

void WdsBrowser::nextSel()
{
  if ( current_sel == selection.count()-1 ) return;
  current_sel++;
  display_selected();
}

void WdsBrowser::prevSel()
{
  if ( current_sel == 0 ) return;
  current_sel--;
  display_selected();
}

void WdsBrowser::gotoSel()
{
  QString target = selection[current_sel].disc;
  QString cmd = "find 5 " + target.remove(QChar(' '));
  //qDebug() << cmd;
  if ( cdcClient ) {
    cdcClient->sendData(cmd);
    cdcClient->sendData("redraw");
    cdcClient->sendData("getcoorbox");
    }
}

void WdsBrowser::set_connected()
{
  cdcStatus->setText("Connected");
  //sendCmdButton->setEnabled(true);
  connectButton->setEnabled(false);
  disconnectButton->setEnabled(true);
}

void WdsBrowser::set_disconnected()
{
  cdcStatus->setText("Not connected");
  //sendCmdButton->setEnabled(false);
  connectButton->setEnabled(true);
  disconnectButton->setEnabled(false);
}

// bool parse_cdc_azcoord(QString msg, int &azimuth, int &alt)
// {
//   QRegExp rx("^OK! Az:([+-][\\d]{1,3})[^+-]+([+-][\\d]{1,3})");
//   bool ok1, ok2;
//   if ( rx.indexIn(msg) == 0 ) {
//     azimuth = rx.cap(1).toInt(&ok1);
//     alt = rx.cap(2).toInt(&ok2);
//     return ok1 && ok2;
//     }
//   else
//     return false;
// }


bool filter_cdc_msg(QString msg)
{
  return msg.startsWith("OK!");
}

void WdsBrowser::displayMsg(const QString &msg)
{
  // int azimuth, alt, z;
  if ( msg[0] != '.' && filter_cdc_msg(msg) ) {
    cdcLog->insertPlainText("CdC> " + msg);
    QScrollBar *sb = cdcLog->verticalScrollBar();
    sb->setValue(sb->maximum());
    /*
    if ( msg.startsWith("OK! Az:") && parse_cdc_azcoord(msg, azimuth, alt) ) {
      fields[14].desc->setText(QString::number(azimuth));
      fields[15].desc->setText(QString::number(alt));
      z = 90-alt;
      fields[16].desc->setText(QString::number(z));
      fields[17].desc->setText(QString::number(tab_adc(z)));
      }
    */
  }
}

// void WdsBrowser::sendCmd()
// {
//   QString cmd = cdcCmdTxt->text();
//   cdcClient->sendData(cmd);
//   cdcClient->sendData("redraw");
//   cdcCmdTxt->setText("");
// }

void WdsBrowser::saveComment()
{
  QString comment = logComment->text();
  QDate date = QDate::currentDate();
  
  if ( comment.isEmpty() ) return; // nothing to save
  QTime time = QTime::currentTime();
  if ( current_sel >= 0 ) {
    QString target = selection[current_sel].disc;
    logStream << date.toString("yyyy-MM-dd") << " " << time.toString("hh-mm-ss: ") << target << ": " <<  comment << "\n";
    }
  else
    logStream << date.toString("yyyy-MM-dd") << " " << time.toString("hh-mm-ss: ") << ": " <<  comment << "\n";
  logStream.flush();
  logComment->clear();
}

void WdsBrowser::connect_cdc()
{
  if ( cdcClient == NULL ) // not yet connected
    //cdcClient = new TcpClient(QHostAddress(QHostAddress::LocalHost), config->cdc_tcp_port);
    cdcClient = new TcpClient(QHostAddress(config->cdc_tcp_addr), config->cdc_tcp_port);
  if ( cdcClient ) {
    connect(cdcClient, SIGNAL(tcp_connected()), this, SLOT(set_connected()));
    connect(cdcClient, SIGNAL(tcp_disconnected()), this, SLOT(set_disconnected()));
    connect(cdcClient, SIGNAL(receivedData(const QString &)), this, SLOT(displayMsg(const QString &)));
    cdcClient->connect();
    }
  else
    QMessageBox::critical(this, "WDS Picker:", tr("cannot open TCP connection to Cartes du Ciel"));
}

void WdsBrowser::disconnect_cdc()
{
  if ( cdcClient != NULL ) { // already connected
    cdcClient->disconnect();
    }
}

bool WdsBrowser::sthToSave() 
{
  return !logComment->text().isEmpty();
}

void WdsBrowser::saveLog()
{
  saveComment();
}
void WdsBrowser::quit()
{
  // cdcClient->quitting();  
}
