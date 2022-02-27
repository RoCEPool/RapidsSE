/*============================================================================*/
/* eCK.h -- Card Kind                                                         */
/*============================================================================*/
#pragma once

enum eCK // card kind
{
  /* */ CK_NONE=0,
    
  /* */ CK_AGGELIST,
  /* */ CK_AGGREG,
  /* */ CK_AS,
  
  
  /* */ CK_CANDINDEX,
  /* */ CK_COMBINE,
  
  /* */ CK_DEBUG,
  
  /* */ CK_EBOUND,
  /* */ CK_EXISTS,
  /* */ CK_EXPR,
  /* */ CK_EXPRBOOL,
  /* */ CK_EXPRCALC,

  /* */ CK_FILTER,

  /* */ CK_GBYELIST,

  /* */ CK_HASH,
  /* */ CK_HAVING,
  
  /* */ CK_INDEX,
  /* */ CK_INDEXJOIN,
  /* */ CK_INDEXPRED,
  /* */ CK_INDEXSCAN,
  
  /* */ CK_LBOUND,
  /* */ CK_LIMIT,
    
  /* */ CK_ON,
  /* */ CK_ORDAGG,
  
  /* */ CK_PREDAND,
  /* */ CK_PREDCMP,
  /* */ CK_PREDOR,
  /* */ CK_PROJECT,
  /* */ CK_PROJELIST,

  /* */ CK_RBOUND,
  
  /* */ CK_SBYELIST,
  /* */ CK_SCAN,
  /* */ CK_SEND,
  /* */ CK_SORT,
  /* */ CK_SORTDIRN,
  /* */ CK_STAGE,
  /* */ CK_STREAMDEF,
  
  /* */ CK_TABLE,
  /* */ CK_TERM,
  /* */ CK_TERMSEQ,
  /* */ CK_TYPE,

  /* */ CK_UBOUND,
  
  /* */ CK_ENUMS
};

static Stz* aCK[]
{
  /* */ "NONE"_Stz,
  
  /* */ "CK_AGGELIST"_Stz,        // CK_AGGELIST
  /* */ "CK_AGGREG"_Stz,          // CK_AGGREG
  /* */ "CK_AS"_Stz,              // CK_AS
  
  /* */ "CK_CANDINDEX"_Stz,       // CK_CANDINDEX
  /* */ "CK_COMBINE"_Stz,         // CK_COMBINE
  
  /* */ "CK_DEBUG"_Stz,           // CK_DEBUG
  
  /* */ "CK_EBOUND"_Stz,          // CK_EBOUND
  /* */ "CK_EXISTS"_Stz,          // CK_EXISTS
  /* */ "CK_EXPR"_Stz,            // CK_EXPR
  /* */ "CK_EXPRBOOL"_Stz,        // CK_EXPRBOOL
  /* */ "CK_EXPRCALC"_Stz,        // CK_EXPRCALC
  
  /* */ "CK_FILTER"_Stz,          // CK_FILTER

  /* */ "CK_GBYELIST"_Stz,        // CK_GBYELIST
  
  /* */ "CK_HASH"_Stz,            // CK_HASH
  /* */ "CK_HAVING"_Stz,          // CK_HAVING
  
  /* */ "CK_INDEX"_Stz,           // CK_INDEX
  /* */ "CK_INDEXJOIN"_Stz,       // CK_INDEXJOIN
  /* */ "CK_INDEXPRED"_Stz,       // CK_INDEXPRED
  /* */ "CK_INDEXSCAN"_Stz,       // CK_INDEXSCAN
  
  /* */ "CK_LBOUND"_Stz,          // CK_LBOUND
  /* */ "CK_LIMIT"_Stz,           // CK_LIMIT
    
  /* */ "CK_ON"_Stz,              // CK_ON
  /* */ "CK_ORDAGG"_Stz,          // CK_ORDAGG
  
  /* */ "CK_PREDAND"_Stz,         // CK_PREDAND
  /* */ "CK_PREDCMP"_Stz,         // CK_PREDCMP
  /* */ "CK_PREDOR"_Stz,          // CK_PREDOR
  /* */ "CK_PROJECT"_Stz,         // CK_PROJECT
  /* */ "CK_PROJELIST"_Stz,       // CK_PROJELIST

  /* */ "CK_RBOUND"_Stz,          // CK_RBOUND

  /* */ "CK_SBYELIST"_Stz,        // CK_SBYELIST
  /* */ "CK_SCAN"_Stz,            // CK_SCAN
  /* */ "CK_SEND"_Stz,            // CK_SEND
  /* */ "CK_SORT"_Stz,            // CK_SORT
  /* */ "CK_SORTDIRN"_Stz,        // CK_SORTDIRN
  /* */ "CK_STAGE"_Stz,           // CK_STAGE
  /* */ "CK_STREAMDEF"_Stz,       // CK_STREAMDEF
  
  /* */ "CK_TABLE"_Stz,           // CK_TABLE
  /* */ "CK_TERM"_Stz,            // CK_TERM
  /* */ "CK_TERMSEQ"_Stz,         // CK_TERMSEQ
  /* */ "CK_TYPE"_Stz,            // CK_TYPE

  /* */ "CK_UBOUND"_Stz,          // CK_UBOUND
  
  /* */ "ENUMS"_Stz
};

