#include "viewport.h"

int count_nodes = 0;

QMap<int, QGraphicsProxyWidget*> proxyMap;

QVector<Node*>  Viewport::nodes;
QListWidget*    Viewport::main_menu         = nullptr;
QWidget*        Viewport::deleteWindow      = nullptr;
QWidget*        Viewport::changeNodeWindow  = nullptr;
QPushButton*    Viewport::btnOkName         = nullptr;
QPushButton*    Viewport::btnOkValue        = nullptr;
QPushButton*    Viewport::deleteNodeWindow  = nullptr;
QLineEdit*      Viewport::newName           = nullptr;
QLineEdit*      Viewport::newValue          = nullptr;
Node*           Viewport::change            = nullptr;
bool            Viewport::isIdBool          = false;
int             Viewport::unique_id         = 0;

Viewport::Viewport(QGraphicsView* m_graphicsView, QGraphicsScene* parent) : QGraphicsScene(parent)
{
    this->m_graphicsView = m_graphicsView;
    
    initChangeNodeWindow();

    main_menu = new QListWidget();

    main_menu->setWindowModality(Qt::ApplicationModal);
    main_menu->setStyleSheet(windows_styles::main_window_style);

    funcItem      = new QListWidgetItem("Функции");
    operatorItem  = new QListWidgetItem("Операторы");
    cycleItem     = new QListWidgetItem("Циклы");
    varsItem      = new QListWidgetItem("Переменные");
    operator2Item = new QListWidgetItem("Операторы сравнения");

    QFont font("Arial");
    font.setPointSize(12);
    font.setWeight(QFont::Bold);

    funcItem->setFont(font);
    operatorItem->setFont(font);
    cycleItem->setFont(font);
    varsItem->setFont(font);
    operator2Item->setFont(font);

    main_menu->addItem(funcItem);
    main_menu->addItem("output");
    main_menu->addItem("input");
    main_menu->addItem("global");
    main_menu->addItem("change value");
    main_menu->addItem("null");
    main_menu->addItem("break");
    main_menu->addItem("endl");

    main_menu->addItem(operatorItem);
    main_menu->addItem("branch");

    main_menu->addItem(cycleItem);
    main_menu->addItem("endless cycle");

    main_menu->addItem(varsItem);
    main_menu->addItem("int");
    main_menu->addItem("float");
    main_menu->addItem("double");
    main_menu->addItem("string");
    main_menu->addItem("bool");

    main_menu->addItem(operator2Item);
    main_menu->addItem("+");
    main_menu->addItem("-");
    main_menu->addItem("*");
    main_menu->addItem("/");
    main_menu->addItem("%");
    main_menu->addItem(">");
    main_menu->addItem("<");
    main_menu->addItem("≥");
    main_menu->addItem("≤");
    main_menu->addItem("≡");
    main_menu->addItem("≠");

    if (main_menu->isVisible())
        main_menu->setVisible(false);

    proxyWindowGetVars = new QGraphicsProxyWidget();

    window_get_vars = new QWidget();
    window_get_vars->setFixedSize(WNDOW_GET_VARS_HEIGHT, WNDOW_GET_VARS_WIDTH);
    window_get_vars->setStyleSheet(windows_styles::get_vars_window_style);
    window_get_vars->setVisible(false);

    var_name = new QLineEdit(window_get_vars);
    var_name->setGeometry(20, 20, 200, (int) 70 / 3);
    var_name->setStyleSheet(windows_styles::dialog_window_label_style);
    var_name->setPlaceholderText("Имя переменной");
    var_name->setAlignment(Qt::AlignCenter);
    var_name->show();

    var_value = new QLineEdit(window_get_vars);
    var_value->setGeometry(20, 20 + 30, 200, (int) 70 / 3);
    var_value->setStyleSheet(windows_styles::dialog_window_label_style);
    var_value->setPlaceholderText("Значение переменной");
    var_value->setAlignment(Qt::AlignCenter);
    var_value->show();

    btn_ok = new QPushButton(window_get_vars);
    btn_ok->setGeometry(20, 20 + 60, 200, (int) 70 / 3);
    btn_ok->setText("OK");
    btn_ok->setStyleSheet(windows_styles::dialog_window_btn_green_style);
    btn_ok->show();

    proxyWindowGetVars->setWidget(window_get_vars);

    this->addItem(proxyWindowGetVars);

    QObject::connect(btn_ok, &QPushButton::clicked, this, &Viewport::create_get_var);
    QObject::connect(main_menu, &QListWidget::itemClicked, this, &Viewport::create_node);
}

