parser grammar NuSMVParser;

options {
  tokenVocab = NuSMVLexer;
}

module             : MODULE WS MAIN WS? newline (varConstruct
                                                | ivarConstruct
                                                | initConstruct
                                                | nextConstruct
                                                | assignConstruct
                                                | newline)* EOF;


assignConstruct    : ASSIGN newline;
varConstruct       : VAR WS id WS? COLON WS? type WS? SC WS?;
ivarConstruct      : IVAR WS id WS? COLON WS? type WS? SC WS?;
initConstruct      : INIT OPEN_PARAN WS? id WS? CLOSE_PARAN WS? assign WS? value WS? SC WS?;
nextConstruct      : NEXT OPEN_PARAN WS? id WS? CLOSE_PARAN WS? assign WS? value WS? SC WS?;

type               : (interval | BOOLEAN | set);
interval           : wholeNumber DOT DOT wholeNumber;
set                : OPEN_CURLY WS? id (WS? COMMA WS? id)* WS? CLOSE_CURLY;
value              : (TRUE | FALSE | id | wholeNumber);

assign             : COLON EQ;
id                 : (alpha) alphaNum*;
wholeNumber        : (DIGIT | DIGIT+);
alpha              : (LOWER_CASE | UPPER_CASE);
alphaNum           : (alpha | DIGIT);
newline            : NL;

