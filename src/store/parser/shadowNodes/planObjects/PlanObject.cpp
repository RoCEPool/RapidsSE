/*============================================================================*/
/* PlanObject.hpp - an abstract base class of all object that appear in the   */
/* main data path of the plan graph.                                          */
/*============================================================================*/

#include "BASE.h"
#include "PlanObject.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "Statement.hpp"
#include "SqlOperator.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_PLANOBJECT_DEFN = "/__root__/definitions/object/planObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Appends a description of the statement into a buffer                       */
/*----------------------------------------------------------------------------*/
void PlanObject::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bValidate(pNode));

  QueryNode* pExpr = static_cast<QueryNode*>(pNode);

  // TODO: manual polymorphism here

  // Statement
  if      (Statement::bIsOfType(pNode))     { Statement::vDescribe(pNode, sBuf);    }

  // SqlOperator
  else if (SqlOperator::bIsOfType(pNode))   { SqlOperator::vDescribe(pNode, sBuf);  }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 PlanObject::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_PLANOBJECT_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
