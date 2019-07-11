#ifdef _WIN32
    #include <windows.h>
    #include <userenv.h>
#elif defined(__linux__)
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

#include "../Types.h"
#include "../Mem.h"
#include "../Cstring.h"
#include "Process.h"

struct FlPipe {
    #ifdef _WIN32
    HANDLE read;
    HANDLE write;
    #elif defined(__linux__)
    int read;
    int write;
    #endif
};

struct FlProcess {
    const char *command;
    #ifdef _WIN32
    SECURITY_ATTRIBUTES sec_attrs;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char *winenvp;
    #elif defined(__linux__)
    pid_t child_pid;
    #endif
    char **argv;
    char **envp;
    struct FlPipe *in;
    struct FlPipe *out;
    struct FlPipe *err;
};

FlPipe fl_process_pipe_new(void)
{
    struct FlPipe *pipe = fl_malloc(sizeof(struct FlPipe));
    return pipe;
}

void fl_process_pipe_delete(FlPipe pipe)
{
    fl_free(pipe);
}

#ifdef _WIN32
bool win32_process_create(FlProcess process)
{
    // Set the bInheritHandle flag so pipe handles are inherited.  
    process->sec_attrs.nLength = sizeof(SECURITY_ATTRIBUTES); 
    process->sec_attrs.bInheritHandle = TRUE;
    process->sec_attrs.lpSecurityDescriptor = NULL;

    if (process->in)
    {
        // Create a pipe for the child process's STDIN.  
        if (!CreatePipe(&process->in->read, &process->in->write, &process->sec_attrs, 0))
            return false;
    }

    if (process->out)
    {
        // Create a pipe for the child process's STDOUT.  
        if (!CreatePipe(&process->out->read, &process->out->write, &process->sec_attrs, 0))
            return false;
    }

    if (process->err)
    {
        // Create a pipe for the child process's STDERR.  
        if (!CreatePipe(&process->err->read, &process->err->write, &process->sec_attrs, 0))
            return false;
    }

    process->si.cb = sizeof(STARTUPINFO);

    if (process->in)
    {
        process->si.hStdInput = process->in->read;
    }

    if (process->out)
    {
        process->si.hStdOutput = process->out->write;
        process->si.hStdError = process->out->write;
    }

    if (process->err)
    {
        process->si.hStdError = process->err;
    }

    process->si.dwFlags |= STARTF_USESTDHANDLES;

    char *command_line = fl_cstring_vdup("%s", process->command);

    if (process->argv)
    {
        // argv[argc] must be NULL
        char **argv = process->argv;
        while (*argv)
        {
            fl_cstring_append(&command_line, " ");
            fl_cstring_append(&command_line, *argv);
            argv++;
        }
    }

    process->winenvp = NULL;
    if (process->envp)
    {
        size_t reqlength = 0;
        char **curenv = process->envp;
        while(*curenv)
        {
            reqlength += strlen(*curenv) + 1;
            curenv++;
        }
        
        process->winenvp = fl_cstring_new(reqlength);
        char *tmp = process->winenvp;
        size_t written = 0;
        curenv = process->envp;
        while(*curenv)
        {
            fl_cstring_copy(tmp, *curenv);
            written += strlen(*curenv) + 1;
            curenv++;
            tmp = process->winenvp + written;
        }
    }

    // Create the child process.
    BOOL result = CreateProcess(
        NULL,
        command_line,         // command line 
        NULL,               // process security attributes 
        NULL,               // primary thread security attributes 
        TRUE,               // handles are inherited 
        0, // creation flags 
        process->winenvp,  
        NULL,               // use parent's current directory 
        &process->si,       // STARTUPINFO pointer 
        &process->pi);      // receives PROCESS_INFORMATION 

    fl_cstring_delete(command_line);

    // If an error occurs, exit the application. 
    if (!result)
        return false;

    return true;
}
#endif

