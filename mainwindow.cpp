#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "icyfiremodel.h"
#include "logleveldelegate.h"
#include "logwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , comboLevel_(nullptr)
    , comboModule_(nullptr)
    , readTimer_(nullptr)
{
    ui->setupUi(this);

    comboLevel_ = new QComboBox(this);
    comboLevel_->setToolTip(tr("Level"));
    comboLevel_->addItem(tr("<All>"));
    comboLevel_->addItem(LogModel::levelToString(LogLevel::Debug));
    comboLevel_->addItem(LogModel::levelToString(LogLevel::Info));
    comboLevel_->addItem(LogModel::levelToString(LogLevel::Warning));
    comboLevel_->addItem(LogModel::levelToString(LogLevel::Critical));
    comboLevel_->addItem(LogModel::levelToString(LogLevel::Fatal));
    ui->toolBarMain->addSeparator();
    ui->toolBarMain->addWidget(comboLevel_);

    LogWindow::findMonoFont();

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionGoToEnd, &QAction::triggered, this, &MainWindow::goToEnd);

    QSettings settings;
    restoreGeometry(settings.value(QS("geo")).toByteArray());
    restoreState(settings.value(QS("state")).toByteArray());

    QTimer::singleShot(0, this, &MainWindow::loadLastSession);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *e)
{
    QSettings settings;
    settings.setValue(QS("geo"), saveGeometry());
    settings.setValue(QS("state"), saveState());
    settings.setValue(QS("filenames"), QStringList{ windows_.keys() });

    QMainWindow::closeEvent(e);
}


void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    const auto *mime = e->mimeData();

    if (mime->hasUrls())
    {
        for (const auto &url : mime->urls())
        {
            if (!url.isLocalFile())
            {
                return;
            }
        }

        e->acceptProposedAction();
    }
}


void MainWindow::dropEvent(QDropEvent *e)
{
    const auto *mime = e->mimeData();
    Q_ASSERT(mime->hasUrls());

    for (const auto &url : mime->urls())
    {
        Q_ASSERT(url.isLocalFile());
        watch(url.toLocalFile());
    }

    e->acceptProposedAction();
}


void MainWindow::loadLastSession()
{
    QSettings settings;
    const auto filenames = settings.value(QS("filenames")).toStringList();

    for (const auto &filename : filenames)
    {
        watch(filename);
    }
}


void MainWindow::openFile()
{
    auto filename = QFileDialog::getOpenFileName(this, {}, {}, tr("Log files (*.log);;All files (*)"));

    if (!filename.isEmpty())
    {
        watch(filename);
    }
}


void MainWindow::watch(const QString &filename)
{
    Q_ASSERT(!filename.isEmpty());

    if (windows_.contains(filename))
    {
        auto *window = windows_.value(filename);
        Q_ASSERT(window != nullptr);
        ui->mdiArea->setActiveSubWindow(window);
        return;
    }

    auto *file = new QFile(filename);

    if (file->open(QFile::ReadOnly | QFile::Text))
    {
        auto *window = new LogWindow(file, this);
        window->setLevel(static_cast<LogLevel>(comboLevel_->currentIndex()));
        connect(window, &LogWindow::closed, this, &MainWindow::unwatch);
        connect(comboLevel_, qOverload<int>(&QComboBox::currentIndexChanged), window, [window](int value) {
            window->setLevel(static_cast<LogLevel>(value));
            }
        );
        ui->mdiArea->addSubWindow(window);
        windows_.insert(filename, window);
        window->showMaximized();

        if (Q_UNLIKELY(readTimer_ == nullptr))
        {
            readTimer_ = new QTimer(this);
            connect(readTimer_, &QTimer::timeout, this, &MainWindow::readCurrent);
            readTimer_->start(250);
        }
    }
    else
    {
        QMessageBox::critical(this, {}, tr("Failed to open file %1: %2").arg(QDir::toNativeSeparators(filename), file->errorString()));
        delete file;
    }
}


void MainWindow::readCurrent()
{
    auto *window = qobject_cast<LogWindow *>(ui->mdiArea->currentSubWindow());

    if (window != nullptr)
    {
        window->readIfAtEnd();
    }
}


void MainWindow::unwatch(const QString &filename)
{
    windows_.remove(filename);
}


void MainWindow::goToEnd()
{
    auto *window = qobject_cast<LogWindow *>(ui->mdiArea->currentSubWindow());

    if (window != nullptr)
    {
        window->readToEnd();
    }
}
