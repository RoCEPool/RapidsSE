/*============================================================================*/
/* Input.hpp -- Input Processing                                              */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

class Input
{
private:
  static const i8 C_INPUT_DEPTH = 4;

  s256   m_sPrompt;    // the prompt
  
  s256        m_sInputLine;
  i8          m_iInputDepth;
  InputStream m_xInputStream[C_INPUT_DEPTH]; // input stream handler
  
public:
  Input(void);
  ~Input(void);
  
  void vNext(char* pWords[]);
  void vPrompt(const char* wPrompt);
  void vInit(char* wWorkarea, char* pWords[]);
  
  void vPushFile(char* pWord);
  void vPopFile(void);
  
  InputStream* pTopInput(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/