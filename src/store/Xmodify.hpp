/*============================================================================*/
/* Xmodify.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xsentinals.hpp"

namespace STORE
{
  class Table;

  class Xmodify : public Xsentinals
  {
  public:
    i8           m_iOdnLo;
    i8           m_iOdnHi;
    char         m_cTableSyn;
    CardCtl*     m_pCardCtl;


    // must be last
    Footer m_xFooter;

  public:
    Xmodify(eCN eClass);
   ~Xmodify(void) noexcept(false);

  public:
    i8 E_iELIST_rpnCalc_Has_stmOrdOdn(Card_ELIST* p_ELIST, Card_OrdElmOdn* p_stmOrdOdn);
    i8 E_iExpr_rpnCalc_WhichGbyKey(Card_Expr* p_expr);

    void M_vStream_Derive_OrdinalRange_table(Card_Stream* p_stream, char cTblSyn);

    void M_vTable_Set_Syn(Card_Table* p_table);

    void M_vTerm_Bump_KeyOrdinal(Card_Term* p_term, i8 nBy=1);
    void M_vTerm_Bump_StreamColOrdinal(Card_Term* p_term, i8 nBy=1);
    void M_vTerm_MarkIfMatchesAn_idxCol(Card_Term* p_term, Card_IdxColList* p_idxColList);

    // generalised drill down

    bool bPRED_Descend(Card_PRED* p_pred, std::function<bool(Card_PRED* )> bfLambdaFn);

    void vStreamAnalysisReset(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
