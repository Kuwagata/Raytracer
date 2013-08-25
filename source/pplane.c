#include "ray.h"

obj_t *pplane_init(FILE *fp, char *objclass) {
   char buf[256];
   char temp[128];
   int brightness;
   triple_t projected;
   FILE *image;
   obj_t *obj = rectangle_init(fp, objclass);
   plane_t *plane = (plane_t *) obj->typeData;
   polygon_t *polygon = (polygon_t *) plane->priv;
   polygon->priv = (pplane_t *) malloc(sizeof(pplane_t));
   pplane_t *pplane = (pplane_t *) polygon->priv;
   
   fgets(buf, sizeof(buf), fp);
   fgets(buf, sizeof(buf), fp);
   fscanf(fp, "%128s", temp);
   fgets(buf, sizeof(buf), fp);
   pplane->filename = strdup(temp);
   
   image = fopen(pplane->filename, "r");
   
   if(image == NULL)
      fprintf(stderr, "SOMETHING HAPPENED\n");
   
   getdouble(fp, &pplane->tilewidth);
   getdouble(fp, &pplane->tileheight);
   
   pplane->imageData = getimage(image, &pplane->imagewidth, &pplane->imageheight);
   
   getint(image, &brightness);
   fgets(buf, sizeof(buf), image);
   
   fprintf(stderr, "This is in buf:%s:\n", buf);
   
   fread(pplane->imageData, 1, pplane->imagewidth * pplane->imageheight * sizeof(pixel), image);
   
   fclose(image);
   
   projected = tl_diff3(&polygon->edges[1].point, &plane->point);
   projected = tl_unitvec3(&projected);
   
   pplane->rotate[0] = projected;
   pplane->rotate[2] = tl_unitvec3(&plane->normal);
   pplane->rotate[1] = tl_cross3(&pplane->rotate[2], &pplane->rotate[0]);
   
   obj->sceneData->getcolor = pp_getcolor;
   
   return obj;
}

void pplane_dump(obj_t *obj) {
   plane_t *plane = (plane_t *) obj->typeData;
   polygon_t *polygon = (polygon_t *) plane->priv;
   pplane_t *pplane = (pplane_t *) polygon->priv;
   
   polygon_dump(obj);
   fprintf(stderr, "Filename: %s\n", pplane->filename);
   fprintf(stderr, "Tiling Width: %lf, Tiling Height: %lf\n", pplane->tilewidth, pplane->tileheight);
   fprintf(stderr, "Image Width: %d, Image Height: %d\n", pplane->imagewidth, pplane->imageheight);
   fprintf(stderr, "Rotation Matrix:\n");
   tl_print3("", &pplane->rotate[0]);
   tl_print3("", &pplane->rotate[1]);
   tl_print3("", &pplane->rotate[2]);
   fprintf(stderr, "\n");
}

triple_t pp_getcolor(obj_t *obj) {
   plane_t *plane = (plane_t *) obj->typeData;
   polygon_t *polygon = (polygon_t *) plane->priv;
   pplane_t *pplane = (pplane_t *) polygon->priv;
   double relx;
   double rely;
   triple_t newhit;
   triple_t color;
   
   newhit = tl_diff3(&obj->sceneData->hitloc, &plane->point);
   newhit = tl_xform3(pplane->rotate, &newhit);
   relx = newhit.x / pplane->tilewidth;
   relx = relx - (int)relx;
   rely = newhit.y / pplane->tileheight;
   rely = rely - (int)rely;
   relx *= pplane->imagewidth;
   rely = 1 - rely;
   rely *= pplane->imageheight;
   relx = (int) relx;
   rely = (int) rely;
   
   color.x = pplane->imageData[(int)((rely * pplane->imagewidth) + relx)].r;
   color.y = pplane->imageData[(int)((rely * pplane->imagewidth) + relx)].g;
   color.z = pplane->imageData[(int)((rely * pplane->imagewidth) + relx)].b;
   
   color.x = (double) color.x / 30;
   color.y = (double) color.y / 30;
   color.z = (double) color.z / 30;
   
   return color;
}
