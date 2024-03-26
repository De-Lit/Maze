#include "cave.h"

#include <stdio.h>

#include <fstream>
#include <string>
#include <tuple>
#include <vector>

void maze::Cave::Parse(string path) {
  int width = 0;
  int height = 0;
  cells_.clear();
  ifstream in(path);
  if (!(in >> width) || !(in >> height) || width <= 0 || height <= 0) {
    in.close();
    throw runtime_error("Incorrect file");
  }
  width += 2;
  height += 2;
  cells_.clear();
  cells_.resize(width, vector<int>(height));
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      if (i == 0 || j == 0 || i == width - 1 || j == height - 1) {
        cells_[i][j] = 1;
      } else {
        if (!(in >> cells_[i][j])) {
          in.close();
          throw runtime_error("Incorrect file");
        }
      }
    }
  }
  in.close();
}

void maze::Cave::GenerateData(int height, int width, double chance) {
  if (height <= 0 || width <= 0) throw runtime_error("Incorrect size");
  cells_.clear();
  cells_.resize(height + 2, vector<int>(width + 2));
  for (size_t i = 0; i < cells_.size(); ++i) {
    for (size_t j = 0; j < cells_[0].size(); ++j) {
      if (i == 0 || j == 0 || i == cells_.size() - 1 ||
          j == cells_[0].size() - 1) {
        cells_[i][j] = 1;
      } else {
        if (chance >= (double)rand() / RAND_MAX) cells_[i][j] = 1;
      }
    }
  }
}

void maze::Cave::EvolutionStep(int born_limit, int death_limit) {
  vector<vector<int>> new_generation_(cells_.size(),
                                      vector<int>(cells_[0].size()));
  for (size_t i = 0; i < cells_.size(); ++i) {
    for (size_t j = 0; j < cells_[0].size(); ++j) {
      if (i == 0 || j == 0 || i == new_generation_.size() - 1 ||
          j == new_generation_[0].size() - 1) {
        new_generation_[i][j] = 1;
      } else {
        switch (cells_[i][j]) {
          case 0:
            if (CountLiveNeighbours(i, j) > born_limit)
              new_generation_[i][j] = 1;
            break;
          case 1:
            if (CountLiveNeighbours(i, j) < death_limit)
              new_generation_[i][j] = 0;
            else
              new_generation_[i][j] = 1;
            break;
        }
      }
    }
  }
  cells_ = move(new_generation_);
}

vector<vector<int>> &maze::Cave::GetData() { return cells_; }

void maze::Cave::Save(string path) {
  if (cells_.empty()) throw runtime_error("Incorrect size");
  std::ofstream out(path);
  out << cells_.size() - 2 << " " << cells_[0].size() - 2 << endl;
  for (size_t i = 1; i < cells_[0].size() - 1; ++i) {
    for (size_t j = 1; j < cells_.size() - 1; ++j) {
      out << cells_[i][j] << " ";
    }
    out << endl;
  }
  out.close();
}

int maze::Cave::CountLiveNeighbours(int row, int col) {
  int count = 0;
  for (int item : {cells_[row][col - 1], cells_[row][col + 1],
                   cells_[row - 1][col], cells_[row + 1][col],
                   cells_[row - 1][col - 1], cells_[row - 1][col + 1],
                   cells_[row + 1][col - 1], cells_[row + 1][col + 1]})
    if (item != 0) ++count;
  return count;
}
