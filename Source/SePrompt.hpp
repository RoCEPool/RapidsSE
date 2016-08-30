/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/

#pragma once

#include "Api.hpp"
#include "Input.hpp"
#include "History.hpp"

class SePrompt
{
private:
  i8       m_iPartition; // which partition
  SEitf*   m_pSEitf;     // interface into rapidsse
  
  Input    m_xInput;     // input processing
  s8K      m_sResponse;  // response from seitf
  History  m_xHistory;   // command history
  
  char     m_wWorkarea[C_16KB];
  char*    m_pWords[16];
  
  bool     m_bContinue;
  
public:
  SePrompt(i8 iPartition);
  ~SePrompt(void);
  
  void vPrompt(void);
  
private:
  void vExecute(void);
  void vIterateRows(void);
  void vProcess(void);
  bool bTestFile(char* pPath);
  
  bool bHelp(char* pWords[]);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/