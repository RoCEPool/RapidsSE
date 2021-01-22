/*============================================================================*/
/* PlanNode.cpp - A generic node in the plan graph                            */
/*============================================================================*/

#include "BASE.h"
#include "PlanNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,PlanNode,CN_PLANNODE)
__aFooterFunction(PlanNode)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PlanNode::PlanNode(eCN eClassNo, Stz* szName, Stz* szType, Value&& xValue,
                   PlanNodeList* pProperties, PlanNodeList* pChildren)
: Object(eClassNo),
  m_sName(szName),
  m_sType(szType),
  m_xValue(xValue),
  m_pProperties(pProperties),
  m_pChildren(pChildren)
{


}

/*----------------------------------------------------------------------------*/
/* copy constructor                                                           */
/*----------------------------------------------------------------------------*/
PlanNode::PlanNode(eCN eClassNo, const PlanNode& oNode)
    : Object(eClassNo),
      m_xValue(const_cast<PlanNode &>(oNode).m_xValue)
{
  m_sName.vCopy(const_cast<PlanNode &>(oNode).m_sName.szStz());
  m_sType.vCopy(const_cast<PlanNode &>(oNode).m_sType.szStz());

  if (oNode.m_pProperties) {
    m_pProperties = new PlanNodeList();
    PlanNodeIter* pPropIter = oNode.m_pProperties->pNewIter();

    while (pPropIter->bHasNext()) {
      PlanNode* pItem = pPropIter->pNext();
      m_pProperties->vAppend(pItem);
    }
    delete pPropIter;

  } else {
    m_pProperties = nullptr;
  }

  if (oNode.m_pChildren) {
    m_pChildren = new PlanNodeList();
    PlanNodeIter* pChildIter = oNode.m_pChildren->pNewIter();

    while (pChildIter->bHasNext()) {
      PlanNode* pItem = pChildIter->pNext();
      m_pChildren->vAppend(pItem);
    }
    delete pChildIter;

  } else {
    m_pChildren = nullptr;
  }
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PlanNode::~PlanNode(void)
{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNODE,_FOOTER_);

  if (m_pProperties != nullptr) delete m_pProperties;
  if (m_pChildren   != nullptr) delete m_pChildren;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Get the name of this PlanNode                                              */
/* Returns null if the name is empty.                                         */
/*----------------------------------------------------------------------------*/
Stz* PlanNode::szName()
{
  if (m_sName.nLength() == 0) {
    return nullptr;
  }
  return m_sName.szStz();
}

/*----------------------------------------------------------------------------*/
/* Get the hierarchical path of type of PlanNode this instance represents.    */
/* Returns null if the type is empty.                                         */
/*----------------------------------------------------------------------------*/
Stz* PlanNode::szType()
{
  if (m_sType.nLength() == 0) {
    return nullptr;
  }

  return m_sType.szStz();
}

/*----------------------------------------------------------------------------*/
/* Get the generic Value object in this PlanNode                              */
/*----------------------------------------------------------------------------*/
Parser::Value PlanNode::xValue()
{
  return m_xValue;
}

/*----------------------------------------------------------------------------*/
/* Get the generic Value object in this PlanNode                              */
/*----------------------------------------------------------------------------*/
Parser::Value* PlanNode::pValue()
{
  return &m_xValue;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the value is null. NB: this is distinct from the type      */
/* being VT_UNKNOWN.                                                          */
/*----------------------------------------------------------------------------*/
b1 PlanNode::bIsNullValue()
{
  return m_xValue.m_bIsNull;
}

/*----------------------------------------------------------------------------*/
/* Returns the type of the value stored                                       */
/*----------------------------------------------------------------------------*/
Value::Type PlanNode::eValueType()
{
  return m_xValue.m_eType;
}

/*----------------------------------------------------------------------------*/
/* Get the boolean value out of the Value object in this PlanNode             */
/*----------------------------------------------------------------------------*/
b1 PlanNode::bValue()
{
  ASSERT(m_xValue.m_eType == Value::VT_BOOLEAN);
  return m_xValue.m_bBoolean;
}

/*----------------------------------------------------------------------------*/
/* Get the integer value out of the Value object in this PlanNode             */
/*----------------------------------------------------------------------------*/
i8 PlanNode::iValue()
{
  ASSERT(m_xValue.m_eType == Value::VT_INT);
  return m_xValue.m_iInt;
}

/*----------------------------------------------------------------------------*/
/* Get the unsigned integer value out of the Value object in this PlanNode    */
/*----------------------------------------------------------------------------*/
u8 PlanNode::uValue()
{
  ASSERT(m_xValue.m_eType == Value::VT_UINT);
  return m_xValue.m_uInt;
}

/*----------------------------------------------------------------------------*/
/* Get the floating point value out of the Value object in this PlanNode      */
/*----------------------------------------------------------------------------*/
d8 PlanNode::dValue()
{
  ASSERT(m_xValue.m_eType == Value::VT_FLOAT);
  return m_xValue.m_dFloat;
}

/*----------------------------------------------------------------------------*/
/* Get the string value out of the Value object in this PlanNode. May be null */
/*----------------------------------------------------------------------------*/
Stz* PlanNode::szValue()
{
  ASSERT(m_xValue.m_eType == Value::VT_STRING);

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
  return (nProperties() > 0);
}

/*----------------------------------------------------------------------------*/
/* Returns true if there are any children of this node                        */
/*----------------------------------------------------------------------------*/
b1 PlanNode::bHasChildren()
{
  return (nChildren() > 0);
}

/*----------------------------------------------------------------------------*/
/* Returns the number of properties in this object.                           */
/*----------------------------------------------------------------------------*/
i8 PlanNode::nProperties()
{
  if (!m_pProperties) {
    return 0;
  }

  return m_pProperties->nItems();
}

/*----------------------------------------------------------------------------*/
/* Returns the number of children in this object.                             */
/*----------------------------------------------------------------------------*/
i8 PlanNode::nChildren()
{
  if (!m_pChildren) {
    return 0;
  }

  return m_pChildren->nItems();
}


/*----------------------------------------------------------------------------*/
/* Returns an iterator for the properties of this node. May be null if there  */
/* are none. The iterator must be deleted after use.                          */
/*----------------------------------------------------------------------------*/
PlanNodeIter* PlanNode::pNewPropertyIter()
{
  if (!m_pProperties) {
    return nullptr;
  }

  return m_pProperties->pNewIter();
}

/*----------------------------------------------------------------------------*/
/* Returns an iterator for the children of this node. May be null if there    */
/* are none. The iterator must be deleted after use.                          */
/*----------------------------------------------------------------------------*/
PlanNodeIter* PlanNode::pNewChildrenIter()
{
  if (!m_pChildren) {
    return nullptr;
  }

  return m_pChildren->pNewIter();
}

/*----------------------------------------------------------------------------*/
/* Returns the first PlanNode child, or nullptr if there is none              */
/*----------------------------------------------------------------------------*/
PlanNode* PlanNode::pFirstChild()
{
  if (!m_pChildren || m_pChildren->nItems() == 0) {
    return nullptr;
  }

  return m_pChildren->pHead();
}

/*----------------------------------------------------------------------------*/
/* Returns the last PlanNode child, or nullptr if there is none              */
/*----------------------------------------------------------------------------*/
PlanNode* PlanNode::pLastChild()
{
  if (!m_pChildren || m_pChildren->nItems() == 0) {
    return nullptr;
  }

  return m_pChildren->pTail();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

