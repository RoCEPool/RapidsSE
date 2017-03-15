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
  
  s256 sHistory = m_xFile.sPath();
  m_xFile.vClose();
  
  // SHOULD REMOVE HISTORY FILE?
  bool bResult = File::bRemove(sHistory);
  if (!bResult) {
    vWarn("Failed to delete history file: %s\n"_Str, sHistory.aArea());
  }
}

/*----------------------------------------------------------------------------*/
/* open command history file                                                  */
/*----------------------------------------------------------------------------*/
void SeHistory::vOpen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  s256 xFileName;
  
  xFileName.vSprint("/tmp/SEitf_%lld_history.txt"_Str,Process::iGetPid());
  
  bool b = m_xFile.bOpen((Str*)xFileName.aArea(),C_OPEN_READ_WRITE_CREATE);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* print command history file                                                 */
/*----------------------------------------------------------------------------*/
void SeHistory::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  s48K xCommand;
  
  vSeekStart();
  
  for (i8 i = 1; bRead((Str*)xCommand.aArea()); i++)
    vTrace("  %lld  %s\n"_Str,i,xCommand.aArea());
  
  vSeekEnd();
}

/*----------------------------------------------------------------------------*/
/* recall command from history file                                           */
/*----------------------------------------------------------------------------*/
bool SeHistory::bRecall(Str* aToken[], i8 nTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  i8 iCommand;
  
  if (aToken[0]->bEqual("!!"_Str)) {
    iCommand = m_iCommands;
    if (! iCommand)
      return false;
  }
  else {
    // get command number
    if (aToken[1]->iLength() < 1)
      return false;
    
    iCommand = aToken[1]->iToI8();
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
  
  s48K sCommand;
  
  i8 i = 0;
  while (bRead((Str*)sCommand.aArea())) {
    
    if (++i == iCommand) {
      
      vSeekEnd();
      
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
bool SeHistory::bRead(Str* aCommand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  bool bResult = m_xFile.bReadLine(aCommand);
  
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
