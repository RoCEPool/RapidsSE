/*============================================================================*/
/* eCK.h -- Card Kind                                                         */
/*============================================================================*/
#pragma once

enum eCK : i8 // card kind
{
  /* */ CK_NONE=0,

  //  operator cards
  /* */ CK_o_ABSJOIN,
  /* */ CK_o_AGGREG,
  /* */ CK_o_CALL,
  /* */ CK_o_CAST,
  /* */ CK_o_CORRELATION,
  /* */ CK_o_CROSSJOIN,
  /* */ CK_o_FILTER,
  /* */ CK_o_FLAT,
  /* */ CK_o_FOLD,
  /* */ CK_o_FROM,
  /* */ CK_o_GBYAGG,
  /* */ CK_o_HASH,
  /* */ CK_o_HAVING,
  /* */ CK_o_INDEXJOIN,
  /* */ CK_o_INDEXSCAN,
  /* */ CK_o_INFLATE,
  /* */ CK_o_KEYEXISTS,
  /* */ CK_o_KEYNOTEXISTS,
  /* */ CK_o_LIMIT,
  /* */ CK_o_ORDAGG,
  /* */ CK_o_PDSROW,
  /* */ CK_o_PROJECT,
  /* */ CK_o_SCAN,
  /* */ CK_o_SEND,
  /* */ CK_o_SORT,
  /* */ CK_o_STAGE,
  /* */ CK_o_TOP,
  /* */ CK_o_WHERE,

  // card parts
  /* */ CK_p_CONSTRAINT,
  /* */ CK_p_EMIT,
  /* */ CK_p_HASHING,
  /* */ CK_p_HSHCOLLIST,
  /* */ CK_p_INJECTABLE,
  /* */ CK_p_KEY,
  /* */ CK_p_LEVEL,
  /* */ CK_p_MAPPING,
  /* */ CK_p_NEEDED,
  /* */ CK_p_NEEDEDv2,
  /* */ CK_p_STREAMKEY,
  /* */ CK_p_TABLE,

  // tertiary cards
  /* */ CK_ADDRESS,
  /* */ CK_ANDCONJ,

  /* */ CK_BOUND,
  /* */ CK_BOUNDS,

  /* */ CK_CALLER,
  /* */ CK_CALLOP,
  /* */ CK_CLAUSE,
  /* */ CK_CORRCOLS,
  /* */ CK_COSTJOINING,
  /* */ CK_COSTNETWORK,
  /* */ CK_COSTSTREAM,
  /* */ CK_COSTUNICAST,
  /* */ CK_COUNT,

  /* */ CK_DEBUG,
  /* */ CK_DECISION,
  /* */ CK_DECK,
  /* */ CK_DISPOSITION,

  /* */ CK_EXPR,

  /* */ CK_FILTPRED,
  /* */ CK_FILTPREDLIST,

  /* */ CK_HSHCOL,

  /* */ CK_IDXCOL,
  /* */ CK_IDXCOLLIST,
  /* */ CK_INDEX,
  /* */ CK_INDEXLIST,
  /* */ CK_INDEXNM,

  /* */ CK_JOINCAST,
  /* */ CK_JOINESTIMATE,
  /* */ CK_JOINORDER,
  /* */ CK_JOINORDERING,
  /* */ CK_JOINPREDLIST,

  /* */ CK_KEYCELLS,

  /* */ CK_MAP,

  /* */ CK_NOTE,
  /* */ CK_NOTCONJ,

  /* */ CK_ON,
  /* */ CK_ORCONJ,
  /* */ CK_OUTERCOLNM,
  /* */ CK_OUTERJOIN,

  /* */ CK_PAYLOADCELLS,
  /* */ CK_PREDICATE,

  /* */ CK_RELNOP,
  /* */ CK_RLNCONJ,
  /* */ CK_ROWCOUNT,
  /* */ CK_ROWSIZE,

  /* */ CK_SECTION,
  /* */ CK_SENDKIND,
  /* */ CK_SPARE,
  /* */ CK_SQYCALLS,
  /* */ CK_SQYINFO,
  /* */ CK_SQYNO,
  /* */ CK_STREAMHEAD,
  /* */ CK_STREAMJOIN,
  /* */ CK_STREAMPER,
  /* */ CK_STREAMPTN,
  /* */ CK_STREAMSET,
  /* */ CK_SUBQUERY,
  /* */ CK_SUBSET,

  /* */ CK_TABLELIST,
  /* */ CK_TABLENM,
  /* */ CK_TABLEPTN,
  /* */ CK_TABLESIZE,
  /* */ CK_TBLSYN,
  /* */ CK_TERM,
  /* */ CK_TERMSEQ,
  /* */ CK_TOPOLOGY,

  /* */ CK_UNIQUE,

  /* */ CK_XCARD,

  /* */ CK_ENUMS
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

extern Stz* aCK[CK_ENUMS];
extern Stz* aCK_short[CK_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
