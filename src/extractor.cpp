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

#include <QtGui>
#include <QFile>
#include <QLabel>
#include <QTextStream>
#include <QVector>
#include <QMap>
#include <QCheckBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QApplication>
#include <QPushButton>
#include <QGridLayout>
#include <QProgressDialog>
#include <QStatusBar>

#include "extractor.h"
#include "wdsentry.h"
#include "wdsfilter.h"
#include "context.h"

WdsExtractor::WdsExtractor(WdsPickContext *context, QStatusBar *status_bar, QWidget *parent)
  : QWidget(parent), context(context), status_bar(status_bar)
{
  QFontMetrics metrics(QApplication::font());
  int default_field_width = metrics.width("88888");

  QGridLayout *layout = new QGridLayout;
  //layout->setSpacing(0);
  //layout->setSizeConstraint (QLayout::SetFixedSize);

  add_field(fields, tr("RA >"), default_field_width, layout, 2, true);
  add_field(fields, tr("RA <"), default_field_width, layout, 2);
  add_field(fields, tr("Dec >"), default_field_width, layout, 2);
  add_field(fields, tr("Dec <"), default_field_width, layout, 2);
  add_field(fields, tr("m1 >"), default_field_width, layout, 2);
  add_field(fields, tr("m1 <"), default_field_width, layout, 2);
  add_field(fields, tr("m2 >"), default_field_width, layout, 2);
  add_field(fields, tr("m2 <"), default_field_width, layout, 2);
  add_field(fields, tr("dm >"), default_field_width, layout, 2);
  add_field(fields, tr("dm <"), default_field_width, layout, 2);
  add_field(fields, tr("sep >"), default_field_width, layout, 2);
  add_field(fields, tr("sep <"), default_field_width, layout, 2);
  add_field(fields, tr("date2 <"), default_field_width, layout, 2);
  add_field(fields, tr("nobs <"), default_field_width, layout, 2);
  add_field(fields, tr("disc"), default_field_width, layout, 2);
  add_field(fields, tr("notes"), default_field_width, layout, 2);

  QPushButton* extractButton = new QPushButton("Extract");
  extractButton->setDefault(true);
  layout->addWidget(extractButton, 8, 0, 1, 4);
  connect(extractButton, SIGNAL(clicked()), this, SLOT(extract()));

  setLayout(layout);
}

void WdsExtractor::add_field(
  QMap<QString,Field> &fields,
  QString lbl,
  int width,
  QGridLayout *layout,
  int ncols,
  bool first_in_layout)
{
  static int row=0, col=0;
  Field f;
  f.checked = new QCheckBox(lbl);
  f.entry = new QLineEdit;
  f.entry->setFixedWidth(width);
  fields.insert(lbl,f);
  row = first_in_layout ? 0 : col==ncols-1 ? row+1 : row;
  col = first_in_layout ? 0 : col==ncols-1 ? 0 : col+1;
  layout->addWidget(f.checked, row, col*2);
  layout->addWidget(f.entry, row, col*2+1);
}

void WdsExtractor::extract()
{
   WdsFilter filt;
   QMapIterator<QString,Field> i(fields);
   while (i.hasNext()) {
     i.next();
     if ( i.value().checked->isChecked() ) 
       filt.add_criterion(i.value().checked->text(), i.value().entry->text());
     }
   selection.clear();
   read_wds_file(context, filt, selection);
   if ( selection.count() > 0 ) {
     emit extracted(selection);
     }
}

void WdsExtractor::read_wds_file(WdsPickContext *ctx, WdsFilter &filt, QVector<WdsEntry> &sel)
{
   QString line;
   QProgressDialog progress(this);
   progress.setLabelText(tr("Scanning %1").arg(ctx->wds->fileName()));
   progress.setRange(1,ctx->wds_count);
   progress.setModal(true);
   int k = 0;
   QTextStream is(ctx->wds);
   ctx->wds->seek(0);
   while ( !is.atEnd() ) {
     line = is.readLine();
     progress.setValue(++k);
     WdsEntry e(line);
     if ( filt.accept(e) ) {
       if ( !ctx->orbits.isEmpty() && ctx->orbits.contains(e.wds) ) {
         e.has_orb = true;
         e.orb = ctx->orbits[e.wds]; 
         }
       else 
         e.has_orb = false;
       sel.append(e);
       }
     }
   status_bar->showMessage(tr("%1").arg(sel.count()));
}

void WdsExtractor::writeSpecFile(const QString &fname)
{
  QFile f(fname);
  if ( f.open(QIODevice::ReadWrite | QIODevice::Text) ) {
    QTextStream os(&f);
    QMapIterator<QString,Field> i(fields);
    while (i.hasNext()) {
      i.next();
      if ( i.value().checked->isChecked() ) 
        os << i.key() << "; "<<  i.value().entry->text() << endl;
      }
    f.close();
    }
  else
    QMessageBox::critical(this, "WDS Picker:", tr("Cannot write spec file %1!\n").arg(fname));
}

void WdsExtractor::readSpecFile(const QString &fname)
{
  QFile f(fname);
  if ( f.open(QIODevice::ReadOnly | QIODevice::Text) ) {
    QTextStream is(&f);
    QString line;
    QStringList e;
    for ( int i=0; !is.atEnd(); i++ ) {
      line = is.readLine();
      e = line.split(";");
      if ( fields.contains(e.at(0)) ) {
        fields[e.at(0)].checked->setChecked(1);
        fields[e.at(0)].entry->setText(e.at(1).trimmed());
        }
      }
    f.close();
  }
  else
    QMessageBox::critical(this, "WDS Picker:", tr("Cannot read config file %1!\n").arg(fname));
}

void WdsExtractor::quit()
{
  qDebug() << "WdsExtractor: quit";
}
