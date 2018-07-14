/*============================================================================*/
/* SeHistory.cpp -- SePrompt History                                          */
/*============================================================================*/

#include "Prompt.h"

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
void SeHistory::vAppend(TokenVec& xTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  vSeekEnd();
  
  for (i8 i_Token = 0; xTokenVec[i_Token]->bHasText(); i_Token++) {
    if (i_Token)
      m_xFile.vPrint("%s"_Stz," ");
    m_xFile.vPrint("%s"_Stz,xTokenVec[i_Token]->aText());
  }
  m_xFile.vPrint("%s"_Stz,"\n");
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
    vWarn("Failed to delete history file: %s\n"_Stz, sHistory.aChar());
  }
}

/*----------------------------------------------------------------------------*/
/* open command history file                                                  */
/*----------------------------------------------------------------------------*/
void SeHistory::vOpen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  s256 xFileName;
  
  xFileName.vSprint("/tmp/SEitf_%lld_history.txt"_Stz,Process::iGetPid());
  
  bool b = m_xFile.bOpen((Stz*)xFileName.aChar(),C_OPEN_READ_WRITE_CREATE);
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
  
  for (i8 i = 1; bRead((Stz*)xCommand.aChar()); i++)
    vTrace("  %lld  %s\n"_Stz,i,xCommand.aChar());
  
  vSeekEnd();
}

/*----------------------------------------------------------------------------*/
/* recall command from history file                                           */
/*----------------------------------------------------------------------------*/
bool SeHistory::bRecall(TokenVec& xTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  i8 iCommand;
  
  if (xTokenVec[0]->bSame("!!"_Stz)) {
    iCommand = m_iCommands;
    if (! iCommand)
      return false;
  }
  else {
    // get command number
    if (xTokenVec[1]->iLength() < 1)
      return false;
    
    iCommand = xTokenVec[1]->iToI8();
  }
  
  vTrace("Recall: %lld\n"_Stz,iCommand);
  
  bool bRecalled = bGet(iCommand,xTokenVec);
  
  return bRecalled;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get command from history file                                              */
/*----------------------------------------------------------------------------*/
bool SeHistory::bGet(i8 iCommand, TokenVec& xTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY,_FOOTER_);
  
  vSeekStart();
  
  s48K sCommand;
  
  i8 i = 0;
  while (bRead((Stz*)sCommand.aChar())) {
    
    if (++i == iCommand) {
      
      vSeekEnd();
      
      m_pTokeniser->vUseString(sCommand);
      
      xTokenVec.vReset();
      
      i8 i = 0;
      while (TN_NONE != m_pTokeniser->eNextToken((char*)xTokenVec[i]->aText())) {
        //vTrace("Word [%lld] %s\n"_Stz,i,pWords[i]);
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
bool SeHistory::bRead(Stz* aCommand)
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
Footer* SeHistory::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEHISTORY);
  
 return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
