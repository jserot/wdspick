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

#ifndef _context_h
#define _context_h

#include <QString>
#include <QFile>
#include <QMap>
#include <QProcess>
//#include <QTextStream>
#include "orbentry.h"
#include "tcpclient.h"

class WdsPickContext {
 public:
 WdsPickContext() : wds(NULL), wds_count(0), mes_count(0) { }

  QFile *wds;
  int wds_count;
  QMap<QString,OrbEntry> orbits;
  QMap<QString,QStringList> mesures;
  int mes_count;  // may be different from mesures.count()
  QMap<int,float> adctab;
  double obs_long;
  double obs_lat;

  QString toString() const;
  friend QTextStream& operator<<(QTextStream &, const WdsPickContext &v);
};

#endif

