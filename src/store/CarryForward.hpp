/*============================================================================*/
/* CarryForward.hpp -- Carry Forward                                          */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "eSM.h"


namespace STORE
{
  class Card_Expr;
  class CardIter;

  class CarryForward : public Object
  {

  private:
    Card_Expr* m_p_sqCrlKey;
    bool       m_bCrlKeyActive;

    Card_Expr* m_p_sqResult;
    bool       m_bResultActive;

  public:
    CardIter*  m_pCF;

    struct {
      bool m_bSeen;
      i8   m_nTables;
      bool m_bReproject;
    } Table;

    struct {
      bool m_bSeen;
      bool m_bMorph_sqlOp_To_EQ;
      bool bReplace_sqCall_With_result;
    } Where_preAgg;

    struct {
      bool m_bSeen;
      i8   m_nBumpBy;
      bool m_bMorph_To_gbyAgg;
    } Aggreg;

    struct {
      bool m_bSeen;
      i8   m_nBumpBy;
      bool m_bInject_foldExpr;
    } GbyAgg;

    struct {
      bool m_bSeen;
      i8   m_nBumpBy;
    } Project_postAgg;

    struct {
      bool m_bSeen;
      bool m_bCorrect_After_Inject;
    } Having;

    struct {
      bool m_bSeen;
      bool m_bCorrect_After_Inject;
    } Sort;

    struct {
      bool m_bSeen;
      bool m_bUnproject_foldExpr;
    } Unproject;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CarryForward(void);
   ~CarryForward(void) noexcept(false);

    bool bCrlKeyActive(void);
    void vCrlKeyActive(bool bCrlKeyActive);

    bool bIsCorrelated(void);

    bool bResultActive(void);
    void vResultActive(bool bResultActive);

    Card_Expr* p_sqCrlKey(void);
    void v_sqCrlKey(Card_Expr* p_sqCrlKey);

    Card_Expr* p_sqResult(void);
    void v_sqResult(Card_Expr* p_sqResult);

    bool bWorkToDo(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vSetSeen(CardIter* pCI);


  protected:
    Footer* aFooter(void);
  };

  CarryForward* _pCarryForward(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
