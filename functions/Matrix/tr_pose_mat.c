#include "typedef.h"
#include "matrix.h"
/**************************************************************************\
@---------------------------------------------------------------------------
@---------------------------------------------------------------------------
@ FILE: tr_pose_mat.c
@---------------------------------------------------------------------------
@---------------------------------------------------------------------------
@
\**************************************************************************/

/**************************************************************************\
@---------------------------------------------------------------------------
@ matrix_TYP *tr_pose(mat)
@ matrix_TYP *mat;
@
@ calculates the tansposed of mat.
@---------------------------------------------------------------------------
@
\**************************************************************************/

/*{{{}}}*/
/*{{{  tr_pose*/
matrix_TYP *
tr_pose (matrix_TYP *mat)
{
int i,j;
matrix_TYP *cmat;
int  **cZ, **cN, **Z, **N;

  if (mat->array.N != NULL ) {
    cmat = init_mat(mat->cols,mat->rows,"r");
    N  =  mat->array.N;
    cN = cmat->array.N;
  } else{
    cmat = init_mat(mat->cols,mat->rows,"");
    N = 0;
    cN = 0;
  }
  Z  =  mat->array.SZ;
  cZ = cmat->array.SZ;

  cmat->flags = mat->flags;

  cmat->kgv= mat->kgv;
  cmat->prime = mat->prime;

  for(i = 0; i < cmat->rows; i++) {
    for(j = 0; j < cmat->cols; j++) {
      cZ[i][j] = Z[j][i];
    }
  }
  if( cN != NULL) {
    for(i = 0; i < cmat->rows; i++) {
      for(j = 0; j < cmat->cols; j++) {
        cN[i][j] = N[j][i];
      }
    }
  }
  return cmat;
}

/*}}}  */

