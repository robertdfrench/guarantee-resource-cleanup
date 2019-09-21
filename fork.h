#ifndef FORK_H
#define FORK_H
#include <unistd.h>

typedef struct {
	int e;
} Fork_FailureToFork;

typedef struct {
	pid_t child;
} Fork_Parent;

typedef struct {
	// nothing
} Fork_Child;

typedef struct {
	int e;
	pid_t child;
} Fork_FailureToJoin;

int fork_context(
	int (*could_not_fork)(Fork_FailureToFork, void*),
	void (*parent)(Fork_Parent, void*),
	int (*child)(Fork_Child, void*),
	int (*could_not_join)(Fork_FailureToJoin, void*),
	void* state
); 
#endif
