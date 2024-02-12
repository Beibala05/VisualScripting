#include "functional.h"

QWidget*        Functional::codeWindow              = nullptr;
QTextBrowser*   Functional::codeTextBrowserWindow   = nullptr;
QString         Functional::current_compiler        = "GCC";
QString         Functional::path_txt;
QString         Functional::path_cpp;
QString         Functional::path_exe;

Functional::Functional(MainWindow* MainWindowObj)
{
    this->main_window = MainWindowObj;
}

Functional::~Functional()
{
    delete codeWindow;
    delete compileResults;
}

int Functional::findTerminal(std::string& command, std::string& __e, size_t __t)
{ 
    std::vector<std::string> terminals = 
    {
        "x-terminal-emulator",
        "gnome-terminal",
        "konsole",
        "xfce4-terminal",
        "lxterminal",
        "mate-terminal",
        "terminator",
        "tilix",
        "xterm"
    };

    for (const auto& term : terminals) 
    {
        if (system(("which " + term + " >/dev/null 2>&1").c_str()) == 0) 
        {
            if (__t == Terminal::RUN_EXE)
                command = term + " -e \"/bin/bash -c '" + Functional::path_exe.toStdString() + "exe; exec bash'\"";
            else if (__t == Terminal::RUN_TERMINAL)
                command = term;
            __e = term;
            break;
        }
    }

    if (command.empty()) 
    {
        MainWindow::initMessageWindow("Не найден ни один поддерживаемый эмулятор терминала", "red");
        return EXIT_FAILURE;
    } 

    return EXIT_SUCCESS;
}

void Functional::run()
{
    QFile file(path_txt);

    if (file.exists())
    {
        #ifdef __LINUX__OS__
            std::string command, emulator;

            int isRunSuccess = findTerminal(command, emulator, Terminal::RUN_EXE);

            if (isRunSuccess == EXIT_SUCCESS)
            {
                MainWindow::initMessageWindow("Запущен эмулятор " + QString::fromStdString(emulator), "green");
                std::system(command.c_str());
            }
            else
            {
                MainWindow::initMessageWindow("Не удалось запустить проект", "red");
            }
        #elif defined (__WINDOWS__OS__)
            const std::string run_exe_win = (std::string)"cmd /k " + path_exe.toStdString();
            std::system(run_exe_win.c_str());
        #else
            qDebug() << "Ошибка при запуске";
        #endif
    }
    else
    {
        MainWindow::initMessageWindow("Сначала необходимо скомпилировать проект", "red");
    }
}

void Functional::compile()
{
    QString code;

    QFile file_txt(path_txt);
    if (file_txt.open(QIODevice::ReadOnly))
    {
        code = file_txt.readAll();
        file_txt.close();
    }
    else
    {
        MainWindow::createFiles();
        return;
    }

    QFile file_cpp(path_cpp);
    if (file_cpp.open(QIODevice::WriteOnly))
    {
        file_cpp.write(code.toUtf8());
        file_cpp.close();
    }
    else
    {
        MainWindow::createFiles();
        return;
    }

    int isCompileSucces;

    if (current_compiler == "GCC")
    {
        const std::string compile_GCC = (std::string)"g++ " + Functional::path_cpp.toStdString() + (std::string)" -o " + Functional::path_exe.toStdString() + (std::string)" -lm";
        MainWindow::initMessageWindow(QString::fromStdString(compile_GCC), "green");
        isCompileSucces = std::system(compile_GCC.c_str());
    }
    else if (current_compiler == "Clang")
    {
        const std::string compile_clang = (std::string)"clang++ " + path_cpp.toStdString() + (std::string)" -o " + path_exe.toStdString() + (std::string)" -lm";
        MainWindow::initMessageWindow(QString::fromStdString(compile_clang), "green");
        isCompileSucces = std::system(compile_clang.c_str());
    }
    else if (current_compiler == "Intel C++ Compiler")
    {
        const std::string compile_icpc = (std::string)"icpc -o " + path_exe.toStdString() + (std::string)" -lm " + path_cpp.toStdString();
        MainWindow::initMessageWindow(QString::fromStdString(compile_icpc), "green");
        isCompileSucces = std::system(compile_icpc.c_str());
    }
    else if (current_compiler == "Digital Mars C++ Compiler")
    {
        const std::string compile_dmc = (std::string)"dmc -o " + path_exe.toStdString() + (std::string)" -lm " + path_cpp.toStdString();
        MainWindow::initMessageWindow(QString::fromStdString(compile_dmc), "green");
        isCompileSucces = std::system(compile_dmc.c_str());
    }
    else if (current_compiler == "PathScale EKOPath Compiler Suite")
    {
        const std::string compile_pathCC = (std::string)"pathCC -o " + path_exe.toStdString() + (std::string)" -lm " + path_cpp.toStdString();
        MainWindow::initMessageWindow(QString::fromStdString(compile_pathCC), "green");
        isCompileSucces = std::system(compile_pathCC.c_str());
    }
    else if (current_compiler == "Microsoft Visual C++")
    {
        const std::string compile_cl = (std::string)"cl /EHsc /Fe " + path_exe.toStdString() + (std::string)" -lm " + path_cpp.toStdString();
        MainWindow::initMessageWindow(QString::fromStdString(compile_cl), "green");
        isCompileSucces = std::system(compile_cl.c_str());
    }

    if (isCompileSucces != 0)
    {
        MainWindow::initMessageWindow("Ошибка при компиляции проекта", "red");

        compileResultsShow();
    }
    else
    {
        MainWindow::initMessageWindow("Проект успешно скомпилирован", "green");
    }
}

