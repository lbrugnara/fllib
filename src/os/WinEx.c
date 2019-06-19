#include <stdio.h>
#include "WinEx.h"

#define MAX_HANDLERS 30

/*
 * Type: struct FlWinRegisteredExceptionHandler
 *
 * Internal usage struct to keep track of handlers functions
 * to be called when an exception with code {exception}
 * is raised by Windows
 *
 * {member: DWORD exception} Exception code of the registered handler
 * {member: FlWinExceptionHandler handler} Handler function
 *
 */
typedef struct
{
    DWORD exception;
    FlWinExceptionHandler handler;
} FlWinRegisteredExceptionHandler;

/*
 * {variable: FlWinRegisteredExceptionHandler[] RegisteredExHandlers}
 *
 * Keep track of registered handlers for each type of exception
 *
 */
static FlWinRegisteredExceptionHandler RegisteredExHandlers[MAX_HANDLERS] = {{0}};
/*
 * {variable: FlWinExceptionHandler PrevHandler}
 *
 * Keeps track of the previous registered exception handler.
 * It is returned when user registers a global handler (windows style)
 * or each time it calls fl_winex_handler_set to register
 * an specific exception handler
 *
 */
static FlWinExceptionHandler PrevHandler = NULL;

/*
 * Function: winex_filter
 *
 * Overrides the previous UnhandledExceptionFilter (if exists)
 * to support fl_winex_handler_set, using specific exceptions
 * codes.
 * This function will be called each time Windows throws an exception
 * but will call the user registered FlWinExceptionHandler only when
 * the exception code is registered by the user.
 *
 * EXCEPTION_POINTERS* ExceptionInfo - Information about the Windows exception
 *
 * {return: LONG} Returns one of three values: EXCEPTION_CONTINUE_SEARCH, EXCEPTION_EXECUTE_HANDLER or EXCEPTION_CONTINUE_EXECUTION
 *
 */
LONG WINAPI winex_filter(EXCEPTION_POINTERS * ExceptionInfo)
{
    FlWinExceptionHandler *handler = NULL;
    for (int i=0; i < MAX_HANDLERS; i++)
    {
        if (RegisteredExHandlers[i].handler == NULL)
            break;
        if (RegisteredExHandlers[i].exception != ExceptionInfo->ExceptionRecord->ExceptionCode)
            continue;
        handler = &RegisteredExHandlers[i].handler;
        break;
    }

    if (handler == NULL)
        return EXCEPTION_CONTINUE_SEARCH;

    return (*handler)(ExceptionInfo);
}

/*
 * Function: fl_winex_handler_set
 *
 * Sets an exception handler for an specific exception code. It saves
 * the user's exception handler in the {RegisteredExHandlers} array
 * and registers the {winex_filter} function as the Unhandled Exception
 * Filter. Every time Windows raises an exception, the {winex_filter}
 * will check the exception code and will call the user's exception
 * handler. Windows supports only one UnhandledExceptionFilter, this way
 * we can provide support for specific exceptions abstracting this limitation
 * to the user.
 * 
 * TODO: This is a simple implementation of an exception handling
 * mechanism. Windows uses threads, and the current implementation allows
 * only 1 thread. We can make usage of GetCurrentThreadId to save the
 * exception filter for specifics threads. The user has to call
 * {fl_winex_handler_set} on each thread, and before the thread
 * finishes, we should have to remove the filter, this way
 * we release the memory used by this module
 *
 * Known bug: https://support.microsoft.com/en-us/kb/173652
 *
 * DWORD exceptionCode -
 * FlWinExceptionHandler handler} Handler function to call when an exception with code {exceptionCode - occurs
 *
 * {return: FlWinExceptionHandler} Returns the previous UnhandledExceptionFilter
 *
 */
