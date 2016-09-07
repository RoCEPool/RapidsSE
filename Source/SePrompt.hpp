/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/

#pragma once

#include "Api.hpp"
#include "SeHistory.hpp"

class SePrompt : public Object
{
  static const i8 C_TOKENS      = 1024;
  static const i8 C_TOKEN_SPACE = 256;
  static const i8 C_TOKEN_AREA  = C_TOKEN_SPACE * C_TOKENS;
  
private:
  i8        m_iPartition; // which partition
  SEitf*    m_pSEitf;     // interface into rapidsse
  bool      m_bContinue;  // continue processing commands?
  s32       m_sPrompt;    // prompt
  
  s8K       m_sResponse;  // response from seitf
  SeHistory m_xHistory;   // command history
  
  char*     m_pToken[C_TOKENS];
  char*     m_pToken2[C_TOKENS];
  Tokeniser m_xTokeniser; // input stream tokeniser

  char      m_wTokenArea[C_TOKEN_AREA];
  char      m_wTokenArea2[C_TOKEN_AREA];
  
  // must be last
  Footer m_xFooter;
  
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
  void vDropTable(void);
 };

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/