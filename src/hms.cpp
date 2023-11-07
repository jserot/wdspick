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

#include "hms.h"

Hms::Hms(QString ss)
{               
  // Format : [s]hhmmss.d[d]
  if ( ss.mid(0,1) == "-" || ss.mid(0,1) == "+" ) {
    h = ss.mid(0,3).toInt();
    m = ss.mid(3,2).toInt();
    s = ss.mid(5,2).toInt();
    }
  else {
    h = ss.mid(0,2).toInt();
    m = ss.mid(2,2).toInt();
    s = ss.mid(4,2).toInt();
    }
}

QString Hms::toString() const
{               
  QString ss;
  ss.sprintf("%0+3d%02d%02d", h, m, s);
  return ss;
}

QTextStream& operator<<(QTextStream& os, const Hms &v)
{               
  QString s = v.toString();
  os << s;
  return os;
}
