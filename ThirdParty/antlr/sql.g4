// Copyright 2014 Boray Data Co. Ltd.  All rights reserved.

grammar sql ;

@lexer::members {
  public boolean isDublQuoteId = true;
}

// statement / query -----------------------------------------------------------
statement         : statementChoice ';' ;
statementChoice   :  queryStatement | explainStatement | statisticsStatement ;

queryStatement    : query ;
explainStatement  : (queryHint)* EXPLAIN (all = ALL)? statementChoice ;
statisticsStatement : (queryHint)* (STATS | STATISTICS) (all = ALL)? statementChoice ;

query             : (queryHint)*
                    ( selectQuery
                    | setQuery
                    | insertQuery
                    | deleteQuery
                    | updateQuery
                    | createQuery
                    | dropQuery
                    | truncateQuery
                    ) ;

queryHint         : HINT_START (hint)? (',' hint)* HINT_END ;

hint              :   name = hintName
                  | ( name = hintName '='  hintValue )
                  | ( name = hintName '='? '(' hintValue (',' hintValue)* ')' )
                  ;

selectQuery       : (WITH withQuery (',' withQuery)*)?
                    selectExpr
                    (ORDER BY orderBy = orderByList)?
                    (LIMIT limit = INTEGER_LITERAL (OFFSET offset = INTEGER_LITERAL)?)?
                  ;

selectExpr        : ( (SELECT | '?')
                      (ALL | (distinct = DISTINCT))?
                      selectList
                      (FROM tableExpr)?
                      (WHERE    where   = predicate)?
                      (GROUP BY groupBy = groupByList)?
                      (HAVING   having  = predicate)?
                    )
                  | ( '(' selectExpr ')' )
                  ;

setQuery          : (WITH withQuery (',' withQuery)*)?
                    setExpr
                    (ORDER BY orderBy = orderByList)?
                    (LIMIT limit = INTEGER_LITERAL (OFFSET offset = INTEGER_LITERAL)?)?
                  ;

setExpr           : selectExpr
                  | setExpr op = INTERSECT (DISTINCT | all = ALL)? (corrClause)? setExpr
                  | setExpr op = (UNION|EXCEPT) (DISTINCT | all = ALL)? (corrClause)? setExpr
                  | '(' setExpr ')'
                  ;

corrClause        : corr = CORRESPONDING (BY '(' corrList ')')? ;

withQuery         : name = tableName
                    (  '(' columnAlias (',' columnAlias)* ')'  )?
                    AS '(' query ')' ;

tableExpr         : ref = tableReference (AS? alias = tableAlias)?        #fetch
                  | tableExpr
                    type = joinType?  joint = joinHint?
                    JOIN
                    tableExpr ON joinPredicate                            #joinOn
                  | tableExpr
                    type = joinType?  joint = joinHint?
                    JOIN
                    tableExpr
                    USING '(' joinUsingList ')'                           #joinUsing
                  | tableExpr
                    (',' | (CROSS JOIN))
                    tableExpr                                             #crossJoin
                  | '(' query ')'       (AS? as = tableAlias)?            #subquery
                  | '(' valuesExpr ')'  (AS? as = tableAlias)?            #valuesSubquery
                  ;

tableReference  : ( (catalog = catalogName '.')? schema = schemaName '.')? table = tableName ;

joinType          : INNER | (outer = (LEFT | RIGHT | FULL) OUTER?) ;

joinHint          : HINT_START
                    method = (LOOP | HASH | BLOCK | BLOOM)
                    ( '='? '(' values = hintValue (',' hintValue)* ')' )?
                    HINT_END ;

valuesExpr        : VALUES '('valueList')' ( ',' '('valueList')' )* ;

insertQuery       : (WITH withQuery (',' withQuery)*)?
                    INSERT INTO? ref = tableReference ( '('columnName (',' columnName)*')' )?
                    (valuesExpr | selectQuery | setQuery) ;

