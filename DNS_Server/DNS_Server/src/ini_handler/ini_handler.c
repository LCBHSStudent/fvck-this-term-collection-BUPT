#include "ini_handler.h"

#pragma warning(disable:4996)
#define PRN_ERRMSG_RETURN printf

char* GetIniKeyString(
    const char* title,
    const char* key,
    const char* filename
) {
    FILE* fp = NULL;
    int     flag = 0;
    char    sTitle[32], *wTmp;
    static char sLine[1024];

    sprintf(sTitle, "[%s]", title);
    if (NULL == (fp = fopen(filename, "r"))) {
        perror("fopen");
        return NULL;
    }

    while (NULL != fgets(sLine, 1024, fp)) {
        // 这是注释行  
        if (0 == strncmp("//", sLine, 2)) continue;
        if ('#' == sLine[0])              continue;

        wTmp = strchr(sLine, '=');
        if ((NULL != wTmp) && (1 == flag)) {
            if (0 == strncmp(key, sLine, wTmp - sLine)) { // 长度依文件读取的为准  
                sLine[strlen(sLine) - 1] = '\0';
                fclose(fp);
                return wTmp + 1;
            }
        }
        else {
            if (0 == strncmp(sTitle, sLine, strlen(sLine) - 1)) { // 长度依文件读取的为准  
                flag = 1; // 找到标题位置  
            }
        }
    }
    fclose(fp);
    return NULL;
}

int GetIniKeyInt(
    const char* title,
    const char* key,
    const char* filename
) {
    return atoi(GetIniKeyString(title, key, filename));
}


int PutIniKeyString(
    const char* title,
    const char* key,
    const char* val,
    const char* filename
) {
    FILE* fpr, * fpw;
    int  flag = 0;
    char sLine[1024], sTitle[32], * wTmp;

    sprintf(sTitle, "[%s]", title);
    if (NULL == (fpr = fopen(filename, "r")))
        PRN_ERRMSG_RETURN("fopen");// 读取原文件  
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        PRN_ERRMSG_RETURN("fopen");// 写入临时文件  

    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // 如果找到要修改的那一行，则不会执行内部的操作  
            wTmp = strchr(sLine, '=');
            if ((NULL != wTmp) && (1 == flag)) {
                if (0 == strncmp(key, sLine, wTmp - sLine)) { // 长度依文件读取的为准  
                    flag = 2;// 更改值，方便写入文件  
                    sprintf(wTmp + 1, "%s\n", val);
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sLine) - 1)) { // 长度依文件读取的为准  
                    flag = 1; // 找到标题位置  
                }
            }
        }

        fputs(sLine, fpw); // 写入临时文件  
    }
    fclose(fpr);
    fclose(fpw);

    sprintf(sLine, "%s.tmp", filename);
    return rename(sLine, filename);// 将临时文件更新到原文件  
}


int PutIniKeyInt(
    const char* title,
    const char* key,
    int         val,
    const char* filename
) {
    char sVal[32];
    sprintf(sVal, "%d", val);
    return PutIniKeyString(title, key, sVal, filename);
}
