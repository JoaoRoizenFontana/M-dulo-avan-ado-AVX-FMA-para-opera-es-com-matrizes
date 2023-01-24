/*
Joao Roizen Fontana - 1710431
Pedro Arduini - 2110132
*/

#include "matrix_lib.h"
#include <immintrin.h>

#define AVX_STEP 8

/*int scalar_matrix_mult(float scalar_value, struct matrix *matrix){
	int i;
	unsigned long itens;
	float *aux;

	if(matrix == NULL){
		return 0;
	}
	
	itens = matrix->height * matrix->width;
	
	aux = matrix->rows;
	for(i=0;i<itens;i++){
	
		*aux *= scalar_value;
		
		aux ++; 
	}
	
	return 1;
}*/


/*int matrix_matrix_mult(struct matrix *matrixA, struct matrix *matrixB, struct matrix *matrixC){
	int i,j,k;
	float *auxA;
	float *auxB;
	float *auxC;	

	if(matrixA == NULL || matrixB == NULL || matrixC == NULL || matrixA->width != matrixB->height){
		return 0;
	}

	auxA = matrixA->rows;
	auxB = matrixB->rows;
	auxC = matrixC->rows;
	


	for(i=0;i<matrixA->height;i++){
		for(j=0;j<matrixA->width;j++){		
				for(k=0;k<matrixB->width;k++){
					auxC[i*matrixB->width+k] += auxA[(i*matrixA->width)+j] * auxB[j*matrixB->width + k];	
					
				}


			
		}


	}


	return 1;
}*/


int scalar_matrix_mult(float scalar_value, struct matrix *matrix)
{
  if(matrix == NULL){
    printf("\nMatriz não declarada.\n");
    return 0;
  }

  __m256 vector, scalar, result;

  float * arrayNext = matrix->rows;

  for(int i = 0; i < matrix->height*matrix->width; i+=AVX_STEP, arrayNext+=AVX_STEP){

    vector = _mm256_load_ps(arrayNext);
    scalar = _mm256_set1_ps(scalar_value);

    result = _mm256_mul_ps(vector, scalar);
      
    _mm256_store_ps(arrayNext, result);
  }

  return 1;
}

int matrix_matrix_mult(struct matrix *matrix_a, struct matrix *matrix_b, struct matrix *matrix_c){ 
  if(matrix_a == NULL || matrix_b == NULL){
    printf("\nUma ou duas das matrizes não declaradas.\n");
    return 0;
  }

  if(matrix_a->width != matrix_b->height){
    printf("\nA matriz A deve ter o número de colunas igual ao número de linhas da matriz B.\n");
    return 0;
  }

  __m256 vectorA, vectorB, vectorC, result;

  float * arrayANext = matrix_a->rows;
  float * arrayBNext = matrix_b->rows;
  float * arrayCNext = matrix_c->rows;

  for(int i = 0; i < matrix_a->height*matrix_a->width; i++, arrayANext++){

    vectorA = _mm256_set1_ps(*arrayANext);
    arrayBNext = matrix_b->rows;

    int row = i / matrix_a->width;
    arrayCNext = matrix_c->rows + row * matrix_b->width;

    for(int k = 0; k < matrix_b->width; k+=AVX_STEP, arrayBNext+=AVX_STEP, arrayCNext+=AVX_STEP){
      vectorB = _mm256_load_ps(arrayBNext);
      vectorC = _mm256_load_ps(arrayCNext);

      result = _mm256_fmadd_ps(vectorA, vectorB, vectorC);
      
      _mm256_store_ps(arrayCNext, result);
    }

  }
  return 1;
}