deleteQuery       : (WITH withQuery (',' withQuery)*)?
                    DELETE FROM? ref = tableReference
                    ( (WHERE predicate) | (FROM tableExpr) )? ;

updateQuery       : (WITH withQuery (',' withQuery)*)?
                    UPDATE ref = tableReference
                    SET columnName '=' expr (',' columnName '=' expr)*
                    ( (WHERE predicate) | (FROM tableExpr) )? ;


createQuery           : createTableQuery | createIndexQuery ;

createTableQuery      : CREATE reference = REFERENCE? TABLE (IF NOT EXISTS)? ref = tableReference
                        (
                            tableDefinition
                          | ( tableDefinition? AS? asSubquery (WITH noData=NO? DATA)? )
                          | ( columnNameList AS? asSubquery (WITH noData=NO? DATA)? )
                        ) ;

createIndexQuery      : CREATE UNIQUE? (INDEX | KEY) (IF NOT EXISTS)? idxName = indexName
                        ON tblName = tableReference
                        ( USING method = indexMethod )?
                        '(' expr (',' expr)* ')'
                        (WITH props = propertyList)?
                        ;

dropQuery         : DROP TABLE         (IF EXISTS)? name = tableReference  # dropTable
                  | DROP (INDEX | KEY) (IF EXISTS)? idxName = indexName ON tblName = tableReference # dropIndex
                  ;

truncateQuery     : TRUNCATE (TABLE)? name = tableReference ;


// table/column definition ------------------------------------------------------------
tableDefinition   : '('
                        (     columnDefinition | tableConstraint | indexDefinition)
                        (',' (columnDefinition | tableConstraint | indexDefinition))*
                    ')'
                    REFERENCE ?
                    tablePartitioning?
                    (WITH props = propertyList)?
                  ;

tablePartitioning : PARTITION BY? method=(HASH | VALUE)? ON? '(' expr (',' expr)* ')' ;

columnDefinition  : name = columnName
                    type = dataType
                    (
                        (notNullable = NOT? nullable = NULL)
                      | (primary = PRIMARY KEY)
                      | (unique  = UNIQUE KEY?)
                      | (unique  = UNIQUE mono = MONOTONIC KEY?)
                    )*
                    (WITH props = propertyList)?
                  ;

columnNameList    : '(' columnName (',' columnName)* ')' ;

tableConstraint   : (primary = PRIMARY KEY  ( expr | ('(' expr (',' expr)* ')') ))
                  | (unique = UNIQUE? mono = MONOTONIC KEY ( expr | ('(' expr (',' expr)* ')') ))
                  ;

indexDefinition   : unique = UNIQUE? (INDEX | KEY) name = indexName?
                    ON? ( expr | ('(' expr (',' expr)* ')') )
                    ( USING method = indexMethod )?
                    (WITH props = propertyList)?
                  ;

indexMethod       : HASH | TREE | BTREE | SKIPLIST ;

dataType          : (sqlDataType (USING className)?) | className
                  ;

sqlDataType       : BOOLEAN
                  | ( INTEGER    ( '(' precision (BITS | BYTES | DIGITS)? ')')? )
                  | ( DECIMAL    ( '(' precision DIGITS? (',' scale DIGITS?)? ')')? )
                  | ( FLOAT      ( '(' precision (BITS | BYTES | DIGITS)? ')')? )
                  | DATE
                  | TIMESTAMP
//                  | INTERVAL
                  | (
                      (
                        (
                          (CHAR | CHARACTER)
                          '(' size (BYTE | BYTES | CHAR | CHARS)? ')'
                        )
                      |
                        (
                          ( ((CHAR | CHARACTER) VARYING?) | VARCHAR )
                          ( '(' size (BYTE | BYTES | CHAR | CHARS)? ')' )?
                        )
                      |
                        (
                          TEXT
                        )
                      )
                      (COLLATE collate = collationName)?
                    )
                  | (BINARY     ( '(' size (BYTE | BYTES)? ')' ) )
                  | (VARBINARY  ( '(' size (BYTE | BYTES)? ')' )? )
                  | BLOB
                  ;

