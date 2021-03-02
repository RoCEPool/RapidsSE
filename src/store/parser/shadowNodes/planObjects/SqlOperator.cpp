/*============================================================================*/
/* SqlOperator.hpp - an abstract base class of all objects that process       */
/* streams of row data.                                                       */
/*============================================================================*/

#include "BASE.h"
#include "SqlOperator.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "store/parser/shadowNodes/planObjects/ProjectOp.hpp"
#include "store/parser/shadowNodes/planObjects/AggregateOp.hpp"
#include "store/parser/shadowNodes/planObjects/dataSources/DataSource.hpp"
#include "store/parser/shadowNodes/planObjects/FilterOp.hpp"
#include "store/parser/shadowNodes/planObjects/LimitOp.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLOPERATOR_DEFN = "/__root__/definitions/object/planObject/sqlOperator"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlOperator::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pExpr = static_cast<QueryNode*>(pNode);

  // TODO: manual polymorphism here

  // Aggregation (group by). Must come before ProjectOp
  if      (AggregateOp::bIsOfType(pNode))   { AggregateOp::vDescribe(pNode, sBuf);   }

  // Projection
  else if (ProjectOp::bIsOfType(pNode))     { ProjectOp::vDescribe(pNode, sBuf);  }

  // DataSource
  else if (DataSource::bIsOfType(pNode))    { DataSource::vDescribe(pNode, sBuf); }
  
  // Filter
  else if (FilterOp::bIsOfType(pNode))      { FilterOp::vDescribe(pNode, sBuf);   }

  // Limit
  else if (LimitOp::bIsOfType(pNode))       { LimitOp::vDescribe(pNode, sBuf);    }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void SqlOperator::vExplain(PlanNode* pNode, s48K& sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pExpr = static_cast<QueryNode*>(pNode);

  // Manual polymorphism here

  if      (AggregateOp::bIsOfType(pNode)) { AggregateOp::vExplain(pNode, sBuf, iIndent);  }

  // Projection
  else if (ProjectOp::bIsOfType(pNode))   { ProjectOp::vExplain(pNode, sBuf, iIndent);    }

  // DataSource
  else if (DataSource::bIsOfType(pNode))  { DataSource::vExplain(pNode, sBuf, iIndent);   }

  // Filter
  else if (FilterOp::bIsOfType(pNode))    { FilterOp::vExplain(pNode, sBuf, iIndent);     }
  
  // Limit
  else if (LimitOp::bIsOfType(pNode))     { LimitOp::vExplain(pNode, sBuf, iIndent);      }

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
