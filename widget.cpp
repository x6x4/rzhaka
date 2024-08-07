#include "widget.hpp"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

FieldGui::FieldGui (Field* field, QWidget *main) :
    m_grid(QGridLayout()), QWidget(main) {

    m_grid.setSpacing(0);
    QVector<QLabel*> labels;

    int cur_cell_idx = 0;

    for (const auto &entry : field->get_cells()) {

            labels.push_back(new QLabel());
            QFont font("Lucida Console", 30);
            CellType cur_cell_type;

            switch (entry.second.m_type) {
                case CellType::BOX:
                    labels[cur_cell_idx]->setStyleSheet(QString("background-color: %1;").arg("orange"));
                    labels[cur_cell_idx]->setText(QString("%1").arg(entry.second.m_wval));
                    labels[cur_cell_idx]->setAlignment(Qt::AlignCenter);
                    labels[cur_cell_idx]->setFont(font);
                    break;
                case CellType::EXIT:
                    labels[cur_cell_idx]->setStyleSheet(QString("background-color: %1;").arg("green"));
                    break;
                case CellType::EMPTY:
                    labels[cur_cell_idx]->setStyleSheet(QString("background-color: %1;").arg("yellow"));
                    break;
                case CellType::WALL:
                    labels[cur_cell_idx]->setStyleSheet(QString("background-color: %1;").arg("blue"));
                    break;
                default:
                    break;
            }

            m_grid.addWidget(labels[cur_cell_idx], entry.first.first, entry.first.second);

            cur_cell_idx++;
    }

    prev = field->get_cur();

    main->setLayout(&m_grid);
    connect(this, &FieldGui::changed, this, &FieldGui::update);
    update(field->get_orient(), field->get_cur());
}

