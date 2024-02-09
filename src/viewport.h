#pragma once

#include "main_window.h"
#include "node.h"
#include "functional.h"
#include "style.h"
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
#include <QLabel>
#include <QCheckBox>
#include <QListWidget>
#include <QPointF>
#include <QListWidgetItem>
#include <QMap>
#include <QVector>
#include <QLine>
#include <QLineEdit>
#include <QPainter>
#include <QFont>
#include <QGraphicsLineItem>
#include <QPen>
#include <QMessageBox>
#include <fstream>


#define WIDTH_NODE            160
#define HEIGHT_NODE           100
#define WIDTH_NODE_GET        80
#define HEIGHT_NODE_GET       60
#define WNDOW_GET_VARS_HEIGHT 240
#define WNDOW_GET_VARS_WIDTH  130

class Node;
class Functional;

extern QVector<Node*>                  vector_connect_node;
extern QVector<Node*>                  vector_connect_get_node;
extern QVector<Node*>                  vector_pos_nodes;
extern QMap<int, Node*>                nodes_map;
extern int                             line_id;
extern int                             left_line_node;     
extern int                             right_line_node;
extern int                             right_line_node_f;
extern bool                            is_branch;
extern bool                            is_get_node;

class Viewport : public QGraphicsScene
{
public:
    Viewport() {};
    Viewport(QGraphicsView*, QGraphicsScene* parent = nullptr);
    ~Viewport();

    friend class Node;
    friend class Functional;

private:
    bool isGet(int);
    void addNode(Node*);
    void removeNode(Node*);
    void initChangeNodeWindow();
    void visibleChangeNodeWindow(int, int, Node*);
    inline void addNodeInMenu(size_t, size_t, size_t, QString, QString);

private slots:
    void deleteNodeWindowSlot();
    void btnOkNameSlot();
    void btnOkValueSlot();

public slots:
    void create_node(QListWidgetItem*);
    void create_get_var();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;

private:
    static QListWidget*                     main_menu;
    static QWidget*                         changeNodeWindow;
    static QPushButton*                     btnOkName;
    static QPushButton*                     btnOkValue;
    static QPushButton*                     deleteNodeWindow;
    static QWidget*                         deleteWindow;
    static QLineEdit*                       newName;
    static QLineEdit*                       newValue;
    static QVector<Node*>                   nodes;
    static QMap<int, QGraphicsLineItem*>    connect_lines_map;
    QGraphicsProxyWidget*                   changeWindowProxy;
    QGraphicsProxyWidget*                   proxyWindowGetVars;
    QListWidgetItem*                        funcItem;
    QListWidgetItem*                        operatorItem;
    QListWidgetItem*                        cycleItem;
    QListWidgetItem*                        varsItem;
    QListWidgetItem*                        operator2Item;
    static Node*                            change;
    QGraphicsView*                          m_graphicsView;
    QWidget*                                MainMenu;
    QWidget*                                window_get_vars;
    QLineEdit*                              var_name;
    QLineEdit*                              var_value;
    QPushButton*                            btn_ok;
    QPointF                                 mouse_pos;
    QString                                 this_node_name;
    QString                                 this_node_value;
    QString                                 get_var_type;
    QPen                                    pen;
    static int                              unique_id;
    static bool                             isIdBool;
    int                                     x_pos;
    int                                     y_pos;
};
