/*============================================================================*/
/* eCK.h -- Card Kind                                                         */
/*============================================================================*/
#pragma once

enum eCK // card kind
{
  /* */ CK_NONE=0,

  /* */ CK_ABSJOIN,
  /* */ CK_AGGREG,
  /* */ CK_AGGREGATES,
  /* */ CK_ANDPRED,
  /* */ CK_AS,

  /* */ CK_BOOLCALC,
  /* */ CK_BOUND,
  /* */ CK_BOUNDS,

  /* */ CK_CALL,
  /* */ CK_CANDINDEX,
  /* */ CK_CAST,
  /* */ CK_CMPPRED,
  /* */ CK_COLNM,
  /* */ CK_COMBINED,
  /* */ CK_COMMENT,
  /* */ CK_CONSTRAINT,
  /* */ CK_COPTNJOIN,
  /* */ CK_COPTNJOINLIST,
  /* */ CK_COPTNPRED,
  /* */ CK_COPTNPREDLIST,
  /* */ CK_CORRELATION,
  /* */ CK_CROSSJOIN,
  /* */ CK_CROSSPTNJOIN,
  /* */ CK_CROSSPTNJOINLIST,
  /* */ CK_CROSSPTNPRED,

  /* */ CK_DEBUG,
  /* */ CK_DECK,
  /* */ CK_DIRECTION,

  /* */ CK_EXPR,
  /* */ CK_EXPRBOOL,
  /* */ CK_EXPRLIST,

  /* */ CK_FILTER,
  /* */ CK_FLAT,
  /* */ CK_FOLD,
  /* */ CK_FOLDELIST,
  /* */ CK_FROM,

  /* */ CK_GBYAGG,

  /* */ CK_HASH,
  /* */ CK_HAVING,

  /* */ CK_IDXCOL,
  /* */ CK_IDXCOLLIST,
  /* */ CK_IDXCOLNM,
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
  /* */ CK_LOAD,
  /* */ CK_LOADJOINED,
  /* */ CK_LOADNETWORK,
  /* */ CK_LOADPERMSEQ,
  /* */ CK_LOADUNICAST,

  /* */ CK_NCORRCOLS,
  /* */ CK_NEXT,
  /* */ CK_NOTE,
  /* */ CK_NOTPRED,
  /* */ CK_NPARMCOLS,

  /* */ CK_ON,
  /* */ CK_OPTION,
  /* */ CK_ORDAGG,
  /* */ CK_ORDELMODN,
  /* */ CK_ORDER,
  /* */ CK_ORPRED,
  /* */ CK_OUTERCOLNM,
  /* */ CK_OUTERJOIN,

  /* */ CK_PARTITION,
  /* */ CK_PERMUTATION,
  /* */ CK_PROJECT,
  /* */ CK_PROJECTION,
  /* */ CK_PTNCOL,
  /* */ CK_PTNCOLLIST,
  /* */ CK_PTNCOLNM,
  /* */ CK_PTNDJOIN,
  /* */ CK_PTNDJOINSET,

  /* */ CK_REASON,
  /* */ CK_RPNCALC,
  /* */ CK_RPNTERMS,

  /* */ CK_SCAN,
  /* */ CK_SECTION,
  /* */ CK_SEND,
  /* */ CK_SENDKIND,
  /* */ CK_SIZE,
  /* */ CK_SORT,
  /* */ CK_STAGE,
  /* */ CK_STMCOL,
  /* */ CK_STMCOLNM,
  /* */ CK_STMCOLODN,
  /* */ CK_SUBQUERY,
  /* */ CK_SUBSET,

  /* */ CK_TABLE,
  /* */ CK_TABLELIST,
  /* */ CK_TABLENM,
  /* */ CK_TABLEPTN,
  /* */ CK_TBLCOLODN,
  /* */ CK_TBLSYN,
  /* */ CK_TERM,
  /* */ CK_TOP,
  /* */ CK_TOPOLOGY,
  /* */ CK_TYPE,

