#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define BUFSIZE 32

void aio_completion_handler(sigval_t sigval)
{
    struct aiocb *req;
    req = (struct aiocb *)sigval.sival_ptr;
    if (aio_error(req) == 0) {
        int ret = aio_return(req);
        printf("read %d bytes\n", ret);
    }
    return;
}

void setup_aiocb(struct aiocb *aio, int fd, off_t offset, int bytes, char *buf, void (*func)(sigval_t))
{
    aio->aio_fildes = fd;
    aio->aio_nbytes = bytes;
    aio->aio_offset = offset;
    aio->aio_buf = buf;
    aio->aio_sigevent.sigev_notify = SIGEV_THREAD;
    aio->aio_sigevent.sigev_notify_function = func;
    aio->aio_sigevent.sigev_notify_attributes = NULL;
    aio->aio_sigevent.sigev_value.sival_ptr = aio;
}

int main(int argc, char **argv)
{
    int r_fd, ret, nread, quit = 0;
    struct aiocb r_aiocb;
    char r_buf[BUFSIZE];

    if (argc < 2) {
        fprintf(stderr, "testaiocallback source\n");
        exit(1);
    }

    r_fd = open(argv[1], O_RDONLY, 0444);
    if (r_fd < 0) perror("open");

    bzero((char *)&r_aiocb, sizeof(struct aiocb));
    setup_aiocb(&r_aiocb, r_fd, 0, BUFSIZE, r_buf, aio_completion_handler);

    ret = aio_read(&r_aiocb);

    sleep(1);

    return 0;
}
