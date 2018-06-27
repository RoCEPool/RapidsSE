/*============================================================================*/
/* JobModel.h -- Job Model namespace                                          */
/*============================================================================*/
#pragma once

#include "Plans.h"

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

#include "Job__Begun.hpp"
#include "Job__End.hpp"
#include "Job__Ended.hpp"
#include "Job__Error.hpp"
#include "Job__Execute.hpp"
#include "Job__Executed.hpp"

#include "Job_CreateCatalog.hpp"
#include "Job_CreateSchema.hpp"
#include "Job_CreateTable.hpp"
#include "Job_CreateTenant.hpp"

#include "Job_DropCatalog.hpp"
#include "Job_DropSchema.hpp"
#include "Job_DropTable.hpp"
#include "Job_DropTenant.hpp"

#include "Job_G35.hpp"

#include "Jobs.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
