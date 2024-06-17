/*============================================================================*/
/* eCK.cpp -- Card Kind                                                       */
/*============================================================================*/
#include "BASE.h"
#include "eCK.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Stz* aCK[CK_ENUMS]
{
  /* */ "NONE"_Stz,

  /* */ "CK_o_ABSJOIN"_Stz,      // CK_o_ABSJOIN
  /* */ "CK_o_AGGREG"_Stz,       // CK_o_AGGREG
  /* */ "CK_o_CALL"_Stz,         // CK_o_CALL
  /* */ "CK_o_CAST"_Stz,         // CK_o_CAST
  /* */ "CK_o_CORRELATION"_Stz,  // CK_o_CORRELATION
  /* */ "CK_o_CROSSJOIN"_Stz,    // CK_o_CROSSJOIN
  /* */ "CK_o_FILTER"_Stz,       // CK_o_FILTER
  /* */ "CK_o_FOLD"_Stz,         // CK_o_FOLD
  /* */ "CK_o_FROM"_Stz,         // CK_o_FROM
  /* */ "CK_o_GBYAGG"_Stz,       // CK_o_GBYAGG
  /* */ "CK_o_GLOBALHASH"_Stz,   // CK_o_GLOBALHASH,
  /* */ "CK_o_HASH"_Stz,         // CK_o_HASH
  /* */ "CK_o_HAVING"_Stz,       // CK_o_HAVING
  /* */ "CK_o_INDEXJOIN"_Stz,    // CK_o_INDEXJOIN
  /* */ "CK_o_INDEXSCAN"_Stz,    // CK_o_INDEXSCAN
  /* */ "CK_o_INFLATE"_Stz,      // CK_o_INFLATE
  /* */ "CK_o_KEYEXISTS"_Stz,    // CK_o_KEYEXISTS
  /* */ "CK_o_KEYNOTEXISTS"_Stz, // CK_o_KEYNOTEXISTS
  /* */ "CK_o_LIMIT"_Stz,        // CK_o_LIMIT
  /* */ "CK_o_ORDAGG"_Stz,       // CK_o_ORDAGG
  /* */ "CK_o_PDSROW"_Stz,       // CK_o_PDSROW
  /* */ "CK_o_PROJECT"_Stz,      // CK_o_PROJECT
  /* */ "CK_o_RRFJOIN"_Stz,      // CK_o_RRFJOIN,
  /* */ "CK_o_SCAN"_Stz,         // CK_o_SCAN
  /* */ "CK_o_SEND"_Stz,         // CK_o_SEND
  /* */ "CK_o_SORT"_Stz,         // CK_o_SORT
  /* */ "CK_o_STAGE"_Stz,        // CK_o_STAGE
  /* */ "CK_o_TOP"_Stz,          // CK_o_TOP
  /* */ "CK_o_WHERE"_Stz,        // CK_o_WHERE

  /* */ "CK_p_CONSTRAINT"_Stz,   // CK_p_CONSTRAINT
  /* */ "CK_p_DECISION"_Stz,     // CK_p_DECISION
  /* */ "CK_p_EMIT"_Stz,         // CK_p_EMIT
  /* */ "CK_p_HASHING"_Stz,      // CK_p_HASHING
  /* */ "CK_p_HSHCOLLIST"_Stz,   // CK_p_HSHCOLLIST
  /* */ "CK_p_INJECTABLE"_Stz,   // CK_p_INJECTABLE
  /* */ "CK_p_KEY"_Stz,          // CK_p_KEY
  /* */ "CK_p_LEVEL"_Stz,        // CK_p_LEVEL
  /* */ "CK_p_MAPPING"_Stz,      // CK_p_MAPPING
  /* */ "CK_p_NEEDED"_Stz,       // CK_p_NEEDED
  /* */ "CK_p_NEEDEDv2"_Stz,     // CK_p_NEEDEDv2
  /* */ "CK_p_STREAMKEY"_Stz,    // CK_p_STREAMKEY
  /* */ "CK_p_TABLE"_Stz,        // CK_p_TABLE

  /* */ "CK_ADDRESS"_Stz,      // CK_ADDRESS
  /* */ "CK_ANDCONJ"_Stz,      // CK_ANDCONJ

  /* */ "CK_BOUND"_Stz,        // CK_BOUND
  /* */ "CK_BOUNDS"_Stz,       // CK_BOUNDS

  /* */ "CK_CALLER"_Stz,       // CK_CALLER
  /* */ "CK_CALLOP"_Stz,       // CK_CALLOP
  /* */ "CK_CLAUSE"_Stz,       // CK_CLAUSE
  /* */ "CK_CORRCOLS"_Stz,     // CK_CORRCOLS
  /* */ "CK_COSTJOINING"_Stz,  // CK_COSTJOINING
  /* */ "CK_COSTNETWORK"_Stz,  // CK_COSTNETWORK
  /* */ "CK_COSTSTREAM"_Stz,   // CK_COSTSTREAM
  /* */ "CK_COSTUNICAST"_Stz,  // CK_COSTUNICAST
  /* */ "CK_COUNT"_Stz,        // CK_COUNT

  /* */ "CK_DEBUG"_Stz,        // CK_DEBUG
  /* */ "CK_DECK"_Stz,         // CK_DECK
  /* */ "CK_DISPOSITION"_Stz,  // CK_DISPOSITION

  /* */ "CK_EXPR"_Stz,         // CK_EXPR

  /* */ "CK_FILTPRED"_Stz,     // CK_FILTPRED
  /* */ "CK_FILTPREDLIST"_Stz, // CK_FILTPREDLIST

  /* */ "CK_HSHCOL"_Stz,       // CK_HSHCOL

  /* */ "CK_IDXCOL"_Stz,       // CK_IDXCOL
  /* */ "CK_IDXCOLLIST"_Stz,   // CK_IDXCOLLIST
  /* */ "CK_INDEX"_Stz,        // CK_INDEX
  /* */ "CK_INDEXLIST"_Stz,    // CK_INDEXLIST
  /* */ "CK_INDEXNM"_Stz,      // CK_INDEXNM

  /* */ "CK_JOINCAST"_Stz,     // CK_JOINCAST
  /* */ "CK_JOINESTIMATE"_Stz, // CK_JOINESTIMATE
  /* */ "CK_JOINORDER"_Stz,    // CK_JOINORDER
  /* */ "CK_JOINORDERING"_Stz, // CK_JOINORDERING
  /* */ "CK_JOINPREDLIST"_Stz, // CK_JOINPREDLIST

  /* */ "CK_KEYCELLS"_Stz,     // CK_KEYCELLS

  /* */ "CK_MAP"_Stz,          // CK_MAP

  /* */ "CK_NOTE"_Stz,         // CK_NOTE
  /* */ "CK_NOTCONJ"_Stz,      // CK_NOTCONJ

  /* */ "CK_ON"_Stz,           // CK_ON
  /* */ "CK_ORCONJ"_Stz,       // CK_ORCONJ
  /* */ "CK_OUTERCOLNM"_Stz,   // CK_OUTERCOLNM
  /* */ "CK_OUTERJOIN"_Stz,    // CK_OUTERJOIN

  /* */ "CK_PAYLOADCELLS"_Stz, // CK_PAYLOADCELLS
  /* */ "CK_PREDICATE"_Stz,    // CK_PREDICATE

  /* */ "CK_RELNOP"_Stz,       // CK_RELNOP
  /* */ "CK_RLNCONJ"_Stz,      // CK_RLNCONJ
  /* */ "CK_ROWCOUNT"_Stz,     // CK_ROWCOUNT
  /* */ "CK_ROWSIZE"_Stz,      // CK_ROWSIZE

  /* */ "CK_SECTION"_Stz,      // CK_SECTION
  /* */ "CK_SENDKIND"_Stz,     // CK_SENDKIND
  /* */ "CK_SPARE"_Stz,        // CK_SPARE
  /* */ "CK_SQYCALLS"_Stz,     // CK_SQYCALLS
  /* */ "CK_SQYINFO"_Stz,      // CK_SQYINFO
  /* */ "CK_SQYNO"_Stz,        // CK_SQYNO
  /* */ "CK_STREAMHEAD"_Stz,   // CK_STREAMHEAD
  /* */ "CK_STREAMJOIN"_Stz,   // CK_STREAMJOIN
  /* */ "CK_STREAMPER"_Stz,    // CK_STREAMPER
  /* */ "CK_STREAMPTN"_Stz,    // CK_STREAMPTN
  /* */ "CK_STREAMSET"_Stz,    // CK_STREAMSET
  /* */ "CK_SUBQUERY"_Stz,     // CK_SUBQUERY
  /* */ "CK_SUBSET"_Stz,       // CK_SUBSET

  /* */ "CK_TABLELIST"_Stz,    // CK_TABLELIST
  /* */ "CK_TABLENM"_Stz,    // CK_TABLENM
  /* */ "CK_TABLEPTN"_Stz,     // CK_TABLEPTN
  /* */ "CK_TABLESIZE"_Stz,    // CK_TABLESIZE
  /* */ "CK_TBLSYN"_Stz,       // CK_TBLSYN
  /* */ "CK_TERM"_Stz,         // CK_TERM
  /* */ "CK_TERMSEQ"_Stz,      // CK_TERMSEQ
  /* */ "CK_TOPOLOGY"_Stz,     // CK_TOPOLOGY

  /* */ "CK_UNIQUE"_Stz,       // CK_UNIQUE

  /* */ "CK_XCARD"_Stz,        // CK_XCARD

//  /* */ "ENUMS"_Stz
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Stz* aCK_short[CK_ENUMS]
{
  /* */ "NONE"_Stz,

  /* */ "AbsJoin"_Stz,         // CK_o_ABSJOIN
  /* */ "Aggreg"_Stz,          // CK_o_AGGREG
  /* */ "Call"_Stz,            // CK_o_CALL
  /* */ "Cast"_Stz,            // CK_o_CAST
  /* */ "Correlation"_Stz,     // CK_o_CORRELATION
  /* */ "CrossJoin"_Stz,       // CK_o_CROSSJOIN
  /* */ "Filter"_Stz,          // CK_o_FILTER
  /* */ "Fold"_Stz,            // CK_o_FOLD
  /* */ "From"_Stz,            // CK_o_FROM
  /* */ "GbyAgg"_Stz,          // CK_o_GBYAGG
  /* */ "GlobalHash"_Stz,      // CK_o_GLOBALHASH
  /* */ "Hash"_Stz,            // CK_o_HASH
  /* */ "Having"_Stz,          // CK_o_HAVING
  /* */ "IndexJoin"_Stz,       // CK_o_INDEXJOIN
  /* */ "IndexScan"_Stz,       // CK_o_INDEXSCAN
  /* */ "Inflate"_Stz,         // CK_o_INFLATE
  /* */ "KeyExists"_Stz,       // CK_o_KEYEXISTS
  /* */ "KeyNotExists"_Stz,    // CK_o_KEYNOTEXISTS
  /* */ "Limit"_Stz,           // CK_o_LIMIT
  /* */ "OrdAgg"_Stz,          // CK_o_ORDAGG
  /* */ "PdsRow"_Stz,          // CK_o_PDS_ROW
  /* */ "Project"_Stz,         // CK_o_PROJECT
  /* */ "RrfJoin"_Stz,         // CK_o_RRFJOIN
  /* */ "Scan"_Stz,            // CK_o_SCAN
  /* */ "SendSE"_Stz,          // CK_o_SEND
  /* */ "Sort"_Stz,            // CK_o_SORT
  /* */ "Stage"_Stz,           // CK_o_STAGE
  /* */ "Top"_Stz,             // CK_o_TOP
  /* */ "Where"_Stz,           // CK_o_WHERE

  /* */ "Constraint"_Stz,      // CK_p_CONSTRAINT
  /* */ "Decision"_Stz,        // CK_p_DECISION
  /* */ "Emit"_Stz,            // CK_p_EMIT
  /* */ "Hashing"_Stz,         // CK_p_HASHING
  /* */ "HshColList"_Stz,      // CK_p_HSHCOLLIST
  /* */ "Injectable"_Stz,      // CK_p_INJECTABLE
  /* */ "Key"_Stz,             // CK_p_KEY
  /* */ "Level"_Stz,           // CK_p_LEVEL
  /* */ "Mapping"_Stz,         // CK_p_MAPPING
  /* */ "Needed"_Stz,          // CK_p_NEEDED
  /* */ "Neededv2"_Stz,        // CK_p_NEEDEDv2
  /* */ "StreamKey"_Stz,       // CK_p_STREAMKEY
  /* */ "Table"_Stz,           // CK_p_TABLE

  /* */ "Address"_Stz,         // CK_ADDRESS
  /* */ "AndConj"_Stz,         // CK_ANDCONJ

  /* */ "Bound"_Stz,           // CK_BOUND
  /* */ "Bounds"_Stz,          // CK_BOUNDS

  /* */ "Caller"_Stz,          // CK_CALLER
  /* */ "CallOp"_Stz,          // CK_CALLOP
  /* */ "Clause"_Stz,          // CK_CLAUSE
  /* */ "CorrCols"_Stz,        // CK_CORRCOLS
  /* */ "CostJoining"_Stz,     // CK_COSTJOINING
  /* */ "CostNetwork"_Stz,     // CK_COSTNETWORK
  /* */ "CostQuery"_Stz,       // CK_COSTSTREAM
  /* */ "CostUnicast"_Stz,     // CK_COSTUNICAST
  /* */ "Count"_Stz,           // CK_COUNT

  /* */ "Debug"_Stz,           // CK_DEBUG
  /* */ "Deck"_Stz,            // CK_DECK
  /* */ "Disp"_Stz,            // CK_DISPOSITION

  /* */ "Expr"_Stz,            // CK_EXPR

  /* */ "FiltPred"_Stz,        // CK_FILTPRED
  /* */ "FltPredList"_Stz,     // CK_FILTPREDLIST

  /* */ "HshCol"_Stz,          // CK_HSHCOL

  /* */ "IdxCol"_Stz,          // CK_IDXCOL
  /* */ "IdxColList"_Stz,      // CK_IDXCOLLIST
  /* */ "Index"_Stz,           // CK_INDEX
  /* */ "IndexList"_Stz,       // CK_INDEXLIST
  /* */ "IndexNm"_Stz,         // CK_INDEXNM

  /* */ "JoinCast"_Stz,        // CK_JOINCAST
  /* */ "JoinEstimate"_Stz,    // CK_JOINESTIMATE
  /* */ "JoinOrder"_Stz,       // CK_JOINORDER
  /* */ "JoinOrdering"_Stz,    // CK_JOINORDERING
  /* */ "JoinPredList"_Stz,    // CK_JOINPREDLIST

  /* */ "KeyCells"_Stz,        // CK_KEYCELLS

  /* */ "Map"_Stz,             // CK_MAP

  /* */ "Note"_Stz,            // CK_NOTE
  /* */ "NotConj"_Stz,         // CK_NOTCONJ

  /* */ "On"_Stz,              // CK_ON
  /* */ "OrConj"_Stz,          // CK_ORCONJ
  /* */ "OuterColNm"_Stz,      // CK_OUTERCOLNM
  /* */ "OuterJoin"_Stz,       // CK_OUTERJOIN

  /* */ "PayloadCells"_Stz,    // CK_PAYLOADCELLS
  /* */ "Predicate"_Stz,       // CK_PREDICATE

  /* */ "RelnOp"_Stz,          // CK_RELNOP
  /* */ "RlnConj"_Stz,         // CK_RLNCONJ
  /* */ "RowCount"_Stz,        // CK_ROWCOUNT
  /* */ "RowSize"_Stz,         // CK_ROWSIZE

  /* */ "Section"_Stz,         // CK_SECTION
  /* */ "SendKind"_Stz,        // CK_SENDKIND
  /* */ "Spare"_Stz,           // CK_SPARE
  /* */ "SqyCalls"_Stz,        // CK_SQYCALLS
  /* */ "SqyInfo"_Stz,         // CK_SQYINFO
  /* */ "SqyNo"_Stz,           // CK_SQYNO
  /* */ "StreamHead"_Stz,      // CK_STREAMHEAD
  /* */ "StreamJoin"_Stz,      // CK_STREAMJOIN
  /* */ "StreamPer"_Stz,       // CK_STREAMPER
  /* */ "StreamPtn"_Stz,       // CK_STREAMPTN
  /* */ "StreamPrmAll"_Stz,    // CK_STREAMPRMALL
  /* */ "SubQuery"_Stz,        // CK_SUBQUERY
  /* */ "SubSet"_Stz,          // CK_SUBSET

  /* */ "TableList"_Stz,       // CK_TABLELIST
  /* */ "TableNm"_Stz,         // CK_TABLENM
  /* */ "TablePtn"_Stz,        // CK_TABLEPTN
  /* */ "TableSize"_Stz,       // CK_TABLESIZE
  /* */ "TblSyn"_Stz,          // CK_TBLSYN
  /* */ "Term"_Stz,            // CK_TERM
  /* */ "TermSeq"_Stz,         // CK_TERMSEQ
  /* */ "Topology"_Stz,        // CK_TOPOLOGY

  /* */ "Unique"_Stz,          // CK_UNIQUE

  /* */ "Xcard"_Stz,           // CK_XCARD

//  /* */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
