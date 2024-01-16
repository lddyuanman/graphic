#include<QDockWidget>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QPushButton>
#include "GraphicSys.h"
#include "ui_GraphicSys.h"

GraphicSys::GraphicSys(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	m_strAppPath = QCoreApplication::applicationDirPath();//.exe的目录
	m_strImageSourcesPath = QString("%1/../GraphicSys/Images/1080_1920/").arg(m_strAppPath);  
	m_strCfgFilePath = QString("%1/../public/config/").arg(m_strAppPath);
    initUI();

	m_stCfgInfo = stTableData();//初始化结构体为空
	m_nTabCount = 0;
}



bool GraphicSys::isFileExist(QString strfile)
{
	QFile file(strfile);
	if (file.exists(strfile))
	{
		return true;
	}
	return false;
}

bool GraphicSys::isSaveFile()
{
	QMessageBox msgBox(NULL);
	msgBox.resize(400, 200);
	msgBox.setWindowTitle(QString::fromLocal8Bit("注意"));
	msgBox.setText(QString::fromLocal8Bit("文件已修改，是否保存？"));
	msgBox.setIcon(QMessageBox::Question);
	msgBox.addButton(QString::fromLocal8Bit("确定"), QMessageBox::AcceptRole);
	msgBox.addButton(QString::fromLocal8Bit("取消"), QMessageBox::RejectRole);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	if (ret == QMessageBox::AcceptRole)
	{
		return true;
	}
	else if (ret == QMessageBox::RejectRole)
	{
		return false;
	}
}

void GraphicSys::addTabWgt()
{
	//最多可打开10个页面
	if (m_nTabCount < INT_MAX_TAB_COUNT)
	{
		m_nTabCount++;

		//m_pTabWiget->addTab(new QWidget, "new");
		QWidget* itemwidget = new QWidget();
		m_pTabWiget->addTab(itemwidget, "new file");
		itemwidget->setStyleSheet("background-color:#00e6ff;");
		//m_pTabWiget->tabBar()->setTabButton(0, QTabBar::RightSide, nullptr);	 //第一个tab取消关闭按钮	
	
		adjustTabBarWidth(m_pTabWiget->count());
	}
	else
	{
		QMessageBox::warning(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("打开文件过多，请先关闭，再打开！"));
	}
}

void GraphicSys::saveNewFile(QString strfile)
{
	//写配置文件
	//以当前文件命名
}

void GraphicSys::adjustTabBarWidth(int ncount)
{
	int ntabarwidth = INT_TAB_BAR_WIDTH / ncount;
	m_pTabWiget->setStyleSheet(QString("QTabBar::tab{color: white; background:#5c8dff; border:1px solid #73b7ff; min-width: %1ex;	min-height: 10ex;}").arg(ntabarwidth));
}

void GraphicSys::initUI()
{    
    setWindowState(Qt::WindowMaximized);//默认最大化显示
	initMenuBar();
	initTopToolBar();
	initLeftToolBar();
	initStatusBar();

	ui.m_pCentralWidget->setStyleSheet("background-color:#9d9c97;");
	this->resize(QApplication::desktop()->width(), QApplication::desktop()->height());
	m_pTabWiget = new QTabWidget(ui.m_pCentralWidget);
	m_pTabWiget->setFixedSize(this->width() - INT_LEFT_TOOLBAR_WIDTH,this->height());
	m_pTabWiget->setStyleSheet("background-color:#9e9c97;");
	m_pTabWiget->setTabShape(QTabWidget::Triangular);
	m_pTabWiget->setTabsClosable(true);
	connect(m_pTabWiget, SIGNAL(tabCloseRequested(int)), this, SLOT(slotTabWgtClose(int)));
	//m_pTabWiget->setStyleSheet("QTabBar::tab { margin-right: 20px; margin-left:20px}");
	//m_pTabWiget->hide();
}

