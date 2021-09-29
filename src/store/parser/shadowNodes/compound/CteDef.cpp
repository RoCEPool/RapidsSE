/*============================================================================*/
/* CteDef.hpp - a definition of a Common Table Expression                     */
/*============================================================================*/

#include "BASE.h"
#include "CteDef.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "ColumnDefs.hpp"
#include "ColumnDef.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_CTEDEF_DEFN   = "/__root__/definitions/object/compound/cteDef"_Stz;
static Stz* C_CTEDEF_NAME   = "cteDef"_Stz;

static Stz* C_ALIASNAME_PROP  = "aliasName"_Stz;
static Stz* C_ALIASNAME_DEFN  = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_COLUMNS_PROP  = "columns"_Stz;
static Stz* C_COLUMNS_DEFN  = "/__root__/definitions/object/list/columnDefs"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create CteDef nodes.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* CteDef::pCreate(Stz* szAliasName)
{
  _static_

  ASSERT(szAliasName);
  ASSERT(szAliasName->nLength() > 0);

  // Create the CteDef object
  QueryNode* pCteDef = new QueryNode(szAliasName, C_CTEDEF_DEFN);

  // Create the aliasName property
  Value xAliasNameVal(szAliasName);
  QueryNode* pAliasName = new QueryNode(C_ALIASNAME_PROP, C_ALIASNAME_DEFN, static_cast<Value&&>(xAliasNameVal));
  pCteDef->vAppendProperty(pAliasName);

  // Create the columns property
  QueryNode* pColumnDefs = ColumnDefs::pCreate();
  pColumnDefs->vSetName(C_COLUMNS_PROP);
  pCteDef->vAppendProperty(pColumnDefs);

  return pCteDef;
}


/*----------------------------------------------------------------------------*/
/* Helper function to create CteDef nodes.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* CteDef::pCreate(Stz* szAliasName, QueryNode* pColumnDefs)
{
  _static_

  ASSERT(szAliasName);
  ASSERT(szAliasName->nLength() > 0);
  ASSERT(pColumnDefs);
  ASSERT(ColumnDefs::bIsOfType(pColumnDefs));
  ASSERT(pColumnDefs->nChildren() > 0);

  // Create the CteDef object
  QueryNode* pCteDef = new QueryNode(szAliasName, C_CTEDEF_DEFN);

  // Create the aliasName property
  Value xAliasNameVal(szAliasName);
  QueryNode* pAliasName = new QueryNode(C_ALIASNAME_PROP, C_ALIASNAME_DEFN, static_cast<Value&&>(xAliasNameVal));
  pCteDef->vAppendProperty(pAliasName);

  // Create the columns property
  pColumnDefs->vSetName(C_COLUMNS_PROP);
  pCteDef->vAppendProperty(pColumnDefs);

  return pCteDef;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the table reference into a buffer                 */
/*----------------------------------------------------------------------------*/
void CteDef::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pCteDef = _pQueryNode(pNode);

  QueryNode* pAliasName   = _pQueryNode(GraphNav::pFindProperty(C_ALIASNAME_PROP, pCteDef));
  QueryNode* pColumns     = _pQueryNode(GraphNav::pFindProperty(C_COLUMNS_PROP, pCteDef));

  ASSERT(pAliasName);
  ASSERT(pAliasName->szValue());
  ASSERT(pColumns);
  ASSERT(pColumns->nChildren() > 0);

  s2K sColumnDesc;
  ColumnDefs::vDescribe(pColumns, sColumnDesc);
  
  sBuf.vCatSprint("CTE : %s (%s)"_Stz, pAliasName->szValue(), sColumnDesc.szStz());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the CteDef definition     */
/*----------------------------------------------------------------------------*/
b1 CteDef::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_CTEDEF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 CteDef::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_CTEDEF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the alias name from CteDef                                             */
/*----------------------------------------------------------------------------*/
Stz* CteDef::szAliasName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pAliasNameNode = _pQueryNode(GraphNav::pFindProperty(C_ALIASNAME_PROP, pNode));
  ASSERT(pAliasNameNode);

  Stz* szAliasName = pAliasNameNode->szValue();

  return szAliasName;
}

/*----------------------------------------------------------------------------*/
/* Returns a QueryNode that represents a given ColumnDef in this table        */
/*----------------------------------------------------------------------------*/
QueryNode* CteDef::pColumnDef(QueryNode* pNode, Stz* szColumnName)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(szColumnName);

  QueryNode* pColumnDef = nullptr;

  // Get the columns property
  QueryNode* pColumnsNode = _pQueryNode(GraphNav::pFindProperty(C_COLUMNS_PROP, pNode));

  if (!pColumnsNode) {
    return pColumnDef;
  }

  PlanNodeIter* pIter = pColumnsNode->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    ASSERT(ColumnDef::bIsOfType(pChild));

    if (ColumnDef::szColumnName(pChild)->bSame(szColumnName)) {
      pColumnDef = pChild;
      break;
    }
  }

  delete pIter;
  return pColumnDef;
}

/*----------------------------------------------------------------------------*/
/* get an iterator of PlanNodes that represent ColumnDefs                     */
/*----------------------------------------------------------------------------*/
PlanNodeIter* CteDef::pColumnDefIter(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnsNode = _pQueryNode(GraphNav::pFindProperty(C_COLUMNS_PROP, pNode));
  ASSERT(pColumnsNode);

  return pColumnsNode->pNewChildrenIter();
}

/*----------------------------------------------------------------------------*/
/* Returns the number of column definitions                                   */
/*----------------------------------------------------------------------------*/
i8 CteDef::nColumns(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnsNode = _pQueryNode(GraphNav::pFindProperty(C_COLUMNS_PROP, pNode));
  ASSERT(pColumnsNode);

  return pColumnsNode->nChildren();
}

/*----------------------------------------------------------------------------*/
/* Adds a column definition to the list of column definitions                 */
/*----------------------------------------------------------------------------*/
void CteDef::vAddColumnDef(QueryNode *pNode, QueryNode *pColumnDef)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pColumnDef);
  ASSERT(ColumnDef::bIsOfType(pColumnDef));

  QueryNode* pColumnsNode = _pQueryNode(GraphNav::pFindProperty(C_COLUMNS_PROP, pNode));
  ASSERT(pColumnsNode);
  pColumnsNode->vAppendChild(pColumnDef);
}

/*----------------------------------------------------------------------------*/
/* Return the first column definition                                         */
/*----------------------------------------------------------------------------*/
QueryNode* CteDef::pFirstColumnDef(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnsNode = _pQueryNode(GraphNav::pFindProperty(C_COLUMNS_PROP, pNode));
  ASSERT(pColumnsNode);

  QueryNode* pFirstChild = _pQueryNode(pColumnsNode->pFirstChild());
  ASSERT(pFirstChild);

  return pFirstChild;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
