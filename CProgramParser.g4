parser grammar CProgramParser;

options { tokenVocab = CProgramLexer; }

// Without the EOF, extraneous invalid input may be ignored.
program
    : stmtList EOF
    ;

stmtList
    : ( preproc | statement )*
    ;

// Preprocessor directives
// Some expressions are valid in preprocessing but not in actual code.
preproc
    : DEFINE Name ENDL #DefineFlag
    | DEFINE Name expression ENDL #DefineConst
    | DEFINE Name macroArgList ENDL #DefineMacro
    | IFDEF Name ENDL #IfDef
    | IFNDEF Name ENDL #IfNotDef
    | UNDEF Name ENDL #UnDef
    | INCLUDE String ENDL #IncludeFile
    | ENDIF ENDL #EndIf
    ;

macroArgList
    : OPAR ( ( Name COMMA )* Name )? CPAR
    ;

statement
    : SEMI #Empty
    | expression SEMI #EvalExpr
    | RETURN expression SEMI #ReturnExpr
    | deflist SEMI #NewVars
    | fnDeclaration #StFnDecl
    | fnImplementation #StFnImpl
    | typeDefinition #StType
    | block #StBlock
    | ifStmt #StIf
    | forLoop #StFor
    | whileLoop #StWhile
    ;

ifStmt
    : IF OPAR expression CPAR statement elseStmt?
    ;

// an else-if clause will be parsed as an else-clause with an if-statement as its predicate.
elseStmt
    : ELSE statement
    ;

forLoop
    : FOR OPAR ( initDef=deflist | initExpr=expression ) SEMI cond=expression SEMI iter=expression CPAR toDo=statement
    ;

whileLoop
    : WHILE OPAR expression CPAR statement
    ;

declaration
    : type varName
    ;

// A variable name to be used in a declaration.
// Note that the brackets in an array declaration can come after the type or the varname.
varName
    : Name #simpleName
    | varName OSQ CSQ #arrName
    ;

// A comma-separated list of variable declarations and/or definitions
deflist
    : declaration #SingleDecl
    | declaration ASSIGN expression #SingleDef
    | deflist COMMA Name #MultDecl
    | deflist COMMA Name ASSIGN expression #MultDef
    ;

fnDeclaration
    : declaration argDeclList SEMI
    ;

fnImplementation
    : declaration argDeclList block
    ;

// A type alias definition
typeDefinition
    : TYPEDEF type Name SEMI
    ;

// A list of argument declarations for a function
argDeclList
    : OPAR ( ( ( declaration | type ) COMMA )* ( declaration | type ) )? CPAR
    ;

// A bracketed list of statements to be executed with its own scope
block
    : OCURL statement* CCURL
    ;

// Any expression which can be evaluated
expression
    : expression op=( INC | DEC ) #SuffixOp
    | expression arglist #Call
    | expression OSQ expression CSQ #Subscript
    | expression op=( DOT | ARROW ) Name #MemberGet
    | OPAR type CPAR initlist #CompLiteral

    | op=( INC | DEC | PLUS | MINUS | LOJNOT | BITNOT | STAR | BITAND | SIZEOF ) expression #PrefixOp
    | OPAR type CPAR expression #CCast

    | left=expression op=( STAR | DIV | MOD ) right=expression #MultOp
    | left=expression op=( PLUS | MINUS ) right=expression #AddOp
    | left=expression op=( LSHIFT | RSHIFT ) right=expression #ShiftOp
    | left=expression op=( LT | LE | GT | GE ) right=expression #CompareOp
    | left=expression op=( EQ | NE ) right=expression #EqOp
    | left=expression BITAND right=expression #BitAndOp
    | left=expression XOR right=expression #BitXorOp
    | left=expression BITOR right=expression #BitOrOp
    | left=expression LOJAND right=expression #LojAndOp
    | left=expression LOJOR right=expression #LojOrOp
    | cond=expression WONDER ifTrue=expression COLON ifFalse=expression #TernaryOp
    | lval=expression op=( ASSIGN | INCBY | DECBY | MULTBY | DIVBY | MODBY | LSHIFTBY | RSHIFTBY | ANDBY | ORBY | XORBY )
        rval=expression #SetVal
    
    | OPAR expression CPAR #Parenthesized
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
    : OPAR ( ( expression COMMA )* expression )? CPAR
    ;

// An initializer list for an array or struct
initlist
    : OCURL ( ( expression COMMA )* expression )? CCURL
    ;

// Any type identifier
type
    : Name #SimpleType
    | type OSQ CSQ #ArrayType
    | type STAR #PtrType
    | CONST type #ConstType
    | type CONST #ConstType
    | kind=( STRUCT | UNION ) Name? structlist #ComplexType
    | kind=( STRUCT | UNION ) Name #AnonType
    | OPAR STAR type CPAR typelist #FnPtrType
    ;

// A parenthesized, comma-separated list of types
typelist
    : OPAR ( ( type COMMA )* type )? CPAR
    ;

// A list of data members in a complex type
structlist
    : OCURL ( type Name SEMI )* CCURL
    ;
