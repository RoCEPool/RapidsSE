/*============================================================================*/
/* Card_Stream.cpp -- Stream                                                  */
/*============================================================================*/

#include "Card_Stream.hpp"
#include "Card_Expr.hpp"
#include "Card_OrderElist.hpp"
#include "Card_StreamElist.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Stream,CK_STREAM)
__aNewFunction(Card_Stream,CN_CARD)
__vDeleteFunction(Card_Stream,CN_CARD)

__bIsCard(STORE,Card_Stream,CK_STREAM)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Stream::Card_Stream(void)
: Card_SUB(CK_STREAM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Stream::~Card_Stream(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply the order of given arg to this stream                                */
/*----------------------------------------------------------------------------*/
void Card_Stream::vApplyOrder(Card_Stream* pCard_Stream_src)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_OrderElist* pCard_OrderElist_src =
    _pCard_OrderElist(pCard_Stream_src->pFindSubCard(CK_ORDERELIST));
  if (! pCard_OrderElist_src)
    return;
  i8 nOrderExprs_src = pCard_OrderElist_src->nItems();

  Card_StreamElist* pCard_StreamElist_dst =
    _pCard_StreamElist(this->pFindSubCard(CK_STREAMELIST));
  if (! pCard_StreamElist_dst)
    return;
  i8 nStreamExprs_dst = pCard_StreamElist_dst->nItems();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Card_OrderElist* pCard_OrderElist_dst = nullptr;

  for (i8 i_Expr = 0; i_Expr < nOrderExprs_src; i_Expr++) {
    if (i_Expr >= nStreamExprs_dst)
      break;
    
    Card_Expr* pCard_Expr_src =
      _pCard_Expr(pCard_OrderElist_src->pGetSubCard(i_Expr));
    Card_Expr* pCard_Expr_tst =
      _pCard_Expr(pCard_StreamElist_dst->pGetSubCard(i_Expr));

    if (! pCard_Expr_tst->bEquivalent(pCard_Expr_src))
      break;

    if (! pCard_OrderElist_dst)
      pCard_OrderElist_dst = new Card_OrderElist;
    
    Card_Expr* pCard_Expr_dst =
      _pCard_Expr(pCard_Expr_src->pDupCardAndSubCards());

    pCard_OrderElist_dst->vAppend(pCard_Expr_dst);
  }
  
  if (pCard_OrderElist_dst)
    this->vAppend(pCard_OrderElist_dst);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
