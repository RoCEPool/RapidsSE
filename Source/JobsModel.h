/*============================================================================*/
/* JobsModel.h -- Jobs model namespace                                        */
/*============================================================================*/
#pragma once

#include "Plans.h"

//-----------------------------------------------------------------------------
namespace JobsModel
{
  void vCheckAssumptions(void);
}

using namespace JobsModel;

//-----------------------------------------------------------------------------

#include "Job.hpp"

#include "Job__Begun.hpp"
#include "Job__End.hpp"
#include "Job__Ended.hpp"
#include "Job__Error.hpp"
#include "Job__Execute.hpp"
#include "Job__Executed.hpp"

#include "Job_CreateTable.hpp"
#include "Job_CreateSchema.hpp"
#include "Job_DropSchema.hpp"
#include "Job_DropTable.hpp"

#include "Jobs.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
