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
    s256 m_sChrSeq;
  };

  struct Cast {
    TypeId m_xTypeId;
    i8     m_nTypeLength;
    i8     m_nTypeScale;
    b8     m_bTypeNullable;
  };

  class Symbol : public Object
  {
  private:
    static const i8 C_FACETS = 6;

    i8 m_nFacets;
    Facet xaFacet[C_FACETS];

    Cast m_xCast;

    TagSet* m_pProperties;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    Symbol(void);
    Symbol(eSF eFacet, i8 iInt);
    Symbol(eSF eFacet, Stz* szChrSeq);

    ~Symbol(void);

    static eRO  eEvalRelnOp(Stz* szSymbol);
    static eQO  eEvalSubQryOp(Stz* szArg);
    static eSF  eEvalSymbol(Stz* szArg);

    static bool bIs_Agg_Fn(Stz* szChrSeq);          // avg, count, ... sum
    static bool bIs_As(Stz* szChrSeq);              // AS
    static bool bIs_Bol_Fn(Stz* szChrSeq);          // between, case ... matchTrailing
    static bool bIs_Bol_Lit(Stz* szChrSeq);         // FALSE, TRUE
    static bool bIs_Bol_Op(Stz* szChrSeq);          // and, not, or
    static bool bIs_Bound(Stz* szChrSeq);           // BOUND, LBOUND, UBOUND
    static bool bIs_Brace(Stz* szChrSeq);           // {
    static bool bIs_Case(Stz* szChrSeq);            // CASE
    static bool bIs_Cast(Stz* szChrSeq);            // CAST
    static bool bIs_CastType(Stz* szChrSeq);        // CASTTYPE
    static bool bIs_Col_Type(Stz* szChrSeq);        // BOOLEAN, ... VARCHAR
    static bool bIs_Comma(Stz* szChrSeq);           // ,
    static bool bIs_Cross(Stz* szChrSeq);           // CROSS
    static bool bIs_Dcm_Lit(Stz* szChrSeq);         // StrLib::bIsDecimal()
    static bool bIs_Direction(Stz* szChrSeq);       // asc, desc
    static bool bIs_Else(Stz* szChrSeq);            // ELSE
    static bool bIs_EndIf(Stz* szChrSeq);           // ENDIF
    static bool bIs_Exp_Col(Stz* szArg);            // $n
    static bool bIs_Fld_Nm(Stz* szChrSeq);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Flt_Lit(Stz* szChrSeq);         // StrLib::bIsFloat()
    static bool bIs_GroupBy(Stz* szChrSeq);         // GROUPBY
    static bool bIs_Idx_Col(Stz* szChrSeq);         // $In
    static bool bIs_Idx_Nm(Stz* szChrSeq);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Int_Lit(Stz* szChrSeq);         // StrLib::bIsInteger()
    static bool bIs_Interval_DT(Stz* szChrSeq);     // INTERVAL_DT
    static bool bIs_Interval_YM(Stz* szChrSeq);     // INTERVAL_YM
    static bool bIs_Itm_Col(Stz* szChrSeq);         // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Ivl_Fn(Stz* szChrSeq);          // extract_year etc
    static bool bIs_List_cnt(Stz* szChrSeq);         // #n
    static bool bIs_Key_Col(Stz* szChrSeq);         // $Kn
    static bool bIs_Mth_Op(Stz* szChrSeq);          // +, -, *, /
    static bool bIs_Negate(Stz* szChrSeq);          // NULL
    static bool bIs_Nmd_Col(Stz* szChrSeq);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_None(Stz* szChrSeq);            // <none>
    static bool bIs_Note(Stz* szChrSeq);            // ...any...
    static bool bIs_Null(Stz* szChrSeq);            // NULL
    static bool bIs_Parenthesis(Stz* szChrSeq);     // ( or )
    static bool bIs_Pay_Col(Stz* szChrSeq);         // $Pn
    static bool bIs_Ppe_Nm(Stz* szChrSeq);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Rln_Op(Stz* szChrSeq);          // <, <=, ... >
    static bool bIs_Sqy_Nm(Stz* szArg);             // $Qn (or __subqueryN)
    static bool bIs_Sqy_Op(Stz* szChrSeq);          // exists, in, notExists, notIn
    static bool bIs_Sqy_Prm(Stz* szChrSeq);         // %n
    static bool bIs_Sqy_Rsl(Stz* szChrSeq);         // __n
    static bool bIs_Stg_Cast(Stz* szChrSeq);        // BROADCAST, HASHCAST
    static bool bIs_Stg_Nm(Stz* szArg);             // Sn
    static bool bIs_Str_Fn(Stz* szChrSeq);          // SUBSTRING2 ... SUBSTRING3
    static bool bIs_String(Stz* szChrSeq);          // 'abc'
    static bool bIs_Tbl_Col(Stz* szArg);            // $c.n
    static bool bIs_Tbl_Nm(Stz* szChrSeq);          // bIsName() but not bIs_Agg_Fn() etc
    static bool bIs_Tbl_Syn(Stz* szChrSeq);         // $c
    static bool bIs_Tbl_Syn_rep(Stz* szChrSeq);     // $C
    static bool bIs_Then(Stz* szChrSeq);            // THEN
    static bool bIs_Type(Stz* szChrSeq);            // TYPE
    static bool bIs_When(Stz* szChrSeq);            // WHEN
    static bool bIs_Xk_Cast(Stz* szChrSeq);         // XK_BROADCAST, XK_HASHCAST, XK_UNICAST

    static bool bPossibleNm(Stz* szChrSeq);

    static bool bSame(Facet* aFacet_a, Facet* aFacet_b);

    static TypeId xTo_TypeId(Stz* szTypeName);

    // - - - - - -

    void vAppendFacet(Facet* aFacet_arg);
    void vAppendFacet(eSF eFacet_arg, Stz* szChrSeq_arg);
    void vAppendFacets(Symbol* pSymbol_arg);

    Cast xCast(void);
    void vCast(Cast xCast);
    void vCastFacets(Symbol* pSymbol_arg);
    void vCastFacetsPpy(Symbol* pSymbol_arg);
    void vConcatFacet(eSF eFacet_arg, Stz* szChrSeq_arg);
    void vConcatFacets(Symbol* pSymbol_arg);

    void vDeleteFacet(eSF eFacet);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    void vDump(void);

    eRO  eEvalRlnOp(void);
    eQO  eEvalSqyOp(void);

    Facet* aFacet(eSF eFacet);
    Facet* aFacet(i8 i_Facet);
    eSF  eFacet(i8 i_Facet=0);
    i8   i_Facet(eSF eFacet);
    i8   i_Facet(eSF eFacet, char cTblSyn);
    Stz* szFacet(eSF eFacet);
    void vFacet(eSF eFacet);
    void vFacet(eSF eFacet, char cChr);
    void vFacet(eSF eFacet, char cChr, i8 iInt);
    void vFacet(eSF eFacet, i8 iInt);
    void vFacet(eSF eFacet, i8 iInt, Symbol* pSymbol);
    void vFacet(eSF eFacet, Stz* szChrSeq);
    i8   nFacets(void);
    void vFacets_To_String(String* pString);
    bool bFacetIs(eSF eFacet_arg);

    bool bHasFacet(eSF eFacet);
    bool bHasFacet(Stz* szChrSeq);
    bool bHasFacets(void);
    bool bHasProperties(void);
    bool bHasType(void);

    void vInc_Tbl_Syn(void);
    void vInit_Tbl_Syn(char cTblSyn);
    bool bIs_Agg_Fn(void);          // avg, count, ... sum
    bool bIs_As(void);              // AS
    bool bIs_Bol_Fn(void);          // between, case ... matchTrailing
    bool bIs_Bol_Lit(void);         // FALSE, TRUE
    bool bIs_Bol_Op(void);          // and, not, or
    bool bIs_Bound(void);           // BOUND, LBOUND, UBOUND
    bool bIs_Brace(void);           // BRACE
    bool bIs_Case(void);            // CASE
    bool bIs_Cast(void);            // CAST
    bool bIs_CastType(void);        // CASTTYPE
    bool bIs_Col_Type(void);        // BOOLEAN, ... VARCHAR
    bool bIs_Comma(void);           // ,
    bool bIs_Cross(void);           // CROSS
    bool bIs_Dcm_Lit(void);         // StrLib::bIsDecimal()
    bool bIs_Direction(void);       // asc, desc
    bool bIs_Else(void);            // ELSE
    bool bIs_EndIf(void);           // ENDIF
    bool bIs_Exp_Col(void);         // $n
    bool bIs_Fld_Nm(void);          // bIsName() but not bIs_Agg_Fn() etc
    bool bIs_Flt_Lit(void);         // StrLib::bIsFloat()
    bool bIs_GroupBy(void);         // GROUPBY
    bool bIs_Idx_Col(void);         // $In
    bool bIs_Idx_Nm(void);          // Sn
    bool bIs_Int_Lit(void);         // StrLib::bIsInteger()
    bool bIs_Interval_DT(void);     // INTERVAL_DT
    bool bIs_Interval_YM(void);     // INTERVAL_YM
    bool bIs_Itm_Col(void);         // C_n
    bool bIs_Ivl_Fn(void);          // extract_year etc
    bool bIs_List_Cnt(void);        // #n
    bool bIs_Key_Col(void);         // $Kn
    bool bIs_Mth_Op(void);          // +, -, *, /
    bool bIs_Negate(void);          // NEGATE
    bool bIs_Nmd_Col(void);         // bIsName() but not bIs_Agg_Fn() etc
    bool bIs_None(void);            //
    bool bIs_Note(void);            // ...any...
    bool bIs_Null(void);            // NULL
    bool bIs_Parenthesis(void);     // ( or )
    bool bIs_Pay_Col(void);         // $Pn
    bool bIs_Ppe_Nm(void);          // bIsName() but not bIs_Agg_Fn() etc
    bool bIs_Rln_Op(void);          // <, <=, ... >
    bool bIs_Sqy_Nm(void);          // __sq%02lld
    bool bIs_Sqy_Op(void);          // exists, in, notExists, notIn
    bool bIs_Sqy_Prm(void);         // %n
    bool bIs_Sqy_Rsl(void);         // __n
    bool bIs_Stg_Cast(void);        // BROADCAST, HASHCAST
    bool bIs_Stg_Nm(void);          // Sn
    bool bIs_Str_Fn(void);          // SUBSTRING2 ... SUBSTRING3
    bool bIs_String(void);          // 'abc''
    bool bIs_Tbl_Col(void);         // $t.c
    bool bIs_Tbl_Nm(void);          // Sn
    bool bIs_Tbl_Syn(void);         // c
    bool bIs_Tbl_Syn_rep(void);     // C
    bool bIs_Then(void);            // THEN
    bool bIs_Type(void);            // TYPE
    bool bIs_When(void);            // WHEN
    bool bIs_Xk_Cast(void);         // XK_BROADCAST, XK_HASHCAST, XK_UNICAST

    bool bMatches(eSF eFacet_arg, Stz* szChrSeq);
    bool bMatches_ignoring(Symbol* pSymbol_arg, eSF* aeFacet_matched=nullptr, eSF eFacet_ignore=SF_NONE);
    bool bMatches(Symbol* pSymbol_arg, eSF eFacet);

    bool bNeedsBrace(void);         // between, case, ... matchTrailing

    void vPushFacet(eSF eFacet_arg, Stz* szChrSeq_arg);

    bool bSame(char cArg);
    bool bSame(Stz* szChrSeq);

    void vSetFacet(eSF eFacet_arg, Stz* szChrSeq_arg);
    char cSF_char(void);
    Stz* szSF_chrSeq(eSF eSymbol_xchk);
    i8   iSF_number(void);
    i8   iSF_ordinal(void);
    char cSF_tblSyn(void);

    TypeId xTypeId(void);
    i8     nTypeLength(void);
    bool   bTypeNullable(void);
    i8     nTypeScale(void);

    bool bValidate(void);
    void vValidate(void);

    Stz* szWhatIs(void);

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
    void vFacet_Pay_Col(Stz* szPayCol);
    void vFacet_Pay_Col(i8 iPayCol);
    void vFacet_Ppe_Nm(Stz* szPpeNm);
    void vFacet_Rln_Op(Stz* szRlnOp);
    void vFacet_Sqy_Nm(i8 iSqyNo);
    void vFacet_Sqy_Nm(Stz* szSqyNm);
    void vFacet_Sqy_Op(Stz* szSqyOp);
    void vFacet_Sqy_Prm(i8 iOrdinal);
    void vFacet_Sqy_Prm(Stz* szSqyPrm);
    void vFacet_Sqy_Rsl(i8 iOrdinal);
    void vFacet_Sqy_Rsl(Stz* szSqyRsl);
    void vFacet_Stg_Cast(Stz* szStgCast);
    void vFacet_Stg_Nm(i8 iStgNo);
    void vFacet_Stm_Col(i8 iStmCol);
    void vFacet_Str_Fn(Stz* szStrFn);
    void vFacet_String(Stz* szString);
    void vFacet_Tbl_Col(Stz* szTblCol);
    void vFacet_Tbl_Col(char cTblSyn, i8 iTblCol);
    void vFacet_Tbl_Nm(Stz* szTblNm);
    void vFacet_Tbl_Syn(Stz* szTblSyn);
    void vFacet_Tbl_Syn(char cTblSyn);
    void vFacet_Tbl_Syn_rep(void);
    void vFacet_Then(Stz* szThen);
    void vFacet_Type(Stz* szType);
    void vFacet_When(Stz* szWhen);
    void vFacet_Xk_Cast(Stz* szXkCast);

    void vFacetAppend(eSF eFacet, Stz* szFacet);
    void vFacetConcat(Symbol* pSymbol_arg);
    void vFacetCopy(Symbol* pSymbol_arg);
    void vFacetCut(i8 i_Facet_cut);
    void vFacetPush(eSF eFacet);
    void vFacetPush(eSF eFacet, Stz* szFacet);
    void vFacetReset(void);
    void vFacetRotate(eSF eFacet);
    void vFacetRotateSet(eSF eFacet, Stz* szFacet);
    void vFacetSet(i8 i_Facet, eSF eFacet, Stz* szFacet);
    void vFacetSprint(eSF eFacet, Stz* szFormat, ...);

  protected:
    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
