/*! ***********************************************************************************************
 *
 * \file        logmodel.h
 * \brief       LogModel 类头文件。
 *
 * \version     0.1
 * \date        2023-02-02
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2020~2023 karoyqiu。
 *
 **************************************************************************************************/
#pragma once
#include <QAbstractTableModel>

class QTextStream;

enum class LogLevel
{
    Unknown,
    Debug,
    Info,
    Warning,
    Critical,
    Fatal,
};

struct LogEntry
{
    QDateTime timestamp;
    QString levelString;
    QString module;
    QString message;
    LogLevel level = LogLevel::Unknown;
    int thread = 0;
};


class LogModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit LogModel(QObject *parent = nullptr)
        : QAbstractTableModel(parent)
        , stream_(nullptr)
    {
    }
    virtual ~LogModel();

    virtual void setStream(QTextStream *value);

    virtual int rowCount(const QModelIndex &parent = {}) const override;
    virtual bool canFetchMore(const QModelIndex &parent) const override;
    virtual void fetchMore(const QModelIndex &parent) override;

    virtual void readToEnd() { readMore(std::numeric_limits<int>::max()); }

    const LogEntry &entryAt(int index) const { return entries_.at(index); }

    static QString levelToString(LogLevel level);

protected:
    static LogLevel levelFromString(const QString &s);

    virtual void readMore(int lines) = 0;

protected:
    static const QBrush blue;
    static const QBrush yellow;
    static const QBrush red;
    static const QBrush gray;
    static int batchSize;

    QTextStream *stream_;
    QVector<LogEntry> entries_;
};