  /* */ CK_UNIQUE,

  /* */ CK_WHERE,

  /* */ CK_AV_AGGP,
  /* */ CK_AV_AGGPN,
  /* */ CK_AV_AGGPNO,
  /* */ CK_AV_AGGREGP,
  /* */ CK_AV_AGGREGPN,
  /* */ CK_AV_AGGREGPNO,
  /* */ CK_AV_FILTER,
  /* */ CK_AV_FILTERPROJECT,
  /* */ CK_AV_GBYAGGP,
  /* */ CK_AV_GBYAGGPN,
  /* */ CK_AV_GBYPNO,
  /* */ CK_AV_JOINSERIES,
  /* */ CK_AV_LIMIT,
  /* */ CK_AV_PROJECT,
  /* */ CK_AV_SCANJOINSERIES,
  /* */ CK_AV_SCANONLY,
  /* */ CK_AV_SORT,

  /* */ CK_ENUMS
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static Stz* aCK[]
{
  /* */ "NONE"_Stz,

  /* */ "CK_ABSJOIN"_Stz,      // CK_ABSJOIN
  /* */ "CK_AGGREG"_Stz,       // CK_AGGREG
  /* */ "CK_AGGREGATES"_Stz,   // CK_AGGREGATES
  /* */ "CK_ANDPRED"_Stz,      // CK_ANDPRED
  /* */ "CK_AS"_Stz,           // CK_AS

  /* */ "CK_BOOLCALC"_Stz,     // CK_BOOLCALC
  /* */ "CK_BOUND"_Stz,        // CK_BOUND
  /* */ "CK_BOUNDS"_Stz,       // CK_BOUNDS

  /* */ "CK_CALL"_Stz,         // CK_CALL
  /* */ "CK_CANDINDEX"_Stz,    // CK_CANDINDEX
  /* */ "CK_CAST"_Stz,         // CK_CAST
  /* */ "CK_CMPPRED"_Stz,      // CK_CMPPRED
  /* */ "CK_COMBINED"_Stz,     // CK_COMBINED
  /* */ "CK_COLNM"_Stz,        // CK_COLNM
  /* */ "CK_COMMENT"_Stz,      // CK_COMMENT
  /* */ "CK_CONSTRAINT"_Stz,   // CK_CONSTRAINT
  /* */ "CK_COPTNJOIN"_Stz,    // CK_COPTNJOIN
  /* */ "CK_COPTNJOINLIST"_Stz,// CK_COPTNJOINLIST
  /* */ "CK_COPTNPRED"_Stz,    // CK_COPTNPRED
  /* */ "CK_COPTNPREDLIST"_Stz,// CK_COPTNPREDLIST
  /* */ "CK_CORRELATION"_Stz,    // CK_CORRELATION
  /* */ "CK_CROSSJOIN"_Stz,    // CK_CROSSJOIN
  /* */ "CK_CROSSPTNJOIN"_Stz,     // CK_CROSSPTNJOIN
  /* */ "CK_CROSSPTNJOINLIST"_Stz, // CK_CROSSPTNJOINLIST
  /* */ "CK_CROSSPTNPRED"_Stz, // CK_CROSSPTNPRED

  /* */ "CK_DEBUG"_Stz,        // CK_DEBUG
  /* */ "CK_DECK"_Stz,         // CK_DECK
  /* */ "CK_DIRECTION"_Stz,    // CK_DIRECTION

  /* */ "CK_EXPR"_Stz,         // CK_EXPR
  /* */ "CK_EXPRBOOL"_Stz,     // CK_EXPRBOOL
  /* */ "CK_EXPRLIST"_Stz,     // CK_EXPRLIST

  /* */ "CK_FILTER"_Stz,       // CK_FILTER
   /* */ "CK_FLAT"_Stz,         // CK_FLAT
  /* */ "CK_FOLD"_Stz,         // CK_FOLD
  /* */ "CK_FOLDELIST"_Stz,    // CK_FOLDELIST
  /* */ "CK_FROM"_Stz,         // CK_FROM

  /* */ "CK_GBYAGG"_Stz,       // CK_GBYAGG

  /* */ "CK_HASH"_Stz,         // CK_HASH
  /* */ "CK_HAVING"_Stz,       // CK_HAVING

  /* */ "CK_IDXCOL"_Stz,       // CK_IDXCOL
  /* */ "CK_IDXCOLLIST"_Stz,   // CK_IDXCOLLIST
  /* */ "CK_IDXCOLNM"_Stz,     // CK_IDXCOLNM
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
  /* */ "CK_LOAD"_Stz,         // CK_LOAD
  /* */ "CK_LOADJOINED"_Stz,   // CK_LOADJOINED
  /* */ "CK_LOADNETWORK"_Stz,  // CK_LOADNETWORK
  /* */ "CK_LOADPERMSEQ"_Stz,  // CK_LOADPERMSEQ
  /* */ "CK_LOADUNICAST"_Stz,  // CK_LOADUNICAST

  /* */ "CK_NCORRCOLS"_Stz,    // CK_NCORRCOLS
  /* */ "CK_NEXT"_Stz,         // CK_NEXT
  /* */ "CK_NOTE"_Stz,         // CK_NOTE
  /* */ "CK_NOTPRED"_Stz,      // CK_NOTPRED
  /* */ "CK_NPARMCOLS"_Stz,    // CK_NPARMCOLS

  /* */ "CK_ON"_Stz,           // CK_ON
  /* */ "CK_OPTION"_Stz,       // CK_OPTION
  /* */ "CK_ORDAGG"_Stz,       // CK_ORDAGG
  /* */ "CK_ORDELMODN"_Stz,    // CK_ORDELMODN
  /* */ "CK_ORDER"_Stz,        // CK_ORDER
  /* */ "CK_ORPRED"_Stz,       // CK_ORPRED
  /* */ "CK_OUTERCOLNM"_Stz,   // CK_OUTERCOLNM
  /* */ "CK_OUTERJOIN"_Stz,    // CK_OUTERJOIN

  /* */ "CK_PARTITION"_Stz,    // CK_PARTITION
  /* */ "CK_PERMUTATION"_Stz,  // CK_PERMUTATION
  /* */ "CK_PROJECT"_Stz,      // CK_PROJECT
  /* */ "CK_PROJECTION"_Stz,   // CK_PROJECTION
  /* */ "CK_PTNCOL"_Stz,       // CK_PTNCOL
  /* */ "CK_PTNCOLLIST"_Stz,   // CK_PTNCOLLIST
  /* */ "CK_PTNCOLNM"_Stz,     // CK_PTNCOLNM
  /* */ "CK_PTNDJOIN"_Stz,     // CK_PTNDJOIN
  /* */ "CK_PTNDJOINSET"_Stz,  // CK_PTNDJOINSET

  /* */ "CK_REASON"_Stz,       // CK_REASON
  /* */ "CK_RPNCALC"_Stz,      // CK_RPNCALC
  /* */ "CK_RPNTERMS"_Stz,     // CK_RPNTERMS

  /* */ "CK_SCAN"_Stz,         // CK_SCAN
  /* */ "CK_SECTION"_Stz,      // CK_SECTION
  /* */ "CK_SEND"_Stz,         // CK_SEND
  /* */ "CK_SENDKIND"_Stz,     // CK_SENDKIND
  /* */ "CK_SIZE"_Stz,         // CK_SIZE
  /* */ "CK_SORT"_Stz,         // CK_SORT
  /* */ "CK_STAGE"_Stz,        // CK_STAGE
  /* */ "CK_STMCOL"_Stz,       // CK_STMCOL
  /* */ "CK_STMCOLNM"_Stz,     // CK_STMCOLNM
  /* */ "CK_STMCOLODN"_Stz,    // CK_STMCOLODN
  /* */ "CK_SUBQUERY"_Stz,     // CK_SUBQUERY
  /* */ "CK_SUBSET"_Stz,       // CK_SUBSET

  /* */ "CK_TABLE"_Stz,        // CK_TABLE
  /* */ "CK_TABLELIST"_Stz,    // CK_TABLELIST
  /* */ "CK_TABLENM"_Stz,      // CK_TABLENM
  /* */ "CK_TABLEPTN"_Stz,     // CK_TABLEPTN
  /* */ "CK_TBLCOLODN"_Stz,    // CK_TBLCOLODN
  /* */ "CK_TBLSYN"_Stz,       // CK_TBLSYN
  /* */ "CK_TERM"_Stz,         // CK_TERM
  /* */ "CK_TOP"_Stz,          // CK_TOP
  /* */ "CK_TOPOLOGY"_Stz,     // CK_TOPOLOGY
  /* */ "CK_TYPE"_Stz,         // CK_TYPE

  /* */ "CK_UNIQUE"_Stz,       // CK_UNIQUE

  /* */ "CK_WHERE"_Stz,        // CK_WHERE

  /* */ "CK_AV_AGGP"_Stz,      // CK_AV_AGGP
  /* */ "CK_AV_AGGPN"_Stz,     // CK_AV_AGGPN
  /* */ "CK_AV_AGGPNO"_Stz,    // CK_AV_AGGPNO
  /* */ "CK_AV_AGGREGP"_Stz,   // CK_AV_AGGREGP
  /* */ "CK_AV_AGGREGPN"_Stz,  // CK_AV_AGGREGPN
  /* */ "CK_AV_AGGREGPNO"_Stz, // CK_AV_AGGREGPNO
  /* */ "CK_AV_FILTER"_Stz,    // CK_AV_FILTER
  /* */ "CK_AV_FILTERPROJECT"_Stz, // CK_AV_FILTERPROJECT
  /* */ "CK_AV_GBYAGGP"_Stz,   // CK_AV_GBYAGGP
  /* */ "CK_AV_GBYAGGPN"_Stz,  // CK_AV_GBYAGGPN
  /* */ "CK_AV_GBYPNO"_Stz,    // CK_AV_GBYPNO
  /* */ "CK_AV_JOINSERIES"_Stz,// CK_AV_JOINSERIES
  /* */ "CK_AV_LIMIT"_Stz,     // CK_AV_LIMIT
  /* */ "CK_AV_PROJECT"_Stz,   // CK_AV_PROJECT
  /* */ "CK_AV_SCANJOINSERIES"_Stz,// CK_AV_SCANJOINSERIES
  /* */ "CK_AV_SCANONLY"_Stz,  // CK_AV_SCANONLY
  /* */ "CK_AV_SORT"_Stz,      // CK_AV_SORT

  /* */ "ENUMS"_Stz
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static Stz* aCK_short[]
{
  /* */ "NONE"_Stz,

  /* */ "absJoin"_Stz,         // CK_ABSJOIN
  /* */ "aggreg"_Stz,          // CK_AGGREG
  /* */ "aggregates"_Stz,      // CK_AGGREGATES
  /* */ "andPred"_Stz,         // CK_ANDPRED
  /* */ "as"_Stz,              // CK_AS

  /* */ "boolCalc"_Stz,        // CK_BOOLCALC
  /* */ "bound"_Stz,           // CK_BOUND
  /* */ "bounds"_Stz,          // CK_BOUNDS

  /* */ "call"_Stz,            // CK_CALL
  /* */ "candIndex"_Stz,       // CK_CANDINDEX
  /* */ "cast"_Stz,            // CK_CAST
  /* */ "cmpPred"_Stz,         // CK_CMPPRED
  /* */ "colNm"_Stz,           // CK_COLNM
  /* */ "combined"_Stz,        // CK_COMBINED
  /* */ "comment"_Stz,         // CK_COMMENT
  /* */ "constraint"_Stz,      // CK_CONSTRAINT
  /* */ "coPtnJoin"_Stz,       // CK_COPTNJOIN
  /* */ "coPtnJoinList"_Stz,   // CK_COPTNJOINLIST
  /* */ "coPtnPred"_Stz,       // CK_COPTNPRED
  /* */ "coPtnPredList"_Stz,   // CK_COPTNPREDLIST
  /* */ "correlation"_Stz,     // CK_CORRELATION
  /* */ "crossJoin"_Stz,       // CK_CROSSJOIN
  /* */ "crossPtnJoin"_Stz,    // CK_CROSSPTNJOIN
  /* */ "crossPtnJoinList"_Stz,// CK_CROSSPTNJOINLIST
  /* */ "crossPtnPred"_Stz,    // CK_CROSSPTNPRED

  /* */ "debug"_Stz,           // CK_DEBUG
  /* */ "deck"_Stz,            // CK_DECK
  /* */ "direction"_Stz,       // CK_DIRECTION

 /* */ "expr"_Stz,            // CK_EXPR
  /* */ "exprBool"_Stz,        // CK_EXPRBOOL
  /* */ "exprList"_Stz,        // CK_EXPRLIST

  /* */ "filter"_Stz,          // CK_FILTER
  /* */ "flat"_Stz,            // CK_FLAT
  /* */ "fold"_Stz,            // CK_FOLD
  /* */ "foldElist"_Stz,       // CK_FOLDELIST
  /* */ "from"_Stz,            // CK_FROM

  /* */ "gbyAgg"_Stz,          // CK_GBYAGG

  /* */ "hash"_Stz,            // CK_HASH
  /* */ "having"_Stz,          // CK_HAVING

  /* */ "idxCol"_Stz,          // CK_IDXCOL
  /* */ "idxColList"_Stz,      // CK_IDXCOLLIST
  /* */ "idxColNm"_Stz,        // CK_IDXCOLNM
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
  /* */ "load"_Stz,            // CK_LOAD
  /* */ "loadJoined"_Stz,      // CK_LOADJOINED
  /* */ "loadNetwork"_Stz,     // CK_LOADNETWORK
  /* */ "loadPermSeq"_Stz,     // CK_LOADPERMSEQ
  /* */ "loadUnicast"_Stz,     // CK_LOADUNICAST

  /* */ "nCorrCols"_Stz,       // CK_NCORRCOLS
  /* */ "next"_Stz,            // CK_NEXT
  /* */ "note"_Stz,            // CK_NOTE
  /* */ "notPred"_Stz,         // CK_NOTPRED
  /* */ "nParmCols"_Stz,       // CK_NPARMCOLS

  /* */ "on"_Stz,              // CK_ON
  /* */ "option"_Stz,          // CK_OPTION
  /* */ "ordAgg"_Stz,          // CK_ORDAGG
  /* */ "ordElmOdn"_Stz,       // CK_ORDELMODN
  /* */ "order"_Stz,           // CK_ORDER
  /* */ "orPred"_Stz,          // CK_ORPRED
  /* */ "outerColNm"_Stz,      // CK_OUTERCOLNM
  /* */ "outerJoin"_Stz,       // CK_OUTERJOIN

  /* */ "partition"_Stz,       // CK_PARTITION
  /* */ "permutation"_Stz,     // CK_PERMUTATION
  /* */ "project"_Stz,         // CK_PROJECT
  /* */ "projection"_Stz,      // CK_PROJECTION
  /* */ "ptnCol"_Stz,          // CK_PTNCOL
  /* */ "ptnColList"_Stz,      // CK_PTNCOLLIST
  /* */ "ptnColNm"_Stz,        // CK_PTNCOLNM
  /* */ "ptndJoin"_Stz,        // CK_PTNDJOIN
  /* */ "ptndJoinSet"_Stz,     // CK_PTNDJOINSET

  /* */ "reason"_Stz,          // CK_REASON
  /* */ "rpnCalc"_Stz,         // CK_RPNCALC
  /* */ "rpnTerms"_Stz,        // CK_RPNTERMS

  /* */ "scan"_Stz,            // CK_SCAN
  /* */ "section"_Stz,         // CK_SECTION
  /* */ "sendSE"_Stz,          // CK_SEND
  /* */ "sendKind"_Stz,        // CK_SENDKIND
  /* */ "size"_Stz,            // CK_SIZE
  /* */ "sort"_Stz,            // CK_SORT
  /* */ "stage"_Stz,           // CK_STAGE
  /* */ "stmCol"_Stz,          // CK_STMCOL
  /* */ "stmColNm"_Stz,        // CK_STMCOLNM
  /* */ "stmColOdn"_Stz,       // CK_STMCOLODN
  /* */ "subQuery"_Stz,        // CK_SUBQUERY
  /* */ "subSet"_Stz,          // CK_SUBSET

  /* */ "table"_Stz,           // CK_TABLE
  /* */ "tableList"_Stz,       // CK_TABLELIST
  /* */ "tableNm"_Stz,         // CK_TABLENM
  /* */ "tablePtn"_Stz,        // CK_TABLEPTN
  /* */ "tblColOdn"_Stz,       // CK_TBLCOLODN
  /* */ "tblSyn"_Stz,          // CK_TBLSYN
  /* */ "term"_Stz,            // CK_TERM
  /* */ "top"_Stz,             // CK_TOP
  /* */ "topology"_Stz,        // CK_TOPOLOGY
  /* */ "type"_Stz,            // CK_TYPE

  /* */ "unique"_Stz,          // CK_UNIQUE

  /* */ "where"_Stz,           // CK_WHERE

  /* */ "av_aggP"_Stz,            // CK_AV_AGGP
  /* */ "av_aggPN"_Stz,           // CK_AV_AGGPN
  /* */ "av_aggPNO"_Stz,          // CK_AV_AGGPNO
  /* */ "av_aggregP"_Stz,         // CK_AV_AGGREGP
  /* */ "av_aggregPN"_Stz,        // CK_AV_AGGREGPN
  /* */ "av_aggregPNO"_Stz,       // CK_AV_AGGREGPNO
  /* */ "av_filter"_Stz,          // CK_AV_FILTER
  /* */ "av_filterProject"_Stz,   // CK_AV_FILTERPROJECT
  /* */ "av_gbyAggP"_Stz,         // CK_AV_GBYAGGP
  /* */ "av_gbyAggPN"_Stz,        // CK_AV_GBYAGGPN
  /* */ "av_gbyPNO"_Stz,          // CK_AV_GBYPNO
  /* */ "av_joinSeries"_Stz,      // CK_AV_JOINSERIES
  /* */ "av_limit"_Stz,           // CK_AV_LIMIT
  /* */ "av_project"_Stz,         // CK_AV_PROJECT
  /* */ "av_scanJoinSeries"_Stz,  // CK_AV_SCANJOINSERIES
  /* */ "av_scanOnly"_Stz,        // CK_AV_SCANONLY
  /* */ "av_sort"_Stz,            // CK_AV_SORT

  /* */ "ENUMS"_Stz
};


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static Stz* aCK_notation[]
{
    /* */ "NONE"_Stz,

    /* */ "aJ"_Stz,           // CK_ABSJOIN
    /* */ "aR"_Stz,           // CK_AGGREG
    /* */ "aggR"_Stz,         // CK_AGGREGATES
    /* */ "aP"_Stz,           // CK_ANDPRED
    /* */ "as"_Stz,           // CK_AS

    /* */ "bC"_Stz,           // CK_BOOLCALC
    /* */ "b"_Stz,            // CK_BOUNDS

    /* */ "call"_Stz,         // CK_CALL
    /* */ "cI"_Stz,           // CK_CANDINDEX
    /* */ "cast"_Stz,         // CK_CAST
    /* */ "cP"_Stz,           // CK_CMPPRED
    /* */ "cmb"_Stz,          // CK_COMBINED
    /* */ "cNm"_Stz,          // CK_COLNM
    /* */ "cmt"_Stz,          // CK_COMMENT
    /* */ "constraint"_Stz,   // CK_CONSTRAINT
    /* */ "cPJ"_Stz,          // CK_COPTNJOIN
    /* */ "cPJL"_Stz,         // CK_COPTNJOINLIST
    /* */ "cPP"_Stz,          // CK_COPTNPRED
    /* */ "cPPL"_Stz,         // CK_COPTNPREDLIST
    /* */ "corL"_Stz,         // CK_CORRELATION
    /* */ "cJ"_Stz,           // CK_CROSSJOIN
    /* */ "crPJ"_Stz,         // CK_CROSSPTNJOIN
    /* */ "crPJL"_Stz,        // CK_CROSSPTNJOINLIST
    /* */ "crPP"_Stz,         // CK_CROSSPTNPRED

    /* */ "dbug"_Stz,         // CK_DEBUG
    /* */ "d"_Stz,            // CK_DECK
    /* */ "dir"_Stz,          // CK_DIRECTION

    /* */ "ex"_Stz,           // CK_EXPR
    /* */ "exB"_Stz,          // CK_EXPRBOOL
    /* */ "exL"_Stz,          // CK_EXPRLIST

    /* */ "f"_Stz,            // CK_FILTER
    /* */ "fl"_Stz,           // CK_FLAT
    /* */ "fo"_Stz,           // CK_FOLD
    /* */ "foEL"_Stz,         // CK_FOLDELIST
    /* */ "frm"_Stz,          // CK_FROM

    /* */ "gA"_Stz,           // CK_GBYAGG

    /* */ "h"_Stz,            // CK_HASH
    /* */ "ha"_Stz,           // CK_HAVING

    /* */ "iC"_Stz,           // CK_IDXCOL
    /* */ "iCL"_Stz,          // CK_IDXCOLLIST
    /* */ "iCN"_Stz,          // CK_IDXCOLNM
    /* */ "i"_Stz,            // CK_INDEX
    /* */ "iJ"_Stz,           // CK_INDEXJOIN
    /* */ "iL"_Stz,           // CK_INDEXLIST
    /* */ "iN"_Stz,           // CK_INDEXNM
    /* */ "iS"_Stz,           // CK_INDEXSCAN

    /* */ "jH"_Stz,           // CK_JOINHEAD
    /* */ "jN"_Stz,           // CK_JOINNEXT
    /* */ "jO"_Stz,           // CK_JOINORDER
    /* */ "jOG"_Stz,          // CK_JOINORDERING
    /* */ "jPA"_Stz,          // CK_JOINPERMALL
    /* */ "jPO"_Stz,          // CK_JOINPERMSEQ
    /* */ "jPeL"_Stz,         // CK_JOINPREDLIST

    /* */ "key"_Stz,          // CK_KEY
    /* */ "kE"_Stz,           // CK_KEYEXISTS
    /* */ "kNE"_Stz,          // CK_KEYNOTEXISTS

    /* */ "lvl"_Stz,          // CK_LEVEL
    /* */ "lmt"_Stz,          // CK_LIMIT
    /* */ "ld"_Stz ,          // CK_LOAD
    /* */ "ldJd"_Stz,         // CK_LOADJOINED
    /* */ "ldN"_Stz,          // CK_LOADNETWORK
    /* */ "ldP"_Stz,          // CK_LOADPERMSEQ
    /* */ "ldU"_Stz,          // CK_LOADUNICAST

    /* */ "nCC"_Stz,          // CK_NCORRCOLS
    /* */ "n"_Stz,            // CK_NEXT
    /* */ "note"_Stz,         // CK_NOTE
    /* */ "nPC"_Stz,          // CK_NPARMCOLS

     /* */ "on"_Stz,           // CK_ON
    /* */ "opt"_Stz,          // CK_OPTION
    /* */ "oA"_Stz,           // CK_ORDAGG
    /* */ "sOO"_Stz,          // CK_ORDELMODN
    /* */ "sOL"_Stz,          // CK_ORDER
    /* */ "oP"_Stz,           // CK_ORPRED
    /* */ "outCol"_Stz,       // CK_OUTERCOLNM
    /* */ "outJoin"_Stz,      // CK_OUTERJOIN

    /* */ "prt"_Stz,          // CK_PARTITION
    /* */ "pmut"_Stz,         // CK_PERMUTATION
    /* */ "pj"_Stz,           // CK_PROJECT
    /* */ "pjtn"_Stz,         // CK_PROJECTION
    /* */ "pC"_Stz,           // CK_PTNCOL
    /* */ "pCL"_Stz,          // CK_PTNCOLLIST
    /* */ "pCN"_Stz,          // CK_PTNCOLNM
    /* */ "pJ"_Stz,           // CK_PTNDJOIN
    /* */ "pJS"_Stz,          // CK_PTNDJOINSET

    /* */ "rE"_Stz,           // CK_REASON
    /* */ "rC"_Stz,           // CK_RPNCALC
    /* */ "rT"_Stz,           // CK_RPNTERMS

    /* */ "s"_Stz,            // CK_SCAN
    /* */ "sect"_Stz,         // CK_ECTION
    /* */ "sSE"_Stz,          // CK_SEND
    /* */ "sK"_Stz,           // CK_SENDKIND
    /* */ "sz"_Stz,           // CK_SIZE
    /* */ "sort"_Stz,         // CK_SORT
    /* */ "st"_Stz,           // CK_STAGE
    /* */ "sC"_Stz,           // CK_STMCOL
    /* */ "sCN"_Stz,          // CK_STMCOLNM
    /* */ "sCO"_Stz,          // CK_STMCOLODN
    /* */ "sQ"_Stz,           // CK_SUBQUERY
    /* */ "sS"_Stz,           // CK_SUBSET

    /* */ "t"_Stz,           // CK_TABLE
    /* */ "tL"_Stz,          // CK_TABLELIST
    /* */ "tN"_Stz,          // CK_TABLENM
    /* */ "tP"_Stz,          // CK_TABLEPTN
    /* */ "tCO"_Stz,         // CK_TBLCOLODN
    /* */ "tSyn"_Stz,        // CK_TBLSYN
    /* */ "trm"_Stz,         // CK_TERM
    /* */ "top"_Stz,         // CK_TOP
    /* */ "tplgy"_Stz,       // CK_TOPOLOGY
    /* */ "ty"_Stz,          // CK_TYPE

    /* */ "u"_Stz,           // CK_UNIQUE

    /* */ "w"_Stz,           // CK_WHERE


    /* */ "av_aP"_Stz,          // CK_AV_AGGP
    /* */ "av_aPN"_Stz,         // CK_AV_AGGPN
    /* */ "av_aPNO"_Stz,        // CK_AV_AGGPNO
    /* */ "av_aggRP"_Stz,       // CK_AV_AGGREGP
    /* */ "av_aggRPN"_Stz,      // CK_AV_AGGREGPN
    /* */ "av_aggRPNO"_Stz,     // CK_AV_AGGREGPNO
    /* */ "av_f"_Stz,           // CK_AV_FILTER
    /* */ "av_fP"_Stz,          // CK_AV_FILTERPROJECT
    /* */ "av_gAP"_Stz,         // CK_AV_GBYAGGP
    /* */ "av_gAPN"_Stz,        // CK_AV_GBYAGGPN
    /* */ "av_gPNO"_Stz,        // CK_AV_GBYPNO
    /* */ "av_jS"_Stz,          // CK_AV_JOINSERIES
    /* */ "av_l"_Stz,           // CK_AV_LIMIT
    /* */ "av_pj"_Stz,          // CK_AV_PROJECT
    /* */ "av_sJS"_Stz,         // CK_AV_SCANJOINSERIES
    /* */ "av_sO"_Stz,          // CK_AV_SCANONLY
    /* */ "av_sort"_Stz,        // CK_AV_SORT

    /* */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
