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
void History::vAppend(char* pWords[])
{
  vEnd();
  
  for (i8 i = 0; *pWords[i]; i++) {
    if (i)
      m_xFile.vPrint("%s"," ");
    m_xFile.vPrint("%s",pWords[i]);
  }
  m_xFile.vPrint("%s","\n");
  m_xFile.vFlush();
  
  m_iCommands++;
}

/*----------------------------------------------------------------------------*/
/* get command from history file                                              */
/*----------------------------------------------------------------------------*/
bool History::bGet(i8 iCommand, char* pWords[])
{
  vStart();
  
  char wCommand[C_4KB];
  
  s16K sCommand;
  
  i8 i = 0;
  while (bRead(wCommand)) {
    if (++i == iCommand) {
      vEnd();
      sCommand = wCommand;
      m_xInputStream.vUseText(sCommand);
      m_xInputStream.vLineOriented(true);
        
      for (i8 i = 0; i < 16; i++)
        *pWords[i] = '\0';
      
      i8 i = 0;
      //pInput->vEOL(false);
      while (LT_NONE != m_xInputStream.eNextLiteral(pWords[i])) {
        //vTrace("Word [%lld] %s\n",i,pWords[i]);
        i++;
      }
      
      return true;
    }
  }
  
  vEnd();
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* print command history file                                                 */
/*----------------------------------------------------------------------------*/
void History::vPrint(void)
{
  char wCommand[C_4KB];
  
  vStart();
  
  for (i8 i = 1; bRead(wCommand); i++)
    vTrace("  %lld  %s\n",i,wCommand);
  
  vEnd();
}

/*----------------------------------------------------------------------------*/
/* recall command from history file                                           */
/*----------------------------------------------------------------------------*/
bool History::bRecall(char* pWords[])
{
  i8 iCommand;
  
  if (String::bEqualTo("!!",pWords[0],C_NO_CASE)) {
    iCommand = m_iCommands;
    if (! iCommand)
      return false;
  }
  else {
    // get command number
    if (String::iLength(pWords[0]) < 2)
      return false;
    
    iCommand = String::iToI8(pWords[0]+1);
  }
  
  vTrace("Recall: %lld\n",iCommand);
  
  bool bRecalled = bGet(iCommand,pWords);
  
  return bRecalled;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
/* close command history file                                                 */
/*----------------------------------------------------------------------------*/
void History::vClose(void)
{
  m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* seek to end of command history file                                        */
/*----------------------------------------------------------------------------*/
void History::vEnd(void)
{
  m_xFile.vSeekEnd();
}

/*----------------------------------------------------------------------------*/
/* seek to start of command history file                                      */
/*----------------------------------------------------------------------------*/
void History::vStart(void)
{
  m_xFile.vSeek(0);
}

/*----------------------------------------------------------------------------*/
/* read line from command history file                                        */
/*----------------------------------------------------------------------------*/
bool History::bRead(char* wCommand)
{
  bool bResult = m_xFile.bReadLine(wCommand);
  
  return bResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/