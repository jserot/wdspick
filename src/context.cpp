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

#include "context.h"
#include <math.h>

QString WdsPickContext::toString() const
{
  QString ss;
  char lg[16], lt[16];
  sprintf(lt, "%2.1f%c", fabs(obs_lat), obs_lat>=0?'N':'S');
  sprintf(lg, "%2.1f%c", fabs(obs_long), obs_long>=0?'W':'E');
  ss.sprintf("Obs:%s/%s     WDS:%d Orb:%d Mes:%d ADC:%s",
             lt, lg,
             wds_count,
             orbits.count(),
             mes_count,
             adctab.count()>0?"Loaded":"n/a");
  return ss;
}

QTextStream& operator<<(QTextStream& os, const WdsPickContext &v)
{
  QString s = v.toString();
  os << s;
  return os;
}
