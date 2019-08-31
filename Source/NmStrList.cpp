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
, m_okLock(LK_NMSTR_LIST,C_NO_REQTUPLE,this,C_NONFOCAL)
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add name-string                                                            */
/*----------------------------------------------------------------------------*/
void NmStrList::vAdd(Stz* szName, Stz* szString)
{
  NmStr* pNmStr = new NmStr(szName,szString);
  
  vAppend(pNmStr);
}

/*----------------------------------------------------------------------------*/
/* add or update name-string                                                  */
/*----------------------------------------------------------------------------*/
void NmStrList::vAddUpdate(Stz* szName, Stz* szString)
{
  NmStr* pNmStr = pFind(szName);
  
  if (pNmStr)
    vUpdate(pNmStr,szString);
  else
    vAdd(szName,szString);
}

/*----------------------------------------------------------------------------*/
/* delete all name-string                                                     */
/*----------------------------------------------------------------------------*/
void NmStrList::vDeleteAll(void)
{
  while (NmStr* pNmStr = pPop())
    delete pNmStr;
}

/*----------------------------------------------------------------------------*/
/* add or update name-string                                                  */
/*----------------------------------------------------------------------------*/
NmStr* NmStrList::pFind(Stz* szName)
{
  for (NmStr* pNmStr = pHead(); pNmStr; pNmStr = pNmStr->pNextNode()) {
    
//    vTrace("  %s = %s\n"_Stz,
//           pNmStr->m_sName.aStz(),
//           pNmStr->m_sValue.aStz());
    
    if (pNmStr->m_sName.bSame(szName))
      return pNmStr;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* add or update name-string                                                  */
/*----------------------------------------------------------------------------*/
void NmStrList::vUpdate(NmStr* pNmStr, Stz* szString)
{
  pNmStr->m_sValue = szString;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(NmStrList)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
