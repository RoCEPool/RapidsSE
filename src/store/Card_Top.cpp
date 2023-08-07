/*============================================================================*/
/* Card_Top.cpp -- Limit                                                    */
/*============================================================================*/

#include "Card_Top.hpp"
#include "Card_Level.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Top,CK_TOP)
__aNewFunction(Card_Top,CN_CARD)
__vDeleteFunction(Card_Top,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Top::Card_Top(Stz* szRows)
: Card_OP(CK_TOP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  SYM(SF_OTHER,szRows);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Top::~Card_Top(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* column mapping                                                             */
/*----------------------------------------------------------------------------*/
void Card_Top::vColumn_Mapping(CardIter *pCI)
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
    pCI->G_vAppendNewChild(pCI->G.p_keyCells,CK_MAP,p_expr_key->pSymbol());
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
      pCI->G_vAppendNewChild(pCI->G.p_payloadCells, CK_MAP,p_expr_projection->pSymbol());
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
//void Card_Top::vColumn_Mapping(CardIter *pCI)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
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
//
//}

/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_Top::vComputeProjection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Top::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_LEVEL,CK_COUNT,CK_NONE);

  pSub_level()->vValidate(pCI,iDepth+1,this);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
