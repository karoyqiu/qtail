#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "icyfiremodel.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    findMonoFont();

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
}


MainWindow::~MainWindow()
{
    delete ui;
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


void MainWindow::openFile()
{
    auto filename = QFileDialog::getOpenFileName(this, {}, {}, tr("Log files (*.log);;All files (*)"));

    if (filename.isEmpty())
    {
        return;
    }

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

        auto *header = view->horizontalHeader();
        header->setStretchLastSection(true);
        header->resizeSections(QHeaderView::ResizeToContents);

        auto *window = ui->mdiArea->addSubWindow(view);
        window->setWindowTitle(filename.section(QLatin1Char('/'), -1));
        window->setWindowFilePath(filename);
    }
    else
    {
        QMessageBox::critical(this, {}, tr("Failed to open file: %1").arg(file->errorString()));
        delete file;
        delete view;
    }
}
