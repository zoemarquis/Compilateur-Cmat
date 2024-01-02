%{
    #include "generation.h"
    #include "symbtab.h"
    #include "EXPR.tab.h"
%}

%option nounput
%option noyywrap

DIGIT    [0-9]

ID [_a-zA-Z][_a-zA-Z0-9]*
INT 0|[1-9]{DIGIT}*
FLOAT {INT}?\.{DIGIT}*|{INT}?(\.{DIGIT}*)?[eE][\-\+]?{INT}
COMMENT \/\*([^\*]|\*+[^\/])*\*\/
SINGLECOMMENT \/\/.*
STRING \"([^"])*\"

%%
"main"                                      { return MAIN;}
"printf"                                    { return PRINTF;}
"printmat"                                  { return PRINTMAT;}
"print"                                     { return PRINT;}
"matrix"                                    { return MATRIX;}
"int"                                       { return INT;}
"float"                                     { return FLOAT;}
"return"                                    { return RETURN;}
"exit"                                      { return EXIT; }

"static"                                    { return STATIC;}
"const"                                     { return CONST;}
"if"                                        { return IF;}
"else"                                      { return ELSE;}
"while"                                     { return WHILE;}
"for"                                       { return FOR;}
{ID}                                        {
                                                strncpy(yylval.strval,yytext,7);
                                                yylval.strval[7] = '\0';
                                                return ID;}

"\."                                        { return DOT; }
"="                                         { return ASSIGN; }
"~"                                         { return TILDE;}
"++"                                        { return PLUSPLUS;}
"+"                                         { return PLUS;}
"--"                                        { return MINUSMINUS;}
"-"                                         { return MINUS;}
"*"                                         { return MULT;}
"/"                                         { return DIV;}
"("                                         { return LPAR; }
")"                                         { return RPAR; }
"{"                                         { return LCURLY; }
"}"                                         { return RCURLY; }
"\["                                        { return LBRACKET;}
"\]"                                        { return RBRACKET;}
","                                         { return COMMA; }
";"                                         { return SEMICOLON; }
"=="                                        { return EQUAL;}
"!="                                        { return NOT_EQUAL; }
"<"                                         { return LESS_THAN; }
">"                                         { return GREATER_THAN; }
"<="                                        { return LTOE; }
">="                                        { return GTOE; }
"\""                                        { return QUOTE; }
"\'"                                        { return APOSTROPHE; }
"\\"                                        { return BACKSLASH; }
{FLOAT}                                     { yylval.floatval = atof(yytext);
                                                return V_FLOAT;}
{INT}                                       { yylval.intval = atoi(yytext);
                                                return V_INT;}
{STRING}                                    { yylval.string = strdup(yytext); 
                                                return V_STRING;}
\r|\n                                       {/*nb_ligne = nb_ligne +1;*/}
{COMMENT}                                   { /*Ne rien faire*/; }
{SINGLECOMMENT}                             { /*Ne rien faire*/; }
[[:space:]]                                 { /*Ne rien faire*/; }

.                                           {printf("<Erreur token inconnu : '%s'>\n", yytext);}

%%
