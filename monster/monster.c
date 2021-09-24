#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void scramble(char * flag) {
	for (int i = 0; i < strlen(flag) + 1; i++) {
		char temp;
		int first = rand() % (strlen(flag));
		int second = rand() % (strlen(flag));

		temp = flag[first];
		flag[first] = flag[second];
		flag[second] = temp;
	}
	return;
}

int main(int argc, char ** argv) {
	time_t t = time(NULL);
	srand(t);	
	char * flag = argv[1];
	scramble(flag);
	printf("%ld %s\n", t, flag);
	return 0;
}
