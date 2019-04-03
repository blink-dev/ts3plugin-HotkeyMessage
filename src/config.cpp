#include "config.h"
#include "ui_configui.h"
#include "plugin.h"
#include "receiverlist.h"

char toUID[512];

char msg_msg[10][512];
char msg_msgswitch[10][512];
int msg_switch_enabled[10];

std::vector<QString> receivers_list_nick;
std::vector<QString> receivers_list_uid;

ReceiverList* recListDialog = new ReceiverList;

config::config(const QString& configLocation, QWidget* parent /* = nullptr */) : QDialog(parent),
	m_ui(new Ui::configui),
	m_settings(std::make_unique<QSettings>(configLocation + "/hotkeymsg_config.ini", QSettings::IniFormat, this))
{
	m_ui->setupUi(this);

	setWindowTitle("HotkeyMessage :: Configuration");

	int w, h;
	w = getConfigOption("dialog_width").toInt();
	h = getConfigOption("dialog_height").toInt();

	if (w == 0) { w = 640; h = 726; }

	config::resize(w, h);

	// Connect UI Elements.
	connect(m_ui->SaveBtn, &QPushButton::clicked, this, &config::saveSettings);
	connect(m_ui->CancelBtn, &QPushButton::clicked, this, &QWidget::close);
	connect(m_ui->ReceiverListBtn, &QPushButton::clicked, this, &config::openRecListDialog);
	connect(m_ui->Hotkey1_pushButton, &QPushButton::clicked, this, &config::setHotkey);
	connect(m_ui->Hotkey2_pushButton, &QPushButton::clicked, this, &config::setHotkey);
	connect(m_ui->Hotkey3_pushButton, &QPushButton::clicked, this, &config::setHotkey);
	connect(m_ui->Hotkey4_pushButton, &QPushButton::clicked, this, &config::setHotkey);
	connect(m_ui->Hotkey5_pushButton, &QPushButton::clicked, this, &config::setHotkey);
	connect(m_ui->Hotkey6_pushButton, &QPushButton::clicked, this, &config::setHotkey);
	connect(m_ui->Hotkey7_pushButton, &QPushButton::clicked, this, &config::setHotkey);
	connect(m_ui->Hotkey8_pushButton, &QPushButton::clicked, this, &config::setHotkey);
	connect(m_ui->Hotkey9_pushButton, &QPushButton::clicked, this, &config::setHotkey);
	connect(m_ui->Hotkey10_pushButton, &QPushButton::clicked, this, &config::setHotkey);
	/*connect(m_ui->pbOk, &QPushButton::clicked, this, &config::saveSettings);
	connect(m_ui->pbCancel, &QPushButton::clicked, this, &QWidget::close);
	// Show off some Lambda connect
	connect(m_ui->chkOption1, &QCheckBox::toggled, this, [&](bool checked) {
		m_ui->txtRandomText->setEnabled(checked);
		m_ui->label->setEnabled(checked);
	});*/
	// If we just care for a single element we could also connect that directly:
	connect(m_ui->Hotkey1_checkBox, &QCheckBox::toggled, m_ui->Hotkey1_lineEdit_2, &QLineEdit::setEnabled);
	connect(m_ui->Hotkey2_checkBox, &QCheckBox::toggled, m_ui->Hotkey2_lineEdit_2, &QLineEdit::setEnabled);
	connect(m_ui->Hotkey3_checkBox, &QCheckBox::toggled, m_ui->Hotkey3_lineEdit_2, &QLineEdit::setEnabled);
	connect(m_ui->Hotkey4_checkBox, &QCheckBox::toggled, m_ui->Hotkey4_lineEdit_2, &QLineEdit::setEnabled);
	connect(m_ui->Hotkey5_checkBox, &QCheckBox::toggled, m_ui->Hotkey5_lineEdit_2, &QLineEdit::setEnabled);
	connect(m_ui->Hotkey6_checkBox, &QCheckBox::toggled, m_ui->Hotkey6_lineEdit_2, &QLineEdit::setEnabled);
	connect(m_ui->Hotkey7_checkBox, &QCheckBox::toggled, m_ui->Hotkey7_lineEdit_2, &QLineEdit::setEnabled);
	connect(m_ui->Hotkey8_checkBox, &QCheckBox::toggled, m_ui->Hotkey8_lineEdit_2, &QLineEdit::setEnabled);
	connect(m_ui->Hotkey9_checkBox, &QCheckBox::toggled, m_ui->Hotkey9_lineEdit_2, &QLineEdit::setEnabled);
	connect(m_ui->Hotkey10_checkBox, &QCheckBox::toggled, m_ui->Hotkey10_lineEdit_2, &QLineEdit::setEnabled);

	loadSettings();
}

