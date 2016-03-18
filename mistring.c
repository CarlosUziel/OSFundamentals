#include <stdlib.h>

int mi_strlen (char* str){
    int size=0,i;
    
    for (i = 0; (*(str+i) != '\0') ; i++)
    {
		size++;
    }
    return size;
}

char* mi_strcpy (char* s1, char* s2){
	
	int i;
	for (i = 0; (*(s2+i) != '\0') ; i++)
	{
		*(s1+i) = *(s2+i);	
	}
	*(s1+i) = '\0';
	return s1;
}

char* mi_strcat (char* s1, char* s2){
	char* s1_end = s1 + mi_strlen(s1)*sizeof(char);
	int i;
	for (i = 0; (*(s2+i) != '\0') ; i++)
	{
		*(s1_end+i) = *(s2+i);
	}
	*(s1_end+i) = '\0';
	return s1;
	
}

char* mi_strdup (char* str){
	char* d = malloc(mi_strlen(str)*sizeof(char) + 1);
	return mi_strcpy(d,str);	
}

int mi_strequals (char* s1, char* s2){
    if (mi_strlen(s1) != mi_strlen(s2))    
		return 0;	
	
	int i;
    for (i = 0; (*(s1+i) != '\0') ; i++)
	{
		if (*(s1+i) != *(s2+i))
			return 0;
				
	}
	return 1;	
}
