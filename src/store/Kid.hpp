/*============================================================================*/
/* Kid.hpp -- kid iterator                                                    */
/*============================================================================*/
#pragma once

#include "Board.hpp"


namespace STORE
{
  static const bool C_SUB = true;

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
    Kid(Card *pParent, Card *pChild);
    ~Kid(void) noexcept(false);

    void vBackstep(void);
    void vBegin(void);
    void vBegin_reverse(void);
    bool bBegun(void);

    eCK  eChild(void);
    i8   i_Child(void);
    Card *pChild(void);

    void vDelete(void);

    bool bEach(void);
    bool bEach_opaque(void);
    bool bEach(eCK eChild);
    bool bEach_opaque(eCK eChild);
    bool bEach(eCK eChild, eCK eSub1);
    bool bEach(eCK eChild, eCK eSub1, eCK eSub2);
    bool bEach(eCK eChild, eCK eSub1, eCK eSub2, eCK eSub3);
    bool bEach_reverse(void);
    bool bEach_reverse(eCK eChild);
    bool bEnded(void);
    void vEnded(void);

    bool bForwards(void);
    void vForwards(bool bForwards);

    bool bLess(void);

    bool bMore(void);

    Card *pNext(void);

    Card *pParent(void);
    void vParent(Card *pParent);
    bool bParentIs(eCK eParent);
    Card *pPrev(void);

    void vRemove(void);
    void vReposition(i8 i_Child);
    void vReposition(Card* pChild);

    Card *pSub1(void);
    Card *pSub2(void);
    Card *pSub3(void);

    bool bWithinRange(void);

    Card_AndConj*     p_andConj(void);
    Card_Bound*       p_bound(bool bSub=false);
    Card_Correlation* p_correlation(bool bSub=false);
    Card_Expr*        p_expr(bool bSub=false);
    Card_Fold*        p_fold(bool bSub=false);
    Card_HshCol*      p_hshCol(bool bSub=false);
    Card_IdxCol*      p_idxCol(bool bSub=false);
    Card_Index*       p_index(bool bSub=false);
    Card_JoinOrder*   p_joinOrder(bool bSub=false);
    Card_Map*         p_map(bool bSub=false);
    Card_RlnConj*     p_rlnConj(bool bSub=false);
    Card_Stage*       p_stage(bool bSub=false);
    Card_StreamHead*  p_streamHead(bool bSub=false);
    Card_StreamJoin*  p_streamJoin(bool bSub=false);
    Card_StreamKey*   p_streamKey(bool bSub=false);
    Card_StreamPer*   p_streamPer(bool bSub=false);
    Card_Table*       p_table(bool bSub=false);
    Card_Term*        p_term(bool bSub=false);

    private:
    void vChildReset(void);
    void vChildUpdate(void);
  };
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