asSubquery        : selectOrValues ;
selectOrValues    : '(' (queryHint)* selectOrValues ')' | (queryHint)* (selectQuery | valuesExpr | setQuery) ;

// lists -----------------------------------------------------------------------
selectList        : selectListItem (',' selectListItem)* ;
selectListItem    : ( expr (AS? as = identifier)? ) | wild = columnWildcard ;

groupByList       : groupByListItem (',' groupByListItem)* ;
groupByListItem   : expr;

orderByList       : orderByListItem (',' orderByListItem)* ;
orderByListItem   : expr desc = (ASC | DESC)?;

joinUsingList     : joinUsingListItem (',' joinUsingListItem)* ;
joinUsingListItem : id = identifier ;

corrList          : corrListItem (',' corrListItem)* ;
corrListItem      : id = identifier ;

propertyList      : property (',' property)* ;
property          : name = propertyName ('=' value = propertyValue)? ;

valueList         : expr (',' expr)* ;


// scalar expressions ----------------------------------------------------------
predicate         : expr ;
joinPredicate     : expr ;
casePredicate     : expr ;


// expressions -----------------------------------------------------------------
expr
 :
   literal                                                          #literalVal
 | variableName                                                     #varRef
 | columnRef                                                        #colRef

// | expr op=( '.' | '..' ) fieldName                                 #fieldRef
// | expr      '->'         subfieldName                              #subfieldRef

 | '(' dataType ')' expr                                            #inlineCast
 |      op=( '-' | '+' ) expr                                       #unaryOp
 | expr '.' func=methodName '(' (expr (',' expr)*)? ')'             #methodCall
 | expr op=( '*' | '/' | '%') expr                                  #binaryOp
 | expr op=( '+' | '-' | '||' ) expr                                #binaryOp
 | expr op=( '<' | '<=' | '=' | '!=' | '<>' | '>=' | '>' ) expr     #binaryOp

 |          CAST        '(' expr (AS | ',') dataType ')'            #castOp
 |          TYPEOF      '(' expr ')'                                #typeOfOp

 |          CASE        (
                            ( caseExpr = expr (WHEN expr THEN expr)+ )
                          | ( WHEN casePredicate THEN expr )+
                        )
                        (ELSE expr)?
                        END                                         #caseOp

 |          IF          '(' casePredicate ',' expr ',' expr ')'     #ifOp
 |          IFNULL      '(' expr ',' expr ')'                       #ifNullOp
 |          NULLIF      '(' expr ',' expr ')'                       #nullIfOp

 |          COALESCE    '(' (expr (',' expr)*)? ')'                 #coalesceOp
 |          DECODE      '(' (expr (',' expr)*)? ')'                 #decodeOp
 |          CONCAT      '(' (expr (',' expr)*)? ')'                 #concatOp

 |          OVERLAY     '(' expr (PLACING | ',') expr (FROM | ',') expr ((FOR | ',') expr)? ')' #overlayFunc
 |          POSITION    '(' expr (IN | ',') expr ')'                #positionFunc

 |          (SUBSTRING | SUBSTR)
                        '(' (
                              ( expr ',' expr (',' length=expr)? )
                            | ( expr FROM expr (FOR length=expr)? )
                            )
                        ')'                                         #subStringFunc

 |          TRIM        '(' (
                              ( expr (',' trimExpr=expr)? )
                            | (LEADING|TRAILING|BOTH)? (trimExpr=expr)? FROM expr
                            )
                        ')'                                         #trimFunc

 |          EXTRACT     '(' (SECOND|MINUTE|HOUR|DAY|WEEK|MONTH|QUARTER|YEAR)
                            FROM expr
                        ')'                                           #extractFunc
