#include "ray.h"

obj_t *object_init(FILE *fp, char *objectclass, int objcat){
/****variables****/
   obj_t *newobj;
   static int idnum = 0;
/****variables****/
   
   newobj = (obj_t *) malloc(sizeof(obj_t));
   newobj->objclass = strdup(objectclass);
   newobj->objcat = objcat;
   newobj->objid = idnum;
   idnum++;
   return newobj;
}

void object_dump(obj_t *obj){
   fprintf(stderr, "Dumping object %d, category %d, type %s\n", obj->objid, obj->objcat, obj->objclass);
   return;
}
