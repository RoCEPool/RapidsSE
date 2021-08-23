/*============================================================================*/
/* MY.hpp -- My helper                                                        */
/*============================================================================*/
#pragma once

//!                    ** Do not include BASE.h **

#include "eZK.h"

namespace BASE
{
  class Task;
}

namespace STORE
{
  class Job;
  class Session;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace OBLIQUE
{
  class MY
  {
  public:
    static i8 nNanoseconds(void);
    static i8 iPtn(void);
  
    static Stz* szCallerNm(void);
    static Stz* szCatalogNm(void);
    static Stz* szGridNm(void);
    static Stz* szHostNm(void);
    static Stz* szJobNm(void);
    static Stz* szNodeNm(void);
    static Stz* szOpNm(void);
    static Stz* szPipelineNm(void);
    static Stz* szSchemaNm(void);
    static Stz* szTableNm(void);
    static Stz* szTaskNm(void);
    static Stz* szTenantNm(void);
    static Stz* szThreadNm(void);
    static Stz* szWordNm(void);
  
    static Dic*       pCatalogDic(void);
    static Dic*       pDic(void);
    static DicStack*  pDicStack(void);
    static Grid*      pGrid(void);
    static Job*       pJob(void);
    static LogicUnit* pLogicUnit(void);
    static Node*      pNode(void);
    static Op*        pOp(void);
    static eZK        eOperand(void);
    static Pipeline*  pPipeline(void);
    static Dic*       pSchemaDic(void);
    static Session*   pSession(void);
    static Stage*     pStage(void);
    static Stream*    pStream(void);
    static Dic*       pTableDic(void);
    static Task*      pTask(void);
    static Dic*       pTenantDic(void);
    static Thread*    pThread(void);
  
    static Stz*       szPath(Stz* szPathBuffer);
    static void       vPath(Stz* szPathBuffer);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
