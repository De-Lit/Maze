#include <QApplication>

#include "controller.h"
#include "cave.h"
#include "maze.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  setlocale(LC_NUMERIC, "C");
  maze::Maze m;
  maze::Cave cv;
  maze::Controller c(m, cv);
  maze::MainWindow mw(c);
  mw.show();
  return a.exec();
}
