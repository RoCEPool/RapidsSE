/*============================================================================*/
/* RegSet.hpp -- Task Register Set                                            */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RegSet* Tasks::_pRegSet(void* aAddr)
{
  RegSet* pRegSet = reinterpret_cast<RegSet*>(aAddr);
  
  RASSERT_TYPE(pRegSet,CN_REGSET);
  
  return pRegSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RegSet::operator new(size_t)
{
  RegSet* pRegSet = (RegSet*)(HeapMem::pAllocate(CN_REGSET));
  
  return pRegSet;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RegSet::operator delete(void* aAddress)
{
  RegSet* pRegSet = reinterpret_cast<RegSet*>(aAddress);
  
  HeapMem::vFree(pRegSet);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RegSet::RegSet(Task* pTask)
: Object(CN_REGSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  m_pTask = pTask;
  
  m_eDdlLocked = LH_HAVE_NO_LOCK;
  m_i_NextRnd  = AM::bDeveloper() ? 0 : C_UNKNOWN;
  m_pSendNode  = nullptr;
  
  m_xpHandler  = nullptr;
  m_bCallAgain = false;
  m_iDebugFlag = 0;

  m_xpNow = new Now;
  
  m_iCounter0 = 0;
  m_iCounter1 = 0;
  m_iCounter2 = 0;
  
  m_aLowerEntry = nullptr;
  m_aUpperEntry = nullptr;
  m_pEntBuffer  = nullptr;

  _checkInitialised_(CN_REGSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RegSet::~RegSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);

  RASSERT(LH_HAVE_NO_LOCK == m_eDdlLocked);
  
  if (! m_xpAggTimer.bIsNull())
    delete *m_xpAggTimer;
  
  if (! m_xpNow.bIsNull())
    delete *m_xpNow;
  
  if (! m_xpTimer.bIsNull())
    delete *m_xpTimer;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* content dump                                                               */
/*----------------------------------------------------------------------------*/
void RegSet::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("RegSet\n"_Stz);
  vTrace("------\n"_Stz);

  vTrace("%p:\n"
         "m_i_NextRnd=%lld\n"
         "m_pSendNode=%p %s\n"_Stz,

         this,
         
         m_i_NextRnd,
         
         m_pSendNode,
         m_pSendNode ? m_pSendNode->szNodeNm() : ""_Stz,
         
         C_VARGS_END
         );

}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(RegSet)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
