#include "encode.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

void gingerbread_cookies() {
        dprintf(1, "A delicious plate of gingerbread men, just for you!\n");
        dprintf(1, "*distant screaming*\n");
}

void kid_cuisine() {
        dprintf(1, "Kid cuisine, now with Ogre Face Potatoes!\n");
        dprintf(1, "*sloshing noises*\n");
}

void animal_crackers() {
        dprintf(1, "Oh animal crackers, one of my favorites!\n");
        dprintf(1, "*neighing intensifies*\n");
}

void twinkies() {
        dprintf(1, "Y'all see zombieland? Yeah.\n");
        dprintf(1, "*groaning from the next table over*\n");
}

void mac_and_cheese() {
        dprintf(1, "Hot and delicious! Served right to you at the Far Far Away Cafe!\n");
}

void dump_binary() {
        FILE * fileptr = fopen("/home/shorty/bosswave", "rb");
        fseek(fileptr, 0, SEEK_END);
        long filelen = ftell(fileptr);
        rewind(fileptr);

        char * buffer = (char *)malloc((filelen + 1) * sizeof(char));
        fread(buffer, filelen, 1, fileptr);
        fclose(fileptr);
        size_t dump_size = 0;
        unsigned char * binary = base64_encode((unsigned char *)buffer, filelen, &dump_size);
        dprintf(1, "%s", binary);
}

void run_binary() {
        dprintf(1, "Run, Run, as fast as you can! You can't catch me, I'm the gingerbread man!\n");
        pid_t pid = fork();
        if (pid == 0) {
                execlp("/home/shorty/bosswave", "/home/shorty/bosswave", "/home/shorty/flag", (char *) NULL);
        } else if (pid < 0) {
                dprintf(1, "Error: could not start the boss wave!\n");
        } else {
                waitpid(pid, NULL, 0);
                dprintf(1, "\nOh...oh god...\n");
                fflush(stdout);
        }
}

int main() {
        unsigned short input;
        while (1) {
				input = 0;
                dprintf(1, "Welcome to the Far Far Away cafe! May I take your order number?\n");
				char input_buffer[8];
				while (input == 0) {
                	dprintf(1, "> ");
					memset(input_buffer, 0, sizeof(input_buffer));
					fgets(input_buffer, sizeof(input_buffer), stdin);
					input = atoi(input_buffer);
				}
                if ((input <= 4 && input >= 1) || (input <= 1338 && input >= 1336)) {
                        switch(input) {
                                case 1:         {
                                                                gingerbread_cookies();
                                                                break;
                                                        }
                                case 2:         {
                                                                kid_cuisine();
                                                                break;
                                                        }
                                case 3:         {
                                                                animal_crackers();
                                                                break;
                                                        }
                                case 4:         {
                                                                twinkies();
                                                                break;
                                                        }
                                case 1336:      {
                                                                dump_binary();
                                                                break;
                                                        }
                                case 1337:      {
                                                                mac_and_cheese();
                                                                break;
                                                        }
                                case 1338:      {
                                                                run_binary();
                                                                break;
                                                        }
                        }
                } else {
                        dprintf(1, "SIR, I MUST ASK YOU TAKE A LOOK AT OUR MENU BEFORE ORDERING!\n");
                        dprintf(1, "WE HAVE SOME SECRET MENU ITEMS AS WELL. HERE'S THE MENU:\n");
                        dprintf(1, "1. Gingerbread Cookies\n");
                        dprintf(1, "2. Kid Cuisine\n");
                        dprintf(1, "3. Animal Crackers\n");
                        dprintf(1, "4. Twinkies\n");
                }
        }


}
