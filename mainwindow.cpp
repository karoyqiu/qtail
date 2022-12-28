#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "icyfiremodel.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
}


MainWindow::~MainWindow()
{
    delete ui;
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
        view->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

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
