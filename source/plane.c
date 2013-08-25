#include "ray.h"

obj_t *plane_init(FILE *fp, char *objclass){
/****variables****/
   obj_t *obj;
   plane_t *plane;
/*****************/
   
   obj = sceneobj_init(fp, objclass);
   obj->sceneData->hits = plane_hits;
   plane = (plane_t *) malloc(sizeof(plane_t));
   gettriple(fp, &plane->point);
   gettriple(fp, &plane->normal);
   obj->typeData = plane;
   return obj;
}

void plane_dump(obj_t *obj){
   sceneobj_dump(obj);
   tl_print3("Point Data: ", &((plane_t *) obj->typeData)->point);
   tl_print3("Normal Data: ", &((plane_t *) obj->typeData)->normal);
   return;
}

double plane_hits(triple_t *base, triple_t *dir, obj_t *obj) {
   plane_t *planePtr  = (plane_t *)obj->typeData;
   sceneobj_t *scenePtr = obj->sceneData;
   double t = -1;
   triple_t hitLocation;
   triple_t scaled;
   triple_t rev_dir;

   if(tl_dot3(&planePtr->normal, dir)==0)
      return t;   
   t = (tl_dot3(&planePtr->normal, &planePtr->point)-tl_dot3(&planePtr->normal, base)) / tl_dot3(&planePtr->normal, dir);
   if(t<0) {
      t = -1;
      return t;
   }
   scaled = tl_scale3(dir, t);
   hitLocation = tl_sum3(base, &scaled);
   if(hitLocation.z > 0) {
      t = -1;
      return t;
   }
   scenePtr->hitloc = hitLocation;
   
   rev_dir = tl_scale3(dir, -1);
   rev_dir = tl_sum3(&rev_dir, &hitLocation);
   rev_dir = tl_unitvec3(&rev_dir);
   if(tl_dot3(&planePtr->normal, &rev_dir) > 0)
      scenePtr->normal = planePtr->normal;
   else
      scenePtr->normal = tl_scale3(&planePtr->normal, -1);
   
   scenePtr->normal = planePtr->normal;
   scenePtr->distance = t;

   return t;
}
