/*============================================================================*/
/* DataSource.hpp - a source of row data into the query                       */
/*============================================================================*/

#include "BASE.h"
#include "DataSource.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"

#include "TableScanOp.hpp"
#include "SubqueryRefOp.hpp"
#include "StringObject.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DATASOURCE_DEFN = "/__root__/definitions/object/planObject/sqlOperator/dataSource"_Stz;

static Stz* C_ALIAS_PROP = "alias"_Stz;

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

  // TODO: manual polymorphism here

  // TableScanOp
  if      (  TableSource::bIsOfType(pNode))    {   TableSource::vDescribe(pNode, sBuf); }
  else if (SubqueryRefOp::bIsOfType(pNode))    { SubqueryRefOp::vDescribe(pNode, sBuf); }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void DataSource::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism here

  // TableScanOp
  if      (  TableSource::bIsOfType(pNode))   {   TableSource::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if (SubqueryRefOp::bIsOfType(pNode))   { SubqueryRefOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void DataSource::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s2K sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Explain any local attributes of this object


    // Get the tableref property
    PlanNode* pAliasNode = GraphNav::pFindProperty(const_cast<Stz*>(C_ALIAS_PROP), pNode);

    if (pAliasNode && pAliasNode->szValue()) {
      ASSERT(StringObject::bIsOfType(pAliasNode));

      // Print the alias name
      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, IF_PLAIN);
      sBuf.vCatSprint("alias: (%s)"_Stz, pAliasNode->szValue());
    }

    return;
  }

  // Manual polymorphism here
  if      (  TableSource::bIsOfType(szType))   {   TableSource::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
  else if (SubqueryRefOp::bIsOfType(szType))   { SubqueryRefOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);  }

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
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 DataSource::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_DATASOURCE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 DataSource::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DATASOURCE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 DataSource::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_DATASOURCE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the data source has an alias                               */
/*----------------------------------------------------------------------------*/
b1 DataSource::bHasAlias(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  PlanNode* pAliasNode = GraphNav::pFindProperty(const_cast<Stz*>(C_ALIAS_PROP), pNode);
  ASSERT(!pAliasNode || StringObject::bIsOfType(pAliasNode));

  return (pAliasNode && pAliasNode->szValue());
}

/*----------------------------------------------------------------------------*/
/* Sets an alias name into this data source                                   */
/*----------------------------------------------------------------------------*/
void DataSource::vSetAlias(QueryNode* pNode, Text* pAliasText)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pAliasNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_ALIAS_PROP), pNode));
  ASSERT(!pAliasNode);

  pAliasNode = StringObject::pCreate(pAliasText);
  pAliasNode->vSetName(C_ALIAS_PROP);
  pNode->vAppendProperty(pAliasNode);
}

/*----------------------------------------------------------------------------*/
/* Retrieves the alias name of the data source, if applicable, otherwise null */
/*----------------------------------------------------------------------------*/
Stz* DataSource::szAliasName(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  PlanNode* pAliasNode = GraphNav::pFindProperty(const_cast<Stz*>(C_ALIAS_PROP), pNode);
  if (!pAliasNode) {
    return nullptr;
  }

  ASSERT(StringObject::bIsOfType(pAliasNode));
  return pAliasNode->szValue();
}

/*----------------------------------------------------------------------------*/
/* Retrieves the catalogname of the data source, if applicable, otherwise null*/
/*----------------------------------------------------------------------------*/
Stz* DataSource::szCatalogName(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  if      (  TableSource::bIsOfType(pNode))  { return TableSource::szCatalogName(pNode); }
  else if (SubqueryRefOp::bIsOfType(pNode))  { return nullptr;                           }
  else                                       {  NEVER_REACH                              }
}

/*----------------------------------------------------------------------------*/
/* Retrieves the schema name of the data source, if applicable, otherwise null*/
/*----------------------------------------------------------------------------*/
Stz* DataSource::szSchemaName(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  if      (  TableSource::bIsOfType(pNode))  { return TableSource::szSchemaName(pNode); }
  else if (SubqueryRefOp::bIsOfType(pNode))  { return nullptr;                          }
  else                                       {  NEVER_REACH                             }
}

