/*============================================================================*/
/* MY.hpp -- My helper                                                        */
/*============================================================================*/
#pragma once

//!                    ** Do not include BASE.h **

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
    static Stz* szJobNm(void);
    static Stz* szGridNm(void);
    static Stz* szWordNm(void);
  
    static Job*     pJob(void);
    static Session* pSession(void);
    static Task*    pTask(void);
  
    /*
     
     Possible Other Functions:
    
     pGrid()
     pNode()
     
     pTenant()
     pCatalog()
     pSchema()
     
     pDic()
     pDicStack()
     
     pPipeline()
     pStage()
     pStrand()
     pOp()
     
     szPath();

     */
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
