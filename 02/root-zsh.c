#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
	char* const* args = NULL;
	system("whoami");
	system("id");
	execl("/bin/zsh", "/bin/zsh", (const char*) NULL);
}
