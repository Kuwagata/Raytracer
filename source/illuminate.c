#include "ray.h"

triple_t diffuse_illumination(
   model_t  *model,        /* pointer to model structure     */
   obj_t    *hitobj)       /* object that was hit by the ray */
{
/****variables****/
   obj_t *currlight;
   list_t *lightlist;
   triple_t total_illumination;
   triple_t processed_light;
/*****************/

   //Point to list of lights in the model
   lightlist = model->lights;

   //Initialize to zero
   total_illumination = tl_scale3(&total_illumination, 0);
   processed_light = tl_scale3(&processed_light, 0);
   
   //Reset list so that position points to head
   l_reset(lightlist);

   //Walk through the lights list and process each light
   //Sum up the total illumination
   while((currlight = l_get(lightlist)) != NULL)
   {
      processed_light = process_light(model->scene, hitobj, currlight->lightData);
      total_illumination = tl_sum3(&total_illumination, &processed_light);
   }

   return total_illumination;
}

triple_t process_light(
   list_t *lst,          /* List of all scene objects      */
   obj_t  *hitobj,       /* The object hit by the ray      */
   lightobj_t *lobj)         /* the current light source       */                
{
   /****variables****/
   triple_t surfacenormal;
   triple_t lightvec;
   triple_t unitlightvec;
   triple_t light;
   obj_t *closest;
   double dotP;
/********************/
   //Initiate light to zero
   light = tl_scale3(&light, 0);
   
   //Initiate closest object to hitobj
   closest = hitobj;
   
   //Calculate the surface normal to the hit object
   surfacenormal = tl_unitvec3(&hitobj->sceneData->normal);
   
   //calculate vector from the hitpoint to the center of the light and unitize
   lightvec = tl_diff3(&lobj->center, &hitobj->sceneData->hitloc);
   unitlightvec = tl_unitvec3(&lightvec);
   
   //Check if the light ray is perpendicular to the surface normal
   //If it is perpendicular, a zero vector is returned
   dotP = tl_dot3(&unitlightvec, &surfacenormal);
   if(dotP < 0) {
      return light;
   }
   
   //Calculate the vector from the light to the hitloc and unitize
   lightvec = tl_diff3(&hitobj->sceneData->hitloc, &lobj->center);
   unitlightvec = tl_unitvec3(&lightvec);
   
   //Use find closest to see if the ray hits another object and gets obscured
   //If it does, return a zero vector
   closest = find_closest_obj(lst, &lobj->center, &unitlightvec, NULL);
   if(closest->objid != hitobj->objid) {
      return light;
   }

   //The light is confirmed to hit the object so scale the emissivity and return the result
   light = tl_scale3(&lobj->emissivity, (hitobj->sceneData->getdiffuse(hitobj) * dotP)/(tl_length3(&lightvec)));

   return light;
}