// |        TO_YMINTERVAL '(' str=STR ')'                             #toYMIntervalFunc
// |        TO_DTINTERVAL '(' str=STR ')'                             #toDTIntervalFunc
// |         TO_TIMESTAMP '(' time_unit ',' expr ')'                  #toTimestampFunc
// |         SINCE_EPOCH  '(' time_unit ',' expr ')'                  #sinceEpochFunc
// |         TRUNCATE     '(' time_unit ',' expr ')'                  #truncateFunc

 |          COUNT       '(' ALL? '*' ')'                            #countAll
 |          COUNT       '(' (ALL | distinct=DISTINCT)? expr ')'     #countFunc
// |          RAND        '(' seed=expr? ')'                          #randFunc

// |     func=analyticFunction '(' (ALL | distinct=DISTINCT)? expr ')'
//            OVER        '('
//                            (partitionBy=PARTITION BY exprList)?
//                            (orderBy=ORDER BY orderByList)?
//                        ')'                                         #windowFunc

 |     func=functionName '(' (ALL | distinct=DISTINCT) expr ')'     #aggFunc
 |     func=functionName '(' (expr (',' expr)*)? ')'                #scalarFunc

 |                      '(' expr ')'                                #parenthetic
 |                      '(' query ')'                               #subqueryExpr

 | expr IS NOT? NULL                                            #isNullOp

// NB: optional clause messes up the precedence somehow (ANTLR4 bug?) [rm]
 | expr    NOT? LIKE expr /*(ESCAPE expr)?*/                    #likeOp
 | expr    NOT? LIKE expr ESCAPE expr                           #likeEscapeOp

// NB: the optional NOT here is never recognized (parses as a NOT op instead) [rm]
 |         NOT? EXISTS ( '(' query ')' )                        #subqueryExistsOp

 | expr    NOT? IN     ( '(' query ')' )                        #subqueryInOp
 | expr    NOT? IN     ( '(' (expr (',' expr)*)? ')' )          #inOp
 | expr    NOT? BETWEEN expr AND expr                           #betweenOp
 |          op=NOT expr                                             #notOp
//
// The semantic predicate below is a hack to fix a precedence problem (ANTLR4 bug?)
// where the AND of a BETWEEN operator is incorrectly interpreted as an AND operator.
// The predicate prevents AND from being recognized as an expression operator when
// it occurs in the context of a BETWEEN operator.  [rm]
//
 | expr {!(_ctx.parent instanceof BetweenOpContext)}? op=AND expr   #andOp
//
// | expr     op=AND expr                                           #andOp
//
 | expr     op=OR  expr                                             #orOp
 ;


// particles  ------------------------------------------------------------------
variableName      : CURRENT_CATALOG | CURRENT_SCHEMA | CURRENT_TIMESTAMP | CURRENT_USER;
columnRef         : ( (((catalogName '.')? (schemaName '.'))? (tableName '.')) )? columnName ;
columnWildcard    : ( (((catalogName '.')? (schemaName '.'))? (tableName '.')) )? '*' ;
//fieldName         : id = identifier ;
className         : ( (qualifier '.')* id = identifier) ;
functionName      : id = identifier ;
methodName        : id = identifier ;

catalogName       : id = identifier ;
schemaName        : id = identifier ;
tableName         : id = identifier ;
tableAlias        : id = identifier ;
columnName        : id = identifier ;
columnAlias       : id = identifier ;
collationName     : id = identifier ;
indexName         : id = identifier ;
propertyName      : id = identifier ;
hintName          : id = identifier ;

propertyValue     : (lit = simpleLiteral | id = identifier) ;
hintValue         : (lit = simpleLiteral | id = identifier) ;

identifier        : IDENTIFIER | nonReservedWord ;
qualifier         : id = identifier ;

precision         : integerLiteral ;
scale             : integerLiteral ;
size              : integerLiteral ;

literal           : NULL
                  | booleanLiteral
                  | integerLiteral
                  | decimalLiteral
                  | floatLiteral
                  | dateLiteral
                  | stringLiteral
                  | timestampLiteral
                  | intervalLiteral
                  ;