/*----------------------------------------------------------------------------*/
/* Retrieves the table name of the data source, if applicable, otherwise null */
/*----------------------------------------------------------------------------*/
Stz* DataSource::szTableName(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  if      (  TableSource::bIsOfType(pNode))  { return TableSource::szTableName(pNode); }
  else if (SubqueryRefOp::bIsOfType(pNode))  { return nullptr;                         }
  else                                       {  NEVER_REACH                            }
}

/*----------------------------------------------------------------------------*/
/* Get the list of column definitions for this data source                    */
/* Returned list must be freed, but not its contents                          */
/*----------------------------------------------------------------------------*/
QueryNodeList* DataSource::pColumnDefs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  if      (  TableSource::bIsOfType(pNode))  { return    TableSource::pColumnDefs(pNode, pRootNode); }
  else if (SubqueryRefOp::bIsOfType(pNode))  { return  SubqueryRefOp::pColumnDefs(pNode, pRootNode); }
  else                                       {  NEVER_REACH                                       }
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions    */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* DataSource::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);

  // manual polymorphism here

  if      (  TableSource::bIsOfType(pNode))  { return   TableSource::pColumnExprs(pNode, pRootNode); }
  else if (SubqueryRefOp::bIsOfType(pNode))  { return SubqueryRefOp::pColumnExprs(pNode, pRootNode); }
  else                                       {  NEVER_REACH                                          }
}

/*----------------------------------------------------------------------------*/
/* Returns true if this data source can resolve the given column reference    */
/*----------------------------------------------------------------------------*/
b1 DataSource::bResolvesColumnRef(QueryNode* pNode, QueryNode* pMetadataNode,
                                  PlanNode* pRootNode, Stz* szColRefTableAliasName,
                                  Stz* szCatalogName,  Stz* szSchemaName,
                                  Stz* szTableName,    Stz* szColumnName)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // manual polymorphism here
  if      (  TableSource::bIsOfType(pNode))  { return   TableSource::bResolvesColumnRef(pNode, pMetadataNode, pRootNode, szColRefTableAliasName, szCatalogName, szSchemaName, szTableName, szColumnName);    }
  else if (SubqueryRefOp::bIsOfType(pNode))  { return SubqueryRefOp::bResolvesColumnRef(pNode, pMetadataNode, pRootNode, szColRefTableAliasName, szCatalogName, szSchemaName, szTableName, szColumnName); }
  else                                       {  NEVER_REACH }
}

/*----------------------------------------------------------------------------*/
/* Prints into the buffer the identifier for the data source.                 */
/* If an alias is set then that is used. Otherwise it could be a table name.  */
/*----------------------------------------------------------------------------*/
void DataSource::vPrintName(QueryNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Partial manual polymorphism here
  if (TableSource::bIsOfType(pNode))  { TableSource::vPrintName(pNode, sBuf); }
  else {
    Stz* szAlias = szAliasName(pNode);
    ASSERT(szAlias);
    sBuf.vCopy(szAlias);
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if this operator (and any expressions it may have) should be  */
/* partitioned when the plan is partitioned. Most operators will simply ask   */
/* this question of their child until we get to a data source. Most tables    */
/* would answer true. A replicated table or data source would answer false.   */
/*----------------------------------------------------------------------------*/
b1 DataSource::bIsPartitionable(QueryNode* pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(PlanObject::bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 0);

  // manual polymorphism here
  if      (  TableSource::bIsOfType(pNode))  { return   TableSource::bIsPartitionable(pNode, pRootNode); }
  else if (SubqueryRefOp::bIsOfType(pNode))  { return SubqueryRefOp::bIsPartitionable(pNode, pRootNode); }
  else                                       {  NEVER_REACH }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
