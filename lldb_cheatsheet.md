# LLDB Debugging Cheatsheet

## Basic Execution Control

| Command | Shortcut | Description |
|---------|----------|-------------|
| `continue` | `c` | Continue execution until next breakpoint |
| `step` | `s` | Step into function calls |
| `next` | `n` | Step over function calls (don't go inside) |
| `finish` | `f` | Step out of current function |
| `return` | `r` | Continue until function returns |

## Breakpoint Management

| Command | Shortcut | Description |
|---------|----------|-------------|
| `breakpoint set --file file.cpp --line 42` | `b file.cpp:42` | Set breakpoint at specific file:line |
| `breakpoint set --name functionName` | `b functionName` | Set breakpoint at function |
| `breakpoint list` | `br l` | List all breakpoints |
| `breakpoint delete 1` | `br del 1` | Delete breakpoint by ID |
| `breakpoint delete` | `br del` | Delete all breakpoints |
| `breakpoint enable 1` | `br en 1` | Enable breakpoint |
| `breakpoint disable 1` | `br dis 1` | Disable breakpoint |

## Variable Inspection

| Command | Shortcut | Description |
|---------|----------|-------------|
| `print variableName` | `p variableName` | Print variable value |
| `print *pointer` | `p *pointer` | Dereference and print pointer |
| `print variableName.size()` | `p variableName.size()` | Call methods on variables |
| `frame variable` | `fr v` | Show all local variables |
| `frame variable --all` | `fr v -a` | Show all variables (including registers) |

## Stack Navigation

| Command | Shortcut | Description |
|---------|----------|-------------|
| `bt` | `bt` | Show backtrace (call stack) |
| `frame select 2` | `fr s 2` | Switch to frame #2 in stack |
| `frame info` | `fr i` | Show current frame info |
| `up` | `up` | Move up one frame in stack |
| `down` | `down` | Move down one frame in stack |

## Memory Inspection

| Command | Shortcut | Description |
|---------|----------|-------------|
| `memory read address` | `x address` | Read memory at address |
| `memory read --size 4 --format x address` | `x/4x address` | Read 4 bytes in hex format |
| `memory read --size 1 --format c address` | `x/1c address` | Read as character |
| `memory write address value` | `mem w address value` | Write to memory |

## Thread Management

| Command | Shortcut | Description |
|---------|----------|-------------|
| `thread list` | `th l` | List all threads |
| `thread select 2` | `th s 2` | Switch to thread #2 |
| `thread continue` | `th c` | Continue current thread |
| `thread step-over` | `th n` | Step over in current thread |

## Advanced Commands

| Command | Shortcut | Description |
|---------|----------|-------------|
| `expression variableName = newValue` | `expr variableName = newValue` | Change variable value |
| `watchpoint set variable variableName` | `w s v variableName` | Set watchpoint on variable |
| `watchpoint list` | `w l` | List watchpoints |
| `process continue` | `pro c` | Continue process |
| `process kill` | `pro k` | Kill process |

## Useful Aliases

| Command | Description |
|---------|-------------|
| `settings set target.inline-breakpoint-strategy always` | Always break at inline functions |
| `settings set target.skip-prologue false` | Don't skip function prologues |
| `settings set target.language c++` | Set language to C++ |

## Conditional Breakpoints

| Command | Description |
|---------|-------------|
| `breakpoint set --file file.cpp --line 42 --condition "i > 10"` | Break only when condition is true |
| `breakpoint set --file file.cpp --line 42 --ignore-count 5` | Skip first 5 hits |

## Tips
- Use `help command` to get detailed help for any command
- Use `apropos keyword` to search for commands
- Use `settings show` to see current settings
- Use `command alias` to create custom shortcuts

## Common Workflow
1. `b file.cpp:line` - Set breakpoint
2. `run` - Start program
3. `n` - Step over
4. `s` - Step into
5. `f` - Step out
6. `p variable` - Inspect variables
7. `c` - Continue to next breakpoint