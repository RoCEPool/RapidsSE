/*============================================================================*/
/* eCK.h -- Card Kind                                                         */
/*============================================================================*/
#pragma once

enum eCK // card kind
{
  /* */ CK_NONE=0,

  /* */ CK_ABSJOIN,
  /* */ CK_AGGELIST,
  /* */ CK_AGGREG,
  /* */ CK_AGGREGATES,
  /* */ CK_ANDPRED,
  /* */ CK_AS,

  /* */ CK_BOOLCALC,
  /* */ CK_BOUNDS,

  /* */ CK_CALCOP,
  /* */ CK_CALL,
  /* */ CK_CANDINDEX,
  /* */ CK_CAST,
  /* */ CK_CLAUSE,
  /* */ CK_CMPPRED,
  /* */ CK_COLUMNREF,
  /* */ CK_COMBINED,
  /* */ CK_COMMENT,
  /* */ CK_CONSTRAINT,
  /* */ CK_COPTNJOIN,
  /* */ CK_COPTNJOINLIST,
  /* */ CK_COPTNPRED,
  /* */ CK_COPTNPREDLIST,
  /* */ CK_CROSSJOIN,
  /* */ CK_CROSSPTNJOIN,
  /* */ CK_CROSSPTNJOINLIST,
  /* */ CK_CROSSPTNPRED,

  /* */ CK_DEBUG,
  /* */ CK_DECK,
  /* */ CK_DIRECTION,

  /* */ CK_EBOUND,
  /* */ CK_EXEPRED,
  /* */ CK_EXISTS,
  /* */ CK_EXPR,
  /* */ CK_EXPRBOOL,

  /* */ CK_FILTER,
  /* */ CK_FILTELIST,
  /* */ CK_FOLD,

  /* */ CK_GBYAGG,
  /* */ CK_GBYELIST,

  /* */ CK_HASH,
  /* */ CK_HAVING,

  /* */ CK_IDXCOL,
  /* */ CK_IDXCOLLIST,
  /* */ CK_IDXCOLNM,
  /* */ CK_INDEX,
  /* */ CK_INDEXJOIN,
  /* */ CK_INDEXLIST,
  /* */ CK_INDEXNM,
  /* */ CK_INDEXPRED,
  /* */ CK_INDEXSCAN,

  /* */ CK_JOINORDER,
  /* */ CK_JOINORDERSEQ,
  /* */ CK_JOINPREDLIST,

  /* */ CK_KEYEXISTS,

  /* */ CK_LBOUND,
  /* */ CK_LEVEL,
  /* */ CK_LIMIT,
  /* */ CK_LINKSTO,

  /* */ CK_MAYBEINDEXJOIN,
  /* */ CK_MANNER,

  /* */ CK_NCORRCOLS,
  /* */ CK_NEXT,
  /* */ CK_NOTE,
  /* */ CK_NPARMCOLS,

  /* */ CK_OBYELIST,
  /* */ CK_OPTION,
  /* */ CK_ORDAGG,
  /* */ CK_ORPRED,

  /* */ CK_PARAM,
  /* */ CK_PARAMLIST,
  /* */ CK_PARTITION,
  /* */ CK_PERMUTATION,
  /* */ CK_PMZWHERE,
  /* */ CK_PROJECT,
  /* */ CK_PROJELIST,
  /* */ CK_PTNCOL,
  /* */ CK_PTNCOLLIST,
  /* */ CK_PTNCOLNM,
  /* */ CK_PTNDJOIN,
  /* */ CK_PTNDJOINSET,

  /* */ CK_RBOUND,
  /* */ CK_RELNOP,
  /* */ CK_RPNCALC,
  /* */ CK_RPNTERMS,

  /* */ CK_SBYELIST,
  /* */ CK_SCAN,
  /* */ CK_SEND,
  /* */ CK_SENDKIND,
  /* */ CK_SIZE,
  /* */ CK_SORT,
  /* */ CK_STAGE,
  /* */ CK_STMCOL,
  /* */ CK_STMCOLLIST,
  /* */ CK_STMCOLNM,
  /* */ CK_STMCOLODN,
  /* */ CK_STREAM,
  /* */ CK_SUBSET,

