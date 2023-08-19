/*============================================================================*/
/* Kid.hpp --                                                                 */
/*============================================================================*/
#pragma once

#include "Board.hpp"


namespace STORE
{
  class Kid //: public Object
  {
  private:
    Card *m_pParent;
    i8 m_i_Child;
  public:

    bool m_bActive;

  private:
//    // must be last
//    Footer m_xFooter;

  public:
//    void* operator new(size_t);
//    void  operator delete(void*);

    Kid(void);

    Kid(Card *pParent);

    ~Kid(void) noexcept(false);

    void vBeginForward(void);

    void vBeginReverse(void);

    void vFirst(void);

    void vLast(void);

    void vNext(void);

    void vPrev(void);

    bool bEach(void);

    bool bEach_reverse(void);

    bool bEach(eCK eCard);

    bool bEach_reverse(eCK eCard);

    Card *pParent(void);

    Card *pChild(void);

    Card *pChild(i8 i_Child);

    eCK eChild(void);

    i8 i_Child(void);

    Card_Bound *  p_bound(void);
    Card_Expr *   p_expr(void);
    Card_IdxCol * p_idxCol(void);
    Card_Index *  p_index(void);
    Card_Map *    p_map(void);
    Card_OP *     p_OP(void);
    Card_Stage *  p_stage(void);
    Card_Table *  p_table(void);
    Card_Term *   p_term(void);

    i8 iGetOdn(void);

    Card *pFirstChild(void);

    Card *pNextChild(void);

    Card *pPrevChild(void);

    Card *pLastChild(void);

    bool bIsFirst(void);

    bool bIsLast(void);

    void vForward(void);

    void vReverse(void);

    bool bInRange(void);

    i8 nChildren(void);

    void vParent(Card *pParent);

    void vChild(i8 i_Child);

    void vBegin(void);

    void vSynch(Card *pChild_with);

//  private:
//
//    Footer* aFooter(void);
  };
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
