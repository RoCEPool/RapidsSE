/*============================================================================*/
/* Card_PtndJoin.cpp -- Generate                                              */
/*============================================================================*/

#include "Card_PtndJoin.hpp"
#include "Card_SubSet.hpp"
#include "Card_PredCmp.hpp"
#include "Card_Table.hpp"
#include "Card_TablePtr.hpp"
#include "Card_TableList.hpp"
#include "StreamDef.hpp"

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
bool Card_PtndJoin::bMayFollowAfter(Card_SubSet* pPtnJoins_sofar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nPtnJoins_sofar = pPtnJoins_sofar->nChildCards();
  if (! nPtnJoins_sofar)
    return true;

  for (i8 i_PtnJoin_sofar = 0; i_PtnJoin_sofar < nPtnJoins_sofar; i_PtnJoin_sofar++) {
    Card_PtndJoin* p_ptndJoin_sofar =
        _pCard_PtndJoin(pPtnJoins_sofar->pGetChild(i_PtnJoin_sofar));

    i8 nPredCmps_sofar = p_ptndJoin_sofar->nChildCards();
    for (i8 i_PredCmp_sofar = 0; i_PredCmp_sofar < nPredCmps_sofar; i_PredCmp_sofar++) {
      Card_PredCmp* p_predCmp_sofar =
          _pCard_PredCmp(p_ptndJoin_sofar->pGetChild(i_PredCmp_sofar));

      i8 nPredCmps_this = this->nChildCards();
      for (i8 i_PredCmp_this = 0; i_PredCmp_this < nPredCmps_this; i_PredCmp_this++) {
        Card_PredCmp *p_predCmp_this =
            _pCard_PredCmp(this->pGetChild(i_PredCmp_this));

        if (p_predCmp_this->bOverlapsWith(p_predCmp_sofar))
          return true;
      }

    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_PtndJoin::vJoinEst(Card_TableList* p_tableList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nJoinedBuff = 0;

  i8 nTables = p_tableList->nChildCards();
  for (i8 i_Table = 0; i_Table < nTables; i_Table++) {
    Card_Table *p_table =
        _pCard_Table(p_tableList->pGetChild(i_Table));

    if (p_table->bHasProperty(C_PPT_TableUsed))
      continue;

    i8 nPredCmps = this->nChildCards();
    for (i8 i_PredCmp = 0; i_PredCmp < nPredCmps; i_PredCmp++) {
      Card_PredCmp *p_predCmp =
          _pCard_PredCmp(this->pGetChild(i_PredCmp));

      if (p_predCmp->bUsesTable(p_table->cTableSyn())) {
        nJoinedBuff += p_table->pGet_tablePtr()->pTable()->pStreamDef()->jTableSize();
        p_table->vAddProperty_nameOnly(C_PPT_TableUsed);
      }
    }

  }

  vAddProperty_int(C_PPT_JoinedBuff,nJoinedBuff);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
