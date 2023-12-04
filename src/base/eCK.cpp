/*============================================================================*/
/* eCK.cpp -- Card Kind                                                       */
/*============================================================================*/
#include "BASE.h"
#include "eCK.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Stz* aCK[CK_ENUMS]
{
  /* */ "NONE"_Stz,

  /* */ "CK_m_ABSJOIN"_Stz,      // CK_m_ABSJOIN
  /* */ "CK_m_AGGREG"_Stz,       // CK_m_AGGREG
  /* */ "CK_m_CALL"_Stz,         // CK_m_CALL
  /* */ "CK_m_CAST"_Stz,         // CK_m_CAST
  /* */ "CK_m_COMBINED"_Stz,     // CK_m_COMBINED
  /* */ "CK_m_CORRELATION"_Stz,  // CK_m_CORRELATION
  /* */ "CK_m_CROSSJOIN"_Stz,    // CK_m_CROSSJOIN
  /* */ "CK_m_FILTER"_Stz,       // CK_m_FILTER
  /* */ "CK_m_FLAT"_Stz,         // CK_m_FLAT
  /* */ "CK_m_FOLD"_Stz,         // CK_m_FOLD
  /* */ "CK_m_FROM"_Stz,         // CK_m_FROM
  /* */ "CK_m_GBYAGG"_Stz,       // CK_m_GBYAGG
  /* */ "CK_m_HASH"_Stz,         // CK_m_HASH
  /* */ "CK_m_HAVING"_Stz,       // CK_m_HAVING
  /* */ "CK_m_INDEXJOIN"_Stz,    // CK_m_INDEXJOIN
  /* */ "CK_m_INDEXSCAN"_Stz,    // CK_m_INDEXSCAN
  /* */ "CK_m_INFLATE"_Stz,      // CK_m_INFLATE
  /* */ "CK_m_KEYEXISTS"_Stz,    // CK_m_KEYEXISTS
  /* */ "CK_m_KEYNOTEXISTS"_Stz, // CK_m_KEYNOTEXISTS
  /* */ "CK_m_LIMIT"_Stz,        // CK_m_LIMIT
  /* */ "CK_m_ORDAGG"_Stz,       // CK_m_ORDAGG
  /* */ "CK_m_PROJECT"_Stz,      // CK_m_PROJECT
  /* */ "CK_m_SCAN"_Stz,         // CK_m_SCAN
  /* */ "CK_m_SEND"_Stz,         // CK_m_SEND
  /* */ "CK_m_SORT"_Stz,         // CK_m_SORT
  /* */ "CK_m_STAGE"_Stz,        // CK_m_STAGE
  /* */ "CK_m_TOP"_Stz,          // CK_m_TOP
  /* */ "CK_m_WHERE"_Stz,        // CK_m_WHERE

  /* */ "CK_ADDRESS"_Stz,      // CK_ADDRESS
  /* */ "CK_ANDCONJ"_Stz,      // CK_ANDCONJ
  /* */ "CK_AS"_Stz,           // CK_AS

  /* */ "CK_BOOLPRED"_Stz,     // CK_BOOLPRED
  /* */ "CK_BOUND"_Stz,        // CK_BOUND
  /* */ "CK_BOUNDS"_Stz,       // CK_BOUNDS

  /* */ "CK_CALLER"_Stz,       // CK_CALLER
  /* */ "CK_CALLOP"_Stz,       // CK_CALLOP
  /* */ "CK_CLAUSE"_Stz,       // CK_CLAUSE
  /* */ "CK_s_CONSTRAINT"_Stz,   // CK_s_CONSTRAINT
  /* */ "CK_CORRCOLS"_Stz,     // CK_CORRCOLS
  /* */ "CK_COSTJOINING"_Stz,  // CK_COSTJOINING
  /* */ "CK_COSTNETWORK"_Stz,  // CK_COSTNETWORK
  /* */ "CK_COSTSTREAM"_Stz,   // CK_COSTSTREAM
  /* */ "CK_COSTUNICAST"_Stz,  // CK_COSTUNICAST
  /* */ "CK_COUNT"_Stz,        // CK_COUNT

  /* */ "CK_DEBUG"_Stz,        // CK_DEBUG
  /* */ "CK_DECK"_Stz,         // CK_DECK
  /* */ "CK_DECISION"_Stz,     // CK_DECISION
  /* */ "CK_DIRECTION"_Stz,    // CK_DIRECTION
  /* */ "CK_DISPOSITION"_Stz,  // CK_DISPOSITION

  /* */ "CK_EMIT"_Stz,      // CK_EMIT
  /* */ "CK_EXPR"_Stz,         // CK_EXPR

  /* */ "CK_FILTPRED"_Stz,     // CK_FILTPRED
  /* */ "CK_FILTPREDLIST"_Stz, // CK_FILTPREDLIST

  /* */ "CK_s_HASHING"_Stz,      // CK_s_HASHING
  /* */ "CK_HSHCOL"_Stz,       // CK_HSHCOL
  /* */ "CK_s_HSHCOLLIST"_Stz,   // CK_s_HSHCOLLIST

  /* */ "CK_IDXCOL"_Stz,       // CK_IDXCOL
  /* */ "CK_IDXCOLLIST"_Stz,   // CK_IDXCOLLIST
  /* */ "CK_INDEX"_Stz,        // CK_INDEX
  /* */ "CK_INDEXLIST"_Stz,    // CK_INDEXLIST
  /* */ "CK_INDEXNM"_Stz,      // CK_INDEXNM
  /* */ "CK_s_INJECTABLE"_Stz,     // CK_s_INJECTABLE

  /* */ "CK_JOINCAST"_Stz,     // CK_JOINCAST
  /* */ "CK_JOINESTIMATE"_Stz, // CK_JOINESTIMATE
  /* */ "CK_JOINORDER"_Stz,    // CK_JOINORDER
  /* */ "CK_JOINORDERING"_Stz, // CK_JOINORDERING
  /* */ "CK_JOINPREDLIST"_Stz, // CK_JOINPREDLIST

  /* */ "CK_s_KEY"_Stz,          // CK_s_KEY
  /* */ "CK_KEYCELLS"_Stz,     // CK_KEYCELLS

  /* */ "CK_s_LEVEL"_Stz,        // CK_s_LEVEL

  /* */ "CK_MAP"_Stz,          // CK_MAP
  /* */ "CK_s_MAPPING"_Stz,      // CK_s_MAPPING

  /* */ "CK_s_NEEDED"_Stz,       // CK_s_NEEDED
  /* */ "CK_NOTE"_Stz,         // CK_NOTE
  /* */ "CK_NOTCONJ"_Stz,      // CK_NOTCONJ

  /* */ "CK_ON"_Stz,           // CK_ON
  /* */ "CK_ORCONJ"_Stz,       // CK_ORCONJ
  /* */ "CK_OUTERCOLNM"_Stz,   // CK_OUTERCOLNM
  /* */ "CK_OUTERJOIN"_Stz,    // CK_OUTERJOIN

  /* */ "CK_s_PAYLOAD"_Stz,      // CK_s_PAYLOAD
  /* */ "CK_PAYLOADCELLS"_Stz, // CK_PAYLOADCELLS

  /* */ "CK_RELNOP"_Stz,       // CK_RELNOP
  /* */ "CK_REQUIRED"_Stz,     // CK_REQUIRED
  /* */ "CK_RLNCONJ"_Stz,      // CK_RLNCONJ
  /* */ "CK_ROWCOUNT"_Stz,     // CK_ROWCOUNT
  /* */ "CK_ROWSIZE"_Stz,      // CK_ROWSIZE
  /* */ "CK_RPNCALC"_Stz,      // CK_RPNCALC
  /* */ "CK_RPNEXPR"_Stz,     // CK_RPNEXPR

  /* */ "CK_SECTION"_Stz,      // CK_SECTION
  /* */ "CK_SENDKIND"_Stz,     // CK_SENDKIND
  /* */ "CK_SPARE"_Stz,        // CK_SPARE
  /* */ "CK_SQYCALLS"_Stz,     // CK_SQYCALLS
  /* */ "CK_SQYINFO"_Stz,      // CK_SQYINFO
  /* */ "CK_SQYNO"_Stz,        // CK_SQYNO
  /* */ "CK_s_STREAM"_Stz,       // CK_s_STREAM
  /* */ "CK_STREAMHEAD"_Stz,   // CK_STREAMHEAD
  /* */ "CK_STREAMJOIN"_Stz,   // CK_STREAMJOIN
  /* */ "CK_STREAMKEY"_Stz,    // CK_STREAMKEY
  /* */ "CK_STREAMPER"_Stz,    // CK_STREAMPER
  /* */ "CK_STREAMPTN"_Stz,    // CK_STREAMPTN
  /* */ "CK_STREAMSET"_Stz,    // CK_STREAMSET
  /* */ "CK_SUBQUERY"_Stz,     // CK_SUBQUERY
  /* */ "CK_SUBSET"_Stz,       // CK_SUBSET
  /* */ "CK_SYNTHESIS"_Stz,   // CK_SYNTHESIS

  /* */ "CK_s_TABLE"_Stz,        // CK_s_TABLE
  /* */ "CK_TABLEINFO"_Stz,    // CK_TABLEINFO
  /* */ "CK_TABLELIST"_Stz,    // CK_TABLELIST
  /* */ "CK_TABLENM"_Stz,      // CK_TABLENM
  /* */ "CK_TABLEPTN"_Stz,     // CK_TABLEPTN
  /* */ "CK_TABLESIZE"_Stz,    // CK_TABLESIZE
  /* */ "CK_TBLSYN"_Stz,       // CK_TBLSYN
  /* */ "CK_TERM"_Stz,         // CK_TERM
  /* */ "CK_TOPOLOGY"_Stz,     // CK_TOPOLOGY

  /* */ "CK_UNIQUE"_Stz,       // CK_UNIQUE

  /* */ "CK_XCARD"_Stz,        // CK_XCARD

//  /* */ "ENUMS"_Stz
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Stz* aCK_short[CK_ENUMS]
{
  /* */ "NONE"_Stz,

  /* */ "absJoin"_Stz,         // CK_m_ABSJOIN
  /* */ "aggreg"_Stz,          // CK_m_AGGREG
  /* */ "call"_Stz,            // CK_m_CALL
  /* */ "cast"_Stz,            // CK_m_CAST
  /* */ "combined"_Stz,        // CK_m_COMBINED
  /* */ "correlation"_Stz,     // CK_m_CORRELATION
  /* */ "crossJoin"_Stz,       // CK_m_CROSSJOIN
  /* */ "filter"_Stz,          // CK_m_FILTER
  /* */ "flat"_Stz,            // CK_m_FLAT
  /* */ "fold"_Stz,            // CK_m_FOLD
  /* */ "from"_Stz,            // CK_m_FROM
  /* */ "gbyAgg"_Stz,          // CK_m_GBYAGG
  /* */ "hash"_Stz,            // CK_m_HASH
  /* */ "having"_Stz,          // CK_m_HAVING
  /* */ "indexJoin"_Stz,       // CK_m_INDEXJOIN
  /* */ "indexScan"_Stz,       // CK_m_INDEXSCAN
  /* */ "inflate"_Stz,         // CK_m_INFLATE
  /* */ "keyExists"_Stz,       // CK_m_KEYEXISTS
  /* */ "keyNotExists"_Stz,    // CK_m_KEYNOTEXISTS
  /* */ "limit"_Stz,           // CK_m_LIMIT
  /* */ "ordAgg"_Stz,          // CK_m_ORDAGG
  /* */ "project"_Stz,         // CK_m_PROJECT
  /* */ "scan"_Stz,            // CK_m_SCAN
  /* */ "sendSE"_Stz,          // CK_m_SEND
  /* */ "sort"_Stz,            // CK_m_SORT
  /* */ "stage"_Stz,           // CK_m_STAGE
  /* */ "top"_Stz,             // CK_m_TOP
  /* */ "where"_Stz,           // CK_m_WHERE

  /* */ "address"_Stz,         // CK_ADDRESS
  /* */ "andConj"_Stz,         // CK_ANDCONJ
  /* */ "as"_Stz,              // CK_AS

  /* */ "boolPred"_Stz,        // CK_BOOLPRED
  /* */ "bound"_Stz,           // CK_BOUND
  /* */ "bounds"_Stz,          // CK_BOUNDS

  /* */ "caller"_Stz,          // CK_CALLER
  /* */ "callOp"_Stz,          // CK_CALLOP
  /* */ "clause"_Stz,          // CK_CLAUSE
  /* */ "constraint"_Stz,      // CK_s_CONSTRAINT
  /* */ "corrCols"_Stz,        // CK_CORRCOLS
  /* */ "costJoining"_Stz,     // CK_COSTJOINING
  /* */ "costNetwork"_Stz,     // CK_COSTNETWORK
  /* */ "costQuery"_Stz,       // CK_COSTSTREAM
  /* */ "costUnicast"_Stz,     // CK_COSTUNICAST
  /* */ "count"_Stz,           // CK_COUNT

  /* */ "debug"_Stz,           // CK_DEBUG
  /* */ "decision"_Stz,        // CK_DECISION
  /* */ "deck"_Stz,            // CK_DECK
  /* */ "direction"_Stz,       // CK_DIRECTION
  /* */ "disp"_Stz,            // CK_DISPOSITION

  /* */ "emit"_Stz,            // CK_EMIT
  /* */ "expr"_Stz,            // CK_EXPR

  /* */ "filtPred"_Stz,        // CK_FILTPRED
  /* */ "filtPredList"_Stz,    // CK_FILTPREDLIST

  /* */ "hashing"_Stz,         // CK_s_HASHING
  /* */ "hshCol"_Stz,          // CK_HSHCOL
  /* */ "hshColList"_Stz,      // CK_s_HSHCOLLIST

  /* */ "idxCol"_Stz,          // CK_IDXCOL
  /* */ "idxColList"_Stz,      // CK_IDXCOLLIST
  /* */ "index"_Stz,           // CK_INDEX
  /* */ "indexList"_Stz,       // CK_INDEXLIST
  /* */ "indexNm"_Stz,         // CK_INDEXNM
  /* */ "injectable"_Stz,      // CK_s_INJECTABLE

  /* */ "joinCast"_Stz,        // CK_JOINCAST
  /* */ "joinEstimate"_Stz,    // CK_JOINESTIMATE
  /* */ "joinOrder"_Stz,       // CK_JOINORDER
  /* */ "joinOrdering"_Stz,    // CK_JOINORDERING
  /* */ "joinPredList"_Stz,    // CK_JOINPREDLIST

  /* */ "key"_Stz,             // CK_s_KEY
  /* */ "keyCells"_Stz,        // CK_KEYCELLS

  /* */ "level"_Stz,           // CK_s_LEVEL

  /* */ "map"_Stz,             // CK_MAP
  /* */ "mapping"_Stz,         // CK_s_MAPPING

  /* */ "needed"_Stz,          // CK_s_NEEDED
  /* */ "note"_Stz,            // CK_NOTE
  /* */ "notConj"_Stz,         // CK_NOTCONJ

  /* */ "on"_Stz,              // CK_ON
  /* */ "orConj"_Stz,          // CK_ORCONJ
  /* */ "outerColNm"_Stz,      // CK_OUTERCOLNM
  /* */ "outerJoin"_Stz,       // CK_OUTERJOIN

  /* */ "payload"_Stz,         // CK_s_PAYLOAD
  /* */ "payloadCells"_Stz,    // CK_PAYLOADCELLS

  /* */ "relnOp"_Stz,          // CK_RELNOP
  /* */ "required"_Stz,        // CK_REQUIRED
  /* */ "rlnConj"_Stz,         // CK_RLNCONJ
  /* */ "rowCount"_Stz,        // CK_ROWCOUNT
  /* */ "rowSize"_Stz,         // CK_ROWSIZE
  /* */ "rpnCalc"_Stz,         // CK_RPNCALC
  /* */ "rpnTerms"_Stz,        // CK_RPNEXPR

  /* */ "section"_Stz,         // CK_SECTION
  /* */ "sendKind"_Stz,        // CK_SENDKIND
  /* */ "spare"_Stz,           // CK_SPARE
  /* */ "sqyCalls"_Stz,        // CK_SQYCALLS
  /* */ "sqyInfo"_Stz,         // CK_SQYINFO
  /* */ "sqyNo"_Stz,           // CK_SQYNO
  /* */ "stream"_Stz,          // CK_s_STREAM
  /* */ "streamHead"_Stz,      // CK_STREAMHEAD
  /* */ "streamJoin"_Stz,      // CK_STREAMJOIN
  /* */ "streamKey"_Stz,       // CK_STREAMKEY
  /* */ "streamPer"_Stz,       // CK_STREAMPER
  /* */ "streamPtn"_Stz,       // CK_STREAMPTN
  /* */ "streamPrmAll"_Stz,    // CK_STREAMSET
  /* */ "subQuery"_Stz,        // CK_SUBQUERY
  /* */ "subSet"_Stz,          // CK_SUBSET
  /* */ "synthesise"_Stz,      // CK_SYNTHESIS

  /* */ "table"_Stz,           // CK_s_TABLE
  /* */ "tableInfo"_Stz,       // CK_TABLEINFO
  /* */ "tableList"_Stz,       // CK_TABLELIST
  /* */ "tableNm"_Stz,         // CK_TABLENM
  /* */ "tablePtn"_Stz,        // CK_TABLEPTN
  /* */ "tableSize"_Stz,       // CK_TABLESIZE
  /* */ "tblSyn"_Stz,          // CK_TBLSYN
  /* */ "term"_Stz,            // CK_TERM
  /* */ "topology"_Stz,        // CK_TOPOLOGY

  /* */ "unique"_Stz,          // CK_UNIQUE

  /* */ "xcard"_Stz,           // CK_XCARD

//  /* */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
