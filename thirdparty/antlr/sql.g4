// Copyright 2014 Boray Data Co. Ltd.  All rights reserved.

grammar sql ;

@lexer::members {
  public: bool isDublQuoteId = true;
}

// statement / query -----------------------------------------------------------
statement         : (ANTLR ':')? statementChoice ';' ;
statementChoice   :  queryStatement | explainStatement ;
//statementChoice   :  queryStatement | explainStatement | statisticsStatement ;

queryStatement    : query ;
explainStatement  : (queryHint)* EXPLAIN  explainOptions* statementChoice ;
explainOptions    : explainStageList |
                    explainFormat    |
                    WITH_NAMES       |
                    WITH_TYPES;
explainStageList  : STAGE explainRange (',' explainRange)* ;
explainRange      : begin=explainRangeChoice ( (TO | '-') end=explainRangeChoice )? ;
explainRangeChoice: all=('*' | ALL) | num=INTEGER_LITERAL | id=identifier ;
explainFormat     : FORMAT (PRETTY | ROW);
statisticsStatement : (queryHint)* (STATS | STATISTICS) statementChoice ;

query             : (queryHint)*
                    ( selectQuery
                    | setQuery
//                    | insertQuery
//                    | deleteQuery
//                    | updateQuery
//                    | createQuery
//                    | dropQuery
//                    | truncateQuery
                    ) ;

queryHint         : HINT_START (hint)? (',' hint)* HINT_END ;

// Note: Some hint values must be processed as expressions, so we explicitly cater for this.
//       Otherwise unqualified columnRefs (e.g. n_nationkey) get treated as simple strings
//       and then never get the processing that expressions do (resolving, qualifying, etc).
hint              :   name = hintName
                  | ( exprName = exprHint '='  expr )
                  | ( exprName = exprHint '='? '(' expr (',' expr)* ')' )
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
                  | setExpr op = (UNION|EXCEPT|INTERSECT) (DISTINCT | all = ALL)? (corrClause)? setExpr
                  | '(' setExpr ')'
                  ;

corrClause        : corr = CORRESPONDING (BY '(' corrList ')')? ;

withQuery         : (RECURSIVE)? name = tableName
                    colAliases?
                    AS '(' query ')' ;

tableExpr         : ref = tableReference (AS? alias = tableAlias)?                      #fetch
                  | tableExpr (queryHint)* (',' | (CROSS JOIN)) tableExpr               #crossJoin
                  | tableExpr (queryHint)* NATURAL (type = joinType?) JOIN tableExpr    #naturalJoin
                  | tableExpr
                    (queryHint)* type = extendedJoinType?
                    JOIN
                    tableExpr ON joinPredicate                                          #joinOn
                  | tableExpr
                    (queryHint)* type = extendedJoinType?
                    JOIN
                    tableExpr
                    USING '(' joinUsingList ')'                                         #joinUsing
                  | '(' query ')'       (AS? as = tableAlias colAliases?)?              #subquery
                  | '(' valuesExpr ')'  (AS? as = tableAlias colAliases?)?              #valuesSubquery
                  ;

tableReference  : ( (catalog = catalogName '.')? schema = schemaName '.')? table = tableName ;

joinType          : INNER |
                    (outer = (LEFT | RIGHT | FULL) OUTER?);

extendedJoinType  : joinType |
                    SEMI |
                    ANTI;

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
                        (notNullable = NOT? nullable = SQL_NULL)
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

//dataType          : (sqlDataType (USING className)?) | className
dataType          : sqlDataType
                  ;

sqlDataType       : BOOLEAN
                  | ( INTEGER    ( '(' precision (BITS | BYTES | DIGITS)? ')')? )
                  | ( DECIMAL    ( '(' precision DIGITS? (',' scale DIGITS?)? ')')? )
                  | ( FLOAT      ( '(' precision (BITS | BYTES | DIGITS)? ')')? )
                  | DATE
                  | TIME
                  | TIMESTAMP
                  | intervalType
                  | fixedLenChar
                  | varyingLenChar
                  | (BINARY     ( '(' size (BYTE | BYTES)? ')' ) )
                  | (VARBINARY  ( '(' size (BYTE | BYTES)? ')' )? )
                  | BLOB
                  ;

intervalType      : INTERVAL (yearMonthIntervalType | dayTimeIntervalType) ;
yearMonthIntervalType :   ( year=YEAR   ('(' precision ')')? )
                        | ( year=YEAR   ('(' precision ')')? TO month=MONTH)
                        | ( month=MONTH ('(' precision ')')? )
                        ;
