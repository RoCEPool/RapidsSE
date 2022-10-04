/*============================================================================*/
/* Sender.cpp --                                                           */
/*============================================================================*/

#include "Sender.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Sender, CN_SENDER)
__aNewFunction(Sender, CN_SENDER)
__vDeleteFunction(Sender, CN_SENDER)
__aFooterFunction(Sender)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Sender::Sender(Bench* pBench)
: TransformerTool(CN_SENDER, pBench)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SENDER, _FOOTER_);

  _checkInitialised_(CN_SENDER);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_SENDER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Sender::~Sender(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SENDER, _FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete sends                                                               */
/*----------------------------------------------------------------------------*/
void Sender::vDelete_Sends(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SENDER, _FOOTER_);

  i8 nMainCards = m_pBoard->nMainCards();
  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
    Card* pMainCard = m_pBoard->pGetMain(i_Main);

    if (CK_SEND != pMainCard->eCard())
      continue;

    //Card* pNextCard = m_pBoard->pGetMain(i_Main + 1);
    //if (! pNextCard)
    //  break;

    //if (CK_STAGE != pNextCard->eCard()) {
    m_pBoard->vRemoveMainCard(pMainCard);
    nMainCards = m_pBoard->nMainCards();
    i_Main--;
    delete pMainCard;
    //}

  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
