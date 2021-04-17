#include <stdio.h>
#include <stdint.h>


int canarin2(int a, int b) {

	int i;

	printf("address of a: %p\n", &a);
	printf("address of b: %p\n", &b);

	for (i=5; i>=0; i--) printf("+%d %p %x\n", i, &b+i, *(&b+i));
	for (i=1; i<10; i++) printf("-%d %p %x\n", i, &b-i, *(&b-i));

		register void *sp asm ("sp");
printf("c2 sp: %p %x\n", sp,  * (unsigned int*)sp);
	
	return 0;

}

void canarin(int a, int b) {

	int i = canarin2(21, 22);

	printf("address of a: %p\n", &a);
	printf("address of b: %p\n", &b);

	for (i=5; i>=0; i--) printf("+%d %p %x\n", i, &b+i, *(&b+i));
	for (i=1; i<10; i++) printf("-%d %p %x\n", i, &b-i, *(&b-i));

		register void *sp asm ("sp");
printf("c1 sp %p %x\n", sp, * (unsigned int*)sp);

}

int main(int argc, char **argv) {
	
	if (argc==1) return 0;
	
	/*unsigned char *buffer[1000];
	for (int i=0; i<1000; i++) buffer[i]=0;

	int num = atoi(argv[1]);
	
	printf("reference address &num: %p\n", &num);

	printf("num is: %lx\n", *(&num));

	int cont = 0;

	for (int i=0; i<100; i++) {buffer[cont++] = *(&num+i); printf("%d %x\n", cont-1, buffer[cont-1]);}
	//for (int i=0; i<num; i++) buffer[cont++] = *(&num-i);

	for (int i=num; i>=0; i--) printf("+%d %x\n", i, buffer[i]);
	for (int i=1; i<num; i++) printf("-%d %x\n", i, buffer[num+i]);
	for (int i=0; i<cont; i++) printf("%d %x\n", i, buffer[i]);*/

	canarin(13, 17);

	printf("\n");	
	
	return 0;
}