/*============================================================================*/
/* CodeSynth.hpp -- Code Synthesizer                                          */
/*============================================================================*/
#pragma once

#include "TinyC.hpp"
#include "Tub.hpp"
#include "eAG.h"
#include "ePJ.h"
#include "IndexDef.hpp"

namespace STORE
{
  static const i8 C_FLATTEN_SOURCE  = C_SIZE_LARGE_OBJECT - C_1KB;

  //----------------------------------------------------------------------------

  class CodeSynth : public Object
  {
  private:
  
    char        m_caCode[C_FLATTEN_SOURCE];
    char*       m_azCodeCrs;

    TokenVec*   m_pTokenVec;
    TypedStack* m_pExprStack;
    TypedStack* m_pAggrStack;
    TypedStack* m_pLowerStack;
    TypedStack* m_pUpperStack;
    TypedStack* m_pPostExpr;
    TypedStack* m_pInitStack;

    s256        m_sLowerPosition;
    s256        m_sUpperPosition;

    StreamDef*  m_pStreamDefLft;
    StreamDef*  m_pStreamDefOut;
    eSR         m_eStencilFrom;
    eSR         m_eStencilTo;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CodeSynth(void);
    CodeSynth(StreamDef* pStreamDefOut, eSR eStencilTo);
    CodeSynth(StreamDef* pStreamDefLeft, StreamDef* pStreamDefOut, eSR eStencilFrom, eSR eStencilTo);
   ~CodeSynth(void) noexcept(false);

    static void vFormColumn(Stz* szColumn, StreamDef* pStreamDef, eSR eStencilRow, i8 i_Column);

    void  vAggrStack(TypedStack* pAggrStack);
    TypedStack* pAggrStack(void);

    void  vAppendFragment(Stz* szFragment);
    void  vAppendFragment(Stz* szFragment, i8* piLineNumber);
    void  vAppendFragmentSemiColon(Stz* szFragment);
    void  vAppendFragment(CodeSynth* pFragment);
    void  vAppendFragment(CodeSynth* pFragment, i8* piLineNumber);

    char* azCode(void);
    void  vPrint(void);
    void  vReset(void);

    void  vSynthAllocTubRow(IntStack* pAggTypeStack, ePJ eProjectKind, bool bEosHandler,
                            Stz* sLabel, Stz* sLabelNext, Stz* sReturnLabel);
    void  vSynthAllocPotRow(IntStack* pAggTypeStack, Stz* sHashExpr, ePJ eProjectKind, bool bEosHandler,
                            Stz* sLabel, Stz* sLabelNext, Stz* sReturnLabel);
    void  vSynthEos(IndexDef* pIndexDef, IntStack* pAggTypeStack);
    void  vSynthFilter(Stz* sFilterExpr, Stz* sLabel, Stz* sLabelNext);
    void  vSynthFilterOrdAgg(Stz* sFilterExpr, Stz* sLabel, Stz* sLabelPrev);
    void  vSynthIndexJoin(StreamDef* pStreamDefRht, IndexDef* pIndexDefJ,
                          Stz* sJoinExpr, Stz* sLabel, Stz* sLabelNext, u8 uSelectColumns);
    void  vSynthIndexScan(IndexDef* pIndexDef, Stz* sIndexScanExpr, Stz* sNextLabel, u8 uSelectColumns);
    void  vSynthInflate(Stz* sTableNm, Stz* sLabel, Stz* sReturnLabel, u8* puSelectColumns);
    void  vSynthKeyExists(StreamDef* pStreamDefRht, IndexDef* pIndexDefJ,
                          Stz* sFilterExpr, Stz* sKeyExpr, Stz* sLabel, Stz* sLabelNext, u8 uSelectColumns);
    void  vSynthOrdAgg(IndexDef* pIndexDef, ePJ eProjectKind, IntStack* pAggTypeStack,
                       Stz* sProjectExpr, Stz* sLabel, Stz* sLabelNext, Stz* sLabelLoop, bool bHashRows);
    void  vSynthSmlAgg(IndexDef* pIndexDef, ePJ eProjectKind, IntStack* pAggTypeStack,
                       Stz* sProjectExpr, Stz* sLabel, Stz* sLabelLoop, bool bHashRows);
    bool  bSynthProject(IndexDef* pIndexDef,ePJ eProjectKind, IntStack* pAggTypeStack,
                        Stz* sProjectExpr, Stz* sLabel, Stz* sLabelNext, Stz* sLabelLoop, bool bHashRows);
    void  vSynthRowInflate(void);
    void  vSynthStruct(u8 uSelectColumns);
    void  vSynthStructArray(u8 uSelectColumns);

  private:
    void vFindKey(IndexDef* pIndexDef, IntStack* pAggTypeStack);
    void vGenerateAggInitCode(IntStack* pAggTypeStack);
    void vGenerateAssign(u8 uSelectColumns);
    void vGenerateInflate(StreamDef *pStreamDef, Stz* szReturnPrefix, i8 nColumndBefore, u8 uSelectColumns);
    i8   iGetValue(eDM eDatum, char* sValue);
    void vInitAgg(eAG eAggType, eDM eExprType);
    void vOrdAggGby(IndexDef* pIndexDef, IntStack* pAggTypeStack, Stz* sLabelNext, Stz* sLabelLoop);
    void vPayloadAgg(IndexDef* pIndexDef, IntStack* pAggTypeStack);
    void vPayloadKey(IndexDef* pIndexDef, eSR eStencilFrom);
    void vPayloadStore(IndexDef* pIndexDef, IntStack* pAggTypeStack);
    void vPayloadToBuffer(IndexDef* pIndexDef, IntStack* pAggTypeStack);
    void vPayloadTraversal(IndexDef* pIndexDef, IntStack* pAggTypeStack);
    bool bProjectAgg(IntStack* pAggTypeStack);
    void vProjectExp(void);
    void vProjectGby(IndexDef* pIndexDef, IntStack* pAggTypeStack);
    void vSmlAggLoad(IntStack* pAggTypeStack);
    void vSmlAggStore(IntStack* pAggTypeStack);
    void vToInfix(char* sExpr, IntStack* pAggTypeStack, ePJ eProjectKind, bool bHashRows);

    Footer* aFooter(void);
  };
  
  CodeSynth* _pCodeSynth(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
