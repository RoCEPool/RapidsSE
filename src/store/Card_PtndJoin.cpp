/*============================================================================*/
/* Card_PtndJoin.cpp -- Generate                                              */
/*============================================================================*/

#include "Card_PtndJoin.hpp"
#include "Card_SubSet.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Size.hpp"

#include "Table.hpp"
#include "StreamDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_PtndJoin,CK_PTNDJOIN)
__aNewFunction(Card_PtndJoin,CN_CARD)
__vDeleteFunction(Card_PtndJoin,CN_CARD)


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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_PtndJoin::bMayFollowAfter(Card_SubSet* pPriorPtnJoins_sofar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nPtnJoins_sofar = pPriorPtnJoins_sofar->nChildCards();
  if (! nPtnJoins_sofar)
    return true;

  for (i8 i_PtnJoin_sofar = 0; i_PtnJoin_sofar < nPtnJoins_sofar; i_PtnJoin_sofar++) {
    Card_PtndJoin* p_ptndJoin_sofar =
        _pCard_PtndJoin(pPriorPtnJoins_sofar->pGetChild(i_PtnJoin_sofar));

    i8 nPredCmps_sofar = p_ptndJoin_sofar->nChildCards();
    for (i8 i_PredCmp_sofar = 0; i_PredCmp_sofar < nPredCmps_sofar; i_PredCmp_sofar++) {
      Card_CmpPred* p_predCmp_sofar =
          _pCard_CmpPred(p_ptndJoin_sofar->pGetChild(i_PredCmp_sofar));

      i8 nPredCmps_this = nChildCards();
      for (i8 i_PredCmp_this = 0; i_PredCmp_this < nPredCmps_this; i_PredCmp_this++) {
        Card_CmpPred *p_predCmp_this =
            _pCard_CmpPred(pGetChild(i_PredCmp_this));

        if (p_predCmp_this->bOverlapsWith(p_predCmp_sofar))
          return true;
      }

    }
  }

  return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
