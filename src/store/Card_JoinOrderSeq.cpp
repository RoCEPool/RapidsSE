/*============================================================================*/
/* Card_JoinOrderSeq.cpp -- Table List                                        */
/*============================================================================*/

#include "Card_JoinOrderSeq.hpp"

#include "Card_CalcBool.hpp"
#include "Card_ColumnRef.hpp"
#include "Card_PredCmp.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_JoinOrderSeq,CK_JOINORDERSEQ)
__aNewFunction(Card_JoinOrderSeq,CN_CARD)
__vDeleteFunction(Card_JoinOrderSeq,CN_CARD)

__bIsCard(STORE,Card_JoinOrderSeq,CK_JOINORDERSEQ)
__pGetSubFunction_Card(STORE,JoinOrderSeq,CK_JOINORDERSEQ)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinOrderSeq::Card_JoinOrderSeq(void)
: Card(CK_JOINORDERSEQ)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinOrderSeq::~Card_JoinOrderSeq(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
