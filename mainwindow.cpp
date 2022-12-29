#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "icyfiremodel.h"
#include "logleveldelegate.h"
#include "logwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , watcher_(nullptr)
{
    ui->setupUi(this);

    watcher_ = new QFileSystemWatcher(this);
    connect(watcher_, &QFileSystemWatcher::fileChanged, this, &MainWindow::readMore);

    findMonoFont();

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);

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


void MainWindow::findMonoFont()
{
    QFontDatabase fdb;
    auto families = fdb.families(QFontDatabase::Latin);

    const QStringList candidates{
        QS("Cascadia Code"),
        QS("Cascadia Mono"),
        QS("Roboto Mono"),
        QS("Consolas"),
    };

    for (const auto &candidate : candidates)
    {
        if (families.contains(candidate))
        {
            mono_ = QFont(candidate, 10);
            return;
        }
    }

    mono_ = QFontDatabase::systemFont(QFontDatabase::FixedFont);
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
    auto *view = new QTableView(this);
    auto *file = new QFile(filename, view);

    if (file->open(QFile::ReadOnly | QFile::Text))
    {
        view->setFont(mono_);

        auto *stream = new QTextStream(file);
        stream->setCodec("UTF-8");
        stream->setAutoDetectUnicode(true);

        auto *model = new IcyfireModel(view);
        model->setStream(stream);
        view->setModel(model);

        auto *dlgt = new LogLevelDelegate(view);
        view->setItemDelegateForColumn(IcyfireModel::LevelColumn, dlgt);

        auto *header = view->horizontalHeader();
        header->setStretchLastSection(true);
        header->resizeSections(QHeaderView::ResizeToContents);

        auto *window = new LogWindow(this);
        window->setWidget(view);
        window->setWindowTitle(filename.section(QLatin1Char('/'), -1));
        window->setWindowFilePath(filename);
        connect(window, &LogWindow::closed, this, &MainWindow::unwatch);
        ui->mdiArea->addSubWindow(window);
        windows_.insert(filename, window);
        window->showMaximized();

        if (!watcher_->addPath(filename))
        {
            qWarning() << "Failed to watch" << filename;
        }
    }
    else
    {
        QMessageBox::critical(this, {}, tr("Failed to open file %1: %2").arg(QDir::toNativeSeparators(filename), file->errorString()));
        delete file;
        delete view;
    }
}


void MainWindow::readMore(const QString &filename)
{
    auto *window = windows_.value(filename);
    auto *view = window->view();
    auto *bar = view->verticalScrollBar();
    auto atEnd = bar->value() == bar->maximum();
    auto *model = window->model();
    model->readMore();

    if (atEnd)
    {
        view->scrollToBottom();
    }
}


void MainWindow::unwatch(const QString &filename)
{
    watcher_->removePath(filename);
    windows_.remove(filename);
}
