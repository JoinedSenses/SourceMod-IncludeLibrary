#if defined _advmotd_enabled
 #endinput
#endif
#define _advmotd_enabled

enum MOTDFailureReason {
	MOTDFailure_Unknown, // Failure reason unknown
	MOTDFailure_Disabled, // Client has explicitly disabled HTML MOTDs
	MOTDFailure_Matchmaking, // HTML MOTD is disabled by Quickplay/matchmaking (TF2 only)
	MOTDFailure_QueryFailed // cl_disablehtmlmotd convar query failed
};

typedef MOTDFailure = function void (int client, MOTDFailureReason reason);

/**
 * Displays an MOTD panel to a client with advanced options
 * 
 * @param client		Client index the panel should be shown to
 * @param title			Title of the MOTD panel (not displayed on all games)
 * @param msg			Content of the MOTD panel; could be a URL, plain text, or a stringtable index
 * @param type			Type of MOTD this is, one of MOTDPANEL_TYPE_TEXT, MOTDPANEL_TYPE_INDEX, MOTDPANEL_TYPE_URL, MOTDPANEL_TYPE_FILE
 * @param visible		Whether the panel should be shown to the client
 * @param big			true if this should be a big MOTD panel (TF2 only)
 * @param verify		true if we should check if the client can actually receive HTML MOTDs before sending it, false otherwise
 * @param callback		A callback to be called if we determine that the client can't receive HTML MOTDs
 * @noreturn
 */
stock void AdvMOTD_ShowMOTDPanel(int client, const char[] title, const char[] msg, int type=MOTDPANEL_TYPE_INDEX, bool visible=true, bool big=false, bool verify=false, MOTDFailure callback=INVALID_FUNCTION) {
	char connectmethod[32];
	if (verify && GetClientInfo(client, "cl_connectmethod", connectmethod, sizeof(connectmethod))) {
		if (StrContains(connectmethod, "quickplay", false) != -1 || StrContains(connectmethod, "matchmaking", false) != -1) {
			if (callback != INVALID_FUNCTION) {
				Call_StartFunction(null, callback);
				Call_PushCell(client);
				Call_PushCell(MOTDFailure_Matchmaking);
				Call_Finish();
			}
			
			return;
		}
	}
	
	KeyValues kv = new KeyValues("data");
	kv.SetString("title", title);
	kv.SetNum("type", type);
	kv.SetString("msg", msg);
	if (big) {
		kv.SetNum("customsvr", 1);
	}
	
	if (verify) {
		DataPack pack = new DataPack();
		pack.WriteCell(kv);
		pack.WriteCell(visible);
		
		if (callback != INVALID_FUNCTION) {
			Handle fwd = CreateForward(ET_Ignore, Param_Cell, Param_Cell);
			AddToForward(fwd, null, callback);
			pack.WriteCell(fwd);
		}
		else {
			pack.WriteCell(0);
		}
		
		QueryClientConVar(client, "cl_disablehtmlmotd", AdvMOTD_OnQueryFinished, pack);
	}
	else {
		ShowVGUIPanel(client, "info", kv, visible);
		CloseHandle(kv);
	}
}

public void AdvMOTD_OnQueryFinished(QueryCookie cookie, int client, ConVarQueryResult result, const char[] cvarName, const char[] cvarValue, DataPack pack) {
	pack.Reset();
	KeyValues kv = pack.ReadCell();
	bool visible = pack.ReadCell();
	Handle fwd = pack.ReadCell();
	delete pack;
	
	if (result != ConVarQuery_Okay || StringToInt(cvarValue)) {
		delete kv;
		
		if (fwd) {
			Call_StartForward(fwd);
			Call_PushCell(client);
			Call_PushCell((result != ConVarQuery_Okay) ? MOTDFailure_QueryFailed : MOTDFailure_Disabled);
			delete fwd;
		}
		return;
	}
	
	ShowVGUIPanel(client, "info", kv, visible);
	delete kv;
}
