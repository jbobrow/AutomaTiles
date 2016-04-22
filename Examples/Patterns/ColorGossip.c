void setup() {
	
}

void draw() {
	
}

void interpolateRGBColor(int[3] &result, int[3] from, int[3] to, float percent) {
	// first convert colors to HSV
	int[3] fromHSV = getHSVfromRGB(fromHSV, from);
	int[3] toHSV = getHSVfromRGB(toHSV, to);
	// tween between HSV values
	int[3] resultHSV;
	resultHSV[0] = fromHSV[0] * (1.0 - percent) + toHSV[0] * percent;
	resultHSV[1] = fromHSV[1] * (1.0 - percent) + toHSV[1] * percent;
	resultHSV[2] = fromHSV[2] * (1.0 - percent) + toHSV[2] * percent;
	getRGBfromHSV(result, resultHSV);
}

void getRGBfromHSV(int[3] &rgb, int[3] hsv) {

}

void getHSVfromRGB(int[3] &hsv, int[3] rgb) {

}
