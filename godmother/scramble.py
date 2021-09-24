inp = input()
print(", ".join(hex(ord(i) ^ 0x1a) for i in inp))