Viewport::~Viewport()
{
    delete funcItem;
    delete operatorItem;
    delete cycleItem;
    delete varsItem;
    delete operator2Item;
    delete main_menu;

    delete proxyWindowGetVars;

    if (changeWindowProxy != nullptr)
    {
        delete changeWindowProxy;
        delete changeNodeWindow;
    }
}

void Viewport::addNode(Node* node)
{
    proxyMap.insert(unique_id, new QGraphicsProxyWidget());
    proxyMap[unique_id]->setWidget(node);
    this->addItem(proxyMap[unique_id]);
    nodes.push_back(node);
}

void Viewport::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->buttons() == Qt::RightButton)
    {
        if (!main_menu->isVisible()) 
        {
            mouse_pos = event->scenePos();

            Viewport::x_pos = mouse_pos.x();
            Viewport::y_pos = mouse_pos.y();

            main_menu->setGeometry(Viewport::x_pos, Viewport::y_pos, 300, 300);
            main_menu->setVisible(true);
        }
        else main_menu->setVisible(false);
        window_get_vars->setVisible(false);
    }

    QGraphicsScene::mousePressEvent(event);
}

void Viewport::create_get_var()
{
    bool check = true;

    QString name  = this->var_name->text();
    QString value = this->var_value->text();

    if (this->get_var_type == "float" || this->get_var_type == "double")
    {
        bool is_name  = true;

        if (name[0].isDigit()) is_name = false;

        if (is_name)
        {
            for (int i = 0; i < Node::var_repeat_names.size(); ++i)
            {
                if (Node::var_repeat_names[i] == name)
                {
                    check = false;
                    break;
                }
                else
                    check = true;
            }
        }

        else
            check = false;
    }

    else if (this->get_var_type == "int")
    {
        bool is_name  = true;

        if (name[0].isDigit()) is_name = false;

        if (is_name)
        {
            for (int i = 0; i < Node::var_repeat_names.size(); ++i)
            {
                if (Node::var_repeat_names[i] == name)
                {
                    check = false;
                    break;
                }
                else
                    check = true;
            }
        }

        else
            check = false;
    }

    else if (this->get_var_type == "string")
    {
        bool is_name  = true;

        if (name[0].isDigit()) is_name = false;

        if (is_name)
        {
            for (int i = 0; i < Node::var_repeat_names.size(); ++i)
            {
                if (Node::var_repeat_names[i] == name)
                {
                    check = false;
                    break;
                }
                else
                    check = true;
            }
        }

        else
            check = false;

    }

    else if (this->get_var_type == "bool")
    {
        bool is_value = true;

        if (value != "false" && value != "true") 
            is_value = false;

        if (is_value)
        {
            for (int i = 0; i < Node::var_repeat_names.size(); ++i)
            {
                if (Node::var_repeat_names[i] == name)
                {
                    check = false;
                    break;
                }
                else
                    check = true;
            }
        }

        else
            check = false;

    }


    if (!check) return;

    this->this_node_name  = this->var_name->text();
    this->this_node_value = this->var_value->text();

    Node::var_repeat_names.push_back(this->this_node_name);

    window_get_vars->setVisible(false);

    if (this->get_var_type == "int")
    {
        Viewport::addNodeInMenu(NodesId::NODE_INT, WIDTH_NODE_GET, HEIGHT_NODE_GET, this->this_node_name, this->this_node_value);
    }

    else if (this->get_var_type == "float")
    {
        Viewport::addNodeInMenu(NodesId::NODE_FLOAT, WIDTH_NODE_GET, HEIGHT_NODE_GET, this->this_node_name, this->this_node_value);
    }

    else if (this->get_var_type == "double")
    {
        Viewport::addNodeInMenu(NodesId::NODE_DOUBLE, WIDTH_NODE_GET, HEIGHT_NODE_GET, this->this_node_name, this->this_node_value);
    }

    else if (this->get_var_type == "string")
    {
        Viewport::addNodeInMenu(NodesId::NODE_STRING, WIDTH_NODE_GET, HEIGHT_NODE_GET, this->this_node_name, this->this_node_value);
    }

    else if (this->get_var_type == "bool")
    {
        Viewport::addNodeInMenu(NodesId::NODE_BOOL, WIDTH_NODE_GET, HEIGHT_NODE_GET, this->this_node_name, this->this_node_value);
    }
    
    this->var_name->clear();
    this->var_value->clear();
}

