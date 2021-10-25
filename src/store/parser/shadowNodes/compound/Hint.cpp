/*============================================================================*/
/* Hint.hpp - a query hint                                                    */
/*============================================================================*/

#include "BASE.h"
#include "Hint.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "ColumnDefs.hpp"
#include "ColumnDef.hpp"
#include "NodeNamer.hpp"
#include "ValueObject.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN   = "/__root__/definitions/object/compound/hint"_Stz;
static Stz* C_NAME   = "hint"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create Hint nodes.                                      */
/*----------------------------------------------------------------------------*/
QueryNode* Hint::pCreate(Text* pHintKey)
{
  _static_

  ASSERT(pHintKey);
  ASSERT(pHintKey->nLength() > 0);

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  // Create the Hint object
  QueryNode* pHint = new QueryNode(sName, C_DEFN);

  return pHint;
}


/*----------------------------------------------------------------------------*/
/* Appends a description of the table reference into a buffer                 */
/*----------------------------------------------------------------------------*/
void Hint::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pHint = _pQueryNode(pNode);
  Stz* szHintName = szHintKey(pHint);
  sBuf.vConcat(szHintName);

  i8 nChildren = pHint->nChildren();
  if (nChildren == 0) {
    return;
  }

  sBuf.vConcat(" = "_Stz);

  if (nChildren > 1) {
    sBuf.vConcat("("_Stz);
  }

  PlanNodeIter* pIter = pHint->pNewChildrenIter();

  while (pIter->bHasNext()) {

    QueryNode* pHintValue = _pQueryNode(pIter->pNext());
    ASSERT(ValueObject::bIsOfType(pHintValue));

    switch (pHintValue->eValueType()) {
      case Value::VT_STRING:
        sBuf.vCatSprint("%s"_Stz, pHintValue->szValue());
        break;

      case Value::VT_BOOLEAN:
        sBuf.vCatSprint("%s"_Stz, pHintValue->bValue() ? "true"_Stz : "false"_Stz);
        break;

      case Value::VT_INT:
        sBuf.vCatSprint("%lld"_Stz, pHintValue->iValue());
        break;

      case Value::VT_FLOAT:
        sBuf.vCatSprint("%f"_Stz, pHintValue->dValue());
        break;

        // TODO: add decimal support
//    case Value::VT_DECIMAL:
//      break;

      case Value::VT_UNKNOWN:
        break;

      default:
        NEVER_REACH
    }
  }

  delete pIter;

  if (nChildren > 1) {
    sBuf.vConcat(")"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the Hint definition     */
/*----------------------------------------------------------------------------*/
b1 Hint::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Hint::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the name of the Hint                                                   */
/*----------------------------------------------------------------------------*/
Stz* Hint::szHintKey(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_STRING);
  Stz* szKey = pNode->szValue();
  ASSERT(szKey);

  return szKey;
}

/*----------------------------------------------------------------------------*/
/* Adds a value to this hint                                                  */
/*----------------------------------------------------------------------------*/
void Hint::vAddHintValue(QueryNode* pNode, QueryNode* pValueNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pValueNode);
  ASSERT(ValueObject::bIsOfType(pValueNode));

  pNode->vAppendChild(pValueNode);
}

/*----------------------------------------------------------------------------*/
/* get an iterator of PlanNodes that represent ColumnDefs                     */
/*----------------------------------------------------------------------------*/
QueryNode* Hint::pFirstValue(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pFirstValue = _pQueryNode(pNode->pFirstChild());
  ASSERT(!pFirstValue || ValueObject::bIsOfType(pFirstValue));

  return pFirstValue;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
