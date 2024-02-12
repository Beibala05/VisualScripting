#include "node.h"

QVector<Node*>                  vector_connect_node;
QVector<Node*>                  vector_connect_get_node;
QVector<Node*>                  vector_pos_nodes;
QVector<QString>                Node::var_names;
QVector<QString>                Node::var_repeat_names;
QMap<int, QGraphicsLineItem*>   Node::connect_lines_map;
QMap<int, Node*>                nodes_map;
int                             Node::line_id = 0;
int                             left_line_node;     
int                             right_line_node;
int                             right_line_node_f;
bool                            is_branch   = false;
bool                            is_get_node = false;
bool                            is_branch_in_nodes = false;
int                             var_name_value = 0;
int                             tab = 1;

Node*                           Node::thisNode = NULL;
Node*                           Node::head     = NULL;
Node*                           current_node;
QString                         Node::path_txt;
QString                         Node::path_cpp;
QString                         Node::path_exe;

Node::Node(Viewport* _viewport, QGraphicsScene* scene, QGraphicsView* view, size_t btn, QString name, QString value, QGraphicsView* parent) : QWidget(parent)
{
    this->scene           = scene;
    this->view            = view;
    this->var_name        = name;
    this->var_value       = value;
    this->_viewport       = _viewport;

    next_node             = NULL;
    next_node_f           = NULL;
    prev_node             = NULL;
    ptr_return_value      = NULL;
    prev_node_get_1       = NULL;
    prev_node_get_2       = NULL;
    
    is_connected_begin    = false;
    is_connected_end      = false;
    is_connected_end_f    = false;
    is_r_value            = false;
    connection_1          = false;
    connection_2          = false;
    check_input_1         = false;
    check_input_2         = false;
    is_head               = false;

    begin                 = 0;
    end                   = 0;
    end_f                 = 0;

    left_line             = 0;
    right_line            = 0;
    right_line_f          = 0;

    left_line_get_1       = 0;
    left_line_get_2       = 0;
    right_line_get        = 0;

    id                    = btn;

    text_begin = new QLabel(this);
    text_begin->setGeometry(QRect(7, 0, 160, 20));

    if (btn == NodesId::NODE_OUTPUT)
    {
        this->setStyleSheet(nodes_styles::any_node_style);
        text_begin->setText(QCoreApplication::translate("", "OUTPUT", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::any_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
    }
    else if (btn == NodesId::NODE_INPUT)
    {
        this->setStyleSheet(nodes_styles::any_node_style);
        text_begin->setText(QCoreApplication::translate("", "INPUT", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::any_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
    }

    else if (btn == NodesId::NODE_BRANCH)
    {
        this->setStyleSheet(nodes_styles::bool_node_style);
        text_begin->setText(QCoreApplication::translate("", "BRANCH", nullptr));

        QLabel* text_true = new QLabel(this);
        text_true->setGeometry(QRect(105, 20, 160, 20));
        text_true->setText(QCoreApplication::translate("", "True", nullptr));
        text_true->setStyleSheet(nodes_styles::var_name_style);

        QLabel* text_false = new QLabel(this);
        text_false->setGeometry(QRect(105, 45, 160, 20));
        text_false->setText(QCoreApplication::translate("", "False", nullptr));
        text_false->setStyleSheet(nodes_styles::var_name_style);

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_end_f = new QPushButton(this);
        btn_end_f->setGeometry(140, 45, 20, 20);
        btn_end_f->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::bool_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_end_f, &QPushButton::clicked, this, &Node::connect_nodes_end_f);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
    }

    else if (btn == NodesId::NODE_ENDLESS_CYCLE)
    {
        this->setStyleSheet(nodes_styles::any_node_style);
        text_begin->setText(QCoreApplication::translate("", "ENDLESS_CYCLE", nullptr));

        QLabel* text_body = new QLabel(this);
        text_body->setGeometry(QRect(105, 45, 160, 20));
        text_body->setText(QCoreApplication::translate("", "Body", nullptr));
        text_body->setStyleSheet(nodes_styles::var_name_style);

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_end_f = new QPushButton(this);
        btn_end_f->setGeometry(140, 45, 20, 20);
        btn_end_f->setStyleSheet(nodes_styles::btn_end_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_end_f, &QPushButton::clicked, this, &Node::connect_nodes_end_f);
    }

    else if (btn == NodesId::NODE_CHANGE_VALUE)
    {
        this->setStyleSheet(nodes_styles::any_node_style);
        text_begin->setText(QCoreApplication::translate("", "CHANGE_VALUE", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::any_input_output_style);

        btn_get_node_2 = new QPushButton(this);
        btn_get_node_2->setGeometry(0, 70, 15, 15);
        btn_get_node_2->setStyleSheet(nodes_styles::any_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
        QObject::connect(btn_get_node_2, &QPushButton::clicked, this, &Node::connect_get_node_input_2);
    }

    else if (btn == NodesId::NODE_GLOBAL)
    {
        this->setStyleSheet(nodes_styles::any_node_style);
        text_begin->setText(QCoreApplication::translate("", "GLOBAL", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::any_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
    }

    else if (btn == NodesId::NODE_NULL)
    {
        this->setStyleSheet(nodes_styles::any_node_style);
        text_begin->setText(QCoreApplication::translate("", "NULL", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
    }

    else if (btn == NodesId::NODE_BREAK)
    {
        this->setStyleSheet(nodes_styles::any_node_style);
        text_begin->setText(QCoreApplication::translate("", "BREAK", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
    }
    
    else if (btn == NodesId::NODE_ENDL)
    {
        this->setStyleSheet(nodes_styles::any_node_style);
        text_begin->setText(QCoreApplication::translate("", "ENDL", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
    }

    else if (btn == NodesId::NODE_OPERATOR_P)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", "+", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_get_node_2 = new QPushButton(this);
        btn_get_node_2->setGeometry(0, 70, 15, 15);
        btn_get_node_2->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(145, 45, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::digit_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
        QObject::connect(btn_get_node_2, &QPushButton::clicked, this, &Node::connect_get_node_input_2);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_r_value);
    }
    else if (btn == NodesId::NODE_OPERATOR_M)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", "-", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_get_node_2 = new QPushButton(this);
        btn_get_node_2->setGeometry(0, 70, 15, 15);
        btn_get_node_2->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(145, 45, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::digit_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
        QObject::connect(btn_get_node_2, &QPushButton::clicked, this, &Node::connect_get_node_input_2);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_r_value);
    }
    else if (btn == NodesId::NODE_OPERATOR_MULT)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", "*", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_get_node_2 = new QPushButton(this);
        btn_get_node_2->setGeometry(0, 70, 15, 15);
        btn_get_node_2->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(145, 45, 15, 15); 
        btn_return_value->setStyleSheet(nodes_styles::digit_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
        QObject::connect(btn_get_node_2, &QPushButton::clicked, this, &Node::connect_get_node_input_2);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_r_value);
    }
    else if (btn == NodesId::NODE_OPERATOR_DIV)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", "/", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_get_node_2 = new QPushButton(this);
        btn_get_node_2->setGeometry(0, 70, 15, 15);
        btn_get_node_2->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(145, 45, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::digit_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
        QObject::connect(btn_get_node_2, &QPushButton::clicked, this, &Node::connect_get_node_input_2);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_r_value);
    }
    else if (btn == NodesId::NODE_OPERATOR_PERCENT)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", "%", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_get_node_2 = new QPushButton(this);
        btn_get_node_2->setGeometry(0, 70, 15, 15);
        btn_get_node_2->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(145, 45, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::digit_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
        QObject::connect(btn_get_node_2, &QPushButton::clicked, this, &Node::connect_get_node_input_2);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_r_value);
    }
    else if (btn == NodesId::NODE_OPERATOR_GREAT)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", ">", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_get_node_2 = new QPushButton(this);
        btn_get_node_2->setGeometry(0, 70, 15, 15);
        btn_get_node_2->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(145, 45, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::bool_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
        QObject::connect(btn_get_node_2, &QPushButton::clicked, this, &Node::connect_get_node_input_2);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_r_value);
    }
    else if (btn == NodesId::NODE_OPERATOR_LESS)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", "<", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_get_node_2 = new QPushButton(this);
        btn_get_node_2->setGeometry(0, 70, 15, 15);
        btn_get_node_2->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(145, 45, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::bool_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
        QObject::connect(btn_get_node_2, &QPushButton::clicked, this, &Node::connect_get_node_input_2);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_r_value);
    }
    else if (btn == NodesId::NODE_OPERATOR_GREAT_OR_EQUAL)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", ">=", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_get_node_2 = new QPushButton(this);
        btn_get_node_2->setGeometry(0, 70, 15, 15);
        btn_get_node_2->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(145, 45, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::bool_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
        QObject::connect(btn_get_node_2, &QPushButton::clicked, this, &Node::connect_get_node_input_2);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_r_value);
    }
    else if (btn == NodesId::NODE_OPERATOR_LESS_OR_EQUAL)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", "<=", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_get_node_2 = new QPushButton(this);
        btn_get_node_2->setGeometry(0, 70, 15, 15);
        btn_get_node_2->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(145, 45, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::bool_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
        QObject::connect(btn_get_node_2, &QPushButton::clicked, this, &Node::connect_get_node_input_2);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_r_value);
    }
    else if (btn == NodesId::NODE_OPERATOR_EQUAL)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", "==", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_get_node_2 = new QPushButton(this);
        btn_get_node_2->setGeometry(0, 70, 15, 15);
        btn_get_node_2->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(145, 45, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::bool_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
        QObject::connect(btn_get_node_2, &QPushButton::clicked, this, &Node::connect_get_node_input_2);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_r_value);
    }
    else if (btn == NodesId::NODE_OPERATOR_NOT_EQUAL)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", "!=", nullptr));

        btn_begin = new QPushButton(this);
        btn_begin->setGeometry(0, 20, 20, 20);
        btn_begin->setStyleSheet(nodes_styles::btn_begin_style);

        btn_end = new QPushButton(this);
        btn_end->setGeometry(140, 20, 20, 20);
        btn_end->setStyleSheet(nodes_styles::btn_end_style);

        btn_get_node_1 = new QPushButton(this);
        btn_get_node_1->setGeometry(0, 45, 15, 15);
        btn_get_node_1->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_get_node_2 = new QPushButton(this);
        btn_get_node_2->setGeometry(0, 70, 15, 15);
        btn_get_node_2->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(145, 45, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::bool_input_output_style);

        this->var_name  = "none";
        this->var_value = "none";

        QObject::connect(btn_begin, &QPushButton::clicked, this, &Node::connect_nodes_begin);
        QObject::connect(btn_end, &QPushButton::clicked, this, &Node::connect_nodes_end);
        QObject::connect(btn_get_node_1, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
        QObject::connect(btn_get_node_2, &QPushButton::clicked, this, &Node::connect_get_node_input_2);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_r_value);
    }

    // get ноды (переменные)
    else if (btn == NodesId::NODE_INT)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", "int", nullptr));

        node_name = new QLabel(this);
        node_name->setText(var_name);
        node_name->setGeometry(5, 20, 70, 20);
        node_name->setStyleSheet(nodes_styles::var_name_style);

        node_value = new QLabel(this);
        node_value->setGeometry(5, 40, 70, 15);
        node_value->setText(var_value);
        node_value->setStyleSheet(nodes_styles::digit_label_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(60, 2, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_set_var = new QPushButton(this);
        btn_set_var->setGeometry(55, 19, 20, 20);
        btn_set_var->setStyleSheet(nodes_styles::set_btn_style);
        btn_set_var->setIcon(QIcon(icons::update_icon));

        QObject::connect(btn_set_var, &QPushButton::clicked, this, &Node::changeNodeSlot);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
    }
    else if (btn == NodesId::NODE_FLOAT)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", "float", nullptr));

        node_name = new QLabel(this);
        node_name->setText(var_name);
        node_name->setGeometry(5, 20, 70, 20);
        node_name->setStyleSheet(nodes_styles::var_name_style);

        node_value = new QLabel(this);
        node_value->setGeometry(5, 40, 70, 15);
        node_value->setText(var_value);
        node_value->setStyleSheet(nodes_styles::digit_label_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(64, 2, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_set_var = new QPushButton(this);
        btn_set_var->setGeometry(50, 19, 20, 20);
        btn_set_var->setStyleSheet(nodes_styles::set_btn_style);
        btn_set_var->setIcon(QIcon(icons::update_icon));

        QObject::connect(btn_set_var, &QPushButton::clicked, this, &Node::changeNodeSlot);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
    }
    else if (btn == NodesId::NODE_DOUBLE)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
        text_begin->setText(QCoreApplication::translate("", "double", nullptr));

        node_name = new QLabel(this);
        node_name->setText(var_name);
        node_name->setGeometry(5, 20, 70, 20);
        node_name->setStyleSheet(nodes_styles::var_name_style);

        node_value = new QLabel(this);
        node_value->setGeometry(5, 40, 70, 15);
        node_value->setText(var_value);
        node_value->setStyleSheet(nodes_styles::digit_label_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(64, 2, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::digit_input_output_style);

        btn_set_var = new QPushButton(this);
        btn_set_var->setGeometry(50, 19, 20, 20);
        btn_set_var->setStyleSheet(nodes_styles::set_btn_style);
        btn_set_var->setIcon(QIcon(icons::update_icon));

        QObject::connect(btn_set_var, &QPushButton::clicked, this, &Node::changeNodeSlot);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
    }
    else if (btn == NodesId::NODE_STRING)
    {
        this->setStyleSheet(nodes_styles::string_node_style);
        text_begin->setText(QCoreApplication::translate("", "string", nullptr));

        node_name = new QLabel(this);
        node_name->setText(var_name);
        node_name->setGeometry(5, 20, 70, 20);
        node_name->setStyleSheet(nodes_styles::var_name_style);

        node_value = new QLabel(this);
        node_value->setGeometry(5, 40, 70, 15);
        node_value->setText(var_value);
        node_value->setStyleSheet(nodes_styles::string_label_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(64, 2, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::string_input_output_style);

        btn_set_var = new QPushButton(this);
        btn_set_var->setGeometry(50, 19, 20, 20);
        btn_set_var->setStyleSheet(nodes_styles::set_btn_style);
        btn_set_var->setIcon(QIcon(icons::update_icon));

        QObject::connect(btn_set_var, &QPushButton::clicked, this, &Node::changeNodeSlot);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
    }
    else if (btn == NodesId::NODE_BOOL)
    {
        this->setStyleSheet(nodes_styles::bool_node_style);
        text_begin->setText(QCoreApplication::translate("", "bool", nullptr));

        node_name = new QLabel(this);
        node_name->setText(var_name);
        node_name->setGeometry(5, 20, 70, 20);
        node_name->setStyleSheet(nodes_styles::var_name_style);

        node_value = new QLabel(this);
        node_value->setGeometry(5, 40, 70, 15);
        node_value->setText(var_value);
        node_value->setStyleSheet(nodes_styles::bool_label_style);

        btn_return_value = new QPushButton(this);
        btn_return_value->setGeometry(64, 2, 15, 15);
        btn_return_value->setStyleSheet(nodes_styles::bool_input_output_style);

        btn_set_var = new QPushButton(this);
        btn_set_var->setGeometry(50, 19, 20, 20);
        btn_set_var->setStyleSheet(nodes_styles::set_btn_style);
        btn_set_var->setIcon(QIcon(icons::update_icon));

        QObject::connect(btn_set_var, &QPushButton::clicked, this, &Node::changeNodeSlot);
        QObject::connect(btn_return_value, &QPushButton::clicked, this, &Node::connect_get_node_input_1);
    }
    
    text_begin->setStyleSheet(nodes_styles::node_name_style);
    QSize viewportSize(8000, 8000);
    view->viewport()->setFixedSize(viewportSize);
    view->setSceneRect(QRect(QPoint(0, 0), viewportSize));
}

Node::~Node()
{
    bool for_get_node;

    if (this == head) head = NULL;


    if(this->id == NodesId::NODE_BREAK || this->id == NodesId::NODE_NULL || this->id == NodesId::NODE_ENDL)
    {
        if (this->next_node != NULL)
        {
            Node* node = this->next_node;
            node->begin = 0;
            node->left_line = 0;
            node->is_connected_begin = false;
            node->prev_node = NULL;

            Node::delete_node_connects(this->right_line);
        }

        if (this->prev_node != NULL)
        {
            Node* node = this->prev_node;
            
            // head
            if (Node::head == node)
            {
                Node::head = NULL;
                QString text = node->text_begin->text();
                text.replace("\t(HEAD)", "");
                node->text_begin->setText(text);
            }

            if (node->next_node == this)
            {
                node->end = 0;
                node->right_line = 0;
                node->is_connected_end = false;
                node->next_node = NULL;
            }
            else if (node->next_node_f == this)
            {
                node->end_f = 0;
                node->right_line_f = 0;
                node->is_connected_end_f = false;
                node->next_node_f = NULL;
            }

            Node::delete_node_connects(this->left_line);
        }
    }

    else if(this->id == NodesId::NODE_BRANCH || this->id == NodesId::NODE_ENDLESS_CYCLE)
    {
        if (this->next_node != NULL)
        {
            Node* node = this->next_node;
            node->begin = 0;
            node->left_line = 0;
            node->is_connected_begin = false;
            node->prev_node = NULL;

            Node::delete_node_connects(this->right_line);
        }

        if (this->next_node_f != NULL)
        {
            Node* node = this->next_node_f;
            node->begin = 0;
            node->left_line = 0;
            node->is_connected_begin = false;
            node->prev_node = NULL;

            Node::delete_node_connects(this->right_line_f);
        }

        if (this->prev_node != NULL)
        {
            Node* node = this->prev_node;

            // head
            if (Node::head == node)
            {
                Node::head = NULL;
                QString text = node->text_begin->text();
                text.replace("\t(HEAD)", "");
                node->text_begin->setText(text);
            }

            if (node->next_node == this)
            {
                node->end = 0;
                node->right_line = 0;
                node->is_connected_end = false;
                node->next_node = NULL;
            }
            else if (node->next_node_f == this)
            {
                node->end_f = 0;
                node->right_line_f = 0;
                node->is_connected_end_f = false;
                node->next_node_f = NULL;
            }

            Node::delete_node_connects(this->left_line);
        }

        if (this->prev_node_get_1 != NULL)
        {
            Node* node = this->prev_node_get_1;

            if (Node::is_get(node, for_get_node))
            {
                node->right_line = 0;
                node->ptr_return_value = NULL;
            }
            else
            {
                node->is_r_value = false;
                node->ptr_return_value = NULL;
            }

            Node::delete_node_connects(this->left_line_get_1);
        }
    }

    else if (Node::is_get(this, for_get_node))
    {
        Node::delete_get_node(this);

        for (int i = 0; i < var_names.size(); ++i)
        {
            if (this->var_name == var_names[i])
            {
                var_names.remove(i);
                break;
            }
        }

        for (int i = 0; i < var_repeat_names.size(); ++i)
        {
            if (this->var_name == var_repeat_names[i])
            {
                var_repeat_names.remove(i);
                break;
            }
        }
    }
    
    else
    {
        if (this->next_node != NULL)
        {
            Node* node = this->next_node;
            node->begin = 0;
            node->left_line = 0;
            node->is_connected_begin = false;
            node->prev_node = NULL;
            

            Node::delete_node_connects(this->right_line);
        }

        if (this->prev_node != NULL)
        {
            Node* node = this->prev_node;

            // head
            if (Node::head == node)
            {
                Node::head = NULL;
                QString text = node->text_begin->text();
                text.replace("\t(HEAD)", "");
                node->text_begin->setText(text);
            }

            if (node->next_node == this)
            {
                node->end = 0;
                node->right_line = 0;
                node->is_connected_end = false;
                node->next_node = NULL;
            }
            else if (node->next_node_f == this)
            {
                node->end_f = 0;
                node->right_line_f = 0;
                node->is_connected_end_f = false;
                node->next_node_f = NULL;
            }

            Node::delete_node_connects(this->left_line);
        }

        if (this->prev_node_get_1 != NULL)
        {
            Node* node = this->prev_node_get_1;

            if (Node::is_get(node, for_get_node))
            {
                node->right_line = 0;
                node->ptr_return_value = NULL;  
            }
            else
            {
                node->is_r_value = false;
                node->ptr_return_value = NULL;
            }

            Node::delete_node_connects(this->left_line_get_1);
        }

        if (this->prev_node_get_2 != NULL)
        {
            Node* node = this->prev_node_get_2;

            if (Node::is_get(node, for_get_node))
            {
                node->right_line = 0;
                node->ptr_return_value = NULL;
            }
            else
            {
                node->is_r_value = false;
                node->ptr_return_value = NULL;
            }

            Node::delete_node_connects(this->left_line_get_2);
        }

        if (this->ptr_return_value != NULL)
        {
            Node* node = this->ptr_return_value;

            if (node->prev_node_get_1 == this)
            {
                node->prev_node_get_1 = NULL;
                node->check_input_1   = false;
            }

            else if (node->prev_node_get_2 == this)
            {
                node->prev_node_get_2 = NULL;
                node->check_input_2   = false;
            }

            this->ptr_return_value = NULL;
            this->is_r_value       = false;

            Node::delete_node_connects(this->right_line_get);
        }
    }
}

void Node::delete_get_node(Node* node)
{
    for (int i = 0; i < _viewport->nodes.size(); ++i)
    {
        if (_viewport->nodes[i]->prev_node_get_1 == node)
        {
            _viewport->nodes[i]->prev_node_get_1 = NULL;
            _viewport->nodes[i]->check_input_1   = false;
            Node::delete_node_connects(Viewport::nodes[i]->left_line_get_1);
        }

        if (_viewport->nodes[i]->prev_node_get_2 == node)
        {
            _viewport->nodes[i]->prev_node_get_2 = NULL;
            _viewport->nodes[i]->check_input_2   = false;
            Node::delete_node_connects(Viewport::nodes[i]->left_line_get_2);
        }
    }
}

void Node::delete_node_connects(int id)
{
    for (const auto& key : connect_lines_map.keys()) 
    {
        if (key == id)
        {
            auto it = connect_lines_map.find(key);
            delete connect_lines_map[key];
            connect_lines_map.erase(it);
        }
    }
}

void Node::connect_nodes_begin()
{
    if (vector_connect_node.size() == 1)
    {
        if (vector_connect_node[0]->u_id != this->u_id)
        {
            vector_connect_node.push_back(this);
        }
        else
        {
            vector_connect_node.clear();
            for (int i = 0; i < Viewport::nodes.size(); ++i)
            {
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_begin_style);
            }
        }
    }

    else 
    {
        vector_connect_node.push_back(this);
        for (int i = 0; i < Viewport::nodes.size(); ++i)
        {
            if (!isGetWithoutStr(Viewport::nodes[i]))
                Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_begin_style);
        }
    }

    if (vector_connect_node.size() == 1 && !is_connected_begin)
    {
        vector_connect_node[0]->begin = 1;
    }
    else if (vector_connect_node.size() == 2 && !is_connected_begin)
    {
        vector_connect_node[1]->begin = 1;
    }

    if (vector_connect_node.size() == 2)
    {
        if ((vector_connect_node[1]->begin == 2 && vector_connect_node[0]->end == 2)
         || (vector_connect_node[0]->begin == 2 && vector_connect_node[1]->end == 2))
        {
            qDebug() << "Значение 1:\t" << "begin = " << vector_connect_node[0]->begin << "end = " << vector_connect_node[0]->end;
            qDebug() << "Значение 2:\t" << "begin = " << vector_connect_node[1]->begin << "end = " << vector_connect_node[1]->end;
            qDebug() << "Соединение уже установлено";

            for (int i = 0; i < Viewport::nodes.size(); ++i)
            {
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_begin_style);
            }

            vector_connect_node.clear();

            if (!is_connected_begin)
            {
                vector_connect_node[0]->begin = 0;
                vector_connect_node[1]->begin = 0;
            }
        }

        else if ((vector_connect_node[1]->begin == 2 && vector_connect_node[0]->end_f == 2)
              || (vector_connect_node[0]->begin == 2 && vector_connect_node[1]->end_f == 2))
        {
            qDebug() << "Значение 1:\t" << "begin = " << vector_connect_node[0]->begin << "end_f = " << vector_connect_node[0]->end_f;
            qDebug() << "Значение 2:\t" << "begin = " << vector_connect_node[1]->begin << "end_f = " << vector_connect_node[1]->end_f;
            qDebug() << "Соединение уже установлено";

            for (int i = 0; i < Viewport::nodes.size(); ++i)
            {
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_begin_style);
            }

            vector_connect_node.clear();

            if (!is_connected_begin)
            {
                vector_connect_node[0]->begin = 0;
                vector_connect_node[1]->begin = 0;
            }
        }

        else if (vector_connect_node[0]->end_f == 1 && vector_connect_node[1]->begin == 1)
        {
            qDebug() << "Значение 1:\t" << "begin = " << vector_connect_node[0]->begin << "end_f = " << vector_connect_node[0]->end_f;
            qDebug() << "Значение 2:\t" << "begin = " << vector_connect_node[1]->begin << "end_f = " << vector_connect_node[1]->end_f;

            if (head != NULL)
            {
                if (vector_connect_node[1] == head) 
                {
                    vector_connect_node[0]->end_f = 0;
                    vector_connect_node[1]->begin = 0;
                    vector_connect_node.clear();
                    return;
                }
            }

            for (int i = 0; i < Viewport::nodes.size(); ++i)
            {
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_begin_style);
            }

            qDebug() << "Соединение установлено";

            if (vector_connect_node[0]->prev_node == NULL && head == NULL)
            {
                head = vector_connect_node[0];
                QString text = vector_connect_node[0]->text_begin->text();
                vector_connect_node[0]->text_begin->setText(text + "\t(HEAD)");
            }

            vector_connect_node[0]->next_node_f = vector_connect_node[1];
            vector_connect_node[1]->prev_node = vector_connect_node[0];

            vector_connect_node[1]->begin   = 2;
            vector_connect_node[0]->end_f   = 2;

            vector_connect_node[1]->is_connected_begin = true;
            vector_connect_node[0]->is_connected_end_f = true;

            int x1 = vector_connect_node[0]->pos().x();
            int y1 = vector_connect_node[0]->pos().y();

            int x2 = vector_connect_node[1]->pos().x();
            int y2 = vector_connect_node[1]->pos().y();

            line_id++;

            vector_connect_node[0]->right_line_f = line_id;
            vector_connect_node[1]->left_line    = line_id;

            connect_lines_map.insert(line_id, new QGraphicsLineItem(QLineF(x1 + 150, y1 + 53, x2, y2 + 30)));

            pen.setWidth(5);
            pen.setColor(Qt::white);
            connect_lines_map[line_id]->setPen(pen);


            scene->addItem(connect_lines_map[line_id]);

            qDebug() << "line_id: " << line_id;
        }

        else if (vector_connect_node[0]->end == 1 && vector_connect_node[1]->begin == 1)
        {
            qDebug() << "Значение 1:\t" << "begin = " << vector_connect_node[0]->begin << "end = " << vector_connect_node[0]->end;
            qDebug() << "Значение 2:\t" << "begin = " << vector_connect_node[1]->begin << "end = " << vector_connect_node[1]->end;

            if (head != NULL)
            {
                if (vector_connect_node[1] == head) 
                {
                    vector_connect_node[0]->end = 0;
                    vector_connect_node[1]->begin = 0;
                    vector_connect_node.clear();
                    return;
                }
            }

            qDebug() << "Соединение установлено";

            for (int i = 0; i < Viewport::nodes.size(); ++i)
            {
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_begin_style);
            }

            if (vector_connect_node[0]->prev_node == NULL && head == NULL)
            {
                head = vector_connect_node[0];
                vector_connect_node[0]->is_head = true;
                QString text = vector_connect_node[0]->text_begin->text();
                vector_connect_node[0]->text_begin->setText(text + "\t(HEAD)");
            }

            vector_connect_node[0]->next_node =  vector_connect_node[1];
            vector_connect_node[1]->prev_node =  vector_connect_node[0];

            vector_connect_node[1]->begin = 2;
            vector_connect_node[0]->end   = 2;

            vector_connect_node[1]->is_connected_begin = true;
            vector_connect_node[0]->is_connected_end   = true;

            int x1 = vector_connect_node[0]->pos().x();
            int y1 = vector_connect_node[0]->pos().y();

            int x2 = vector_connect_node[1]->pos().x();
            int y2 = vector_connect_node[1]->pos().y();

            line_id++;

            vector_connect_node[0]->right_line = line_id;
            vector_connect_node[1]->left_line  = line_id;

            connect_lines_map.insert(line_id, new QGraphicsLineItem(QLineF(x1 + 150, y1 + 30, x2, y2 + 30)));

            pen.setWidth(5);
            pen.setColor(Qt::white);
            connect_lines_map[line_id]->setPen(pen);


            scene->addItem(connect_lines_map[line_id]);

            qDebug() << "line_id: " << line_id;
        }   
        else 
        {
            qDebug() << "Значение 1:\t" << "begin = " << vector_connect_node[0]->begin << "end = " << vector_connect_node[0]->end;
            qDebug() << "Значение 2:\t" << "begin = " << vector_connect_node[1]->begin << "end = " << vector_connect_node[1]->end;

            vector_connect_node[0]->begin = 0;
            vector_connect_node[1]->begin = 0;
            
            vector_connect_node[0]->end = 0;
            vector_connect_node[1]->end = 0;

            vector_connect_node[0]->end_f = 0;
            vector_connect_node[1]->end_f = 0;

            qDebug() << "Соединение не возможно";

            for (int i = 0; i < Viewport::nodes.size(); ++i)
            {
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_begin_style);
            }
        }

        vector_connect_node.clear();
    }
}

