# The Noctis Programming Language

## Overview
**Noctis** is a simple, safe, and explicit programming language inspired by C’s minimalism and Rust’s memory safety. It is designed to be beginner-friendly while enforcing strong guarantees about memory usage and program correctness.  

Noctis emphasizes **clarity, ownership, and explicitness**, avoiding hidden complexity like type inference or implicit conversions. Its syntax is intentionally minimal, making it approachable for small projects, systems-level experiments, and learning the foundations of safe programming.

## Roadmap
- [X] Language Specification
- [X] Lexer
- [ ] Error Handling
- [ ] Parser
- [ ] Semantic Analysis
- [ ] Intermediate Representation
- [ ] Code Generation
- [ ] Standard Library
- [ ] Tooling


## Goals
- **Simplicity**: C-like syntax with straightforward semantics.
- **Safety**: Rust-inspired ownership and borrowing rules prevent memory errors and race conditions.
- **Explicitness**: Types, mutability, and ownership must be declared, eliminating ambiguity.
- **Ease of Use**: A small, consistent feature set that is easy to learn and teach.



## Language Features
```fng
-----------------
-- Imports     --
-----------------
-- Import entire modules or specific symbols.
import math.tomb;
import math.tomb::{add, PI};

-----------------
-- Variables   --
-----------------
-- Declared with `let`, immutable by default.
-- Use `mut` to enable mutation.
let y: float = 3.14;
let mut x: int = 5;

-----------------
-- Functions   --
-----------------
-- Declared with `func`, parameters and return types are explicit.
-- Functions are private by default; use `export` to make public.
export func add(a: int, b: int) -> int {
    return a + b;
}

func main() -> unit {
    let mut i: int = 5;
    print(i);
}

--------------------
-- Control Flow   --
--------------------
-- Supports `if`, `else`, `while`, and C-style `for` loops.
if x == y {
    print("Equal");
} else {
    print("Not equal");
}

while i != 0 {
    print(i);
    i = i - 1;
}

for j: int = 0; j < 5; j = j + 1 {
    print(j);
}

----------------------
-- Memory Safety  --
----------------------
-- Ownership: variables own their values and are freed at end of scope.
-- Move semantics: passing a value transfers ownership.
-- Borrowing: references (`&`) allow safe read-only access.
-- Mutable borrowing: `&mut` enables modification with exclusivity.

-- Ownership transfer
func takes_ownership(s: string) -> unit {
    print(s);  -- Freed at end of scope
}

func main_ownership() -> unit {
    let s: string = "hello";
    takes_ownership(s);
    -- print(s);  -- ERROR: s was moved
}

-- Borrowing
func print_length(s: &string) -> unit {
    print(size(s));
}

func main_borrow() -> unit {
    let s: string = "hello";
    print_length(&s);  -- Borrowed, s still valid
    print(s);
}

-- Mutable Borrowing
func to_uppercase(s: &mut string) -> unit {
    s = uppercase(s);
}

func main_mut() -> unit {
    let mut s: string = "hello";
    to_uppercase(&mut s);
    print(s);  -- Safely modified
}

-- Error: multiple mutable borrows not allowed
let x: &mut string = &mut s;
let y: &mut string = &mut s;  -- ERROR

-------------------
-- Types         --
-------------------
-- Primitives: int, float, bool, string, unit (void)
-- Strongly typed, no implicit conversions

---------------------
-- Comments        --
---------------------
-- Single-line: --
(* Multi-line *)
