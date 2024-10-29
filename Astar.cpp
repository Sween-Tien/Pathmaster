#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>
#include <cmath>
#include "Astar.h"
#include <QMessageBox>
#include <QPushButton>


using namespace std;

// 定义方向编码和移动方式
const int dx[] = { 0, 1, 0, -1 };  // 行移动：右、下、左、上
const int dy[] = { 1, 0, -1, 0 };  // 列移动：右、下、左、上

// 位置结构体，用于存储迷宫中的坐标
struct Position {
    int row;
    int col;
};

// 节点结构体，用于A*算法
struct Node {
    int row, col, direction;
    double g; // 从起点到当前位置的实际代价
    double f; // 总代价：f = g + h
    vector<Position> path; // 存储路径

    // 构造函数
    Node(int r, int c, int dir, double g_cost, double f_cost, vector<Position> p)
        : row(r), col(c), direction(dir), g(g_cost), f(f_cost), path(move(p)) {}

    bool operator>(const Node& other) const {
        return f > other.f; // 优先队列中按f值升序排列（小f值优先）
    }
};

// 计算启发式距离（曼哈顿距离）
double heuristic(int row, int col, int target_row, int target_col) {
    return abs(row - target_row) + abs(col - target_col);
}

// A*算法寻找最平滑路径
vector<Position> aStarSmoothPath(const vector<vector<int>>& grid, int start_row, int start_col, int target_row, int target_col, double turn_penalty) {
    priority_queue<Node, vector<Node>, greater<Node>> open_list;
    vector<vector<double>> g_cost(grid.size(), vector<double>(grid[0].size(), numeric_limits<double>::max()));
    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));

    open_list.push(Node(start_row, start_col, -1, 0, 0, { {start_row, start_col} }));
    g_cost[start_row][start_col] = 0;

    while (!open_list.empty()) {
        Node current = open_list.top();
        open_list.pop();

        if (visited[current.row][current.col]) {
            continue;
        }

        visited[current.row][current.col] = true;

        // 如果到达目标位置，返回路径
        if (current.row == target_row && current.col == target_col) {
            return current.path;
        }

        // 遍历四个方向：右、下、左、上
        for (int direction = 0; direction < 4; ++direction) {
            int next_row = current.row + dx[direction];
            int next_col = current.col + dy[direction];

            // 检查边界条件和障碍物
            if (next_row >= 0 && next_row < grid.size() && next_col >= 0 && next_col < grid[0].size() && grid[next_row][next_col] == 0) {
                double new_g = current.g + 1;

                // 如果方向改变，增加惩罚值
                if (current.direction != -1 && current.direction != direction) {
                    new_g += turn_penalty;
                }

                if (new_g < g_cost[next_row][next_col]) {
                    g_cost[next_row][next_col] = new_g;
                    double h = heuristic(next_row, next_col, target_row, target_col);
                    double new_f = new_g + h;
                    vector<Position> new_path = current.path;
                    new_path.push_back({ next_row, next_col });
                    open_list.push(Node(next_row, next_col, direction, new_g, new_f, new_path));
                }
            }
        }
    }

    return {}; // 如果没有找到通路，返回空路径
}

// 对找到的路径进行平滑处理
vector<Position> smoothPath(const vector<Position>& path) {
    vector<Position> smooth_path = path;

    for (size_t i = 1; i < smooth_path.size() - 1; ++i) {
        Position prev = smooth_path[i - 1];
        Position current = smooth_path[i];
        Position next = smooth_path[i + 1];

        // 检查是否形成拐角，如果是，则尝试平滑处理
        if ((prev.row != current.row && prev.col != current.col) &&
            (next.row != current.row && next.col != current.col)) {
            // 使用插值的方式平滑路径
            smooth_path[i] = { prev.row, current.col };
        }
    }

    return smooth_path;
}


void Astararray::arrayAstar(const std::vector<std::vector<int>>& array, QWidget* parent)
{
    vector<vector<int>> grid = array;

    int start_row = 0, start_col = 0;
    int target_row = 9, target_col = 9;
    double turn_penalty = 2.0;

    vector<Position> best_path = aStarSmoothPath(grid, start_row, start_col, target_row, target_col, turn_penalty);
    vector<Position> smooth_path = smoothPath(best_path);

    QString result; // 拼接输出字符串
    vector<vector<int>> path_array;  // 用于存储导出的路径数组

    for (const auto& point : smooth_path) {
        int x = point.row;
        int y = point.col;
   


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