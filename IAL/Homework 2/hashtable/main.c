#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int HT_SIZE = 13;

int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}
int main()
{

//while (found != NULL && strcmp(found->key,key))
char s1[] = "hi";
char s2[] = "hi";
char s3[] = "hello there";
char *found1 = NULL;
char *found2 = s1;
printf("%d\n", strcmp(s1,s2));
printf("%d\n", strcmp(s1,s3));
if (strcmp(s1,s3))
  printf("hello\n");
if (found2 != NULL && strcmp(s1,s3))
  printf("hi\n");

}