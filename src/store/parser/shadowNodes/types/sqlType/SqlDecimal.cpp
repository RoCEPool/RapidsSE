/*============================================================================*/
/* SqlDecimal.hpp - an SQL exact-numeric decimal data type used in the plan   */
/*============================================================================*/

#include "BASE.h"
#include "SqlDecimal.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLDECIMAL_DEFN     = "/__root__/definitions/types/sqlType/decimal"_Stz;

static Stz* C_PRECISION_PROP_NAME = "precision"_Stz;
static Stz* C_PRECISION_DEFN      = "/__root__/definitions/object/valueObject/integerObject"_Stz;
static Stz* C_SCALE_PROP_NAME     = "scale"_Stz;
static Stz* C_SCALE_DEFN          = "/__root__/definitions/object/valueObject/integerObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type with default precision and scale          */
/*----------------------------------------------------------------------------*/
PlanNode* SqlDecimal::pCreate()
{
  return pCreate(-1, -1);
}

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/* Use -1 for precision and scale if not applicable.                          */
/*----------------------------------------------------------------------------*/
PlanNode* SqlDecimal::pCreate(i8 iPrecision, i8 iScale)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_SQLDECIMAL_DEFN, sName);
  QueryNode* pNode = new QueryNode(sName, C_SQLDECIMAL_DEFN);

  // Add in any common properties
  SqlType::vCreateCommon(pNode, TypeId::xIdOf(CT_DECIMAL));

  if (iPrecision != -1) {
    ASSERT(iPrecision > 0);

    Value xValue(iPrecision);
    QueryNode* pPrecisionNode = new QueryNode(C_PRECISION_PROP_NAME, C_PRECISION_DEFN, static_cast<Value&&>(xValue));
    pNode->vAppendProperty(pPrecisionNode);
  }

  if (iScale != -1) {
    ASSERT(iScale > 0);

    Value xValue(iScale);
    QueryNode* pScaleNode = new QueryNode(C_SCALE_PROP_NAME, C_SCALE_DEFN, static_cast<Value&&>(xValue));
    pNode->vAppendProperty(pScaleNode);
  }

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlDecimal::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  sBuf.vConcat("DECIMAL"_Stz);

  PlanNode* pPrecisionNode = GraphNav::pFindProperty(C_PRECISION_PROP_NAME, pNode);
  PlanNode* pScaleNode     = GraphNav::pFindProperty(C_SCALE_PROP_NAME, pNode);

  if (pPrecisionNode) {
    if (pScaleNode) {
      sBuf.vCatSprint("(%lld,%lld)"_Stz, pPrecisionNode->iValue(), pScaleNode->iValue());

    } else {
      sBuf.vCatSprint("(%lld)"_Stz, pPrecisionNode->iValue());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlDecimal::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLDECIMAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SqlDecimal::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SQLDECIMAL_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
