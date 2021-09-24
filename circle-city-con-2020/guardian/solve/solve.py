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