void Node::connect_get_node_input_1()
{
    if (check_input_1) 
    {
        return;
    }

    if (vector_connect_get_node.size() == 1)
    {
        if (vector_connect_get_node[0]->u_id != this->u_id)
        {
            vector_connect_get_node.push_back(this);
        }
        else
            vector_connect_get_node.clear();
    }
    else
        vector_connect_get_node.push_back(this);

    if (vector_connect_get_node.size() == 2)
    {
        bool is_get = false;
        bool is_get_second_element = false;

        if (vector_connect_get_node[0]->id == NodesId::NODE_INT    ||
            vector_connect_get_node[0]->id == NodesId::NODE_FLOAT  ||
            vector_connect_get_node[0]->id == NodesId::NODE_DOUBLE ||
            vector_connect_get_node[0]->id == NodesId::NODE_STRING ||
            vector_connect_get_node[0]->id == NodesId::NODE_BOOL)
            is_get = true;

        if (vector_connect_get_node[1]->id == NodesId::NODE_INT    ||
            vector_connect_get_node[1]->id == NodesId::NODE_FLOAT  ||
            vector_connect_get_node[1]->id == NodesId::NODE_DOUBLE ||
            vector_connect_get_node[1]->id == NodesId::NODE_STRING ||
            vector_connect_get_node[1]->id == NodesId::NODE_BOOL)
            is_get_second_element = true;


        if (!is_get && !vector_connect_get_node[0]->is_r_value)
        {
            vector_connect_get_node.clear();
            return;
        }

        if (vector_connect_get_node[0]->is_r_value)
        {
            if (vector_connect_get_node[0]->next_node == vector_connect_get_node[1] &&
                vector_connect_get_node[1]->prev_node == vector_connect_get_node[0])
            {
                ;
            }
            else
            {
                vector_connect_get_node[0]->is_r_value = false;
                vector_connect_get_node.clear();
                return;
            }
        }
        
        if (is_get_second_element)
        {
            vector_connect_get_node.clear();
            return;
        }

        if (is_get && !is_get_second_element)
        {
            if ((
                vector_connect_get_node[0]->id == NodesId::NODE_INT                     ||
                vector_connect_get_node[0]->id == NodesId::NODE_FLOAT                   ||
                vector_connect_get_node[0]->id == NodesId::NODE_DOUBLE
            ) && (
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_P              ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_M              ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_MULT           ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_DIV            ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_PERCENT        ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT          ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS           ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT_OR_EQUAL ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS_OR_EQUAL  ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_EQUAL          ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_NOT_EQUAL      ||
                vector_connect_get_node[1]->id == NodesId::NODE_INPUT                   ||
                vector_connect_get_node[1]->id == NodesId::NODE_OUTPUT                  ||
                vector_connect_get_node[1]->id == NodesId::NODE_CHANGE_VALUE            ||
                vector_connect_get_node[1]->id == NodesId::NODE_GLOBAL
            ))
            {
                get_node_pen.setWidth(3);
                get_node_pen.setColor(Qt::green);
                qDebug() << "Содеинть можно";
            }

            else if((
                vector_connect_get_node[0]->id == NodesId::NODE_STRING            
            ) && (
                vector_connect_get_node[1]->id == NodesId::NODE_INPUT                   ||
                vector_connect_get_node[1]->id == NodesId::NODE_OUTPUT                  ||
                vector_connect_get_node[1]->id == NodesId::NODE_GLOBAL                  ||
                vector_connect_get_node[1]->id == NodesId::NODE_CHANGE_VALUE            ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_EQUAL          ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_NOT_EQUAL      ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT          ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS           ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT_OR_EQUAL ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS_OR_EQUAL 
            ))
            {
                get_node_pen.setWidth(3);
                get_node_pen.setColor(QColorConstants::Svg::pink);
                qDebug() << "Содеинть можно";
            }

            else if (vector_connect_get_node[0]->id == NodesId::NODE_BOOL                    && 
                    (vector_connect_get_node[1]->id == NodesId::NODE_BRANCH                  ||
                     vector_connect_get_node[1]->id == NodesId::NODE_INPUT                   ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OUTPUT                  ||
                     vector_connect_get_node[1]->id == NodesId::NODE_CHANGE_VALUE            ||
                     vector_connect_get_node[1]->id == NodesId::NODE_GLOBAL                  ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_EQUAL          ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_NOT_EQUAL      ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT          ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS           ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT_OR_EQUAL ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS_OR_EQUAL 
                    ))
            {
                get_node_pen.setWidth(3);
                get_node_pen.setColor(Qt::red);
                qDebug() << "Содеинть можно";
            }

            else
            {
                qDebug() << "Содеинть нельзя";
                vector_connect_get_node.clear();
                return;
            }
        }

        else if (!is_get && !is_get_second_element)
        {
            if ((
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_GREAT          ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_LESS           ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_GREAT_OR_EQUAL ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_LESS_OR_EQUAL  ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_EQUAL          ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_NOT_EQUAL
            ) && (
                vector_connect_get_node[1]->id == NodesId::NODE_INPUT                   ||
                vector_connect_get_node[1]->id == NodesId::NODE_OUTPUT                  ||
                vector_connect_get_node[1]->id == NodesId::NODE_BRANCH                  ||
                vector_connect_get_node[1]->id == NodesId::NODE_CHANGE_VALUE
            ))
            {
                get_node_pen.setColor(Qt::red);
                qDebug() << "Содеинть можно";
            }

            else if ((
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_P              ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_M              ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_MULT           ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_DIV            ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_PERCENT
            ) && (
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_P              ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_M              ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_MULT           ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_DIV            ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_PERCENT        ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT          ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS           ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT_OR_EQUAL ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS_OR_EQUAL  ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_EQUAL          ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_NOT_EQUAL      ||
                vector_connect_get_node[1]->id == NodesId::NODE_INPUT                   ||
                vector_connect_get_node[1]->id == NodesId::NODE_OUTPUT                  ||
                vector_connect_get_node[1]->id == NodesId::NODE_CHANGE_VALUE
            ))
            {
                get_node_pen.setColor(Qt::green);
                qDebug() << "Содеинть можно";
            }
            
            else
            {
                qDebug() << "Содеинть нельзя";
                vector_connect_get_node.clear();
                return;
            }
        }

        check_input_1 = true;

        vector_connect_get_node[0]->ptr_return_value = vector_connect_get_node[1];
        vector_connect_get_node[1]->prev_node_get_1  = vector_connect_get_node[0];

        vector_connect_get_node[0]->connection_1 = true;

        int x1 = vector_connect_get_node[0]->pos().x();
        int y1 = vector_connect_get_node[0]->pos().y();

        int x2 = vector_connect_get_node[1]->pos().x();
        int y2 = vector_connect_get_node[1]->pos().y();

        line_id++;

        vector_connect_get_node[0]->right_line_get  = line_id;
        vector_connect_get_node[1]->left_line_get_1 = line_id;

        if (vector_connect_get_node[0]->is_r_value)
            connect_lines_map.insert(line_id, new QGraphicsLineItem(QLineF(x1 + 150, y1 + 53, x2, y2 + 53)));

        else
            connect_lines_map.insert(line_id, new QGraphicsLineItem(QLineF(x1 + 75, y1 + 2, x2, y2 + 50)));

        get_node_pen.setWidth(3);
        connect_lines_map[line_id]->setPen(get_node_pen);

        scene->addItem(connect_lines_map[line_id]);

        qDebug() << "line_id: " << line_id;

        vector_connect_get_node.clear();
    
    }
}

