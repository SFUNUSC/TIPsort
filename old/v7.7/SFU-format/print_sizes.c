#include "SFU-format.h"

int main()
{

  printf("Size of raw_event is %12ld\n",sizeof(raw_event));
  printf("Size of rheader   is %12ld\n",sizeof(rheader));
  printf("Size of RF        is %12ld\n",sizeof(RF));
  printf("Size of Tigress   is %12ld\n",sizeof(Tigress));
  printf("Size of PINARRAY  is %12ld\n",sizeof(PINARRAY));
  printf("Size of PINBD     is %12ld\n",sizeof(PINBD));
  printf("Size of S3        is %12ld\n",sizeof(S3));
  printf("Size of CSIARRAY  is %12ld\n",sizeof(CSIARRAY));
  return 0;
}
