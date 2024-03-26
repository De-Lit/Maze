#include <gtest/gtest.h>

#include "../model/cave.h"
#include "../model/maze.h"

TEST(MazeCreate, 1) {
  // Arrange
  // Act
  maze::Maze m;
  // Assert
  auto pair = m.GetData();
  auto path = m.GetPath();
  ASSERT_EQ(pair.first.empty(), true);
  ASSERT_EQ(pair.second.empty(), true);
  ASSERT_EQ(path.empty(), true);
}

TEST(MazeParse, 1) {
  // Arrange
  maze::Maze m;
  std::string file_dir = "tests/materials/maze_example";
  // Act
  m.Parse(file_dir);
  // Assert
  auto pair = m.GetData();
  auto path = m.GetPath();
  ASSERT_EQ(pair.first.size(), 10);
  ASSERT_EQ(pair.first[0].size(), 10);
  ASSERT_EQ(pair.first[0][0], 0);
  ASSERT_EQ(pair.first[9][9], 1);
  ASSERT_EQ(pair.second.size(), 10);
  ASSERT_EQ(pair.second[0].size(), 10);
  ASSERT_EQ(pair.second[0][0], 0);
  ASSERT_EQ(pair.second[9][9], 1);
  ASSERT_EQ(path.empty(), true);
}

TEST(MazeParse, 2) {
  // Arrange
  maze::Maze m;
  std::string file_dir = "tests/materials/incorrect_example_1";
  // Act
  // Assert
  ASSERT_THROW(m.Parse(file_dir), std::runtime_error);
}

TEST(MazeParse, 3) {
  // Arrange
  maze::Maze m;
  std::string file_dir = "tests/materials/incorrect_example_2";
  // Act
  // Assert
  ASSERT_THROW(m.Parse(file_dir), std::runtime_error);
}

TEST(MazeParse, 4) {
  // Arrange
  maze::Maze m;
  std::string file_dir = "tests/materials/incorrect_example_3";
  // Act
  // Assert
  ASSERT_THROW(m.Parse(file_dir), std::runtime_error);
}

TEST(MazeGenerateDate, 1) {
  // Arrange
  maze::Maze m;
  int rows = 5;
  int columns = 5;
  // Act
  m.GenerateData(rows, columns);
  // Assert
  auto pair = m.GetData();
  auto path = m.GetPath();
  ASSERT_EQ(pair.first.size(), rows);
  ASSERT_EQ(pair.first[0].size(), columns);
  ASSERT_EQ(pair.second.size(), rows);
  ASSERT_EQ(pair.second[0].size(), columns);
  ASSERT_EQ(path.empty(), true);
}

TEST(MazeGenerateDate, 2) {
  // Arrange
  maze::Maze m;
  int rows = 5;
  int columns = 10;
  // Act
  m.GenerateData(rows, columns);
  // Assert
  auto pair = m.GetData();
  auto path = m.GetPath();
  ASSERT_EQ(pair.first.size(), rows);
  ASSERT_EQ(pair.first[0].size(), columns);
  ASSERT_EQ(pair.second.size(), rows);
  ASSERT_EQ(pair.second[0].size(), columns);
  ASSERT_EQ(path.empty(), true);
}

TEST(MazeGenerateDate, 3) {
  // Arrange
  maze::Maze m;
  int rows = 10;
  int columns = 5;
  // Act
  m.GenerateData(rows, columns);
  // Assert
  auto pair = m.GetData();
  auto path = m.GetPath();
  ASSERT_EQ(pair.first.size(), rows);
  ASSERT_EQ(pair.first[0].size(), columns);
  ASSERT_EQ(pair.second.size(), rows);
  ASSERT_EQ(pair.second[0].size(), columns);
  ASSERT_EQ(path.empty(), true);
}

