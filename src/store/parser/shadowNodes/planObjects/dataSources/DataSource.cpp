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
  //else if (IndexScanOp::bIsOfType(pNode))      { IndexScanOp::vDescribe(pNode, sBuf);   }

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


/*----------------------------------------------------------------------------*/
/* Retrieves the catalogname of the data source, if applicable, otherwise null*/
/*----------------------------------------------------------------------------*/
Stz* DataSource::szCatalogName(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  // TableScanOp
  if      (TableScanOp::bIsOfType(pNode))  { return TableScanOp::szCatalogName(pNode); }
//else if (IndexScanOp::bIsOfType(pNode))  { return IndexScanOp::szCatalogName(pNode); }
//else if ( SubqueryOp::bIsOfType(pNode))  { return nullptr;                           }
  else                                     {  NEVER_REACH                              }
}

/*----------------------------------------------------------------------------*/
/* Retrieves the schema name of the data source, if applicable, otherwise null*/
/*----------------------------------------------------------------------------*/
Stz* DataSource::szSchemaName(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  // TableScanOp
  if      (TableScanOp::bIsOfType(pNode))  { return TableScanOp::szSchemaName(pNode); }
//else if (IndexScanOp::bIsOfType(pNode))  { return IndexScanOp::szSchemaName(pNode); }
//else if ( SubqueryOp::bIsOfType(pNode))  { return nullptr;                          }
  else                                     {  NEVER_REACH                             }
}

/*----------------------------------------------------------------------------*/
/* Retrieves the table name of the data source, if applicable, otherwise null */
/*----------------------------------------------------------------------------*/
Stz* DataSource::szTableName(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  // TableScanOp
  if      (TableScanOp::bIsOfType(pNode))  { return TableScanOp::szTableName(pNode); }
//else if (IndexScanOp::bIsOfType(pNode))  { return IndexScanOp::szTableName(pNode); }
//else if ( SubqueryOp::bIsOfType(pNode))  { return nullptr;                         }
  else                                     {  NEVER_REACH                            }
}

/*----------------------------------------------------------------------------*/
/* Get the list of column definitions for this data source                    */
/* Returned list must be freed, but not its contents                          */
/*----------------------------------------------------------------------------*/
PlanNodeList* DataSource::pColumnDefs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  if      (TableScanOp::bIsOfType(pNode))  { return TableScanOp::pColumnDefs(pNode, pRootNode); }
//else if (IndexScanOp::bIsOfType(pNode))  { return IndexScanOp::pColumnDefs(pNode, pRootNode); }
//else if ( SubqueryOp::bIsOfType(pNode))  { return  SubqueryOp::pColumnDefs(pNode, pRootNode); }
  else                                     {  NEVER_REACH                                       }
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedExprs that represent the column expressions      */
/* that this PlanObject will return. All nodes returned here must be freed.   */
/*----------------------------------------------------------------------------*/
PlanNodeList* DataSource::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);

  // manual polymorphism here

  if      (TableScanOp::bIsOfType(pNode))  { return TableScanOp::pColumnExprs(pNode, pRootNode); }
//else if (IndexScanOp::bIsOfType(pNode))  { return IndexScanOp::pColumnExprs(pNode, pRootNode); }
//else if ( SubqueryOp::bIsOfType(pNode))  { return  SubqueryOp::pColumnExprs(pNode, pRootNode); }
  else                                     {  NEVER_REACH                                       }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
