
#ifdef _WIN32
#pragma warning (disable : 4100)  /* Disable Unreferenced parameter warning */
#include <Windows.h>
#endif

#include "plugin.h"


config* pConf = nullptr;

struct TS3Functions ts3Functions;

#ifdef _WIN32
#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#define snprintf sprintf_s
#else
#define _strcpy(dest, destSize, src) { strncpy(dest, src, destSize-1); (dest)[destSize-1] = '\0'; }
#endif

#define PLUGIN_API_VERSION 22

static char* pluginID = NULL;

const char* getPluginID()
{
	return pluginID;
}

#ifdef _WIN32
/* Helper function to convert wchar_T to Utf-8 encoded strings on Windows */
static int wcharToUtf8(const wchar_t* str, char** result) {
	int outlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, 0, 0);
	*result = (char*)malloc(outlen);
	if(WideCharToMultiByte(CP_UTF8, 0, str, -1, *result, outlen, 0, 0) == 0) {
		*result = NULL;
		return -1;
	}
	return 0;
}
#endif

anyID toID = -1;

int switch_msg[10] = { 1,1,1,1,1,1,1,1,1,1 };

anyID getClientIdByUniqueId(const char* cluid) {
	anyID foundid = -1;
	anyID* clients;
	ts3Functions.getClientList(ts3Functions.getCurrentServerConnectionHandlerID(), &clients);
	char* uid = NULL;
	for (int i = 0; clients[i]; ++i) {
		if (ts3Functions.getClientVariableAsString(ts3Functions.getCurrentServerConnectionHandlerID(), clients[i], CLIENT_UNIQUE_IDENTIFIER, &uid) == ERROR_ok) {
			if (!strcmp(cluid, uid)) {
				foundid = clients[i];
				ts3Functions.freeMemory(uid);
				if (foundid) {
					toID = foundid;
					break;
				}
			}
		}
	}

	ts3Functions.freeMemory(clients);
	return foundid;
}

/*********************************** Required functions ************************************/
/*
 * If any of these required functions is not implemented, TS3 will refuse to load the plugin
 */

/* Unique name identifying this plugin */
const char* ts3plugin_name() {
#ifdef _WIN32
	/* TeamSpeak expects UTF-8 encoded characters. Following demonstrates a possibility how to convert UTF-16 wchar_t into UTF-8. */
	static char* result = NULL;  /* Static variable so it's allocated only once */
	if(!result) {
		const wchar_t* name = L"Hotkey Message";
		if(wcharToUtf8(name, &result) == -1) {  /* Convert name into UTF-8 encoded result */
			result = "Hotkey Message";  /* Conversion failed, fallback here */
		}
	}
	return result;
#else
	return "Hotkey Message";
#endif
}

/* Plugin version */
const char* ts3plugin_version() {
    return "1.0.3";
}

/* Plugin API version. Must be the same as the clients API major version, else the plugin fails to load. */
int ts3plugin_apiVersion() {
	return PLUGIN_API_VERSION;
}

/* Plugin author */
const char* ts3plugin_author() {
	/* If you want to use wchar_t, see ts3plugin_name() on how to use */
    return "blink;";
}

/* Plugin description */
const char* ts3plugin_description() {
	/* If you want to use wchar_t, see ts3plugin_name() on how to use */
    return "Send messages with hotkeys";
}

/* Set TeamSpeak 3 callback functions */
void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
    ts3Functions = funcs;
}

/*
 * Custom code called right after loading the plugin. Returns 0 on success, 1 on failure.
 * If the function returns 1 on failure, the plugin will be unloaded again.
 */
int ts3plugin_init() {
	if (pConf == nullptr) {
		char path[128];
		ts3Functions.getConfigPath(path, 128);
		// Can use the qParentWidget pointer here, to make whatever window the client deems appropriate the parent of our config dialog
		//pConf = new config(QString::fromUtf8(path), static_cast<QWidget*>(qParentWidget));
		// or just ignore it and create it without a parent.
		pConf = new config(QString::fromUtf8(path));
	}

	pConf->loadVariables();

    return 0;  /* 0 = success, 1 = failure, -2 = failure but client will not show a "failed to load" warning */
	/* -2 is a very special case and should only be used if a plugin displays a dialog (e.g. overlay) asking the user to disable
	 * the plugin again, avoiding the show another dialog by the client telling the user the plugin failed to load.
	 * For normal case, if a plugin really failed to load because of an error, the correct return value is 1. */
}

