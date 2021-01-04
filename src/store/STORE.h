/*============================================================================*/
/* STORE.h -- STORE namespace                                                 */
/*============================================================================*/
#pragma once

//------------------------------------------------------------------------------

namespace STORE
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

// declare a function pointer to any member function of Step
typedef void (Step::*fpSTEPFN)(void);

//------------------------------------------------------------------------------

#include "Args.hpp"

//------------------------------------------------------------------------------

#include "Definition.hpp"

//------------------------------------------------------------------------------

#include "BucketMap.hpp"
#include "PtnMap.hpp"

#include "StreamDef.hpp"

#include "IndexDef.hpp"

#include "Index.hpp"

#include "Folder.hpp"
#include "Schema.hpp"
#include "Catalog.hpp"
#include "Tenant.hpp"
#include "Tenants.hpp"

#include "Marker.hpp"

//------------------------------------------------------------------------------

#include "Row.hpp"
#include "NmlStr.hpp"
#include "Record.hpp"
#include "RowBox_t.hpp"

#include "RowSingle.hpp"

//------------------------------------------------------------------------------

#include "RowUte.hpp"

#include "RowBuffer.hpp"

#include "HashSet.hpp"
#include "Pot.hpp"


#include "InfoSetBase.hpp"
#include "InfoSet.hpp"
#include "StreamSet.hpp"

#include "Pay.hpp"
#include "Key.hpp"
#include "NEO.h"
#include "Ent.hpp"
#include "Ent_flat.hpp"
#include "EntWrk.hpp"

//------------------------------------------------------------------------------

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "OverLine.hpp"
#include "TailLine.hpp"
#include "TailStr.hpp"
#include "Crw.hpp"

//------------------------------------------------------------------------------

#include "Encoding_v2.hpp"
#include "Encoding.hpp"
#include "Packing_v2.hpp"

#include "CrwBuffer.hpp"
#include "RowCompactor.hpp"
#include "RowInflator.hpp"

#include "KeyBuilder.hpp"

#include "SkipRoots.hpp"

#include "EntBuffer.hpp"

#include "EntBufSorter.hpp"

#include "IndexTree.hpp"

#include "Crv.hpp"
#include "CrvBuffer.hpp"
#include "ReadApi.hpp"

//------------------------------------------------------------------------------

#include "TablePtn.hpp"

#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "PtnStream.hpp"

#include "Table.hpp"

#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "Session.hpp"

//------------------------------------------------------------------------------

#include "Arg.hpp"
#include "RelOp.hpp"
#include "Predicate.hpp"
#include "PredicateVec.hpp"

#include "AccessPath.hpp"

#include "Plan.hpp"
#include "Planner.hpp"

#include "OpSpec.hpp"
#include "StageSpec.hpp"
#include "PipelineSpec.hpp"

#include "Xmits.hpp"
//.#include "Op.hpp"
//#include "Stream.hpp"
//#include "Stage.hpp"
//#include "Pipeline.hpp"

//#include "Response.hpp"

//#include "Result.hpp"
//#include "JobFilePath.hpp"
//#include "Job.hpp"
//#include "JobLog.hpp"
//#include "Job__Find.hpp"
//#include "XmitOperand.hpp"
//#include "JobList.hpp"

//#include "Procedure.hpp"

//#include "Dynamo.hpp"

//------------------------------------------------------------------------------

#include "RluCommon.h"

#include "RluDiary.hpp"

#include "Device.hpp"
#include "DevIXT.hpp"
#include "DevROW.hpp"

#include "LogicTimer.hpp"

#include "LogicCompiler.hpp"
#include "Project.hpp"
#include "Filter.hpp"
#include "Range.hpp"

#include "LogicUnit.hpp"

//------------------------------------------------------------------------------

#include "Scheduler.hpp"

#include "Thread.hpp"
#include "ThreadQueue.hpp"
#include "ThreadVec.hpp"

#include "Queues.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
