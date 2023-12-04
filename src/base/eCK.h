/*============================================================================*/
/* eCK.h -- Card Kind                                                         */
/*============================================================================*/
#pragma once

enum eCK : i8 // card kind
{
  /* */ CK_NONE=0,

  /* */ CK_m_ABSJOIN,
  /* */ CK_ADDRESS,
  /* */ CK_m_AGGREG,
  /* */ CK_ANDCONJ,
  /* */ CK_AS,

  /* */ CK_BOOLPRED,
  /* */ CK_BOUND,
  /* */ CK_BOUNDS,

  /* */ CK_m_CALL,
  /* */ CK_CALLER,
  /* */ CK_CALLOP,
  /* */ CK_m_CAST,
  /* */ CK_CLAUSE,
  /* */ CK_m_COMBINED,
  /* */ CK_CONSTRAINT,
  /* */ CK_CORRCOLS,
  /* */ CK_m_CORRELATION,
  /* */ CK_COSTJOINING,
  /* */ CK_COSTNETWORK,
  /* */ CK_COSTSTREAM,
  /* */ CK_COSTUNICAST,
  /* */ CK_COUNT,
  /* */ CK_m_CROSSJOIN,

  /* */ CK_DEBUG,
  /* */ CK_DECISION,
  /* */ CK_DECK,
  /* */ CK_DIRECTION,
  /* */ CK_DISPOSITION,

  /* */ CK_EMIT,
  /* */ CK_EXPR,

  /* */ CK_m_FILTER,
  /* */ CK_FILTPRED,
  /* */ CK_FILTPREDLIST,
  /* */ CK_m_FLAT,
  /* */ CK_m_FOLD,
  /* */ CK_m_FROM,

  /* */ CK_m_GBYAGG,

  /* */ CK_m_HASH,
  /* */ CK_HASHING,
  /* */ CK_m_HAVING,
  /* */ CK_HSHCOL,
  /* */ CK_HSHCOLLIST,

  /* */ CK_IDXCOL,
  /* */ CK_IDXCOLLIST,
  /* */ CK_INDEX,
  /* */ CK_m_INDEXJOIN,
  /* */ CK_INDEXLIST,
  /* */ CK_INDEXNM,
  /* */ CK_m_INDEXSCAN,
  /* */ CK_m_INFLATE,
  /* */ CK_INJECTABLE,

  /* */ CK_JOINCAST,
  /* */ CK_JOINESTIMATE,
  /* */ CK_JOINORDER,
  /* */ CK_JOINORDERING,
  /* */ CK_JOINPREDLIST,

  /* */ CK_KEY,
  /* */ CK_KEYCELLS,
  /* */ CK_m_KEYEXISTS,
  /* */ CK_m_KEYNOTEXISTS,

  /* */ CK_LEVEL,
  /* */ CK_m_LIMIT,

  /* */ CK_MAP,
  /* */ CK_MAPPING,

  /* */ CK_NEEDED,
  /* */ CK_NOTE,
  /* */ CK_NOTCONJ,

  /* */ CK_ON,
  /* */ CK_m_ORDAGG,
  /* */ CK_ORCONJ,
  /* */ CK_OUTERCOLNM,
  /* */ CK_OUTERJOIN,

  /* */ CK_PAYLOAD,
  /* */ CK_PAYLOADCELLS,
  /* */ CK_m_PROJECT,

  /* */ CK_RELNOP,
  /* */ CK_REQUIRED,
  /* */ CK_RLNCONJ,
  /* */ CK_ROWCOUNT,
  /* */ CK_ROWSIZE,
  /* */ CK_RPNCALC,
  /* */ CK_RPNEXPR,

  /* */ CK_m_SCAN,
  /* */ CK_SECTION,
  /* */ CK_m_SEND,
  /* */ CK_SENDKIND,
  /* */ CK_m_SORT,
  /* */ CK_SPARE,
  /* */ CK_SQYCALLS,
  /* */ CK_SQYINFO,
  /* */ CK_SQYNO,
  /* */ CK_m_STAGE,
  /* */ CK_STREAM,
  /* */ CK_STREAMHEAD,
  /* */ CK_STREAMJOIN,
  /* */ CK_STREAMKEY,
  /* */ CK_STREAMPER,
  /* */ CK_STREAMPTN,
  /* */ CK_STREAMSET,
  /* */ CK_SUBQUERY,
  /* */ CK_SUBSET,
  /* */ CK_SYNTHESIS,

  /* */ CK_TABLE,
  /* */ CK_TABLEINFO,
  /* */ CK_TABLELIST,
  /* */ CK_TABLENM,
  /* */ CK_TABLEPTN,
  /* */ CK_TABLESIZE,
  /* */ CK_TBLSYN,
  /* */ CK_TERM,
  /* */ CK_m_TOP,
  /* */ CK_TOPOLOGY,

  /* */ CK_UNIQUE,

  /* */ CK_m_WHERE,

  /* */ CK_XCARD,

  /* */ CK_ENUMS
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

extern Stz* aCK[CK_ENUMS];
extern Stz* aCK_short[CK_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