/* Custom code called right before the plugin is unloaded */
void ts3plugin_shutdown() {

	if (pConf) {
		pConf->close();
		delete pConf;
		pConf = nullptr;
	}

	if(pluginID) {
		free(pluginID);
		pluginID = NULL;
	}
}

/****************************** Optional functions ********************************/
/*
 * Following functions are optional, if not needed you don't need to implement them.
 */

/* Tell client if plugin offers a configuration window. If this function is not implemented, it's an assumed "does not offer" (PLUGIN_OFFERS_NO_CONFIGURE). */
int ts3plugin_offersConfigure() {
	printf("PLUGIN: offersConfigure\n");
	/*
	 * Return values:
	 * PLUGIN_OFFERS_NO_CONFIGURE         - Plugin does not implement ts3plugin_configure
	 * PLUGIN_OFFERS_CONFIGURE_NEW_THREAD - Plugin does implement ts3plugin_configure and requests to run this function in an own thread
	 * PLUGIN_OFFERS_CONFIGURE_QT_THREAD  - Plugin does implement ts3plugin_configure and requests to run this function in the Qt GUI thread
	 */
	return PLUGIN_OFFERS_CONFIGURE_QT_THREAD;  /* In this case ts3plugin_configure does not need to be implemented */
}

/* Plugin might offer a configuration window. If ts3plugin_offersConfigure returns 0, this function does not need to be implemented. */
void ts3plugin_configure(void* handle, void* qParentWidget) {
	if (pConf == nullptr) {
		char path[128];
		ts3Functions.getConfigPath(path, 128);
		// Can use the qParentWidget pointer here, to make whatever window the client deems appropriate the parent of our config dialog
		//pConf = new config(QString::fromUtf8(path), static_cast<QWidget*>(qParentWidget));
		// or just ignore it and create it without a parent.
		pConf = new config(QString::fromUtf8(path));
	}
	if (pConf->isVisible()) {
		// Window is already displayed somewhere, bring it to the top and give it focus
		pConf->raise();
		pConf->activateWindow();
	}
	else {
		pConf->show();
	}
}

/*
 * If the plugin wants to use error return codes, plugin commands, hotkeys or menu items, it needs to register a command ID. This function will be
 * automatically called after the plugin was initialized. This function is optional. If you don't use these features, this function can be omitted.
 * Note the passed pluginID parameter is no longer valid after calling this function, so you must copy it and store it in the plugin.
 */
void ts3plugin_registerPluginID(const char* id) {
	const size_t sz = strlen(id) + 1;
	pluginID = (char*)malloc(sz * sizeof(char));
	_strcpy(pluginID, sz, id);  /* The id buffer will invalidate after exiting this function */
}

/* Plugin command keyword. Return NULL or "" if not used. */
const char* ts3plugin_commandKeyword() {
	return NULL;
}

/* Plugin processes console command. Return 0 if plugin handled the command, 1 if not handled. */
int ts3plugin_processCommand(uint64 serverConnectionHandlerID, const char* command) {
	return 0;  /* Plugin handled command */
}

/* Required to release the memory for parameter "data" allocated in ts3plugin_infoData and ts3plugin_initMenus */
void ts3plugin_freeMemory(void* data) {
	free(data);
}

/*
 * Plugin requests to be always automatically loaded by the TeamSpeak 3 client unless
 * the user manually disabled it in the plugin dialog.
 * This function is optional. If missing, no autoload is assumed.
 */
int ts3plugin_requestAutoload() {
	return 0;  /* 1 = request autoloaded, 0 = do not request autoload */
}

/* Helper function to create a menu item */
static struct PluginMenuItem* createMenuItem(enum PluginMenuType type, int id, const char* text, const char* icon) {
	struct PluginMenuItem* menuItem = (struct PluginMenuItem*)malloc(sizeof(struct PluginMenuItem));
	menuItem->type = type;
	menuItem->id = id;
	_strcpy(menuItem->text, PLUGIN_MENU_BUFSZ, text);
	_strcpy(menuItem->icon, PLUGIN_MENU_BUFSZ, icon);
	return menuItem;
}