void Viewport::create_node(QListWidgetItem* item)
{
    qDebug() << "Создана нода:  " + item->text();

    if (item->text() == "output")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_OUTPUT, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "input")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_INPUT, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "branch")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_BRANCH, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "endless cycle")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_ENDLESS_CYCLE, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "change value")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_CHANGE_VALUE, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "global")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_GLOBAL, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "null")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_NULL, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "break")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_BREAK, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "endl")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_ENDL, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "+")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_OPERATOR_P, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "-")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_OPERATOR_M, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "*")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_OPERATOR_MULT, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "/")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_OPERATOR_DIV, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "%")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_OPERATOR_PERCENT, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == ">")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_OPERATOR_GREAT, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "<")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_OPERATOR_LESS, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "≥")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_OPERATOR_GREAT_OR_EQUAL, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "≤")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_OPERATOR_LESS_OR_EQUAL, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "≡")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_OPERATOR_EQUAL, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    else if (item->text() == "≠")
    {
        count_nodes += 1;
        Viewport::addNodeInMenu(NodesId::NODE_OPERATOR_NOT_EQUAL, WIDTH_NODE, HEIGHT_NODE, " ", " ");
        main_menu->setVisible(false);
    }

    // get ноды (переменные)
    else if (item->text() == "int")
    {
        window_get_vars->setGeometry(Viewport::x_pos, Viewport::y_pos, WNDOW_GET_VARS_HEIGHT, WNDOW_GET_VARS_WIDTH);
        window_get_vars->setVisible(true);
        var_value->setVisible(true);
        this->get_var_type = "int";
        main_menu->setVisible(false);
    }

    else if (item->text() == "float")
    {
        window_get_vars->setGeometry(Viewport::x_pos, Viewport::y_pos, WNDOW_GET_VARS_HEIGHT, WNDOW_GET_VARS_WIDTH);
        window_get_vars->setVisible(true);
        var_value->setVisible(true);
        this->get_var_type = "float";
        main_menu->setVisible(false);
    }

    else if (item->text() == "double")
    {
        window_get_vars->setGeometry(Viewport::x_pos, Viewport::y_pos, WNDOW_GET_VARS_HEIGHT, WNDOW_GET_VARS_WIDTH);
        window_get_vars->setVisible(true);
        var_value->setVisible(true);
        this->get_var_type = "double";
        main_menu->setVisible(false);
    }

    else if (item->text() == "string")
    {
        window_get_vars->setGeometry(Viewport::x_pos, Viewport::y_pos, WNDOW_GET_VARS_HEIGHT, WNDOW_GET_VARS_WIDTH);
        window_get_vars->setVisible(true);
        var_value->setVisible(true);
        this->get_var_type = "string";
        main_menu->setVisible(false);
    }

    else if (item->text() == "bool")
    {
        window_get_vars->setGeometry(Viewport::x_pos, Viewport::y_pos, WNDOW_GET_VARS_HEIGHT, WNDOW_GET_VARS_WIDTH);
        window_get_vars->setVisible(true);
        var_value->setVisible(true);
        this->get_var_type = "bool";
        main_menu->setVisible(false);
    }
}

void Viewport::addNodeInMenu(size_t node_id, size_t width, size_t height, QString name, QString value)
{
    unique_id++;
    nodes_map.insert(unique_id, new Node(this, this, m_graphicsView, node_id, name, value));
    nodes_map[unique_id]->u_id = unique_id;
    nodes_map[unique_id]->setGeometry(mouse_pos.x(), mouse_pos.y(), width, height);
    Viewport::addNode(nodes_map[unique_id]);
}

