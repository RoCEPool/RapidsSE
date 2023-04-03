/*============================================================================*/
/* Card_LoadJoined.cpp -- Load of the actual Join                              */
/*============================================================================*/

#include "Card_LoadJoined.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_LoadNetwork.hpp"
#include "Card_LoadUnicast.hpp"
#include "Card_Size.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_LoadJoined, CK_LOADJOINED)
__aNewFunction(Card_LoadJoined, CN_CARD)
__vDeleteFunction(Card_LoadJoined, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_LoadJoined::Card_LoadJoined(void)
: Card(CK_LOADJOINED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_LoadJoined::~Card_LoadJoined(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* estimate number of buffers produced by join                                */
/*----------------------------------------------------------------------------*/
i8 Card_LoadJoined::nEstJoinVol(i8 nBuff_arriving,
                                Card_TableList* p_tableList,
                                char cTblSyn_r,
                                i8 iSize_r)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nNodes = AM::PTNMAP::nKnownNodes();

  //i8 iSize_r = p_tableList->pGetTableSize(cTblSyn_r)->iValue();

  i8 nBuff_produced = C_UNKNOWN;

  if (nBuff_arriving > iSize_r) {
    d8 dExpansionCoeff = (d8) nBuff_arriving / (d8) iSize_r;
    nBuff_produced = (i8) (dExpansionCoeff * (d8) (nBuff_arriving + iSize_r));
  }
  else {
    d8 dExpansionCoeff = (d8) iSize_r / (d8) nBuff_arriving;
    nBuff_produced = (i8) (dExpansionCoeff * (d8) (nBuff_arriving + iSize_r));
  }

  vValue(nBuff_produced);

  return nBuff_produced;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
