/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/
#pragma once

#include "Prompt.h"


static Stz* C_DATE_FORMAT          = "date_format"_Stz;
static Stz* C_ENCLOSED_BY_LEADING  = "enclosed_by_leading"_Stz;
static Stz* C_ENCLOSED_BY_TRAILING = "enclosed_by_trailing"_Stz;
static Stz* C_ESCAPE_CHAR          = "escape_char"_Stz;
static Stz* C_FIELD_DELIMITER      = "field_delimiter"_Stz;
static Stz* C_RECORD_ENDS          = "record_ends"_Stz;
static Stz* C_RECORD_TERMINATOR    = "record_terminator"_Stz;
static Stz* C_SAME_MACHINE         = "same_machine"_Stz;

namespace Prompt
{

  class SePrompt : public Object
  {
  public:
    bool       m_bContinue;  // continue processing commands?
    SEitf*     m_pSEitf;     // interface into rapidsse
    s48K       m_sResponse;  // response from seitf
  
    CommandProcessor* m_pCommandProcessor;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SePrompt(void);
    ~SePrompt(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    // these are the prompt.xyz task primitives
    bool bExecuteFunction(void);

    // other
    SEitf* pSEitf(void);  

  private:

    Footer* aFooter(void);
  
  //----------------------------------------------------------------------------
  // to be removed
    bool bTokenCheck(i8 nTokens);
  
    TokenVec* pTknVec(void);
    TokenVec* pTknVec2(void);
  
    Token* pTkn(i8 i_Token);
    Token* pTkn2(i8 i_Token);

  public:
    bool bCmd_AnalyzeTable(void);
    bool bCmd_BeginInsert(void);
    bool bCmd_CreateCatalog(void);
    bool bCmd_CreateIndex(void);
    bool bCmd_CreateSchema(void);
    bool bCmd_CreateTenant(void);
    bool bCmd_CreateUnique(void);
    bool bCmd_DanalyzeTable(void);
    bool bCmd_Docol(void);
    bool bCmd_DistParLoadTable(void);
    bool bCmd_DropTenant(void);
    bool bCmd_DropCatalog(void);
    bool bCmd_DropIndex(void);
    bool bCmd_DropSchema(void);
    bool bCmd_EndInsert(void);
    bool bCmd_Help(void);
    bool bCmd_InsertRow(void);
    //bool bCmd_LoadTable(void);
    bool bCmd_PrintDir(void);
    bool bCmd_UseSchema(void);
  
    bool bCmd_CreateCatalogR35(void);
    bool bCmd_CreateIndexR35(void);
    bool bCmd_CreateSchemaR35(void);
    bool bCmd_CreateTenantR35(void);
  
    bool bCmd_DropCatalogR35(void);
    bool bCmd_DropIndexR35(void);
    bool bCmd_DropSchemaR35(void);
    bool bCmd_DropTenantR35(void);
 
  private:
    i8   iCopyTokens(TokenVec* pFromTokenVec, TokenVec* pToTokenVec);
    bool bDropReturnTable(Stz* aTableName);

    bool bRemoveTable(Stz* pTableName);
    bool bValidateCreateIndex(i8 iNumTokens);
  
    bool bValidateCreateIndexR35(i8 iNumTokens);
  
    bool bReturnTable(void);
    bool bIndexReturnTable(Stz* aTableName, Stz* aIndexName);
    void vIterateBuffers(void);
    bool bExecuteRequest(void);

  };

  SePrompt* _pSePrompt(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
