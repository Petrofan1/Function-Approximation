#include "header.hpp"
#include "widget.h"

using namespace std;

int main(int argc, char* argv[])
{
    int parse = parse_command_line(argc, argv);
    int n, k, p;
    double eps;
    char* file_name;
    if(parse == -1) return 0;
    else if(parse == 2)
    {
        n = 32;
        k = 1;
        eps = 1e-16;
        file_name = 0;
        p = 4;
    }
    else
    {
        file_name = argv[1];
        n = atoi(argv[2]);
        k = atoi(argv[4]);
        eps = atof(argv[5]);
        p = atoi(argv[6]);
    }

    QApplication app(argc, argv);
    QAction *action;
    Widget* w;
    QMainWindow *window;
    QMenuBar *actions_bar;
    QToolBar *tool_bar;
    try
    {
    
        w = new Widget(0, file_name, k, n, eps, p);
        window = new QMainWindow;
        actions_bar = new QMenuBar(window);
        tool_bar = new QToolBar(window);

        w->setAutoFillBackground(true);
        action = actions_bar->addAction("&Change function", w, SLOT(change_function()));
        action->setShortcut(QString("0"));
        action = actions_bar->addAction("&Change graph", w, SLOT(change_graph()));
        action->setShortcut(QString("1"));
        action = actions_bar->addAction("&Increase scale", w, SLOT(increase_scale()));
        action->setShortcut(QString("2"));
        action = actions_bar->addAction("&Reduce scale", w, SLOT(reduce_scale()));
        action->setShortcut(QString("3"));
        action = actions_bar->addAction("&Increase n", w, SLOT(increase_n()));
        action->setShortcut(QString("4"));
        action = actions_bar->addAction("&Reduce n", w, SLOT(reduce_n()));
        action->setShortcut(QString("5"));
        action = actions_bar->addAction("&Add delta", w, SLOT(increase_p()));
        action->setShortcut(QString("6"));
        action = actions_bar->addAction("&Substract delta", w, SLOT(reduce_p()));
        action->setShortcut(QString("7"));
        action = actions_bar->addAction("&Rotate clockwise", w, SLOT(rotate_clockwise()));
        action->setShortcut(QString("8"));
        action = actions_bar->addAction("&Rotate counterclockwise", w, SLOT(rotate_counterclockwise()));
        action->setShortcut(QString("9"));
        
        
        action = tool_bar->addAction("&Change projection", w, SLOT(change_projection()));
        action->setShortcut(QString("p"));
        action = tool_bar->addAction("&Change point of view", w, SLOT(change_point_of_view()));
        action->setShortcut(QString("-"));
        action = tool_bar->addAction("&Draw a coordinate grid", w, SLOT(coordinate_grid()));
        action->setShortcut(QString("c"));
        action = tool_bar->addAction("&Draw a split grid", w, SLOT(split_grid()));
        action->setShortcut(QString("g"));
        action = tool_bar->addAction ("&Exit", window, SLOT (close()));
        action->setShortcut (QString ("Ctrl+C"));

        

        window->setMenuBar(actions_bar);
        window->addToolBar(Qt::BottomToolBarArea, tool_bar);
        window->setCentralWidget(w);
        window->show();
        app.exec();

    }
    catch(int err)
    {
        return -1;
    }
    
    delete actions_bar;
    delete tool_bar;
    delete w;
    delete window;
    return 0;
}