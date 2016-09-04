/*============================================================================*/
/* Input.hpp -- Input Processing                                              */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

class Input

{
private:
  Tokenizer m_xTokenizer; // input stream parser

public:
  Input(void);
  ~Input(void);
  
  void vNext(char* pWords[]);
  void vPrompt(const char* wPrompt);
  void vInit(char* wWorkarea, char* pWords[]);
  void vLineMode(bool bLineMode);
  void vPushFile(char* pWord);
  void vPopFile(void);
  bool bEOF(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/