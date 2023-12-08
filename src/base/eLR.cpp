/*============================================================================*/
/* eLR.cpp -- Logic Unit Registers                                            */
/*============================================================================*/
#include "BASE.h"
#include "eLR.h"

Stz* aLR[LR_ENUMS]
{
  /*  */ "LR_NONE"_Stz,
  
  /*  */ "LR_ADAPTORWL"_Stz,
  /*  */ "LR_AGGTIMER"_Stz,
  /*  */ "LR_AGGTYPESTACK"_Stz,
  /*  */ "LR_ANSWERWL"_Stz,

  /*  */ "LR_BKTMAP"_Stz,
  
  /*  */ "LR_CHILDEXIT"_Stz,
  /*  */ "LR_COMBINED"_Stz,
  /*  */ "LR_CRWBUFFER"_Stz,
  /*  */ "LR_CRWTOROW"_Stz,
  /*  */ "LR_CSVBUFFER"_Stz,
  
  /*  */ "LR_DATABUFFER"_Stz,
  /*  */ "LR_DECK"_Stz,
  /*  */ "LR_DIR0"_Stz,
  /*  */ "LR_DIR1"_Stz,
  /*  */ "LR_DYNAMIC"_Stz,
  /*  */ "LR_DYNAMO"_Stz,
  
  /*  */ "LR_ERRORMARKER"_Stz,
  /*  */ "LR_EXITOPERAND"_Stz,
  /*  */ "LR_EXPEVAL"_Stz,
  
  /*  */ "LR_FILE0"_Stz,
  /*  */ "LR_FILE1"_Stz,
  /*  */ "LR_FILTER"_Stz,
  
  /*  */ "LR_GRID"_Stz,
  
  /*  */ "LR_HANDLER"_Stz,
  
  /*  */ "LR_INDEX"_Stz,
  /*  */ "LR_INDEXDEF"_Stz,
  /*  */ "LR_INDEXSTREAM"_Stz,
  /*  */ "LR_INTSTACK"_Stz,
  
  /*  */ "LR_JOBMETRICS"_Stz,

  /*  */ "LR_LIBINFO"_Stz,
  /*  */ "LR_LIST"_Stz,

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
  /*  */ "LR_PDSCURSOR"_Stz,
  /*  */ "LR_PATHIDENTIFIER"_Stz,
  /*  */ "LR_PIPEIN"_Stz,
  /*  */ "LR_PIPELINE"_Stz,
  /*  */ "LR_PLANOPERAND"_Stz,
  /*  */ "LR_PORT"_Stz,
  /*  */ "LR_POT"_Stz,
  /*  */ "LR_PREEXPSTACK"_Stz,
  /*  */ "LR_PTNDEF"_Stz,
  
  /*  */ "LR_RECVDOBJECT"_Stz,
  /*  */ "LR_RECVWL"_Stz,
	/*  */ "LR_ROW"_Stz,
  /*  */ "LR_ROWBUFFER"_Stz,
	/*  */ "LR_ROWBUFFERERR"_Stz,
	/*  */ "LR_ROWBUFFERWARN"_Stz,
	/*  */ "LR_ROWONCE"_Stz,
  /*  */ "LR_ROWCOUNTS"_Stz,

  /*  */ "LR_SENDNODE"_Stz,
  /*  */ "LR_SENDS"_Stz,
  /*  */ "LR_SENDWL"_Stz,
  /*  */ "LR_SOCKET"_Stz,
  /*  */ "LR_STAGE"_Stz,
  /*  */ "LR_STATS"_Stz,
  /*  */ "LR_STENCIL"_Stz,
  /*  */ "LR_STREAM"_Stz,
  /*  */ "LR_STREAMDEFLFT"_Stz,
  /*  */ "LR_STREAMDEFOUT"_Stz,
  /*  */ "LR_STREAMDEFRHT"_Stz,
  /*  */ "LR_SUBCALL"_Stz,
  /*  */ "LR_SUBPIPE"_Stz,
  /*  */ "LR_SUBRETURN"_Stz,
  /*  */ "LR_SUBSET"_Stz,
  
  /*  */ "LR_TABLE"_Stz,
	/*  */ "LR_TABLEDEF"_Stz,
	/*  */ "LR_TABLEDEFERR"_Stz,
	/*  */ "LR_TABLEDEFWARN"_Stz,
	/*  */ "LR_TABLEPTN"_Stz,
  /*  */ "LR_TAGSET"_Stz,
  /*  */ "LR_TASKLIST"_Stz,
  /*  */ "LR_TEXT"_Stz,
  /*  */ "LR_TIMER"_Stz,
  /*  */ "LR_TINYC"_Stz,
  /*  */ "LR_TOKEYHSHROW"_Stz,
  /*  */ "LR_TOLGLPTNCRW"_Stz,
  /*  */ "LR_TOLGLPTNNST"_Stz,
  /*  */ "LR_TOLGLPTNROW"_Stz,
  /*  */ "LR_TUB"_Stz,
  /*  */ "LR_TX"_Stz,
  
  /*  */ "LR_VERTEX"_Stz,
  /*  */ "LR_VERTEXSPEC"_Stz,

//  /*  */ "LR_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
