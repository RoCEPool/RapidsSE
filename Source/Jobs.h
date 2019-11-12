/*============================================================================*/
/* Jobs.h -- Jobs namespace                                                   */
/*============================================================================*/
#pragma once

#include "DataOperators.h"

//------------------------------------------------------------------------------

namespace Jobs
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "Completer.hpp"
#include "ExportBuffer.hpp"
#include "FetchBuffer.hpp"

#include "Response.hpp"

#include "JobExtractRow.hpp"
#include "JobFilePath.hpp"
#include "JobIndex.hpp"
#include "JobResult.hpp"
#include "JobTable.hpp"

#include "Job.hpp"

#include "JobLog.hpp"

#include "JobWriteBehind.hpp"

#include "RowAgg.hpp"
#include "RowOnce.hpp"

#include "JobHelper.hpp"

#include "Job__Begun.hpp"
#include "Job__Cancel.hpp"
#include "Job__Cancelled.hpp"
#include "Job__End.hpp"
#include "Job__Ended.hpp"
#include "Job__Execute.hpp"
#include "Job__Executed.hpp"
#include "Job__Find.hpp"

#include "Job_Alarm.hpp"
#include "Job_Answer.hpp"

#include "Job_Boot.hpp"

#include "Job_Connector.hpp"
#include "Job_CopyBuffer.hpp"
#include "Job_CreateIndex.hpp"

#include "Job_Pipeline.hpp"

#include "Job_Export.hpp"

#include "Job_FetchRows.hpp"

#include "Job_InsertRow.hpp"
#include "Job_InsertTable.hpp"

#include "Job_KidExport.hpp"

#include "Job_LoadTable.hpp"

#include "Job_NodeInfo.hpp"

#include "Job_Operand.hpp"

#include "Job_Periodic.hpp"
#include "Job_Publish.hpp"

#include "Job_Purge.hpp"

#include "Job_Receiver.hpp"

#include "Job_Sanity.hpp"
#include "Job_Select.hpp"
#include "Job_Start.hpp"
#include "Job_StartCluster.hpp"

#include "Job_SubExporter.hpp"
#include "Job_SubFetch.hpp"
#include "Job_SubInserter.hpp"
#include "Job_SubLoader.hpp"
#include "Job_SubPublish.hpp"
#include "Job_SubPurger.hpp"
#include "Job_SubScan.hpp"
#include "Job_SubTruncate.hpp"
#include "Job_SubWalker.hpp"
#include "Job_Supervisor.hpp"

#include "Job_Universal.hpp"

#include "Job_Waker.hpp"
#include "Job_Walk.hpp"
#include "Job_WriteBuffer.hpp"
#include "Job_WriteRow.hpp"

#include "JobList.hpp"

#include "JobDesc.hpp"
#include "JobClass.hpp"
#include "JobCatalog.hpp"

#include "SP_Job.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