  /* */ CK_TABLE,
  /* */ CK_TABLELIST,
  /* */ CK_TABLENM,
  /* */ CK_TABLEPTN,
  /* */ CK_TBLCOLODN,
  /* */ CK_TERM,
  /* */ CK_TOPOLOGY,
  /* */ CK_TSTPRED,
  /* */ CK_TWIST,
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
  /* */ "CK_AGGREG"_Stz,       // CK_AGGREG
  /* */ "CK_AGGREGATES"_Stz,   // CK_AGGREGATES
  /* */ "CK_ANDPRED"_Stz,      // CK_ANDPRED
  /* */ "CK_AS"_Stz,           // CK_AS

  /* */ "CK_BOOLCALC"_Stz,     // CK_BOOLCALC
  /* */ "CK_BOUNDS"_Stz,       // CK_BOUNDS

  /* */ "CK_CALCOP"_Stz,       // CK_CALCOP
  /* */ "CK_CALL"_Stz,         // CK_CALL
  /* */ "CK_CANDINDEX"_Stz,    // CK_CANDINDEX
  /* */ "CK_CAST"_Stz,         // CK_CAST
  /* */ "CK_CLAUSE"_Stz,       // CK_CLAUSE
  /* */ "CK_CMPPRED"_Stz,      // CK_CMPPRED
  /* */ "CK_COLUMNREF"_Stz,    // CK_COLUMNREF
  /* */ "CK_COMBINED"_Stz,     // CK_COMBINED
  /* */ "CK_COMMENT"_Stz,      // CK_COMMENT
  /* */ "CK_CONSTRAINT"_Stz,   // CK_CONSTRAINT
  /* */ "CK_COPTNJOIN"_Stz,    // CK_COPTNJOIN
  /* */ "CK_COPTNJOINLIST"_Stz,// CK_COPTNJOINLIST
  /* */ "CK_COPTNPRED"_Stz,    // CK_COPTNPRED
  /* */ "CK_COPTNPREDLIST"_Stz,// CK_COPTNPREDLIST
  /* */ "CK_CROSSJOIN"_Stz,    // CK_CROSSJOIN
  /* */ "CK_CROSSPTNJOIN"_Stz,     // CK_CROSSPTNJOIN
  /* */ "CK_CROSSPTNJOINLIST"_Stz, // CK_CROSSPTNJOINLIST
  /* */ "CK_CROSSPTNPRED"_Stz,     // CK_CROSSPTNPRED

  /* */ "CK_DEBUG"_Stz,        // CK_DEBUG
  /* */ "CK_DECK"_Stz,         // CK_DECK
  /* */ "CK_DIRECTION"_Stz,    // CK_DIRECTION

  /* */ "CK_EBOUND"_Stz,       // CK_EBOUND
  /* */ "CK_EXEPRED"_Stz,      // CK_EXEPRED
  /* */ "CK_EXISTS"_Stz,       // CK_EXISTS
  /* */ "CK_EXPR"_Stz,         // CK_EXPR
  /* */ "CK_EXPRBOOL"_Stz,     // CK_EXPRBOOL

  /* */ "CK_FILTER"_Stz,       // CK_FILTER
  /* */ "CK_FILTELIST"_Stz,    // CK_FILTELIST
  /* */ "CK_FOLD"_Stz,         // CK_FOLD

  /* */ "CK_GBYAGG"_Stz,       // CK_GBYAGG
  /* */ "CK_GBYELIST"_Stz,     // CK_GBYELIST

  /* */ "CK_HASH"_Stz,         // CK_HASH
  /* */ "CK_HAVING"_Stz,       // CK_HAVING

  /* */ "CK_IDXCOL"_Stz,       // CK_IDXCOL
  /* */ "CK_IDXCOLLIST"_Stz,   // CK_IDXCOLLIST
  /* */ "CK_IDXCOLNM"_Stz,     // CK_IDXCOLNM
  /* */ "CK_INDEX"_Stz,        // CK_INDEX
  /* */ "CK_INDEXJOIN"_Stz,    // CK_INDEXJOIN
  /* */ "CK_INDEXLIST"_Stz,    // CK_INDEXLIST
  /* */ "CK_INDEXNM"_Stz,      // CK_INDEXNM
  /* */ "CK_INDEXPRED"_Stz,    // CK_INDEXPRED
  /* */ "CK_INDEXSCAN"_Stz,    // CK_INDEXSCAN

