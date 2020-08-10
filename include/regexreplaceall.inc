//	#include <sourcemod>
//	#include <regex>

//	public void OnPluginStart() {
//		char originaltext[128] = "This is a test";
//		char[] search = "(test)";
//		char[] replace = "big \\1";

//		RegexReplaceAll(originaltext, sizeof(originaltext), search, replace);
//		PrintToChatAll("%s", originaltext);
//	}


//	void RegexReplaceAll(char[] text, int size1, const char[] search, const char[] repl, int size2) {
	
// Written by JoinedSenses
stock void RegexReplaceAll(char[] text, int size, const char[] search, const char[] repl) {
	RegexError errorcode;
	Regex re = new Regex(search, .errcode = errorcode);

	if (errorcode != REGEX_ERROR_NONE) {
		LogError("Malformed regex: %s", search);
		delete re;
		return;
	}

	int matchCount = re.MatchAll(text, errorcode);
	if (matchCount == 0) {
		delete re;
		return;
	}

	if (errorcode != REGEX_ERROR_NONE) {
		LogError("Regex error %i. Text: %s Search Pattern: %s", errorcode, text, search);
		delete re;
		return;
	}

	int captureCount = re.CaptureCount();

	char[][][] match = new char[matchCount][captureCount][128];

	for (int i = 0; i < matchCount; i++) {
		for (int j = 0; j < captureCount; j++) {
			re.GetSubString(j, match[i][j], 128, i);
		}
	}

	char replace[64];

	Regex regexCaptures = new Regex("\\\\\\d+", .errcode = errorcode);
	int capCount = regexCaptures.MatchAll(repl, errorcode);

	char buffer[8];
	for (int i = 0; i < matchCount; i++) {
		strcopy(replace, sizeof(replace), repl);

		if (capCount > 0) {
			for (int j = 0; j < capCount; j++) {
				regexCaptures.GetSubString(0, buffer, sizeof(buffer), j);

				int index = StringToInt(buffer[1]);
				if (index >= captureCount) {
					continue;
				}

				ReplaceString(replace, sizeof(replace), buffer, match[i][index]);
			}
		}
		
		ReplaceString(text, size, match[i][0], replace);
	}

	delete re;
	delete regexCaptures;
}
