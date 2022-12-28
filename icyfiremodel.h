﻿/*! ***********************************************************************************************
 *
 * \file        icyfiremodel.h
 * \brief       IcyfireModel 类头文件。
 *
 * \version     0.1
 * \date        2022-12-28
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2022 开心网络。
 *
 **************************************************************************************************/
#pragma once
#include <QAbstractTableModel>

class QTextStream;


class IcyfireModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        ThreadColumn,
        TimestampColumn,
        LevelColumn,
        ModuleColumn,
        MessageColumn,
        ColumnCount
    };

    explicit IcyfireModel(QObject *parent = nullptr);
    virtual ~IcyfireModel();

    void setStream(QTextStream *value);

    // Header:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    virtual int rowCount(const QModelIndex &parent = {}) const override;
    virtual int columnCount(const QModelIndex &parent = {}) const override;

    virtual QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;

private:
    void readMore();

private:
    struct LogEntry
    {
        QDateTime timestamp;
        QString level;
        QString module;
        QString message;
        int thread = 0;
    };

    QTextStream *stream_;
    QVector<LogEntry> entries_;
};
