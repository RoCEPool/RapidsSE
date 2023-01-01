/*============================================================================*/
/* Xderive.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "CardIter.hpp"

namespace STORE
{
  class Table;

  class Xderive : public CardIter
  {
  public:
    CardCtl*     m_pCardCtl;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Xderive(eCN eClass);
   ~Xderive(void) noexcept(false);

    Card_Expr* D_pDerive_Gby_expr_From_stmCol(Card_StmCol* p_stmCol);
    Card_Expr* D_pDerive_Proj_expr_From_Gby_expr(Card_Expr* p_expr_gby, Stz* szAs=nullptr);
    Card_Expr* D_pDerive_Proj_expr_From_stmCol(Card_StmCol* p_stmCol);

    void D_vDerive_ObyElist(Card_ObyElist* p_obyElist, Index* pIndex);

    char D_cPRED_TableSyn_First(Card_PRED* p_PRED);

    Card_Expr* D_pProject_Simple_expr_Of(Card_Expr* p_expr, i8 i_Col);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
