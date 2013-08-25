#include "ray.h"

obj_t *sphere_init(FILE *fp, char *objclass){
/****variables****/
   obj_t *obj;
   sphere_t *sphere;
/*****************/
   
   obj = sceneobj_init(fp, objclass);
   obj->sceneData->hits = sphere_hits;
   sphere = (sphere_t *) malloc(sizeof(sphere_t));
   gettriple(fp, &sphere->center);
   getdouble(fp, &sphere->radius);
   obj->typeData = sphere;
   return obj;
}

void sphere_dump(obj_t *obj){
   sceneobj_dump((obj_t *) obj);
   fprintf(stderr, "Center coordinates:  %lf, %lf, %lf\n", ((sphere_t *) obj->typeData)->center.x, ((sphere_t *) obj->typeData)->center.y, ((sphere_t *) obj->typeData)->center.z);
   fprintf(stderr, "Radius:  %lf\n\n", ((sphere_t *) obj->typeData)->radius);
   return;
}

double sphere_hits(triple_t *base, triple_t *dir, obj_t *objPtr) {
/****variables****/
   sphere_t *spherePtr  = (sphere_t *)objPtr->typeData;
   sceneobj_t *scenePtr = (sceneobj_t *)objPtr->sceneData;

   double result;
   double a;
   double b;
   double c;
   double discriminant;
   triple_t hitLocation;
   triple_t norm;
   triple_t newbase;
   triple_t newcenter;
   triple_t scaled;
   triple_t dirunit;
   triple_t hitdiff;
   double hitdifflen;
   result = -1;
/*****************/

   newcenter = tl_diff3(&spherePtr->center, &spherePtr->center);
   newbase = tl_diff3(base, &spherePtr->center);

   a = tl_dot3(dir, dir);
   b = 2 * tl_dot3(&newbase, dir);
   c = tl_dot3(&newbase, &newbase) - spherePtr->radius*spherePtr->radius;
   discriminant = b * b - 4 * a * c;

   if(discriminant <= 0)
      return result;
   else {
      result = ((-b) - sqrt(discriminant))/(2*a);
      dirunit = tl_unitvec3(dir);
      scaled = tl_scale3(&dirunit,result);
      hitLocation = tl_sum3(base, &scaled);
      if(hitLocation.z > 0)
         return -1;
      else
         scenePtr->hitloc = hitLocation;
      hitdiff = tl_diff3(&hitLocation, &spherePtr->center);
      hitdifflen = tl_length3(&hitdiff);
      norm = tl_scale3(&hitdiff,1/(hitdifflen));
      scenePtr->normal = norm;
      scenePtr->distance = tl_length3(&scaled);
   }
   
   return result;
}
