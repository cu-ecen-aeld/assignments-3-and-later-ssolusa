#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


// Optional: use these functions to add debug or error prints to your application
// #define DEBUG_LOG(msg,...)
// #define DEBUG_LOG(msg,...)
//     printf("threading: " msg "\n" , ##__VA_ARGS__)
// #define ERROR_LOG(msg,...)
//     printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)


void* threadfunc(void* thread_param) {
    // TODO:
    //     - Wait
    //     - Obtain mutex
    //     - Wait
    //     - Release mutex as described by thread_data structure
    // HINT: use a cast like the one below to obtain thread arguments from your parameter
    //       struct thread_data* thread_func_args = (struct thread_data *) thread_param;


    printf("ssolusa; threadfunc() START...\n");


    int rc;


    struct thread_data* func_args;
    func_args = (struct thread_data*) thread_param;


    printf("ssolusa; sleep ms...\n");
    usleep(func_args->wait_to_obtain_ms);


    printf("ssolusa; mutex_lock()...\n");
    rc = pthread_mutex_lock(func_args->mutex);
    if ( rc != 0 ) {
        printf("ssolusa; ERROR: pthread_mutex_lock() failed; rc = %d...return false...\n", rc);
        func_args->thread_complete_success = false;
        return thread_param;
    }


    printf("ssolusa; Sleep ms...\n");
    usleep(func_args->wait_to_release_ms);


    printf("ssolusa; mutex_unlock()...\n");
    rc = pthread_mutex_unlock(func_args->mutex);
    if ( rc != 0 ) {
        printf("ssolusa; ERROR: pthread_mutex_unlock() failed; rc = %d...return false...\n", rc);
        func_args->thread_complete_success = false;
        return thread_param;
    }


    printf("ssolusa; threadfunc() END...rc = %d...returning...return func_args...\n", rc);
    func_args->thread_complete_success = true;
    return thread_param;
}


bool start_thread_obtaining_mutex(
    pthread_t *thread,           // input is "&thread"
    pthread_mutex_t *mutex,      // input is "&mutex"
    int wait_to_obtain_ms,       // input is "1"
    int wait_to_release_ms)      // input is "1"
    {

    // TODO:
    //     - Allocate memory for thread_data.
    //     - Setup mutex and wait arguments
    //     - Pass thread_data to created thread using threadfunc() as entry point.
    //     - Return true if successful.
    // See implementation details in threading.h file comment block


    printf("ssolusa; start_thread_obtaining_mutex() START...\n");


    int rc;


    struct thread_data* input_data;
    input_data = (struct thread_data*) malloc(sizeof(struct thread_data));
    if ( input_data == NULL ) {
        return false;
    }
    input_data->mutex = mutex;
    input_data->wait_to_obtain_ms = wait_to_obtain_ms;
    input_data->wait_to_release_ms = wait_to_release_ms;
    input_data->thread_complete_success = false;


/*  SOURCE OF ERROR with unit-test.sh
    printf("ssolusa; mutex_init()...\n");
    int rc = pthread_mutex_init(mutex, NULL);
    if ( rc != 0 ) {
        printf("ssolusa; ERROR: pthread_mutex_init() failed; rc = %d...return false...\n", rc);
        free(input_data);
        return false;
    }
*/


    printf("ssolusa; pthread_create()...\n");
    rc = pthread_create(thread,
                        NULL,
                        threadfunc,
                        input_data);    // See threading.h
    if ( rc != 0 ) {
        printf("ssolusa; ERROR: pthread_create() failed; rc = %d...return false...\n", rc);
        free(input_data);
        return false;
    }


    printf("ssolusa; start_thread_obtaining_mutex() END...rc = %d...returning...\n", rc);
    return true;    // Was false
}

