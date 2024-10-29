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

// ���巽�������ƶ���ʽ
const int dx[] = { 0, 1, 0, -1 };  // ���ƶ����ҡ��¡�����
const int dy[] = { 1, 0, -1, 0 };  // ���ƶ����ҡ��¡�����

// λ�ýṹ�壬���ڴ洢�Թ��е�����
struct Position {
    int row;
    int col;
};

// �ڵ�ṹ�壬����A*�㷨
struct Node {
    int row, col, direction;
    double g; // ����㵽��ǰλ�õ�ʵ�ʴ���
    double f; // �ܴ��ۣ�f = g + h
    vector<Position> path; // �洢·��

    // ���캯��
    Node(int r, int c, int dir, double g_cost, double f_cost, vector<Position> p)
        : row(r), col(c), direction(dir), g(g_cost), f(f_cost), path(move(p)) {}

    bool operator>(const Node& other) const {
        return f > other.f; // ���ȶ����а�fֵ�������У�Сfֵ���ȣ�
    }
};

// ��������ʽ���루�����پ��룩
double heuristic(int row, int col, int target_row, int target_col) {
    return abs(row - target_row) + abs(col - target_col);
}

// A*�㷨Ѱ����ƽ��·��
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

        // �������Ŀ��λ�ã�����·��
        if (current.row == target_row && current.col == target_col) {
            return current.path;
        }

        // �����ĸ������ҡ��¡�����
        for (int direction = 0; direction < 4; ++direction) {
            int next_row = current.row + dx[direction];
            int next_col = current.col + dy[direction];

            // ���߽��������ϰ���
            if (next_row >= 0 && next_row < grid.size() && next_col >= 0 && next_col < grid[0].size() && grid[next_row][next_col] == 0) {
                double new_g = current.g + 1;

                // �������ı䣬���ӳͷ�ֵ
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

    return {}; // ���û���ҵ�ͨ·�����ؿ�·��
}

// ���ҵ���·������ƽ������
vector<Position> smoothPath(const vector<Position>& path) {
    vector<Position> smooth_path = path;

    for (size_t i = 1; i < smooth_path.size() - 1; ++i) {
        Position prev = smooth_path[i - 1];
        Position current = smooth_path[i];
        Position next = smooth_path[i + 1];

        // ����Ƿ��γɹսǣ�����ǣ�����ƽ������
        if ((prev.row != current.row && prev.col != current.col) &&
            (next.row != current.row && next.col != current.col)) {
            // ʹ�ò�ֵ�ķ�ʽƽ��·��
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

    QString result; // ƴ������ַ���
    vector<vector<int>> path_array;  // ���ڴ洢������·������

    for (const auto& point : smooth_path) {
        int x = point.row;
        int y = point.col;
   


        // ��·���ϵ�������ӵ� path_array ��
        path_array.push_back({ x, y });

        // ƴ�ӳ��ַ�����ʽ������ "(x, y)"
        result += QString("(%1, %2)\n").arg(x + 1).arg(y + 1);

        // �ڿ���̨���
        //std::cout << "(" << x << ", " << y << ")" << std::endl;
    }

    // ��ӡ����·������ (row, line, direction)
    if (!best_path.empty()) {
        QMessageBox::information(parent, "Best Path", result);
    }
    else {
        QMessageBox::information(parent, "Best Path", "NO PATH FOUND!");
    }
    path_array.push_back({ 9, 9 });
    for (const auto& coords : path_array) {
        std::cout << "[" << coords[0] << ", " << coords[1] << "]" << std::endl;

        // ������ť�Ķ����������� P00, P12
        QString buttonName = QString("P%1%2").arg(coords[0]).arg(coords[1]);

        // ʹ�� findChild() ���Ҷ�Ӧ�İ�ť
        QPushButton* button = parent->findChild<QPushButton*>(buttonName);
        if (button) {
            // ����ť��������Ϊ��ɫ
            button->setStyleSheet("background-color: #536bb6;");
        }
    }
    return;
}