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

// ���巽������Ͷ�Ӧ�ķ������
const int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
const int directionCodes[4] = { 2, 1, 4, 3 }; // ������룺��=2, ��=1, ��=4, ��=3

// BFS�㷨
vector<tuple<int, int, int>> bfsShortestPath(const vector<vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    // ��������յ��Ƿ���Ч
    if (grid[0][0] == 1 || grid[rows - 1][cols - 1] == 1) {
        return {}; // �����յ�Ϊ�ϰ����޷�����
    }

    // ����һ����������BFS
    queue<pair<int, int>> q;
    q.push({ 0, 0 }); // ����㿪ʼ
    vector<vector<int>> distance(rows, vector<int>(cols, -1)); // �������
    distance[0][0] = 0; // ���ľ�����Ϊ0��������㣩

    // ��¼·���ķ���
    vector<vector<pair<int, int>>> prev(rows, vector<pair<int, int>>(cols, { -1, -1 }));

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        // �����ĸ�����
        for (int dir = 0; dir < 4; ++dir) {
            int newX = x + directions[dir][0];
            int newY = y + directions[dir][1];

            // �����λ���Ƿ��ڱ߽�������ͨ��
            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && grid[newX][newY] == 0 && distance[newX][newY] == -1) {
                // ���¾���
                distance[newX][newY] = distance[x][y] + 1;
                q.push({ newX, newY }); // �������

                // ��¼·��
                prev[newX][newY] = { x, y }; // ��¼��ǰ���ǰ����

                // ��������յ㣬����·��������
                if (newX == rows - 1 && newY == cols - 1) {
                    vector<tuple<int, int, int>> path; // �洢·����Ԫ��
                    int curX = newX, curY = newY;

                    // ����·��
                    while (curX != 0 || curY != 0) {
                        auto [prevX, prevY] = prev[curX][curY];
                        int direction;
                        if (prevX == curX - 1) direction = 1; // ����
                        else if (prevX == curX + 1) direction = 2; // ����
                        else if (prevY == curY - 1) direction = 3; // ����
                        else if (prevY == curY + 1) direction = 4; // ����
                        path.push_back({ prevX + 1, prevY + 1, direction }); // ��¼·����ע���1�Է��������ʽ
                        curX = prevX;
                        curY = prevY;
                    }

                    // ������
                    path.push_back({ 1, 1, 0 }); // �����㣬���������壨��0��ʾ��

                    // ��ת·������ȷ������㵽�յ��˳��
                    reverse(path.begin(), path.end());

                    // ���յ���ӵ�·�������
                    path.push_back({ rows, cols, -1 }); // ����յ㣬��������Ϊ-1
                    return path; // ����·��
                }
            }
        }
    }

    return {}; // ���û���ҵ�·��
}



void Greedyarray::arrayGreedy(const std::vector<std::vector<int>>& array, QWidget* parent) {
    vector<vector<int>> grid = array;
    // ����BFS�㷨�ҵ����·��
    vector<tuple<int, int, int>> path = bfsShortestPath(grid);

    // ����������·������Ϊ��ά����
    QString result; // ƴ������ַ���
    vector<vector<int>> path_array;  // ���ڴ洢������·������

    for (const auto& point : path) {
        int x = std::get<0>(point);
        int y = std::get<1>(point);
        int z = std::get<2>(point);

        // ��·���ϵ�������ӵ� path_array ��
        path_array.push_back({ x-1, y-1 });

        // ƴ�ӳ��ַ�����ʽ������ "(x, y)"
        result += QString("(%1, %2, %3)\n").arg(x).arg(y).arg(z);

        // �ڿ���̨���
        //std::cout << "(" << x << ", " << y << ")" << std::endl;
    }

    if (!result.isEmpty()) {
        // ��GUI��������ʾ·��
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
