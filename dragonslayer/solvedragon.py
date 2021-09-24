#!/usr/bin/env python2
from pwn import *
import string

def count_successes(check):
    p = process('./dragonslayer')
    p.sendline(check)
    
    result = p.recvall()
    print(result)

    count = len(result.split('\n'))
    return count

def check_successes(count, check):
    this_count = count_successes(check)
    print("{} vs {}".format(count,this_count))
    return count < this_count

curr_flag = 'b'

curr_count = count_successes(curr_flag) 

while True:
    for c in string.printable:
        temp_flag = curr_flag + c
        print(temp_flag)

        if check_successes(curr_count, temp_flag):
            curr_flag = temp_flag
            curr_count+= 1
            continue

        if curr_flag[-1] == '}':
            break
        print(curr_flag)
