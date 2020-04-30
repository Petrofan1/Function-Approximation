#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>

#include "window.h"

int main (int argc, char *argv[])
{
  QApplication app(argc, argv);
  QAction *action;
  QMainWindow *window = new QMainWindow;
  QMenuBar *actions_bar = new QMenuBar(window);
  Window *graph = new Window(window);
  if(graph->command_line(argc, argv))
  {
    QMessageBox::warning (0,  "Error!", "Usage: a b n k\nn > 2\nk = 0...6");
    delete window;
    return -1;
  }
  action = actions_bar->addAction("&Change method", graph, SLOT(change_approximation()));
  action->setShortcut(QString("1"));
  action = actions_bar->addAction("&Change function", graph, SLOT(change_function()));
  action->setShortcut(QString("0"));
  action = actions_bar->addAction("&Increase n", graph, SLOT(increase_n()));
  action->setShortcut(QString("4"));
  action = actions_bar->addAction("&Reduce n", graph, SLOT(reduce_n()));
  action->setShortcut(QString("5"));
  action = actions_bar->addAction("&Add delta", graph, SLOT(add_delta()));
  action->setShortcut(QString("6"));
  action = actions_bar->addAction("&Subtract delta", graph, SLOT(subtract_delta()));
  action->setShortcut(QString("7"));
  action = actions_bar->addAction("-", graph, SLOT(increase_scale()));
  action->setShortcut(QString("3"));
  action = actions_bar->addAction("+", graph, SLOT(decrease_scale()));
  action->setShortcut(QString("2"));
  action = actions_bar->addAction ("&Exit", window, SLOT (close ()));
  action->setShortcut (QString ("Ctrl+C"));
  actions_bar->setMaximumHeight(30);
  window->setMenuBar(actions_bar);
  window->setCentralWidget(graph);
  window->show();
  app.exec();
  delete window;
  return 0;
}
