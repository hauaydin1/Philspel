/*
 * Include the provided hashtable library
 */
#include "hashtable.h"

/*
 * Include the header file
 */
#include "philspel.h"

/*
 * Standard IO and file routines
 */
#include <stdio.h>

/*
 * General utility routines (including malloc())
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routine
 */
#include <string.h>

#include <unistd.h>

/*
 * this hashtable stores the dictionary
 */
HashTable *dictionary;

/*
 * the MAIN routine.  You can safely print debugging information
 * to standard error (stderr) and it will be ignored in the grading
 * process in the same way which this does.
 */
int main(int argc, char **argv){
  if(argc != 2){
    fprintf(stderr, "Specify a dictionary\n");
    return 0;
  }
  /*
   * Allocate a hash table to store the dictionary
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(2255, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /* main in C should always return 0 as a way of telling
     whatever program invoked this that everything went OK
     */
  return 0;
}

/*
 * You need to define this function. void *s can be safely casted
 * to a char * (null terminated string) which is done for you here for
 * convenience.
 */

unsigned int stringHash(void *s){
  char *string = (char *) s;
  unsigned int hash = 0;
  while (*string) {
        // equivalent to: hash = 65599*hash + (*str++);
    hash = (*string++) + (hash << 6) + (hash << 16) - hash;
  }
  return (hash & 0x7FFFFFFF);
  /* hashcode generator obtained from https://www.byvoid.com/zhs/blog/string-hash-compare */
}

/*
 * You need to define this function.  It should return a nonzero
 * value if the two strings are identical (case sensitive comparison)
 * and 0 otherwise.
 */
int stringEquals(void *s1, void *s2){
  char *string1 = (char *) s1;
  char *string2 = (char *) s2;
  if (strcmp(string1, string2) == 0) {
    return 1;
  }
  else {
    return 0;
  }
  /*fprintf(stderr,"Need to define stringEquals\n");
  exit(0);*/
}

/*
 * this function should read in every word in the dictionary and
 * store it in the dictionary.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, exit.  You will need to allocate
 * (using malloc()) space for each word.  As described in the specs, you
 * can initially assume that no word is longer than 70 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You can NOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(0)
 * to cleanly exit the program. Since the format is one word at a time, with
 * returns in between, you can
 * safely use fscanf() to read in the strings.
 */
 void readDictionary(char *filename){
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
      fprintf(stderr, "File does not exist");
      exit(0);
    }
    else {
      int count = 0;
      char c = fgetc(fp);
      char *storage = (char*) malloc(70 * sizeof(char));
      char nextLine = '\n';
      while (c != EOF) {
        while (c != nextLine) {
          storage[count] = c;
          count += 1;
          c = fgetc(fp);
        }
        storage[count] = '\0'; 
        char* temp = malloc((strlen(storage) + 1) * sizeof(char));
        strcpy(temp, storage);
        insertData(dictionary, temp, temp);
       // printf("%s", findData(dictionary, "this"));
        count = 0;
        c = fgetc(fp);
        //printf("%s\n", storage);
      }
    }
  }

  /*fprintf(stderr,"Need to define readDictionary\n");
  exit(0);*/







/*
 * This should process standard input and copy it to standard output
 * as specified in specs.  EG, if a standard dictionary was used
 * and the string "this is a taest of  this-proGram" was given to
 * standard input, the output to standard output (stdout) should be
 * "this is a teast [sic] of  this-proGram".  All words should be checked
 * against the dictionary as they are input, again with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not being found, by appending " [sic]" after the
 * error.
 *
 * Since we care about preserving whitespace, and pass on all non alphabet
 * characters untouched, and with all non alphabet characters acting as
 * word breaks, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), so you will probably have
 * to get characters from standard input one at a time.
 *
 * As stated in the specs, you can initially assume that no word is longer than
 * 70 characters, but you may have strings of non-alphabetic characters (eg,
 * numbers, punctuation) which are longer than 70 characters. For the final 20%
 * of your grade, you can no longer assume words have a bounded length.
 */
// void processInput(){

//   fprintf(stderr,"Need to define processInput\n");
//   exit(0);
// }

void processInput(){
  char c;
  int count = 0; 
  char *sic = " [sic]";
  int original = 0;
  int lower = 0;
  int capital = 0;
  char *wordlst = malloc(70 * sizeof(char));
  while((c = getchar())) {
    if (isalpha(c)) {
      wordlst[count] = c;
      count++;
    } 
    else {
      wordlst[count] = '\0';
      if (wordlst[0] != '\0') {
        fprintf(stdout, "%s", wordlst);
        //char tempList[70];
        //strcpy(tempList, wordlst);
        //fprintf(stdout, "About to look for: %s\n", wordlst);
        if (findData(dictionary, wordlst) != NULL) {
          original = 1;
        }
        int i = 0;
        for (i = 1; wordlst[i] != '\0'; i++) {
          wordlst[i] = tolower(wordlst[i]);
        }
        if (findData(dictionary, wordlst) != NULL) {
          capital = 1;
        }        
        wordlst[0] = tolower(wordlst[0]);
        if (findData(dictionary, wordlst) != NULL) {
          lower = 1;
        }

        if (!original && !lower && !capital) {
          // fprintf(stdout, "original value is %d\n", original);

          // fprintf(stdout, "capital value is %d\n", capital);          
          // fprintf(stdout, "lower value is %d\n", lower);
          fprintf(stdout, "%s", sic);
        }
      }
      count = 0;
      original = 0;
      lower = 0;
      capital = 0;
      wordlst = (char *) malloc(sizeof(char) * 70);

      if (c != EOF) {
        putchar(c);
      } 
      else {
        return;
      }  
    }
  } 
}
