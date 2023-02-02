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

class QFile;
class QTableView;
class LogModel;
class LogProxyModel;


class LogWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    explicit LogWindow(QFile *file, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    //QTableView *view() const { return view_; }
    //LogModel *model() const { return model_; };

    static void findMonoFont();

    void readIfAtEnd();
    void readToEnd();

signals:
    void closed(const QString &filename);

protected:
    virtual void closeEvent(QCloseEvent *e) override;

private:
    static QFont mono_;

    QTableView *view_;
    LogProxyModel *proxy_;
    LogModel *model_;
};
