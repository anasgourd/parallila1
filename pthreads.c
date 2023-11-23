#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define  num_threads 4
typedef struct {
    double *values;
    int *indptr;
    int *indices;
} CSRMatrix;

typedef struct {
    double *values;
    int *indptr;
    int *indices;
} CSCMatrix;

typedef struct {
    CSRMatrix matrix;
    int *vector;
    CSCMatrix result;
    int col_start;
    int col_end;
} ThreadSplit;

pthread_mutex_t total_nnz;
void *calc(void *arg){

    ThreadSplit; *data = ( ThreadSplit; *)arg; //to data tha exei ta strut tvn thread pou perasame mesv tou orismatos arg
    int s, val, start_idx, end_idx;
    double el;
   total_nnz=csc.indptr[0];
    for (int col =data->col_start; col < data->col_end; col++){
	nnz=0;
	for(int i=0; i<n;i++){        //kanei tis prakseis gia kathe thread 
	    val = 0;
            start_idx = data->matrix.indptr[i];
            end_idx = data->matrix.indptr[i + 1];
	  for (int j = start_idx; j < end_idx; j++) {
                s = data->matrix.indices[j];
                el = data->matrix.values[j];

                if (data->vector[s] == col) {
                    val += el;
                }

	}
	pthread_mutex_lock(&total_nnz);

	total_nnz+=nnz;         //grafei ston csc stin katallili thesi
	 if (val != 0) {
	   csc.values[nnz]=val;
           csc.indices[nnz]=i;

	
	}
	csc.indptr[col] = total_nnz;
	pthread_mutex_lock(&total_nnz);

  }

	 return NULL;
 
}

CSCMatrix parallel_sparse_matrix_multiplication(CSRMatrix matrix, int *vector, int c, int n,int num_threads) {
    CSCMatrix csc;
    csc.values = NULL;
    csc.indices = NULL;
    csc.indptr = NULL;

    csc.values = (double *)malloc(matrix.indptr[n] * sizeof(double));
    csc.indices = (int *)malloc(matrix.indptr[n] * sizeof(int));
    csc.indptr = (int *)malloc((c + 1) * sizeof(int));

    if (csc.values == NULL || csc.indices == NULL || csc.indptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(csc.values);
        free(csc.indices);
        free(csc.indptr);
        return ERROR_CSC_MATRIX;
    }
  
    pthread_t threads[num_threads];  // 1.orizv ta threads

	ThreadSplit thread_data[num_threads]; //2.orizv pinaka apo struct Threadsplit .kath ena tha antistoixei sto kommati tou pinaka pou douleuei to thread
int cols_per_thread = c / num_threads;

for (int i = 0; i < num_threads; i++) {
        thread_data[i].matrix = matrix;
        thread_data[i].vector = vector;
        thread_data[i].result = csc;
        thread_data[i].col_start = i * cols_per_thread;
        thread_data[i].col_end = (i == num_threads - 1) ? c : (i + 1) * cols_per_thread;  //3.xvrizv tis c stilles tou tlkou pinaka ana thread
	 pthread_create(&threads[i], NULL, calc, (void *)&thread_data[i]); //4.dhmiourgv ta thread k ta bazv na ektelesoun kathena ti synartisi ypologismou 

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);           //5.perimenv na teleivsoun ola ta thread
    }

	 pthread_mutex_destroy(&total_nnz);
	
	return csc;
	//kanonika synexeia gia csr...
	
	
}
	