  /* */ "CK_JOINORDER"_Stz,    // CK_JOINORDER
  /* */ "CK_JOINORDERSEQ"_Stz, // CK_JOINORDERSEQ
  /* */ "CK_JOINPREDLIST"_Stz, // CK_JOINPREDLIST

  /* */ "CK_KEYEXISTS"_Stz,    // CK_KEYEXISTS

  /* */ "CK_LBOUND"_Stz,       // CK_LBOUND
  /* */ "CK_LEVEL"_Stz,        // CK_LEVEL
  /* */ "CK_LIMIT"_Stz,        // CK_LIMIT
  /* */ "CK_LINKSTO"_Stz,      // CK_LINKSTO

  /* */ "CK_MAYBEINDEXJOIN"_Stz, // CK_MAYBEINDEXJOIN
  /* */ "CK_MANNER"_Stz,       // CK_MANNER

  /* */ "CK_NCORRCOLS"_Stz     // CK_NCORRCOLS
  /* */ "CK_NEXT"_Stz,         // CK_NEXT
  /* */ "CK_NOTE"_Stz,         // CK_NOTE
  /* */ "CK_NPARMCOLS"_Stz     // CK_NPARMCOLS

  /* */ "CK_OBYELIST"_Stz,     // CK_OBYELIST
  /* */ "CK_OPTION"_Stz,       // CK_OPTION
  /* */ "CK_ORDAGG"_Stz,       // CK_ORDAGG
  /* */ "CK_ORPRED"_Stz,       // CK_ORPRED

  /* */ "CK_PARAM"_Stz,        // CK_PARAM
  /* */ "CK_PARAMLIST"_Stz,    // CK_PARAMLIST
  /* */ "CK_PARTITION"_Stz,    // CK_PARTITION
  /* */ "CK_PERMUTATION"_Stz,  // CK_PERMUTATION
  /* */ "CK_PMZWHERE"_Stz,     // CK_PMZWHERE
  /* */ "CK_PROJECT"_Stz,      // CK_PROJECT
  /* */ "CK_PROJELIST"_Stz,    // CK_PROJELIST
  /* */ "CK_PTNCOL"_Stz,       // CK_PTNCOL
  /* */ "CK_PTNCOLLIST"_Stz,   // CK_PTNCOLLIST
  /* */ "CK_PTNCOLNM"_Stz,     // CK_PTNCOLNM
  /* */ "CK_PTNDJOIN"_Stz,     // CK_PTNDJOIN
  /* */ "CK_PTNDJOINSET"_Stz,  // CK_PTNDJOINSET

  /* */ "CK_RBOUND"_Stz,       // CK_RBOUND
  /* */ "CK_RELNOP"_Stz,       // CK_RELNOP
  /* */ "CK_RPNCALC"_Stz,      // CK_RPNCALC
  /* */ "CK_RPNTERMS"_Stz,     // CK_RPNTERMS

  /* */ "CK_SBYELIST"_Stz,     // CK_SBYELIST
  /* */ "CK_SCAN"_Stz,         // CK_SCAN
  /* */ "CK_SEND"_Stz,         // CK_SEND
  /* */ "CK_SENDKIND"_Stz,     // CK_SENDKIND
  /* */ "CK_SIZE"_Stz,         // CK_SIZE
  /* */ "CK_SORT"_Stz,         // CK_SORT
  /* */ "CK_STAGE"_Stz,        // CK_STAGE
  /* */ "CK_STMCOL"_Stz,       // CK_STMCOL
  /* */ "CK_STMCOLLIST"_Stz,   // CK_STMCOLLIST
  /* */ "CK_STMCOLNM"_Stz,     // CK_STMCOLNM
  /* */ "CK_STMCOLODN"_Stz,    // CK_STMCOLODN
  /* */ "CK_STREAM"_Stz,       // CK_STREAM
  /* */ "CK_SUBSET"_Stz,       // CK_SUBSET