void Node::connect_get_node_input_2()
{
    if (check_input_2) 
    {
        return;
    }

    if (vector_connect_get_node.size() == 1)
    {
        if (vector_connect_get_node[0]->u_id != this->u_id)
        {
            vector_connect_get_node.push_back(this);
        }
        else
            vector_connect_get_node.clear();
    }
    else
        vector_connect_get_node.push_back(this);

    if (vector_connect_get_node.size() == 2)
    {
        bool is_get = false;
        bool is_get_second_element = false;

        if (vector_connect_get_node[0]->id == NodesId::NODE_INT    ||
            vector_connect_get_node[0]->id == NodesId::NODE_FLOAT  ||
            vector_connect_get_node[0]->id == NodesId::NODE_DOUBLE ||
            vector_connect_get_node[0]->id == NodesId::NODE_STRING ||
            vector_connect_get_node[0]->id == NodesId::NODE_BOOL)
            is_get = true;

        if (vector_connect_get_node[1]->id == NodesId::NODE_INT    ||
            vector_connect_get_node[1]->id == NodesId::NODE_FLOAT  ||
            vector_connect_get_node[1]->id == NodesId::NODE_DOUBLE ||
            vector_connect_get_node[1]->id == NodesId::NODE_STRING ||
            vector_connect_get_node[1]->id == NodesId::NODE_BOOL)
            is_get_second_element = true;


        if (!is_get && !vector_connect_get_node[0]->is_r_value)
        {
            vector_connect_get_node.clear();
            return;
        }

        if (vector_connect_get_node[0]->is_r_value)
        {
            if (vector_connect_get_node[0]->next_node == vector_connect_get_node[1] &&
                vector_connect_get_node[1]->prev_node == vector_connect_get_node[0])
            {
                ;
            }
            else
            {
                vector_connect_get_node[0]->is_r_value = false;
                vector_connect_get_node.clear();
                return;
            }
        }

        if (is_get_second_element)
        {
            vector_connect_get_node.clear();
            return;
        }

        if (is_get && !is_get_second_element)
        {
            if ((
                vector_connect_get_node[0]->id == NodesId::NODE_INT                     ||
                vector_connect_get_node[0]->id == NodesId::NODE_FLOAT                   ||
                vector_connect_get_node[0]->id == NodesId::NODE_DOUBLE
            ) && (
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_P              ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_M              ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_MULT           ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_DIV            ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_PERCENT        ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT          ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS           ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT_OR_EQUAL ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS_OR_EQUAL  ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_EQUAL          ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_NOT_EQUAL      ||
                vector_connect_get_node[1]->id == NodesId::NODE_INPUT                   ||
                vector_connect_get_node[1]->id == NodesId::NODE_OUTPUT                  ||
                vector_connect_get_node[1]->id == NodesId::NODE_CHANGE_VALUE            ||
                vector_connect_get_node[1]->id == NodesId::NODE_GLOBAL
            ))
            {
                get_node_pen.setColor(Qt::green);
                qDebug() << "Содеинть можно";
            }

            else if((
                vector_connect_get_node[0]->id == NodesId::NODE_STRING            
            ) && (
                vector_connect_get_node[1]->id == NodesId::NODE_INPUT                   ||
                vector_connect_get_node[1]->id == NodesId::NODE_OUTPUT                  ||
                vector_connect_get_node[1]->id == NodesId::NODE_GLOBAL                  ||
                vector_connect_get_node[1]->id == NodesId::NODE_CHANGE_VALUE            ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_EQUAL          ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_NOT_EQUAL      ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT          ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS           ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT_OR_EQUAL ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS_OR_EQUAL
                
            ))
            {
                get_node_pen.setColor(QColorConstants::Svg::pink);
                qDebug() << "Содеинть можно";
            }

            else if (vector_connect_get_node[0]->id == NodesId::NODE_BOOL                    && 
                    (vector_connect_get_node[1]->id == NodesId::NODE_BRANCH                  ||
                     vector_connect_get_node[1]->id == NodesId::NODE_INPUT                   ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OUTPUT                  ||
                     vector_connect_get_node[1]->id == NodesId::NODE_CHANGE_VALUE            ||
                     vector_connect_get_node[1]->id == NodesId::NODE_GLOBAL                  ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_EQUAL          ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_NOT_EQUAL      ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT          ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS           ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT_OR_EQUAL ||
                     vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS_OR_EQUAL
                     ))
            {
                get_node_pen.setColor(Qt::red);
            }



            else
            {
                vector_connect_get_node.clear();
                return;
            }
        }

        else if (!is_get && !is_get_second_element)
        {
            if ((
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_GREAT          ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_LESS           ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_GREAT_OR_EQUAL ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_LESS_OR_EQUAL  ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_EQUAL          ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_NOT_EQUAL
            ) && (
                vector_connect_get_node[1]->id == NodesId::NODE_INPUT                   ||
                vector_connect_get_node[1]->id == NodesId::NODE_OUTPUT                  ||
                vector_connect_get_node[1]->id == NodesId::NODE_BRANCH                  ||
                vector_connect_get_node[1]->id == NodesId::NODE_CHANGE_VALUE
            ))
            {
                get_node_pen.setColor(Qt::red);
            }

            else if ((
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_P              ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_M              ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_MULT           ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_DIV            ||
                vector_connect_get_node[0]->id == NodesId::NODE_OPERATOR_PERCENT
            ) && (
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_P              ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_M              ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_MULT           ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_DIV            ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_PERCENT        ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT          ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS           ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_GREAT_OR_EQUAL ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_LESS_OR_EQUAL  ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_EQUAL          ||
                vector_connect_get_node[1]->id == NodesId::NODE_OPERATOR_NOT_EQUAL      ||
                vector_connect_get_node[1]->id == NodesId::NODE_INPUT                   ||
                vector_connect_get_node[1]->id == NodesId::NODE_OUTPUT                  ||
                vector_connect_get_node[1]->id == NodesId::NODE_CHANGE_VALUE
            ))
            {
                get_node_pen.setColor(Qt::green);
            }
            
            else
            {
                vector_connect_get_node.clear();
                return;
            }
        }

        check_input_2 = true;

        vector_connect_get_node[0]->ptr_return_value = vector_connect_get_node[1];
        vector_connect_get_node[1]->prev_node_get_2  = vector_connect_get_node[0];

        vector_connect_get_node[0]->connection_2 = true;

        int x1 = vector_connect_get_node[0]->pos().x();
        int y1 = vector_connect_get_node[0]->pos().y();

        int x2 = vector_connect_get_node[1]->pos().x();
        int y2 = vector_connect_get_node[1]->pos().y();

        line_id++;

        vector_connect_get_node[0]->right_line_get   = line_id;
        vector_connect_get_node[1]->left_line_get_2  = line_id;

        if (vector_connect_get_node[0]->is_r_value)
            connect_lines_map.insert(line_id, new QGraphicsLineItem(QLineF(x1 + 150, y1 + 53, x2, y2 + 78)));
        else
            connect_lines_map.insert(line_id, new QGraphicsLineItem(QLineF(x1 + 75, y1 + 2, x2, y2 + 78)));

        get_node_pen.setWidth(3);
        connect_lines_map[line_id]->setPen(get_node_pen);

        scene->addItem(connect_lines_map[line_id]);
        vector_connect_get_node.clear();
    }
}

