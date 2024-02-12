#include "main_window.h"

QListWidget*    MainWindow::varTypesList        = nullptr;
QListWidget*    MainWindow::varNamesList        = nullptr;
QListWidget*    MainWindow::varValuesList       = nullptr;
QListWidget*    MainWindow::funcTypesList       = nullptr;
QListWidget*    MainWindow::funcNamesList       = nullptr;
QWidget*        MainWindow::central_widget      = nullptr;
QWidget*        MainWindow::controlPanel        = nullptr;
QWidget*        MainWindow::messageWindow       = nullptr;
QComboBox*      MainWindow::comboBoxFunctions   = nullptr;
QComboBox*      MainWindow::comboBoxVars        = nullptr;
QTimer*         MainWindow::timer               = nullptr;
MainWindow*     MainWindow::win                 = nullptr;
QString         MainWindow::path_txt;
QString         MainWindow::path_cpp;
QString         MainWindow::path_exe;

MainWindow::MainWindow(QGraphicsView* view, QWidget* parent) : QMainWindow(parent)
{ 
    node_obj = new Node();
    func_obj = new Functional();

    m_graphicsView = view;

    central_widget = new QWidget(this);
    this->setCentralWidget(central_widget);
    this->setMinimumSize(320, 80);
    this->setStyleSheet(windows_styles::main_window_style);

    scene = new Viewport(view);
    m_graphicsView->setScene(scene);
    m_graphicsView->setParent(central_widget);

    compilerChoose = new QWidget(central_widget);
    compilerChoose->setGeometry(275, 55, 200, 125);
    compilerChoose->setStyleSheet(windows_styles::dialog_window_btn_green_style);
    compilerChoose->show();
    compilerChoose->setVisible(false);

    comboBoxCompiler = new QComboBox(compilerChoose);
    comboBoxCompiler->setGeometry(10, 20, 180, 40);
    comboBoxCompiler->setStyleSheet(windows_styles::dialog_window_btn_green_style);

    comboBoxCompiler->addItem("GCC");
    comboBoxCompiler->addItem("Clang");
    comboBoxCompiler->addItem("Intel C++ Compiler");
    comboBoxCompiler->addItem("Digital Mars C++ Compiler");
    comboBoxCompiler->addItem("PathScale EKOPath Compiler Suite");
    comboBoxCompiler->addItem("Microsoft Visual C++");

    btnApplyCompiler = new QPushButton("Применить", compilerChoose);
    btnApplyCompiler->setStyleSheet(windows_styles::dialog_window_btn_green_style);
    btnApplyCompiler->setGeometry(10, 65, 180, 40);

    controlPanel = new QWidget(central_widget);
    controlPanel->setGeometry(0, 0, MAIN_WINDOW_WIDTH, 50);
    controlPanel->setStyleSheet(windows_styles::dialog_window_btn_green_style);
    controlPanel->show();

    hideAndShowPanel = new QPushButton("↑", central_widget);
    hideAndShowPanel->setGeometry(0, 50 + 5, 20, 20);
    hideAndShowPanel->setStyleSheet(windows_styles::dialog_window_btn_red_style);
    hideAndShowPanel->show();

    save = new QPushButton(controlPanel);
    save->setGeometry(5, 5, 40, 40);
    save->setIcon(QIcon(icons::save_icon));

    compile = new QPushButton(controlPanel);
    compile->setGeometry(50, 5, 40, 40);
    compile->setIcon(QIcon(icons::compile_icon));

    run = new QPushButton(controlPanel);
    run->setGeometry(95, 5, 40, 40);
    run->setIcon(QIcon(icons::run_icon));

    terminal = new QPushButton(controlPanel);
    terminal->setGeometry(140, 5, 40, 40);
    terminal->setIcon(QIcon(icons::terminal_icon));

    code = new QPushButton(controlPanel);
    code->setGeometry(185, 5, 40, 40);
    code->setIcon(QIcon(icons::code_icon));

    compiler = new QPushButton(controlPanel);
    compiler->setGeometry(230, 5, 40, 40);
    compiler->setIcon(QIcon(icons::compiler_icon));
    
    QMainWindow::resize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
    QMainWindow::show();

    QObject::connect(hideAndShowPanel, &QPushButton::clicked, this, &MainWindow::hideAndShowPanelSlot);
    QObject::connect(btnApplyCompiler, &QPushButton::clicked, this, &MainWindow::btnApplyCompilerSlot);
    QObject::connect(save, &QPushButton::clicked, node_obj, &Node::save_nodes_slot);
    QObject::connect(compile, &QPushButton::clicked, func_obj, &Functional::compile);
    QObject::connect(run, &QPushButton::clicked, func_obj, &Functional::run);
    QObject::connect(terminal, &QPushButton::clicked, func_obj, &Functional::terminal_slot);
    QObject::connect(code, &QPushButton::clicked, func_obj, &Functional::code_slot);
    QObject::connect(compiler, &QPushButton::clicked, this, &MainWindow::compilerSlot);

    MainWindow::createFiles();
    initEmptyProject();
}

