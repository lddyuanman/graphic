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

	m_strAppPath = QCoreApplication::applicationDirPath();//.exe��Ŀ¼
	m_strImageSourcesPath = QString("%1/../GraphicSys/Images/1080_1920/").arg(m_strAppPath);  
	m_strCfgFilePath = QString("%1/../public/config/").arg(m_strAppPath);
    initUI();

	m_stCfgInfo = stTableData();//��ʼ���ṹ��Ϊ��
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
	msgBox.setWindowTitle(QString::fromLocal8Bit("ע��"));
	msgBox.setText(QString::fromLocal8Bit("�ļ����޸ģ��Ƿ񱣴棿"));
	msgBox.setIcon(QMessageBox::Question);
	msgBox.addButton(QString::fromLocal8Bit("ȷ��"), QMessageBox::AcceptRole);
	msgBox.addButton(QString::fromLocal8Bit("ȡ��"), QMessageBox::RejectRole);
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
	//���ɴ�10��ҳ��
	if (m_nTabCount < INT_MAX_TAB_COUNT)
	{
		m_nTabCount++;

		//m_pTabWiget->addTab(new QWidget, "new");
		QWidget* itemwidget = new QWidget();
		m_pTabWiget->addTab(itemwidget, "new file");
		itemwidget->setStyleSheet("background-color:#00e6ff;");
		//m_pTabWiget->tabBar()->setTabButton(0, QTabBar::RightSide, nullptr);	 //��һ��tabȡ���رհ�ť	
	
		adjustTabBarWidth(m_pTabWiget->count());
	}
	else
	{
		QMessageBox::warning(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ļ����࣬���ȹرգ��ٴ򿪣�"));
	}
}

void GraphicSys::saveNewFile(QString strfile)
{
	//д�����ļ�
	//�Ե�ǰ�ļ�����
}

void GraphicSys::adjustTabBarWidth(int ncount)
{
	int ntabarwidth = INT_TAB_BAR_WIDTH / ncount;
	m_pTabWiget->setStyleSheet(QString("QTabBar::tab{color: white; background:#5c8dff; border:1px solid #73b7ff; min-width: %1ex;	min-height: 10ex;}").arg(ntabarwidth));
}

void GraphicSys::initUI()
{    
    setWindowState(Qt::WindowMaximized);//Ĭ�������ʾ
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
	//1���˵���  ���ֻ����һ��
	//QMenuBar* bar = menuBar();	
	//setMenuBar(bar);//�������봰����
	ui.m_pMenuBar->setStyleSheet("background-color:#eeeaec;");

	//QMenu* fileMenu = menuBar()->addMenu(QString::fromLocal8Bit("�ļ�"));//�����˵�
	QMenu* fileMenu = ui.m_pMenuBar->addMenu(QString::fromLocal8Bit("�ļ�"));
	QMenu* editMenu = ui.m_pMenuBar->addMenu(QString::fromLocal8Bit("�༭"));
		
	QAction* newAction = fileMenu->addAction(QString::fromLocal8Bit("�½�"));//�����˵���
	newAction->setIcon(QIcon(QString("%1newIcon.png").arg(m_strImageSourcesPath)));
	fileMenu->addSeparator();//��ӷָ���
	QAction* openAction = fileMenu->addAction(QString::fromLocal8Bit("��"));
	openAction->setIcon(QIcon(QString("%1openIcon.png").arg(m_strImageSourcesPath)));

	connect(newAction, &QAction::triggered, this, &GraphicSys::slotActionNewFile);
	connect(openAction, &QAction::triggered, this, &GraphicSys::slotActionOpenFile);
}

void GraphicSys::initTopToolBar()
{
	//2�������� �����ж��
	//����������
	ui.m_pMainToolBar->setFloatable(false);
	ui.m_pMainToolBar->setStyleSheet("background-color:#f3eee8;");

	const QIcon newActIcon = QIcon(QString("%1new.png").arg(m_strImageSourcesPath));
	m_pActNewIcon = new QAction(newActIcon, QStringLiteral("�½�(&N)"), this);//ʵ����readAct
	m_pActNewIcon->setStatusTip(QStringLiteral("�½�"));//�������ͣ����ʾ����
	m_pActNewIcon->setShortcuts(QKeySequence::Open);//�򿪿�ݼ�
	connect(m_pActNewIcon, &QAction::triggered, this, &GraphicSys::slotActionNewFile);//�󶨶���ִ������

	const QIcon openActIcon = QIcon(QString("%1open.png").arg(m_strImageSourcesPath));
	m_pActOpenIcon = new QAction(openActIcon, QStringLiteral("��(&O)"), this);//ʵ����readAct
	m_pActOpenIcon->setStatusTip(QStringLiteral("��"));//�������ͣ����ʾ����
	m_pActOpenIcon->setShortcuts(QKeySequence::Open);//�򿪿�ݼ�
	connect(m_pActOpenIcon, &QAction::triggered, this, &GraphicSys::slotActionOpenFile);//�󶨶���ִ������

	const QIcon saveActIcon = QIcon(QString("%1save.png").arg(m_strImageSourcesPath));
	m_pActSaveIcon = new QAction(saveActIcon, QStringLiteral("����(&S)"), this);//ʵ����readAct
	m_pActSaveIcon->setStatusTip(QStringLiteral("����"));//�������ͣ����ʾ����
	m_pActSaveIcon->setShortcuts(QKeySequence::Open);//�򿪿�ݼ�
	connect(m_pActSaveIcon, &QAction::triggered, this, &GraphicSys::slotActionSaveFile);//�󶨶���ִ������

	//�������з���С����
	ui.m_pMainToolBar->addAction(m_pActNewIcon);	
	ui.m_pMainToolBar->addAction(m_pActOpenIcon);
	ui.m_pMainToolBar->addAction(m_pActSaveIcon);

	ui.m_pMainToolBar->addSeparator();	//��ӷָ���
}

