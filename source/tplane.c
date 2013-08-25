#include "ray.h"

obj_t *tplane_init(FILE *fp, char *objclass) { 
/****variables****/
   obj_t *obj;
   triple_t projected;
   tplane_t *tplane;
   plane_t *plane;
/*****************/

   obj = plane_init(fp, objclass);
   plane = (plane_t *)(obj->typeData);
   tplane = (tplane_t *) malloc(sizeof(tplane_t));
   plane->priv = tplane;
   gettriple(fp, &tplane->orientation);
   getdouble(fp, &tplane->tileWidth);
   getdouble(fp, &tplane->tileHeight);
   tplane->orientation = tl_unitvec3(&tplane->orientation);
   projected = tl_project3(&plane->normal, &tplane->orientation);
   projected = tl_unitvec3(&projected);
   
   tplane->rotate[0] = projected;
   tplane->rotate[2] = tl_unitvec3(&plane->normal);
   tplane->rotate[1] = tl_cross3(&tplane->rotate[2], &tplane->rotate[0]);
   
   gettriple(fp, &tplane->bkgrndcolor);
   getdouble(fp, &tplane->bkgrnddiff);
   getdouble(fp, &tplane->bkgrndspec);
   
   /* Override default pointers to surface color routines */ 
   obj->sceneData->getcolor  = tp_getcolor;
   obj->sceneData->getdiffuse = tp_getdiffuse;
   obj->sceneData->getspecular = tp_getspecular;
 
   return(obj); 
}

void tplane_dump(obj_t *obj) {
   plane_t *plane = (plane_t *)(obj->typeData);
   tplane_t *tplane = (tplane_t *) (plane->priv);
   plane_dump(obj);
   fprintf(stderr, "Tile Height: %lf   Tile Width: %lf\n", tplane->tileHeight, tplane->tileWidth);
   tl_print3("Orientation: ", &tplane->orientation);
   tl_print3("Background Color: ", &tplane->bkgrndcolor);
   fprintf(stderr, "Background Diffuse: %lf\n", tplane->bkgrnddiff);
   fprintf(stderr, "Background Specular: %lf\n", tplane->bkgrndspec);
   fprintf(stderr, "Rotation Matrix:\n");
   tl_print3("", &tplane->rotate[0]);
   tl_print3("", &tplane->rotate[1]);
   tl_print3("", &tplane->rotate[2]);
   fprintf(stderr, "\n");
}

/**/ 
triple_t tp_getcolor(obj_t *obj)  {
   plane_t  *plnPtr = (plane_t *)obj->typeData; 
   tplane_t *tp  = (tplane_t *)plnPtr->priv; 
 
   if (tp_select(obj)) 
      return obj->sceneData->color; 
   else 
      return tp->bkgrndcolor;  
}

/**/ 
double tp_getdiffuse(obj_t *obj) {
   plane_t  *plnPtr = (plane_t *)obj->typeData;
   tplane_t *tp  = (tplane_t *)plnPtr->priv;
   
   if (tp_select(obj))
      return obj->sceneData->diffuse;
   else
      return tp->bkgrnddiff;
}

/**/ 
double tp_getspecular(obj_t *obj) {
   plane_t  *plnPtr = (plane_t *)obj->typeData;
   tplane_t *tp  = (tplane_t *)plnPtr->priv;
   
   if (tp_select(obj))
      return obj->sceneData->specular;
   else
      return tp->bkgrndspec;
}

int tp_select(obj_t *obj) {
   plane_t *plane = (plane_t *) obj->typeData;
   tplane_t *tplane = (tplane_t*)(plane->priv);
   
   int relx;
   int rely;
   triple_t newhit;
   
   newhit = tl_diff3(&obj->sceneData->hitloc, &plane->point);
   newhit = tl_xform3(tplane->rotate, &newhit);
   relx = (int) (newhit.x / tplane->tileWidth);
   rely = (int) (newhit.y / tplane->tileHeight);
   
   if((newhit.x < 0) && (newhit.y < 0))
      return (relx + rely) % 2;
   if((newhit.x < 0) || (newhit.y < 0))
      return (relx + rely + 1) % 2;
   else
      return (relx + rely) % 2;
}
