#include <stdio.h>
#include <string.h>
#include <ctype.h>

char * my_strcpy(char * str1, const char * str2) {
    char * p = str1;
    while (*str2) {
        *p = *str2;
        ++p, ++str2;
    }
    *p = 0;
    return str1;
}

int my_strncmp(const char * str1, const char * str2, int num) {
    while ((--num > 0) && *str1 && *str1 == *str2) {
        ++str1, ++str2;
    }
    return *str1 - *str2;
}

char * my_strcat(char * str1, const char * str2) {
    char * p = str1;
    while (*p) ++p;
    while (*str2) {
        *(p++) = *(str2++);
    }
    *p = 0;
    return str1;
}

char * my_strchr(char *str, int ch) {
    while (*str && *(str) != ch) ++str;
    if (!*str) return NULL;
    return str;
}

char * my_strstr (char *str1, char * str2) {
    while (*str1) {
        char *bgn1 = str1;
        char *bgn2 = str2;
        while (*str2 && *str1 == *str2) {
            ++str1, ++str2;
        }
        if (*str2 == 0) {
            return bgn1;
        }
        str1 = bgn1;
        str2 = bgn2;
        ++str1;
    }
    return NULL;
}

char * my_strrchr (char *str, int ch) {
    char *res = NULL;
    while (*str) {
        if (*str == ch) {
            res = str;
        }
        ++str;
    }
    return res;
}

char * my_strrstr (char *str1, char * str2) {
    char *res = NULL;
    while (*str1) {
        char *bgn1 = str1;
        char *bgn2 = str2;
        while (*str2 && *str1 == *str2) {
            ++str1, ++str2;
        }
        if (*str2 == 0) {
            res = bgn1;
        }
        str1 = bgn1;
        str2 = bgn2;
        ++str1;
    }
    return res;
}

const char * MY_STRCPY = "my_strcpy";
const char * MY_STRNCMP = "my_strncmp";
const char * MY_STRCAT = "my_strcat";
const char * MY_STRCHR = "my_strchr";
const char * MY_STRSTR = "my_strstr";
const char * MY_STRRCHR = "my_strrchr";
const char * MY_STRRSTR = "my_strrstr";

enum {
    MAX_CMD_SIZE = 20,
    MAX_STR_SIZE = 100
};

void str() {
    char cmd[MAX_CMD_SIZE];
    char str1[MAX_STR_SIZE];
    char str2[MAX_STR_SIZE];
        
    scanf("%s", cmd);
    if (!strcmp(cmd, MY_STRCPY)) {
        scanf("%s", str1);
        scanf("%s", str2);
        printf("MY_STRCPY\n");
        printf("str1::%s\n", str1);
        printf("str2::%s\n", str2);
        printf("RESULT::%s\n", my_strcpy(str1, str2));
    } else if (!strcmp(cmd, MY_STRNCMP)) {
        scanf("%s", str1);
        scanf("%s", str2);
        int num;
        scanf("%d", &num);

        printf("MY_STRNCMP\n");
        printf("str1::%s\n", str1);
        printf("str2::%s\n", str2);
        printf("num::%d\n", num);
        printf("RESULT:: %d\n", my_strncmp(str1, str2, num));
        printf("RESULT:: %d\n", strncmp(str1, str2, num));
        
    } else if (!strcmp(cmd, MY_STRCAT)) {
        scanf("%s", str1);
        scanf("%s", str2);

        printf("MY_STRCAT\n");
        printf("str1::%s\n", str1);
        printf("str2::%s\n", str2);
        printf("RESULT::%s\n", my_strcat(str1, str2));
    } else if (!strcmp(cmd, MY_STRCHR)) {
        scanf("%s", str1);
        char ch;
        scanf("\n%c", &ch); 

        printf("MY_STRCHR\n");
        printf("str1::%s\n", str1);
        printf("ch::%c\n", ch);
        printf("RESULT::%s\n", my_strchr(str1, ch));
    } else if (!strcmp(cmd, MY_STRSTR)) {
        scanf("%s", str1);
        scanf("%s", str2);
        printf("MY_STRSTR\n");
        printf("str1::%s\n", str1);
        printf("str2::%s\n", str2);
        printf("RESULT::%s\n", my_strstr(str1, str2));
    } else if (!strcmp(cmd, MY_STRRCHR)) {
        scanf("%s", str1);
        char ch;
        scanf("%c", &ch); 
        
        printf("MY_STRCHR\n");
        printf("str1::%s\n", str1);
        printf("ch::%c\n", ch);
        printf("RESULT::");
        
        printf("%s\n", my_strrchr(str1, ch));
    } else if (!strcmp(cmd, MY_STRRSTR)) {
        scanf("%s", str1);
        scanf("%s", str2);
        
        printf("MY_STRRSTR\n");
        printf("str1::%s\n", str1);
        printf("str2::%s\n", str2);
        printf("RESULT::%s\n", my_strrstr(str1, str2));
    }
}