dayTimeIntervalType   :   ( day=DAY     ('(' precision ')')? )
                        | ( day=DAY     ('(' precision ')')? TO (hour=HOUR | min=MINUTE | sec=SECOND ('(' scale ')')? ))
                        | ( hour=HOUR   ('(' precision ')')? TO (min=MINUTE | sec=SECOND ('(' scale ')')? ))
                        | ( min=MINUTE  ('(' precision ')')? TO (sec=SECOND ('(' scale ')')? ))
                        | ( hour=HOUR   ('(' precision ')')? )
                        | ( min=MINUTE  ('(' precision ')')? )
                        | ( sec=SECOND  ('(' precision (',' scale)?')')? )
                        ;

fixedLenChar      : (CHAR | CHARACTER)
                    '(' size (BYTE | BYTES | CHAR | CHARS)? ')'
                    (COLLATE collate = collationName)? ;
varyingLenChar    : (
                      ( ((CHAR | CHARACTER) VARYING?) | VARCHAR )
                      ( '(' size (BYTE | BYTES | CHAR | CHARS)? ')' )?
                    |
                      TEXT
                    ) (COLLATE collate = collationName)?;

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

colAliases        : '(' colAliasList ')' ;
colAliasList      : colAliasListItem (',' colAliasListItem)* ;
colAliasListItem  : id = identifier ;

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
                        (ELSE elseExpr=expr)?
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
                              ( src=expr ','  start=expr (',' length=expr)? )
                            | ( src=expr FROM start=expr (FOR length=expr)? )
                            )
                        ')'                                         #subStringFunc

 |          TRIM        '(' (
                              ( expr (',' trimExpr=expr)? )
                            | (LEADING|TRAILING|BOTH)? (trimExpr=expr)? FROM expr
                            )
                        ')'                                         #trimFunc

 |          EXTRACT     '(' (SECOND|MINUTE|HOUR|DAY|MONTH|YEAR)
                            FROM expr
                        ')'                                           #extractFunc
// |        TO_YMINTERVAL '(' str=STR ')'                             #toYMIntervalFunc
// |        TO_DTINTERVAL '(' str=STR ')'                             #toDTIntervalFunc
// |         TO_TIMESTAMP '(' time_unit ',' expr ')'                  #toTimestampFunc
// |         SINCE_EPOCH  '(' time_unit ',' expr ')'                  #sinceEpochFunc
// |         TRUNCATE     '(' time_unit ',' expr ')'                  #truncateFunc

 |          COUNT       '(' ALL? '*' ')'                            #countAll
 |     func=COUNT       '(' (ALL | distinct=DISTINCT)? expr ')'     #aggFunc
 |     func=SUM         '(' (ALL | distinct=DISTINCT)? expr ')'     #aggFunc
 |     func=AVG         '(' (ALL | distinct=DISTINCT)? expr ')'     #aggFunc
 |     func=MIN         '(' (ALL | distinct=DISTINCT)? expr ')'     #aggFunc
 |     func=MAX         '(' (ALL | distinct=DISTINCT)? expr ')'     #aggFunc
// |          RAND        '(' seed=expr? ')'                          #randFunc

// |     func=analyticFunction '(' (ALL | distinct=DISTINCT)? expr ')'
//            OVER        '('
//                            (partitionBy=PARTITION BY exprList)?
//                            (orderBy=ORDER BY orderByList)?
//                        ')'                                         #windowFunc

 |     func=functionName '(' (expr (',' expr)*)? ')'                #scalarFunc

 |                      '(' expr ')'                                #parenthetic
 |                      '(' query ')'                               #subqueryExpr

 | expr IS NOT? SQL_NULL                                            #isNullOp

// NB: optional clause messes up the precedence somehow (ANTLR4 bug?) [rm]
 | expr    NOT? LIKE expr /*(ESCAPE expr)?*/                        #likeOp
 | expr    NOT? LIKE expr ESCAPE expr                               #likeEscapeOp

// NB: the optional NOT here is never recognized (parses as a NOT op instead) [rm]
 |         NOT? EXISTS ( '(' query ')' )                            #subqueryExistsOp

 | expr    NOT? IN     ( '(' query ')' )                            #subqueryInOp
 | expr    NOT? IN     ( '(' (expr (',' expr)*)? ')' )              #inOp
 | expr    NOT? BETWEEN (expr AND expr)                             #betweenOp
 |          op=NOT expr                                             #logicOp
//
// The semantic predicate below is a hack to fix a precedence problem (ANTLR4 bug?)
// where the AND of a BETWEEN operator is incorrectly interpreted as an AND operator.
// The predicate prevents AND from being recognized as an expression operator when
// it occurs in the context of a BETWEEN operator.  [rm]
 | expr {!(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent))}? op=AND expr   #logicOp
 | expr                                                                op=XOR expr   #logicOp
 | expr                                                                op=OR  expr   #logicOp
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
exprHint          : (HASHKEY) ;   // The names of hints whose values must be expressions

propertyValue     : (lit = simpleLiteral | id = identifier) ;
hintValue         : (lit = simpleLiteral | id = identifier | expression = expr) ;

