#ifndef ARRAYPR0CESSOR_H
#define ARRAYPROCESSOR_H
#include <vector>
#include <QMessageBox>

class Dijksarray {
public:
    static void arrayDijks(const std::vector<std::vector<int>>& array, QWidget* parent);
};

#endif // ARRAYPROCESSOR_H