#if defined(__linux__)
bool linux_process_create(FlProcess process)
{
    if (process->in)
        if (pipe((int[2]){ process->in->read, process->in->write }) < 0)
            return false;

    if (process->out)
        if (pipe((int[2]){ process->out->read, process->out->write }) < 0)
            return false;

    if (process->err)
        if (pipe((int[2]){ process->err->read, process->err->write }) < 0)
            return false;

    process->child_pid = fork();

    if (process->child_pid < 0)
        return false;

    if (process->child_pid == 0) // child
    {
        // redirect stdin
        if (process->in)
        {
            if (dup2(process->in->read, STDIN_FILENO) == -1)
            {
                exit(-1);
            }
        }

        // redirect stdout
        if (process->out)
        {
            if (dup2(process->out->write, STDOUT_FILENO) == -1)
            {
                exit(-1);
            }

            // redirect stderr
            if (dup2(process->out->write, STDERR_FILENO) == -1)
            {
                exit(-1);
            }
        }

        if (process->err)
        {
            // redirect stderr
            if (dup2(process->out->write, STDERR_FILENO) == -1)
            {
                exit(-1);
            }
        }

        if (process->in)
        {
            close(process->in->read);
            close(process->in->write);
        }

        if (process->out)
        {
            close(process->out->read);
            close(process->out->write);
        }

        if (process->err)
        {
            close(process->err->read);
            close(process->err->write);
        }

        int exit_code = execve(process->command, process->argv, process->envp);

        exit(exit_code);
    }

    // parent
    if (process->in)
        close(process->in->read);

    if (process->out)
        close(process->out->write);

    return true;
}
#endif

FlProcess fl_process_create(const char *cmd, char **argv, char **envp, FlPipe in, FlPipe out, FlPipe err)
{
    struct FlProcess *process = fl_malloc(sizeof(struct FlProcess));

    process->command = cmd;
    process->argv = argv;

    if (envp)
        process->envp = envp;

    if (in)
        process->in = in;

    if (out)
        process->out = out;

    if (err)
        process->err = err;

    #ifdef _WIN32
    if (!win32_process_create(process))
    {
        fl_process_delete(process);
        return NULL;
    }
    #elif defined(__linux__)
    if (!linux_process_create(process))
    {
        fl_process_delete(process);
        return NULL;
    }
    #endif

    return process;
}

bool fl_process_wait(FlProcess process)
{
    #ifdef _WIN32

    DWORD status = WaitForSingleObject(process->pi.hProcess, INFINITE);
    return status == WAIT_OBJECT_0;

    #elif defined(__linux__)

    int status = 0;
    pid_t wpid = waitpid(process->child_pid,  &status, 0);    
    return wpid == process->child_pid && WIFEXITED(status) != 0;

    #endif
}

void fl_process_delete(FlProcess process)
{
    #ifdef _WIN32
    {
        TerminateProcess(process->pi.hProcess, 0);

        if (process->pi.hThread)
            CloseHandle(process->pi.hThread);

        if (process->pi.hProcess)
            CloseHandle(process->pi.hProcess);

        if (process->in)
        {
            CloseHandle(process->in->read);
            CloseHandle(process->in->write);
        }

        if (process->out)
        {
            CloseHandle(process->out->read);
            CloseHandle(process->out->write);
        }

        if (process->err)
        {
            CloseHandle(process->err->read);
            CloseHandle(process->err->write);
        }

        if (process->winenvp)
            fl_cstring_delete(process->winenvp);
    }
    #elif defined(__linux__)
    {
        if (process->in)
        {
            close(process->in->read);
            close(process->in->write);
        }

        if (process->out)
        {
            close(process->out->read);
            close(process->out->write);
        }

        if (process->err)
        {
            close(process->err->read);
            close(process->err->write);
        }
    }
    #endif

    if (process->in)
        fl_process_pipe_delete(process->in);

    if (process->out)
        fl_process_pipe_delete(process->out);

    if (process->err)
        fl_process_pipe_delete(process->err);

    fl_free(process);
}

char** fl_process_argv(FlProcess process)
{
    return process->argv;
}

char** fl_process_environ(FlProcess process)
{
    return process->envp;
}

size_t fl_process_write_to_stdin(FlProcess process, const char *message, size_t length) 
{
    #ifdef _WIN32
    DWORD written;

    if (!process->in)
    {
        process->in = fl_process_pipe_new();
        // Create a pipe for the child process's STDIN.  
        if (!CreatePipe(&process->in->read, &process->in->write, &process->sec_attrs, 0))
            return 0;

        // Ensure the write handle to the pipe for STDIN is not inherited.  
        if (!SetHandleInformation(process->in->read, HANDLE_FLAG_INHERIT, 0))
            return 0;

        process->si.hStdInput = process->in->read;
    }

    if (!WriteFile(process->in->write, message, length, &written, NULL))
        return 0;

    return (size_t)written;

    #elif defined(__linux__)
    return 0;
    #endif

}

