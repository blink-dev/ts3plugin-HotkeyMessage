#ifndef RECEIVERLIST_H
#define RECEIVERLIST_H

#include <QDialog>
#include <QMenu>

namespace Ui {
class ReceiverList;
}

class ReceiverList : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiverList(QWidget *parent = 0);
    ~ReceiverList();

	void AddItem_toList(QString& key, QString& value);
	void ClearTreeWidget();

private slots:
    void on_actionDelete_triggered();

    void on_treeWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::ReceiverList *ui;
};

#endif // RECEIVERLIST_H
