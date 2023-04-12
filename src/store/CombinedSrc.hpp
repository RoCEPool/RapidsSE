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
#include "IndexDef.hpp"
#include "FunMatrix.h"
#include "OpSketch.hpp"

namespace STORE {

  static const i8 C_FLATTEN_SOURCE_V2 = C_SIZE_LARGE_OBJECT - C_1KB;

  class Stencil_v2;

  //----------------------------------------------------------------------------

  class CombinedSrc : public Object {
  private:

    File* m_pFileHeader;
    File* m_pFunctionHeader;
    File* m_pFunctionFooter;
    File* m_pFileCode;

    char m_caCode[C_FLATTEN_SOURCE_V2];
    char *m_azCodeCrs;

    // must be last
    Footer m_xFooter;

  public:
    void *operator new(size_t);

    void operator delete(void *);

    CombinedSrc(void);

    ~CombinedSrc(void) noexcept(false);

    void vSynthesizeCode(Stencil_v2* pStencil);

    char *azCode(void);

    void vPrint(void);

    void vReset(void);


  private:
    void vAppendCodeFragment(OpSketch* pOpSketch, Stz* szFunction, Column* pColumn = nullptr);
    void vAppendCodeInflateCols(OpSketch* pOpSketch, Stz* szFunction);
    void vAppendFileHeaders(void);
    void vAppendFunctionFooter(eHK eHandlerKind);
    void vAppendFunctionHeader(eHK eHandlerKind, Stz* szStageNm);
    void vAppendStencilTo(OpSketch* pOpSketch);
    void vAppendCodeTubCols(OpSketch* pOpSketch);
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
