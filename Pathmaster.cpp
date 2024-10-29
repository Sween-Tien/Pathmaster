#include "Pathmaster.h"
#include "ui_Pathmaster.h"
#include <QMessageBox>
#include <QDebug>
#include "saveman.h"
#include "Dijkstra.h"
#include "Astar.h"
#include "Greedy.h"

Pathmaster::Pathmaster(QWidget *parent)
    : QMainWindow(parent)
    ,ui()
{
    ui.setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowTitleHint); // 无边框
    setAttribute(Qt::WA_TranslucentBackground); // 设置窗口为透明背景

    // 初始化按钮状态矩阵，全为 0（未点击）
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            matrix.resize(10, std::vector<int>(10, 0)); // 初始化矩阵值为 0

            // 获取按钮对象名，例如 P00, P01, ..., P99
            QString buttonName = QString("P%1%2").arg(i).arg(j);
            QPushButton* button = this->findChild<QPushButton*>(buttonName);

            if (i == 0 && j == 0 || i == 9 && j == 9) {
                button->setStyleSheet("background-color: #082E54;");
            }

            // 连接按钮的 clicked() 信号到自定义槽 handleButtonClick()
            connect(button, &QPushButton::clicked, this, &Pathmaster::handleButtonClick);
        }
    }


    // 连接确认按钮的 clicked() 信号到 returnMatrix 槽
    connect(ui.confirm, &QPushButton::clicked, this, &Pathmaster::returnMatrix);
    connect(ui.save, &QPushButton::clicked, this, &Pathmaster::savepeople);
    connect(ui.reverse, &QPushButton::clicked, this, &Pathmaster::reversebutton);
    connect(ui.deep, &QPushButton::clicked, this, &Pathmaster::Dijksalgo);
    connect(ui.shortest, &QPushButton::clicked, this, &Pathmaster::Astaralgo);
    connect(ui.greedy, &QPushButton::clicked, this, &Pathmaster::Greedyalgo);
}

Pathmaster::~Pathmaster()
{}

void Pathmaster::handleButtonClick()
{
    // 获取发送信号的按钮
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        // 从按钮的 objectName 提取行列索引
        QString buttonName = button->objectName();
        int row = buttonName.mid(1, 1).toInt();
        int col = buttonName.mid(2, 1).toInt();

        // 更新矩阵状态为 1（表示已点击）
        matrix[row][col] = 1;

        // 设置按钮为黄色表示已点击
        button->setStyleSheet("background-color: #ccccb8;"); // 将按钮背景色设置为黄色
        button->setEnabled(false);  // 禁用已点击的按钮
    }
}

// 处理确认按钮点击事件，返回矩阵
void Pathmaster::returnMatrix()
{
    QString result;
    for (const auto& row : matrix) {
        for (const auto& val : row) {
            result += QString::number(val) + " ";
        }
        result += "\n";
    }

    // 弹出消息框显示结果
    QMessageBox::information(this, "Matrix", result);
}

void Pathmaster::reversebutton()
{
    // 遍历所有子控件
    const QList<QPushButton*> buttons = findChildren<QPushButton*>();

    // 遍历所有按钮并恢复为白色
    for (QPushButton* button : buttons) {
        if (button->styleSheet() == "background-color: #536bb6;" || button->styleSheet() == "background-color: #ccccb8;") {
            button->setStyleSheet("background-color: white;");  // 恢复为白色

            // 从按钮的 objectName 提取行列索引
            QString buttonName = button->objectName();
            int row = buttonName.mid(1, 1).toInt();
            int col = buttonName.mid(2, 1).toInt();

            // 将相应的矩阵状态设置为 0（未点击）
            matrix[row][col] = 0;
            button->setEnabled(true); // 启用按钮
        }
        if (button->objectName() == "P00" || button->objectName() == "P99")
        {
            button->setStyleSheet("background-color:#082E54;");
        }
    }
    // 确保矩阵在此处重置为全零矩阵，已在上述循环中处理
    // matrix.assign(10, std::vector<int>(10, 0)); // 重置矩阵为 10x10 的全零矩阵
}

