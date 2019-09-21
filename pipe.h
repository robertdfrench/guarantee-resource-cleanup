#ifndef PIPE_H
#define PIPE_H
typedef struct {
	int e;
} PipeAcquisitionFailure;

typedef struct {
	int r;
	int w;
} Pipe;

typedef struct {
	int e;
} PipeReleaseFailureRead;

typedef struct {
	int e;
} PipeReleaseFailureWrite;

int pipe_context(
	int (*could_not_acquire)(PipeAcquisitionFailure, void*),
	int (*success)(Pipe, void*),
	int (*could_not_release_r)(PipeReleaseFailureRead, void*),
	int (*could_not_release_w)(PipeReleaseFailureWrite, void*),
	void* state
); 
#endif
