#include "MatrixBS.h"
/*====================================================================================*/
int main(int argc, char *argv[])
{
  char *mat_name,*proj_name,*bg_name;
  FILE *fmat,*fproj,*fbg,*output;
  int rl;
  int i,j;
  char title[132];
  char title2[132];

  //initialize variables
  rl=0;
  i=0;
  j=0;

  //zero out all histograms
  memset(mat,0,sizeof(mat));
  memset(matBS,0,sizeof(matBS));
  memset(matBG,0,sizeof(matBG));
  memset(proj,0,sizeof(proj));
  memset(bg,0,sizeof(bg));
  memset(projOut,0,sizeof(projOut));

  //make the root histogram for saving the background subtracted matrix
  hMatBS = new TH2D("MatrixBS","MatrixBS",S1K,0,S4K-1,S1K,0,S4K-1);
  hMatBS->Reset();

  //make the root histogram for saving the background matrix
  hBG = new TH2D("Background","Background",S1K,0,S4K-1,S1K,0,S4K-1);
  hBG->Reset();

  //here is how you use the program
  if(argc!=4)
    {
      printf("\nProgram sorts a background subtracted matrix file.\nAll files should have dim = 4096 (4096x4096 for matrix).\n\nINPUT:\n1) Matrix file name (.mat)\n2) Total projection spectrum file name (.spe)\n3) Background spectrum file name (.spe)\n\nOUTPUT:\n1) Background subtracted matrix (.mat)\n2) Background subtracted matrix (.root)\n3) Background matrix (.root)\n\n");
      printf("usage:\nMatrixBS matrix_name total_proj_name background_name\n\n");
      exit(-1);
    }
  
  //file names from argument vector
  mat_name = argv[1];
  proj_name = argv[2];
  bg_name = argv[3];

  //open the matrix file
  fmat = fopen(mat_name, "r");
  if (fmat == NULL) 
    {
      printf("\n*** Could not open matrix file for reading ***\n");
      exit(1);
    }
  
  fread(mat,S4K*S4K*sizeof(short int),1,fmat);
  fclose(fmat);
  
  //open the projection file
  fproj = fopen(proj_name, "r");
  if (fproj == NULL) 
    {
      printf("\n*** Could not open total projection spectrum file for reading ***\n");
      exit(1);
    }
  
  //read in the header from the projection spectrum file.
  //header structure:
  /************
   name_length
   name (string)
   spectrum_length_size
   spectrum_size (bytes)
  ************/
  fread(&rl,4,1,fproj);
  //printf("\n%d\n",rl);
  fread(buf,rl,sizeof(char),fproj);
  //printf("%s\n",buf);
  fread(&rl,4,1,fproj);
  //printf("%d\n",rl);
  fread(&rl,4,1,fproj);
  //printf("%d\n\n",rl);

  //read in the projection spectrum to projection histogram
  fread(proj,sizeof(float),S4K,fproj);

  //read footer
  //spectrum_size_(bytes)
  fread(&rl,4,1,fproj);
  //printf("%d\n\n",rl);

  //close the projection file
  fclose(fproj);

  //open the background file
  fbg = fopen(bg_name, "r");
  if (fbg == NULL) 
    {
      printf("\n*** Could not open background spectrum file for reading ***\n");
      exit(1);
    }

  //read in the header from the background spectrum file
  //header structure:
  /************
   name_length
   name (string)
   spectrum_length_size
   spectrum_size (bytes)
  ************/
  fread(&rl,4,1,fbg);
  //printf("\n%d\n",rl);
  fread(buf,rl,sizeof(char),fbg);
  //printf("%s\n",buf);
  fread(&rl,4,1,fbg);
  //printf("%d\n",rl);
  fread(&rl,4,1,fbg);
  //printf("%d\n\n",rl);

  //read in the background spectrum to background histogram
  fread(bg,sizeof(float),S4K,fbg);

  //read footer
  //spectrum_size_(bytes)
  fread(&rl,4,1,fproj);
  //printf("%d\n\n",rl);
  
  //close the background file
  fclose(fbg);

  /******************************************** 
     Make the background subtracted matrix 
     Procedure from Radford NIM A361 (1995) 306 
  *********************************************/

  //normalization of the background
  float T=0.;

  for(i=0;i<S4K;i++)
      T+=proj[i]; //equivalent to summing over the matrix

  //compute the background matrix
  for(i=0;i<S4K;i++)
    for(j=0;j<S4K;j++)
      if(mat[i][j]>0)
	{
	  matBG[i][j] = (1/T)*(proj[i]*bg[j]+bg[i]*proj[j]-bg[i]*bg[j]);
	  hBG->SetBinContent(i/4,j/4,hBG->GetBinContent(i/4,j/4)+matBG[i][j]);
	}
  
  //compute the background subtracted matrix values
  for(i=0;i<S4K;i++)
    for(j=0;j<S4K;j++)
      if(mat[i][j]>0)
	{
	  matBS[i][j]=(int)rint(mat[i][j]-matBG[i][j]);
	  hMatBS->SetBinContent(i/4,j/4,hMatBS->GetBinContent(i/4,j/4)+matBS[i][j]);
	}
  
  //save the background subtracted matrix
  if((output=fopen("matrixBS.mat","w"))==NULL)
    {
      printf("\nI can't open file matrixBS.mat\n");
      exit(EXIT_FAILURE);
    }
	  
  fwrite(matBS,S4K*S4K*sizeof(short int),1,output);
  fclose(output);

  //save the background subtracted total projection spectrum
  for(i=0;i<S4K;i++)
    for(j=0;j<S4K;j++)
      projOut[i]+=matBS[i][j];
  
  if((output=fopen("projectionBS.spn","w"))==NULL)
    {
      printf("\nI can't open file projectionBS.spn\n");
      exit(EXIT_FAILURE);
    }
	  
  fwrite(projOut,S4K*sizeof(int),1,output);
  fclose(output);
  
  //save the background subtracted matrix histogram
  sprintf(title,"matrixBS.root");
  TFile f(title, "recreate");
  hMatBS->GetXaxis()->SetTitle("E1 [keV]");
  hMatBS->GetXaxis()->CenterTitle(true);
  hMatBS->GetYaxis()->SetTitle("E2 [keV]");
  hMatBS->GetYaxis()->CenterTitle(true);
  hMatBS->GetYaxis()->SetTitleOffset(1.5);
  hMatBS->SetOption("COLZ");
  gStyle->SetPalette(1);
  hMatBS->Write();

  //save the background histogram
  sprintf(title2,"background.root");
  TFile f1(title2, "recreate");
  hBG->GetXaxis()->SetTitle("E1 [keV]");
  hBG->GetXaxis()->CenterTitle(true);
  hBG->GetYaxis()->SetTitle("E2 [keV]");
  hBG->GetYaxis()->CenterTitle(true);
  hBG->GetYaxis()->SetTitleOffset(1.5);
  hBG->SetOption("COLZ");
  gStyle->SetPalette(1);
  hBG->Write();
  
  return 0;
}

  

