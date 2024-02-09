#pragma once

#include "node.h"
#include "functional.h"
#include "style.h"
#include "viewport.h"
#include "cross.h"

#include <QApplication>
#include <QToolBar>
#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include <QStatusBar>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <QResizeEvent>
#include <QDebug>
#include <QThread>
#include <QObject>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QTimer>
#include <QLineEdit>
#include <QComboBox>
#include <cstdlib>
#include <QStandardPaths>
#include <QMessageBox>

#define MAIN_WINDOW_HEIGHT 750
#define MAIN_WINDOW_WIDTH  1200

class Node;
class Functional;
class Viewport;

class MainWindow : public QMainWindow
{
public:
    MainWindow() {};
    MainWindow(QGraphicsView*, QWidget* parent = nullptr);
    ~MainWindow();

    friend class Node;
    friend class Functional;

    static QString path_txt;
    static QString path_cpp;
    static QString path_exe;

protected:
    void resizeEvent(QResizeEvent*) override;
    void closeEvent(QCloseEvent*)   override;

private slots:
    void hideAndShowPanelSlot();
    void compilerSlot();
    void btnApplyCompilerSlot();
    void onTimeout();

private:
    void initEmptyProject();
    static void userPermission();
    static void createFiles();
    static void initMessageWindow(QString, QString);

private:
    Viewport*               scene;
    QGraphicsView*          m_graphicsView;
    static QTimer*          timer;
    static QWidget*         controlPanel;
    static QWidget*         messageWindow;
    static QWidget*         central_widget;
    QWidget*                compilerChoose;
    QPushButton*            save;
    QPushButton*            compile;
    QPushButton*            run;
    QPushButton*            terminal;
    QPushButton*            code;
    QPushButton*            compiler;
    QPushButton*            hideAndShowPanel;
    QPushButton*            btnApplyCompiler;
    QComboBox*              comboBoxCompiler;
    static QListWidget*     varTypesList;
    static QListWidget*     varNamesList;
    static QListWidget*     varValuesList;
    static QListWidget*     funcTypesList;
    static QListWidget*     funcNamesList;
    static QComboBox*       comboBoxFunctions;
    static QComboBox*       comboBoxVars;
    QLineEdit*              changeVarName;
    QLineEdit*              changeVarValue;
    QLineEdit*              changeFuncName;
    QLineEdit*              changeFuncValue;
    Node*                   node_obj;
    Functional*             func_obj;
    static MainWindow*      win;
};
