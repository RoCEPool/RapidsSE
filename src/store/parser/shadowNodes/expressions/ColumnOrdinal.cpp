/*============================================================================*/
/* ColumnOrdinal.hpp - A reference to a column by its ordinal number (1-based)*/
/*============================================================================*/

#include "BASE.h"
#include "ColumnOrdinal.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"

#include "ShadowRoot.hpp"
#include "MetadataNode.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_COLUMN_ORDINAL_DEFN = "/__root__/definitions/object/expr/ColumnOrdinal"_Stz;

static Stz* C_COLUMNORDINAL_NAME_PREFIX = "ExprColOrdinal"_Stz;


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
  ShadowNode::vObjName(C_COLUMNORDINAL_NAME_PREFIX, sName);

  Value xValue(iOrdinal);

  QueryNode* pColOrdinal = new QueryNode(sName, C_COLUMN_ORDINAL_DEFN, static_cast<Value&&>(xValue));

  return pColOrdinal;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ColumnOrdinal::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnOrdinal = static_cast<QueryNode*>(pNode);

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
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT ColumnOrdinal::eType(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

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
