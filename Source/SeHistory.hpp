/*============================================================================*/
/* SeHistory.hpp -- SePrompt History                                          */
/*============================================================================*/
#pragma once

#include "StorageEngine.h"

class SeHistory : public Object
{
private:
  File  m_xFile;          // history file
  i8    m_iCommands;      // number of commands in history
  Tokeniser* m_pTokeniser; // tokenizer while reading file
  
  // must be last
  Footer m_xFooter;
  
public:
  SeHistory(void);
  ~SeHistory(void);
  
  void vAppend(TokenSet& xTokenSet);
  void vClose(void);
  void vOpen(void);
  void vPrint(void);
  bool bRecall(TokenSet& xTokenSet);
  
private:
  void vSeekStart(void);
  void vSeekEnd(void);
  bool bGet(i8 iCommand, TokenSet& xTokenAry);
  bool bRead(Str* aCommand);
  
  Footer* aFooter(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