void GraphicSys::initLeftToolBar()
{
	//��๤����
	m_pLeftToolBar = new QToolBar(QStringLiteral("��๤����"),this);
	addToolBar(Qt::LeftToolBarArea, m_pLeftToolBar);
	m_pLeftToolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);//����ֻ��������ͣ��
    m_pLeftToolBar->setFloatable(false);//���ø���
	m_pLeftToolBar->setStyleSheet("background-color:#f3eee8;");

	const QIcon recportActIcon = QIcon(QString("%1report.png").arg(m_strImageSourcesPath));;
	m_pActReport = new QAction(recportActIcon, QStringLiteral("���ܱ���(&R)"), this);//ʵ����readAct
	m_pActReport->setStatusTip(QStringLiteral("���ܱ���"));//�������ͣ����ʾ����
	m_pActReport->setShortcuts(QKeySequence::Open);//�򿪿�ݼ�
	//connect(m_pActReport, &QAction::triggered, this, &MainWindow::ReadFile);//�󶨶���ִ������
		
	const QIcon curveActIcon = QIcon(QString("%1curve.png").arg(m_strImageSourcesPath));
	m_pActCurve = new QAction(curveActIcon, QStringLiteral("����(&C)"), this);//ʵ����readAct
	m_pActCurve->setStatusTip(QStringLiteral("����"));//�������ͣ����ʾ����
	m_pActCurve->setShortcuts(QKeySequence::Open);//�򿪿�ݼ�
	//connect(m_pActReport, &QAction::triggered, this, &MainWindow::ReadFile);//�󶨶���ִ������

	//�������з���С����
	m_pLeftToolBar->addAction(m_pActReport);	
	m_pLeftToolBar->addSeparator();//��ӷָ���
	m_pLeftToolBar->addAction(m_pActCurve);
}

void GraphicSys::initStatusBar()
{
	//3��״̬�� ֻ����һ��
	//QStatusBar* stBar = statusBar();
	//setStatusBar(stBar);

	QLabel* label1 = new QLabel(QString::fromLocal8Bit("�����ʾ��Ϣ"), this);
	ui.m_pStatusBar->addWidget(label1);

	QLabel* label2 = new QLabel(QString::fromLocal8Bit("�Ҳ���ʾ��Ϣ"), this);
	ui.m_pStatusBar->addPermanentWidget(label2);
}

void GraphicSys::initFloatableWgt()
{
	//4��í�Ӳ���(��������)  �����ж��
	QDockWidget* dock = new QDockWidget("aaa", this);
	addDockWidget(Qt::TopDockWidgetArea, dock);
	//ֻ��������ͣ��
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
	//pFilename->setGeometry((QApplication::desktop()->width() - 800) / 2, (QApplication::desktop()->height() - 480) / 2, 800, 480);//δ������
	pFilename->setDirectory(fileInfo.absolutePath());//���ô򿪵�Ĭ��Ŀ¼
	//pFilename->setNameFilter(tr("exe(*.xls)"));//�����޶��ļ�����
	//pFilename->setOption(QFileDialog::DontResolveSymlinks);//���Լ�������·����������ת

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
	m_jsonProcess.getCfgInfo(m_stCfgInfo, strFileName);//��ȡ���ڳ�ʼ����ֵm_stCfgInfo
	//����m_stCfgInfo�½�
}

void GraphicSys::slotActionSaveFile()
{
	//����
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
			file.remove(strfile);//ɾ���ɵ��ļ�
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
	//QTabWidget��Ӵ򿪹���ʱ���һ��tab���������رչ��������tab���е�tabʵ�ֵ�
	if (m_pTabWiget->widget(nindex) == NULL )
	{
		return;
	}
	//�����½��ĵ��޸ĺ󱣴���ʾ????
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