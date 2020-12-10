/*============================================================================*/
/* PlanNode.cpp - A generic node in the plan graph                            */
/*============================================================================*/

#include "Parser.h"
#include "PlanNode.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,PlanNode,CN_PLANNODE)
__aFooterFunction(PlanNode)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PlanNode::PlanNode(eCN eClassNo)
: Object(eClassNo),
  m_xValue{.m_iInt = 0},
  m_pAttributes(nullptr),
  m_pChildren(nullptr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNODE,_FOOTER_);
  
  _checkInitialised_(CN_PLANNODE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PLANNODE,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PlanNode::~PlanNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNODE,_FOOTER_);
  
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
/* Get the cross reference PlanNode value out of the Value object. May be null*/
/*----------------------------------------------------------------------------*/
PlanNode* PlanNode::getXrefValue()
{
  return m_xValue.m_pXref;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

