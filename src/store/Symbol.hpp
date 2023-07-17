/*============================================================================*/
/* Symbol.hpp -- Abstract Symbol                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Ppy.hpp"

namespace STORE
{
  class Symbol : public Object
  {
  public:
    s256    m_sSymbol;
    TagSet* m_pProperties;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    Symbol(void);
    Symbol(Stz* szSymbol);

    void operator =(i8 iArg);
    void operator =(Stz* szArg);
    void operator =(Symbol* pSymbol);
    void operator =(Symbol& rxSymbol);

    // auto conversion
    operator Stz*(void);

    static bool b_IsSubQueryName(Stz* szStz);  // __subqueryN
    static eCT  eTo_eCT_str(Stz* szType);
    static eCT  eTo_eCT_char(Stz* szType);

    eCT  eColType(void);

    eRO  eEvalRelnOp(void);
    eQO  eEvalSubQryOp(void);
    i8   iExtractOrdinal(void);
    char cExtractTableSynonym(void);

    bool bIsAggOp(void);            // avg, count, ... sum
    bool bIsBoolFn(void);           // between, case ... matchTrailing
    bool bIsBound(void);            // LBOUND, UBOUND
    bool bIsColNm(void);            //
    bool bIsColumnOrdinal(void);    // $c.n
    bool bIsComma(void);            // ,
    bool bIsCrossRef(void);         // #n
    bool bIsIndexColumn(void);      // $In
    bool bIsKeyOrdinal(void);       // $Kn
    bool bIsName(void);             //
    bool bIsNumber(void);           //
    bool bIsNumbOp(void);           //
    bool bIsParameterOrdinal(void); // %n
    bool bIsParenthesis(void);      // ( or )
    bool bIsRelnOp(void);           // <, <=, ... >
    bool bIsStreamOrdinal(void);    // $n
    bool bIsSubQueryOp(void);       // exists, in, notExists, notIn
    bool bIsSubQueryColumn(void);   // __n
    bool bIsSyntheticAs(void);      // __n
    bool bIsValueOrdinal(void);     // $n or $c.n

    bool bNeedsBrace(void);         // between, case, ... matchTrailing

    void vPpyStr(String* pString);


    bool bSameAs(char cArg);
    bool bSameAs(Stz* szArg);
    bool bSameAs(Symbol* pArg);

    void vSet(Symbol* pSymbol);
    void vSetCrossRef(i8 iCrossRef);
    void vSetIndexColOrdinal(i8 iIdxColOdn);
    void vSetKeyOrdinal(i8 iKeyOdn);
    void vSetStreamOrdinal(i8 iStmColOdn);
    void vSetSubQueryOrdinal(i8 iSqNo);
    void vSetSyntheticAs(i8 iAsNo);
    void vSetTableColOrdinal(char cTblSyn, i8 iTblColOdn);
    void vSetTableColSyn(char cTblSyn);
    void vSprint(Stz* szFormat, ...);

    Stz* szSymbol(void);

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

  protected:
    Footer* aFooter(void);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
