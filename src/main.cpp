
// main.cpp

// includes

#include <cstdio>
#include <cstdlib>

#include "attack.h"
#include "book.h"
#include "hash.h"
#include "move_do.h"
#include "option.h"
#include "pawn.h"
#include "piece.h"
#include "protocol.h"
#include "random.h"
#include "square.h"
#include "trans.h"
#include "util.h"
#include "value.h"
#include "vector.h"
#include "bitbase.h"
#include "person.h"

// functions
void LoadPersonalityFile(void);
void InitVars(void);

// main()

int main(int argc, char * argv[]) {

   // init

   util_init();
   my_random_init(); // for opening book

   printf("Gambit Fruit is based on Fruit 2.1 by Fabien Letouzey with changes by Thomas Gaksch and Ryan Benitez\n");
   //printf("Bitbases by Daniel Shawul\n");
   printf("Personality Code by Dann Corbit\n");

   // early initialisation (the rest is done after UCI options are parsed in protocol.cpp)

   LoadPersonalityFile();
   //init_bitbases();
   option_init();

   square_init();
   piece_init();
   pawn_init_bit();
   value_init();
   vector_init();
   attack_init();
   move_do_init();

   random_init();
   hash_init();

   trans_init(Trans);
   book_init();

   // loop

   loop();

   return EXIT_SUCCESS;
}

/* Load in the personality file */
void LoadPersonalityFile(void) {
  FILE *fp;
   
  init_personality();

  if ((fp = fopen("default.per","r")) != NULL) {
    fprintf(stdout,"Loading Personality File %s ... ","default.per");
    (void)read_person(fp);
    fclose(fp);
    fprintf(stdout,"OK\n");
  }
  else {
    fprintf(stdout,"Could Not Find Personality File %s. \n","default.per");
  }
}


// end of main.cpp

