#include <config.h>
#include "utils_helper.h"

char* ParseUrlFromData(char* buffer, int size) {
	static char result[ARR_LENGTH] = { 0 };
	memset(result, 0, ARR_LENGTH);
	
	char url[ARR_LENGTH] = { 0 };
	memcpy(url, buffer, size);

	int i = 0,
		j = 0,
		k = 0;
	int len = (int)strlen(url);

	while (i < len) {
		if (url[i] > 0 && url[i] <= 63)
			for (j = url[i], i++; j > 0; j--, i++, k++)
				result[k] = url[i];
		if (buffer[i] != 0) {
			result[k] = '.';
			k++;
		}
	}
	result[k] = '\0';

	return result;
}