FlWinExceptionHandler fl_winex_handler_set(DWORD exceptionCode, FlWinExceptionHandler handler)
{
    int i=0;
    for (; i < MAX_HANDLERS; i++)
    {
        if (RegisteredExHandlers[i].handler != NULL)
            continue;
        RegisteredExHandlers[i].exception = exceptionCode;
        RegisteredExHandlers[i].handler = handler;
        break;
    }
    // Set the exception filter
    if (i==0)
        PrevHandler = SetUnhandledExceptionFilter(winex_filter);
    return PrevHandler;
}

/*
 * Function: fl_winex_global_handler_set
 *
 * Sets a global exception handler. It suffers the same problem
 * as fl_winex_handler_set, The usage of PrevHandler makes the
 * module incompatible to use in threaded applications. The usage
 * of GetCurrentThreadId could help to keep track of the registered
 * exception filters for each thread. 
 *
 * FlWinExceptionHandler handler -
 *
 * {return: FlWinExceptionHandler} Returns the previous UnhandledExceptionFilter
 *
 */
FlWinExceptionHandler fl_winex_global_handler_set(FlWinExceptionHandler handler)
{
    return (PrevHandler = SetUnhandledExceptionFilter(handler));
}

/*
 * Function: fl_winex_message_get
 *
 * Using {exceptionCode} copies into {destmsg} a brief description
 * of the exception
 *
 * DWORD exceptionCode - Exception code to look for a message
 * char* destmsg - Destination string to use to copy the message
 *
 * {return: void}
 *
 */
void fl_winex_message_get(DWORD exceptionCode, char *destmsg, size_t maxlength)
{
    if (destmsg == NULL)
        return;
    char *msg = NULL;
    switch (exceptionCode) 
    {
        case EXCEPTION_ACCESS_VIOLATION:
            msg = "[EXCEPTION_ACCESS_VIOLATION] The thread tried to read from or write to a virtual address for which it does not have the appropriate access.";
            break;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            msg = "[EXCEPTION_ARRAY_BOUNDS_EXCEEDED] The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking.";
            break;
        case EXCEPTION_BREAKPOINT:
            msg = "[EXCEPTION_BREAKPOINT] A breakpoint was encountered.";
            break;
        case EXCEPTION_DATATYPE_MISALIGNMENT:
            msg = "[EXCEPTION_DATATYPE_MISALIGNMENT] The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on.";
            break;
        case EXCEPTION_FLT_DENORMAL_OPERAND:
            msg = "[EXCEPTION_FLT_DENORMAL_OPERAND] One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value.";
            break;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            msg = "[EXCEPTION_FLT_DIVIDE_BY_ZERO] The thread tried to divide a floating-point value by a floating-point divisor of zero.";
            break;
        case EXCEPTION_FLT_INEXACT_RESULT:
            msg = "[EXCEPTION_FLT_INEXACT_RESULT] The result of a floating-point operation cannot be represented exactly as a decimal fraction.";
            break;
        case EXCEPTION_FLT_INVALID_OPERATION:
            msg = "[EXCEPTION_FLT_INVALID_OPERATION] This exception represents any floating-point exception not included in this list.";
            break;
        case EXCEPTION_FLT_OVERFLOW:
            msg = "[EXCEPTION_FLT_OVERFLOW] The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.";
            break;
        case EXCEPTION_FLT_STACK_CHECK:
            msg = "[EXCEPTION_FLT_STACK_CHECK] The stack overflowed or underflowed as the result of a floating-point operation.";
            break;
        case EXCEPTION_FLT_UNDERFLOW:
            msg = "[EXCEPTION_FLT_UNDERFLOW] The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.";
            break;
        case EXCEPTION_GUARD_PAGE:
            msg = "[EXCEPTION_GUARD_PAGE] The thread accessed memory allocated with the PAGE_GUARD modifier.";
            break;
        case EXCEPTION_ILLEGAL_INSTRUCTION:
            msg = "[EXCEPTION_ILLEGAL_INSTRUCTION] The thread tried to execute an invalid instruction.";
            break;
        case EXCEPTION_IN_PAGE_ERROR:
            msg = "[EXCEPTION_IN_PAGE_ERROR] The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network.";
            break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            msg = "[EXCEPTION_INT_DIVIDE_BY_ZERO] The thread tried to divide an integer value by an integer divisor of zero.";
            break;
        case EXCEPTION_INT_OVERFLOW:
            msg = "[EXCEPTION_INT_OVERFLOW] The result of an integer operation caused a carry out of the most significant bit of the result.";
            break;
        case EXCEPTION_INVALID_DISPOSITION:
            msg = "[EXCEPTION_INVALID_DISPOSITION] An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception.";
            break;
        case EXCEPTION_INVALID_HANDLE:
            msg = "[EXCEPTION_INVALID_HANDLE] The thread used a handle to a kernel object that was invalid (probably because it had been closed.)";
            break;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            msg = "[EXCEPTION_NONCONTINUABLE_EXCEPTION] The thread tried to continue execution after a noncontinuable exception occurred.";
            break;
        case EXCEPTION_PRIV_INSTRUCTION:
            msg = "[EXCEPTION_PRIV_INSTRUCTION] The thread tried to execute an instruction whose operation is not allowed in the current machine mode.";
            break;
        case EXCEPTION_SINGLE_STEP:
            msg = "[EXCEPTION_SINGLE_STEP] A trace trap or other single-instruction mechanism signaled that one instruction has been executed.";
            break;
        case EXCEPTION_STACK_OVERFLOW:
            msg = "[EXCEPTION_STACK_OVERFLOW] The thread used up its stack.";
            break;
        // Status
        case STATUS_SEGMENT_NOTIFICATION:
            msg = "[STATUS_SEGMENT_NOTIFICATION]";
            break;
        case STATUS_NO_MEMORY:
            msg = "[STATUS_NO_MEMORY] Not enough virtual memory or paging file quota is available to complete the specified operation.";
            break;
        case STATUS_CONTROL_C_EXIT:
            msg = "[STATUS_CONTROL_C_EXIT]";
            break;
        default:
            sprintf(msg, "Unknown error: code %lu", exceptionCode);
            break;
    }
    if (msg == NULL)
        return;

    size_t strlength = strlen(msg);
    memcpy(destmsg, msg, strlength < maxlength ? strlength : maxlength);
}

