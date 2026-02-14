#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
// #include <pthread.h>    // ssolusa; is already in threading.h


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


    struct thread_data* thread_data = (struct thread_data*) thread_param;


    printf("ssolusa; threadfunc() START...\n");


    printf("ssolusa; sleep ms...\n");
    usleep(thread_data->wait_to_obtain_ms);    // ssolusa; sleep 1ms before locking


    printf("ssolusa; mutex_lock()...\n");
    int rc = pthread_mutex_lock(thread_data->mutex);
    if ( rc != 0 ) {
        printf("ssolusa; ERROR: pthread_mutex_lock() failed; rc = %d...return false...\n", rc);
        thread_data->thread_complete_success = false;
        return thread_param;
    }


    printf("ssolusa; Sleep ms...\n");
    usleep(thread_data->wait_to_release_ms);    // ssolusa; wait 1ms after locking.


    printf("ssolusa; mutex_unlock()...\n");
    rc = pthread_mutex_unlock(thread_data->mutex);
    if ( rc != 0 ) {
        printf("ssolusa; ERROR: pthread_mutex_unlock() failed; rc = %d...return false...\n", rc);
        thread_data->thread_complete_success = false;
        return thread_param;
    }


    printf("ssolusa; threadfunc() END...rc = %d...returning...return func_args...\n", rc);
    thread_data->thread_complete_success = true;
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


    struct thread_data* thread_data = malloc(sizeof(struct thread_data));
    if ( !thread_data ) {
        return false;
    }
    thread_data->mutex = mutex;
    thread_data->wait_to_obtain_ms = wait_to_obtain_ms;
    thread_data->wait_to_release_ms = wait_to_release_ms;
    thread_data->thread_complete_success = false;


    printf("ssolusa; mutex_init()...\n");
    int rc = pthread_mutex_init(mutex, NULL);
    if ( rc != 0 ) {
        printf("ssolusa; ERROR: pthread_mutex_init() failed; rc = %d...return false...\n", rc);
        free(thread_data);
        return false;
    }


    printf("ssolusa; pthread_create()...\n");
    rc = pthread_create(thread,
                        NULL,
                        threadfunc,
                        thread_data);    // See threading.h
    if ( rc != 0 ) {
        printf("ssolusa; ERROR: pthread_create() failed; rc = %d...return false...\n", rc);
        free(thread_data);
        return false;
    }


    // printf("ssolusa; sleep 50ms before returning...\n");
    // usleep(50);

/*
    printf("ssolusa; try_join()...\n");
    rc = pthread_tryjoin_np(thread, NULL);
    if ( rc == EBUSY ) {
        printf("**********EBUSY***********\n");
    } else {
        printf("EBUSY rc = %d\n", rc);
    }
*/

    printf("ssolusa; pthread_join()...\n");
    rc = pthread_join(*thread, NULL);
    if ( rc != 0 ) {
        printf("ssolusa; ERROR: pthread_join() failed; rc = %d...return false...\n", rc);
        free(thread_data);
        return false;
    }


    pthread_mutex_destroy(mutex);
    free(thread_data);


    // printf("ssolusa; pthread_exit()...\n");     // Don't use; ends ./unit-test.sh early
    // pthread_exit(thread);


    // printf("ssolusa; pthread_detach()...\n");
    // pthread_detach(*thread);
    // pthread_mutex_destroy(&mutex->mutex);


    printf("ssolusa; start_thread_obtaining_mutex() END...rc = %d...returning...\n", rc);
    // return false;    // Part of original
    return true;        // Outputs to thread_started; used in TEST_ASSERT_TRUE_MESSAGE(thread_started)
}

