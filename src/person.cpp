#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "person.h"

personality person;

/* Basic data for the parameters */
/* Name of the parameter, enum for the parameter, default, minimum, maximum: */
const phunk phunk_list[] = {
  {"override_ucioptions" , person_override_ucioptions, 0, 0, 1},
  {"Extension_cap" , person_Extension_cap, 16, 16, 48},
  {"Check_extension_level" , person_Check_extension_level, 16, 0, 32},
  {"Recap_extension_level" , person_Recap_extension_level, 16, 0, 32},
  {"Pawn_to_7th_extension_level" , person_Pawn_to_7th_extension_level, 16, 0, 32},
  {"Threat_extension_level" , person_Threat_extension_level, 16, 0, 32},
  {"Single_reply_extension_level" , person_Single_reply_extension_level, 8, 0, 32},
  {"Use_same_threat_extensions" , person_Use_same_threat_extensions, 1, 0, 1},
  {"Exclude_Same_threat_from_cap" , person_Exclude_Same_threat_from_cap, 1, 0, 1},
  {"Same_threat_extension_level" , person_Same_threat_extension_level, 8, -16, 16},
  {"History_Pruning" , person_History_Pruning, 1, 0, 1},
  {"History_Threshold" , person_History_Threshold, 60, 0, 100},
  {"Rebel_Reduction" , person_Rebel_Reduction, 0, 0, 1},
  {"Futility_Pruning" , person_Futility_Pruning, 1, 0, 1},
  {"Futility_Margin_Depth1", person_Futility_Margin_Depth1, 300, 0, 3500},
  {"Futility_Margin_Depth2", person_Futility_Margin_Depth2, 500, 0, 3500},
  {"Futility_Margin_Depth3", person_Futility_Margin_Depth3, 975, 0, 3500},
  {"Delta_Pruning", person_Delta_Pruning, 0, 0, 1},
  {"Delta_Margin", person_Delta_Margin, 50, 0, 1000},
  {"Quiescence_Check_Plies", person_Quiescence_Check_Plies, 1, 0, 2},
  {"Piece_Activity", person_Piece_Activity, 100, 0, 400},
  {"King_Safety", person_King_Safety, 100, 0, 400},
  {"Passed_Pawns", person_Passed_Pawns, 100, 0, 400},
  {"Pawn_square_table_weight", person_Pawn_square_table_weight, 100, 0, 400},
  {"Use_King_Tropism", person_Use_King_Tropism, 1, 0, 1},
  {"knight_tropism", person_knight_tropism, 4, 0, 10},
  {"bishop_tropism", person_bishop_tropism, 2, 0, 10},
  {"rook_tropism", person_rook_tropism, 2, 0, 10},
  {"queen_tropism", person_queen_tropism, 4, 0, 10},
  {"Fruit_Lazy_Eval", person_Fruit_Lazy_Eval, 1, 0, 1},
  {"Fruit_Lazy_Eval_Margin", person_Fruit_Lazy_Eval_Margin, 200, 0, 900},
  {"Fruit_Exchange_Bonus", person_Fruit_Exchange_Bonus, 30, 0, 100},
  {"Fruit_King_Safety" , person_Fruit_King_Safety, 1, 0, 1},
  {"Fruit_King_Safety_Margin", person_Fruit_King_Safety_Margin, 1600, 500, 2500},
  {"Opening_Pawn_Value", person_Opening_Pawn_Value, 90, 0, 1000},
  {"Opening_Knight_Value", person_Opening_Knight_Value, 320, 0, 1000},
  {"Opening_Bishop_Value", person_Opening_Bishop_Value, 325, 0, 1000},
  {"Opening_Rook_Value", person_Opening_Rook_Value, 500, 0, 1000},
  {"Opening_Queen_Value", person_Opening_Queen_Value, 975, 0, 1000},
  {"Endgame_Pawn_Value", person_Endgame_Pawn_Value, 100, 0, 1000},
  {"Endgame_Knight_Value", person_Endgame_Knight_Value, 320, 0, 1000},
  {"Endgame_Bishop_Value", person_Endgame_Bishop_Value, 325, 0, 1000},
  {"Endgame_Rook_Value", person_Endgame_Rook_Value, 500, 0, 1000},
  {"Endgame_Queen_Value", person_Endgame_Queen_Value, 975, 0, 1000},
  {"Bishop_Pair_Opening", person_Bishop_Pair_Opening, 50, 0, 1000},
  {"Bishop_Pair_Endgame", person_Bishop_Pair_Endgame, 50, 0, 1000},
  {"Queen_Knight_combo", person_Queen_Knight_combo, 20, 0, 1000},
  {"Rook_Bishop_combo", person_Rook_Bishop_combo, 20, 0, 1000},
  {"Rook_on_bad_pawn_file_opening_bonus", person_Rook_on_bad_pawn_file_opening_bonus, 8, 0, 100},
  {"Rook_on_bad_pawn_file_endgame_bonus", person_Rook_on_bad_pawn_file_endgame_bonus, 8, 0, 100},
  {"Pawn_Duo_Opening", person_Pawn_Duo_Opening, 6, 0, 100},
  {"Pawn_Duo_Endgame", person_Pawn_Duo_Endgame, 6, 0, 100},
  {"Pawn_Doubled_Opening", person_Pawn_Doubled_Opening, 10, 0, 100},
  {"Pawn_Doubled_Endgame", person_Pawn_Doubled_Endgame, 20, 0, 100},
  {"Pawn_Isolated_Opening", person_Pawn_Isolated_Opening, 12, 0, 100},
  {"Pawn_Isolated_Opening_Open", person_Pawn_Isolated_Opening_Open, 16, 0, 100},
  {"Pawn_Isolated_Endgame", person_Pawn_Isolated_Endgame, 20, 0, 100},
  {"Pawn_Backward_Opening", person_Pawn_Backward_Opening, 8, 0, 100},
  {"Pawn_Backward_Opening_Open", person_Pawn_Backward_Opening_Open, 12, 0, 100},
  {"Pawn_Backward_Endgame", person_Pawn_Backward_Endgame, 12, 0, 100},
  {"Pawn_Candidate_Opening_Min", person_Pawn_Candidate_Opening_Min, 5, 0, 500},
  {"Pawn_Candidate_Opening_Max", person_Pawn_Candidate_Opening_Max, 55, 0, 500},
  {"Pawn_Candidate_Endgame_Min", person_Pawn_Candidate_Endgame_Min, 10, 0, 500},
  {"Pawn_Candidate_Endgame_Max", person_Pawn_Candidate_Endgame_Max, 110, 0, 500},
  {"Contempt_Factor", person_Contempt_Factor, 0, -1000, 1000},
  {NULL, person_oops_too_far, 0, 0, 0}
};