MainWindow::~MainWindow()
{
    delete central_widget;
    delete scene;
    delete m_graphicsView;

    delete func_obj;
    delete node_obj;

    delete timer;
    delete win;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    if (scene && m_graphicsView)
    {
        scene->setSceneRect(0, 0, width(), height());
        m_graphicsView->resize(width(), height());
    }

    controlPanel->setGeometry(0, 0, width(), 50);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ;
    QMainWindow::closeEvent(event);
}

void MainWindow::initEmptyProject()
{
    QFile file(MainWindow::path_txt);

    if (file.open(QIODevice::WriteOnly))
    {

        file.write("#include <iostream>\n#include <cmath>\n\n\nint main()\n{\n\t;\n\treturn 0;\n}");

        file.close();
    }
    else
    {
        createFiles();
    }
}

void MainWindow::userPermission()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(nullptr, "Разрешение на запись", "Приложению нужно разрешение для создания файлов в директории с временными файлами\n\nБудут созадны три файла exe.txt\nexe.cpp\nexe.exe\n\nРазрешить?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        qDebug() << "Пользователь дал разрешение на создание файлов";
    }
    else
    {
        qDebug() << "Пользователь отказал в разрешении на создание файлов";

        QMessageBox::critical(nullptr, "Отказ доступа", "Для дальнейшей работы с программой придется дать доступ к директории с временными файлами\n\nНИКАКИХ ВИРУСОВ НЕТ !!!");

        MainWindow::userPermission();
    }
}

void MainWindow::createFiles()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);

    if (path.isEmpty())
    {
        MainWindow::userPermission();
    }

    path_txt = path + "/textCodeInformationNodes.txt";
    path_cpp = path + "/CPPCodeInformationNodes.cpp";
#ifdef __LINUX__OS__
    path_exe = path + "/executable_nodes";
#elif defined (__WINDOWS__OS__)
    path_exe = path + "/executable_nodes.exe";
#endif

    Functional::path_txt = path_txt;
    Functional::path_cpp = path_cpp;
    Functional::path_exe = path_exe;

    Node::path_txt = path_txt;
    Node::path_cpp = path_cpp;
    Node::path_exe = path_exe;

    QFile file_txt(path_txt);
    QFile file_cpp(path_cpp);
    QFile file_exe(path_exe);

    if (!file_txt.exists())
    {
        file_txt.open(QIODevice::WriteOnly);
        file_txt.close();

        QMessageBox::information(nullptr, "Работа с файлами", "Создан файл: " + path_txt);
    }
    if (!file_cpp.exists())
    {
        file_cpp.open(QIODevice::WriteOnly);
        file_cpp.close();

        QMessageBox::information(nullptr, "Работа с файлами", "Создан файл: " + path_cpp);
    }
    if (!file_exe.exists())
    {
        file_exe.open(QIODevice::WriteOnly);
        file_exe.close();

        QMessageBox::information(nullptr, "Работа с файлами", "Создан файл: " + path_exe);
    }
}

void MainWindow::hideAndShowPanelSlot()
{
    if (controlPanel->isVisible())
    {
        controlPanel->setVisible(false);
        hideAndShowPanel->setText("↓");
        hideAndShowPanel->setGeometry(0, 5, 20, 20);
        hideAndShowPanel->setStyleSheet(windows_styles::dialog_window_btn_green_style);
    }
    else
    {
        controlPanel->setVisible(true);
        hideAndShowPanel->setText("↑");
        hideAndShowPanel->setGeometry(0, 50 + 5, 20, 20);
        hideAndShowPanel->setStyleSheet(windows_styles::dialog_window_btn_red_style);
    }
}

void MainWindow::compilerSlot()
{
    if (compilerChoose->isVisible())
        compilerChoose->setVisible(false);
    else
        compilerChoose->setVisible(true);
}

void MainWindow::btnApplyCompilerSlot()
{
    func_obj->current_compiler = comboBoxCompiler->currentText();
    compilerChoose->setVisible(false);
}

void MainWindow::initMessageWindow(QString text, QString color)
{
    if (messageWindow) return;

    win = new MainWindow();
    QFont font("Montserrat", 15);

    timer = new QTimer(win);
    
    messageWindow = new QWidget(MainWindow::central_widget);
    messageWindow->setGeometry(10, MAIN_WINDOW_HEIGHT - 100, MAIN_WINDOW_WIDTH, 50);

    QLabel* message = new QLabel(messageWindow);
    message->setText(text);
    message->setFont(font);

    if (color == "red")
        message->setStyleSheet("color: #FF0000;");
    else
        message->setStyleSheet("color: #00FF00;");

    messageWindow->show();
    QObject::connect(timer, &QTimer::timeout, win, &MainWindow::onTimeout);
    timer->start(3000);
}

void MainWindow::onTimeout()
{
    delete messageWindow;
    messageWindow = nullptr;
    timer->deleteLater();
}
