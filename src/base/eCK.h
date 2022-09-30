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
  /* */ CK_AS,

  /* */ CK_BOARD,

  /* */ CK_CALC,
  /* */ CK_CALCBOOL,
  /* */ CK_CANDINDEX,
  /* */ CK_CARDPTR,
  /* */ CK_COLUMNREF,
  /* */ CK_COMBINED,
  /* */ CK_COPTN,
  /* */ CK_COPTNPRED,
  /* */ CK_CROSSPTNPRED,

  /* */ CK_DEBUG,
  /* */ CK_DIRECTION,

  /* */ CK_EBOUND,
  /* */ CK_EXISTS,
  /* */ CK_EXPR,
  /* */ CK_EXPRBOOL,

  /* */ CK_FILTER,
  /* */ CK_FILTELIST,

  /* */ CK_GBYELIST,
  /* */ CK_GENERATE,

  /* */ CK_HASH,
  /* */ CK_HAVING,

  /* */ CK_IDXCOL,
  /* */ CK_IDXCOLLIST,
  /* */ CK_INDEXJOIN,
  /* */ CK_INDEXLIST,
  /* */ CK_INDEXPRED,
  /* */ CK_INDEXPTR,
  /* */ CK_INDEXSCAN,

  /* */ CK_JOINPRED,
  /* */ CK_JOINPREDLIST,

  /* */ CK_LBOUND,
  /* */ CK_LIMIT,

  /* */ CK_MAP,
  /* */ CK_MAPSET,
  /* */ CK_MODE,

  /* */ CK_NAME,

  /* */ CK_OBYELIST,
  /* */ CK_ON,
  /* */ CK_OPTION,
  /* */ CK_ORDAGG,

  /* */ CK_PERMUTATION,
  /* */ CK_PREDAND,
  /* */ CK_PREDCMP,
  /* */ CK_PREDOR,
  /* */ CK_PROJECT,
  /* */ CK_PROJELIST,

  /* */ CK_RBOUND,
  /* */ CK_ROWS,
  /* */ CK_RPNTERMS,

  /* */ CK_SBYELIST,
  /* */ CK_SCAN,
  /* */ CK_SCANPRED,
  /* */ CK_SEND,
  /* */ CK_SENDKIND,
  /* */ CK_SENDS,
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
  /* */ CK_TABLESYN,
  /* */ CK_TERM,
  /* */ CK_TYPE,

  /* */ CK_UBOUND,
  /* */ CK_UNIQUE,

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
  /* */ "CK_AS"_Stz,           // CK_AS
  
  /* */ "CK_BOARD"_Stz,        // CK_BOARD

  /* */ "CK_CALC"_Stz,         // CK_CALC
  /* */ "CK_CALCBOOL"_Stz,     // CK_CALCBOOL
  /* */ "CK_CANDINDEX"_Stz,    // CK_CANDINDEX
  /* */ "CK_CARDPTR"_Stz,      // CK_CARDPTR
  /* */ "CK_COLUMNREF"_Stz,    // CK_COLUMNREF
  /* */ "CK_COMBINED"_Stz,     // CK_COMBINED
  /* */ "CK_COPTN"_Stz,        // CK_COPTN
  /* */ "CK_COPTNPRED"_Stz,    // CK_COPTNPRED
  /* */ "CK_CROSSPTNPRED"_Stz, // CK_CROSSPTNPRED

  /* */ "CK_DEBUG"_Stz,        // CK_DEBUG
  /* */ "CK_DIRECTION"_Stz,    // CK_DIRECTION

  /* */ "CK_EBOUND"_Stz,       // CK_EBOUND
  /* */ "CK_EXISTS"_Stz,       // CK_EXISTS
  /* */ "CK_EXPR"_Stz,         // CK_EXPR
  /* */ "CK_EXPRBOOL"_Stz,     // CK_EXPRBOOL

  /* */ "CK_FILTER"_Stz,       // CK_FILTER
  /* */ "CK_FILTELIST"_Stz,    // CK_FILTELIST

  /* */ "CK_GBYELIST"_Stz,     // CK_GBYELIST
  /* */ "CK_GENERATE"_Stz,     // CK_GENERATE

  /* */ "CK_HASH"_Stz,         // CK_HASH
  /* */ "CK_HAVING"_Stz,       // CK_HAVING

  /* */ "CK_IDXCOL"_Stz,       // CK_IDXCOL
  /* */ "CK_IDXCOLLIST"_Stz,   // CK_IDXCOLLIST
  /* */ "CK_INDEXJOIN"_Stz,    // CK_INDEXJOIN
  /* */ "CK_INDEXLIST"_Stz,    // CK_INDEXLIST
  /* */ "CK_INDEXPRED"_Stz,    // CK_INDEXPRED
  /* */ "CK_INDEXPTR"_Stz,     // CK_INDEXPTR
  /* */ "CK_INDEXSCAN"_Stz,    // CK_INDEXSCAN

  /* */ "CK_JOINPRED"_Stz,     // CK_JOINPRED
  /* */ "CK_JOINPREDLIST"_Stz, // CK_JOINPREDLIST

  /* */ "CK_LBOUND"_Stz,       // CK_LBOUND
  /* */ "CK_LIMIT"_Stz,        // CK_LIMIT

  /* */ "CK_MAP"_Stz,          // CK_MAP
  /* */ "CK_MAPSET"_Stz,       // CK_MAPSET
  /* */ "CK_MODE"_Stz,         // CK_MODE

  /* */ "CK_NAME"_Stz,         // CK_NAME

  /* */ "CK_OBYELIST"_Stz,     // CK_OBYELIST
  /* */ "CK_ON"_Stz,           // CK_ON
  /* */ "CK_OPTION"_Stz,       // CK_OPTION
  /* */ "CK_ORDAGG"_Stz,       // CK_ORDAGG

  /* */ "CK_PERMUTATION"_Stz,  // CK_PERMUTATION
  /* */ "CK_PREDAND"_Stz,      // CK_PREDAND
  /* */ "CK_PREDCMP"_Stz,      // CK_PREDCMP
  /* */ "CK_PREDOR"_Stz,       // CK_PREDOR
  /* */ "CK_PROJECT"_Stz,      // CK_PROJECT
  /* */ "CK_PROJELIST"_Stz,    // CK_PROJELIST

  /* */ "CK_RBOUND"_Stz,       // CK_RBOUND
  /* */ "CK_ROWS"_Stz,         // CK_ROWS
  /* */ "CK_RPNTERMS"_Stz,     // CK_RPNTERMS

  /* */ "CK_SBYELIST"_Stz,     // CK_SBYELIST
  /* */ "CK_SCAN"_Stz,         // CK_SCAN
  /* */ "CK_SCANPRED"_Stz,     // CK_SCANPRED
  /* */ "CK_SEND"_Stz,         // CK_SEND
  /* */ "CK_SENDKIND"_Stz,     // CK_SENDKIND
  /* */ "CK_SENDS"_Stz,        // CK_SENDS
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
  /* */ "CK_TABLESYN"_Stz,     // CK_TABLESYN
  /* */ "CK_TERM"_Stz,         // CK_TERM
  /* */ "CK_TYPE"_Stz,         // CK_TYPE

  /* */ "CK_UBOUND"_Stz,       // CK_UBOUND
  /* */ "CK_UNIQUE"_Stz,       // CK_UNIQUE

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
  /* */ "as"_Stz,              // CK_AS
  
  /* */ "board"_Stz,           // CK_BOARD

  /* */ "calc"_Stz,            // CK_CALC
  /* */ "calcBool"_Stz,        // CK_CALCBOOL
  /* */ "candIndex"_Stz,       // CK_CANDINDEX
  /* */ "{card}"_Stz,          // CK_CARDPTR
  /* */ "columnRef"_Stz,       // CK_COLUMNREF
  /* */ "combined"_Stz,        // CK_COMBINED
  /* */ "co-ptn"_Stz,          // CK_COPTN
  /* */ "co-ptnPred"_Stz,      // CK_COPTNPRED
  /* */ "cross-ptnPred"_Stz,   // CK_CROSSPTNPRED

  /* */ "debug"_Stz,           // CK_DEBUG
  /* */ "direction"_Stz,       // CK_DIRECTION

  /* */ "ebound"_Stz,          // CK_EBOUND
  /* */ "exists"_Stz,          // CK_EXISTS
  /* */ "expr"_Stz,            // CK_EXPR
  /* */ "exprBool"_Stz,        // CK_EXPRBOOL

  /* */ "filter"_Stz,          // CK_FILTER
  /* */ "filtElist"_Stz,       // CK_FILTELIST

  /* */ "gbyElist"_Stz,        // CK_GBYELIST
  /* */ "generate"_Stz,        // CK_GENERATE

  /* */ "hash"_Stz,            // CK_HASH
  /* */ "having"_Stz,          // CK_HAVING

  /* */ "idxCol"_Stz,          // CK_IDXCOL
  /* */ "idxColList"_Stz,      // CK_IDXCOLLIST
  /* */ "indexJoin"_Stz,       // CK_INDEXJOIN
  /* */ "indexList"_Stz,       // CK_INDEXLIST
  /* */ "indexPred"_Stz,       // CK_INDEXPRED
  /* */ "{index}"_Stz,         // CK_INDEXPTR
  /* */ "indexScan"_Stz,       // CK_INDEXSCAN

  /* */ "joinPred"_Stz,        // CK_JOINPRED
  /* */ "joinPredList"_Stz,    // CK_JOINPREDLIST

  /* */ "lbound"_Stz,          // CK_LBOUND
  /* */ "limit"_Stz,           // CK_LIMIT

  /* */ "map"_Stz,             // CK_MAP
  /* */ "mapSet"_Stz,          // CK_MAPSET
  /* */ "mode"_Stz,            // CK_MODE

  /* */ "name"_Stz,            // CK_NAME

  /* */ "obyElist"_Stz,        // CK_OBYELIST
  /* */ "on"_Stz,              // CK_ON
  /* */ "option"_Stz,          // CK_OPTION
  /* */ "ordAgg"_Stz,          // CK_ORDAGG

  /* */ "permutation"_Stz,     // CK_PERMUTATION
  /* */ "predAnd"_Stz,         // CK_PREDAND
  /* */ "predCmp"_Stz,         // CK_PREDCMP
  /* */ "predOr"_Stz,          // CK_PREDOR
  /* */ "project"_Stz,         // CK_PROJECT
  /* */ "projElist"_Stz,       // CK_PROJELIST

  /* */ "rbound"_Stz,          // CK_RBOUND
  /* */ "rows"_Stz,            // CK_ROWS
  /* */ "rpnTerms"_Stz,        // CK_RPNTERMS

  /* */ "sbyElist"_Stz,        // CK_SBYELIST
  /* */ "scan"_Stz,            // CK_SCAN
  /* */ "scanPred"_Stz,        // CK_SCANPRED
  /* */ "sendSE"_Stz,          // CK_SEND
  /* */ "sendKind"_Stz,        // CK_SENDKIND
  /* */ "sends"_Stz,           // CK_SENDS
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
  /* */ "tableSyn"_Stz,        // CK_TABLESYN
  /* */ "term"_Stz,            // CK_TERM
  /* */ "type"_Stz,            // CK_TYPE

  /* */ "ubound"_Stz,          // CK_UBOUND
  /* */ "unique"_Stz,          // CK_UNIQUE

  /* */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