simpleLiteral     : NULL
                  | booleanLiteral
                  | integerLiteral
                  | decimalLiteral
                  | floatLiteral
                  | stringLiteral
                  ;

booleanLiteral    : TRUE | FALSE ;
integerLiteral    : INTEGER_LITERAL ;
decimalLiteral    : DECIMAL_LITERAL ;
floatLiteral      : FLOAT_LITERAL ;
dateLiteral       : DATE str = STRING_LITERAL ;
timestampLiteral  : TIMESTAMP str=STRING_LITERAL ;
stringLiteral     : STRING_LITERAL ;
intervalLiteral   : ( sql92CalendarInterval | sql92TimeInterval | looseInterval ) ;

looseInterval     : INTERVAL str=expr ;

sql92CalendarInterval :
                  INTERVAL str=STRING_LITERAL
                  (
                      ( year=YEAR   ('(' precision ')')? )
                    | ( year=YEAR   ('(' precision ')')? TO month=MONTH)
                    | ( month=MONTH ('(' precision ')')? )
                  ) ;

sql92TimeInterval :
                  INTERVAL str=STRING_LITERAL
                  (
                      ( day=DAY     ('(' precision ')')? )
                    | ( day=DAY     ('(' precision ')')? TO (hour=HOUR | min=MINUTE | sec=SECOND ('(' scale ')')? ))
                    | ( hour=HOUR   ('(' precision ')')? TO (min=MINUTE | sec=SECOND ('(' scale ')')? ))
                    | ( min=MINUTE  ('(' precision ')')? TO (sec=SECOND ('(' scale ')')? ))
                    | ( hour=HOUR   ('(' precision ')')? )
                    | ( min=MINUTE  ('(' precision ')')? )
                    | ( sec=SECOND  ('(' precision (',' scale)?')')? )
                  ) ;


// lexer ------------------------------------------------------

nonReservedWord   :
                    ASC | BIT | BITS | BLOCK | BLOOM | BOTH | BTREE | BY | BYTE | BYTES
                  | CATALOG | CHARS | COALESCE | CONCAT | CORRESPONDING | COUNT | CREATE
                  | CURRENT_CATALOG | CURRENT_SCHEMA | CURRENT_TIMESTAMP | CURRENT_USER
                  | DATA | DATE | DAY | DAYS | DECODE | DELETE | DESC | DETAIL
                  | DIGITS | DROP
                  | ESCAPE | EXCEPT | EXPLAIN | EXTRACT | FEDERATION
                  | HASH | HOUR | HOURS | IFNULL | INDEX | INSERT | INTO | IS
                  | JOIN | KEY | LEFT | LOOP | MINUTE | MINUTES | MONOTONIC | MONTH | MONTHS
                  | NO | NODE | NULLIF | OFFSET | OUTER | OVERLAY
                  | PARTITION | POSITION | QUARTER| QUARTERS | REFERENCE | RIGHT
                  | SCHEMA | SECOND | SECONDS | SET | SKIPLIST | SUBSTR | SUBSTRING
                  | TABLE | TREE | TYPE | UNION | UNIQUE | UPDATE | USING | VALUE | VALUES | VARYING
                  | WEEK | WEEKS | YEAR | YEARS
                  | STATS | STATISTICS
                  | INTEGER | DECIMAL | FLOAT | TIMESTAMP | CHARACTER | CHAR | CHARS
                  | BOOLEAN | BINARY | VARBINARY
                  ;

