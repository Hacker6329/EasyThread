#ifndef EASY_THREAD_H
#define EASY_THREAD_H

//Defs
#define INFINITE_TIME 0xFFFFFFFF

//Including Libraries

#ifdef _WIN32

    //Needed Libraries
    #include <Windows.h>

    //EasyThread Windows Variables Type
    typedef DWORD State;
    typedef DWORD ThreadIdentifier;

    //EasyThread Windows Structure
    typedef struct{
        State state;
        HANDLE thread;
	}EasyThread;

    //Functions Pre-Declaration
    ThreadIdentifier thread_create(EasyThread* threadStructure, void* functionPointer, void* functionArgument);
    State thread_join(EasyThread* threadStructure);
    State thread_kill(EasyThread* threadStructure);
    void thread_getCallerThread(EasyThread* threadStructure);

    //Function Declaration
    ThreadIdentifier thread_create(EasyThread* threadStructure, void* functionPointer, void* functionArgument){
        ThreadIdentifier threadIdentifier;
        threadStructure->thread = CreateThread(0, 0, functionPointer, functionArgument, 0, &threadIdentifier);
        if(threadStructure->thread == 0){
            return (threadStructure->state = -1);
        }
        return threadIdentifier;
    }
    State thread_join(EasyThread* threadStructure){
        return (threadStructure->state = WaitForSingleObject(threadStructure->thread, INFINITE_TIME));
    }
    State thread_kill(EasyThread* threadStructure){
        return (threadStructure->state = TerminateThread(threadStructure->thread, GetExitCodeThread(threadStructure->thread, NULL)));
    }
    void thread_getCallerThread(EasyThread* threadStructure){
        threadStructure->thread = GetCurrentThread();
        threadStructure->state = 0;
    }

#else

    //Needed Libraries
    #include <pthread.h>

    //EasyThread UNIX Variables Type
    typedef int State;
    typedef int ThreadIdentifier;

    //EasySocket UNIX Structure
    typedef struct{
        State state;
        pthread_t thread;
    }EasyThread;

    //Functions Pre-Declaration
    State thread_create(EasyThread* threadStructure, void* functionPointer, void* functionArgument);
    State thread_join(EasyThread* threadStructure);
    State thread_kill(EasyThread* threadStructure);
    void thread_getCallerThread(EasyThread* threadStructure);

    //Function Declaration
    ThreadIdentifier thread_create(EasyThread* threadStructure, void* functionPointer, void* functionArgument){
        threadStructure->state = pthread_create(&(threadStructure->thread), 0, functionPointer, functionArgument);
        return NULL;
    }
    State thread_join(EasyThread* threadStructure){
        return (threadStructure->state = pthread_join(threadStructure->thread, NULL));
    }
    State thread_kill(EasyThread* threadStructure){
        return (threadStructure->state = pthread_cancel(threadStructure->thread));
    }
    void thread_getCallerThread(EasyThread* threadStructure){
        threadStructure->thread = pthread_self();
        threadStructure->state = 0;
    }

#endif

#endif