
// search_full.cpp

// includes

#include "attack.h"
#include "board.h"
#include "colour.h"
#include "eval.h"
#include "list.h"
#include "move.h"
#include "move_check.h"
#include "move_do.h"
#include "option.h"
#include "pawn.h"
#include "piece.h"
#include "pst.h"
#include "pv.h"
#include "recog.h"
#include "search.h"
#include "search_full.h"
#include "see.h"
#include "sort.h"
#include "trans.h"
#include "util.h"
#include "value.h"
#include "person.h"

#define ABS(x) ((x)<0?-(x):(x))

int ValueDraw;

// constants and variables
static const int Fractional = 16;

static int ext_cap = 16;
static int check_ext = 16;
static int recap_ext = 16;
static int pawn_ext = 16;
static int single_reply_ext = 16;
static int threat_ext = 16;
//static int ppush_ext = 8;

static bool exclude_bme_from_cap = false;
static bool use_bme = false;
static int bm_ext = 12;

// Fractional Extensions 
#define REAL_DEPTH(depth) (depth/Fractional)
#define FRACT_DEPTH(depth) (depth*Fractional)

// main search

static const bool UseDistancePruning = true;

// transposition table

//static const bool UseTrans = true;
static const int TransDepth = Fractional;

static const bool UseMateValues = true; // use mate values from shallower searches?

// null move

static /* const */ bool UseNull = true;
static /* const */ bool UseNullEval = true; // true
static const int NullDepth = 2*Fractional;
static /* const */ int NullReduction = 3*Fractional;

static /* const */ bool UseVer = true;
static /* const */ bool UseVerEndgame = true; // true
static /* const */ int VerReduction = 5*Fractional; // was 3

// move ordering

static const bool UseIID = true;
static const int IIDDepth = 3*Fractional;
static const int IIDReduction = 2*Fractional;

// extensions

static const bool ExtendSingleReply = true; // true

// rebel reduction
static bool use_rebel_reduction = true; // I hope I did this right...

// history pruning

static /* const */ bool UseHistory = true;
static const int HistoryDepth = 3*Fractional;
static const int HistoryMoveNb = 3;
static /* const */ int HistoryValue = 9830; // 60%
//static const int HistoryBound = 2458; // * 16384 + 50) / 100 10%=1638 15%=2458 20%=3277
//static const bool UseExtendedHistory = false;
static const bool HistoryReSearch = true;

// futility pruning

static /* const */ bool UseFutility = false; // false
static /* const */ int FutilityMargin1 = 300;
static /* const */ int FutilityMargin2 = 500;
static /* const */ int FutilityMargin3 = 950;

// quiescence search

static /* const */ bool UseDelta = false; // false
static /* const */ int DeltaMargin = 50;

static /* const */ int CheckNb = 1;
static /* const */ int CheckDepth = 0; // 1 - CheckNb

// misc

static const int NodeAll = -1;
static const int NodePV  =  0;
static const int NodeCut = +1;

// macros

#define NODE_OPP(type)     (-(type))
#define DEPTH_MATCH(d1,d2) ((d1)>=(d2))

// prototypes

static int  full_root            (list_t * list, board_t * board, int alpha, int beta, int depth, int height, int search_type);

#if MP
static int  full_search          (board_t * board, int alpha, int beta, int depth, int height, mv_t pv[], int node_type, int exclusive);
#else
static int  full_search          (board_t * board, int alpha, int beta, int depth, int height, mv_t pv[], int node_type);
#endif

static int  full_no_null         (board_t * board, int alpha, int beta, int depth, int height, mv_t pv[], int node_type, int trans_move, int * best_move);

static int  full_quiescence      (board_t * board, int alpha, int beta, int depth, int height, mv_t pv[]);

static int  full_new_depth       (int depth, int move, board_t * board, bool single_reply, bool in_pv, bool threat, bool same_threat);

static bool do_null              (const board_t * board);
static bool do_ver               (const board_t * board);

static void pv_fill              (const mv_t pv[], board_t * board);

static bool move_is_dangerous    (int move, const board_t * board);
static bool capture_is_dangerous (int move, const board_t * board);

static bool simple_stalemate     (const board_t * board);

static bool is_passed(const board_t * board, int to);

// functions

// search_init()

