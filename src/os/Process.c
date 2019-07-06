#ifdef _WIN32
    #include <windows.h>
    #include <userenv.h>
#endif

#include "../Types.h"
#include "../Mem.h"
#include "../Cstring.h"
#include "Process.h"

struct FlPipe {
    #ifdef _WIN32
    HANDLE read;
    HANDLE write;
    #endif
};

struct FlProcess {
    #ifdef _WIN32
    SECURITY_ATTRIBUTES sec_attrs;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    struct FlPipe *in;
    struct FlPipe *out;
    struct FlPipe *err;
    char **env;
    #endif
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

FlProcess fl_process_create(const char *cmd, const char **argv, char **envp, FlPipe in, FlPipe out, FlPipe err)
{
    struct FlProcess *process = fl_malloc(sizeof(struct FlProcess));

    if (envp)
        process->env = envp;

    if (in)
        process->in = in;

    if (out)
        process->out = out;

    if (err)
        process->err = err;

    #ifdef _WIN32    

    // Set the bInheritHandle flag so pipe handles are inherited.  
    process->sec_attrs.nLength = sizeof(SECURITY_ATTRIBUTES); 
    process->sec_attrs.bInheritHandle = TRUE;
    process->sec_attrs.lpSecurityDescriptor = NULL;

    if (process->in)
    {
        // Create a pipe for the child process's STDIN.  
        if (!CreatePipe(&process->in->read, &process->in->write, &process->sec_attrs, 0))
            return NULL;
    }

    if (process->out)
    {
        // Create a pipe for the child process's STDOUT.  
        if (!CreatePipe(&process->out->read, &process->out->write, &process->sec_attrs, 0))
            return NULL;
    }

    if (process->err)
    {
        // Create a pipe for the child process's STDERR.  
        if (!CreatePipe(&process->err->read, &process->err->write, &process->sec_attrs, 0))
            return NULL;
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

    // Create the child process.
    BOOL result = CreateProcess(
        NULL, 
        (char*)cmd,         // command line 
        NULL,               // process security attributes 
        NULL,               // primary thread security attributes 
        TRUE,               // handles are inherited 
        0, // creation flags 
        process->env ? *process->env : NULL,  
        NULL,               // use parent's current directory 
        &process->si,       // STARTUPINFO pointer 
        &process->pi);      // receives PROCESS_INFORMATION 

    // If an error occurs, exit the application. 
    if (!result)
        return NULL;

    #endif

    return process;
}

void fl_process_wait(FlProcess process)
{
    WaitForSingleObject(process->pi.hProcess, INFINITE);
}

void fl_process_delete(FlProcess process)
{
    #ifdef _WIN32
    if (process->pi.hThread)
        CloseHandle(process->pi.hThread);
    if (process->pi.hProcess)
        CloseHandle(process->pi.hProcess);
    #endif

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

    fl_free(process);
}

size_t fl_process_write_to_stdin(FlProcess process, const char *message, size_t length) 
{
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
}

bool fl_process_poll_stdout(FlProcess process, unsigned long sleep_millisecods, int max_tries)
{
    flm_assert(process, "Process cannot be NULL");

    if (process->out == NULL)
        return false;

    DWORD excode = 0;
    GetExitCodeProcess(process->pi.hProcess, &excode);

    if(excode != STILL_ACTIVE)
        return false;

    unsigned long tries = 0;
    do {
        //now check to see if process has anything to say
        unsigned long available = 0;
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
}

bool fl_process_poll_stderr(FlProcess process, unsigned long sleep_millisecods, int max_tries)
{
    flm_assert(process, "Process cannot be NULL");

    if (process->err == NULL)
        return false;

    DWORD excode = 0;
    GetExitCodeProcess(process->pi.hProcess, &excode);

    if(excode != STILL_ACTIVE)
        return false;
        
    unsigned long tries = 0;
    do {
        //now check to see if process has anything to say
        unsigned long available = 0;
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
}

bool fl_process_pending_stdout(FlProcess process)
{
    flm_assert(process, "Process cannot be NULL");

    if (process->out == NULL)
        return false;

    DWORD excode = 0;
    GetExitCodeProcess(process->pi.hProcess, &excode);

    if(excode != STILL_ACTIVE)
        return false;
        
    //now check to see if process has anything to say
    unsigned long available = 0;
    if(!PeekNamedPipe(process->out->read, NULL, 0, NULL, &available, NULL))
        return false;

    return available > 0;
}

bool fl_process_pending_stderr(FlProcess process)
{
    flm_assert(process, "Process cannot be NULL");

    if (process->err == NULL)
        return false;

    DWORD excode = 0;
    GetExitCodeProcess(process->pi.hProcess, &excode);

    if(excode != STILL_ACTIVE)
        return false;
        
    //now check to see if process has anything to say
    unsigned long available = 0;
    if(!PeekNamedPipe(process->err->read, NULL, 0, NULL, &available, NULL))
        return false;

    return available > 0;
}

char* fl_process_read_from_stdout(FlProcess process)
{
    if (!fl_process_pending_stdout(process))
        return NULL;

    unsigned long available = 0;
    if (!PeekNamedPipe(process->out->read, NULL, 0, NULL, &available, NULL))
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
}

char* fl_process_read_from_stderr(FlProcess process)
{
    if (!fl_process_pending_stderr(process))
        return NULL;

    unsigned long available = 0;
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
}
