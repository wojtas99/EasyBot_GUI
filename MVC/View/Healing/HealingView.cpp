//
// Created by blazz on 12.10.2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_HealingView.h" resolved

#include "HealingView.h"

#include <iostream>
#include <__msvc_ostream.hpp>


HealingView::HealingView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Healing_View) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    connect(ui->add_pushButton, &QPushButton::clicked, this, [this]() {
        auto action = ui->action_comboBox->currentText();
        auto heal = ui->heal_lineEdit->text();
        auto condition = ui->condition_comboBox->currentText();
        auto random = ui->random_lineEdit->text().toInt();
        auto below = ui->belowHp_lineEdit->text().toInt();
        auto above = ui->aboveHp_lineEdit->text().toInt();
        auto minimum = ui->minimum_lineEdit->text().toInt();
        emit addItem_signal(action, heal, condition, random, below, above, minimum);
        ui->action_comboBox->setCurrentIndex(0);
        ui->condition_comboBox->setCurrentIndex(0);
        ui->heal_lineEdit->clear();
        ui->random_lineEdit->clear();
        ui->belowHp_lineEdit->clear();
        ui->aboveHp_lineEdit->clear();
        ui->minimum_lineEdit->clear();
    });

    connect(ui->healing_listWidget, &QListWidget::itemDoubleClicked, this, [this](){
        auto currentIndex = ui->healing_listWidget->currentRow();
        ui->healing_listWidget->takeItem(currentIndex);
        emit deleteItem_signal(currentIndex);
    });
}

HealingView::~HealingView() {
    delete ui;
}

void HealingView::addItem(const QString& item) {
    ui->healing_listWidget->addItem(item);
}

void HealingView::clearListWidget() {
    ui->healing_listWidget->clear();
}
