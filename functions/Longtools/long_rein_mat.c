#include "typedef.h"
#include "utils.h"
#include "gmp.h"
#include "longtools.h"
#include "matrix.h"
/**************************************************************************\
@---------------------------------------------------------------------------
@---------------------------------------------------------------------------
@ FILE: long_rein_mat.c
@---------------------------------------------------------------------------
@---------------------------------------------------------------------------
@
\**************************************************************************/


/**************************************************************************\
@---------------------------------------------------------------------------
@ matrix_TYP *long_rein_mat(Mat)
@ matrix_TYP *Mat;
@
@ long_rein_mat calculates a matrix M such that the rows of M
@ form a Z-basis of the lattice of all integral points in the
@ the vectorspace generated by the rows of Mat.
@---------------------------------------------------------------------------
@
\**************************************************************************/
matrix_TYP *
long_rein_mat (matrix_TYP *Mat)
{

   int i,j,k, step, stepclear;
   int cols, rows, rang;
   MP_INT **trf, **M, **Mt, **M1, **M2, **M3, **M4, **T1, *merkpointer;
   matrix_TYP *erg;
   MP_INT a1, a2, x1, x2, y1, y2, merk, g, f;
 
   cols = Mat->cols;
   rows = Mat->rows;
   mpz_init(&a1), mpz_init(&a2);
   mpz_init(&x1), mpz_init(&x2);
   mpz_init(&y1), mpz_init(&y2);
   mpz_init(&f), mpz_init(&g);
   mpz_init(&merk);
   /***************************************************************\
   | Set Mt= Mat^{tr} transform Mt to Hermite normal form.
   \***************************************************************/
   Mt = (MP_INT **)xmalloc(cols *sizeof(MP_INT *));
   for(i=0;i<cols;i++)
   {
     Mt[i] = (MP_INT *)xmalloc(rows *sizeof(MP_INT));
     for(j=0;j<rows;j++)
       mpz_init_set_si(&Mt[i][j], Mat->array.SZ[j][i]);
   }

   MP_hnf(Mt, cols, rows);
   /***************************************************************\
   | Set trf =  left_tans
   \***************************************************************/
     trf = (MP_INT **)xmalloc(rows *sizeof(MP_INT *));
     for(i=0;i<rows;i++)
     {
       trf[i] = (MP_INT *)xmalloc(rows *sizeof(MP_INT));
       for(j=0;j<rows;j++)
         mpz_init_set_si(&trf[i][j], 0);
       mpz_set_si(&trf[i][i], 1);
     }
   /***************************************************************\
   | Set M= Mt^{tr} transform Mt to Hermite normal form.
   \***************************************************************/
   M = (MP_INT **)xmalloc(rows *sizeof(MP_INT *));
   for(i=0;i<rows;i++)
   {
     M[i] = (MP_INT *)xmalloc(cols *sizeof(MP_INT));
     for(j=0;j<cols;j++)
       mpz_init_set(&M[i][j], &Mt[j][i]);
   }

   /* output for debugging purposes
   dump_MP_mat(trf,rows,rows,"trf");
   dump_MP_mat(M,rows,rows,"M"); */

   rang = MP_hnf_simultaneous(M, rows, cols, trf, rows);

   /* output for debugging purposes
   dump_MP_mat(trf,rows,rows,"trf");
   dump_MP_mat(M,rows,rows,"M"); */

   /***************************************************************\
   | Clear the space allocated for 'Mt'
   \***************************************************************/
   for(i=0;i<cols;i++)
   {
     for(j=0;j<rows;j++)
       mpz_clear(&Mt[i][j]);
     free(Mt[i]);
   }
   free(Mt);

   /***************************************************************\
   | Now the elementary divisor algorithm starts
   \***************************************************************/
   for(step = 0; step < rang;step++)
   {
      do
      {
         /*------------------------------------------------------*\
         | Clear the 'step'^th row of M
         \*------------------------------------------------------*/
          for(i=step;i<rang && mpz_cmp_si(&M[step][i], 0) == 0; i++);
          if(i!=step)
          {

            /* swap the step-th col with the i-th col */
            for(j=step;j<rang;j++)
            {
              mpz_set(&merk, &M[j][step]);
              mpz_set(&M[j][step], &M[j][i]);
              mpz_set(&M[j][i], &merk);
            }
          }
          if(mpz_cmp_si(&M[step][step], 0) < 0)
          {
            for(i=step;i<rows;i++)
              mpz_neg(&M[i][step], &M[i][step]);
          }
          for(i=step+1;i<rang;i++)
          {
            if(mpz_cmp_si(&M[step][i], 0) != 0)
            {
              if(mpz_cmp_si(&M[step][step], 1) == 0)
              {
                mpz_set(&f, &M[step][i]);
                for(j=step+1;j<rang;j++)
                {
                  mpz_mul(&merk, &M[j][step], &f);
                  mpz_sub(&M[j][i], &M[j][i], &merk);
                }
                mpz_set_si(&M[step][i], 0);

                   /*
                   for(j=0;j<rows;j++)
                   {
                     mpz_mul(&merk, &trf[j][step], &f);
                     mpz_sub(&trf[j][i], &trf[j][i], &merk);
                   }*/
              }
              else
              {
                 /* changed 11/06/99 (tilman) from
                 mpz_gcdext(&g, &a1, &a2, &M[step][step], &M[step][i]); to */
                 mpz_abs( &g,&M[step][i]);
                 if (mpz_cmp(&M[step][step],&g) == 0){
                    mpz_set_si(&a1,1);
                    mpz_set_si(&a2,0);
                 }
                 else{
                    mpz_gcdext(&g, &a1, &a2, &M[step][step], &M[step][i]);
                 }


                 mpz_div(&x1, &M[step][i], &g);
                 mpz_div(&x2, &M[step][step], &g);
                 mpz_neg(&x2, &x2);

                 for(j=step+1; j<rang;j++)
                 {
                    mpz_mul(&y1, &a1, &M[j][step]);
                    mpz_mul(&merk, &a2, &M[j][i]);
                    mpz_add(&y1, &y1, &merk);

                    mpz_mul(&y2, &x1, &M[j][step]);
                    mpz_mul(&merk, &x2, &M[j][i]);
                    mpz_add(&y2, &y2, &merk);

                    mpz_set(&M[j][step], &y1);
                    mpz_set(&M[j][i], &y2);
                 }
                 mpz_set(&M[step][step], &g);
                 mpz_set_si(&M[step][i], 0);

                 /*
                   for(j=0; j<rows;j++)
                   {
                      mpz_mul(&y1, &a1, &trf[j][step]);
                      mpz_mul(&merk, &a2, &trf[j][i]);
                      mpz_add(&y1, &y1, &merk);

                      mpz_mul(&y2, &x1, &trf[j][step]);
                      mpz_mul(&merk, &x2, &trf[j][i]);
                      mpz_add(&y2, &y2, &merk);

                      mpz_set(&trf[j][step], &y1);
                      mpz_set(&trf[j][i], &y2);
                   }*/
              }
            }
          }
         /*------------------------------------------------------*\
         | Clear the 'step'^th column of M
         \*------------------------------------------------------*/
          for(i=step;i<rang && mpz_cmp_si(&M[i][step], 0) == 0; i++);
          if(i!=step)
          {
            merkpointer = M[step];
            M[step] = M[i];
            M[i] = merkpointer;

              merkpointer = trf[step];
              trf[step] = trf[i];
              trf[i] = merkpointer;
          }
          if(mpz_cmp_si(&M[step][step], 0) < 0)
          {
            for(i=step;i<rows;i++)
              mpz_neg(&M[i][step], &M[i][step]);
          }
          for(i=step+1;i<rang;i++)
          {
            if(mpz_cmp_si(&M[i][step], 0) != 0)
            {
              if(mpz_cmp_si(&M[step][step], 1) == 0)
              {
                mpz_set(&f, &M[i][step]);
                for(j=step+1;j<rang;j++)
                {
                  mpz_mul(&merk, &M[step][j], &f);
                  mpz_sub(&M[i][j], &M[i][j], &merk);
                }
                mpz_set_si(&M[i][step], 0);

                /* inserted the next 4 lines: tilman 4/12/96 */
                for (j=0;j<rang;j++){
                  mpz_mul(&merk, &trf[step][j], &f);
                  mpz_sub(&trf[i][j], &trf[i][j], &merk);
                }

              }
              else
              {
                 /* changed on 8/1/97 tilman from
                 mpz_gcdext(&g, &a1, &a2, &M[step][step], &M[i][step]);
                 to : */
                 mpz_abs( &g,&M[i][step]);
                 if (mpz_cmp(&M[step][step],&g) == 0){
                    mpz_set_si(&a1,1);
                    mpz_set_si(&a2,0);
                 }
                 else{
                    mpz_gcdext(&g, &a1, &a2, &M[step][step], &M[i][step]);
                 }

                 mpz_div(&x1, &M[i][step], &g);
                 mpz_div(&x2, &M[step][step], &g);
                 mpz_neg(&x2, &x2);

                 for(j=step+1; j<rang;j++)
                 {
                    mpz_mul(&y1, &a1, &M[step][j]);
                    mpz_mul(&merk, &a2, &M[i][j]);
                    mpz_add(&y1, &y1, &merk);

                    mpz_mul(&y2, &x1, &M[step][j]);
                    mpz_mul(&merk, &x2, &M[i][j]);
                    mpz_add(&y2, &y2, &merk);

                    mpz_set(&M[step][j], &y1);
                    mpz_set(&M[i][j], &y2);
                 }

                 /* inserted the next 12 lines: 4/12/96 tilman */
                 for (j=0;j<rows;j++){
                    mpz_mul(&y1, &a1, &trf[step][j]);
                    mpz_mul(&merk, &a2, &trf[i][j]);
                    mpz_add(&y1, &y1, &merk);

                    mpz_mul(&y2, &x1, &trf[step][j]);
                    mpz_mul(&merk, &x2, &trf[i][j]);
                    mpz_add(&y2, &y2, &merk);

                    mpz_set(&trf[step][j], &y1);
                    mpz_set(&trf[i][j], &y2);
                 } 

                 mpz_set(&M[step][step], &g);
                 mpz_set_si(&M[i][step], 0);
              }
            }
          }
         /*------------------------------------------------------*\
         | Check whether the column and row are both clear
         \*------------------------------------------------------*/
         stepclear = TRUE;
         for(i=step+1;i<cols && stepclear == TRUE;i++)
         {
           if(mpz_cmp_si(&M[step][i], 0) != 0)
             stepclear = FALSE;
         }
         for(i=step+1;i<rows && stepclear == TRUE;i++)
         {
           if(mpz_cmp_si(&M[i][step], 0) != 0)
             stepclear = FALSE;
         }
      }while(stepclear == FALSE);
      if(mpz_cmp_si(&M[step][step], 0) < 0)
         mpz_neg(&M[step][step], &M[step][step]);

      /* output for debugging purposes
      printf("step = %d\n",step);
      dump_MP_mat(trf,rows,rows,"trf");
      dump_MP_mat(M,rows,rows,"M"); */

   }

   /* output for debugging purposes
   dump_MP_mat(trf,rows,rows,"trf");
   dump_MP_mat(M,rows,rows,"M"); */

   /*-------------------------------------------------------------------*\
   |  Multiply trf with Mat
   \*-------------------------------------------------------------------*/
   M1 = matrix_to_MP_mat(Mat);
   M2 = (MP_INT **)xmalloc(rang *sizeof(MP_INT *));
   for(i=0;i<rang;i++)
   {
     M2[i] = (MP_INT *)xmalloc(cols *sizeof(MP_INT));
     for(j=0;j<cols;j++)
        mpz_init(&M2[i][j]);
   }
   for(i=0;i<rang;i++)
     for(j=0;j<cols;j++)
     {
        for(k=0;k<rows;k++)
        {
          mpz_mul(&merk, &trf[i][k], &M1[k][j]);
          mpz_add(&M2[i][j], &M2[i][j], &merk);
        }

        /* changed 4/12/96 tilman from:
        mpz_div(&M2[i][j], &M2[i][i], &M[i][i]);
        to : */
        mpz_div(&M2[i][j], &M2[i][j], &M[i][i]);

     }

   /* output for debugging purposes
   dump_MP_mat(M2,rows,rows,"M2"); */

   /***************************************************************\
   | make a pair_reduction to the rows of M2
   \***************************************************************/
   M3 = init_MP_mat(rang, rang);
   for(i=0;i<rang;i++)
     for(j=0;j<=i;j++)
     {
       for(k=0;k<cols;k++)
       {
        mpz_mul(&merk, &M2[i][k], &M2[j][k]);
        mpz_add(&M3[i][j], &merk, &M3[i][j]);
       }
     }
   for(i=0;i<rang;i++)
     for(j=0;j<i;j++)
       mpz_set(&M3[j][i], &M3[i][j]);

   /* output for debugging purposes
   dump_MP_mat(M3,rows,rows,"M3"); */

   T1 = init_MP_mat(rang, rang);
   for(i=0;i<rang;i++)
     mpz_set_si(&T1[i][i], 1);
   MP_pair_red(M3, T1, rang);

   /* output for debugging purposes
   dump_MP_mat(T1,rows,rows,"T1"); */

   M4 = init_MP_mat(rang, cols);
   for(i=0;i<rang;i++)
    for(j=0;j<cols;j++)
      for(k=0;k<rang;k++)
      {
         mpz_mul(&merk, &T1[i][k], &M2[k][j]);
         mpz_add(&M4[i][j], &M4[i][j], &merk);
      }
   
   /* output for debugging purposes
   dump_MP_mat(M4,rows,rows,"M4"); */

   erg = MP_mat_to_matrix(M4, rang, cols);

   free_MP_mat(M, rows, cols); free(M);
   free_MP_mat(M1, rows, cols); free(M1);
   free_MP_mat(M2, rang, cols); free(M2);
   free_MP_mat(M3, rang, rang); free(M3);
   free_MP_mat(M4, rang, cols); free(M4);
   free_MP_mat(trf, rows,rows); free(trf);
   /* changed 11/3/97 tilman from
   free_MP_mat(T1, rang, rang); free(M2);
   to: */
   free_MP_mat(T1, rang, rang); free(T1);
   merkpointer = NULL;
   mpz_clear(&a1), mpz_clear(&a2);
   mpz_clear(&x1), mpz_clear(&x2);
   mpz_clear(&y1), mpz_clear(&y2);
   mpz_clear(&f), mpz_clear(&g);
   mpz_clear(&merk);
   return(erg);
}

