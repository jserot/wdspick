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

#ifndef _wdspicker_h
#define _wdspicker_h

#include <QWidget>
#include "extractor.h"
#include "browser.h"

#define VERSION "2.1"

class QTabWidget;
class QStatusBar;
class WdsPickConfig;
class WdsPickContext;
class QStatusBar;
class QLabel;

class WdsPicker : public QWidget
{
  Q_OBJECT
public:
  WdsPicker(WdsPickConfig *config, WdsPickContext *ctx, QStatusBar *status);

  void readSpecFile(const QString &fileName);
  void writeSpecFile(const QString &fileName);
  void writeExtracted(const QString &fileName);
  void extract();
  void quit();

public:
  WdsExtractor *wdsExtractor;
  WdsBrowser *wdsBrowser;
private:
  QTabWidget *tabWidget;
  WdsPickContext *context;
  QStatusBar *status_bar;
};


#endif
