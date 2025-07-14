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

#include "config.h"

#include <QStringList>
#include <QFileInfo>
#include <QDir>

WdsPickConfig::WdsPickConfig(QString app_dirname)
{
  app_dir = app_dirname;
  QFile configFile(app_dir + "config");
  QString data_dir = app_dir + "data/";
  // Set default values (in case the config file cannot be found / read) 
  wds_cat_index = 5;
  cdc_tcp_addr = "127.0.0.1";
  cdc_tcp_port = 3292;
  log_file_prefix = "wdspick_";
#if defined(Q_OS_LINUX)
  cdc_path = "/usr/bin/skychart";
#else
#if defined(Q_OS_WIN)
  cdc_path = "C:\\Program Files\\Ciel\\skychart.exe";
#else
#if defined(Q_OS_MAC)
  cdc_path = " /Applications/Cartes du Ciel/skychart.app";
#endif
#endif
#endif
  cdc_args = "";
  launch_cdc_on_start = false;
  obs_long = 0.0;
  obs_lat = 45.0;
  wds_file = data_dir + "wds.txt";
  orb_file = data_dir + "orb.txt";
  adc_file = data_dir + "adc.txt";
  mes_file = data_dir + "mesures.txt";
  if ( configFile.exists() ) {
    configFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream is(&configFile);
    QString line;
    while( !is.atEnd() ) {
      line = is.readLine();
      if ( line[0] == '#' ) continue;
      QStringList items = line.split("=");
      QString key = items.at(0).trimmed();
      QString value = items.at(1).trimmed();
      if ( key == "wds_cat_index" ) wds_cat_index = value.toInt();
      if ( key == "cdc_path" ) cdc_path = value;
      if ( key == "cdc_args" ) cdc_args = value;
      if ( key == "cdc_tcp_addr" ) cdc_tcp_addr = value;
      if ( key == "cdc_tcp_port" ) cdc_tcp_port = value.toInt();
      if ( key == "launch_cdc_on_start" ) launch_cdc_on_start = value=="yes";
      if ( key == "obs_long" ) obs_long = value.toDouble();
      if ( key == "obs_lat" ) obs_lat = value.toDouble();
      }
    is.flush();
    configFile.close();
    }
}

void WdsPickConfig::dump(QString filename) 
{               
  QFile file(filename);
  file.open(QIODevice::ReadWrite | QIODevice::Text);
  QTextStream os(&file);
  os << *this;
  file.close();
}

QTextStream& operator<<(QTextStream& os, const WdsPickConfig &c)
{               
  os << "app_dir = " << c.app_dir << "\n";
  os << "wds_file = " << c.wds_file << "\n";
  os << "wds_cat_index = " << c.wds_cat_index << "\n";
  os << "orb_file = " << c.orb_file << "\n";
  os << "adc_file = " << c.adc_file << "\n";
  os << "cdc_path = " << c.cdc_path << "\n";
  os << "cdc_args = " << c.cdc_args << "\n";
  os << "cdc_tcp_port = " << c.cdc_tcp_port << "\n";
  os << "launch_cdc_on_start = " << (c.launch_cdc_on_start == true ? "yes" : "no") << "\n";
  os << "mes_file = " << c.mes_file << "\n";
  os << "log_file_prefix = " << c.log_file_prefix << "\n";
  os << "obs_long = " << c.obs_long << "\n";
  os << "obs_lat = " << c.obs_lat << "\n";
  return os;
}
