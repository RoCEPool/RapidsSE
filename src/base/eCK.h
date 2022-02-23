/*============================================================================*/
/* eCK.h -- Card Kind                                                         */
/*============================================================================*/
#pragma once

enum eCK // card kind
{
  /* */ CK_NONE=0,
    
  /* */ CK_AGGREG,
  /* */ CK_ANDPRED,
  /* */ CK_AS,
  
  /* */ CK_BOOLPRED,
  
  /* */ CK_CALCPRED,
  /* */ CK_CANDIDATEINDEX,
  /* */ CK_COMBINE,
  /* */ CK_COMPAREPRED,
  /* */ CK_CORRSUBQUERY,
  /* */ CK_CREATETABLE,
  
  /* */ CK_DEBUG,
  /* */ CK_DROPTABLE,
  
  /* */ CK_EBOUND,
  /* */ CK_EXISTS,
  
  /* */ CK_FILTER,

  /* */ CK_GBYLIST,

  /* */ CK_HASH,
  /* */ CK_HASHCAST,
  /* */ CK_HAVING,
  
  /* */ CK_INDEX,
  /* */ CK_INDEXJOIN,
  /* */ CK_INDEXPRED,
  /* */ CK_INDEXSCAN,
  
  /* */ CK_LBOUND,
  /* */ CK_LIMIT,
  
  /* */ CK_MAINSUBQUERY,
  
  /* */ CK_NODECAST,
  /* */ CK_NONCSUBQUERY,
  
  /* */ CK_ON,
  /* */ CK_ORDAGG,
  /* */ CK_ORIGINCAST,
  /* */ CK_ORPRED,
  
  /* */ CK_PROJECT,
  
  /* */ CK_RBOUND,
  
  /* */ CK_SCAN,
  /* */ CK_SEND,
  /* */ CK_SORT,
  /* */ CK_STAGE,
  /* */ CK_STREAMDEF,
  
  /* */ CK_TABLE,
  /* */ CK_TERM,
  /* */ CK_TERMLIST,
  /* */ CK_TYPE,
  /* */ CK_TYPEDEXPR,

  /* */ CK_UBOUND,
  
  /* */ CK_ENUMS
};

static Stz* aCK[]
{
  /* */ "NONE"_Stz,
  
  /* */ "CK_AGGREG"_Stz,          // CK_AGGREG
  /* */ "CK_ANDPRED"_Stz,         // CK_ANDPRED
  /* */ "CK_AS"_Stz,              // CK_AS
  
  /* */ "CK_BOOLPRED"_Stz,        // CK_BOOLPRED

  /* */ "CK_CALCPRED"_Stz,        // CK_CALCPRED
  /* */ "CK_CANDIDATEINDEX"_Stz,  // CK_CANDIDATEINDEX
  /* */ "CK_COMBINE"_Stz,         // CK_COMBINE
  /* */ "CK_COMPAREPRED"_Stz,     // CK_COMPAREPRED
  /* */ "CK_CORRSUBQUERY"_Stz,    // CK_CORRSUBQUERY
  /* */ "CK_CREATETABLE"_Stz,     // CK_CREATETABLE
  
  /* */ "CK_DEBUG"_Stz,           // CK_DEBUG
  /* */ "CK_DROPTABLE"_Stz,       // CK_DROPTABLE
  
  /* */ "CK_EBOUND"_Stz,          // CK_EBOUND
  /* */ "CK_EXISTS"_Stz,          // CK_EXISTS
  
  /* */ "CK_FILTER"_Stz,          // CK_FILTER

  /* */ "CK_GBYLIST"_Stz,         // CK_GBYLIST
  
  /* */ "CK_HASH"_Stz,            // CK_HASH
  /* */ "CK_HASHCAST"_Stz,        // CK_HASHCAST
  /* */ "CK_HAVING"_Stz,          // CK_HAVING
  
  /* */ "CK_INDEX"_Stz,           // CK_INDEX
  /* */ "CK_INDEXJOIN"_Stz,       // CK_INDEXJOIN
  /* */ "CK_INDEXPRED"_Stz,       // CK_INDEXPRED
  /* */ "CK_INDEXSCAN"_Stz,       // CK_INDEXSCAN
  
  /* */ "CK_LBOUND"_Stz,          // CK_LBOUND
  /* */ "CK_LIMIT"_Stz,           // CK_LIMIT
  
  /* */ "CK_MAINSUBQUERY"_Stz,    // CK_MAINSUBQUERY
  
  /* */ "CK_NODECAST"_Stz,        // CK_NODECAST
  /* */ "CK_NONCSUBQUERY"_Stz,    // CK_NONCSUBQUERY
  
  /* */ "CK_ON"_Stz,              // CK_ON
  /* */ "CK_ORDAGG"_Stz,          // CK_ORDAGG
  /* */ "CK_ORIGINCAST"_Stz,      // CK_ORIGINCAST
  /* */ "CK_ORPRED"_Stz,          // CK_ORPRED
  
  /* */ "CK_PROJECT"_Stz,         // CK_PROJECT
  
  /* */ "CK_RBOUND"_Stz,          // CK_RBOUND

  /* */ "CK_SCAN"_Stz,            // CK_SEND
  /* */ "CK_SEND"_Stz,            // CK_SCAN
  /* */ "CK_SORT"_Stz,            // CK_SORT
  /* */ "CK_STAGE"_Stz,           // CK_STAGE
  /* */ "CK_STREAMDEF"_Stz,       // CK_STREAMDEF
  
  /* */ "CK_TABLE"_Stz,           // CK_TABLE,
  /* */ "CK_TERM"_Stz,            // CK_TERM,
  /* */ "CK_TERMLIST"_Stz,        // CK_TERMLIST,
  /* */ "CK_TYPE"_Stz,            // CK_TYPE,
  /* */ "CK_TYPEDEXPR"_Stz,       // CK_TYPEDEXPR

  /* */ "CK_UBOUND"_Stz,          // CK_UBOUND
  
  /* */ "ENUMS"_Stz
};

