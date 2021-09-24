from pathlib import Path

if __name__ == "__main__":
    with open(Path(__file__).resolve().parents[3] / "solve" / "flag.txt", "rb") as f:
        flg = f.read().splitlines()[0].lstrip().rstrip()
        print("{", ", ".join(map(hex, flg)), "}")
