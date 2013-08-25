#include "ray.h"

/** getint **/
int getint(FILE *fp, int *result) {
    char buf[256];
    int  code;

    while ((code=fscanf(fp, "%d", result)) != 1) {
       if (fgets(buf, sizeof(buf), fp) == NULL) {
          return(-1);
       }
       if (buf[0] != '#') {
          fprintf(stderr, "getint[%d]: Error in input: \"%s\"\n", code, buf);
          return(0);
       }
    }
    return(1);
}

/** getdouble **/
int getdouble(FILE *fp, double *result) {
    char buf[256];
    int  code;

    while ((code=fscanf(fp, "%lf", result)) != 1) {
       if (fgets(buf, sizeof(buf), fp) == NULL) {
          return(-1);
       }
       if (buf[0] != '#') {
          fprintf(stderr, "getdouble[%d]: Error in input: \"%s\"\n", code, buf);
          return(0);
       }
    }
    return(1);
}

/**tl_gettriple3 **/
/* Gets a triple from a specified file */
int gettriple(FILE *fp, triple_t *triple) {
   int x = 0;
   assert(x = getdouble(fp, &triple->x)==1);
   assert(x = getdouble(fp, &triple->y)==1);
   assert(x = getdouble(fp, &triple->z)==1);
   return x;
}
