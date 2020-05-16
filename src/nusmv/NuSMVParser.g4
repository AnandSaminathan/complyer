parser grammar NuSMVParser;

options {
  tokenVocab = NuSMVLexer;
}

nusmv              : (newline | module)* module ( safetySpec
                                                | ltlSpec
                                                | newline
                                                )* EOF; 

module             : MODULE WS name=id WS? parameters? WS? newline ( declaration
                                                                   | moduleCall
                                                                   | assignment
                                                                   | init
                                                                   | trans
                                                                   | define
                                                                   | newline
                                                                   | WS
                                                                   )+;

safetySpec         : SAFETYSPEC newline WS? formula WS? (newline bound)?;
ltlSpec            : LTLSPEC newline WS? formula WS? newline bound;

parameters         : WS? OPEN_P formula (COMMA WS? formula)* CLOSE_P;
declaration        : VAR WS id WS? COLON WS? type WS? SC WS?;
moduleCall         : VAR WS name=id WS? COLON WS? PROCESS? WS? moduleName=id WS? parameters? WS? SC WS?;
assignment         : ASSIGN WS? newline ( initAssignment
                                        | seqNextAssignment
                                        | conNextAssignment
                                        | newline
                                        | WS
                                        )+;

init               : INIT WS? newline WS? formula;
trans              : TRANS WS? newline WS? formula;
define             : DEFINE WS? newline (definition | newline)+;

bound              : WS? BOUND WS? val=wholeNumber WS? SC;

initAssignment     : WS? INITF OPEN_P WS? id WS? CLOSE_P WS? assign WS? seqExpression WS? SC WS?;
seqNextAssignment  : WS? NEXT OPEN_P WS? id WS? CLOSE_P WS? assign WS? seqExpression WS? SC WS?;
conNextAssignment  : WS? NEXT OPEN_P WS? set WS? CLOSE_P WS? assign WS? conExpression WS? SC WS?;

// sequential expression

seqExpression      : seqSimpleExpr
                   | seqCaseExpr
                   | seqIntervalExpr
                   | seqSetExpr
                   ;

seqSimpleExpr      : formula;
seqCaseExpr        : WS? CASE WS? newline (seqCaseSubExpr | newline)+ WS? ESAC WS?;
seqIntervalExpr    : from=wholeNumber DOT DOT to=wholeNumber;
seqSetExpr         : OPEN_C WS? seqSetSubExpr (WS? COMMA WS? seqSetSubExpr)* WS? CLOSE_C WS?;
seqCaseSubExpr     : WS? formula WS? COLON WS? seqExpression WS? SC WS?;
seqSetSubExpr      : seqExpression;

// concurrent expression

conExpression      : formula WS? label WS? COLON WS? seqSetExpr;

definition         : WS? id WS? assign WS? formula WS? SC WS?;

label              : OPEN_S WS? id WS? CLOSE_S;
type               : BOOLEAN | INTEGER;
set                : OPEN_C WS? id (WS? COMMA WS? id)* WS? CLOSE_C;
value              : (TRUE | FALSE | id | wholeNumber);

formula            : (operators | OPEN_P | CLOSE_P | id | nextId | TRUE | FALSE | wholeNumber | WS)+;
assign             : COLON ASSGN;
nextId             : NEXT UNDERSCORE id;
id                 : (alpha | UNDERSCORE) (alphaNum | UNDERSCORE | DOT)*;
wholeNumber        : (DIGIT)+;
alpha              : (LOWER_CASE | UPPER_CASE);
alphaNum           : (alpha | DIGIT);
newline            : NL;
operators          : (PLUS | MINUS | DIV | MUL | MOD | NOT | AND | OR | IMPLIES | EQUIVALENT | EQ 
                      | NEQ | GT | LT | GTE | LTE 
                      | NEXTOP | UNTIL | RELEASE | GLOBAL | FUTURE);

