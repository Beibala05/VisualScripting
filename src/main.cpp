#include "main_window.h"
#include "style.h"
#include "viewport.h"
#include "node.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setStyleSheet(windows_styles::main_window_style);
    
    QGraphicsView* view = new QGraphicsView();
    view->setStyleSheet(windows_styles::main_window_style);

    MainWindow* window = new MainWindow(view);
    window->setStyleSheet(windows_styles::main_window_style);

    view->resize(window->width(), window->height());

    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->show();

    return app.exec();
}