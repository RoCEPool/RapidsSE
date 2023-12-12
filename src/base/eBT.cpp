/*============================================================================*/
/* eBT.cpp -- Buffer Type                                                     */
/*============================================================================*/
#include "BASE.h"
#include "eBT.h"

Stz* aBT[BT_ENUMS]
{
  /*  0 */ "NONE"_Stz,

  /*    */ "BT_AFFECTEDROWS"_Stz,
	/*    */ "BT_CALLPIPELINE"_Stz,
	/*    */ "BT_COMMAND"_Stz,
	/*    */ "BT_COMPILEERRORS"_Stz,
	/*    */ "BT_COUNT"_Stz,
	/*    */ "BT_DATA"_Stz,
	/*    */ "BT_DEREFMETRICS"_Stz,
	/*    */ "BT_EXECERRORS"_Stz,
	/*    */ "BT_EXPLAIN"_Stz,
	/*    */ "BT_FORKCOMMAND"_Stz,
	/*    */ "BT_JOBOPERANDS"_Stz,
	/*    */ "BT_JOBOPERATORS"_Stz,
	/*    */ "BT_JOBMETRICS"_Stz,
	/*    */ "BT_JOBNETDISSECT"_Stz,
	/*    */ "BT_JOBNETWORK"_Stz,
	/*    */ "BT_LOADRESULT"_Stz,
	/*    */ "BT_LOCKSTATS"_Stz,
	/*    */ "BT_NODEINFO"_Stz,
	/*    */ "BT_PAGEINREQUEST"_Stz,
	/*    */ "BT_PATH"_Stz,
	/*    */ "BT_PROGRESSUNITS"_Stz,
	/*    */ "BT_PROMPTOUT"_Stz,
	/*    */ "BT_SEEKOFFSETS"_Stz,
  /*    */ "BT_SYSLOCKS"_Stz,
	/*    */ "BT_SYSMETRICS"_Stz,
	/*    */ "BT_TOTALUNITS"_Stz,
	/*    */ "BT_WARNINGS"_Stz,

//  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
