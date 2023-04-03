/*============================================================================*/
/* eCK.h -- Card Kind                                                         */
/*============================================================================*/
#pragma once

enum eCK // card kind
{
  /* */ CK_NONE=0,

  /* */ CK_ABSJOIN,
  /* */ CK_AGGREG,
  /* */ CK_ANDPRED,
  /* */ CK_AS,

  /* */ CK_BOOLCALC,
  /* */ CK_BOUND,
  /* */ CK_BOUNDS,

  /* */ CK_CALL,
  /* */ CK_CAST,
  /* */ CK_CMPPRED,
  /* */ CK_COMBINED,
  /* */ CK_CONSTRAINT,
  /* */ CK_CORRELATION,
  /* */ CK_COSTQUERY,
  /* */ CK_COSTNEXT,
  /* */ CK_CROSSJOIN,

  /* */ CK_DEBUG,
  /* */ CK_DECK,
  /* */ CK_DIRECTION,

  /* */ CK_EXPR,

  /* */ CK_FILTER,
  /* */ CK_FLAT,
  /* */ CK_FOLD,
  /* */ CK_FOLDELIST,
  /* */ CK_FROM,

  /* */ CK_GBYAGG,

  /* */ CK_HASH,
  /* */ CK_HAVING,
  /* */ CK_HSHCOL,

  /* */ CK_IDXCOL,
  /* */ CK_IDXCOLLIST,
  /* */ CK_INDEX,
  /* */ CK_INDEXJOIN,
  /* */ CK_INDEXLIST,
  /* */ CK_INDEXNM,
  /* */ CK_INDEXSCAN,

  /* */ CK_JOINHEAD,
  /* */ CK_JOINNEXT,
  /* */ CK_JOINORDER,
  /* */ CK_JOINORDERING,
  /* */ CK_JOINPERMALL,
  /* */ CK_JOINPERMSEQ,
  /* */ CK_JOINPREDLIST,

  /* */ CK_KEY,
  /* */ CK_KEYEXISTS,
  /* */ CK_KEYNOTEXISTS,

  /* */ CK_LEVEL,
  /* */ CK_LIMIT,
  /* */ CK_LOADJOINED,
  /* */ CK_LOADNETWORK,
  /* */ CK_LOADUNICAST,

  /* */ CK_NCORRCOLS,
  /* */ CK_NEEDED,
  /* */ CK_NEXT,
  /* */ CK_NOTE,
  /* */ CK_NOTPRED,

  /* */ CK_ON,
  /* */ CK_ORDAGG,
  /* */ CK_ORPRED,
  /* */ CK_OUTERCOLNM,
  /* */ CK_OUTERJOIN,

  /* */ CK_PROJECT,
  /* */ CK_PROJECTION,

  /* */ CK_RPNCALC,
  /* */ CK_RPNTERMS,

  /* */ CK_SCAN,
  /* */ CK_SECTION,
  /* */ CK_SEND,
  /* */ CK_SENDKIND,
  /* */ CK_SIZE,
  /* */ CK_SORT,
  /* */ CK_STAGE,
  /* */ CK_SUBSET,

  /* */ CK_TABLE,
  /* */ CK_TABLELIST,
  /* */ CK_TABLENM,
  /* */ CK_TABLEPTN,
  /* */ CK_TBLSYN,
  /* */ CK_TERM,
  /* */ CK_TOP,
  /* */ CK_TOPOLOGY,
  /* */ CK_TYPE,

  /* */ CK_UNIQUE,

  /* */ CK_WHERE,

