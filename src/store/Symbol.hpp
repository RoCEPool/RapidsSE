/*============================================================================*/
/* Symbol.hpp -- Abstract Symbol                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Ppy.hpp"
#include "eQO.h"
#include "eRO.h"
#include "eSF.h"
#include "eST.h"

namespace STORE
{
  class Symbol : public Object
  {
  private:
    eSF     m_eSymbol;
    eST     m_eSketch;
    s256    m_sSymbol;
    TagSet* m_pProperties;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    Symbol(void);
    Symbol(eSF eSymbol, i8 iInt);
    Symbol(eSF eSymbol, Stz* szStz);

    ~Symbol(void);

    static eRO  eEvalRelnOp(Stz* szSymbol);
    static eQO  eEvalSubQryOp(Stz* szSymbol);
    static eSF  eEvalSymbol(Stz* szSymbol);

    static bool bIs_Agg_Fn(Stz* szArg);          // avg, count, ... sum
    static bool bIs_As(Stz* szArg);              // AS
    static bool bIs_As_Nm(Stz* szArg);           // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Bol_Fn(Stz* szArg);          // between, case ... matchTrailing
    static bool bIs_Bol_Lit(Stz* szArg);         // FALSE, TRUE
    static bool bIs_Bol_Op(Stz* szArg);          // and, not, or
    static bool bIs_Bound(Stz* szArg);           // BOUND, LBOUND, UBOUND
    static bool bIs_Brace(Stz* szArg);           // {
    static bool bIs_Col_Nm(Stz* szArg);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Col_No(Stz* szArg);          // $n
    static bool bIs_Col_Tc(Stz* szArg);          // $c.n
    static bool bIs_Col_Type(Stz* szArg);        // BOOLEAN, ... VARCHAR
    static bool bIs_Comma(Stz* szArg);           // ,
    static bool bIs_Cross(Stz* szArg);           // CROSS
    static bool bIs_Dcm_Lit(Stz* szArg);         // StrLib::bIsDecimal()
    static bool bIs_Direction(Stz* szArg);       // asc, desc
    static bool bIs_Fld_Nm(Stz* szArg);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Flt_Lit(Stz* szArg);         // StrLib::bIsFloat()
    static bool bIs_GroupBy(Stz* szArg);         // GROUPBY
    static bool bIs_Idx_Col(Stz* szArg);         // $In
    static bool bIs_Idx_Nm(Stz* szArg);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Int_Lit(Stz* szArg);         // StrLib::bIsInteger()
    static bool bIs_Key_Col(Stz* szArg);         // $Kn
    static bool bIs_Mth_Op(Stz* szArg);          // +, -, *, /
    static bool bIs_Note(Stz* szArg);            // ...any...
    static bool bIs_Number(Stz* szArg);          // StrLib::bIsDecimal/Float/Integer()
    static bool bIs_Parenthesis(Stz* szArg);     // ( or )
    static bool bIs_Ppe_Nm(Stz* szArg);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Rln_Op(Stz* szArg);          // <, <=, ... >
    static bool bIs_Sqy_Crl_Key(Stz* szStz);     // _SQCRLKEY_
    static bool bIs_Sqy_Nm(Stz* szStz);          // __subqueryN
    static bool bIs_Sqy_No(Stz* szStz);          // __subqueryN
    static bool bIs_Sqy_Op(Stz* szArg);          // exists, in, notExists, notIn
    static bool bIs_Sqy_Prm(Stz* szArg);         // %n
    static bool bIs_Sqy_Rsl(Stz* szArg);         // __n
    static bool bIs_Stg_Cast(Stz* szArg);        // BROADCAST, HASHCAST
    static bool bIs_Stg_Nm(Stz* szArg);          // Sn
    static bool bIs_String(Stz* szArg);          // 'abc'
    static bool bIs_Tbl_Nm(Stz* szArg);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Tbl_Syn(Stz* szArg);         // $c
    static bool bIs_Tbl_Syn_rep(Stz* szArg);     // $C
    static bool bIs_Type(Stz* szArg);            // TYPE
    static bool bIs_When(Stz* szArg);            // WHEN
    static bool bIs_Xk_Cast(Stz* szArg);         // XK_BROADCAST, XK_HASHCAST, XK_UNICAST
    static bool bIs_Xrf_No(Stz* szArg);          // #n

    static void vStripDisamb(String* psArg);

    static eCT  eTo_eCT_str(Stz* szType);
    static eCT  eTo_eCT_char(Stz* szType);

    // - - - - - -

    eRO  eEvalRelnOp(void);
    eQO  eEvalSubQryOp(void);

    bool bHasSymbol(void);
    bool bHasProperties(void);

    void vInc_Tbl_Syn(void);
    void vInit_Tbl_Syn(char cTblSyn);

    bool bIs_Agg_Fn(void);          // avg, count, ... sum
    bool bIs_As(void);              // AS
    bool bIs_As_Nm(void);           //
    bool bIs_As_Nm_syn(void);       // __COLn
    bool bIs_Bol_Fn(void);          // between, case ... matchTrailing
    bool bIs_Bol_Lit(void);         // FALSE, TRUE
    bool bIs_Bol_Op(void);          // and, not, or
    bool bIs_Bound(void);           // BOUND, LBOUND, UBOUND
    bool bIs_Brace(void);           // BRACE
    bool bIs_Col_Nm(void);          // bIsName() but not bIs_Agg_Fn() etc
    bool bIs_Col_Nm_dis(void);      // C_n
    bool bIs_Col_Nm_syn(void);      // C_n
    bool bIs_Col_No(void);          // $n
    bool bIs_Col_Tc(void);          // $c.n
    bool bIs_Col_Type(void);        // BOOLEAN, ... VARCHAR
    bool bIs_Comma(void);           // ,
    bool bIs_Cross(void);           // CROSS
    bool bIs_Dcm_Lit(void);         // StrLib::bIsDecimal()
    bool bIs_Direction(void);       // asc, desc
    bool bIs_Fld_Nm(void);          // bIsName() but not bIs_Agg_Fn() etc
    bool bIs_Flt_Lit(void);         // StrLib::bIsFloat()
    bool bIs_GroupBy(void);         // GROUPBY
    bool bIs_Idx_Col(void);         // $In
    bool bIs_Idx_Nm(void);          // Sn
    bool bIs_Int_Lit(void);         // StrLib::bIsInteger()
    bool bIs_Key_Col(void);         // $Kn
    bool bIs_Mth_Op(void);          // +, -, *, /
    bool bIs_Note(void);            // ...any...
    bool bIs_Number(void);          // StrLib::bIsDecimal/Float/Integer()
    bool bIs_Parenthesis(void);     // ( or )
    bool bIs_Ppe_Nm(void);          // bIsName() but not bIs_Agg_Fn() etc
    bool bIs_Rln_Op(void);          // <, <=, ... >
    bool bIs_Sqy_No(void);          // __sq%02lld
    bool bIs_Sqy_Op(void);          // exists, in, notExists, notIn
    bool bIs_Sqy_Prm(void);         // %n
    bool bIs_Stg_Cast(void);        // BROADCAST, HASHCAST
    bool bIs_Sqy_Rsl(void);         // __n
    bool bIs_Stg_Nm(void);          // Sn
    bool bIs_String(void);          // 'abc''
    bool bIs_Tbl_Nm(void);          // Sn
    bool bIs_Tbl_Syn(void);         // c
    bool bIs_Tbl_Syn_rep(void);     // c
    bool bIs_Type(void);            // TYPE
    bool bIs_When(void);            // WHEN
    bool bIs_Xk_Cast(void);         // XK_BROADCAST, XK_HASHCAST, XK_UNICAST
    bool bIs_Xrf_No(void);          // #n

    bool bNeedsBrace(void);         // between, case, ... matchTrailing

    void vPpy_To_String(String* pString);

    bool bSame(char cArg);
    bool bSame(Stz* szArg);
    bool bSame(Symbol* pArg);

    char cSF_char(void);
    i8   iSF_number(void);
    i8   iSF_ordinal(void);
    Stz* szSF_symbol(eSF eSymbol_xchk);
    char cSF_tblSyn(void);

    void vSym(eSF eSymbol);
    void vSym(eSF eSymbol, char cChr);
    void vSym(eSF eSymbol, char cChr, i8 iInt);
    void vSym(eSF eSymbol, i8 iInt);
    void vSym(eSF eSymbol, i8 iInt, Symbol* pSymbol);
    void vSym(eSF eSymbol, Stz* szStz);
    void vSym(Symbol* pSymbol);
    void vSymPpy(Symbol* pSymbol);

    bool bTest_mustHave_ppy(Stz* szPpy, ...);

    Stz* szWhatIs(void);

    void Y_vAll(Symbol* pSymbol_arg);
    void Y_vCut(Stz* szPpyNm);
//  char Y_cGet(i8 i_Ppy);
//  char Y_cGet(Stz* szPptNm);
    i8   Y_iGet(i8 i_Ppy);
    i8   Y_iGet(Stz* szPpyNm);
    Object* Y_pGet(i8 i_Ppy);
    Object* Y_pGet(Stz* szPpyNm);
    Stz* Y_szGet(i8 i_Ppy);
    Stz* Y_szGet(Stz* szPpyNm);
    Ppy  Y_xGet(i8 i_Ppy);
    Ppy  Y_xGet(Stz* szPpyNm);
    bool Y_bHas(Stz* szPpyNm);
    i8   Y_nPpy(void);
    Stz* Y_szPpyNm(i8 i_Ppy);
    eTN  Y_ePpyTkn(i8 i_Ppy);
    eTN  Y_ePpyTkn(Stz* szPpyNm);
    void Y_vSet(Stz* szPpyNm);
    void Y_vSet(Stz* szPpyNm, i8 iPpy);
    void Y_vSet(Stz* szPpyNm, Object* pObject);
    void Y_vSet(Stz* szPpyNm, Stz* szPpy);
    void Y_vSet(Stz* szPpyNm, Symbol* pSymbol);
    void Y_vSetSprint(Stz* szPpyNm, Stz* szFormat, ...);

    eCT  Y_eColType(void);
    Stz* Y_szColType(void);
    void Y_vColType(Stz* szColType);

  private:
    void vSet_Agg_Fn(Stz* szAggFn);
    void vSet_As(Stz* szAs);
    void vSet_As_Nm(Stz* szAsNm);
    void vSet_As_Nm_syn(i8 iAsNo);
    void vSet_Bol_Fn(Stz* szBolFn);
    void vSet_Bol_Lit(Stz* szBolLit);
    void vSet_Bol_Op(Stz* szBolOp);
    void vSet_Bound(Stz* szBound);
    void vSet_Brace(Stz* szBrace);
    void vSet_Col_Nm(Stz* szColNm);
    void vSet_Col_Nm_dis(i8 i_Child, Symbol* pSymbol);
    void vSet_Col_Nm_syn(Stz* szColNm_syn);
    void vSet_Col_No(Stz* szStz);
    void vSet_Col_No(i8 iStmColOdn);
    void vSet_Col_Tc(char cTblSyn, i8 iTblColOdn);
    void vSet_Col_Tc(Stz* szColTc);
    void vSet_Col_Type(Stz* szColType);
    void vSet_Comma(Stz* szComma);
    void vSet_Cross(Stz* szCross);
    void vSet_Dcm_Lit(Stz* szDcmLit);
    void vSet_Direction(Stz* szColNm);
    void vSet_Fld_Nm(Stz* szFldNm);
    void vSet_Flt_Lit(Stz* szFltLit);
    void vSet_GroupBy(Stz* szGroupBy);
    void vSet_Idx_Col(i8 iIdxColOdn);
    void vSet_Idx_Col(Stz* szIdxCol);
    void vSet_Idx_Nm(Stz* szIdxNm);
    void vSet_Int_Lit(Stz* szFltLit);
    void vSet_Key_Col(i8 iKeyOdn);
    void vSet_Key_Col(Stz* szKeyCol);
    void vSet_Mth_Op(Stz* szMthOp);
    void vSet_Note(Stz* szNote);
    void vSet_Number(i8 iNumber);
    void vSet_Number(Stz* szNumber);
    void vSet_Parenthesis(Stz* szParenthesis);
    void vSet_Ppe_Nm(Stz* szPpeNm);
    void vSet_Prm_Col(i8 iOrdinal);
    void vSet_Prm_Col(Stz* szPrmCol);
    void vSet_Rln_Op(Stz* szRlnOp);
    void vSet_Sqy_No(i8 iSqNo);
    void vSet_Sqy_Op(Stz* szSqyOp);
    void vSet_Stg_Cast(Stz* szStgCast);
    void vSet_Stg_Nm(i8 iStageNo);
    void vSet_String(Stz* szString);
    void vSet_Tbl_Nm(Stz* szTblNm);
    void vSet_Tbl_Syn(char cTblSyn);
    void vSet_Tbl_Syn(Stz* szTblSyn);
    void vSet_Tbl_Syn_rep(void);
    void vSet_Type(Stz* szType);
    void vSet_When(Stz* szWhen);
    void vSet_Xk_Cast(Stz* szXkCast);
    void vSet_Xrf_No(i8 iXrefNo);
    void vSet_Xrf_No(Stz* szXrefNo);

    void vSymSprint(Stz* szFormat, ...);

  protected:
    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