TEST(MazeFindPath, 1) {
  // Arrange
  maze::Maze m;
  std::string file_dir = "tests/materials/maze_example";
  m.Parse(file_dir);
  auto pair = m.GetData();
  int rows = pair.first.size();
  int columns = pair.first[0].size();
  // Act
  m.FindPath(0, 0, rows - 1, columns - 1);
  // Assert
  auto path = m.GetPath();
  ASSERT_EQ(path.empty(), false);
  ASSERT_EQ(path.front().first, 0);
  ASSERT_EQ(path.front().second, 0);
  ASSERT_EQ(path.back().first, rows - 1);
  ASSERT_EQ(path.back().second, columns - 1);
}

TEST(MazeFindPath, 2) {
  // Arrange
  maze::Maze m;
  std::string file_dir = "tests/materials/maze_example";
  m.Parse(file_dir);
  auto pair = m.GetData();
  int rows = pair.first.size();
  int columns = pair.first[0].size();
  // Act
  // Assert
  ASSERT_THROW(m.FindPath(0, 0, rows, columns), std::runtime_error);
}

TEST(MazeSave, 1) {
  // Arrange
  maze::Maze m;
  maze::Maze m_load;
  std::string file_dir = "tests/materials/maze_example";
  std::string save_file_dir = "tests/materials/save_test";
  m.Parse(file_dir);
  // Act
  m.Save(save_file_dir);
  // Assert
  m_load.Parse(save_file_dir);
  auto pair_m = m.GetData();
  auto pair_m_load = m.GetData();

  ASSERT_EQ(pair_m.first.size(), pair_m_load.first.size());
  ASSERT_EQ(pair_m.first.empty(), false);
  ASSERT_EQ(pair_m.second.empty(), false);
  ASSERT_EQ(pair_m.first[0].size(), pair_m_load.first[0].size());
  ASSERT_EQ(pair_m.second.size(), pair_m_load.second.size());
  ASSERT_EQ(pair_m.first[0].empty(), false);
  ASSERT_EQ(pair_m.second[0].empty(), false);
  ASSERT_EQ(pair_m.second[0].size(), pair_m_load.second[0].size());
  for (size_t i = 0; i < pair_m.first.size(); ++i) {
    for (size_t j = 0; j < pair_m.first[0].size(); ++j) {
      ASSERT_EQ(pair_m.first[i][j], pair_m_load.first[i][j]);
      ASSERT_EQ(pair_m.second[i][j], pair_m_load.second[i][j]);
    }
  }
  remove(save_file_dir.c_str());
}

TEST(MazeSave, 2) {
  // Arrange
  maze::Maze m;
  std::string save_file_dir = "tests/materials/save_test";
  // Act
  // Assert
  ASSERT_THROW(m.Save(save_file_dir), std::runtime_error);
}

TEST(CaveCreate, 1) {
  // Arrange
  // Act
  maze::Cave c;
  // Assert
  auto arr = c.GetData();
  ASSERT_EQ(arr.empty(), true);
}

TEST(CaveParse, 1) {
  // Arrange
  maze::Cave c;
  std::string file_dir = "tests/materials/cave_example_1";
  // Act
  c.Parse(file_dir);
  // Assert
  auto arr = c.GetData();
  ASSERT_EQ(arr.size() - 2, 10);
  ASSERT_EQ(arr[0].size() - 2, 10);
  ASSERT_EQ(arr[1][1], 1);
  ASSERT_EQ(arr[arr.size() - 2][arr.size() - 2], 0);
}

TEST(CaveParse, 2) {
  // Arrange
  maze::Cave c;
  std::string file_dir = "tests/materials/incorrect_example_1";
  // Act
  // Assert
  ASSERT_THROW(c.Parse(file_dir), std::runtime_error);
}

TEST(CaveParse, 3) {
  // Arrange
  maze::Cave c;
  std::string file_dir = "tests/materials/incorrect_example_2";
  // Act
  // Assert
  ASSERT_THROW(c.Parse(file_dir), std::runtime_error);
}

TEST(CaveGenerateDate, 1) {
  // Arrange
  maze::Cave c;
  int rows = 5;
  int columns = 5;
  // Act
  c.GenerateData(rows, columns, 0.5);
  // Assert
  auto arr = c.GetData();
  ASSERT_EQ(arr.size() - 2, 5);
  ASSERT_EQ(arr[0].size() - 2, 5);
}

