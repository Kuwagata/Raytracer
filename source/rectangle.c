#include "ray.h"

obj_t *rectangle_init(FILE *fp, char *objClass) { 
/****variables****/
   obj_t *polygon;
   edge_t *edgearray;
   plane_t *planePtr;
   polygon_t *polygonPtr;
   triple_t orientation;
   double width;
   double height;
   triple_t projected;
   triple_t planenorm;
   triple_t cross;
   triple_t tempedge;
/*****************/
   
   // Create polygon to contain the rectangle
   polygon = polygon_init(fp, objClass);
   
   // Create pointers for convenience
   planePtr = (plane_t *) polygon->typeData;
   polygonPtr = (polygon_t *) planePtr->priv;
   
   // Make edge array of four edges and assign it to the polygon
   edgearray = (edge_t *) malloc(4 * sizeof(edge_t));
   polygonPtr->edges = edgearray;
   polygonPtr->numEdges = 4;
   edgearray->point = planePtr->point;
   
   // Retrieve data from file
   gettriple(fp, &orientation);
   getdouble(fp, &width);
   getdouble(fp, &height);
   
   // Unitize orientation and the normal that defines the plane
   orientation = tl_unitvec3(&orientation);
   planenorm = tl_unitvec3(&planePtr->normal);
   
   // Project the orientation onto the plane and scale it by the width
   projected = tl_project3(&planenorm, &orientation);
   projected = tl_unitvec3(&projected);
   projected = tl_scale3(&projected, width);
   
   // Calculate the next point
   polygonPtr->edges[1].point = tl_sum3(&edgearray->point, &projected);
   
   // Calculate a vector from point one to point two
   tempedge = tl_diff3(&polygonPtr->edges[1].point, &edgearray->point);
   
   // Find a perpendicular vector, unitize it, and scale it to get the next side
   cross = tl_cross3(&planenorm, &tempedge);
   cross = tl_unitvec3(&cross);
   cross = tl_scale3(&cross, height);
   
   // Calculate the last two points
   polygonPtr->edges[2].point = tl_sum3(&polygonPtr->edges[1].point, &cross);
   polygonPtr->edges[3].point = tl_sum3(&edgearray->point, &cross);
   
   // Store the direction vectors of the edges in the edge array
   // Note the first was previously calculated, it just has to be unitized
   edgearray->edgeDir = tl_unitvec3(&tempedge);
   tempedge = tl_diff3(&polygonPtr->edges[2].point, &polygonPtr->edges[1].point);
   polygonPtr->edges[1].edgeDir = tl_unitvec3(&tempedge);
   tempedge = tl_diff3(&polygonPtr->edges[3].point, &polygonPtr->edges[2].point);
   polygonPtr->edges[2].edgeDir = tl_unitvec3(&tempedge);
   tempedge = tl_diff3(&edgearray->point, &polygonPtr->edges[3].point);
   polygonPtr->edges[3].edgeDir = tl_unitvec3(&tempedge);
   
   // Return the newly created rectangle
   return polygon;
} 
