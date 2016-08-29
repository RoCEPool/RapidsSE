/*============================================================================*/
/* History.cpp -- command history                                             */
/*============================================================================*/

#include "History.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
History::History(void)
{
  m_wHistory[0] = '\0';
  //m_xHistoryFile
  m_iCommand = 0;
  m_bSkipHistory = false;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
History::~History(void)
{
}

/*----------------------------------------------------------------------------*/
/* append command to history file                                             */
/*----------------------------------------------------------------------------*/
void History::vAppend(char* pWords[])
{
  for (i8 i = 0; *pWords[i]; i++) {
    if (i)
      m_xHistoryFile.vPrint("%s"," ");
    m_xHistoryFile.vPrint("%s",pWords[i]);
  }
  m_xHistoryFile.vPrint("%s","\n");
  m_xHistoryFile.vFlush();
  
  m_iCommand++;
}

/*----------------------------------------------------------------------------*/
/* close command history file                                                 */
/*----------------------------------------------------------------------------*/
void History::vClose(void)
{
  m_xHistoryFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* seek to end of command history file                                        */
/*----------------------------------------------------------------------------*/
void History::vEnd(void)
{
  m_xHistoryFile.vSeekEnd();
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
      //      String::vCopy(pWords[0],wCommand);
      {
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
        
      }
      
      return true;
    }
  }
  
  vEnd();
  return false;
}

/*----------------------------------------------------------------------------*/
/* open command history file                                                  */
/*----------------------------------------------------------------------------*/
void History::vOpen(void)
{
  char wFileName[C_1KB];
  
  String::vSprint(wFileName,"/tmp/SEitf_%lld_history.txt",Process::iGetPid());
  
  bool b = m_xHistoryFile.bOpen(wFileName,C_OPEN_READ_WRITE_CREATE);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* print command history file                                                 */
/*----------------------------------------------------------------------------*/
void History::vPrint(void)
{
  vStart();
  
  char wCommand[C_4KB];
  
  i8 i = 1;
  while (bRead(wCommand)) {
    vTrace("%lld %s\n",i++,wCommand);
  }
  
  vEnd();
}

/*----------------------------------------------------------------------------*/
/* read command history file                                                  */
/*----------------------------------------------------------------------------*/
bool History::bRead(char* wCommand)
{
  bool bResult = m_xHistoryFile.bReadLine(wCommand);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* recall command from history file                                           */
/*----------------------------------------------------------------------------*/
bool History::bRecall(char* pWords[])
{
  i8 iCommand;
  
  if (String::bEqualTo("!!",pWords[0],C_NO_CASE)) {
    iCommand = m_iCommand;
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
  
  bool b = bGet(iCommand,pWords);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* seek to start of command history file                                      */
/*----------------------------------------------------------------------------*/
void History::vStart(void)
{
  m_xHistoryFile.vSeek(0);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/