void search_init() {
   const char * string;


   // null-move options

   string = option_get_string("NullMove Pruning");

   if (false) {
   } else if (my_string_equal(string,"Always")) {
      UseNull = true;
      UseNullEval = false;
   } else if (my_string_equal(string,"Fail High")) {
      UseNull = true;
      UseNullEval = true;
   } else if (my_string_equal(string,"Never")) {
      UseNull = false;
      UseNullEval = false;
   } else {
      ASSERT(false);
      UseNull = true;
      UseNullEval = true;
   }

   NullReduction = option_get_int("NullMove Reduction")*Fractional;

   string = option_get_string("Verification Search");

   if (false) {
   } else if (my_string_equal(string,"Always")) {
      UseVer = true;
      UseVerEndgame = false;
   } else if (my_string_equal(string,"Endgame")) {
      UseVer = true;
      UseVerEndgame = true;
   } else if (my_string_equal(string,"Never")) {
      UseVer = false;
      UseVerEndgame = false;
   } else {
      ASSERT(false);
      UseVer = true;
      UseVerEndgame = true;
   }

   VerReduction = option_get_int("Verification Reduction")*Fractional;

if (!person.override_ucioptions) {
   // draw value
   ValueDraw = option_get_int("Contempt Factor");   

   // extentions
   exclude_bme_from_cap = option_get_bool("Exclude Same threat from cap");
   use_bme = option_get_bool("Use same threat extensions");
   bm_ext = option_get_int("Same threat extensions level");

   ext_cap = option_get_int("Extension cap");
   check_ext = option_get_int("Check extension level");
   recap_ext = option_get_int("Recap extension level");
   pawn_ext = option_get_int("Pawn to 7th extension level");
   threat_ext = option_get_int("Threat extension level");
   single_reply_ext = option_get_int("Single reply extension level");


   // history-pruning options

   UseHistory = option_get_bool("History Pruning");
   HistoryValue = (option_get_int("History Threshold") * 16384 + 50) / 100;

   // rebel reduction options
   use_rebel_reduction = option_get_bool("Rebel Reduction");

   // futility-pruning options

   UseFutility = option_get_bool("Futility Pruning");
   FutilityMargin3 = option_get_int("Futility Margin (Depth=3)");
   FutilityMargin2 = option_get_int("Futility Margin (Depth=2)");
   FutilityMargin1 = option_get_int("Futility Margin (Depth=1)");

   // delta-pruning options

   UseDelta = option_get_bool("Delta Pruning");
   DeltaMargin = option_get_int("Delta Margin");

   // quiescence-search options

   CheckNb = option_get_int("Quiescence Check Plies");
} else {

   // draw value
   ValueDraw = person.Contempt_Factor;   

   // extentions
   exclude_bme_from_cap = person.Exclude_Same_threat_from_cap;
   use_bme = person.Use_same_threat_extensions;
   bm_ext = person.Same_threat_extension_level;

   ext_cap = person.Extension_cap;
   check_ext = person.Check_extension_level;
   recap_ext = person.Recap_extension_level;
   pawn_ext = person.Pawn_to_7th_extension_level;
   threat_ext = person.Threat_extension_level;
   single_reply_ext = person.Single_reply_extension_level;


   // history-pruning options

   UseHistory = person.History_Pruning;
   HistoryValue = (person.History_Threshold * 16384 + 50) / 100;

   // rebel reduction options
   use_rebel_reduction = person.Rebel_Reduction;

   // futility-pruning options

   UseFutility = person.Futility_Pruning;
   FutilityMargin3 = person.Futility_Margin_Depth3;
   FutilityMargin2 = person.Futility_Margin_Depth2;
   FutilityMargin1 = person.Futility_Margin_Depth1;

   // delta-pruning options

   UseDelta = person.Delta_Pruning;
   DeltaMargin = person.Delta_Margin;

   // quiescence-search options

   CheckNb = person.Quiescence_Check_Plies;
}

   CheckDepth = 1 - CheckNb;

}

// search_full_init()

void search_full_init(list_t * list, board_t * board) {

   //const char * string;
   int trans_move, trans_min_depth, trans_max_depth, trans_min_value, trans_max_value;

   ASSERT(list_is_ok(list));
   ASSERT(board_is_ok(board));
   
   // standard sort

   list_note(list);
   list_sort(list);

   // basic sort

   trans_move = MoveNone;
   /*if (UseTrans)*/ trans_retrieve(Trans,board->key,&trans_move,&trans_min_depth,&trans_max_depth,&trans_min_value,&trans_max_value);

   note_moves(list,board,0,trans_move);
   list_sort(list);
}

// search_full_root()

int search_full_root(list_t * list, board_t * board, int depth, int search_type) {

   int value;

   ASSERT(list_is_ok(list));
   ASSERT(board_is_ok(board));
   ASSERT(depth_is_ok(depth));
   ASSERT(search_type==SearchNormal||search_type==SearchShort);

   ASSERT(list==SearchRoot->list);
   ASSERT(!LIST_IS_EMPTY(list));
   ASSERT(board==SearchCurrent->board);
   ASSERT(board_is_legal(board));
   ASSERT(depth>=1);

   value = full_root(list,board,-ValueInf,+ValueInf,FRACT_DEPTH(depth),0,search_type);

   ASSERT(value_is_ok(value));
   ASSERT(LIST_VALUE(list,0)==value);

   return value;
}

// full_root()

