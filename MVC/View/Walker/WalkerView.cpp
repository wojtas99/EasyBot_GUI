#include "WalkerView.h"

#include <iostream>
#include <QButtonGroup>


WalkerView::WalkerView(QWidget *parent)
    : ui(new Ui::Walker_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    auto direction_buttonGroup = new QButtonGroup();
    direction_buttonGroup->addButton(ui->n_radioButton, 0);
    direction_buttonGroup->addButton(ui->e_radioButton, 1);
    direction_buttonGroup->addButton(ui->s_radioButton, 2);
    direction_buttonGroup->addButton(ui->w_radioButton, 3);
    direction_buttonGroup->addButton(ui->ne_radioButton, 4);
    direction_buttonGroup->addButton(ui->se_radioButton, 5);
    direction_buttonGroup->addButton(ui->sw_radioButton, 6);
    direction_buttonGroup->addButton(ui->nw_radioButton, 7);
    direction_buttonGroup->addButton(ui->c_radioButton, 8);

    auto option_buttonGroup = new QButtonGroup();
    option_buttonGroup->addButton(ui->stand_radioButton);
    option_buttonGroup->addButton(ui->node_radioButton );
    option_buttonGroup->addButton(ui->lure_radioButton);
    option_buttonGroup->addButton(ui->label_radioButton);
    option_buttonGroup->addButton(ui->action_radioButton);

    connect(ui->add_pushButton, &QPushButton::clicked, this, [this, direction_buttonGroup, option_buttonGroup]() {
        auto direction = direction_buttonGroup->checkedButton()->text();
        auto option = option_buttonGroup->checkedButton()->text();
        auto action = ui->action_textEdit->toPlainText().trimmed();
        emit addItem_signal(direction, option, action);
        ui->action_textEdit->clear();
    });
    connect(ui->record_checkBox, &QCheckBox::toggled, this, [this, direction_buttonGroup, option_buttonGroup]() {
        auto state = ui->record_checkBox->isChecked();
        auto sqmDist = ui->record_lineEdit->text().toInt();
        auto direction = direction_buttonGroup->checkedButton()->text();
        auto option = option_buttonGroup->checkedButton()->text();
        emit recordWaypoints_signal(state, sqmDist, direction, option);
    });

    connect(ui->waypoints_listWidget, &QListWidget::itemDoubleClicked, this, [this](){
        auto currentIndex = ui->waypoints_listWidget->currentRow();
        ui->waypoints_listWidget->takeItem(currentIndex);
        emit deleteItem_signal(currentIndex);
    });
}

WalkerView::~WalkerView() {
    delete ui;
}

void WalkerView::addItem(const QString &item)
{
    ui->waypoints_listWidget->addItem(item);
}

void WalkerView::indexUpdate(int index) {
    ui->waypoints_listWidget->setCurrentRow(index);
    ui->waypoints_listWidget->setFocus();
}

void WalkerView::clearListWidget() {
    ui->waypoints_listWidget->clear();
}
