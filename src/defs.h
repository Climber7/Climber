﻿//
// Created by Climber on 2020/6/3.
//

#ifndef CLIMBER_EVENT_H
#define CLIMBER_EVENT_H

#include <wx/defs.h>

enum {
    ID_MENU_STATUS = wxID_HIGHEST + 1,
    ID_MENU_TOGGLE,
    ID_MENU_PROXY_DIRECT_MODE,
    ID_MENU_PROXY_PAC_MODE,
    ID_MENU_PROXY_GLOBAL_MODE,
    ID_MENU_UPDATE_GFWLIST,
    ID_MENU_EDIT_USER_RULE,
    ID_MENU_SERVER_ITEM_START,
    ID_MENU_SERVER_ITEM_END = ID_MENU_SERVER_ITEM_START + 1000,
    ID_MENU_SERVERS_SETTINGS,
    ID_MENU_SERVERS_REFRESH,
    ID_MENU_PREFERENCES,
    ID_MENU_COPY_TERMINAL_PROXY_COMMAND_BASH,
    ID_MENU_COPY_TERMINAL_PROXY_COMMAND_CMD,
    ID_MENU_OPEN_CONFIG_DIRECTORY,
    ID_MENU_OPEN_LOG_DIRECTORY,
    ID_MENU_CHECK_FOR_UPDATES,
    ID_MENU_ABOUT,
    ID_MENU_EXIT,

    ID_FRAME_PREFERENCES,
    ID_FRAME_SERVERS_SETTINGS,
    ID_FRAME_ABOUT,

    ID_PREFERENCES_MAIN_TAB,
    ID_CHOICE_LANGUAGE,
    ID_CHECK_BOX_AUTO_START,
    ID_CHECK_BOX_SHARE_ON_LAN,
    ID_SPIN_SOCKS_PORT,
    ID_SPIN_HTTP_PORT,
    ID_SPIN_PAC_PORT,
    ID_TEXT_CTRL_PROXY_BYPASS,
    ID_BUTTON_APPLY_PROXY_SETTINGS,
    ID_BUTTON_CANCEL_PROXY_SETTINGS,
    ID_DATA_VIEW_SERVER_LIST,

    ID_MENU_NEW_SERVER_MANUALLY,
    ID_MENU_NEW_SERVER_URI,
    ID_MENU_NEW_SERVER_URI_FROM_CLIPBOARD,
    ID_MENU_NEW_SERVER_QR_CODE_ON_SCREEN,
    ID_MENU_NEW_SERVER_QR_CODE_FILE,
    ID_MENU_SHARE_SERVER,
    ID_MENU_EDIT_SERVER,
    ID_MENU_DELETE_SERVER,
    ID_MENU_DUPLICATE_SERVER,
    ID_MENU_MOVE_UP_SERVER,
    ID_MENU_MOVE_DOWN_SERVER,


};

#define PROXY_MODE_DIRECT ("direct")
#define PROXY_MODE_PAC ("pac")
#define PROXY_MODE_GLOBAL ("global")

#define DEFAULT_PROXY_MODE (PROXY_MODE_DIRECT)
#define DEFAULT_LANGUAGE ("en_US")
#define DEFAULT_SHARE_ON_LAN (false)
#define DEFAULT_SOCKS_PORT (1080)
#define DEFAULT_HTTP_PORT (8118)
#define DEFAULT_PAC_PORT (8119)
#ifdef CLIMBER_DARWIN
#define DEFAULT_PROXY_BYPASS (R"(FE80::/64
127.0.0.1
::1
FD00::/8
192.168.0.0/16
10.0.0.0/8
localhost
)")
#endif
#ifdef CLIMBER_WINDOWS
#define DEFAULT_PROXY_BYPASS (R"(localhost
127.*
10.*
192.168.*
)")
#endif


#endif //CLIMBER_EVENT_H
