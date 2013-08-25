/* Ray Tracer Prototype Statements */

#ifndef RAYHDRS_H
#define RAYHDRS_H

/** getnum.c **/
int getint(FILE *inFP, int *result);
int getdouble(FILE *inFP, double *result);
int gettriple(FILE *inFP, triple_t *triple);

/** environemnt.c **/
env_t *environment_init(FILE *inFP, int columns);
void environment_dump(env_t *env);
triple_t map_pix_to_world(env_t *env, int x, int y);

/** model.c **/
int model_init(FILE *fp, model_t *model);
void model_dump(model_t *model);

/** object.c **/
obj_t *object_init(FILE *fp, char *objclass, int objcat);
void object_dump(obj_t *objPtr);

/** sceneobj.c **/
obj_t *sceneobj_init(FILE *fp, char *objclass);
void sceneobj_dump(obj_t *obj);
triple_t getambient(obj_t * obj);
double getdiff(obj_t * obj);
double getspec(obj_t * obj);

/** sphere .c **/
obj_t *sphere_init(FILE *fp, char *objclass);
void sphere_dump(obj_t *objPtr);
double sphere_hits(triple_t *base, triple_t *dir, obj_t  *obj);

/** plane.c **/
obj_t *plane_init(FILE *fp, char *objclass);
void plane_dump(obj_t *objPtr);
double plane_hits(triple_t *base, triple_t *dir, obj_t  *obj);

/** tplane.c **/
obj_t *tplane_init(FILE *fp, char *objclass);
void tplane_dump(obj_t *obj);
triple_t tp_getcolor(obj_t *obj);
double tp_getdiffuse(obj_t *obj);
double tp_getspecular(obj_t *obj);
int tp_select(obj_t *obj);

/** pplane.c **/
obj_t *pplane_init(FILE *fp, char *objclass);
void pplane_dump(obj_t *obj);
triple_t pp_getcolor(obj_t *obj);

/** cylinder.c **/
obj_t *cylinder_init(FILE *fp, char *objclass);
void cylinder_dump(obj_t *obj);
double cylinder_hits(triple_t *base, triple_t *dir, obj_t  *obj);

/** image **/
void make_image(FILE *outFP, model_t *model);
void make_pixel(model_t *model, int colndx, int rowndx, pixel *pixPtr);

/** ray **/
void ray_trace(model_t *model, triple_t *base, triple_t *unitDir,
                  triple_t *intensity, double total_dist,
                  obj_t *last_hit);
obj_t *find_closest_obj(list_t *scene, triple_t *base, triple_t *unitDir,
   obj_t *lasthit);
   
/** illuminate **/
triple_t diffuse_illumination(model_t *model, obj_t *hitobj);
triple_t process_light(list_t *lst, obj_t *hitobj, lightobj_t *lobj);

/** light **/
obj_t *light_init(FILE *fp, char* objclass);
void light_dump(obj_t *lightobj);

/** polygon **/
obj_t *polygon_init( FILE *in, char *objclass);
void polygon_dump(obj_t *obj);
double polygon_hits(triple_t *base, triple_t *dir, obj_t *obj);

/** triangle **/
obj_t *triangle_init(FILE *fp, char *objClass);

/** rectangle **/
obj_t *rectangle_init(FILE *fp, char *objClass);

/** polytest **/
obj_t *polytest_init(FILE *fp, char *objClass);

/** getimage.c **/
pixel *getimage(FILE *imageFilePtr, int *columnsPtr, int *rowsPtr);
#endif
