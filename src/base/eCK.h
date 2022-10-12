/*============================================================================*/
/* eCK.h -- Card Kind                                                         */
/*============================================================================*/
#pragma once

enum eCK // card kind
{
  /* */ CK_NONE=0,

  /* */ CK_ABSJOIN,
  /* */ CK_AGGELIST,
  /* */ CK_AGGLEVEL,
  /* */ CK_AGGREG,
  /* */ CK_AGGREGATES,
  /* */ CK_ANDPRED,
  /* */ CK_AS,

  /* */ CK_CALC,
  /* */ CK_CALCBOOL,
  /* */ CK_CANDINDEX,
  /* */ CK_CARDPTR,
  /* */ CK_CMPPRED,
  /* */ CK_COLUMNREF,
  /* */ CK_COMBINED,
  /* */ CK_COMMENT,
  /* */ CK_COPTNJOIN,
  /* */ CK_COPTNJOINLIST,
  /* */ CK_COPTNPRED,
  /* */ CK_COPTNPREDLIST,
  /* */ CK_CROSSPTNJOIN,
  /* */ CK_CROSSPTNJOINLIST,
  /* */ CK_CROSSPTNPRED,

  /* */ CK_DEBUG,
  /* */ CK_DECK,
  /* */ CK_DIRECTION,

  /* */ CK_EBOUND,
  /* */ CK_EXISTS,
  /* */ CK_EXPR,
  /* */ CK_EXPRBOOL,

  /* */ CK_FILTER,
  /* */ CK_FILTELIST,

  /* */ CK_GBYELIST,

  /* */ CK_HASH,
  /* */ CK_HAVING,

  /* */ CK_IDXCOLNM,
  /* */ CK_IDXCOLLIST,
  /* */ CK_INDEXJOIN,
  /* */ CK_INDEXLIST,
  /* */ CK_INDEXPRED,
  /* */ CK_INDEXPTR,
  /* */ CK_INDEXSCAN,

  /* */ CK_JOINORDER,
  /* */ CK_JOINORDERSEQ,
  /* */ CK_JOINPRED,
  /* */ CK_JOINPREDLIST,

  /* */ CK_LBOUND,
  /* */ CK_LIMIT,

  /* */ CK_OBYELIST,
  /* */ CK_OPTION,
  /* */ CK_ORDAGG,
  /* */ CK_ORPRED,

  /* */ CK_PERMUTATION,
  /* */ CK_PROJECT,
  /* */ CK_PROJELIST,
  /* */ CK_PTNDJOIN,
  /* */ CK_PTNDJOINSET,

  /* */ CK_RBOUND,
  /* */ CK_RPNTERMS,

  /* */ CK_SBYELIST,
  /* */ CK_SCAN,
  /* */ CK_SCANPRED,
  /* */ CK_SEND,
  /* */ CK_SENDKIND,
  /* */ CK_SORT,
  /* */ CK_STAGE,
  /* */ CK_STAGETOPO,
  /* */ CK_STREAM,
  /* */ CK_STREAMCOL,
  /* */ CK_STREAMCOLLIST,
  /* */ CK_STREAMELIST,
  /* */ CK_SUBSET,

  /* */ CK_TABLE,
  /* */ CK_TABLELIST,
  /* */ CK_TABLEPTN,
  /* */ CK_TABLEPTR,
  /* */ CK_TERM,
  /* */ CK_TYPE,

  /* */ CK_UBOUND,
  /* */ CK_UNIQUE,

  /* */ CK_WHERE,

