
// option.cpp

// includes

#include <cstdlib>

#include "option.h"
#include "protocol.h"
#include "util.h"

// types

struct option_t {
   const char * var;
   bool declare;
   const char * init;
   const char * type;
   const char * extra;
   const char * val;
};

// variables

static option_t Option[] = {

   { "Hash", true, "16", "spin", "min 4 max 1024", NULL },

   { "Ponder", true, "false", "check", "", NULL },

   { "OwnBook",  true, "true",           "check",  "", NULL },
   { "BookFile", true, "book_small.bin", "string", "", NULL },

   { "Extension cap",     true, "16",        "spin",  "min 16 max 48", NULL },

   { "Check extension level",   true, "16",   "spin",  "min 0 max 32", NULL },
   { "Recap extension level",   true, "16",   "spin",  "min 0 max 32", NULL },
   { "Pawn to 7th extension level",   true, "16",   "spin",  "min 0 max 32", NULL },
   { "Threat extension level",   true, "16",  "spin",  "min 0 max 32", NULL },
   { "Single reply extension level",   true, "8",  "spin",  "min 0 max 32", NULL },

   { "Use same threat extensions",     true, "true", "check", "", NULL },
   { "Exclude Same threat from cap",  true, "true",           "check",  "", NULL },

   { "Same threat extensions level",   true, "8",  "spin",  "min -16 max 16", NULL },

   { "NullMove Pruning",   true, "Always", "combo", "var Always var Fail High var Never", NULL },
   { "NullMove Reduction",     true, "3",         "spin",  "min 1 max 3", NULL },
   { "Verification Search",    true, "Always",   "combo", "var Always var Endgame var Never", NULL },
   { "Verification Reduction", true, "5",         "spin",  "min 1 max 6", NULL },

   { "History Pruning",     true, "true", "check", "", NULL },
   { "History Threshold",   true, "60",   "spin",  "min 0 max 100", NULL },

   { "Rebel Reduction",     true, "false", "check", "", NULL },

   { "Futility Pruning", true, "true", "check", "", NULL },
   { "Futility Margin (Depth=1)",  true, "300",   "spin",  "min 0 max 3500", NULL },
   { "Futility Margin (Depth=2)",  true, "500",   "spin",  "min 0 max 3500", NULL },
   { "Futility Margin (Depth=3)",  true, "975",   "spin",  "min 0 max 3500", NULL },

   { "Delta Pruning", true, "false", "check", "", NULL },
   { "Delta Margin",  true, "50",    "spin",  "min 0 max 1000", NULL },

   { "Quiescence Check Plies", true, "1", "spin", "min 0 max 2", NULL },

   { "Piece Activity",  true, "100", "spin", "min 0 max 400", NULL },
   { "King Safety",     true, "100", "spin", "min 0 max 400", NULL },
   { "Passed Pawns",    true, "100", "spin", "min 0 max 400", NULL },
   { "Pawn square table weight",  true, "100", "spin", "min 0 max 400", NULL },

   { "Use King Tropism", 	true, "true", "check", "", NULL },
   { "knight tropism",  	true, "4",    "spin",  "min 0 max 10", NULL },
   { "bishop tropism",  	true, "2",    "spin",  "min 0 max 10", NULL },
   { "rook tropism",  		true, "2",    "spin",  "min 0 max 10", NULL },
   { "queen tropism",  		true, "4",    "spin",  "min 0 max 10", NULL },

   { "Fruit Lazy Eval", true, "true", "check", "", NULL },
   { "Fruit Lazy Eval Margin",  true, "200",    "spin",  "min 0 max 900", NULL },
   { "Fruit Exchange Bonus",  true, "30",    "spin",  "min 0 max 100", NULL }, 
   { "Fruit King Safety", true, "true", "check", "", NULL },
   { "Fruit King Safety Margin",  true, "1600",    "spin",  "min 500 max 2500", NULL },

   { "Opening Pawn Value",	true, "90", "spin", "min 0 max 1000", NULL },
   { "Opening Knight Value",	true, "320", "spin", "min 0 max 1000", NULL },
   { "Opening Bishop Value",	true, "325", "spin", "min 0 max 1000", NULL },
   { "Opening Rook Value",	true, "500", "spin", "min 0 max 1000", NULL },
   { "Opening Queen Value",	true, "975", "spin", "min 0 max 1000", NULL },

   { "Endgame Pawn Value",	true, "100", "spin", "min 0 max 1000", NULL },
   { "Endgame Knight Value",	true, "320", "spin", "min 0 max 1000", NULL },
   { "Endgame Bishop Value",	true, "325", "spin", "min 0 max 1000", NULL },
   { "Endgame Rook Value",	true, "500", "spin", "min 0 max 1000", NULL },
   { "Endgame Queen Value",	true, "975", "spin", "min 0 max 1000", NULL },

   { "Bishop Pair Opening",	true, "50", "spin", "min 0 max 1000", NULL },
   { "Bishop Pair Endgame",	true, "50", "spin", "min 0 max 1000", NULL },

   { "Queen Knight combo", 	true, "20", "spin", "min 0 max 1000", NULL },
   { "Rook Bishop combo",	true, "20", "spin", "min 0 max 1000", NULL },

   { "Rook on bad pawn file opening bonus", 	true, "8", "spin", "min 0 max 100", NULL },
   { "Rook on bad pawn file endgame bonus",	true, "8", "spin", "min 0 max 100", NULL },

   { "Pawn Duo Opening", 	true, "6", "spin", "min 0 max 100", NULL },
   { "Pawn Duo Endgame",	true, "6", "spin", "min 0 max 100", NULL },

   { "Pawn Doubled Opening", 	true, "10", "spin", "min 0 max 100", NULL },
   { "Pawn Doubled Endgame",	true, "20", "spin", "min 0 max 100", NULL },

   { "Pawn Isolated Opening", 	true, "12", "spin", "min 0 max 100", NULL },
   { "Pawn Isolated Opening Open",true, "16", "spin", "min 0 max 100", NULL },
   { "Pawn Isolated Endgame", 	true, "20", "spin", "min 0 max 100", NULL },

   { "Pawn Backward Opening",	true, "8", "spin", "min 0 max 100", NULL },
   { "Pawn Backward Opening Open",true, "12", "spin", "min 0 max 100", NULL },
   { "Pawn Backward Endgame",	true, "12", "spin", "min 0 max 100", NULL },

   { "Pawn Candidate Opening Min", 	true, "5", "spin", "min 0 max 500", NULL },
   { "Pawn Candidate Opening Max",	true, "55", "spin", "min 0 max 500", NULL },
   { "Pawn Candidate Endgame Min", 	true, "10", "spin", "min 0 max 500", NULL },
   { "Pawn Candidate Endgame Max",	true, "110", "spin", "min 0 max 500", NULL },

   { "Contempt Factor", 	true, "0",    "spin",  "min -1000 max 1000", NULL },

   { NULL, false, NULL, NULL, NULL, NULL, },
};

