#ifndef JOBLISTDIALOG_H
#define JOBLISTDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class jobListDialog;
}

class jobListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit jobListDialog(QWidget *parent = 0);
    ~jobListDialog();

    void setItems(QStringList *item_list);
    int getJobSelectedId() const {return m_job_selected_id;}

public slots:
    void onItemClicked(QTableWidgetItem *item);
    void onClick();

private:
    Ui::jobListDialog *ui;
    int m_job_selected_id;
};

#endif // JOBLISTDIALOG_H