static Stz* aCK_short[]
{
  /* */ "NONE"_Stz,
    
  /* */ "aggreg"_Stz,          // CK_AGGREG
  /* */ "andPred"_Stz,         // CK_ANDPRED
  /* */ "as"_Stz,              // CK_AS
  
  /* */ "boolPred"_Stz,        // CK_BOOLPRED
  
  /* */ "calcPred"_Stz,        // CK_CALCPRED
  /* */ "candidateIndex"_Stz,  // CK_CANDIDATEINDEX
  /* */ "combine"_Stz,         // CK_COMBINE
  /* */ "comparePred"_Stz,     // CK_COMPAREPRED
  /* */ "corrSubquery"_Stz,    // CK_CORRSUBQUERY
  /* */ "createTable"_Stz,     // CK_CREATETABLE
  
  /* */ "debug"_Stz,           // CK_DEBUG
  /* */ "dropTable"_Stz,       // CK_DROPTABLE
  
  /* */ "ebound"_Stz,          // CK_EBOUND
  /* */ "exists"_Stz,          // CK_EXISTS
  
  /* */ "filter"_Stz,          // CK_FILTER

  /* */ "gbyList"_Stz,         // CK_GBYLIST

  /* */ "hash"_Stz,            // CK_HASH
  /* */ "hashCast"_Stz,        // CK_HASHCAST
  /* */ "having"_Stz,          // CK_HAVING
  
  /* */ "index"_Stz,           // CK_INDEX
  /* */ "indexJoin"_Stz,       // CK_INDEXJOIN
  /* */ "indexPred"_Stz,       // CK_INDEXPRED
  /* */ "indexScan"_Stz,       // CK_INDEXSCAN
  
  /* */ "lbound"_Stz,          // CK_LBOUND
  /* */ "limit"_Stz,           // CK_LIMIT
  
  /* */ "mainSubquery"_Stz,    // CK_MAINSUBQUERY
  
  /* */ "nodeCast"_Stz,        // CK_NODECAST
  /* */ "noncSubquery"_Stz,    // CK_NONCSUBQUERY
  
  /* */ "on"_Stz,              // CK_ON
  /* */ "ordAgg"_Stz,          // CK_ORDAGG
  /* */ "originCast"_Stz,      // CK_ORIGINCAST
  /* */ "orPred"_Stz,          // CK_ORPRED
  
  /* */ "project"_Stz,         // CK_PROJECT
  
  /* */ "rbound"_Stz,          // CK_RBOUND
  
  /* */ "scan"_Stz,            // CK_SEND
  /* */ "send"_Stz,            // CK_SCAN
  /* */ "sort"_Stz,            // CK_SORT
  /* */ "stage"_Stz,           // CK_STAGE
  /* */ "streamDef"_Stz,       // CK_STREAMDEF
  
  /* */ "table"_Stz,           // CK_TABLE,
  /* */ "term"_Stz,            // CK_TERM,
  /* */ "termList"_Stz,        // CK_TERMLIST,
  /* */ "type"_Stz,            // CK_TYPE,
  /* */ "typedExpr"_Stz,       // CK_TYPEDEXPR

  /* */ "ubound"_Stz,          // CK_UBOUND
  
  /* */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
