/*============================================================================*/
/* eCK.cpp -- Card Kind                                                       */
/*============================================================================*/
#include "BASE.h"
#include "eCK.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Stz* aCK[CK_ENUMS]
{
  /* */ "NONE"_Stz,

  /* */ "CK_ABSJOIN"_Stz,      // CK_ABSJOIN
  /* */ "CK_ADDRESS"_Stz,      // CK_ADDRESS
  /* */ "CK_AGGREG"_Stz,       // CK_AGGREG
  /* */ "CK_ANDCONJ"_Stz,      // CK_ANDCONJ
  /* */ "CK_ARRIVING"_Stz,     // CK_ARRIVING
  /* */ "CK_AS"_Stz,           // CK_AS

  /* */ "CK_BOOLCALC"_Stz,     // CK_BOOLCALC
  /* */ "CK_BOUND"_Stz,        // CK_BOUND
  /* */ "CK_BOUNDS"_Stz,       // CK_BOUNDS

  /* */ "CK_CALL"_Stz,         // CK_CALL
  /* */ "CK_CALLER"_Stz,       // CK_CALLER
  /* */ "CK_CALLOP"_Stz,       // CK_CALLOP
  /* */ "CK_CAST"_Stz,         // CK_CAST
  /* */ "CK_CLAUSE"_Stz,       // CK_CLAUSE
  /* */ "CK_CMPCONJ"_Stz,      // CK_CMPCONJ
  /* */ "CK_COMBINED"_Stz,     // CK_COMBINED
  /* */ "CK_CONSTRAINT"_Stz,   // CK_CONSTRAINT
  /* */ "CK_CORRCOLS"_Stz,     // CK_CORRCOLS
  /* */ "CK_CORRELATION"_Stz,  // CK_CORRELATION
  /* */ "CK_COSTJOINING"_Stz,  // CK_COSTJOINING
  /* */ "CK_COSTNETWORK"_Stz,  // CK_COSTNETWORK
  /* */ "CK_COSTSTREAM"_Stz,   // CK_COSTSTREAM
  /* */ "CK_COSTUNICAST"_Stz,  // CK_COSTUNICAST
  /* */ "CK_COUNT"_Stz,        // CK_COUNT
  /* */ "CK_CROSSJOIN"_Stz,    // CK_CROSSJOIN

  /* */ "CK_DEBUG"_Stz,        // CK_DEBUG
  /* */ "CK_DECK"_Stz,         // CK_DECK
  /* */ "CK_DECISION"_Stz,     // CK_DECISION
  /* */ "CK_DIRECTION"_Stz,    // CK_DIRECTION
  /* */ "CK_DISPOSITION"_Stz,  // CK_DISPOSITION

  /* */ "CK_EXPR"_Stz,         // CK_EXPR

  /* */ "CK_FILTER"_Stz,       // CK_FILTER
  /* */ "CK_FILTPRED"_Stz,     // CK_FILTPRED
  /* */ "CK_FILTPREDLIST"_Stz, // CK_FILTPREDLIST
  /* */ "CK_FLAT"_Stz,         // CK_FLAT
  /* */ "CK_FOLD"_Stz,         // CK_FOLD
  /* */ "CK_FROM"_Stz,         // CK_FROM

  /* */ "CK_GBYAGG"_Stz,       // CK_GBYAGG

  /* */ "CK_HASH"_Stz,         // CK_HASH
  /* */ "CK_HASHING"_Stz,      // CK_HASHING
  /* */ "CK_HAVING"_Stz,       // CK_HAVING
  /* */ "CK_HSHCOL"_Stz,       // CK_HSHCOL
  /* */ "CK_HSHCOLLIST"_Stz,   // CK_HSHCOLLIST

  /* */ "CK_IDXCOL"_Stz,       // CK_IDXCOL
  /* */ "CK_IDXCOLLIST"_Stz,   // CK_IDXCOLLIST
  /* */ "CK_INDEX"_Stz,        // CK_INDEX
  /* */ "CK_INDEXJOIN"_Stz,    // CK_INDEXJOIN
  /* */ "CK_INDEXLIST"_Stz,    // CK_INDEXLIST
  /* */ "CK_INDEXNM"_Stz,      // CK_INDEXNM
  /* */ "CK_INDEXSCAN"_Stz,    // CK_INDEXSCAN
  /* */ "CK_INJECTED"_Stz,     // CK_INJECTED

  /* */ "CK_JOINCAST"_Stz,     // CK_JOINCAST
  /* */ "CK_JOINESTIMATE"_Stz, // CK_JOINESTIMATE
  /* */ "CK_JOINORDER"_Stz,    // CK_JOINORDER
  /* */ "CK_JOINORDERING"_Stz, // CK_JOINORDERING
  /* */ "CK_JOINPREDLIST"_Stz, // CK_JOINPREDLIST

  /* */ "CK_KEY"_Stz,          // CK_KEY
  /* */ "CK_KEYCELLS"_Stz,     // CK_KEYCELLS
  /* */ "CK_KEYEXISTS"_Stz,    // CK_KEYEXISTS
  /* */ "CK_KEYNOTEXISTS"_Stz, // CK_KEYNOTEXISTS

  /* */ "CK_LEVEL"_Stz,        // CK_LEVEL
  /* */ "CK_LIMIT"_Stz,        // CK_LIMIT

  /* */ "CK_MAP"_Stz,          // CK_MAP
  /* */ "CK_MAPPING"_Stz,      // CK_MAPPING

  /* */ "CK_NEEDED"_Stz,       // CK_NEEDED
  /* */ "CK_NOTE"_Stz,         // CK_NOTE
  /* */ "CK_NOTCONJ"_Stz,      // CK_NOTCONJ

  /* */ "CK_ON"_Stz,           // CK_ON
  /* */ "CK_ORDAGG"_Stz,       // CK_ORDAGG
  /* */ "CK_ORCONJ"_Stz,       // CK_ORCONJ
  /* */ "CK_OUTERCOLNM"_Stz,   // CK_OUTERCOLNM
  /* */ "CK_OUTERJOIN"_Stz,    // CK_OUTERJOIN

  /* */ "CK_PAYLOADCELLS"_Stz, // CK_PAYLOADCELLS
  /* */ "CK_PROJECT"_Stz,      // CK_PROJECT
  /* */ "CK_PROJECTION"_Stz,   // CK_PROJECTION

  /* */ "CK_RELNOP"_Stz,       // CK_RELNOP
  /* */ "CK_RQUIRED"_Stz,      // CK_REQUIRED
  /* */ "CK_ROWCOUNT"_Stz,     // CK_ROWCOUNT
  /* */ "CK_ROWSIZE"_Stz,      // CK_ROWSIZE
  /* */ "CK_RPNCALC"_Stz,      // CK_RPNCALC
  /* */ "CK_RPNTERMS"_Stz,     // CK_RPNTERMS

  /* */ "CK_SCAN"_Stz,         // CK_SCAN
  /* */ "CK_SECTION"_Stz,      // CK_SECTION
  /* */ "CK_SEND"_Stz,         // CK_SEND
  /* */ "CK_SENDKIND"_Stz,     // CK_SENDKIND
  /* */ "CK_SORT"_Stz,         // CK_SORT
  /* */ "CK_SPARE"_Stz,        // CK_SPARE
  /* */ "CK_SQYCALLS"_Stz,     // CK_SQYCALLS
  /* */ "CK_SQYINFO"_Stz,      // CK_SQYINFO
  /* */ "CK_SQYNO"_Stz,        // CK_SQYNO
  /* */ "CK_STAGE"_Stz,        // CK_STAGE
  /* */ "CK_STREAM"_Stz,       // CK_STREAM
  /* */ "CK_STREAMHEAD"_Stz,   // CK_STREAMHEAD
  /* */ "CK_STREAMJOIN"_Stz,   // CK_STREAMJOIN
  /* */ "CK_STREAMKEY"_Stz,    // CK_STREAMKEY
  /* */ "CK_STREAMPER"_Stz,    // CK_STREAMPER
  /* */ "CK_STREAMPTN"_Stz,    // CK_STREAMPTN
  /* */ "CK_STREAMSET"_Stz,    // CK_STREAMSET
  /* */ "CK_SUBQUERY"_Stz,     // CK_SUBQUERY
  /* */ "CK_SUBSET"_Stz,       // CK_SUBSET
  /* */ "CK_SYNTHESIS"_Stz,   // CK_SYNTHESIS

  /* */ "CK_TABLE"_Stz,        // CK_TABLE
  /* */ "CK_TABLEINFO"_Stz,    // CK_TABLEINFO
  /* */ "CK_TABLELIST"_Stz,    // CK_TABLELIST
  /* */ "CK_TABLENM"_Stz,      // CK_TABLENM
  /* */ "CK_TABLEPTN"_Stz,     // CK_TABLEPTN
  /* */ "CK_TABLESIZE"_Stz,    // CK_TABLESIZE
  /* */ "CK_TBLSYN"_Stz,       // CK_TBLSYN
  /* */ "CK_TERM"_Stz,         // CK_TERM
  /* */ "CK_TOP"_Stz,          // CK_TOP
  /* */ "CK_TOPOLOGY"_Stz,     // CK_TOPOLOGY

  /* */ "CK_UNIQUE"_Stz,       // CK_UNIQUE

  /* */ "CK_WHERE"_Stz,        // CK_WHERE

  /* */ "CK_XCARD"_Stz,        // CK_XCARD

//  /* */ "ENUMS"_Stz
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Stz* aCK_short[CK_ENUMS]
{
  /* */ "NONE"_Stz,

  /* */ "absJoin"_Stz,         // CK_ABSJOIN
  /* */ "address"_Stz,         // CK_ADDRESS
  /* */ "aggreg"_Stz,          // CK_AGGREG
  /* */ "andConj"_Stz,         // CK_ANDCONJ
  /* */ "arriving"_Stz,        // CK_ARRIVING
  /* */ "as"_Stz,              // CK_AS

  /* */ "boolCalc"_Stz,        // CK_BOOLCALC
  /* */ "bound"_Stz,           // CK_BOUND
  /* */ "bounds"_Stz,          // CK_BOUNDS

  /* */ "call"_Stz,            // CK_CALL
  /* */ "caller"_Stz,          // CK_CALLER
  /* */ "callOp"_Stz,          // CK_CALLOP
  /* */ "cast"_Stz,            // CK_CAST
  /* */ "clause"_Stz,          // CK_CLAUSE
  /* */ "cmpPred"_Stz,         // CK_CMPCONJ
  /* */ "combined"_Stz,        // CK_COMBINED
  /* */ "constraint"_Stz,      // CK_CONSTRAINT
  /* */ "corrCols"_Stz,        // CK_CORRCOLS
  /* */ "correlation"_Stz,     // CK_CORRELATIO
  /* */ "costJoining"_Stz,     // CK_COSTJOINING
  /* */ "costNetwork"_Stz,     // CK_COSTNETWORK
  /* */ "costQuery"_Stz,       // CK_COSTSTREAM
  /* */ "costUnicast"_Stz,     // CK_COSTUNICAST
  /* */ "count"_Stz,           // CK_COUNT
  /* */ "crossJoin"_Stz,       // CK_CROSSJOIN

  /* */ "debug"_Stz,           // CK_DEBUG
  /* */ "decision"_Stz,        // CK_DECISION
  /* */ "deck"_Stz,            // CK_DECK
  /* */ "direction"_Stz,       // CK_DIRECTION
  /* */ "disp"_Stz,            // CK_DISPOSITION

  /* */ "expr"_Stz,            // CK_EXPR

  /* */ "filter"_Stz,          // CK_FILTER
  /* */ "filtPred"_Stz,        // CK_FILTPRED
  /* */ "filtPredList"_Stz,    // CK_FILTPREDLIST
  /* */ "flat"_Stz,            // CK_FLAT
  /* */ "fold"_Stz,            // CK_FOLD
  /* */ "from"_Stz,            // CK_FROM

  /* */ "gbyAgg"_Stz,          // CK_GBYAGG

  /* */ "hash"_Stz,            // CK_HASH
  /* */ "hashing"_Stz,         // CK_HASHING
  /* */ "having"_Stz,          // CK_HAVING
  /* */ "hshCol"_Stz,          // CK_HSHCOL
  /* */ "hshColList"_Stz,      // CK_HSHCOLLIST

  /* */ "idxCol"_Stz,          // CK_IDXCOL
  /* */ "idxColList"_Stz,      // CK_IDXCOLLIST
  /* */ "index"_Stz,           // CK_INDEX
  /* */ "indexJoin"_Stz,       // CK_INDEXJOIN
  /* */ "indexList"_Stz,       // CK_INDEXLIST
  /* */ "indexNm"_Stz,         // CK_INDEXNM
  /* */ "indexScan"_Stz,       // CK_INDEXSCAN
  /* */ "injected"_Stz,        // CK_INJECTED

  /* */ "joinCast"_Stz,        // CK_JOINCAST
  /* */ "joinEstimate"_Stz,    // CK_JOINESTIMATE
  /* */ "joinOrder"_Stz,       // CK_JOINORDER
  /* */ "joinOrdering"_Stz,    // CK_JOINORDERING
  /* */ "joinPredList"_Stz,    // CK_JOINPREDLIST

  /* */ "key"_Stz,             // CK_KEY
  /* */ "keyCells"_Stz,        // CK_KEYCELLS
  /* */ "keyExists"_Stz,       // CK_KEYEXISTS
  /* */ "keyNotExists"_Stz,    // CK_KEYNOTEXISTS

  /* */ "level"_Stz,           // CK_LEVEL
  /* */ "limit"_Stz,           // CK_LIMIT

  /* */ "map"_Stz,             // CK_MAP
  /* */ "mapping"_Stz,         // CK_MAPPING

  /* */ "needed"_Stz,          // CK_NEEDED
  /* */ "note"_Stz,            // CK_NOTE
  /* */ "notPred"_Stz,         // CK_NOTCONJ

  /* */ "on"_Stz,              // CK_ON
  /* */ "ordAgg"_Stz,          // CK_ORDAGG
  /* */ "orPred"_Stz,          // CK_ORCONJ
  /* */ "outerColNm"_Stz,      // CK_OUTERCOLNM
  /* */ "outerJoin"_Stz,       // CK_OUTERJOIN

  /* */ "payloadCells"_Stz,    // CK_PAYLOADCELLS
  /* */ "project"_Stz,         // CK_PROJECT
  /* */ "projection"_Stz,      // CK_PROJECTION

  /* */ "relnOp"_Stz,          // CK_RELNOP
  /* */ "required"_Stz,        // CK_REQUIRED
  /* */ "rowCount"_Stz,        // CK_ROWCOUNT
  /* */ "rowSize"_Stz,         // CK_ROWSIZE
  /* */ "rpnCalc"_Stz,         // CK_RPNCALC
  /* */ "rpnTerms"_Stz,        // CK_RPNTERMS

  /* */ "scan"_Stz,            // CK_SCAN
  /* */ "section"_Stz,         // CK_SECTION
  /* */ "sendSE"_Stz,          // CK_SEND
  /* */ "sendKind"_Stz,        // CK_SENDKIND
  /* */ "sort"_Stz,            // CK_SORT
  /* */ "spare"_Stz,           // CK_SPARE
  /* */ "sqyCalls"_Stz,        // CK_SQYCALLS
  /* */ "sqyInfo"_Stz,         // CK_SQYINFO
  /* */ "sqyNo"_Stz,           // CK_SQYNO
  /* */ "stage"_Stz,           // CK_STAGE
  /* */ "stream"_Stz,          // CK_STREAM
  /* */ "streamHead"_Stz,      // CK_STREAMHEAD
  /* */ "streamJoin"_Stz,      // CK_STREAMJOIN
  /* */ "streamKey"_Stz,       // CK_STREAMKEY
  /* */ "streamPer"_Stz,       // CK_STREAMPER
  /* */ "streamPtn"_Stz,       // CK_STREAMPTN
  /* */ "streamPrmAll"_Stz,    // CK_STREAMSET
  /* */ "subQuery"_Stz,        // CK_SUBQUERY
  /* */ "subSet"_Stz,          // CK_SUBSET
  /* */ "synthesise"_Stz,      // CK_SYNTHESIS

  /* */ "table"_Stz,           // CK_TABLE
  /* */ "tableInfo"_Stz,       // CK_TABLEINFO
  /* */ "tableList"_Stz,       // CK_TABLELIST
  /* */ "tableNm"_Stz,         // CK_TABLENM
  /* */ "tablePtn"_Stz,        // CK_TABLEPTN
  /* */ "tableSize"_Stz,       // CK_TABLESIZE
  /* */ "tblSyn"_Stz,          // CK_TBLSYN
  /* */ "term"_Stz,            // CK_TERM
  /* */ "top"_Stz,             // CK_TOP
  /* */ "topology"_Stz,        // CK_TOPOLOGY

  /* */ "unique"_Stz,          // CK_UNIQUE

  /* */ "where"_Stz,           // CK_WHERE

  /* */ "xcard"_Stz,           // CK_XCARD

//  /* */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
