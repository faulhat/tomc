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
// These should be parsed by extracting these expressions and reparsing them.
preproc
    : DEFINE PRE_Name ENDL #DefineFlag
    | DEFINE PRE_Name PRE_Other ENDL #DefineConst
    | DEFINE PRE_Name PRE_Other ENDL #DefineMacro
    | IFDEF PRE_Name ENDL #IfDef
    | IFNDEF PRE_Name ENDL #IfNotDef
    | UNDEF PRE_Name ENDL #UnDef
    | INCLUDE PRE_String ENDL #IncludeFile
    | INCLUDE PRE_Path_Inc ENDL #IncludeFromPath
    | ENDIF ENDL #EndIf
    ;

macroArgList
    : OPAR ( ( Name COMMA )* Name )? CPAR
    ;

statement
    : RETURN expression SEMI #ReturnExpr
    | fnDeclaration #StFnDecl
    | fnImplementation #StFnImpl
    | typeDefinition #StType
    | block #StBlock
    | ifStmt #StIf
    | forLoop #StFor
    | whileLoop #StWhile
    | SEMI #Empty
    | expression SEMI #EvalExpr
    ;

ifStmt
    : IF OPAR expression CPAR statement elseStmt?
    ;

// an else-if clause will be parsed as an else-clause with an if-statement as its predicate.
elseStmt
    : ELSE statement
    ;

forLoop
    : FOR OPAR init=expression SEMI cond=expression SEMI iter=expression CPAR toDo=statement
    ;

whileLoop
    : WHILE OPAR expression CPAR statement
    ;

// A variable name to be used in a declaration.
// Note that the brackets in an array declaration can come after the type or the varname.
varName
    : Name #simpleName
    | varName OSQ CSQ #arrName
    ;

fnDeclaration
    : type varName argDeclList SEMI
    ;

fnImplementation
    : type varName argDeclList block
    ;

// A type alias definition
typeDefinition
    : TYPEDEF type Name SEMI
    ;

// A list of argument declarations for a function
argDeclList
    : OPAR ( ( type varName? COMMA )* type varName? )? CPAR
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
    
    | type varName EQ expression #Definition
    | type varName #Declaration

    | fst=expression COMMA snd=expression #CommaSeq

    | OPAR expression CPAR #Parenthesized
    | Name #NameAtom
    | Int #IntAtom
    | Float #FloatAtom
    | String #StrAtom
    | Char #CharAtom

    // For use in preprocessor macros
    // Detect usage outside the preprocessor and throw an error semantically
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
