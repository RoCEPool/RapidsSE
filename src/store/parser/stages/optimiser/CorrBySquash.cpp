/*============================================================================*/
/* CorrBySquash.hpp - Push down ProjectOps that implement the CORRESPONDINB BY*/
/* clause and squash any redundant ProjectOps below it                        */
/*============================================================================*/

#include "BASE.h"
#include "CorrBySquash.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "ShadowRoot.hpp"
#include "ShadowPlan.hpp"
#include "StatementMarker.hpp"
#include "SqlException.hpp"
#include "ProjectedColumn.hpp"
#include "StringObject.hpp"
#include "ProjectOp.hpp"
#include "Expr.hpp"
#include "ColumnRef.hpp"

#include "SetOp.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Searches for any CORRESPONDING BY ProjectOps and squashes them if possible */
/*----------------------------------------------------------------------------*/
void CorrBySquash::vSearchAndSquash(PlanNode *pRootNode)
{
  PlanNode* pPlanNode = ShadowRoot::pPlanNode(pRootNode);
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));


}



/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
