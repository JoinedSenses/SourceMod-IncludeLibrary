/**
 * gameME Plugin
 * http://www.gameme.com
 * Copyright (C) 2007-2016 TTS Oetzel & Goerz GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#if defined _gameme_included_
  #endinput
#endif
#define _gameme_included_


// raw message handling
#define RAW_MESSAGE_RANK				1
#define RAW_MESSAGE_PLACE				2
#define RAW_MESSAGE_KDEATH				3
#define RAW_MESSAGE_SESSION_DATA		4
#define RAW_MESSAGE_TOP10				5
#define RAW_MESSAGE_NEXT				6

// callbacks
#define RAW_MESSAGE_CALLBACK_PLAYER		101
#define RAW_MESSAGE_CALLBACK_TOP10		102
#define RAW_MESSAGE_CALLBACK_NEXT		103


forward void onGameMEStatsRank(int command, int client, char[] message_prefix, DataPack &datapack);
forward void onGameMEStatsPublicCommand(int command, int client, char[] message_prefix, DataPack &datapack);
forward void onGameMEStatsTop10(int command, int client, char[] message_prefix, DataPack &datapack);
forward void onGameMEStatsNext(int command, int client, char[] message_prefix, DataPack &datapack);


typedef gameMEStatsCallback = function void (int command, int payload, int client, DataPack &datapack);
/**
 * Query gameME Stats data from a client
 * 
 * @param request		Request type (only 'playerinfo' available)
 * @param client		Client index.
 * @param func			Callback function.
 * @param payload		Payload to distinct queries.
 * @noreturn
 */
native void QueryGameMEStats(char[] request, int client, gameMEStatsCallback callback, int payload = 0);


typedef gameMEStatsTop10Callback = function void (int command, int payload, DataPack &datapack);

/**
 * Query Top10 players from gameME Stats
 * 
 * @param request		Request type (currently only 'top10' available)
 * @param client		Client index.
 * @param func			Callback function.
 * @param payload		Payload to distinct queries.
 * @noreturn
 */
native void QueryGameMEStatsTop10(char[] request, int client, gameMEStatsTop10Callback callback, int payload = 0);


typedef gameMEStatsNextCallback = function void (int command, int payload, int client, DataPack &datapack);

/**
 * Query next players from gameME Stats for s specified client
 * 
 * @param request		Request type (currently only 'next' available)
 * @param client		Client index.
 * @param func			Callback function.
 * @param payload		Payload to distinct queries.
 * @noreturn
 */
native void QueryGameMEStatsNext(char[] request, int client, gameMEStatsNextCallback callback, int payload = 0);


typedef gameMEStatsIntCallback = function void (int command, int payload, int caller[MAXPLAYERS + 1], int target[MAXPLAYERS + 1], const char[] message_prefix, const char[] message);

/**
 * Query internal (preformatted) gameME Stats data
 * 
 * @param request		Request type.
 * @param client		Client index.
 * @param func			Callback function.
 * @param payload		Payload to distinct queries.
 * @noreturn
 */
native void QueryIntGameMEStats(char[] request, int client, gameMEStatsIntCallback callback, int payload = 0, int limit = 0);


/**
 * Display internal gameME Stats menu.
 *
 * @param client    	The client index the menu is displayed to.
 * @param time    		How long should the menu be displayed.
 * @param message    	The menu message.
 * @param need_handler  Define, if a menu handler is needed.
 * @noreturn
 */
native void DisplayGameMEStatsMenu(int client, int time, char[] message, int need_handler); 


/**
 * Color gameME entitites
 *
 * @param message	String.
 */
native void gameMEStatsColorEntities(char[] message); 


public SharedPlugin __pl_gameme =  {
	name = "gameme",
	file = "gameme.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
}

#if !defined REQUIRE_PLUGIN
public void __pl_gameme_SetNTVOptional() {
	MarkNativeAsOptional("QueryGameMEStats");
	MarkNativeAsOptional("QueryGameMEStatsTop10");
	MarkNativeAsOptional("QueryGameMEStatsNext");
	MarkNativeAsOptional("QueryIntGameMEStats");
	MarkNativeAsOptional("DisplayGameMEStatsMenu");
	MarkNativeAsOptional("gameMEStatsColorEntities");
}
#endif