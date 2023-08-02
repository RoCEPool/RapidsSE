/*============================================================================*/
/* Symbol.hpp -- Abstract Symbol                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Ppy.hpp"
#include "eQO.h"
#include "eRO.h"
#include "eSF.h"

namespace STORE
{
  class Symbol : public Object
  {
  private:
    eSF     m_eSymbol;
    s256    m_sSymbol;
    TagSet* m_pProperties;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    Symbol(void);
    Symbol(Stz* szSymbol, eSF eSymbol);
    Symbol(Symbol* pSymbol);
    ~Symbol(void);

    static eRO  eEvalRelnOp(Stz* szSymbol);
    static eQO  eEvalSubQryOp(Stz* szSymbol);
    static eSF  eEvalSymbol(Stz* szSymbol);

    static bool bIsAggFn(Stz* szArg);            // avg, count, ... sum
    static bool bIsBoolFn(Stz* szArg);           // between, case ... matchTrailing
    static bool bIsBoolOp(Stz* szArg);           // and, not, or
    static bool bIsBound(Stz* szArg);            // BOUND, LBOUND, UBOUND
    static bool bIsColNm(Stz* szArg);            // bIsName() but not bIsAggFn() etc
    static bool bIsColumnOrdinal(Stz* szArg);    // $c.n
    static bool bIsComma(Stz* szArg);            // ,
    static bool bIsCrossRef(Stz* szArg);         // #n
    static bool bIsDirection(Stz* szArg);        // asc, desc
    static bool bIsDisambColNm(Stz* szArg);      // C_n
    static bool bIsIndexColumn(Stz* szArg);      // $In
    static bool bIsKeyOrdinal(Stz* szArg);       // $Kn
    static bool bIsMathOp(Stz* szArg);           // +, -, *, /
    static bool bIsName(Stz* szArg);             // StrLib::bIsSqlName()
    static bool bIsNumber(Stz* szArg);           // StrLib::bIsDecimal/Float/Integer()
    static bool bIsOther(Stz* szArg);            //
    static bool bIsParameterOrdinal(Stz* szArg); // %n
    static bool bIsParenthesis(Stz* szArg);      // ( or )
    static bool bIsRelnOp(Stz* szArg);           // <, <=, ... >
    static bool bIsStreamOrdinal(Stz* szArg);    // $n
    static bool bIsSubQueryCKey(Stz* szStz);     // _SQCRLKEY_
    static bool bIsSubQueryName(Stz* szStz);     // __subqueryN
    static bool bIsSubQueryOp(Stz* szArg);       // exists, in, notExists, notIn
    static bool bIsSubQueryColumn(Stz* szArg);   // __n
    static bool bIsSyntheticAs(Stz* szArg);      // __COLn
    static bool bIsTableSyn(Stz* szArg);         // $c
    static bool bIsValueOrdinal(Stz* szArg);     // $n or $c.n

    static void vStripDisamb(String* psArg);

    static eCT  eTo_eCT_str(Stz* szType);
    static eCT  eTo_eCT_char(Stz* szType);

    // - - - - - -

    eCT  eColType(void);
    Stz* szColType(void);
    void vColType(Stz* szColType);


    eRO  eEvalRelnOp(void);
    eQO  eEvalSubQryOp(void);
    i8   iExtractOrdinal(void);
    char cExtractTableSynonym(void);

    bool bHasSymbolicValue(void);
    bool bHasProperties(void);

    void vIncTblSyn(void);

    bool bIsAggFn(void);            // avg, count, ... sum
    bool bIsBoolFn(void);           // between, case ... matchTrailing
    bool bIsBoolOp(void);           // and, not, or
    bool bIsBound(void);            // BOUND, LBOUND, UBOUND
    bool bIsColNm(void);            // bIsName() but not bIsAggFn() etc
    bool bIsColumnOrdinal(void);    // $c.n
    bool bIsComma(void);            // ,
    bool bIsCrossRef(void);         // #n
    bool bIsDirection(void);        // asc, desc
    bool bIsDisambColNm(void);      // C_n
    bool bIsIndexColumn(void);      // $In
    bool bIsKeyOrdinal(void);       // $Kn
    bool bIsMathOp(void);           // +, -, *, /
    bool bIsName(void);             // StrLib::bIsSqlName()
    bool bIsNumber(void);           // StrLib::bIsDecimal/Float/Integer()
    bool bIsOther(void);            //
    bool bIsParameterOrdinal(void); // %n
    bool bIsParenthesis(void);      // ( or )
    bool bIsRelnOp(void);           // <, <=, ... >
    bool bIsStreamOrdinal(void);    // $n
    bool bIsSubQueryOp(void);       // exists, in, notExists, notIn
    bool bIsSubQueryColumn(void);   // __n
    bool bIsSyntheticAs(void);      // __COLn
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
    void vSetTableSyn(char cTblSyn);

    Stz* szSF__print(void);
    Stz* szSF_any(void);
    char cSF_char(void);
    i8   iSF_number(void);
    Stz* szSF_other(void);
    Stz* szSF_string(void);

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

  private:
    void vSprint(Stz* szFormat, ...);

  protected:
    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
