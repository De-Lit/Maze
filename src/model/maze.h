#ifndef MAZE_MODEL_MAZE_H_
#define MAZE_MODEL_MAZE_H_

#include <cstdio>
#include <fstream>
#include <list>
#include <string>
#include <vector>

using namespace std;

namespace maze {
class Maze {
 public:
  Maze() = default;
  Maze(const Maze &other) = delete;
  Maze(Maze &&other) = delete;
  Maze &operator=(const Maze &other) = delete;
  Maze &operator=(Maze &&other) = delete;
  ~Maze() = default;

  void Parse(string path);
  pair<vector<vector<int>> &, vector<vector<int>> &> GetData();
  list<pair<int, int>> &GetPath();
  void GenerateData(int height, int width);
  void Save(string path);
  void FindPath(int start_x, int start_y, int end_x, int end_y);

 private:
  class Builder {
   public:
    Builder() = delete;
    Builder(int height, int width);
    Builder(const Builder &other) = delete;
    Builder(Builder &&other) = delete;
    Builder &operator=(const Builder &other) = delete;
    Builder &operator=(Builder &&other) = delete;
    ~Builder() = default;

    pair<vector<vector<int>> &, vector<vector<int>> &> GetData();

   private:
    void AssignUniqueSet();
    void AddingVerticalWalls(int row);
    void MergeSet(int index, int element);
    void AddingHorizontalWalls(int row);
    int CalculateUniqueSet(int element);
    void CheckedHorizontalWalls(int row);
    int CalculateHorizontalWalls(int element, int row);
    void PreparatingNewLine(int row);
    void AddingEndLine();
    void CheckedEndLine();

    vector<vector<int>> horizontal_;
    vector<vector<int>> vertical_;
    vector<int> maze_row_;
    int counter_ = 0;
  };

  void MakeStep(vector<vector<int>> &v, int k);

  vector<vector<int>> horizontal_;
  vector<vector<int>> vertical_;
  list<pair<int, int>> back_path_;
};
}  // namespace maze

#endif  // MAZE_MODEL_MAZE_H_
