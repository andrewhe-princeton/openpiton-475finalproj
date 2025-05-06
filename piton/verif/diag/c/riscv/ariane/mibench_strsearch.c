/* +++Date last modified: 05-Jul-1997 */

/*
**        A Pratt-Boyer-Moore string search, written by Jerry Coffin
**  sometime or other in 1991.  Removed from original program, and
**  (incorrectly) rewritten for separate, generic use in early 1992.
**  Corrected with help from Thad Smith, late March and early
**  April 1992...hopefully it's correct this time. Revised by Bob Stout.
**
**  This is hereby placed in the Public Domain by its author.
**
**  10/21/93 rdg  Fixed bug found by Jeff Dunlop
*/

#include <stddef.h>
/* #include <string.h> */ /* Removed string.h */
#include <limits.h>

/* Custom strlen implementation */
size_t my_strlen(const char *s) {
    size_t length = 0;
    while (s[length] != '\0') {
        length++;
    }
    return length;
}

/* Custom strncmp implementation */
int my_strncmp(const char *s1, const char *s2, size_t n) {
    if (n == 0) {
        return 0;
    }
    while (n > 0) {
        if (*s1 != *s2) {
            return *(unsigned char *)s1 - *(unsigned char *)s2;
        }
        if (*s1 == '\0') { 
            return 0; 
        }
        s1++;
        s2++;
        n--;
    }
    return 0; /* n characters matched */
}


static size_t table[UCHAR_MAX + 1];
static size_t len;
static char *findme;

/*
**  Call this with the string to locate to initialize the table
*/

void init_search(const char *string)
{
      size_t i;

      len = my_strlen(string); /* Replaced strlen with my_strlen */
      for (i = 0; i <= UCHAR_MAX; i++)                      /* rdg 10/93 */
            table[i] = len;
      for (i = 0; i < len; i++)
            table[(unsigned char)string[i]] = len - i - 1;
      findme = (char *)string;
}

/*
**  Call this with a buffer to search
*/

char *strsearch(const char *string)
{
      register size_t shift;
      register size_t pos = len - 1;
      char *here;
      size_t limit=my_strlen(string); /* Replaced strlen with my_strlen */

      while (pos < limit)
      {
            while( pos < limit &&
                  (shift = table[(unsigned char)string[pos]]) > 0)
            {
                  pos += shift;
            }
            if (0 == shift)
            {
                  if (0 == my_strncmp(findme,
                        here = (char *)&string[pos-len+1], len)) /* Replaced strncmp with my_strncmp */
                  {
                        return(here);
                  }
                  else  pos++;
            }
      }
      return NULL;
}

#include <stdio.h>

// ANDREW: Modified this to fill the cache
#define SEGMENT_A_128 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
#define SEGMENT_B_128 "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
#define SEGMENT_C_128 "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
#define SEGMENT_D_128 "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"



// Define a 1KB block (1024 chars = 8 * 128 chars)
#define BLOCK_A_1K SEGMENT_A_128 SEGMENT_A_128 SEGMENT_A_128 SEGMENT_A_128 SEGMENT_A_128 SEGMENT_A_128 SEGMENT_A_128 SEGMENT_A_128
#define BLOCK_B_1K SEGMENT_B_128 SEGMENT_B_128 SEGMENT_B_128 SEGMENT_B_128 SEGMENT_B_128 SEGMENT_B_128 SEGMENT_B_128 SEGMENT_B_128
#define BLOCK_C_1K SEGMENT_C_128 SEGMENT_C_128 SEGMENT_C_128 SEGMENT_C_128 SEGMENT_C_128 SEGMENT_C_128 SEGMENT_C_128 SEGMENT_C_128
#define BLOCK_D_1K SEGMENT_D_128 SEGMENT_D_128 SEGMENT_D_128 SEGMENT_D_128 SEGMENT_D_128 SEGMENT_D_128 SEGMENT_D_128 SEGMENT_D_128


#define BLOCK_A_16K \
    BLOCK_A_1K BLOCK_A_1K BLOCK_A_1K BLOCK_A_1K \
    BLOCK_A_1K BLOCK_A_1K BLOCK_A_1K BLOCK_A_1K \
    BLOCK_A_1K BLOCK_A_1K BLOCK_A_1K BLOCK_A_1K \
    BLOCK_A_1K BLOCK_A_1K BLOCK_A_1K BLOCK_A_1K
#define BLOCK_B_16K \
    BLOCK_B_1K BLOCK_B_1K BLOCK_B_1K BLOCK_B_1K \
    BLOCK_B_1K BLOCK_B_1K BLOCK_B_1K BLOCK_B_1K \
    BLOCK_B_1K BLOCK_B_1K BLOCK_B_1K BLOCK_B_1K \
    BLOCK_B_1K BLOCK_B_1K BLOCK_B_1K BLOCK_B_1K
#define BLOCK_C_16K \
    BLOCK_C_1K BLOCK_C_1K BLOCK_C_1K BLOCK_C_1K \
    BLOCK_C_1K BLOCK_C_1K BLOCK_C_1K BLOCK_C_1K \
    BLOCK_C_1K BLOCK_C_1K BLOCK_C_1K BLOCK_C_1K \
    BLOCK_C_1K BLOCK_C_1K BLOCK_C_1K BLOCK_C_1K
#define BLOCK_D_16K \
    BLOCK_D_1K BLOCK_D_1K BLOCK_D_1K BLOCK_D_1K \
    BLOCK_D_1K BLOCK_D_1K BLOCK_D_1K BLOCK_D_1K \
    BLOCK_D_1K BLOCK_D_1K BLOCK_D_1K BLOCK_D_1K \
    BLOCK_D_1K BLOCK_D_1K BLOCK_D_1K BLOCK_D_1K


#define STRING_A_80K_BASE BLOCK_A_16K BLOCK_A_16K BLOCK_A_16K BLOCK_A_16K BLOCK_A_16K
#define STRING_B_80K_BASE BLOCK_B_16K BLOCK_B_16K BLOCK_B_16K BLOCK_B_16K BLOCK_B_16K
#define STRING_C_80K_BASE BLOCK_C_16K BLOCK_C_16K BLOCK_C_16K BLOCK_C_16K BLOCK_C_16K
#define STRING_D_80K_BASE BLOCK_D_16K BLOCK_D_16K BLOCK_D_16K BLOCK_D_16K BLOCK_D_16K


#define STRING_A_80K STRING_A_80K_BASE "AlphaPatternHere"
#define STRING_B_80K STRING_B_80K_BASE "BravoPatternHere"
#define STRING_C_80K STRING_C_80K_BASE "CharliePatternHere"
#define STRING_D_80K STRING_D_80K_BASE "DeltaPatternHere"

int main()
{
      char *here;
      char *find_strings[] = {
"AlphaPatternHere",
NULL};
      char *search_strings[] = { 
        STRING_A_80K,
        STRING_B_80K,
        STRING_C_80K,
        STRING_D_80K,
        STRING_A_80K /
        // Ensure search_strings has a corresponding entry for each find_strings entry
};
      int i;

      for (i = 0; find_strings[i]; i++)
      {
            init_search(find_strings[i]);
            here = strsearch(search_strings[i]);
      
            if (here)
              printf("found");
            else
              printf("not found");
            putchar('\n');
      }

      return 0;
}
