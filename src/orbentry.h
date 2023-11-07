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

#ifndef _orb_entry
#define _orb_entry

#include <QTextStream>

class OrbEntry {
public:
  OrbEntry() { };
  OrbEntry(const QString line);

  QString wds;
  QString disc;
  QString comp;
  int grade;
  QString ref;
  float pa[5];
  float sep[5];
  QString notes;

  QString toString() const;

  friend QTextStream& operator<<(QTextStream &, const OrbEntry &v);
};

#endif

