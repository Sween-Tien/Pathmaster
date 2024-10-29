#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include "ui_Pathmaster.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Pathmaster; }
QT_END_NAMESPACE

class Pathmaster : public QMainWindow
{
    Q_OBJECT

public:
    Pathmaster(QWidget *parent = nullptr);
    ~Pathmaster();

public slots:
    void handleButtonClick();
    void returnMatrix(); 
    void savepeople();
    void reversebutton();
    void Dijksalgo();
    void Astaralgo();
    void Greedyalgo();

private:
    Ui::PathmasterClass ui;
    std::vector<std::vector<int>> matrix;
};