void Node::connect_get_node_r_value()
{
    if (this->is_r_value) return;

    if (vector_connect_get_node.size() == 1)
    {
        if (vector_connect_get_node[0]->u_id != this->u_id)
        {
            this->is_r_value = true;
            vector_connect_get_node.push_back(this);
        }
        else
        {
            this->is_r_value = false;
            vector_connect_get_node.clear();
        }
    }
    else
    {
        this->is_r_value = true;
        vector_connect_get_node.push_back(this);
    }


    if (vector_connect_get_node.size() == 2)
    {
        bool is_get = false;

        if (vector_connect_get_node[0]->id == NodesId::NODE_INT    ||
            vector_connect_get_node[0]->id == NodesId::NODE_FLOAT  ||
            vector_connect_get_node[0]->id == NodesId::NODE_DOUBLE ||
            vector_connect_get_node[0]->id == NodesId::NODE_STRING ||
            vector_connect_get_node[0]->id == NodesId::NODE_BOOL)
            is_get = true;


        if (is_get)
        {
            vector_connect_get_node.clear();
            return;
        }

        vector_connect_get_node[0]->ptr_return_value = vector_connect_get_node[1];
        vector_connect_get_node[1]->prev_node_get_2  = vector_connect_get_node[0];

        int x1 = vector_connect_get_node[0]->pos().x();
        int y1 = vector_connect_get_node[0]->pos().y();

        int x2 = vector_connect_get_node[1]->pos().x();
        int y2 = vector_connect_get_node[1]->pos().y();

        line_id++;

        vector_connect_get_node[0]->right_line_get   = line_id;
        vector_connect_get_node[1]->left_line_get_2  = line_id;

        connect_lines_map.insert(line_id, new QGraphicsLineItem(QLineF(x1 + 75, y1 + 2, x2, y2 + 78)));

        get_node_pen.setWidth(3);
        get_node_pen.setColor(Qt::green);
        connect_lines_map[line_id]->setPen(get_node_pen);

        scene->addItem(connect_lines_map[line_id]);

        vector_connect_get_node.clear();
    }
}

void Node::connect_nodes_end()
{
    if (vector_connect_node.size() == 1)
    {
        if (vector_connect_node[0]->u_id != this->u_id)
        {
            vector_connect_node.push_back(this);
        }
        else
        {
            vector_connect_node.clear();

            for (int i = 0; i < Viewport::nodes.size(); ++i)
            {
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_begin_style);
            }
        }
    }

    else 
    {
        vector_connect_node.push_back(this);

        for (int i = 0; i < Viewport::nodes.size(); ++i)
        {
            if (Viewport::nodes[i] != this && Viewport::nodes[i] != head && Viewport::nodes[i]->prev_node == NULL)
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_current_begin_style);
        }
    }

    if (vector_connect_node.size() == 1 && !is_connected_end)
    {
        vector_connect_node[0]->end = 1;
    }
    else if (vector_connect_node.size() == 2 && !is_connected_end)
    {
        vector_connect_node[1]->end = 1;
    }

    if (vector_connect_node.size() == 2)
    {
        if ((vector_connect_node[1]->begin == 2 && vector_connect_node[0]->end == 2)
         || (vector_connect_node[0]->begin == 2 && vector_connect_node[1]->end == 2))
        {

            if (!is_connected_end)
            {
                vector_connect_node[0]->end = 0;
                vector_connect_node[1]->end = 0;
            }

            for (int i = 0; i < Viewport::nodes.size(); ++i)
            {
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_current_begin_style);
            }
        }

        else if (vector_connect_node[0]->end == 1 && vector_connect_node[1]->begin == 1)
        {

            vector_connect_node[0]->next_node = vector_connect_node[1];
            vector_connect_node[1]->prev_node = vector_connect_node[0];

            vector_connect_node[1]->begin = 2;
            vector_connect_node[0]->end   = 2;

            vector_connect_node[1]->is_connected_begin = true;
            vector_connect_node[0]->is_connected_end   = true;

            int x1 = vector_connect_node[0]->pos().x();
            int y1 = vector_connect_node[0]->pos().y();

            int x2 = vector_connect_node[1]->pos().x();
            int y2 = vector_connect_node[1]->pos().y();

            line_id++;

            vector_connect_node[0]->right_line = line_id;
            vector_connect_node[1]->left_line  = line_id;

            connect_lines_map.insert(line_id, new QGraphicsLineItem(QLineF(x1 + 150, y1 + 30, x2, y2 + 30)));

            pen.setWidth(5);
            pen.setColor(Qt::white);
            connect_lines_map[line_id]->setPen(pen);

            scene->addItem(connect_lines_map[line_id]);

        } 

        else 
        {
            vector_connect_node[0]->begin = 0;
            vector_connect_node[1]->begin = 0;
            
            vector_connect_node[0]->end = 0;
            vector_connect_node[1]->end = 0;

            for (int i = 0; i < Viewport::nodes.size(); ++i)
            {
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_current_begin_style);
            }
        }

        vector_connect_node.clear();
    }
}

void Node::connect_nodes_end_f()
{
    if (vector_connect_node.size() == 1)
    {
        if (vector_connect_node[0]->u_id != this->u_id)
        {
            vector_connect_node.push_back(this);
        }
        else
        {
            vector_connect_node.clear();
            for (int i = 0; i < Viewport::nodes.size(); ++i)
            {
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_begin_style);
            }
        }
    }

    else 
    {
        vector_connect_node.push_back(this);

        for (int i = 0; i < Viewport::nodes.size(); ++i)
        {
            if (Viewport::nodes[i] != this && Viewport::nodes[i] != head && Viewport::nodes[i]->prev_node == NULL)
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_current_begin_style);
        }
    }

    if (vector_connect_node.size() == 1 && !is_connected_end_f)
    {
        vector_connect_node[0]->end_f = 1;
    }
    else if (vector_connect_node.size() == 2 && !is_connected_end_f)
    {
        vector_connect_node[1]->end_f = 1;
    }



    if (vector_connect_node.size() == 2)
    {
        if ((vector_connect_node[1]->begin == 2 && vector_connect_node[0]->end_f == 2)
         || (vector_connect_node[0]->begin == 2 && vector_connect_node[1]->end_f == 2))
        {

            if (!is_connected_end_f)
            {
                vector_connect_node[0]->end_f = 0;
                vector_connect_node[1]->end_f = 0;
            }

            for (int i = 0; i < Viewport::nodes.size(); ++i)
            {
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_current_begin_style);
            }
        }

        else if (vector_connect_node[0]->end_f == 1 && vector_connect_node[1]->begin == 1)
        {

            vector_connect_node[0]->next_node_f =  vector_connect_node[1];
            vector_connect_node[1]->prev_node   =  vector_connect_node[0];

            vector_connect_node[1]->begin = 2;
            vector_connect_node[0]->end_f = 2;

            vector_connect_node[1]->is_connected_begin = true;
            vector_connect_node[0]->is_connected_end_f   = true;

            int x1 = vector_connect_node[0]->pos().x();
            int y1 = vector_connect_node[0]->pos().y();

            int x2 = vector_connect_node[1]->pos().x();
            int y2 = vector_connect_node[1]->pos().y();

            line_id++;

            vector_connect_node[0]->right_line_f = line_id;
            vector_connect_node[1]->left_line    = line_id;

            connect_lines_map.insert(line_id, new QGraphicsLineItem(QLineF(x1 + 150, y1 + 30, x2, y2 + 30)));

            pen.setWidth(5);
            pen.setColor(Qt::white);
            connect_lines_map[line_id]->setPen(pen);

            scene->addItem(connect_lines_map[line_id]);

            qDebug() << "line_id: " << line_id;

        } 

        else 
        {
            vector_connect_node[0]->begin = 0;
            vector_connect_node[1]->begin = 0;
            
            vector_connect_node[0]->end_f = 0;
            vector_connect_node[1]->end_f = 0;

            for (int i = 0; i < Viewport::nodes.size(); ++i)
            {
                if (!isGetWithoutStr(Viewport::nodes[i]))
                    Viewport::nodes[i]->btn_begin->setStyleSheet(nodes_styles::btn_current_begin_style);
            }

        }

        vector_connect_node.clear();
    }
}

void Node::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Backspace && thisNode != NULL)
    {
        for (auto it = nodes_map.begin(); it != nodes_map.end(); ++it)
        {
            if (it.value() == this)
            {
                auto k = it.key();
                it = nodes_map.erase(it);
                nodes_map.remove(k);
                _viewport->removeNode(thisNode);
                break;
            }
        }
    }
}

