## DONOR SCRIPTS
Important guidelines that should be followed or otherwise better organization techniques to be followed.




### File Structure
Each donor is to have their own .inc, a file that's included into core.inc.
Donor file names should contain their full name and SQL ID. Format: Jacob_Malcom-508781 so we can find people in A-Z ordering based on their name.
Keep donor scripts in `./donor`

### Hooks
Hooks allow you to chain together code snipplets while keeping it as one function or callback accessible through many different files.
If you do not know how hooks work correctly immediately learn through following [this tutorial](http://forum.sa-mp.com/showthread.php?t=570910). NOTE: We use y_hooks from the YSI libary http://forum.sa-mp.com/showthread.php?t=570883

Hooks are processed during compilation, "preprocessing", hence does not affect the performance in-game.
When using hooks assure the callback or function is always called before your code snipplet. This assures if your code is faulty it will not crash the entire function, but it will process the rest of the function before reaching your snipplet.

### Organization & Readability
Use line spacing between functions, comments that describes important detail (such as loop invariants and post conditions).

Post condition: what's the expected output, or what should x look after the code has executed? Example: `"the array should be in A-Z order left to right."`
Loop invariant: what inside the loop should always remain true? Example: I want to sort a list so the loop invariant inside the loop is that `"the left side to index i of the array will always be sorted."`

Here's [a link](http://patorjk.com/software/taag/#p=display&f=Big&t=Type%20Something%20) to create bold headers that stand out I usually use these types of headings for i.e "Saving", "Loading", "Hooks", major headings only.

### Avoiding namespace-type errors
To avoid definition collisions especially with enumerators as we cannot simply make them static,
```
//give the data unique names
enum KlashBlah_BarInfo {
  klashSQLID,
  klashOwnerID,
  Float:klashX,
  Float:klashY,
  Float:klashZ
}

new KlashBarData[MAX_BARS][KlashBlah_BarInfo];

//make it easier to access/use, simplfy the length of your code.
#define bInfo(%0, %1) KlashBarData[%0][klash%1]

//usage: bInfo(i, SQLID) to access it, where i is the index (i.e inside loop).

...
//Then at the bottom of the script (same file),

#undef bInfo

```

Questions? Don't hesitate to ask.
