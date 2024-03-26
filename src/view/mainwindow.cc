#include "mainwindow.h"

#include <QFileDialog>
#include <QImage>
#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QWidget>
#include <cstring>
#include <QTimer>

#include "ui_mainwindow.h"

maze::MainWindow::MainWindow(maze::Controller &c)
    : ui_(new Ui::MainWindow), c_(&c) {
  ui_->setupUi(this);

  connect(&timer_, &QTimer::timeout, this, &MainWindow::AutoEvolutionStep);
}

maze::MainWindow::~MainWindow() { delete ui_; }

void maze::MainWindow::paintEvent(QPaintEvent *) {
  switch (trigger) {
    case 0: {
      MazePaint();
      break;
    }
    case 1:
      CavePaint();
      break;
  }
}

void maze::MainWindow::on_pushOpenMazeButton_clicked() {
  QString file_name = QFileDialog::getOpenFileName(this, "Load file");
  if (strlen(qPrintable(file_name))) {
    auto status = c_->ParseMaze(file_name);
    ui_->statusbar->showMessage(status.first);
    ui_->label->setText(status.second);
    trigger = 0;
    update();
  }
}

void maze::MainWindow::on_pushGenerateMazeButton_clicked() {
  c_->GenerateMaze(ui_->spinBox->value(), ui_->spinBox_2->value());
  ui_->label->setText("");
  trigger = 0;
  update();
}

void maze::MainWindow::on_pushSaveMazeButton_clicked() {
  if (trigger == 0) {
    QString file_name =
        QFileDialog::getSaveFileName(this, "Save File", QDir::homePath());
    if (!file_name.isNull()) {
      auto status = c_->SaveMaze(file_name);
      ui_->statusbar->showMessage(status);
    }
  }
}

void maze::MainWindow::on_pushFindPathButton_clicked() {
  auto status =
      c_->FindPath(ui_->startXspinBox->value(), ui_->startYspinBox->value(),
                   ui_->endXspinBox->value(), ui_->endYspinBox->value());
  ui_->statusbar->showMessage(status);
  update();
}

void maze::MainWindow::on_pushOpenCaveButton_clicked() {
  QString file_name = QFileDialog::getOpenFileName(this, "Load file");
  if (strlen(qPrintable(file_name))) {
    auto status = c_->ParseCave(file_name);
    ui_->statusbar->showMessage(status.first);
    ui_->label->setText(status.second);
    trigger = 1;
    update();
  }
}

void maze::MainWindow::MazePaint() {
  auto maze = c_->GetMaze();
  auto path = c_->GetPath();
  if (get<0>(maze).size() != 0 && get<1>(maze).size() != 0) {
    auto delta_height = ui_->widget->height() / get<0>(maze).size();
    auto delta_width = ui_->widget->width() / get<0>(maze)[0].size();
    auto shift_x =
        (ui_->widget->width() - get<0>(maze)[0].size() * delta_width) / 2;
    auto shift_y =
        (ui_->widget->height() - get<0>(maze).size() * delta_height) / 2;
    QPen pen;
    pen.setWidth(2);
    QPainter painter(this);
    painter.setPen(pen);
    QBrush backgroundBrush("#1E776D");
    painter.fillRect(ui_->widget->rect(), backgroundBrush);

    for (size_t i = 0; i < get<0>(maze).size(); ++i) {
      for (size_t j = 0; j < get<0>(maze)[0].size(); ++j) {
        if (i == 0) {
          painter.drawLine(j * delta_width + shift_x, shift_y,
                           (j + 1) * delta_width + shift_x, shift_y);
        }
        if (j == 0) {
          painter.drawLine(shift_x, i * delta_height + shift_y, shift_x,
                           (i + 1) * delta_height + shift_y);
        }
        if (get<0>(maze)[i][j] == 1) {
          painter.drawLine((j + 1) * delta_width + shift_x,
                           i * delta_height + shift_y,
                           (j + 1) * delta_width + shift_x,
                           (i + 1) * delta_height + shift_y);
        }
        if (get<1>(maze)[i][j] == 1) {
          painter.drawLine(j * delta_width + shift_x,
                           (i + 1) * delta_height + shift_y,
                           (j + 1) * delta_width + shift_x,
                           (i + 1) * delta_height + shift_y);
        }
      }
    }
    if (!path.empty()) {
      pen.setColor(Qt::cyan);
      painter.setPen(pen);
      for (auto point_1 = path.begin(), point_2 = ++path.begin();
           point_2 != path.end(); ++point_1, ++point_2)
        painter.drawLine(
            point_1->second * delta_width + delta_width / 2 + shift_x,
            point_1->first * delta_height + delta_height / 2 + shift_y,
            point_2->second * delta_width + delta_width / 2 + shift_x,
            point_2->first * delta_height + delta_height / 2 + shift_y);
    }
  }
}

void maze::MainWindow::CavePaint() {
  auto cave = c_->GetCave();
  if (cave.size() != 0 && cave[0].size() != 0) {
    auto delta_height = ui_->widget->height() / cave.size();
    auto delta_width = ui_->widget->width() / cave[0].size();
    auto shift_x = (ui_->widget->width() - cave[0].size() * delta_width) / 2;
    auto shift_y = (ui_->widget->height() - cave.size() * delta_height) / 2;
    QPolygon pol;
    QBrush brush;
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    QPainter painter(this);
    painter.setBrush(brush);
    QBrush backgroundBrush("#1E776D");
    painter.fillRect(ui_->widget->rect(), backgroundBrush);
    for (size_t i = 1; i < cave.size() - 1; ++i) {
      for (size_t j = 1; j < cave[0].size() - 1; ++j) {
        if (cave[i][j] == 1) {
          pol.clear();
          pol << QPoint(j * delta_width + shift_x, i * delta_height + shift_y)
              << QPoint(j * delta_width + delta_width + shift_x,
                        i * delta_height + shift_y)
              << QPoint(j * delta_width + delta_width + shift_x,
                        i * delta_height + delta_height + shift_y)
              << QPoint(j * delta_width + shift_x,
                        i * delta_height + delta_height + shift_y)
              << QPoint(j * delta_width + shift_x, i * delta_height + shift_y);
          painter.drawPolygon(pol);
        }
      }
    }
  }
}

void maze::MainWindow::on_pushEvolutionStepButton_clicked() {
  if (trigger == 1) {
    c_->EvolutionStep(ui_->spinBox_3->value(), ui_->spinBox_4->value());
    c_->GetCave();
    update();
  }
}

void maze::MainWindow::on_pushGenerateCaveButton_clicked() {
  c_->GenerateCave(ui_->spinBox_5->value(), ui_->spinBox_6->value(),
                   ui_->doubleSpinBox->value());
  ui_->label->setText("");
  trigger = 1;
  update();
}

void maze::MainWindow::on_pushEvolutionButton_clicked() {
  if (trigger == 1) {
    timer_.setInterval(ui_->spinBox_8->value());
    timer_.start();
  }
}

void maze::MainWindow::AutoEvolutionStep() {
  on_pushEvolutionStepButton_clicked();
  count_steps_++;
  if (count_steps_ == ui_->spinBox_7->value()) {
    timer_.stop();
    count_steps_ = 0;
  }
}

void maze::MainWindow::on_pushSaveCaveButton_clicked() {
  if (trigger == 1) {
    QString file_name =
        QFileDialog::getSaveFileName(this, "Save File", QDir::homePath());
    if (!file_name.isNull()) {
      auto status = c_->SaveCave(file_name);
      ui_->statusbar->showMessage(status);
    }
  }
}
