use std::io::{self,stdin,BufRead};
use std::fs::File;
use std::path::Path;
use std::vec::Vec;
use std::marker::Copy;
use std::fmt;
use std::env;

const PLACE: &str = "🤚";
const GRAB: &str = "🤏";
const SPIN: &str = "💫";
const MASH: &str = "✨";
const CLAP: &str = "👏";
const RUN: &str = "👟";
const JUMP: &str = "☝️";
const PICK: &str = "🖖";
const FLIP: &str = "🤸";

const SEES: &str = "👀";
const ACTION: &str = "💥";
const NOTHING: &str = "🚫";
const FORWARD: &str = "⏩";
const BACKWARD: &str = "⏪";

const MONKEY: &str = "🦍";
const NANA: &str = "🍌";
const FORENSICS_FLAG: &str = "CCC{h4m_w45_th3_0r1g1n4l_p0dm4n_4str0n4ut}";

#[derive(Copy, Clone)]
enum Operation {
    PUSH,   // "PLACE"
    POP,    // "GRAB"
    LSL,    // "SPIN"
    AND,   // "MASH"
    XOR,    // "PICK"
    ADD,    // "CLAP"
    SUB,    // "RUN"
    SWP,    // "FLIP"
    JMP     // "JUMP"
}

#[derive(Copy, Clone)]
enum Qualifier {
    Z,
    NZ
}

#[derive(Copy, Clone)]
enum Direction {
    FORWARD, 
    BACKWARD 
}

#[derive(Copy, Clone)]
struct Instr {
    op: Operation,
    qual: Option<Qualifier>,
    dir: Option<Direction>,
    op1: Option<u8>,
}

impl fmt::Display for Instr {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{} {} {} {}", self.opstr(), self.qualstr(), self.dirstr(), self.op1str())
    }
}

impl Instr {
    pub fn new(op: Operation, qual: Option<Qualifier>, direction: Option<Direction>, op1: Option<u8>) -> Instr {
        Instr {
            op: op,
            qual: qual,
            dir: direction,
            op1: op1,
        }
    }

    fn opstr(self: &Instr) -> String {
        return match self.op {
            Operation::PUSH => "PUSH",
            Operation::POP  => "POP",
            Operation::LSL  => "LSL",
            Operation::AND  => "AND",
            Operation::XOR  => "XOR",
            Operation::ADD  => "ADD",
            Operation::SUB  => "SUB",
            Operation::SWP  => "SWP",
            Operation::JMP  => "JMP"
        }.to_string();
    }

    fn qualstr(self: &Instr) -> String {
        return match self.qual {
            Some(Qualifier::Z)  => "Z",
            Some(Qualifier::NZ) => "NZ",
            None                => ""
        }.to_string();
    }

    fn dirstr(self: &Instr) -> String {
        return match self.dir {
            Some(Direction::FORWARD)    => "FWD",
            Some(Direction::BACKWARD)   => "BCK",
            None                        => ""

        }.to_string();
    }

    fn op1str(self: &Instr) -> String {
        match self.op1 {
            Some(u)         => u.to_string(),
            None            => "".to_string()
        }
    }

    fn do_pop(self: &Instr, cpu: &mut CPU) {
        match self.qual {
            // Qualifiers: pop the value above the bottom of the stack, copy value up
            Some(Qualifier::Z) => {
                if cpu.deref() == 0 {
                    cpu.tr = cpu.upop();
                }
            }
            Some(Qualifier::NZ) => {
                if cpu.deref() != 0 {
                    cpu.tr = cpu.upop();
                }
            }
            // No qualifier: pop the bottom off
            None => {
                cpu.tr = cpu.pop();
            }
        }
    }

    fn do_push(self: &Instr, cpu: &mut CPU) {
        match self.qual {
            Some(Qualifier::Z) => {
                match self.op1 {
                    Some(u) => {
                        if cpu.deref() == 0 {
                            let tmp = cpu.pop();
                            cpu.push(u);
                            cpu.push(tmp);
                        }
                    }
                    None => {
                        if cpu.deref() == 0 {
                            let tmp = cpu.pop();
                            cpu.push(cpu.tr);
                            cpu.push(tmp);
                        }
                    }
                }
            }
            Some(Qualifier::NZ) => {
                match self.op1 {
                    Some(u) => {
                        if cpu.deref() != 0 {
                            let tmp = cpu.pop();
                            cpu.push(u);
                            cpu.push(tmp);
                        }
                    }
                    None => {
                        if cpu.deref() != 0 {
                            let tmp = cpu.pop();
                            cpu.push(cpu.tr);
                            cpu.push(tmp);
                        }
                    }
                }
            }
            None => {
                match self.op1 {
                    Some(u) => {
                        cpu.push(u);
                    }
                    None => {
                        cpu.push(cpu.tr);
                    }
                }
            }
        }
    }

