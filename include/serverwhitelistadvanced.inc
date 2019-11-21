/**
*	This forward is called whenever someone is kicked by the plugin ServerWhitelistAdvanced
*
*	This is called ONCE per kill, not once per assist.
*
*	@param	client			Client that is going to be kicked
*	@param	isFromBlacklist	true if the client being kicked because of the blacklist cache
*	@param	szSteamId		SteamId of the kicked client
*	@param	szIP			IP of the kicked client
*	
*	@return Action:Plugin_Continue to keep kicking the client, or another value to prevent the kick
*/
forward Action OnClientKickedPre_ServerWhitelistAdvanced(int client, bool isFromBlacklistCache, const char[] szSteamId, const char[] szIP);

//=====Pending
/**
* @return true if the client status concerning the whitelist is still pending (due to SteamGroups), false otherwise (meaning he'd then be whitelisted AND checked).
*/
native bool IsClientWhitelistStatusPending(int client);

//=====Is whitelisted
/**
* @return true if the SteamId is whitelisted, false otherwise
*/
native bool IsSteamIdWhitelisted(const char[] szSteamId, bool currentOnly);
/**
* @return true if the IP is whitelisted, false otherwise
*/
native bool IsIPWhitelisted(const char[] szSteamId, bool currentOnly);
/**
* @return true if the SteamGroupId is whitelisted, false otherwise
*/
native bool IsSteamGroupWhitelisted(int steamGroupId, bool currentOnly);

//=====Cache
/**
* Returns whether the steam id is whitelist or not.
*
* Whitelisted SteamIds are the ones being quickly accepted when someone join, because they did try to join before and were ok.
* The whitelist cache is used to prevent people from being checked at the Valve server (when using SteamGroups).
*
* @return true if the SteamId is cached in the whitelist, false otherwise.
*/
native bool IsSteamIdWhitelistCached(const char[] szSteamId);
/**
* Returns whether the steam id is blacklisted or not.
*
* Blacklisted SteamIds are the ones being quickly denied when someone join, because they did try to join before.
*
* @return true if the SteamId is cached in the blacklist, false otherwise.
*/
native bool IsSteamIdBlacklistCached(const char[] szSteamId);