  /* */ "CK_TABLE"_Stz,        // CK_TABLE
  /* */ "CK_TABLELIST"_Stz,    // CK_TABLELIST
  /* */ "CK_TABLENM"_Stz,      // CK_TABLENM
  /* */ "CK_TABLEPTN"_Stz,     // CK_TABLEPTN
  /* */ "CK_TBLCOLODN"_Stz,    // CK_TBLCOLODN
  /* */ "CK_TERM"_Stz,         // CK_TERM
  /* */ "CK_TOPOLOGY"_Stz,     // CK_TOPOLOGY
  /* */ "CK_TSTPRED"_Stz,      // CK_TSTPRED
  /* */ "CK_TWIST"_Stz,        // CK_TWIST
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
  /* */ "aggreg"_Stz,          // CK_AGGREG
  /* */ "aggregates"_Stz,      // CK_AGGREGATES
  /* */ "andPred"_Stz,         // CK_ANDPRED
  /* */ "as"_Stz,              // CK_AS

  /* */ "boolCalc"_Stz,        // CK_BOOLCALC
  /* */ "bounds"_Stz,          // CK_BOUNDS

  /* */ "calcOp"_Stz,          // CK_CALCOP
  /* */ "call"_Stz,            // CK_CALL
  /* */ "candIndex"_Stz,       // CK_CANDINDEX
  /* */ "cast"_Stz,            // CK_CAST
  /* */ "clause"_Stz,          // CK_CLAUSE
  /* */ "cmpPred"_Stz,         // CK_CMPPRED
  /* */ "columnRef"_Stz,       // CK_COLUMNREF
  /* */ "combined"_Stz,        // CK_COMBINED
  /* */ "comment"_Stz,         // CK_COMMENT
  /* */ "constraint"_Stz,      // CK_CONSTRAINT
  /* */ "coPtnJoin"_Stz,       // CK_COPTNJOIN
  /* */ "coPtnJoinList"_Stz,   // CK_COPTNJOINLIST
  /* */ "coPtnPred"_Stz,       // CK_COPTNPRED
  /* */ "coPtnPredList"_Stz,   // CK_COPTNPREDLIST
  /* */ "crossJoin"_Stz,       // CK_CROSSJOIN
  /* */ "crossPtnJoin"_Stz,    // CK_CROSSPTNJOIN
  /* */ "crossPtnJoinList"_Stz,// CK_CROSSPTNJOINLIST
  /* */ "crossPtnPred"_Stz,    // CK_CROSSPTNPRED

  /* */ "debug"_Stz,           // CK_DEBUG
  /* */ "deck"_Stz,            // CK_DECK
  /* */ "direction"_Stz,       // CK_DIRECTION

  /* */ "ebound"_Stz,          // CK_EBOUND
  /* */ "exePred"_Stz,         // CK_EXEPRED
  /* */ "exists"_Stz,          // CK_EXISTS
  /* */ "expr"_Stz,            // CK_EXPR
  /* */ "exprBool"_Stz,        // CK_EXPRBOOL

  /* */ "filter"_Stz,          // CK_FILTER
  /* */ "filtElist"_Stz,       // CK_FILTELIST
  /* */ "fold"_Stz,            // CK_FOLD

  /* */ "gbyAgg"_Stz,          // CK_GBYAGG
  /* */ "gbyElist"_Stz,        // CK_GBYELIST

  /* */ "hash"_Stz,            // CK_HASH
  /* */ "having"_Stz,          // CK_HAVING

  /* */ "idxCol"_Stz,          // CK_IDXCOL
  /* */ "idxColList"_Stz,      // CK_IDXCOLLIST
  /* */ "idxColNm"_Stz,        // CK_IDXCOLNM
  /* */ "index"_Stz,           // CK_INDEX
  /* */ "indexJoin"_Stz,       // CK_INDEXJOIN
  /* */ "indexList"_Stz,       // CK_INDEXLIST
  /* */ "indexNm"_Stz,         // CK_INDEXNM
  /* */ "indexPred"_Stz,       // CK_INDEXPRED
  /* */ "indexScan"_Stz,       // CK_INDEXSCAN

