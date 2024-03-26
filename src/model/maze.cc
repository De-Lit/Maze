#include "maze.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

void maze::Maze::Parse(string path) {
  int width = 0;
  int height = 0;
  ifstream in(path);
  if (!(in >> width) || !(in >> height) || width <= 0 || height <= 0) {
    in.close();
    throw runtime_error("Incorrect file");
  }
  vertical_.clear();
  horizontal_.clear();
  back_path_.clear();
  vertical_.resize(height, vector<int>(width));
  horizontal_.resize(height, vector<int>(width));
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (!(in >> vertical_[i][j])) {
        in.close();
        throw runtime_error("Incorrect file");
      }
    }
  }
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (!(in >> horizontal_[i][j])) {
        in.close();
        throw runtime_error("Incorrect file");
      }
    }
  }
  in.close();
}

pair<vector<vector<int>>&, vector<vector<int>>&> maze::Maze::GetData() {
  return pair<vector<vector<int>>&, vector<vector<int>>&>{vertical_,
                                                          horizontal_};
}

list<pair<int, int>>& maze::Maze::GetPath() { return back_path_; }

void maze::Maze::GenerateData(int height, int width) {
  Builder builder(height, width);
  auto pair = builder.GetData();
  vertical_ = move(pair.first);
  horizontal_ = move(pair.second);
  back_path_.clear();
}

void maze::Maze::Save(string path) {
  if (vertical_.empty() || horizontal_.empty())
    throw runtime_error("Incorrect size");
  std::ofstream out(path);
  out << vertical_.size() << " " << vertical_[0].size() << endl;
  for (size_t i = 0; i < vertical_[0].size(); ++i) {
    for (size_t j = 0; j < vertical_.size(); ++j) {
      out << vertical_[i][j] << " ";
    }
    out << endl;
  }
  out << endl;
  for (size_t i = 0; i < horizontal_[0].size(); ++i) {
    for (size_t j = 0; j < horizontal_.size(); ++j) {
      out << horizontal_[i][j] << " ";
    }
    out << endl;
  }
  out.close();
}

void maze::Maze::FindPath(int start_x, int start_y, int end_x, int end_y) {
  if (start_x < 0 || (size_t)start_x >= horizontal_.size() || start_y < 0 ||
      (size_t)start_y >= horizontal_[0].size() || end_x < 0 ||
      (size_t)end_x >= horizontal_.size() || end_y < 0 ||
      (size_t)end_y >= horizontal_[0].size())
    throw runtime_error("ArrayIndexOutOfBounds");
  vector<vector<int>> path(vertical_.size(), vector<int>(vertical_[0].size()));
  path[start_x][start_y] = 1;
  int k = 1;
  for (; path[end_x][end_y] == 0; ++k) {
    MakeStep(path, k);
  }
  list<pair<int, int>> back_path{pair<int, int>(end_x, end_y)};
  k = path[end_x][end_y];
  for (int i = end_x, j = end_y; k > 1;) {
    if (i > 0 && path[i - 1][j] == k - 1 && horizontal_[i - 1][j] == 0) {
      --i;
      back_path.push_front(pair<int, int>(i, j));
      --k;
    } else if (j > 0 && path[i][j - 1] == k - 1 && vertical_[i][j - 1] == 0) {
      --j;
      back_path.push_front(pair<int, int>(i, j));
      --k;
    } else if ((size_t)i < path.size() - 1 && path[i + 1][j] == k - 1 &&
               horizontal_[i][j] == 0) {
      ++i;
      back_path.push_front(pair<int, int>(i, j));
      --k;
    } else if ((size_t)j < path[0].size() - 1 && path[i][j + 1] == k - 1 &&
               vertical_[i][j] == 0) {
      ++j;
      back_path.push_front(pair<int, int>(i, j));
      --k;
    }
  }
  back_path.push_front(pair<int, int>(start_x, start_y));
  back_path_ = move(back_path);
}

void maze::Maze::MakeStep(vector<vector<int>>& v, int k) {
  for (size_t i = 0; i < v.size(); ++i) {
    for (size_t j = 0; j < v[0].size(); ++j) {
      if (v[i][j] == k) {
        if (i > 0 && v[i - 1][j] == 0 && horizontal_[i - 1][j] == 0)
          v[i - 1][j] = k + 1;
        if (j > 0 && v[i][j - 1] == 0 && vertical_[i][j - 1] == 0)
          v[i][j - 1] = k + 1;
        if ((size_t)i < v.size() - 1 && v[i + 1][j] == 0 &&
            horizontal_[i][j] == 0)
          v[i + 1][j] = k + 1;
        if ((size_t)j < v[0].size() - 1 && v[i][j + 1] == 0 &&
            vertical_[i][j] == 0)
          v[i][j + 1] = k + 1;
      }
    }
  }
}

