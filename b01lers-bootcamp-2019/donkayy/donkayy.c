#include <stdlib.h>
#include <stdio.h>

void get_out_of_my_swamp() {
	exit(0);
}

int main() {
	printf("Donkey: ");
	char * input = (char *)calloc(20, sizeof(char));
	scanf("%19s", input);
	if (input[0] != 'b') {
		get_out_of_my_swamp();
	}
	if (input[1] != '0') {
		get_out_of_my_swamp();
	}
	if (input[2] != 'c') {
		get_out_of_my_swamp();
	}
	if (input[3] != 't') {
		get_out_of_my_swamp();
	}
	if (input[4] != 'f') {
		get_out_of_my_swamp();
	}
	if (input[5] != '{') {
		get_out_of_my_swamp();
	}
	if (input[6] != 'h') {
		get_out_of_my_swamp();
	}
	if (input[7] != 'i') {
		get_out_of_my_swamp();
	}
	if (input[8] != '_') {
		get_out_of_my_swamp();
	}
	if (input[9] != 'p') {
		get_out_of_my_swamp();
	}
	if (input[10] != 'r') {
		get_out_of_my_swamp();
	}
	if (input[11] != '1') {
		get_out_of_my_swamp();
	}
	if (input[12] != 'n') {
		get_out_of_my_swamp();
	}
	if (input[13] != 'c') {
		get_out_of_my_swamp();
	}
	if (input[14] != '3') {
		get_out_of_my_swamp();
	}
	if (input[15] != 's') {
		get_out_of_my_swamp();
	}
	if (input[16] != 's') {
		get_out_of_my_swamp();
	}
	if (input[17] != '!') {
		get_out_of_my_swamp();
	}
	if (input[18] != '}') {
		get_out_of_my_swamp();
	}
	free(input);
	printf("Fiona: It talks!\n");
}
