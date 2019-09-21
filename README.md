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
conditions. Because this failure must be handled in a context-specific way,
[there is no general way to do this][4].

[1]: https://www.merriam-webster.com/dictionary/back%20talk
[2]: http://man7.org/linux/man-pages/man3/free.3.html
[3]: http://man7.org/linux/man-pages/man2/close.2.html
[4]: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#e16-destructors-deallocation-and-swap-must-never-fail