identifier        : IDENTIFIER | nonReservedWord ;
qualifier         : id = identifier ;

precision         : integerLiteral ;
scale             : integerLiteral ;
size              : integerLiteral ;

literal           : SQL_NULL
                  | booleanLiteral
                  | signedIntegerLiteral
                  | signedDecimalLiteral
                  | signedFloatLiteral
                  | dateLiteral
                  | timeLiteral
                  | stringLiteral
                  | timestampLiteral
                  | intervalLiteral
                  ;

simpleLiteral     : SQL_NULL
                  | booleanLiteral
                  | signedIntegerLiteral
                  | signedDecimalLiteral
                  | signedFloatLiteral
                  | stringLiteral
                  ;

booleanLiteral    : SQL_TRUE | SQL_FALSE ;
integerLiteral    : INTEGER_LITERAL ;
signedIntegerLiteral : ('+' | neg='-')? INTEGER_LITERAL ;
decimalLiteral    : DECIMAL_LITERAL ;
signedDecimalLiteral : ('+' | neg='-')? DECIMAL_LITERAL ;
floatLiteral      : FLOAT_LITERAL ;
signedFloatLiteral : ('+' | neg='-')? FLOAT_LITERAL ;
dateLiteral       : DATE str = stringLiteral ;
timeLiteral       : TIME str = stringLiteral ;
timestampLiteral  : TIMESTAMP stringLiteral ;
stringLiteral     : STRING_LITERAL ;
intervalLiteral   : ( yearMonthInterval | dayTimeInterval /* | looseInterval */ ) ;

// looseInterval     : INTERVAL str=expr ;

yearMonthInterval :
                  INTERVAL  ('+' | neg='-')? stringLiteral
                  (
                      ( year=YEAR   ('(' precision ')')? )
                    | ( year=YEAR   ('(' precision ')')? TO month=MONTH)
                    | ( month=MONTH ('(' precision ')')? )
                  ) ;

dayTimeInterval   :
                  INTERVAL  ('+' | neg='-')? stringLiteral
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
                    ANTLR | ASC
                  | BIT | BITS | BLOCK | BLOOM | BTREE | BYTE | BYTES
                  | CATALOG | CHARS | CONCAT
                  | DATA | DAYS | DECODE | DESC | DETAIL | DIGITS
                  | EXPLAIN
                  | SQL_FALSE | FEDERATION | FORMAT
                  | HASH | HASHKEY | HOURS
                  | IF | IFNULL | INDEX
                  | KEY
                  | LIMIT | LOOP
                  | MINUTES | MONOTONIC | MONTHS
                  | NODE | NOW
                  | OFFSET
                  | PLACING | PRETTY
                  | QUARTER| QUARTERS
                  | REFERENCE
                  | SCHEMA | SECONDS | SKIPLIST | STAGE | STATS | STATISTICS
                  | STATISTICS |SUBSTR
                  | TEXT | TOP | TREE | TRUNCATE | TYPE | TYPEOF
                  | VALUE
                  | WEEK | WEEKS | WITH_NAMES | WITH_TYPES
                  | XOR
                  | YEARS
                  ;

