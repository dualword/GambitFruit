This release may be slightly slower than Beta 2 was because it is a release in-between other changes. The purpose is to fix some small bugs and add an option for people to be able to tune the personality options better.  To use the text file default.per open the file in a text editor and set override_options to 1.  For options that are true or false use 1 and 0.  One depth equals 16 for extentions and reductions.  

Added:
text file personality tuning option (set override_options to 1 to use)
Fractal ply extensions (with extension cap and tuning options)
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
Pawns on Bishop colour penalty (fewer pawns is better for bishops and with one bishop you should not block it with your own pawns)
Pawn duo bonus
Blocked center pawn penalty
Rebel reductions with researches
Show stats at end of pv to make tuning easier
Same threat extensions
Other small changes that likely affect nothing

Todo:
Bitbases (using Scorpio bitbase files)
Pawn Ram handling
Tune extensions and reductions
More draw recog
Tune king safety with tropism
Simple SMP using hash sharing (Fabian will do SMP in Fruit better with his future release)
Attack table updated in make_move
Clean up my bugs added to Fabians great code

Key:
MT = Mate threats
HC = History cuts (reductions really)
HR = History Researches
FC = Futility cuts
RC = Razor cuts (reductions)
DC = Delta cuts
RR = Rebel reductions (an idea from Ed's web site)
RRR = Rebel reduction researches 
BM = Same threats (same move threat on same piece)

Thank you to Fabian for Fruit 2.1, Thomas for Toga, and to Bryan Hofmann for the window compile.  
