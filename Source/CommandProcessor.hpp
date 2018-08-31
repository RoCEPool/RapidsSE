/*============================================================================*/
/* CommandProcessor.hpp - Command Processor                                   */
/*============================================================================*/
#pragma once

#include "Interface.h"

namespace Interface
{
  class CommandProcessor : public Object
  {
  private:
    Tokeniser* m_pTokeniser; // input stream tokeniser
    JobClass_o*  m_pJobClass_o;  // job class
    Job*       m_pJob;       // job
  
    TokenVec   m_xTokenVec2; // another token vector TO BE DELETED
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CommandProcessor(void);
    ~CommandProcessor(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    static bool bExecute(Stz* aCommand);
  
    bool bExecuteFile(void);

    void vPushFile(Stz* aPath);
    bool bRunFile(void);
    bool bTokenCheck(i8 nTokens);

    bool bBlankLine(void);
    void vDeleteJob(SEitf* pSEitf);
    bool bFindJobClass(void);
    void vGetCommand(void);
    void vGetCommandArgument(void);
    void vGetCommandInput(void);
    Job* pJob(void);
    JobClass_o* pJobClass(void);
    void vPostJob(SEitf* pSEitf);
    void vPreJob(SEitf* pSEitf);
    bool bSpawnJob(SEitf* pSEitf);
    bool bTestFile(Stz* aPath);

    Token* pTkn(i8 i_Token);
    TokenVec* pTknVec(void);
  
    void vTokenise(Stz* aCommand);
  
    Footer* aFooter(void);
  };
  
  CommandProcessor* _pCommandProcessor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
