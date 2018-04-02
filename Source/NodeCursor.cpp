/*============================================================================*/
/* NodeCursor.cpp -- Cursor over Nodes in Grid                                */
/*============================================================================*/

#include "GridModel.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
NodeCursor* GridModel::_pNodeCursor(void* aAddr)
{
  NodeCursor* pNodeCursor = reinterpret_cast<NodeCursor*>(aAddr);
  
  RASSERT( !pNodeCursor || CN_NODECURSOR == pNodeCursor->eClass() );
  
  return pNodeCursor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* NodeCursor::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heap::pAllocate(CN_NODECURSOR);
  
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
  
  Heap::vFree(pObject);
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
void NodeCursor::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_NODECURSOR);
  RASSERT(pObject->eClass() == CN_NODECURSOR);
  
  NodeCursor* pNodeCursor = reinterpret_cast<NodeCursor*>(pObject);
  
  vTrace("\n"_Stz);
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
Footer* NodeCursor::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODECURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
