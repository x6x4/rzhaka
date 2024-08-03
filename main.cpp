#include "qgridlayout.h"
#include "qlabel.h"
#include "qnamespace.h"
#include "qpair.h"
#include "qtablewidget.h"
#include "widget.h"
#include <QtWidgets>
#include <cstddef>
#include <utility>

class Field {

    QGridLayout& m_grid;
    QPair<int, int> cur_cell = QPair(2,1);

public:
    Field (QGridLayout& grid) : m_grid(grid) {}

    void move_f() {

        cur_cell.first -= 2;
        auto x = m_grid.itemAtPosition(cur_cell.first, cur_cell.second)->widget();

        ((QLabel*) x)->setText("ROBOT");
        ((QLabel*) x)->setAlignment(Qt::AlignCenter);
    }
};




int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget main;

    QGridLayout qgl;
    qgl.setSpacing(0);
    Field field(qgl);

    QVector<QLabel*> qlbls;
    for (int i = 0; i < 7; i++) {
        qlbls.push_back(new QLabel());
    }

    qlbls[0]->setStyleSheet(QString("background-color: %1;").arg("green"));
    qgl.addWidget(qlbls[0], 0, 1);

    qlbls[1]->setStyleSheet(QString("background-color: %1;").arg("blue"));
    qgl.addWidget(qlbls[1], 1, 0);

    qlbls[2]->setStyleSheet(QString("background-color: %1;").arg("yellow"));
    qgl.addWidget(qlbls[2], 1, 2);

    qlbls[3]->setStyleSheet(QString("background-color: %1;").arg("orange"));
    qgl.addWidget(qlbls[3], 3, 0);

    qlbls[4]->setStyleSheet(QString("background-color: %1;").arg("blue"));
    qgl.addWidget(qlbls[4], 3, 2);

    qlbls[5]->setStyleSheet(QString("background-color: %1;").arg("yellow"));
    qgl.addWidget(qlbls[5], 4, 1);

    qlbls[6]->setStyleSheet(QString("background-color: %1;").arg("yellow"));
    qgl.addWidget(qlbls[6], 2, 1);

    main.setLayout(&qgl);

    main.resize(600, 600);
    field.move_f();

    main.show();

    return app.exec();
}

