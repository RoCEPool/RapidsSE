/*============================================================================*/
/* RegSet.hpp -- Task Register Set                                            */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,RegSet,CN_REGSET)
__aNewFunction(RegSet,CN_REGSET)
__vDeleteFunction(RegSet,CN_REGSET)
__aFooterFunction(RegSet)

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
/* content dump                                                               */
/*----------------------------------------------------------------------------*/
Calculator* RegSet::pCalculator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  return & m_oCalculator;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
