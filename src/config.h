#pragma once
#include <QtWidgets/QDialog>
#pragma comment (lib, "Qt5Widgets")
#pragma comment (lib, "Qt5Core")
#include <QtCore/QSettings>
#include <memory>

namespace Ui {
	class configui;
}

class config : public QDialog
{
	Q_OBJECT
public:
	config(const QString& configLocation, QWidget *parent = nullptr);
	~config();
	config(const config& other) = delete;
	config& operator=(const config& other) = delete;

	void setConfigOption(const QString& option, const QVariant& value);
	QVariant getConfigOption(const QString& option) const;

	void saveRecList(const QString& nick,const QString& uid);

	void ClearWidgetsItem(int indexx);
	void ClearAll();

	int comboBox_SelectedItem(int value);

	void loadVariables();

protected:
	void showEvent(QShowEvent* e) override;

private:
	Ui::configui* m_ui;
	std::unique_ptr<QSettings> m_settings;

	void saveSettings();
	void loadSettings();

	void openRecListDialog();
	
	void setHotkey();

};

