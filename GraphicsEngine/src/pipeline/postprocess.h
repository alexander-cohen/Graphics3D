#include <stdlib.h>
int *postprocess(int *inarray, int width, int height, int (*shader)(int *, int, int, int, int));
int pp_sobelfilter_shader_x(int *inarr, int width, int height, int i, int j);