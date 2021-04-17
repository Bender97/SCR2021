# LIVE buffer overflow lesson
## Exercise 3

```
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
```
> from loop.c

`if count<8   (e.g., count = 7), it's all ok`

when count>=8, buffer overflow:<br>
	when i=8, inside for loop:<br>
		`buf[8] = inbuf[8]`

BUT &(buf[8]) is equal to &i <br>
AND inbuf[8] is some value<br>

so we change the value of the iterator. IF inbuf[8]<count, THEN infinte loop