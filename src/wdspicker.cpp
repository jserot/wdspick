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
#include <QTabWidget>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QProcess>
#include <QMap>
#include <QStringList>
#include <QDesktopServices>
#include <QLabel>

#include "wdspicker.h"
#include "extractor.h"
#include "browser.h"
#include "config.h"
#include "context.h"

WdsPicker::WdsPicker(WdsPickConfig *config, WdsPickContext *ctx, QStatusBar *status_bar) :
  context(ctx), status_bar(status_bar)
{
  if ( ctx->wds_count == 0 ) 
    QMessageBox::critical(this, "WdsPick:", tr("Cannot read WDS file \"%1\"").arg(config->wds_file));
  if ( ctx->orbits.count() == 0 )
    QMessageBox::warning(this, "WdsPick:", tr("No orbit file loaded\n"));
//   if ( ctx->mes_count == 0 )
//     QMessageBox::warning(this, "WdsPick:", tr("No measure file loaded\n"));
//   if ( ctx->adctab.count() == 0 )
//     QMessageBox::warning(this, "WdsPick:", tr("No ADC file loaded\n"));

  QTabWidget *tabWidget = new QTabWidget();
  wdsExtractor = new WdsExtractor(context, status_bar);
  wdsBrowser = new WdsBrowser(config, context, wdsExtractor, tabWidget);

  tabWidget->addTab(wdsExtractor, tr("Extract"));
  tabWidget->addTab(wdsBrowser, tr("Browse"));

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tabWidget);
  setLayout(mainLayout);
}

void WdsPicker::readSpecFile(const QString &fileName)
{
  wdsExtractor->readSpecFile(fileName);
  //status_bar->showMessage(tr("Spec file loaded"));
}


void WdsPicker::writeSpecFile(const QString &fileName)
{
  wdsExtractor->writeSpecFile(fileName);
  //status_bar->showMessage(tr("Spec file saved"));
}

void WdsPicker::writeExtracted(const QString &fileName)
{
  wdsExtractor->writeSelection(fileName);
  //status_bar->showMessage(tr("Selection saved"));
}

void WdsPicker::extract()
{
  wdsExtractor->extract();
}

void WdsPicker::quit()
{
  wdsExtractor->quit();
  wdsBrowser->quit();
}
