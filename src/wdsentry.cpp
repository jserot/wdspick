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

WdsEntry::WdsEntry(const QString line)
{
  // See : http://ad.usno.navy.mil/wds/Webtextfiles/wdsweb_format.txt
  wds = get_field(line, 1, 10);
  disc = get_field(line, 11, 17);
  comp = get_field(line, 18, 22).trimmed();
  last_obs = get_field(line, 29, 32).toInt();
  nb_obs = get_field(line, 34, 37).toInt();
  pa = get_field(line, 43, 45).toDouble();
  sep = get_field(line, 53, 57).toDouble();
  m1 = get_field(line, 59, 63).toDouble();
  m2 = get_field(line, 65, 69).toDouble();
  sptype = get_field(line, 71, 79);
  ra = get_field(line, 113, 118);
  dec = get_field(line, 122, 128);
  notes = get_field(line, 108, 111);
}

QString WdsEntry::toString() const
{
  QString ss;
  ss.sprintf("%s %s%s nobs=%4d last=%d m1=%4.1f m2=%4.1f sep=%5.2f pa=%5.1f ra=%s dec=%s sp=%s notes=%s",
             qPrintable(wds), qPrintable(disc), qPrintable(comp),
             nb_obs, last_obs,
             m1, m2, sep, pa, 
             qPrintable(ra.toString()), qPrintable(dec.toString()), qPrintable(sptype), qPrintable(notes));
  if ( has_orb ) 
    ss.sprintf(" ogrd=%d oref=%s osep[1]=%5.2f opa[1]=%5.1f", orb.grade, qPrintable(orb.ref), orb.sep[1], orb.pa[1]);
  ss.sprintf("\n");
  return ss;
}

QTextStream& operator<<(QTextStream& os, const WdsEntry &v)
{
  QString s = v.toString();
  os << s;
//   os << v.wds << " " << v.disc << v.comp 
//      << "nobs=" <<  v.nb_obs << " last=" << v.last_obs 
//      << " m1=" << v.m1 << " m2=" << v.m2
//      << " sep=" << v.sep << " pa=" << v.pa
//      << " ra=" << v.ra << " dec=" << v.dec
//      << " notes=" << v.notes;
  return os;
}