void Viewport::removeNode(Node* node)
{
    if (node == Node::head)
    {
        Node::head = NULL;
        node->is_head = false;
    }

    for (const auto& key : proxyMap.keys()) 
    {
        if (node->u_id == key)
        {
            auto it = proxyMap.find(key);
            this->removeItem(proxyMap[key]);
            delete proxyMap[key];
            proxyMap.erase(it);

            for (int i = 0; i < nodes.size(); ++i)
            {
                if (nodes[i]->u_id == node->u_id)
                {
                    nodes.remove(i);
                    break;
                }
            }
        }
    }
}

bool Viewport::isGet(int id)
{
    if (id == NodesId::NODE_INT    ||
        id == NodesId::NODE_FLOAT  ||
        id == NodesId::NODE_DOUBLE ||
        id == NodesId::NODE_STRING ||
        id == NodesId::NODE_BOOL)
        return true;
    else
        return false;
}

void Viewport::visibleChangeNodeWindow(int x_pos, int y_pos, Node* node)
{
    changeNodeWindow->setGeometry(x_pos, y_pos, 150, 84);
    change = node;
    changeWindowProxy->show();

    if (node->id == NodesId::NODE_BOOL)
    {
        Viewport::isIdBool = true;
    }
    else
    {
        Viewport::isIdBool = false;
    }
}

void Viewport::initChangeNodeWindow()
{
    changeWindowProxy = new QGraphicsProxyWidget();
    changeNodeWindow  = new QWidget();
    btnOkName         = new QPushButton(changeNodeWindow);
    btnOkValue        = new QPushButton(changeNodeWindow);
    deleteNodeWindow  = new QPushButton("Закрыть окно", changeNodeWindow);
    newName           = new QLineEdit(changeNodeWindow);
    newValue          = new QLineEdit(changeNodeWindow);

    changeNodeWindow->setGeometry(0, 0, 150, 84);
    changeNodeWindow->setStyleSheet(windows_styles::dialog_window_btn_green_style);
    newName->setGeometry(0, 0, 120, 25);
    newName->setStyleSheet(windows_styles::dialog_window_btn_green_style);
    newName->setPlaceholderText("Новое имя");
    newName->setAlignment(Qt::AlignCenter);
    newValue->setGeometry(0, 27, 120, 25);
    newValue->setStyleSheet(windows_styles::dialog_window_btn_green_style);
    newValue->setPlaceholderText("Новое значение");
    newValue->setAlignment(Qt::AlignCenter);
    btnOkName->setGeometry(125, 0, 25, 25);
    btnOkName->setStyleSheet(windows_styles::dialog_window_btn_green_style);
    btnOkName->setIcon(QIcon(icons::good));
    btnOkValue->setGeometry(125, 27, 25, 25);
    btnOkValue->setStyleSheet(windows_styles::dialog_window_btn_green_style);
    btnOkValue->setIcon(QIcon(icons::good));
    deleteNodeWindow->setGeometry(0, 56, 150, 25);
    deleteNodeWindow->setStyleSheet(windows_styles::dialog_window_btn_red_style);

    changeWindowProxy->setWidget(changeNodeWindow);
    this->addItem(changeWindowProxy);

    QObject::connect(deleteNodeWindow, &QPushButton::clicked, this, &Viewport::deleteNodeWindowSlot);
    QObject::connect(btnOkName, &QPushButton::clicked, this, &Viewport::btnOkNameSlot);
    QObject::connect(btnOkValue, &QPushButton::clicked, this, &Viewport::btnOkValueSlot);

    changeWindowProxy->hide();
}

void Viewport::deleteNodeWindowSlot()
{
    changeWindowProxy->hide();
}

void Viewport::btnOkNameSlot()
{
    QString new_name = newName->text();
    QString OldName  = change->var_name;

    for (int i = 0; i < Node::var_names.size(); ++i)
    {
        if (Node::var_names[i] == OldName)
        {
            Node::var_names[i] = new_name;
        }
    }

    for (int i = 0; i < Node::var_repeat_names.size(); ++i)
    {
        if (Node::var_repeat_names[i] == OldName)
        {
            Node::var_repeat_names[i] = new_name;
        }
    }

    change->var_name = new_name;
    change->node_name->setText(new_name);
    newName->setText("");
}

void Viewport::btnOkValueSlot()
{
    QString new_value = newValue->text();
    change->var_value = new_value;
    change->node_value->setText(new_value);
    newValue->setText("");
}