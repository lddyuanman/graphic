#pragma once

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include <QTabWidget>
#include "ui_GraphicSys.h"
#include "../public/include/JsonProcess.h"
#include "../public/include/DataStruct.h"

#define INI_QPUSHBUTTON_HEIGHT 40
#define INI_QPUSHBUTTON_WIDTH 40
#define INT_LEFT_TOOLBAR_WIDTH 40
#define INT_TAB_BAR_HEIGHT 10
#define INT_TAB_BAR_WIDTH 519
#define INT_MAX_TAB_COUNT 20

class GraphicSys : public QMainWindow
{
    Q_OBJECT

public:
    GraphicSys(QWidget *parent = Q_NULLPTR);

    bool isFileExist(QString strfile);
    bool isSaveFile();
private:
    void initUI(); 
    void initMenuBar();
    void initTopToolBar();
    void initLeftToolBar();
    void initStatusBar();
    void initFloatableWgt();

    void addTabWgt();//ÃÌº”“ª“≥tabwidget
    void saveNewFile(QString strfile);
    void adjustTabBarWidth(int ncount);
public slots:
    void slotActionNewFile();
    void slotActionOpenFile();
    void slotActionSaveFile();

    void slotTabWgtClose(int nindex);

private:

    QAction* m_pActNewIcon;
    QAction* m_pActOpenIcon;
    QAction* m_pActSaveIcon;

    QToolBar* m_pLeftToolBar;
    QAction* m_pActReport;
    QAction* m_pActCurve;

    QTabWidget* m_pTabWiget;
    int m_nTabCount;
 
    QString m_strAppPath;
    QString m_strImageSourcesPath;
    QString m_strCfgFilePath;

    JsonProcess m_jsonProcess;
    stTableData m_stCfgInfo;

	Ui::GraphicSysClass ui;
};
