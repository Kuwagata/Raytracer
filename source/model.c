#include "ray.h"

// Helper Structure
/*
Helper structure allows for function pointers to be assigned easily and for
the different types of scene/light objects to be modified without extensive
modification of other sections of code.
*/
struct helper_t {
   char *objecttype;
   obj_t *(*init)(FILE *, char *);
   void (*dump)(obj_t *);
} helper[] = {
   {"sphere", sphere_init, sphere_dump},
   {"plane", plane_init, plane_dump},
   {"pointlight", light_init, light_dump},
   {"rectangle", rectangle_init, polygon_dump},
   {"triangle", triangle_init, polygon_dump},
   {"polytest", polytest_init, polygon_dump},
   {"tplane", tplane_init, tplane_dump},
   {"pplane", pplane_init, pplane_dump},
   {"cylinder", cylinder_init, cylinder_dump}
};

/** model_init **/
int model_init(FILE *in, model_t *model) {
/****variables****/
   obj_t *obj = NULL;
   char  buf[256];
   char  objclass[128];
   int ctr;
   int flag;
/*****************/
   
/* Load the objects in the scene */
   while (fscanf(in, "%128s", objclass) == 1)
   {
      /* The word just inputted must be either a class name or the
         beginning of a comment 
      */
      flag = 0;
      if (objclass[0] == '#') {
         /* It's a comment -- skip the rest of the line */
         fgets(buf, sizeof(buf), in); /* consume rest of line */
      }
      else {
         
         for(ctr = 0; ctr < (sizeof(helper)/sizeof(*helper)); ctr++) {
            // Use helper struct to call correct init
            if(strcmp(objclass, (helper[ctr]).objecttype) == 0) {
               obj = helper[ctr].init(in, objclass);
               flag = 1;
               break;
            }
         }
         
         // Catches objects not in the helper struct
         if (flag == 0) {
            fprintf(stderr, "Unrecognized object type\n");
            exit(1);
         }
         
         // Catches failed initiation procedure otherwise adds
         if (obj == NULL) {
            fprintf(stderr, "Failed to load type %s \n", objclass);
            model_dump(model);
            exit(2);
         }
         else
         {
            /* Add object to scene object linked list */
            if(obj->objcat == 0)
               l_add(model->scene, obj);
            else
               l_add(model->lights, obj);
         }
      } /* End else "not a comment" */
   }
   return(0);
}

/** model_dump **/
void model_dump(model_t *model) {
/****variables****/
   obj_t *obj;
   int ctr;
   int flag;
/*****************/
   
   l_reset(model->scene);
   l_reset(model->lights);

   while ((obj = l_get(model->scene)) != NULL)
   {
      flag = 0;
      for(ctr = 0; ctr < (sizeof(helper)/sizeof(*helper)); ctr++) {
         if(strcmp(obj->objclass, (helper[ctr]).objecttype) == 0) {
            (helper[ctr]).dump(obj);
            flag = 1;
            break;
         }
      }
      if(flag == 0) {
         fprintf(stderr, "Bad class\n");
         exit(1);
      }
   }

   while ((obj = l_get(model->lights)) != NULL)
   {
      flag = 0;
      for(ctr = 0; ctr < (sizeof(helper)/sizeof(*helper)); ctr++) {
         if(strcmp(obj->objclass, helper[ctr].objecttype) == 0) {
            helper[ctr].dump(obj);
            flag = 1;
            break;
         }
      }
      if(flag == 0) {
         fprintf(stderr, "Bad class\n");
         exit(1);
      }
   }
}
