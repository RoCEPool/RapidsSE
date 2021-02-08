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
  
    static Job*       pJob(void);
    static LogicUnit* pLogicUnit(void);
    static Session*   pSession(void);
    static Task*      pTask(void);
    static Thread*    pThread(void);

    // tbd...
    static Grid*     pGrid(void);
    static Node*     pNode(void);
    static Op*       pOp(void);
    static eZK       eOperand(void);
    static Pipeline* pPipeline(void);
    static Stage*    pStage(void);
    static Stream*   pStream(void);

    /*
     
     Possible Other Functions:
         
     pTenant()
     pCatalog()
     pSchema()
     
     pDic()
     pDicStack()
     
     szPath();

     */
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
