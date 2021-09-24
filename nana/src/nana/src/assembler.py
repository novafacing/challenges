import argparse
from pathlib import Path
from typing import Dict
import re


def subs(line: str, repl: Dict[str, str]) -> str:
    for i in repl:
        line = line.replace(i, repl[i])
    return line


def replace(line: str) -> str:
    return "🦍 " + re.sub(
        r"0x[0-9A-Fa-f]+($| )",
        lambda n: ("🍌 " * int(n.group(0), 16)).rstrip(),
        re.sub(
            r" [0-9]+ ",
            lambda n: " " + ("🍌 " * int(n.group(0))).rstrip(),
            subs(
                line,
                {
                    "push": "🤚",
                    "pop": "🤏",
                    "lsl": "💫",
                    "and": "✨",
                    "xor": "🖖",
                    "add": "👏",
                    "sub": "👟",
                    "swp": "🤸",
                    "jmp": "☝️",
                    "nz": "👀 💥",
                    "z": "👀 🚫",
                    "fwd": "⏩",
                    "bck": "⏪",
                },
            ),
        ),
    )


def run(infile: str, outfile: str) -> None:
    _if = Path(infile)

    if not _if.exists():
        print(f"{_if} does not exist.")
        exit(1)

    with open(_if, "r") as f:
        code = map(
            replace,
            filter(
                lambda l: l[0] != "#",
                map(lambda l: l.lstrip().rstrip(), f.read().splitlines()),
            ),
        )

    with open(outfile, "w") as f:
        for c in code:
            f.write(c)
            f.write("\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--infile", "-i", required=True, action="store", help="The file to assemble"
    )
    parser.add_argument(
        "--outfile", "-o", required=True, action="store", help="The file to assemble"
    )
    args = parser.parse_args()
    run(args.infile, args.outfile)
