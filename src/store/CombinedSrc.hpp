/*============================================================================*/
/* CombinedSrc.hpp -- Combined Source                                         */
/*============================================================================*/
#pragma once

#include "TinyC.hpp"
#include "Tub.hpp"
#include "ePE.h"
#include "ePL.h"
#include "eEK.h"
#include "eFM.h"
#include "eKE.h"
#include "IndexDef.hpp"
#include "FunMatrix.h"
#include "OpSketch.hpp"

namespace STORE {

  static const i8 C_FLATTEN_SOURCE_V2 = C_48K; //C_SIZE_LARGE_OBJECT - C_1KB;

  class Stencil_v2;

  //----------------------------------------------------------------------------

  class CombinedSrc : public Object {
  private:

    File* m_pFileHeader;
    File* m_pFunctionHeader;
    File* m_pFunctionFooter;
    File* m_pCommonCode;

    char m_caCode[C_FLATTEN_SOURCE_V2];
    char *m_azCodeCrs;

    // must be last
    Footer m_xFooter;

  public:
    void *operator new(size_t);

    void operator delete(void *);

    CombinedSrc(void);
    ~CombinedSrc(void) noexcept(false);

    static void vMakeColumnNmStruct(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column);

    void vSynthesizeCode(Stencil_v2* pStencil, bool bFileHeader);

    char *azCode(void);

    void vPrint(void);

    void vReset(void);


  private:
    void vAppendCodeFragment(OpSketch* pOpSketch, Stz* szFunction, File* pCodeFile = nullptr);
    void vAppendCodeFragment(OpSketch* pOpSketch, Stz* szFunction, Column* pColumn, File* pCodeFile = nullptr);
    void vAppendCodeFragmentPL(OpSketch* pOpSketch, Stz* szFunction, Column* pColumn, i8 iPayLoad);
    void vAppendCodeFragmentKey(OpSketch* pOpSketch, Stz* szFunction, File* pCodeFile, i8 iKeyType);
    void vAppendCodeInflateCols(OpSketch* pOpSketch, Stz* szReturnPrefix, File* pCodeFile = nullptr);
    void vAppendCodeInflateRightCols(OpSketch* pOpSketch, Stz* szReturnPrefix, File* pCodeFile = nullptr);
    void vAppendCodeInflateRowCols(OpSketch* pOpSketch, Stz* szFunction, File* pCodeFile);
    void vAppendFileHeaders(void);
    void vAppendFunctionFooter(OpSketch* pOpSketch, eHK eHandlerKind);
    void vAppendFunctionHeader(OpSketch* pOpSketch, eHK eHandlerKind, Stz* szStageNm);
    void vAppendStencilTo(OpSketch* pOpSketch);
    void vAppendCodeCopyCols(OpSketch* pOpSketch, Stz* szFunction);
    void vAppendCodeNullRightCols(OpSketch* pOpSketch, Stz* szFunction);
    void vAppendCodeKeyToTubCols(OpSketch* pOpSketch);
    void vAppendCodePopulatePayloadCols(OpSketch* pOpSketch);
    void vAppendCodePayLoadToTubCols(OpSketch* pOpSketch);
    void vAppendCodePixCols(OpSketch* pOpSketch);
    void vAppendCodeIndexCols(OpSketch* pOpSketch);
    void vCloseCodeFiles(void);
    void vGenerateExprCode(TypedStack* pTypedStack, Stz* sIndent);
    bool bOpenCodeFiles(void);

    Footer *aFooter(void);
  };

  CombinedSrc *_pCombinedSrc(void *aAddr);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
