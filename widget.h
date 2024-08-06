#ifndef WIDGET_H
#define WIDGET_H

#include "qcolor.h"
#include "qdebug.h"
#include "qevent.h"
#include "qlabel.h"
#include "qlayout.h"
#include "qmap.h"
#include "qmath.h"
#include "qnamespace.h"
#include "qpalette.h"
#include <QWidget>
#include <QPainter>

#include "/home/cracky/rzhaka/backend/runtime/runtime.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
};



class FieldGui : public QWidget {
    Q_OBJECT

    QGridLayout m_grid;
    Field *m_field;

public:

    FieldGui (Field* field, QWidget *main);
    void wait() {

    }

signals:
    void changed();

private slots:
    void update() {
        auto Old = m_grid.itemAtPosition(m_field->get_prev().first, m_field->get_prev().second)->widget();

        switch (m_field->get_orient()) {
            case Orient::NORTH:
                ((QLabel*) Old)->setText("^");
                break;
            case Orient::NORTH_EAST:
                ((QLabel*) Old)->setText("/");
                break;
            case Orient::SOUTH_EAST:
                ((QLabel*) Old)->setText("v>");
                break;
            case Orient::SOUTH:
                ((QLabel*) Old)->setText("v");
                break;
            case Orient::SOUTH_WEST:
                ((QLabel*) Old)->setText("<v");
                break;
            case Orient::NORTH_WEST:
                ((QLabel*) Old)->setText("\\");
                break;
        }

        ((QLabel*) Old)->setAlignment(Qt::AlignCenter);

        auto New = m_grid.itemAtPosition(m_field->get_cur().first, m_field->get_cur().second)->widget();

        ((QLabel*) New)->setText("ROBOT");
        ((QLabel*) New)->setAlignment(Qt::AlignCenter);
    }
};





#endif // WIDGET_H
