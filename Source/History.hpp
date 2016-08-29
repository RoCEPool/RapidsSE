/*============================================================================*/
/* History.hpp -- command history                                             */
/*============================================================================*/

#pragma once

#include "Api.hpp"

class History
{
private:
  File  m_xFile;              // history file
  i8    m_iCommands;          // number of commands in history
 
  InputStream m_xInputStream; // input stream handler
  
public:
  History(void);
  ~History(void);
  
  void vAppend(char* pWords[]);
  bool bGet(i8 iCommand, char* pWords[]);
  void vPrint(void);
  bool bRecall(char* pWords[]);
  
  void vOpen(void);
  void vClose(void);
  void vStart(void);
  void vEnd(void);
  bool bRead(char* wCommand);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/