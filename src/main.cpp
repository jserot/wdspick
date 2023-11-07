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

#include <QApplication>
#include <QDate>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

#include "tcpclient.h"
#include "mainwindow.h"
#include "config.h"
#include "context.h"

int openWdsFile(WdsPickConfig *config, WdsPickContext *context);
int readOrbFile(WdsPickConfig *config, WdsPickContext *context);
int readMesFile(WdsPickConfig *config, WdsPickContext *context);
int readAdcFile(WdsPickConfig *config, WdsPickContext *context);
bool launchCdcProcess(WdsPickConfig *config);

int main(int argc, char *argv[])
{
  //Q_INIT_RESOURCE(wdspick);
  QApplication app(argc, argv);
  app.setApplicationName("WdsPick");
#if defined(Q_OS_MAC)
  QString rsrc_path = app.applicationDirPath() + "/../Resources/";
#else
  QString rsrc_path = "./";
#endif
  QFile iniFile(rsrc_path+"wdspick.ini");
  QFile wdsFile;
  WdsPickConfig *config;
  if ( iniFile.exists()  )
    config = new WdsPickConfig(&iniFile);
  else 
    config = new WdsPickConfig(); // No .ini file found. Reverting to defaults values for various file names");
  config->rsrc_path = rsrc_path;
  config->dump("config.log");
  WdsPickContext *context = new WdsPickContext;
  openWdsFile(config, context); 
  readOrbFile(config, context);
  readMesFile(config, context);
  readAdcFile(config, context);
  if ( config->launch_cdc_on_start ) launchCdcProcess(config);
  context->obs_long = config->obs_long;
  context->obs_lat = config->obs_lat;
  MainWindow mainWin(config, context);
  mainWin.show();
  return app.exec();
}

int openWdsFile(WdsPickConfig *config, WdsPickContext *ctx)
{
  int k=0;
  QFile* file = new QFile;
  file->setFileName(config->rsrc_path+config->wds_file);
  // We don't read the file in memory now. Only count lines. Loading will only occur after filtering
  if ( file->open(QIODevice::ReadOnly) ) {
    QTextStream is(file);
    while ( !is.atEnd() ) {
      is.readLine();
      k++;
     }
    ctx->wds = file;  // leave it open
    ctx->wds_count = k;
    return k;
    }
  else
    return 0;
}

int readOrbFile(WdsPickConfig *config, WdsPickContext *ctx)
{
  int k = 0;
  QFile file;
  QString line;
  file.setFileName(config->rsrc_path+config->orb_file);
  if ( file.open(QIODevice::ReadOnly) ) {
    QTextStream is(&file);
    while ( !is.atEnd() ) {
      line = is.readLine();
      OrbEntry e(line);
      ctx->orbits.insert(e.wds, e);
      k++;
     }
    file.close();
    return k;
    }
  else
    return 0;
}

int readMesFile(WdsPickConfig *config, WdsPickContext *ctx)
{
  QFile file;
  QString id, info;
  QStringList l, infos;
  int k = 0;
  file.setFileName(config->rsrc_path+config->mes_file);
  if ( file.open(QIODevice::ReadOnly) ) {
    QTextStream is(&file);
    while ( !is.atEnd() ) {
      l = is.readLine().split(";");
      id = l[0].trimmed();
      infos = l.mid(1);
      info = infos.join(", ");
      if ( ctx->mesures.contains(id) ) 
        ctx->mesures[id].append(info);
      else
        ctx->mesures.insert(id, QStringList(info));
      k++;
     }
    file.close();
    ctx->mes_count=k;
    return k;
    }
  else
    return 0;
}

int readAdcFile(WdsPickConfig *config, WdsPickContext *ctx)
{
  QFile file;
  QStringList l;
  int k = 0;
  file.setFileName(config->rsrc_path+config->adc_file);
  if ( file.open(QIODevice::ReadOnly) ) {
    QTextStream is(&file);
    while ( !is.atEnd() ) {
     l = is.readLine().split(';');
     ctx->adctab.insert(l[0].toInt(), l[1].toFloat());
     k++;
     }
    file.close();
    return k;
    }
  else
    return 0;
}

// TODO : factorise four prev fns 

bool launchCdcProcess(WdsPickConfig *config)
{
  bool proc_started;
#if defined(Q_OS_MAC)
  QString appPath = config->cdc_path;
  proc_started = QDesktopServices::openUrl(QUrl::fromLocalFile(appPath));
  // TO DO : add args ?
#else 
  QString program = config->cdc_path;
  QProcess *process = new QProcess(NULL);
  proc_started = process->startDetached(config->cdc_path, config->cdc_args.split(""));
#endif
  return proc_started;
}

