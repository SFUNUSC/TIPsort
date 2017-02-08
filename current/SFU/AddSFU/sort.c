#include "sort.h"

int analyze_data(raw_event *data)
{
	encode(data,output,enb); //put the event in the output file
	return SEPARATOR_DISCARD;
}
/*====================================================================================*/

int main(int argc, char *argv[])
{
	input_names_type* name;
	FILE *cluster;
	char DataFile[132];

	if(argc!=3)
		{
			printf("addSFU input_list SFU_output_data\n\n");
			printf("Gracefully concatenates .sfu files specified in the file input_list (one .sfu file per line) into a single .sfu file.\n\nUnlike the `cat` command, this will display any errors in the structure of the input file(s), and also indicate copying progress.\n\n.");
			exit(-1);
		}

	name=(input_names_type*)malloc(sizeof(input_names_type));
	memset(name,0,sizeof(input_names_type));
	strcpy(name->fname.cluster_file,argv[1]);
	strcpy(name->fname.out_data,argv[2]);

	if((output=fopen(name->fname.out_data,"w"))==NULL)
		{
			printf("\nERROR: cannot open output file: %s\n",name->fname.out_data);
			exit(-2);
		}

	memset(enb,0,sizeof(enb));
	enb[0]=BUFFER_TAG;
	enb[1]++;
	enb[1]++;

	if((cluster=fopen(name->fname.cluster_file,"r"))==NULL)
		{
			printf("ERROR: Input list file does not exist.\n");
			exit(-2);
		}

	while(fscanf(cluster,"%s",DataFile)!=EOF)
		{
			printf("Reading data from file %s.\n",DataFile);
			strcpy(name->fname.inp_data,DataFile);
			sort(name);
			//save the last buffer which will be dropped otherwise
			//if enb[1]==2 then the buffer contains no data, only the header
			if(enb[1]>2)
				fwrite(enb,sizeof(int),BUFFSIZE,output);
		}
}
