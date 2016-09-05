/*============================================================================*/
/* History.hpp -- command history                                             */
/*============================================================================*/

#pragma once

#include "Api.hpp"

class History
{
private:
  File  m_xFile;          // history file
  i8    m_iCommands;      // number of commands in history
  Tokenizer m_xTokenizer; // tokenizer while reading file
  
public:
  History(void);
  ~History(void);
  
  void vAppend(char* pTokens[]);
  void vClose(void);
  void vOpen(void);
  void vPrint(void);
  bool bRecall(char* pTokens[]);
  
private:
  void vSeekStart(void);
  void vSeekEnd(void);
  bool bGet(i8 iCommand, char* pTokens[]);
  bool bRead(char* wCommand);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/