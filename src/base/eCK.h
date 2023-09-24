/*============================================================================*/
/* eCK.h -- Card Kind                                                         */
/*============================================================================*/
#pragma once

enum eCK : i8 // card kind
{
  /* */ CK_NONE=0,

  /* */ CK_ABSJOIN,
  /* */ CK_ADDRESS,
  /* */ CK_AGGREG,
  /* */ CK_ANDCONJ,
  /* */ CK_ARRIVING,
  /* */ CK_AS,

  /* */ CK_BOOLCALC,
  /* */ CK_BOUND,
  /* */ CK_BOUNDS,

  /* */ CK_CALL,
  /* */ CK_CALLER,
  /* */ CK_CALLOP,
  /* */ CK_CAST,
  /* */ CK_CLAUSE,
  /* */ CK_CMPCONJ,
  /* */ CK_COMBINED,
  /* */ CK_CONSTRAINT,
  /* */ CK_CORRCOLS,
  /* */ CK_CORRELATION,
  /* */ CK_COSTJOINING,
  /* */ CK_COSTNETWORK,
  /* */ CK_COSTSTREAM,
  /* */ CK_COSTUNICAST,
  /* */ CK_COUNT,
  /* */ CK_CROSSJOIN,

  /* */ CK_DEBUG,
  /* */ CK_DECISION,
  /* */ CK_DECK,
  /* */ CK_DIRECTION,
  /* */ CK_DISPOSITION,

  /* */ CK_EXPR,

  /* */ CK_FILTER,
  /* */ CK_FILTPRED,
  /* */ CK_FILTPREDLIST,
  /* */ CK_FLAT,
  /* */ CK_FOLD,
  /* */ CK_FROM,

  /* */ CK_GBYAGG,

  /* */ CK_HASH,
  /* */ CK_HASHING,
  /* */ CK_HAVING,
  /* */ CK_HSHCOL,
  /* */ CK_HSHCOLLIST,

  /* */ CK_IDXCOL,
  /* */ CK_IDXCOLLIST,
  /* */ CK_INDEX,
  /* */ CK_INDEXJOIN,
  /* */ CK_INDEXLIST,
  /* */ CK_INDEXNM,
  /* */ CK_INDEXSCAN,
  /* */ CK_INJECTED,

  /* */ CK_JOINCAST,
  /* */ CK_JOINESTIMATE,
  /* */ CK_JOINORDER,
  /* */ CK_JOINORDERING,
  /* */ CK_JOINPREDLIST,

  /* */ CK_KEY,
  /* */ CK_KEYCELLS,
  /* */ CK_KEYEXISTS,
  /* */ CK_KEYNOTEXISTS,

  /* */ CK_LEVEL,
  /* */ CK_LIMIT,

  /* */ CK_MAP,
  /* */ CK_MAPPING,

  /* */ CK_NEEDED,
  /* */ CK_NOTE,
  /* */ CK_NOTCONJ,

  /* */ CK_ON,
  /* */ CK_ORDAGG,
  /* */ CK_ORCONJ,
  /* */ CK_OUTERCOLNM,
  /* */ CK_OUTERJOIN,

  /* */ CK_PAYLOADCELLS,
  /* */ CK_PROJECT,
  /* */ CK_PROJECTION,

  /* */ CK_RELNOP,
  /* */ CK_REQUIRED,
  /* */ CK_ROWCOUNT,
  /* */ CK_ROWSIZE,
  /* */ CK_RPNCALC,
  /* */ CK_RPNTERMS,

  /* */ CK_SCAN,
  /* */ CK_SECTION,
  /* */ CK_SEND,
  /* */ CK_SENDKIND,
  /* */ CK_SORT,
  /* */ CK_SPARE,
  /* */ CK_SQYCALLS,
  /* */ CK_SQYINFO,
  /* */ CK_SQYNO,
  /* */ CK_STAGE,
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
  /* */ CK_TOP,
  /* */ CK_TOPOLOGY,

  /* */ CK_UNIQUE,

  /* */ CK_WHERE,

  /* */ CK_XCARD,

  /* */ CK_ENUMS
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

extern Stz* aCK[CK_ENUMS];
extern Stz* aCK_short[CK_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
