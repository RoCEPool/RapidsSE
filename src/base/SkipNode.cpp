/*============================================================================*/
/* SkipNode.cpp -- Node in Skip List                                          */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SkipNode* BASE::_pSkipNode(void* aAddr)
{
  SkipNode* pSkipNode = reinterpret_cast<SkipNode*>(aAddr);
  
  RASSERT_TYPE(pSkipNode,CN_SKIPNODE);
  
  return pSkipNode;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* SkipNode::operator new(size_t, i8 iHeight, SlabAllocator* pSlabAllocator)
{
  int nBytes = sizeof(SkipNode) + ((int)iHeight * sizeof(SkipNode*));
  //i8 nMaxBytes = iMaxSize();
  //vTrace("SkipNode iMaxSize %lld\n"_Stz,nMaxBytes);
    
  SkipNode* pSkipNode;
  
  if (pSlabAllocator) {
    pSkipNode = (SkipNode*)pSlabAllocator->ayAllotBytes(nBytes);
  }
  else {
    pSkipNode = (SkipNode*)Heaps::pAllocate(CN_SKIPNODE);
  }
  
  pSkipNode->m_iHeight = iHeight;
  
  return pSkipNode;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SkipNode::operator delete(void* pSkipNodeAddress)
{
  SkipNode* pSkipNode = reinterpret_cast<SkipNode*>(pSkipNodeAddress);
  
  Heaps::vFree(pSkipNode);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
//SkipNode::SkipNode(void)
//: Object(CN_SKIPNODE)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE,_FOOTER_);
//
//  _checkInitialised_(CN_SKIPNODE);
//
//  _postcondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE,_FOOTER_);
//}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SkipNode::SkipNode(void* pObject)
: Object(CN_SKIPNODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE);
  
  m_pObject = pObject;
  //m_nRight
  ASSERT(m_iHeight >= 0 && m_iHeight <= C_SKIPLIST_LEVELS);
  
  for (i8 i_Level = 0; i_Level < m_iHeight; i_Level++)
    m_pRight[i_Level] = nullptr;
  
  for (i8 i_Level = 0; i_Level < C_SKIPLIST_LEVELS; i_Level++)
    m_caFlag[i_Level] = 0;
  
  _setFooter_();
  
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate maximum size of a skip node                                      */
/*----------------------------------------------------------------------------*/
void SkipNode::vResetFlags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPNODE);

  for (i8 i_Level = 0; i_Level < C_SKIPLIST_LEVELS; i_Level++)
    m_caFlag[i_Level] = 0;
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
  
  i8 nSizeBytes = sizeof(SkipNode) + (sizeof(SkipNode*) * (m_iHeight - 1));
  
  Footer* aFooter = aAdd<Footer*>(this,nSizeBytes);
  
  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
