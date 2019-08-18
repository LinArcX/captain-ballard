#include <stdio.h>
#include <string.h>

#define bufSize 1024

int read_file_line_by_line(char *path) {

  FILE *fp;
  char buf[bufSize];
  if ((fp = fopen(path, "r")) == NULL) {
    /* Open source file. */
    perror("fopen source-file");
    return 1;
  }

  while (fgets(buf, sizeof(buf), fp) != NULL) {
    buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
    printf("%s\n", buf);
  }
  fclose(fp);
  return 0;
}
