

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_SIZE_OF_PASSWORD 100
#define Q 256 //ASCII symbols quantity

  float total_length = 0;
  int MIN = 101; // minimal length of input passwords for easy checking
  float AVG = 0; // average passwords length
  float q = 0; //passwords' quantity
  bool Stats = false;
  bool args_status_check = false;
  int LEVEL; // starting parameter, determines security's level
  int PARAM;
  char array[MAX_SIZE_OF_PASSWORD+1];  // +1 because we want to have some space for '\0' symbol
  char different_symbols[Q+1];
  int M = 0;
  int n = 0;


  int my_islower (){

      for (int i = 0; array[i] != '\0'; i++){
          if ((97 <= array[i]) && (array[i] <= 122)) return 0;
      }
    return 1;
  }


  int my_isupper (){

      for (int i = 0; array[i] != '\0'; i++){
          if ((65 <= array[i]) && (array[i] <= 90)) return 0;
      }
      return 1;
  }

  int my_isdigit (){

        for (int i = 0; array[i] != '\0'; i++){
            if ((48 <= array[i]) && (array[i] <= 57)) return 0;
        }
        return 1;
  }



void input_parameters_check(){

    if ((LEVEL < 1) || (LEVEL > 4)){
        printf ("error: Value of input argument LEVEL has to be unsigned integer in interval [1;4]\n");
        args_status_check = true;
    }
    if (PARAM < 1){
        args_status_check = true;
        printf ("error: Value of input argument PARAM has to be unsigned integer and also is not equal to 0\n");
    }

}

void passwords_input()
{
    fgets(array, MAX_SIZE_OF_PASSWORD, stdin);
}

int passwords_security_level()
{
  int N = PARAM - 1;
  bool hasAcceptibleSymbols = false;
  bool hasUpperCase = false;
  bool hasLowerCase = false;
  bool hasDigit = false;
  bool sameSymbolsInARow = false;
  bool sameSubstringsInARow = false;

      if (LEVEL >= 1 && PARAM >= 1){

        // the password has to contain at least 1 lowercase letter

          for (int i = 0;array[i] != '\0'; i++){
              if (my_islower(array[i]) == 0) hasLowerCase = true;
          }
          if (hasLowerCase == false) return 1;


        //the password has to contain at least 1 uppercase letter

      for (int i = 0;array[i] != '\0'; i++){
          if (my_isupper(array[i]) == 0) hasUpperCase = true;
      }
      if (hasUpperCase == false) return 1;
          if(hasLowerCase != 0 && hasUpperCase != 0 && LEVEL == 1) return 0;

      }


      if (LEVEL >= 2){

         // the password has to contain at least 1 digit

          if (PARAM >= 3 || LEVEL > 2){
              for (int i = 0;array[i] != '\0'; i++){
                  if(my_isdigit(array[i]) == 0) hasDigit = true;
              }
            if (hasDigit == false) return 1;
          }


        if (PARAM >= 4 || LEVEL > 2){

            //the password has to contain special symbols

        for (int i = 0;array[i] != '\0'; i++){
            if (((32 <= array[i]) && (array[i] <= 47)) ||
                ((58 <= array[i]) && (array[i] <= 64)) ||
                ((91 <= array[i]) && (array[i] <= 96)) ||
                ((123 <= array[i]) && (array[i] <= 126))) hasAcceptibleSymbols = true;
        }

            if (hasAcceptibleSymbols == false) return 1;
        }

      if (LEVEL == 2) return 0;
      }

       if (LEVEL >= 3){

         //the password cannot contain "PARAM - 1"  or more symbols in a row

       int count = 0;
       int max_count = 0;
          if (PARAM == 1) return 0;
              for (int i = 0;array[i] != '\0'; i++){
                  if (PARAM >= 2){
                      if (array[i] == array[i+1]) count++;
                      if (count > max_count) max_count = count;
                      if (array[i] != array[i+1]) count = 0;
                  }
              }
        if (max_count >= N) sameSymbolsInARow = true;
        if (sameSymbolsInARow == true) return 1;
        else
          return 0;
       }





      if (LEVEL == 4){
          // the password cannot contain PARAM substrings in a row
          if (PARAM == 1) return 0;
              for (int i = 0;array[i] != '\0'; i++){
                  if (PARAM >= 2){
                      if (array[i] == array[i + N]) sameSubstringsInARow = true;
                  }
              }
        if (sameSubstringsInARow == true) return 1;
          else return 0;
      }


return 0;
}


  int my_string_compare(const char *str1, const char *str2){

      for (int i = 0; str2[i] != '\0'; i++){
          if (str1[i] != str2[i]) return 1;
      }
   return 0;

  }





int show()
{
    int countOfSymbols = 0;
        for (int i = 0; array[i] != '\0'; i++) countOfSymbols++;
            if (countOfSymbols >= MAX_SIZE_OF_PASSWORD)
                printf("error: you input too long password, the max length of password is %d symbols\n", MAX_SIZE_OF_PASSWORD);
if (passwords_security_level(LEVEL, PARAM) == 0)
    printf("%s\n", array);

return 0;
}




void stats_show(){
    int NCHARS = 0;
        // passwords' minimal length counter
    int current_length = 0;

        if (Stats == true){
            for (int i = 0; array[i] != '\0'; i++){
            current_length++;
            }
                total_length = total_length + current_length;
                q++;
                    if (MIN > current_length) MIN = current_length;
                        AVG = total_length/q;



bool found = false;

        for (int i = 0; array[i] != '\0'; i++){
            for (int j = 0; different_symbols[j] != '\0' ; j++){
                if (different_symbols[j] == array[i]){
                    found = true;
                }
            }
            if (!found){
                different_symbols[n] = array[i];
                n++;
            }
            found = false;
        }
    for (int i = 0; different_symbols[i] != '\0'; i++) NCHARS++;
        printf("Statistika:\nRuznych znaku:%d\nMinimalni delka:%d\nPrumerna delka:%g\n", NCHARS-1, MIN-1, AVG-1);
      }
}



int main(int argc, char *argv[])
{
   if (argc < 2){
  printf("error: you have to choose input parameters LEVEL and PARAM\n");
  printf("LEVEL - unsigned integer of interval [1;4]\n");
  printf("PARAM - unsigned integer not equal to 0\n");
      return 1;
}

   LEVEL = atoi(argv[1]);
   PARAM = atoi(argv[2]);

   if (argc > 2){
      my_string_compare(argv[3], "--stats");
          if (my_string_compare(argv[3], "--stats") == 0) Stats = true;

    }
    input_parameters_check(LEVEL, PARAM);
        if (args_status_check == true) return 1;
    do
    {
      passwords_input();
      passwords_security_level(LEVEL, PARAM);
      show();
      stats_show();

    }
        while (*array != EOF);

return 0;
}
