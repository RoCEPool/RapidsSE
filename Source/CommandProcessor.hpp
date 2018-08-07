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
    JobClass*  m_pJobClass;  // job class
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
  
    bool bExecuteFile(void);

    void vPushFile(Stz* aPath);
    bool bRunFile(void);
    bool bTokenCheck(i8 nTokens);

    bool bBlankLine(void);
    void vDeleteJob(SEitf* pSEitf);
    bool bDispatchJob(SEitf* pSEitf);
    void vEndJob(SEitf* pSEitf);
    bool bFindJobClass(void);
    void vGetCommand(void);
    void vGetCommandArgument(void);
    void vGetCommandInput(void);
    Job* pJob(void);
    JobClass* pJobClass(void);
    void vNewJob(SEitf* pSEitf);
    bool bTestFile(Stz* aPath);

    Token* pTkn(i8 i_Token);
    TokenVec* pTknVec(void);
    TokenVec* pTknVec2(void);
  
    Footer* aFooter(void);
  };
  
  CommandProcessor* _pCommandProcessor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
