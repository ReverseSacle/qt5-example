#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define __DEBUG__
#ifdef __DEBUG__
#include <iostream>
#endif

#include <fstream>
#include <String>
#include <QtWidgets>
#include <QWebEngineView>
#include <QWebEngineSettings>

#define YAML_CPP_STATIC_DEFINE
#include "yaml-cpp/yaml.h"

using std::ifstream;
using std::string;
using namespace YAML;

/**
 * 通过自定义QPushButton,
 * 实现鼠标进入时显示鼠标小手，
 * 离开时恢复原始鼠标样式
**/
class CustomQButton: public QPushButton
{
protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

public:
    CustomQButton(const QString &text, QWidget *parent = nullptr);
};

class MyWebEngineView : public QWebEngineView
{
public:
    MyWebEngineView(QWidget *parent = nullptr);

protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override {
        Q_UNUSED(type);
        qDebug() << "createWindow";
        MyWebEngineView *newView = new MyWebEngineView();
        newView->show();
        return newView;
    }
};

/**
 * 自定义QMainWindow，
 * 而不是通过`.ui`文件来设计
**/
class MainWindow: public QMainWindow
{
    Q_OBJECT

private:
    int width;
    int height;
    int col;
    QGridLayout* layout; // 界面整体水平布局
    QWidget* centralWidget; // 界面中心画面布局
    QWebEngineView* webView; // 打开web网页的Chromium引擎
    QMenuBar* menuBar; // 菜单栏
    QToolBar* toolBar; // 工具栏
    QMetaObject::Connection fresh_connection; // 用于刷新对象的更新，用于组件追踪
    void createPage(); // 创建界面布局
    void createStatusBar(); // 创建界面菜单
    QSize sizeHint() const override; // 固定默认窗口大小

private slots:
    void handleHomePageAction();
    void handleConfigAction(); // 菜单`配置`的动作
    void handleFreshAction(); // 菜单`刷新`的动作
    void buttonClickAction(const QString& url); // 按钮点击的动作

public:
    // 有参构成函数,x => 长, y => 宽, z => 每行多少个按钮
    MainWindow(int x = 1600,int y = 1080,int z = 5);
};


#endif // MAINWINDOW_H