bool fl_process_poll_stdout(FlProcess process, unsigned long sleep_millisecods, int max_tries)
{
    flm_assert(process, "Process cannot be NULL");

    if (process->out == NULL)
        return false;

    #ifdef _WIN32
    DWORD excode = 0;
    GetExitCodeProcess(process->pi.hProcess, &excode);

    if(excode != STILL_ACTIVE)
        return false;

    unsigned long tries = 0;
    do {
        //now check to see if process has anything to say
        DWORD available = 0;
        BOOL success = PeekNamedPipe(process->out->read, NULL, 0, NULL, &available, NULL);

        if (!success)
            return false;
        
        if(available > 0)
            return true;

        if (max_tries > 0)
        {
            if (tries >= (unsigned long)max_tries)
                return false;
            tries++;
        }

        Sleep(sleep_millisecods);

    } while (true);

    #elif defined(__linux__)
    return false;
    #endif
}

bool fl_process_poll_stderr(FlProcess process, unsigned long sleep_millisecods, int max_tries)
{
    flm_assert(process, "Process cannot be NULL");

    if (process->err == NULL)
        return false;

    #ifdef _WIN32
    DWORD excode = 0;
    GetExitCodeProcess(process->pi.hProcess, &excode);

    if(excode != STILL_ACTIVE)
        return false;
        
    unsigned long tries = 0;
    do {
        //now check to see if process has anything to say
        DWORD available = 0;
        BOOL success = PeekNamedPipe(process->err->read, NULL, 0, NULL, &available, NULL);

        if (!success)
            return false;
        
        if(available > 0)
            return true;

        if (max_tries > 0)
        {
            if (tries >= (unsigned long)max_tries)
                return false;
            tries++;
        }

        Sleep(sleep_millisecods);

    } while (true);

    #elif defined(__linux__)
    return false;
    #endif
}

bool fl_process_pending_stdout(FlProcess process)
{
    flm_assert(process, "Process cannot be NULL");

    if (process->out == NULL)
        return false;

    #ifdef _WIN32
    DWORD excode = 0;
    GetExitCodeProcess(process->pi.hProcess, &excode);

    if(excode != STILL_ACTIVE)
        return false;
        
    //now check to see if process has anything to say
    DWORD available = 0;
    if(!PeekNamedPipe(process->out->read, NULL, 0, NULL, &available, NULL))
        return false;

    return available > 0;

    #elif defined(__linux__)
    return false;
    #endif
}

bool fl_process_pending_stderr(FlProcess process)
{
    flm_assert(process, "Process cannot be NULL");

    if (process->err == NULL)
        return false;

    #ifdef _WIN32
    DWORD excode = 0;
    GetExitCodeProcess(process->pi.hProcess, &excode);

    if(excode != STILL_ACTIVE)
        return false;
        
    //now check to see if process has anything to say
    DWORD available = 0;
    if(!PeekNamedPipe(process->err->read, NULL, 0, NULL, &available, NULL))
        return false;

    return available > 0;

    #elif defined(__linux__)
    return false;
    #endif
}

char* fl_process_read_from_stdout(FlProcess process)
{
    if (!fl_process_pending_stdout(process))
        return NULL;

    #ifdef _WIN32
    DWORD available = 0;
    if (!PeekNamedPipe(process->out->read, NULL, 0, NULL, &available, NULL))
        return NULL;

    if (available == 0)
        return NULL;

    // cstring_new adds 1 for the null character
    char *msg = fl_cstring_new(available);

    DWORD read = 0;
    BOOL result = ReadFile(process->out->read, msg, available, &read, NULL);

    if (!result)
    {
        // Something happened
        fl_cstring_delete(msg);
        return NULL;
    }

    if (read < available)
    {
        // Something happened
        fl_cstring_delete(msg);
        return NULL;
    }

    return msg;

    #elif defined(__linux__)
    return NULL;
    #endif
}

char* fl_process_read_from_stderr(FlProcess process)
{
    if (!fl_process_pending_stderr(process))
        return NULL;

    #ifdef _WIN32

    DWORD available = 0;
    if (!PeekNamedPipe(process->err->read, NULL, 0, NULL, &available, NULL))
        return NULL;

    // cstring_new adds 1 for the null character
    char *msg = fl_cstring_new(available);

    DWORD read = 0;
    BOOL result = ReadFile(process->err->read, msg, available, &read, NULL);

    if (!result)
    {
        // Something happened
        fl_cstring_delete(msg);
        return NULL;
    }

    if (read < available)
    {
        // Something happened
        fl_cstring_delete(msg);
        return NULL;
    }

    return msg;

    #elif defined(__linux__)
    return NULL;
    #endif
}
