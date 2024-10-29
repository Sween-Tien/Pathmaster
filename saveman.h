#ifndef ARRAYPROCESSOR_H
#define ARRAYPROCESSOR_H
#include <vector>
#include <QMessageBox>

class savearray {
public:
    static void arraysave(const std::vector<std::vector<int>>& array, QWidget* parent);
};

#endif // ARRAYPROCESSOR_H