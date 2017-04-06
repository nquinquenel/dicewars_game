#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define INT_BUFFER_SIZE 10


char* concatint(char *s1, int n )
{
   

   char tempbuff[INT_BUFFER_SIZE];
   sprintf(tempbuff,"%d",n);
    char *finals = malloc(strlen(s1)+strlen(tempbuff)+3);//+1 for the zero-terminator
    strcpy(finals, s1);
    strcat(finals, tempbuff);
    strcat(finals,"\t");
    return finals;
}

int main(){
	int n=21351;
	char *s1 = {""};
	s1=concatint(s1,n);
	printf("%s",s1);
	
	
	printf("test");
}

