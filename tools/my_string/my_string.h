//copy str2 to str1
char * my_strcpy(char * str1, const char * str2);

//cmp first num chars. 
int my_strncmp(char * str1, char * str2, int num);

//append str2 to str1
char * my_strcat(char * str1, const char * str2);

//return first ptr : *ptr == ch
char * my_strchr(char *str, int ch);

//return first ptr in str1: ptr[0] = str2[0] .. ptr[len(str2) - 1] == str2[len(str2) - 1]
char * my_strstr (char *str1, const char * str2);

//last ptr: *ptr == ch
char * my_strrchr (char *str, int ch);

//last strstr
char * my_strrstr (char *str1, char * str2);

void str();