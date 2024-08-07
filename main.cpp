#include "widget.hpp"
#include <QDebug>
#include <QApplication>
#include <string>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget main;

    Field init_field("../../backend/hexagons.labhex");
    FieldGui gui(&init_field, &main);

    gui.wait();

    main.resize(600, 600);
    main.show();

    return app.exec();
}

