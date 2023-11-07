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

// #include <QtWidgets>   QT5 specific !
// Under QT4, have to list individual components..
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QApplication>
#include <QProcess>
#include <QDesktopServices>
//#include <QSettings>

#include "mainwindow.h"
#include "wdspicker.h"

MainWindow::MainWindow(WdsPickConfig *config, WdsPickContext *context)
{
    createStatusBar(context);
    wdsPicker = new WdsPicker(config, context, status_bar);
    setCentralWidget(wdsPicker);
    createActions();
    createMenus();
    //readSettings();
    //setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if ( maybeSave() ) 
      event->accept();
    else
      event->ignore();
    // wdsPicker->quit();
}

bool MainWindow::maybeSave()
{
  if (wdsPicker->wdsBrowser->sthToSave() ) {
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("WdsPick"), tr("The log file has been modified.\n"
                        "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save) {
      wdsPicker->wdsBrowser->saveLog();
      return true;
      }
    else if (ret == QMessageBox::Cancel)
      return false;
    }
  return true;
}

void MainWindow::openSpecFile()
{
  QString fileName = QFileDialog::getOpenFileName(this);
  if (!fileName.isEmpty())
    wdsPicker->readSpecFile(fileName);
}

void MainWindow::saveSpecFile()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty())
      wdsPicker->writeSpecFile(fileName);
}

void MainWindow::extract()
{
  wdsPicker->extract();
}

static QString aboutTxt = QMessageBox::tr(
"<center><h3>WdsPick version 2.2</h3></center>"
"<center>(c) Jocelyn Serot, 2018-now</center>"
"<center><code>jocelyn.serot@free.fr</code></center>"
"<center><a href=\"http://www.astrosurf.com/legalet/Astro/WdsPIck\">www.astrosurf.com/legalet/Astro/WdsPIck</a></center>"
"<p>WdsPick is mailware : if you find it useful, find bugs or have suggestions for improvements, please send an email to the author.<p>"
"<h4>DISTRIBUTION</h4>"
"<p>Any commercial usage of the WdsPick software is STRICTLY FORBIDDEN without explicit authorization by the \
author. You can make copy and distribute WdsPick at the MANDATORY CONDITION that the distributed version \
includes ALL the files of the original distribution (as found on the website). The best way to \
advertise WdsPick is therefore to forward the URL of the website.</p>"
"<h4>IMPORTANT</h4>"
"<p>The WdsPick software is distributed as is with no warranty of any kind. \
In no event shall the author be liable for any damages resulting from its usage</p>"
"<h4>ACKNOWLEDGEMENTS</h4>"
"<p>Development of the WdsPick software has been made possible by the availibility of \
the <a href=\"http://www.usno.navy.mil/USNO/astrometry/optical-IR-prod/wds/WDS\">Washington Double Star Catalog</a> \
and the <a href=\"http://www.usno.navy.mil/USNO/astrometry/optical-IR-prod/wds/ORB6\">Sixth Orbit Catalog</a>, \
both maintained at the <a href=http://www.usno.navy.mil/USNO>U.S. Naval Observatory</a>");
// TO FIX: should read version number from ../VERSION

void MainWindow::about()
{
  QMessageBox::about(this, tr("About Application"), aboutTxt);
}

void MainWindow::createActions()
{
    openAct = new QAction(/*QIcon(":/images/open.png"),*/  tr("&Open Spec File..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    //openAct->setStatusTip(tr("Open a specification file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openSpecFile()));

    saveAsAct = new QAction(tr("&Save Spec File..."), this);
    saveAsAct->setShortcuts(QKeySequence::Save);
    //saveAsAct->setStatusTip(tr("Save the current specifications in a file"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveSpecFile()));

    exitAct = new QAction(tr("Q&uit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    //exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    extractAct = new QAction(tr("E&xtract"), this);
    extractAct->setShortcut(tr("Ctrl+E"));
    //extractAct->setStatusTip(tr("Extract data for browsing"));
    connect(extractAct, SIGNAL(triggered()), this, SLOT(extract()));

    aboutAct = new QAction(tr("&About"), this);
    //aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

//     aboutQtAct = new QAction(tr("About &Qt"), this);
//     aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
//     connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    actionMenu = menuBar()->addMenu(tr("&Action"));
    actionMenu->addAction(extractAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    //helpMenu->addAction(aboutQtAct);
}

void MainWindow::createStatusBar(WdsPickContext *ctx)
{
  QFont f( "Courrier", 10, QFont::Normal);
  QLabel *l = new QLabel;
  l->setFont( f);
  //l->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  //l->setTextFormat(Qt::RichText);
  l->setText(ctx->toString());
  status_bar = statusBar();
  status_bar->addPermanentWidget(l);
}

// void MainWindow::readSettings()
// {
//     QSettings settings("QtProject", "Application Example");
//     QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
//     QSize size = settings.value("size", QSize(400, 400)).toSize();
//     resize(size);
//     move(pos);
// }

// void MainWindow::writeSettings()
// {
//     QSettings settings("QtProject", "Application Example");
//     settings.setValue("pos", pos());
//     settings.setValue("size", size());
// }


