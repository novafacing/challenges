# Details

| Name       | artform    |
| ---------- | ---------- |
| Category   | re         |
| Author     | novafacing |
| Difficulty | Baby       |

# Solution

This is a very straightforward reversing challenge. We can solve it with no specific tools using `objdump`, a command-line disassembler powered by `bfd`.

If we disassemble the main function: 

`objdump -d -Mintel ./artform` (`-d` => Disassemble executable sections, `-Mintel` => output in Intel format)

We get some code in the `.text` section. The `.text` section is where the real code of the program is stored, so that's usually the only place we need to focus for easy `re` challenges. The code is not particularly long, but I will paste in the important bit only. We scroll through the section briefly and notice immediately this sequence of instructions:

```asm
 mov    BYTE PTR [rbp-0x21],0x33
 mov    BYTE PTR [rbp-0x30],0x43
 mov    BYTE PTR [rbp-0x1a],0x5f
 mov    BYTE PTR [rbp-0x2c],0x31
 mov    BYTE PTR [rbp-0x16],0x35
 mov    BYTE PTR [rbp-0x17],0x75
 mov    BYTE PTR [rbp-0x27],0x33
 mov    BYTE PTR [rbp-0x25],0x74
 mov    BYTE PTR [rbp-0x19],0x62
 mov    BYTE PTR [rbp-0x1f],0x74
 mov    BYTE PTR [rbp-0x1c],0x68
 mov    BYTE PTR [rbp-0x2e],0x43
 mov    BYTE PTR [rbp-0x1b],0x33
 mov    BYTE PTR [rbp-0x26],0x5f
 mov    BYTE PTR [rbp-0x2b],0x5f
 mov    BYTE PTR [rbp-0x1d],0x74
 mov    BYTE PTR [rbp-0x14],0x7d
 mov    BYTE PTR [rbp-0x2f],0x43
 mov    BYTE PTR [rbp-0x1e],0x5f
 mov    BYTE PTR [rbp-0x20],0x34
 mov    BYTE PTR [rbp-0x28],0x6b
 mov    BYTE PTR [rbp-0x2d],0x7b
 mov    BYTE PTR [rbp-0x18],0x72
 mov    BYTE PTR [rbp-0x15],0x68
 mov    BYTE PTR [rbp-0x23],0x5f
 mov    BYTE PTR [rbp-0x22],0x62
 mov    BYTE PTR [rbp-0x29],0x31
 mov    BYTE PTR [rbp-0x2a],0x6c
 mov    BYTE PTR [rbp-0x24],0x30
```

Looks somewhat flaglike! We're storing a bunch of ASCII characters into an array, then we overwrite that array with all `A`s:

```asm
lea    rax,[rbp-0x30]
mov    edx,0x20
mov    esi,0x41
mov    rdi,rax
call   1090 <memset@plt>
```

Then finally, we print the array out:

```asm
lea    rax,[rbp-0x30]
mov    rsi,rax
lea    rdi,[rip+0xdac]        # 2008 <memset@plt+0xf78>
mov    eax,0x0
call   1080 <printf@plt>
```

There are two good ways to solve this, both extremely simple:

1. Write down the ASCII in the right order to get the flag.
2. Patch the binary to remove the `memset` and just run it.

Either solution will get us the right answer.