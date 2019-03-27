#include "config.h"
#include "ui_configui.h"

#include "plugin.h"

char toUID[512];

char msg_msg[512][11];
char msg_msgswitch[512][11];
int msg_switch_enabled[11];


config::config(const QString& configLocation, QWidget* parent /* = nullptr */) : QDialog(parent),
	m_ui(std::make_unique<Ui::configui>()),
	m_settings(std::make_unique<QSettings>(configLocation + "/hotkeymsg_config.ini", QSettings::IniFormat, this))
{
	m_ui->setupUi(this);

	setWindowTitle("HotkeyMessage :: Config");

	// Connect UI Elements.
	connect(m_ui->SaveBtn, &QPushButton::clicked, this, &config::saveSettings);
	connect(m_ui->CancelBtn, &QPushButton::clicked, this, &QWidget::close);
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
}

config::~config() {
	m_settings->sync();
}


void config::setConfigOption(const QString& option, const QVariant& value) {
	m_settings->setValue(option, value);
}

QVariant config::getConfigOption(const QString& option) const {
	return m_settings->value(option);
}

void config::showEvent(QShowEvent* /* e */) {
	loadSettings();
}

void config::saveSettings() 
{
	setConfigOption("Hotkey1_message", m_ui->Hotkey1_lineEdit->text());
	setConfigOption("Hotkey1_switch_enabled", m_ui->Hotkey1_checkBox->isChecked());
	setConfigOption("Hotkey1_switch_message", m_ui->Hotkey1_lineEdit_2->text());

	setConfigOption("Hotkey2_message", m_ui->Hotkey2_lineEdit->text());
	setConfigOption("Hotkey2_switch_enabled", m_ui->Hotkey2_checkBox->isChecked());
	setConfigOption("Hotkey2_switch_message", m_ui->Hotkey2_lineEdit_2->text());

	setConfigOption("Hotkey3_message", m_ui->Hotkey3_lineEdit->text());
	setConfigOption("Hotkey3_switch_enabled", m_ui->Hotkey3_checkBox->isChecked());
	setConfigOption("Hotkey3_switch_message", m_ui->Hotkey3_lineEdit_2->text());

	setConfigOption("Hotkey4_message", m_ui->Hotkey4_lineEdit->text());
	setConfigOption("Hotkey4_switch_enabled", m_ui->Hotkey4_checkBox->isChecked());
	setConfigOption("Hotkey4_switch_message", m_ui->Hotkey4_lineEdit_2->text());

	setConfigOption("Hotkey5_message", m_ui->Hotkey5_lineEdit->text());
	setConfigOption("Hotkey5_switch_enabled", m_ui->Hotkey5_checkBox->isChecked());
	setConfigOption("Hotkey5_switch_message", m_ui->Hotkey5_lineEdit_2->text());

	setConfigOption("Hotkey6_message", m_ui->Hotkey6_lineEdit->text());
	setConfigOption("Hotkey6_switch_enabled", m_ui->Hotkey6_checkBox->isChecked());
	setConfigOption("Hotkey6_switch_message", m_ui->Hotkey6_lineEdit_2->text());

	setConfigOption("Hotkey7_message", m_ui->Hotkey7_lineEdit->text());
	setConfigOption("Hotkey7_switch_enabled", m_ui->Hotkey7_checkBox->isChecked());
	setConfigOption("Hotkey7_switch_message", m_ui->Hotkey7_lineEdit_2->text());

	setConfigOption("Hotkey8_message", m_ui->Hotkey8_lineEdit->text());
	setConfigOption("Hotkey8_switch_enabled", m_ui->Hotkey8_checkBox->isChecked());
	setConfigOption("Hotkey8_switch_message", m_ui->Hotkey8_lineEdit_2->text());

	setConfigOption("Hotkey9_message", m_ui->Hotkey9_lineEdit->text());
	setConfigOption("Hotkey9_switch_enabled", m_ui->Hotkey9_checkBox->isChecked());
	setConfigOption("Hotkey9_switch_message", m_ui->Hotkey9_lineEdit_2->text());

	setConfigOption("Hotkey10_message", m_ui->Hotkey10_lineEdit->text());
	setConfigOption("Hotkey10_switch_enabled", m_ui->Hotkey10_checkBox->isChecked());
	setConfigOption("Hotkey10_switch_message", m_ui->Hotkey10_lineEdit_2->text());

	//setConfigOption("intVal", m_ui->speSomeIntValue->value());
	//setConfigOption("text", m_ui->txtRandomText->toPlainText());

	config::loadVariables();

	close();
}

