/*! ***********************************************************************************************
 *
 * \file        logleveldelegate.cpp
 * \brief       LogLevelDelegate 类源文件。
 *
 * \version     0.1
 * \date        2022-12-29
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2022 开心网络。
 *
 **************************************************************************************************/
#include "logleveldelegate.h"
#include "logmodel.h"


QString LogLevelDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    if (value.type() == QMetaType::Int)
    {
        auto level = static_cast<LogLevel>(value.toInt());
        Q_ASSERT(level != LogLevel::Unknown);
        return LogModel::levelToString(level);
    }

    return QStyledItemDelegate::displayText(value, locale);
}
