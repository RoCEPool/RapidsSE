/*============================================================================*/
/* CommandParser.cpp - Parses all external SE commands & produces tasks       */
/*============================================================================*/
#pragma once

#include "sqlBaseVisitor.h"
#include "SqlException.hpp"

namespace STORE
{
class CommandParser : public Object, public Parser::sqlBaseVisitor
  {
  private:
    // The query text that was entered by the user. May contain more than 1 statement.
    s48K m_sQueryStr;

    TagSet*   m_pCommonTagSet;
    Task*     m_pParentTask;
    TaskList* m_pTaskList;

    TagSet*   m_pCurStmtTagSet;
    s48K      m_sCurStmtStr;
    Stz*      m_szProcName;
    TokenVec* m_pArgValues;
    TaskList* m_pChildTasks;  // e.g. a "run" command is represented by a series of child tasks

    Stz*      m_szCurrentFile;

    Footer m_xFooter;

   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    CommandParser(TagSet* pCommonTagSet, Task* pParentTask);
    ~CommandParser(void) noexcept;

    TaskList* pParseCommand(Stz* szCommand, Stz* szFilePath = nullptr);


    Footer* aFooter(void);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Visited grammar rules:
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Top level statement rules (in rule order)
    antlrcpp::Any visitStatements(Parser::sqlParser::StatementsContext *pCtx)               override;
    antlrcpp::Any visitStatement(Parser::sqlParser::StatementContext *pCtx)                 override;
    antlrcpp::Any visitStatementChoice(Parser::sqlParser::StatementChoiceContext *pCtx)     override;
    antlrcpp::Any visitKeyOnlyTag(Parser::sqlParser::KeyOnlyTagContext *pCtx)               override;
    antlrcpp::Any visitKeyValueTag(Parser::sqlParser::KeyValueTagContext *pCtx)             override;

    // Command specific rules (in alphabetical order)
    antlrcpp::Any visitAnidxCmd(Parser::sqlParser::AnidxCmdContext *pCtx)                   override;
    antlrcpp::Any visitAntblCmd(Parser::sqlParser::AntblCmdContext *pCtx)                   override;
    antlrcpp::Any visitBpaCmd(Parser::sqlParser::BpaCmdContext *pCtx)                       override;
    antlrcpp::Any visitCompactIdxCmd(Parser::sqlParser::CompactIdxCmdContext *pCtx)         override;
    antlrcpp::Any visitCountCmd(Parser::sqlParser::CountCmdContext *pCtx)                   override;
    antlrcpp::Any visitCreateIndexCmd(Parser::sqlParser::CreateIndexCmdContext *pCtx)       override;
    antlrcpp::Any visitCreateTableCmd(Parser::sqlParser::CreateTableCmdContext* pCtx)       override;
    antlrcpp::Any visitDocolCmd(Parser::sqlParser::DocolCmdContext *pCtx)                   override;
    antlrcpp::Any visitDloadCmd(Parser::sqlParser::DloadCmdContext *pCtx)                   override;
    antlrcpp::Any visitDropIndexCmd(Parser::sqlParser::DropIndexCmdContext* pCtx)           override;
    antlrcpp::Any visitDropTableCmd(Parser::sqlParser::DropTableCmdContext* pCtx)           override;
    antlrcpp::Any visitExplainCmd(Parser::sqlParser::ExplainCmdContext* pCtx)               override;
    antlrcpp::Any visitExportCmd(Parser::sqlParser::ExportCmdContext* pCtx)                 override;
    antlrcpp::Any visitFileCmd(Parser::sqlParser::FileCmdContext *pCtx)                     override;
    antlrcpp::Any visitLoadCmd(Parser::sqlParser::LoadCmdContext *pCtx)                     override;
    antlrcpp::Any visitLoadOption(Parser::sqlParser::LoadOptionContext *pCtx)               override;
    antlrcpp::Any visitLoadReplicatedCmd(Parser::sqlParser::LoadReplicatedCmdContext *pCtx) override;
    antlrcpp::Any visitLogCmd(Parser::sqlParser::LogCmdContext *pCtx)                       override;
    antlrcpp::Any visitLscolCmd(Parser::sqlParser::LscolCmdContext *pCtx)                   override;
    antlrcpp::Any visitLsidxCmd(Parser::sqlParser::LsidxCmdContext *pCtx)                   override;
    antlrcpp::Any visitLsidxcolCmd(Parser::sqlParser::LsidxcolCmdContext *pCtx)             override;
    antlrcpp::Any visitLsptncolCmd(Parser::sqlParser::LsptncolCmdContext *pCtx)             override;
    antlrcpp::Any visitQueryCmd(Parser::sqlParser::QueryCmdContext* pCtx)                   override;
    antlrcpp::Any visitSetTraceCmd(Parser::sqlParser::SetTraceCmdContext *pCtx)             override;
    antlrcpp::Any visitSleepCmd(Parser::sqlParser::SleepCmdContext *pCtx)                   override;
    antlrcpp::Any visitSzidxCmd(Parser::sqlParser::SzidxCmdContext *pCtx)                   override;
    antlrcpp::Any visitSztblCmd(Parser::sqlParser::SztblCmdContext *pCtx)                   override;
    antlrcpp::Any visitTrackmemoryCmd(Parser::sqlParser::TrackmemoryCmdContext *pCtx)       override;
    antlrcpp::Any visitTruncateCmd(Parser::sqlParser::TruncateCmdContext *pCtx)             override;
    antlrcpp::Any visitUnloadCmd(Parser::sqlParser::UnloadCmdContext *pCtx)                 override;


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private:
    Task*        pCreateNoopTask(Stz* szStmt);
    static Text* pCleanId(Parser::sqlParser::IdentifierContext* pCtx);
    static Text* pCleanString(Parser::sqlParser::StringLiteralContext* pCtx);
    static Text* pCleanString(antlr4::Token* pToken);
    static void  vCleanString(Parser::sqlParser::StringLiteralContext* pCtx, s256& sBuf, eEC eLenErr);
    static void  vCleanString(Parser::sqlParser::IdentifierContext* pCtx, s256& sBuf, eEC eLenErr);
    static void  vCleanString(antlr4::Token* pToken, s256& sBuf, eEC eLenErr);
    static void  vCleanString(std::string& sStr, s256& sBuf, eEC eLenErr);
    static void  vCleanTag(Parser::sqlParser::TagNameContext* pCtx, s256& sBuf);
    static void  vCleanTag(antlr4::Token* pToken, s256& sBuf);
    static i8    iCleanInt(Parser::sqlParser::SignedIntegerLiteralContext* pCtx);
    static i8    iCleanInt(Parser::sqlParser::IntegerLiteralContext* pCtx);
    static i8    iCleanInt(antlr4::Token* pToken);
    static i8    iCleanInt(std::string& sStr);
    static u8    uCleanInt(Parser::sqlParser::SignedIntegerLiteralContext* pCtx);
    static u8    uCleanInt(Parser::sqlParser::IntegerLiteralContext* pCtx);
    static u8    uCleanInt(antlr4::Token* pToken);
    static u8    uCleanInt(std::string& sStr);
    static void  vReconstituteStatementWithSpaces(antlr4::tree::ParseTree* pNode, sSTMT& sStmt, char** ppcCrs);

  };
  
  CommandParser* _pCommandParser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