/* Setup default values for the personality if no input is specified */
void init_personality(void) {
  int start;
  phunk ph;

  for (start = 0;; start++) {
    ph = phunk_list[start];
    if (ph.p == NULL) break;
    if (ph.def > ph.maxval || ph.def < ph.minval) {
      fprintf(stdout,"Strange Default Value for %s  (%d)\n",ph.p,ph.def);
    }
    PersonSetVal(&ph, ph.def);
  }
}

/* Check that a personality trait is within range */
void check_range(int *val, phunk p) {
  if (*val > p.maxval) {
    printf("\n%s was outside the allowed range of %d to %d (given at %d) and was reset to %d\n",  p.p, p.minval, p.maxval, *val, p.maxval);
    *val = p.maxval;
  }
  if (*val < p.minval) {
    printf("\n%s was outside the allowed range of %d to %d (given at %d) and was reset to %d\n",  p.p, p.minval, p.maxval, *val, p.minval);
    *val = p.minval;
  }
}

/* Read in a personality file */
int read_person(FILE * pfile) {
  char string[4096], param[4096], *ch;
  int  start, pos;
  phunk ph;
  int found_something = 0;

  if (pfile == NULL) {
    puts("Error reading from personality file");
    return 0;
  }
  while (fgets(string, sizeof string, pfile)) {
    int found = 0;

    ch = string;
    while (*ch != ';') ch++;
    while (*(++ch) == ' ');
     // Backwards compatibility
    if (strstr(ch,"person.")) ch += 7;
    strcpy(param,ch);

    pos=0;
    while (param[pos]!=' ' && param[pos]!='\n') pos++;
    param[pos]=0;

    start = 0;
    for (start = 0;; start++) {
      ph = phunk_list[start];
      if (ph.p == NULL) break;
      found = (strcmp(param, ph.p) == 0);
      if (found) break;
    }
    if (!found) printf("\nERROR reading personality.  Did not understand file entry %s\n", string);
    else {
      int val = atoi(string);
      found_something = 1;
      check_range(&val, ph);
      PersonSetVal(&ph,val);
    }
  }
  return found_something;
}

