/*============================================================================*/
/* JobModel.h -- Job Model namespace                                          */
/*============================================================================*/
#pragma once

#include "Plans.h"

//-----------------------------------------------------------------------------
namespace Prompt
{
  class SePrompt;
}

//-----------------------------------------------------------------------------
namespace Interface
{
  class SEitf;
}

//-----------------------------------------------------------------------------
namespace JobsModel
{
  void vCheckAssumptions(void);
}

using namespace JobsModel;

//-----------------------------------------------------------------------------

#include "Job.hpp"

#include "JobDicPath.hpp"
#include "JobObjectName.hpp"
#include "JobResult.hpp"

#include "Job__Begun.hpp"
#include "Job__Cancel.hpp"
#include "Job__Cancelled.hpp"
#include "Job__End.hpp"
#include "Job__Ended.hpp"
#include "Job__Execute.hpp"
#include "Job__Executed.hpp"

#include "Job_Cf.hpp"
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

#include "Job_G35.hpp"

#include "Job_Lf.hpp"
#include "Job_ListColumns.hpp"
#include "Job_ListCatalogs.hpp"
#include "Job_ListTables.hpp"
#include "Job_ListTenants.hpp"

#include "Job_Prompt.hpp"
#include "Job_Pwf.hpp"
#include "Job_Sanity.hpp"
#include "Job_Space.hpp"
#include "Job_Status.hpp"

#include "Jobs.hpp"

#include "JobClass.hpp"
#include "JobCatalog.hpp"
#include "JobNote.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
