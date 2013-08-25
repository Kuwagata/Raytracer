#include "ray.h"

obj_t *polygon_init( FILE *in, char *objclass) { 
/****variables****/
   obj_t *obj;
   plane_t *planePtr;
/*****************/
   
   // Create a new object based on the plane type
   obj = plane_init(in, objclass);
   
   // Create a pointer for convenience
   planePtr = (plane_t *) obj->typeData;

   // Allocate room for a polygon type
   planePtr->priv = (void *) malloc(sizeof(polygon_t));
   
   // Override hits function with polygon hits
   obj->sceneData->hits = polygon_hits;
   
   // Return the newly created polygon
   return(obj);
} 

void polygon_dump(obj_t *obj) {
/****variables****/
   int edgectr;
   edge_t *edgeptr;
/*****************/
   
   // Point to polygon parts
   polygon_t *polygon = (polygon_t *) ((plane_t *) obj->typeData)->priv;
   
   // Dump the plane components
   plane_dump(obj);
   
   fprintf(stderr, "Polygon data:\n");
   fprintf(stderr, "Number of edges: %d\n", polygon->numEdges);
   for(edgectr = 0; edgectr < polygon->numEdges; edgectr++) {
      edgeptr = polygon->edges;
      fprintf(stderr, "Edge %d: ", edgectr);
      tl_print3("Point:     ", &(edgeptr + edgectr)->point);
      fprintf(stderr, "\n");
      tl_print3("        Direction: ", &(edgeptr + edgectr)->edgeDir);
      fprintf(stderr, "\n");
   }
}

double polygon_hits(triple_t *base, triple_t *dir, obj_t *obj) { 
/****variables****/
   double dist;
   int edgectr;
   polygon_t *polygonPtr;
   triple_t vec_to_hit;
   triple_t crossproduct;
   double dotproduct;
   double lastdot;
/*****************/
   
   // Call plane hits to determine the distance
   dist = plane_hits(base, dir, obj);
   
   // If it misses, return
   if(dist == -1)
      return -1;
      
   // Point to polygon parts
   polygonPtr = (polygon_t *) (((plane_t *)obj->typeData)->priv);
      
   // Initiate lastdot to zero
   lastdot = 0;
   
   // For every edge:
   for(edgectr = 0; edgectr < polygonPtr->numEdges; edgectr++) { 
      // Calculate a unit vector from the point to the hitlocation
      vec_to_hit = tl_diff3(&obj->sceneData->hitloc, &polygonPtr->edges[edgectr].point);
      vec_to_hit = tl_unitvec3(&vec_to_hit);
      
      // Cross the direction with the previous vector
      crossproduct = tl_cross3(&polygonPtr->edges[edgectr].edgeDir, &vec_to_hit);
      crossproduct = tl_unitvec3(&crossproduct);
      
      // dot the cross with the normal
      dotproduct = tl_dot3(&crossproduct, &obj->sceneData->normal);

      // If it's not the first edge
      if(edgectr != 0)
         if((lastdot * dotproduct) <= 0)
            return -1;
      // Record the dot product in the last
      lastdot = dotproduct;
    }
    
    // Return distance calculated by plane_hits
    return dist;
}
