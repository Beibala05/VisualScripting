#pragma once

#include "main_window.h"
#include "viewport.h"
#include "functional.h"
#include "style.h"
#include <cmath>
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
#include <QGraphicsLineItem>
#include <QPen>
#include <QFile>
#include <QTextStream> 

#define WIDTH_NODE        160
#define HEIGHT_NODE       100
#define WIDTH_NODE_GET    80
#define HEIGHT_NODE_GET   60

extern int count_nodes;

class Viewport;
class MainWindow;
class Functional;

enum NodesId
{
    NODE_OUTPUT = 1,                // output
    NODE_INPUT,                     // input
    NODE_BRANCH,                    // branch
    NODE_ENDLESS_CYCLE,             // endless cycle
    NODE_CHANGE_VALUE,              // change value
    NODE_GLOBAL,                    // global
    NODE_NULL,                      // null node 
    NODE_BREAK,                     // break
    NODE_ENDL,                      // endl (\n)                   
    NODE_INT,                       // int
    NODE_FLOAT,                     // float
    NODE_DOUBLE,                    // double
    NODE_STRING,                    // string
    NODE_BOOL,                      // bool
    NODE_OPERATOR_P,                // +
    NODE_OPERATOR_M,                // -
    NODE_OPERATOR_MULT,             // *
    NODE_OPERATOR_DIV,              // /
    NODE_OPERATOR_PERCENT,          // %
    NODE_OPERATOR_GREAT,            // >
    NODE_OPERATOR_LESS,             // <
    NODE_OPERATOR_GREAT_OR_EQUAL,   // >=
    NODE_OPERATOR_LESS_OR_EQUAL,    // <=
    NODE_OPERATOR_EQUAL,            // ==
    NODE_OPERATOR_NOT_EQUAL,        // !=
};

class Node final : public QWidget
{
public:
    Node() {};
    Node(Viewport*, QGraphicsScene*, QGraphicsView*, size_t, QString, QString, QGraphicsView* parent = nullptr);
    ~Node();

    static QString path_txt;
    static QString path_cpp;
    static QString path_exe;

public:
    Node* next_node;
    Node* next_node_f;
    Node* prev_node;
    Node* prev_node_get_1;
    Node* prev_node_get_2;
    Node* ptr_return_value;

    int   u_id;             // Уникальное ID для каждого объекта ноды
    int   id;               // Уникальное ID для каждого объекта класса
    int   left_line;
    int   right_line;
    int   right_line_f;
    int   left_line_get_1;
    int   left_line_get_2;
    int   right_line_get;
    
    short begin;
    short end;
    short end_f;
    
    bool  is_connected_begin;
    bool  is_connected_end;
    bool  is_connected_end_f;
    bool  connection_1;
    bool  connection_2;
    bool  is_r_value;
    bool  check_input_1;
    bool  check_input_2;
    bool  is_head;

    QString var_name;
    QString var_value;

    friend class Viewport;
    friend class MainWindow;
    friend class Functional;

public:
    // Левая нода
    QPushButton* btn_begin;

    // Правая нода
    QPushButton* btn_end;

    // Для бранча (false)
    QPushButton* btn_end_f;

    // Входные параметры
    QPushButton* btn_get_node_1;
    QPushButton* btn_get_node_2;

    // Выходной параметр
    QPushButton* btn_return_value;

    // Изменить имя и значение для переменных
    QPushButton* btn_set_var;

    QLine* line;

public slots:
    void connect_nodes_begin();
    void connect_nodes_end();
    void connect_nodes_end_f();
    void connect_get_node_input_1();
    void connect_get_node_input_2();
    void connect_get_node_r_value();
    void connect_get_lines();
    void save_nodes_slot();
    void changeNodeSlot();

protected:
    void mousePressEvent(QMouseEvent*)      override;
    void mouseMoveEvent(QMouseEvent*)       override;
    void keyPressEvent(QKeyEvent*)          override;
    void mouseReleaseEvent(QMouseEvent*)    override;

private:
    QPoint                  m_mouseStartPosition;
    QGraphicsScene*         scene;
    QGraphicsView*          view;
    QLabel*                 text_begin;
    QLabel*                 node_name;
    QLabel*                 bool_value;
    QLabel*                 node_value;
    QCheckBox*              bool_check_box;
    static QPushButton*     btnExitChangeValue;
    QPen                    pen;
    QPen                    get_node_pen;
    bool                    is_widget_move;

    double                  r_double;
    bool                    r_bool;

    QString                 r_name;

    Viewport*               _viewport;
    
    static Node*            head;

    static QMap<int, QGraphicsLineItem*>   connect_lines_map;
    static QVector<QString>                var_repeat_names;
    static QVector<QString>                var_names;
    static int                             line_id;
    static Node*                           thisNode;


private:
    void            save_nodes();
    inline void     nodes_to_cpp(Node*);
    inline bool     check(QString);
    inline QString  type_node(Node*);
    inline QString  type_set_node(Node*);
    inline bool     is_get(Node*, bool&);
    inline void     delete_get_node(Node*);
    inline void     delete_node_connects(int);
    inline bool     isGetWithoutStr(Node*);
};
