/*============================================================================*/
/* Card_LoadPermSeq.cpp -- JLoad of join Permutation Sequence                 */
/*============================================================================*/

#include "Card_LoadPermSeq.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_LoadNetwork.hpp"
#include "Card_LoadUnicast.hpp"
#include "Card_Table.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_LoadPermSeq, CK_LOADPERMSEQ)
__aNewFunction(Card_LoadPermSeq, CN_CARD)
__vDeleteFunction(Card_LoadPermSeq, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_LoadPermSeq::Card_LoadPermSeq(void)
: Card(CK_LOADPERMSEQ)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_LoadPermSeq::~Card_LoadPermSeq(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this permutation sequence load less than given arg?                     */
/*----------------------------------------------------------------------------*/
bool Card_LoadPermSeq::bIsLessLoad(Card_LoadPermSeq* p_loadPermSeq_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_LoadNetwork* p_loadNetwork_this = pSub_loadNetwork();
  Card_LoadNetwork* p_loadNetwork_arg  = p_loadPermSeq_arg->pSub_loadNetwork();

  if (p_loadNetwork_this->iValue() < p_loadNetwork_arg->iValue())
    return true;

  Card_LoadUnicast* p_loadUnicast_this = pSub_loadUnicast();
  Card_LoadUnicast* p_loadUnicast_arg  = p_loadPermSeq_arg->pSub_loadUnicast();

  if (p_loadUnicast_this->iValue() < p_loadUnicast_arg->iValue())
    return true;

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
