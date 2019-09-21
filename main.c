#include <stdio.h>
#include "pipe.h"
#include "fork.h"

int acquisition_failure(PipeAcquisitionFailure f, void* s) {
	printf("Acquisition Failure. %d.\n", f.e);
	return 1;
}

int could_not_fork(Fork_FailureToFork f, void* s) {
	printf("Could not fork a child\n");
	return 1;
}

void parent(Fork_Parent me, void* s) {
	Pipe* p = (Pipe*)s;
	char buf[1024];
	size_t bytes_read = read(p->r, buf, 1023);
	buf[bytes_read] = 0;
	printf("I read '%s' from child\n", buf);
}

int child(Fork_Child me, void* s) {
	Pipe* p = (Pipe*)s;
	return write(p->w, "sup", 3);
}

int could_not_join(Fork_FailureToJoin f, void* s) {
	printf("Child may be xombied\n");
	return 1;
}

int normal(Pipe p, void* s) {
	return fork_context(
		could_not_fork,
		parent,
		child,
		could_not_join,
		&p
	);
}

int close_failure_read(PipeReleaseFailureRead f, void* s) {
	printf("Couldn't close the read pipe. %d\n", f.e);
	return 1;
}

int close_failure_write(PipeReleaseFailureWrite f, void* s) {
	printf("Cloudn't close the write pipe. %d\n", f.e);
	return 1;
}

int main() {
	return pipe_context(
		acquisition_failure,
		normal,
		close_failure_read,
		close_failure_write,
		NULL
	);
}
