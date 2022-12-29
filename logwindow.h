/*! ***********************************************************************************************
 *
 * \file        logwindow.h
 * \brief       LogWindow 类头文件。
 *
 * \version     0.1
 * \date        2022-12-29
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2022 开心网络。
 *
 **************************************************************************************************/
#pragma once
#include <QMdiSubWindow>

class QTableView;
class IcyfireModel;


class LogWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    LogWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    QTableView *view() const;
    IcyfireModel *model() const;

signals:
    void closed(const QString &filename);

protected:
    virtual void closeEvent(QCloseEvent *e) override;
};
