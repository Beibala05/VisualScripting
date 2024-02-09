#pragma once

#include "main_window.h"
#include "node.h"
#include "viewport.h"
#include "style.h"

#include <cstdlib>
#include <QVector>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QLineEdit>
#include <QListWidget>
#include <QFileDialog>
#include <QTextBrowser>
#include <QChar>
#include <QGridLayout>
#include <QFont>
#include <QPixmap>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>

#define HEIGHT_SETTINGS_WINDOW 250
#define WIDTH_SETTINGS_WINDOW  150

enum Terminal
{
    RUN_EXE,
    RUN_TERMINAL
};

class Node;
class MainWindow;
class Viewport;

class Functional : public QObject
{
public:
    Functional() {};
    explicit Functional(MainWindow*);
    ~Functional();

    friend class MainWindow;
    friend class Viewport;
    friend class Node;

    static QString path_txt;
    static QString path_cpp;
    static QString path_exe;

public slots:
    void run();
    void compile();
    void code_slot();
    void terminal_slot();

private slots:
    void compileResultsShow();
    void getMoreInformationSlot();

private:
    void initCodeWindow();
    int  findTerminal(std::string&, std::string&, size_t);

private:
    MainWindow*                         main_window;
    static QWidget*                     codeWindow;
    QWidget*                            compileResults;
    QLabel*                             errorIcon;
    QLabel*                             errorInformationText;
    static QTextBrowser*                codeTextBrowserWindow;
    QPushButton*                        getMoreInformation;
    static QString                      current_compiler;

//    const std::string compile_clang         = (std::string)"clang++ " + path_cpp.toStdString() + (std::string)" -o " + path_exe.toStdString() + (std::string)" -lm";
//    const std::string compile_icpc          = (std::string)"icpc -o " + path_exe.toStdString() + (std::string)" -lm " + path_cpp.toStdString();
//    const std::string compile_dmc           = (std::string)"dmc -o " + path_exe.toStdString() + (std::string)" -lm " + path_cpp.toStdString();
//    const std::string compile_pathCC        = (std::string)"pathCC -o " + path_exe.toStdString() + (std::string)" -lm " + path_cpp.toStdString();
//    const std::string compile_cl            = (std::string)"cl /EHsc /Fe " + path_exe.toStdString() + (std::string)" -lm " + path_cpp.toStdString();
};
