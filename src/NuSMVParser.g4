parser grammar NuSMVParser;

options {
  tokenVocab = NuSMVLexer;
}

module             : MODULE WS MAIN WS? newline (varDeclaration
                                                | assignBlock
                                                | safetySpecBlock
                                                | newline)* EOF;


assignBlock        : ASSIGN newline (init | next)*;
varDeclaration     : VAR WS id WS? COLON WS? type WS? SC WS?;
init               : INIT OPEN_PARAN WS? id WS? CLOSE_PARAN WS? assign WS? expression WS? SC WS?;
next               : NEXT OPEN_PARAN WS? (id | set) WS? CLOSE_PARAN WS? assign WS? expression WS? SC WS?;
safetySpecBlock    : SAFETYSPEC newline WS? formula;

expression         : (simpleExpression | caseExpression);

simpleExpression   : formula;
caseExpression     : CASE newline 
                     (WS? simpleExpression WS? COLON WS? simpleExpression SC WS?)+;

type               : (interval | BOOLEAN | INTEGER | set);
interval           : wholeNumber DOT DOT wholeNumber;
set                : OPEN_CURLY WS? id (WS? COMMA WS? id)* WS? CLOSE_CURLY;
value              : (TRUE | FALSE | id | wholeNumber);

formula            : (operators | id | TRUE | FALSE | wholeNumber | WS)*;
assign             : COLON ASSGN;
id                 : (alpha) alphaNum*;
wholeNumber        : (DIGIT | DIGIT+);
alpha              : (LOWER_CASE | UPPER_CASE);
alphaNum           : (alpha | DIGIT);
newline            : NL;
operators          : (PLUS | MINUS | DIV | MUL 
                      | NOT | AND | OR | IMPLIES | EQUIVALENT 
                      | EQ | NEQ | GT | LT | GTE | LTE 
                      | NEXTOP | UNTIL | RELEASE | GLOBAL | FUTURE);

