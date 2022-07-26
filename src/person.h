#ifndef PERSONALITY_DATA_DEFINED
#define PERSONALITY_DATA_DEFINED

typedef struct tag_pers {
int override_ucioptions;
int Extension_cap;
int Check_extension_level;
int Recap_extension_level;
int Pawn_to_7th_extension_level;
int Threat_extension_level;
int Single_reply_extension_level;
int Use_same_threat_extensions;
int Exclude_Same_threat_from_cap;
int Same_threat_extension_level;
int History_Pruning;
int History_Threshold;
int Rebel_Reduction;
int Futility_Pruning;
int Futility_Margin_Depth1;
int Futility_Margin_Depth2;
int Futility_Margin_Depth3;
int Delta_Pruning;
int Delta_Margin;
int Quiescence_Check_Plies;
int Piece_Activity;
int King_Safety;
int Passed_Pawns;
int Pawn_square_table_weight;
int Use_King_Tropism;
int knight_tropism;
int bishop_tropism;
int rook_tropism;
int queen_tropism;
int Fruit_Lazy_Eval;
int Fruit_Lazy_Eval_Margin;
int Fruit_Exchange_Bonus; 
int Fruit_King_Safety;
int Fruit_King_Safety_Margin;
int Opening_Pawn_Value;
int Opening_Knight_Value;
int Opening_Bishop_Value;
int Opening_Rook_Value;
int Opening_Queen_Value;
int Endgame_Pawn_Value;
int Endgame_Knight_Value;
int Endgame_Bishop_Value;
int Endgame_Rook_Value;
int Endgame_Queen_Value;
int Bishop_Pair_Opening;
int Bishop_Pair_Endgame;
int Queen_Knight_combo;
int Rook_Bishop_combo;
int Rook_on_bad_pawn_file_opening_bonus;
int Rook_on_bad_pawn_file_endgame_bonus;
int Pawn_Duo_Opening;
int Pawn_Duo_Endgame;
int Pawn_Doubled_Opening;
int Pawn_Doubled_Endgame;
int Pawn_Isolated_Opening;
int Pawn_Isolated_Opening_Open;
int Pawn_Isolated_Endgame;
int Pawn_Backward_Opening;
int Pawn_Backward_Opening_Open;
int Pawn_Backward_Endgame;
int Pawn_Candidate_Opening_Min;
int Pawn_Candidate_Opening_Max;
int Pawn_Candidate_Endgame_Min;
int Pawn_Candidate_Endgame_Max;
int Contempt_Factor;

} personality;

 /* Enum for the personality entry types */
enum pft {
person_override_ucioptions,
person_Extension_cap,
person_Check_extension_level,
person_Recap_extension_level,
person_Pawn_to_7th_extension_level,
person_Threat_extension_level,
person_Single_reply_extension_level,
person_Use_same_threat_extensions,
person_Exclude_Same_threat_from_cap,
person_Same_threat_extension_level,
person_History_Pruning,
person_History_Threshold,
person_Rebel_Reduction,
person_Futility_Pruning,
person_Futility_Margin_Depth1,
person_Futility_Margin_Depth2,
person_Futility_Margin_Depth3,
person_Delta_Pruning,
person_Delta_Margin,
person_Quiescence_Check_Plies,
person_Piece_Activity,
person_King_Safety,
person_Passed_Pawns,
person_Pawn_square_table_weight,
person_Use_King_Tropism,
person_knight_tropism,
person_bishop_tropism,
person_rook_tropism,
person_queen_tropism,
person_Fruit_Lazy_Eval,
person_Fruit_Lazy_Eval_Margin,
person_Fruit_Exchange_Bonus,
person_Fruit_King_Safety,
person_Fruit_King_Safety_Margin,
person_Opening_Pawn_Value,
person_Opening_Knight_Value,
person_Opening_Bishop_Value,
person_Opening_Rook_Value,
person_Opening_Queen_Value,
person_Endgame_Pawn_Value,
person_Endgame_Knight_Value,
person_Endgame_Bishop_Value,
person_Endgame_Rook_Value,
person_Endgame_Queen_Value,
person_Bishop_Pair_Opening,
person_Bishop_Pair_Endgame,
person_Queen_Knight_combo,
person_Rook_Bishop_combo,
person_Rook_on_bad_pawn_file_opening_bonus,
person_Rook_on_bad_pawn_file_endgame_bonus,
person_Pawn_Duo_Opening,
person_Pawn_Duo_Endgame,
person_Pawn_Doubled_Opening,
person_Pawn_Doubled_Endgame,
person_Pawn_Isolated_Opening,
person_Pawn_Isolated_Opening_Open,
person_Pawn_Isolated_Endgame,
person_Pawn_Backward_Opening,
person_Pawn_Backward_Opening_Open,
person_Pawn_Backward_Endgame,
person_Pawn_Candidate_Opening_Min,
person_Pawn_Candidate_Opening_Max,
person_Pawn_Candidate_Endgame_Min,
person_Pawn_Candidate_Endgame_Max,
person_Contempt_Factor,
person_oops_too_far,
};

/* Personality type data entry */
typedef struct tag_phunk {
  const char *p; /* String title of the parameter */
  enum pft e; /* Enum representing the parameter */
  int def; /* Default value */
  int minval; /* Minimum value allowed for the parameter */
  int maxval; /* Maximum value allowed for the parameter */
} phunk;


extern personality person;


extern void init_personality(void );
extern int write_person(FILE *pfile);
extern int read_person(FILE *pfile);
void PersonSetVal(phunk *, int);

#endif /* PERSONALITY_DATA_DEFINED */