// prototypes

static option_t * option_find (const char var[]);

// functions

// option_init()

void option_init() {

   option_t * opt;

   for (opt = &Option[0]; opt->var != NULL; opt++) {
      option_set(opt->var,opt->init);
   }
}

// option_list()

void option_list() {

   option_t * opt;

   for (opt = &Option[0]; opt->var != NULL; opt++) {
      if (opt->declare) {
         if (opt->extra != NULL && *opt->extra != '\0') {
            send("option name %s type %s default %s %s",opt->var,opt->type,opt->val,opt->extra);
         } else {
            send("option name %s type %s default %s",opt->var,opt->type,opt->val);
         }
      }
   }
}

// option_set()

bool option_set(const char var[], const char val[]) {

   option_t * opt;

   ASSERT(var!=NULL);
   ASSERT(val!=NULL);

   opt = option_find(var);
   if (opt == NULL) return false;

   my_string_set(&opt->val,val);

   return true;
}

// option_get()

const char * option_get(const char var[]) {

   option_t * opt;

   ASSERT(var!=NULL);

   opt = option_find(var);
   if (opt == NULL) my_fatal("option_get(): unknown option \"%s\"\n",var);

   return opt->val;
}

// option_get_bool()

bool option_get_bool(const char var[]) {

   const char * val;

   val = option_get(var);

   if (false) {
   } else if (my_string_equal(val,"true") || my_string_equal(val,"yes") || my_string_equal(val,"1")) {
      return true;
   } else if (my_string_equal(val,"false") || my_string_equal(val,"no") || my_string_equal(val,"0")) {
      return false;
   }

   ASSERT(false);

   return false;
}

// option_get_int()

int option_get_int(const char var[]) {

   const char * val;

   val = option_get(var);

   return atoi(val);
}

// option_get_string()

const char * option_get_string(const char var[]) {

   const char * val;

   val = option_get(var);

   return val;
}

// option_find()

static option_t * option_find(const char var[]) {

   option_t * opt;

   ASSERT(var!=NULL);

   for (opt = &Option[0]; opt->var != NULL; opt++) {
      if (my_string_equal(opt->var,var)) return opt;
   }

   return NULL;
}

// end of option.cpp

