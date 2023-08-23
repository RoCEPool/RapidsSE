/*============================================================================*/
/* Kid.hpp --                                                                 */
/*============================================================================*/
#pragma once

#include "Board.hpp"


namespace STORE
{
  class Kid
  {
  private:
    Card *m_pParent;
    i8 m_i_Child;

    Card *m_pChild;

  public:
    Kid(void);
    Kid(Card *pParent);
    ~Kid(void) noexcept(false);

    void vBegin(void);
    void vBeginForward(void);
    void vBeginReverse(void);

    eCK  eChild(void);
    i8   i_Child(void);
    Card *pChild(void);
    void vChild(i8 i_Child);

    bool bEach(void);
    bool bEach(eCK eCard);
    bool bEach_reverse(void);
    bool bEach_reverse(eCK eCard);

    void vForward(void);

    i8 iGetOdn(void);

    bool bInRange(void);

    void vNext(void);

    Card *pParent(void);
    void vParent(Card *pParent);
    void vPrev(void);

    void vReverse(void);

    void vSynch(Card *pChild_with);

    Card_Bound *  p_bound(void);
    Card_Expr *   p_expr(void);
    Card_IdxCol * p_idxCol(void);
    Card_Index *  p_index(void);
    Card_Map *    p_map(void);
    Card_OP *     p_OP(void);
    Card_Stage *  p_stage(void);
    Card_Table *  p_table(void);
    Card_Term *   p_term(void);

    private:
    void vChildDec(void);
    void vChildInc(void);
    void vChildReset(void);
    void vChildUpdate(void);

  };
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
