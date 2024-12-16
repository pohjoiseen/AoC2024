# Advent of Code 2024, solutions by Alexander Ulyanov

This is the first time I've been doing [Advent of Code](https://adventofcode.com/2024),
in fact it's been many years if not decades since I did such problems last time
(excluding maybe some job interviews that I don't really remember anymore).
I participated in some programming olympiads in high school, even won the regional level
once, but I never actually was all that great at mathematics and algorithms, which is what
these kinds of problems require.  And while I've been employed as a programmer for 15 years,
we all know that most programming jobs rarely if ever require fancy maths and algorithms
either.  Still I decided to try AoC this one time and ended up enjoying it quite a bit.
The forgiving nature of AoC makes it especially easy to start, and then you get a bit hooked.

I chose C++ as my language, using only its standard library.  I never used it for any
professional work, only for a few very occasional experiments.  Modern C++ seems quite
well-suited for this kind of problems, with plenty basic data structures and other 
features included (when speaking about vectors, sets, deques etc. below I always refer to
the actual C++ templates), and of course fast enough it lets you get away with a lot
bruteforcey things (and you can parallelize some problems for extra oomph).  Still I might
be doing suboptimal things here and there, and I had to look at cppreference.com all the time.
I didn't do unnecessary input error checking and my coding style evolved somewhat over these
days, for example, in first problems I didn't bother splitting into functions at all,
not even moving out input parsing.  And of course style mostly still remained quick and dirty
until the end, which is only natural for this kind of task.

The machine used was AMD Ryzen 5 3600 (6-core), 24 GB RAM.  The platform was Arch Linux x64
and the compiler GCC 14.2.1.  There's no makefile; unless specified otherwise in source
file comments, all files can be compiled by hand with a trivial command line like
`g++ -O2 -o 01-1 01-1.cc`.  Execution times below always refer to the second subproblem
(always the more difficult one), compiled with `-O2`; many tasks are much slower without
this flag.

So far (as of day 15) I've been able to solve all problems by myself, although mostly
not in a perfectly optimal way.  I had to look up some things, but quite rarely
(apart from basic C++ library stuff), and generally I knew what to look for.
For a few problems I have parallelized or better optimized versions but generally
didn't bother.  I looked at [/r/adventofcode](https://old.reddit.com/r/adventofcode/)
Reddit after solving every task, noting which optimizations people tried.

In this file I restate all the problems in a more straightforward, non-whimsical way,
and briefly explain my solutions.  I also note how difficult I found every problem,
and how fast the solution runs on my machine.


## Day 1 "Historial Hysteria"

_Very simple._

**Problem 1**: the input is a list of pairs of integers, so, in practice, two sequences
of integers.  The smallest number in first sequence must be paired with the smallest in
the second sequence, then second smallest with second smallest and so on.  Then calculate
the "distance" (difference) between all resulting pairs and add up.

**Solution 1**: trivial, read numbers in two vectors, sort both, and then iterate through
both and sum the difference.

**Problem 2**: for the same input, determine how many times each number from the first
sequence occurs in sequence.  Multiply each number from first sequence by its occurrences
in second sequence and add up.

**Solution 2**: now keep a vector for the first sequence, and a map of number -> occurrences
for the second sequence.  Then iterate through the vector and look up occurrences of the
same number from the map.

**Execution time**: trivial.


## Day 2 "Red-Nosed Reports"

_Very simple._

**Problem 1**: the input consists of sequences of integers ("levels").  The sequence ("report")
is considered "safe" when all numbers in it keep either increasing or decreasing, and the
difference between all adjacent numbers is between 1 and 3.  Find how many of the reports
are safe.

**Solution 1**: trivial, for each sequence determine direction from the first two numbers,
and then iterate through, checking that the direction is the same for each adjacent
pair and the difference for each pair is between 1 and 3.  Repeat, sum up.

**Problem 2**: an formerly "unsafe" report is now safe, if it's possible to make it
fit the rules by removing exactly one level from it.

**Solution 2**: bruteforcey, if the sequence is not safe initially, loop through trying
to replace each number with a -1 (which we then ignore) and checking if it's safe now.
The input is small enough (1000 sequences of no more than 8 numbers) that it doesn't make
sense to even try to think of something else.

**Execution time**: trivial.


## Day 3 "Mull It Over"

_Simple._

**Problem 1**: the input is a long garbled text string, which contains here and their
some "multiplication instructions", of the form `mul(X,Y)`, where X and Y are small integers.
Find all well-formed instructions, calculate their results and add up.

**Solution 1**: straightforward, iterate through string looking for `mul(` characters,
and then keep trying to read in the rest of the "instruction" (number, comma, number,
closing parenthese), and multiple numbers and add to result if succeeded.

**Problem 2**: also look for `do()` and `don't()` instructions.  `don't()` turns off
processing of subsequent `mul(X,Y)` and `do()` turns it back on.  Initially the processing
is on.

**Solution 2**: not really any more difficult, look for the appropriate sequences
along with `mul(X,Y)` and keep the appropriate flag.

**Execution time**: trivial.


## Day 4 "Ceres Search"

_Simple._

**Problem 1**: the input is a rectangular text block, consisting of `X`, `M`, `A` and `S`
letters.  Count the number of occurrences of `XMAS` word, which can be arranged
in any way (forwards or backwards, horizontally, vertically or diagonally).

**Solution 1**: straightforward again, read input into a vector of strings, loop through
looking for an `X`, and, if found, manually check all possible 8 directions of `XMAS` word
which may start from it (some directions may be excluded if `X` is too close to the edge
of the block).

**Problem 2**: now look for two `MAS` words, which are arranged in an X-pattern, crossing
each other at `A` letter.  They can again be arranged in any direction.

**Solution 2**: even simpler, now look for `A` and manually check for `M` and `S` next to it
in allowed arrangements.  No need for bounds checking if we just examine only `A`s at least
one letter away from each bound.

**Execution time**: trivial.


## Day 5 "Print Queue"

_Medium._

**Problem 1**: the input is a list of pairs of integers ("page ordering rules")
and then some sequences of integers ("page updates").  "Page ordering rules" are used
to validate "page updates"; the first "page" in a pair must always occur before the
second one, if they both occur in the same update.  Find out which updates satisfy
all rules, and sum up the values of middle pages of every valid update (all updates
have non-even length, so there is always a middle element in each).

**Solution 1**: input parsing is a bit more involved than in most problems, but we end
up with a vector of pairs for rules, and then process updates line by line.
For each update, loop through its numbers, for each number, loop through rules,
and if the number is equal to either of numbers in a rule, use `std::find()`
look for violations of that rule in the rest of the update.

**Problem 2**: now take only the incorrectly-ordered updates, and reorder them so that
they start satisfying the rules.  Get the sum of middle elements of these updates after
reordering.

**Solution 2**: the problem is rather forgiving, as the input is carefully crafted to be
sortable in an unique way, which is of course generally not at all guaranteed.
I wrote a comparator function which goes through all the ordering rules,
then used it first to check if the sequence is already ordered, and then to pass it to
`std::sort()` in case it isn't.  This works nicely and in fact ends up shorter than
solution 1, but, again, it's just that the input actually allows that.

**Execution time**: a few milliseconds.


## Day 6 "Guard Gallivant"

_Medium.  Basic optimizations done but more would have been possible._

**Problem 1**: the input is a map of a big room, mostly empty floor (`.`) but with
some randomly placed obstacles (`#`).  There is also one guard (`^`).  The guard walks
in a straight line, strictly horizontally/vertically, and upon hitting an obstacle
always turns right.  Eventually he is going to leave the map.  The problem is to count
unique floor tiles that get visited by the guard (count only once even if tile is
visited multiple times).

**Solution 1**: just do the actual walk, replacing all unvisited tiles with `X`, and
count the number of `X`s at the end.

**Problem 2**: try placing one more obstacle to the map to an empty spot so that
the guard, instead of eventually walking off the map, would start looping infinitely.
Calculate the total possible number of positions where one more obstacle can be placed
in this way.

**Solution 2**: the most basic optimizations here are to not examine the spots where
the guard never steps to begin with, and to detect loops in some not too insane way.
For that, first do the walk for the original map, and then loop over all `X`s we got,
copying the map and placing an obstacle to each of them.  On each walk, keep a vector
of (x, y, direction) tuples for all the turns we made, and when making a new turn look
through this vector and see if this position alread occurred; if so, we have a loop
(the rules are such that one repetition would be enough to detect a loop).

**Execution time**: about 50 ms.

**Optimized version** (_06-2-par.cc_): the basic version is already fairly fast; still,
more properly optimized versions are possible according to Reddit.  But I just tried
to make it even faster by going multithreaded; a parallel for-loop is easy enough these days
with C++ standard library with `std::execution::par` (if you add like five obscure
compiler arguments).  This reduced execution time to 15-17 ms.


## Day 7 "Bridge Repair"

_Medium.  Bruteforced to execution time of a bit over a second,
then used a hint from Reddit for a proper solution._

**Problem 1**: the input is a set of lines, each line containing a set of at least three
numbers.  Those are actually equations, and the first number ("test value") in each line
must equal an expression composed of the rest of the numbers.  The expressions are
evaluated strictly left to right, and may contain addition and multiplication operators.
It might be possible to insert some combination of operators into expression so that
the equation gets true, or it might not.  The result must be a sum of all test values
for equations that can be made true.

**Solution 1**: bruteforcey.  There are only two possible operators, so 2^(N-1) combinations.
Since the expressions do not have more than 12-13 numbers, it is easily possible
to just try out all combinations (and for that just use some bit arithmetic, loop
0..2^(N-1) and look at indivial bits, 0 = addition 1 = multiplication).

**Problem 2**: same but add one more possible operator, concatenation (so that 12 and 34
would result in 1234).

**Solution 2**: for initial solution, just kept on bruteforcing, and kept using
bit arithmetic to make possible combinations, using now two bits per operator (since
there are three, not four possible operators, just ignore the fourth combination).
Implemented concatenation using logarithm and powers, although probably converting
to and from strings could also work just as well.

**Execution time**: over 8 seconds, not that great.

**Optimized version** (_07-2-par.cc_): tried again a multithreaded version,
parallelizing the main loop over input with `std::execution::par`.  Execution time
reduced to 1.15-1.3 seconds.

**Properly optimized version** (_07-2-rec.cc_): this is the proper version,
for which I had to look at Reddit for a hint.  It is recursive, going from right to left,
trying to reverse all three possible operations but only if it actually make sense
(reversing addition must not go below zero, reversing multiplication must not give
fractional result, reversing concatenation is possible if our current result already ends in the correct digits).  This lets us go down to a few milliseconds, with no need for
multithreading.


## Day 8 "Resonant Collinarity"

_Simple._

**Problem 1**: the input is a rectangular block of characters representing a map.
It is mostly empty space (represented by dots) but also contains some "antennas"
(alphanumeric characters).  There are at least two antennas of each kind.  For each
pair of antennas of the same kind, find two "antinodes", which are locations on the
line that joins these antennas, at the same distance from each of antennas as the
distance between antennas themselves, like that (`a` = antenna, `#` = antinode):

```
..........
...#......
..........
....a.....
..........
.....a....
..........
......#...
..........
..........
```

There might be multiple antinodes at the same location, for multiple pairs of antennas.
Find the total unique number of antinode coordinates that fit into the original map.

**Solution 1**: parse input into a map of antenna character -> vector of antenna
coordinates.  For every character, go through all possible pairs (this is simple:
`for (int i = 0; i < length; i++)` `for (int j = i + 1; j < length; j++)`).
Get the difference between their coordinates, add it to one of the antennas and subtract
from another, and, if we are still within map bounds, add to a set of antinode coordinates.
The result is then the length of that set.

**Problem 2**: for every pair of antennas there can be not two but
any number of antinodes, just repeating away from antennas on the same line with the same
interval.

**Solution 2**: same, but for both antennas just keep adding or subtracting same
offsets to coords until we get outside of the map.

**Execution time**: trivial.


## Day 9 "Disk Fragmenter"

_Simple._

**Problem 1**: there is a long line representing a "file allocation table of a disk drive".
All "files" are arranged sequentially, with possible empty gaps between them.
All files and gaps are no more than 9 "blocks" long.  Therefore the line consists
of digits, one represents first file length, another empty space length, third
second file length and so on.  Each file implicitly also has an ID, which is just its
0-indexed sequential number on the disk in this original allocation.

The disk needs to be "defragmented" by moving all files to the beginning and all empty
space to the end.  Taking the last occupied "block" of the disk, move it to the
first available empty block on the disk.  Keep doing this until all the occupied
space is contiguous in the beginning of the disk and all empty space contiguous in the
end of the disk.  The order of this "defragmentation" must be exactly as specified,
last block to first empty space.

After finishing calculate the "checksum", by multiplying the index of every occupied
block of the disk by the ID of file to which this block now belongs, and summing up.

**Solution 1**: more or less straightforward, reconstruct the disk in memory as a vector
of ints, which contain file ID in this block or -1 for empty space.  Then loop two
iterators, from the end for files and from the beginning for empty space; move block
if empty space found and free space iterator still points before the files iterator.
Keep doing until files iterator reaches the beginning of the vector (bit of an overhead
but had a bit of difficulty figuring out where to stop).

**Problem 2**: instead of individual blocks, now move only entire files.  Starting from the
end again, every file must be moved into the beginning of the first possible free space
chunk big enough to fit it.  Only attempt this once, if cannot move file on the first
try, do not come back to it.

**Solution 2**: represent the "disk" now in a different way, just keep two
vectors of files and empty space extents (chunks), with the first block number, length
and, for files, their ID.  Then go through files vector from the end, and for every file
go through empty space vector from the beginning until reached the block number of the file,
and if a big enough empty space chunk found, move file (change block number) and adjust empty 
space chunk length and block number to match the remaining space.  The empty space chunk
may end up with a zero length after that, that is completely fine.  Checksum function
needs to be adjusted accordingly.

**Execution time**: 12-15 ms.  Probably could be smaller and probably could be further
optimized.


## Day 10 "Hoof it"

_Simple._

**Problem 1**: there is a rectangular block of characters between 0 and 9, which
represent heights on a topographic map.  0's are possible "trailheads" and 9's
"hiking destinations"/hilltops.  For every 0 count the number of different possible
destinations reachable from it, so that there is an increment of one
on each step, and steps can be horizontal and vertical but not diagonal.  Sum
up for all 0's.

**Solution 1**: iterate through the data and find all 9's, for every one run
a flood-fill.  For a flood-fill, use a deque, examining and discarding points
in the head, and enqueuing new points to the tail.  Enqueue points that are
adjacent and less by one than the point we're examining.  Whenever we reach 0,
put its coordinates into a set and discard the point.  Using a set ensures
a trailhead is only counted once, even if there are multiple possible valid routes
to it.  Start with enqueuing the original 9, and repeat until deque empty.
For all found 0's, store them in a map structure, coords -> counter of trails
(to different destinations) starting there.  Sum up.

**Problem 2**: instead of possible destinations reachable from each "trailhead",
count the number of different possible "trails" (which still must end at a 9
according to the original rules).

**Solution 2**: as many on Reddit, when solving problem 1 I accidentally originally
solved problem 2, before realizing that in problem 1 destinations reached must be unique.
So solution 1 is trivially converted into solution 2 by just replacing a set
with a vector.

**Execution time**: trivial.


## Day 11 "Plutonian Pebbles"

_Difficult, the first one that requires serious thinking._

**Problem 1**: there is a sequence of a few positive integers ("pebbles with engraved
numbers").  Iterate through it ("blink").  On each iteration, one of the three things
happens with every number:

* 0 turns to 1
* Numbers consisting of an even number of digits split in two (like, 1234 to 12 and 34)
* All other numbers get multiplied by 2024

The result is the total length of sequence after 25 iterations/blinks.

**Solution 1**: straightforward, just keep the sequence in a list, iterate
through it according to the rules and find its length after 25 iterations.

**Problem 2**: exactly the same but 75 iterations instead.

**Solution 2**: now this gets interesting, the naive solution is exponential,
and the actual sequence for test data would be hundreds of trillions long.
But we don't really need the sequence, we need only its length.  The optimization
I came up with is precalculating a table for numbers between 0 and 9, all the way up to
75 steps.  N-th row of the table contains the length of sequence the number will
develop into after N iterations.  All numbers tend to eventually split down to
individual digits, and sequences for these digits become recursive (splitting to
individual digits again) soon enough, so starting with a few rows of seed data we
can easily calculate such table row by row.  8 is a bit annoying as it's less
cleanly recursive than others but still works.

Then, iterating over the actual sequence, every time we see a number between 0 and 9,
we just replace it with the precalculated sub-sequence length for the correct
remaining number of iterations.  (Use negative sign to distinguish these values
from "live" ones, which need to be iterated on.)

This works nicely enough but probably is still more complicated than it needs to be.
From Reddit, a simple solution is to just keep only unique numbers in sequence, along
with the number of times they repeat.  Didn't try to experiment.

Having the naive solution 1 on hand really helps here, to verify the optimized
version for small inputs.

(False starts: first tried to just rewrite with recursing, memory was no longer blowing
up but probably didn't help the execution time.)

**Execution time**: 0.3 seconds.  Probably would be less with the simpler algorithm.


## Day 12 "Garden Groups"

_Simple._

**Problem 1**: there is a rectangular text block filled with regions of same characters
(which represent certain kinds of flowers growing on a field).  For each such region,
get its "price", which is its area multiplied by total perimeter, assuming every individual
character is a square with a side of one.  Regions can be of any size and shape,
possibly including holes (filled with one or more other regions). There might be
multiple discontinuous regions of same character.  Sum up all prices.

**Solution 1**: keep field as a vector of strings, keep track of visited characters
by having another vector of strings.  Find regions by simply looping through and doing
a flood-fill on with a deque (as in the day 10 problem).  During the flood-fill, count all
filled characters, and increase perimeter counter every time when hitting a different
character or the bounds of the overall block.  Sum up everything.

**Problem 2**: instead of perimeter length, calculate the number of continuous
sides of every region.  For example a square-shaped region would always have four sides
regardless of its size.  For the purpose of this problem "8-shaped" regions should count as
two regions, not one, so for something like:

```
AAAAAA
ABBAAA
ABBAAA
AAABBA
AAABBA
AAAAAA
```

There are two B regions with 4+4 sides, not one with 6 sides.

**Solution 2**: begin as with solution one, but for every region, instead of keeping
a perimeter counter save all "edge" coordinates; horizontal and vertical edges to separate vectors (for horizontal edge the coordinates would be of the character above this edge,
and for vertical edge the coordinates would be of the character to the left of this edge.
-1 is a valid coordinate value in this case).  Apart from coordinates save a "direction"
for every edge (`true` if the edge is "pointing" right/down, `false` if up/left).
For vertical edges, record X and Y coordinates flipped.

After going through a region, sort the horizontal edge vector, up to down and left to right.
Then just go through it and count discontinuities (when X coordinate between adjacent
edges differs by more than one, or Y coordinate differs, or direction bit differs).
For vertical edges do the same; since we saved them with coordinates flipped, same function
works.  Sum, multiply by area, and sum up for all regions.

(False starts: got tripped up by the rule about 8-shaped regions, for some reason
assumed just keeping edge coordinates would be enough, but had to add a direction bit then.
Thankfully the rules explicitly warn about this issue and provide a test case.)

**Execution time**: a few milliseconds.


## Day 13 "Claw Contraption"

_Very simple but needed to look up some super basic maths._

**Problem 1**: the input is grouped into tuples of six integers (embedded into text
snippets of fixed shape).  Two of them represent some "prize" coordinates on a plane, other
four represent how much a "claw", starting from zero, moves right/up when "pressing" one
of the two "buttons".  The goal is to determine the smallest possible number of
button presses to reach prizes.  For some tuples this is not necessarily possible at all.  If
it's possible, then a press of one of the buttons costs one "token" and of another one
three "tokens"; the result is the sum of "tokens" spent.

**Solution 1**: this is in practice no more than a very basic system of two linear equations
with two variables.  Such a system can have zero, infinitely many or exactly one solution.
These are solvable with some simple formulas from school (which I however
needed to Google); if the denominator in those formulas is zero, the system is not solvable
and can be discarded.  The numbers of "button presses" need to be integer, so inputs with 
fractional solutions can also be discarded.  For reading input here `scanf()` is the
easiest option.

**Problem 2**: all coordinates of "prizes" need to be increased by 10000000000000, everything
else same.  The problem does not mention expected result for test input.

**Solution 2**: nothing special needs to be done, just use doubles and long long for the 
result, the precision is enough.  Probably the "smallest possible" part is intentionally
misleading, so people can try with some pathfinding algorithms and then trip up when getting
such huge coordinates, while for equations this doesn't matter at all.

**Execution time**: trivial.


## Day 14 "Restroom Redoubt"

_Medium, problem vaguely formulated and some manual work is in practice necessary._

**Problem 1**: the input is grouped into tuples of four integers (embedded into some text lines
of fixed shape), which represent initial positions and velocities of "robots".  "Robots"
move on a rectangular field; its dimensions are non-even and known beforehand (11x7 for
test input, 101x103 for real input).  On each iteration robots move on this field according
to their velocities.  More than one robot may end up in the same position, and upon
leaving the field robots "teleport" (wrap around) to the opposite side.  The goal is
to iterate the field 100 times, then find numbers of robots in each of its quadrants
and multiply the four numbers. Since the field has non-even dimensions, the central row
and column in it are not considered to belong to any quadrant and robots located on those
must be ignored.

**Solution 1**: straightforward, read input data into a vector of tuples, get
field dimensions from command line args, and iterate through the vector, updating the
positions for every robot.  The output step, with the quadrants and all, is actually
(slightly) more complicated than the actual solution.

**Problem 2**: it is known that after a certain number of iterations robots will arrange
themselves into a shape of a Christmas tree.  Find that number of iterations.

**Solution 2**: the problem does not actually specify what exactly this Christmas tree
shape should look like, which I guess is the whole point; I looked for any links in the
text of the problem, and tried to google for problems from previous years (the problem is 
phrased as "robots seem awfully similar to ones built and used at the North Pole", which
sounds like it might refer to some older one), but no luck :).  I started writing some code
to look for a triangle shape, which probably would have actually solved the problem
(finding the top part of the Christmas tree) but couldn't get it to work quickly.
While debugging I just made the program visualize all iterations, and noticed that some
kind of a vague pattern emerges quite frequently.  I then looked for any
kind of a somewhat long repeated horizontal sequence of robots ("11111"), piping the output
to `less` and using its search function.  To my surprize, it actually found quite soon
(after a few thousand iterations) this nice picture:

```
1111111111111111111111111111111
1                             1
1                             1
1                             1
1                             1
1              1              1
1             111             1
1            11111            1
1           1111111           1
1          111111111          1
1            11111            1
1           1111111           1
1          111111111          1
1         11111111111         1
1        1111111111111        1
1          111111111          1
1         11111111111         1
1        1111111111111        1
1       111111111111111       1
1      11111111111111111      1
1        1111111111111        1
1       111111111111111       1
1      11111111111111111      1
1     1111111111111111111     1
1    111111111111111111111    1
1             111             1
1             111             1
1             111             1
1                             1
1                             1
1                             1
1                             1
1111111111111111111111111111111
```

I ended up adjusting code so that it looks for ten "robots" in a horizontal row
(by sorting robots by height and width after every iteration and looking for
a sequence of robots with only X coordinate differing by one), which is enough to find
this particular pattern with this particular input.  Hardly a clean solution, but I guess
the point is that you don't know which shape exactly you are even looking for anyway,
so this is in practice not feasible to fully automate.

**Execution time**: 150 ms, not particularly efficient but again the nature of the task is
such that I don't care too much.


## Day 15 "Warehouse Woes"

_Medium, not difficult algorithmically but a fair bit of code._

**Problem 1**: the input is a rectangular block of text, representing a map of a large room,
followed by empty line and one or more lines of move instructions for a "robot".  The map
may contain characters `.` (empty floor), `#` (wall), `O` (box) and exactly one `@` (robot).
Move instructions are lines of `<`, `^`, `>`, `v` characters, representing movement
directions; this part is split into lines only for convenience, and should be treated as
one long sequence of characters with newlines ignored.

The goal is to execute all given move instructions for the robot.  The robot may push
one or more "boxes" in front of itself, if necessary and if there's space.
If the instruction would require the robot to walk into a wall or push one or more
boxes into a wall, nothing happens and this instruction is simply skipped.

After executing all moves, the output should be the sum of coordinates of all boxes,
such as for a box with coordinates (X, Y) 100Y+X should be added to the result.

It is undefined what happens when the robot walks or pushes boxes outside of the room,
but all test inputs have solid walls across all map edges.

**Solution 1**: straightforward.  Read the map into a vector of strings, and move
instructions into one more string.  Robot coordinates are easier to keep in separate variables,
and on the map we put just a `.` to that position.  We iterate through the moves, examining
the character in the next position.  If `#`, skip, if `.`, just move there; if `O`,
then we continue looping in that direction until a `#` or `.` is found; in first case skip,
in second case put a `O` into that position and `.` into the position where the robot
has stepped.  After finishing just loop through the field and add up coordinates
for all `O`s found.

**Problem 2**: although the input is the same, the map which it represents should be now
blown up to be twice as wide.  `.` now represents two floor cells (`..`), `#` two wall
cells (`##`) and `O` a box which is two characters wide (`[]`).  The robot however is still
the same 1x1 size; `@` in input means the robot shoud be put to the left cell of two (`@.`).
The robot still continues to move one step at a time according to instructions, and can still
push boxes, but thanks to being 2x1 in size they are not necessarily in a straight
row but can be interlocked in various ways; but still the robot can either push arbitrarily
many boxes, if there's space, or won't push any at all, if there's not enough space.
The output must be the coordinates of the left "half" of ever box summed up in the same way
as before.

**Solution 2**: widen the map when reading it in, using `[]` to mark boxes, as in problem
description.  Walking the map looks largely the same, except for pushing boxes.
We have now two separate different functions for horizontal and vertical pushes.
Horizontal is similar to the original solution, except that we should actually move box
characters, not just put one to the end and remove one in the beginning as before, as
the sequence of boxes now looks like `[][][]`, not just uniformly `OOOOO`.

For vertical movement, we build a vector of all boxes to be moved (the coordinates of
their left halves to be precise).  We start by putting into it just the box immediately
next to the robot (not forgetting that the robot can be pushing on either its left or right
half).  For every box in the vector, we look what is in the space where it should be pushed
to.  If there's at least one wall, the entire push is impossible and stop here; if there's
a box or two, put it to the end of the vector, examining several possible ways
for this box/boxes to be placed:

```
[]. .[] [] [][]
.[] []. [] .[].
```

The last case doesn't need to be explicitly tested for as it's just the first two cases
together.  When we reach the end of the vector, there's nothing else to push, and if we
didn't bail out before, then the push is indeed possible.

Then we just do the actual push, by going through that vector backwards, and for every box
putting `[]` into the adjacent row, and `..` in place where the box was.  Going through
the vector backwards ensures boxes are moved in correct order instead of getting overwritten
by each other.

**Execution time**: trivial.


## Day 16 "Reindeer Maze"

_Difficult.  Had to look up A* algorithm that I don't remember ever actually implementing,
then took a long time to debug second subproblem._

**Problem 1**: the input is a rectangular block of text with a map of a maze, with `.`
marking floors and `#` marking walls.  `S` marks the starting spot and `E` ending spot.
A "reindeer" must find a way from `S` to `E` for the lowest possible cost.  Reindeer's
position is determined not only by coordinates but also by orientation
(north/west/east/south); the reindeer always starts walking facing east.  A step forward
costs 1 point, but a 90 degrees turn in any direction costs 1000 points.  The output
must be the lowest possible cost of the way from `S` to `E`.

**Solution 1**: this is more or less a classic pathfinding problem, with turns as separate
actions adding a small twist.  I implemented the A* algorithm, using C++'s
priority queue template and a distance estimation function that simply adds up X and Y
differences of current coordinates with the ending ones.  I was broadly aware of this
algorithm and knew to start with it, but I don't remember ever having to actually implement
it myself, so this was actually rather educational.  To calculate new positions I have a
table of 12 possible moves (simple move forward or turn + move).  I keep a backtracking
map for visualization (commented out).  Generally I guess a rather straightforward task.

Afterwards, reading Reddit I did realize that A* doesn't help much over more straightforward
Dijkstra in this particular puzzle because the distance estimation function is actually
a quite poor fit here, since turns are massively more expensive than moves.  Oh well.
In any case it runs in milliseconds and I now remember how A* is supposed to work.

**Problem 2**: instead of just finding the lowest possible cost, find all possible routes
between `S` and `E` which have the same, lowest possible, cost.  With test inputs and
the real big input this in practice means a few places where the route splits in two
or more and soon merges back again.  Count all distinct map squares visited on all
routes (including `S` and `E` squares themselves).

**Solution 2**: this was an annoying one and took fairly long time to debug, but I guess
conceptually it's rather simple.  Finding all possible routes means we can downgrade
from A* to Dijkstra and first calculate costs for every (x, y, direction) tuple that
is accessible from the starting point.  (After reaching finish, we can trim squares
that result in cost greater than for finish.)

On the second step, we start from the ending position(s) and flood-fill, backtracking
along those tuples according to problem's rules, and marking previously unvisited squares
on the way.  A minor complication here is that the ending position can be reached
from any possible direction, so we may need to start not from one but from several ending
positions, differing only in direction, as long as all of them have the same minimal cost, calculated on the first step.

**Execution time**: about 25 ms.  Probably could be improved by using better data
structures.


## To be continued
