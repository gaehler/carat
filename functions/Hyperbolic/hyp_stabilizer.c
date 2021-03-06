#include "typedef.h"
#include "utils.h"
#include "autgrp.h"
#include "matrix.h"
#include "datei.h"

/************************************************************************** \
@---------------------------------------------------------------------------
@---------------------------------------------------------------------------
@ FILE: hyp_stabilizer.c
@---------------------------------------------------------------------------
@---------------------------------------------------------------------------
@
\**************************************************************************/


/**************************************************************************\
@---------------------------------------------------------------------------
@ bravais_TYP *hyperbolic_stabilizer(x, S)
@ matrix_TYP *x, *S;
@
@ the functions 'hyperbolic_stabilizer' calculates generators 
@ for a finite subgroup G of GL_n(Q) with
@    vg = v   and gSg^{tr} = S for all g in G.
@ where v denotes the first row of x.
@ The group H of all integral matrices with this property is
@ a subgroup of H.
@---------------------------------------------------------------------------
@
\**************************************************************************/
bravais_TYP *
hyperbolic_stabilizer (matrix_TYP *x, matrix_TYP *S)
{
  matrix_TYP *xS, *xS0, **Sred, *T, *Ti, *W, *W1;
  bravais_TYP *G1, *G;
  int i,j,k, n;

  n = x->cols;
  xS = mat_mul(x, S);
  xS0 = solve_mat(xS);
  Sred = (matrix_TYP **)xmalloc(1*sizeof(matrix_TYP *));
  Sred[0] = scal_pr(xS0, S, 1);
  G1 = pr_aut(Sred, 1, NULL, 0, NULL);
  G = (bravais_TYP *)xmalloc(sizeof(bravais_TYP));
  G->gen_no = G1->gen_no;
  G->gen = (matrix_TYP **)xmalloc(G->gen_no *sizeof(matrix_TYP *));
  G->zentr_no = G->form_no = G->normal_no = G->cen_no = 0;
  for(i=0;i<100;i++)
   G->divisors[i] = G1->divisors[i];
  G->order = G1->order;
  G->dim = n;
  T = init_mat(n,n,"");
  for(i=0;i<n-1;i++)
  {
    for(j=0;j<n;j++)
     T->array.SZ[i][j] = xS0->array.SZ[i][j];
  }
  for(i=0;i<n;i++)
     T->array.SZ[n-1][i] = x->array.SZ[0][i];
  Ti = mat_inv(T);
  W = init_mat(n,n,"");
  W->array.SZ[n-1][n-1] = 1;
  for(i=0;i<G->gen_no;i++)
  {
    for(j=0;j<n-1;j++)
      for(k=0;k<n-1;k++)
        W->array.SZ[j][k] = G1->gen[i]->array.SZ[j][k];
    W1 = mat_mul(Ti, W);
    G->gen[i] = mat_mul(W1, T);
    free_mat(W1); 
  }
  free_mat(W); free_mat(T); free_mat(Ti);
  free_bravais(G1);
  free_mat(xS); free_mat(xS0); free_mat(Sred[0]); free(Sred);
  return(G);
}
