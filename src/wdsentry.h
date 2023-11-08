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

#ifndef _wds_entry
#define _wds_entry

#include "hms.h"
#include "orbentry.h"

class WdsEntry {
public:
  WdsEntry() { };
  WdsEntry(const QString line);

  QString wds;
  QString disc;
  QString comp;
  int last_obs;
  int nb_obs;
  float pa;
  float sep;
  float m1;
  float m2;
  Hms ra;
  Hms dec;
  QString notes;
  QString sptype;
  bool has_orb;
  OrbEntry orb;

  QString toString() const;

  void write_csv(QTextStream& os) const;

  friend QTextStream& operator<<(QTextStream &, const WdsEntry &v);
};

#endif

