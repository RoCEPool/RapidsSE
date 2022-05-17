/*============================================================================*/
/* ColumnOrdinal.hpp - A reference to a column by its ordinal number (1-based)*/
/*============================================================================*/

#include "BASE.h"
#include "ColumnOrdinal.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "NodeNamer.hpp"

#include "ShadowRoot.hpp"
#include "MetadataNode.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_COLUMN_ORDINAL_DEFN = "/__root__/definitions/object/expr/columnOrdinal"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create expression column reference nodes.               */
/*----------------------------------------------------------------------------*/
QueryNode* ColumnOrdinal::pCreate(i8 iOrdinal)
{
  _static_
  ASSERT(iOrdinal > 0);

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_COLUMN_ORDINAL_DEFN, sName);

  Value xValue(iOrdinal);

  QueryNode* pColOrdinal = new QueryNode(sName, C_COLUMN_ORDINAL_DEFN, static_cast<Value&&>(xValue));

  return pColOrdinal;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ColumnOrdinal::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  QueryNode* pColumnOrdinal = _pQueryNode(pNode);

  sBuf.vCatSprint("$%lld"_Stz, iOrdinal(pColumnOrdinal));
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 ColumnOrdinal::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_COLUMN_ORDINAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ColumnOrdinal::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_COLUMN_ORDINAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
i8 ColumnOrdinal::iType(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pNode);
  UNUSED(pRootNode);

  // This should never be called because an column ordinal knows nothing of
  // the expression nodes it is representing. Instead, the symbolic expression
  // should be queried for it's type, scale, precision, etc.
  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns the 1-based reference to a column                                  */
/*----------------------------------------------------------------------------*/
i8 ColumnOrdinal::iOrdinal(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  return pNode->iValue();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
