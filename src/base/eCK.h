/*============================================================================*/
/* eCK.h -- Card Kind                                                         */
/*============================================================================*/
#pragma once

enum eCK // card kind
{
  /* */ CK_NONE=0,
  
  /* */ CK_CARD,
  
  /* */ CK_AGGREG,
  /* */ CK_AGGREGPRED,
  /* */ CK_ANDPRED,
  /* */ CK_AS,
  /* */ CK_ATOM,
  /* */ CK_ATOMLIST,
  
  /* */ CK_BY,
  /* */ CK_BYLIST,
  /* */ CK_BYPRED,
  
  /* */ CK_COMBINE,
  /* */ CK_COMPAREPRED,
  /* */ CK_CORRSUBQUERY,
  /* */ CK_CREATETABLE,
  
  /* */ CK_DEBUG,
  /* */ CK_DROPTABLE,
  
  /* */ CK_EXISTS,
  /* */ CK_EXPR,
  /* */ CK_EXPRLIST,
  /* */ CK_FILTER,
  /* */ CK_FILTERLIST,
  
  /* */ CK_HASH,
  /* */ CK_HASHCAST,
  /* */ CK_HAVING,
  /* */ CK_HAVINGLIST,
  
  /* */ CK_INDEX,
  /* */ CK_INDEXJOIN,
  /* */ CK_INDEXJOINLIST,
  /* */ CK_INDEXPRED,
  /* */ CK_INDEXSCAN,
  /* */ CK_INDEXSCANLIST,
  
  /* */ CK_LIMIT,
  
  /* */ CK_MAINSUBQUERY,
  
  /* */ CK_NODECAST,
  /* */ CK_NONCSUBQUERY,
  
  /* */ CK_ON,
  /* */ CK_ONLIST,
  /* */ CK_ORDAGG,
  /* */ CK_ORIGINCAST,
  /* */ CK_ORPRED,
  
  /* */ CK_PREDICATE,
  /* */ CK_PROJECT,
  /* */ CK_PROJECTION,
  /* */ CK_PROJECTPRED,
  /* */ CK_PROPERTIES,
  /* */ CK_PROPERTY,
  
  /* */ CK_SCAN,
  /* */ CK_SEND,
  /* */ CK_SORT,
  /* */ CK_SORTLIST,
  /* */ CK_STAGE,
  /* */ CK_STREAMDEF,
  
  /* */ CK_TABLE,
  /* */ CK_TABLELIST,
  /* */ CK_TERM,
  /* */ CK_TERMLIST,
  /* */ CK_TYPE,

  /* */ CK_ENUMS
};

static Stz* aCK[]
{
  /* */ "NONE"_Stz,
  
  /* */ "CK_CARD"_Stz,            // CK_CARD
  
  /* */ "CK_AGGREG"_Stz,          // CK_AGGREG
  /* */ "CK_AGGREGPRED"_Stz,      // CK_AGGREGPRED
  /* */ "CK_ANDPRED"_Stz,         // CK_ANDPRED
  /* */ "CK_AS"_Stz,              // CK_AS
  /* */ "CK_ATOM"_Stz,            // CK_ATOM
  /* */ "CK_ATOMLIST"_Stz,        // CK_ATOMLIST
  
  /* */ "CK_BY"_Stz,              // CK_BY
  /* */ "CK_BYLIST"_Stz,          // CK_BYLIST
  /* */ "CK_BYPRED"_Stz,          // CK_BYPRED
  
  /* */ "CK_COMBINE"_Stz,         // CK_COMBINE
  /* */ "CK_COMPAREPRED"_Stz,     // CK_COMPAREPRED
  /* */ "CK_CORRSUBQUERY"_Stz,    // CK_CORRSUBQUERY
  /* */ "CK_CREATETABLE"_Stz,     // CK_CREATETABLE
  
  /* */ "CK_DEBUG"_Stz,           // CK_DEBUG
  /* */ "CK_DROPTABLE"_Stz,       // CK_DROPTABLE
  
  /* */ "CK_EXISTS"_Stz,          // CK_EXISTS
  /* */ "CK_EXPR"_Stz,            // CK_EXPR
  /* */ "CK_EXPRLIST"_Stz,        // CK_EXPRLIST
  
  /* */ "CK_FILTER"_Stz,          // CK_FILTER
  /* */ "CK_FILTERLIST"_Stz,      // CK_FILTERLIST
  
  /* */ "CK_HASH"_Stz,            // CK_HASH
  /* */ "CK_HASHCAST"_Stz,        // CK_HASHCAST
  /* */ "CK_HAVING"_Stz,          // CK_HAVING
  /* */ "CK_HAVINGLIST"_Stz,      // CK_HAVINGLIST
  
  /* */ "CK_INDEX"_Stz,           // CK_INDEX
  /* */ "CK_INDEXJOIN"_Stz,       // CK_INDEXJOIN
  /* */ "CK_INDEXJOINLIST"_Stz,   // CK_INDEXJOINLIST
  /* */ "CK_INDEXPRED"_Stz,       // CK_INDEXPRED
  /* */ "CK_INDEXSCAN"_Stz,       // CK_INDEXSCAN
  /* */ "CK_INDEXSCANLIST"_Stz,   // CK_INDEXSCANLIST
  
  /* */ "CK_LIMIT"_Stz,           // CK_LIMIT
  
  /* */ "CK_MAINSUBQUERY"_Stz,    // CK_MAINSUBQUERY
  
  /* */ "CK_NODECAST"_Stz,        // CK_NODECAST
  /* */ "CK_NONCSUBQUERY"_Stz,    // CK_NONCSUBQUERY
  
  /* */ "CK_ON"_Stz,              // CK_ON
  /* */ "CK_ONLIST"_Stz,          // CK_ONLIST
  /* */ "CK_ORDAGG"_Stz,          // CK_ORDAGG
  /* */ "CK_ORIGINCAST"_Stz,      // CK_ORIGINCAST
  /* */ "CK_ORPRED"_Stz,          // CK_ORPRED
  
  /* */ "CK_PREDICATE"_Stz,       // CK_PREDICATE
  /* */ "CK_PROJECT"_Stz,         // CK_PROJECT
  /* */ "CK_PROJECTION"_Stz,      // CK_PROJECTION
  /* */ "CK_PROJECTPRED"_Stz,     // CK_PROJECTPRED
  /* */ "CK_PROPERTIES"_Stz,      // CK_PROPERTIES
  /* */ "CK_PROPERTY"_Stz,        // CK_PROPERTY
  
  /* */ "CK_SCAN"_Stz,            // CK_SEND
  /* */ "CK_SEND"_Stz,            // CK_SCAN
  /* */ "CK_SORT"_Stz,            // CK_SORT
  /* */ "CK_SORTLIST"_Stz,        // CK_SORTLIST
  /* */ "CK_STAGE"_Stz,           // CK_STAGE
  /* */ "CK_STREAMDEF"_Stz,       // CK_STREAMDEF
  
  /* */ "CK_TABLE"_Stz,           // CK_TABLE,
  /* */ "CK_TABLELIST"_Stz,       // CK_TABLELIST,
  /* */ "CK_TERM"_Stz,            // CK_TERM,
  /* */ "CK_TERMLIST"_Stz,        // CK_TERMLIST,
  /* */ "CK_TYPE"_Stz,            // CK_TYPE,

  /* */ "ENUMS"_Stz
};

