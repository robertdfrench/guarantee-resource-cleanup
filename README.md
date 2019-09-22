# Guaranteed Cleanup Pattern in C
RAII is not well suited for resources that might [back talk][1] when we try to
close them. Instead, we look at a pattern where function pointers must be
provided for each failure case, so that we can make a compile-time guarantee
to handle fussy resources.

### free vs close
When we [`free()`][2] some memory, no error codes are returned. This command
always succeeds, so it fits the criteria for a C++ destructor or Rust drop
statement: it must not cause further problems of its own.

However, this is not the case with [`close()`][3], which can fail under certain
conditions. Here's what the [C++ Core Guidelines][4] say about this:

> Many have tried to write reliable code violating this rule for examples, such as a network connection that “refuses to close”. To the best of our knowledge nobody has found a general way of doing this. Occasionally, for very specific examples, you can get away with setting some state for future cleanup. For example, we might put a socket that does not want to close on a “bad socket” list, to be examined by a regular sweep of the system state. Every example we have seen of this is error-prone, specialized, and often buggy.

The pattern presented in this work attempts to establish a general way of
dealing with resources which may refuse to close.

### In-line Guarantees
Consider this small program which forks and then prints a message delivered to
parent from child:

```c
#include <unistd.h>
#include <sys/errno.h>
#include <sys/wait.h>

int main() {
	int fds[2];
	pipe(fds);

	pid_t pid = fork();
	if (pid == 0) {
		char message[14] = "Hello, World!";
		write(fd[1], message, 14);
	} else {
		char message[14];
		read(fd[0], message, 14);
		printf("The child says '%s'\n", message);
	}
	return 0;
}
```

There's a lot that can go wrong here. Each of `pipe`, `fork`, `write`, and
`read` can fail and cause unanticipated results:

* If `pipe` fails, the program won't print a useful message, but it will exit 0.
* If either `fork` or `write` fails, the program could hang forever attempting to read from a descriptor to which no data will ever be written.

[1]: https://www.merriam-webster.com/dictionary/back%20talk
[2]: http://man7.org/linux/man-pages/man3/free.3.html
[3]: http://man7.org/linux/man-pages/man2/close.2.html
[4]: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#e16-destructors-deallocation-and-swap-must-never-fail