ALL                   : A L L;
AND                   : A N D;
ANTI                  : A N T I;      // A RapidsSE reserved word so the grammar doesn't treat it as a table alias
ANTLR                 : A N T L R;
ANY                   : A N Y;
AS                    : A S;
ASC                   : A S C;
AVG                   : A V G;
BETWEEN               : B E T W E E N;
BINARY                : B I N A R Y;
BIT                   : B I T;
BITS                  : B I T S;
BLOB                  : B L O B;
BLOCK                 : B L O C K;
BLOOM                 : B L O O M;
BOTH                  : B O T H;
BOOLEAN               : B O O L E A N;
BTREE                 : B T R E E;
BY                    : B Y;
BYTE                  : B Y T E;
BYTES                 : B Y T E S;
CASE                  : C A S E;
CAST                  : C A S T;
CATALOG               : C A T A L O G;
CHAR                  : C H A R;
CHARS                 : C H A R S;
CHARACTER             : C H A R A C T E R;
COALESCE              : C O A L E S C E;
COLLATE               : C O L L A T E;
CONCAT                : C O N C A T;
CORRESPONDING         : C O R R E S P O N D I N G;
COUNT                 : C O U N T;
CREATE                : C R E A T E;
CROSS                 : C R O S S;
CURRENT_CATALOG       : C U R R E N T '_' C A T A L O G;
CURRENT_SCHEMA        : C U R R E N T '_' S C H E M A;
CURRENT_TIMESTAMP     : C U R R E N T '_' T I M E S T A M P;
CURRENT_USER          : C U R R E N T '_' U S E R;
DATA                  : D A T A;
DATE                  : D A T E;
DAY                   : D A Y;
DAYS                  : D A Y S;
DECIMAL               : D E C I M A L;
DECODE                : D E C O D E;
DELETE                : D E L E T E;
DESC                  : D E S C;
DETAIL                : D E T A I L;
DIGITS                : D I G I T S ;
DISTINCT              : D I S T I N C T;
DROP                  : D R O P;
ELSE                  : E L S E;
END                   : E N D;
ESCAPE                : E S C A P E;
EXCEPT                : E X C E P T;
EXISTS                : E X I S T S;
EXPLAIN               : E X P L A I N;
EXTRACT               : E X T R A C T;
SQL_FALSE             : F A L S E;
FEDERATION            : F E D E R A T I O N;
FLOAT                 : F L O A T;
FOR                   : F O R;
FORMAT                : F O R M A T;
FULL                  : F U L L;
FROM                  : F R O M;
GROUP                 : G R O U P;
HASH                  : H A S H;
HASHKEY               : H A S H K E Y;
HAVING                : H A V I N G;
HOUR                  : H O U R;
HOURS                 : H O U R S;
IF                    : I F;
IFNULL                : I F N U L L;
IN                    : I N;
INDEX                 : I N D E X;
INNER                 : I N N E R;
INSERT                : I N S E R T;
INTEGER               : I N T E G E R;
INTERSECT             : I N T E R S E C T;
INTERVAL              : I N T E R V A L;
INTO                  : I N T O;
IS                    : I S;
JOIN                  : J O I N;
KEY                   : K E Y;
LEADING               : L E A D I N G;
LEFT                  : L E F T;
LIKE                  : L I K E;
LIMIT                 : L I M I T;
LOOP                  : L O O P;
MAX                   : M A X;
MIN                   : M I N;
MINUTE                : M I N U T E;
MINUTES               : M I N U T E S;
MONOTONIC             : M O N O T O N I C;
MONTH                 : M O N T H;
MONTHS                : M O N T H S;
NATURAL               : N A T U R A L;
NO                    : N O;
NODE                  : N O D E;
NOT                   : N O T;
NOW                   : N O W;
SQL_NULL              : N U L L;
NULLIF                : N U L L I F;
OFFSET                : O F F S E T;
ON                    : O N;
OR                    : O R;
ORDER                 : O R D E R;
OUTER                 : O U T E R;
OVER                  : O V E R;
OVERLAY               : O V E R L A Y;
PARTITION             : P A R T I T I O N;
PLACING               : P L A C I N G;
POSITION              : P O S I T I O N;
PRETTY                : P R E T T Y;
PRIMARY               : P R I M A R Y;
QUARTER               : Q U A R T E R;
QUARTERS              : Q U A R T E R S;
RECURSIVE             : R E C U R S I V E;
REFERENCE             : R E F E R E N C E;
RIGHT                 : R I G H T;
ROW                   : R O W;
SCHEMA                : S C H E M A;
SECOND                : S E C O N D;
SECONDS               : S E C O N D S;
SELECT                : S E L E C T;
SEMI                  : S E M I;      // A RapidsSE reserved word so the grammar doesn't treat it as a table alias
SET                   : S E T;
SKIPLIST              : S K I P L I S T;
STAGE                 : S T A G E;
STATS                 : S T A T S;
STATISTICS            : S T A T I S T I C S;
SUBSTR                : S U B S T R;
SUBSTRING             : S U B S T R I N G;
SUM                   : S U M;
TABLE                 : T A B L E;
TEXT                  : T E X T;
THEN                  : T H E N;
TIME                  : T I M E;
TIMESTAMP             : T I M E S T A M P;
TO                    : T O;
TOP                   : T O P;
TREE                  : T R E E;
TRIM                  : T R I M;
TRAILING              : T R A I L I N G;
TRUNCATE              : T R U N C A T E;
SQL_TRUE              : T R U E;
TYPE                  : T Y P E;
TYPEOF                : T Y P E O F;
UNION                 : U N I O N;
UNIQUE                : U N I Q U E;
UPDATE                : U P D A T E;
USING                 : U S I N G;
VALUE                 : V A L U E;  // Note: SQL says this should be a reserved word, but TPC-H Q11 uses it as a column alias!
VALUES                : V A L U E S ;
VARBINARY             : V A R B I N A R Y;
VARCHAR               : V A R C H A R;
VARYING               : V A R Y I N G;
WEEK                  : W E E K;
WEEKS                 : W E E K S;
WHEN                  : W H E N;
WHERE                 : W H E R E;
WITH_NAMES            : W I T H '_' N A M E S;
WITH_TYPES            : W I T H '_' T Y P E S;
WITH                  : W I T H;
XOR                   : X O R;
YEAR                  : Y E A R;
YEARS                 : Y E A R S;



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
