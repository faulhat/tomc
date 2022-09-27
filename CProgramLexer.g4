lexer grammar CProgramLexer;

fragment DIGIT: [0-9] ;

Int: ( '0x' | '0b' )? DIGIT+ 'l'? ;
Float: ( DIGIT+ '.' DIGIT* | '.' DIGIT+ ) 'f'? ;
String: '"' ( ~'"' | '\\"' )*? '"' ;
Char: '\'' ( ~'\'' | '\\\'' )*? '\'' ;

HASH: '#' -> mode(STRINGIFY) ;
TokenConcat: [a-zA-Z_] [a-zA-Z_0-9]* '##' [a-zA-Z_0-9]+ ;

// Keywords
TYPEDEF: 'typedef' ;
RETURN: 'return' ;
IF: 'if' ;
ELSE: 'else' ;
FOR: 'for' ;
WHILE: 'while' ;
STRUCT: 'struct' ;
UNION: 'union' ;
CONST: 'const' ;

// Symbols
SEMI: ';' ;
COMMA: ',' ;
OSQ: '[' ;
CSQ: ']' ;
OCURL: '{' ;
CCURL: '}' ;
OPAR: '(' ;
CPAR: ')' ;
INC: '++' ;
DEC: '--' ;
DOT: '.' ;
ARROW: '->' ;
STAR: '*' ;
PLUS: '+' ;
MINUS: '-' ;
LOJNOT: '!' ;
BITNOT: '~' ;
BITAND: '&' ;
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
XOR: '^' ;
BITOR: '|' ;
LOJAND: '&&' ;
LOJOR: '||' ;
WONDER: '?' ;
COLON: ':' ;

COMMENT: '/*' .*? '*/' -> skip ;
LCOMMENT: '//' .*? ( [\n\r] | EOF ) -> skip ;

WS: [ \n\t\r] -> skip ;
DEFINE: '#define' -> mode(PREPROC) ;
IFDEF: '#ifdef' -> mode(PREPROC) ;
IFNDEF: '#ifndef' -> mode(PREPROC) ;
UNDEF: '#undef' -> mode(PREPROC) ;
INCLUDE: '#include' -> mode(PREPROC) ;
ENDIF: '#endif' -> mode(PREPROC) ;

Name: [a-zA-Z_] [a-zA-Z_0-9]* ;

mode PREPROC;
IGNORE: ( ' ' | '\t' | '\\' [\n\r] ) -> skip ;
ENDL: ( '\n' | '\r' | EOF ) -> mode(DEFAULT_MODE) ;
PRE_String: '"' ( ~'"' | '\\"' )*? '"' ;
PRE_Path_Inc: '<' ( ~'>' | '\\>' )*? '>' ;
PRE_Name: [a-zA-Z_] [a-zA-Z_0-9]* ;
PRE_Other: .+? ;

mode STRINGIFY;
ArgName: [a-zA-Z_] [a-zA-Z_0-9]* -> mode(DEFAULT_MODE) ;
