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
/* estimate join volume (as part of cost estimate of next join)               */
/*----------------------------------------------------------------------------*/
i8 Card_LoadJoined::nEstJoinVolume(i8 nNodes,
                                   eXK eNextCast,
                                   i8 nBuff_arriving,
                                   Card_TableList* p_tableList,
                                   char cTblSyn_r,
                                   i8 iSize_r)

{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //i8 iSize_r = p_tableList->pGetTableSize(cTblSyn_r)->iValue();

  //i8 iLoadJoined = (i8)(1.0 * (nBuff_arriving * iSize_r));

  i8 iLoadJoined = C_UNKNOWN;
  if (nBuff_arriving > iSize_r)
    iLoadJoined = ((d8)nBuff_arriving / (d8)iSize_r) * (nBuff_arriving + iSize_r);
  else
    iLoadJoined = ((d8)iSize_r / (d8)nBuff_arriving) * (nBuff_arriving + iSize_r);

  vValue((i8)iLoadJoined);

  return iLoadJoined;
}


//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