config::~config() {
	m_settings->sync();
	delete m_ui;
	if (recListDialog) {
		recListDialog->close();
		delete recListDialog;
		recListDialog = nullptr;
	}
}


void config::setConfigOption(const QString& option, const QVariant& value) {
	m_settings->setValue(option, value);
}

QVariant config::getConfigOption(const QString& option) const {
	return m_settings->value(option);
}

void config::showEvent(QShowEvent* /* e */) {
	//loadSettings();
}

void config::saveSettings()
{
	setConfigOption("dialog_width", config::size().width());
	setConfigOption("dialog_height", config::size().height());

	setConfigOption("Hotkey1_message", m_ui->Hotkey1_lineEdit->text());
	setConfigOption("Hotkey1_switch_enabled", m_ui->Hotkey1_checkBox->isChecked());
	setConfigOption("Hotkey1_switch_message", m_ui->Hotkey1_lineEdit_2->text());
	setConfigOption("Hotkey1_comboBox", m_ui->Hotkey1_comboBox->currentText());

	setConfigOption("Hotkey2_message", m_ui->Hotkey2_lineEdit->text());
	setConfigOption("Hotkey2_switch_enabled", m_ui->Hotkey2_checkBox->isChecked());
	setConfigOption("Hotkey2_switch_message", m_ui->Hotkey2_lineEdit_2->text());
	setConfigOption("Hotkey2_comboBox", m_ui->Hotkey2_comboBox->currentText());

	setConfigOption("Hotkey3_message", m_ui->Hotkey3_lineEdit->text());
	setConfigOption("Hotkey3_switch_enabled", m_ui->Hotkey3_checkBox->isChecked());
	setConfigOption("Hotkey3_switch_message", m_ui->Hotkey3_lineEdit_2->text());
	setConfigOption("Hotkey3_comboBox", m_ui->Hotkey3_comboBox->currentText());

	setConfigOption("Hotkey4_message", m_ui->Hotkey4_lineEdit->text());
	setConfigOption("Hotkey4_switch_enabled", m_ui->Hotkey4_checkBox->isChecked());
	setConfigOption("Hotkey4_switch_message", m_ui->Hotkey4_lineEdit_2->text());
	setConfigOption("Hotkey4_comboBox", m_ui->Hotkey4_comboBox->currentText());

	setConfigOption("Hotkey5_message", m_ui->Hotkey5_lineEdit->text());
	setConfigOption("Hotkey5_switch_enabled", m_ui->Hotkey5_checkBox->isChecked());
	setConfigOption("Hotkey5_switch_message", m_ui->Hotkey5_lineEdit_2->text());
	setConfigOption("Hotkey5_comboBox", m_ui->Hotkey5_comboBox->currentText());

	setConfigOption("Hotkey6_message", m_ui->Hotkey6_lineEdit->text());
	setConfigOption("Hotkey6_switch_enabled", m_ui->Hotkey6_checkBox->isChecked());
	setConfigOption("Hotkey6_switch_message", m_ui->Hotkey6_lineEdit_2->text());
	setConfigOption("Hotkey6_comboBox", m_ui->Hotkey6_comboBox->currentText());

	setConfigOption("Hotkey7_message", m_ui->Hotkey7_lineEdit->text());
	setConfigOption("Hotkey7_switch_enabled", m_ui->Hotkey7_checkBox->isChecked());
	setConfigOption("Hotkey7_switch_message", m_ui->Hotkey7_lineEdit_2->text());
	setConfigOption("Hotkey7_comboBox", m_ui->Hotkey7_comboBox->currentText());

	setConfigOption("Hotkey8_message", m_ui->Hotkey8_lineEdit->text());
	setConfigOption("Hotkey8_switch_enabled", m_ui->Hotkey8_checkBox->isChecked());
	setConfigOption("Hotkey8_switch_message", m_ui->Hotkey8_lineEdit_2->text());
	setConfigOption("Hotkey8_comboBox", m_ui->Hotkey8_comboBox->currentText());

	setConfigOption("Hotkey9_message", m_ui->Hotkey9_lineEdit->text());
	setConfigOption("Hotkey9_switch_enabled", m_ui->Hotkey9_checkBox->isChecked());
	setConfigOption("Hotkey9_switch_message", m_ui->Hotkey9_lineEdit_2->text());
	setConfigOption("Hotkey9_comboBox", m_ui->Hotkey9_comboBox->currentText());

	setConfigOption("Hotkey10_message", m_ui->Hotkey10_lineEdit->text());
	setConfigOption("Hotkey10_switch_enabled", m_ui->Hotkey10_checkBox->isChecked());
	setConfigOption("Hotkey10_switch_message", m_ui->Hotkey10_lineEdit_2->text());
	setConfigOption("Hotkey10_comboBox", m_ui->Hotkey10_comboBox->currentText());

	config::loadVariables();

	close();
}

