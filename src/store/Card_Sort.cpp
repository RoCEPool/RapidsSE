/*============================================================================*/
/* Card_Sort.cpp -- Sort                                                      */
/*============================================================================*/

#include "Card_Sort.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Sort,CK_SORT)
__aNewFunction(Card_Sort,CN_CARD)
__vDeleteFunction(Card_Sort,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Sort::Card_Sort(void)
: Card_OP(CK_SORT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Sort::~Card_Sort(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* column mapping                                                             */
/*----------------------------------------------------------------------------*/
void Card_Sort::vColumn_Mapping(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);


  pCI->G.p_key = pSub_key();
  pCI->G.p_projection = pSub_projection();

  i8 nExpr_key = 0;
  i8 i_Emitted = 1;
  i8 i_Cell_payload = 1;
  i8 i_Cell_key;


  pCI->G_vAppendNewChild(this,CK_MAPPING);
  pCI->G_vAppendNewChild(pCI->G.p_mapping,CK_KEYCELLS);
  pCI->G_vAppendNewChild(pCI->G.p_mapping,CK_PAYLOADCELLS);


  i8 i_Emit = 0;

  i8 i_Cell = 0;
  for (Kid xKey(pCI->G.p_key); xKey.bEach(CK_EXPR); /**/) {
    Card_Expr *p_expr_key = xKey.p_expr();

    i_Cell++;
    pCI->G_vAppendNewChild(pCI->G.p_keyCells,CK_MAP,p_expr_key->szSymbol());
    pCI->G.p_map->pSymbol()->Y_vSet(P_Cell, i_Cell);


    //pCI->G.p_map->pSymbol()->Y_vSet(P_EmittedCol, i_Emit);
  }

  i_Cell = 0;
  for (Kid xProjection(pCI->G.p_projection); xProjection.bEach(CK_EXPR); /**/) {
    Card_Expr *p_expr_projection = xProjection.p_expr();

    if (p_expr_projection->bIsExpressionOfKey()) {
      Symbol *pSymbol_keyCol = p_expr_projection->pFirstKeyCol();

      Card_Map* p_map_keyCell = pCI->G.p_keyCells->pDecodeMap(pSymbol_keyCol);

      i_Emit++;
      p_map_keyCell->pSymbol()->Y_vSet(P_EmittedCol, i_Emit);
      p_map_keyCell->pSymbol()->Y_vSet(P_StmCol, p_expr_projection->szSymbol());

    }
    else {
      i_Cell++;
      i_Emit++;
      pCI->G_vAppendNewChild(pCI->G.p_payloadCells, CK_MAP,p_expr_projection->szSymbol());
      pCI->G.p_map->pSymbol()->Y_vSet(P_Cell, i_Cell);

      pCI->G.p_map->pSymbol()->Y_vSet(P_EmittedCol, i_Emit);
    }
  }






  for (Kid xPrjn(pCI->G.p_projection); xPrjn.bEach(CK_EXPR); /**/) {
    Card_Expr *p_expr_prjn = xPrjn.p_expr();

    if (p_expr_prjn->bIsExpressionOfKey()) {

      //pCI->G_vAppendNewChild(pCI->G.p_keyCells,CK_MAP);

      nExpr_key++;

      Symbol *pSymbol_key = p_expr_prjn->pFirstKeyCol();

      i_Cell_key = pSymbol_key->iExtractOrdinal();

      p_expr_prjn->pSymbol()->Y_vSet(P_EmittedCol, i_Emitted++);
      p_expr_prjn->pSymbol()->Y_vSet(P_Key);
      p_expr_prjn->pSymbol()->Y_vSet(P_Cell, i_Cell_key);
    }
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    else {

      //pCI->G_vAppendNewChild(pCI->G.p_payloadCells,CK_MAP);

      Symbol* pSymbol_stmCol_projection = p_expr_prjn->pFirstStmCol();

      if (pSymbol_stmCol_projection) {
        i_Cell_payload = pSymbol_stmCol_projection->iExtractOrdinal() - nExpr_key;

        p_expr_prjn->pSymbol()->Y_vSet(P_EmittedCol, i_Emitted++);
        p_expr_prjn->pSymbol()->Y_vSet(P_Payload);
        p_expr_prjn->pSymbol()->Y_vSet(P_Cell, i_Cell_payload++);
      }
      else {

        p_expr_prjn->pSymbol()->Y_vSet(P_EmittedCol, i_Emitted++);
        p_expr_prjn->pSymbol()->Y_vSet(P_Payload);
        p_expr_prjn->pSymbol()->Y_vSet(P_Cell, i_Cell_payload++);
      }

    }
  }

}
/*----------------------------------------------------------------------------*/
//void Card_Sort::vColumn_Mapping(CardIter *pCI)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//
//  pCI->G.p_key = pSub_key();
//  pCI->G.p_projection = pSub_projection();
//
//  i8 nExpr_key = 0;
//  i8 i_Emitted = 1;
//  i8 i_Cell_payload = 1;
//  i8 i_Cell_key;
//
//  for (Kid xPrjn(pCI->G.p_projection); xPrjn.bEach(CK_EXPR); /**/) {
//    Card_Expr *p_expr_prjn = xPrjn.p_expr();
//
//    if (p_expr_prjn->bIsExpressionOfKey()) {
//
//      nExpr_key++;
//
//      Symbol *pSymbol_key = p_expr_prjn->pFirstKeyCol();
//
//      i_Cell_key = pSymbol_key->iExtractOrdinal();
//
//      p_expr_prjn->pSymbol()->Y_vSet(P_EmittedCol, i_Emitted++);
//      p_expr_prjn->pSymbol()->Y_vSet(P_Key);
//      p_expr_prjn->pSymbol()->Y_vSet(P_Cell, i_Cell_key);
//    }
//      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//    else {
//
//      Symbol* pSymbol_stmCol_projection = p_expr_prjn->pFirstStmCol();
//
//      if (pSymbol_stmCol_projection) {
//        i_Cell_payload = pSymbol_stmCol_projection->iExtractOrdinal() - nExpr_key;
//
//        p_expr_prjn->pSymbol()->Y_vSet(P_EmittedCol, i_Emitted++);
//        p_expr_prjn->pSymbol()->Y_vSet(P_Payload);
//        p_expr_prjn->pSymbol()->Y_vSet(P_Cell, i_Cell_payload++);
//      }
//      else {
//
//        p_expr_prjn->pSymbol()->Y_vSet(P_EmittedCol, i_Emitted++);
//        p_expr_prjn->pSymbol()->Y_vSet(P_Payload);
//        p_expr_prjn->pSymbol()->Y_vSet(P_Cell, i_Cell_payload++);
//      }
//
//    }
//  }
//
//}

/*----------------------------------------------------------------------------*/
/* test & cut sort                                                            */
/*----------------------------------------------------------------------------*/
void Card_Sort::vCut_sort(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! pCI->StreamOrder.m_pKey_running)
    return;

  pCI->G.p_key = pSub_key();

  if (! pCI->G.p_key->bIsEqual(pCI->StreamOrder.m_pKey_running))
    return;

  pCI->G.pDeck->vDeleteChild(this);
}

/*----------------------------------------------------------------------------*/
/* derive projection key                                                      */
/*----------------------------------------------------------------------------*/
void Card_Sort::vDerive_Projection_Key(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->StreamOrder.m_pKey_running = pSub_key();
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Sort::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_key = pCI->G.p_sort->pSub_key();

  if ((pCI->G.p_projection = pCI->G.p_sort->pSub_projection()))
    pCI->G.p_sort->vDeleteChild(pCI->G.p_projection);

  Card_Projection* p_projection_sort =  pCI->Analysis.m_pProjection_prior->pPropagate(pCI);
  for (Kid xKey_sort(pSub_key()); xKey_sort.bEach(); /**/) {
    Card_Expr* p_expr_key = xKey_sort.p_expr();

    Symbol* pSym_expr_x = p_expr_key->pFirstStmCol();
    Card_Expr* p_expr_prj = p_projection_sort->pDecodeExpr(pSym_expr_x);

    p_expr_prj->pGetTerm(0)->vSymbol(p_expr_key);
  }

  pCI->G.p_sort->vAppend(p_projection_sort);

  pCI->Analysis.m_pProjection_prior = pSub_projection();

  pCI->vRenumberCards();
}
/*----------------------------------------------------------------------------*/
//void Card_Sort::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  pCI->G.p_key = pCI->G.p_sort->pSub_key();
//
//  if ((pCI->G.p_projection = pCI->G.p_sort->pSub_projection())) {
//
//    pCI->G.p_sort->vDeleteChild(pCI->G.p_projection);
//
//    //pCI->G.p_sort->vAppend(pCI->Analysis.m_pProjection_prior->pPropagate());
//
//
//    pCI->G_vAppendNewChild(pCI->G.p_sort,CK_PROJECTION);
//    for (Kid xPrior(pCI->Analysis.m_pProjection_prior); xPrior.bEach(); /**/) {
//      Card_Expr* p_expr_prior = xPrior.p_expr();
//
//      if (p_expr_prior->bIsExpressionOfKey()) {
//        Symbol* pSym = p_expr_prior->pGetTerm(0)->pSymbol();
//        Card_Expr* p_expr_key = pCI->G.p_key->pFind_expr_Of_term(pSym);
//        if (p_expr_key) {
//
//          // todo: create simple expr from key expr
//
//          pCI->G.p_projection->vAppend( nullptr );
//
//          continue;
//        }
//
//      }
//
//      // todo: ... clone expr?
//    }
//
//  }
//
//  pCI->Analysis.m_pProjection_prior = pSub_projection();
//
//  pCI->vRenumberCards();
//}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Sort::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_LEVEL,CK_KEY,CK_PROJECTION,CK_NONE);

  pSub_level()->vValidate(pCI,iDepth+1,this);
  pSub_key()->vValidate(pCI,iDepth+1,this);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
