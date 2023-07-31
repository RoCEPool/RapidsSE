/*============================================================================*/
/* Symbol.hpp -- Abstract Symbol                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Ppy.hpp"
#include "eSF.h"

namespace STORE
{
  class Symbol : public Object
  {
  public:
    eSF     m_eSymbol;
    s256    m_sSymbol;
  public:
    TagSet* m_pProperties;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    Symbol(void);
    Symbol(Stz* szSymbol, eSF eSymbol);
    Symbol(Symbol* pSymbol);

    static eSF  eEvalForm(Stz* szArg);
    static eRO  eEvalRelnOp(Stz* szArg);


    static bool bIsAggOp(Stz* szArg);            // avg, count, ... sum
    static bool b_IsSubQueryName(Stz* szStz);  // __subqueryN
    static bool bIsBoolFn(Stz* szArg);           // between, case ... matchTrailing
    static bool bIsBound(Stz* szArg);            // LBOUND, UBOUND
    static bool bIsColNm(Stz* szArg);            //
    static bool bIsColumnOrdinal(Stz* szArg);    // $c.n
    static bool bIsComma(Stz* szArg);            // ,
    static bool bIsCrossRef(Stz* szArg);         // #n
    static bool bIsDisambiguatedColNm(Stz* szArg);  // C_n
    static bool bIsIndexColumn(Stz* szArg);      // $In
    static bool bIsKeyOrdinal(Stz* szArg);       // $Kn
    static bool bIsName(Stz* szArg);             //
    static bool bIsNumber(Stz* szArg);           //
    static bool bIsNumbOp(Stz* szArg);           //
    static bool bIsOther(Stz* szArg);            //
    static bool bIsParameterOrdinal(Stz* szArg); // %n
    static bool bIsParenthesis(Stz* szArg);      // ( or )
    static bool bIsRelnOp(Stz* szArg);           // <, <=, ... >
    static bool bIsStreamOrdinal(Stz* szArg);    // $n
    static bool bIsSubQueryOp(Stz* szArg);       // exists, in, notExists, notIn
    static bool bIsSubQueryColumn(Stz* szArg);   // __n
    static bool bIsSyntheticAs(Stz* szArg);      // __n
    static bool bIsTableSyn(Stz* szArg);         // $c
    static bool bIsValueOrdinal(Stz* szArg);     // $n or $c.n

    static eCT  eTo_eCT_str(Stz* szType);
    static eCT  eTo_eCT_char(Stz* szType);

    // - - - - - -

    eCT  eColType(void);

    eRO  eEvalRelnOp(void);
    eQO  eEvalSubQryOp(void);
    i8   iExtractOrdinal(void);
    char cExtractTableSynonym(void);

    bool bHasSymbolicValue(void);

    bool bIsAggOp(void);            // avg, count, ... sum
    bool bIsBoolFn(void);           // between, case ... matchTrailing
    bool bIsBound(void);            // LBOUND, UBOUND
    bool bIsColNm(void);            //
    bool bIsColumnOrdinal(void);    // $c.n
    bool bIsComma(void);            // ,
    bool bIsCrossRef(void);         // #n
    bool bIsDisambiguatedColNm(void);  // C_n
    bool bIsIndexColumn(void);      // $In
    bool bIsKeyOrdinal(void);       // $Kn
    bool bIsName(void);             //
    bool bIsNumber(void);           //
    bool bIsNumbOp(void);           //
    bool bIsOther(void);            //
    bool bIsParameterOrdinal(void); // %n
    bool bIsParenthesis(void);      // ( or )
    bool bIsRelnOp(void);           // <, <=, ... >
    bool bIsStreamOrdinal(void);    // $n
    bool bIsSubQueryOp(void);       // exists, in, notExists, notIn
    bool bIsSubQueryColumn(void);   // __n
    bool bIsSyntheticAs(void);      // __n
    bool bIsTableSyn(void);         // c
    bool bIsValueOrdinal(void);     // $n or $c.n

    bool bNeedsBrace(void);         // between, case, ... matchTrailing

    void vPpyStr(String* pString);


    bool bSame(char cArg);
    bool bSame(Stz* szArg);
    bool bSame(Symbol* pArg);

    void vSet(Symbol* pSymbol);
    void vSet(eSF eSymbol, Stz* szSymbol);
    void vSet_asName(Stz* szAsNm);
    void vSet_colNm(Stz* szColNm);
    void vSetCrossRef(i8 iCrossRef);
    void vSetDisambiguatedColNm(i8 i_Child, Stz* szColNm);
    void vSetIndexColOrdinal(i8 iIdxColOdn);
    void vSetKeyOrdinal(i8 iKeyOdn);
    void vSet_number(i8 iNumber);
    void vSet_other(Stz* szOther);
    void vSet_parameterOrdinal(i8 iOrdinal);
    void vSet_stageNo(i8 iStageNo);
    void vSetStreamOrdinal(i8 iStmColOdn);
    void vSetSubQueryOrdinal(i8 iSqNo);
    void vSetSyntheticAs(i8 iAsNo);
    void vSetTableColOrdinal(char cTblSyn, i8 iTblColOdn);
    void vSetTableColSyn(char cTblSyn);

    Stz* szSF__print(void);
    Stz* szSF_any(void);
    char cSF_char(void);
    i8   iSF_number(void);
    Stz* szSF_other(void);
    Stz* szSF_string(void);


    bool bTest_mustHave_ppy(Stz* szPpy, ...);

    Stz* szType(void);
    void vType(Stz* szType);

    Stz* szWhatIs(void);

    void Y_vAll(Card* pCard_arg);
    void Y_vAll(TagSet* pProperties);
    void Y_vCut(Stz* szPpyNm);
//    char Y_cGet(i8 i_Ppy);
//    char Y_cGet(Stz* szPptNm);
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

  private:
    void vSprint(Stz* szFormat, ...);

  protected:
    Footer* aFooter(void);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
