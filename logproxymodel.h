/*! ***********************************************************************************************
 *
 * \file        logproxymodel.h
 * \brief       LogProxyModel 类头文件。
 *
 * \version     0.1
 * \date        2023-02-02
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2020~2023 karoyqiu。
 *
 **************************************************************************************************/
#pragma once
#include <QSortFilterProxyModel>

#include "logmodel.h"


class LogProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit LogProxyModel(QObject *parent = nullptr);

    virtual void setSourceModel(QAbstractItemModel *sourceModel) override;

    void setLevel(LogLevel level);

protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    LogModel *model_;
    LogLevel level_;
};
