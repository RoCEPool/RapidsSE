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
  /*  */ LR_BUCKET,
  /*  */ LR_BUCKETGET,
  /*  */ LR_BUCKETINFO,
  
  /*  */ LR_COMBINED,
  /*  */ LR_CRWBUFFER,
  /*  */ LR_CRWTOROW,
  /*  */ LR_CSVBUFFER,
  
  /*  */ LR_DATABUFFER,
  /*  */ LR_DIR0,
  /*  */ LR_DIR1,
  /*  */ LR_DYNAMO,
  
  /*  */ LR_ERRORMARKER,
  /*  */ LR_EXPEVAL,
  
  /*  */ LR_FILE0,
  /*  */ LR_FILE1,
  /*  */ LR_FILTER,
  
  /*  */ LR_GRID,
  
  /*  */ LR_HANDLER,
  
  /*  */ LR_INDEX,
  /*  */ LR_INDEXDEF,
  /*  */ LR_INDEXSTREAM,
  /*  */ LR_INFOSET,
  /*  */ LR_INFOSETROW,
  /*  */ LR_INFOSETTABLE,
  /*  */ LR_INTSTACK,
  
  /*  */ LR_JOBMETRICS,
  
  /*  */ LR_LIST,
  /*  */ LR_LOGICCOMPILER,
  
  /*  */ LR_MARKER,
  /*  */ LR_MEMORYHOG,
  /*  */ LR_META,
  
  /*  */ LR_NAMEDBUFFER,
  /*  */ LR_NODE,
  /*  */ LR_NODE_I,
  /*  */ LR_NOW,
  
  /*  */ LR_OP,
  /*  */ LR_OPERANDOOO,
  /*  */ LR_OPTIMISER,
  
  /*  */ LR_PARCEL,
  /*  */ LR_PATHIDENTIFIER,
  /*  */ LR_PIPEIN,
  /*  */ LR_PIPELINE,
  /*  */ LR_PIPESPEC,
  /*  */ LR_POT,
  /*  */ LR_PLANOPERAND,
  /*  */ LR_PREEXPSTACK,
  /*  */ LR_PTNSTREAM,
  
  /*  */ LR_RECVDOBJECT,
  /*  */ LR_RECVWL,
  /*  */ LR_ROWBUFFER,
  /*  */ LR_ROWONCE,
  
  /*  */ LR_SENDNODE,
  /*  */ LR_SENDS,
  /*  */ LR_SENDWL,
  /*  */ LR_SHARDDEF,
  /*  */ LR_SOCKET,
  
  /*  */ LR_STAGE,
  /*  */ LR_STATS,
  /*  */ LR_STENCIL,
  /*  */ LR_STREAM,
  /*  */ LR_STREAMDEFLFT,
  /*  */ LR_STREAMDEFOUT,
  /*  */ LR_STREAMDEFRHT,
  /*  */ LR_STREAMSET,
  /*  */ LR_STREAMSETTABLE,
  /*  */ LR_SUBCALL,
  /*  */ LR_SUBPIPE,
  /*  */ LR_SUBRETURN,
  /*  */ LR_SUBSET,
  
  /*  */ LR_TABLE,
  /*  */ LR_TABLEPTN,
  /*  */ LR_TAGSET,
  /*  */ LR_TIMER,
  /*  */ LR_TINYC,
  /*  */ LR_TOBKTCRW,
  /*  */ LR_TOBKTROW,
  /*  */ LR_TONSTROW,
  /*  */ LR_TOPTNCRW,
  /*  */ LR_TOPTNROW,
  /*  */ LR_TOROW,
  /*  */ LR_TUB,
  /*  */ LR_TX,
  
  /*  */ LR_ENUMS
};