void Node::mouseReleaseEvent(QMouseEvent* event)
{
    if (this->id == NodesId::NODE_OUTPUT ||
        this->id == NodesId::NODE_INPUT  ||
        this->id == NodesId::NODE_ENDLESS_CYCLE ||
        this->id == NodesId::NODE_CHANGE_VALUE ||
        this->id == NodesId::NODE_GLOBAL ||
        this->id == NodesId::NODE_NULL ||
        this->id == NodesId::NODE_BREAK ||
        this->id == NodesId::NODE_ENDL)
    {
        this->setStyleSheet(nodes_styles::any_node_style);
    }

    else if (this->id == NodesId::NODE_INT || this->id == NodesId::NODE_FLOAT || this->id == NodesId::NODE_DOUBLE)
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
    }

    else if (this->id == NodesId::NODE_STRING)
    {
        this->setStyleSheet(nodes_styles::string_node_style);
    }
    
    else if (this->id == NodesId::NODE_BOOL || this->id == NodesId::NODE_BRANCH)
    {
        this->setStyleSheet(nodes_styles::bool_node_style);
    }
    else
    {
        this->setStyleSheet(nodes_styles::digit_node_style);
    }
}

void Node::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) 
    {
        m_mouseStartPosition = event->pos();
        is_widget_move = true;

        if (this->id == NodesId::NODE_OUTPUT ||
            this->id == NodesId::NODE_INPUT  ||
            this->id == NodesId::NODE_ENDLESS_CYCLE ||
            this->id == NodesId::NODE_CHANGE_VALUE ||
            this->id == NodesId::NODE_GLOBAL ||
            this->id == NodesId::NODE_NULL ||
            this->id == NodesId::NODE_BREAK ||
            this->id == NodesId::NODE_ENDL)
        {
            this->setStyleSheet(current_nodes_styles::any_node_current_style);
        }

        else if (this->id == NodesId::NODE_INT || this->id == NodesId::NODE_FLOAT || this->id == NodesId::NODE_DOUBLE)
        {
            this->setStyleSheet(current_nodes_styles::digit_node_current_style);
        }

        else if (this->id == NodesId::NODE_STRING)
        {
            this->setStyleSheet(current_nodes_styles::string_node_current_style);
        }
        
        else if (this->id == NodesId::NODE_BOOL || this->id == NodesId::NODE_BRANCH)
        {
            this->setStyleSheet(current_nodes_styles::bool_node_current_style);
        }
        else
        {
            this->setStyleSheet(current_nodes_styles::digit_node_current_style);
        }

        thisNode = this;
    }
}

void Node::changeNodeSlot()
{
    _viewport->visibleChangeNodeWindow(this->pos().x() + this->width(), this->pos().y(), this);
}

void Node::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && (QWidget::geometry().x() >= 0 && QWidget::geometry().x() <= 10000 && QWidget::geometry().y() >= 0 && QWidget::geometry().y() <= 10000) && is_widget_move)
    {
        QPoint diff = event->pos() - m_mouseStartPosition;
        QWidget::move(this->QWidget::pos() + diff);

        bool check = false;
        for (int i = 0; i < Viewport::nodes.size(); ++i)
        {
            int x = Viewport::nodes[i]->pos().x();
            int y = Viewport::nodes[i]->pos().y();

            if (Viewport::nodes[i]->next_node == this)
            {
                connect_lines_map[Viewport::nodes[i]->right_line]->setLine(x + 150, y + 30, this->pos().x(), this->pos().y() + 30);
            }
            if (Viewport::nodes[i]->prev_node == this)
            {   
                if (this->right_line != Viewport::nodes[i]->left_line)
                    connect_lines_map[Viewport::nodes[i]->left_line]->setLine(this->pos().x() + 150, this->pos().y() + 53, x, y + 30);
                else
                    connect_lines_map[Viewport::nodes[i]->left_line]->setLine(this->pos().x() + 150, this->pos().y() + 30, x, y + 30);
            }
            if (Viewport::nodes[i]->next_node_f == this)
            {
                connect_lines_map[Viewport::nodes[i]->right_line_f]->setLine(x + 150, y + 53, this->pos().x(), this->pos().y() + 30);
            }
            if (Viewport::nodes[i]->prev_node_get_1 == this)
            {
                if (not is_get(this, check))
                    connect_lines_map[Viewport::nodes[i]->left_line_get_1]->setLine(this->pos().x() + 150, this->pos().y() + 53, x, y + 53);
                else
                    connect_lines_map[Viewport::nodes[i]->left_line_get_1]->setLine(this->pos().x() + 75, this->pos().y() + 2, x, y + 50);
            }
            if (Viewport::nodes[i]->prev_node_get_2 == this)
            {
                if (not is_get(this, check))
                    connect_lines_map[Viewport::nodes[i]->left_line_get_2]->setLine(this->pos().x() + 150, this->pos().y() + 53, x, y + 78);
                else
                    connect_lines_map[Viewport::nodes[i]->left_line_get_2]->setLine(this->pos().x() + 75, this->pos().y() + 2, x, y + 78);
            }
            if (Viewport::nodes[i]->ptr_return_value == this && !is_get(Viewport::nodes[i], check))
            {
                if (this->prev_node_get_1 == Viewport::nodes[i])
                    connect_lines_map[Viewport::nodes[i]->right_line_get]->setLine(x + 150, y + 53, this->pos().x(), this->pos().y() + 53);
                else
                    connect_lines_map[Viewport::nodes[i]->right_line_get]->setLine(x + 150, y + 53, this->pos().x(), this->pos().y() + 78);
            }
            if (this->prev_node_get_1 == Viewport::nodes[i] && is_get(Viewport::nodes[i], check))
            {
                connect_lines_map[this->left_line_get_1]->setLine(x + 75, y + 2, this->pos().x(), this->pos().y() + 50);
            }
            if (this->prev_node_get_2 == Viewport::nodes[i] && is_get(Viewport::nodes[i], check))
            {
                connect_lines_map[this->left_line_get_2]->setLine(x + 75, y + 2, this->pos().x(), this->pos().y() + 78);
            }
        }
    }
    else
    {
        if (QWidget::geometry().x() < 0) QWidget::move(QWidget::geometry().x() + 5, QWidget::geometry().y());
        else if (QWidget::geometry().y() < 0) QWidget::move(QWidget::geometry().x(), QWidget::geometry().y() + 5);
        else if (QWidget::geometry().x() > 10000) QWidget::move(QWidget::geometry().x() - 5, QWidget::geometry().y());
        else if (QWidget::geometry().y() > 10000) QWidget::move(QWidget::geometry().x(), QWidget::geometry().y() - 5);
        is_widget_move = false;
    }
}

void Node::connect_get_lines()
{
    int line_coor_x = this->pos().x();
    int line_coor_y = this->pos().y();

    if (this->prev_node_get_1 != NULL && this->prev_node_get_2 != NULL && this->ptr_return_value != NULL)
    {
        int x3 = this->prev_node_get_1->pos().x();
        int y3 = this->prev_node_get_1->pos().y();

        int x4 = this->ptr_return_value->pos().x();
        int y4 = this->ptr_return_value->pos().y();

        int x5 = this->prev_node_get_2->pos().x();
        int y5 = this->prev_node_get_2->pos().y();

        bool is_get_1 = false;
        bool is_get_2 = false;

        if (this->prev_node_get_1->id == NodesId::NODE_INT    ||
            this->prev_node_get_1->id == NodesId::NODE_FLOAT  ||
            this->prev_node_get_1->id == NodesId::NODE_DOUBLE ||
            this->prev_node_get_1->id == NodesId::NODE_STRING ||
            this->prev_node_get_1->id == NodesId::NODE_BOOL)
            is_get_1 = true;

        if (this->prev_node_get_2->id == NodesId::NODE_INT    ||
            this->prev_node_get_2->id == NodesId::NODE_FLOAT  ||
            this->prev_node_get_2->id == NodesId::NODE_DOUBLE ||
            this->prev_node_get_2->id == NodesId::NODE_STRING ||
            this->prev_node_get_2->id == NodesId::NODE_BOOL)
            is_get_2 = true;


        if (this->right_line_get == this->ptr_return_value->left_line_get_1)
            connect_lines_map[this->right_line_get]->setLine(line_coor_x + 150, line_coor_y + 53, x4, y4 + 53);
        else
            connect_lines_map[this->right_line_get]->setLine(line_coor_x + 150, line_coor_y + 53, x4, y4 + 78);

        if (this->left_line_get_1 == this->prev_node_get_1->right_line_get && !is_get_1)
            connect_lines_map[this->left_line_get_1]->setLine(line_coor_x, line_coor_y + 53, x3 + 150, y3 + 53);
        else
            connect_lines_map[this->left_line_get_1]->setLine(line_coor_x, line_coor_y + 50, x3 + 75, y3 + 2);

        if (!is_get_2)
            connect_lines_map[this->left_line_get_2]->setLine(line_coor_x, line_coor_y + 78, x5 + 150, y5 + 53);
        else
            connect_lines_map[this->left_line_get_2]->setLine(line_coor_x, line_coor_y + 78, x5 + 75, y5 + 2);
    }

    else if (this->prev_node_get_2 != NULL && this->ptr_return_value != NULL)
    {
        int x3 = this->prev_node_get_2->pos().x();
        int y3 = this->prev_node_get_2->pos().y();

        int x4 = this->ptr_return_value->pos().x();
        int y4 = this->ptr_return_value->pos().y();

        bool is_get_2 = false;

        if (this->prev_node_get_2->id == NodesId::NODE_INT    ||
            this->prev_node_get_2->id == NodesId::NODE_FLOAT  ||
            this->prev_node_get_2->id == NodesId::NODE_DOUBLE ||
            this->prev_node_get_2->id == NodesId::NODE_STRING ||
            this->prev_node_get_2->id == NodesId::NODE_BOOL)
            is_get_2 = true;

        if (this->right_line_get == this->ptr_return_value->left_line_get_1)
            connect_lines_map[this->right_line_get]->setLine(line_coor_x + 150, line_coor_y + 53, x4, y4 + 53);
        
        else
            connect_lines_map[this->right_line_get]->setLine(line_coor_x + 150, line_coor_y + 53, x4, y4 + 78);

        if (!is_get_2)
            connect_lines_map[this->left_line_get_2]->setLine(line_coor_x, line_coor_y + 78, x3 + 150, y3 + 53);
        else
            connect_lines_map[this->left_line_get_2]->setLine(line_coor_x, line_coor_y + 78, x3 + 75, y3 + 2);
    }

    else if (this->prev_node_get_1 != NULL && this->ptr_return_value != NULL)
    {
        int x3 = this->prev_node_get_1->pos().x();
        int y3 = this->prev_node_get_1->pos().y();

        int x4 = this->ptr_return_value->pos().x();
        int y4 = this->ptr_return_value->pos().y();

        bool is_get_1 = false;

        if (this->prev_node_get_1->id == NodesId::NODE_INT    ||
            this->prev_node_get_1->id == NodesId::NODE_FLOAT  ||
            this->prev_node_get_1->id == NodesId::NODE_DOUBLE ||
            this->prev_node_get_1->id == NodesId::NODE_STRING ||
            this->prev_node_get_1->id == NodesId::NODE_BOOL)
            is_get_1 = true;

        if (this->right_line_get == this->ptr_return_value->left_line_get_1)
            connect_lines_map[this->right_line_get]->setLine(line_coor_x + 150, line_coor_y + 53, x4, y4 + 53);
        
        else
            connect_lines_map[this->right_line_get]->setLine(line_coor_x + 150, line_coor_y + 53, x4, y4 + 78);

        if (this->left_line_get_1 == this->prev_node_get_1->right_line_get && !is_get_1)
            connect_lines_map[this->left_line_get_1]->setLine(line_coor_x, line_coor_y + 53, x3 + 150, y3 + 53);
        else
            connect_lines_map[this->left_line_get_1]->setLine(line_coor_x, line_coor_y + 50, x3 + 75, y3 + 2);
    }

    else if (this->prev_node_get_1 != NULL && this->prev_node_get_2 != NULL)
    {
        int x3 = this->prev_node_get_1->pos().x();
        int y3 = this->prev_node_get_1->pos().y();

        int x4 = this->prev_node_get_2->pos().x();
        int y4 = this->prev_node_get_2->pos().y();

        bool is_get_1 = false;
        bool is_get_2 = false;

        if (this->prev_node_get_1->id == NodesId::NODE_INT    ||
            this->prev_node_get_1->id == NodesId::NODE_FLOAT  ||
            this->prev_node_get_1->id == NodesId::NODE_DOUBLE ||
            this->prev_node_get_1->id == NodesId::NODE_STRING ||
            this->prev_node_get_1->id == NodesId::NODE_BOOL)
            is_get_1 = true;

        if (this->prev_node_get_2->id == NodesId::NODE_INT    ||
            this->prev_node_get_2->id == NodesId::NODE_FLOAT  ||
            this->prev_node_get_2->id == NodesId::NODE_DOUBLE ||
            this->prev_node_get_2->id == NodesId::NODE_STRING ||
            this->prev_node_get_2->id == NodesId::NODE_BOOL)
            is_get_2 = true;

        if (this->left_line_get_1 == this->prev_node_get_1->right_line_get && !is_get_1)
            connect_lines_map[this->left_line_get_1]->setLine(line_coor_x, line_coor_y + 53, x3 + 150, y3 + 53);
        else
            connect_lines_map[this->left_line_get_1]->setLine(line_coor_x, line_coor_y + 50, x3 + 75, y3 + 2);

        if (!is_get_2)
            connect_lines_map[this->left_line_get_2]->setLine(line_coor_x, line_coor_y + 78, x4 + 150, y4 + 53);
        else
            connect_lines_map[this->left_line_get_2]->setLine(line_coor_x, line_coor_y + 78, x4 + 75, y4 + 2);
    }

    else if (this->ptr_return_value != NULL)
    {
        int x3 = this->ptr_return_value->pos().x();
        int y3 = this->ptr_return_value->pos().y();

        if (this->right_line_get == this->ptr_return_value->left_line_get_1)
            connect_lines_map[this->right_line_get]->setLine(line_coor_x + 150, line_coor_y + 53, x3, y3 + 53);
        
        else
            connect_lines_map[this->right_line_get]->setLine(line_coor_x + 150, line_coor_y + 53, x3, y3 + 78);
    }

    else if (this->prev_node_get_1 != NULL)
    {
        int x3 = this->prev_node_get_1->pos().x();
        int y3 = this->prev_node_get_1->pos().y();

        bool is_get = false;

        if (this->prev_node_get_1->id == NodesId::NODE_INT    ||
            this->prev_node_get_1->id == NodesId::NODE_FLOAT  ||
            this->prev_node_get_1->id == NodesId::NODE_DOUBLE ||
            this->prev_node_get_1->id == NodesId::NODE_STRING ||
            this->prev_node_get_1->id == NodesId::NODE_BOOL)
            is_get = true;

        if (this->left_line_get_1 == this->prev_node_get_1->right_line_get && !is_get)
            connect_lines_map[this->left_line_get_1]->setLine(line_coor_x, line_coor_y + 53, x3 + 150, y3 + 53);

        else
            connect_lines_map[this->left_line_get_1]->setLine(line_coor_x, line_coor_y + 50, x3 + 75, y3 + 2);
    }

    else if (this->prev_node_get_2 != NULL)
    {
        int x3 = this->prev_node_get_2->pos().x();
        int y3 = this->prev_node_get_2->pos().y();

        bool is_get = false;

        if (this->prev_node_get_2->id == NodesId::NODE_INT    ||
            this->prev_node_get_2->id == NodesId::NODE_FLOAT  ||
            this->prev_node_get_2->id == NodesId::NODE_DOUBLE ||
            this->prev_node_get_2->id == NodesId::NODE_STRING ||
            this->prev_node_get_2->id == NodesId::NODE_BOOL)
            is_get = true;

        if (this->left_line_get_2 == this->prev_node_get_2->right_line_get && !is_get)
            connect_lines_map[this->left_line_get_2]->setLine(line_coor_x, line_coor_y + 78, x3 + 150, y3 + 53);
        else
            connect_lines_map[this->left_line_get_2]->setLine(line_coor_x, line_coor_y + 78, x3 + 75, y3 + 2);
    }
}

