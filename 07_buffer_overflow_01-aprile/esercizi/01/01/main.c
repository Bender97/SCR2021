#include <stdio.h>
#include <stdlib.h>

void loop(unsigned char count, unsigned char *inbuf)
{
    unsigned char i = 0;
    unsigned char buf[8];
    printf("count:%X inbuf:%X i:%X buf:%X\n", &count, inbuf, &i, buf);
    for(i=0;i<count;i++)
    {
        buf[i] = inbuf[i];
        printf("%X %d\n", &(buf[i]), i);
    }

    printf("now return\n");
}

int main(int argc, char** argv)
{
    int val = 7;
    int k = 0;
    int dim = atoi(argv[1]);
    unsigned char theBuf[dim];
    if(argc > 2)
    {
        val = atoi(argv[2]);
    }
    for(k=0;k<dim;k++)
    {
        theBuf[k] = val;
    }
    loop(dim, theBuf);
}
