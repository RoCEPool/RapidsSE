/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/

#pragma once

#include "Api.hpp"
#include "History.hpp"

class SePrompt
{
private:
  i8        m_iPartition; // which partition
  SEitf*    m_pSEitf;     // interface into rapidsse
  bool      m_bContinue;  // continue proecssing commands?
  
  s8K       m_sResponse;  // response from seitf
  History   m_xHistory;   // command history
  
  char      m_wWorkarea[C_16KB];
  char*     m_pTokens[16];
  Tokenizer m_xTokenizer; // input stream parser
  
  
public:
  SePrompt(i8 iPartition);
  ~SePrompt(void);
  
  void vLoop(void);
  
private:
  void vGetCommand(void);
  void vProcessCommand(void);
  
  void vExecuteRequest(void);
  void vIterateRows(void);
  void vHelp(void);
  bool bTestFile(char* pPath);
  void vPushFile(char* pWord);
  
  void vCreateTable(void);
  bool bValidateType(char* pType);
  
  void vInit(char* wWorkarea, char* pWords[]);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/