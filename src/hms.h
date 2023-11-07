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

#ifndef _hms_h
#define _hms_h

#include <QTextStream>

class Hms
{
public:
  Hms(const short _h=0,
      const unsigned short _m=0,
      const unsigned short _s=0)
    { h = _h; m = _m; s=_s; }
  Hms(const QString s);
  short h;           // hr ou deg
  unsigned short m;  // mn
  unsigned short s;  // sec

  QString toString() const;

  friend QTextStream& operator<<(QTextStream &, const Hms &v);
};

#endif
