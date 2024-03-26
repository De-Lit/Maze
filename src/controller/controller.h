#ifndef MAZE_CONTROLLER_CONTROLLER_H_
#define MAZE_CONTROLLER_CONTROLLER_H_

#include <QString>
#include <iostream>

#include "cave.h"
#include "maze.h"

namespace maze {
class Controller {
 public:
  Controller() = default;
  Controller(maze::Maze &m, maze::Cave &c) : m_(&m), c_(&c) {}
  Controller(const Controller &other) = delete;
  Controller(Controller &&other) = delete;
  Controller &operator=(const Controller &other) = delete;
  Controller &operator=(Controller &&other) = delete;
  ~Controller() = default;

  pair<QString, QString> ParseMaze(QString path) {
    try {
      m_->Parse(path.toStdString());
      return pair<QString, QString>("Success", path);
    } catch (const std::exception &e) {
      return pair<QString, QString>("failure: incorrect file " + path, "");
    }
  }

  pair<vector<vector<int>> &, vector<vector<int>> &> GetMaze() {
    return m_->GetData();
  }

  list<pair<int, int>> &GetPath() { return m_->GetPath(); }

  void GenerateMaze(int height, int width) { m_->GenerateData(height, width); }

  QString FindPath(int start_x, int start_y, int end_x, int end_y) {
    try {
      m_->FindPath(start_x, start_y, end_x, end_y);
      return "Success";
    } catch (const std::exception &e) {
      return e.what();
    }
  }

  QString SaveMaze(QString path) {
    try {
      m_->Save(path.toStdString());
      return "File" + path + "saved";
    } catch (const std::exception &e) {
      return "failure: emty size. "
             "File " +
             path + " not saved";
    }
  }

  pair<QString, QString> ParseCave(QString path) {
    try {
      c_->Parse(path.toStdString());
      return pair<QString, QString>("Success", path);
    } catch (const std::exception &e) {
      return pair<QString, QString>("failure: incorrect file " + path, "");
    }
  }

  vector<vector<int>> &GetCave() { return c_->GetData(); }

  void EvolutionStep(int born_limit, int death_limit) {
    c_->EvolutionStep(born_limit, death_limit);
  }

  void GenerateCave(int height, int width, double chance) {
    c_->GenerateData(height, width, chance);
  }

  QString SaveCave(QString path) {
    try {
      c_->Save(path.toStdString());
      return "File" + path + "saved";
    } catch (const std::exception &e) {
      return "failure: emty size. "
             "File " +
             path + " not saved";
    }
  }

 private:
  maze::Maze *m_ = nullptr;
  maze::Cave *c_ = nullptr;
};
}  // namespace maze
#endif  // MAZE_CONTROLLER_CONTROLLER_H_
