/*============================================================================*/
/* CmdLineArgs.hpp - command line arguments                                   */
/*============================================================================*/
#pragma once

#include "SysArgs.hpp"


namespace STORE
{
  static const i8 C_DEF_LVL_LEVELS = 3;
  static const i8 C_DEF_LVL_RATIO  = 3; // this means 1/2^3 i.e. reducing by 8

  class CmdLineArgs : public Object
  {
    friend class OBLIQUE::AM;
    friend class STORE::Engine;
    friend class BASE::Charser;
    friend class BASE::Input;

    friend class PROMPT::SePrompt;

  public:

    SysArgs m_xSysArgs;

  private:

    // must be last
    Footer m_xFooter;
    
  public:
    CmdLineArgs(void);
   ~CmdLineArgs(void) noexcept(false);
    
    static i8 nBurstArgs(s256 saArgv[], Stz* szArgStr);
  
    void vEnvironmentVariables(void);
    void vInit(void);
    bool bParse(i8 argc, s256 szaArgv[]);
    void vPrint(void);
    void vPrintStartupInfo(void);
    void vPrintHelp(void);
    void vSanityCheck(void);
    void vScribble(void);
  
    bool bUserPrompt(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
