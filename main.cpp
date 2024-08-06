#include "qgridlayout.h"
#include "qlabel.h"
#include "qnamespace.h"
#include "qpair.h"
#include "qtablewidget.h"
#include "widget.h"
#include <QtWidgets>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include <QDebug>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget main;

    Field init_field("/home/cracky/rzhaka/backend/hexagons.labhex");
    FieldGui gui(&init_field, &main);

    gui.wait();

    main.resize(600, 600);
    main.show();

    return app.exec();
}

