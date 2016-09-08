/*============================================================================*/
/* NodeCursor.cpp -- Cursor over Nodes in Grid                                */
/*============================================================================*/

#include "GridModel.h"

extern Heap*   gpSmallHeap;

extern Grid*   gpGrid;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* NodeCursor::operator new(size_t iSize)
{
  Object* pObject = gpSmallHeap->pAllocObject(CN_NODECURSOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* NodeCursor::operator new(size_t iSize, NodeCursor* pNodeCursor)
{
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
  
  Memory::vCheckInitialised(this,sizeof(NodeCursor));
  
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