    fn do_norm(self: &Instr, cpu: &mut CPU, op: fn(u8, u8) -> u8) {
        match self.qual {
            Some(Qualifier::Z) => {
                if cpu.deref() == 0 {
                    let tmp = cpu.pop();
                    let val1 = cpu.pop();
                    let val2 = cpu.pop();
                    cpu.push(op(val1, val2));
                    cpu.push(tmp);
                }

            }
            Some(Qualifier::NZ) => {
                if cpu.deref() != 0 {
                    let tmp = cpu.pop();
                    let val1 = cpu.pop();
                    let val2 = cpu.pop();
                    cpu.push(op(val1, val2));
                    cpu.push(tmp);
                }
            }
            None => {
                let val1 = cpu.pop();
                let val2 = cpu.pop();
                cpu.push(op(val1, val2));
            }
        }

    }

    fn do_lsl(self: &Instr, cpu: &mut CPU) {
        self.do_norm(cpu, |a, b| a << (b as u32));
    }

    fn do_and(self: &Instr, cpu: &mut CPU) {
        self.do_norm(cpu, |a, b| a & b);
    }

    fn do_xor(self: &Instr, cpu: &mut CPU) {
        self.do_norm(cpu, |a, b| a ^ b);
    }

    fn do_add(self: &Instr, cpu: &mut CPU) {
        self.do_norm(cpu, |a, b| a + b);
    }

    fn do_sub(self: &Instr, cpu: &mut CPU) {
        self.do_norm(cpu, |a, b| a - b);

    }

    fn do_swp(self: &Instr, cpu: &mut CPU) {
        let a = cpu.pop();
        let b = cpu.pop();
        cpu.push(a);
        cpu.push(b);
    }

    fn do_jmp(self: &Instr, cpu: &mut CPU) {
        let top = cpu.deref();
        // Holy code reuse! I wrote this on
        // a plane and couldn't search how to return a 
        // Box::new(|o| cpu.jmp_fwd(o)) closure. Oh well!
        match self.qual {
            Some(Qualifier::NZ) => {
                if top != 0 {
                    match self.dir {
                        Some(Direction::FORWARD) => {
                            cpu.jmp_fwd(self.op1.unwrap());
                        }
                        Some(Direction::BACKWARD) => {
                            cpu.jmp_bck(self.op1.unwrap());
                        }
                        None => panic!("ERR: No Jump Direction")
                    }
                }
            }
            Some(Qualifier::Z) => {
                if top == 0 {
                    match self.dir {
                        Some(Direction::FORWARD) => {
                            cpu.jmp_fwd(self.op1.unwrap());
                        }
                        Some(Direction::BACKWARD) => {
                            cpu.jmp_bck(self.op1.unwrap());
                        }
                        None => panic!("ERR: No Jump Direction")
                    }
                }
            }
            None => {
                match self.dir {
                    Some(Direction::FORWARD) => {
                        cpu.jmp_fwd(self.op1.unwrap());
                    }
                    Some(Direction::BACKWARD) => {
                        cpu.jmp_bck(self.op1.unwrap());
                    }
                    None => panic!("ERR: No Jump Direction")
                }
            }
        }
    }

    fn execute(self: &Instr, cpu: &mut CPU) {
        match self.op {
            Operation::POP => self.do_pop(cpu),
            Operation::PUSH => self.do_push(cpu),
            Operation::LSL => self.do_lsl(cpu),
            Operation::AND => self.do_and(cpu),
            Operation::XOR => self.do_xor(cpu),
            Operation::ADD => self.do_add(cpu),
            Operation::SUB => self.do_sub(cpu),
            Operation::SWP => self.do_swp(cpu),
            Operation::JMP => self.do_jmp(cpu),
        }
    }
}

struct CPU {
    wsp: usize,
    wip: usize,
    code: Vec<Instr>,
    stack: [u8; CPU::MAX_STK + 1],
    tr: u8,
}

impl CPU {
    const MAX_STK: usize = 256;
    pub fn new(code: Vec<Instr>) -> CPU {
        CPU {
            wsp: CPU::MAX_STK,
            wip: 0,
            code: code,
            stack: [0; CPU::MAX_STK + 1],
            tr: 0,
        }
    }

    fn run(self: &mut CPU, dbg: bool) {
        while self.wip < self.code.len() {
            self.execute();
            if dbg {
                self.pstate();
            }
        }
    }

