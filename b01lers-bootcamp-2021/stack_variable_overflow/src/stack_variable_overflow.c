#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *password;
char *flag;

void read_file(const char *filename, char **buf) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    size_t fsize = ftell(fp);
    if (fsize == -1) {
        perror("ftell");
        exit(1);
    }
    rewind(fp);
    *buf = (char *)calloc(fsize + 1, sizeof(char));
    if (!buf) {
        perror("calloc");
        exit(1);
    }
    if (fread(*buf, sizeof(char), fsize, fp) != fsize) {
        perror("fread");
        exit(1);
    }
}

void setup(void) {
    read_file("password.txt", &password);
    read_file("flag.txt", &flag);
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

int main(void) {
    struct {
        char buf[128];
        uint8_t auth;
    } auth_state;
    setup();

    auth_state.auth = 0;
    memset(auth_state.buf, 0x0, sizeof(auth_state.buf));
    printf("Password: ");
    gets(auth_state.buf);

    if (memcmp(auth_state.buf, password, strlen(password)) == 0) {
        auth_state.auth = 1;
    }

    if (auth_state.auth != 0) {
        printf("Flag: %s\n", flag);
    } else {
        printf("Access denied.\n");
    }
}