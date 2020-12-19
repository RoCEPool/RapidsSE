/*============================================================================*/
/* PlanNode.cpp - A generic node in the plan graph                            */
/*============================================================================*/

#include "parser/Parser.h"
#include "PlanNode.hpp"

const Stz* PlanNode::C_ROOT_NAME        = "__root__"_Stz;

// Paths of nodes
const Stz* PlanNode::C_ROOT_PATH        = "/__root__"_Stz;
const Stz* PlanNode::C_PRIMITIVES_PATH  = "/__root__/primitives"_Stz;
const Stz* PlanNode::C_DEFINITIONS_PATH = "/__root__/definitions"_Stz;
const Stz* PlanNode::C_PLAN_PATH        = "/__root__/plan"_Stz;
const Stz* PlanNode::C_METADATA_PATH    = "/__root__/metadata"_Stz;
const Stz* PlanNode::C_STATISTICS_PATH  = "/__root__/statistics"_Stz;

const Stz* PlanNode::C_PROPERTY_SEPARATOR = ":"_Stz;
const Stz* PlanNode::C_CHILD_SEPARATOR    = "/"_Stz;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,PlanNode,CN_PLANNODE)
__aFooterFunction(PlanNode)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PlanNode::PlanNode(eCN eClassNo, Stz* szName, Stz* szType, Value xValue,
                   PlanNodeList* pProperties, PlanNodeList* pChildren)
: Object(eClassNo),
  m_sName(szName),
  m_sType(szType),
  m_xValue{xValue},
  m_pProperties(pProperties),
  m_pChildren(pChildren)
{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNODE,_FOOTER_);
//
//  _checkInitialised_(CN_PLANNODE);
//
//  _postcondition_(_ADDRESS_,_MAGIC_,CN_PLANNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PlanNode::~PlanNode(void)
{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNODE,_FOOTER_);

  if (m_xValue.bMustFree && m_xValue.m_pText) {
    delete m_xValue.m_pText;
  }

  if (m_pProperties != nullptr) delete m_pProperties;
  if (m_pChildren   != nullptr) delete m_pChildren;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Get the name of this PlanNode                                              */
/*----------------------------------------------------------------------------*/
Stz* PlanNode::getName()
{
  return m_sName.szStz();
}

/*----------------------------------------------------------------------------*/
/* Get the hierarchical path of type of PlanNode this instance represents     */
/*----------------------------------------------------------------------------*/
Stz* PlanNode::getType()
{
  return m_sType.szStz();
}

/*----------------------------------------------------------------------------*/
/* Get the generic Value object in this PlanNode                              */
/*----------------------------------------------------------------------------*/
Parser::Value PlanNode::getValue()
{
  return m_xValue;
}

/*----------------------------------------------------------------------------*/
/* Get the boolean value out of the Value object in this PlanNode             */
/*----------------------------------------------------------------------------*/
b1 PlanNode::getBooleanValue()
{
  return m_xValue.m_bBoolean;
}

/*----------------------------------------------------------------------------*/
/* Get the integer value out of the Value object in this PlanNode             */
/*----------------------------------------------------------------------------*/
i8 PlanNode::getIntValue()
{
  return m_xValue.m_iInt;
}

/*----------------------------------------------------------------------------*/
/* Get the unsigned integer value out of the Value object in this PlanNode    */
/*----------------------------------------------------------------------------*/
u8 PlanNode::getUnsignedIntValue()
{
  return m_xValue.m_uInt;
}

/*----------------------------------------------------------------------------*/
/* Get the floating point value out of the Value object in this PlanNode      */
/*----------------------------------------------------------------------------*/
d8 PlanNode::getFloatValue()
{
  return m_xValue.m_dFloat;
}

/*----------------------------------------------------------------------------*/
/* Get the string value out of the Value object in this PlanNode. May be null */
/*----------------------------------------------------------------------------*/
Stz* PlanNode::getStringValue()
{
  if (m_xValue.m_pText == nullptr) {
    return nullptr;
  }

  return m_xValue.m_pText->szGet();
}

/*----------------------------------------------------------------------------*/
/* Returns true if there are any properties of this node                      */
/*----------------------------------------------------------------------------*/
b1 PlanNode::bHasProperties()
{
  return (m_pProperties && m_pProperties->pHead());
}

/*----------------------------------------------------------------------------*/
/* Returns true if there are any children of this node                        */
/*----------------------------------------------------------------------------*/
b1 PlanNode::bHasChildren()
{
  return (m_pChildren && m_pChildren->pHead());
}

/*----------------------------------------------------------------------------*/
/* Returns the first property of this node. May be null.                      */
/*----------------------------------------------------------------------------*/
PlanNode* PlanNode::pFirstProperty()
{
  if (!m_pProperties) {
    return nullptr;
  }

  return m_pProperties->pHead();
}

/*----------------------------------------------------------------------------*/
/* Returns the first child of this node. May be null.                         */
/*----------------------------------------------------------------------------*/
PlanNode* PlanNode::pFirstChild()
{
  if (!m_pChildren) {
    return nullptr;
  }

  return m_pChildren->pHead();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

