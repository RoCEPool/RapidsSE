/*============================================================================*/
/* SkipNode.cpp -- Node in Skip List                                          */
/*============================================================================*/

#include "Structures.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SkipNode* Structures::_pSkipNode(void* pAddr)
{
  SkipNode* pSkipNode = reinterpret_cast<SkipNode*>(pAddr);
  
  return pSkipNode;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* SkipNode::operator new(size_t iSize, i4 iLevel)
{
  RASSERT(iSize);
  
//  int nBytes = sizeof(SkipNode) + ((int)iLevel * sizeof(SkipNode*));
//  void* pNode = static_cast<void*>(malloc(nBytes));

  i8 nMaxBytes = iMaxSize();
  vTrace("SkipNode iMaxSize %lld\n",nMaxBytes);
  
  SkipNode* pSkipNode = (SkipNode*)gpSmallHeap->pAllocObject(CN_SKIPNODE,nMaxBytes);
  
  pSkipNode->m_nForward = iLevel;
  
  return pSkipNode;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SkipNode::operator delete(void* pSkipNodeAddress)
{
  SkipNode* pSkipNode = reinterpret_cast<SkipNode*>(pSkipNodeAddress);
  
  gpSmallHeap->vFreeObject(pSkipNode);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SkipNode::SkipNode(void* pObject)
: Object(CN_SKIPNODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE);
  
  m_pObject = pObject;
  //m_nForward
  for (i8 iForward = 0; iForward < m_nForward; iForward++)
    m_pForward[iForward] = nullptr;
  
  _setFooter_();
  
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate maximum size of a skip node                                      */
/*----------------------------------------------------------------------------*/
i8 SkipNode::iMaxSize(void)
{
  i8 nMaxBytes = sizeof(SkipNode)
                   + (sizeof(SkipNode*) * (C_SKIPLIST_LEVELS - 1))
                   + sizeof(Footer);
  
  return nMaxBytes;
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
  
  i8 nSizeBytes = sizeof(SkipNode) + (sizeof(SkipNode*) * (m_nForward - 1));
  
  Footer* pFooter = static_cast<Footer*>(pvAdd(this,nSizeBytes));
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/