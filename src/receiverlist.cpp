#include "receiverlist.h"
#include "ui_receiverlist.h"
#include "plugin.h"
#include "config.h"
#include <algorithm>

ReceiverList::ReceiverList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReceiverList)
{
    ui->setupUi(this);
	setWindowTitle("HotkeyMessage :: Receiver List");
}

ReceiverList::~ReceiverList()
{
    delete ui;
}

void ReceiverList::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(ui->actionDelete);
    ui->actionDelete->setData(QVariant(pos));

    menu.exec( ui->treeWidget->mapToGlobal(pos) );
}

void ReceiverList::on_actionDelete_triggered()
{
    QTreeWidgetItem *clickedItem = ui->treeWidget->itemAt(ui->actionDelete->data().toPoint());
	if (!clickedItem) return;
	receivers_list_nick.erase(std::remove(receivers_list_nick.begin(),receivers_list_nick.end(), clickedItem->text(0)),receivers_list_nick.end());
	receivers_list_uid.erase(std::remove(receivers_list_uid.begin(), receivers_list_uid.end(), clickedItem->text(1)), receivers_list_uid.end());
	int i = ui->treeWidget->selectedItems().indexOf(clickedItem);
	pConf->ClearWidgetsItem(i);
	if(ui->treeWidget->topLevelItemCount() == 1) pConf->ClearAll();
	delete clickedItem;
}

void ReceiverList::AddItem_toList(QString& key, QString& value)
{
	QTreeWidgetItem* newItem = new QTreeWidgetItem();
	newItem->setText(0, key);
	newItem->setText(1, value);
	ui->treeWidget->addTopLevelItem(newItem);
}

void ReceiverList::ClearTreeWidget()
{
	ui->treeWidget->clear();
}


