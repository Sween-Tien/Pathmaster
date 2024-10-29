#include <stack>
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>
#include <cmath>
#include <QString>
#include <QMessageBox>
#include <QPushButton>
#include "Greedy.h"

using namespace std;

// 定义方向数组和对应的方向编码
const int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
const int directionCodes[4] = { 2, 1, 4, 3 }; // 方向编码：上=2, 下=1, 左=4, 右=3

// BFS算法
vector<tuple<int, int, int>> bfsShortestPath(const vector<vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    // 检查起点和终点是否有效
    if (grid[0][0] == 1 || grid[rows - 1][cols - 1] == 1) {
        return {}; // 起点或终点为障碍，无法到达
    }

    // 创建一个队列用于BFS
    queue<pair<int, int>> q;
    q.push({ 0, 0 }); // 从起点开始
    vector<vector<int>> distance(rows, vector<int>(cols, -1)); // 距离矩阵
    distance[0][0] = 0; // 起点的距离设为0（包含起点）

    // 记录路径的方向
    vector<vector<pair<int, int>>> prev(rows, vector<pair<int, int>>(cols, { -1, -1 }));

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        // 遍历四个方向
        for (int dir = 0; dir < 4; ++dir) {
            int newX = x + directions[dir][0];
            int newY = y + directions[dir][1];

            // 检查新位置是否在边界内且是通道
            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && grid[newX][newY] == 0 && distance[newX][newY] == -1) {
                // 更新距离
                distance[newX][newY] = distance[x][y] + 1;
                q.push({ newX, newY }); // 加入队列

                // 记录路径
                prev[newX][newY] = { x, y }; // 记录当前点的前驱点

                // 如果到达终点，构造路径并返回
                if (newX == rows - 1 && newY == cols - 1) {
                    vector<tuple<int, int, int>> path; // 存储路径三元组
                    int curX = newX, curY = newY;

                    // 回溯路径
                    while (curX != 0 || curY != 0) {
                        auto [prevX, prevY] = prev[curX][curY];
                        int direction;
                        if (prevX == curX - 1) direction = 1; // 向下
                        else if (prevX == curX + 1) direction = 2; // 向上
                        else if (prevY == curY - 1) direction = 3; // 向右
                        else if (prevY == curY + 1) direction = 4; // 向左
                        path.push_back({ prevX + 1, prevY + 1, direction }); // 记录路径，注意加1以符合输出格式
                        curX = prevX;
                        curY = prevY;
                    }

                    // 添加起点
                    path.push_back({ 1, 1, 0 }); // 添加起点，方向无意义（用0表示）

                    // 反转路径，以确保从起点到终点的顺序
                    reverse(path.begin(), path.end());

                    // 将终点添加到路径的最后
                    path.push_back({ rows, cols, -1 }); // 添加终点，方向设置为-1
                    return path; // 返回路径
                }
            }
        }
    }

    return {}; // 如果没有找到路径
}



void Greedyarray::arrayGreedy(const std::vector<std::vector<int>>& array, QWidget* parent) {
    vector<vector<int>> grid = array;
    // 调用BFS算法找到最短路径
    vector<tuple<int, int, int>> path = bfsShortestPath(grid);

    // 输出结果并将路径导出为二维数组
    QString result; // 拼接输出字符串
    vector<vector<int>> path_array;  // 用于存储导出的路径数组

    for (const auto& point : path) {
        int x = std::get<0>(point);
        int y = std::get<1>(point);
        int z = std::get<2>(point);

        // 将路径上的坐标添加到 path_array 中
        path_array.push_back({ x-1, y-1 });

        // 拼接成字符串格式，例如 "(x, y)"
        result += QString("(%1, %2, %3)\n").arg(x).arg(y).arg(z);

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
