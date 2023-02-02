/*! ***********************************************************************************************
 *
 * \file        logproxymodel.cpp
 * \brief       LogProxyModel 类源文件。
 *
 * \version     0.1
 * \date        2023-02-02
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2020~2023 karoyqiu。
 *
 **************************************************************************************************/
#include "logproxymodel.h"


LogProxyModel::LogProxyModel(QObject *parent /*= nullptr*/)
    : QSortFilterProxyModel(parent)
    , level_(LogLevel::Unknown)
    , model_(nullptr)
{
}


void LogProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    model_ = qobject_cast<LogModel *>(sourceModel);
    Q_ASSERT(model_ != nullptr);
    QSortFilterProxyModel::setSourceModel(sourceModel);
}


void LogProxyModel::setLevel(LogLevel level)
{
    level_ = level;
    invalidateFilter();
}


bool LogProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (level_ == LogLevel::Unknown)
    {
        return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
    }

    const auto &entry = model_->entryAt(sourceRow);
    return entry.level == level_;
}
