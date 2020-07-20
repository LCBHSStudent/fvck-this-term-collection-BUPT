#pragma once
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>

#define PRN_ERRMSG_RETURN printf

/*
 * @prarms：        title
 *                      配置文件中一组数据的标识
 *                  key
 *                      这组数据中要读出的值的标识
 *                  filename
 *                      要读取的文件路径
 * @return：        有效字符串 / NULL
 */
char* GetIniKeyString(
	const char* title,
	const char* key,
	const char* filename
);

/*
 * @params：        title
 *                      配置文件中一组数据的标识
 *                  key
 *                      这组数据中要读出的值的标识
 *                  filename
 *                      要读取的文件路径
 * @return：         有效INT值 / 0
 */
int GetIniKeyInt(
	const char* title,
	const char* key,
	const char* filename
);

/*
 * @params：        title
 *                      配置文件中一组数据的标识
 *                  key
 *                      这组数据中要读出的值的标识
 *                  val
 *                      更改后的值
 *                  filename
 *                      要读取的文件路径
 * @return：         success: 0 / failed: -1
 */
int PutIniKeyString(
	const char* title,
	const char* key,
	const char* val,
	const char* filename
);

/*
 * @params：        title
 *                      配置文件中一组数据的标识
 *                  key
 *                      这组数据中要读出的值的标识
 *                  val
 *                      更改后的值
 *                  filename
 *                      要读取的文件路径
 * @return：         success: 0 / failed: -1
 */
int PutIniKeyInt(
	const char* title,
	const char* key,
	int			val,
	const char* filename
);
