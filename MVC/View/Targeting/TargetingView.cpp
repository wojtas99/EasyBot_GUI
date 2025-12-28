//
// Created by blazz on 12.10.2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TargetingView.h" resolved

#include "TargetingView.h"


TargetingView::TargetingView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Targeting_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    connect(ui->add_pushButton, &QPushButton::clicked, this, [this]() {
        QString targetName = ui->name_lineEdit->text();
        int dist = ui->dist_comboBox->currentIndex();
        int count = ui->count_comboBox->currentIndex() + 1;
        QString desiredStance = ui->stance_comboBox->currentText();
        QString monstersAttacks = ui->monsterAttacks_comboBox->currentText();
        emit addItem_signal(targetName, dist, count, desiredStance, monstersAttacks);
        ui->name_lineEdit->clear();
        ui->dist_comboBox->setCurrentIndex(0);
        ui->stance_comboBox->setCurrentIndex(0);
        ui->count_comboBox->setCurrentIndex(0);
        ui->monsterAttacks_comboBox->setCurrentIndex(0);
    });

    connect(ui->shootable_checkBox, &QCheckBox::toggled, this, &TargetingView::shootableState_signal);
    connect(ui->reachable_checkBox, &QCheckBox::toggled, this, &TargetingView::reachableState_signal);
    connect(ui->open_checkBox, &QCheckBox::toggled, this, &TargetingView::openCorpseState_signal);
    connect(ui->dist_horizontalSlider, &QSlider::valueChanged, this, &TargetingView::stayAwayDist_signal);

    connect(ui->targets_listWidget, &QListWidget::itemDoubleClicked, this, [this](){
        auto currentIndex = ui->targets_listWidget->currentRow();
        ui->targets_listWidget->takeItem(currentIndex);
        emit deleteItem_signal(currentIndex);
    });
}

TargetingView::~TargetingView() {
    delete ui;
}

void TargetingView::addItem(const QString &item) {
    ui->targets_listWidget->addItem(item);
}

void TargetingView::clearListWidget() {
    ui->targets_listWidget->clear();
}
