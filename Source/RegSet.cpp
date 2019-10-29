/*============================================================================*/
/* RegSet.hpp -- Task Register Set                                            */
/*============================================================================*/

#include "Registers.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RegSet* Registers::_pRegSet(void* aAddr)
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
  
  m_i_NextRnd   = AM::bDeveloper() ? 0 : C_UNKNOWN;
  m_pSendNode   = nullptr;
  
  _checkInitialised_(CN_REGSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RegSet::~RegSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);

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
         m_pSendNode ? m_pSendNode->szNodeName() : ""_Stz,
         
         C_ENDARGS
         );

}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(RegSet)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
