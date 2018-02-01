// Qt
#include <QPushButton>

#include "jobListDialog.h"
#include "ui_jobListDialog.h"


jobListDialog::jobListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::jobListDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    QStringList labels;
    labels << "JobID" << "Partition" << "Name" << "User" << "ST" << "Time" << "Nodes" << "NodeList";
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    m_job_selected_id = -1;

    connect(ui->tableWidget, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(onItemClicked(QTableWidgetItem *)));
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(onClick()));
    connect(ui->buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked(bool)), this, SLOT(onClick()));
}

jobListDialog::~jobListDialog()
{
    delete ui;
}

//-----------------------------------------------------------------------------
void jobListDialog::setItems(QStringList *item_list) {
    int ncols = ui->tableWidget->columnCount();
    int nrows = int(item_list->size() / ncols) - 1;
    ui->tableWidget->setRowCount(nrows);
    for (int j=ncols; j<item_list->size(); ++j) {
        QTableWidgetItem *item = new QTableWidgetItem(tr("%1").arg(item_list->at(j)));
        ui->tableWidget->setItem(int(j / ncols) - 1, j % ncols, item);
    }
}

//-----------------------------------------------------------------------------
void jobListDialog::onItemClicked(QTableWidgetItem *item)
{
    int current_row = item->row();
    ui->tableWidget->setCurrentCell(current_row, 0);
    m_job_selected_id = ui->tableWidget->currentItem()->text().toInt();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

//-----------------------------------------------------------------------------
void jobListDialog::onClick()
{
    this->close();
}