static Stz* aCK_short[]
{
  /* */ "NONE"_Stz,
  
  /* */ "card"_Stz,            // CK_CARD
  
  /* */ "aggreg"_Stz,          // CK_AGGREG
  /* */ "aggregpred"_Stz,      // CK_AGGREGPRED
  /* */ "andpred"_Stz,         // CK_ANDPRED
  /* */ "as"_Stz,              // CK_AS
  /* */ "atom"_Stz,            // CK_ATOM
  /* */ "atomlist"_Stz,        // CK_ATOMLIST
  
  /* */ "by"_Stz,              // CK_BY
  /* */ "bylist"_Stz,          // CK_BYLIST
  /* */ "bypred"_Stz,          // CK_BYPRED
  
  /* */ "combine"_Stz,         // CK_COMBINE
  /* */ "comparepred"_Stz,     // CK_COMPAREPRED
  /* */ "corrsubquery"_Stz,    // CK_CORRSUBQUERY
  /* */ "createtable"_Stz,     // CK_CREATETABLE
  
  /* */ "debug"_Stz,           // CK_DEBUG
  /* */ "droptable"_Stz,       // CK_DROPTABLE
  
  /* */ "exists"_Stz,          // CK_EXISTS
  /* */ "expr"_Stz,            // CK_EXPR
  /* */ "exprlist"_Stz,        // CK_EXPRLIST
  
  /* */ "filter"_Stz,          // CK_FILTER
  /* */ "filterlist"_Stz,      // CK_FILTERLIST
  
  /* */ "hash"_Stz,            // CK_HASH
  /* */ "hashcast"_Stz,        // CK_HASHCAST
  /* */ "having"_Stz,          // CK_HAVING
  /* */ "havinglist"_Stz,      // CK_HAVINGLIST
  
  /* */ "index"_Stz,           // CK_INDEX
  /* */ "indexjoin"_Stz,       // CK_INDEXJOIN
  /* */ "indexjoinlist"_Stz,   // CK_INDEXJOINLIST
  /* */ "indexpred"_Stz,       // CK_INDEXPRED
  /* */ "indexscan"_Stz,       // CK_INDEXSCAN
  /* */ "indexscanlist"_Stz,   // CK_INDEXSCANLIST
  
  /* */ "limit"_Stz,           // CK_LIMIT
  
  /* */ "mainsubquery"_Stz,    // CK_MAINSUBQUERY
  
  /* */ "nodecast"_Stz,        // CK_NODECAST
  /* */ "noncsubquery"_Stz,    // CK_NONCSUBQUERY
  
  /* */ "on"_Stz,              // CK_ON
  /* */ "onlist"_Stz,          // CK_ONLIST
  /* */ "ordagg"_Stz,          // CK_ORDAGG
  /* */ "origincast"_Stz,      // CK_ORIGINCAST
  /* */ "orpred"_Stz,          // CK_ORPRED
  
  /* */ "predicate"_Stz,       // CK_PREDICATE
  /* */ "project"_Stz,         // CK_PROJECT
  /* */ "projection"_Stz,      // CK_PROJECTION
  /* */ "projectpred"_Stz,     // CK_PROJECTPRED
  /* */ "properties"_Stz,      // CK_PROPERTIES
  /* */ "property"_Stz,        // CK_PROPERTY
  
  /* */ "scan"_Stz,            // CK_SEND
  /* */ "send"_Stz,            // CK_SCAN
  /* */ "sort"_Stz,            // CK_SORT
  /* */ "sortlist"_Stz,        // CK_SORTLIST
  /* */ "stage"_Stz,           // CK_STAGE
  /* */ "streamdef"_Stz,       // CK_STREAMDEF
  
  /* */ "table"_Stz,           // CK_TABLE,
  /* */ "tablelist"_Stz,       // CK_TABLELIST,
  /* */ "term"_Stz,            // CK_TERM,
  /* */ "termlist"_Stz,        // CK_TERMLIST,
  /* */ "type"_Stz,            // CK_TYPE,

  /* */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
