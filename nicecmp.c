#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

#define LINELEN (80)

// submitted by Tom Mandel ID:205633688

char *mygets(char *buf, int len);
int mygeti();

int main(int argc, char *argv[])
{
	char *cmpstr[] = {"lexcmp", "lencmp"};
	int veclen = sizeof(cmpstr)/sizeof(char *);
	char str1[LINELEN + 1];
	char str2[LINELEN + 1];
	char res[LINELEN + 1];
	int index;

	/* Pipe file descriptors */
	int pass1[2];     // pass1= Parent to child 1
	int pass2[2]; //pass2= Parent to child 2
	int get[2]; //get= any child to Parent

	if (pipe(pass1) == -1)								/* Create pipe */
	return -2;
	if (pipe(pass2) == -1)
	return -2;
	if (pipe(get) == -1)
	return -2;

	// lexcmp child process
	int rc1 = fork();
	if(rc1==-1)
	return -2;

	if(rc1==0){
		if(close(pass1[1]) == -1)
		return -2;
		if(close(get[0]) == -1)
		return -2;

		if(dup2(pass1[0], 0) == -1)
		return -2;
		if(dup2(get[1], 1) == -1)
		return -2;

		char* myargs[3] = {"loopcmp", "lexcmp", NULL};
		if (execv(myargs[0],myargs) == -1)
		return -2;

		if (close(pass1[0]) == -1)
		return -2;
		if (close(get[1]) == -1)
		return -2;
	}
	// lencmp child process
	int rc2 = fork();
	if(rc2==-1)
	return -2;

	if(rc2==0){
		if(close(pass2[1]) == -1)
		return -2;
		if(close(get[0]) == -1)
		return -2;

		if(dup2(pass2[0], 0) == -1)
		return -2;
		if(dup2(get[1], 1) == -1)
		return -2;

		char* myargs[3] = {"loopcmp", "lencmp", NULL};
		if (execv(myargs[0],myargs) == -1)
		return -2;

		if (close(pass2[0]) == -1)
		return -2;
		if (close(get[1]) == -1)
		return -2;
	}

	while (1)
	{
		printf("Please enter first string:\n");
		if (mygets(str1, LINELEN) == NULL)
		break;
		printf("Please enter second string:\n");
		if (mygets(str2, LINELEN) == NULL)
		break;

		do {
			printf("Please choose:\n");
			for (int i=0 ; i < veclen ; i++)
			printf("%d - %s\n", i, cmpstr[i]);
			index = mygeti();
		} while ((index < 0) || (index >= veclen));

		printf("%s(%s, %s) == ", cmpstr[index], str1, str2);
		strcat(str1, "\n");
		strcat(str2, "\n");

		if (index == 0){
			if(write(pass1[1], str1, strlen(str1)) == -1)
			return -2;
			if(write(pass1[1], str2, strlen(str2)) == -1)
			return -2;
			if(read(get[0], &res, LINELEN) <0)
			return -2;

			printf("%d\n", atoi(res));
		}
		else if (index == 1) 				//will run lencmp
		{
			if(write(pass2[1], str1, strlen(str1)) == -1)
			return -2;
			if(write(pass2[1], str2, strlen(str2)) == -1)
			return -2;
			if(read(get[0], &res, LINELEN) <0)
			return -2;
			printf("%d\n", atoi(res));
		}
		fflush(stdout);
	}
	if (close(pass1[0]) == -1)
	return -2;
	if (close(pass1[1]) == -1)
	return -2;
	if (close(pass2[0]) == -1)
	return -2;
	if (close(pass2[1]) == -1)
	return -2;
	if (close(get[0]) == -1)
	return -2;
	if (close(get[1]) == -1)
	return -2;

	return 0;
}

char *mygets(char *buf, int len)
{
	char *retval;

	retval = fgets(buf, len, stdin);
	buf[len] = '\0';
	if (buf[strlen(buf) - 1] == 10) /* trim \r */
	buf[strlen(buf) - 1] = '\0';
	else if (retval)
	while (getchar() != '\n'); /* get to eol */

	return retval;
}

int mygeti()
{
	int ch;
	int retval=0;

	while(isspace(ch=getchar()));
	while(isdigit(ch))
	{
		retval = retval * 10 + ch - '0';
		ch = getchar();
	}
	while (ch != '\n')
	ch = getchar();
	return retval;
}