/* Some makros to make the code to create menu items a bit more readable */
#define BEGIN_CREATE_MENUS(x) const size_t sz = x + 1; size_t n = 0; *menuItems = (struct PluginMenuItem**)malloc(sizeof(struct PluginMenuItem*) * sz);
#define CREATE_MENU_ITEM(a, b, c, d) (*menuItems)[n++] = createMenuItem(a, b, c, d);
#define END_CREATE_MENUS (*menuItems)[n++] = NULL; assert(n == sz);

/*
 * Menu IDs for this plugin. Pass these IDs when creating a menuitem to the TS3 client. When the menu item is triggered,
 * ts3plugin_onMenuItemEvent will be called passing the menu ID of the triggered menu item.
 * These IDs are freely choosable by the plugin author. It's not really needed to use an enum, it just looks prettier.
 */
enum {
	MENU_ID_GLOBAL_1 = 1,
	MENU_ID_CLIENT_1
};

/*
 * Initialize plugin menus.
 * This function is called after ts3plugin_init and ts3plugin_registerPluginID. A pluginID is required for plugin menus to work.
 * Both ts3plugin_registerPluginID and ts3plugin_freeMemory must be implemented to use menus.
 * If plugin menus are not used by a plugin, do not implement this function or return NULL.
 */
void ts3plugin_initMenus(struct PluginMenuItem*** menuItems, char** menuIcon) {
	/*
	 * Create the menus
	 * There are three types of menu items:
	 * - PLUGIN_MENU_TYPE_CLIENT:  Client context menu
	 * - PLUGIN_MENU_TYPE_CHANNEL: Channel context menu
	 * - PLUGIN_MENU_TYPE_GLOBAL:  "Plugins" menu in menu bar of main window
	 *
	 * Menu IDs are used to identify the menu item when ts3plugin_onMenuItemEvent is called
	 *
	 * The menu text is required, max length is 128 characters
	 *
	 * The icon is optional, max length is 128 characters. When not using icons, just pass an empty string.
	 * Icons are loaded from a subdirectory in the TeamSpeak client plugins folder. The subdirectory must be named like the
	 * plugin filename, without dll/so/dylib suffix
	 * e.g. for "test_plugin.dll", icon "1.png" is loaded from <TeamSpeak 3 Client install dir>\plugins\test_plugin\1.png
	 */

	BEGIN_CREATE_MENUS(2);  /* IMPORTANT: Number of menu items must be correct! */
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_GLOBAL_1, "Open Settings", "manage.png");
	CREATE_MENU_ITEM(PLUGIN_MENU_TYPE_CLIENT,  MENU_ID_CLIENT_1,  "Add Receiver",  "select.png");
	END_CREATE_MENUS;  /* Includes an assert checking if the number of menu items matched */

	/*
	 * Specify an optional icon for the plugin. This icon is used for the plugins submenu within context and main menus
	 * If unused, set menuIcon to NULL
	 */
	*menuIcon = (char*)malloc(PLUGIN_MENU_BUFSZ * sizeof(char));
	_strcpy(*menuIcon, PLUGIN_MENU_BUFSZ, "t.png");

	/*
	 * Menus can be enabled or disabled with: ts3Functions.setPluginMenuEnabled(pluginID, menuID, 0|1);
	 * Test it with plugin command: /test enablemenu <menuID> <0|1>
	 * Menus are enabled by default. Please note that shown menus will not automatically enable or disable when calling this function to
	 * ensure Qt menus are not modified by any thread other the UI thread. The enabled or disable state will change the next time a
	 * menu is displayed.
	 */
	/* For example, this would disable MENU_ID_GLOBAL_2: */
	/* ts3Functions.setPluginMenuEnabled(pluginID, MENU_ID_GLOBAL_2, 0); */

	/* All memory allocated in this function will be automatically released by the TeamSpeak client later by calling ts3plugin_freeMemory */
}

