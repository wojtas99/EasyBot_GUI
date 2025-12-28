#include "SpellsView.h"


SpellsView::SpellsView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Spells_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    connect(ui->add_pushButton, &QPushButton::clicked, this,[this] {
        auto target_name = ui->name_lineEdit->text();
        auto dist = ui->dist_comboBox->currentIndex();
        auto count = ui->count_comboBox->currentIndex() + 1;
        auto spell = ui->spell_comboBox->currentText();
        auto spell_name = ui->spell_lineEdit->text();
        auto type = ui->type_comboBox->currentIndex();
        auto from = ui->from_lineEdit->text().toInt();
        auto to = ui->to_lineEdit->text().toInt();
        auto minHp = ui->playerHp_lineEdit->text().toInt();
        auto minMp = ui->playerMp_lineEdit->text().toInt();

        emit addItem_signal(target_name, dist, count, spell, spell_name,type, from, to,minHp, minMp);

        ui->name_lineEdit->clear();
        ui->dist_comboBox->setCurrentIndex(0);
        ui->count_comboBox->setCurrentIndex(0);
        ui->spell_comboBox->setCurrentIndex(0);
        ui->spell_lineEdit->clear();
        ui->type_comboBox->setCurrentIndex(0);
        ui->from_lineEdit->clear();
        ui->to_lineEdit->clear();
        ui->playerHp_lineEdit->clear();
        ui->playerMp_lineEdit->clear();
    });
    connect(ui->spells_listWidget, &QListWidget::itemDoubleClicked, this, [this](){
        auto currentIndex = ui->spells_listWidget->currentRow();
        ui->spells_listWidget->takeItem(currentIndex);
        emit deleteItem_signal(currentIndex);
    });
}

SpellsView::~SpellsView() {
    delete ui;
}

void SpellsView::addItem(const QString& item) {
    ui->spells_listWidget->addItem(item);
}

void SpellsView::clearListWidget() {
    ui->spells_listWidget->clear();
}
