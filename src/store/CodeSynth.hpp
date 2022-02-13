/*============================================================================*/
/* CodeSynth.hpp -- Code Synthesizer                                          */
/*============================================================================*/
#pragma once

#include "TinyC.hpp"
#include "Tub.hpp"
#include "eAG.h"
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

    TokenVec *m_pTokenVec;
    TypedStack *m_pExprStack;
    TypedStack *m_pPostStack;
    TypedStack *m_pLowerStack;
    TypedStack *m_pUpperStack;
    TypedStack *m_pInitStack;

    s256 m_sLowerPosition;
    s256 m_sUpperPosition;

    StreamDef *m_pStreamDefLft;
    StreamDef *m_pStreamDefOut;
    IndexDef  *m_pIndexDef;
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
              TypedStack *pPostStack, eSR eStencilFrom, eSR eStencilTo);

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

    bool bSynthAggGby(IndexDef *pIndexDef, IntStack *pAggTypeStack,
                      Stz *sProjectExpr, Stz *sLabel, Stz *sLabelLoop, bool bHashRows);

    void vSynthAllocTubRow(IntStack *pAggTypeStack, bool bEosHandler, Stz *sLabel, Stz *sLabelNext);

    void vSynthAllocPotRow(IntStack *pAggTypeStack, Stz *sHashExpr, bool bEosHandler,
                           Stz *sLabel, Stz *sLabelNext);

    void vSynthEosPayload(IndexDef *pIndexDef, IntStack *pAggTypeStack);

    void vSynthEosTop(IndexDef *pIndexDef);

    void vSynthFilter(Stz *sFilterExpr, Stz *sLabel);

    void vSynthFilterOrdAgg(Stz *sFilterExpr, Stz *sLabel, Stz *sLabelPrev);

    void vSynthIndexJoin(StreamDef *pStreamDefRht, IndexDef *pIndexDefJ,
                         Stz *sJoinExpr, Stz *sLabel, Stz *sLabelNext, u8 uSelectColumns);

    void vSynthIndexScan(IndexDef *pIndexDef, Stz *sIndexScanExpr, Stz *sLabel, Stz *sNextLabel, u8 uSelectColumns);

    void vSynthInflateCrw(Stz *sTableNm, Stz *sLabel, Stz *sReturnLabel, u8 *puSelectColumns);

    void vSynthInflateRow(Stz *sLabel);

    void vSynthLabel(Stz* szOpNm, Stz *sLabel, Stz* sNextLabel);

    void vSynthKeyExists(StreamDef *pStreamDefRht, IndexDef *pIndexDefJ,
                         Stz *sFilterExpr, Stz *sKeyExpr, Stz *sLabel, u8 uSelectColumns);

    void vSynthLimit(i8 iLimit, Stz *sLabel, Stz *sLabelNext, Stz *sLabelLoop);

    void vSynthOrdAgg(IndexDef *pIndexDef, eEK eExprKind, IntStack *pAggTypeStack,
                      Stz *sProjectExpr, Stz *sLabel, Stz *sLabelNext, Stz *sLabelLoop, bool bHashRows);

    void vSynthSmlAgg(IndexDef *pIndexDef, eEK eExprKind, IntStack *pAggTypeStack,
                      Stz *sProjectExpr, Stz *sLabel, Stz *sLabelLoop, bool bHashRows);

    bool bSynthPrjAgg(IntStack *pAggTypeStack, Stz *sProjectExpr, Stz *sLabel, Stz *sLabelLoop, bool bHashRows);

    bool bSynthPrjExp(IntStack *pAggTypeStack, Stz *sProjectExpr, Stz *sLabel, Stz *sLabelNext, Stz *sLabelLoop,
                      bool bHashRows);

    void vSynthStruct(u8 uSelectColumns);

    void vSynthStructArray(u8 uSelectColumns);

    void vSynthTop(IndexDef *pIndexDef, Stz *sKeyExpr, i8 iTopCount, Stz *sLabel, Stz *sLabelLoop);

  private:
    void vFindKey(IndexDef *pIndexDef, IntStack *pAggTypeStack);

    void vGenerateAggCode(IntStack *pAggTypeStack);

    void vGenerateAggInitCode(IntStack *pAggTypeStack);

    void vGenerateAssign(u8 uSelectColumns);

    void vGenerateFilter(void);

    void vGenerateGoto(Stz *sLabel);

    void vGenerateInflate(StreamDef *pStreamDef, Stz *szReturnPrefix, i8 nColumndBefore, u8 uSelectColumns);

    void vGenerateInitStub(Stz *szComment, Stz *sLabel);

    void vGenerateKeyExistsFilter(Stz *sExpr);

    void vGenerateProcessRow(Stz *sLabel);

    bool bGeneratePrjAgg(IntStack *pAggTypeStack);

    void vGeneratePrjExp(void);

    void vGeneratePrjGby(IndexDef *pIndexDef, IntStack *pAggTypeStack);

    void vGenerateStencilCast(void);
    void vGenerateStencilCastTo(void);
    void vGenerateErrorCheck(void);

    void vGenerateStruct(u8 uSelectColumns);

    i8 iGetValue(eDM eDatum, char *sValue);

    void vInitAgg(eAG eAggType, eDM eExprKind);

    void vOrdAggGby(IndexDef *pIndexDef, IntStack *pAggTypeStack, Stz *sLabelNext, Stz *sLabelLoop);

    void vPayloadAgg(IndexDef *pIndexDef, IntStack *pAggTypeStack);

    void vPayloadKey(IndexDef *pIndexDef, Stz *szEntWrk, Stz *szEntKey);

    void vPayloadStore(IndexDef *pIndexDef, IntStack *pAggTypeStack);

    void vPayloadToBuffer(IndexDef *pIndexDef, IntStack *pAggTypeStack, bool bTop);

    void vPayloadTraversal(IndexDef *pIndexDef, IntStack *pAggTypeStack);

    void vPushAggCol(TypedStack *pExprStack, eEK eExprKind, bool bHashRows, i8 i_Col, i8& i_DupCtr);
    void vPushAssignCol(TypedStack* pExprStack, IntStack* pTypeStack, eEK eExprKind, Token* pTkn, i8 i_Col, i8& i_DupCtr);
    void vPushArithmetic(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn);
    void vPushCast(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn, eDM eCastType);
    void vPushLiteral(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn, eDM eLiteral);
    void vPushLogical(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn);
    void vPushOrdCol(TypedStack *pExprStack, IntStack* pTypeStack, Token *pTkn);
    void vPushRelational(TypedStack *pExprStack, IntStack *pTypeStack, Token *pTkn);

    void vSmlAggLoad(IntStack *pAggTypeStack);

    void vSmlAggStore(IntStack *pAggTypeStack);

    void vGenerateExpr(char *sExpr, IntStack *pAggTypeStack,
                       eEK eExprKind, bool bHashRows, StreamDef *pStreamDef);

    void vToInfix(char *sExpr, IntStack *pAggTypeStack,
                  eEK eExprKind, bool bHashRows, StreamDef *pStreamDef);

    void vTopIndexTraversal(IndexDef *pIndexDef);

    Stz *szInfix(TypedStack *pExprStack, i8 iProject);

    void vGetInfix(s4K &sInfix, TypedStack *pExprStack, i8 iProject);

    Footer *aFooter(void);
  };

  CodeSynth *_pCodeSynth(void *aAddr);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
