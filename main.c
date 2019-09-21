#include <stdio.h>
#include "pipe.h"

int acquisition_failure(PipeAcquisitionFailure f, void* s) {
	char* state = (char*)s;
	printf("Acquisition Failure. %d. %s\n", f.e, state);
	return 1;
}

int normal(Pipe p, void* s) {
	char* state = (char*)s;
	printf("Normal pipe area. [%d, %d]. %s\n", p.r, p.w, state);
	return 0;
}

int close_failure_read(PipeReleaseFailureRead f, void* s) {
	char* state = (char*)s;
	printf("Couldn't close the read pipe. %d. %s\n", f.e, state);
	return 1;
}

int close_failure_write(PipeReleaseFailureWrite f, void* s) {
	char* state = (char*)s;
	printf("Cloudn't close the write pipe. %d. %s\n", f.e, state);
	return 1;
}

int main() {
	char* state = "This is my shared state";
	return pipe_context(
		acquisition_failure,
		normal,
		read_close_failure,
		write_close_failure,
		state
	);
}