maze::Maze::Builder::Builder(int height, int width)
    : horizontal_(height, vector<int>(width)),
      vertical_(height, vector<int>(width)),
      maze_row_(width) {
  if (height <= 0 || width <= 0) throw runtime_error("Incorrect size");
  for (int i = 0; i < height - 1; ++i) {
    AssignUniqueSet();
    AddingVerticalWalls(i);
    AddingHorizontalWalls(i);
    CheckedHorizontalWalls(i);
    PreparatingNewLine(i);
  }
  AddingEndLine();
}

void maze::Maze::Builder::AssignUniqueSet() {
  for (size_t i = 0; i < maze_row_.size(); ++i) {
    if (maze_row_[i] == 0) {
      counter_++;
      maze_row_[i] = counter_;
    }
  }
}

/* Добавление правой вертикальной стенки */
void maze::Maze::Builder::AddingVerticalWalls(int row) {
  for (size_t i = 0; i < maze_row_.size() - 1; ++i) {
    /* Ставим стенку или нет */
    int choise = rand() % 2;
    /* Проверка условия для предотовращения зацикливания */
    if (choise == 1 || maze_row_[i] == maze_row_[i + 1]) {
      vertical_[row][i] = 1;
    } else {
      /* Объединение ячеек в одно множество */
      MergeSet(i, maze_row_[i]);
    }
  }
  /* Добавление правой стенки в последней ячейки */
  vertical_[row][maze_row_.size() - 1] = 1;
}

/* Объединение ячеек в одно множество */
void maze::Maze::Builder::MergeSet(int index, int element) {
  int mutable_set = maze_row_[index + 1];
  for (size_t i = 0; i < maze_row_.size(); ++i) {
    /* Проверка ячеек на одно множество */
    if (maze_row_[i] == mutable_set) {
      /* Объединение ячейки в множество */
      maze_row_[i] = element;
    }
  }
}

/* Добавление нижней горизонтальной стенки */
void maze::Maze::Builder::AddingHorizontalWalls(int row) {
  for (size_t i = 0; i < maze_row_.size(); ++i) {
    /* Ставим стенку или нет */
    int choise = rand() % 2;
    /* Проверка, что множество имеет более одной ячейки (это предовратит
     * замкнутые области  */
    if (CalculateUniqueSet(maze_row_[i]) != 1 && choise == 1) {
      /* Ставим горизонтальную стенку */
      horizontal_[row][i] = 1;
    }
  }
}

/* Подсчет ячеек, которые содержаться в уникальном множестве */
int maze::Maze::Builder::CalculateUniqueSet(int element) {
  int count_uniq_set = 0;
  for (size_t i = 0; i < maze_row_.size(); ++i) {
    if (maze_row_[i] == element) {
      count_uniq_set++;
    }
  }
  return count_uniq_set;
}

/* Проверка условие 4.1 и 4.2 */
void maze::Maze::Builder::CheckedHorizontalWalls(int row) {
  for (size_t i = 0; i < maze_row_.size(); ++i) {
    if (CalculateHorizontalWalls(maze_row_[i], row) == 0) {
      horizontal_[row][i] = 0;
    }
  }
}

/* Подсчет горизонтальных стен у ячеек уникального множества */
int maze::Maze::Builder::CalculateHorizontalWalls(int element, int row) {
  int count_horizontal_walls = 0;
  for (size_t i = 0; i < maze_row_.size(); ++i) {
    if (maze_row_[i] == element && horizontal_[row][i] == 0) {
      count_horizontal_walls++;
    }
  }
  return count_horizontal_walls;
}

void maze::Maze::Builder::PreparatingNewLine(int row) {
  for (size_t i = 0; i < maze_row_.size(); ++i) {
    if (horizontal_[row][i] == 1) {
      /* Присваиваем ячейки пустое множество */
      maze_row_[i] = 0;
    }
  }
}

/* Добавление последней строки */
void maze::Maze::Builder::AddingEndLine() {
  AssignUniqueSet();
  AddingVerticalWalls(vertical_.size() - 1);
  CheckedEndLine();
}

/* Проверка условий на добавление последней строки */
void maze::Maze::Builder::CheckedEndLine() {
  for (size_t i = 0; i < maze_row_.size() - 1; ++i) {
    /* Проверка условия пункта 5.2.1 */
    if (maze_row_[i] != maze_row_[i + 1]) {
      /* Убираем вертикальную стенку */
      vertical_[vertical_.size() - 1][i] = 0;
      /* Объединяем множества */
      MergeSet(i, maze_row_[i]);
    }
    /* Добавляем горизонтальные стенки */
    horizontal_[horizontal_.size() - 1][i] = 1;
  }
  /* Добавляем горизонтальную стенку в последней ячейке */
  horizontal_[horizontal_.size() - 1][horizontal_[0].size() - 1] = 1;
}

pair<vector<vector<int>>&, vector<vector<int>>&>
maze::Maze::Builder::GetData() {
  return pair<vector<vector<int>>&, vector<vector<int>>&>{vertical_,
                                                          horizontal_};
}
