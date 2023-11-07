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

#include "wdsfilter.h"
#include <math.h>

bool WdsFilter::accept(const WdsEntry &e)
{
  QMapIterator<QString, QString> i(this->criteria);
  bool r = true;
  while ( i.hasNext() ) {
    i.next();
    if ( i.key() == "RA >" ) r = r && e.ra.h >= i.value().toInt();
    if ( i.key() == "RA <" ) r = r && e.ra.h < i.value().toInt();  // equal excluded here
    if ( i.key() == "Dec >" ) r = r && e.dec.h >= i.value().toInt();
    if ( i.key() == "Dec <" ) r = r && e.dec.h <= i.value().toInt();
    if ( i.key() == "m1 >" ) r = r && e.m1 >= i.value().toFloat();
    if ( i.key() == "m1 <" ) r = r && e.m1 <= i.value().toFloat();
    if ( i.key() == "m2 >" ) r = r && e.m2 >= i.value().toFloat();
    if ( i.key() == "m2 <" ) r = r && e.m2 <= i.value().toFloat();
    if ( i.key() == "dm >" ) r = r && fabs(e.m1-e.m2) >= i.value().toFloat();
    if ( i.key() == "dm <" ) r = r && fabs(e.m1-e.m2) <= i.value().toFloat();
    if ( i.key() == "sep >" ) r = r && e.sep >= i.value().toFloat();
    if ( i.key() == "sep <" ) r = r && e.sep <= i.value().toFloat();
    if ( i.key() == "date2 <" ) r = r && e.last_obs <= i.value().toInt();
    if ( i.key() == "nobs <" ) r = r && e.nb_obs <= i.value().toInt();
    if ( i.key() == "disc" ) {
      QString pat = i.value();
      if ( pat.startsWith("-") )  // All except those ..
        r = r && ( ! e.disc.startsWith(pat.remove(0,1)));
      else                           // All those
        r = r && e.disc.startsWith(pat);
      }
    if ( i.key() == "notes" ) r = r && e.notes.contains(i.value());
  }
  return r;
}

QString WdsFilter::toString() const
{               
  QString ss = "[";
  QMapIterator<QString, QString> i(criteria);
  while ( i.hasNext() ) {
    i.next();
    ss += "k=" + i.key() + ",v=" + i.value() + "; ";
    }
  ss += "]";
  return ss;
}

QTextStream& operator<<(QTextStream& os, const WdsFilter &v)
{               
  QString s = v.toString();
  os << s;
  return os;
}
