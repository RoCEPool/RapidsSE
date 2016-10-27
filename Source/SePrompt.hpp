/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/

#pragma once

#include "Api.hpp"
#include "SeHistory.hpp"
#include "Error.hpp"

using namespace Magic;

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
  bool      m_bUseString; // use string
  
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
  
  static void vDump(eCN eClass, Object* pObject, const char* pTab);
  
  void vLoop(void);
  bool bExecuteString(s& sCommand, eEC& eErrorCode);
  bool bExecuteString(s& sCommand, eEC& eErrorCode, s& sResponse);
  bool bExecuteString(s& sCommand, eEC& eErrorCode, char** ppRowPtr);

private:
  void vGetCommand(void);
  void vGetCommandArgument(void);
  void vGetCommandInput(void);
  
  bool bProcessCommand(bool bPostProcess = true);
  
  bool bExecuteRequest(void);
  bool bGetFirstRow(void);
  bool bGetNextRow(void);
  void vIterateRows(void);
  void vHelp(void);
  bool bTestFile(char* pPath);
  void vPushFile(char* pWord);
  
  bool bBeginInsert(void);
  bool bChangeWorkingDir(void);
  
  bool bCreateCatalog(void);
  bool bCreateSchema(void);
  bool bCreateTable(void);
  bool bCreateTenant(void);
  
  void vCollectStatement(void);
  bool bDocol(void);
  
  bool bDropCatalog(void);
  bool bDropSchema(void);
  bool bDropTable(void);
  bool bDropTable(char* pTableName);
  bool bDropTenant(void);
  
  bool bEndInsert(void);
  bool bInsertRow(void);
  bool bPoolSpace(void);
  bool bPrintWorkingDir(void);
  
  bool bListDir(void);

  Footer* pFooter(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
