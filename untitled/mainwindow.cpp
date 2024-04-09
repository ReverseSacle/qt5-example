#include "mainwindow.h"

/************************ CustomQButton ************************/
void CustomQButton::enterEvent(QEvent *event)
{
    QPushButton::enterEvent(event);
    setCursor(Qt::PointingHandCursor);
}

void CustomQButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    unsetCursor();
}

CustomQButton::CustomQButton(const QString &text, QWidget *parent)
: QPushButton(text, parent) {}
/************************ CustomQButton ************************/


/************************ MainWindow ************************/
// 创建窗口布局
void MainWindow::createPage()
{
    string configFile = "_config.yml";

    ifstream file(configFile);
    if(!file.is_open())
    {
        QMessageBox::information(
            nullptr,
            "通知",
            "当前根目录中没有_config.yml文件\0"
        );
        return;
    }
    file.close();

//    this->resize(1920,1080); // 修改窗口大小
    YAML::Node config = YAML::LoadFile(configFile);

    if(!config["websites"])
    {
        QMessageBox::information(
            nullptr,
            "通知",
            "_config.yml文件中没有websites键名\0"
        );
        return;
    }

    if(config["websites"].IsNull() && !config["websites"].IsSequence())
    {
        QMessageBox::information(
            nullptr,
            "通知",
            "_config.yml文件中websites键的值为空或格式错误\0"
        );
        return;
    }

    int row_each = col;
    int i = 0;

    for(const auto& pair: config["websites"])
    {
        const QString name = QString::fromStdString(pair.first.as<std::string>());
        const QString url = QString::fromStdString(pair.second.as<std::string>());


//        qDebug() << name << ": " << url;
        CustomQButton* newButton = new CustomQButton(name);
        newButton->setStyleSheet("QPushB");
        connect(newButton,&QPushButton::clicked,[=](){ buttonClickAction(url); });

        layout->addWidget(newButton,i / row_each,i % row_each);
        ++i;
    }
}

// 创建窗口菜单/工具栏
void MainWindow::createStatusBar()
{
/*********************************** 菜单栏 ***********************************/
//    // 创建一个菜单栏
//    menuBar = this->menuBar();

//    // 创建一个菜单选项
//    QMenu* fileMenu = menuBar->addMenu(tr("File"));

//    // 在菜单选项中添加一些菜单选项组件
//    QAction* configAction = new QAction("ConfigFile",this);
//    fileMenu->addAction(configAction);
//    connect(configAction,&QAction::triggered,this,&MainWindow::handleConfigAction);

//    // 直接点击菜单就触发事件
//    QAction* configMenu = menuBar->addAction(tr("配置"));
//    connect(configMenu,&QAction::triggered,this,&MainWindow::handleConfigAction);

//    QAction* freshMenu = menuBar->addAction(tr("刷新"));
//    connect(freshMenu,&QAction::triggered,this,&MainWindow::handleFreshAction);

//    QAction* homePageMenu = menuBar->addAction(tr("主页"));
//    connect(homePageMenu,&QAction::triggered,this,&MainWindow::handleHomePageAction);

/*********************************** 菜单栏 ***********************************/

/*********************************** 工具栏 ***********************************/
    toolBar = new QToolBar();
    this->addToolBar(Qt::TopToolBarArea,toolBar);

    QAction* configMenu = toolBar->addAction(tr("配置"));
    connect(configMenu,&QAction::triggered,this,&MainWindow::handleConfigAction);

    QAction* freshMenu = toolBar->addAction(tr("刷新"));

    QAction* homePageMenu = toolBar->addAction(tr("主页"));
    connect(homePageMenu,&QAction::triggered,this,&MainWindow::handleHomePageAction);

    toolBar->setMovable(false); // 固定工具栏，设置为不可移动
/*********************************** 工具栏 ***********************************/
}

QSize MainWindow::sizeHint() const { return QSize(width,height); }

void MainWindow::handleHomePageAction()
{
    if(nullptr != webView)
    {
        webView->close();
        delete webView;
        webView = nullptr;
    }

    centralWidget = new QWidget(this);
    layout = new QGridLayout(centralWidget);

    createPage();

    /**
     * connect()函数会累积绑定的函数，
     * 每一次调用会调用累积的函数，
     * 一个函数可以重复累积，因此
     * 需要先将之前的绑定函数取消
    **/
    disconnect(fresh_connection);
    fresh_connection = connect(
        toolBar->actions().at(1),
        &QAction::triggered,
        this,
        &MainWindow::handleFreshAction
    );
    layout->setAlignment(Qt::AlignTop);
    this->setCentralWidget(centralWidget);

}

void MainWindow::handleConfigAction()
{
//    // 获取当前目录路径
//    QString currentPath = QDir::currentPath();

//    // 打印当前目录路径
//    qDebug() << "Current directory:" << currentPath;

//    // 拼接路径
//    QString configFile = QDir(currentPath).filePath("_config.yml");

    QString configFile = "_config.yml";
    QProcess::startDetached(
        "cmd",
        QStringList() << "/c" << "start" << "" << configFile
    );
}

// 刷新界面，重绘
void MainWindow::handleFreshAction() { this->update(); }

void MainWindow::buttonClickAction(const QString& url)
{
//    qDebug() << url;
    if(nullptr == webView)
    {
        webView = new QWebEngineView(this);

        disconnect(fresh_connection);
        fresh_connection = connect(
            toolBar->actions().at(1),
            &QAction::triggered,
            this,
            [=](){
            webView->reload();
        });

//        webView->setZoomFactor(1); // 手动放到页面大小
        this->setCentralWidget(webView);
    }

    webView->setUrl(QUrl(url));
}

MainWindow::MainWindow(int x,int y,int z)
: width(x),height(y),col(z),webView(nullptr)
{
    createStatusBar();

//    /**
//     * 不能直接在QMainWindow上设置QLayout。
//     * 需要创建一个QWidget并将其设置为QMainWindow上的中央小部件，
//     * 并将QLayout分配给它
//    **/
//    centralWidget = new QWidget(this);
//    layout = new QHBoxLayout (centralWidget);
//    createPage();

//    layout->setAlignment(Qt::AlignTop); // 移动到顶部
//    this->setCentralWidget(centralWidget);

    handleHomePageAction();
}
/************************ MainWindow ************************/