void Node::save_nodes_slot()
{
    QFile file(Node::path_txt);
    
    current_node = head;

    if (file.open(QFile::WriteOnly | QFile::Truncate))
    {
        file.close();
    }

    
    if (file.open(QIODevice::Append))
    {
        file.write("#include <iostream>\n#include <cmath>\n\n\nint main()\n{\n");
        file.close();
    }
    else
    {
        MainWindow::createFiles();
        return;
    }

    var_names.clear();

    var_name_value = 0;
    tab = 1;

    Node::save_nodes();
    file.open(QIODevice::Append);
    file.write("\n\treturn 0;\n}");
    file.close();

    MainWindow::initMessageWindow("Сохранено", "green");

    if (Functional::codeTextBrowserWindow)
    {
        file.open(QIODevice::ReadOnly);
        Functional::codeTextBrowserWindow->setText(file.readAll());
        file.close();
    }
}

void Node::save_nodes()
{
    if (head == NULL) return;

    while (current_node->next_node != NULL)
    {
        qDebug() << "id: " << current_node->id << "\tu_id: " << current_node->u_id;

        if (current_node->id != NodesId::NODE_BRANCH && current_node->id != NodesId::NODE_INPUT && current_node->id != NodesId::NODE_OUTPUT && current_node->id != NodesId::NODE_GLOBAL && current_node->id != NodesId::NODE_NULL && current_node->id != NodesId::NODE_ENDLESS_CYCLE && current_node->id != NodesId::NODE_BREAK && current_node->id != NodesId::NODE_ENDL)
        {
            if (current_node->prev_node_get_1 == NULL || current_node->prev_node_get_2 == NULL) return;
        }

        if (current_node->id == NodesId::NODE_INPUT || current_node->id == NODE_OUTPUT || current_node->id == NODE_GLOBAL)
        {
            if (current_node->prev_node_get_1 == NULL) return;
        }

        if (current_node->id == NodesId::NODE_BRANCH)
        {
            if (current_node->prev_node_get_1 == NULL) return;


            Node* prev_node = current_node->prev_node_get_1;
            bool _check = false;
            bool is_prev_node_1_get = Node::is_get(prev_node, _check);

            if (current_node->next_node != NULL)
            {
                QString _tab = "\t";
                QString tabs = _tab.repeated(tab);

                QString var_name  = current_node->prev_node_get_1->var_name;
                QString var_value = current_node->prev_node_get_1->var_value;

                QFile file(Node::path_txt);
                file.open(QIODevice::Append);

                if (!check(var_name) && is_prev_node_1_get)
                {
                    file.write("\n" + tabs.toUtf8() + "bool " + var_name.toUtf8() + " = " + var_value.toUtf8() + ";");
                    var_names.push_back(var_name);
                    file.write("\n" + tabs.toUtf8() + "if (" + var_name.toUtf8() + ")\n" + tabs.toUtf8() + "{");
                }
                else if (!is_prev_node_1_get)
                {
                    file.write("\n" + tabs.toUtf8() + "if (" + current_node->prev_node->r_name.toUtf8() + ")\n" + tabs.toUtf8() + "{");
                }
                else
                {
                    file.write("\n" + tabs.toUtf8() + "if (" + var_name.toUtf8() + ")\n" + tabs.toUtf8() + "{");
                }
                file.close();

                tab++;

                Node* mem_ptr = current_node;

                current_node = current_node->next_node;

                save_nodes();

                file.open(QIODevice::Append);
                file.write("\n" + tabs.toUtf8() + "}\n");
                file.close();

                current_node = mem_ptr;

                if (current_node->next_node_f != NULL)
                {
                    tab--;

                    file.open(QIODevice::Append);
                    file.write(tabs.toUtf8() + "else\n" + tabs.toUtf8() + "{");
                    file.close();

                    current_node = current_node->next_node_f;
                    
                    tab++;

                    save_nodes();

                    file.open(QIODevice::Append);
                    file.write("\n" + tabs.toUtf8() + "}");
                    file.close();

                    tab--;

                    return;
                }
                else return;
            }
            else return;
        }

        if (current_node->id == NodesId::NODE_ENDLESS_CYCLE)
        {
            if (current_node->next_node_f == NULL || current_node->next_node == NULL) return;

            QString _tab = "\t";
            QString tabs = _tab.repeated(tab);

            QFile file(path_txt);
            file.open(QIODevice::Append);
            file.write("\n" + tabs.toUtf8() + "while (1)\n" + tabs.toUtf8() + "{");
            file.close();

            tab++;

            Node* mem_ptr = current_node;

            current_node = current_node->next_node_f;

            save_nodes();

            tab--;

            file.open(QIODevice::Append);
            file.write("\n" + tabs.toUtf8() + "}");
            file.close();

            current_node = mem_ptr;

            current_node = current_node->next_node;

            save_nodes();

            return;
        }

        else
        {
            Node::nodes_to_cpp(current_node);
            current_node = current_node->next_node;
        }
    }

    if (current_node->id != NodesId::NODE_BRANCH && current_node->id != NodesId::NODE_INPUT && current_node->id != NodesId::NODE_OUTPUT && current_node->id != NodesId::NODE_GLOBAL && current_node->id != NodesId::NODE_NULL && current_node->id != NodesId::NODE_ENDLESS_CYCLE && current_node->id != NodesId::NODE_BREAK && current_node->id != NodesId::NODE_ENDL)
    {
        if (current_node->prev_node_get_1 == NULL || current_node->prev_node_get_2 == NULL) return;
    }

    if (current_node->id == NodesId::NODE_INPUT || current_node->id == NODE_OUTPUT || current_node->id == NodesId::NODE_BRANCH || current_node->id == NodesId::NODE_GLOBAL)
    {
        if (current_node->prev_node_get_1 == NULL) return;
    }

    Node::nodes_to_cpp(current_node);
    qDebug() << "id: " << current_node->id << "\tu_id: " << current_node->u_id;
}