    fn pstate(self: &CPU) {
        println!("-----REGS------");
        println!("WSP: {} WIP: {}", self.wsp, self.wip);
        println!("TR: {}", self.tr);
        if self.wip < self.code.len() {
            println!("---CUR INSTR---");
            println!("{}", self.code[self.wip]);
        }
        println!("-----STACK-----");
        for i in self.wsp..CPU::MAX_STK {
            println!("{}: {}", i, self.stack[i]);
        }
        println!("");
    }

    fn jmp_fwd(self: &mut CPU, val: u8) {
        if self.wip + (val as usize) > self.code.len() {
            panic!("ERR: Cannot Execute Instruction");
        }
        self.wip += val as usize;
    }

    fn jmp_bck(self: &mut CPU, val: u8) {
        let tmp = self.wip;
        self.wip -= val as usize;
        if tmp < self.wip {
            panic!("ERR: Illegal Jump");
        }
    }

    fn push(self: &mut CPU, val: u8) {
        self.wsp -= 1;
        self.stack[self.wsp] = val;
    }

    fn execute(self: &mut CPU) {
        let cur = self.code[self.wip];
        if self.wip + 1 > self.code.len() {
            panic!("ERR: Cannot Execute Instruction");
        }
        self.wip += 1;
        cur.execute(self);
    }

    fn deref(self: &CPU) -> u8{
        if self.wsp < CPU::MAX_STK {
            return self.stack[self.wsp];
        } else {
            panic!("ERR: Out Of Bounds Dereference");
        }
    }

    fn upop(self: &mut CPU) -> u8 {
        if self.wsp < CPU::MAX_STK {
            let tmp = self.stack[self.wsp];
            self.wsp += 1;
            let top = self.stack[self.wsp];
            self.stack[self.wsp] = tmp;
            return top;
        } else {
            panic!("ERR: Out Of Bounds Operation");
        }

    }

    fn pop(self: &mut CPU) -> u8 {
        if self.wsp < CPU::MAX_STK {
            let top = self.stack[self.wsp];
            self.wsp += 1;
            return top;
        } else {
            panic!("ERR: Out Of Bounds Operation");
        }
    }

    fn dump_code(self: &CPU) {
        for instr in self.code.iter() {
            println!("{}", instr);
        }
    }
}

fn parseline(line: String) -> Instr {
    let tokens: Vec<&str> = line.split_whitespace().collect();

    if tokens.len() < 2 {
        panic!("ERR: Instruction Too Short");
    }

    // Every line starts with MONKEY
    if tokens[0] != MONKEY {
        panic!("ERR: Actor Must Be MONKEY");
    }

    // Monkey does what?
    let op = match tokens[1] {
        PLACE   => Operation::PUSH,
        GRAB    => Operation::POP,
        SPIN    => Operation::LSL,
        MASH    => Operation::AND,
        PICK    => Operation::XOR,
        CLAP    => Operation::ADD,
        RUN     => Operation::SUB,
        FLIP    => Operation::SWP,
        JUMP    => Operation::JMP,
        _ => panic!("ERR: Unknown Action")
    };

    if tokens.len() == 2 {
        return Instr::new(op, None, None, None);
    }

    // Yeah yeah...I know it's bad code and overpermissive
    // In my defense this is an RE chal, and I don't know rust
    if tokens.len() >= 4 {
        if tokens[2] == SEES {
            let qual = if tokens[3] == ACTION {
                Some(Qualifier::NZ)
            } else if tokens[3] == NOTHING {
                Some(Qualifier::Z)
            } else {
                None
            };

            let (s, direction) = if tokens.len() >= 6 && (tokens[4] == FORWARD || tokens[4] == BACKWARD) {
                (5, if tokens[4] == FORWARD { Some(Direction::FORWARD) } else { Some(Direction::BACKWARD) })
            } else {
                (4, None)
            };

            let op1 = if tokens.len() > s {
                Some(*&tokens[s..].iter().filter(|&t| *t == NANA).count() as u8)
            } else {
                None
            };

            return Instr::new(op, qual, direction, op1);
        } else {
            return Instr::new(op, None, None, Some(*&tokens[2..].iter().filter(|&t| *t == NANA).count() as u8));
        }
    } else {
        return Instr::new(op, None, None, Some(*&tokens[2..].iter().filter(|&t| *t == NANA).count() as u8));
    }
}

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if let Ok(lines) = read_lines(&args[1]) {
        let code = lines.map(|l| parseline(l.unwrap())).collect::<Vec<Instr>>();
        let mut cpu = CPU::new(code);
        let mut s = String::new();
        println!("⊂((〃’⊥’〃))⊃");
        stdin().read_line(&mut s).expect("ERR: Read Error");  

        cpu.run(false);
    } else {
        panic!("ERR: Parse Error")
    };

}