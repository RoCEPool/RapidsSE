/*============================================================================*/
/* CarryForward.hpp -- Carry Forward                                          */
/*============================================================================*/
#pragma once

#include "BASE.h"


namespace STORE
{
  class Card_Expr;
  class CardIter;

  class CarryForward : public Object
  {

  public:

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
      bool m_bMorph_To_gbyAgg;
    } Aggreg;

    struct {
      bool m_bSeen;
      bool m_bInject_foldExpr;
    } GbyAgg;

    struct {
      bool m_bSeen;
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
