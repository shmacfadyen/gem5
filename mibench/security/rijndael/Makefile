rijndael: aes.c aes.h aestab.h aesxam.c Makefile
	arm-linux-gnueabi-gcc -static -O3 -o rijndael_encode aes.c aesxam.c
	arm-linux-gnueabi-gcc -static -O3 -o rijndael_decode aes.c aesxam.c

clean:
	rm -rf rijndael_encode rijndael_decode output*
