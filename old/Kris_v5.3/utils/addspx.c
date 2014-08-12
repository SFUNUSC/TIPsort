
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define DIM  16384

int main(int argc, char **argv) {

  FILE *fp1,*fp2,*fp3;
  char *inp_name1,*inp_name2,*out_name;
  int i,j, n;
  int hist1[DIM],hist2[DIM];
 
  if (argc !=5) {
    printf("usage: addspn nrows inp_name1 inp_name2 out_name \n");
    exit(1);
  }


  n=atoi(argv[1]);
  inp_name1 = argv[2];
  inp_name2 = argv[3];
  out_name = argv[4];

  fp1 = fopen(inp_name1, "r");
  if (fp1 == NULL) {
    printf("\n*** Could not open matrix file 1 for reading\n");
    exit(1);
  }

 

  fp2 = fopen(inp_name2, "r");
  if (fp2 == NULL) {
    printf("\n*** Could not open matrix file 2 for reading\n");
    exit(1);
  }
 
  
  fp3 = fopen(out_name, "w");
  if (fp3 == NULL) {
    printf("\n*** Could not open output file for writing\n");
    exit(1);
  }

  for (i = 0; i < n; i++)
    {
      printf("Adding row %d\n",i);
      fread(hist1, sizeof(hist1), 1, fp1);
      fread(hist2, sizeof(hist2), 1, fp2);
      for (j = 0; j < DIM; j++)
	hist1[j]+=hist2[j];
      fwrite(hist1, sizeof(hist1), 1, fp3);
    }

  fclose(fp1); 
  fclose(fp2); 
  fclose(fp3); 

  return 0;
}





