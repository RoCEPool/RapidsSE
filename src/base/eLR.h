/*============================================================================*/
/* eLR.h -- Logic Unit Registers                                              */
/*============================================================================*/
#pragma once

enum eLR // Logic Unit Registers
{
  /*  */ LR_NONE=0,

  /*  */ LR_ADAPTORWL,
  /*  */ LR_AGGTIMER,
  /*  */ LR_AGGTYPESTACK,
  /*  */ LR_ANSWERWL,
  
  /*  */ LR_BKTMAP,

  /*  */ LR_CHILDEXIT,
  /*  */ LR_COMBINED,
  /*  */ LR_CRWBUFFER,
  /*  */ LR_CRWTOROW,
  /*  */ LR_CSVBUFFER,
  
  /*  */ LR_DATABUFFER,
  /*  */ LR_DECK,
  /*  */ LR_DIR0,
  /*  */ LR_DIR1,
  /*  */ LR_DYNAMIC,
  /*  */ LR_DYNAMO,
  
  /*  */ LR_ERRORMARKER,
  /*  */ LR_EXITOPERAND,
  /*  */ LR_EXPEVAL,
  
  /*  */ LR_FILE0,
  /*  */ LR_FILE1,
  /*  */ LR_FILTER,
  
  /*  */ LR_GRID,
  
  /*  */ LR_HANDLER,
  
  /*  */ LR_INDEX,
  /*  */ LR_INDEXDEF,
  /*  */ LR_INDEXSTREAM,
  /*  */ LR_INTSTACK,
  
  /*  */ LR_JOBMETRICS,

  /*  */ LR_LIBINFO,
  /*  */ LR_LIST,

  /*  */ LR_MARKER,
  /*  */ LR_MEMORYHOG,
  /*  */ LR_META,

  /*  */ LR_NODE,
  /*  */ LR_NODE_I,
  /*  */ LR_NOW,
  
  /*  */ LR_OP,
  /*  */ LR_OPERANDOOO,
  /*  */ LR_OPTIMISER,
  
  /*  */ LR_PARCEL,
  /*  */ LR_PDS,
	/*  */ LR_PDSCURSOR,
  /*  */ LR_PATHIDENTIFIER,
  /*  */ LR_PIPEIN,
  /*  */ LR_PIPELINE,
  /*  */ LR_PLANOPERAND,
  /*  */ LR_PORT,
  /*  */ LR_POT,
  /*  */ LR_PREEXPSTACK,
  /*  */ LR_PTNDEF,
  
  /*  */ LR_RECVDOBJECT,
  /*  */ LR_RECVWL,
  /*  */ LR_ROW,
  /*  */ LR_ROWBUFFER,
	/*  */ LR_ROWBUFFERERR,
	/*  */ LR_ROWBUFFERWARN,
  /*  */ LR_ROWONCE,
  /*  */ LR_ROWCOUNTS,

  /*  */ LR_SENDNODE,
  /*  */ LR_SENDS,
  /*  */ LR_SENDWL,
  /*  */ LR_SOCKET,
  /*  */ LR_STAGE,
  /*  */ LR_STATS,
  /*  */ LR_STENCIL,
  /*  */ LR_STREAM,
  /*  */ LR_STREAMDEFLFT,
  /*  */ LR_STREAMDEFOUT,
  /*  */ LR_STREAMDEFRHT,
  /*  */ LR_SUBCALL,
  /*  */ LR_SUBPIPE,
  /*  */ LR_SUBRETURN,
  /*  */ LR_SUBSET,
  
  /*  */ LR_TABLE,
	/*  */ LR_TABLEDEF,
	/*  */ LR_TABLEDEFERR,
	/*  */ LR_TABLEDEFWARN,
	/*  */ LR_TABLEPTN,
  /*  */ LR_TAGSET,
  /*  */ LR_TASKLIST,
  /*  */ LR_TEXT,
  /*  */ LR_TIMER,
  /*  */ LR_TINYC,
  /*  */ LR_TOKEYHSHROW,
  /*  */ LR_TOLGLPTNCRW,
  /*  */ LR_TOLGLPTNNST,
  /*  */ LR_TOLGLPTNROW,
  /*  */ LR_TUB,
  /*  */ LR_TX,

  /*  */ LR_VERTEX,
  /*  */ LR_VERTEXSPEC,

  /*  */ LR_ENUMS
};

extern Stz* aLR[LR_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