void Node::nodes_to_cpp(Node* node)
{
    QFile file(Node::path_txt);

    Node* prev_node_1;
    Node* prev_node_2;
    QString line;
    
    bool is_get_node_str = false;
    bool is_get_node_str_2 = false;


    QString _tab = "\t";
    QString tabs = _tab.repeated(tab);

    QString quotation_mark = "\"";


    file.open(QIODevice::Append);

    if (node->id == NodesId::NODE_OUTPUT)
    {
        prev_node_1       = node->prev_node_get_1;
        QString var_name  = prev_node_1->var_name;
        QString var_value;
        QString var_type  = Node::type_node(prev_node_1);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);

        if (is_prev_node_1_get)
        {
            if (var_type == "bool")
            {
                var_value = prev_node_1->var_value;
            }
            else
                var_value = prev_node_1->node_value->text();
        }


        if (!check(var_name) && is_prev_node_1_get)
        {
            if (is_get_node_str)
            {
                file.write("\n" + tabs.toUtf8() + var_type.toUtf8() + " " + var_name.toUtf8() + " = " + quotation_mark.toUtf8() + var_value.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str = false;
            }
            else 
                file.write("\n" + tabs.toUtf8() + var_type.toUtf8() + " " + var_name.toUtf8() + " = " + var_value.toUtf8() + ";");
            var_names.push_back(var_name);
            line = "\n" + tabs.toUtf8() + "std::cout << " + (QString) var_name + ";";
            file.write(line.toUtf8());
        }

        else if (!is_prev_node_1_get)
        {
            line = "\n" + tabs.toUtf8() + "std::cout << " + node->prev_node->r_name + ";";
            file.write(line.toUtf8());
        }

        else
        {
            line = "\n" + tabs.toUtf8() + "std::cout << " + (QString) var_name + ";";
            file.write(line.toUtf8());
        }
    }

    else if (node->id == NodesId::NODE_INPUT)
    {
        prev_node_1 = node->prev_node_get_1;
        QString var_name  = prev_node_1->var_name;
        QString var_value;
        QString var_type  = Node::type_node(prev_node_1);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);

        if (is_prev_node_1_get)
        {
            if (var_type == "bool")
            {
                var_value = prev_node_1->var_value;
            }
            else
                var_value = prev_node_1->node_value->text();
        }

        if (!check(var_name) && is_prev_node_1_get)
        {
            if (is_get_node_str)
            {
                file.write("\n" + tabs.toUtf8() + var_type.toUtf8() + " " + var_name.toUtf8() + " = " + quotation_mark.toUtf8() + var_value.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str = false;
            }
            else 
                file.write("\n" + tabs.toUtf8() + var_type.toUtf8() + " " + var_name.toUtf8() + " = " + var_value.toUtf8() + ";");
            var_names.push_back(var_name);
            line = "\n" + tabs.toUtf8() + "std::cin >> " + (QString) var_name + ";";                              
            file.write(line.toUtf8());
        }

        else if (!is_prev_node_1_get)
        {
            line = "\n" + tabs.toUtf8() + "std::cin >> " + node->prev_node->r_name + ";";
            file.write(line.toUtf8());
        }

        else
        {
            line = "\n" + tabs.toUtf8() + "std::cin >> " + (QString) var_name + ";";                              
            file.write(line.toUtf8());
        }
    }

    else if (node->id == NodesId::NODE_BRANCH)
    {
        tab++;
    }

    else if (node->id == NodesId::NODE_CHANGE_VALUE)
    {
        prev_node_1             = node->prev_node_get_1;
        QString var_name_1      = prev_node_1->var_name;
        QString var_value_1     = prev_node_1->var_value;
        QString var_type_1      = Node::type_node(prev_node_1);
        QString var_type_set_1  = Node::type_set_node(prev_node_1);
        

        prev_node_2             = node->prev_node_get_2;
        QString var_name_2      = prev_node_2->var_name;
        QString var_value_2     = prev_node_2->var_value;
        QString var_type_2      = Node::type_node(prev_node_2);
        QString var_type_set_2  = Node::type_set_node(prev_node_2);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);
        bool is_prev_node_2_get = Node::is_get(prev_node_2, is_get_node_str);

        if (is_prev_node_2_get && var_type_set_1 == var_type_set_2)
        {
            if (!check(var_name_1))
            {
                if (is_prev_node_1_get)
                {
                    file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + var_value_1.toUtf8() + ";");
                    var_names.push_back(var_name_1);
                }
            }

            if (!check(var_name_2))
            {
                file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + var_value_2.toUtf8() + ";");
                var_names.push_back(var_name_2);
            }

            if (is_prev_node_1_get)
            {
                line = "\n" + tabs.toUtf8() + var_name_2 + " = " + var_name_1 + ";";                              
                file.write(line.toUtf8());
            }
            else if (!is_prev_node_1_get)
            {
                line = "\n" + tabs.toUtf8() + var_name_2 + " = " + prev_node_1->r_name + ";";                              
                file.write(line.toUtf8());
            }
        }
    }

    else if (node->id == NodesId::NODE_GLOBAL)
    {
        prev_node_1       = node->prev_node_get_1;
        QString var_name  = prev_node_1->var_name;
        QString var_value;
        QString var_type  = Node::type_node(prev_node_1);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);


        if (var_type == "bool")
        {
            var_value = prev_node_1->var_value;
        }
        else
            var_value = prev_node_1->node_value->text();

        if (!check(var_name) && is_prev_node_1_get)
        {
            if (is_get_node_str)
            {
                file.write("\n" + tabs.toUtf8() + var_type.toUtf8() + " " + var_name.toUtf8() + " = " + quotation_mark.toUtf8() + var_value.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str = false;
            }
            else 
                file.write("\n" + tabs.toUtf8() + var_type.toUtf8() + " " + var_name.toUtf8() + " = " + var_value.toUtf8() + ";");
            var_names.push_back(var_name);
        }
    }

    else if (node->id == NodesId::NODE_NULL)
    {
        line = "\n" + tabs.toUtf8() + ";";                              
        file.write(line.toUtf8());
    }

    else if (node->id == NodesId::NODE_ENDL)
    {
        line = "\n" + tabs.toUtf8() + "std::cout << std::endl;";                              
        file.write(line.toUtf8());
    }

    else if (node->id == NodesId::NODE_BREAK)
    {
        line = "\n" + tabs.toUtf8() + "break;";                              
        file.write(line.toUtf8());
    }

    else if (node->id == NodesId::NODE_OPERATOR_P)
    {
        prev_node_1         = node->prev_node_get_1;
        QString var_name_1  = prev_node_1->var_name;
        QString var_value_1 = prev_node_1->var_value;
        QString var_type_1  = Node::type_node(prev_node_1);

        prev_node_2         = node->prev_node_get_2;
        QString var_name_2  = prev_node_2->var_name;
        QString var_value_2 = prev_node_2->var_value;
        QString var_type_2  = Node::type_node(prev_node_2);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);
        bool is_prev_node_2_get = Node::is_get(prev_node_2, is_get_node_str);

        if (!check(var_name_1) && is_prev_node_1_get)
        {
            file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + var_value_1.toUtf8() + ";");
            var_names.push_back(var_name_1);
        }

        if (!check(var_name_2) && is_prev_node_2_get)
        {
            file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + var_value_2.toUtf8() + ";");
            var_names.push_back(var_name_2);
        }

        if (!is_prev_node_1_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name +  " = " + node->prev_node->r_name + " + " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }

        if (!is_prev_node_2_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name +  " = " + (QString) var_name_1 + " + " + node->prev_node->r_name + ";";
            file.write(line.toUtf8());
        }


        if (is_prev_node_1_get && is_prev_node_2_get)
        {
            bool ok1, ok2;
            double double_value_1 = var_value_1.toDouble(&ok1);
            double double_value_2 = var_value_2.toDouble(&ok2);

            if (ok1 && ok2)
            {
                node->r_double = (double) double_value_1 + double_value_2;
            }

            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name + " = " + (QString) var_name_1 + " + " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }
    }

    else if (node->id == NodesId::NODE_OPERATOR_M)
    {
        prev_node_1         = node->prev_node_get_1;
        QString var_name_1  = prev_node_1->var_name;
        QString var_value_1 = prev_node_1->var_value;
        QString var_type_1  = Node::type_node(prev_node_1);

        prev_node_2         = node->prev_node_get_2;
        QString var_name_2  = prev_node_2->var_name;
        QString var_value_2 = prev_node_2->var_value;
        QString var_type_2  = Node::type_node(prev_node_2);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);
        bool is_prev_node_2_get = Node::is_get(prev_node_2, is_get_node_str);

        if (!check(var_name_1) && is_prev_node_1_get)
        {
            file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + var_value_1.toUtf8() + ";");
            var_names.push_back(var_name_1);
        }

        if (!check(var_name_2) && is_prev_node_2_get)
        {
            file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + var_value_2.toUtf8() + ";");
            var_names.push_back(var_name_2);
        }

        if (!is_prev_node_1_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name +  " = " + node->prev_node->r_name + " - " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }

        if (!is_prev_node_2_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name +  " = " + (QString) var_name_1 + " - " + node->prev_node->r_name + ";";
            file.write(line.toUtf8());
        }


        if (is_prev_node_1_get && is_prev_node_2_get)
        {
            bool ok1, ok2;
            double double_value_1 = var_value_1.toDouble(&ok1);
            double double_value_2 = var_value_2.toDouble(&ok2);

            if (ok1 && ok2)
            {
                node->r_double = (double) double_value_1 - double_value_2;
            }

            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name + " = " + (QString) var_name_1 + " - " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }
    }

    else if (node->id == NodesId::NODE_OPERATOR_MULT)
    {
        prev_node_1         = node->prev_node_get_1;
        QString var_name_1  = prev_node_1->var_name;
        QString var_value_1 = prev_node_1->var_value;
        QString var_type_1  = Node::type_node(prev_node_1);

        prev_node_2         = node->prev_node_get_2;
        QString var_name_2  = prev_node_2->var_name;
        QString var_value_2 = prev_node_2->var_value;
        QString var_type_2  = Node::type_node(prev_node_2);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);
        bool is_prev_node_2_get = Node::is_get(prev_node_2, is_get_node_str);

        if (!check(var_name_1) && is_prev_node_1_get)
        {
            file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + var_value_1.toUtf8() + ";");
            var_names.push_back(var_name_1);
        }

        if (!check(var_name_2) && is_prev_node_2_get)
        {
            file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + var_value_2.toUtf8() + ";");
            var_names.push_back(var_name_2);
        }

        if (!is_prev_node_1_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name +  " = " + node->prev_node->r_name + " * " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }

        if (!is_prev_node_2_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name +  " = " + (QString) var_name_1 + " * " + node->prev_node->r_name + ";";
            file.write(line.toUtf8());
        }


        if (is_prev_node_1_get && is_prev_node_2_get)
        {
            bool ok1, ok2;
            double double_value_1 = var_value_1.toDouble(&ok1);
            double double_value_2 = var_value_2.toDouble(&ok2);

            if (ok1 && ok2)
            {
                node->r_double = (double) double_value_1 * double_value_2;
            }

            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name + " = " + (QString) var_name_1 + " * " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }
    }

    else if (node->id == NodesId::NODE_OPERATOR_DIV)
    {
        prev_node_1         = node->prev_node_get_1;
        QString var_name_1  = prev_node_1->var_name;
        QString var_value_1 = prev_node_1->var_value;
        QString var_type_1  = Node::type_node(prev_node_1);

        prev_node_2         = node->prev_node_get_2;
        QString var_name_2  = prev_node_2->var_name;
        QString var_value_2 = prev_node_2->var_value;
        QString var_type_2  = Node::type_node(prev_node_2);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);
        bool is_prev_node_2_get = Node::is_get(prev_node_2, is_get_node_str);

        if (!check(var_name_1) && is_prev_node_1_get)
        {
            file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + var_value_1.toUtf8() + ";");
            var_names.push_back(var_name_1);
        }

        if (!check(var_name_2) && is_prev_node_2_get)
        {
            file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + var_value_2.toUtf8() + ";");
            var_names.push_back(var_name_2);
        }

        if (!is_prev_node_1_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name +  " = " + node->prev_node->r_name + " / " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }

        if (!is_prev_node_2_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name +  " = " + (QString) var_name_1 + " / " + node->prev_node->r_name + ";";
            file.write(line.toUtf8());
        }


        if (is_prev_node_1_get && is_prev_node_2_get)
        {
            bool ok1, ok2;
            double double_value_1 = var_value_1.toDouble(&ok1);
            double double_value_2 = var_value_2.toDouble(&ok2);

            if (ok1 && ok2)
            {
                node->r_double = (double) double_value_1 / double_value_2;
            }

            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name + " = " + (QString) var_name_1 + " / " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }
    }

    else if (node->id == NodesId::NODE_OPERATOR_PERCENT)
    {
        prev_node_1         = node->prev_node_get_1;
        QString var_name_1  = prev_node_1->var_name;
        QString var_value_1 = prev_node_1->var_value;
        QString var_type_1  = Node::type_node(prev_node_1);

        prev_node_2         = node->prev_node_get_2;
        QString var_name_2  = prev_node_2->var_name;
        QString var_value_2 = prev_node_2->var_value;
        QString var_type_2  = Node::type_node(prev_node_2);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);
        bool is_prev_node_2_get = Node::is_get(prev_node_2, is_get_node_str);

        if (!check(var_name_1) && is_prev_node_1_get)
        {
            file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + var_value_1.toUtf8() + ";");
            var_names.push_back(var_name_1);
        }

        if (!check(var_name_2) && is_prev_node_2_get)
        {
            file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + var_value_2.toUtf8() + ";");
            var_names.push_back(var_name_2);
        }

        if (!is_prev_node_1_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name + " = " + "fmod(" + node->prev_node->r_name + ", " + (QString) var_name_2 + ");";
            file.write(line.toUtf8());
        }

        if (!is_prev_node_2_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name + " = " + "fmod(" + (QString) var_name_1 + ", " + node->prev_node->r_name + ");";
            file.write(line.toUtf8());
        }


        if (is_prev_node_1_get && is_prev_node_2_get)
        {
            bool ok1, ok2;
            double double_value_1 = var_value_1.toDouble(&ok1);
            double double_value_2 = var_value_2.toDouble(&ok2);

            if (ok1 && ok2)
            {
                node->r_double = (double) fmod(double_value_1, double_value_2);
            }

            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "double " + r_var_name + " = " + "fmod(" + (QString) var_name_1 + ", " + (QString) var_name_2 + ");";
            file.write(line.toUtf8());
        }
    }

    else if (node->id == NodesId::NODE_OPERATOR_GREAT)
    {
        prev_node_1         = node->prev_node_get_1;
        QString var_name_1  = prev_node_1->var_name;
        QString var_value_1 = prev_node_1->var_value;
        QString var_type_1  = Node::type_node(prev_node_1);

        prev_node_2         = node->prev_node_get_2;
        QString var_name_2  = prev_node_2->var_name;
        QString var_value_2 = prev_node_2->var_value;
        QString var_type_2  = Node::type_node(prev_node_2);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);
        bool is_prev_node_2_get = Node::is_get(prev_node_2, is_get_node_str);

        if (is_prev_node_1_get)
        {
            if (var_type_1 == "bool")
            {
                var_value_1 = prev_node_1->var_value;
            }
            else
                var_value_1 = prev_node_1->node_value->text();
        }

        if (is_prev_node_2_get)
        {
            if (var_type_2 == "bool")
            {
                var_value_2 = prev_node_2->var_value;
            }
            else
                var_value_2 = prev_node_2->node_value->text();
        }


        if (!check(var_name_1) && is_prev_node_1_get)
        {
            if (is_get_node_str)
            {
                file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + quotation_mark.toUtf8() + var_value_1.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str = false;
            }
            else
                file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + var_value_1.toUtf8() + ";");
            var_names.push_back(var_name_1);
        }

        if (!check(var_name_2) && is_prev_node_2_get)
        {
            if (is_get_node_str_2)
            {
                file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + quotation_mark.toUtf8() + var_value_2.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str_2 = false;
            }
            else
                file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + var_value_2.toUtf8() + ";");
            var_names.push_back(var_name_2);
        }

        if (!is_prev_node_1_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name +  " = " + node->prev_node->r_name + " > " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }

        if (!is_prev_node_2_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name +  " = " + (QString) var_name_1 + " > " + node->prev_node->r_name + ";";
            file.write(line.toUtf8());
        }


        if (is_prev_node_1_get && is_prev_node_2_get)
        {
            bool ok1, ok2;
            double double_value_1 = var_value_1.toDouble(&ok1);
            double double_value_2 = var_value_2.toDouble(&ok2);

            if (ok1 && ok2)
            {
                node->r_bool = (bool) double_value_1 > double_value_2;
            }

            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name + " = " + (QString) var_name_1 + " > " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }
    }

    else if (node->id == NodesId::NODE_OPERATOR_LESS)
    {
        prev_node_1         = node->prev_node_get_1;
        QString var_name_1  = prev_node_1->var_name;
        QString var_value_1 = prev_node_1->var_value;
        QString var_type_1  = Node::type_node(prev_node_1);

        prev_node_2         = node->prev_node_get_2;
        QString var_name_2  = prev_node_2->var_name;
        QString var_value_2 = prev_node_2->var_value;
        QString var_type_2  = Node::type_node(prev_node_2);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);
        bool is_prev_node_2_get = Node::is_get(prev_node_2, is_get_node_str);

        if (is_prev_node_1_get)
        {
            if (var_type_1 == "bool")
            {
                var_value_1 = prev_node_1->var_value;
            }
            else
                var_value_1 = prev_node_1->node_value->text();
        }

        if (is_prev_node_2_get)
        {
            if (var_type_2 == "bool")
            {
                var_value_2 = prev_node_2->var_value;
            }
            else
                var_value_2 = prev_node_2->node_value->text();
        }


        if (!check(var_name_1) && is_prev_node_1_get)
        {
            if (is_get_node_str)
            {
                file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + quotation_mark.toUtf8() + var_value_1.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str = false;
            }
            else
                file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + var_value_1.toUtf8() + ";");
            var_names.push_back(var_name_1);
        }

        if (!check(var_name_2) && is_prev_node_2_get)
        {
            if (is_get_node_str_2)
            {
                file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + quotation_mark.toUtf8() + var_value_2.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str_2 = false;
            }
            else
                file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + var_value_2.toUtf8() + ";");
            var_names.push_back(var_name_2);
        }

        if (!is_prev_node_1_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name +  " = " + node->prev_node->r_name + " < " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }

        if (!is_prev_node_2_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name +  " = " + (QString) var_name_1 + " < " + node->prev_node->r_name + ";";
            file.write(line.toUtf8());
        }


        if (is_prev_node_1_get && is_prev_node_2_get)
        {
            bool ok1, ok2;
            double double_value_1 = var_value_1.toDouble(&ok1);
            double double_value_2 = var_value_2.toDouble(&ok2);

            if (ok1 && ok2)
            {
                node->r_bool = (bool) double_value_1 < double_value_2;
            }

            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name + " = " + (QString) var_name_1 + " < " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }
    }

    else if (node->id == NodesId::NODE_OPERATOR_GREAT_OR_EQUAL)
    {
        prev_node_1         = node->prev_node_get_1;
        QString var_name_1  = prev_node_1->var_name;
        QString var_value_1 = prev_node_1->var_value;
        QString var_type_1  = Node::type_node(prev_node_1);

        prev_node_2         = node->prev_node_get_2;
        QString var_name_2  = prev_node_2->var_name;
        QString var_value_2 = prev_node_2->var_value;
        QString var_type_2  = Node::type_node(prev_node_2);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);
        bool is_prev_node_2_get = Node::is_get(prev_node_2, is_get_node_str);

        if (is_prev_node_1_get)
        {
            if (var_type_1 == "bool")
            {
                var_value_1 = prev_node_1->var_value;
            }
            else
                var_value_1 = prev_node_1->node_value->text();
        }

        if (is_prev_node_2_get)
        {
            if (var_type_2 == "bool")
            {
                var_value_2 = prev_node_2->var_value;
            }
            else
                var_value_2 = prev_node_2->node_value->text();
        }


        if (!check(var_name_1) && is_prev_node_1_get)
        {
            if (is_get_node_str)
            {
                file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + quotation_mark.toUtf8() + var_value_1.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str = false;
            }
            else
                file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + var_value_1.toUtf8() + ";");
            var_names.push_back(var_name_1);
        }

        if (!check(var_name_2) && is_prev_node_2_get)
        {
            if (is_get_node_str_2)
            {
                file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + quotation_mark.toUtf8() + var_value_2.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str_2 = false;
            }
            else
                file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + var_value_2.toUtf8() + ";");
            var_names.push_back(var_name_2);
        }

        if (!is_prev_node_1_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name +  " = " + node->prev_node->r_name + " >= " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }

        if (!is_prev_node_2_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name +  " = " + (QString) var_name_1 + " >= " + node->prev_node->r_name + ";";
            file.write(line.toUtf8());
        }


        if (is_prev_node_1_get && is_prev_node_2_get)
        {
            bool ok1, ok2;
            double double_value_1 = var_value_1.toDouble(&ok1);
            double double_value_2 = var_value_2.toDouble(&ok2);

            if (ok1 && ok2)
            {
                node->r_bool = (bool) double_value_1 >= double_value_2;
            }

            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name + " = " + (QString) var_name_1 + " >= " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }
    }

    else if (node->id == NodesId::NODE_OPERATOR_LESS_OR_EQUAL)
    {
        prev_node_1         = node->prev_node_get_1;
        QString var_name_1  = prev_node_1->var_name;
        QString var_value_1 = prev_node_1->var_value;
        QString var_type_1  = Node::type_node(prev_node_1);

        prev_node_2         = node->prev_node_get_2;
        QString var_name_2  = prev_node_2->var_name;
        QString var_value_2 = prev_node_2->var_value;
        QString var_type_2  = Node::type_node(prev_node_2);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);
        bool is_prev_node_2_get = Node::is_get(prev_node_2, is_get_node_str_2);

        if (is_prev_node_1_get)
        {
            if (var_type_1 == "bool")
            {
                var_value_1 = prev_node_1->var_value;
            }
            else
                var_value_1 = prev_node_1->node_value->text();
        }

        if (is_prev_node_2_get)
        {
            if (var_type_2 == "bool")
            {
                var_value_2 = prev_node_2->var_value;
            }
            else
                var_value_2 = prev_node_2->node_value->text();
        }


        if (!check(var_name_1) && is_prev_node_1_get)
        {
            if (is_get_node_str)
            {
                file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + quotation_mark.toUtf8() + var_value_1.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str = false;
            }
            else
                file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + var_value_1.toUtf8() + ";");
            var_names.push_back(var_name_1);
        }

        if (!check(var_name_2) && is_prev_node_2_get)
        {
            if (is_get_node_str_2)
            {
                file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + quotation_mark.toUtf8() + var_value_2.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str_2 = false;
            }
            else
                file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + var_value_2.toUtf8() + ";");
            var_names.push_back(var_name_2);
        }

        if (!is_prev_node_1_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name +  " = " + node->prev_node->r_name + " <= " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }

        if (!is_prev_node_2_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name +  " = " + (QString) var_name_1 + " <= " + node->prev_node->r_name + ";";
            file.write(line.toUtf8());
        }


        if (is_prev_node_1_get && is_prev_node_2_get)
        {
            bool ok1, ok2;
            double double_value_1 = var_value_1.toDouble(&ok1);
            double double_value_2 = var_value_2.toDouble(&ok2);

            if (ok1 && ok2)
            {
                node->r_bool = (bool) double_value_1 <= double_value_2;
            }

            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name + " = " + (QString) var_name_1 + " <= " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }
    }

    else if (node->id == NodesId::NODE_OPERATOR_EQUAL)
    {
        prev_node_1         = node->prev_node_get_1;
        QString var_name_1  = prev_node_1->var_name;
        QString var_value_1 = prev_node_1->var_value;
        QString var_type_1  = Node::type_node(prev_node_1);

        prev_node_2         = node->prev_node_get_2;
        QString var_name_2  = prev_node_2->var_name;
        QString var_value_2 = prev_node_2->var_value;
        QString var_type_2  = Node::type_node(prev_node_2);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);
        bool is_prev_node_2_get = Node::is_get(prev_node_2, is_get_node_str_2);

        if (is_prev_node_1_get)
        {
            if (var_type_1 == "bool")
            {
                var_value_1 = prev_node_1->var_value;
            }
            else
                var_value_1 = prev_node_1->node_value->text();
        }

        if (is_prev_node_2_get)
        {
            if (var_type_2 == "bool")
            {
                var_value_2 = prev_node_2->var_value;
            }
            else
                var_value_2 = prev_node_2->node_value->text();
        }

        if (!check(var_name_1) && is_prev_node_1_get)
        {
            if (is_get_node_str)
            {
                file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + quotation_mark.toUtf8() + var_value_1.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str = false;
            }
            else
                file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + var_value_1.toUtf8() + ";");
            var_names.push_back(var_name_1);
        }

        if (!check(var_name_2) && is_prev_node_2_get)
        {
            if (is_get_node_str_2)
            {
                file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + quotation_mark.toUtf8() + var_value_2.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str_2 = false;
            }
            else
                file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + var_value_2.toUtf8() + ";");
            var_names.push_back(var_name_2);
        }

        if (!is_prev_node_1_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name +  " = " + node->prev_node->r_name + " == " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }

        if (!is_prev_node_2_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name +  " = " + (QString) var_name_1 + " == " + node->prev_node->r_name + ";";
            file.write(line.toUtf8());
        }


        if (is_prev_node_1_get && is_prev_node_2_get)
        {
            bool ok1, ok2;
            double double_value_1 = var_value_1.toDouble(&ok1);
            double double_value_2 = var_value_2.toDouble(&ok2);

            if (ok1 && ok2)
            {
                node->r_bool = (bool) double_value_1 == double_value_2;
            }

            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name + " = " + (QString) var_name_1 + " == " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }
    }

    else if (node->id == NodesId::NODE_OPERATOR_NOT_EQUAL)
    {
        prev_node_1         = node->prev_node_get_1;
        QString var_name_1  = prev_node_1->var_name;
        QString var_value_1;
        QString var_type_1  = Node::type_node(prev_node_1);

        prev_node_2         = node->prev_node_get_2;
        QString var_name_2  = prev_node_2->var_name;
        QString var_value_2;
        QString var_type_2  = Node::type_node(prev_node_2);

        bool is_prev_node_1_get = Node::is_get(prev_node_1, is_get_node_str);
        bool is_prev_node_2_get = Node::is_get(prev_node_2, is_get_node_str_2);

        if (is_prev_node_1_get)
        {
            if (var_type_1 == "bool")
            {
                var_value_1 = prev_node_1->var_value;
            }
            else
                var_value_1 = prev_node_1->node_value->text();
        }

        if (is_prev_node_2_get)
        {
            if (var_type_2 == "bool")
            {
                var_value_2 = prev_node_2->var_value;
            }
            else
                var_value_2 = prev_node_2->node_value->text();
        }

        if (!check(var_name_1) && is_prev_node_1_get)
        {
            if (is_get_node_str)
            {
                file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + quotation_mark.toUtf8() +  var_value_1.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str = false;
            }
            else
                file.write("\n" + tabs.toUtf8() + var_type_1.toUtf8() + " " + var_name_1.toUtf8() + " = " + var_value_1.toUtf8() + ";");
            var_names.push_back(var_name_1);
        }

        if (!check(var_name_2) && is_prev_node_2_get)
        {
            if (is_get_node_str_2)
            {
                file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + quotation_mark.toUtf8() + var_value_2.toUtf8() + quotation_mark.toUtf8() + ";");
                is_get_node_str_2 = false;
            }
            else
                file.write("\n" + tabs.toUtf8() + var_type_2.toUtf8() + " " + var_name_2.toUtf8() + " = " + var_value_2.toUtf8() + ";");

            var_names.push_back(var_name_2);
        }

        if (!is_prev_node_1_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name +  " = " + node->prev_node->r_name + " != " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }

        if (!is_prev_node_2_get)
        {
            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name +  " = " + (QString) var_name_1 + " != " + node->prev_node->r_name + ";";
            file.write(line.toUtf8());
        }


        if (is_prev_node_1_get && is_prev_node_2_get)
        {
            bool ok1, ok2;
            double double_value_1 = var_value_1.toDouble(&ok1);
            double double_value_2 = var_value_2.toDouble(&ok2);

            if (ok1 && ok2)
            {
                node->r_bool = (bool) double_value_1 != double_value_2;
            }

            var_name_value++;
            QString r_var_name = "var_" + QString::number(var_name_value);
            node->r_name = r_var_name;
            line = "\n" + tabs.toUtf8() + "bool " + r_var_name + " = " + (QString) var_name_1 + " != " + (QString) var_name_2 + ";";
            file.write(line.toUtf8());
        }
    }


    file.close();
}

