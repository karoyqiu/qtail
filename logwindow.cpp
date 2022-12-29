/*! ***********************************************************************************************
 *
 * \file        logwindow.cpp
 * \brief       LogWindow 类源文件。
 *
 * \version     0.1
 * \date        2022-12-29
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2022 开心网络。
 *
 **************************************************************************************************/
#include "logwindow.h"
#include "icyfiremodel.h"


LogWindow::LogWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMdiSubWindow(parent, flags)
{
    setAttribute(Qt::WA_DeleteOnClose);
}


QTableView *LogWindow::view() const
{
    return qobject_cast<QTableView *>(widget());
}


IcyfireModel *LogWindow::model() const
{
    return qobject_cast<IcyfireModel *>(view()->model());
}


void LogWindow::closeEvent(QCloseEvent *e)
{
    QMdiSubWindow::closeEvent(e);
    emit closed(windowFilePath());
}