int config::comboBox_SelectedItem(int value)
{
	switch (value) {
		case 1:return m_ui->Hotkey1_comboBox->currentIndex(); break;
		case 2:return m_ui->Hotkey2_comboBox->currentIndex(); break;
		case 3:return m_ui->Hotkey3_comboBox->currentIndex(); break;
		case 4:return m_ui->Hotkey4_comboBox->currentIndex(); break;
		case 5:return m_ui->Hotkey5_comboBox->currentIndex(); break;
		case 6:return m_ui->Hotkey6_comboBox->currentIndex(); break;
		case 7:return m_ui->Hotkey7_comboBox->currentIndex(); break;
		case 8:return m_ui->Hotkey8_comboBox->currentIndex(); break;
		case 9:return m_ui->Hotkey9_comboBox->currentIndex(); break;
		case 10:return m_ui->Hotkey10_comboBox->currentIndex(); break;
	}
}

void config::saveRecList(const QString& nick,const QString& uid)
{
	int size = m_settings->beginReadArray("Receivers");
	m_settings->endArray();
	m_settings->beginWriteArray("Receivers");
	m_settings->setArrayIndex(size);
	m_settings->setValue("nick", nick);
	m_settings->setValue("uid", uid);
	m_settings->endArray();

	m_ui->Hotkey1_comboBox->addItem(nick);
	m_ui->Hotkey2_comboBox->addItem(nick);
	m_ui->Hotkey3_comboBox->addItem(nick);
	m_ui->Hotkey4_comboBox->addItem(nick);
	m_ui->Hotkey5_comboBox->addItem(nick);
	m_ui->Hotkey6_comboBox->addItem(nick);
	m_ui->Hotkey7_comboBox->addItem(nick);
	m_ui->Hotkey8_comboBox->addItem(nick);
	m_ui->Hotkey9_comboBox->addItem(nick);
	m_ui->Hotkey10_comboBox->addItem(nick);
	recListDialog->AddItem_toList((QString)nick, (QString)uid);
}

void config::ClearAll() {
	m_settings->beginWriteArray("Receivers");
	m_settings->remove("");
	m_settings->endArray();
}

void config::ClearWidgetsItem(QString& text)
{
	m_settings->beginWriteArray("Receivers");
	m_settings->remove("");
	m_settings->endArray();

	m_settings->beginWriteArray("Receivers");
	for (int i = 0; i < static_cast<int>(receivers_list_nick.size()); i++) {
		m_settings->setArrayIndex(i);
		m_settings->setValue("nick", receivers_list_nick[i]);
		m_settings->setValue("uid", receivers_list_uid[i]);
	}
	m_settings->endArray();

	m_ui->Hotkey1_comboBox->removeItem(m_ui->Hotkey1_comboBox->findText(text));
	m_ui->Hotkey2_comboBox->removeItem(m_ui->Hotkey2_comboBox->findText(text));
	m_ui->Hotkey3_comboBox->removeItem(m_ui->Hotkey3_comboBox->findText(text));
	m_ui->Hotkey4_comboBox->removeItem(m_ui->Hotkey4_comboBox->findText(text));
	m_ui->Hotkey5_comboBox->removeItem(m_ui->Hotkey5_comboBox->findText(text));
	m_ui->Hotkey6_comboBox->removeItem(m_ui->Hotkey6_comboBox->findText(text));
	m_ui->Hotkey7_comboBox->removeItem(m_ui->Hotkey7_comboBox->findText(text));
	m_ui->Hotkey8_comboBox->removeItem(m_ui->Hotkey8_comboBox->findText(text));
	m_ui->Hotkey9_comboBox->removeItem(m_ui->Hotkey9_comboBox->findText(text));
	m_ui->Hotkey10_comboBox->removeItem(m_ui->Hotkey10_comboBox->findText(text));


}

