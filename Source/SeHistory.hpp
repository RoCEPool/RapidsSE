/*============================================================================*/
/* SeHistory.hpp -- SePrompt History                                          */
/*============================================================================*/

#pragma once

#include "Api.hpp"

class SeHistory : public Object
{
private:
  File  m_xFile;          // history file
  i8    m_iCommands;      // number of commands in history
  Tokeniser m_xTokeniser; // tokenizer while reading file
  
  // must be last
  Footer m_xFooter;
  
public:
  SeHistory(void);
  ~SeHistory(void);
  
  void vAppend(char* pToken[], i8 nTokens);
  void vClose(void);
  void vOpen(void);
  void vPrint(void);
  bool bRecall(char* pToken[], i8 nTokens);
  
private:
  void vSeekStart(void);
  void vSeekEnd(void);
  bool bGet(i8 iCommand, char* pToken[], i8 nTokens);
  bool bRead(char* wCommand);
  
  Footer* pFooter(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/