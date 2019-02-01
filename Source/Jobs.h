/*============================================================================*/
/* Jobs.h -- Jobs namespace                                                   */
/*============================================================================*/
#pragma once

#include "Plans.h"

//------------------------------------------------------------------------------
namespace Prompt
{
  class SePrompt;
}

//------------------------------------------------------------------------------
namespace Interface
{
  class Session;
}

//------------------------------------------------------------------------------
namespace Jobs
{
  void vCheckAssumptions(void);
}

using namespace Jobs;

//------------------------------------------------------------------------------

enum eJL // jobs list
{
  /* 0 */ JL_NONE=0,
  /* 1 */ JL_ORIGINATING,
  /* 2 */ JL_PARTICIPATING
};

static Stz* aJL[]
{
  /* 0 */ "None"_Stz,
  /* 1 */ "Originating"_Stz,
  /* 2 */ "Participating"_Stz
};

//------------------------------------------------------------------------------

#include "Decider.hpp"
#include "FetchBuffer.hpp"
#include "JobDicPath.hpp"
#include "JobFilePath.hpp"
#include "JobObjectName.hpp"
#include "JobResult.hpp"
#include "JobRow.hpp"
#include "Job.hpp"
#include "JobWriteBehind.hpp"
#include "RowOnce.hpp"

#include "Job__Begun.hpp"
#include "Job__Cancel.hpp"
#include "Job__Cancelled.hpp"
#include "Job__End.hpp"
#include "Job__Ended.hpp"
#include "Job__Execute.hpp"
#include "Job__Executed.hpp"
#include "Job__Find.hpp"

#include "Job_Alarm.hpp"
#include "Job_AnalyzeTable.hpp"
#include "Job_AnalyzeIndex.hpp"
#include "Job_Answer.hpp"

#include "Job_Boot.hpp"

#include "Job_Cat.hpp"
#include "Job_Cf.hpp"
#include "Job_CompactIndex.hpp"
#include "Job_ConnectCluster.hpp"
#include "Job_Connector.hpp"
#include "Job_CopyBuffer.hpp"
#include "Job_CreateCatalog.hpp"
#include "Job_CreateIndex.hpp"
#include "Job_CreateSchema.hpp"
#include "Job_CreateTable.hpp"
#include "Job_CreateTenant.hpp"

#include "Job_Docol.hpp"
#include "Job_DropCatalog.hpp"
#include "Job_DropIndex.hpp"
#include "Job_DropSchema.hpp"
#include "Job_DropTable.hpp"
#include "Job_DropTenant.hpp"
#include "Job_DumpIndex.hpp"

#include "Job_Export.hpp"

#include "Job_FetchRows.hpp"

#include "Job_Generate.hpp"

#include "Job_Ingest.hpp"
#include "Job_InsertRow.hpp"
#include "Job_InsertTable.hpp"

#include "Job_Lf.hpp"
#include "Job_ListColumns.hpp"
#include "Job_ListCatalogs.hpp"
#include "Job_ListIdxColumns.hpp"
#include "Job_ListIndexes.hpp"
#include "Job_ListPartColumns.hpp"
#include "Job_ListPartitions.hpp"
#include "Job_ListSchemas.hpp"
#include "Job_ListTables.hpp"
#include "Job_ListTenants.hpp"
#include "Job_LoadTable.hpp"

#include "Job_Monitor.hpp"

#include "Job_NetHog.hpp"
#include "Job_NodeInfo.hpp"

#include "Job_Periodic.hpp"
#include "Job_Ping.hpp"
#include "Job_Publish.hpp"
#include "Job_Purge.hpp"
#include "Job_Pwf.hpp"

#include "Job_Receiver.hpp"
#include "Job_Repeater.hpp"
#include "Job_Report.hpp"

#include "Job_Sanity.hpp"
#include "Job_SelectTable.hpp"
#include "Job_Sink.hpp"
#include "Job_SizeIndex.hpp"
#include "Job_SizeTable.hpp"
#include "Job_Space.hpp"
#include "Job_ShowCluster.hpp"
#include "Job_Start.hpp"
#include "Job_StartCluster.hpp"
#include "Job_Status.hpp"

#include "Job_SubFetch.hpp"
#include "Job_SubIngester.hpp"
#include "Job_SubInserter.hpp"
#include "Job_SubLoader.hpp"
#include "Job_SubPublish.hpp"
#include "Job_SubPurger.hpp"
#include "Job_SubTruncate.hpp"

#include "Job_TableSizes.hpp"
#include "Job_TruncateTable.hpp"

#include "Job_UseSchema.hpp"

#include "Job_VerifyIndex.hpp"
#include "Job_VisualizeIndex.hpp"

#include "Job_WaitCluster.hpp"
#include "Job_WriteBuffer.hpp"
#include "Job_WriteRow.hpp"

#include "JobList.hpp"

#include "JobClass.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
