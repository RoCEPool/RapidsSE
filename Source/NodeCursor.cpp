/*============================================================================*/
/* NodeCursor.cpp -- Cursor over Nodes in Grid                                */
/*============================================================================*/

#include "GridModel.h"

extern Heap*   gpSmallHeap;

extern Grid*   gpGrid;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
NodeCursor* GridModel::_pNodeCursor(void* pAddr)
{
  NodeCursor* pNodeCursor = reinterpret_cast<NodeCursor*>(pAddr);
  
  return pNodeCursor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* NodeCursor::operator new(size_t iSize)
{
  RASSERT(iSize);
  Object* pObject = gpSmallHeap->pAllocObject(CN_NODECURSOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* NodeCursor::operator new(size_t iSize, NodeCursor* pNodeCursor)
{
  RASSERT(iSize);
  return pNodeCursor;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void NodeCursor::operator delete(void* pNodeAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pNodeAddress);
  
  gpSmallHeap->vFreeObject(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
NodeCursor::NodeCursor(List_t<Node>* pList)
: Object(CN_NODECURSOR),
  ListCursor_t<Node>(pList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODECURSOR,_FOOTER_);
  
  _checkInitialised_(CN_NODECURSOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_NODECURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
NodeCursor::~NodeCursor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODECURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump node cursor                                                           */
/*----------------------------------------------------------------------------*/
void NodeCursor::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_NODECURSOR);
  ASSERT(pObject->eClass() == CN_NODECURSOR);
  
  NodeCursor* pNodeCursor = reinterpret_cast<NodeCursor*>(pObject);
  
  vTrace("%sNodeCursor %p:\n", pTab, pNodeCursor);
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void NodeCursor::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODECURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* NodeCursor::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODECURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/