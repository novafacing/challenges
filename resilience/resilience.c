#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int in(char c, char * test) {
	for (int i = 0; i < strlen(test); i++) {
		if (c == test[i]) {
			return 1;
		}
	}
	return 0;
}

int keycheck(char * key) {
//format 1986-0134570-lsrat-slsstr
//4 numbers either 1 8, 9 or 6
//7 leetspeak characters (0, 1, 3, 4, 5, 7)
//5 letters (a, l, s, t, r)
//(slstsr)
	for (int i = 0; i < 4; i++) {
		if (!in(key[i], "1869")) {
			printf("Product Activation Failed. %c\n", key[i]);
			return 0;
		}
	}
	for (int i = 4; i < 12; i++) {
		if (!in(key[i], "013457")) {
			printf("Product Activation Failed. %c\n", key[i]);
			return 0;
		}
	}
	for (int i = 12; i < 18; i++) {
		if (!in(key[i], "alstr")) {
			printf("Product Activation Failed. %c\n", key[i]);
			return 0;
		}
	}
	if (strcmp(&key[18], "slstsr") != 0) {
		printf("Product Activation Failed. %s\n", &key[18]);
		return 0;
	}
	return 1;
}

int main(int argc, char ** argv) {
	char * key = argv[1];
	if (keycheck(key)) {
		printf("Product Activated.\n");
	}
}
