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
#include "logleveldelegate.h"
#include "logproxymodel.h"

QFont LogWindow::mono_;


LogWindow::LogWindow(QFile *file, QWidget *parent, Qt::WindowFlags flags)
    : QMdiSubWindow(parent, flags)
    , view_(nullptr)
    , proxy_(nullptr)
    , model_(nullptr)
{
    Q_ASSERT(file != nullptr && file->isOpen() && file->isReadable());
    file->setParent(this);

    view_ = new QTableView(this);
    view_->setFont(mono_);

    auto *stream = new QTextStream(file);
    stream->setCodec("UTF-8");
    stream->setAutoDetectUnicode(true);

    model_ = new IcyfireModel(this);
    model_->setStream(stream);

    proxy_ = new LogProxyModel(this);
    proxy_->setSourceModel(model_);
    view_->setModel(proxy_);

    auto *dlgt = new LogLevelDelegate(this);
    view_->setItemDelegateForColumn(IcyfireModel::LevelColumn, dlgt);

    auto *header = view_->horizontalHeader();
    header->setStretchLastSection(true);
    header->resizeSections(QHeaderView::ResizeToContents);

    setWidget(view_);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(file->fileName().section(QLatin1Char('/'), -1));
    setWindowFilePath(file->fileName());
}


void LogWindow::findMonoFont()
{
    QFontDatabase fdb;
    auto families = fdb.families(QFontDatabase::Latin);

    const QStringList candidates{
        QS("Cascadia Code"),
        QS("Cascadia Mono"),
        QS("Roboto Mono"),
        QS("Consolas"),
    };

    for (const auto &candidate : candidates)
    {
        if (families.contains(candidate))
        {
            mono_ = QFont(candidate, 10);
            return;
        }
    }

    mono_ = QFontDatabase::systemFont(QFontDatabase::FixedFont);
}


void LogWindow::readIfAtEnd()
{
    if (!model_->checkTruncated())
    {
        const auto *bar = view_->verticalScrollBar();

        if (bar->value() == bar->maximum())
        {
            readToEnd();
        }
    }
}


void LogWindow::readToEnd()
{
    model_->readToEnd();
    view_->scrollToBottom();
}


void LogWindow::setLevel(LogLevel level)
{
    proxy_->setLevel(level);
}


void LogWindow::closeEvent(QCloseEvent *e)
{
    QMdiSubWindow::closeEvent(e);
    emit closed(windowFilePath());
}
