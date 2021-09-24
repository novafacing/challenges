#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *deliver_msg = "{\" error \": \"Unknown CDN error\"}";
static const char *list_msg = "{\" error \": \"CDN contains no content at %p\"}";
static const char *choose_msg = "{\" error \": \"CDN contains no content to choose\"}";
static const char *banner =
    " ________ ________  ________  ________      \n"
    "|\\  _____\\\\   ____\\|\\   ___ \\|\\   ___  \\    \n"
    "\\ \\  \\__/\\ \\  \\___|\\ \\  \\_|\\ \\ \\  \\\\ \\  \\   \n"
    " \\ \\   __\\\\ \\  \\    \\ \\  \\ \\\\ \\ \\  \\\\ \\  \\  \n"
    "  \\ \\  \\_| \\ \\  \\____\\ \\  \\_\\\\ \\ \\  \\\\ \\  \\ \n"
    "   \\ \\__\\   \\ \\_______\\ \\_______\\ \\__\\\\ \\__\\\n"
    "    \\|__|    \\|_______|\\|_______|\\|__| \\|__|\n";
static const char *prompt =
    "\n1. List files.\n2. Choose files.\n3. Deliver files.\n4. Quit.\n";
static const char *bye = "Goodbye!\n";

void setup(void) {
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

void win(void) {
    size_t fs = 0;
    FILE *fp = fopen("fawn.jpg", "r");
    fseek(fp, 0, SEEK_END);
    fs = ftell(fp);
    rewind(fp);
    uint8_t *buf = (uint8_t *)calloc(fs, sizeof(uint8_t));
    fread(buf, sizeof(uint8_t), fs, fp);
    fclose(fp);
    fwrite(buf, sizeof(uint8_t), fs, stdout);
    free(buf);
    return;
}

void deliver(void) {
    size_t deliver_msg_len = strlen(deliver_msg);
    fwrite(deliver_msg, sizeof(char), deliver_msg_len, stdout);
    return;
}

int main(void) {
    setup();
    struct __attribute__((packed)) {
        char buf[0x10];
        void (*fptr)(void);
    } s;
    int opt = 0;
    s.fptr = deliver;
    printf("%s\n", banner);
    while (opt != 4) {
        printf("%s\ncmd> ", prompt);
        fgets(s.buf, sizeof(s.buf) + sizeof(void *) + 1, stdin);
        opt = atoi(s.buf);
        if (opt > 4 || opt < 1) {
            printf("Please choose a valid option!\n");
            continue;
        }
        switch (opt) {
            case 1:
                printf(list_msg, win);
                break;
            case 2:
                printf("%s", choose_msg);
                break;
            case 3:
                s.fptr();
                break;
            case 4:
                printf("%s", bye);
                exit(0);
                break;
        }
    }
}