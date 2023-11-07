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

#ifndef _astrometry_h
#define _astrometry_h

typedef struct { double ra; double dec; } t_eqcoord; 

typedef struct { double az; double h; } t_azcoord; 

QTime get_sideral_time(QDateTime now, double longitude);
QTime get_hour_angle(QTime lst, QTime ra);
t_azcoord eq2az_coord(QTime lst, double lat, t_eqcoord c);

#endif