/* Helper function to create a hotkey */
static struct PluginHotkey* createHotkey(const char* keyword, const char* description) {
	struct PluginHotkey* hotkey = (struct PluginHotkey*)malloc(sizeof(struct PluginHotkey));
	_strcpy(hotkey->keyword, PLUGIN_HOTKEY_BUFSZ, keyword);
	_strcpy(hotkey->description, PLUGIN_HOTKEY_BUFSZ, description);
	return hotkey;
}

/* Some makros to make the code to create hotkeys a bit more readable */
#define BEGIN_CREATE_HOTKEYS(x) const size_t sz = x + 1; size_t n = 0; *hotkeys = (struct PluginHotkey**)malloc(sizeof(struct PluginHotkey*) * sz);
#define CREATE_HOTKEY(a, b) (*hotkeys)[n++] = createHotkey(a, b);
#define END_CREATE_HOTKEYS (*hotkeys)[n++] = NULL; assert(n == sz);

/*
 * Initialize plugin hotkeys. If your plugin does not use this feature, this function can be omitted.
 * Hotkeys require ts3plugin_registerPluginID and ts3plugin_freeMemory to be implemented.
 * This function is automatically called by the client after ts3plugin_init.
 */
void ts3plugin_initHotkeys(struct PluginHotkey*** hotkeys) {
	/* Register hotkeys giving a keyword and a description.
	 * The keyword will be later passed to ts3plugin_onHotkeyEvent to identify which hotkey was triggered.
	 * The description is shown in the clients hotkey dialog. */
	BEGIN_CREATE_HOTKEYS(10);  /* Create 10 hotkeys. Size must be correct for allocating memory. */
	CREATE_HOTKEY("send_message1", "Send Message 1");
	CREATE_HOTKEY("send_message2", "Send Message 2");
	CREATE_HOTKEY("send_message3", "Send Message 3");
	CREATE_HOTKEY("send_message4", "Send Message 4");
	CREATE_HOTKEY("send_message5", "Send Message 5");
	CREATE_HOTKEY("send_message6", "Send Message 6");
	CREATE_HOTKEY("send_message7", "Send Message 7");
	CREATE_HOTKEY("send_message8", "Send Message 8");
	CREATE_HOTKEY("send_message9", "Send Message 9");
	CREATE_HOTKEY("send_message10", "Send Message 10");
	END_CREATE_HOTKEYS;

	/* The client will call ts3plugin_freeMemory to release all allocated memory */
}