void GraphicSys::initMenuBar()
{
	//1、菜单栏  最多只能有一个
	//QMenuBar* bar = menuBar();	
	//setMenuBar(bar);//把栏放入窗口中
	ui.m_pMenuBar->setStyleSheet("background-color:#eeeaec;");

	//QMenu* fileMenu = menuBar()->addMenu(QString::fromLocal8Bit("文件"));//创建菜单
	QMenu* fileMenu = ui.m_pMenuBar->addMenu(QString::fromLocal8Bit("文件"));
	QMenu* editMenu = ui.m_pMenuBar->addMenu(QString::fromLocal8Bit("编辑"));
		
	QAction* newAction = fileMenu->addAction(QString::fromLocal8Bit("新建"));//创建菜单项
	newAction->setIcon(QIcon(QString("%1newIcon.png").arg(m_strImageSourcesPath)));
	fileMenu->addSeparator();//添加分割线
	QAction* openAction = fileMenu->addAction(QString::fromLocal8Bit("打开"));
	openAction->setIcon(QIcon(QString("%1openIcon.png").arg(m_strImageSourcesPath)));

	connect(newAction, &QAction::triggered, this, &GraphicSys::slotActionNewFile);
	connect(openAction, &QAction::triggered, this, &GraphicSys::slotActionOpenFile);
}

void GraphicSys::initTopToolBar()
{
	//2、工具栏 可以有多个
	//顶部工具栏
	ui.m_pMainToolBar->setFloatable(false);
	ui.m_pMainToolBar->setStyleSheet("background-color:#f3eee8;");

	const QIcon newActIcon = QIcon(QString("%1new.png").arg(m_strImageSourcesPath));
	m_pActNewIcon = new QAction(newActIcon, QStringLiteral("新建(&N)"), this);//实例化readAct
	m_pActNewIcon->setStatusTip(QStringLiteral("新建"));//设置鼠标停留显示内容
	m_pActNewIcon->setShortcuts(QKeySequence::Open);//打开快捷键
	connect(m_pActNewIcon, &QAction::triggered, this, &GraphicSys::slotActionNewFile);//绑定动作执行内容

	const QIcon openActIcon = QIcon(QString("%1open.png").arg(m_strImageSourcesPath));
	m_pActOpenIcon = new QAction(openActIcon, QStringLiteral("打开(&O)"), this);//实例化readAct
	m_pActOpenIcon->setStatusTip(QStringLiteral("打开"));//设置鼠标停留显示内容
	m_pActOpenIcon->setShortcuts(QKeySequence::Open);//打开快捷键
	connect(m_pActOpenIcon, &QAction::triggered, this, &GraphicSys::slotActionOpenFile);//绑定动作执行内容

	const QIcon saveActIcon = QIcon(QString("%1save.png").arg(m_strImageSourcesPath));
	m_pActSaveIcon = new QAction(saveActIcon, QStringLiteral("保存(&S)"), this);//实例化readAct
	m_pActSaveIcon->setStatusTip(QStringLiteral("保存"));//设置鼠标停留显示内容
	m_pActSaveIcon->setShortcuts(QKeySequence::Open);//打开快捷键
	connect(m_pActSaveIcon, &QAction::triggered, this, &GraphicSys::slotActionSaveFile);//绑定动作执行内容

	//工具栏中放入小部件
	ui.m_pMainToolBar->addAction(m_pActNewIcon);	
	ui.m_pMainToolBar->addAction(m_pActOpenIcon);
	ui.m_pMainToolBar->addAction(m_pActSaveIcon);

	ui.m_pMainToolBar->addSeparator();	//添加分割线
}

void GraphicSys::initLeftToolBar()
{
	//左侧工具栏
	m_pLeftToolBar = new QToolBar(QStringLiteral("左侧工具栏"),this);
	addToolBar(Qt::LeftToolBarArea, m_pLeftToolBar);
	m_pLeftToolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);//设置只允许左右停靠
    m_pLeftToolBar->setFloatable(false);//设置浮动
	m_pLeftToolBar->setStyleSheet("background-color:#f3eee8;");

	const QIcon recportActIcon = QIcon(QString("%1report.png").arg(m_strImageSourcesPath));;
	m_pActReport = new QAction(recportActIcon, QStringLiteral("智能报表(&R)"), this);//实例化readAct
	m_pActReport->setStatusTip(QStringLiteral("智能报表"));//设置鼠标停留显示内容
	m_pActReport->setShortcuts(QKeySequence::Open);//打开快捷键
	//connect(m_pActReport, &QAction::triggered, this, &MainWindow::ReadFile);//绑定动作执行内容
		
	const QIcon curveActIcon = QIcon(QString("%1curve.png").arg(m_strImageSourcesPath));
	m_pActCurve = new QAction(curveActIcon, QStringLiteral("曲线(&C)"), this);//实例化readAct
	m_pActCurve->setStatusTip(QStringLiteral("曲线"));//设置鼠标停留显示内容
	m_pActCurve->setShortcuts(QKeySequence::Open);//打开快捷键
	//connect(m_pActReport, &QAction::triggered, this, &MainWindow::ReadFile);//绑定动作执行内容

	//工具栏中放入小部件
	m_pLeftToolBar->addAction(m_pActReport);	
	m_pLeftToolBar->addSeparator();//添加分割线
	m_pLeftToolBar->addAction(m_pActCurve);
}