/*************************************************************************
@
@------------------------------------------------------------------------
@
@ void long_rein_formspace(matrix_TYP **forms,int number,int option)
@
@ option:       drives the behaviour of the function, for
@               option == 0 there is no assumption on the symetry
@                           of these matrices made.
@               option == 1 the matrices are assumed to be symetric.
@               option == 2 the matrices are assumed to be skew symetric.
@
@------------------------------------------------------------------------
@
**************************************************************************/
int long_rein_formspace(matrix_TYP **forms,int number,int option)
{
   int i,
       j,
       k,
       l,
       rank,
       dim = forms[0]->cols;

   matrix_TYP *rein,
              *res;

   if (option == 0){
      rein = init_mat(number,dim*dim,"");
   }
   else if (option == 1){
      rein = init_mat(number,(dim*(dim+1))/2,"");
   }
   else if (option == 2){
      rein = init_mat(number,(dim*(dim+2))/2,"");
   }
   else{
      fprintf(stderr,"you must specify one of the options 0,1,2 in");
      fprintf(stderr," long_rein_formspace\n");
      exit(1);
   }

   for (i=0;i<number;i++){
      l=0;
      for (j=0;j<dim;j++){
         if (option == 0){
            for (k=0;k<dim;k++){
               rein->array.SZ[i][l] = forms[i]->array.SZ[j][k];
               l++;
            }
         }
         else if (option == 1){
            for (k=0;k<=j;k++){
               rein->array.SZ[i][l] = forms[i]->array.SZ[j][k];
               l++;
            }
         }
         else{
            for (k=0;k<j;k++){
               rein->array.SZ[i][l] = forms[i]->array.SZ[j][k];
               l++;
            }
         }
      }
   } 

   res = long_rein_mat(rein);

   for (i=0;i<res->rows;i++){
      l=0;
      for (j=0;j<dim;j++){
         if (option == 0){
            for (k=0;k<dim;k++){
               forms[i]->array.SZ[j][k] = res->array.SZ[i][l];
               l++;
            }
         }
         else if (option == 1){
            for (k=0;k<=j;k++){
               forms[i]->array.SZ[j][k] = res->array.SZ[i][l];
               forms[i]->array.SZ[k][j] = res->array.SZ[i][l];
               l++;
            }
         }
         else{
            for (k=0;k<j;k++){
               forms[i]->array.SZ[j][k] = res->array.SZ[i][l];
               forms[i]->array.SZ[k][j] = -res->array.SZ[i][l];
               l++;
            }
            forms[i]->array.SZ[j][j] = 0;
         }
      }
   } 

   rank = res->rows;

   for (i=0;i<number;i++) Check_mat(forms[i]);

   free_mat(rein);
   free_mat(res);

   return rank;
}