void config::loadSettings() 
{
	int size = m_settings->beginReadArray("Receivers");
	receivers_list_nick.resize(size);
	receivers_list_uid.resize(size);
	for (int i = 0; i < size; i++) {
		m_settings->setArrayIndex(i);
		receivers_list_nick[i] = m_settings->value("nick").toString();
		receivers_list_uid[i]= m_settings->value("uid").toString();
	}
	m_settings->endArray();

	m_ui->Hotkey1_comboBox->addItem("[Channel]");
	m_ui->Hotkey2_comboBox->addItem("[Channel]");
	m_ui->Hotkey3_comboBox->addItem("[Channel]");
	m_ui->Hotkey4_comboBox->addItem("[Channel]");
	m_ui->Hotkey5_comboBox->addItem("[Channel]");
	m_ui->Hotkey6_comboBox->addItem("[Channel]");
	m_ui->Hotkey7_comboBox->addItem("[Channel]");
	m_ui->Hotkey8_comboBox->addItem("[Channel]");
	m_ui->Hotkey9_comboBox->addItem("[Channel]");
	m_ui->Hotkey10_comboBox->addItem("[Channel]");

	for (int i = 0; i < static_cast<int>(receivers_list_nick.size()); i++) {
		m_ui->Hotkey1_comboBox->addItem(receivers_list_nick[i]);
		m_ui->Hotkey2_comboBox->addItem(receivers_list_nick[i]);
		m_ui->Hotkey3_comboBox->addItem(receivers_list_nick[i]);
		m_ui->Hotkey4_comboBox->addItem(receivers_list_nick[i]);
		m_ui->Hotkey5_comboBox->addItem(receivers_list_nick[i]);
		m_ui->Hotkey6_comboBox->addItem(receivers_list_nick[i]);
		m_ui->Hotkey7_comboBox->addItem(receivers_list_nick[i]);
		m_ui->Hotkey8_comboBox->addItem(receivers_list_nick[i]);
		m_ui->Hotkey9_comboBox->addItem(receivers_list_nick[i]);
		m_ui->Hotkey10_comboBox->addItem(receivers_list_nick[i]);
		recListDialog->AddItem_toList(receivers_list_nick[i], receivers_list_uid[i]);
	}

	m_ui->Hotkey1_lineEdit->setText(getConfigOption("Hotkey1_message").toString());
	m_ui->Hotkey1_checkBox->setChecked(getConfigOption("Hotkey1_switch_enabled").toBool());
	m_ui->Hotkey1_lineEdit_2->setText(getConfigOption("Hotkey1_switch_message").toString());
	m_ui->Hotkey1_comboBox->setCurrentText(getConfigOption("Hotkey1_comboBox").toString());

	m_ui->Hotkey2_lineEdit->setText(getConfigOption("Hotkey2_message").toString());
	m_ui->Hotkey2_checkBox->setChecked(getConfigOption("Hotkey2_switch_enabled").toBool());
	m_ui->Hotkey2_lineEdit_2->setText(getConfigOption("Hotkey2_switch_message").toString());
	m_ui->Hotkey2_comboBox->setCurrentText(getConfigOption("Hotkey2_comboBox").toString());

	m_ui->Hotkey3_lineEdit->setText(getConfigOption("Hotkey3_message").toString());
	m_ui->Hotkey3_checkBox->setChecked(getConfigOption("Hotkey3_switch_enabled").toBool());
	m_ui->Hotkey3_lineEdit_2->setText(getConfigOption("Hotkey3_switch_message").toString());
	m_ui->Hotkey3_comboBox->setCurrentText(getConfigOption("Hotkey3_comboBox").toString());

	m_ui->Hotkey4_lineEdit->setText(getConfigOption("Hotkey4_message").toString());
	m_ui->Hotkey4_checkBox->setChecked(getConfigOption("Hotkey4_switch_enabled").toBool());
	m_ui->Hotkey4_lineEdit_2->setText(getConfigOption("Hotkey4_switch_message").toString());
	m_ui->Hotkey4_comboBox->setCurrentText(getConfigOption("Hotkey4_comboBox").toString());

	m_ui->Hotkey5_lineEdit->setText(getConfigOption("Hotkey5_message").toString());
	m_ui->Hotkey5_checkBox->setChecked(getConfigOption("Hotkey5_switch_enabled").toBool());
	m_ui->Hotkey5_lineEdit_2->setText(getConfigOption("Hotkey5_switch_message").toString());
	m_ui->Hotkey5_comboBox->setCurrentText(getConfigOption("Hotkey5_comboBox").toString());

	m_ui->Hotkey6_lineEdit->setText(getConfigOption("Hotkey6_message").toString());
	m_ui->Hotkey6_checkBox->setChecked(getConfigOption("Hotkey6_switch_enabled").toBool());
	m_ui->Hotkey6_lineEdit_2->setText(getConfigOption("Hotkey6_switch_message").toString());
	m_ui->Hotkey6_comboBox->setCurrentText(getConfigOption("Hotkey6_comboBox").toString());

	m_ui->Hotkey7_lineEdit->setText(getConfigOption("Hotkey7_message").toString());
	m_ui->Hotkey7_checkBox->setChecked(getConfigOption("Hotkey7_switch_enabled").toBool());
	m_ui->Hotkey7_lineEdit_2->setText(getConfigOption("Hotkey7_switch_message").toString());
	m_ui->Hotkey7_comboBox->setCurrentText(getConfigOption("Hotkey7_comboBox").toString());

	m_ui->Hotkey8_lineEdit->setText(getConfigOption("Hotkey8_message").toString());
	m_ui->Hotkey8_checkBox->setChecked(getConfigOption("Hotkey8_switch_enabled").toBool());
	m_ui->Hotkey8_lineEdit_2->setText(getConfigOption("Hotkey8_switch_message").toString());
	m_ui->Hotkey8_comboBox->setCurrentText(getConfigOption("Hotkey8_comboBox").toString());

	m_ui->Hotkey9_lineEdit->setText(getConfigOption("Hotkey9_message").toString());
	m_ui->Hotkey9_checkBox->setChecked(getConfigOption("Hotkey9_switch_enabled").toBool());
	m_ui->Hotkey9_lineEdit_2->setText(getConfigOption("Hotkey9_switch_message").toString());
	m_ui->Hotkey9_comboBox->setCurrentText(getConfigOption("Hotkey9_comboBox").toString());

	m_ui->Hotkey10_lineEdit->setText(getConfigOption("Hotkey10_message").toString());
	m_ui->Hotkey10_checkBox->setChecked(getConfigOption("Hotkey10_switch_enabled").toBool());
	m_ui->Hotkey10_lineEdit_2->setText(getConfigOption("Hotkey10_switch_message").toString());
	m_ui->Hotkey10_comboBox->setCurrentText(getConfigOption("Hotkey10_comboBox").toString());
}

