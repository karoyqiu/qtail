#include "stable.h"
#include "mainwindow.h"


static void checkDarkMode(QApplication &app)
{
#ifdef Q_OS_WIN
    QSettings settings(QS(R"(HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)"), QSettings::NativeFormat);

    if (settings.value("AppsUseLightTheme") == 0)
    {
        app.setStyle(QStyleFactory::create(QS("Fusion")));

        QPalette darkPalette;
        QColor darkColor(45, 45, 45);
        QColor disabledColor(127, 127, 127);
        QColor textColor(204, 204, 204);
        darkPalette.setColor(QPalette::Window, darkColor);
        darkPalette.setColor(QPalette::WindowText, textColor);
        darkPalette.setColor(QPalette::Base, QColor(18, 18, 18));
        darkPalette.setColor(QPalette::AlternateBase, darkColor);
        darkPalette.setColor(QPalette::ToolTipBase, textColor);
        darkPalette.setColor(QPalette::ToolTipText, textColor);
        darkPalette.setColor(QPalette::Text, textColor);
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
        darkPalette.setColor(QPalette::Button, darkColor);
        darkPalette.setColor(QPalette::ButtonText, textColor);
        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);

        app.setPalette(darkPalette);

        app.setStyleSheet(QS("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }"));
    }
#else
    Q_UNUSED(app);
#endif
}


int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

    QApplication a(argc, argv);
    QApplication::setApplicationName(QS("qtail"));
    QApplication::setOrganizationName(QS("karoyqiu"));
    QApplication::setOrganizationDomain(QS("karoyqiu.gmail.com"));
    QApplication::setApplicationDisplayName(QApplication::translate("main", "qtail"));

    checkDarkMode(a);
    QIcon::setThemeName(QS("elementary"));

    MainWindow w;
    w.show();

    return QApplication::exec();
}
