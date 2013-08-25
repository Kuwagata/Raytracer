#include "ray.h"

obj_t *triangle_init(FILE *fp, char *objClass) { 
/****variables****/
   obj_t *polygon;
   plane_t *planePtr;
   polygon_t *polygonPtr;
   triple_t orientation;
   double edgelen;
   triple_t projected;
   triple_t planenorm;
   triple_t tempedge;
/****variables****/
   
   // Create a polygon for the triangle
   polygon = polygon_init(fp, objClass);
   
   // Create pointers for convenience
   planePtr = (plane_t *) polygon->typeData;
   polygonPtr = (polygon_t *) planePtr->priv;
   
   planenorm = tl_unitvec3(&planePtr->normal);
   
   // Create array of edges and initiate it
   polygonPtr->edges = (edge_t *) malloc(3 * sizeof(edge_t));
   polygonPtr->numEdges = 3;
   polygonPtr->edges[0].point = planePtr->point;
   
   // Retrieve first orientation and length from file
   gettriple(fp, &orientation);
   getdouble(fp, &edgelen);
   // Unitize orientation and normal to the plane that forms the triangle
   orientation = tl_unitvec3(&orientation);
   // Project orientation on the plane and unitize
   projected = tl_project3(&planenorm, &orientation);
   projected = tl_unitvec3(&projected);
   // Scale projected by the length of the edge
   projected = tl_scale3(&projected, edgelen);
   // Calculate the second point
   polygonPtr->edges[1].point = tl_sum3(&polygonPtr->edges[0].point, &projected);
   
   // Retrieve next orientation and length from file
   gettriple(fp, &orientation);
   getdouble(fp, &edgelen);
   // Unitize the orientation
   orientation = tl_unitvec3(&orientation);
   // Project orientation on the plane and unitize
   projected = tl_project3(&planenorm, &orientation);
   projected = tl_unitvec3(&projected);
   // Scale projected by the length of the edge
   projected = tl_scale3(&projected, edgelen);
   // Calculate the last point
   polygonPtr->edges[2].point = tl_sum3(&polygonPtr->edges[0].point, &projected);
   
   // Calculate the direction vectors for each edge and store them
   tempedge = tl_diff3(&polygonPtr->edges[1].point, &polygonPtr->edges[0].point);
   polygonPtr->edges[0].edgeDir = tl_unitvec3(&tempedge);

   tempedge = tl_diff3(&polygonPtr->edges[2].point, &polygonPtr->edges[1].point);
   polygonPtr->edges[1].edgeDir = tl_unitvec3(&tempedge);

   tempedge = tl_diff3(&polygonPtr->edges[0].point, &polygonPtr->edges[2].point);
   polygonPtr->edges[2].edgeDir = tl_unitvec3(&tempedge);
   
   // Return newly created triangle
   return polygon;
}
