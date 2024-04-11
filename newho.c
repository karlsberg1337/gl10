#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h>
#include <unistd.h>
#include <string.h>

void error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}

int main (int argc, char *argv[])
{
	char *phrase = argv[1]; 
	char *vars[] = {"RSS_FEED=https://ngs.ru/rss", NULL};
	FILE *f = fopen("stories.txt", "w"); 
	if (!f) {
		error("Error stories.txt!");
	}
	pid_t pid = fork();
	if (pid == -1) {
		error("Error cloning the process");
	}
	if (!pid) {
		if (dup2(fileno(f), 1) == -1) {
			error("Error standart input!");
		}
	
		if (execle("/usr/bin/python3", "/usr/bin/python3", "./rssgossip.py", phrase, NULL, vars) == -1) {
			error("Error start script");
		}
	}
	return 0;
}