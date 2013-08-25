#include "ray.h"

/** environment_init **/
env_t *environment_init(FILE *inFP, int columns) {
   env_t *newProjPtr;
   
   srand48((unsigned int) time(NULL));
   
   /* Allocate space for environment data */
   newProjPtr = (env_t *) malloc(sizeof(env_t));

   /* Fetch the viewpoint (x, y, z) coordinates */
   gettriple(inFP, &newProjPtr->view_point);

   /* Fetch the world x and y unit sizes */
   assert(getdouble(inFP, &newProjPtr->winWWidth)==1);
   assert(getdouble(inFP, &newProjPtr->winWHeight)==1);

   /* Set pixel column and row dimensions */
   newProjPtr->winPWidth = columns;
   newProjPtr->winPHeight =
        (newProjPtr->winWHeight/newProjPtr->winWWidth)*columns;

   return(newProjPtr);
} /* End environment_init */

/** environment_dump **/
void environment_dump(env_t *environment) {
   fprintf(stderr, "Environment Dump:\n");
   tl_print3(      "   Viewpoint: ", &environment->view_point);
   fprintf(stderr, "   Pixel size: %8d %8d\n",
          environment->winPWidth,
          environment->winPHeight);
   fprintf(stderr, "   World size: %8.4lf %8.4lf\n",
          environment->winWWidth,
          environment->winWHeight);
   fprintf(stderr, "   Samples Taken: %d\n", environment->samples);
} /* end environment_dump */

/** map_pix_to_world **/
triple_t map_pix_to_world (env_t *environment, int x, int y) {
   triple_t world;
   
   world.x = (((double)(x)/(double)(environment->winPWidth-1))*environment->winWWidth);
   world.x -= environment->winWWidth/2.0;
   world.y = (((double)(y)/(double)(environment->winPHeight-1))*environment->winWHeight);
   world.y -= environment->winWHeight/2.0;
   world.z = 0;
   return(world);
} /* End map_pix_to_world */


/** raytest **/
void raytest(env_t *environment) {
   int column;                  /* Screen column number           */
   int row;                     /* Screen row number              */
   triple_t worldPix;           /* Pixel world coordinates        */
   triple_t directionV  ;       /* Vector from viewpoint to pixel */
   double len;                  /* Vector length                  */
   triple_t directionUnitV;     /* Unit vector to pixel           */

   /* Process each pixel on the screen */
   fprintf(stderr, " col  row       x       y  length unitv.x unitv.y unitv.z\n");
   for (row=0; row<environment->winPHeight; row++) {
      for (column=0; column<environment->winPWidth; column++) {
          /* Convert screen coordinates to world coordinates */
          worldPix = map_pix_to_world(environment, column, row);

          /* Determine the vector from the viewpoint to the screen pixel */
          directionV = tl_diff3(&environment->view_point, &worldPix);
          /* Compute its length */
          len = tl_length3(&directionV);
          /* And create its corresponding unit vector */
          directionUnitV = tl_unitvec3(&directionV);

          /* Print the results */
          fprintf(stderr, "%4d %4d %7.3lf %7.3lf %7.3lf %7.3lf %7.3lf %7.3lf\n",
                           column, row, 
                           worldPix.x, worldPix.y, 
                           len, 
                           directionUnitV.x, 
                           directionUnitV.y, 
                           directionUnitV.z); 
      }
   }
} /* End raytest */
