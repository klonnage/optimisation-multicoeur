#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#define SIZE 1000

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

void sum(float *a,float *b,float *tmp)  {
    int i,j;
    for (j=0; j<SIZE; j++)
        for (i=0; i<SIZE; i++)
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
        timersub(&start, &end, &delta);
        printf("%ld.%06ld\n", delta.tv_sec, delta.tv_usec);
        fflush(stdout);
    }
}

#ifndef timersub
void _timersub(struct timeval* a, struct timeval *b, struct timeval *res) {
    if(b -> tv_usec - a -> tv_usec < 0) {
        res -> tv_usec = 1000000 + b -> tv_usec - a -> tv_usec;
        res -> tv_usec = b -> tv_sec - a -> tv_sec - 1;
    } else {
        res -> tv_usec = b -> tv_usec - a -> tv_usec;
        res -> tv_usec = b -> tv_sec - a -> tv_sec;
    }
}
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
    for (j=0; j<SIZE; j++)    
        for (i=0; i<SIZE; i++)    
            c[i*SIZE+j]=b[i*SIZE+j]=i+1./(j+1);
    for (j=0; j<SIZE; j++)    
        for (i=0; i<SIZE; i++)    
            for (k=0; k<SIZE; k++)    
                a[k*SIZE*SIZE+i*SIZE+j]=i+j+k;
    
    struct timeval start, end, delta;
    gettimeofday(&start, NULL);
    prodpoly(a,b,c);
    gettimeofday(&end, NULL);
    timersub(&start, &end, &delta);
    printf("%ld.%06ld\n", delta.tv_sec, delta.tv_usec);
    for (j=0; j<SIZE; j++)    
        for (i=0; i<SIZE; i++)    
            s+=c[i*SIZE+j];
    fprintf(stderr,"\n%f",s);
    free(a);
    free(b);
    return 0;
}

