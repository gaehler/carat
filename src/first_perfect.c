#include "typedef.h"
#include "datei.h"
#include "matrix.h"
#include "getput.h"
#include "bravais.h"
#include "voronoi.h"


int main (int argc, char *argv[])
{
   bravais_TYP *G, *Gtr;
   matrix_TYP *A, *P, *S;
   int Pmin;

        read_header(argc, argv);
        if(FILEANZ != 3){
          printf("Usage: %s 'file1' 'file2' 'file3'\n",argv[0]);
          printf("\n");
          printf("file1: bravais_TYP containing the finite unimodular group G.\n");
          printf("file2: bravais_TYP containing G^{tr}.\n");
          printf("file3: matrix_TYP containing a positive definite G-invariant form F.\n");
          printf("\n");
          printf("Returns a G-perfect form in the neighbourhood of F.\n");
          printf("\n");
          printf("Cf. Normalizer, Perfect_neighbours.\n");
          if (is_option('h')){
             exit(0);
          }
          else{
             exit(31);
          }
        }

        /* diagonistics for memory-leakages */
        if (is_option('h')){
           INFO_LEVEL = optionnumber('h');
        }

        G = get_bravais(FILENAMES[0]);
        Gtr = get_bravais(FILENAMES[1]);
        A = get_mat(FILENAMES[2]);
        S = trace_bifo(G->form, Gtr->form, G->form_no);
        P = first_perfect(A, G, Gtr->form, S, &Pmin);
        put_mat(P, NULL, "G-perfect form", 2);
        printf("The Minimum is %d\n", Pmin);
        put_mat(S, NULL, "tr_bifo", 2);

        /* cleaning up the memory */
        free_mat(P);
        free_mat(S);
        free_mat(A);
        free_bravais(G);
        free_bravais(Gtr);

        exit(0);
}
