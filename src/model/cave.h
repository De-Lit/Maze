#ifndef MAZE_MODEL_CAVE_H_
#define MAZE_MODEL_CAVE_H_

#include <cstdio>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

namespace maze {
class Cave {
 public:
  Cave() = default;
  Cave(const Cave &other) = delete;
  Cave(Cave &&other) = delete;
  Cave &operator=(const Cave &other) = delete;
  Cave &operator=(Cave &&other) = delete;
  ~Cave() = default;

  void Parse(string path);
  void GenerateData(int height, int width, double chance);
  void EvolutionStep(int born_limit, int death_limit);
  vector<vector<int>> &GetData();
  void Save(string path);

 private:
  int CountLiveNeighbours(int row, int coll);

  vector<vector<int>> cells_;
};
}  // namespace maze

#endif  // MAZE_MODEL_CAVE_H_