TEST(CaveGenerateDate, 2) {
  // Arrange
  maze::Cave c;
  int rows = 5;
  int columns = 10;
  // Act
  c.GenerateData(rows, columns, 1.);
  // Assert
  auto arr = c.GetData();
  ASSERT_EQ(arr.size() - 2, rows);
  ASSERT_EQ(arr[0].size() - 2, columns);
  for (auto row : arr)
    for (auto value : row) ASSERT_EQ(value, 1);
}

TEST(CaveGenerateDate, 3) {
  // Arrange
  maze::Cave c;
  int rows = 10;
  int columns = 5;
  // Act
  c.GenerateData(rows, columns, 0.);
  // Assert
  auto arr = c.GetData();
  ASSERT_EQ(arr.size() - 2, rows);
  ASSERT_EQ(arr[0].size() - 2, columns);
  for (size_t i = 1; i < arr.size() - 1; ++i)
    for (size_t j = 1; j < arr[0].size() - 1; ++j) ASSERT_EQ(arr[i][j], 0);
}

TEST(CaveEvolutionStep, 1) {
  // Arrange
  maze::Cave c;
  maze::Cave c_load;
  std::string file_dir = "tests/materials/cave_example_1";
  std::string load_file_dir = "tests/materials/cave_example_2";
  c.Parse(file_dir);
  int born_limit = 4;
  int death_limit = 3;
  int step_count = 7;
  // Act
  for (int i = 0; i < step_count; ++i) c.EvolutionStep(born_limit, death_limit);
  // Assert
  c_load.Parse(load_file_dir);
  auto arr_c = c.GetData();
  auto arr_c_load = c_load.GetData();

  ASSERT_EQ(arr_c.size(), arr_c_load.size());
  ASSERT_EQ(arr_c.empty(), false);
  ASSERT_EQ(arr_c_load.empty(), false);
  ASSERT_EQ(arr_c[0].size(), arr_c_load[0].size());
  for (size_t i = 0; i < arr_c.size(); ++i)
    for (size_t j = 0; j < arr_c[0].size(); ++j)
      ASSERT_EQ(arr_c[i][j], arr_c_load[i][j]);
}

TEST(CaveEvolutionStep, 2) {
  // Arrange
  maze::Cave c;
  std::string file_dir = "tests/materials/cave_example_3";
  c.Parse(file_dir);
  int born_limit = 2;
  int death_limit = 5;
  int step_count = 19;
  // Act
  for (int i = 0; i < step_count; ++i) c.EvolutionStep(born_limit, death_limit);
  // Assert
  auto arr = c.GetData();
  for (size_t i = 0; i < arr.size(); ++i)
    for (size_t j = 0; j < arr[0].size(); ++j) ASSERT_EQ(arr[i][j], 1);
}

TEST(CaveSave, 1) {
  // Arrange
  maze::Cave c;
  maze::Cave c_load;
  std::string file_dir = "tests/materials/cave_example_1";
  std::string save_file_dir = "tests/materials/save_test";
  c.Parse(file_dir);
  // Act
  c.Save(save_file_dir);
  // Assert
  c_load.Parse(save_file_dir);
  auto arr_c = c.GetData();
  auto arr_c_load = c.GetData();

  ASSERT_EQ(arr_c.size(), arr_c_load.size());
  ASSERT_EQ(arr_c.empty(), false);
  ASSERT_EQ(arr_c_load.empty(), false);
  ASSERT_EQ(arr_c[0].size(), arr_c_load[0].size());
  for (size_t i = 0; i < arr_c.size(); ++i)
    for (size_t j = 0; j < arr_c[0].size(); ++j)
      ASSERT_EQ(arr_c[i][j], arr_c_load[i][j]);
  remove(save_file_dir.c_str());
}

TEST(CaveSave, 2) {
  // Arrange
  maze::Cave c;
  std::string save_file_dir = "tests/materials/save_test";
  // Act
  // Assert
  ASSERT_THROW(c.Save(save_file_dir), std::runtime_error);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
