#include "ray.h"

obj_t *sceneobj_init(FILE *fp, char *objclass){
/****variables****/
   obj_t *base;
   sceneobj_t *sceneObject;
/*****************/
   
   // Create the object to hold the sceneobj
   base = object_init(fp, objclass, SCENEOBJ);
   
   // Allocate memory for the sceneobj
   sceneObject = (sceneobj_t *) malloc(sizeof(sceneobj_t));
   
   // Read in the scene object's data from the file
   gettriple(fp, &sceneObject->color);
   getdouble(fp, &sceneObject->diffuse);
   getdouble(fp, &sceneObject->specular);
   
   // Set function pointers for retrieving attributes
   sceneObject->getcolor = getambient;
   sceneObject->getdiffuse = getdiff;
   sceneObject->getspecular = getspec;
   
   // Attach sceneobj to the base object and return
   base->sceneData = sceneObject;
   return base;
}

void sceneobj_dump(obj_t *obj){
   object_dump(obj);
   tl_print3("Ambient color: ", &obj->sceneData->color);
   fprintf(stderr, "Diffuse reflectivity:  %lf\n", obj->sceneData->diffuse);
   fprintf(stderr, "Specular reflectivity:  %lf\n", obj->sceneData->specular);
   return;
}

triple_t getambient(obj_t * obj) {
   return obj->sceneData->color;
}

double getdiff(obj_t * obj) {
   return obj->sceneData->diffuse;
}

double getspec(obj_t * obj) {
   return obj->sceneData->specular;
}
