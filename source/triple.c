/* CPSC 210
 *
 * triple.c
 *
 * Vector and triple functions
 *
 */

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "triple.h"

/** tl_length3 **/
/* Computes the length of a 3-D vector */
double tl_length3(triple_t * v1) {
   double sum = 0.0;
   
   sum = v1->x*v1->x + v1->y*v1->y + v1->z*v1->z;
   
   return sqrt(sum);
}


/** tl_scale3 **/
/* Scales a 3-D vector by a specified factor. Returns a new vector */
triple_t  tl_scale3(triple_t * v, double factor) {
   triple_t result;
   
   result.x = v->x * factor;
   result.y = v->y * factor;
   result.z = v->z * factor;

   return result;
}


/** tl_unitvec3 **/
/* Returns a unit vector in the same direction as the vector v.  */
triple_t tl_unitvec3(triple_t * v) {
   triple_t result = {0, 0, 0};
   double len;

   len = tl_length3(v);
   if (len != 0) {
      result.x = v->x / len;
      result.y = v->y / len;
      result.z = v->z / len;
   }
   
   return result;
}


/** tl_dot3 **/
/* Computes the dot product of two 3-D vectors */
double tl_dot3(triple_t * v1, triple_t * v2) {
   double sum;
   
   sum = (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z);
   
   return sum;
}


/** tl_diff3 **/
/* Returns a new triple that has the value of v1 - v2 */
triple_t tl_diff3(triple_t * v1, triple_t * v2) {
   triple_t result;

   result.x = v1->x - v2->x;
   result.y = v1->y - v2->y;
   result.z = v1->z - v2->z;
   
   return result;
}


/** tl_sum3 **/
/* Returns a new triple that has the value of v1 + v2 */
triple_t tl_sum3(triple_t * v1, triple_t * v2) {
   triple_t  result;
   
   result.x = v1->x + v2->x;
   result.y = v1->y + v2->y;
   result.z = v1->z + v2->z;
   
   return result;
}

/** tl_print3 **/
/* Print a triple to stderr, with a label */
void tl_print3(char *label, triple_t *v) {
   fprintf(stderr, "%s %8.4lf %8.4lf %8.4lf\n", label, v->x, v->y, v->z);
}

/** tl_cross3 **/
/* Cross product of two triples */
triple_t tl_cross3(triple_t *v, triple_t *w) {
   triple_t result;
   
   result.x = (v->y * w->z) - (v->z * w->y);
   result.y = (v->z * w->x) - (v->x * w->z);
   result.z = (v->x * w->y) - (v->y * w->x);
   
   return result;
}

/** tl_project3 **/
/* Projects a vector v on a plane with a specified normal */
triple_t tl_project3(triple_t *normal, triple_t *v) {
   double dot;
   triple_t scale;
   triple_t result;
   dot = tl_dot3(normal, v);
   scale = tl_scale3(normal, dot);
   result = tl_diff3(v, &scale);
   return result;
}

triple_t tl_xform3(triple_t rotation[3], triple_t *p) {
   triple_t result;
   result.x = tl_dot3(&rotation[0], p);
   result.y = tl_dot3(&rotation[1], p);
   result.z = tl_dot3(&rotation[2], p);
   return result;
}

void tl_xpose3(triple_t orig[], triple_t result[]) {
   //1-1 entry
   result[0].x = orig[0].x;
   
   //Interchange 2-1 and 1-2
   result[0].y = orig[1].x;
   result[0].z = orig[2].x;
   
   //Interchange 3-1 and 1-3
   result[1].x = orig[0].y;
   result[2].x = orig[0].z;
   
   //2-2 Entry
   result[1].y = orig[1].y;
   
   //Interchange 3-2 and 2-3
   result[1].z = orig[2].y;
   result[2].y = orig[1].z;
   
   //3-3 Entry
   result[2].z = orig[2].z;
}
