#if defined _console_table_included
	#endinput
#endif
#define _console_table_included

#include <sourcemod>

/**
 * Formatting a orianted table for console output
 *
 * @param  sBuffer   String to be Formated
 * @param  maxlength size of sBuffer
 * @param  sValues   Table content, first dem symbols the row, second dem symbols the col
 * @param  rows      Count of the rows
 * @param  cols      Count of the cols
 *
 * @noreturn
 */
stock void MakeConsoleTable(char[] sBuffer, int maxlength, char[][][] sValues, int rows, int cols) {
	sBuffer[0] = '\0';

	int tabCount[cols];
	int len;
	int tab;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			len = strlen(sValues[i][j]);
			if (len > 15) {
				len -= 15;
				tab = 2;
				if (len > 0) {
					tab += (len / 8);
				}
			}
			else {
				tab = 1;
			}
			if (tab > tabCount[j]) {
				tabCount[j] = tab;
			}
		}
	}


	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			FormatEx(sBuffer, maxlength, "%s%s", sBuffer, sValues[i][j]);

			len = strlen(sValues[i][j]);
			if (len > 15) {
				len -= 15;
				tab = 2;
				if (len > 0) {
					tab += (len / 8);
				}

				tab = tabCount[j] - tab + 1;
			}
			else {
				tab = tabCount[j]
			}
			for (int k = 0; k < tab; k++) {
				Format(sBuffer, maxlength, "%s\t", sBuffer);
			}
		}
		Format(sBuffer, maxlength, "%s\n", sBuffer);
	}

}