static Stz* aLR[]
{
  /*  */ "LR_NONE"_Stz,
  
  /*  */ "LR_ADAPTORWL"_Stz,
  /*  */ "LR_AGGTIMER"_Stz,
  /*  */ "LR_AGGTYPESTACK"_Stz,
  /*  */ "LR_ANSWERWL"_Stz,

  /*  */ "LR_BKTMAP"_Stz,
  /*  */ "LR_BUCKET"_Stz,
  /*  */ "LR_BUCKETGET"_Stz,
  /*  */ "LR_BUCKETINFO"_Stz,
  
  /*  */ "LR_COMBINED"_Stz,
  /*  */ "LR_CRWBUFFER"_Stz,
  /*  */ "LR_CRWTOROW"_Stz,
  /*  */ "LR_CSVBUFFER"_Stz,
  
  /*  */ "LR_DATABUFFER"_Stz,
  
  /*  */ "LR_DIR0"_Stz,
  /*  */ "LR_DIR1"_Stz,
  /*  */ "LR_DYNAMO"_Stz,
  
  /*  */ "LR_ERRORMARKER"_Stz,
  /*  */ "LR_EXPEVAL"_Stz,
  
  /*  */ "LR_FILE0"_Stz,
  /*  */ "LR_FILE1"_Stz,
  /*  */ "LR_FILTER"_Stz,
  
  /*  */ "LR_GRID"_Stz,
  
  /*  */ "LR_HANDLER"_Stz,
  
  /*  */ "LR_INDEX"_Stz,
  /*  */ "LR_INDEXDEF"_Stz,
  /*  */ "LR_INDEXSTREAM"_Stz,
  /*  */ "LR_INFOSET"_Stz,
  /*  */ "LR_INFOSETROW"_Stz,
  /*  */ "LR_INFOSETTABLE"_Stz,
  /*  */ "LR_INTSTACK"_Stz,
  
  /*  */ "LR_JOBMETRICS"_Stz,
  
  /*  */ "LR_LIST"_Stz,
  /*  */ "LR_LOGICCOMPILER"_Stz,
  
  /*  */ "LR_MARKER"_Stz,
  /*  */ "LR_MEMORYHOG"_Stz,
  /*  */ "LR_META"_Stz,
  
  /*  */ "LR_NAMEDBUFFER"_Stz,
  /*  */ "LR_NODE"_Stz,
  /*  */ "LR_NODE_I"_Stz,
  /*  */ "LR_NOW"_Stz,
  
  /*  */ "LR_OP"_Stz,
  /*  */ "LR_OPERANDOOO"_Stz,
  /*  */ "LR_OPTIMISER"_Stz,
  
  /*  */ "LR_PARCEL"_Stz,
  /*  */ "LR_PATHIDENTIFIER"_Stz,
  /*  */ "LR_PIPEIN"_Stz,
  /*  */ "LR_PIPELINE"_Stz,
  /*  */ "LR_PIPESPEC"_Stz,
  /*  */ "LR_PLANOPERAND"_Stz,
  /*  */ "LR_POT"_Stz,
  /*  */ "LR_PREEXPSTACK"_Stz,
  /*  */ "LR_PTNSTREAM"_Stz,
  
  /*  */ "LR_RECVDOBJECT"_Stz,
  /*  */ "LR_RECVWL"_Stz,
  /*  */ "LR_ROWBUFFER"_Stz,
  /*  */ "LR_ROWONCE"_Stz,
  
  /*  */ "LR_SENDNODE"_Stz,
  /*  */ "LR_SENDS"_Stz,
  /*  */ "LR_SENDWL"_Stz,
  /*  */ "LR_SHARDDEF"_Stz,
  /*  */ "LR_SOCKET"_Stz,
  /*  */ "LR_STAGE"_Stz,
  /*  */ "LR_STATS"_Stz,
  /*  */ "LR_STENCIL"_Stz,
  /*  */ "LR_STREAM"_Stz,
  /*  */ "LR_STREAMDEFLFT"_Stz,
  /*  */ "LR_STREAMDEFOUT"_Stz,
  /*  */ "LR_STREAMDEFRHT"_Stz,
  /*  */ "LR_STREAMSET"_Stz,
  /*  */ "LR_STREAMSETTABLE"_Stz,
  /*  */ "LR_SUBCALL"_Stz,
  /*  */ "LR_SUBPIPE"_Stz,
  /*  */ "LR_SUBRETURN"_Stz,
  /*  */ "LR_SUBSET"_Stz,
  
  /*  */ "LR_TABLE"_Stz,
  /*  */ "LR_TABLEPTN"_Stz,
  /*  */ "LR_TAGSET"_Stz,
  /*  */ "LR_TIMER"_Stz,
  /*  */ "LR_TINYC"_Stz,
  /*  */ "LR_TOBKTCRW"_Stz,
  /*  */ "LR_TOBKTROW"_Stz,
  /*  */ "LR_TONSTROW"_Stz,
  /*  */ "LR_TOPTNCRW"_Stz,
  /*  */ "LR_TOPTNROW"_Stz,
  /*  */ "LR_TOROW"_Stz,
  /*  */ "LR_TUB"_Stz,
  /*  */ "LR_TX"_Stz,
  
  /*  */ "LR_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
