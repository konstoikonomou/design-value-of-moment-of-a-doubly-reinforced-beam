# Design-value-of-moment-of-a-doubly-reinforced-beam
Program that calculates the design value of moment of a doubly reinforced rectangular beam, according to Eurocode 2. 

It takes as input the geometry of the rectangular beam and the quantity and size of steel bars 
(in tension and compression).

At first, it checks if the failure is occuring with the desired form (first the steel bars in tension reach the
yield point and then concrete fails in compression) and calculates the design value of moment, 
as well as the final depth of neutral axis in mm.

Input: 
beam geometry characteristics in mm, 
quantity of steel bars (in tension and in compression), 
diameter of steel bars (in tension and in compression)

Output:
design value of moment in kNm, 
final depth of neutral axis in mm