void ts3plugin_onMenuItemEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID) {
	printf("PLUGIN: onMenuItemEvent: serverConnectionHandlerID=%llu, type=%d, menuItemID=%d, selectedItemID=%llu\n", (long long unsigned int)serverConnectionHandlerID, type, menuItemID, (long long unsigned int)selectedItemID);
	switch(type) {
		case PLUGIN_MENU_TYPE_GLOBAL:
			switch (menuItemID) {
					case MENU_ID_GLOBAL_1: {
						if(pConf == nullptr) {
							char path[128];
							ts3Functions.getConfigPath(path, 128);
							// Can use the qParentWidget pointer here, to make whatever window the client deems appropriate the parent of our config dialog
							//pConf = new config(QString::fromUtf8(path), static_cast<QWidget*>(qParentWidget));
							// or just ignore it and create it without a parent.
							pConf = new config(QString::fromUtf8(path));
						}
						if (pConf->isVisible()) {
							// Window is already displayed somewhere, bring it to the top and give it focus
							pConf->raise();
							pConf->activateWindow();
						}
						else {
							pConf->show();
						}
					}
					break;
				default:
					break;
			}
			break;
		case PLUGIN_MENU_TYPE_CLIENT:
			/* Client contextmenu item was triggered. selectedItemID is the clientID of the selected client */
			switch(menuItemID) {
					case MENU_ID_CLIENT_1: {
						char* nick = NULL;
						char* uid = NULL;

						ts3Functions.getClientVariableAsString(serverConnectionHandlerID, selectedItemID, CLIENT_NICKNAME, &nick);
						ts3Functions.getClientVariableAsString(serverConnectionHandlerID, selectedItemID, CLIENT_UNIQUE_IDENTIFIER, &uid);
						
						receivers_list_nick.push_back(nick);
						receivers_list_uid.push_back(uid);

						pConf->saveRecList(nick, uid);
					}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void send_message(int num) {
	int cbSelItem = pConf->comboBox_SelectedItem(num + 1);
	if (cbSelItem == -1) return;
	if (cbSelItem == 0) {
		if (msg_switch_enabled[num]) {
			if (switch_msg[num]) {
				if (ts3Functions.requestSendChannelTextMsg(ts3Functions.getCurrentServerConnectionHandlerID(), msg_msg[num], NULL, NULL) != ERROR_ok) {
					ts3Functions.logMessage("Error requesting send text message", LogLevel_ERROR, "HotkeyMessage", ts3Functions.getCurrentServerConnectionHandlerID());
				}
				switch_msg[num] = 0;
			}
			else {
				if (ts3Functions.requestSendChannelTextMsg(ts3Functions.getCurrentServerConnectionHandlerID(), msg_msgswitch[num], NULL, NULL) != ERROR_ok) {
					ts3Functions.logMessage("Error requesting send text message", LogLevel_ERROR, "HotkeyMessage", ts3Functions.getCurrentServerConnectionHandlerID());
				}
				switch_msg[num] = 1;
			}
		}
		else {
			if (ts3Functions.requestSendChannelTextMsg(ts3Functions.getCurrentServerConnectionHandlerID(), msg_msg[num], NULL, NULL) != ERROR_ok) {
				ts3Functions.logMessage("Error requesting send text message", LogLevel_ERROR, "HotkeyMessage", ts3Functions.getCurrentServerConnectionHandlerID());
			}
		}
		return;
	}
	strcpy_s(toUID, receivers_list_uid[(cbSelItem-1)].toStdString().c_str());
	toID = getClientIdByUniqueId(toUID);
	if (msg_switch_enabled[num]) {
		if (switch_msg[num]) {
			if (ts3Functions.requestSendPrivateTextMsg(ts3Functions.getCurrentServerConnectionHandlerID(), msg_msg[num], toID, NULL) != ERROR_ok) {
				ts3Functions.logMessage("Error requesting send text message", LogLevel_ERROR, "HotkeyMessage", ts3Functions.getCurrentServerConnectionHandlerID());
			}
			switch_msg[num] = 0;
		}
		else {
			if (ts3Functions.requestSendPrivateTextMsg(ts3Functions.getCurrentServerConnectionHandlerID(), msg_msgswitch[num], toID, NULL) != ERROR_ok) {
				ts3Functions.logMessage("Error requesting send text message", LogLevel_ERROR, "HotkeyMessage", ts3Functions.getCurrentServerConnectionHandlerID());
			}
			switch_msg[num] = 1;
		}
	}
	else {
		if (ts3Functions.requestSendPrivateTextMsg(ts3Functions.getCurrentServerConnectionHandlerID(), msg_msg[num], toID, NULL) != ERROR_ok) {
			ts3Functions.logMessage("Error requesting send text message", LogLevel_ERROR, "HotkeyMessage", ts3Functions.getCurrentServerConnectionHandlerID());
		}
	}
}

/* This function is called if a plugin hotkey was pressed. Omit if hotkeys are unused. */
void ts3plugin_onHotkeyEvent(const char* keyword) {
	int error;
	ts3Functions.getConnectionStatus(ts3Functions.getCurrentServerConnectionHandlerID(), &error);
	if (error != 4) return;
	int helper;
	sscanf_s(keyword, "send_message%d", &helper);

	for (int i = 1; i < 11; i++) {
		if (i == helper) {
			send_message(i - 1);
			break;
		}
	}
}

/* Called when recording a hotkey has finished after calling ts3Functions.requestHotkeyInputDialog */
void ts3plugin_onHotkeyRecordedEvent(const char* keyword, const char* key) {
	//if (!strcmp(key, "")) { pConf->setButtonText("Set Hotkey"); return; }
	//pConf->setButtonText(key);
	char msg[30] = "Hotkey set: ";
	strcat_s(msg, key);
	ts3Functions.printMessageToCurrentTab(msg);
	//ts3Functions.logMessage(key, LogLevel_DEBUG, "HotkeyMessage", ts3Functions.getCurrentServerConnectionHandlerID());
}
