

---

# Simple Shell Project

Welcome to our Simple Shell project, a foundational project that embarks on the recreation of a simple UNIX command line interpreter.

## Background

The project emphasizes:

- Basics of programming
- Basics of C language
- Basics of thinking like an engineer
- Group work dynamics
- The art and skill of learning to learn

You can read more about pair programming and its significance [here](https://intranet.alxswe.com/concepts/121).

## Learning Objectives

By the end of this project, participants would be knowledgeable in:

- The origin and pioneers of the Unix operating system and its shell.
- Working of a shell.
- PID and PPID fundamentals.
- Manipulating the environment of the current process.
- Differences between a function and a system call.
- Process creation.
- The three prototypes of `main`.
- Execution of programs using the `execve` system call.
- Process suspension until child process termination.
- EOF or "end-of-file".

## Requirements

- Editors: vi, vim, emacs.
- Compilation on Ubuntu 20.04 LTS using gcc with options `-Wall -Werror -Wextra -pedantic -std=gnu89`.
- Adherence to Betty coding style.
- No memory leaks.
- A maximum of 5 functions per file.
- Include guarded header files.
- Limit system calls usage.
- A `README.md` file for project description.
- An `AUTHORS` file listing contributors.

## Compilation

Compile the shell with:

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

## Testing

(Provide instructions for testing here)

## Tasks

### 1. Betty Coding Style

Ensure your code adheres to the Betty coding standards.

### 2. Simple Shell 0.1

A UNIX command line interpreter featuring:

- User prompt for command input.
- Error handling including non-existent executables.
- Handling of "end of file" condition (Ctrl+D).

### 3. Simple Shell 0.2

Enhanced version of 0.1 to handle:

- Command lines with arguments.

### 4. Simple Shell 0.3

Build upon 0.2 with:

- Handling the PATH.
- Conditional `fork` based on command existence.

### 5. Simple Shell 0.4

Expanding on 0.3 by introducing:

- `exit` built-in command.
- No arguments to be handled for the `exit` built-in.

### 6. Simple Shell 1.0

Incorporate the features from 0.4 and:

- Implement the `env` built-in to display the current environment.

## Collaborative Project

This is a group project. Please refer to the "Important message from Julien" at the start of the description for more guidelines on collaboration and pair programming.

---

Feel free to modify and expand upon this template to better fit your project's specifics and any additional information you want to include.