void GraphicSys::initStatusBar()
{
	//3、状态栏 只能有一个
	//QStatusBar* stBar = statusBar();
	//setStatusBar(stBar);

	QLabel* label1 = new QLabel(QString::fromLocal8Bit("左侧提示信息"), this);
	ui.m_pStatusBar->addWidget(label1);

	QLabel* label2 = new QLabel(QString::fromLocal8Bit("右侧提示信息"), this);
	ui.m_pStatusBar->addPermanentWidget(label2);
}

void GraphicSys::initFloatableWgt()
{
	//4、铆接部件(浮动窗口)  可以有多个
	QDockWidget* dock = new QDockWidget("aaa", this);
	addDockWidget(Qt::TopDockWidgetArea, dock);
	//只允许左右停靠
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);	
}

void GraphicSys::slotActionOpenFile()
{
	if (m_strCfgFilePath == NULL)
	{
		return;
	}

	QFileInfo fileInfo(m_strCfgFilePath);
	QFileDialog* pFilename = new QFileDialog(this, Qt::Dialog);
	//pFilename->setGeometry((QApplication::desktop()->width() - 800) / 2, (QApplication::desktop()->height() - 480) / 2, 800, 480);//未起作用
	pFilename->setDirectory(fileInfo.absolutePath());//设置打开的默认目录
	//pFilename->setNameFilter(tr("exe(*.xls)"));//设置限定文件类型
	//pFilename->setOption(QFileDialog::DontResolveSymlinks);//可以加载链接路径而不会跳转

	if (pFilename->exec() != QFileDialog::Accepted)
	{
		return;
	}

	QString strFileName = pFilename->selectedFiles().first();

	delete pFilename;

	if (strFileName.isEmpty())
	{
		//QMessageBox::information(this, "Error Message", "No File Selected");
		return;
	}
	//QDesktopServices::openUrl(QUrl::fromLocalFile(strFileName));
	m_jsonProcess.getCfgInfo(m_stCfgInfo, strFileName);//获取窗口初始化各值m_stCfgInfo
	//根据m_stCfgInfo新建
}

void GraphicSys::slotActionSaveFile()
{
	//保存
	if (m_pTabWiget->count() <= 0)
	{
		return;
	}
	QString strfile = QString("%1/%2").arg(m_strCfgFilePath).arg(m_pTabWiget->currentWidget()->windowTitle());
	QFile file(strfile);
	if (isSaveFile())
	{
		if (isFileExist(strfile))
		{
			file.remove(strfile);//删除旧的文件
		}
		saveNewFile(strfile);
	}
	else
	{
		return;
	}
}

void GraphicSys::slotTabWgtClose(int nindex)
{
	//QTabWidget添加打开功能时针对一个tab操作，而关闭功能是针对tab所有的tab实现的
	if (m_pTabWiget->widget(nindex) == NULL )
	{
		return;
	}
	//处理新建文档修改后保存提示????
	/*if (!isSaveFile())
	{
		return;
	}*/

	QString strfile = QString("%1/%2").arg(m_strCfgFilePath).arg(m_pTabWiget->currentWidget()->windowTitle());
	saveNewFile(strfile);
	
	//m_pTabWiget->removeTab(nindex);	
	QWidget* tabitem = m_pTabWiget->widget(nindex);
	tabitem->disconnect();
	m_pTabWiget->removeTab(nindex);
	delete tabitem;
	tabitem = NULL;

	adjustTabBarWidth(m_pTabWiget->count());
}

void GraphicSys::slotActionNewFile()
{
	addTabWgt();
}