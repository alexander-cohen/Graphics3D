#include "postprocess.h"

int pp_sobelfilter_shader_x(int *inarr, int width, int height, int i, int j) {
    int iT = i == 0 ? 1 : i-1;
    int iB = i == height-1 ? height-2 : i+1;
    int jL = j = 0 ? 1 : j-1;
    int jR = j == width-1 ? width-2 : j+1;
    int idx;
    int energy = 0;
    //printf("postprocessing i=%d, j=%d\n", i, j);
    for(idx = 0; idx < 17; idx += 8) {
        int x_energy, y_energy;
        // X convolve
        x_energy = (inarr[iT*width+jR] >> idx) % 256;
        x_energy += (2*inarr[i*width+jR] >> idx) % 256;
        x_energy += (inarr[iB*width+jR] >> idx) % 256;
        x_energy -= (inarr[iT*width+jL] >> idx) % 256;
        x_energy -= (2*inarr[i*width+jL] >> idx) % 256;
        x_energy -= (inarr[iB*width+jL] >> idx) % 256;
        // Y convolve
        y_energy = (inarr[iB*width+jL] >> idx) % 256;
        y_energy += (2*inarr[iB*width+j] >> idx) % 256;
        y_energy += (inarr[iB*width+jR] >> idx) % 256;
        y_energy -= (inarr[iT*width+jL] >> idx) % 256;
        y_energy -= (2*inarr[iT*width+j] >> idx) % 256;
        y_energy -= (inarr[iT*width+jR] >> idx) % 256;

        energy += y_energy*y_energy+x_energy*x_energy;
    }   
    return energy;
}
int *postprocess(int *inarray, int width, int height, int (*shader)(int *, int, int, int, int)) {
	int i, j, idx = 0;
	int *outarr = malloc(width*height*sizeof(int));
	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++, idx++) {
			outarr[idx] = shader(inarray, width, height, i, j);
		}
	}
	return outarr;
}

int default_postprocess_shader(int *inarray, int width, int height, int i, int j) {
	return inarray[i*width+j];
}