/*
 * Control Handler
 *
 */
#define CMDCTRLS_NUMBER 6

typedef struct
{
    DWORD ctrl_type;
    FlWinCmdControlHandler handler;
} FlConsoleCtrl;

static FlConsoleCtrl ConsoleCtrl[CMDCTRLS_NUMBER] = {{0}};

BOOL __stdcall winctrl_handler(DWORD ctrlType)
{
    FlWinCmdControlHandler *handler = NULL;
    for (int i=0; i < CMDCTRLS_NUMBER; i++)
    {
        if (ConsoleCtrl[i].handler == NULL)
            break;
        if (ConsoleCtrl[i].ctrl_type != ctrlType)
            continue;
        handler = &ConsoleCtrl[i].handler;
        break;
    }

    if (handler == NULL)
        return ctrlType == CTRL_BREAK_EVENT || ctrlType == CTRL_LOGOFF_EVENT || ctrlType ==  CTRL_SHUTDOWN_EVENT;

    return (*handler)(ctrlType);
}

void fl_wincmd_control_handler_set(DWORD ctrl_type, FlWinCmdControlHandler handler)
{
    int i=0;
    for (; i < CMDCTRLS_NUMBER; i++)
    {
        if (ConsoleCtrl[i].handler != NULL)
            continue;
        ConsoleCtrl[i].ctrl_type = ctrl_type;
        ConsoleCtrl[i].handler = handler;
        break;
    }
    // Set the ConsoleCtrl handler
    if (i==0)
        SetConsoleCtrlHandler((PHANDLER_ROUTINE)winctrl_handler, TRUE);
}
