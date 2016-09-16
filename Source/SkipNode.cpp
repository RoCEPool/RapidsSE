/*============================================================================*/
/*  SkipNode.cpp - a node in the list                                         */
/*============================================================================*/

#include "Structures.h"

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* SkipNode::operator new(size_t iSize, i4 iLevel)
{
  ASSERT(iSize);
  int nBytes = sizeof(SkipNode) + ((int)iLevel * sizeof(SkipNode*));
  
  void* pNode = static_cast<void*>(malloc(nBytes));
  
  ((SkipNode*)pNode)->m_iForward = iLevel;
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SkipNode::SkipNode(void* pObject)
: Object(CN_SKIPNODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE,_FOOTER_);
  
  m_pObject = pObject;
  
  _checkInitialised_(sizeof(SkipNode));

  _postcondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SkipNode::SkipNode()
: Object(CN_SKIPNODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE,_FOOTER_);
 
  _checkInitialised_(sizeof(SkipNode));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SkipNode::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/