/*============================================================================*/
/* Card_Av_FilterProject.cpp -- Filter Project                                */
/*============================================================================*/

#include "Card_Av_FilterProject.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Av_FilterProject, CK_AV_FILTERPROJECT)
__aNewFunction(Card_Av_FilterProject, CN_CARD)
__vDeleteFunction(Card_Av_FilterProject, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Av_FilterProject::Card_Av_FilterProject(void)
    : Card(CK_AV_FILTERPROJECT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Av_FilterProject::~Card_Av_FilterProject(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
