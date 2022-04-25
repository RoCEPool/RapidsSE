/*============================================================================*/
/* CodeSynth.hpp -- Code Synthesizer                                          */
/*============================================================================*/
#pragma once

#include "TinyC.hpp"
#include "Tub.hpp"
#include "ePE.h"
#include "ePL.h"
#include "eEK.h"
#include "eFM.h"
#include "IndexDef.hpp"
#include "FunMatrix.h"

namespace STORE {

  static const i8 C_FLATTEN_SOURCE = C_SIZE_LARGE_OBJECT - C_1KB;

  //----------------------------------------------------------------------------

  class CodeSynth : public Object {
  private:

    char m_caCode[C_FLATTEN_SOURCE];
    char *m_azCodeCrs;

    TokenVec*   m_pTokenVec;
    TypedStack* m_pExprStack;
    TypedStack* m_pNullStack;
    TypedStack* m_pGbyStack;
    TypedStack* m_pLowerStack;
    TypedStack* m_pUpperStack;
    TypedStack* m_pInitStack;

    s256 m_sLowerPosition;
    s256 m_sUpperPosition;

    IntStack*  m_pPreExpStack;
    StreamDef* m_pStreamDefLft;
    StreamDef* m_pStreamDefOut;
    IndexDef*  m_pIndexDef;
    eSR m_eStencilFrom;
    eSR m_eStencilTo;

    i8 m_iLimitRows;

    // must be last
    Footer m_xFooter;

  public:
    void *operator new(size_t);

    void operator delete(void *);

    CodeSynth(void);

    CodeSynth(StreamDef *pStreamDefOut, eSR eStencilTo);

    CodeSynth(StreamDef *pStreamDefLeft, StreamDef *pStreamDefOut, IndexDef* pIndexDef,
              IntStack* pPreExpStack, eSR eStencilFrom, eSR eStencilTo);

    ~CodeSynth(void) noexcept(false);

    static void vMakeColumnNm(StreamDef *pStreamDef, Stz *szColumn, eSR eStencilRow, i8 i_Column);
    static void vMakeColumnNm(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column);
    static void vMakeColumnNmStruct(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column);
    static void vMakeColumnNmStruct_O(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column);
    static void vMakeIdxColNmStruct(IndexDef* pIndexDef, s256& sColNm, i8 i_Column);
    static void vMakeIdxColNmStruct_O(IndexDef* pIndexDef, s256& sColNm, i8 i_Column);
    void vPostStack(TypedStack *pPostStack);

    TypedStack *pPostStack(void);

    void vAppendFragment(Stz *szFragment);

    void vAppendFragment(Stz *szFragment, i8 *piLineNumber);

    void vAppendFragment(CodeSynth *pFragment);

    void vAppendFragment(CodeSynth *pFragment, i8 *piLineNumber);

    char *azCode(void);

    void vPrint(void);

    void vReset(void);

    TypedStack* pSynthAggGby(Stz *sProjectExpr, Stz *sLabel, Stz *sLabelLoop);

    void vSynthAllocTubRow(TypedStack* pPostStack, bool bEosHandler, Stz *sLabel, Stz *sLabelNext,
                           Stz* sLabelOrdAgg);

    void vSynthAllocPotRow(TypedStack* pPostStack, Stz *sHashExpr, bool bEosHandler,
                           Stz *sLabel, Stz *sLabelNext, Stz* sLabelOrdAgg);

    void vSynthEos(IndexDef *pIndexDef, TypedStack* pPostStack, Stz* sLabelNext);

    void vSynthFilter(Stz *sFilterExpr, bool bOrdAgg, Stz *sLabel, Stz* sLabelPrev, Stz* sLabelLoop);

    void vSynthIndexJoin(StreamDef *pStreamDefRht, IndexDef *pIndexDefJ,
                         Stz *sJoinExpr, Stz *sLabel, Stz *sLabelNext, u8 uSelectColumns);

    void vSynthIndexScan(Stz *sIndexScanExpr, Stz *sLabel, Stz *sLabelNext, Stz* sLabelOrdAgg, u8 uSelectColumns);

    void vSynthInflateCrw(Stz *sTableNm, Stz *sLabel, Stz *sReturnLabel, Stz* sLabelOrdAgg, u8 *puSelectColumns);

    void vSynthInflateRow(Stz *sLabel, Stz* sLabelOrdAgg);

    void vSynthLabel(Stz* szOpNm, Stz *sLabel, Stz* sNextLabel);

    void vSynthKeyExists(StreamDef *pStreamDefRht, IndexDef *pIndexDefJ,
                         Stz *sFilterExpr, Stz *sKeyExpr, Stz *sLabel, u8 uSelectColumns);

