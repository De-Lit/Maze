#ifndef MAZE_VIEW_MAINWINDOW_H_
#define MAZE_VIEW_MAINWINDOW_H_

#include <QFileDialog>
#include <QImage>
#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QWidget>
#include <cstring>

#include "controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace maze {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(maze::Controller &c);
  ~MainWindow();

 protected:
  void paintEvent(QPaintEvent *) override;

 private slots:
  void on_pushOpenMazeButton_clicked();

  void on_pushGenerateMazeButton_clicked();

  void on_pushSaveMazeButton_clicked();

  void on_pushFindPathButton_clicked();

  void on_pushOpenCaveButton_clicked();

  void on_pushEvolutionStepButton_clicked();

  void on_pushGenerateCaveButton_clicked();

  void on_pushEvolutionButton_clicked();

  void on_pushSaveCaveButton_clicked();

  void AutoEvolutionStep();

 private:
  void MazePaint();
  void CavePaint();

  Ui::MainWindow *ui_ = nullptr;
  Controller *c_ = nullptr;
  int trigger = 0;
  QTimer timer_;
  int count_steps_ = 0;
};
}  // namespace maze
#endif  // MAZE_VIEW_MAINWINDOW_H_