static int full_root(list_t * list, board_t * board, int alpha, int beta, int depth, int height, int search_type) {

   int old_alpha;
   int value, best_value;
   int i, move;
   int new_depth;
   undo_t undo[1];
   mv_t new_pv[HeightMax];

   ASSERT(list_is_ok(list));
   ASSERT(board_is_ok(board));
   ASSERT(range_is_ok(alpha,beta));
   ASSERT(depth_is_ok(depth));
   ASSERT(height_is_ok(height));
   ASSERT(search_type==SearchNormal||search_type==SearchShort);

   ASSERT(list==SearchRoot->list);
   ASSERT(!LIST_IS_EMPTY(list));
   ASSERT(board==SearchCurrent->board);
   ASSERT(board_is_legal(board));
   ASSERT(depth>=1);

   // init

   SearchCurrent->node_nb++;
   SearchInfo->check_nb--;

   for (i = 0; i < LIST_SIZE(list); i++) list->value[i] = ValueNone;

   old_alpha = alpha;
   best_value = ValueNone;

   // move loop

   for (i = 0; i < LIST_SIZE(list); i++) {

      move = LIST_MOVE(list,i);

      SearchRoot->depth = REAL_DEPTH(depth);
      SearchRoot->move = move;
      SearchRoot->move_pos = i;
      SearchRoot->move_nb = LIST_SIZE(list);

      search_update_root();

      new_depth = full_new_depth(depth,move,board,board_is_check(board)&&LIST_SIZE(list)==1,true, false, false);

      move_do(board,move,undo);

      if (search_type == SearchShort || best_value == ValueNone) { // first move
         value = -full_search(board,-beta,-alpha,new_depth,height+1,new_pv,NodePV);
      } else { // other moves
         value = -full_search(board,-alpha-1,-alpha,new_depth,height+1,new_pv,NodeCut);
         if (value > alpha) { // && value < beta
            SearchRoot->change = true;
            SearchRoot->easy = false;
            SearchRoot->flag = false;
            search_update_root();
            value = -full_search(board,-beta,-alpha,new_depth,height+1,new_pv,NodePV);
         }
      }

      move_undo(board,move,undo);

      if (value <= alpha) { // upper bound
         list->value[i] = old_alpha;
      } else if (value >= beta) { // lower bound
         list->value[i] = beta;
      } else { // alpha < value < beta => exact value
         list->value[i] = value;
      }

      if (value > best_value && (best_value == ValueNone || value > alpha)) {

         SearchBest->move = move;
         SearchBest->value = value;
         if (value <= alpha) { // upper bound
            SearchBest->flags = SearchUpper;
         } else if (value >= beta) { // lower bound
            SearchBest->flags = SearchLower;
         } else { // alpha < value < beta => exact value
            SearchBest->flags = SearchExact;
         }
         SearchBest->depth = REAL_DEPTH(depth);
         pv_cat(SearchBest->pv,new_pv,move);

         search_update_best();
      }

      if (value > best_value) {
         best_value = value;
         if (value > alpha) {
            if (search_type == SearchNormal) alpha = value;
            if (value >= beta) break;
         }
      }
   }

   ASSERT(value_is_ok(best_value));

   list_sort(list);

   ASSERT(SearchBest->move==LIST_MOVE(list,0));
   ASSERT(SearchBest->value==best_value);

   if (/*UseTrans && */best_value > old_alpha && best_value < beta) {
      pv_fill(SearchBest->pv,board);
   }

   return best_value;
}

