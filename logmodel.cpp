/*! ***********************************************************************************************
 *
 * \file        logmodel.cpp
 * \brief       LogModel 类源文件。
 *
 * \version     0.1
 * \date        2023-02-02
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2020~2023 karoyqiu。
 *
 **************************************************************************************************/
#include "logmodel.h"

const QBrush LogModel::blue(QColor(97, 175, 239));
const QBrush LogModel::yellow(QColor(229, 192, 123));
const QBrush LogModel::red(QColor(224, 108, 117));
const QBrush LogModel::gray(Qt::gray);

int LogModel::batchSize = 128;


LogModel::~LogModel()
{
    delete stream_;
}


void LogModel::setStream(QTextStream *value)
{
    if (!entries_.isEmpty())
    {
        beginRemoveRows({}, 0, entries_.size() - 1);
        entries_.clear();
        endRemoveRows();
    }

    delete stream_;
    stream_ = value;

    if (stream_ != nullptr)
    {
        readMore(batchSize);
    }
}


int LogModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return entries_.count();
}


bool LogModel::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid() || stream_ == nullptr)
    {
        return false;
    }

    return !stream_->atEnd();
}


void LogModel::fetchMore(const QModelIndex &parent)
{
    if (!parent.isValid() && stream_ != nullptr)
    {
        readMore(batchSize);
    }
}


QString LogModel::levelToString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Debug:
        return tr("Debug");
    case LogLevel::Info:
        return tr("Info");
    case LogLevel::Warning:
        return tr("Warning");
    case LogLevel::Critical:
        return tr("Error");
    case LogLevel::Fatal:
        return tr("Fatal");
    case LogLevel::Unknown:
    default:
        return tr("<Unknown>");
    }
}


LogLevel LogModel::levelFromString(const QString &s)
{
    auto lower = s.toLower();

    if (lower == QL("debug") || lower == QL("dbg"))
    {
        return LogLevel::Debug;
    }

    if (lower == QL("info"))
    {
        return LogLevel::Info;
    }

    if (lower == QL("warning") || lower == QL("warn"))
    {
        return LogLevel::Warning;
    }

    if (lower == QL("critical") || lower == QL("error"))
    {
        return LogLevel::Critical;
    }

    if (lower == QL("fatal"))
    {
        return LogLevel::Fatal;
    }

    return LogLevel::Unknown;
}
