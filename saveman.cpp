#include "saveman.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <QMessageBox>
#include <QPushButton>

using namespace std;

// ���巽�����
const int EAST = 0, SOUTH = 1, WEST = 2, NORTH = 3;
// �����ĸ�������ƶ���ʽ�������ϡ�������
const int dx[] = { 0, 1, 0, -1 };  // ��Ӧ�е��ƶ������䡢���¡�����
const int dy[] = { 1, 0, -1, 0 };  // ��Ӧ�е��ƶ������ҡ����¡�����

// ���ڴ洢���п��ܵ�·��
vector<vector<tuple<int, int, int>>> all_paths;
vector<tuple<int, int, int>> best_path;

// ���������������
void dfs(vector<vector<int>>& grid, vector<tuple<int, int, int>>& path, int row, int col, int end_row, int end_col, QWidget* parent) {
    // �����ǰ����Խ��������ϰ���ֱ�ӷ���
    if (row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size() || grid[row][col] == 1) {
        return;
    }

    // �������Ŀ��λ�ã�����ǰ·����ӵ�����·��������
    if (row == end_row && col == end_col) {
        all_paths.push_back(path);
        if (best_path.empty() || path.size() < best_path.size()) {
            best_path = path; // ��������·��
        }
        return;
    }

    // ��ǵ�ǰ�����ѷ���
    grid[row][col] = 1;

    // �����ĸ����򣺶����ϡ�������
    for (int direction = 0; direction < 4; ++direction) {
        int next_row = row + dx[direction];
        int next_col = col + dy[direction];
        path.emplace_back(row, col, direction); // ��ӵ�ǰλ�úͷ���·��
        dfs(grid, path, next_row, next_col, end_row, end_col, parent);
        path.pop_back(); // ����
    }

    // ȡ�����
    grid[row][col] = 0;
}


void savearray::arraysave(const std::vector<std::vector<int>>& array, QWidget* parent) {
    vector<vector<int>> grid = array;

    int start_row = 0, start_col = 0;
    int end_row = 9, end_col = 9;

    vector<tuple<int, int, int>> path;
    dfs(grid, path, start_row, start_col, end_row, end_col, parent);

    // ����������·������Ϊ��ά����
    QString result; // ƴ������ַ���
    vector<vector<int>> path_array;  // ���ڴ洢������·������

    for (const auto& point : best_path) {
        int x = std::get<0>(point);
        int y = std::get<1>(point);
        int z = std::get<2>(point);

        // ��·���ϵ�������ӵ� path_array ��
        path_array.push_back({ x, y });

        // ƴ�ӳ��ַ�����ʽ������ "(x, y)"
        result += QString("(%1, %2, %3)\n").arg(x+1).arg(y+1).arg(z);

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