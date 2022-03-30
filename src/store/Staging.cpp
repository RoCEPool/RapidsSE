/*============================================================================*/
/* StageSet.cpp -- List of StageSet                                               */
/*============================================================================*/

#include "BASE.h"
#include "Staging.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Staging, CN_STAGING)
__aNewFunction(Staging, CN_STAGING)
__vDeleteFunction(Staging, CN_STAGING)
__aFooterFunction(Staging)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Staging::Staging(PipeWork* pPipeWork, PipeSpec* pPipeSpec)
: ListNL_t<Stage>(CN_STAGING)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGING, _FOOTER_);

  m_pPipeWork = pPipeWork;
  m_pPipeSpec = pPipeSpec;

  _checkInitialised_(CN_STAGING);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_STAGING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Staging::~Staging(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGING, _FOOTER_);

  while (Stage* pStage = pPop())
    delete pStage;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get pipework                                                               */
/*----------------------------------------------------------------------------*/
PipeWork* Staging::pPipeWork(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGING,_FOOTER_);
  
  return m_pPipeWork;
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
