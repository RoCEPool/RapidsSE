/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/

#pragma once

#include "Api.hpp"
#include "History.hpp"

class SePrompt
{
private:
  i8       m_iPartition; // which partition
  SEitf*   m_pSEitf;     // interface into rapidsse
  
  s8K      m_sResponse;  // response from seitf
  History  m_xHistory;   // command history
  
  char     m_wWorkarea[C_16KB];
  char*    m_pTokens[16];
  
  bool     m_bContinue;
  
  Tokenizer m_xTokenizer; // input stream parser
  
  
public:
  SePrompt(i8 iPartition);
  ~SePrompt(void);
  
  void vPrompt(void);
  
private:
  void vCreateTable(void);
  void vExecute(void);
  void vIterateRows(void);
  void vProcess(void);
  bool bTestFile(char* pPath);
  bool bValidateType(char* pType);
  
  void vHelp(void);
  
  //---
  
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