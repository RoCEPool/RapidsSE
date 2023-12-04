/*============================================================================*/
/* eCK.h -- Card Kind                                                         */
/*============================================================================*/
#pragma once

enum eCK : i8 // card kind
{
  /* */ CK_NONE=0,

  /* */ CK_m_ABSJOIN,
  /* */ CK_m_AGGREG,
  /* */ CK_m_CALL,
  /* */ CK_m_CAST,
  /* */ CK_m_COMBINED,
  /* */ CK_m_CORRELATION,
  /* */ CK_m_CROSSJOIN,
  /* */ CK_m_FILTER,
  /* */ CK_m_FLAT,
  /* */ CK_m_FOLD,
  /* */ CK_m_FROM,
  /* */ CK_m_GBYAGG,
  /* */ CK_m_HASH,
  /* */ CK_m_HAVING,
  /* */ CK_m_INDEXJOIN,
  /* */ CK_m_INDEXSCAN,
  /* */ CK_m_INFLATE,
  /* */ CK_m_KEYEXISTS,
  /* */ CK_m_KEYNOTEXISTS,
  /* */ CK_m_LIMIT,
  /* */ CK_m_ORDAGG,
  /* */ CK_m_PROJECT,
  /* */ CK_m_SCAN,
  /* */ CK_m_SEND,
  /* */ CK_m_SORT,
  /* */ CK_m_STAGE,
  /* */ CK_m_TOP,
  /* */ CK_m_WHERE,

  /* */ CK_ADDRESS,
  /* */ CK_ANDCONJ,
  /* */ CK_AS,

  /* */ CK_BOOLPRED,
  /* */ CK_BOUND,
  /* */ CK_BOUNDS,

  /* */ CK_CALLER,
  /* */ CK_CALLOP,
  /* */ CK_CLAUSE,
  /* */ CK_s_CONSTRAINT,
  /* */ CK_CORRCOLS,
  /* */ CK_COSTJOINING,
  /* */ CK_COSTNETWORK,
  /* */ CK_COSTSTREAM,
  /* */ CK_COSTUNICAST,
  /* */ CK_COUNT,

  /* */ CK_DEBUG,
  /* */ CK_DECISION,
  /* */ CK_DECK,
  /* */ CK_DIRECTION,
  /* */ CK_DISPOSITION,

  /* */ CK_EMIT,
  /* */ CK_EXPR,

  /* */ CK_FILTPRED,
  /* */ CK_FILTPREDLIST,

  /* */ CK_s_HASHING,
  /* */ CK_HSHCOL,
  /* */ CK_s_HSHCOLLIST,

  /* */ CK_IDXCOL,
  /* */ CK_IDXCOLLIST,
  /* */ CK_INDEX,
  /* */ CK_INDEXLIST,
  /* */ CK_INDEXNM,
  /* */ CK_s_INJECTABLE,

  /* */ CK_JOINCAST,
  /* */ CK_JOINESTIMATE,
  /* */ CK_JOINORDER,
  /* */ CK_JOINORDERING,
  /* */ CK_JOINPREDLIST,

  /* */ CK_s_KEY,
  /* */ CK_KEYCELLS,

  /* */ CK_s_LEVEL,

  /* */ CK_MAP,
  /* */ CK_s_MAPPING,

  /* */ CK_s_NEEDED,
  /* */ CK_NOTE,
  /* */ CK_NOTCONJ,

  /* */ CK_ON,
  /* */ CK_ORCONJ,
  /* */ CK_OUTERCOLNM,
  /* */ CK_OUTERJOIN,

  /* */ CK_s_PAYLOAD,
  /* */ CK_PAYLOADCELLS,

  /* */ CK_RELNOP,
  /* */ CK_REQUIRED,
  /* */ CK_RLNCONJ,
  /* */ CK_ROWCOUNT,
  /* */ CK_ROWSIZE,
  /* */ CK_RPNCALC,
  /* */ CK_RPNEXPR,

  /* */ CK_SECTION,
  /* */ CK_SENDKIND,
  /* */ CK_SPARE,
  /* */ CK_SQYCALLS,
  /* */ CK_SQYINFO,
  /* */ CK_SQYNO,
  /* */ CK_s_STREAM,
  /* */ CK_STREAMHEAD,
  /* */ CK_STREAMJOIN,
  /* */ CK_STREAMKEY,
  /* */ CK_STREAMPER,
  /* */ CK_STREAMPTN,
  /* */ CK_STREAMSET,
  /* */ CK_SUBQUERY,
  /* */ CK_SUBSET,
  /* */ CK_SYNTHESIS,

  /* */ CK_s_TABLE,
  /* */ CK_TABLEINFO,
  /* */ CK_TABLELIST,
  /* */ CK_TABLENM,
  /* */ CK_TABLEPTN,
  /* */ CK_TABLESIZE,
  /* */ CK_TBLSYN,
  /* */ CK_TERM,
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
