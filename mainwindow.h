#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LogWindow;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent *e) override;

private:
    void findMonoFont();
    void loadLastSession();
    void openFile();
    void watch(const QString &filename);
    void readMore(const QString &filename);
    void unwatch(const QString &filename);

private:
    Ui::MainWindow *ui;
    QFileSystemWatcher *watcher_;
    QMap<QString, LogWindow *> windows_;
    QFont mono_;
};
