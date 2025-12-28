#include "ScriptsView.h"

#include <iostream>
#include <QCheckBox>
#include <QTextEdit>
#include <QEvent>
#include "ScriptEditorDialog.h"

#include "Highlighter.h"
#include "ui_ScriptsView.h"


ScriptsView::ScriptsView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Scripts_View) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    ui->scripts_tableWidget->horizontalHeader()
        ->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->scripts_tableWidget->horizontalHeader()
        ->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    connect(ui->add_pushButton, &QPushButton::clicked, this, [this]() {
        int row = ui->scripts_tableWidget->rowCount();
        ui->scripts_tableWidget->insertRow(row);

        auto *textEdit = new QTextEdit(ui->scripts_tableWidget);
        ui->scripts_tableWidget->setCellWidget(row, 0, textEdit);
        new Highlighter(textEdit->document());
        textEdit->installEventFilter(this);
        textEdit->viewport()->installEventFilter(this);

        auto *checkBox = new QCheckBox();


        connect(checkBox, &QCheckBox::toggled, this, [this, row](bool checked) {
            if (checked) {
                QWidget *cellWidget = ui->scripts_tableWidget->cellWidget(row, 0);
                QTextEdit *textEdit = qobject_cast<QTextEdit *>(cellWidget);
                textEdit->setEnabled(false);
                emit addItem_signal(row, checked, textEdit->toPlainText());

            } else {
                QWidget *cellWidget = ui->scripts_tableWidget->cellWidget(row, 0);
                QTextEdit *textEdit = qobject_cast<QTextEdit *>(cellWidget);
                textEdit->setEnabled(true);
                emit addItem_signal(row, checked, textEdit->toPlainText());
            }
        });

        QWidget *checkBoxWidget = new QWidget(ui->scripts_tableWidget);
        QHBoxLayout *layout = new QHBoxLayout(checkBoxWidget);
        layout->addWidget(checkBox);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);
        ui->scripts_tableWidget->setCellWidget(row, 1, checkBoxWidget);
    });
}

ScriptsView::~ScriptsView() {
    delete ui;
}

bool ScriptsView::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonDblClick) {
        QTextEdit *textEdit = qobject_cast<QTextEdit*>(obj);
        if (!textEdit && obj->parent()) {
            textEdit = qobject_cast<QTextEdit*>(obj->parent());
        }

        if (textEdit && textEdit->isEnabled()) {
            ScriptEditorDialog dialog(textEdit->toPlainText(), this);
            if (dialog.exec() == QDialog::Accepted) {
                textEdit->setPlainText(dialog.getScript());
            }
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

