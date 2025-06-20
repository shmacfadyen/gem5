all: susan_smoothing susan_edges susan_corners

susan_smoothing: susan.c Makefile
	arm-linux-gnueabi-gcc -static -O4 -o susan_smoothing susan.c -lm 

susan_edges: susan.c Makefile
	arm-linux-gnueabi-gcc -static -O4 -o susan_edges susan.c -lm 

susan_corners: susan.c Makefile
	arm-linux-gnueabi-gcc -static -O4 -o susan_corners susan.c -lm 

clean:
	rm -rf susan_corners susan_edges susan_smoothing output*