/* Set the value for a personality file entry */
void PersonSetVal(phunk *ph, int val) {
  switch (ph->e) {
  case person_override_ucioptions: person.override_ucioptions = val; break;  
  case person_Extension_cap: person.Extension_cap = val; break;
  case person_Check_extension_level: person.Check_extension_level = val; break;
  case person_Recap_extension_level: person.Recap_extension_level = val; break;
  case person_Pawn_to_7th_extension_level: person.Pawn_to_7th_extension_level = val; break;
  case person_Threat_extension_level: person.Threat_extension_level = val; break;
  case person_Single_reply_extension_level: person.Single_reply_extension_level = val; break;
  case person_Use_same_threat_extensions: person.Use_same_threat_extensions = val; break;
  case person_Exclude_Same_threat_from_cap: person.Exclude_Same_threat_from_cap = val; break;
  case person_Same_threat_extension_level: person.Same_threat_extension_level = val; break;
  case person_History_Pruning: person.History_Pruning = val; break;
  case person_History_Threshold: person.History_Threshold = val; break;
  case person_Rebel_Reduction: person.Rebel_Reduction = val; break;
  case person_Futility_Pruning: person.Futility_Pruning = val; break;
  case person_Futility_Margin_Depth1: person.Futility_Margin_Depth1 = val; break;
  case person_Futility_Margin_Depth2: person.Futility_Margin_Depth2 = val; break;
  case person_Futility_Margin_Depth3: person.Futility_Margin_Depth3 = val; break;
  case person_Delta_Pruning: person.Delta_Pruning = val; break;
  case person_Delta_Margin: person.Delta_Margin = val; break;
  case person_Quiescence_Check_Plies: person.Quiescence_Check_Plies = val; break;
  case person_Piece_Activity: person.Piece_Activity = val; break;
  case person_King_Safety: person.King_Safety = val; break;
  case person_Passed_Pawns: person.Passed_Pawns = val; break;
  case person_Pawn_square_table_weight: person.Pawn_square_table_weight = val; break;
  case person_Use_King_Tropism: person.Use_King_Tropism = val; break;
  case person_knight_tropism: person.knight_tropism = val; break;
  case person_bishop_tropism: person.bishop_tropism = val; break;
  case person_rook_tropism: person.rook_tropism = val; break;
  case person_queen_tropism: person.queen_tropism = val; break;
  case person_Fruit_Lazy_Eval: person.Fruit_Lazy_Eval = val; break;
  case person_Fruit_Lazy_Eval_Margin: person.Fruit_Lazy_Eval_Margin = val; break;
  case person_Fruit_Exchange_Bonus: person.Fruit_Exchange_Bonus = val; break;
  case person_Fruit_King_Safety: person.Fruit_King_Safety = val; break;
  case person_Fruit_King_Safety_Margin: person.Fruit_King_Safety_Margin = val; break;
  case person_Opening_Pawn_Value: person.Opening_Pawn_Value = val; break;
  case person_Opening_Knight_Value: person.Opening_Knight_Value = val; break;
  case person_Opening_Bishop_Value: person.Opening_Bishop_Value = val; break;
  case person_Opening_Rook_Value: person.Opening_Rook_Value = val; break;
  case person_Opening_Queen_Value: person.Opening_Queen_Value = val; break;
  case person_Endgame_Pawn_Value: person.Endgame_Pawn_Value = val; break;
  case person_Endgame_Knight_Value: person.Endgame_Knight_Value = val; break;
  case person_Endgame_Bishop_Value: person.Endgame_Bishop_Value = val; break;
  case person_Endgame_Rook_Value: person.Endgame_Rook_Value = val; break;
  case person_Endgame_Queen_Value: person.Endgame_Queen_Value = val; break;
  case person_Bishop_Pair_Opening: person.Bishop_Pair_Opening = val; break;
  case person_Bishop_Pair_Endgame: person.Bishop_Pair_Endgame = val; break;
  case person_Queen_Knight_combo: person.Queen_Knight_combo = val; break;
  case person_Rook_Bishop_combo: person.Rook_Bishop_combo = val; break;
  case person_Rook_on_bad_pawn_file_opening_bonus: person.Rook_on_bad_pawn_file_opening_bonus = val; break;
  case person_Rook_on_bad_pawn_file_endgame_bonus: person.Rook_on_bad_pawn_file_endgame_bonus = val; break;
  case person_Pawn_Duo_Opening: person.Pawn_Duo_Opening = val; break;
  case person_Pawn_Duo_Endgame: person.Pawn_Duo_Endgame = val; break;
  case person_Pawn_Doubled_Opening: person.Pawn_Doubled_Opening = val; break;
  case person_Pawn_Doubled_Endgame: person.Pawn_Doubled_Endgame = val; break;
  case person_Pawn_Isolated_Opening: person.Pawn_Isolated_Opening = val; break;
  case person_Pawn_Isolated_Opening_Open: person.Pawn_Isolated_Opening_Open = val; break;
  case person_Pawn_Isolated_Endgame: person.Pawn_Isolated_Endgame = val; break;
  case person_Pawn_Backward_Opening: person.Pawn_Backward_Opening = val; break;
  case person_Pawn_Backward_Opening_Open: person.Pawn_Backward_Opening_Open = val; break;
  case person_Pawn_Backward_Endgame: person.Pawn_Backward_Endgame = val; break;
  case person_Pawn_Candidate_Opening_Min: person.Pawn_Candidate_Opening_Min = val; break;
  case person_Pawn_Candidate_Opening_Max: person.Pawn_Candidate_Opening_Max = val; break;
  case person_Pawn_Candidate_Endgame_Min: person.Pawn_Candidate_Endgame_Min = val; break;
  case person_Pawn_Candidate_Endgame_Max: person.Pawn_Candidate_Endgame_Max = val; break;
  case person_Contempt_Factor: person.Contempt_Factor = val; break;
  default: fprintf(stderr,"Could Not Set Parameter %s\n",ph->p); break;
  }
}

