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

#ifndef _wdsfilter_h
#define _wdsfilter_h

#include <QString>
#include <QMap>
#include "wdsentry.h"

class WdsFilter {
public:
  WdsFilter() { };
  
  void add_criterion(QString name, QString value) { criteria.insert(name, value); }

  bool accept(const WdsEntry &e);

  QMap<QString, QString> criteria;

  QString toString() const;

  friend QTextStream& operator<<(QTextStream &, const WdsFilter &);
};

#endif
