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
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;

private:
    void findMonoFont();
    void loadLastSession();
    void openFile();
    void watch(const QString &filename);
    void readFile(const QString &filename);
    void readWindow(LogWindow *window);
    void readAll();
    void unwatch(const QString &filename);
    void goToEnd();

private:
    Ui::MainWindow *ui;
    QComboBox *comboLevel_;
    QComboBox *comboModule_;
    QFileSystemWatcher *watcher_;
    QTimer *readTimer_;
    QMap<QString, LogWindow *> windows_;
    QFont mono_;
};
