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

WdsPickConfig::WdsPickConfig(QFile *file)
{
  wds_file = "wds.txt"; // Use default if no corresp value found in file
  orb_file = "orb.txt"; // Use default if no corresp value found in file
  wds_cat_index = 5;
  cdc_tcp_port = 3292;
  mes_file = "mesures.csv";
  log_file_prefix = "wdspick_";
  file->open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream is(file);
  QString line;
  while( !is.atEnd() ) {
    line = is.readLine();
    if ( line[0] == '#' ) continue;
    QStringList items = line.split("=");
    QString key = items.at(0).trimmed();
    QString value = items.at(1).trimmed();
    if ( key == "wds_file" ) wds_file = value;
    if ( key == "wds_cat_index" ) wds_cat_index = value.toInt();
    if ( key == "cdc_tcp_addr" ) cdc_tcp_addr = value;
    if ( key == "cdc_tcp_port" ) cdc_tcp_port = value.toInt();
    if ( key == "mes_file" ) mes_file = value;
    if ( key == "log_file_prefix" ) log_file_prefix = value;
    if ( key == "launch_cdc_on_start" ) launch_cdc_on_start = value=="yes";
    if ( key == "cdc_path" ) cdc_path = value;
    if ( key == "cdc_args" ) cdc_args = value;
    if ( key == "obs_long" ) obs_long = value.toDouble();
    if ( key == "obs_lat" ) obs_lat = value.toDouble();
    if ( key == "adc_file" ) adc_file = value;
    }
 is.flush();
 file->close();
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
  os << "wds_file = " << c.wds_file << "\n";
  os << "orb_file = " << c.orb_file << "\n";
  os << "wds_cat_index = " << c.wds_cat_index << "\n";
  os << "cdc_tcp_port = " << c.cdc_tcp_port << "\n";
  os << "mes_file = " << c.mes_file << "\n";
  os << "log_file_prefix = " << c.log_file_prefix << "\n";
  os << "launch_cdc_on_start = " << (c.launch_cdc_on_start == true ? "yes" : "no") << "\n";
  os << "cdc_path = " << c.cdc_path << "\n";
  os << "cdc_args = " << c.cdc_args << "\n";
  os << "cdc_args = " << c.cdc_args << "\n";
  os << "obs_long = " << c.obs_long << "\n";
  os << "obs_lat = " << c.obs_lat << "\n";
  os << "adc_file = " << c.adc_file << "\n";
  return os;
}