void config::loadSettings() 
{
	m_ui->Hotkey1_lineEdit->setText(getConfigOption("Hotkey1_message").toString());
	m_ui->Hotkey1_checkBox->setChecked(getConfigOption("Hotkey1_switch_enabled").toBool());
	m_ui->Hotkey1_lineEdit_2->setText(getConfigOption("Hotkey1_switch_message").toString());

	m_ui->Hotkey2_lineEdit->setText(getConfigOption("Hotkey2_message").toString());
	m_ui->Hotkey2_checkBox->setChecked(getConfigOption("Hotkey2_switch_enabled").toBool());
	m_ui->Hotkey2_lineEdit_2->setText(getConfigOption("Hotkey2_switch_message").toString());

	m_ui->Hotkey3_lineEdit->setText(getConfigOption("Hotkey3_message").toString());
	m_ui->Hotkey3_checkBox->setChecked(getConfigOption("Hotkey3_switch_enabled").toBool());
	m_ui->Hotkey3_lineEdit_2->setText(getConfigOption("Hotkey3_switch_message").toString());

	m_ui->Hotkey4_lineEdit->setText(getConfigOption("Hotkey4_message").toString());
	m_ui->Hotkey4_checkBox->setChecked(getConfigOption("Hotkey4_switch_enabled").toBool());
	m_ui->Hotkey4_lineEdit_2->setText(getConfigOption("Hotkey4_switch_message").toString());

	m_ui->Hotkey5_lineEdit->setText(getConfigOption("Hotkey5_message").toString());
	m_ui->Hotkey5_checkBox->setChecked(getConfigOption("Hotkey5_switch_enabled").toBool());
	m_ui->Hotkey5_lineEdit_2->setText(getConfigOption("Hotkey5_switch_message").toString());

	m_ui->Hotkey6_lineEdit->setText(getConfigOption("Hotkey6_message").toString());
	m_ui->Hotkey6_checkBox->setChecked(getConfigOption("Hotkey6_switch_enabled").toBool());
	m_ui->Hotkey6_lineEdit_2->setText(getConfigOption("Hotkey6_switch_message").toString());

	m_ui->Hotkey7_lineEdit->setText(getConfigOption("Hotkey7_message").toString());
	m_ui->Hotkey7_checkBox->setChecked(getConfigOption("Hotkey7_switch_enabled").toBool());
	m_ui->Hotkey7_lineEdit_2->setText(getConfigOption("Hotkey7_switch_message").toString());

	m_ui->Hotkey8_lineEdit->setText(getConfigOption("Hotkey8_message").toString());
	m_ui->Hotkey8_checkBox->setChecked(getConfigOption("Hotkey8_switch_enabled").toBool());
	m_ui->Hotkey8_lineEdit_2->setText(getConfigOption("Hotkey8_switch_message").toString());

	m_ui->Hotkey9_lineEdit->setText(getConfigOption("Hotkey9_message").toString());
	m_ui->Hotkey9_checkBox->setChecked(getConfigOption("Hotkey9_switch_enabled").toBool());
	m_ui->Hotkey9_lineEdit_2->setText(getConfigOption("Hotkey9_switch_message").toString());

	m_ui->Hotkey10_lineEdit->setText(getConfigOption("Hotkey10_message").toString());
	m_ui->Hotkey10_checkBox->setChecked(getConfigOption("Hotkey10_switch_enabled").toBool());
	m_ui->Hotkey10_lineEdit_2->setText(getConfigOption("Hotkey10_switch_message").toString());
	
	/*m_ui->chkOption1->setChecked(getConfigOption("opt1").toBool());
	m_ui->txtRandomText->setPlainText(getConfigOption("text").toString());
	m_ui->speSomeIntValue->setValue(getConfigOption("intVal").toInt());*/

	// Manually set enabled state of textbox, as the event doesn't fire if the state is the same as it was already (ie. unchecked).
	//m_ui->txtRandomText->setEnabled(m_ui->chkOption1->isChecked());
}

void config::loadVariables() {
	strcpy_s(toUID, getConfigOption("toUID").toString().toStdString().c_str());

	for (int i = 1; i < 11; i++) {
		char buf[50];
		sprintf_s(buf, "%s%d%s", "Hotkey", i, "_message");
		char buf2[50];
		sprintf_s(buf2, "%s%d%s", "Hotkey", i, "_switch_message");
		char buf3[50];
		sprintf_s(buf3, "%s%d%s", "Hotkey", i, "_switch_enabled");

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
