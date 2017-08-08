int *postprocess(int *inarray, int width, int height, int (*shader)(int *, int, int, int, int)) {
	int i, j, idx = 0;
	int *outarr = malloc(width*height*sizeof(int));
	for(i = 0; i < width; i++) {
		for(j = 0; j < height; j++; idx++) {
			outarr[idx] = shader(inarray, width, height, i, j);
		}
	}
	return outarr;
}