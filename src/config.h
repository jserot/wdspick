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
#include <QTextStream>

class WdsPickConfig
{
public:
  WdsPickConfig(QString app_dirname);

  QString app_dir;
  QString wds_file;
  int wds_cat_index;
  QString orb_file;
  QString adc_file;
  QString cdc_path;
  QString cdc_args;
  int cdc_tcp_port;
  QString cdc_tcp_addr;
  bool launch_cdc_on_start;
  QString mes_file;
  QString log_file_prefix;
  double obs_long;
  double obs_lat;

  friend QTextStream& operator<<(QTextStream &, const WdsPickConfig &c);
  void dump(QString filename);
};

#endif
