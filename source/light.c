#include "ray.h"

obj_t *light_init(FILE *fp, char* objclass) {
/****variables****/
   obj_t *obj;
/*****************/
   
   // Create a new object specified as a light
   obj = object_init(fp, objclass, LIGHTOBJ);
   
   // Allocate space for the light data
   lightobj_t *light = (lightobj_t *) malloc(sizeof(lightobj_t));
   
   // Assign light data to the object
   obj->lightData = light;
   
   // Retrieve data from file
   gettriple(fp, &light->emissivity);
   gettriple(fp, &light->center);
   
   // Return the object
   return obj;
}

void light_dump(obj_t *obj) {
   lightobj_t *light = obj->lightData;
   object_dump(obj);
   fprintf(stderr, "Light Data:\n");
   tl_print3("Center coordinates: ", &light->center);
   tl_print3("Emissivity: ", &light->emissivity);
}
