#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    /**
     * 启用高DPI缩放的应用程序属性设置。
     * 在高分辨率显示屏上，启用高DPI缩放可以
     * 确保Qt应用程序在高DPI环境下正确显示，
     * 并保持界面元素的大小和布局比例，就是保证窗口适应电脑屏幕的缩放比
    **/
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    MainWindow main_window;

    // 导入qss样式
    QFile styleFile(":/new.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text))
    {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
        styleFile.close();
    }

    main_window.show();
    return app.exec();
}
