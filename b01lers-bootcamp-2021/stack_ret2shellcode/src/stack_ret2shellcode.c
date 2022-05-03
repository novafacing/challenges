#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void setup(void) {
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
    printf("shell %p > ", &auth_state.buf);
    fgets(auth_state.buf, sizeof(auth_state) + 0x20, stdin);

    if (memcmp(auth_state.buf, "friend", strlen("friend")) == 0) {
        auth_state.auth = 1;
    }

    if (auth_state.auth != 0) {
        printf("Access denied.\n");
    } else {
        printf("Access denied.\n");
    }
}