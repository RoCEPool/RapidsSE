/*============================================================================*/
/* Kid.hpp -- kid iterator                                                    */
/*============================================================================*/
#pragma once

#include "Board.hpp"


namespace STORE
{
  class Kid
  {
  private:
    Card *m_pParent;

    i8   m_i_Child;
    Card *m_pChild;

    Card *m_pSub1;
    Card *m_pSub2;
    Card *m_pSub3;

    bool  m_bBegun;
    bool  m_bForwards;
    bool  m_bEnded;


  public:
    Kid(void);
    Kid(Card *pParent);
    ~Kid(void) noexcept(false);

    void vBeginEach_forward(void);
    void vBeginEach_reverse(void);
    bool bBegun(void);

    eCK  eChild(void);
    i8   i_Child(void);
    Card *pChild(void);

    void vDelete(void);

    bool bEach(void);
    bool bEach(eCK eCard);
    bool bEach(eCK eCard, eCK eSub1);
    bool bEach(eCK eCard, eCK eSub1, eCK eSub2);
    bool bEach(eCK eCard, eCK eSub1, eCK eSub2, eCK eSub3);
    bool bEach_reverse(void);
    bool bEach_reverse(eCK eCard);
    bool bEnded(void);
    void vEnded(void);

    bool bForwards(void);
    void vForwards(bool bForwards);

    i8   iGetOdn(void);

    bool bLess(void);

    bool bMore(void);

    Card *pNext(void);

    Card *pParent(void);
    void vParent(Card *pParent);
    bool bParentIs(eCK eParent);
    Card *pPrev(void);

    void vRemove(void);
    void vRemove(Card *pChild);
    void vResynch(i8 i_Child);
    void vResynch(Card* pCard);
    void vRewind(void);

    Card *pSub1(void);
    Card *pSub2(void);
    Card *pSub3(void);

    bool bWithinRange(void);

    Card_Bound*       p_bound(void);
    Card_CmpPred*     p_cmpPred(void);
    Card_Correlation* p_correlation(void);
    Card_Expr*        p_expr(void);
    Card_Fold*        p_fold(void);
    Card_HshCol*      p_hshCol(void);
    Card_IdxCol*      p_idxCol(void);
    Card_Index*       p_index(void);
    Card_JoinOrder*   p_joinOrder(void);
    Card_Map*         p_map(void);
    Card_OP*          p_OP(void);
    Card_RpnCalc*     p_rpnCalc(void);
    Card_Stage*       p_stage(void);
    Card_StreamHead*  p_streamHead(void);
    Card_StreamJoin*  p_streamJoin(void);
    Card_StreamKey*   p_streamKey(void);
    Card_StreamPer*   p_streamPer(void);
    Card_Table*       p_table(void);
    Card_Term*        p_term(void);

    private:
    void vChildReset(void);
    void vChildUpdate(void);
  };
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
