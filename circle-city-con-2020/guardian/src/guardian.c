#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FLAGFILE "flag.txt"
const char *owl = "!WWWWWeeu..   ..ueeWWWWW!\n"
                  " \"$$(    R$$e$$R    )$$\"\n"
                  "  \"$8oeeo. \"*\" .oeeo8$\"\n"
                  "  .$$#\"\"\"*$i i$*\"\"\"#$$.\n"
                  "  9$\" @*c $$ $$F @*c $N\n"
                  "  9$  NeP $$ $$L NeP $$\n"
                  "  `$$uuuuo$$ $$uuuuu$$\"\n"
                  "  x$P**$$P*$\"$P#$$$*R$L\n"
                  " x$$   #$k #$F :$P` '#$i\n"
                  " $$     #$  #  $$     #$k\n"
                  "d$\"     '$L   x$F     '$$\n"
                  "$$      '$E   9$>      9$>\n"
                  "$6       $F   ?$>      9$>\n"
                  "$$      d$    '$&      8$\n"
                  "\"$k    x$$     !$k    :$$\n"
                  " #$b  u$$L      9$b.  $$\"\n"
                  " '#$od$#$$u....u$P$Nu@$\"\n"
                  " ..?$R)..?R$$$$*\"  #$P\n"
                  " $$$$$$$$$$$$$$@WWWW$NWWW\n"
                  " `````\"\"3$F\"\"\"\"#$F\"\"\"\"\"\"\"\n"
                  "        @$.... '$B\n"
                  "       d$$$$$$$$$$:\n"
                  "       ````````````\n";

char *getflag(void) {
    FILE *ffp = fopen(FLAGFILE, "r");
    struct stat st;
    fstat(fileno(ffp), &st);
    size_t fsz = st.st_size;
    char *flag = (char *)calloc(sizeof(char), fsz + 2);

    if (fread(flag, sizeof(char), fsz + 1, ffp) == 0) {
        free(flag);
        flag = NULL;
    } else {
        if (flag[fsz] == '\n') {
            flag[fsz] = '\x00';
        }
    }

    fclose(ffp);
    return flag;
}

void setup(void) {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

int main(void) {
    setup();
    char *flag = getflag();

    if (!flag) {
        exit(0);
    }

    size_t flaglen = strlen(flag);
    char *input = (char *)calloc(sizeof(char), flaglen + 2);

    if (!input) {
        exit(0);
    }

    printf("%s\n\nHOOOOOOOOOO Goes there? Do you have the password?\n> ", owl);
    if (fgets(input, flaglen + 1, stdin) == NULL) {
        exit(0);
    }
    size_t rd = strlen(input);

    if (input[rd] == '\n') {
        input[rd] = '\x00';
    }

    for (size_t i = 0; i < flaglen; i++) {
        if (input[i] == flag[i]) {
            printf("✅  ");
            if ((i + 1) % 8 == 0) {
                printf("\n");
            }
        } else {
            printf("\nHoo hoo hoo!\nThat is incorrect, my guardian.\n");
            exit(0);
        }
    }

    printf("\nWe will do our best.....you have fought well.\n");
}