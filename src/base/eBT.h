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
	/*    */ BT_EXECERRORS,
	/*    */ BT_EXPLAIN,
	/*    */ BT_FORKCOMMAND,
	/*    */ BT_LOADRESULT,
	/*    */ BT_LOCKSTATS,
	/*    */ BT_NODEINFO,
	/*    */ BT_PAGEINREQUEST,
	/*    */ BT_PATH,
	/*    */ BT_PROGRESSUNITS,
	/*    */ BT_PROMPTOUT,
	/*    */ BT_SEEKOFFSETS,
	/*    */ BT_TOTALUNITS,
	/*    */ BT_WARNINGS,

  /*    */ BT_ENUMS,
};

extern Stz* aBT[BT_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
