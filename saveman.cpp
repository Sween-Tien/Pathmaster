#include "saveman.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <QMessageBox>
#include <QPushButton>

using namespace std;

// 定义方向编码
const int EAST = 0, SOUTH = 1, WEST = 2, NORTH = 3;
// 定义四个方向的移动方式：东、南、西、北
const int dx[] = { 0, 1, 0, -1 };  // 对应行的移动：不变、向下、向上
const int dy[] = { 1, 0, -1, 0 };  // 对应列的移动：向右、向下、向左

// 用于存储所有可能的路径
vector<vector<tuple<int, int, int>>> all_paths;
vector<tuple<int, int, int>> best_path;

// 深度优先搜索函数
void dfs(vector<vector<int>>& grid, vector<tuple<int, int, int>>& path, int row, int col, int end_row, int end_col, QWidget* parent) {
    // 如果当前坐标越界或遇到障碍，直接返回
    if (row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size() || grid[row][col] == 1) {
        return;
    }

    // 如果到达目标位置，将当前路径添加到所有路径集合中
    if (row == end_row && col == end_col) {
        all_paths.push_back(path);
        if (best_path.empty() || path.size() < best_path.size()) {
            best_path = path; // 更新最优路径
        }
        return;
    }

    // 标记当前坐标已访问
    grid[row][col] = 1;

    // 遍历四个方向：东、南、西、北
    for (int direction = 0; direction < 4; ++direction) {
        int next_row = row + dx[direction];
        int next_col = col + dy[direction];
        path.emplace_back(row, col, direction); // 添加当前位置和方向到路径
        dfs(grid, path, next_row, next_col, end_row, end_col, parent);
        path.pop_back(); // 回溯
    }

    // 取消标记
    grid[row][col] = 0;
}


void savearray::arraysave(const std::vector<std::vector<int>>& array, QWidget* parent) {
    vector<vector<int>> grid = array;

    int start_row = 0, start_col = 0;
    int end_row = 9, end_col = 9;

    vector<tuple<int, int, int>> path;
    dfs(grid, path, start_row, start_col, end_row, end_col, parent);

    // 输出结果并将路径导出为二维数组
    QString result; // 拼接输出字符串
    vector<vector<int>> path_array;  // 用于存储导出的路径数组

    for (const auto& point : best_path) {
        int x = std::get<0>(point);
        int y = std::get<1>(point);
        int z = std::get<2>(point);

        // 将路径上的坐标添加到 path_array 中
        path_array.push_back({ x, y });

        // 拼接成字符串格式，例如 "(x, y)"
        result += QString("(%1, %2, %3)\n").arg(x+1).arg(y+1).arg(z);

        // 在控制台输出
        //std::cout << "(" << x << ", " << y << ")" << std::endl;
    }

    if (!result.isEmpty()) {
        // 在GUI窗口中显示路径
        QMessageBox::information(parent, "Best Path", result);
    }
    else {
        QMessageBox::information(parent, "Best Path", "NO PATH FOUND!");
    }

    path_array.push_back({ 9, 9 });
    for (const auto& coords : path_array) {
        std::cout << "[" << coords[0] << ", " << coords[1] << "]" << std::endl;

        // 构建按钮的对象名，例如 P00, P12
        QString buttonName = QString("P%1%2").arg(coords[0]).arg(coords[1]);

        // 使用 findChild() 查找对应的按钮
        QPushButton* button = parent->findChild<QPushButton*>(buttonName);
        if (button) {
            // 将按钮背景设置为紫色
            button->setStyleSheet("background-color: #536bb6;");
        }
    }
    return;
}