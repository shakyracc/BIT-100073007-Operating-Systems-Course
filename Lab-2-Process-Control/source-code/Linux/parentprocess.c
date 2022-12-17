#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

int main (int argc, char *argv[]) {

	struct timeval start, end, start2;

	pid_t pid;
	pid = fork();

	if(pid==0) {
		int sts_cd = execv("/home/shak/Documents/BIT-100073007-Operating-Systems-Course/Lab-2-Process-Control/Source-Code/Linux/childprocess", argv);
		printf("There is an issues with the running command\n");
		if (sts_cd == -1) {
			printf("execv error! \n");
			return 1;
		}
	}
	else if ( pid > 0 ) {
		gettimeofday(&start, NULL);
		printf("The child process start time is: %ld seconds %ld micro seconds\n", start.tv_sec, start.tv_usec);
		int wc = wait(NULL);
		gettimeofday(&end, NULL);
		printf("The child process end time is: %ld seconds %ld micro seconds\n", end.tv_sec, end.tv_usec);
		printf("The child process elapsed time is: %lds.%ldms\n", (end.tv_sec - start.tv_sec), (end.tv_usec - start.tv_usec) / 1000);
	}
	else {
		printf("Error while forking \n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

