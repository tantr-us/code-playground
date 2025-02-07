#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Common.h"
#include "LList.h"
#include "Request.h"
#include "Response.h"
#include "Pool.h"

int main()
{
    int fd;

    printf(" *** Obtaining mkfifo...");
    if (mkfifo(FIFO_PATH, FIFO_PERMISSION) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    printf("Done\n");

    pool_list = LList_init(); // initialize pool list

    printf("Server started.\n");
    while (TRUE)
    {
        
        Request request;
        request.process_id = -1;
        printf("Waiting for request...\n");
        
        // // Open FIFO
        // while((fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK)) == -1)
        // {
        //     perror("open");
        //     printf("Retry opening FIFO...\n");
        //     usleep(1000000); // Sleep for 100ms before retrying
        // }
        // while (read(fd, &request, sizeof(Request)) == -1)
        // {
        //     perror("read");
        //     printf("Retry reading request...\n");
        //     usleep(1000000); // Sleep for 100ms before retrying
        // }
        
            // open semaphore
        fd = open(FIFO_PATH, O_RDONLY);
        read(fd, &request, sizeof(Request));
        close(fd);

        handle_request(request, pool_list);
        PoolList_print(pool_list);

        // if (request.process_id != -1)
        // {
        //     handle_request(request, pool_list);
        //     print_pool_list(pool_list);
        // }
    }


    return 0;
}