void config::loadVariables() {
	for (int i = 0; i < 10; i++) {
		char buf[50];
		sprintf_s(buf, "%s%d%s", "Hotkey", i + 1, "_message");
		char buf2[50];
		sprintf_s(buf2, "%s%d%s", "Hotkey", i + 1, "_switch_message");
		char buf3[50];
		sprintf_s(buf3, "%s%d%s", "Hotkey", i + 1, "_switch_enabled");

		strcpy_s(msg_msg[i], getConfigOption(buf).toString().toStdString().c_str());
		strcpy_s(msg_msgswitch[i], getConfigOption(buf2).toString().toStdString().c_str());
		msg_switch_enabled[i] = getConfigOption(buf3).toBool();
	}
}

void config::setHotkey()
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());

	int configId = 0;
	QString objName = button->objectName();

	sscanf_s(objName.toStdString().c_str(), "Hotkey%d", &configId);
	
	char buf[16];
	sprintf_s(buf, "send_message%d", configId);
	ts3Functions.requestHotkeyInputDialog(getPluginID(), buf, 1, this);
	
}

void config::openRecListDialog()
{
	if (recListDialog->isVisible()) {
		// Window is already displayed somewhere, bring it to the top and give it focus
		recListDialog->raise();
		recListDialog->activateWindow();
	}
	else {
		recListDialog->show();
	}
}
