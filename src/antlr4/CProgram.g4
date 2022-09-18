grammar CProgram;

// Without the EOF, extraneous invalid input may be ignored.
program
    : (preproc | statement)* EOF
    ;

// Preprocessor directives
// Some expressions are valid in preprocessing but not in actual code.
// TODO: These errors should be detected semantically.
preproc
    : '#define' Name ENDL #DefineFlag
    | '#define' Name expression ENDL #DefineConst
    | '#define' Name macroArgList expression ENDL #DefineMacro
    | '#ifdef' Name ENDL #IfDef
    | '#ifndef' Name ENDL #IfNotDef
    | '#undef' Name ENDL #UnDef
    | '#endif' ENDL #EndIf
    | '#include' String ENDL #IncludeFile
    ;

macroArgList
    : '(' ( ( Name ',' )* Name )? ')'
    ;

statement
    : ';'
    | expression ';'
    | deflist ';'
    | fnDeclaration
    | fnImplementation
    | typeDefinition
    | block
    | ifStmt
    | forLoop
    | whileLoop
    ;

// else-if clauses will be parsed as an else-clause with an if-statement as its predicate.
ifElse
    : ifStmt 'else' statement
    ;

ifStmt
    : 'if' '(' expression ')' statement
    ;

forLoop
    : 'for' '(' ( initDef=deflist | initExpr=expression ) ';' cond=expression ';' iter=expression ')' toDo=statement
    ;

whileLoop
    : 'while' '(' expression ')' statement
    ;

declaration
    : type varName
    ;

// A variable name to be used in a declaration.
// Note that the brackets in an array declaration can come after the type or the varname.
varName
    : Name
    | varName '[' ']'
    ;

// A comma-separated list of variable declarations and/or definitions
deflist
    : declaration #SingleDecl
    | declaration ASSIGN expression #SingleDef
    | deflist ',' Name #MultDecl
    | deflist ',' Name ASSIGN expression #MultDef
    ;

fnDeclaration
    : declaration argDeclList ';'
    ;

fnImplementation
    : declaration argDeclList block
    ;

// A type alias definition
typeDefinition
    : 'typedef' type Name ';'
    ;

// A list of argument declarations for a function
argDeclList
    : '(' ( ( ( declaration | type ) ',' )* ( declaration | type ) )? ')'
    ;

// A bracketed list of statements to be executed with its own scope
block
    : '{' statement* '}'
    ;

// Any expression which can be evaluated
expression
    : expression op=( INC | DEC ) #SuffixOp
    | expression arglist #Call
    | expression '[' expression ']' #Subscript
    | expression op=( DOT | ARROW ) Name #MemberGet
    | '(' type ')' arglist #CompLiteral

    | op=( INC | DEC | PLUS | MINUS | LOJNOT | BITNOT | STAR | AND | SIZEOF ) expression #PrefixOp
    | '(' type ')' expression #CCast

    | left=expression op=( STAR | DIV | MOD ) right=expression #MultOp
    | left=expression op=( PLUS | MINUS ) right=expression #AddOp
    | left=expression op=( LSHIFT | RSHIFT ) right=expression #ShiftOp
    | left=expression op=( LT | LE | GT | GE ) right=expression #CompareOp
    | left=expression op=( EQ | NE ) right=expression #EqOp
    | left=expression AND right=expression #BitAndOp
    | left=expression '^' right=expression #BitXorOp
    | left=expression '|' right=expression #BitOrOp
    | left=expression '&&' right=expression #LojAndOp
    | left=expression '||' right=expression #LojOrOp
    | cond=expression '?' ifTrue=expression ':' ifFalse=expression #TernaryOp
    | lval=expression op=( ASSIGN | INCBY | DECBY | MULTBY | DIVBY | MODBY | LSHIFTBY | RSHIFTBY | ANDBY | ORBY | XORBY )
        rval=expression #ReAssign
    
    | '(' expression ')' #Parenthesized
    | Name #NameAtom
    | Int #IntAtom
    | Float #FloatAtom
    | String #StrAtom
    | Char #CharAtom
    | StringifyToken #StringifyTokenSpecial
    | TokenConcat #ConcatTokensSpecial
    ;

// A parenthesized list of expressions to be passed as arguments to a function
arglist
    : '(' ( ( expression ',' )* expression )? ')'
    ;

// An initializer list for an array or struct
initlist
    : '{' ( ( expression ',' )* expression )? '}'
    ;

// Any type identifier
type
    : Name #SimpleType
    | type '[' ']' #ArrayType
    | type STAR #PtrType
    | CONST type #ConstType
    | type CONST #ConstType
    | kind=( STRUCT | UNION ) Name? structlist #ComplexType
    | kind=( STRUCT | UNION ) Name #UndefType
    | '(' STAR type ')' typelist #FnPtrType
    ;

// A parenthesized, comma-separated list of types
typelist
    : '(' ( ( type ',' )* type )? ')'
    ;

// A list of data members in a complex type
structlist
    : '{' ( type Name ';' )* '}'
    ;

ENDL: ( '\n' | '\r' | EOF ) ;
fragment DIGIT: [0-9] ;

Name: [a-zA-Z_] [a-zA-Z_0-9]* ;
StringifyToken: '#' [a-zA-Z_] [a-zA-Z_0-9]* ;
TokenConcat: [a-zA-Z_] [a-zA-Z_0-9]* '##' [a-zA-Z_0-9]+ ;
Int: ( '0x' | '0b' )? DIGIT+ 'l'? ;
Float: ( DIGIT+ '.' DIGIT* | '.' DIGIT+ ) 'f'? ;
String: '"' ( ~'"' | '\\"' ) '"' ;
Char: '\'' ( ~'\'' | '\\\'' ) '\'' ;

INC: '++' ;
DEC: '--' ;
DOT: '.' ;
ARROW: '->' ;
STAR: '*' ;
PLUS: '+' ;
MINUS: '-' ;
LOJNOT: '!' ;
BITNOT: '~' ;
AND: '&' ;
SIZEOF: 'sizeof' ;
DIV: '/' ;
MOD: '%' ;
LSHIFT: '<<' ;
RSHIFT: '>>' ;
LT: '<' ;
LE: '<=' ;
GT: '>' ;
GE: '>=' ;
EQ: '==' ;
NE: '!=' ;
ASSIGN: '=' ;
INCBY: '+=' ;
DECBY: '-=' ;
MULTBY: '*=' ;
DIVBY: '/=' ;
MODBY: '%=' ;
LSHIFTBY: '<<=' ;
RSHIFTBY: '>>=' ;
ANDBY: '&=' ;
ORBY: '|=' ;
XORBY: '^=' ;
STRUCT: 'struct' ;
UNION: 'union' ;
CONST: 'const' ;

COMMENT: '/*' .*? '*/' -> skip ;
LCOMMENT: '//' .*? ( [\n\r] | EOF ) -> skip ;
WS: [ \n\t\r] -> skip ;
