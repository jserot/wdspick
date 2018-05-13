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

#include <math.h>
#include <stdio.h>

#include <QDateTime>
#include <QDebug>
#include "astrometry.h"

#define PI2 (M_PI*2)

typedef struct { int h; int m; int s; } t_hms;

typedef struct { int d; int m; int s; } t_dms;

double deg2rad(double x) { return x*M_PI/180; }

double rad2deg(double x) { return x*180/M_PI; }

double sgn(double x) { return x<0 ? -1.0 : 1.0; }

t_dms deg2dms(double x) {
  double f;
  t_dms r;
  r.d = (int)(x);
  f = (x-r.d)*60;
  r.m = (int)(f);
  f = (f-r.m)*60;
  r.s = (int)(f);
  return r;
}

double dms2deg(t_dms x) { return x.d+x.m/60.0+x.s/3600.0; }

double hms2deg(t_hms x) { return (x.h+x.m/60.0+x.s/3600.0)*15; }

t_hms deg2hms(double x) { t_hms r; t_dms t = deg2dms(x/15); r.h=t.d; r.m=t.m; r.s=t.s; return r; }

int ndays(int a, int m, int j) // from 0 jan 1901
{
  double r;
  static double offset[12] = { 0, 31, 59.25, 90.25, 120.25, 151.25, 181.25, 212.25, 243.25, 273.25, 304.25, 334.25 };
  a = a-1901;
  r = a * 365.25 + j + offset[m-1];
  return r;
}

typedef struct {
  double deg;
  t_dms dms;
  t_hms hms;
} angle;

typedef struct { angle lg; angle lt; } t_gpos; // Longitude, latitude

double tsid(double n) // Sideral time at Greenwhich at day n, 0h UTC
{
  double r;
  // r = 23750.3 + 236.555362 * n;  // sec
  // return fmod(r,86400.0)+0.5;
  r = 98.965 + 0.985647342 * n;     // deg
  return fmod(r,360.0);
} 

t_azcoord eq2az(
  double phi, // Latitude (deg)
  double hh,  // Angle horaire (TS-RA) (deg)
  double delta)  // Declinaison (deg)
{
  double sh, a;
  t_azcoord r;
  phi = deg2rad(phi);
  hh = deg2rad(hh);
  delta = deg2rad(delta);
  sh = sin(phi)*sin(delta)+cos(phi)*cos(delta)*cos(hh); // refraction omitted
  r.h = rad2deg(asin(sh));
//   cz = sin(phi)*sin(delta)+cos(phi)*cos(delta)*cos(hh); // refraction omitted
//   z = acos(cz);
//   r.h = 90-rad2deg(z);
//   sa = z != 0 ? cos(delta)*sin(hh)/sin(z) : 0;
//   ca = z != 0 ? (sin(phi)*cos(delta)*cos(hh)-cos(phi)*sin(delta))/sin(z) : 1;
//   a = atan(sa/ca);
  a = atan2(cos(delta)*sin(hh),sin(phi)*cos(delta)*cos(hh)-cos(phi)*sin(delta));
  r.az = rad2deg(a)+180;
  return r;
}

QTime get_sideral_time(QDateTime now, double longitude)
{
  int n;
  QDate d = now.date();
  QTime t = now.time();
  double hf;
  angle utc, ts0, ts;
  utc.hms.h = t.hour();
  utc.hms.m = t.minute();
  utc.hms.s = t.second();
  n = ndays(d.year(),d.month(),d.day());
  hf = (t.hour()+t.minute()/60.0+t.second()/3600.0)/24.0; 
  ts0.deg = tsid(n+hf);
  ts0.hms = deg2hms(ts0.deg);
  utc.deg = hms2deg(utc.hms);
  ts.deg = fmod(ts0.deg+utc.deg-longitude, 360.0);
  ts.hms = deg2hms(ts.deg);
  return QTime(ts.hms.h,ts.hms.m,ts.hms.s);
}

#define QTIME2DEG(t) ((((t).hour())+((t).minute())/60.0+((t).second())/3600.0)*15)

QTime get_hour_angle(QTime lst, QTime ra)
{
  angle ha;
  ha.deg = QTIME2DEG(lst)-QTIME2DEG(ra); // Hour angle = local sideral time - RA
  if ( ha.deg < 0.0 ) ha.deg += 360;
  ha.hms = deg2hms(ha.deg);
  return QTime(ha.hms.h,ha.hms.m,ha.hms.s);
}

t_azcoord eq2az_coord(QTime lst, double lat, t_eqcoord c)
{
  t_azcoord r;
  double ts = QTIME2DEG(lst);
  double ha = ts - c.ra;   // Angle horaire
  if ( ha < 0 ) ha += 360.0;
  r = eq2az(lat, ha, c.dec);
  return r;
}
