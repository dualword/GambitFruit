Added:
Mate threat extension
Limited Razoring
Rook pawn score adjustment
Rook bishop bonus
Queen Knight bonus
Two Rook penalty
Two Knight penalty
Minor tuning to piece values
King tropism 
Side to move bonus
Rook on bad pawn file bonus
Removed some init for loops (very small speed up)
changed futility pruning
Pawns on Bishop colour penalty (fewer pawns is better for bishops and with one bishop you should not block it with your own pawns)
Blocked pawn penalty
Other small changes that likely affect nothing

Todo:
Pawn Ram handleing
Fractal ply extensions
Tune extensions and reductions
Improve pawn structure
More draw recog
Tune king safety with tropism
Simple SMP using hash sharing (Fabian will do SMP in Fruit better with his future release)
Attack table updated in make_move
Same threat extension (did not help in my fist test but will try again after fractal ply)
Clean up my bugs added to Fabians great code

*note I have found 2 bugs that do not affect play strength but make this program and any other program based on Fruit easily detectable as clones of Fruit.  I will not fix them because there is no good reason to cover up the origin of any program based on Fruit or any other program.
