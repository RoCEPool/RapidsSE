/*============================================================================*/
/* SkipNode.cpp -- Node in Skip List                                          */
/*============================================================================*/

#include "Structures.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SkipNode* Structures::_pSkipNode(void* aAddr)
{
  SkipNode* pSkipNode = reinterpret_cast<SkipNode*>(aAddr);
  
  RASSERT_TYPE(pSkipNode,CN_SKIPNODE);
  
  return pSkipNode;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* SkipNode::operator new(size_t, i8 iLevel, SlabAllocator* pSlabAllocator)
{
  int nBytes = sizeof(SkipNode) + ((int)iLevel * sizeof(SkipNode*));
  //i8 nMaxBytes = iMaxSize();
  //vTrace("SkipNode iMaxSize %lld\n"_Stz,nMaxBytes);
    
  SkipNode* pSkipNode;
  
  if (pSlabAllocator) {
    pSkipNode = (SkipNode*)pSlabAllocator->ayAllotBytes(nBytes);
  }
  else {
    pSkipNode = (SkipNode*)HeapMem::pAllocate(CN_SKIPNODE);
  }
  
  pSkipNode->m_nRight = iLevel;
  
  return pSkipNode;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SkipNode::operator delete(void* pSkipNodeAddress)
{
  SkipNode* pSkipNode = reinterpret_cast<SkipNode*>(pSkipNodeAddress);
  
  HeapMem::vFree(pSkipNode);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SkipNode::SkipNode(void* pObject)
: Object(CN_SKIPNODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE);
  
  m_pObject = pObject;
  //m_nRight
  ASSERT(m_nRight >= 0 && m_nRight <= C_SKIPLIST_LEVELS);
  
  for (i8 iForward = 0; iForward < m_nRight; iForward++)
    m_pRight[iForward] = nullptr;
  
  _setFooter_();
  
  _checkInitialised_(CN_SKIPNODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SkipNode::SkipNode()
: Object(CN_SKIPNODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE,_FOOTER_);
 
  _checkInitialised_(CN_SKIPNODE);
  
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
Footer* SkipNode::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE);
  
  i8 nSizeBytes = sizeof(SkipNode) + (sizeof(SkipNode*) * (m_nRight - 1));
  
  Footer* aFooter = aAdd<Footer*>(this,nSizeBytes);
  
  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
