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
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void DataSource::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

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
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void DataSource::vExplain(PlanNode* pNode, s48K& sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pExpr = static_cast<QueryNode*>(pNode);

  // Manual polymorphism here

  // TableScanOp
  if      (TableScanOp::bIsOfType(pNode))   { TableScanOp::vExplain(pNode, sBuf, iIndent); }

  // IndexScanOp
  //else if (IndexScanOp::bIsOfType(pNode))   { IndexScanOp::vExplain(pNode, sBuf, iIndent); }

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
