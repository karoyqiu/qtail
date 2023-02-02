/*! ***********************************************************************************************
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
#include "logmodel.h"


class IcyfireModel : public LogModel
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

    using LogModel::LogModel;

    // Header:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    virtual int columnCount(const QModelIndex &parent = {}) const override;
    virtual QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;

private:
    virtual void readMore(int lines) override;
};
