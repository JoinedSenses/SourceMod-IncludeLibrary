/**
 * base64.inc - Base64 Encoding and Decoding Library Functions
 *
 * Version: 1.0.1
 * Date: 2009-08-27
 * Author: SirLamer
 *
 * Provides some basic Base64 encoding and decoding functions.
 * 
 * Adopted from Base64 AMX Mod X library function found here:
 * http://forums.alliedmods.net/showthread.php?t=87252
 * 
 * For an example application, check out the Jukebox for Source plugin, written by SirLamer:
 * http://forums.alliedmods.net/showthread.php?t=100537
 */

#if defined _Base64_included
	#endinput
#endif

#define _Base64_included

// The encoding table
static const char g_base64Table[] =
  // 0000000000111111111122222222223333333333444444444455555555556666
  // 0123456789012345678901234567890123456789012345678901234567890123
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// The decoding table
static const int g_decodeTable[] = {
//  0   1   2   3   4   5   6   7   8   9
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   //   0 -   9
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   //  10 -  19
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   //  20 -  29
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   //  30 -  39
    0,  0,  0,  62, 0,  0,  0,  63, 52, 53,  //  40 -  49
    54, 55, 56, 57, 58, 59, 60, 61, 0,  0,   //  50 -  59
    0,  0,  0,  0,  0,  0,  1,  2,  3,  4,   //  60 -  69
    5,  6,  7,  8,  9,  10, 11, 12, 13, 14,  //  70 -  79
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24,  //  80 -  89
    25, 0,  0,  0,  0,  0,  0,  26, 27, 28,  //  90 -  99
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38,  // 100 - 109
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48,  // 110 - 119
    49, 50, 51, 0,  0,  0,  0,  0,  0,  0,   // 120 - 129
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 130 - 139
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 140 - 149
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 150 - 159
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 160 - 169
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 170 - 179
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 180 - 189
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 190 - 199
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 200 - 209
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 210 - 219
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 220 - 229
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 230 - 239
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 240 - 249
    0,  0,  0,  0,  0,  0                    // 250 - 256
};

// For some reason the standard demands a string in 24-bit (3 character) intervals.
// This fill character is used to identify unused bytes at the end of the string.
#define FILL_CHAR '='

// The conversion characters between the standard and URL-compliance Base64 protocols
static const char g_mimeChars[] = "+/=";
static const char g_urlChars[] = "-_.";

/**
 * Encodes a string or binary data into Base64.
 * 
 * @param result            The storage buffer for the Base64-encoded result.
 * @param len               The maximum length of the storage buffer, in characters/bytes.
 * @param input             The input string or binary data to be encoded.
 * @param sourcelen         Optional number of characters or length in bytes to be read from the input source.
 *                          This is not needed for a text string, but is important for binary data since there
 *                          is no end-of-line character.
 * @return                  The length of the written Base64 string, in bytes.
 */
stock int EncodeBase64(char[] result, int len, const char[] input, int sourcelen=0) {
	// If the read length was specified, use it; otherwise, pull the string length from the input.
	int inputLen = sourcelen > 0 ? sourcelen : strlen(input); // The string length to be read from the input
	int resPos; // The string position in the result buffer

	// Loop through and generate the Base64 encoded string
	// NOTE: This performs the standard encoding process.  Do not manipulate the logic within this loop.
	for (int nPos = 0; nPos < inputLen; ++nPos) {
		int cCode = (input[nPos] >> 2) & 0x3f;

		resPos += FormatEx(result[resPos], len - resPos, "%c", g_base64Table[cCode]);

		cCode = (input[nPos] << 4) & 0x3f;
		if (++nPos < inputLen) {
			cCode |= (input[nPos] >> 4) & 0x0f;
		}

		resPos += FormatEx(result[resPos], len - resPos, "%c", g_base64Table[cCode]);

		if (nPos < inputLen) {
			cCode = (input[nPos] << 2) & 0x3f;

			if (++nPos < inputLen) {
				cCode |= (input[nPos] >> 6) & 0x03;
			}

			resPos += FormatEx(result[resPos], len - resPos, "%c", g_base64Table[cCode]);
		}
		else {
			++nPos;
			resPos += FormatEx(result[resPos], len - resPos, "%c", FILL_CHAR);
		}

		if (nPos < inputLen) {
			cCode = input[nPos] & 0x3f;
			resPos += FormatEx(result[resPos], len - resPos, "%c", g_base64Table[cCode]);
		}
		else {
			resPos += FormatEx(result[resPos], len - resPos, "%c", FILL_CHAR);
		}
	}

	return resPos;
}

