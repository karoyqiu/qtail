/*! ***********************************************************************************************
 *
 * \file        icyfiremodel.cpp
 * \brief       IcyfireModel 类源文件。
 *
 * \version     0.1
 * \date        2022-12-28
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2022 开心网络。
 *
 **************************************************************************************************/
#include "icyfiremodel.h"


static LogLevel levelFromString(const QString &s)
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


IcyfireModel::IcyfireModel(QObject *parent)
    : QAbstractTableModel(parent)
    , stream_(nullptr)
{
}


IcyfireModel::~IcyfireModel()
{
    delete stream_;
}


void IcyfireModel::setStream(QTextStream *value)
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
        readMore();
    }
}


QVariant IcyfireModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant var;

    if (orientation == Qt::Horizontal && (role == Qt::DisplayRole || role == Qt::ToolTipRole))
    {
        switch (section)
        {
        case ThreadColumn:
            var = tr("Thread");
            break;
        case TimestampColumn:
            var = tr("Timestamp");
            break;
        case LevelColumn:
            var = tr("Level");
            break;
        case ModuleColumn:
            var = tr("Module");
            break;
        case MessageColumn:
            var = tr("Message");
            break;
        default:
            break;
        }
    }
    else
    {
        var = QAbstractTableModel::headerData(section, orientation, role);
    }

    return var;
}


int IcyfireModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return entries_.size();
}


int IcyfireModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return ColumnCount;
}


QVariant IcyfireModel::data(const QModelIndex &idx, int role) const
{
    Q_ASSERT(checkIndex(idx, CheckIndexOption::IndexIsValid | CheckIndexOption::ParentIsInvalid));

    QVariant var;
    const auto &entry = entries_.at(idx.row());

    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
    {
        switch (idx.column())
        {
        case ThreadColumn:
            var = entry.thread;
            break;
        case TimestampColumn:
            var = entry.timestamp;
            break;
        case LevelColumn:
            if (entry.level == LogLevel::Unknown)
            {
                var = entry.levelString;
            }
            else
            {
                var = static_cast<int>(entry.level);
            }
            break;
        case ModuleColumn:
            var = entry.module;
            break;
        case MessageColumn:
            var = entry.message;
            break;
        default:
            break;
        }
    }
    else if (role == Qt::ForegroundRole)
    {
        static const QBrush blue(QColor(97, 175, 239));
        static const QBrush yellow(QColor(229, 192, 123));
        static const QBrush red(QColor(224, 108, 117));
        static const QBrush gray(Qt::gray);

        switch (entry.level)
        {
        case LogLevel::Debug:
            var = gray;
            break;
        case LogLevel::Info:
            var = blue;
            break;
        case LogLevel::Warning:
            var = yellow;
            break;
        case LogLevel::Critical:
        case LogLevel::Fatal:
            var = red;
            break;
        default:
            break;
        }
    }

    return var;
}


bool IcyfireModel::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid() || stream_ == nullptr)
    {
        return false;
    }

    return !stream_->atEnd();
}


void IcyfireModel::fetchMore(const QModelIndex &parent)
{
    if (!parent.isValid() && stream_ != nullptr)
    {
        readMore();
    }
}


void IcyfireModel::readMore()
{
    Q_ASSERT(stream_ != nullptr);
    static const QRegularExpression regexp(QS(R"(\[(\d+);(\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}.\d{3}),(\w+)]\s(\[[\w.]+]\s|<[\w.]+>\s)?(.+))"));

    int count = 0;

    for (; !stream_->atEnd() && count < 128; count++)
    {
        auto line = stream_->readLine();
        auto result = regexp.match(line);
        LogEntry entry;

        if (result.hasMatch())
        {
            entry.thread = result.captured(1).toInt();
            entry.timestamp = QDateTime::fromString(result.captured(2), Qt::ISODateWithMs);
            entry.levelString = result.captured(3);
            entry.level = levelFromString(entry.levelString);
            entry.module = result.captured(4);

            if (entry.level != LogLevel::Unknown)
            {
                entry.levelString.clear();
            }

            if (!entry.module.isEmpty())
            {
                entry.module.remove(0, 1).chop(2);
            }

            entry.message = result.captured(5);
        }
        else
        {
            entry.message = line;
        }

        entries_.append(entry);
    }

    if (count > 0)
    {
        beginInsertRows({}, entries_.size(), entries_.size() + count - 1);
        endInsertRows();
    }
}
