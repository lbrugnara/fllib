#ifndef FL_PROCESS_H
#define FL_PROCESS_H

typedef struct FlProcess *FlProcess;
typedef struct FlPipe *FlPipe;

FlPipe fl_process_pipe_new(void);
void fl_process_pipe_delete(FlPipe pipe);

FlProcess fl_process_create(const char *cmd, char **argv, char **envp, FlPipe in, FlPipe out, FlPipe err);
void fl_process_free(FlProcess process);
bool fl_process_wait(FlProcess process);
char** fl_process_argv(FlProcess process);
char** fl_process_environ(FlProcess process);
size_t fl_process_write_to_stdin(FlProcess process, const char *message, size_t length);
bool fl_process_poll_stdout(FlProcess process, unsigned long sleep_milliseconds, int max_tries);
bool fl_process_poll_stderr(FlProcess process, unsigned long sleep_milliseconds, int max_tries);
bool fl_process_pending_stdout(FlProcess process);
bool fl_process_pending_stderr(FlProcess process);
char* fl_process_read_from_stdout(FlProcess process);
char* fl_process_read_from_stderr(FlProcess process);

#endif /* FL_PROCESS_H */
