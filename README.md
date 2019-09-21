# Forced Cleanup Pattern in C
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

[1]: https://www.merriam-webster.com/dictionary/back%20talk
[2]: http://man7.org/linux/man-pages/man3/free.3.html
[3]: http://man7.org/linux/man-pages/man2/close.2.html
[4]: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#e16-destructors-deallocation-and-swap-must-never-fail