  /* */ CK_ENUMS
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static Stz* aCK[]
{
  /* */ "NONE"_Stz,

  /* */ "CK_ABSJOIN"_Stz,      // CK_ABSJOIN
  /* */ "CK_AGGELIST"_Stz,     // CK_AGGELIST
  /* */ "CK_AGGLEVEL"_Stz,     // CK_AGGLEVEL
  /* */ "CK_AGGREG"_Stz,       // CK_AGGREG
  /* */ "CK_AGGREGATES"_Stz,   // CK_AGGREGATES
  /* */ "CK_ANDPRED"_Stz,      // CK_ANDPRED
  /* */ "CK_AS"_Stz,           // CK_AS

  /* */ "CK_CALC"_Stz,         // CK_CALC
  /* */ "CK_CALCBOOL"_Stz,     // CK_CALCBOOL
  /* */ "CK_CANDINDEX"_Stz,    // CK_CANDINDEX
  /* */ "CK_CARDPTR"_Stz,      // CK_CARDPTR
  /* */ "CK_CMPPRED"_Stz,      // CK_CMPPRED
  /* */ "CK_COLUMNREF"_Stz,    // CK_COLUMNREF
  /* */ "CK_COMBINED"_Stz,     // CK_COMBINED
  /* */ "CK_COMMENT"_Stz,      // CK_COMMENT
  /* */ "CK_COPTNJOIN"_Stz,    // CK_COPTNJOIN
  /* */ "CK_COPTNJOINLIST"_Stz,// CK_COPTNJOINLIST
  /* */ "CK_COPTNPRED"_Stz,    // CK_COPTNPRED
  /* */ "CK_COPTNPREDLIST"_Stz,// CK_COPTNPREDLIST
  /* */ "CK_CROSSPTNJOIN"_Stz,     // CK_CROSSPTNJOIN
  /* */ "CK_CROSSPTNJOINLIST"_Stz, // CK_CROSSPTNJOINLIST
  /* */ "CK_CROSSPTNPRED"_Stz, // CK_CROSSPTNPRED

  /* */ "CK_DEBUG"_Stz,        // CK_DEBUG
  /* */ "CK_DECK"_Stz,         // CK_DECK
  /* */ "CK_DIRECTION"_Stz,    // CK_DIRECTION

  /* */ "CK_EBOUND"_Stz,       // CK_EBOUND
  /* */ "CK_EXISTS"_Stz,       // CK_EXISTS
  /* */ "CK_EXPR"_Stz,         // CK_EXPR
  /* */ "CK_EXPRBOOL"_Stz,     // CK_EXPRBOOL

  /* */ "CK_FILTER"_Stz,       // CK_FILTER
  /* */ "CK_FILTELIST"_Stz,    // CK_FILTELIST

  /* */ "CK_GBYELIST"_Stz,     // CK_GBYELIST

  /* */ "CK_HASH"_Stz,         // CK_HASH
  /* */ "CK_HAVING"_Stz,       // CK_HAVING

  /* */ "CK_IDXCOLNM"_Stz,       // CK_IDXCOLNM
  /* */ "CK_IDXCOLLIST"_Stz,   // CK_IDXCOLLIST
  /* */ "CK_INDEXJOIN"_Stz,    // CK_INDEXJOIN
  /* */ "CK_INDEXLIST"_Stz,    // CK_INDEXLIST
  /* */ "CK_INDEXPRED"_Stz,    // CK_INDEXPRED
  /* */ "CK_INDEXPTR"_Stz,     // CK_INDEXPTR
  /* */ "CK_INDEXSCAN"_Stz,    // CK_INDEXSCAN

  /* */ "CK_JOINORDER"_Stz,    // CK_JOINORDER
  /* */ "CK_JOINORDERSEQ"_Stz, // CK_JOINORDERSEQ
  /* */ "CK_JOINPRED"_Stz,     // CK_JOINPRED
  /* */ "CK_JOINPREDLIST"_Stz, // CK_JOINPREDLIST

  /* */ "CK_LBOUND"_Stz,       // CK_LBOUND
  /* */ "CK_LIMIT"_Stz,        // CK_LIMIT

  /* */ "CK_OBYELIST"_Stz,     // CK_OBYELIST
  /* */ "CK_OPTION"_Stz,       // CK_OPTION
  /* */ "CK_ORDAGG"_Stz,       // CK_ORDAGG
  /* */ "CK_ORPRED"_Stz,       // CK_ORPRED

  /* */ "CK_PERMUTATION"_Stz,  // CK_PERMUTATION
  /* */ "CK_PROJECT"_Stz,      // CK_PROJECT
  /* */ "CK_PROJELIST"_Stz,    // CK_PROJELIST
  /* */ "CK_PTNDJOIN"_Stz,     // CK_PTNDJOIN
  /* */ "CK_PTNDJOINSET"_Stz,  // CK_PTNDJOINSET

  /* */ "CK_RBOUND"_Stz,       // CK_RBOUND
  /* */ "CK_RPNTERMS"_Stz,     // CK_RPNTERMS

  /* */ "CK_SBYELIST"_Stz,     // CK_SBYELIST
  /* */ "CK_SCAN"_Stz,         // CK_SCAN
  /* */ "CK_SCANPRED"_Stz,     // CK_SCANPRED
  /* */ "CK_SEND"_Stz,         // CK_SEND
  /* */ "CK_SENDKIND"_Stz,     // CK_SENDKIND
  /* */ "CK_SORT"_Stz,         // CK_SORT
  /* */ "CK_STAGE"_Stz,        // CK_STAGE
  /* */ "CK_STAGETOPO"_Stz,    // CK_STAGETOPO
  /* */ "CK_STREAM"_Stz,       // CK_STREAM
  /* */ "CK_STREAMCOL"_Stz,    // CK_STREAMCOL
  /* */ "CK_STREAMCOLLIST"_Stz,// CK_STREAMCOLLIST
  /* */ "CK_STREAMELIST"_Stz,  // CK_STREAMELIST
  /* */ "CK_SUBSET"_Stz,       // CK_SUBSET

  /* */ "CK_TABLE"_Stz,        // CK_TABLE
  /* */ "CK_TABLELIST"_Stz,    // CK_TABLELIST
  /* */ "CK_TABLEPTN"_Stz,     // CK_TABLEPTN
  /* */ "CK_TABLEPTR"_Stz,     // CK_TABLEPTR
  /* */ "CK_TERM"_Stz,         // CK_TERM
  /* */ "CK_TYPE"_Stz,         // CK_TYPE

  /* */ "CK_UBOUND"_Stz,       // CK_UBOUND
  /* */ "CK_UNIQUE"_Stz,       // CK_UNIQUE

  /* */ "CK_WHERE"_Stz,        // CK_WHERE

  /* */ "ENUMS"_Stz
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static Stz* aCK_short[]
{
  /* */ "NONE"_Stz,

  /* */ "absJoin"_Stz,         // CK_ABSJOIN
  /* */ "aggElist"_Stz,        // CK_AGGELIST
  /* */ "aggLevel"_Stz,        // CK_AGGLEVEL
  /* */ "aggreg"_Stz,          // CK_AGGREG
  /* */ "aggregates"_Stz,      // CK_AGGREGATES
  /* */ "andPred"_Stz,         // CK_ANDPRED
  /* */ "as"_Stz,              // CK_AS

  /* */ "calc"_Stz,            // CK_CALC
  /* */ "calcBool"_Stz,        // CK_CALCBOOL
  /* */ "candIndex"_Stz,       // CK_CANDINDEX
  /* */ "{card}"_Stz,          // CK_CARDPTR
  /* */ "cmpPred"_Stz,         // CK_CMPPRED
  /* */ "columnRef"_Stz,       // CK_COLUMNREF
  /* */ "combined"_Stz,        // CK_COMBINED
  /* */ "comment"_Stz,         // CK_COMMENT
  /* */ "coPtnJoin"_Stz,       // CK_COPTNJOIN
  /* */ "coPtnJoinList"_Stz,   // CK_COPTNJOINLIST
  /* */ "coPtnPred"_Stz,       // CK_COPTNPRED
  /* */ "coPtnPredList"_Stz,   // CK_COPTNPREDLIST
  /* */ "crossPtnJoin"_Stz,    // CK_CROSSPTNJOIN
  /* */ "crossPtnJoinList"_Stz,// CK_CROSSPTNJOINLIST
  /* */ "crossPtnPred"_Stz,    // CK_CROSSPTNPRED

  /* */ "debug"_Stz,           // CK_DEBUG
  /* */ "deck"_Stz,            // CK_DECK
  /* */ "direction"_Stz,       // CK_DIRECTION

  /* */ "ebound"_Stz,          // CK_EBOUND
  /* */ "exists"_Stz,          // CK_EXISTS
  /* */ "expr"_Stz,            // CK_EXPR
  /* */ "exprBool"_Stz,        // CK_EXPRBOOL

  /* */ "filter"_Stz,          // CK_FILTER
  /* */ "filtElist"_Stz,       // CK_FILTELIST

  /* */ "gbyElist"_Stz,        // CK_GBYELIST

  /* */ "hash"_Stz,            // CK_HASH
  /* */ "having"_Stz,          // CK_HAVING

  /* */ "idxColNm"_Stz,        // CK_IDXCOLNM
  /* */ "idxColList"_Stz,      // CK_IDXCOLLIST
  /* */ "indexJoin"_Stz,       // CK_INDEXJOIN
  /* */ "indexList"_Stz,       // CK_INDEXLIST
  /* */ "indexPred"_Stz,       // CK_INDEXPRED
  /* */ "{index}"_Stz,         // CK_INDEXPTR
  /* */ "indexScan"_Stz,       // CK_INDEXSCAN

  /* */ "joinOrder"_Stz,       // CK_JOINORDER
  /* */ "joinOrderSeq"_Stz,    // CK_JOINORDERSEQ
  /* */ "joinPred"_Stz,        // CK_JOINPRED
  /* */ "joinPredList"_Stz,    // CK_JOINPREDLIST

  /* */ "lbound"_Stz,          // CK_LBOUND
  /* */ "limit"_Stz,           // CK_LIMIT

  /* */ "obyElist"_Stz,        // CK_OBYELIST
  /* */ "option"_Stz,          // CK_OPTION
  /* */ "ordAgg"_Stz,          // CK_ORDAGG
  /* */ "orPred"_Stz,          // CK_ORPRED

  /* */ "permutation"_Stz,     // CK_PERMUTATION
  /* */ "project"_Stz,         // CK_PROJECT
  /* */ "projElist"_Stz,       // CK_PROJELIST
  /* */ "ptndJoin"_Stz,        // CK_PTNDJOIN
  /* */ "ptndJoinSet"_Stz,     // CK_PTNDJOINSET

  /* */ "rbound"_Stz,          // CK_RBOUND
  /* */ "rpnTerms"_Stz,        // CK_RPNTERMS

  /* */ "sbyElist"_Stz,        // CK_SBYELIST
  /* */ "scan"_Stz,            // CK_SCAN
  /* */ "scanPred"_Stz,        // CK_SCANPRED
  /* */ "sendSE"_Stz,          // CK_SEND
  /* */ "sendKind"_Stz,        // CK_SENDKIND
  /* */ "sort"_Stz,            // CK_SORT
  /* */ "stage"_Stz,           // CK_STAGE
  /* */ "stageTopo"_Stz,       // CK_STAGETOPO
  /* */ "stream"_Stz,          // CK_STREAM
  /* */ "streamCol"_Stz,       // CK_STREAMCOL
  /* */ "streamColList"_Stz,   // CK_STREAMCOLLIST
  /* */ "streamElist"_Stz,     // CK_STREAMELIST
  /* */ "subSet"_Stz,          // CK_SUBSET

  /* */ "table"_Stz,           // CK_TABLE
  /* */ "tableList"_Stz,       // CK_TABLELIST
  /* */ "tablePtn"_Stz,        // CK_TABLEPTN
  /* */ "{table}"_Stz,         // CK_TABLEPTR
  /* */ "term"_Stz,            // CK_TERM
  /* */ "type"_Stz,            // CK_TYPE

  /* */ "ubound"_Stz,          // CK_UBOUND
  /* */ "unique"_Stz,          // CK_UNIQUE

  /* */ "where"_Stz,           // CK_WHERE

  /* */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