bool Node::check(QString name)
{
    for (int i = 0; i < var_names.size(); ++i)
    {
        if (name == var_names[i])
            return true;
    }

    return false;
}

QString Node::type_node(Node* node)
{
    if (node->id == NodesId::NODE_INT)      return "int";
    if (node->id == NodesId::NODE_FLOAT)    return "float";
    if (node->id == NodesId::NODE_DOUBLE)   return "double";
    if (node->id == NodesId::NODE_STRING)   return "std::string";
    if (node->id == NodesId::NODE_BOOL)     return "bool";
    return "not_found_1";
}

QString Node::type_set_node(Node* node)
{
    if (node->id == NodesId::NODE_INT               ||
        node->id == NodesId::NODE_FLOAT             ||
        node->id == NodesId::NODE_DOUBLE            ||
        node->id == NodesId::NODE_OPERATOR_P        ||
        node->id == NodesId::NODE_OPERATOR_M        ||
        node->id == NodesId::NODE_OPERATOR_MULT     ||
        node->id == NodesId::NODE_OPERATOR_DIV      ||
        node->id == NodesId::NODE_OPERATOR_PERCENT
    )   return "digit";

    else if(
        node->id == NodesId::NODE_BOOL                      ||
        node->id == NodesId::NODE_OPERATOR_GREAT            ||
        node->id == NodesId::NODE_OPERATOR_LESS             ||
        node->id == NodesId::NODE_OPERATOR_GREAT_OR_EQUAL   ||
        node->id == NodesId::NODE_OPERATOR_LESS_OR_EQUAL    ||
        node->id == NodesId::NODE_OPERATOR_EQUAL            ||
        node->id == NodesId::NODE_OPERATOR_NOT_EQUAL
    )   return "bool";

    else if(
        node->id == NodesId::NODE_STRING
    )   return "string";

    return "not_found_2";
}

bool Node::is_get(Node* node, bool& is_str)
{
    if (node->id == NodesId::NODE_INT    ||
        node->id == NodesId::NODE_FLOAT  ||
        node->id == NodesId::NODE_DOUBLE ||
        node->id == NodesId::NODE_BOOL)
        return true;

    if (node->id == NodesId::NODE_STRING)
    {
        is_str = true;
        return true;
    }

    return false;
}

bool Node::isGetWithoutStr(Node* node)
{
    if (node->id == NodesId::NODE_INT    ||
        node->id == NodesId::NODE_FLOAT  ||
        node->id == NodesId::NODE_DOUBLE ||
        node->id == NodesId::NODE_BOOL   ||
        node->id == NodesId::NODE_STRING)
        return true;
    else
        return false;
}