ALL                   : A L L;
AND                   : A N D;
ANY                   : A N Y;
AS                    : A S;
ASC                   : A S C;
BETWEEN               : B E T W E E N;
BOTH                  : B O T H;
BY                    : B Y;
CATALOG               : C A T A L O G;
COLLATE               : C O L L A T E;
CORRESPONDING         : C O R R E S P O N D I N G;
CREATE                : C R E A T E;
CROSS                 : C R O S S;
CURRENT_CATALOG       : C U R R E N T '_' C A T A L O G;
CURRENT_SCHEMA        : C U R R E N T '_' S C H E M A;
CURRENT_TIMESTAMP     : C U R R E N T '_' T I M E S T A M P;
CURRENT_USER          : C U R R E N T '_' U S E R;
DATA                  : D A T A;
DATE                  : D A T E;
DELETE                : D E L E T E;
DESC                  : D E S C;
DETAIL                : D E T A I L;
DISTINCT              : D I S T I N C T;
DROP                  : D R O P;
ESCAPE                : E S C A P E;
EXCEPT                : E X C E P T;
EXPLAIN               : E X P L A I N;
FALSE                 : F A L S E;
FEDERATION            : F E D E R A T I O N;
FOR                   : F O R;
FULL                  : F U L L;
FROM                  : F R O M;
GROUP                 : G R O U P;
HAVING                : H A V I N G;
IN                    : I N;
INDEX                 : I N D E X;
INNER                 : I N N E R;
INSERT                : I N S E R T;
INTERSECT             : I N T E R S E C T;
INTO                  : I N T O;
IS                    : I S;
JOIN                  : J O I N;
KEY                   : K E Y;
LEADING               : L E A D I N G;
LEFT                  : L E F T;
LIKE                  : L I K E;
LIMIT                 : L I M I T;
MONOTONIC             : M O N O T O N I C;
NO                    : N O;
NODE                  : N O D E;
NOT                   : N O T;
NOW                   : N O W;
NULL                  : N U L L;
OFFSET                : O F F S E T;
ON                    : O N;
OR                    : O R;
ORDER                 : O R D E R;
OUTER                 : O U T E R;
OVER                  : O V E R;
OVERLAY               : O V E R L A Y;
PARTITION             : P A R T I T I O N;
PLACING               : P L A C I N G;
PRIMARY               : P R I M A R Y;
REFERENCE             : R E F E R E N C E;
RIGHT                 : R I G H T;
SCHEMA                : S C H E M A;
SELECT                : S E L E C T;
SET                   : S E T;
STATS                 : S T A T S;
STATISTICS            : S T A T I S T I C S;
TABLE                 : T A B L E;
TO                    : T O;
TOP                   : T O P;
TRAILING              : T R A I L I N G;
TRUNCATE              : T R U N C A T E;
TRUE                  : T R U E;
TYPE                  : T Y P E;
UNION                 : U N I O N;
UNIQUE                : U N I Q U E;
UPDATE                : U P D A T E;
USING                 : U S I N G;
VALUES                : V A L U E S ;
WHERE                 : W H E R E;
WITH                  : W I T H;

BOOLEAN               : B O O L E A N;
INTEGER               : I N T E G E R;
DECIMAL               : D E C I M A L;
FLOAT                 : F L O A T;
TIMESTAMP             : T I M E S T A M P;
INTERVAL              : I N T E R V A L;
CHAR                  : C H A R;
CHARACTER             : C H A R A C T E R;
VARCHAR               : V A R C H A R;
TEXT                  : T E X T;
BINARY                : B I N A R Y;
VARBINARY             : V A R B I N A R Y;
BLOB                  : B L O B;
VARYING               : V A R Y I N G;
BIT                   : B I T;
BITS                  : B I T S;
BYTE                  : B Y T E;
BYTES                 : B Y T E S;
CHARS                 : C H A R S;
//DIGIT                 : D I G I T ;
DIGITS                : D I G I T S ;

CASE                  : C A S E;
CAST                  : C A S T;
COALESCE              : C O A L E S C E;
CONCAT                : C O N C A T;
COUNT                 : C O U N T;
DECODE                : D E C O D E;
EXTRACT               : E X T R A C T;
EXISTS                : E X I S T S;
IF                    : I F;
IFNULL                : I F N U L L;
NULLIF                : N U L L I F;
POSITION              : P O S I T I O N;
SUBSTR                : S U B S T R;
SUBSTRING             : S U B S T R I N G;
TRIM                  : T R I M;
TYPEOF                : T Y P E O F;

