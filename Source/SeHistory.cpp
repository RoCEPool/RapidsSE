/*============================================================================*/
/* SeHistory.cpp -- SePrompt History                                          */
/*============================================================================*/

#include "SeHistory.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SeHistory::SeHistory(void)
: Object(CN_SEHISTORY)
{
  //m_xFile
  m_iCommands = 0;
  
  //m_xInputStream
  m_pTokeniser = new Tokeniser;
  
  m_pTokeniser->vPush();
  
  _checkInitialised_(CN_SEHISTORY);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SeHistory::~SeHistory(void)
{
  vClose();
  
  delete m_pTokeniser;
}

/*----------------------------------------------------------------------------*/
/* append command to history file                                             */
/*----------------------------------------------------------------------------*/
void SeHistory::vAppend(Str* aToken[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  vSeekEnd();
  
  for (i8 i = 0; *aToken[i]->a(); i++) {
    if (i)
      m_xFile.vPrint("%s"_Str," ");
    m_xFile.vPrint("%s"_Str,aToken[i]->a());
  }
  m_xFile.vPrint("%s"_Str,"\n");
  m_xFile.vFlush();
  
  m_iCommands++;
}

/*----------------------------------------------------------------------------*/
/* close command history file                                                 */
/*----------------------------------------------------------------------------*/
void SeHistory::vClose(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* open command history file                                                  */
/*----------------------------------------------------------------------------*/
void SeHistory::vOpen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  char wFileName[C_1KB];
  
  StrLib::vSprint(wFileName,"/tmp/SEitf_%lld_history.txt"_Str,Process::iGetPid());
  
  bool b = m_xFile.bOpen((Str*)wFileName,C_OPEN_READ_WRITE_CREATE);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* print command history file                                                 */
/*----------------------------------------------------------------------------*/
void SeHistory::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  char wCommand[C_4KB];
  
  vSeekStart();
  
  for (i8 i = 1; bRead(wCommand); i++)
    vTrace("  %lld  %s\n"_Str,i,wCommand);
  
  vSeekEnd();
}

/*----------------------------------------------------------------------------*/
/* recall command from history file                                           */
/*----------------------------------------------------------------------------*/
bool SeHistory::bRecall(Str* aToken[], i8 nTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  i8 iCommand;
  
  if (StrLib::bEqualTo("!!",aToken[0]->a(),C_NO_CASE)) {
    iCommand = m_iCommands;
    if (! iCommand)
      return false;
  }
  else {
    // get command number
    if (StrLib::iLength(aToken[0]->a()) < 2)
      return false;
    
    iCommand = StrLib::iToI8(aToken[0]->a()+1);
  }
  
  vTrace("Recall: %lld\n"_Str,iCommand);
  
  bool bRecalled = bGet(iCommand,aToken,nTokens);
  
  return bRecalled;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get command from history file                                              */
/*----------------------------------------------------------------------------*/
bool SeHistory::bGet(i8 iCommand, Str* aToken[], i8 nTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  vSeekStart();
  
  char wCommand[C_4KB];
  
  s48K sCommand;
  
  i8 i = 0;
  while (bRead(wCommand)) {
    if (++i == iCommand) {
      vSeekEnd();
      sCommand = wCommand;
      
      m_pTokeniser->vUseString(sCommand);
      
      m_pTokeniser->vResetTokens(aToken,nTokens);
      
      i8 i = 0;
      while (TK_NONE != m_pTokeniser->eNextToken(aToken[i]->a())) {
        //vTrace("Word [%lld] %s\n"_Str,i,pWords[i]);
        i++;
      }
      
      return true;
    }
  }
  
  vSeekEnd();
  
  m_pTokeniser->vUseEnd();
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* read line from command history file                                        */
/*----------------------------------------------------------------------------*/
bool SeHistory::bRead(char* wCommand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  bool bResult = m_xFile.bReadLine((Str*)wCommand);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* seek to start of command history file                                      */
/*----------------------------------------------------------------------------*/
void SeHistory::vSeekStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  m_xFile.vSeek(0);
}

/*----------------------------------------------------------------------------*/
/* seek to end of command history file                                        */
/*----------------------------------------------------------------------------*/
void SeHistory::vSeekEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  m_xFile.vSeekEnd();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SeHistory::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY);
  
 return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
