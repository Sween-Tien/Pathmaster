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

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowTitleHint); // �ޱ߿�
    setAttribute(Qt::WA_TranslucentBackground); // ���ô���Ϊ͸������

    // ��ʼ����ť״̬����ȫΪ 0��δ�����
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            matrix.resize(10, std::vector<int>(10, 0)); // ��ʼ������ֵΪ 0

            // ��ȡ��ť������������ P00, P01, ..., P99
            QString buttonName = QString("P%1%2").arg(i).arg(j);
            QPushButton* button = this->findChild<QPushButton*>(buttonName);

            if (i == 0 && j == 0 || i == 9 && j == 9) {
                button->setStyleSheet("background-color: #082E54;");
            }

            // ���Ӱ�ť�� clicked() �źŵ��Զ���� handleButtonClick()
            connect(button, &QPushButton::clicked, this, &Pathmaster::handleButtonClick);
        }
    }


    // ����ȷ�ϰ�ť�� clicked() �źŵ� returnMatrix ��
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
    // ��ȡ�����źŵİ�ť
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        // �Ӱ�ť�� objectName ��ȡ��������
        QString buttonName = button->objectName();
        int row = buttonName.mid(1, 1).toInt();
        int col = buttonName.mid(2, 1).toInt();

        // ���¾���״̬Ϊ 1����ʾ�ѵ����
        matrix[row][col] = 1;

        // ���ð�ťΪ��ɫ��ʾ�ѵ��
        button->setStyleSheet("background-color: #ccccb8;"); // ����ť����ɫ����Ϊ��ɫ
        button->setEnabled(false);  // �����ѵ���İ�ť
    }
}

// ����ȷ�ϰ�ť����¼������ؾ���
void Pathmaster::returnMatrix()
{
    QString result;
    for (const auto& row : matrix) {
        for (const auto& val : row) {
            result += QString::number(val) + " ";
        }
        result += "\n";
    }

    // ������Ϣ����ʾ���
    QMessageBox::information(this, "Matrix", result);
}

void Pathmaster::reversebutton()
{
    // ���������ӿؼ�
    const QList<QPushButton*> buttons = findChildren<QPushButton*>();

    // �������а�ť���ָ�Ϊ��ɫ
    for (QPushButton* button : buttons) {
        if (button->styleSheet() == "background-color: #536bb6;" || button->styleSheet() == "background-color: #ccccb8;") {
            button->setStyleSheet("background-color: white;");  // �ָ�Ϊ��ɫ

            // �Ӱ�ť�� objectName ��ȡ��������
            QString buttonName = button->objectName();
            int row = buttonName.mid(1, 1).toInt();
            int col = buttonName.mid(2, 1).toInt();

            // ����Ӧ�ľ���״̬����Ϊ 0��δ�����
            matrix[row][col] = 0;
            button->setEnabled(true); // ���ð�ť
        }
        if (button->objectName() == "P00" || button->objectName() == "P99")
        {
            button->setStyleSheet("background-color:#082E54;");
        }
    }
    // ȷ�������ڴ˴�����Ϊȫ�������������ѭ���д���
    // matrix.assign(10, std::vector<int>(10, 0)); // ���þ���Ϊ 10x10 ��ȫ�����
}

//�����������
void Pathmaster::savepeople()
{
    // ���������ӿؼ�
    const QList<QPushButton*> buttons = findChildren<QPushButton*>();

    // �������а�ť���ָ�Ϊ��ɫ
    for (QPushButton* button : buttons) {
        if (button->styleSheet() == "background-color: #536bb6;") {
            button->setStyleSheet("background-color: white;");  // �ָ�Ϊ��ɫ

            // �Ӱ�ť�� objectName ��ȡ��������
            QString buttonName = button->objectName();
            int row = buttonName.mid(1, 1).toInt();
            int col = buttonName.mid(2, 1).toInt();

            // ����Ӧ�ľ���״̬����Ϊ 0��δ�����
            matrix[row][col] = 0;
            button->setEnabled(true); // ���ð�ť
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
    // ���������ӿؼ�
    const QList<QPushButton*> buttons = findChildren<QPushButton*>();

    // �������а�ť���ָ�Ϊ��ɫ
    for (QPushButton* button : buttons) {
        if (button->styleSheet() == "background-color: #536bb6;") {
            button->setStyleSheet("background-color: white;");  // �ָ�Ϊ��ɫ

            // �Ӱ�ť�� objectName ��ȡ��������
            QString buttonName = button->objectName();
            int row = buttonName.mid(1, 1).toInt();
            int col = buttonName.mid(2, 1).toInt();

            // ����Ӧ�ľ���״̬����Ϊ 0��δ�����
            matrix[row][col] = 0;
            button->setEnabled(true); // ���ð�ť
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
    // ���������ӿؼ�
    const QList<QPushButton*> buttons = findChildren<QPushButton*>();

    // �������а�ť���ָ�Ϊ��ɫ
    for (QPushButton* button : buttons) {
        if (button->styleSheet() == "background-color: #536bb6;") {
            button->setStyleSheet("background-color: white;");  // �ָ�Ϊ��ɫ

            // �Ӱ�ť�� objectName ��ȡ��������
            QString buttonName = button->objectName();
            int row = buttonName.mid(1, 1).toInt();
            int col = buttonName.mid(2, 1).toInt();

            // ����Ӧ�ľ���״̬����Ϊ 0��δ�����
            matrix[row][col] = 0;
            button->setEnabled(true); // ���ð�ť
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
    // ���������ӿؼ�
    const QList<QPushButton*> buttons = findChildren<QPushButton*>();

    // �������а�ť���ָ�Ϊ��ɫ
    for (QPushButton* button : buttons) {
        if (button->styleSheet() == "background-color: #536bb6;") {
            button->setStyleSheet("background-color: white;");  // �ָ�Ϊ��ɫ

            // �Ӱ�ť�� objectName ��ȡ��������
            QString buttonName = button->objectName();
            int row = buttonName.mid(1, 1).toInt();
            int col = buttonName.mid(2, 1).toInt();

            // ����Ӧ�ľ���״̬����Ϊ 0��δ�����
            matrix[row][col] = 0;
            button->setEnabled(true); // ���ð�ť
        }
        if (button->objectName() == "P00" || button->objectName() == "P99")
        {
            button->setStyleSheet("background-color:#082E54;");
        }
    }
    Greedyarray::arrayGreedy(matrix, this);
}