// full_search()
#if MP
static int full_search(board_t * board, int alpha, int beta, int depth, int height, mv_t pv[], int node_type, int exclusive) {
#else
static int full_search(board_t * board, int alpha, int beta, int depth, int height, mv_t pv[], int node_type) {
#endif

   bool in_check;
   bool single_reply;
   bool reduced;
   bool rebel_reduction;
   //bool razor_reduced;
   bool threat;
   bool bm_threat;

   int trans_move, trans_depth, trans_min_depth, trans_max_depth, trans_min_value, trans_max_value;
   int min_value, max_value;
   int old_alpha;
   int value, best_value;
   int move, best_move;
   int new_depth;
   int played_nb;
   int i;
   int opt_value;
   int mb;
   int FutilityMargin;
   int remaining_depth;
   int margin;
   attack_t attack[1];
   sort_t sort[1];
   undo_t undo[1];
   mv_t new_pv[HeightMax];
   mv_t played[256];

   ASSERT(board!=NULL);
   ASSERT(range_is_ok(alpha,beta));
   ASSERT(depth_is_ok(depth));
   ASSERT(height_is_ok(height));
   ASSERT(pv!=NULL);
   ASSERT(node_type==NodePV||node_type==NodeCut||node_type==NodeAll);

   ASSERT(board_is_legal(board));

   // horizon?

   if (depth < Fractional) return full_quiescence(board,alpha,beta,0,height,pv);

   // init

   SearchCurrent->node_nb++;
   SearchInfo->check_nb--;
   PV_CLEAR(pv);

   if (height > SearchCurrent->max_depth) SearchCurrent->max_depth = height;

   if (SearchInfo->check_nb <= 0) {
      SearchInfo->check_nb += SearchInfo->check_inc;
      search_check();
   }

   // draw?

   if (board_is_repetition(board) || recog_draw(board)) return ValueDraw;

   // mate-distance pruning

   if (UseDistancePruning) {

      // lower bound

      value = VALUE_MATE(height+2); // does not work if the current position is mate
      if (value > alpha && board_is_mate(board)) value = VALUE_MATE(height);

      if (value > alpha) {
         alpha = value;
         if (value >= beta) return value;
      }

      // upper bound

      value = -VALUE_MATE(height+1);

      if (value < beta) {
         beta = value;
         if (value <= alpha) return value;
      }
   }

   // transposition table

   trans_move = MoveNone;

   if (/*UseTrans && */depth >= TransDepth) {

      if (trans_retrieve(Trans,board->key,&trans_move,&trans_min_depth,&trans_max_depth,&trans_min_value,&trans_max_value)) {

         // trans_move is now updated

         if (node_type != NodePV) {

            if (UseMateValues) {

               if (trans_min_value > +ValueEvalInf && trans_min_depth < REAL_DEPTH(depth)) {
                  trans_min_depth = REAL_DEPTH(depth);
               }

               if (trans_max_value < -ValueEvalInf && trans_max_depth < REAL_DEPTH(depth)) {
                  trans_max_depth = REAL_DEPTH(depth);
               }
            }

            min_value = -ValueInf;

            if (DEPTH_MATCH(trans_min_depth,REAL_DEPTH(depth))) {
               min_value = value_from_trans(trans_min_value,height);
               if (min_value >= beta) return min_value;
            }

            max_value = +ValueInf;

            if (DEPTH_MATCH(trans_max_depth,REAL_DEPTH(depth))) {
               max_value = value_from_trans(trans_max_value,height);
               if (max_value <= alpha) return max_value;
            }

            if (min_value == max_value) return min_value; // exact match
         }
      }
   }

   // height limit

   if (height >= HeightMax-1) return eval(board, alpha, beta);

   // more init

   old_alpha = alpha;
   best_value = ValueNone;
   best_move = MoveNone;
   played_nb = 0;

   attack_set(attack,board);
   in_check = ATTACK_IN_CHECK(attack);
   threat = false;
   bm_threat = false;

   // null-move pruning

   if (UseNull && depth >= NullDepth && node_type != NodePV) {

      if (!in_check
       && !value_is_mate(beta)
       && do_null(board)
       && (!UseNullEval || depth <= NullReduction+Fractional || eval(board,alpha, beta) >= beta)) {

         // null-move search

         new_depth = depth - NullReduction - Fractional;

         move_do_null(board,undo);
         value = -full_search(board,-beta,-beta+1,new_depth,height+1,new_pv,NODE_OPP(node_type));
         move_undo_null(board,undo);

         // verification search

         if (UseVer && depth > VerReduction) {

            if (value >= beta && (!UseVerEndgame || do_ver(board))) {

               new_depth = depth - VerReduction;
               ASSERT(new_depth>0);

               value = full_no_null(board,alpha,beta,new_depth,height,new_pv,NodeCut,trans_move,&move);

               if (value >= beta) {
                  ASSERT(move==new_pv[0]);
                  played[played_nb++] = move;
                  best_move = move;
                  best_value = value;
                  pv_copy(pv,new_pv);
                  goto cut;
               }
            }
         }


	// mate threat
	if (value == VALUE_MATE(height+2)) { threat = true; mate_threats++; }
	
	// same threat
	if (value + 200 < alpha) {

		if (move == board->bm_move[board->turn]) {
			if (board->square[MOVE_TO(move)] != Empty) {	
				if (VALUE_PIECE(board->square[MOVE_TO(move)]) == board->bm_value[board->turn]) { bm_threat = true; bm_threats++; }

			} else {
				if (VALUE_PIECE(board->square[MOVE_TO(move)]) == 0) { bm_threat = true; bm_threats++; }
			}
		}

		board->bm_move[board->turn] = move;
		if (board->square[MOVE_TO(move)] != Empty) { 
			board->bm_value[board->turn] = VALUE_PIECE(board->square[MOVE_TO(move)]);
		} else {
			board->bm_value[board->turn] = 0;
		}
	} else {
		board->bm_move[board->turn] = 0;
		board->bm_value[board->turn] = 0;
	}

         // pruning

         if (value >= beta) {

            if (value > +ValueEvalInf) value = +ValueEvalInf; // do not return unproven mates
            ASSERT(!value_is_mate(value));

            // pv_cat(pv,new_pv,MoveNull);

            best_move = MoveNone;
            best_value = value;
            goto cut;
         }
      }
   } else {
	board->bm_move[board->turn] = 0;
	board->bm_value[board->turn] = 0;
   }

   // Internal Iterative Deepening

   if (UseIID && depth >= IIDDepth && node_type == NodePV && trans_move == MoveNone) {

      new_depth = depth - IIDReduction;
      ASSERT(new_depth>0);

      value = full_search(board,alpha,beta,new_depth,height,new_pv,node_type);
      if (value <= alpha) value = full_search(board,-ValueInf,beta,new_depth,height,new_pv,node_type);

      trans_move = new_pv[0];
   }

   remaining_depth = height - REAL_DEPTH(depth);

   // move generation

   sort_init(sort,board,attack,REAL_DEPTH(depth),height,trans_move);

   single_reply = false;
   if (in_check && LIST_SIZE(sort->list) == 1) single_reply = true; // HACK

   // move loop

   opt_value = +ValueInf;

   while ((move=sort_next(sort)) != MoveNone) {

      // extensions

	  //if (threat) new_depth++;
      new_depth = full_new_depth(depth,move,board,single_reply,node_type==NodePV, threat, bm_threat);

      // rebel reduction
      rebel_reduction = false;

      if (use_rebel_reduction && remaining_depth > 2 && node_type != NodePV && !in_check && board->square[MOVE_TO(move)] == Empty && !MOVE_IS_PROMOTE(move)) { 

	if (remaining_depth <= 4) margin = 500;
	else if (remaining_depth <= 6) margin = 700;
	else if (remaining_depth <= 8) margin = 975;
	else margin = 1500;

	opt_value = board->piece_material[White] - board->piece_material[Black];
	if (COLOUR_IS_BLACK(board->turn)){ 
		opt_value = -opt_value;
	}

	if (alpha > opt_value + margin) { 
		new_depth -= Fractional; 
		rebel_reductions++;
		rebel_reduction = true;
	}

      }

      // history pruning

      reduced = false;
	  //xreduced = false;
      if (UseHistory && depth >= HistoryDepth && node_type != NodePV) {
         if (!in_check && played_nb >= HistoryMoveNb && new_depth < depth) {
            ASSERT(best_value!=ValueNone);
            ASSERT(played_nb>0);
            ASSERT(sort->pos>0&&move==LIST_MOVE(sort->list,sort->pos-1));
            value = sort->value; // history score
            if (value < HistoryValue) {
               ASSERT(value>=0&&value<16384);
               ASSERT(move!=trans_move);
               ASSERT(!move_is_tactical(move,board));
               ASSERT(!move_is_check(move,board));
               new_depth -= Fractional;
               reduced = true;
	       history_cuts++;
			   //if (UseExtendedHistory && value < HistoryBound && new_depth > 1){
				 //  new_depth--;
				   //xreduced = true;
			   //}
            }
         }
      }


      // futility pruning
	//razor_reduced = false;
	  if (UseFutility && depth < 4*Fractional && node_type != NodePV && board->piece_size[COLOUR_OPP(board->turn)] > 3) {

         if (!in_check && new_depth < depth && !move_is_tactical(move,board) && !move_is_dangerous(move,board)) {

            ASSERT(!move_is_check(move,board));

			if (depth < 2*Fractional)
				FutilityMargin = FutilityMargin1;
			else if (depth < 3*Fractional)
				FutilityMargin = FutilityMargin2;
			else
				FutilityMargin = FutilityMargin3; 

            // optimistic evaluation
            // if (opt_value == +ValueInf) {
			    mb = board->piece_material[White] - board->piece_material[Black];
				if (COLOUR_IS_BLACK(board->turn)){ 
					mb = -mb /* - VALUE_PIECE(board->square[MOVE_TO(move)]) */;
				}
				/* else{
					mb = mb + VALUE_PIECE(board->square[MOVE_TO(move)]);
				}; */
               opt_value = mb + FutilityMargin;
               ASSERT(opt_value<+ValueInf);
            // }

            value = opt_value;

            // pruning

            if (value <= alpha) {

               if (value > best_value  && depth < 3 ) {
                  best_value = value;
                  PV_CLEAR(pv);
               } 
			   if(depth >= 3*Fractional && board->piece_size[COLOUR_OPP(board->turn)] > 3) {
			       new_depth -= Fractional;
				//razor_reduced = true;
			       razor_cuts++;
			   } else { 
      				   //move_undo(board,move,undo);
				   futility_cuts++;
				   continue;
			   }
            }
         }
      }  

/*      if (UseFutility && depth == 1 && node_type != NodePV) {

         if (!in_check && new_depth == 0 && !move_is_tactical(move,board) && !move_is_dangerous(move,board)) {

            ASSERT(!move_is_check(move,board));

            // optimistic evaluation

            if (opt_value == +ValueInf) {
               opt_value = eval(board) + FutilityMargin;
               ASSERT(opt_value<+ValueInf);
            }

            value = opt_value;

            // pruning

            if (value <= alpha) {

               if (value > best_value) {
                  best_value = value;
                  PV_CLEAR(pv);
               }

               continue;
            }
         }
      } */

      // recursive search

      move_do(board,move,undo);

      if (node_type != NodePV || best_value == ValueNone) { // first move
         value = -full_search(board,-beta,-alpha,new_depth,height+1,new_pv,NODE_OPP(node_type));
      } else { // other moves
         value = -full_search(board,-alpha-1,-alpha,new_depth,height+1,new_pv,NodeCut);
         if (value > alpha) { // && value < beta
            value = -full_search(board,-beta,-alpha,new_depth,height+1,new_pv,NodePV);
         }
      }

      // history-pruning re-search

      if (HistoryReSearch && reduced && value >= beta) {

         ASSERT(node_type!=NodePV);

         new_depth += Fractional;
	 history_research++;
         ASSERT(new_depth==depth-1);

		 //if (UseExtendedHistory && xreduced) new_depth++;

         value = -full_search(board,-beta,-alpha,new_depth,height+1,new_pv,NODE_OPP(node_type));
      }

      else if (rebel_reduction && value >= beta) {

         ASSERT(node_type!=NodePV);

         new_depth += Fractional;
	 rebel_researchs++;

         value = -full_search(board,-beta,-alpha,new_depth,height+1,new_pv,NODE_OPP(node_type));
      } /*else if (razor_reduced && value >= beta) {

         ASSERT(node_type!=NodePV);

         new_depth += Fractional;
	 razor_researchs++;

         value = -full_search(board,-beta,-alpha,new_depth,height+1,new_pv,NODE_OPP(node_type));
      }*/

      move_undo(board,move,undo);

      played[played_nb++] = move;

      if (value > best_value) {
         best_value = value;
         pv_cat(pv,new_pv,move);
         if (value > alpha) {
            alpha = value;
            best_move = move;
            if (value >= beta) goto cut;
         }
      }

      if (node_type == NodeCut) node_type = NodeAll;
   }

   // ALL node

   if (best_value == ValueNone) { // no legal move
      if (in_check) {
         ASSERT(board_is_mate(board));
         return VALUE_MATE(height);
      } else {
         ASSERT(board_is_stalemate(board));
         return ValueDraw;
      }
   }

cut:

   ASSERT(value_is_ok(best_value));

   // move ordering

   if (best_move != MoveNone) {

      good_move(best_move,board,REAL_DEPTH(depth),height);

      if (best_value >= beta && !move_is_tactical(best_move,board)) {

         ASSERT(played_nb>0&&played[played_nb-1]==best_move);

         for (i = 0; i < played_nb-1; i++) {
            move = played[i];
            ASSERT(move!=best_move);
            history_bad(move,board);
         }

         history_good(best_move,board);
      }
   }

   // transposition table

   if (/*UseTrans && */depth >= TransDepth) {

      trans_move = best_move;
      trans_depth = REAL_DEPTH(depth);
      trans_min_value = (best_value > old_alpha) ? value_to_trans(best_value,height) : -ValueInf;
      trans_max_value = (best_value < beta)      ? value_to_trans(best_value,height) : +ValueInf;

      trans_store(Trans,board->key,trans_move,trans_depth,trans_min_value,trans_max_value);
   }

   return best_value;
}

// full_no_null()

static int full_no_null(board_t * board, int alpha, int beta, int depth, int height, mv_t pv[], int node_type, int trans_move, int * best_move) {

   int value, best_value;
   int move;
   int new_depth;
   attack_t attack[1];
   sort_t sort[1];
   undo_t undo[1];
   mv_t new_pv[HeightMax];

   ASSERT(board!=NULL);
   ASSERT(range_is_ok(alpha,beta));
   ASSERT(depth_is_ok(depth));
   ASSERT(height_is_ok(height));
   ASSERT(pv!=NULL);
   ASSERT(node_type==NodePV||node_type==NodeCut||node_type==NodeAll);
   ASSERT(trans_move==MoveNone||move_is_ok(trans_move));
   ASSERT(best_move!=NULL);

   ASSERT(board_is_legal(board));
   ASSERT(!board_is_check(board));
   ASSERT(depth>=1);

   // init

   SearchCurrent->node_nb++;
   SearchInfo->check_nb--;
   PV_CLEAR(pv);

   if (height > SearchCurrent->max_depth) SearchCurrent->max_depth = height;

   if (SearchInfo->check_nb <= 0) {
      SearchInfo->check_nb += SearchInfo->check_inc;
      search_check();
   }

   attack_set(attack,board);
   ASSERT(!ATTACK_IN_CHECK(attack));

   *best_move = MoveNone;
   best_value = ValueNone;

   // move loop

   sort_init(sort,board,attack,REAL_DEPTH(depth),height,trans_move);

   while ((move=sort_next(sort)) != MoveNone) {

      new_depth = full_new_depth(depth,move,board,false,false, false, false);

      move_do(board,move,undo);
      value = -full_search(board,-beta,-alpha,new_depth,height+1,new_pv,NODE_OPP(node_type));
      move_undo(board,move,undo);

      if (value > best_value) {
         best_value = value;
         pv_cat(pv,new_pv,move);
         if (value > alpha) {
            alpha = value;
            *best_move = move;
            if (value >= beta) goto cut;
         }
      }
   }

   // ALL node

   if (best_value == ValueNone) { // no legal move => stalemate
      ASSERT(board_is_stalemate(board));
      best_value = ValueDraw;
   }

cut:

   ASSERT(value_is_ok(best_value));

   return best_value;
}

// full_quiescence()

static int full_quiescence(board_t * board, int alpha, int beta, int depth, int height, mv_t pv[]) {

   bool in_check;
   int old_alpha;
   int value, best_value;
   int best_move;
   int move;
   int opt_value;
   attack_t attack[1];
   sort_t sort[1];
   undo_t undo[1];
   mv_t new_pv[HeightMax];

   ASSERT(board!=NULL);
   ASSERT(range_is_ok(alpha,beta));
   ASSERT(depth_is_ok(depth));
   ASSERT(height_is_ok(height));
   ASSERT(pv!=NULL);

   ASSERT(board_is_legal(board));
   ASSERT(depth<=0);

   // init

   SearchCurrent->node_nb++;
   SearchInfo->check_nb--;
   PV_CLEAR(pv);

   if (height > SearchCurrent->max_depth) SearchCurrent->max_depth = height;

   if (SearchInfo->check_nb <= 0) {
      SearchInfo->check_nb += SearchInfo->check_inc;
      search_check();
   }

   // draw?

   if (board_is_repetition(board) || recog_draw(board)) return ValueDraw;

   // mate-distance pruning

   if (UseDistancePruning) {

      // lower bound

      value = VALUE_MATE(height+2); // does not work if the current position is mate
      if (value > alpha && board_is_mate(board)) value = VALUE_MATE(height);

      if (value > alpha) {
         alpha = value;
         if (value >= beta) return value;
      }

      // upper bound

      value = -VALUE_MATE(height+1);

      if (value < beta) {
         beta = value;
         if (value <= alpha) return value;
      }
   }

   // more init

   attack_set(attack,board);
   in_check = ATTACK_IN_CHECK(attack);

   if (in_check) {
      ASSERT(depth<0);
      depth += Fractional; // in-check extension
   }

   // height limit

   if (height >= HeightMax-1) return eval(board, alpha, beta);

   // more init

   old_alpha = alpha;
   best_value = ValueNone;
   best_move = MoveNone;

   /* if (UseDelta) */ opt_value = +ValueInf;

   if (!in_check) {

      // lone-king stalemate?

      if (simple_stalemate(board)) return ValueDraw;

      // stand pat

      value = eval(board, alpha, beta);

      ASSERT(value>best_value);
      best_value = value;
      if (value > alpha) {
         alpha = value;
         if (value >= beta) goto cut;
      }

      if (UseDelta) {
         opt_value = value + DeltaMargin;
         ASSERT(opt_value<+ValueInf);
      }
   }

   // move loop

   sort_init_qs(sort,board,attack,REAL_DEPTH(depth)>=CheckDepth);

   while ((move=sort_next_qs(sort)) != MoveNone) {

      // delta pruning

      if (UseDelta && beta == old_alpha+1) {

         if (!in_check && !move_is_check(move,board) && !capture_is_dangerous(move,board)) {

            ASSERT(move_is_tactical(move,board));

            // optimistic evaluation

            value = opt_value;

            int to = MOVE_TO(move);
            int capture = board->square[to];

            if (capture != Empty) {
               value += VALUE_PIECE(capture);
            } else if (MOVE_IS_EN_PASSANT(move)) {
               value += ValuePawn;
            }

            if (MOVE_IS_PROMOTE(move)) value += ValueQueen - ValuePawn;

            // pruning

            if (value <= alpha) {

               if (value > best_value) {
                  best_value = value;
                  PV_CLEAR(pv);
               }
	       delta_cuts++;
               continue;
            }
         }
      }

      move_do(board,move,undo);
      value = -full_quiescence(board,-beta,-alpha,depth-Fractional,height+1,new_pv);
      move_undo(board,move,undo);

      if (value > best_value) {
         best_value = value;
         pv_cat(pv,new_pv,move);
         if (value > alpha) {
            alpha = value;
            best_move = move;
            if (value >= beta) goto cut;
         }
      }
   }

   // ALL node

   if (best_value == ValueNone) { // no legal move
      ASSERT(board_is_mate(board));
      return VALUE_MATE(height);
   }

cut:

   ASSERT(value_is_ok(best_value));

   return best_value;
}

// full_new_depth()

static int full_new_depth(int depth, int move, board_t * board, bool single_reply, bool in_pv, bool threat, bool same_threat) { 

   int new_depth;
   ASSERT(depth_is_ok(depth));
   ASSERT(move_is_ok(move));
   ASSERT(board!=NULL);
   ASSERT(single_reply==true||single_reply==false);
   ASSERT(in_pv==true||in_pv==false);

   ASSERT(depth>0);

   new_depth = 0;


   if (in_pv && MOVE_TO(move) == board->cap_sq // recapture
              && see_move(move,board) > 0) {
	new_depth += recap_ext;
   }

   if (in_pv && PIECE_IS_PAWN(MOVE_PIECE(move,board))
              && PAWN_RANK(MOVE_TO(move),board->turn) == Rank7
              && see_move(move,board) >= 0) {
	new_depth += pawn_ext;
   } /*else if (in_pv && PIECE_IS_PAWN(MOVE_PIECE(move,board)) 
		&& is_passed(board, MOVE_TO(move)) 
		&& see_move(move,board) >= 0) {
	new_depth += ppush_ext;
   }*/

   if (in_pv && threat) {
	new_depth += threat_ext;
   }

   if (move_is_check(move,board)) {
	new_depth += check_ext;
   }

   if (single_reply && ExtendSingleReply) {
	new_depth += single_reply_ext;
   }

   if (exclude_bme_from_cap) {
   	if (new_depth > ext_cap) new_depth = ext_cap;

  	if (use_bme && same_threat) {
		new_depth += bm_ext;
   	}
   } else {
  	if (use_bme && same_threat) {
		new_depth += bm_ext;
   	}

   	if (new_depth > ext_cap) new_depth = ext_cap;

   }


   ASSERT(new_depth>=0&&new_depth<=depth);

   new_depth += depth - Fractional;
   return new_depth;
}


// do_null()

static bool do_null(const board_t * board) {

   ASSERT(board!=NULL);

   // use null move if the side-to-move has at least one piece

   return board->piece_size[board->turn] >= 2; // king + one piece
}

// do_ver()

static bool do_ver(const board_t * board) {

   ASSERT(board!=NULL);

   // use verification if the side-to-move has at most one piece

   return board->piece_size[board->turn] <= 3; // king + one piece was 2
}

// pv_fill()

static void pv_fill(const mv_t pv[], board_t * board) {

   int move;
   int trans_move, trans_depth, trans_min_value, trans_max_value;
   undo_t undo[1];

   ASSERT(pv!=NULL);
   ASSERT(board!=NULL);

   //ASSERT(UseTrans);

   move = *pv;

   if (move != MoveNone && move != MoveNull) {

      move_do(board,move,undo);
      pv_fill(pv+1,board);
      move_undo(board,move,undo);

      trans_move = move;
      trans_depth = -127; // HACK
      trans_min_value = -ValueInf;
      trans_max_value = +ValueInf;

      trans_store(Trans,board->key,trans_move,trans_depth,trans_min_value,trans_max_value);
   }
}

// move_is_dangerous()

static bool move_is_dangerous(int move, const board_t * board) {

   int piece;

   ASSERT(move_is_ok(move));
   ASSERT(board!=NULL);

   ASSERT(!move_is_tactical(move,board));

   piece = MOVE_PIECE(move,board);

   if (PIECE_IS_PAWN(piece)
    && PAWN_RANK(MOVE_TO(move),board->turn) >= Rank7) {
      return true;
   }

   return false;
}

// capture_is_dangerous()

static bool capture_is_dangerous(int move, const board_t * board) {

   int piece, capture;

   ASSERT(move_is_ok(move));
   ASSERT(board!=NULL);

   ASSERT(move_is_tactical(move,board));

   piece = MOVE_PIECE(move,board);

   if (PIECE_IS_PAWN(piece)
    && PAWN_RANK(MOVE_TO(move),board->turn) >= Rank7) {
      return true;
   }

   capture = move_capture(move,board);

   if (PIECE_IS_QUEEN(capture)) return true;

   if (PIECE_IS_PAWN(capture)
    && PAWN_RANK(MOVE_TO(move),board->turn) <= Rank2) {
      return true;
   }

   return false;
}

// simple_stalemate()

static bool simple_stalemate(const board_t * board) {

   int me, opp;
   int king;
   int opp_flag;
   int from, to;
   int capture;
   const inc_t * inc_ptr;
   int inc;

   ASSERT(board!=NULL);

   ASSERT(board_is_legal(board));
   ASSERT(!board_is_check(board));

   // lone king?

   me = board->turn;
   if (board->piece_size[me] != 1 || board->pawn_size[me] != 0) return false; // no

   // king in a corner?

   king = KING_POS(board,me);
   if (king != A1 && king != H1 && king != A8 && king != H8) return false; // no

   // init

   opp = COLOUR_OPP(me);
   opp_flag = COLOUR_FLAG(opp);

   // king can move?

   from = king;

   for (inc_ptr = KingInc; (inc=*inc_ptr) != IncNone; inc_ptr++) {
      to = from + inc;
      capture = board->square[to];
      if (capture == Empty || FLAG_IS(capture,opp_flag)) {
         if (!is_attacked(board,to,opp)) return false; // legal king move
      }
   }

   // no legal move

   ASSERT(board_is_stalemate((board_t*)board));

   return true;
}

// is_passed()
/*
static bool is_passed(const board_t * board, int to) {

   int t2;
   int me, opp;
   int file, rank;

   me = COLOUR_IS_WHITE(board->turn);
   opp = COLOUR_OPP(me);
   file = SQUARE_FILE(to);
   rank = PAWN_RANK(to,me);

   //t1 = board->pawn_file[me][file-1] | board->pawn_file[me][file+1];
   t2 = board->pawn_file[me][file] | BitRev[board->pawn_file[opp][file]];

   
   // passed pawns
   if ((t2 & BitGT[rank]) == 0) {

	if (((BitRev[board->pawn_file[opp][file-1]] | BitRev[board->pawn_file[opp][file+1]]) & BitGT[rank]) == 0) {
		return true;

   	}
   }

   return false;

}
*/

// end of search_full.cpp

