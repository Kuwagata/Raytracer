#include "ray.h"

void make_image(FILE *outFP, model_t *model){

/****variables****/
   pixel *pixmap;
   pixel *pixPtr;
   int x; int y;
   int row; int col;
/*****************/
   
   row = model->env->winPHeight;
   col = model->env->winPWidth;
   
   pixmap = (pixel *) malloc(sizeof(pixel) * row * col);
   pixPtr = pixmap;
   
   for(y = row - 1; y >= 0; y--)
      for(x = 0; x < col; x++) {
         make_pixel(model, x, y, pixPtr);
         pixPtr++;
      }
   
   fprintf(outFP, "P6 %d %d 255\n", col, row);
   
   fwrite(pixmap, 1, row * col * sizeof(pixel), outFP);
}

void make_pixel(model_t *model, int colndx, int rowndx, pixel *pixPtr){
/****variables****/
   triple_t world;
   triple_t intensity;
   triple_t temp_intensity;
   triple_t dir;
   double ctrone;
   double ctrtwo;
/*****************/
   
   intensity = tl_scale3(&intensity, 0);
   
   for(ctrone = -0.5; ctrone <= 0.5; ctrone += 0.5) {
      for(ctrtwo = -0.5; ctrtwo <= 0.5; ctrtwo += 0.5) {
         world = map_pix_to_world(model->env, colndx + ctrtwo, rowndx + ctrone);
         dir = tl_diff3(&world, &model->env->view_point);
         dir = tl_unitvec3(&dir);
         temp_intensity = tl_scale3(&temp_intensity, 0);
         ray_trace(model, &model->env->view_point, &dir, &temp_intensity, 0.0, NULL);
         intensity = tl_sum3(&intensity, &temp_intensity);
      }
   }
   intensity = tl_scale3(&intensity, 1/(9.0));
   
   if(intensity.x > 1)
      intensity.x = 1;
   if(intensity.y > 1)
      intensity.y = 1;
   if(intensity.z > 1)
      intensity.z = 1;
   
   pixPtr->r = intensity.x * 255;
   pixPtr->g = intensity.y * 255;
   pixPtr->b = intensity.z * 255;
}
