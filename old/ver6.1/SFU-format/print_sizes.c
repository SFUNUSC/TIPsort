#include "SFU-format.h"

int main()
{

  printf("Size of raw_event is %12ld\n",sizeof(raw_event));
  printf("Size of rheader   is %12ld\n",sizeof(rheader));
  printf("Size of RF        is %12ld\n",sizeof(RF));
  printf("Size of Tigress   is %12ld\n",sizeof(Tigress));
  printf("Size of PINArray  is %12ld\n",sizeof(PINArray));
  printf("Size of PINBD     is %12ld\n",sizeof(PINBD));
  printf("Size of S3        is %12ld\n",sizeof(S3));
  printf("Size of CsIArray  is %12ld\n",sizeof(CsIArray));
  return 0;
}