static Stz* aCK_short[]
{
  /* */ "NONE"_Stz,
    
  /* */ "aggElist"_Stz,        // CK_AGGELIST
  /* */ "aggreg"_Stz,          // CK_AGGREG
  /* */ "as"_Stz,              // CK_AS
  
  /* */ "candIndex"_Stz,       // CK_CANDINDEX
  /* */ "combine"_Stz,         // CK_COMBINE

  /* */ "debug"_Stz,           // CK_DEBUG

  /* */ "ebound"_Stz,          // CK_EBOUND
  /* */ "exists"_Stz,          // CK_EXISTS
  /* */ "expr"_Stz,            // CK_EXPR
  /* */ "exprBool"_Stz,        // CK_EXPRBOOL
  /* */ "exprCalc"_Stz,        // CK_EXPRCALC

  /* */ "filter"_Stz,          // CK_FILTER

  /* */ "gbyElist"_Stz,        // CK_GBYELIST

  /* */ "hash"_Stz,            // CK_HASH
  /* */ "having"_Stz,          // CK_HAVING
  
  /* */ "{index}"_Stz,         // CK_INDEX
  /* */ "indexJoin"_Stz,       // CK_INDEXJOIN
  /* */ "indexPred"_Stz,       // CK_INDEXPRED
  /* */ "indexScan"_Stz,       // CK_INDEXSCAN
  
  /* */ "lbound"_Stz,          // CK_LBOUND
  /* */ "limit"_Stz,           // CK_LIMIT
    
  /* */ "on"_Stz,              // CK_ON
  /* */ "ordAgg"_Stz,          // CK_ORDAGG
  
  /* */ "predAnd"_Stz,         // CK_PREDAND
  /* */ "predCmp"_Stz,         // CK_PREDCMP
  /* */ "predOr"_Stz,          // CK_PREDOR
  /* */ "project"_Stz,         // CK_PROJECT
  /* */ "projElist"_Stz,       // CK_PROJELIST

  /* */ "rbound"_Stz,          // CK_RBOUND
  
  /* */ "sbyElist"_Stz,        // CK_SBYELIST
  /* */ "scan"_Stz,            // CK_SCAN
  /* */ "send"_Stz,            // CK_SEND
  /* */ "sort"_Stz,            // CK_SORT
  /* */ "sortDirn"_Stz,        // CK_SORTDIRN
  /* */ "stage"_Stz,           // CK_STAGE
  /* */ "{streamDef}"_Stz,     // CK_STREAMDEF
  
  /* */ "{table}"_Stz,         // CK_TABLE
  /* */ "term"_Stz,            // CK_TERM
  /* */ "termSeq"_Stz,         // CK_TERMSEQ
  /* */ "type"_Stz,            // CK_TYPE

  /* */ "ubound"_Stz,          // CK_UBOUND
  
  /* */ "ENUMS"_Stz
};

static Stz* aCK_op[]
{
  /* */ "NONE"_Stz,
    
  /* */ "(none)"_Stz,          // CK_AGGELIST
  /* */ "aggreg    2"_Stz,     // CK_AGGREG
  /* */ "(none)"_Stz,          // CK_AS
  
  /* */ "(none)"_Stz,          // CK_CANDINDEX
  /* */ "combined  0"_Stz,     // CK_COMBINE

  /* */ "(none)"_Stz,          // CK_DEBUG

  /* */ "(none)"_Stz,          // CK_EBOUND
  /* */ "(none)"_Stz,          // CK_EXISTS
  /* */ "(none)"_Stz,          // CK_EXPR
  /* */ "(none)"_Stz,          // CK_EXPRBOOL
  /* */ "(none)"_Stz,          // CK_EXPRCALC

  /* */ "filter    2"_Stz,     // CK_FILTER

  /* */ "(none)"_Stz,          // CK_GBYELIST

  /* */ "(none)"_Stz,          // CK_HASH
  /* */ "(none)"_Stz,          // CK_HAVING
  
  /* */ "(none)"_Stz,          // CK_INDEX
  /* */ "indexJoin 2"_Stz,     // CK_INDEXJOIN
  /* */ "(none)"_Stz,          // CK_INDEXPRED
  /* */ "indexScan 2"_Stz,     // CK_INDEXSCAN
  
  /* */ "(none)"_Stz,          // CK_LBOUND
  /* */ "limit     2"_Stz,     // CK_LIMIT
    
  /* */ "(none)"_Stz,          // CK_ON
  /* */ "ordagg    2"_Stz,     // CK_ORDAGG
  
  /* */ "(none)"_Stz,          // CK_PREDAND
  /* */ "(none)"_Stz,          // CK_PREDCMP
  /* */ "(none)"_Stz,          // CK_PREDOR
  /* */ "project   2"_Stz,     // CK_PROJECT
  /* */ "(none)"_Stz,          // CK_PROJELIST

  /* */ "(none)"_Stz,          // CK_RBOUND
  
  /* */ "(none)"_Stz,          // CK_SBYELIST
  /* */ "scan       "_Stz,     // CK_SCAN
  /* */ "(none)"_Stz,          // CK_SEND
  /* */ "sort       "_Stz,     // CK_SORT
  /* */ "(none)"_Stz,          // CK_SORTDIRN
  /* */ "(none)"_Stz,          // CK_STAGE
  /* */ "(none)"_Stz,          // CK_STREAMDEF
  
  /* */ "(none)"_Stz,          // CK_TABLE
  /* */ "(none)"_Stz,          // CK_TERM
  /* */ "(none)"_Stz,          // CK_TERMSEQ
  /* */ "(none)"_Stz,          // CK_TYPE

  /* */ "(none)"_Stz,          // CK_UBOUND
  
  /* */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
