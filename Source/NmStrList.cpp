/*============================================================================*/
/* NmStrList.hpp - list of name-strings                                       */
/*============================================================================*/

#include "Transform.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
NmStrList* Transform::_pNmStrList(void* aAddr)
{
  NmStrList* pNmStrList = reinterpret_cast<NmStrList*>(aAddr);
  
  RASSERT_TYPE(pNmStrList,CN_NMSTRLIST);
  
  return pNmStrList;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* NmStrList::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_NMSTRLIST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void NmStrList::operator delete(void* pTaskQueueAddress)
{
  NmStrList* pTaskQueue = reinterpret_cast<NmStrList*>(pTaskQueueAddress);
  
  HeapMem::vFree(pTaskQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
NmStrList::NmStrList(void)
: ListNL_t<NmStr>(CN_NMSTRLIST)
, m_okLock(LK_TASK_QUEUE,C_NO_REQTUPLE,this,C_NONFOCAL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NMSTRLIST,_FOOTER_);
  
  _checkInitialised_(CN_NMSTRLIST);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_NMSTRLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
NmStrList::~NmStrList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NMSTRLIST,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(NmStrList)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
