lexer grammar NuSMVLexer;

COMMENT     : '--' ~('\n')* -> skip;
BOUND       : 'BOUND';
LTLSPEC     : 'LTLSPEC';
PROCESS     : 'process';
SAFETYSPEC  : 'SAFETYSPEC';
CASE        : 'case';
ESAC        : 'esac';
TRUE        : 'true';
FALSE       : 'false';
INITF       : 'init';
NEXT        : 'next';
BOOLEAN     : 'boolean';
INTEGER     : 'integer';
VAR         : 'VAR';
IVAR        : 'IVAR';
MODULE      : 'MODULE';
MFOTLSPEC   : 'MFOTLSPEC';
ASSIGN      : 'ASSIGN';
INIT        : 'INIT';
TRANS       : 'TRANS';
DEFINE      : 'DEFINE';
LOWER_CASE  : 'a'..'z';
UPPER_CASE  : 'A'..'Z';
DIGIT       : '0'..'9';
PLUS        : '+';
MINUS       : '-';
DIV         : '/';
MUL         : '*';
MOD 		    : '%';
NOT         : '!';
AND         : '&&';
OR          : '||';
IMPLIES     : '->';
EQUIVALENT  : '<->';
EQ          : '==';
NEQ         : '!=';
GT          : '>';
LT          : '<';
GTE         : '>=';
LTE         : '<=';
NEXTOP      : 'X';
UNTIL       : 'U';
RELEASE     : 'R';
GLOBAL      : 'G';
FUTURE      : 'F';
WS          : (' ' | '\t')+;
NL          : ('\n' | '\r')+;
DOT         : '.';
COMMA       : ',';
COLON       : ':';
SC          : ';';
ASSGN       : '=';
OPEN_C      : '{';
CLOSE_C     : '}';
OPEN_P      : '(';
CLOSE_P     : ')';
UNDERSCORE  : '_';
OPEN_S      : '[';
CLOSE_S     : ']';
