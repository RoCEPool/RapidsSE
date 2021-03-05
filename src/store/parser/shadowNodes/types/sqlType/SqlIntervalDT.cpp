/*============================================================================*/
/* SqlIntervalDY.hpp - an SQL Day-Time interval data type used in the plan    */
/*============================================================================*/

#include "BASE.h"
#include "SqlIntervalDT.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLINTERVAL_DEFN = "/__root__/definitions/types/sqlType/intervalDT"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* SqlIntervalDT::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(C_SQLINTERVAL_DEFN), sName);
//  Value xTypeValue((i8) CT_INTERVAL_DT);   // TODO: enable when eCT has been updated
  RASSERT(0);                       // TODO: temporary
  Value xTypeValue((i8) -1);
  PlanNode* pNode = new QueryNode(sName, C_SQLINTERVAL_DEFN, static_cast<Value&&>(xTypeValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlIntervalDT::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("INTERVAL DAY TO SECOND"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlIntervalDT::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLINTERVAL_DEFN);
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
