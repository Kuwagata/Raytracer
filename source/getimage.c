#include "ray.h"

pixel *getimage(FILE *imageFilePtr, int *columnsPtr, int *rowsPtr)
{
   char header[2];
   int temp;
   int result;
   pixel *inputData;
   
   if (fscanf(imageFilePtr, "%2s", header) != 1)
   {
      fprintf(stderr, "Premature end to PPM header\n");
      exit(1);
   }
   
   /* read columns and rows, returning null on error */
   /* First, read columns */
   result = getint(imageFilePtr, &temp);
   if(result != 1)
   {
      fprintf(stderr, "Error in reading columns...\n");
      return NULL;
   }
   
   *columnsPtr = temp;
   
   /* Second, read rows */
   result = getint(imageFilePtr, &temp);
   if (result != 1)
   {
      fprintf(stderr, "Error in reading rows...\n");
      return NULL;
   }
   
   *rowsPtr = temp;
   
   inputData = (pixel *) malloc(*rowsPtr * *columnsPtr * sizeof(pixel));
   
   return inputData;
}
