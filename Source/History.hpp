/*============================================================================*/
/* History.hpp -- command history                                             */
/*============================================================================*/

#pragma once

#include "Api.hpp"

class History
{
private:
  char  m_wHistory[C_4KB];
  File  m_xHistoryFile;
  i8    m_iCommand;
  bool  m_bSkipHistory;
 
  InputStream m_xInputStream; // input stream handler
  
public:
  History(void);
  ~History(void);
  
  void vAppend(char* pWords[]);
  void vClose(void);
  bool bGet(i8 iCommand, char* pWords[]);
  void vOpen(void);
  void vPrint(void);
  bool bRead(char* wCommand);
  bool bRecall(char* pWords[]);
  void vStart(void);
  void vEnd(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/