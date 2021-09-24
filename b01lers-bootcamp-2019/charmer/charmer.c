#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "packet.h"
#define HOSTNAME "127.0.0.1"
#define PORT 1337

char * get_msg() {
	size_t size = 48;
	char * str = (char *)malloc(size * sizeof(char));
	memset(str, 0, size);
	scanf("%42s", str);
	return str;
}

packet * init_packet() {
	packet * p = (packet *)malloc(sizeof(packet));
	char * blocks[BLOCK_COUNT] = {p->block_a, p->block_b, p->block_c, p->block_d, p->block_e, p->block_f};
	int * counts[BLOCK_COUNT] = {p->count_a, p->count_b, p->count_c, p->count_d, p->count_e, p->count_f};
	for (int i = 0; i < BLOCK_COUNT; i++) memset(blocks[i], 0, BLOCK_SIZE);
	for (int i = 0; i < BLOCK_COUNT; i++) counts[i][0] = 0;
	strcpy(p->preamble, "1234");
	strcpy(p->postamble, "1234");
	return p;
}

int fill_packet(packet * p, char * msg) {
	size_t msg_size = strlen(msg);
	size_t block_size = msg_size / BLOCK_COUNT;
	size_t leftover = msg_size % BLOCK_COUNT;
	if (!block_size && !leftover) {
		return 0;
	}
	char * blocks[BLOCK_COUNT] = {p->block_a, p->block_b, p->block_c, p->block_d, p->block_e, p->block_f};
	int * counts[BLOCK_COUNT] = {p->count_a, p->count_b, p->count_c, p->count_d, p->count_e, p->count_f};
	for (int block_idx  = 0; block_idx < block_size; block_idx++) {
		for (int block_num = 0; block_num < BLOCK_COUNT; block_num++) {
			blocks[block_num][block_idx] = *msg++;
			counts[block_num][0]++;
		}
	}
	for (int block_num = 0; block_num < leftover; block_num++) {
		blocks[block_num][block_size] = *msg++;
		counts[block_num][0]++;
	}
	return 1;
}

void print_packet(packet * p) {
	int * counts[BLOCK_COUNT] = {p->count_a, p->count_b, p->count_c, p->count_d, p->count_e, p->count_f};
	char * blocks[BLOCK_COUNT] = {p->block_a, p->block_b, p->block_c, p->block_d, p->block_e, p->block_f};
	for (int i = 0; i < BLOCK_COUNT; i++) {
		printf("%d\n", counts[i][0]);
		printf("%s\n", blocks[i]);
	}
}

int init_sock(char * host, int port) {
	struct sockaddr_in socket_addr;
	memset(&socket_addr, 0, sizeof(socket_addr));

	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons((unsigned short) port);

	struct hostent * ptrh = gethostbyname(host);
	if (ptrh == NULL) {
		fprintf(stderr, "Error: host %s not found.\n", host);
		exit(1);
	}

	memcpy(&socket_addr.sin_addr, ptrh->h_addr, ptrh->h_length);

	struct protoent * ptrp = getprotobyname("tcp");
	if (ptrp == NULL) {
		fprintf(stderr, "Error: protocol tcp not found.\n");
		exit(1);
	}

	int sock = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
	if (sock < 0) {
		fprintf(stderr, "Error: failed to create socket.\n");
		exit(1);
	}

	if (connect(sock, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) < 0) {
		fprintf(stderr, "Error: failed to connect to server %s:%d\n", host, port);
		exit(1);
	}

	return sock;
}

void write_msg(int sock, packet * p) {
	int * counts[BLOCK_COUNT] = {p->count_a, p->count_b, p->count_c, p->count_d, p->count_e, p->count_f};
	char * blocks[BLOCK_COUNT] = {p->block_a, p->block_b, p->block_c, p->block_d, p->block_e, p->block_f};
	write(sock, p->preamble, 4 * sizeof(char));
	for (int i = 0; i < BLOCK_COUNT; i++) {
		write(sock, counts[i], 1 * sizeof(char));
		write(sock, blocks[i], strlen(blocks[i]));
	}
	write(sock, p->postamble, 4 * sizeof(char));
}

int main() {
	char * msg = get_msg();
	packet * p = init_packet();
	if (fill_packet(p, msg)) {
		printf("Message accepted!\n");
		//print_packet(p);
	}
	int sockfd = init_sock(HOSTNAME, PORT);
	write_msg(sockfd, p);
	free(msg);
}