  /* */ "joinOrder"_Stz,       // CK_JOINORDER
  /* */ "joinOrderSeq"_Stz,    // CK_JOINORDERSEQ
  /* */ "joinPredList"_Stz,    // CK_JOINPREDLIST

  /* */ "keyExists"_Stz,       // CK_KEYEXISTS

  /* */ "lbound"_Stz,          // CK_LBOUND
  /* */ "level"_Stz,           // CK_LEVEL
  /* */ "limit"_Stz,           // CK_LIMIT
  /* */ "linksTo"_Stz,         // CK_LINKSTO

  /* */ "maybeIndexJoin"_Stz,  // CK_MAYBEINDEXJOIN
  /* */ "manner"_Stz,          // CK_MANNER

  /* */ "nCorrCols"_Stz,       // CK_NCORRCOLS
  /* */ "next"_Stz,            // CK_NEXT
  /* */ "note"_Stz,            // CK_NOTE
  /* */ "nParmCols"_Stz,       // CK_NPARMCOLS

  /* */ "obyElist"_Stz,        // CK_OBYELIST
  /* */ "option"_Stz,          // CK_OPTION
  /* */ "ordAgg"_Stz,          // CK_ORDAGG
  /* */ "orPred"_Stz,          // CK_ORPRED

  /* */ "param"_Stz,           // CK_PARAM
  /* */ "paramList"_Stz,       // CK_PARAMLIST
  /* */ "partition"_Stz,       // CK_PARTITION
  /* */ "permutation"_Stz,     // CK_PERMUTATION
  /* */ "pmzWhere"_Stz,        // CK_PMZWHERE
  /* */ "project"_Stz,         // CK_PROJECT
  /* */ "projElist"_Stz,       // CK_PROJELIST
  /* */ "ptnCol"_Stz,          // CK_PTNCOL
  /* */ "ptnColList"_Stz,      // CK_PTNCOLLIST
  /* */ "ptnColNm"_Stz,        // CK_PTNCOLNM
  /* */ "ptndJoin"_Stz,        // CK_PTNDJOIN
  /* */ "ptndJoinSet"_Stz,     // CK_PTNDJOINSET

  /* */ "rbound"_Stz,          // CK_RBOUND
  /* */ "relnOp"_Stz,          // CK_RELNOP
  /* */ "rpnCalc"_Stz,         // CK_RPNCALC
  /* */ "rpnTerms"_Stz,        // CK_RPNTERMS

  /* */ "sbyElist"_Stz,        // CK_SBYELIST
  /* */ "scan"_Stz,            // CK_SCAN
  /* */ "sendSE"_Stz,          // CK_SEND
  /* */ "sendKind"_Stz,        // CK_SENDKIND
  /* */ "size"_Stz,            // CK_SIZE
  /* */ "sort"_Stz,            // CK_SORT
  /* */ "stage"_Stz,           // CK_STAGE
  /* */ "stmCol"_Stz,          // CK_STMCOL
  /* */ "stmColList"_Stz,      // CK_STMCOLLIST
  /* */ "stmColNm"_Stz,        // CK_STMCOLNM
  /* */ "stmColOdn"_Stz,       // CK_STMCOLODN
  /* */ "stream"_Stz,          // CK_STREAM
  /* */ "subSet"_Stz,          // CK_SUBSET

  /* */ "table"_Stz,           // CK_TABLE
  /* */ "tableList"_Stz,       // CK_TABLELIST
  /* */ "tableNm"_Stz,         // CK_TABLENM
  /* */ "tablePtn"_Stz,        // CK_TABLEPTN
  /* */ "tblColOdn"_Stz,       // CK_TBLCOLODN
  /* */ "term"_Stz,            // CK_TERM
  /* */ "topology"_Stz,        // CK_TOPOLOGY
  /* */ "tstPred"_Stz,         // CK_TSTPRED
  /* */ "twist"_Stz,           // CK_TWIST
  /* */ "type"_Stz,            // CK_TYPE

  /* */ "ubound"_Stz,          // CK_UBOUND
  /* */ "unique"_Stz,          // CK_UNIQUE

  /* */ "where"_Stz,           // CK_WHERE

  /* */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
