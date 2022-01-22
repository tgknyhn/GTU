%%
and             {printf("KW_AND\n");}           /* Keywords */
or              {printf("KW_OR\n");}
not             {printf("KW_NOT\n");}
equal           {printf("KW_EQUAL\n");}
less            {printf("KW_LESS\n");}
nil             {printf("KW_NIL\n");}
list            {printf("KW_LIST\n");}
append          {printf("KW_APPEND\n");}
concat          {printf("KW_CONCAT\n");}
set             {printf("KW_SET\n");}
deffun          {printf("KW_DEFFUN\n");}
for             {printf("KW_FOR\n");}
if              {printf("KW_IF\n");}
exit            {printf("KW_EXIT\n");}
load            {printf("KW_LOAD\n");}
disp            {printf("KW_DISP\n");}
true            {printf("KW_TRUE\n");}
false           {printf("KW_FALSE\n");}

"+"             {printf("OP_PLUS\n");}         /* Operators */
"-"             {printf("OP_MINUS\n");}
"/"             {printf("OP_DIV\n");}
"*"             {printf("OP_MULT\n");}
"("             {printf("OP_OP\n");}
")"             {printf("OP_CP\n");}
"**"            {printf("OP_DBLMULT\n");}
"‘"             {printf("OP_OC");}
"’"             {printf("OP_CC\n");}
","             {printf("OP_COMMA\n");}


^;;.*$                  {printf("COMMENT\n");}        /* ^;; -> Starts with ;; | .*$ -> Ends with any numbers of any character */
0                       {printf("VALUE\n");}          /* Unsigned Integer (0) */
[1-9][0-9]*             {printf("VALUE\n");}          /* Unsigned Integer (No leading 0)*/
[1-9][0-9]*[.][0-9]*    {printf("VALUE\n");}          /* Unsigned Real Number */
[a-zA-Z][a-zA-Z0-9]*    {printf("IDENTIFIER\n");}     /* Ideftifier */

[ \t]                   ;
\n                      {return 0;}
.                       {printf("Unidentified token\n");}
%%

int yywrap() {}

int main() {

    while(1) {
        printf("Enter: ");
        yylex();
    }
        
    return 0;
}