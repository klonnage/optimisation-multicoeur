#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#define SIZE 1000

void _timersub(struct timeval* a, struct timeval *b, struct timeval *res) {
    if(b -> tv_usec - a -> tv_usec < 0) {
        res -> tv_usec = 1000000 + b -> tv_usec - a -> tv_usec;
        res -> tv_sec = b -> tv_sec - a -> tv_sec - 1;
    } else {
        res -> tv_usec = b -> tv_usec - a -> tv_usec;
        res -> tv_sec = b -> tv_sec - a -> tv_sec;
    }
}

void print_time(struct timeval* perf, char sep) {
    printf("%ld.%06ld%c", perf -> tv_sec, perf -> tv_usec, sep);
}

void prod(float *a,float *b,float *tmp)  {
    int i,j,k;
    for (j=0; j<SIZE; j++)
        for (i=0; i<SIZE; i++)
            tmp[i*SIZE+j]=0;
    for (k=0; k<SIZE; k++)
        for (j=0; j<SIZE; j++)
            for (i=0; i<SIZE; i++)
                tmp[i*SIZE+j] += a[i*SIZE+k]*b[k*SIZE+j];
}

void prod_locality(float *a,float *b,float *tmp)  {
    int i,j,k;
    memset(tmp, 0, sizeof(float) * SIZE * SIZE);
    for (k=0; k<SIZE; k++)
        for (j=0; j<SIZE; j++)
            for (i=0; i<SIZE; i++)
                tmp[i*SIZE+j] += a[i*SIZE+k]*b[k*SIZE+j];
}

void sum(float *a,float *b,float *tmp)  {
    int i,j;
    for (j=0; j<SIZE; j++)
        for (i=0; i<SIZE; i++)
	        tmp[i*SIZE+j] = a[i*SIZE+j]+b[i*SIZE+j];;			    
}

void sum_locality(float *a,float *b,float *tmp)  {
    int i,j;
    for (i=0; i<SIZE; i++)
       for (j=0; j<SIZE; j++)
	        tmp[i*SIZE+j] = a[i*SIZE+j]+b[i*SIZE+j];;			    
}

void prodpoly(float *a,float *b,float *c)  {
    int k;
    float *tmp=(float*)malloc(sizeof(float)*SIZE*SIZE);
    for (k=0; k<SIZE; k++)  {
        struct timeval start, end, delta;
        gettimeofday(&start, NULL);
        prod(&a[k*SIZE*SIZE],c,tmp);
        sum(tmp,b,c);
        gettimeofday(&end, NULL);
        _timersub(&start, &end, &delta);
        print_time(&delta, ',');
        printf("prod_sum\n");
        fflush(stdout);
    }
}

#ifndef timersub
#define timersub _timersub
#endif

int main() {
    int i,j,k;
    float *a,*b,*c;
    float s=0;
    a=(float *)malloc(sizeof(float)*SIZE*SIZE*SIZE);
    b=(float*)malloc(sizeof(float)*SIZE*SIZE);
    c=(float*)malloc(sizeof(float)*SIZE*SIZE);
    
    /* Initialization */
    struct timeval start, end, delta;
    gettimeofday(&start, NULL);
    for (i=0; i<SIZE; i++)    
        for (j=0; j<SIZE; j++)    
            c[i*SIZE+j]=b[i*SIZE+j]=i+1./(j+1);
    gettimeofday(&end, NULL);
    _timersub(&start, &end, &delta);
    print_time(&delta, ',');
    printf("init_BC\n");

    gettimeofday(&start, NULL);
    for (k=0; k<SIZE; k++)    
        for (i=0; i<SIZE; i++)    
            for (j=0; j<SIZE; j++)    
                a[k*SIZE*SIZE+i*SIZE+j]=i+j+k;
    gettimeofday(&end, NULL);
    _timersub(&start, &end, &delta);
    print_time(&delta, ',');
    printf("init_A\n");

    exit(EXIT_SUCCESS);

    //struct timeval start, end, delta;
    gettimeofday(&start, NULL);
    prodpoly(a,b,c);
    gettimeofday(&end, NULL);
    _timersub(&start, &end, &delta);
    print_time(&delta, ',');
    printf("total_app\n");
    for (j=0; j<SIZE; j++)    
        for (i=0; i<SIZE; i++)    
            s+=c[i*SIZE+j];
    fprintf(stderr,"\n%f",s);
    free(a);
    free(b);
    return 0;
}

