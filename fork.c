#include <unistd.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include "fork.h"

int fork_context(
	int (*could_not_fork)(Fork_FailureToFork, void*),
	void (*parent)(Fork_Parent, void*),
	int (*child)(Fork_Child, void*),
	int (*could_not_join)(Fork_FailureToJoin, void*),
	void* state
) {
	pid_t pid = fork();
	if (pid == -1) {
		Fork_FailureToFork f = { .e = errno };
		return could_not_fork(f, state);
	}

	if (pid == 0) {
		Fork_Child c;
		return child(c, state);
	}

	Fork_Parent p = { .child = pid };
	parent(p, state);

	int child_rc = 0;
	if (waitpid(pid, &child_rc, 0) == -1) {
		Fork_FailureToJoin f = { .e = errno, .child = pid };
		return could_not_join(f, state);
	}

	return child_rc;
}
