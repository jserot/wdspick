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

#ifndef _wdsextract_h
#define _wdsextract_h

#include <QFile>
#include <QWidget>
#include <QVector>
#include <QMap>
#include <QGridLayout>

#include "wdsentry.h"
#include "wdsfilter.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QStatusBar;
class WdsPickContext;

class WdsExtractor : public QWidget
{
Q_OBJECT
public:
  WdsExtractor(WdsPickContext *context, QStatusBar *status_bar, QWidget *parent = 0);
  void readSpecFile(const QString &filename);
  void writeSpecFile(const QString &filename);

signals:
  void extracted(QVector<WdsEntry> &);

public slots:
  void extract();
  void quit();

 private:
  struct Field { QCheckBox *checked; QLineEdit *entry; };
  QMap<QString,Field> fields;
  void add_field(QMap<QString,Field> &fields, QString lbl, int width, QGridLayout *layout, int ncols, bool first_in_layout = false);
  QVector<WdsEntry> selection;
  WdsPickContext *context;
  void read_wds_file(WdsPickContext *ctx, WdsFilter &filt, QVector<WdsEntry> &sel);
  QStatusBar *status_bar;
};


#endif
