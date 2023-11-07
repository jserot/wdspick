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

#ifndef _config_h
#define _config_h

#include <QString>
#include <QFile>
#include <QTextStream>

class WdsPickConfig
{
public:
  WdsPickConfig(
         QString _wds_file = "wds.txt",
         QString _orb_file = "orb.txt",
         int _wds_cat_index = 5,
         QString _cdc_tcp_addr = "127.0.0.1",
         int _cdc_tcp_port = 3292,
         QString _mes_file = "mesures.csv",
         QString _log_file_prefix = "wdspick_",
         bool _launch_cdc_on_start = false,
         QString _cdc_path = "C:\\Program Files\\Ciel\\skychart.exe",
         QString _cdc_args = "",
         double _obs_long = 0.0,
         double _obs_lat = 45.0,
         QString _adc_file = "adc.txt")
    { wds_file = _wds_file;
      orb_file = _orb_file;
      wds_cat_index = _wds_cat_index;
      cdc_tcp_addr = _cdc_tcp_addr;
      cdc_tcp_port = _cdc_tcp_port;
      mes_file = _mes_file;
      log_file_prefix = _log_file_prefix;
      launch_cdc_on_start = _launch_cdc_on_start;
      cdc_path = _cdc_path;
      cdc_args = _cdc_args;
      obs_long = _obs_long;
      obs_lat = _obs_lat;
      adc_file = _adc_file; }

  WdsPickConfig(QFile *file);

  QString rsrc_path;
  QString wds_file;
  QString orb_file;
  int wds_cat_index;
  int cdc_tcp_port;
  QString cdc_tcp_addr;
  QString mes_file;
  QString log_file_prefix;
  bool launch_cdc_on_start;
  QString cdc_path;
  QString cdc_args;
  double obs_long;
  double obs_lat;
  QString adc_file;

  friend QTextStream& operator<<(QTextStream &, const WdsPickConfig &c);
  void dump(QString filename);
};

#endif
