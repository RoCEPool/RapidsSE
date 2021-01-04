/*============================================================================*/
/* DataSource.hpp - a source of row data into the query                       */
/*============================================================================*/

#include "BASE.h"
#include "DataSource.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "TableScanOp.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DATASOURCE_DEFN = "/__root__/definitions/object/planObject/sqlOperator/dataSource"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Appends a description of the statement into a buffer                       */
/*----------------------------------------------------------------------------*/
void DataSource::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bValidate(pNode));

  QueryNode* pExpr = static_cast<QueryNode*>(pNode);

  // TODO: manual polymorphism here

  // TableScanOp
  if      (TableScanOp::bIsOfType(pNode))     { TableScanOp::vDescribe(pNode, sBuf);  }

  // Others
  //else if (FilterOp::bIsOfType(pNode))      { FilterOp::vDescribe(pNode, sBuf);   }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 DataSource::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DATASOURCE_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
