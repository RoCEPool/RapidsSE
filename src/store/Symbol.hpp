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
  struct Facet {
    eSF  m_eFacet;
    s256 m_sFacet;
  };

  struct Cast {
    TypeId m_xTypeId;
    i8     m_nTypeLength;
    i8     m_nTypeScale;
    b8     m_bTypeNullable;
  };

  class CardIter;

  class Symbol : public Object
  {
  private:
    static const i8 C_FACETS = 6;

    i8    m_nFacets;
    Facet xaFacet[C_FACETS];
    Cast  m_xCast;
    TagSet* m_pProperties;

    bool m_bChanged;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    Symbol(void);
    Symbol(eSF eFacet, char cChar, i8 iInt);
    Symbol(eSF eFacet, i8 iInt);
    Symbol(eSF eFacet, i8 iInt, Cast xCast);
    Symbol(eSF eFacet, Stz* szArg);
    Symbol(eSF eFacet, Stz* szArg, Cast xCast);

    ~Symbol(void);

    static eRO  eEvalRelnOp(Stz* szArg);
    static eQO  eEvalSubQryOp(Stz* szArg);
    static eSF  eEval_facet(Stz* szArg);

    static bool bIs_Agg_Fn(Stz* szArg);          // avg, count, ... sum
    static bool bIs_As(Stz* szArg);              // AS
    static bool bIs_Bol_Fn(Stz* szArg);          // between, case ... matchTrailing
    static bool bIs_Bol_Lit(Stz* szArg);         // FALSE, TRUE
    static bool bIs_Bol_Op(Stz* szArg);          // and, not, or
    static bool bIs_Bound(Stz* szArg);           // BOUND, LBOUND, UBOUND
    static bool bIs_Brace(Stz* szArg);           // {
    static bool bIs_Case(Stz* szArg);            // CASE
    static bool bIs_Cast(Stz* szArg);            // CAST
    static bool bIs_CastType(Stz* szArg);        // CASTTYPE
    static bool bIs_Col_Type(Stz* szArg);        // BOOLEAN, ... VARCHAR
    static bool bIs_Comma(Stz* szArg);           // ,
    static bool bIs_Cross(Stz* szArg);           // CROSS
    static bool bIs_Dcm_Lit(Stz* szArg);         // StrLib::bIsDecimal()
    static bool bIs_Direction(Stz* szArg);       // asc, desc
    static bool bIs_Else(Stz* szArg);            // ELSE
    static bool bIs_EndIf(Stz* szArg);           // ENDIF
    static bool bIs_Exp_Col(Stz* szArg);         // $n
    static bool bIs_Fld_Nm(Stz* szArg);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Flt_Lit(Stz* szArg);         // StrLib::bIsFloat()
    static bool bIs_GroupBy(Stz* szArg);         // GROUPBY
    static bool bIs_Idx_Col(Stz* szArg);         // $In
    static bool bIs_Idx_Nm(Stz* szArg);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Int_Lit(Stz* szArg);         // StrLib::bIsInteger()
    static bool bIs_Interval_DT(Stz* szArg);     // INTERVAL_DT
    static bool bIs_Interval_YM(Stz* szArg);     // INTERVAL_YM
    static bool bIs_Itm_Col(Stz* szArg);         // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Ivl_Fn(Stz* szArg);          // extract_year etc
    static bool bIs_List_Cnt(Stz* szArg);        // #n
    static bool bIs_Key_Col(Stz* szArg);         // $Kn
    static bool bIs_Mth_Op(Stz* szArg);          // +, -, *, /
    static bool bIs_Negate(Stz* szArg);          // NULL
    static bool bIs_Nmd_Col(Stz* szArg);         // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Nmd_Col_synthetic(Stz* szArg);  // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Note(Stz* szArg);            // ...any...
    static bool bIs_Null(Stz* szArg);            // NULL
    static bool bIs_Parenthesis(Stz* szArg);     // ( or )
    static bool bIs_Rln_Op(Stz* szArg);          // <, <=, ... >
    static bool bIs_Sqy_Nm(Stz* szArg);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Sqy_No(Stz* szArg);          // $Qn (or __subqueryN)
    static bool bIs_Sqy_Op(Stz* szArg);          // exists, in, notExists, notIn
    static bool bIs_Sqy_Prm(Stz* szArg);         // %n
    static bool bIs_Sqy_Rsl(Stz* szArg);         // __n
    static bool bIs_Stg_Cast(Stz* szArg);        // BROADCAST, HASHCAST
    static bool bIs_Stg_Nm(Stz* szArg);          // Sn
    static bool bIs_Str_Fn(Stz* szArg);          // SUBSTRING2 ... SUBSTRING3
    static bool bIs_Str_Lit(Stz* szArg);          // 'abc'
    static bool bIs_Tbl_Col(Stz* szArg);         // $c.n
    static bool bIs_Tbl_Nm(Stz* szArg);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Tbl_Syn(Stz* szArg);         // $c
    static bool bIs_Tbl_Syn_rep(Stz* szArg);     // $C
    static bool bIs_Then(Stz* szArg);            // THEN
    static bool bIs_Type(Stz* szArg);            // TYPE
    static bool bIs_When(Stz* szArg);            // WHEN
    static bool bIs_Xk_Cast(Stz* szArg);         // XK_BROADCAST, XK_HASHCAST, XK_UNICAST

    static bool bIs_Rw_Aggregates(Stz* szArg);   // AGGREGATES

    static bool bIs_bound(Facet* aFacet);    //
    static bool bIs_rlnConj(Facet* aFacet);  //
    static bool bIs_term(Facet* aFacet);     //

    static bool bPossibleNm(Stz* szArg);

    static bool bSame(Facet* aFacet_a, Facet* aFacet_b);

    static TypeId xTo_TypeId(Stz* szTypeName);

    // - - - - - -

    void vApplyCast(Cast xCast);
    void vApplyCastFacets(Symbol* pSymbol_arg);
    void vApplyCastFacetsPpy(Symbol* pSymbol_arg);
    void vApplyFacet(Facet* aFacet);
    void vApplyFacet(eSF eFacet);
    void vApplyFacet(eSF eFacet, char cChr);
    void vApplyFacet(eSF eFacet, char cChr, i8 iInt);
    void vApplyFacet(eSF eFacet, i8 iInt);
    void vApplyFacet(eSF eFacet, Stz* szArg);
    void vApplyFacet_varg(eSF eFacet, Stz* szFormat, ...);
    void vApplyFacet(eSF eFacet, i8 iInt, Symbol* pSymbol);
    void vApplyFacet(eSF eFacet, Symbol* pSymbol);
    void vApplyFacets(Symbol* pSymbol_arg);

    Cast xCast(void);
    bool bChanged(void);
    void vChanged(bool bChanged);
    eCT  eColType(void);
    void vCullFacet_except(eSF eFacet_except);
    void vCullFacet_leaving(eSF eFacet_except1, eSF eFacet_except2);

    void vDeleteFacet(eSF eFacet);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    void vDump(Stz* szTitle=""_Stz);

    Facet* aFacet(eSF eFacet);
    Facet* aFacet(i8 i_Facet);
    eSF  eFacet(i8 i_Facet);
    i8   i_Facet(eSF eFacet);
    i8   i_Facet(eSF eFacet, char cTblSyn);
    Stz* szFacet(eSF eFacet);
    Stz* szFacet(i8 i_Facet);
    Stz* szFacet_bound(void);
    Stz* szFacet_rlnConj(void);
    Stz* szFacet_single(void);
    Stz* szFacet_term(void);
    i8   nFacets(void);
    void vFacets_To_String(String* pString);

    bool bHasFacet(eSF eFacet);
    bool bHasFacet(eSF eFacet, Stz* szArg);
    bool bHasFacet(Stz* szArg);
    bool bHasFacets(void);
    bool bHasFacets(i8 /*eSF*/ iFacet_first, ...);
    bool bHasProperties(void);

    void vInc_Tbl_Syn(void);
    void vInit_Tbl_Syn(char cTblSyn);

    bool bIs_Col(void);  // SF_EXP_COL,SF_KEY_COL,SF_ITM_COL,SF_NMD_COL,SF_TBL_COL
    bool bIs_Lit(void);  // FALSE, TRUE, ...

    bool bMatches(eSF eFacet_arg, Stz* szFacet_arg);
    bool bMatches(eSF eFacet, Symbol* pSymbol_arg);
    bool bMatches(Symbol* pSymbol_arg);
    bool bMatches_ignoring(Symbol* pSymbol_arg, eSF eFacet_ignore);
    bool bMultiFaceted(i8 nFacets_arg=C_UNKNOWN);

    bool bNeedsBrace(void);         // between, case, ... matchTrailing

    void vResetFacets(void);
    eRO  eRlnOp(void);

    bool bSameCast(Symbol* pSymbol_arg);
    bool bSingleFaceted(eSF eFacet_arg=SF_NONE);
    eQO  eSqyOp(void);

    char   cTblSyn(void);
    i8     iTo_integer(eSF eFacet);
    i8     iTo_ordinal(eSF eFacet);
    TypeId xTypeId(void);
    i8     nTypeLength(void);
    bool   bTypeNullable(void);
    i8     nTypeScale(void);

    bool bValidate(void);
    void vValidate(void);

    bool bValidCast(void);
    bool bValidLenScale(void);
    bool bValidType(void);

    void Y_vAll(Symbol* pSymbol_arg);
    void Y_vCut(Stz* szPpyNm);
    i8   Y_iGet(i8 i_Ppy);
    i8   Y_iGet(Stz* szPpyNm);
    Object* Y_pGet(i8 i_Ppy);
    Object* Y_pGet(Stz* szPpyNm);
    Stz* Y_szGet(i8 i_Ppy);
    Stz* Y_szGet(Stz* szPpyNm);
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
    void Y_vTo_String(String* pString);

  private:

    void vCastReset(void);

    void vFacet_Agg_Fn(Stz* szAggFn);
    void vFacet_As(Stz* szAs);
    void vFacet_Bol_Fn(Stz* szBolFn);
    void vFacet_Bol_Lit(Stz* szBolLit);
    void vFacet_Bol_Op(Stz* szBolOp);
    void vFacet_Bound(Stz* szBound);
    void vFacet_Brace(Stz* szBrace);
    void vFacet_Case(Stz* szCase);
    void vFacet_Cast(Stz* szCast);
    void vFacet_CastType(Stz* szCastType);
    void vFacet_Col_Type(Stz* szColType);
    void vFacet_Comma(Stz* szComma);
    void vFacet_Cross(Stz* szCross);
    void vFacet_Dcm_Lit(Stz* szDcmLit);
    void vFacet_Direction(Stz* szDirection);
    void vFacet_Else(Stz* szElse);
    void vFacet_EndIf(Stz* szEndIf);
    void vFacet_Exp_Col(Stz* szExpCol);
    void vFacet_Fld_Nm(Stz* szFldNm);
    void vFacet_Flt_Lit(Stz* szFltLit);
    void vFacet_GroupBy(Stz* szGroupBy);
    void vFacet_Idx_Col(Stz* szIdxCol);
    void vFacet_Idx_Col(i8 iIdxCol);
    void vFacet_Idx_Nm(Stz* szIdxNm);
    void vFacet_Int_Lit(Stz* szIntLit);
    void vFacet_Int_Lit(i8 iIntLit);
    void vFacet_Interval_DT(Stz* szInterval_DT);
    void vFacet_Interval_YM(Stz* szInterval_YM);
    void vFacet_Itm_Col(i8 i_Child);
    void vFacet_Itm_Col(i8 i_Child, Symbol* pSymbol);
    void vFacet_Itm_Col(Stz* szIntLit);
    void vFacet_Ivl_Fn(Stz* szIvlFn);
    void vFacet_List_Cnt(Stz* szListCnt);
    void vFacet_List_Cnt(i8 nListCnt);
    void vFacet_Key_Col(Stz* szKeyCol);
    void vFacet_Key_Col(i8 iKeyCol);
    void vFacet_Mth_Op(Stz* szMthOp);
    void vFacet_Negate(Stz* szNegate);
    void vFacet_Nmd_Col(i8 iColNo);
    void vFacet_Nmd_Col(Stz* szColNm);
    void vFacet_Note(Stz* szNote);
    void vFacet_Null(Stz* szNull);
    void vFacet_Parenthesis(Stz* szParenthesis);
    void vFacet_Rln_Op(Stz* szRlnOp);
    void vFacet_Sqy_Nm(Stz* szPpeNm);
    void vFacet_Sqy_No(i8 iSqyNo);
    void vFacet_Sqy_No(Stz* szSqyNo);
    void vFacet_Sqy_Op(Stz* szSqyOp);
    void vFacet_Sqy_Prm(i8 iOrdinal);
    void vFacet_Sqy_Prm(Stz* szSqyPrm);
    void vFacet_Sqy_Rsl(i8 iOrdinal);
    void vFacet_Sqy_Rsl(Stz* szSqyRsl);
    void vFacet_Stg_Cast(Stz* szStgCast);
    void vFacet_Stg_Nm(i8 iStgNo);
    void vFacet_Exp_Col(i8 iExpCol);
    void vFacet_Str_Fn(Stz* szStrFn);
    void vFacet_Str_Lit(Stz* szStrLit);
    void vFacet_Tbl_Col(Stz* szTblCol);
    void vFacet_Tbl_Col(char cTblSyn, i8 iTblCol);
    void vFacet_Tbl_Nm(Stz* szTblNm);
    void vFacet_Tbl_Syn(Stz* szTblSyn);
    void vFacet_Tbl_Syn(char cTblSyn);
    void vFacet_Tbl_Syn_rep(void);
    void vFacet_Tbl_Syn_rep(Stz* szTblSyn);
    void vFacet_Then(Stz* szThen);
    void vFacet_Type(Stz* szType);
    void vFacet_When(Stz* szWhen);
    void vFacet_Xk_Cast(Stz* szXkCast);
    void vFacet_Rw_Aggregates(Stz* szAggregates);

    void vFacetCut(i8 i_Facet_cut);
    void vFacetSet(eSF eFacet, Stz* szFacet);
    void vFacetSet(i8 i_Facet, eSF eFacet, Stz* szFacet);
    void vFacetSprint(eSF eFacet, Stz* szFormat, ...);

  protected:
    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
