DW APPROVED

TributeStance
------

Josh Hofing, Zane Sterling, Sebastian Conybeare

The Operating Model:
  TributeStance operates over a distributed net of computers, which is kicked off with one hosting a source file and beginning execution. It will execute the program one line at a time, and as soon as it reaches a non-primitive statement, will fill the result of that statement with a Future (see below).
  
  Futures are a construct wherein execution of a statement is put off sometime into the future. When a future's statement gets executed, it is called a Fulfilled Future. Otherwise, it is an Unfulfilled Future.

  When a statement which takes an argument that is an Unfulfilled Future, it will wait until that Future becomes Fulfilled before continuing execution.

  In order to evaluate Futures, other computers (or at least TributeStance interpreters) must evaluate that information. An interpreter *ABSOLUTELY MAY NOT* evaluate a Future that it created.
  
  For this reason, TributeStance interpreters listen on a socket for connecting to, and will respond with a list of all TributeStance interpreters that are connected to it. When an interpreter is added, it first assigns itself a unique ID, then broadcasts its presence to all interpreters on the network, which also remember it.

  When a Future requires evaluation, it is sent to another interpreter to be evaluated. When it has sucessfully been evaluated, it will inform the interpreter that sent it that Future, and make that data available to it.

  The exact details of the networking protocol will be described in a future document.

Includes:
  The #include statement is a special, reserved statement. It will take a filename, relative to the directory in which the source file is, on the machine where the source file is loaded (the first host). It might look something like:
    #include "filename.ts"
  This statement would dump the entirety of the file with the name "filename.ts" in its place. It will also add "filename.ts" to an index of files already imported, and future calls will ensure that their file has not already been included.

Special Function Marks:
  In order to make TributeStance actually usable, this specifiction defines two special marks that can be introduced before a function is declared.

  local:
    The local keyword before a function declaration causes that function to never create a Future. Instead, it will immediately be evaluated, and it's result will be immediately stored. Additionally, the function is expected to be run on the machine on which it was created.

  cached:
    The cached keyword before a function declaration causes that function to store, for each unique set of inputs it recieves, the output. Before executing, this cache will be searched through, and, if the current inputs already have a known output, it will return that without actually evaluating the function. If it does not have a cached output, it will evaluate the function and store the result in the cache.

Types:
  bit: Either a 0 or a 1
  list: A sequence of primitives (ie, lists or bits or functions)
  function: An operation that takes a series of arguments and returns a single value

Lists:
  Lists are a series of types, and can be constructed with the notation (val1, val2, val3, etc)

Functions:
  Functions take a series of named arguments, and return a single value
  The syntax for declaring a function is
  fn(arg1, arg2, arg3, etc) {
    statement;
    statement;
    statement;
    return statement;
  }

Literals:
  The literal representations of bits are 0 for off and 1 for on.
  The literal representation of the empty list is ()
  The literal representation of a list with elements is (el1, el2, el3, ..., eln)

Native Operations:
  All Operations use function notation, so using one looks like:
    op(arg1, arg2);
  All native operations take either 1 or two arguments.
  On bits: 
    OR, |: If either bit is 1, return 1, otherwise return 0
      OR(1, 1) -> 1
      |(1, 0) -> 1
      |(0, 0) -> 0
    NOT, !: If the input is 1, return 0, else return 1
  On lists:
    CAR, [: Returns the last element of a list
    CDR, ]: Returns a list containing all the elements of the input list, except for the final one
    PUSH, <: Appends an element to a list, returns the resulting list (does not modify the input)
    EMPTY, ?: Returns 1 if the list is empty, 0 if not.

Short Circuiting:
  The native OR operation will, if the first element is a bit with the value of 1, not evaluate the second statement.

Comments:
  Comments are everything on a line following a //, and will not be evaluated

Conditionals:
  There is exactly one type of conditional in TributeStance: If
  If takes a bit, and two functions. If the bit is 1, the first function will be executed, otherwise the second will be executed.

Informal Definition: A "character" is a list containing exactly 8 bits, representing an ascii value.

IO:
  read(list) takes a list of characters, and reads the corresponding file into a list of characters, does so for every machine connected, and returns a list containing all of those.
  write(list, list) takes 2 lists of characters. The first contains a filename, and the second contains the file contents. It may be written to any machine connected. returns 0.
  print(list) takes a character, and prints that as if it were an ascii value to all connected machines. If the character is invalid, some horrible thing happens. returns 0.

Example Program: AND
//Assumes both args are bits
var AND = fn(b1, b2) {
  var c1 = ~(b1)
  var c2 = ~(b2)
  var res = ~(|(c1, c2))
  return res
}
// We'll also make & AND, because brevity
var & = AND

var NAND = fn(b1,b2, AND) {
  return ~(AND(b1, b2))
}
NAND(1, 0, AND) // we have to pass in all functions that aren't primitives
