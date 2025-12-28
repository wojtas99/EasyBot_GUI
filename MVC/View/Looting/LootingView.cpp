//
// Created by blazz on 12.10.2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LootingView.h" resolved

#include "LootingView.h"
#include "ui_LootingView.h"


LootingView::LootingView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Looting_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    connect(ui->add_pushButton, &QPushButton::clicked, this, [this]() {
        auto itemID = ui->source_lineEdit->text().toInt();
        auto destination = ui->dest_lineEdit->text();
        auto lootingSpeed = ui->speedlineEdit->text().toInt();
        emit addItem_signal(itemID, destination, lootingSpeed);
        ui->source_lineEdit->clear();
        ui->dest_lineEdit->clear();
        ui->speedlineEdit->clear();
    });

    connect(ui->nextBp_checkBox, &QCheckBox::toggled, this, &LootingView::nextBpState_signal);
    connect(ui->containers_checkBox, &QCheckBox::toggled, this, &LootingView::corpseBpState_signal);

    connect(ui->loot_listWidget, &QListWidget::itemDoubleClicked, this, [this](){
        auto currentIndex = ui->loot_listWidget->currentRow();
        ui->loot_listWidget->takeItem(currentIndex);
        emit deleteItem_signal(currentIndex);
    });
}

LootingView::~LootingView() {
    delete ui;
}

void LootingView::addItem(const QString& item) {
    ui->loot_listWidget->addItem(item);
}

void LootingView::clearListWidget() {
    ui->loot_listWidget->clear();
}
