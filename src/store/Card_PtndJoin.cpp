/*============================================================================*/
/* Card_PtndJoin.cpp -- Generate                                              */
/*============================================================================*/

#include "Card_PtndJoin.hpp"
#include "Card_SubSet.hpp"
#include "Card_PredCmp.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_PtndJoin,CK_PTNDJOIN)
__aNewFunction(Card_PtndJoin,CN_CARD)
__vDeleteFunction(Card_PtndJoin,CN_CARD)

__bIsCard(STORE,Card_PtndJoin,CK_PTNDJOIN)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_PtndJoin::Card_PtndJoin(void)
: Card(CK_PTNDJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_PtndJoin::~Card_PtndJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_PtndJoin::bMayFollowAfter(Card_SubSet* pPriorPtnJoins)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nPriorPtnJoins = pPriorPtnJoins->nChildCards();
  if (! nPriorPtnJoins)
    return true;

  for (i8 i_PriorPtnJoin = 0; i_PriorPtnJoin < nPriorPtnJoins; i_PriorPtnJoin++) {
    Card_PtndJoin* p_ptndJoin =
        _pCard_PtndJoin(pPriorPtnJoins->pGetChild(i_PriorPtnJoin));

    i8 nPredCmps = p_ptndJoin->nChildCards();
    for (i8 i_PredCmp = 0; i_PredCmp < nPredCmps; i_PredCmp++) {
      Card_PredCmp* p_predCmp =
          _pCard_PredCmp(p_ptndJoin->pGetChild(i_PredCmp));

      p_predCmp->vDump();
    }
  }

  return true;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
