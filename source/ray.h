/* ray.h
 *
 * Revision: October 5, 2007 */

#ifndef RAY_H
#define RAY_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include <assert.h>

#include "triple.h"
#include "list.h"


/** Defines **/
#define SCENEOBJ 0
#define LIGHTOBJ 1


/** pixel                 **/
typedef struct pixel {
   unsigned char r;
   unsigned char g;
   unsigned char b;
} pixel;

/** Light object category **/
typedef struct lightobj 
{
   triple_t   emissivity;  /* Light rbg color            */ 
   triple_t   center;      /* Center of light            */ 
}  lightobj_t;

struct obj_type;

/** Scene object category **/
typedef struct sceneobj {
   /* Surface reflectivity values */
   triple_t color;          /* Ambient surface color          */
   double   diffuse;        /* Diffuse lighting reflectivity  */
   double   specular;       /* Specular reflectivity          */
   
   /* Hits function */
   double (*hits)(triple_t *base, triple_t *dir, struct obj_type *obj);
   
   /* Pointers to object's light reflection functions */
   triple_t (*getcolor)    (struct obj_type *obj);
   double   (*getdiffuse)  (struct obj_type *obj);
   double   (*getspecular) (struct obj_type *obj);

   /* "Hit" information for object */
   triple_t  hitloc;         /* Last hit point                         */
   triple_t  normal;         /* Normal at hit point                    */
   double   distance;        /* Distance from "base" to last hit point */
} sceneobj_t;


/** Generic object type **/
typedef struct obj_type
{
   struct  obj_type  *next;   /* Next object in list                   */
   int     objid;             /* Numeric serial # for debug            */
   int     objcat;            /* Object category (e.g. scene or light) */
   char    *objclass;         /* Type class name (e.g. "plane")        */
   void    *typeData;         /* Type specfic data                     */
   union {
      sceneobj_t *sceneData;  /* Scene object category data            */
      lightobj_t *lightData;  /* Light object category data            */
   };
} obj_t;

typedef struct sphere_type
{
   triple_t  center;          /* Center of sphere                      */
   double  radius;            /* Sphere radius                         */
   void    *priv;             /* Data for specialized types            */
}  sphere_t;

typedef struct plane_type
{
   triple_t point;            /* Arbitrary point on plane              */
   triple_t normal;           /* Normal vector to plane                */
   void     *priv;            /* Data for specialized types            */
}  plane_t;

typedef struct tplane_type
{
   triple_t orientation;
   triple_t bkgrndcolor;
   triple_t rotate[3];
   double bkgrnddiff;
   double bkgrndspec;
   double tileHeight;
   double tileWidth;
}  tplane_t;

typedef struct pplane_type
{
   triple_t rotate[3];
   char *filename;
   double tilewidth;
   double tileheight;
   int imagewidth;
   int imageheight;
   pixel *imageData;
}  pplane_t;

typedef struct cylinder_type
{
   triple_t center;       //center line starting point
   triple_t centerline;   //orientation
   double radius;         //cylinder radius
   double height;         //cylinder height (length)
   triple_t rotate[3];    //rotation matrix
   triple_t irotate[3];   //inverse rotation matrix
}  cyl_t;

typedef struct environment
{
   int    samples;             /* Samples to take                  */
   int    winPWidth;           /* Window dimensions                */
   int    winPHeight;          /*    in pixels                     */
   double winWWidth;           /* Window dimensions                */ 
   double winWHeight;          /*    in world coordinates          */
   triple_t  view_point;       /* Viewpoint location               */
} env_t;

typedef struct model_type
{
   env_t   *env;                /* Scenario data                   */
   list_t  *lights;             /* Lights linked list              */
   list_t  *scene;              /* Scene objects linked list       */
}  model_t;

typedef struct edge_type
{ 
   triple_t point; 
   triple_t edgeDir; 
} edge_t;

typedef struct polygon_type
{ 
   int numEdges; 
   edge_t *edges;
   void *priv;
} polygon_t;

#include "rayhdrs.h"

#endif

