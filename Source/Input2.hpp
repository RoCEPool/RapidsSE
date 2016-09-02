/*============================================================================*/
/* Input.hpp -- Input Processing                                              */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

class Input2
{
private:
  InputParser m_xInputParser; // input stream parser

public:
  Input2(void);
  ~Input2(void);
  
  void vNext(char* pWords[]);
  void vPrompt(const char* wPrompt);
  void vInit(char* wWorkarea, char* pWords[]);
  
  void vPushFile(char* pWord);
  void vPopFile(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/