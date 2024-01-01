/*============================================================================*/
/* eCK.h -- Card Kind                                                         */
/*============================================================================*/
#pragma once

enum eCK : i8 // card kind
{
  /* */ CK_NONE=0,

  // main cards
  //  construction cards
  /* */ CK_c_ABSJOIN,
  /* */ CK_c_COMBINED,
  /* */ CK_c_FLAT,
  /* */ CK_c_FOLD,
  /* */ CK_c_SEND,
  /* */ CK_c_STAGE,

  //  operator cards
  /* */ CK_o_AGGREG,
  /* */ CK_o_CALL,
  /* */ CK_o_CAST,
  /* */ CK_o_CORRELATION,
  /* */ CK_o_CROSSJOIN,
  /* */ CK_o_FILTER,
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
  /* */ CK_o_PROJECT,
  /* */ CK_o_SCAN,
  /* */ CK_o_SORT,
  /* */ CK_o_TOP,
  /* */ CK_o_WHERE,

  // secondary cards
  /* */ CK_s_CONSTRAINT,
  /* */ CK_s_HASHING,
  /* */ CK_s_HSHCOLLIST,
  /* */ CK_s_INJECTABLE,
  /* */ CK_s_KEY,
  /* */ CK_s_LEVEL,
  /* */ CK_s_MAPPING,
  /* */ CK_s_NEEDED,
  /* */ CK_s_STREAM,
  /* */ CK_s_TABLE,

  // tertiary cards
  /* */ CK_t_EMIT,
  /* */ CK_t_HSHCOL,
  /* */ CK_t_INDEXLIST,
  /* */ CK_t_TABLEINFO,
  /* */ CK_t_TABLENM,

  /* */ CK_ADDRESS,
  /* */ CK_ANDCONJ,
  /* */ CK_AS,

  /* */ CK_BOOLPRED,
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
  /* */ CK_DIRECTION,
  /* */ CK_DISPOSITION,

  /* */ CK_EXPR,

  /* */ CK_FILTPRED,
  /* */ CK_FILTPREDLIST,

  /* */ CK_IDXCOL,
  /* */ CK_IDXCOLLIST,
  /* */ CK_INDEX,
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
  /* */ CK_STREAMHEAD,
  /* */ CK_STREAMJOIN,
  /* */ CK_STREAMKEY,
  /* */ CK_STREAMPER,
  /* */ CK_STREAMPTN,
  /* */ CK_STREAMSET,
  /* */ CK_SUBQUERY,
  /* */ CK_SUBSET,
  /* */ CK_SYNTHESIS,

  /* */ CK_TABLELIST,
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
