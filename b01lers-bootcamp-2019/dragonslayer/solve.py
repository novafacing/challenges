from pwn import *

index = 0
alph_ind_cur = 0
last_succ = 0
alphabet = list('abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()`~]_}[{\'\";:/?.>,<\|')
flag_cur = list('b' + alphabet[0] * 61);

while (flag_cur[-1] != '}'):
    # p = remote('13.82.57.100', 11032)
    p = process('./dragonslayer');
    print(p.recvline());

    p.sendline(''.join(flag_cur));

    outs = p.recvall();
    print(outs);
    succs = (len(outs.split()));
    if succs > last_succ:
        alph_ind_cur = 0
    last_succ = succs
    flag_cur[succs] = alphabet[alph_ind_cur];
    alph_ind_cur += 1;
    print(flag_cur)
