parser grammar NuSMVParser;

options {
  tokenVocab = NuSMVLexer;
}

module             : (comment | newline)* MODULE WS MAIN WS? newline (varDeclaration
                                                                      | assignBlock
                                                                      | safetySpecBlock
                                                                      | ltlSpecBlock
                                                                      | comment
                                                                      | newline)+ EOF;


assignBlock        : ASSIGN newline (init | next | concurrentNext | comment | newline)+;
varDeclaration     : VAR WS id WS? COLON WS? type WS? SC WS?;
init               : INIT OPEN_PARAN WS? id WS? CLOSE_PARAN WS? assign WS? expression WS? SC WS?;
next               : NEXT OPEN_PARAN WS? id WS? CLOSE_PARAN WS? assign WS? expression WS? SC WS?;
concurrentNext     : NEXT OPEN_PARAN WS? set WS? CLOSE_PARAN WS? assign WS? conExpression WS?;
safetySpecBlock    : SAFETYSPEC newline WS? simpleExpression;
ltlSpecBlock       : LTLSPEC newline WS? simpleExpression WS? newline BOUND WS? bound=wholeNumber;
comment            : doubleDash ~(NL)*?;

expression         : caseExpression   #caseExpr
                   | simpleExpression #simpleExpr
                   | set              #setExpr
                   | interval         #intervalExpr
                   ;

conExpression      : WS? antecedent=simpleExpression WS? label WS? COLON WS? consequent=set SC WS?;

caseExpression     : CASE WS? newline? (caseSubExpression | newline)+ WS? ESAC;

caseSubExpression  : WS? antecedent=simpleExpression WS? COLON WS? consequent=simpleExpression SC WS?;

simpleExpression   : formula;

label              : OPEN_SQ WS? name=id WS? CLOSE_SQ;
type               : BOOLEAN | INTEGER;
interval           : from=wholeNumber DOT DOT to=wholeNumber;
set                : OPEN_CURLY WS? simpleExpression (WS? COMMA WS? simpleExpression)* WS? CLOSE_CURLY;
value              : (TRUE | FALSE | id | wholeNumber);

formula            : (operators | OPEN_PARAN | CLOSE_PARAN | id | nextId | TRUE | FALSE | wholeNumber | WS)+;
assign             : COLON ASSGN;
nextId             : NEXT UNDERSCORE id;
id                 : (alpha | UNDERSCORE) (alphaNum | UNDERSCORE)*;
wholeNumber        : (DIGIT | DIGIT+);
alpha              : (LOWER_CASE | UPPER_CASE);
alphaNum           : (alpha | DIGIT);
newline            : NL;
doubleDash         : MINUS MINUS;
operators          : (PLUS | MINUS | DIV | MUL 
                      | NOT | AND | OR | IMPLIES | EQUIVALENT 
                      | EQ | NEQ | GT | LT | GTE | LTE 
                      | NEXTOP | UNTIL | RELEASE | GLOBAL | FUTURE);

