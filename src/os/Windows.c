#include <stdio.h>
#include "Windows.h"

#define EXCEPTIONS_NUMBER 29

typedef struct
{
    DWORD exception;
    FlSignalHandler handler;
} FlWinException;

static FlWinException ExMap[EXCEPTIONS_NUMBER] = {{0}};

LONG WINAPI fl_exception_filter(EXCEPTION_POINTERS * ExceptionInfo)
{
    FlSignalHandler *handler = NULL;
    for (int i=0; i < EXCEPTIONS_NUMBER; i++)
    {
        if (ExMap[i].handler == NULL)
            break;
        if (ExMap[i].exception != ExceptionInfo->ExceptionRecord->ExceptionCode)
            continue;
        handler = &ExMap[i].handler;
        break;
    }

    if (handler == NULL)
        return EXCEPTION_CONTINUE_SEARCH;

    return (*handler)(ExceptionInfo);
}

void fl_exception_handler_set(unsigned long sig, FlSignalHandler handler)
{
    int i=0;
    for (; i < EXCEPTIONS_NUMBER; i++)
    {
        if (ExMap[i].handler != NULL)
            continue;
        ExMap[i].exception = (DWORD)sig;
        ExMap[i].handler = handler;
        break;
    }
    // Set the exception filter
    if (i==0)
        SetUnhandledExceptionFilter(fl_exception_filter);
}

void fl_exception_global_handler_set(FlSignalHandler handler)
{
    SetUnhandledExceptionFilter(handler);
}

void fl_exception_message_get(DWORD exceptionCode, char *destmsg)
{
    if (destmsg == NULL)
        return;
    char *msg = NULL;
    switch (exceptionCode) 
    {
        case EXCEPTION_ACCESS_VIOLATION:
            msg = "ACCESS_VIOLATION: The thread tried to read from or write to a virtual address for which it does not have the appropriate access. ";
            break;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            msg = "ARRAY_BOUNDS_EXCEEDED: The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking. ";
            break;
        case EXCEPTION_BREAKPOINT:
            msg = "BREAKPOINT: A breakpoint was encountered. ";
            break;
        case EXCEPTION_DATATYPE_MISALIGNMENT:
            msg = "DATATYPE_MISALIGNMENT: The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on. ";
            break;
        case EXCEPTION_FLT_DENORMAL_OPERAND:
            msg = "FLT_DENORMAL_OPERAND: One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value. ";
            break;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            msg = "FLT_DIVIDE_BY_ZERO: The thread tried to divide a floating-point value by a floating-point divisor of zero. ";
            break;
        case EXCEPTION_FLT_INEXACT_RESULT:
            msg = "FLT_INEXACT_RESULT: The result of a floating-point operation cannot be represented exactly as a decimal fraction. ";
            break;
        case EXCEPTION_FLT_INVALID_OPERATION:
            msg = "FLT_INVALID_OPERATION: This exception represents any floating-point exception not included in this list. ";
            break;
        case EXCEPTION_FLT_OVERFLOW:
            msg = "FLT_OVERFLOW: The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type. ";
            break;
        case EXCEPTION_FLT_STACK_CHECK:
            msg = "FLT_STACK_CHECK: The stack overflowed or underflowed as the result of a floating-point operation. ";
            break;
        case EXCEPTION_FLT_UNDERFLOW:
            msg = "FLT_UNDERFLOW: The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type. ";
            break;
        case EXCEPTION_ILLEGAL_INSTRUCTION:
            msg = "ILLEGAL_INSTRUCTION: The thread tried to execute an invalid instruction. ";
            break;
        case EXCEPTION_IN_PAGE_ERROR:
            msg = "IN_PAGE_ERROR: The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network. ";
            break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            msg = "INT_DIVIDE_BY_ZERO: The thread tried to divide an integer value by an integer divisor of zero. ";
            break;
        case EXCEPTION_INT_OVERFLOW:
            msg = "INT_OVERFLOW: The result of an integer operation caused a carry out of the most significant bit of the result. ";
            break;
        case EXCEPTION_INVALID_DISPOSITION:
            msg = "INVALID_DISPOSITION: An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception. ";
            break;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            msg = "NONCONTINUABLE_EXCEPTION: The thread tried to continue execution after a noncontinuable exception occurred. ";
            break;
        case EXCEPTION_PRIV_INSTRUCTION:
            msg = "PRIV_INSTRUCTION: The thread tried to execute an instruction whose operation is not allowed in the current machine mode. ";
            break;
        case EXCEPTION_SINGLE_STEP:
            msg = "SINGLE_STEP: A trace trap or other single-instruction mechanism signaled that one instruction has been executed. ";
            break;
        case EXCEPTION_STACK_OVERFLOW:
            msg = "STACK_OVERFLOW: The thread used up its stack. ";
            break;
        case STATUS_WAIT_0:
            msg = "STATUS_WAIT_0";
            break;
        case STATUS_ABANDONED_WAIT_0:
            msg = "STATUS_ABANDONED_WAIT_0";
            break;
        case STATUS_USER_APC:
            msg = "TATUS_USER_APC";
            break;
        case STATUS_TIMEOUT:
            msg = "STATUS_TIMEOUT";
            break;
        case STATUS_PENDING:
            msg = "STATUS_PENDING";
            break;
        case STATUS_SEGMENT_NOTIFICATION:
            msg = "STATUS_SEGMENT_NOTIFICATION";
            break;
        case STATUS_GUARD_PAGE_VIOLATION:
            msg = "STATUS_GUARD_PAGE_VIOLATION";
            break;
        case STATUS_NO_MEMORY:
            msg = "STATUS_NO_MEMORY";
            break;
        case STATUS_CONTROL_C_EXIT:
            msg = "STATUS_CONTROL_C_EXIT";
            break;
        default:
            sprintf(msg, "Unknown error: code %lu", exceptionCode);
            break;
    }
    if (msg == NULL)
        return;
    memcpy(destmsg, msg, strlen(msg));
}

/* -------------------------------------------------------------
 * Control Handler
 * -------------------------------------------------------------
 */
#define CMDCTRLS_NUMBER 6

typedef struct
{
    DWORD ctrl_type;
    FlConsoleControlHandler handler;
} FlConsoleCtrl;

static FlConsoleCtrl ConsoleCtrl[CMDCTRLS_NUMBER] = {{0}};

BOOL fl_control_handler( DWORD ctrlType )
{
    FlConsoleControlHandler *handler = NULL;
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

void fl_consolecontrol_handler(unsigned long ctrl_type, FlConsoleControlHandler handler)
{
    int i=0;
    for (; i < CMDCTRLS_NUMBER; i++)
    {
        if (ConsoleCtrl[i].handler != NULL)
            continue;
        ConsoleCtrl[i].ctrl_type = (DWORD)ctrl_type;
        ConsoleCtrl[i].handler = handler;
        break;
    }
    // Set the ConsoleCtrl handler
    if (i==0)
        SetConsoleCtrlHandler((PHANDLER_ROUTINE)fl_control_handler, TRUE);
}