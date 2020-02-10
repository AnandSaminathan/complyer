lexer grammar NuSMVLexer;


TRUE        : 'TRUE';
FALSE       : 'FALSE';
MAIN        : 'main';
INIT        : 'init';
NEXT        : 'next';
BOOLEAN     : 'boolean';
VAR         : 'VAR';
IVAR        : 'IVAR';
MODULE      : 'MODULE';
MFOTLSPEC   : 'MFOTLSPEC';
ASSIGN      : 'ASSIGN';
LOWER_CASE  : 'a'..'z';
UPPER_CASE  : 'A'..'Z';
DIGIT       : '0'..'9';
WS          : (' ' | '\t')+;
NL          : ('\n')+;
DOT         : '.';
COMMA       : ',';
COLON       : ':';
SC          : ';';
EQ          : '=';
OPEN_CURLY  : '{';
CLOSE_CURLY : '}';
OPEN_PARAN  : '(';
CLOSE_PARAN : ')';