void Functional::terminal_slot()
{
    #ifdef __LINUX__OS__
        std::string command, emulator;
        int isTerminalRun = findTerminal(command, emulator, Terminal::RUN_TERMINAL);

        if (isTerminalRun == EXIT_FAILURE)
        {
            MainWindow::initMessageWindow("Ошибка при запуске эмулятора терминала", "red");
        }
        else
        {
            MainWindow::initMessageWindow("Эмулятор " + QString::fromStdString(emulator) + " запущен", "green");
            std::system(command.c_str());
            MainWindow::initMessageWindow("Эмулятор " + QString::fromStdString(emulator) + " закрыт", "red");
        }
    #elif defined (__WINDOWS__OS__)
        std::system("cmd");
    #endif
}

void Functional::code_slot()
{
    initCodeWindow();
}

void Functional::initCodeWindow()
{
    QFile file(path_txt);

    codeWindow = new QWidget();
    codeWindow->setWindowTitle("Перевод нод на C++");
    codeWindow->setStyleSheet(windows_styles::main_window_style);

    QGridLayout* gridLayout = new QGridLayout(codeWindow);

    codeWindow->setLayout(gridLayout);

    codeTextBrowserWindow = new QTextBrowser(codeWindow);

    gridLayout->addWidget(codeTextBrowserWindow, 0, 0);

    QFont font;
    font.setPointSize(12);

    codeTextBrowserWindow->setFont(font);


    if (!file.exists())
    {
        MainWindow::initMessageWindow("Ошибка чтения файла", "red");
        codeTextBrowserWindow->setText("Ошибка чтения файла");
    }
    else
    {
        if (file.open(QIODevice::ReadOnly))
        {
            codeTextBrowserWindow->setText(file.readAll());
            file.close();
        }
        else
        {
            MainWindow::createFiles();
            return;
        }
    }

    codeWindow->show();
}

void Functional::compileResultsShow()
{
    compileResults = new QWidget();
    compileResults->setWindowModality(Qt::ApplicationModal);
    compileResults->setWindowTitle("Ошибка компиляции");
    compileResults->setStyleSheet(windows_styles::main_window_style);
    compileResults->setFixedSize(500, 200);

    QFont   font("Montserrat", 11);
    QFont   font_btn("Montserrat", 9);
    QPixmap errIcon(icons::bad);

    errorIcon = new QLabel(compileResults);
    errorIcon->setGeometry(10, 10, 100, 150);
    errorIcon->setPixmap(errIcon);

    errorInformationText = new QLabel(compileResults);
    errorInformationText->setGeometry(120, 10, 500 - 100 - 20, 150);
    errorInformationText->setFont(font);
    errorInformationText->setText("Компиляции завершилась с ошибкой,\nвозможные причины:"
    "\n  -Не удалось найти компилятор"
    "\n  -Ошибка при инициализации переменной"
    "\n  -Ошибка в нодах");

    getMoreInformation = new QPushButton(compileResults);
    getMoreInformation->setGeometry(370, 160, 120, 30);
    getMoreInformation->setStyleSheet(windows_styles::dialog_window_btn_green_style);
    getMoreInformation->setFont(font_btn);
    getMoreInformation->setText("Подробнее");

    compileResults->show();

    QObject::connect(getMoreInformation, &QPushButton::clicked, this, &Functional::getMoreInformationSlot);
}

void Functional::getMoreInformationSlot()
{
    QWidget* informationWidget = new QWidget();
    
    QMessageBox::information(informationWidget, "Подробно об ошибках", "Убедитесь что выбранный вами компилятор "
    "установлен на ваш ПК, можно проверить в терминале с помощью команды: имя_компилятора --version (для UNIX подобных систем)\n\n"
    "Так же убедитесь, что все переменные имеют корректные имена без специальных символов: (~@?/}[ и тп, а также значения переменных должны "
    "быть иницализированы\n\n"
    "Кроме этого, если нода имеет входные параметры, то они не должны быть пустыми без переданных значений");

    delete informationWidget;
}
