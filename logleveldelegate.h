/*! ***********************************************************************************************
 *
 * \file        logleveldelegate.h
 * \brief       LogLevelDelegate 类头文件。
 *
 * \version     0.1
 * \date        2022-12-29
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2022 开心网络。
 *
 **************************************************************************************************/
#pragma once
#include <QStyledItemDelegate>


class LogLevelDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    using QStyledItemDelegate::QStyledItemDelegate;

    virtual QString displayText(const QVariant &value, const QLocale &locale) const override;
};