//广度优先搜索
void Pathmaster::savepeople()
{
    // 遍历所有子控件
    const QList<QPushButton*> buttons = findChildren<QPushButton*>();

    // 遍历所有按钮并恢复为白色
    for (QPushButton* button : buttons) {
        if (button->styleSheet() == "background-color: #536bb6;") {
            button->setStyleSheet("background-color: white;");  // 恢复为白色

            // 从按钮的 objectName 提取行列索引
            QString buttonName = button->objectName();
            int row = buttonName.mid(1, 1).toInt();
            int col = buttonName.mid(2, 1).toInt();

            // 将相应的矩阵状态设置为 0（未点击）
            matrix[row][col] = 0;
            button->setEnabled(true); // 启用按钮
        }
        if (button->objectName() == "P00" || button->objectName() == "P99")
        {
            button->setStyleSheet("background-color:#082E54;");
        }
    }
    savearray::arraysave(matrix, this);
}

void Pathmaster::Dijksalgo()
{
    // 遍历所有子控件
    const QList<QPushButton*> buttons = findChildren<QPushButton*>();

    // 遍历所有按钮并恢复为白色
    for (QPushButton* button : buttons) {
        if (button->styleSheet() == "background-color: #536bb6;") {
            button->setStyleSheet("background-color: white;");  // 恢复为白色

            // 从按钮的 objectName 提取行列索引
            QString buttonName = button->objectName();
            int row = buttonName.mid(1, 1).toInt();
            int col = buttonName.mid(2, 1).toInt();

            // 将相应的矩阵状态设置为 0（未点击）
            matrix[row][col] = 0;
            button->setEnabled(true); // 启用按钮
        }
        if (button->objectName() == "P00" || button->objectName() == "P99")
        {
            button->setStyleSheet("background-color:#082E54;");
        }
    }
    Dijksarray::arrayDijks(matrix, this);
}

void Pathmaster::Astaralgo()
{
    // 遍历所有子控件
    const QList<QPushButton*> buttons = findChildren<QPushButton*>();

    // 遍历所有按钮并恢复为白色
    for (QPushButton* button : buttons) {
        if (button->styleSheet() == "background-color: #536bb6;") {
            button->setStyleSheet("background-color: white;");  // 恢复为白色

            // 从按钮的 objectName 提取行列索引
            QString buttonName = button->objectName();
            int row = buttonName.mid(1, 1).toInt();
            int col = buttonName.mid(2, 1).toInt();

            // 将相应的矩阵状态设置为 0（未点击）
            matrix[row][col] = 0;
            button->setEnabled(true); // 启用按钮
        }
        if (button->objectName() == "P00" || button->objectName() == "P99")
        {
            button->setStyleSheet("background-color:#082E54;");
        }
    }
    Astararray::arrayAstar(matrix, this);
}

void Pathmaster::Greedyalgo()
{
    // 遍历所有子控件
    const QList<QPushButton*> buttons = findChildren<QPushButton*>();

    // 遍历所有按钮并恢复为白色
    for (QPushButton* button : buttons) {
        if (button->styleSheet() == "background-color: #536bb6;") {
            button->setStyleSheet("background-color: white;");  // 恢复为白色

            // 从按钮的 objectName 提取行列索引
            QString buttonName = button->objectName();
            int row = buttonName.mid(1, 1).toInt();
            int col = buttonName.mid(2, 1).toInt();

            // 将相应的矩阵状态设置为 0（未点击）
            matrix[row][col] = 0;
            button->setEnabled(true); // 启用按钮
        }
        if (button->objectName() == "P00" || button->objectName() == "P99")
        {
            button->setStyleSheet("background-color:#082E54;");
        }
    }
    Greedyarray::arrayGreedy(matrix, this);
}