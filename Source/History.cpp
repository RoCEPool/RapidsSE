/*============================================================================*/
/* History.cpp -- command history                                             */
/*============================================================================*/

#include "History.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
History::History(void)
{
  //m_xFile
  m_iCommands = 0;
  
  //m_xInputStream
  m_xTokenizer.vPush();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
History::~History(void)
{
  vClose();
}

/*----------------------------------------------------------------------------*/
/* append command to history file                                             */
/*----------------------------------------------------------------------------*/
void History::vAppend(char* pToken[])
{
  vSeekEnd();
  
  for (i8 i = 0; *pToken[i]; i++) {
    if (i)
      m_xFile.vPrint("%s"," ");
    m_xFile.vPrint("%s",pToken[i]);
  }
  m_xFile.vPrint("%s","\n");
  m_xFile.vFlush();
  
  m_iCommands++;
}

/*----------------------------------------------------------------------------*/
/* close command history file                                                 */
/*----------------------------------------------------------------------------*/
void History::vClose(void)
{
  m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* open command history file                                                  */
/*----------------------------------------------------------------------------*/
void History::vOpen(void)
{
  char wFileName[C_1KB];
  
  String::vSprint(wFileName,"/tmp/SEitf_%lld_history.txt",Process::iGetPid());
  
  bool b = m_xFile.bOpen(wFileName,C_OPEN_READ_WRITE_CREATE);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* print command history file                                                 */
/*----------------------------------------------------------------------------*/
void History::vPrint(void)
{
  char wCommand[C_4KB];
  
  vSeekStart();
  
  for (i8 i = 1; bRead(wCommand); i++)
    vTrace("  %lld  %s\n",i,wCommand);
  
  vSeekEnd();
}

/*----------------------------------------------------------------------------*/
/* recall command from history file                                           */
/*----------------------------------------------------------------------------*/
bool History::bRecall(char* pToken[])
{
  i8 iCommand;
  
  if (String::bEqualTo("!!",pToken[0],C_NO_CASE)) {
    iCommand = m_iCommands;
    if (! iCommand)
      return false;
  }
  else {
    // get command number
    if (String::iLength(pToken[0]) < 2)
      return false;
    
    iCommand = String::iToI8(pToken[0]+1);
  }
  
  vTrace("Recall: %lld\n",iCommand);
  
  bool bRecalled = bGet(iCommand,pToken);
  
  return bRecalled;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get command from history file                                              */
/*----------------------------------------------------------------------------*/
bool History::bGet(i8 iCommand, char* pToken[])
{
  vSeekStart();
  
  char wCommand[C_4KB];
  
  s16K sCommand;
  
  i8 i = 0;
  while (bRead(wCommand)) {
    if (++i == iCommand) {
      vSeekEnd();
      sCommand = wCommand;
      
      m_xTokenizer.vUseString(sCommand);
      
      for (i8 i = 0; i < 16; i++)
        *pToken[i] = '\0';
      
      i8 i = 0;
      while (TK_NONE != m_xTokenizer.eNextToken(pToken[i])) {
        //vTrace("Word [%lld] %s\n",i,pWords[i]);
        i++;
      }
      
      return true;
    }
  }
  
  vSeekEnd();
  
  m_xTokenizer.vEndTokenizing();
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* read line from command history file                                        */
/*----------------------------------------------------------------------------*/
bool History::bRead(char* wCommand)
{
  bool bResult = m_xFile.bReadLine(wCommand);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* seek to start of command history file                                      */
/*----------------------------------------------------------------------------*/
void History::vSeekStart(void)
{
  m_xFile.vSeek(0);
}

/*----------------------------------------------------------------------------*/
/* seek to end of command history file                                        */
/*----------------------------------------------------------------------------*/
void History::vSeekEnd(void)
{
  m_xFile.vSeekEnd();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/