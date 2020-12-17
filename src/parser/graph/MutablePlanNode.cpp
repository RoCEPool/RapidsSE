/*============================================================================*/
/* MutablePlanNode.cpp - An node of the plan graph that can be modified       */
/*============================================================================*/

#include "../Parser.h"
#include "MutablePlanNode.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,MutablePlanNode,CN_MUTABLEPLANNODE)
__aFooterFunction(MutablePlanNode)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MutablePlanNode::MutablePlanNode(eCN eClassNo, Stz* szName, Stz* szType, Value xValue,
                                 PlanNodeList* pProperties, PlanNodeList* pChildren)
    : PlanNode(eClassNo, szName, szType, xValue, pProperties, pChildren)
{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTABLEPLANNODE,_FOOTER_);
//
//  _checkInitialised_(CN_MUTABLEPLANNODE);
//
//  _postcondition_(_ADDRESS_,_MAGIC_,CN_MUTABLEPLANNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MutablePlanNode::~MutablePlanNode(void)
{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_MUTABLEPLANNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Sets the name of this plan node                                            */
/*----------------------------------------------------------------------------*/
void MutablePlanNode::vSetName(Stz* szName)
{
  m_sName.vCopy(szName);
}


/*----------------------------------------------------------------------------*/
/* Sets the type of this node                                                 */
/*----------------------------------------------------------------------------*/
void MutablePlanNode::vSetType(Stz* szType)
{
  // TODO: Validate that it is a valid type
  m_sType.vCopy(szType);
}

/*----------------------------------------------------------------------------*/
/* Sets the value component of this node                                      */
/*----------------------------------------------------------------------------*/
void MutablePlanNode::vSetValue(Value xValue)
{
  m_xValue = xValue;
}

/*----------------------------------------------------------------------------*/
/* Appends a node as a property of this node                                  */
/*----------------------------------------------------------------------------*/
void MutablePlanNode::vAppendProperty(PlanNode* pNode)
{
  if (!m_pProperties) {
    m_pProperties = new PlanNodeList();
  }

  m_pProperties->vAppend(pNode);
}

/*----------------------------------------------------------------------------*/
/* Appends a node as a child of this node                                     */
/*----------------------------------------------------------------------------*/
void MutablePlanNode::vAppendChild(PlanNode* pNode)
{
  if (!m_pChildren) {
    m_pChildren = new PlanNodeList();
  }

  m_pChildren->vAppend(pNode);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

