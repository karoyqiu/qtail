#pragma once
#include <QByteArray>
#include <QString>

#define QB(x)   QByteArrayLiteral(x)
#define QS(x)   QStringLiteral(x)


inline QLatin1String QL(const char *s)
{
    return QLatin1String(s);
}

inline QLatin1Char QL(char ch)
{
    return QLatin1Char(ch);
}
