*This project has been created as part of the 42 curriculum by kriad.*

# get_next_line

## Description
`get_next_line` is a small C project that implements a function capable of reading a file descriptor **line by line**.  
Each call to `get_next_line(int fd)` returns the next line from `fd` (including the trailing `\n` if it exists). When there is nothing left to read (EOF) or an error occurs, it returns `NULL`.

**Goal:** learn how to use `read()`, handle buffers, manage memory safely, and keep leftover data between calls using a static “stash”.

---

## Instructions

### Requirements
- A C compiler (clang or gcc)
- Standard libc
- Project header: `get_next_line.h`
- Utilities: `get_next_line_utils.c` (with `ft_*` helpers)

### Compilation

cc -Wall -Wextra -Werror get_next_line.c get_next_line_utils.c

cc -Wall -Wextra -Werror -D BUFFER_SIZE=10 get_next_line_bonus.c get_next_line_utils_bonus.c

## Resources

### Classic references
- `man 2 read`
- POSIX documentation for `read()` and file descriptors
- 42 subject PDF for `get_next_line` (project requirements, return rules, bonus rules)

### AI usage (transparency)
AI (ChatGPT) was used for:
- chatgpt "i used it to review the project in general and helped me whith the memory leak "

## Algorithm (Detailed explanation & justification)

### Goal
`get_next_line(int fd)` must return the next line from a file descriptor on each call, including the `\n` when present, and return `NULL` on EOF or error.  
The main challenge is that `read()` returns an arbitrary number of bytes per call, so a line can be split across multiple reads.

### Chosen approach: persistent stash (static storage)
The implementation keeps a **static string called `stash`** that stores:
- unread leftover bytes from previous calls
- newly read bytes until a full line is available

This is necessary because once `read()` returns data, we must keep any remaining bytes (after the returned line) for the next call.

### Step-by-step algorithm
Each call follows three phases:

1) **Read & append (build the stash)**
- Allocate a temporary `buffer` of size `BUFFER_SIZE + 1`
- While the stash does not contain `\n`:
  - `read(fd, buffer, BUFFER_SIZE)`
  - stop if `read()` returns `0` (EOF) or `-1` (error)
  - null-terminate `buffer`
  - append `buffer` to `stash` (string join)

This ensures that `stash` contains either:
- at least one complete line (there is a `\n`), or
- only the final remaining content at EOF

2) **Extract one line**
- Find the index of the first `\n` (or end of string if there is no newline)
- Return a substring from `stash[0]` to that position (including `\n` when present)

3) **Clean the stash (keep leftovers)**
- Remove the extracted line from the stash
- Duplicate the remaining part (after `\n`) into a new stash
- Free the old stash
- If nothing remains, free and set stash to `NULL`

### Why this algorithm is a good fit
- **Correctness with partial reads:** lines can be split across reads; the stash makes the function behave consistently.
- **Performance:** reading chunks of size `BUFFER_SIZE` reduces the number of syscalls compared to reading one character at a time.
- **Memory safety:** all intermediate allocations are freed; the stash persists only as long as needed.
- **Clear behavior at EOF:** if there is leftover content without `\n`, it is returned as the last line; the next call returns `NULL`.

### Complexity (informal)
- Reading is done in chunks of `BUFFER_SIZE`.
- The total work is proportional to the amount of data read.
- Note: repeated `ft_strjoin` can cause extra copying for very large lines, but this trade-off keeps the implementation simple and is acceptable for the project constraints.
