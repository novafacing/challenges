#define BLOCK_COUNT 6
#define BLOCK_SIZE 8

typedef struct packet {
	char preamble[4];
	int count_a[1];
	char block_a[8];
	int count_b[1];
	char block_b[8];
	int count_c[1];
	char block_c[8];
	int count_d[1];
	char block_d[8];
	int count_e[1];
	char block_e[8];
	int count_f[1];
	char block_f[8];
	char postamble[4];
} packet;
