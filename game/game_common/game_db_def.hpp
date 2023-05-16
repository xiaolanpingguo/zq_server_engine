#pragma once


#include <string>


namespace zq{


// ------------mongo define begin-------------

#define DB_NAME "zq"

// account
#define COL_ACCOUNT "account"
#define ACCOUNT_KEY_INDEX "_id"
#define ACCOUNT_KEY_SDK_CHANNEL_ID "sdk_channel_id"
#define ACCOUNT_KEY_SDK_USER_ID "sdk_user_id"
#define ACCOUNT_KEY_PROFILE_ID "profile_id"
#define ACCOUNT_KEY_BIN "bin"

// player
#define COL_PLAYER "player"
#define PLAYER_KEY_INDEX "user_id"
#define PLAYER_KEY_SVRID "svr_id"
#define PALYER_KEY_BIN "bin"


// nickname
#define COL_PLAYER_NAME "player_name"
#define PLAEYR_NAME_KEY_INDEX "_id"

// ------------mongo define  end-------------



// ------------redis define begin-------------

// user session
#define RD_USER_SESSION_KEY(USER_ID) std::string("user_session@") + USER_ID


#define RD_Z_ZONE_LOAD_KEY "z_zone_servers_load" 


// ------------redis define end-------------

}

