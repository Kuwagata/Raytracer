#include "ray.h"
#define DEBUG

int main(int argc, char *argv[]) {
/****variables****/
   int pixcols;
   int numsamples;
   env_t *environment;
   FILE *inFile;
   FILE *outFile;
   model_t *model;
/*****************/
   
   switch(argc) {
      case 4:
         numsamples = 1;
         break;
      case 5:
         numsamples = atoi(argv[4]);
         break;
      default:
         fprintf(stderr, "Proper usage is: \n");
         fprintf(stderr, "\\executable datafile imagename pixelwidth sampling\n\n");
         return(1);
   }
   
   #ifdef DEBUG
   fprintf(stderr, "OPENING FILES\n");
   #endif
   
   //Open files; check for bad files
   inFile = fopen(argv[1], "r");
   if (inFile == NULL) {
      fprintf(stderr, "INPUT FILENAME IS BAD\n");
      return(1);
   }
   
   outFile = fopen(argv[2], "w");
   if (outFile == NULL) {
      fprintf(stderr, "OUTPUT FILENAME IS BAD\n");
      return(1);
   }
   
   //Store pixel width of image
   pixcols = atoi(argv[3]);
   
   #ifdef DEBUG
   fprintf(stderr, "ALLOCATING MEMORY FOR MODEL\n");
   #endif
   
   //Allocate memory for a new model object
   model = (model_t *) malloc(sizeof(model_t));
    
   #ifdef DEBUG
   fprintf(stderr, "CREATING AND ASSIGNING ENVIRONMENT\n");
   #endif
   
   //Initiate environment -- read in data from file
   //Print environment
   environment = environment_init(inFile, pixcols);
   environment->samples = numsamples;
   environment_dump(environment);
   model->env = environment;
   
   #ifdef DEBUG
   fprintf(stderr, "CREATING LIGHT/SCENE LISTS\n");
   #endif
   
   //Allocate lists for scene and light objects
   model->scene = l_new();
   model->lights = l_new();
   
   #ifdef DEBUG
   fprintf(stderr, "INITIATING MODEL\n");
   #endif
   
   //Initiate the model -- read in scene and light objects
   //Print model
   model_init(inFile, model);
   model_dump(model);
   
   fprintf(stderr, "\nMAKING IMAGE...PLEASE WAIT...\n");
   
   //Create the image by raytracing
   make_image(outFile, model);
   
   #ifdef DEBUG
   fprintf(stderr, "CLOSING FILES\n");
   #endif
   
   //Close files
   fclose(inFile);
   fclose(outFile);
   
   fprintf(stderr, "\nIMAGE COMPLETED\n");
   
   return 0;
}
