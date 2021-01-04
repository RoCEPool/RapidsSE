/*============================================================================*/
/* SqlOperator.hpp - an abstract base class of all objects that process       */
/* streams of row data.                                                       */
/*============================================================================*/

#include "BASE.h"
#include "SqlOperator.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "ProjectOp.hpp"
#include "DataSource.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLOPERATOR_DEFN = "/__root__/definitions/object/planObject/sqlOperator"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Appends a description of the statement into a buffer                       */
/*----------------------------------------------------------------------------*/
void SqlOperator::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bValidate(pNode));

  QueryNode* pExpr = static_cast<QueryNode*>(pNode);

  // TODO: manual polymorphism here

  // Projection
  if      (ProjectOp::bIsOfType(pNode))     { ProjectOp::vDescribe(pNode, sBuf);  }

  // Filter
  //else if (FilterOp::bIsOfType(pNode))      { FilterOp::vDescribe(pNode, sBuf);   }

  // DataSource
  else if (DataSource::bIsOfType(pNode))    { DataSource::vDescribe(pNode, sBuf);   }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlOperator::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLOPERATOR_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