  /* */ CK_ENUMS
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static Stz* aCK[]
{
  /* */ "NONE"_Stz,

  /* */ "CK_ABSJOIN"_Stz,      // CK_ABSJOIN
  /* */ "CK_AGGREG"_Stz,       // CK_AGGREG
  /* */ "CK_ANDPRED"_Stz,      // CK_ANDPRED
  /* */ "CK_AS"_Stz,           // CK_AS

  /* */ "CK_BOOLCALC"_Stz,     // CK_BOOLCALC
  /* */ "CK_BOUND"_Stz,        // CK_BOUND
  /* */ "CK_BOUNDS"_Stz,       // CK_BOUNDS

  /* */ "CK_CALL"_Stz,         // CK_CALL
  /* */ "CK_CAST"_Stz,         // CK_CAST
  /* */ "CK_CMPPRED"_Stz,      // CK_CMPPRED
  /* */ "CK_COMBINED"_Stz,     // CK_COMBINED
  /* */ "CK_CONSTRAINT"_Stz,   // CK_CONSTRAINT
  /* */ "CK_CORRELATION"_Stz,  // CK_CORRELATION
  /* */ "CK_COSTQUERY"_Stz,    // CK_COSTQUERY
  /* */ "CK_COSTNEXT"_Stz,         // CK_COSTNEXT
  /* */ "CK_CROSSJOIN"_Stz,    // CK_CROSSJOIN

  /* */ "CK_DEBUG"_Stz,        // CK_DEBUG
  /* */ "CK_DECK"_Stz,         // CK_DECK
  /* */ "CK_DIRECTION"_Stz,    // CK_DIRECTION

  /* */ "CK_EXPR"_Stz,         // CK_EXPR

  /* */ "CK_FILTER"_Stz,       // CK_FILTER
  /* */ "CK_FLAT"_Stz,         // CK_FLAT
  /* */ "CK_FOLD"_Stz,         // CK_FOLD
  /* */ "CK_FOLDELIST"_Stz,    // CK_FOLDELIST
  /* */ "CK_FROM"_Stz,         // CK_FROM

  /* */ "CK_GBYAGG"_Stz,       // CK_GBYAGG

  /* */ "CK_HASH"_Stz,         // CK_HASH
  /* */ "CK_HAVING"_Stz,       // CK_HAVING
  /* */ "CK_HSHCOL"_Stz,       // CK_HSHCOL

  /* */ "CK_IDXCOL"_Stz,       // CK_IDXCOL
  /* */ "CK_IDXCOLLIST"_Stz,   // CK_IDXCOLLIST
  /* */ "CK_INDEX"_Stz,        // CK_INDEX
  /* */ "CK_INDEXJOIN"_Stz,    // CK_INDEXJOIN
  /* */ "CK_INDEXLIST"_Stz,    // CK_INDEXLIST
  /* */ "CK_INDEXNM"_Stz,      // CK_INDEXNM
  /* */ "CK_INDEXSCAN"_Stz,    // CK_INDEXSCAN

  /* */ "CK_JOINHEAD"_Stz,     // CK_JOINHEAD
  /* */ "CK_JOINNEXT"_Stz,     // CK_JOINNEXT
  /* */ "CK_JOINORDER"_Stz,    // CK_JOINORDER
  /* */ "CK_JOINORDERING"_Stz, // CK_JOINORDERING
  /* */ "CK_JOINPERMALL"_Stz,  // CK_JOINPERMALL
  /* */ "CK_JOINPERMSEQ"_Stz,  // CK_JOINPERMSEQ
  /* */ "CK_JOINPREDLIST"_Stz, // CK_JOINPREDLIST

  /* */ "CK_KEY"_Stz,          // CK_KEY
  /* */ "CK_KEYEXISTS"_Stz,    // CK_KEYEXISTS
  /* */ "CK_KEYNOTEXISTS"_Stz, // CK_KEYNOTEXISTS

  /* */ "CK_LEVEL"_Stz,        // CK_LEVEL
  /* */ "CK_LIMIT"_Stz,        // CK_LIMIT
  /* */ "CK_LOADJOINED"_Stz,   // CK_LOADJOINED
  /* */ "CK_LOADNETWORK"_Stz,  // CK_LOADNETWORK
  /* */ "CK_LOADUNICAST"_Stz,  // CK_LOADUNICAST

  /* */ "CK_NCORRCOLS"_Stz,    // CK_NCORRCOLS
  /* */ "CK_NEEDED"_Stz,       // CK_NEEDED
  /* */ "CK_NEXT"_Stz,         // CK_NEXT
  /* */ "CK_NOTE"_Stz,         // CK_NOTE
  /* */ "CK_NOTPRED"_Stz,      // CK_NOTPRED

  /* */ "CK_ON"_Stz,           // CK_ON
  /* */ "CK_ORDAGG"_Stz,       // CK_ORDAGG
  /* */ "CK_ORPRED"_Stz,       // CK_ORPRED
  /* */ "CK_OUTERCOLNM"_Stz,   // CK_OUTERCOLNM
  /* */ "CK_OUTERJOIN"_Stz,    // CK_OUTERJOIN

  /* */ "CK_PROJECT"_Stz,      // CK_PROJECT
  /* */ "CK_PROJECTION"_Stz,   // CK_PROJECTION

  /* */ "CK_RPNCALC"_Stz,      // CK_RPNCALC
  /* */ "CK_RPNTERMS"_Stz,     // CK_RPNTERMS

  /* */ "CK_SCAN"_Stz,         // CK_SCAN
  /* */ "CK_SECTION"_Stz,      // CK_SECTION
  /* */ "CK_SEND"_Stz,         // CK_SEND
  /* */ "CK_SENDKIND"_Stz,     // CK_SENDKIND
  /* */ "CK_SIZE"_Stz,         // CK_SIZE
  /* */ "CK_SORT"_Stz,         // CK_SORT
  /* */ "CK_STAGE"_Stz,        // CK_STAGE
  /* */ "CK_SUBSET"_Stz,       // CK_SUBSET

  /* */ "CK_TABLE"_Stz,        // CK_TABLE
  /* */ "CK_TABLELIST"_Stz,    // CK_TABLELIST
  /* */ "CK_TABLENM"_Stz,      // CK_TABLENM
  /* */ "CK_TABLEPTN"_Stz,     // CK_TABLEPTN
  /* */ "CK_TBLSYN"_Stz,       // CK_TBLSYN
  /* */ "CK_TERM"_Stz,         // CK_TERM
  /* */ "CK_TOP"_Stz,          // CK_TOP
  /* */ "CK_TOPOLOGY"_Stz,     // CK_TOPOLOGY
  /* */ "CK_TYPE"_Stz,         // CK_TYPE

  /* */ "CK_UNIQUE"_Stz,       // CK_UNIQUE

  /* */ "CK_WHERE"_Stz,        // CK_WHERE

  /* */ "ENUMS"_Stz
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static Stz* aCK_short[]
{
  /* */ "NONE"_Stz,

  /* */ "absJoin"_Stz,         // CK_ABSJOIN
  /* */ "aggreg"_Stz,          // CK_AGGREG
  /* */ "andPred"_Stz,         // CK_ANDPRED
  /* */ "as"_Stz,              // CK_AS

  /* */ "boolCalc"_Stz,        // CK_BOOLCALC
  /* */ "bound"_Stz,           // CK_BOUND
  /* */ "bounds"_Stz,          // CK_BOUNDS

  /* */ "call"_Stz,            // CK_CALL
  /* */ "cast"_Stz,            // CK_CAST
  /* */ "cmpPred"_Stz,         // CK_CMPPRED
  /* */ "combined"_Stz,        // CK_COMBINED
  /* */ "constraint"_Stz,      // CK_CONSTRAINT
  /* */ "correlation"_Stz,     // CK_CORRELATION
  /* */ "costQuery"_Stz,       // CK_COSTQUERY
  /* */ "costNext"_Stz,        // CK_COSTNEXT
  /* */ "crossJoin"_Stz,       // CK_CROSSJOIN

  /* */ "debug"_Stz,           // CK_DEBUG
  /* */ "deck"_Stz,            // CK_DECK
  /* */ "direction"_Stz,       // CK_DIRECTION

  /* */ "expr"_Stz,            // CK_EXPR

  /* */ "filter"_Stz,          // CK_FILTER
  /* */ "flat"_Stz,            // CK_FLAT
  /* */ "fold"_Stz,            // CK_FOLD
  /* */ "foldElist"_Stz,       // CK_FOLDELIST
  /* */ "from"_Stz,            // CK_FROM

  /* */ "gbyAgg"_Stz,          // CK_GBYAGG

  /* */ "hash"_Stz,            // CK_HASH
  /* */ "having"_Stz,          // CK_HAVING
  /* */ "hshCol"_Stz,          // CK_HSHCOL

  /* */ "idxCol"_Stz,          // CK_IDXCOL
  /* */ "idxColList"_Stz,      // CK_IDXCOLLIST
  /* */ "index"_Stz,           // CK_INDEX
  /* */ "indexJoin"_Stz,       // CK_INDEXJOIN
  /* */ "indexList"_Stz,       // CK_INDEXLIST
  /* */ "indexNm"_Stz,         // CK_INDEXNM
  /* */ "indexScan"_Stz,       // CK_INDEXSCAN

  /* */ "joinHead"_Stz,        // CK_JOINHEAD
  /* */ "joinNext"_Stz,        // CK_JOINNEXT
  /* */ "joinOrder"_Stz,       // CK_JOINORDER
  /* */ "joinOrdering"_Stz,    // CK_JOINORDERING
  /* */ "joinPermAll"_Stz,     // CK_JOINPERMALL
  /* */ "joinPermSeq"_Stz,     // CK_JOINPERMSEQ
  /* */ "joinPredList"_Stz,    // CK_JOINPREDLIST

  /* */ "key"_Stz,             // CK_KEY
  /* */ "keyExists"_Stz,       // CK_KEYEXISTS
  /* */ "keyNotExists"_Stz,    // CK_KEYNOTEXISTS

  /* */ "level"_Stz,           // CK_LEVEL
  /* */ "limit"_Stz,           // CK_LIMIT
  /* */ "loadJoined"_Stz,      // CK_LOADJOINED
  /* */ "loadNetwork"_Stz,     // CK_LOADNETWORK
  /* */ "loadUnicast"_Stz,     // CK_LOADUNICAST

  /* */ "nCorrCols"_Stz,       // CK_NCORRCOLS
  /* */ "needed"_Stz,          // CK_NEEDED
  /* */ "next"_Stz,            // CK_NEXT
  /* */ "note"_Stz,            // CK_NOTE
  /* */ "notPred"_Stz,         // CK_NOTPRED

  /* */ "on"_Stz,              // CK_ON
  /* */ "ordAgg"_Stz,          // CK_ORDAGG
  /* */ "orPred"_Stz,          // CK_ORPRED
  /* */ "outerColNm"_Stz,      // CK_OUTERCOLNM
  /* */ "outerJoin"_Stz,       // CK_OUTERJOIN

  /* */ "project"_Stz,         // CK_PROJECT
  /* */ "projection"_Stz,      // CK_PROJECTION

  /* */ "rpnCalc"_Stz,         // CK_RPNCALC
  /* */ "rpnTerms"_Stz,        // CK_RPNTERMS

  /* */ "scan"_Stz,            // CK_SCAN
  /* */ "section"_Stz,         // CK_SECTION
  /* */ "sendSE"_Stz,          // CK_SEND
  /* */ "sendKind"_Stz,        // CK_SENDKIND
  /* */ "size"_Stz,            // CK_SIZE
  /* */ "sort"_Stz,            // CK_SORT
  /* */ "stage"_Stz,           // CK_STAGE
  /* */ "subSet"_Stz,          // CK_SUBSET

  /* */ "table"_Stz,           // CK_TABLE
  /* */ "tableList"_Stz,       // CK_TABLELIST
  /* */ "tableNm"_Stz,         // CK_TABLENM
  /* */ "tablePtn"_Stz,        // CK_TABLEPTN
  /* */ "tblSyn"_Stz,          // CK_TBLSYN
  /* */ "term"_Stz,            // CK_TERM
  /* */ "top"_Stz,             // CK_TOP
  /* */ "topology"_Stz,        // CK_TOPOLOGY
  /* */ "type"_Stz,            // CK_TYPE

  /* */ "unique"_Stz,          // CK_UNIQUE

  /* */ "where"_Stz,           // CK_WHERE

  /* */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