/**
 * Decodes a Base64 string.
 * 
 * @param result            The storage buffer for the decoded text string or binary data.
 * @param len               The maximum length of the storage buffer, in characters/bytes.
 * @param input             The input string in compliant Base64 format to be decoded.
 * @return                  The length of the decoded data, in bytes.
 */
stock int DecodeBase64(char[] result, int len, const char[] input) {
	int inputLen = strlen(input); // The string length to be read from the input
	int resPos; // The string position in the result buffer

	// Loop through and generate the Base64 encoded string
	// NOTE: This performs the standard encoding process. Do NOT manipulate the logic within this loop.
	for (int nPos = 0; nPos < inputLen; nPos++) {
		int c = g_decodeTable[input[nPos++]];
		int c1 = g_decodeTable[input[nPos]];

		c = (c << 2) | ((c1 >> 4) & 0x3);

		resPos += FormatEx(result[resPos], len - resPos, "%c", c);

		if (++nPos < inputLen) {
			c = input[nPos];

			if (c == FILL_CHAR) {
				break;
			}

			c = g_decodeTable[input[nPos]];
			c1 = ((c1 << 4) & 0xf0) | ((c >> 2) & 0xf);

			resPos += FormatEx(result[resPos], len - resPos, "%c", c1);
		}

		if (++nPos < inputLen) {
			c1 = input[nPos];

			if (c1 == FILL_CHAR) {
				break;
			}

			c1 = g_decodeTable[input[nPos]];
			c = ((c << 6) & 0xc0) | c1;

			resPos += FormatEx(result[resPos], len - resPos, "%c", c);
		}
	}

	return resPos;
}

/**
 * Converts a standards-compliant Base64 string to the commonly accepted URL-compliant alternative.
 * 
 * Note: The result will be the same length as the input string as long as the output buffer is large enough.
 * 
 * @param result            The storage buffer for the URL-compliant result.
 * @param len               The maximum length of the storage buffer, in characters/bytes.
 * @param input             The standards-compliant Base64 input string to be converted.
 * @return                  Number of cells written.
 */
stock int Base64MimeToUrl(char[] result, int len, const char[] input) {
	int mimeCharsLen = sizeof(g_mimeChars); // Length of the two standards vs. URL character lists
	int inputLen = strlen(input); // The string length to be read from the input
	int tempChar; // Buffer character

	char[] temp = new char[inputLen+1]; // Buffer string

	// Loop through string
	for (int i = 0; i < inputLen; i++) {
		tempChar = input[i];

		for (int j = 0; j < mimeCharsLen; j++) {
			if (tempChar == g_mimeChars[j]) {
				tempChar = g_urlChars[j];
				break;
			}
		}

		temp[i] = tempChar;
	}

	temp[inputLen] = '\0';

	return strcopy(result, len, temp);
}

/**
 * Converts a URL-compliant Base64 string to the standards-compliant version.
 * 
 * Note: The result will be the same length as the input string as long as the output buffer is large enough.
 * 
 * @param result            The storage buffer for the standards-compliant result.
 * @param len               The maximum length of the storage buffer, in characters/bytes.
 * @param input             The URL-compliant Base64 input string to be converted.
 * @return                  Number of cells written.
 */
stock int Base64UrlToMime(char[] result, int len, const char[] input) {
	int mimeCharsLen = sizeof(g_mimeChars); // Length of the two standards vs. URL character lists
	int inputLen = strlen(input); // The string length to be read from the input
	int tempChar; // Buffer character

	char[] temp = new char[inputLen+1]; // Buffer string
	
	// Loop through string
	for (int i = 0; i < inputLen; i++) {
		tempChar = input[i];

		for (int j = 0; j < mimeCharsLen; j++) {
			if (tempChar == g_urlChars[j]) {
				tempChar = g_mimeChars[j];
				break;
			}
		}

		temp[i] = tempChar;
	}

	temp[inputLen] = '\0';

	return strcopy(result, len, temp);
}
