#include <unistd.h>
#include <sys/errno.h>
#include "pipe.h"

int pipe_context(
		int (*could_not_acquire)(PipeAcquisitionFailure, void*),
		int (*success)(Pipe, void*),
		int (*could_not_release_r)(PipeReleaseFailureRead, void*),
		int (*could_not_release_w)(PipeReleaseFailureWrite, void*),
		void* state
		) {
	int fd[2];

	if (pipe(fd) == -1) {
		PipeAcquisitionFailure failure = { .e = errno };
		return could_not_acquire(failure, state);
	}

	Pipe p = { .r = fd[0], .w = fd[1] };
	success(p, state);

	if (close(p.r) == -1) {
		PipeReleaseFailureRead failure = { .e = errno };
		return could_not_release_r(failure, state);
	}

	if (close(p.w) == -1) {
		PipeReleaseFailureWrite failure = { .e = errno };
		return could_not_release_w(failure, state);
	}

	return 0;
}

