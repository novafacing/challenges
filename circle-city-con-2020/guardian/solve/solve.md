# Details

| Name       | guardian   |
| ---------- | ---------- |
| Category   | re         |
| Author     | novafacing |
| Difficulty | Baby       |

# Solution

We get a binary and, strangely enough, a remote. This is an RE challenge, so clearly there is something more to it. No exploits, but something will happen on the server (authors don't write remote challenges for no reason, it's extra work)!

If we run `checksec` on the binary, we'll see it has pretty good protections:

```    Arch:     amd64-64-little
    RELRO:    Full RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
    FORTIFY:  Enabled
```

Actually, it has full protections. If you found a pwn though, let me know!

Anyway, we'll open this one in Ghidra to make it truly BABY. We could do this one in GDB or objdump, but don't work harder! Work smarter.

We start off at `main`, of course. I've cleaned up some of the decompilation already so we can tell what we're dealing with (and for decreasing the length of the writeup).

```c
undefined8 main(void)

{
    long lVar1;
    char *__s;
    size_t sVar2;
    char *__s_00;
    char *pcVar3;
    ulong uVar4;
    long in_FS_OFFSET;
    
    lVar1 = *(long *)(in_FS_OFFSET + 0x28);
    setup();
    __s = (char *)getflag();
    if (__s != NULL) {
        sVar2 = strlen(__s);
        __s_00 = (char *)calloc(0x1,sVar2 + 0x2);
        if (__s_00 != NULL) {
            __printf_chk(0x1,"%s\n\nHOOOOOOOOOO Goes there? Do you have the password?\n> ",owl);
            pcVar3 = fgets(__s_00,(int)sVar2 + 0x1,stdin);
            if (pcVar3 != NULL) {
                if (sVar2 != 0x0) {
                    uVar4 = 0x0;
                    do {
                        while( true ) {
                            if (__s_00[uVar4] != __s[uVar4]) {
                                puts("\nHoo hoo hoo!\nThat is incorrect, my guardian.");
                                goto LAB_00101336;
                            }
                            uVar4 += 0x1;
                            __printf_chk(0x1,&DAT_0010200f);
                            if ((uVar4 & 0x7) != 0x0) break;
                            putchar(0xa);
                            if (uVar4 == sVar2) goto LAB_001012ff;
                        }
                    } while (uVar4 != sVar2);
                }
LAB_001012ff:
                puts("\nWe will do our best.....you have fought well.");
                if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
                    return 0x0;
                }
                    /* WARNING: Subroutine does not return */
                __stack_chk_fail();
            }
        }
    }
LAB_00101336:
                    /* WARNING: Subroutine does not return */
    exit(0x0);
}
```

So, a few things here. First, we call `setup`. This just does a couple `man setvbuf` calls to make sure our IO is unbuffered. Simple stuff.

Next, we call `getflag`:

```c

void * getflag(void)

{
    int __fildes;
    FILE *__stream;
    void *__ptr;
    size_t sVar1;
    void *pvVar2;
    long in_FS_OFFSET;
    stat sStack184;
    long local_20;
    
    local_20 = *(long *)(in_FS_OFFSET + 0x28);
    __stream = fopen("flag.txt","r");
    __fildes = fileno(__stream);
    __fxstat(0x1,__fildes,&sStack184);
    __ptr = calloc(0x1,sStack184.st_size + 0x2);
    sVar1 = fread(__ptr,0x1,sStack184.st_size + 0x1,__stream);
    if (sVar1 == 0x0) {
        pvVar2 = NULL;
        free(__ptr);
    }
    else {
        pvVar2 = __ptr;
        if (*(char *)(sStack184.st_size + (long)__ptr) == '\n') {
            *(char *)(sStack184.st_size + (long)__ptr) = '\0';
        }
    }
    fclose(__stream);
    if (local_20 == *(long *)(in_FS_OFFSET + 0x28)) {
        return pvVar2;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
}
```

This one is also pretty simple. We `fopen` the flag file, stat it to get the size of the file, then allocate a buffer of that size and read the file into it. We also overwrite a trailing newline with a nullbyte. Notice also that if fread fails (returns 0), we will return `NULL` here.

Ok, so the return value of `getflag` is actually the flag. How do we get it, though?

Well, we input it ourselves, actually:

```c
sVar2 = strlen(__s);
__s_00 = (char *)calloc(0x1,sVar2 + 0x2);
if (__s_00 != NULL) {
	__printf_chk(0x1,"%s\n\nHOOOOOOOOOO Goes there? Do you have the password?\n> ",owl);
	
	pcVar3 = fgets(__s_00,(int)sVar2 + 0x1,stdin);
```

Here, `pcVar3` is just a pointer to our input buffer, which was allocated as the length of the flag + 2. Finally, the meat of the program. We enter a loop:

```c
do {
	while( true ) {
		if (input[index] != flag[index]) {
			puts("\nHoo hoo hoo!\nThat is incorrect, my guardian.");
			goto LAB_00101336;
		}
		index += 0x1;
		__printf_chk(0x1,&DAT_0010200f);
		if ((index & 0x7) != 0x0) break;
		putchar(0xa);
		if (index == sVar2) goto LAB_001012ff;
	}
} while (index != sVar2);
```

Pretty straightforward. We loop from 0 to the length of the flag (remember from above?), and each time we check if the flag at that index and our input at that index are the same. If not, we say incorrect and exit and if so, we loop again after printing something (which turns out to be a check mark).

To solve this, we can just brute force the check! Because the program tells us once we have the next letter correct, we can continue checking character by character until we get the flag! This reduces the complexity of guessing a 64-character string (a TON) down to just $$64 \cdot 255$$ Quite manageable!


So we'll brute force it with something like this:

```python
from pwn import *

index = 0
alph_ind_cur = 0
last_succ = 0
alphabet = list(
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()`~]_}[{'\";:/?.>,<\|"
)
flag_cur = []
last_ct = 0
i = 0
while True:
    for c in alphabet:
        try:
            p = remote("localhost", 2000)
            attempt = "".join(flag_cur) + c
            print(attempt)
            p.recvuntil("\n> ")
            p.sendline(attempt)
            successes = str(p.recvuntil("Hoo"), "utf-8")
            print(successes)
            ct = successes.count("✅")
            if ct > last_ct:
                flag_cur.append(c)
                if c == "}":
                    print("Got flag: ", "".join(flag_cur))
                    break
                last_ct = ct

        except:
            print("Oopsie with attempt ", attempt)
        finally:
            p.close()
```

Not the prettiest thing in the world, but basically we just extend the string each time we receive an additional check mark in response (because as we've figured out, that means we got a char right). Eventually, we'll get the flag!