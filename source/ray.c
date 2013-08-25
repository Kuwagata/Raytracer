#include "ray.h"

void ray_trace(model_t *model, triple_t *base, triple_t *unitDir, triple_t *intensity, double total_dist, obj_t *last_hit){
/****variables****/
   obj_t *closest;
   triple_t dist;
   triple_t diff_ill;
   triple_t reflected_intensity;
   triple_t v;
   triple_t u;
   triple_t n;
   triple_t temp;
   double specular;
/*****************/
   
   // Initiate containers to zero
   diff_ill = tl_scale3(&diff_ill, 0);
   dist = tl_scale3(&dist, 0);
   
   // Find the closest object along the ray's path
   closest = find_closest_obj(model->scene, base, unitDir, last_hit);
   // Return if the ray misses
   if(closest == NULL)
      return;
   
   specular = closest->sceneData->getspecular(closest);
   
   // Calculate the distance between the base and the hit location and add
   // to total_dist variable
   dist = tl_diff3(&closest->sceneData->hitloc, base);
   total_dist += tl_length3(&dist);
   // Add the object's color to the total_intensity
   *intensity = closest->sceneData->getcolor(closest);
   // Add the diffuse illumination to a container and put it in total_intensity
   diff_ill = diffuse_illumination(model, closest);
   *intensity = tl_sum3(intensity, &diff_ill);
   // Scale the total intensity by the total distance
   *intensity = tl_scale3(intensity, (1/(total_dist)));
   
   if(specular != 0) {
      reflected_intensity = tl_scale3(&reflected_intensity, 0);
      u = tl_scale3(unitDir, -1);
      u = tl_unitvec3(&u);
      n = tl_unitvec3(&closest->sceneData->normal);
      temp = tl_scale3(&n, 2 * tl_dot3(&u, &n));
      v = tl_diff3(&temp, &u);
      v = tl_unitvec3(&v);
      ray_trace(model, &closest->sceneData->hitloc, &v, &reflected_intensity, total_dist, closest);
      reflected_intensity = tl_scale3(&reflected_intensity, specular);
      *intensity = tl_sum3(intensity, &reflected_intensity);
   }
}

obj_t *find_closest_obj(list_t *scene, triple_t *base, triple_t *unitDir, obj_t *lasthit){
/****variables****/
   obj_t *obj;
   obj_t *clstobj;
   double dist;
/*****************/
   
   // Reset the scene list and initate variables
   l_reset(scene);
   clstobj = NULL;
   obj = NULL;
   dist = 0;
   
   // Run through the scene list
   while ((obj = ((obj_t *)l_get(scene))) != NULL)
   {
      // If the object is the last hit, don't do anything as it is a waste
      if(obj != lasthit) {
         // Call the object's hits function
         dist = obj->sceneData->hits(base, unitDir, obj);
         
         // If the distance is negative, it missed
         if(dist > 0) {
            // First hit scenario -- auto assigns
            if(clstobj == NULL) {
               clstobj = obj;
            }
            else {
               // Multiple hit objects scenario -- checks to see who is closest
               if(dist < clstobj->sceneData->distance){
                  clstobj = obj;
               }
            }
         }
      }
   }
   return clstobj;
}
