#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src) {
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            dst[y + x * n] = src[x + y * n];
        }
    }
}

/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int *dst, int *src) {
    int i = 0, j;
    for (; i + blocksize < n; i += blocksize) {
        for (j = 0; j + blocksize < n; j += blocksize) {
            int *temp_dst = dst + j + i * n;
            int *temp_src = src + i + j * n;
            for (int x = 0; x < blocksize; x++) {
                for (int y = 0; y < blocksize; y++) {
                    temp_dst[x + y * n] = temp_src[y + x * n];
                }
            }
        }
    }

    int m = i;
    for (; i < n; i++) {
        for (int k = 0; k < n; k++) {
            dst[k + i * n] = src[i + k * n];
        }
    }
    for (; j < n; j++) {
        for (int p = 0; p < m; p++) {
            dst[j + p * n] = src[p + j * n];
        }
    }
}

void benchmark(int *A, int *B, int n, int blocksize,
               void (*transpose)(int, int, int *, int *), char *description) {

    int i, j;
    printf("Testing %s: ", description);

    /* initialize A,B to random integers */
    srand48(time(NULL));
    for (i = 0; i < n * n; i++) A[i] = lrand48();
    for (i = 0; i < n * n; i++) B[i] = lrand48();

    /* measure performance */
    struct timeval start, end;

    gettimeofday(&start, NULL);
    transpose(n, blocksize, B, A);
    gettimeofday(&end, NULL);

    double seconds = (end.tv_sec - start.tv_sec) +
                     1.0e-6 * (end.tv_usec - start.tv_usec);
    printf("%g milliseconds\n", seconds * 1e3);
    /* check correctness */
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (B[j + i * n] != A[i + j * n]) {
                printf("Error!!!! Transpose does not result in correct answer!!\n");
                exit(-1);
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: transpose <n> <blocksize>\nExiting.\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    int blocksize = atoi(argv[2]);

    /* allocate an n*n block of integers for the matrices */
    int *A = (int *) malloc(n * n * sizeof(int));
    int *B = (int *) malloc(n * n * sizeof(int));

    /* run tests */
    benchmark(A, B, n, blocksize, transpose_naive, "naive transpose");
    benchmark(A, B, n, blocksize, transpose_blocking, "transpose with blocking");

    /* release resources */
    free(A);
    free(B);
    return 0;
}

/*
sean@sean-Lenovo-XiaoXin-310-15IKB:~/course/CS61C-2015/week9/08$ ./transpose 100 20
Testing naive transpose: 0.02 milliseconds
Testing transpose with blocking: 0.018 milliseconds
sean@sean-Lenovo-XiaoXin-310-15IKB:~/course/CS61C-2015/week9/08$ ./transpose 500 20
Testing naive transpose: 0.773 milliseconds
Testing transpose with blocking: 0.569 milliseconds
sean@sean-Lenovo-XiaoXin-310-15IKB:~/course/CS61C-2015/week9/08$ ./transpose 1000 20
Testing naive transpose: 1.413 milliseconds
Testing transpose with blocking: 1.03 milliseconds
sean@sean-Lenovo-XiaoXin-310-15IKB:~/course/CS61C-2015/week9/08$ ./transpose 2000 20
Testing naive transpose: 15.525 milliseconds
Testing transpose with blocking: 4.498 milliseconds
sean@sean-Lenovo-XiaoXin-310-15IKB:~/course/CS61C-2015/week9/08$ ./transpose 5000 20
Testing naive transpose: 139.787 milliseconds
Testing transpose with blocking: 33.558 milliseconds
sean@sean-Lenovo-XiaoXin-310-15IKB:~/course/CS61C-2015/week9/08$ ./transpose 10000 20
Testing naive transpose: 703.791 milliseconds
Testing transpose with blocking: 235.959 milliseconds
sean@sean-Lenovo-XiaoXin-310-15IKB:~/course/CS61C-2015/week9/08$ ./transpose 10000 50
Testing naive transpose: 703.618 milliseconds
Testing transpose with blocking: 263.444 milliseconds
sean@sean-Lenovo-XiaoXin-310-15IKB:~/course/CS61C-2015/week9/08$ ./transpose 10000 100
Testing naive transpose: 702.409 milliseconds
Testing transpose with blocking: 203.267 milliseconds
sean@sean-Lenovo-XiaoXin-310-15IKB:~/course/CS61C-2015/week9/08$ ./transpose 10000 500
Testing naive transpose: 707.946 milliseconds
Testing transpose with blocking: 222.404 milliseconds
sean@sean-Lenovo-XiaoXin-310-15IKB:~/course/CS61C-2015/week9/08$ ./transpose 10000 1000
Testing naive transpose: 705.71 milliseconds
Testing transpose with blocking: 291.137 milliseconds
 */