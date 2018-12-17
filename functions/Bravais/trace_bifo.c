#include "typedef.h"
#include "tools.h"

/************************************************************************** \
@---------------------------------------------------------------------------
@---------------------------------------------------------------------------
@ FILE: trace_bifo.c
@---------------------------------------------------------------------------
@---------------------------------------------------------------------------
@
\**************************************************************************/

/**************************************************************************\
@---------------------------------------------------------------------------
@ Calculates S = (S[i][j]) with S[i][j] = trace(F1[i]F2[j]) for 1<= i,j <= anz.
@ Then g = gcd(S[i][j]) is calculated and  the result is S = 1/g * S.
@
@---------------------------------------------------------------------------
@
\*************************************************************************/
matrix_TYP *trace_bifo(F1, F2, anz)
matrix_TYP **F1, **F2;
int anz;
{
   int i,j,k,l, Sij, n;
   matrix_TYP *S;
   int g,x;

   extern matrix_TYP *init_mat();

   /* changed tilman 7/1/97 form
   n = F1[1]->cols;
   to: */
   n = F1[0]->cols;
   S = init_mat(anz, anz, "");
   for(i=0;i<anz;i++)
    for(j=0;j<anz;j++)
    {
      Sij = 0;
      for(k=0;k<n;k++)
        for(l=0;l<n;l++)
         Sij += F1[i]->array.SZ[k][l] * F2[j]->array.SZ[l][k];
      S->array.SZ[i][j] = Sij;
    }
   g = 0;
   for(i=0; i<anz && g != 1; i++)
     for(j=0; j<anz && g != 1; j++)
     {
       if(S->array.SZ[i][j] != 0)
       {
         if(g == 0)
         {
           g = S->array.SZ[i][j];
           if(g < 0)
             g = -g;
         }
         else
         {
           x = GGT(g, S->array.SZ[i][j]);
           if(x<0)
             g = -x;
           else g = x;
         }
       }
     }

     if(g != 1 && g != 0)
     {
       for(i=0; i<anz && g != 1; i++)
         for(j=0; j<anz && g != 1; j++)
             S->array.SZ[i][j] /= g;
     }
     return(S);
}
