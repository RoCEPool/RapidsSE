/*============================================================================*/
/* eBT.h -- Buffer Type                                                       */
/*============================================================================*/
#pragma once

enum eBT // buffer type
{
  /*  0 */ BT_NONE=0,

  /*    */ BT_AFFECTEDROWS,
	/*    */ BT_CALLPIPELINE,
	/*    */ BT_COMMAND,
	/*    */ BT_COMPILEERRORS,
	/*    */ BT_COUNT,
	/*    */ BT_DATA,
	/*    */ BT_DEREFMETRICS,
	/*    */ BT_EXECERRORS,
	/*    */ BT_EXPLAIN,
	/*    */ BT_FORKCOMMAND,
	/*    */ BT_JOBOPERANDS,
	/*    */ BT_JOBOPERATORS,
	/*    */ BT_JOBMETRICS,
	/*    */ BT_JOBNETDISSECT,
	/*    */ BT_JOBNETWORK,
	/*    */ BT_LOADRESULT,
	/*    */ BT_LOCKSTATS,
	/*    */ BT_NODEINFO,
	/*    */ BT_PAGEINREQUEST,
	/*    */ BT_PATH,
	/*    */ BT_PROGRESSUNITS,
	/*    */ BT_PROMPTOUT,
	/*    */ BT_SEEKOFFSETS,
	/*    */ BT_SYSLOCKS,
	/*    */ BT_SYSMETRICS,
	/*    */ BT_TOTALUNITS,
	/*    */ BT_WARNINGS,

  /*    */ BT_ENUMS,
};

extern Stz* aBT[BT_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
