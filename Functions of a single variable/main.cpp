#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>

#include "window.h"

int main (int argc, char *argv[])
{
  QApplication app (argc, argv);

  QMainWindow *window = new QMainWindow;
  QMenuBar *tool_bar = new QMenuBar(window);
  Window *graph_area = new Window(window);
  QAction *action;

  if (graph_area->command_line(argc, argv))
  {
    QMessageBox::warning (0,  "Wrong input arguments!", 
                              "Usage: a b n");
    return -1;
  }

  action = tool_bar->addAction("&Change method", graph_area, SLOT(change_approx()));
  action->setShortcut (QString("1"));
  action = tool_bar->addAction("&Change function", graph_area, SLOT(change_func()));
  action->setShortcut (QString("3"));
  action = tool_bar->addAction("&Exit", window, SLOT (close ()));
  action->setShortcut (QString("Ctrl+X"));
  action = tool_bar->addAction("&Reduce n", graph_area, SLOT(reduce_n()));
  action->setShortcut (QString("2"));
  action = tool_bar->addAction("&Increase n", graph_area, SLOT(increase_n()));
  action->setShortcut (QString("8"));
  action = tool_bar->addAction("&Residual", graph_area, SLOT(residual()));
  action->setShortcut (QString("5"));
  action = tool_bar->addAction("-", graph_area, SLOT(increase_scale()));
  action->setShortcut (QString("-"));
  action = tool_bar->addAction("+", graph_area, SLOT(decrease_scale()));
  action->setShortcut (QString("+"));

  tool_bar->setMaximumHeight(30);

  window->setMenuBar(tool_bar);
  window->setCentralWidget (graph_area);
  window->setWindowTitle("Graph");

  window->show();
  app.exec();
  delete window;
  return 0;
}
