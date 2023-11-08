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

#ifndef _mainwindow_h
#define _mainwindow_h

#include <QMainWindow>
#include <QCloseEvent>
#include "config.h"
#include "wdspicker.h"
#include "context.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(WdsPickConfig *config, WdsPickContext *context);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void openSpecFile();
    void saveSpecFile();
    void selectionExtracted();
    void saveExtraction();
    void extract();
    void about();

private:
    void createActions();
    void createMenus();
    //void createToolBars();
    void createStatusBar(WdsPickContext *ctx);
    //void readSettings();
    //void writeSettings();
    void readSpecFile(const QString &fileName);
    void writeSpecFile(const QString &fileName);
    void writeExtracted(const QString &fileName);
    bool maybeSave();

    WdsPicker *wdsPicker;
    QStatusBar *status_bar;

    QMenu *fileMenu;
    QMenu *actionMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QAction *saveSpecAsAct;
    QAction *saveExtractedAsAct;
    QAction *exitAct;
    QAction *extractAct;
    QAction *aboutAct;
    //QAction *aboutQtAct;
};

#endif
