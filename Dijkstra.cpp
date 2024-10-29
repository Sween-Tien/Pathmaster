#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>
#include "Dijkstra.h"
#include <QMessageBox>
#include <QPushButton>

using namespace std;

/// 定义方向编码和移动方式
const int dx[] = { 0, 1, 0, -1 };  // 行移动：东、南、西、北
const int dy[] = { 1, 0, -1, 0 };  // 列移动：东、南、西、北

// BFS算法寻找最短路径
vector<tuple<int, int>> bfsSearch(const vector<vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    // 检查起点和终点是否可达
    if (grid[0][0] == 1 || grid[rows - 1][cols - 1] == 1) {
        return {}; // 起点或终点被障碍物阻挡
    }

    queue<tuple<int, int, vector<tuple<int, int>>>> q; // 存储坐标和路径
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    // 起点
    q.push({ 0, 0, {{0, 0}} });
    visited[0][0] = true;

    while (!q.empty()) {
        auto [row, col, path] = q.front();
        q.pop();

        // 如果到达目标位置，返回路径
        if (row == rows - 1 && col == cols - 1) {
            return path; // 返回路径
        }

        // 遍历四个方向：东、南、西、北
        for (int direction = 0; direction < 4; ++direction) {
            int next_row = row + dx[direction];
            int next_col = col + dy[direction];

            // 检查新位置是否在网格范围内且未被访问，并且不是障碍物
            if (next_row >= 0 && next_row < rows && next_col >= 0 && next_col < cols &&
                grid[next_row][next_col] == 0 && !visited[next_row][next_col]) {

                visited[next_row][next_col] = true; // 标记为已访问

                // 更新路径并入队
                auto new_path = path;
                new_path.emplace_back(next_row, next_col); // 添加新节点到路径中
                q.push({ next_row, next_col, new_path });
            }
        }
    }

    return {}; // 如果没有找到通路，返回空路径
}

void Dijksarray::arrayDijks(const std::vector<std::vector<int>>& array, QWidget* parent) {
    // 使用题目中提供的9×8矩阵
    vector<vector<int>> grid = array;

    vector<tuple<int, int>> best_path = bfsSearch(grid);

    QString result; // 拼接输出字符串
    vector<vector<int>> path_array;  // 用于存储导出的路径数组

    for (const auto& point : best_path) {
        int x = std::get<0>(point);
        int y = std::get<1>(point);

       
        // 将路径上的坐标添加到 path_array 中
        path_array.push_back({ x, y });

        // 拼接成字符串格式，例如 "(x, y)"
        result += QString("(%1, %2)\n").arg(x + 1).arg(y + 1);

        // 在控制台输出
        //std::cout << "(" << x << ", " << y << ")" << std::endl;
    }

    // 打印最优路径坐标 (row, line, direction)
    if (!best_path.empty()) {
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