WHEN                  : W H E N;
THEN                  : T H E N;
ELSE                  : E L S E;
END                   : E N D;

DAY                   : D A Y;
DAYS                  : D A Y S;
HOUR                  : H O U R;
HOURS                 : H O U R S;
MINUTE                : M I N U T E;
MINUTES               : M I N U T E S;
MONTH                 : M O N T H;
MONTHS                : M O N T H S;
QUARTER               : Q U A R T E R;
QUARTERS              : Q U A R T E R S;
SECOND                : S E C O N D;
SECONDS               : S E C O N D S;
WEEK                  : W E E K;
WEEKS                 : W E E K S;
YEAR                  : Y E A R;
YEARS                 : Y E A R S;

BLOCK                 : B L O C K;
BLOOM                 : B L O O M;
BTREE                 : B T R E E;
HASH                  : H A S H;
LOOP                  : L O O P;
SKIPLIST              : S K I P L I S T;
TREE                  : T R E E;
VALUE                 : V A L U E;

fragment A            : 'a' | 'A';
fragment B            : 'b' | 'B';
fragment C            : 'c' | 'C';
fragment D            : 'd' | 'D';
fragment E            : 'e' | 'E';
fragment F            : 'f' | 'F';
fragment G            : 'g' | 'G';
fragment H            : 'h' | 'H';
fragment I            : 'i' | 'I';
fragment J            : 'j' | 'J';
fragment K            : 'k' | 'K';
fragment L            : 'l' | 'L';
fragment M            : 'm' | 'M';
fragment N            : 'n' | 'N';
fragment O            : 'o' | 'O';
fragment P            : 'p' | 'P';
fragment Q            : 'q' | 'Q';
fragment R            : 'r' | 'R';
fragment S            : 's' | 'S';
fragment T            : 't' | 'T';
fragment U            : 'u' | 'U';
fragment V            : 'v' | 'V';
fragment W            : 'w' | 'W';
fragment X            : 'x' | 'X';
fragment Y            : 'y' | 'Y';
fragment Z            : 'z' | 'Z';

fragment LETTER       : 'a' .. 'z' | 'A' .. 'Z' ;
fragment DIGIT        : '0' .. '9' ;

fragment DUBLSINGL    : '\'\'' ;
fragment DUBLDUBL     : '""' ;


IDENTIFIER            : PLAIN_IDENTIFIER | BAKTIKD_IDENTIFIER | DUBLQUOTED_IDENTIFIER ;
PLAIN_IDENTIFIER      : LETTER (LETTER | DIGIT | '_')* ;
BAKTIKD_IDENTIFIER    : '`' (~[`\r\n])+ '`';
DUBLQUOTED_IDENTIFIER : { isDublQuoteId }?  ( '"' (DUBLDUBL | ~["\r\n])+ '"' );

STRING_LITERAL        : SINGLQUOTED_STRING | DUBLQUOTED_STRING ;
SINGLQUOTED_STRING    : '\'' (DUBLSINGL | (~'\''))* '\'' ;
DUBLQUOTED_STRING     : '"' (DUBLDUBL | (~'"'))* '"' ;    // NB: not matched if { isDublQuotedId }

FLOAT_LITERAL         : (DECIMAL_LITERAL | DIGIT+) ('e' | 'E') ('+' | '-')? DIGIT+ ;
DECIMAL_LITERAL       : (DIGIT+ '.' DIGIT*) | '.' DIGIT+ ;
INTEGER_LITERAL       : DIGIT+ ;

HINT_START            : '/*+' ;
HINT_END              : '*/' ;

SLASH_COMMENT         : '//' .*? '\n'         -> channel(HIDDEN) ;
SQL_COMMENT           : '--' .*? '\n'         -> channel(HIDDEN) ;
BLOCK_COMMENT         : '/*' ~('+') .*? '*/'  -> channel(HIDDEN) ;   // match block comments but not SQL hints

WHITESPACE            : [ \t\n\r]+            -> channel(HIDDEN) ;
