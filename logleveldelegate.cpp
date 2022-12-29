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
#include "icyfiremodel.h"


QString LogLevelDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    if (value.type() == QMetaType::Int)
    {
        auto level = static_cast<LogLevel>(value.toInt());
        Q_ASSERT(level != LogLevel::Unknown);

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
        default:
            break;
        }
    }

    return QStyledItemDelegate::displayText(value, locale);
}
