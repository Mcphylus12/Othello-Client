# Othello-Client

An Othello Client written for my final year project. 
The src code is a code blocks project that includes a gui that supports single and multi-player.
The single player mode uses an min-max tree with alpha beta pruning and a weighted board heuristic to calculate
a score for a given board state. 

There also a exists a mode in which two computer opponents with different weights in their heuristic analysis functions
play each other in a winner stays on format. If a particular set of weightings gains more than a configured number of consecutive wins
the weghtings are written as a file.
