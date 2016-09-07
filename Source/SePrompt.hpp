/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/

#pragma once

#include "Api.hpp"
#include "History.hpp"

class SePrompt : public Object
{
  static const i8 C_TOKENS      = 10240;
  static const i8 C_TOKEN_SPACE = C_1KB;
  static const i8 C_TOKEN_AREA  = C_TOKEN_SPACE * C_TOKENS;
  
private:
  i8        m_iPartition; // which partition
  SEitf*    m_pSEitf;     // interface into rapidsse
  bool      m_bContinue;  // continue processing commands?
  s32       m_sPrompt;    // prompt
  
  s8K       m_sResponse;  // response from seitf
  History   m_xHistory;   // command history
  
  char*     m_pToken[C_TOKENS];
  Tokenizer m_xTokenizer; // input stream parser

  char      m_wTokenArea[C_TOKEN_AREA];
  
public:
  void* operator new(size_t);
  void  operator delete(void*);

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
  void vCreateTable2(void);
  bool bValidateType(char* pType);
  
  void vCollectStatement(void);
 };

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/