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

#include "wdsentry.h"

inline QString get_field(const QString &src, int start, int end)
{
  return src.mid(start-1, end-start+1);
}

OrbEntry::OrbEntry(const QString line)
{
  // See : http://ad.usno.navy.mil/wds/orb6/orb6format.txt
  wds = get_field(line, 1, 10);
  disc = get_field(line, 12, 18);
  comp = get_field(line, 19, 20);
  grade = get_field(line, 30, 30).toInt();
  ref = get_field(line, 35, 42);
  pa[0] = get_field(line, 47, 51).toDouble();
  sep[0] = get_field(line, 55, 60).toDouble();
  pa[1] = get_field(line, 64, 68).toDouble();
  sep[1] = get_field(line, 72, 77).toDouble();
  pa[2] = get_field(line, 81, 85).toDouble();
  sep[2] = get_field(line, 89, 94).toDouble();
  pa[3] = get_field(line, 98, 102).toDouble();
  sep[3] = get_field(line, 106, 111).toDouble();
  pa[4] = get_field(line, 115, 119).toDouble();
  sep[4] = get_field(line, 123, 60).toDouble();
  notes = get_field(line, 131, 147);
}

QString OrbEntry::toString() const
{
  QString ss;
  ss.sprintf("%s %s%s grade=%d ref=%s", qPrintable(wds), qPrintable(disc), qPrintable(comp), grade, qPrintable(ref));
  for ( int i=0; i<5; i++) 
    ss.sprintf(" sep[%d]=%5.2f pa[%d]=%5.1f", i+1, pa[i], i+1, sep[i]);
  ss.sprintf("notes=%s", qPrintable(notes));
  return ss;
}

QTextStream& operator<<(QTextStream& os, const OrbEntry &v)
{
  QString s = v.toString();
  os << s;
  return os;
}
