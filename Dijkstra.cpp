#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>
#include "Dijkstra.h"
#include <QMessageBox>
#include <QPushButton>

using namespace std;

/// ���巽�������ƶ���ʽ
const int dx[] = { 0, 1, 0, -1 };  // ���ƶ��������ϡ�������
const int dy[] = { 1, 0, -1, 0 };  // ���ƶ��������ϡ�������

// BFS�㷨Ѱ�����·��
vector<tuple<int, int>> bfsSearch(const vector<vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    // ��������յ��Ƿ�ɴ�
    if (grid[0][0] == 1 || grid[rows - 1][cols - 1] == 1) {
        return {}; // �����յ㱻�ϰ����赲
    }

    queue<tuple<int, int, vector<tuple<int, int>>>> q; // �洢�����·��
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    // ���
    q.push({ 0, 0, {{0, 0}} });
    visited[0][0] = true;

    while (!q.empty()) {
        auto [row, col, path] = q.front();
        q.pop();

        // �������Ŀ��λ�ã�����·��
        if (row == rows - 1 && col == cols - 1) {
            return path; // ����·��
        }

        // �����ĸ����򣺶����ϡ�������
        for (int direction = 0; direction < 4; ++direction) {
            int next_row = row + dx[direction];
            int next_col = col + dy[direction];

            // �����λ���Ƿ�������Χ����δ�����ʣ����Ҳ����ϰ���
            if (next_row >= 0 && next_row < rows && next_col >= 0 && next_col < cols &&
                grid[next_row][next_col] == 0 && !visited[next_row][next_col]) {

                visited[next_row][next_col] = true; // ���Ϊ�ѷ���

                // ����·�������
                auto new_path = path;
                new_path.emplace_back(next_row, next_col); // ����½ڵ㵽·����
                q.push({ next_row, next_col, new_path });
            }
        }
    }

    return {}; // ���û���ҵ�ͨ·�����ؿ�·��
}

void Dijksarray::arrayDijks(const std::vector<std::vector<int>>& array, QWidget* parent) {
    // ʹ����Ŀ���ṩ��9��8����
    vector<vector<int>> grid = array;

    vector<tuple<int, int>> best_path = bfsSearch(grid);

    QString result; // ƴ������ַ���
    vector<vector<int>> path_array;  // ���ڴ洢������·������

    for (const auto& point : best_path) {
        int x = std::get<0>(point);
        int y = std::get<1>(point);

       
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
