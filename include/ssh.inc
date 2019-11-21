#if defined _ssh_included
	#endinput
#endif
#define _ssh_included

/**
 * Ban a player from using sprays.
 * 
 * @param client					Client index.
 * @param todelete					Delete the client's spray if it's there?
 * @error					Throw error if client is invalid/already spray banned.
 * @noreturn
 */
native void SSH_BanClient(int client, bool todelete = true);

/**
 * Unban a spray banned player from using sprays.
 * 
 * @param client					Client index.
 * @error					Throw error if client is invalid/not banned.
 * @noreturn
 */
native void SSH_UnbanClient(int client);

/**
 * Check if the client is spray banned.
 * 
 * @param client		Client index
 * @error					Throw error if client is invalid.
 * @return 				True if spray banned, false otherwise.
 */
native int SSH_IsBanned(int client);

/**
 * Called when a client gets spray banned.
 *
 * @param client		Client index
 *
 * @noreturn
 */
forward void SSH_OnBan(int client);

/**
 * Called when a client gets spray unbanned.
 *
 * @param client		Client index
 *
 * @noreturn
 */
forward void SSH_OnUnban(int client);

public SharedPlugin __pl_ranking = {
	name = "ssh",
	file = "ssh.smx",
	#if defined REQUIRE_PLUGIN
	required = 1,
	#else
	required = 0,
	#endif
};

public void __pl_ranking_SetNTVOptional() {
	MarkNativeAsOptional("SSH_BanClient");
	MarkNativeAsOptional("SSH_UnbanClient");
	MarkNativeAsOptional("SSH_IsBanned");
}