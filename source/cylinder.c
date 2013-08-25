#include "ray.h"

obj_t *cylinder_init(FILE *fp, char *objclass) {
   obj_t *obj = sceneobj_init(fp, objclass);
   obj->typeData = (cyl_t *) malloc(sizeof(cyl_t));
   cyl_t *cylinder = (cyl_t *) obj->typeData;
   triple_t xaxis = {1, 0, 0};
   triple_t yaxis = {0, 1, 0};
   
   gettriple(fp, &cylinder->center);
   gettriple(fp, &cylinder->centerline);
   getdouble(fp, &cylinder->radius);
   getdouble(fp, &cylinder->height);
   
   obj->sceneData->hits = cylinder_hits;
   
   cylinder->rotate[1] = tl_unitvec3(&cylinder->centerline);
   if(abs(tl_dot3(&xaxis, &cylinder->centerline))==1)
      cylinder->rotate[2] = tl_cross3(&cylinder->centerline, &yaxis);
   else
      cylinder->rotate[2] = tl_cross3(&cylinder->centerline, &xaxis);
   cylinder->rotate[2] = tl_unitvec3(&cylinder->rotate[2]);
   cylinder->rotate[0] = tl_cross3(&cylinder->rotate[1], &cylinder->rotate[2]);
   cylinder->rotate[0] = tl_unitvec3(&cylinder->rotate[0]);
   
   tl_xpose3(cylinder->rotate, cylinder->irotate);
   
   return obj;
}

void cylinder_dump(obj_t *obj) {
      cyl_t *cylinder = (cyl_t *) obj->typeData;
   object_dump(obj);
   fprintf(stderr, "Cylinder data:\n");
   tl_print3("Center: ", &cylinder->center);
   tl_print3("Centerline: ", &cylinder->centerline);
   fprintf(stderr, "Cylinder radius:     %lf\n", cylinder->radius);
   fprintf(stderr, "Cylinder height:     %lf\n", cylinder->height);
   
   fprintf(stderr, "Rotation Matrix: \n");
   tl_print3("", &cylinder->rotate[0]);
   tl_print3("", &cylinder->rotate[1]);
   tl_print3("", &cylinder->rotate[2]);
   fprintf(stderr, "Inverse Matrix: \n");
   tl_print3("", &cylinder->irotate[0]);
   tl_print3("", &cylinder->irotate[1]);
   tl_print3("", &cylinder->irotate[2]);
   
   fprintf(stderr, "\n");
}

double cylinder_hits(triple_t *base, triple_t *dir, obj_t  *obj) {
   cyl_t *cylinder = (cyl_t *) obj->typeData;
   
   double t = 0;
   double a = 0;
   double b = 0;
   double c = 0;
   double discriminant = 0;
   
   triple_t B = tl_diff3(base, &cylinder->center);
   triple_t D = *dir;
   triple_t H;
   triple_t N;
   triple_t temp;
   
   B = tl_xform3(cylinder->rotate, &B);
   D = tl_xform3(cylinder->rotate, &D);
   
   a = (D.x * D.x) + (D.z * D.z);
   b = 2 * ((B.x * D.x) + (B.z * D.z));
   c = ((B.x * B.x) + (B.z * B.z)) - (cylinder->radius * cylinder->radius);
   
   if((discriminant = ((b * b) - 4 * a * c)) < 0)
      return -1;
      
   t = ((-b) - sqrt(discriminant)) / (2 * a);
   temp = tl_scale3(&D, t);
   H = tl_sum3(&B, &temp);
   
   if((H.y < 0)||(H.y > cylinder->height)) {
      t = ((-b) + sqrt(discriminant)) / (2 * a);
      temp = tl_scale3(&D, t);
      H = tl_sum3(&B, &temp);
      if((H.y < 0)||(H.y > cylinder->height))
         return -1;
   }
   
   N.x = H.x;
   N.y = 0;
   N.z = H.z;
   
   N = tl_unitvec3(&H);
   
   H = tl_xform3(cylinder->irotate, &H);
   N = tl_xform3(cylinder->irotate, &N);
   
   H = tl_sum3(&H, &cylinder->center);
   
   if(H.z > 0)
      return -1;
   
   obj->sceneData->hitloc = H;
   obj->sceneData->normal = N;
   
   return t;
}