    void vSynthLimit(i8 iLimit, Stz *sLabel, Stz *sLabelNext, Stz *sLabelLoop);

    void vSynthOrdAgg(Stz *sProjectExpr, Stz *sLabel, Stz *sLabelNext, Stz *sLabelLoop);

    TypedStack* pSynthPrjAgg(Stz *sProjectExpr, Stz *sLabel, Stz *sLabelLoop);

    bool bSynthPrjExp(Stz *sProjectExpr, Stz *sLabel, Stz *sLabelNext, Stz *sLabelLoop);

    void vSynthReturn(void);

    void vSynthSort( Stz *sProjectExpr, Stz *sLabel, Stz *sLabelNext);

    void vSynthStruct(u8 uSelectColumns, IndexDef* pIndexDef = nullptr);

    void vSynthStructArray(u8 uSelectColumns);

    void vSynthTop(Stz *sKeyExpr, i8 iTopCount, Stz *sLabel, Stz *sLabelLoop);

  private:
    void vFindKey(IndexDef *pIndexDef, ePL ePayload);

    void vGenerateAssign(u8 uSelectColumns);

    void vGenerateExprCode(TypedStack* pTypedStack, Stz* sIndent);

    void vGenerateGoto(Stz *sLabel);

    void vGenerateInflate(StreamDef *pStreamDef, Stz *szReturnPrefix, i8 nColumndBefore, u8 uSelectColumns);

    void vGenerateInitStub(Stz *szComment, Stz *sLabel);

    void vGenerateInitVars(void);

    void vGenerateKeyExistsFilter(StreamDef* pStreamDef, Stz *sExpr);

    void vGenerateProcessRow(Stz *sLabel);
    void vGenerateProcessRowOrdAgg(Stz *sLabel);

    void vGeneratePrjAgg(void);

    void vGeneratePrjExp(void);

    void vGeneratePrjGby(void);

    void vGenerateSort(void);

    void vGenerateStencilCast(void);
    void vGenerateStencilCastTo(void);
    void vGenerateErrorCheck(Stz* sIndent);

    void vGenerateStruct(u8 uSelectColumns, IndexDef* pIndexDef = nullptr);

    void vGetValue(eDM eDatum, char *sValue, i8& iValue, d8& dValue);

    void vInitAgg(ePE ePreExp, eDM eExprKind);

    void vOrdAggGby(Stz *sLabelNext, Stz *sLabelLoop);

    void vPayloadAgg(void);

    void vPayloadCreate(void);
    void vPayloadInsert(ePL ePayload, i8 iTopCount);
    void vPayloadKey(TypedStack* pExprStack);
    void vPayloadPopulate(ePL ePayload);
    void vPayloadStore(IndexDef *pIndexDef);
    void vPayloadToBuffer(IndexDef *pIndexDef, TypedStack* pPostStack);
    void vPayloadTraversal(IndexDef *pIndexDef, TypedStack* pPostStack, Stz* sLabelNext);

    void vPushAggCol(TypedStack *pExprStack, TypedStack* pPostStack, ePE ePreExp, eEK eExprKind, eDM eExprType, i8 i_Col, i8& i_DupCtr);
    void vPushAssignCol(TypedStack* pExprStack, TypedStack* pPostStack, eEK eExprKind,
                        ePE ePreExp, i8 i_Col, i8& i_DupCtr);
    void vPushArithmetic(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn);
    void vPushCast(TypedStack *pExprStack, IntStack *pTypeStack, eDM eCastType);
    void vPushColumn(TypedStack *pExprStack, IntStack *pTypeStack, StreamDef* pStreamDef, Token *pTkn);
    void vPushCount(TypedStack *pExprStack, IntStack *pExprType, Token *pTkn);
    void vPushFunc(TypedStack* pExprStack, IntStack* pTypeStack, ePE ePreExp);
    void vPushLiteral(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn, ePE ePreExp, eEK eExprKind, eDM eLiteral);
    void vPushLogical(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn);
    void vPushOrdCol(TypedStack *pExprStack, IntStack* pTypeStack, ePE ePreExp, eEK eExprKind, Token *pTkn);
    void vPushRelational(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn);

    void vGenerateExpr(char *sExpr, TypedStack* pPostStack, eEK eExprKind, StreamDef *pStreamDef);
    void vGenerateInExpr(void);

    Stz *szInfix(TypedStack *pExprStack, i8 iProject);

    void vGetInfix(s4K &sInfix, TypedStack *pExprStack, i8 iProject);
    TypedStack* pWorkingStack(eEK eExprKind, ePE ePreExp, TypedStack* pPostStack);

    Footer *aFooter(void);
  };

  CodeSynth *_pCodeSynth(void *aAddr);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
