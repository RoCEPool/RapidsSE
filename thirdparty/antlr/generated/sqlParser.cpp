
// Generated from /Users/craig/repos/rapidsse/thirdparty/antlr/sql.g4 by ANTLR 4.8


#include "sqlVisitor.h"

#include "sqlParser.h"


using namespace antlrcpp;
using namespace Parser;
using namespace antlr4;

sqlParser::sqlParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

sqlParser::~sqlParser() {
  delete _interpreter;
}

std::string sqlParser::getGrammarFileName() const {
  return "sql.g4";
}

const std::vector<std::string>& sqlParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& sqlParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- StatementsContext ------------------------------------------------------------------

sqlParser::StatementsContext::StatementsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::StatementsContext::EOF() {
  return getToken(sqlParser::EOF, 0);
}

std::vector<sqlParser::StatementContext *> sqlParser::StatementsContext::statement() {
  return getRuleContexts<sqlParser::StatementContext>();
}

sqlParser::StatementContext* sqlParser::StatementsContext::statement(size_t i) {
  return getRuleContext<sqlParser::StatementContext>(i);
}


size_t sqlParser::StatementsContext::getRuleIndex() const {
  return sqlParser::RuleStatements;
}


antlrcpp::Any sqlParser::StatementsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitStatements(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::StatementsContext* sqlParser::statements() {
  StatementsContext *_localctx = _tracker.createInstance<StatementsContext>(_ctx, getState());
  enterRule(_localctx, 0, sqlParser::RuleStatements);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(305); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(304);
      statement();
      setState(307); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::T__0)
      | (1ULL << sqlParser::T__1)
      | (1ULL << sqlParser::T__4)
      | (1ULL << sqlParser::T__6)
      | (1ULL << sqlParser::T__8)
      | (1ULL << sqlParser::ANALYSE)
      | (1ULL << sqlParser::ANALYZE)
      | (1ULL << sqlParser::ANIDX)
      | (1ULL << sqlParser::ANTBL)
      | (1ULL << sqlParser::BPA)
      | (1ULL << sqlParser::BYE)
      | (1ULL << sqlParser::COMPACTIDX)
      | (1ULL << sqlParser::COUNT)
      | (1ULL << sqlParser::CREATE))) != 0) || ((((_la - 79) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 79)) & ((1ULL << (sqlParser::DISCONNECT - 79))
      | (1ULL << (sqlParser::DLOAD - 79))
      | (1ULL << (sqlParser::DOCOL - 79))
      | (1ULL << (sqlParser::DROP - 79))
      | (1ULL << (sqlParser::EXIT_TOKEN - 79))
      | (1ULL << (sqlParser::EXPLAIN - 79))
      | (1ULL << (sqlParser::EXPORT - 79))
      | (1ULL << (sqlParser::FILE - 79))
      | (1ULL << (sqlParser::LOADREPLICATED - 79))
      | (1ULL << (sqlParser::LOAD - 79))
      | (1ULL << (sqlParser::LOG - 79))
      | (1ULL << (sqlParser::LSCOL - 79))
      | (1ULL << (sqlParser::LSIDXCOL - 79))
      | (1ULL << (sqlParser::LSIDX - 79))
      | (1ULL << (sqlParser::LSPTNCOL - 79))
      | (1ULL << (sqlParser::LSTBL - 79))
      | (1ULL << (sqlParser::MEMORYLEAKS - 79)))) != 0) || ((((_la - 156) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 156)) & ((1ULL << (sqlParser::NOOP - 156))
      | (1ULL << (sqlParser::NOP - 156))
      | (1ULL << (sqlParser::PIPELINE - 156))
      | (1ULL << (sqlParser::SELECT - 156))
      | (1ULL << (sqlParser::SLEEP - 156))
      | (1ULL << (sqlParser::SPACE - 156))
      | (1ULL << (sqlParser::STATUS - 156))
      | (1ULL << (sqlParser::SZIDX - 156))
      | (1ULL << (sqlParser::SZTBL - 156))
      | (1ULL << (sqlParser::TABLESIZES - 156))
      | (1ULL << (sqlParser::TRACKMEMORY - 156)))) != 0) || ((((_la - 223) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 223)) & ((1ULL << (sqlParser::TRUNCATE - 223))
      | (1ULL << (sqlParser::UNLOAD - 223))
      | (1ULL << (sqlParser::WITH - 223))
      | (1ULL << (sqlParser::HINT_START - 223)))) != 0));
    setState(309);
    match(sqlParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

sqlParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::TagsContext* sqlParser::StatementContext::tags() {
  return getRuleContext<sqlParser::TagsContext>(0);
}

sqlParser::StatementChoiceContext* sqlParser::StatementContext::statementChoice() {
  return getRuleContext<sqlParser::StatementChoiceContext>(0);
}


size_t sqlParser::StatementContext::getRuleIndex() const {
  return sqlParser::RuleStatement;
}


antlrcpp::Any sqlParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::StatementContext* sqlParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 2, sqlParser::RuleStatement);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(311);
    tags();
    setState(312);
    statementChoice();
    setState(313);
    match(sqlParser::T__0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementChoiceContext ------------------------------------------------------------------

sqlParser::StatementChoiceContext::StatementChoiceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::AnidxCmdContext* sqlParser::StatementChoiceContext::anidxCmd() {
  return getRuleContext<sqlParser::AnidxCmdContext>(0);
}

sqlParser::AntblCmdContext* sqlParser::StatementChoiceContext::antblCmd() {
  return getRuleContext<sqlParser::AntblCmdContext>(0);
}

sqlParser::CompactIdxCmdContext* sqlParser::StatementChoiceContext::compactIdxCmd() {
  return getRuleContext<sqlParser::CompactIdxCmdContext>(0);
}

sqlParser::CountCmdContext* sqlParser::StatementChoiceContext::countCmd() {
  return getRuleContext<sqlParser::CountCmdContext>(0);
}

sqlParser::CreateCmdContext* sqlParser::StatementChoiceContext::createCmd() {
  return getRuleContext<sqlParser::CreateCmdContext>(0);
}

sqlParser::DisconnectCmdContext* sqlParser::StatementChoiceContext::disconnectCmd() {
  return getRuleContext<sqlParser::DisconnectCmdContext>(0);
}

sqlParser::DloadCmdContext* sqlParser::StatementChoiceContext::dloadCmd() {
  return getRuleContext<sqlParser::DloadCmdContext>(0);
}

sqlParser::DocolCmdContext* sqlParser::StatementChoiceContext::docolCmd() {
  return getRuleContext<sqlParser::DocolCmdContext>(0);
}

sqlParser::DropCmdContext* sqlParser::StatementChoiceContext::dropCmd() {
  return getRuleContext<sqlParser::DropCmdContext>(0);
}

sqlParser::ExplainCmdContext* sqlParser::StatementChoiceContext::explainCmd() {
  return getRuleContext<sqlParser::ExplainCmdContext>(0);
}

sqlParser::ExportCmdContext* sqlParser::StatementChoiceContext::exportCmd() {
  return getRuleContext<sqlParser::ExportCmdContext>(0);
}

sqlParser::LoadCmdContext* sqlParser::StatementChoiceContext::loadCmd() {
  return getRuleContext<sqlParser::LoadCmdContext>(0);
}

sqlParser::LoadReplicatedCmdContext* sqlParser::StatementChoiceContext::loadReplicatedCmd() {
  return getRuleContext<sqlParser::LoadReplicatedCmdContext>(0);
}

sqlParser::LogCmdContext* sqlParser::StatementChoiceContext::logCmd() {
  return getRuleContext<sqlParser::LogCmdContext>(0);
}

sqlParser::LscolCmdContext* sqlParser::StatementChoiceContext::lscolCmd() {
  return getRuleContext<sqlParser::LscolCmdContext>(0);
}

sqlParser::LsidxCmdContext* sqlParser::StatementChoiceContext::lsidxCmd() {
  return getRuleContext<sqlParser::LsidxCmdContext>(0);
}

sqlParser::LsidxcolCmdContext* sqlParser::StatementChoiceContext::lsidxcolCmd() {
  return getRuleContext<sqlParser::LsidxcolCmdContext>(0);
}

sqlParser::LsptncolCmdContext* sqlParser::StatementChoiceContext::lsptncolCmd() {
  return getRuleContext<sqlParser::LsptncolCmdContext>(0);
}

sqlParser::LstblCmdContext* sqlParser::StatementChoiceContext::lstblCmd() {
  return getRuleContext<sqlParser::LstblCmdContext>(0);
}

sqlParser::PipelineCmdContext* sqlParser::StatementChoiceContext::pipelineCmd() {
  return getRuleContext<sqlParser::PipelineCmdContext>(0);
}

sqlParser::QueryCmdContext* sqlParser::StatementChoiceContext::queryCmd() {
  return getRuleContext<sqlParser::QueryCmdContext>(0);
}

sqlParser::SleepCmdContext* sqlParser::StatementChoiceContext::sleepCmd() {
  return getRuleContext<sqlParser::SleepCmdContext>(0);
}

sqlParser::SpaceCmdContext* sqlParser::StatementChoiceContext::spaceCmd() {
  return getRuleContext<sqlParser::SpaceCmdContext>(0);
}

sqlParser::StatusCmdContext* sqlParser::StatementChoiceContext::statusCmd() {
  return getRuleContext<sqlParser::StatusCmdContext>(0);
}

sqlParser::SzidxCmdContext* sqlParser::StatementChoiceContext::szidxCmd() {
  return getRuleContext<sqlParser::SzidxCmdContext>(0);
}

sqlParser::SztblCmdContext* sqlParser::StatementChoiceContext::sztblCmd() {
  return getRuleContext<sqlParser::SztblCmdContext>(0);
}

sqlParser::TableSizesCmdContext* sqlParser::StatementChoiceContext::tableSizesCmd() {
  return getRuleContext<sqlParser::TableSizesCmdContext>(0);
}

sqlParser::TruncateCmdContext* sqlParser::StatementChoiceContext::truncateCmd() {
  return getRuleContext<sqlParser::TruncateCmdContext>(0);
}

sqlParser::UnloadCmdContext* sqlParser::StatementChoiceContext::unloadCmd() {
  return getRuleContext<sqlParser::UnloadCmdContext>(0);
}

sqlParser::TrackmemoryCmdContext* sqlParser::StatementChoiceContext::trackmemoryCmd() {
  return getRuleContext<sqlParser::TrackmemoryCmdContext>(0);
}

sqlParser::MemoryleaksCmdContext* sqlParser::StatementChoiceContext::memoryleaksCmd() {
  return getRuleContext<sqlParser::MemoryleaksCmdContext>(0);
}

sqlParser::BpaCmdContext* sqlParser::StatementChoiceContext::bpaCmd() {
  return getRuleContext<sqlParser::BpaCmdContext>(0);
}

sqlParser::FileCmdContext* sqlParser::StatementChoiceContext::fileCmd() {
  return getRuleContext<sqlParser::FileCmdContext>(0);
}

sqlParser::NoopCmdContext* sqlParser::StatementChoiceContext::noopCmd() {
  return getRuleContext<sqlParser::NoopCmdContext>(0);
}

sqlParser::EmptyCmdContext* sqlParser::StatementChoiceContext::emptyCmd() {
  return getRuleContext<sqlParser::EmptyCmdContext>(0);
}


size_t sqlParser::StatementChoiceContext::getRuleIndex() const {
  return sqlParser::RuleStatementChoice;
}


antlrcpp::Any sqlParser::StatementChoiceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitStatementChoice(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::StatementChoiceContext* sqlParser::statementChoice() {
  StatementChoiceContext *_localctx = _tracker.createInstance<StatementChoiceContext>(_ctx, getState());
  enterRule(_localctx, 4, sqlParser::RuleStatementChoice);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(351);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);

      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(316);
      anidxCmd();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(317);
      antblCmd();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(318);
      compactIdxCmd();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(319);
      countCmd();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(320);
      createCmd();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(321);
      disconnectCmd();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(322);
      dloadCmd();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(323);
      docolCmd();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(324);
      dropCmd();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(325);
      explainCmd();
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(326);
      exportCmd();
      break;
    }

    case 13: {
      enterOuterAlt(_localctx, 13);
      setState(327);
      loadCmd();
      break;
    }

    case 14: {
      enterOuterAlt(_localctx, 14);
      setState(328);
      loadReplicatedCmd();
      break;
    }

    case 15: {
      enterOuterAlt(_localctx, 15);
      setState(329);
      logCmd();
      break;
    }

    case 16: {
      enterOuterAlt(_localctx, 16);
      setState(330);
      lscolCmd();
      break;
    }

    case 17: {
      enterOuterAlt(_localctx, 17);
      setState(331);
      lsidxCmd();
      break;
    }

    case 18: {
      enterOuterAlt(_localctx, 18);
      setState(332);
      lsidxcolCmd();
      break;
    }

    case 19: {
      enterOuterAlt(_localctx, 19);
      setState(333);
      lsptncolCmd();
      break;
    }

    case 20: {
      enterOuterAlt(_localctx, 20);
      setState(334);
      lstblCmd();
      break;
    }

    case 21: {
      enterOuterAlt(_localctx, 21);
      setState(335);
      pipelineCmd();
      break;
    }

    case 22: {
      enterOuterAlt(_localctx, 22);
      setState(336);
      queryCmd();
      break;
    }

    case 23: {
      enterOuterAlt(_localctx, 23);
      setState(337);
      sleepCmd();
      break;
    }

    case 24: {
      enterOuterAlt(_localctx, 24);
      setState(338);
      spaceCmd();
      break;
    }

    case 25: {
      enterOuterAlt(_localctx, 25);
      setState(339);
      statusCmd();
      break;
    }

    case 26: {
      enterOuterAlt(_localctx, 26);
      setState(340);
      szidxCmd();
      break;
    }

    case 27: {
      enterOuterAlt(_localctx, 27);
      setState(341);
      sztblCmd();
      break;
    }

    case 28: {
      enterOuterAlt(_localctx, 28);
      setState(342);
      tableSizesCmd();
      break;
    }

    case 29: {
      enterOuterAlt(_localctx, 29);
      setState(343);
      truncateCmd();
      break;
    }

    case 30: {
      enterOuterAlt(_localctx, 30);
      setState(344);
      unloadCmd();
      break;
    }

    case 31: {
      enterOuterAlt(_localctx, 31);
      setState(345);
      trackmemoryCmd();
      break;
    }

    case 32: {
      enterOuterAlt(_localctx, 32);
      setState(346);
      memoryleaksCmd();
      break;
    }

    case 33: {
      enterOuterAlt(_localctx, 33);
      setState(347);
      bpaCmd();
      break;
    }

    case 34: {
      enterOuterAlt(_localctx, 34);
      setState(348);
      fileCmd();
      break;
    }

    case 35: {
      enterOuterAlt(_localctx, 35);
      setState(349);
      noopCmd();
      break;
    }

    case 36: {
      enterOuterAlt(_localctx, 36);
      setState(350);
      emptyCmd();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TagsContext ------------------------------------------------------------------

sqlParser::TagsContext::TagsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<sqlParser::KeyOnlyTagContext *> sqlParser::TagsContext::keyOnlyTag() {
  return getRuleContexts<sqlParser::KeyOnlyTagContext>();
}

sqlParser::KeyOnlyTagContext* sqlParser::TagsContext::keyOnlyTag(size_t i) {
  return getRuleContext<sqlParser::KeyOnlyTagContext>(i);
}

std::vector<sqlParser::KeyValueTagContext *> sqlParser::TagsContext::keyValueTag() {
  return getRuleContexts<sqlParser::KeyValueTagContext>();
}

sqlParser::KeyValueTagContext* sqlParser::TagsContext::keyValueTag(size_t i) {
  return getRuleContext<sqlParser::KeyValueTagContext>(i);
}


size_t sqlParser::TagsContext::getRuleIndex() const {
  return sqlParser::RuleTags;
}


antlrcpp::Any sqlParser::TagsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTags(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TagsContext* sqlParser::tags() {
  TagsContext *_localctx = _tracker.createInstance<TagsContext>(_ctx, getState());
  enterRule(_localctx, 6, sqlParser::RuleTags);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(357);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__1

    || _la == sqlParser::T__4) {
      setState(355);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
      case 1: {
        setState(353);
        keyOnlyTag();
        break;
      }

      case 2: {
        setState(354);
        keyValueTag();
        break;
      }

      }
      setState(359);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- KeyValueTagContext ------------------------------------------------------------------

sqlParser::KeyValueTagContext::KeyValueTagContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::TagNameContext* sqlParser::KeyValueTagContext::tagName() {
  return getRuleContext<sqlParser::TagNameContext>(0);
}

sqlParser::StringLiteralContext* sqlParser::KeyValueTagContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}

tree::TerminalNode* sqlParser::KeyValueTagContext::INTEGER_LITERAL() {
  return getToken(sqlParser::INTEGER_LITERAL, 0);
}


size_t sqlParser::KeyValueTagContext::getRuleIndex() const {
  return sqlParser::RuleKeyValueTag;
}


antlrcpp::Any sqlParser::KeyValueTagContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitKeyValueTag(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::KeyValueTagContext* sqlParser::keyValueTag() {
  KeyValueTagContext *_localctx = _tracker.createInstance<KeyValueTagContext>(_ctx, getState());
  enterRule(_localctx, 8, sqlParser::RuleKeyValueTag);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(360);
    match(sqlParser::T__1);
    setState(361);
    dynamic_cast<KeyValueTagContext *>(_localctx)->keyName = tagName();
    setState(362);
    match(sqlParser::T__2);
    setState(363);
    match(sqlParser::T__3);
    setState(366);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::DUBLQUOTED_IDENTIFIER:
      case sqlParser::SINGLQUOTED_STRING:
      case sqlParser::DUBLQUOTED_STRING: {
        setState(364);
        stringLiteral();
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        setState(365);
        match(sqlParser::INTEGER_LITERAL);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- KeyOnlyTagContext ------------------------------------------------------------------

sqlParser::KeyOnlyTagContext::KeyOnlyTagContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::TagNameContext* sqlParser::KeyOnlyTagContext::tagName() {
  return getRuleContext<sqlParser::TagNameContext>(0);
}


size_t sqlParser::KeyOnlyTagContext::getRuleIndex() const {
  return sqlParser::RuleKeyOnlyTag;
}


antlrcpp::Any sqlParser::KeyOnlyTagContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitKeyOnlyTag(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::KeyOnlyTagContext* sqlParser::keyOnlyTag() {
  KeyOnlyTagContext *_localctx = _tracker.createInstance<KeyOnlyTagContext>(_ctx, getState());
  enterRule(_localctx, 10, sqlParser::RuleKeyOnlyTag);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(374);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__4: {
        enterOuterAlt(_localctx, 1);
        setState(368);
        match(sqlParser::T__4);
        setState(369);
        tagName();
        break;
      }

      case sqlParser::T__1: {
        enterOuterAlt(_localctx, 2);
        setState(370);
        match(sqlParser::T__1);
        setState(371);
        tagName();
        setState(372);
        match(sqlParser::T__2);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TagNameContext ------------------------------------------------------------------

sqlParser::TagNameContext::TagNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::TagNameContext::PLAIN_IDENTIFIER() {
  return getToken(sqlParser::PLAIN_IDENTIFIER, 0);
}

tree::TerminalNode* sqlParser::TagNameContext::TIME() {
  return getToken(sqlParser::TIME, 0);
}


size_t sqlParser::TagNameContext::getRuleIndex() const {
  return sqlParser::RuleTagName;
}


antlrcpp::Any sqlParser::TagNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTagName(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TagNameContext* sqlParser::tagName() {
  TagNameContext *_localctx = _tracker.createInstance<TagNameContext>(_ctx, getState());
  enterRule(_localctx, 12, sqlParser::RuleTagName);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(376);
    _la = _input->LA(1);
    if (!(_la == sqlParser::TIME

    || _la == sqlParser::PLAIN_IDENTIFIER)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QueryCmdContext ------------------------------------------------------------------

sqlParser::QueryCmdContext::QueryCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::QueryContext* sqlParser::QueryCmdContext::query() {
  return getRuleContext<sqlParser::QueryContext>(0);
}


size_t sqlParser::QueryCmdContext::getRuleIndex() const {
  return sqlParser::RuleQueryCmd;
}


antlrcpp::Any sqlParser::QueryCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitQueryCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::QueryCmdContext* sqlParser::queryCmd() {
  QueryCmdContext *_localctx = _tracker.createInstance<QueryCmdContext>(_ctx, getState());
  enterRule(_localctx, 14, sqlParser::RuleQueryCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(378);
    query();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QueryContext ------------------------------------------------------------------

sqlParser::QueryContext::QueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::SelectQueryContext* sqlParser::QueryContext::selectQuery() {
  return getRuleContext<sqlParser::SelectQueryContext>(0);
}

sqlParser::SetQueryContext* sqlParser::QueryContext::setQuery() {
  return getRuleContext<sqlParser::SetQueryContext>(0);
}

std::vector<sqlParser::QueryHintContext *> sqlParser::QueryContext::queryHint() {
  return getRuleContexts<sqlParser::QueryHintContext>();
}

sqlParser::QueryHintContext* sqlParser::QueryContext::queryHint(size_t i) {
  return getRuleContext<sqlParser::QueryHintContext>(i);
}


size_t sqlParser::QueryContext::getRuleIndex() const {
  return sqlParser::RuleQuery;
}


antlrcpp::Any sqlParser::QueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::QueryContext* sqlParser::query() {
  QueryContext *_localctx = _tracker.createInstance<QueryContext>(_ctx, getState());
  enterRule(_localctx, 16, sqlParser::RuleQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(383);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(380);
      queryHint();
      setState(385);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(388);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      setState(386);
      selectQuery();
      break;
    }

    case 2: {
      setState(387);
      setQuery();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QueryHintContext ------------------------------------------------------------------

sqlParser::QueryHintContext::QueryHintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::QueryHintContext::HINT_START() {
  return getToken(sqlParser::HINT_START, 0);
}

tree::TerminalNode* sqlParser::QueryHintContext::HINT_END() {
  return getToken(sqlParser::HINT_END, 0);
}

std::vector<sqlParser::HintContext *> sqlParser::QueryHintContext::hint() {
  return getRuleContexts<sqlParser::HintContext>();
}

sqlParser::HintContext* sqlParser::QueryHintContext::hint(size_t i) {
  return getRuleContext<sqlParser::HintContext>(i);
}


size_t sqlParser::QueryHintContext::getRuleIndex() const {
  return sqlParser::RuleQueryHint;
}


antlrcpp::Any sqlParser::QueryHintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitQueryHint(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::QueryHintContext* sqlParser::queryHint() {
  QueryHintContext *_localctx = _tracker.createInstance<QueryHintContext>(_ctx, getState());
  enterRule(_localctx, 18, sqlParser::RuleQueryHint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(390);
    match(sqlParser::HINT_START);
    setState(392);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::ANALYSE)
      | (1ULL << sqlParser::ANALYZE)
      | (1ULL << sqlParser::ANIDX)
      | (1ULL << sqlParser::ANTBL)
      | (1ULL << sqlParser::ASC)
      | (1ULL << sqlParser::BIT)
      | (1ULL << sqlParser::BITS)
      | (1ULL << sqlParser::BLOCK)
      | (1ULL << sqlParser::BLOOM)
      | (1ULL << sqlParser::BPA)
      | (1ULL << sqlParser::BTREE)
      | (1ULL << sqlParser::BYE)
      | (1ULL << sqlParser::BYTE)
      | (1ULL << sqlParser::BYTES)
      | (1ULL << sqlParser::CATALOG)
      | (1ULL << sqlParser::CHARS)
      | (1ULL << sqlParser::COMPACTIDX)
      | (1ULL << sqlParser::CONCAT))) != 0) || ((((_la - 68) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 68)) & ((1ULL << (sqlParser::DATA - 68))
      | (1ULL << (sqlParser::DATE_FORMAT - 68))
      | (1ULL << (sqlParser::DAYS - 68))
      | (1ULL << (sqlParser::DECODE - 68))
      | (1ULL << (sqlParser::DESC - 68))
      | (1ULL << (sqlParser::DETAIL - 68))
      | (1ULL << (sqlParser::DIGITS - 68))
      | (1ULL << (sqlParser::DLOAD - 68))
      | (1ULL << (sqlParser::DOCOL - 68))
      | (1ULL << (sqlParser::ECHO - 68))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 68))
      | (1ULL << (sqlParser::EXIT_TOKEN - 68))
      | (1ULL << (sqlParser::EXPLAIN - 68))
      | (1ULL << (sqlParser::EXPORT - 68))
      | (1ULL << (sqlParser::SQL_FALSE - 68))
      | (1ULL << (sqlParser::FEDERATION - 68))
      | (1ULL << (sqlParser::FIELD_DELIMITER - 68))
      | (1ULL << (sqlParser::FILE - 68))
      | (1ULL << (sqlParser::FORCE - 68))
      | (1ULL << (sqlParser::FORMAT - 68))
      | (1ULL << (sqlParser::HASH - 68))
      | (1ULL << (sqlParser::HASHKEY - 68))
      | (1ULL << (sqlParser::HOURS - 68))
      | (1ULL << (sqlParser::IF - 68))
      | (1ULL << (sqlParser::IFNULL - 68))
      | (1ULL << (sqlParser::INDEX - 68))
      | (1ULL << (sqlParser::INTERVAL_DT - 68))
      | (1ULL << (sqlParser::INTERVAL_YM - 68))
      | (1ULL << (sqlParser::KEY - 68))
      | (1ULL << (sqlParser::LIMIT - 68))
      | (1ULL << (sqlParser::LOADREPLICATED - 68)))) != 0) || ((((_la - 132) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 132)) & ((1ULL << (sqlParser::LOAD - 132))
      | (1ULL << (sqlParser::LOG - 132))
      | (1ULL << (sqlParser::LOOP - 132))
      | (1ULL << (sqlParser::LSIDX - 132))
      | (1ULL << (sqlParser::LSPTNCOL - 132))
      | (1ULL << (sqlParser::LSTBL - 132))
      | (1ULL << (sqlParser::MEMORYLEAKS - 132))
      | (1ULL << (sqlParser::MILLIS - 132))
      | (1ULL << (sqlParser::MILLISECOND - 132))
      | (1ULL << (sqlParser::MILLISECONDS - 132))
      | (1ULL << (sqlParser::MINUTES - 132))
      | (1ULL << (sqlParser::MONOTONIC - 132))
      | (1ULL << (sqlParser::MONTHS - 132))
      | (1ULL << (sqlParser::MS - 132))
      | (1ULL << (sqlParser::NODES - 132))
      | (1ULL << (sqlParser::NODE - 132))
      | (1ULL << (sqlParser::NOOP - 132))
      | (1ULL << (sqlParser::NOP - 132))
      | (1ULL << (sqlParser::NOW - 132))
      | (1ULL << (sqlParser::OBJECTS - 132))
      | (1ULL << (sqlParser::OFF - 132))
      | (1ULL << (sqlParser::OFFSET - 132))
      | (1ULL << (sqlParser::ORIGIN - 132))
      | (1ULL << (sqlParser::PARTITIONS - 132))
      | (1ULL << (sqlParser::PATHS - 132))
      | (1ULL << (sqlParser::PIPELINE - 132))
      | (1ULL << (sqlParser::PLACING - 132))
      | (1ULL << (sqlParser::PRETTY - 132))
      | (1ULL << (sqlParser::QUARTER - 132))
      | (1ULL << (sqlParser::QUARTERS - 132))
      | (1ULL << (sqlParser::RECORD_ENDS - 132))
      | (1ULL << (sqlParser::REFERENCE - 132))
      | (1ULL << (sqlParser::REGIONS - 132))
      | (1ULL << (sqlParser::REPLICATED - 132))
      | (1ULL << (sqlParser::ROWSIZE - 132))
      | (1ULL << (sqlParser::SCHEMA - 132))
      | (1ULL << (sqlParser::SEC - 132))
      | (1ULL << (sqlParser::SECONDS - 132)))) != 0) || ((((_la - 197) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 197)) & ((1ULL << (sqlParser::SKIPLIST - 197))
      | (1ULL << (sqlParser::SLEEP - 197))
      | (1ULL << (sqlParser::SPACE - 197))
      | (1ULL << (sqlParser::STAGE - 197))
      | (1ULL << (sqlParser::STATS - 197))
      | (1ULL << (sqlParser::STATISTICS - 197))
      | (1ULL << (sqlParser::STATUS - 197))
      | (1ULL << (sqlParser::SUBSTR - 197))
      | (1ULL << (sqlParser::SZIDX - 197))
      | (1ULL << (sqlParser::SZTBL - 197))
      | (1ULL << (sqlParser::S_ONLY - 197))
      | (1ULL << (sqlParser::TABLESIZE - 197))
      | (1ULL << (sqlParser::TEXT - 197))
      | (1ULL << (sqlParser::TOP - 197))
      | (1ULL << (sqlParser::TRACKMEMORY - 197))
      | (1ULL << (sqlParser::TREE - 197))
      | (1ULL << (sqlParser::TRUNCATE - 197))
      | (1ULL << (sqlParser::TYPE - 197))
      | (1ULL << (sqlParser::TYPEOF - 197))
      | (1ULL << (sqlParser::UNLOAD - 197))
      | (1ULL << (sqlParser::VALUE - 197))
      | (1ULL << (sqlParser::WEEK - 197))
      | (1ULL << (sqlParser::WEEKS - 197))
      | (1ULL << (sqlParser::WITH_NAMES - 197))
      | (1ULL << (sqlParser::WITH_TYPES - 197))
      | (1ULL << (sqlParser::XOR - 197))
      | (1ULL << (sqlParser::YEARS - 197))
      | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 197))
      | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 197))
      | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 197)))) != 0)) {
      setState(391);
      hint();
    }
    setState(398);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(394);
      match(sqlParser::T__5);
      setState(395);
      hint();
      setState(400);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(401);
    match(sqlParser::HINT_END);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- HintContext ------------------------------------------------------------------

sqlParser::HintContext::HintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::HintNameContext* sqlParser::HintContext::hintName() {
  return getRuleContext<sqlParser::HintNameContext>(0);
}

std::vector<sqlParser::ExprContext *> sqlParser::HintContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::HintContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::ExprHintContext* sqlParser::HintContext::exprHint() {
  return getRuleContext<sqlParser::ExprHintContext>(0);
}

std::vector<sqlParser::HintValueContext *> sqlParser::HintContext::hintValue() {
  return getRuleContexts<sqlParser::HintValueContext>();
}

sqlParser::HintValueContext* sqlParser::HintContext::hintValue(size_t i) {
  return getRuleContext<sqlParser::HintValueContext>(i);
}


size_t sqlParser::HintContext::getRuleIndex() const {
  return sqlParser::RuleHint;
}


antlrcpp::Any sqlParser::HintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitHint(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::HintContext* sqlParser::hint() {
  HintContext *_localctx = _tracker.createInstance<HintContext>(_ctx, getState());
  enterRule(_localctx, 20, sqlParser::RuleHint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(442);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(403);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(404);
      dynamic_cast<HintContext *>(_localctx)->exprName = exprHint();
      setState(405);
      match(sqlParser::T__3);
      setState(406);
      expr(0);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(408);
      dynamic_cast<HintContext *>(_localctx)->exprName = exprHint();
      setState(410);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__3) {
        setState(409);
        match(sqlParser::T__3);
      }
      setState(412);
      match(sqlParser::T__6);
      setState(413);
      expr(0);
      setState(418);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(414);
        match(sqlParser::T__5);
        setState(415);
        expr(0);
        setState(420);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(421);
      match(sqlParser::T__7);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(423);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(424);
      match(sqlParser::T__3);
      setState(425);
      hintValue();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(427);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(429);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__3) {
        setState(428);
        match(sqlParser::T__3);
      }
      setState(431);
      match(sqlParser::T__6);
      setState(432);
      hintValue();
      setState(437);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(433);
        match(sqlParser::T__5);
        setState(434);
        hintValue();
        setState(439);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(440);
      match(sqlParser::T__7);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectQueryContext ------------------------------------------------------------------

sqlParser::SelectQueryContext::SelectQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::SelectExprContext* sqlParser::SelectQueryContext::selectExpr() {
  return getRuleContext<sqlParser::SelectExprContext>(0);
}

tree::TerminalNode* sqlParser::SelectQueryContext::WITH() {
  return getToken(sqlParser::WITH, 0);
}

std::vector<sqlParser::WithQueryContext *> sqlParser::SelectQueryContext::withQuery() {
  return getRuleContexts<sqlParser::WithQueryContext>();
}

sqlParser::WithQueryContext* sqlParser::SelectQueryContext::withQuery(size_t i) {
  return getRuleContext<sqlParser::WithQueryContext>(i);
}

tree::TerminalNode* sqlParser::SelectQueryContext::ORDER() {
  return getToken(sqlParser::ORDER, 0);
}

tree::TerminalNode* sqlParser::SelectQueryContext::BY() {
  return getToken(sqlParser::BY, 0);
}

tree::TerminalNode* sqlParser::SelectQueryContext::LIMIT() {
  return getToken(sqlParser::LIMIT, 0);
}

sqlParser::OrderByListContext* sqlParser::SelectQueryContext::orderByList() {
  return getRuleContext<sqlParser::OrderByListContext>(0);
}

std::vector<tree::TerminalNode *> sqlParser::SelectQueryContext::INTEGER_LITERAL() {
  return getTokens(sqlParser::INTEGER_LITERAL);
}

tree::TerminalNode* sqlParser::SelectQueryContext::INTEGER_LITERAL(size_t i) {
  return getToken(sqlParser::INTEGER_LITERAL, i);
}

tree::TerminalNode* sqlParser::SelectQueryContext::OFFSET() {
  return getToken(sqlParser::OFFSET, 0);
}


size_t sqlParser::SelectQueryContext::getRuleIndex() const {
  return sqlParser::RuleSelectQuery;
}


antlrcpp::Any sqlParser::SelectQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSelectQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SelectQueryContext* sqlParser::selectQuery() {
  SelectQueryContext *_localctx = _tracker.createInstance<SelectQueryContext>(_ctx, getState());
  enterRule(_localctx, 22, sqlParser::RuleSelectQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(453);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(444);
      match(sqlParser::WITH);
      setState(445);
      withQuery();
      setState(450);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(446);
        match(sqlParser::T__5);
        setState(447);
        withQuery();
        setState(452);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(455);
    selectExpr();
    setState(459);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(456);
      match(sqlParser::ORDER);
      setState(457);
      match(sqlParser::BY);
      setState(458);
      dynamic_cast<SelectQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(467);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(461);
      match(sqlParser::LIMIT);
      setState(462);
      dynamic_cast<SelectQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(465);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(463);
        match(sqlParser::OFFSET);
        setState(464);
        dynamic_cast<SelectQueryContext *>(_localctx)->offset = match(sqlParser::INTEGER_LITERAL);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectExprContext ------------------------------------------------------------------

sqlParser::SelectExprContext::SelectExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::SelectListContext* sqlParser::SelectExprContext::selectList() {
  return getRuleContext<sqlParser::SelectListContext>(0);
}

tree::TerminalNode* sqlParser::SelectExprContext::SELECT() {
  return getToken(sqlParser::SELECT, 0);
}

tree::TerminalNode* sqlParser::SelectExprContext::ALL() {
  return getToken(sqlParser::ALL, 0);
}

tree::TerminalNode* sqlParser::SelectExprContext::FROM() {
  return getToken(sqlParser::FROM, 0);
}

sqlParser::TableExprContext* sqlParser::SelectExprContext::tableExpr() {
  return getRuleContext<sqlParser::TableExprContext>(0);
}

tree::TerminalNode* sqlParser::SelectExprContext::WHERE() {
  return getToken(sqlParser::WHERE, 0);
}

tree::TerminalNode* sqlParser::SelectExprContext::GROUP() {
  return getToken(sqlParser::GROUP, 0);
}

tree::TerminalNode* sqlParser::SelectExprContext::BY() {
  return getToken(sqlParser::BY, 0);
}

tree::TerminalNode* sqlParser::SelectExprContext::HAVING() {
  return getToken(sqlParser::HAVING, 0);
}

std::vector<sqlParser::PredicateContext *> sqlParser::SelectExprContext::predicate() {
  return getRuleContexts<sqlParser::PredicateContext>();
}

sqlParser::PredicateContext* sqlParser::SelectExprContext::predicate(size_t i) {
  return getRuleContext<sqlParser::PredicateContext>(i);
}

sqlParser::GroupByListContext* sqlParser::SelectExprContext::groupByList() {
  return getRuleContext<sqlParser::GroupByListContext>(0);
}

tree::TerminalNode* sqlParser::SelectExprContext::DISTINCT() {
  return getToken(sqlParser::DISTINCT, 0);
}

sqlParser::SelectExprContext* sqlParser::SelectExprContext::selectExpr() {
  return getRuleContext<sqlParser::SelectExprContext>(0);
}


size_t sqlParser::SelectExprContext::getRuleIndex() const {
  return sqlParser::RuleSelectExpr;
}


antlrcpp::Any sqlParser::SelectExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSelectExpr(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SelectExprContext* sqlParser::selectExpr() {
  SelectExprContext *_localctx = _tracker.createInstance<SelectExprContext>(_ctx, getState());
  enterRule(_localctx, 24, sqlParser::RuleSelectExpr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(496);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__8:
      case sqlParser::SELECT: {
        enterOuterAlt(_localctx, 1);
        setState(469);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__8 || _la == sqlParser::SELECT)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(472);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::ALL: {
            setState(470);
            match(sqlParser::ALL);
            break;
          }

          case sqlParser::DISTINCT: {
            setState(471);
            dynamic_cast<SelectExprContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
            break;
          }

          case sqlParser::T__6:
          case sqlParser::T__10:
          case sqlParser::T__11:
          case sqlParser::T__14:
          case sqlParser::ANALYSE:
          case sqlParser::ANALYZE:
          case sqlParser::ANIDX:
          case sqlParser::ANTBL:
          case sqlParser::ASC:
          case sqlParser::AVG:
          case sqlParser::BIT:
          case sqlParser::BITS:
          case sqlParser::BLOCK:
          case sqlParser::BLOOM:
          case sqlParser::BPA:
          case sqlParser::BTREE:
          case sqlParser::BYE:
          case sqlParser::BYTE:
          case sqlParser::BYTES:
          case sqlParser::CASE:
          case sqlParser::CAST:
          case sqlParser::CATALOG:
          case sqlParser::CHARS:
          case sqlParser::COALESCE:
          case sqlParser::COMPACTIDX:
          case sqlParser::CONCAT:
          case sqlParser::COUNT:
          case sqlParser::CURRENT_CATALOG:
          case sqlParser::CURRENT_SCHEMA:
          case sqlParser::CURRENT_TIMESTAMP:
          case sqlParser::CURRENT_USER:
          case sqlParser::DATA:
          case sqlParser::DATE:
          case sqlParser::DATE_FORMAT:
          case sqlParser::DAYS:
          case sqlParser::DECODE:
          case sqlParser::DESC:
          case sqlParser::DETAIL:
          case sqlParser::DIGITS:
          case sqlParser::DLOAD:
          case sqlParser::DOCOL:
          case sqlParser::ECHO:
          case sqlParser::ESCAPE_CHAR:
          case sqlParser::EXISTS:
          case sqlParser::EXIT_TOKEN:
          case sqlParser::EXPLAIN:
          case sqlParser::EXPORT:
          case sqlParser::EXTRACT:
          case sqlParser::SQL_FALSE:
          case sqlParser::FEDERATION:
          case sqlParser::FIELD_DELIMITER:
          case sqlParser::FILE:
          case sqlParser::FORCE:
          case sqlParser::FORMAT:
          case sqlParser::HASH:
          case sqlParser::HASHKEY:
          case sqlParser::HOURS:
          case sqlParser::IF:
          case sqlParser::IFNULL:
          case sqlParser::INDEX:
          case sqlParser::INTERVAL_DT:
          case sqlParser::INTERVAL_YM:
          case sqlParser::INTERVAL:
          case sqlParser::KEY:
          case sqlParser::LIMIT:
          case sqlParser::LOADREPLICATED:
          case sqlParser::LOAD:
          case sqlParser::LOG:
          case sqlParser::LOOP:
          case sqlParser::LSIDX:
          case sqlParser::LSPTNCOL:
          case sqlParser::LSTBL:
          case sqlParser::MAX:
          case sqlParser::MEMORYLEAKS:
          case sqlParser::MILLIS:
          case sqlParser::MILLISECOND:
          case sqlParser::MILLISECONDS:
          case sqlParser::MIN:
          case sqlParser::MINUTES:
          case sqlParser::MONOTONIC:
          case sqlParser::MONTHS:
          case sqlParser::MS:
          case sqlParser::NODES:
          case sqlParser::NODE:
          case sqlParser::NOOP:
          case sqlParser::NOP:
          case sqlParser::NOT:
          case sqlParser::NOW:
          case sqlParser::SQL_NULL:
          case sqlParser::NULLIF:
          case sqlParser::OBJECTS:
          case sqlParser::OFF:
          case sqlParser::OFFSET:
          case sqlParser::ORIGIN:
          case sqlParser::OVERLAY:
          case sqlParser::PARTITIONS:
          case sqlParser::PATHS:
          case sqlParser::PIPELINE:
          case sqlParser::PLACING:
          case sqlParser::POSITION:
          case sqlParser::PRETTY:
          case sqlParser::QUARTER:
          case sqlParser::QUARTERS:
          case sqlParser::RECORD_ENDS:
          case sqlParser::REFERENCE:
          case sqlParser::REGIONS:
          case sqlParser::REPLICATED:
          case sqlParser::ROWSIZE:
          case sqlParser::SCHEMA:
          case sqlParser::SEC:
          case sqlParser::SECONDS:
          case sqlParser::SKIPLIST:
          case sqlParser::SLEEP:
          case sqlParser::SPACE:
          case sqlParser::STAGE:
          case sqlParser::STATS:
          case sqlParser::STATISTICS:
          case sqlParser::STATUS:
          case sqlParser::SUBSTR:
          case sqlParser::SUBSTRING:
          case sqlParser::SUM:
          case sqlParser::SZIDX:
          case sqlParser::SZTBL:
          case sqlParser::S_ONLY:
          case sqlParser::TABLESIZE:
          case sqlParser::TEXT:
          case sqlParser::TIME:
          case sqlParser::TIMESTAMP:
          case sqlParser::TOP:
          case sqlParser::TRACKMEMORY:
          case sqlParser::TREE:
          case sqlParser::TRIM:
          case sqlParser::TRUNCATE:
          case sqlParser::SQL_TRUE:
          case sqlParser::TYPE:
          case sqlParser::TYPEOF:
          case sqlParser::UNLOAD:
          case sqlParser::VALUE:
          case sqlParser::WEEK:
          case sqlParser::WEEKS:
          case sqlParser::WITH_NAMES:
          case sqlParser::WITH_TYPES:
          case sqlParser::XOR:
          case sqlParser::YEARS:
          case sqlParser::PLAIN_IDENTIFIER:
          case sqlParser::BAKTIKD_IDENTIFIER:
          case sqlParser::DUBLQUOTED_IDENTIFIER:
          case sqlParser::SINGLQUOTED_STRING:
          case sqlParser::DUBLQUOTED_STRING:
          case sqlParser::FLOAT_LITERAL:
          case sqlParser::DECIMAL_LITERAL:
          case sqlParser::INTEGER_LITERAL: {
            break;
          }

        default:
          break;
        }
        setState(474);
        selectList();
        setState(477);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
        case 1: {
          setState(475);
          match(sqlParser::FROM);
          setState(476);
          tableExpr(0);
          break;
        }

        }
        setState(481);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
        case 1: {
          setState(479);
          match(sqlParser::WHERE);
          setState(480);
          dynamic_cast<SelectExprContext *>(_localctx)->where = predicate();
          break;
        }

        }
        setState(486);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
        case 1: {
          setState(483);
          match(sqlParser::GROUP);
          setState(484);
          match(sqlParser::BY);
          setState(485);
          dynamic_cast<SelectExprContext *>(_localctx)->groupBy = groupByList();
          break;
        }

        }
        setState(490);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
        case 1: {
          setState(488);
          match(sqlParser::HAVING);
          setState(489);
          dynamic_cast<SelectExprContext *>(_localctx)->having = predicate();
          break;
        }

        }
        break;
      }

      case sqlParser::T__6: {
        enterOuterAlt(_localctx, 2);
        setState(492);
        match(sqlParser::T__6);
        setState(493);
        selectExpr();
        setState(494);
        match(sqlParser::T__7);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SetQueryContext ------------------------------------------------------------------

sqlParser::SetQueryContext::SetQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::SetExprContext* sqlParser::SetQueryContext::setExpr() {
  return getRuleContext<sqlParser::SetExprContext>(0);
}

tree::TerminalNode* sqlParser::SetQueryContext::WITH() {
  return getToken(sqlParser::WITH, 0);
}

std::vector<sqlParser::WithQueryContext *> sqlParser::SetQueryContext::withQuery() {
  return getRuleContexts<sqlParser::WithQueryContext>();
}

sqlParser::WithQueryContext* sqlParser::SetQueryContext::withQuery(size_t i) {
  return getRuleContext<sqlParser::WithQueryContext>(i);
}

tree::TerminalNode* sqlParser::SetQueryContext::ORDER() {
  return getToken(sqlParser::ORDER, 0);
}

tree::TerminalNode* sqlParser::SetQueryContext::BY() {
  return getToken(sqlParser::BY, 0);
}

tree::TerminalNode* sqlParser::SetQueryContext::LIMIT() {
  return getToken(sqlParser::LIMIT, 0);
}

sqlParser::OrderByListContext* sqlParser::SetQueryContext::orderByList() {
  return getRuleContext<sqlParser::OrderByListContext>(0);
}

std::vector<tree::TerminalNode *> sqlParser::SetQueryContext::INTEGER_LITERAL() {
  return getTokens(sqlParser::INTEGER_LITERAL);
}

tree::TerminalNode* sqlParser::SetQueryContext::INTEGER_LITERAL(size_t i) {
  return getToken(sqlParser::INTEGER_LITERAL, i);
}

tree::TerminalNode* sqlParser::SetQueryContext::OFFSET() {
  return getToken(sqlParser::OFFSET, 0);
}


size_t sqlParser::SetQueryContext::getRuleIndex() const {
  return sqlParser::RuleSetQuery;
}


antlrcpp::Any sqlParser::SetQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSetQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SetQueryContext* sqlParser::setQuery() {
  SetQueryContext *_localctx = _tracker.createInstance<SetQueryContext>(_ctx, getState());
  enterRule(_localctx, 26, sqlParser::RuleSetQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(507);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(498);
      match(sqlParser::WITH);
      setState(499);
      withQuery();
      setState(504);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(500);
        match(sqlParser::T__5);
        setState(501);
        withQuery();
        setState(506);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(509);
    setExpr(0);
    setState(513);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(510);
      match(sqlParser::ORDER);
      setState(511);
      match(sqlParser::BY);
      setState(512);
      dynamic_cast<SetQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(521);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(515);
      match(sqlParser::LIMIT);
      setState(516);
      dynamic_cast<SetQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(519);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(517);
        match(sqlParser::OFFSET);
        setState(518);
        dynamic_cast<SetQueryContext *>(_localctx)->offset = match(sqlParser::INTEGER_LITERAL);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SetExprContext ------------------------------------------------------------------

sqlParser::SetExprContext::SetExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::SelectExprContext* sqlParser::SetExprContext::selectExpr() {
  return getRuleContext<sqlParser::SelectExprContext>(0);
}

std::vector<sqlParser::SetExprContext *> sqlParser::SetExprContext::setExpr() {
  return getRuleContexts<sqlParser::SetExprContext>();
}

sqlParser::SetExprContext* sqlParser::SetExprContext::setExpr(size_t i) {
  return getRuleContext<sqlParser::SetExprContext>(i);
}

tree::TerminalNode* sqlParser::SetExprContext::UNION() {
  return getToken(sqlParser::UNION, 0);
}

tree::TerminalNode* sqlParser::SetExprContext::EXCEPT() {
  return getToken(sqlParser::EXCEPT, 0);
}

tree::TerminalNode* sqlParser::SetExprContext::INTERSECT() {
  return getToken(sqlParser::INTERSECT, 0);
}

tree::TerminalNode* sqlParser::SetExprContext::DISTINCT() {
  return getToken(sqlParser::DISTINCT, 0);
}

sqlParser::CorrClauseContext* sqlParser::SetExprContext::corrClause() {
  return getRuleContext<sqlParser::CorrClauseContext>(0);
}

tree::TerminalNode* sqlParser::SetExprContext::ALL() {
  return getToken(sqlParser::ALL, 0);
}


size_t sqlParser::SetExprContext::getRuleIndex() const {
  return sqlParser::RuleSetExpr;
}


antlrcpp::Any sqlParser::SetExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSetExpr(this);
  else
    return visitor->visitChildren(this);
}


sqlParser::SetExprContext* sqlParser::setExpr() {
   return setExpr(0);
}

sqlParser::SetExprContext* sqlParser::setExpr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  sqlParser::SetExprContext *_localctx = _tracker.createInstance<SetExprContext>(_ctx, parentState);
  sqlParser::SetExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 28;
  enterRecursionRule(_localctx, 28, sqlParser::RuleSetExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(529);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
    case 1: {
      setState(524);
      selectExpr();
      break;
    }

    case 2: {
      setState(525);
      match(sqlParser::T__6);
      setState(526);
      setExpr(0);
      setState(527);
      match(sqlParser::T__7);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(543);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<SetExprContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleSetExpr);
        setState(531);

        if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
        setState(532);
        dynamic_cast<SetExprContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == sqlParser::EXCEPT

        || _la == sqlParser::INTERSECT || _la == sqlParser::UNION)) {
          dynamic_cast<SetExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(535);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::DISTINCT: {
            setState(533);
            match(sqlParser::DISTINCT);
            break;
          }

          case sqlParser::ALL: {
            setState(534);
            dynamic_cast<SetExprContext *>(_localctx)->all = match(sqlParser::ALL);
            break;
          }

          case sqlParser::T__6:
          case sqlParser::T__8:
          case sqlParser::CORRESPONDING:
          case sqlParser::SELECT: {
            break;
          }

        default:
          break;
        }
        setState(538);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::CORRESPONDING) {
          setState(537);
          corrClause();
        }
        setState(540);
        setExpr(3); 
      }
      setState(545);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- CorrClauseContext ------------------------------------------------------------------

sqlParser::CorrClauseContext::CorrClauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::CorrClauseContext::CORRESPONDING() {
  return getToken(sqlParser::CORRESPONDING, 0);
}

tree::TerminalNode* sqlParser::CorrClauseContext::BY() {
  return getToken(sqlParser::BY, 0);
}

sqlParser::CorrListContext* sqlParser::CorrClauseContext::corrList() {
  return getRuleContext<sqlParser::CorrListContext>(0);
}


size_t sqlParser::CorrClauseContext::getRuleIndex() const {
  return sqlParser::RuleCorrClause;
}


antlrcpp::Any sqlParser::CorrClauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCorrClause(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CorrClauseContext* sqlParser::corrClause() {
  CorrClauseContext *_localctx = _tracker.createInstance<CorrClauseContext>(_ctx, getState());
  enterRule(_localctx, 30, sqlParser::RuleCorrClause);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(546);
    dynamic_cast<CorrClauseContext *>(_localctx)->corr = match(sqlParser::CORRESPONDING);
    setState(552);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::BY) {
      setState(547);
      match(sqlParser::BY);
      setState(548);
      match(sqlParser::T__6);
      setState(549);
      corrList();
      setState(550);
      match(sqlParser::T__7);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WithQueryContext ------------------------------------------------------------------

sqlParser::WithQueryContext::WithQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::WithQueryContext::AS() {
  return getToken(sqlParser::AS, 0);
}

sqlParser::QueryContext* sqlParser::WithQueryContext::query() {
  return getRuleContext<sqlParser::QueryContext>(0);
}

sqlParser::TableNameContext* sqlParser::WithQueryContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

tree::TerminalNode* sqlParser::WithQueryContext::RECURSIVE() {
  return getToken(sqlParser::RECURSIVE, 0);
}

sqlParser::ColAliasesContext* sqlParser::WithQueryContext::colAliases() {
  return getRuleContext<sqlParser::ColAliasesContext>(0);
}


size_t sqlParser::WithQueryContext::getRuleIndex() const {
  return sqlParser::RuleWithQuery;
}


antlrcpp::Any sqlParser::WithQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitWithQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::WithQueryContext* sqlParser::withQuery() {
  WithQueryContext *_localctx = _tracker.createInstance<WithQueryContext>(_ctx, getState());
  enterRule(_localctx, 32, sqlParser::RuleWithQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(555);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::RECURSIVE) {
      setState(554);
      match(sqlParser::RECURSIVE);
    }
    setState(557);
    dynamic_cast<WithQueryContext *>(_localctx)->name = tableName();
    setState(559);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__6) {
      setState(558);
      colAliases();
    }
    setState(561);
    match(sqlParser::AS);
    setState(562);
    match(sqlParser::T__6);
    setState(563);
    query();
    setState(564);
    match(sqlParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TableExprContext ------------------------------------------------------------------

sqlParser::TableExprContext::TableExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t sqlParser::TableExprContext::getRuleIndex() const {
  return sqlParser::RuleTableExpr;
}

void sqlParser::TableExprContext::copyFrom(TableExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- SubqueryContext ------------------------------------------------------------------

sqlParser::QueryContext* sqlParser::SubqueryContext::query() {
  return getRuleContext<sqlParser::QueryContext>(0);
}

sqlParser::TableAliasContext* sqlParser::SubqueryContext::tableAlias() {
  return getRuleContext<sqlParser::TableAliasContext>(0);
}

tree::TerminalNode* sqlParser::SubqueryContext::AS() {
  return getToken(sqlParser::AS, 0);
}

sqlParser::ColAliasesContext* sqlParser::SubqueryContext::colAliases() {
  return getRuleContext<sqlParser::ColAliasesContext>(0);
}

sqlParser::SubqueryContext::SubqueryContext(TableExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::SubqueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSubquery(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NaturalJoinContext ------------------------------------------------------------------

std::vector<sqlParser::TableExprContext *> sqlParser::NaturalJoinContext::tableExpr() {
  return getRuleContexts<sqlParser::TableExprContext>();
}

sqlParser::TableExprContext* sqlParser::NaturalJoinContext::tableExpr(size_t i) {
  return getRuleContext<sqlParser::TableExprContext>(i);
}

tree::TerminalNode* sqlParser::NaturalJoinContext::NATURAL() {
  return getToken(sqlParser::NATURAL, 0);
}

tree::TerminalNode* sqlParser::NaturalJoinContext::JOIN() {
  return getToken(sqlParser::JOIN, 0);
}

std::vector<sqlParser::QueryHintContext *> sqlParser::NaturalJoinContext::queryHint() {
  return getRuleContexts<sqlParser::QueryHintContext>();
}

sqlParser::QueryHintContext* sqlParser::NaturalJoinContext::queryHint(size_t i) {
  return getRuleContext<sqlParser::QueryHintContext>(i);
}

sqlParser::JoinTypeContext* sqlParser::NaturalJoinContext::joinType() {
  return getRuleContext<sqlParser::JoinTypeContext>(0);
}

sqlParser::NaturalJoinContext::NaturalJoinContext(TableExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::NaturalJoinContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitNaturalJoin(this);
  else
    return visitor->visitChildren(this);
}
//----------------- JoinUsingContext ------------------------------------------------------------------

std::vector<sqlParser::TableExprContext *> sqlParser::JoinUsingContext::tableExpr() {
  return getRuleContexts<sqlParser::TableExprContext>();
}

sqlParser::TableExprContext* sqlParser::JoinUsingContext::tableExpr(size_t i) {
  return getRuleContext<sqlParser::TableExprContext>(i);
}

tree::TerminalNode* sqlParser::JoinUsingContext::JOIN() {
  return getToken(sqlParser::JOIN, 0);
}

tree::TerminalNode* sqlParser::JoinUsingContext::USING() {
  return getToken(sqlParser::USING, 0);
}

sqlParser::JoinUsingListContext* sqlParser::JoinUsingContext::joinUsingList() {
  return getRuleContext<sqlParser::JoinUsingListContext>(0);
}

std::vector<sqlParser::QueryHintContext *> sqlParser::JoinUsingContext::queryHint() {
  return getRuleContexts<sqlParser::QueryHintContext>();
}

sqlParser::QueryHintContext* sqlParser::JoinUsingContext::queryHint(size_t i) {
  return getRuleContext<sqlParser::QueryHintContext>(i);
}

sqlParser::ExtendedJoinTypeContext* sqlParser::JoinUsingContext::extendedJoinType() {
  return getRuleContext<sqlParser::ExtendedJoinTypeContext>(0);
}

sqlParser::JoinUsingContext::JoinUsingContext(TableExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::JoinUsingContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitJoinUsing(this);
  else
    return visitor->visitChildren(this);
}
//----------------- JoinOnContext ------------------------------------------------------------------

std::vector<sqlParser::TableExprContext *> sqlParser::JoinOnContext::tableExpr() {
  return getRuleContexts<sqlParser::TableExprContext>();
}

sqlParser::TableExprContext* sqlParser::JoinOnContext::tableExpr(size_t i) {
  return getRuleContext<sqlParser::TableExprContext>(i);
}

tree::TerminalNode* sqlParser::JoinOnContext::JOIN() {
  return getToken(sqlParser::JOIN, 0);
}

tree::TerminalNode* sqlParser::JoinOnContext::ON() {
  return getToken(sqlParser::ON, 0);
}

sqlParser::JoinPredicateContext* sqlParser::JoinOnContext::joinPredicate() {
  return getRuleContext<sqlParser::JoinPredicateContext>(0);
}

std::vector<sqlParser::QueryHintContext *> sqlParser::JoinOnContext::queryHint() {
  return getRuleContexts<sqlParser::QueryHintContext>();
}

sqlParser::QueryHintContext* sqlParser::JoinOnContext::queryHint(size_t i) {
  return getRuleContext<sqlParser::QueryHintContext>(i);
}

sqlParser::ExtendedJoinTypeContext* sqlParser::JoinOnContext::extendedJoinType() {
  return getRuleContext<sqlParser::ExtendedJoinTypeContext>(0);
}

sqlParser::JoinOnContext::JoinOnContext(TableExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::JoinOnContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitJoinOn(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FetchContext ------------------------------------------------------------------

sqlParser::TableReferenceContext* sqlParser::FetchContext::tableReference() {
  return getRuleContext<sqlParser::TableReferenceContext>(0);
}

sqlParser::TableAliasContext* sqlParser::FetchContext::tableAlias() {
  return getRuleContext<sqlParser::TableAliasContext>(0);
}

tree::TerminalNode* sqlParser::FetchContext::AS() {
  return getToken(sqlParser::AS, 0);
}

sqlParser::FetchContext::FetchContext(TableExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::FetchContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitFetch(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ValuesSubqueryContext ------------------------------------------------------------------

sqlParser::ValuesExprContext* sqlParser::ValuesSubqueryContext::valuesExpr() {
  return getRuleContext<sqlParser::ValuesExprContext>(0);
}

sqlParser::TableAliasContext* sqlParser::ValuesSubqueryContext::tableAlias() {
  return getRuleContext<sqlParser::TableAliasContext>(0);
}

tree::TerminalNode* sqlParser::ValuesSubqueryContext::AS() {
  return getToken(sqlParser::AS, 0);
}

sqlParser::ColAliasesContext* sqlParser::ValuesSubqueryContext::colAliases() {
  return getRuleContext<sqlParser::ColAliasesContext>(0);
}

sqlParser::ValuesSubqueryContext::ValuesSubqueryContext(TableExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::ValuesSubqueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitValuesSubquery(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CrossJoinContext ------------------------------------------------------------------

std::vector<sqlParser::TableExprContext *> sqlParser::CrossJoinContext::tableExpr() {
  return getRuleContexts<sqlParser::TableExprContext>();
}

sqlParser::TableExprContext* sqlParser::CrossJoinContext::tableExpr(size_t i) {
  return getRuleContext<sqlParser::TableExprContext>(i);
}

std::vector<sqlParser::QueryHintContext *> sqlParser::CrossJoinContext::queryHint() {
  return getRuleContexts<sqlParser::QueryHintContext>();
}

sqlParser::QueryHintContext* sqlParser::CrossJoinContext::queryHint(size_t i) {
  return getRuleContext<sqlParser::QueryHintContext>(i);
}

tree::TerminalNode* sqlParser::CrossJoinContext::CROSS() {
  return getToken(sqlParser::CROSS, 0);
}

tree::TerminalNode* sqlParser::CrossJoinContext::JOIN() {
  return getToken(sqlParser::JOIN, 0);
}

sqlParser::CrossJoinContext::CrossJoinContext(TableExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::CrossJoinContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCrossJoin(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TableExprContext* sqlParser::tableExpr() {
   return tableExpr(0);
}

sqlParser::TableExprContext* sqlParser::tableExpr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  sqlParser::TableExprContext *_localctx = _tracker.createInstance<TableExprContext>(_ctx, parentState);
  sqlParser::TableExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 34;
  enterRecursionRule(_localctx, 34, sqlParser::RuleTableExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(598);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<FetchContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(567);
      dynamic_cast<FetchContext *>(_localctx)->ref = tableReference();
      setState(572);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
      case 1: {
        setState(569);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(568);
          match(sqlParser::AS);
        }
        setState(571);
        dynamic_cast<FetchContext *>(_localctx)->alias = tableAlias();
        break;
      }

      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<SubqueryContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(574);
      match(sqlParser::T__6);
      setState(575);
      query();
      setState(576);
      match(sqlParser::T__7);
      setState(584);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 42, _ctx)) {
      case 1: {
        setState(578);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(577);
          match(sqlParser::AS);
        }
        setState(580);
        dynamic_cast<SubqueryContext *>(_localctx)->as = tableAlias();
        setState(582);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx)) {
        case 1: {
          setState(581);
          colAliases();
          break;
        }

        }
        break;
      }

      }
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ValuesSubqueryContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(586);
      match(sqlParser::T__6);
      setState(587);
      valuesExpr();
      setState(588);
      match(sqlParser::T__7);
      setState(596);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 45, _ctx)) {
      case 1: {
        setState(590);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(589);
          match(sqlParser::AS);
        }
        setState(592);
        dynamic_cast<ValuesSubqueryContext *>(_localctx)->as = tableAlias();
        setState(594);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 44, _ctx)) {
        case 1: {
          setState(593);
          colAliases();
          break;
        }

        }
        break;
      }

      }
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(660);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(658);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 55, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<CrossJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(600);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(604);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(601);
            queryHint();
            setState(606);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(610);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case sqlParser::T__5: {
              setState(607);
              match(sqlParser::T__5);
              break;
            }

            case sqlParser::CROSS: {
              setState(608);
              match(sqlParser::CROSS);
              setState(609);
              match(sqlParser::JOIN);
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(612);
          tableExpr(7);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<NaturalJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(613);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(617);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(614);
            queryHint();
            setState(619);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(620);
          match(sqlParser::NATURAL);

          setState(622);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 103) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 103)) & ((1ULL << (sqlParser::FULL - 103))
            | (1ULL << (sqlParser::INNER - 103))
            | (1ULL << (sqlParser::LEFT - 103)))) != 0) || _la == sqlParser::RIGHT) {
            setState(621);
            dynamic_cast<NaturalJoinContext *>(_localctx)->type = joinType();
          }
          setState(624);
          match(sqlParser::JOIN);
          setState(625);
          tableExpr(6);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<JoinOnContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(626);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(630);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(627);
            queryHint();
            setState(632);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(634);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::ANTI || ((((_la - 103) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 103)) & ((1ULL << (sqlParser::FULL - 103))
            | (1ULL << (sqlParser::INNER - 103))
            | (1ULL << (sqlParser::LEFT - 103)))) != 0) || _la == sqlParser::RIGHT

          || _la == sqlParser::SEMI) {
            setState(633);
            dynamic_cast<JoinOnContext *>(_localctx)->type = extendedJoinType();
          }
          setState(636);
          match(sqlParser::JOIN);
          setState(637);
          tableExpr(0);
          setState(638);
          match(sqlParser::ON);
          setState(639);
          joinPredicate();
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<JoinUsingContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(641);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(645);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(642);
            queryHint();
            setState(647);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(649);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::ANTI || ((((_la - 103) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 103)) & ((1ULL << (sqlParser::FULL - 103))
            | (1ULL << (sqlParser::INNER - 103))
            | (1ULL << (sqlParser::LEFT - 103)))) != 0) || _la == sqlParser::RIGHT

          || _la == sqlParser::SEMI) {
            setState(648);
            dynamic_cast<JoinUsingContext *>(_localctx)->type = extendedJoinType();
          }
          setState(651);
          match(sqlParser::JOIN);
          setState(652);
          tableExpr(0);
          setState(653);
          match(sqlParser::USING);
          setState(654);
          match(sqlParser::T__6);
          setState(655);
          joinUsingList();
          setState(656);
          match(sqlParser::T__7);
          break;
        }

        } 
      }
      setState(662);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- TableReferenceContext ------------------------------------------------------------------

sqlParser::TableReferenceContext::TableReferenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::TableNameContext* sqlParser::TableReferenceContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::SchemaNameContext* sqlParser::TableReferenceContext::schemaName() {
  return getRuleContext<sqlParser::SchemaNameContext>(0);
}

sqlParser::CatalogNameContext* sqlParser::TableReferenceContext::catalogName() {
  return getRuleContext<sqlParser::CatalogNameContext>(0);
}


size_t sqlParser::TableReferenceContext::getRuleIndex() const {
  return sqlParser::RuleTableReference;
}


antlrcpp::Any sqlParser::TableReferenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTableReference(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TableReferenceContext* sqlParser::tableReference() {
  TableReferenceContext *_localctx = _tracker.createInstance<TableReferenceContext>(_ctx, getState());
  enterRule(_localctx, 36, sqlParser::RuleTableReference);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(671);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 58, _ctx)) {
    case 1: {
      setState(666);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 57, _ctx)) {
      case 1: {
        setState(663);
        dynamic_cast<TableReferenceContext *>(_localctx)->catalog = catalogName();
        setState(664);
        match(sqlParser::T__9);
        break;
      }

      }
      setState(668);
      dynamic_cast<TableReferenceContext *>(_localctx)->schema = schemaName();
      setState(669);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(673);
    dynamic_cast<TableReferenceContext *>(_localctx)->table = tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- JoinTypeContext ------------------------------------------------------------------

sqlParser::JoinTypeContext::JoinTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::JoinTypeContext::INNER() {
  return getToken(sqlParser::INNER, 0);
}

tree::TerminalNode* sqlParser::JoinTypeContext::LEFT() {
  return getToken(sqlParser::LEFT, 0);
}

tree::TerminalNode* sqlParser::JoinTypeContext::RIGHT() {
  return getToken(sqlParser::RIGHT, 0);
}

tree::TerminalNode* sqlParser::JoinTypeContext::FULL() {
  return getToken(sqlParser::FULL, 0);
}

tree::TerminalNode* sqlParser::JoinTypeContext::OUTER() {
  return getToken(sqlParser::OUTER, 0);
}


size_t sqlParser::JoinTypeContext::getRuleIndex() const {
  return sqlParser::RuleJoinType;
}


antlrcpp::Any sqlParser::JoinTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitJoinType(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::JoinTypeContext* sqlParser::joinType() {
  JoinTypeContext *_localctx = _tracker.createInstance<JoinTypeContext>(_ctx, getState());
  enterRule(_localctx, 38, sqlParser::RuleJoinType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(680);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::INNER: {
        enterOuterAlt(_localctx, 1);
        setState(675);
        match(sqlParser::INNER);
        break;
      }

      case sqlParser::FULL:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 2);
        setState(676);
        dynamic_cast<JoinTypeContext *>(_localctx)->outer = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == sqlParser::FULL

        || _la == sqlParser::LEFT || _la == sqlParser::RIGHT)) {
          dynamic_cast<JoinTypeContext *>(_localctx)->outer = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(678);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::OUTER) {
          setState(677);
          match(sqlParser::OUTER);
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExtendedJoinTypeContext ------------------------------------------------------------------

sqlParser::ExtendedJoinTypeContext::ExtendedJoinTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::JoinTypeContext* sqlParser::ExtendedJoinTypeContext::joinType() {
  return getRuleContext<sqlParser::JoinTypeContext>(0);
}

tree::TerminalNode* sqlParser::ExtendedJoinTypeContext::SEMI() {
  return getToken(sqlParser::SEMI, 0);
}

tree::TerminalNode* sqlParser::ExtendedJoinTypeContext::ANTI() {
  return getToken(sqlParser::ANTI, 0);
}


size_t sqlParser::ExtendedJoinTypeContext::getRuleIndex() const {
  return sqlParser::RuleExtendedJoinType;
}


antlrcpp::Any sqlParser::ExtendedJoinTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitExtendedJoinType(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ExtendedJoinTypeContext* sqlParser::extendedJoinType() {
  ExtendedJoinTypeContext *_localctx = _tracker.createInstance<ExtendedJoinTypeContext>(_ctx, getState());
  enterRule(_localctx, 40, sqlParser::RuleExtendedJoinType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(685);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::FULL:
      case sqlParser::INNER:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 1);
        setState(682);
        joinType();
        break;
      }

      case sqlParser::SEMI: {
        enterOuterAlt(_localctx, 2);
        setState(683);
        match(sqlParser::SEMI);
        break;
      }

      case sqlParser::ANTI: {
        enterOuterAlt(_localctx, 3);
        setState(684);
        match(sqlParser::ANTI);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValuesExprContext ------------------------------------------------------------------

sqlParser::ValuesExprContext::ValuesExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::ValuesExprContext::VALUES() {
  return getToken(sqlParser::VALUES, 0);
}

std::vector<sqlParser::ValueListContext *> sqlParser::ValuesExprContext::valueList() {
  return getRuleContexts<sqlParser::ValueListContext>();
}

sqlParser::ValueListContext* sqlParser::ValuesExprContext::valueList(size_t i) {
  return getRuleContext<sqlParser::ValueListContext>(i);
}


size_t sqlParser::ValuesExprContext::getRuleIndex() const {
  return sqlParser::RuleValuesExpr;
}


antlrcpp::Any sqlParser::ValuesExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitValuesExpr(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ValuesExprContext* sqlParser::valuesExpr() {
  ValuesExprContext *_localctx = _tracker.createInstance<ValuesExprContext>(_ctx, getState());
  enterRule(_localctx, 42, sqlParser::RuleValuesExpr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(687);
    match(sqlParser::VALUES);
    setState(688);
    match(sqlParser::T__6);
    setState(689);
    valueList();
    setState(690);
    match(sqlParser::T__7);
    setState(698);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(691);
      match(sqlParser::T__5);
      setState(692);
      match(sqlParser::T__6);
      setState(693);
      valueList();
      setState(694);
      match(sqlParser::T__7);
      setState(700);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AnidxCmdContext ------------------------------------------------------------------

sqlParser::AnidxCmdContext::AnidxCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::TableNameContext* sqlParser::AnidxCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::IndexNameContext* sqlParser::AnidxCmdContext::indexName() {
  return getRuleContext<sqlParser::IndexNameContext>(0);
}

tree::TerminalNode* sqlParser::AnidxCmdContext::ANIDX() {
  return getToken(sqlParser::ANIDX, 0);
}

tree::TerminalNode* sqlParser::AnidxCmdContext::TABLE() {
  return getToken(sqlParser::TABLE, 0);
}

tree::TerminalNode* sqlParser::AnidxCmdContext::ANALYZE() {
  return getToken(sqlParser::ANALYZE, 0);
}

tree::TerminalNode* sqlParser::AnidxCmdContext::ANALYSE() {
  return getToken(sqlParser::ANALYSE, 0);
}


size_t sqlParser::AnidxCmdContext::getRuleIndex() const {
  return sqlParser::RuleAnidxCmd;
}


antlrcpp::Any sqlParser::AnidxCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitAnidxCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::AnidxCmdContext* sqlParser::anidxCmd() {
  AnidxCmdContext *_localctx = _tracker.createInstance<AnidxCmdContext>(_ctx, getState());
  enterRule(_localctx, 44, sqlParser::RuleAnidxCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(704);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::ANIDX: {
        setState(701);
        match(sqlParser::ANIDX);
        break;
      }

      case sqlParser::ANALYSE:
      case sqlParser::ANALYZE: {
        setState(702);
        _la = _input->LA(1);
        if (!(_la == sqlParser::ANALYSE

        || _la == sqlParser::ANALYZE)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(703);
        match(sqlParser::TABLE);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(706);
    tableName();
    setState(707);
    indexName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AntblCmdContext ------------------------------------------------------------------

sqlParser::AntblCmdContext::AntblCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::TableNameContext* sqlParser::AntblCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

tree::TerminalNode* sqlParser::AntblCmdContext::ANTBL() {
  return getToken(sqlParser::ANTBL, 0);
}

tree::TerminalNode* sqlParser::AntblCmdContext::INDEX() {
  return getToken(sqlParser::INDEX, 0);
}

tree::TerminalNode* sqlParser::AntblCmdContext::ANALYZE() {
  return getToken(sqlParser::ANALYZE, 0);
}

tree::TerminalNode* sqlParser::AntblCmdContext::ANALYSE() {
  return getToken(sqlParser::ANALYSE, 0);
}


size_t sqlParser::AntblCmdContext::getRuleIndex() const {
  return sqlParser::RuleAntblCmd;
}


antlrcpp::Any sqlParser::AntblCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitAntblCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::AntblCmdContext* sqlParser::antblCmd() {
  AntblCmdContext *_localctx = _tracker.createInstance<AntblCmdContext>(_ctx, getState());
  enterRule(_localctx, 46, sqlParser::RuleAntblCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(712);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::ANTBL: {
        setState(709);
        match(sqlParser::ANTBL);
        break;
      }

      case sqlParser::ANALYSE:
      case sqlParser::ANALYZE: {
        setState(710);
        _la = _input->LA(1);
        if (!(_la == sqlParser::ANALYSE

        || _la == sqlParser::ANALYZE)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(711);
        match(sqlParser::INDEX);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(714);
    tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BpaCmdContext ------------------------------------------------------------------

sqlParser::BpaCmdContext::BpaCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::BpaCmdContext::BPA() {
  return getToken(sqlParser::BPA, 0);
}

tree::TerminalNode* sqlParser::BpaCmdContext::REGIONS() {
  return getToken(sqlParser::REGIONS, 0);
}

tree::TerminalNode* sqlParser::BpaCmdContext::OBJECTS() {
  return getToken(sqlParser::OBJECTS, 0);
}

tree::TerminalNode* sqlParser::BpaCmdContext::ALL() {
  return getToken(sqlParser::ALL, 0);
}


size_t sqlParser::BpaCmdContext::getRuleIndex() const {
  return sqlParser::RuleBpaCmd;
}


antlrcpp::Any sqlParser::BpaCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitBpaCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::BpaCmdContext* sqlParser::bpaCmd() {
  BpaCmdContext *_localctx = _tracker.createInstance<BpaCmdContext>(_ctx, getState());
  enterRule(_localctx, 48, sqlParser::RuleBpaCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(716);
    match(sqlParser::BPA);
    setState(718);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ALL || _la == sqlParser::OBJECTS

    || _la == sqlParser::REGIONS) {
      setState(717);
      dynamic_cast<BpaCmdContext *>(_localctx)->type = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == sqlParser::ALL || _la == sqlParser::OBJECTS

      || _la == sqlParser::REGIONS)) {
        dynamic_cast<BpaCmdContext *>(_localctx)->type = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CompactIdxCmdContext ------------------------------------------------------------------

sqlParser::CompactIdxCmdContext::CompactIdxCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::CompactIdxCmdContext::COMPACTIDX() {
  return getToken(sqlParser::COMPACTIDX, 0);
}

sqlParser::TableNameContext* sqlParser::CompactIdxCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::IndexNameContext* sqlParser::CompactIdxCmdContext::indexName() {
  return getRuleContext<sqlParser::IndexNameContext>(0);
}


size_t sqlParser::CompactIdxCmdContext::getRuleIndex() const {
  return sqlParser::RuleCompactIdxCmd;
}


antlrcpp::Any sqlParser::CompactIdxCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCompactIdxCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CompactIdxCmdContext* sqlParser::compactIdxCmd() {
  CompactIdxCmdContext *_localctx = _tracker.createInstance<CompactIdxCmdContext>(_ctx, getState());
  enterRule(_localctx, 50, sqlParser::RuleCompactIdxCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(720);
    match(sqlParser::COMPACTIDX);
    setState(721);
    tableName();
    setState(722);
    indexName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CountCmdContext ------------------------------------------------------------------

sqlParser::CountCmdContext::CountCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::CountCmdContext::COUNT() {
  return getToken(sqlParser::COUNT, 0);
}

sqlParser::TableNameContext* sqlParser::CountCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}


size_t sqlParser::CountCmdContext::getRuleIndex() const {
  return sqlParser::RuleCountCmd;
}


antlrcpp::Any sqlParser::CountCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCountCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CountCmdContext* sqlParser::countCmd() {
  CountCmdContext *_localctx = _tracker.createInstance<CountCmdContext>(_ctx, getState());
  enterRule(_localctx, 52, sqlParser::RuleCountCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(724);
    match(sqlParser::COUNT);
    setState(725);
    tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CreateCmdContext ------------------------------------------------------------------

sqlParser::CreateCmdContext::CreateCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::CreateTableCmdContext* sqlParser::CreateCmdContext::createTableCmd() {
  return getRuleContext<sqlParser::CreateTableCmdContext>(0);
}

sqlParser::CreateIndexCmdContext* sqlParser::CreateCmdContext::createIndexCmd() {
  return getRuleContext<sqlParser::CreateIndexCmdContext>(0);
}


size_t sqlParser::CreateCmdContext::getRuleIndex() const {
  return sqlParser::RuleCreateCmd;
}


antlrcpp::Any sqlParser::CreateCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCreateCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CreateCmdContext* sqlParser::createCmd() {
  CreateCmdContext *_localctx = _tracker.createInstance<CreateCmdContext>(_ctx, getState());
  enterRule(_localctx, 54, sqlParser::RuleCreateCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(729);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 66, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(727);
      createTableCmd();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(728);
      createIndexCmd();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CreateTableCmdContext ------------------------------------------------------------------

sqlParser::CreateTableCmdContext::CreateTableCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::CreateTableCmdContext::CREATE() {
  return getToken(sqlParser::CREATE, 0);
}

tree::TerminalNode* sqlParser::CreateTableCmdContext::TABLE() {
  return getToken(sqlParser::TABLE, 0);
}

sqlParser::TableNameContext* sqlParser::CreateTableCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::TableDefinitionContext* sqlParser::CreateTableCmdContext::tableDefinition() {
  return getRuleContext<sqlParser::TableDefinitionContext>(0);
}


size_t sqlParser::CreateTableCmdContext::getRuleIndex() const {
  return sqlParser::RuleCreateTableCmd;
}


antlrcpp::Any sqlParser::CreateTableCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCreateTableCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CreateTableCmdContext* sqlParser::createTableCmd() {
  CreateTableCmdContext *_localctx = _tracker.createInstance<CreateTableCmdContext>(_ctx, getState());
  enterRule(_localctx, 56, sqlParser::RuleCreateTableCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(731);
    match(sqlParser::CREATE);
    setState(732);
    match(sqlParser::TABLE);
    setState(733);
    tableName();

    setState(734);
    tableDefinition();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CreateIndexCmdContext ------------------------------------------------------------------

sqlParser::CreateIndexCmdContext::CreateIndexCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::CreateIndexCmdContext::CREATE() {
  return getToken(sqlParser::CREATE, 0);
}

tree::TerminalNode* sqlParser::CreateIndexCmdContext::INDEX() {
  return getToken(sqlParser::INDEX, 0);
}

tree::TerminalNode* sqlParser::CreateIndexCmdContext::ON() {
  return getToken(sqlParser::ON, 0);
}

sqlParser::TableNameContext* sqlParser::CreateIndexCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

std::vector<sqlParser::ExprContext *> sqlParser::CreateIndexCmdContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::CreateIndexCmdContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::IndexNameContext* sqlParser::CreateIndexCmdContext::indexName() {
  return getRuleContext<sqlParser::IndexNameContext>(0);
}

tree::TerminalNode* sqlParser::CreateIndexCmdContext::UNIQUE() {
  return getToken(sqlParser::UNIQUE, 0);
}


size_t sqlParser::CreateIndexCmdContext::getRuleIndex() const {
  return sqlParser::RuleCreateIndexCmd;
}


antlrcpp::Any sqlParser::CreateIndexCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCreateIndexCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CreateIndexCmdContext* sqlParser::createIndexCmd() {
  CreateIndexCmdContext *_localctx = _tracker.createInstance<CreateIndexCmdContext>(_ctx, getState());
  enterRule(_localctx, 58, sqlParser::RuleCreateIndexCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(736);
    match(sqlParser::CREATE);
    setState(738);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(737);
      match(sqlParser::UNIQUE);
    }
    setState(740);
    match(sqlParser::INDEX);
    setState(741);
    dynamic_cast<CreateIndexCmdContext *>(_localctx)->idxName = indexName();
    setState(742);
    match(sqlParser::ON);
    setState(743);
    tableName();
    setState(744);
    match(sqlParser::T__6);
    setState(745);
    expr(0);
    setState(750);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(746);
      match(sqlParser::T__5);
      setState(747);
      expr(0);
      setState(752);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(753);
    match(sqlParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeleteCmdContext ------------------------------------------------------------------

sqlParser::DeleteCmdContext::DeleteCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DeleteCmdContext::DELETE() {
  return getToken(sqlParser::DELETE, 0);
}

sqlParser::TableReferenceContext* sqlParser::DeleteCmdContext::tableReference() {
  return getRuleContext<sqlParser::TableReferenceContext>(0);
}

tree::TerminalNode* sqlParser::DeleteCmdContext::WITH() {
  return getToken(sqlParser::WITH, 0);
}

std::vector<sqlParser::WithQueryContext *> sqlParser::DeleteCmdContext::withQuery() {
  return getRuleContexts<sqlParser::WithQueryContext>();
}

sqlParser::WithQueryContext* sqlParser::DeleteCmdContext::withQuery(size_t i) {
  return getRuleContext<sqlParser::WithQueryContext>(i);
}

std::vector<tree::TerminalNode *> sqlParser::DeleteCmdContext::FROM() {
  return getTokens(sqlParser::FROM);
}

tree::TerminalNode* sqlParser::DeleteCmdContext::FROM(size_t i) {
  return getToken(sqlParser::FROM, i);
}

tree::TerminalNode* sqlParser::DeleteCmdContext::WHERE() {
  return getToken(sqlParser::WHERE, 0);
}

sqlParser::PredicateContext* sqlParser::DeleteCmdContext::predicate() {
  return getRuleContext<sqlParser::PredicateContext>(0);
}

sqlParser::TableExprContext* sqlParser::DeleteCmdContext::tableExpr() {
  return getRuleContext<sqlParser::TableExprContext>(0);
}


size_t sqlParser::DeleteCmdContext::getRuleIndex() const {
  return sqlParser::RuleDeleteCmd;
}


antlrcpp::Any sqlParser::DeleteCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDeleteCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DeleteCmdContext* sqlParser::deleteCmd() {
  DeleteCmdContext *_localctx = _tracker.createInstance<DeleteCmdContext>(_ctx, getState());
  enterRule(_localctx, 60, sqlParser::RuleDeleteCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(764);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(755);
      match(sqlParser::WITH);
      setState(756);
      withQuery();
      setState(761);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(757);
        match(sqlParser::T__5);
        setState(758);
        withQuery();
        setState(763);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(766);
    match(sqlParser::DELETE);
    setState(768);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::FROM) {
      setState(767);
      match(sqlParser::FROM);
    }
    setState(770);
    dynamic_cast<DeleteCmdContext *>(_localctx)->ref = tableReference();
    setState(775);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(771);
        match(sqlParser::WHERE);
        setState(772);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(773);
        match(sqlParser::FROM);
        setState(774);
        tableExpr(0);
        break;
      }

      case sqlParser::EOF: {
        break;
      }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DisconnectCmdContext ------------------------------------------------------------------

sqlParser::DisconnectCmdContext::DisconnectCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DisconnectCmdContext::DISCONNECT() {
  return getToken(sqlParser::DISCONNECT, 0);
}

tree::TerminalNode* sqlParser::DisconnectCmdContext::BYE() {
  return getToken(sqlParser::BYE, 0);
}

tree::TerminalNode* sqlParser::DisconnectCmdContext::EXIT_TOKEN() {
  return getToken(sqlParser::EXIT_TOKEN, 0);
}


size_t sqlParser::DisconnectCmdContext::getRuleIndex() const {
  return sqlParser::RuleDisconnectCmd;
}


antlrcpp::Any sqlParser::DisconnectCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDisconnectCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DisconnectCmdContext* sqlParser::disconnectCmd() {
  DisconnectCmdContext *_localctx = _tracker.createInstance<DisconnectCmdContext>(_ctx, getState());
  enterRule(_localctx, 62, sqlParser::RuleDisconnectCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(777);
    _la = _input->LA(1);
    if (!(((((_la - 46) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 46)) & ((1ULL << (sqlParser::BYE - 46))
      | (1ULL << (sqlParser::DISCONNECT - 46))
      | (1ULL << (sqlParser::EXIT_TOKEN - 46)))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DloadCmdContext ------------------------------------------------------------------

sqlParser::DloadCmdContext::DloadCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DloadCmdContext::DLOAD() {
  return getToken(sqlParser::DLOAD, 0);
}

sqlParser::TableNameContext* sqlParser::DloadCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

tree::TerminalNode* sqlParser::DloadCmdContext::PATHS() {
  return getToken(sqlParser::PATHS, 0);
}

std::vector<sqlParser::StringLiteralContext *> sqlParser::DloadCmdContext::stringLiteral() {
  return getRuleContexts<sqlParser::StringLiteralContext>();
}

sqlParser::StringLiteralContext* sqlParser::DloadCmdContext::stringLiteral(size_t i) {
  return getRuleContext<sqlParser::StringLiteralContext>(i);
}

std::vector<sqlParser::LoadOptionContext *> sqlParser::DloadCmdContext::loadOption() {
  return getRuleContexts<sqlParser::LoadOptionContext>();
}

sqlParser::LoadOptionContext* sqlParser::DloadCmdContext::loadOption(size_t i) {
  return getRuleContext<sqlParser::LoadOptionContext>(i);
}


size_t sqlParser::DloadCmdContext::getRuleIndex() const {
  return sqlParser::RuleDloadCmd;
}


antlrcpp::Any sqlParser::DloadCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDloadCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DloadCmdContext* sqlParser::dloadCmd() {
  DloadCmdContext *_localctx = _tracker.createInstance<DloadCmdContext>(_ctx, getState());
  enterRule(_localctx, 64, sqlParser::RuleDloadCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(779);
    match(sqlParser::DLOAD);
    setState(780);
    tableName();
    setState(781);
    match(sqlParser::PATHS);
    setState(783); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(782);
      stringLiteral();
      setState(785); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (((((_la - 249) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 249)) & ((1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 249))
      | (1ULL << (sqlParser::SINGLQUOTED_STRING - 249))
      | (1ULL << (sqlParser::DUBLQUOTED_STRING - 249)))) != 0));
    setState(797);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__1 || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (sqlParser::DATE_FORMAT - 70))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 70))
      | (1ULL << (sqlParser::FIELD_DELIMITER - 70)))) != 0) || _la == sqlParser::RECORD_ENDS) {
      setState(787);
      loadOption();
      setState(794);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__1

      || _la == sqlParser::T__5 || ((((_la - 70) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 70)) & ((1ULL << (sqlParser::DATE_FORMAT - 70))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 70))
        | (1ULL << (sqlParser::FIELD_DELIMITER - 70)))) != 0) || _la == sqlParser::RECORD_ENDS) {
        setState(789);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(788);
          match(sqlParser::T__5);
        }
        setState(791);
        loadOption();
        setState(796);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DocolCmdContext ------------------------------------------------------------------

sqlParser::DocolCmdContext::DocolCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DocolCmdContext::DOCOL() {
  return getToken(sqlParser::DOCOL, 0);
}

sqlParser::IdentifierContext* sqlParser::DocolCmdContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::DocolCmdContext::getRuleIndex() const {
  return sqlParser::RuleDocolCmd;
}


antlrcpp::Any sqlParser::DocolCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDocolCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DocolCmdContext* sqlParser::docolCmd() {
  DocolCmdContext *_localctx = _tracker.createInstance<DocolCmdContext>(_ctx, getState());
  enterRule(_localctx, 66, sqlParser::RuleDocolCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(799);
    match(sqlParser::DOCOL);
    setState(800);
    identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DropCmdContext ------------------------------------------------------------------

sqlParser::DropCmdContext::DropCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::DropTableCmdContext* sqlParser::DropCmdContext::dropTableCmd() {
  return getRuleContext<sqlParser::DropTableCmdContext>(0);
}

sqlParser::DropIndexCmdContext* sqlParser::DropCmdContext::dropIndexCmd() {
  return getRuleContext<sqlParser::DropIndexCmdContext>(0);
}


size_t sqlParser::DropCmdContext::getRuleIndex() const {
  return sqlParser::RuleDropCmd;
}


antlrcpp::Any sqlParser::DropCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDropCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DropCmdContext* sqlParser::dropCmd() {
  DropCmdContext *_localctx = _tracker.createInstance<DropCmdContext>(_ctx, getState());
  enterRule(_localctx, 68, sqlParser::RuleDropCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(804);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 77, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(802);
      dropTableCmd();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(803);
      dropIndexCmd();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DropTableCmdContext ------------------------------------------------------------------

sqlParser::DropTableCmdContext::DropTableCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DropTableCmdContext::DROP() {
  return getToken(sqlParser::DROP, 0);
}

tree::TerminalNode* sqlParser::DropTableCmdContext::TABLE() {
  return getToken(sqlParser::TABLE, 0);
}

sqlParser::TableNameContext* sqlParser::DropTableCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}


size_t sqlParser::DropTableCmdContext::getRuleIndex() const {
  return sqlParser::RuleDropTableCmd;
}


antlrcpp::Any sqlParser::DropTableCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDropTableCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DropTableCmdContext* sqlParser::dropTableCmd() {
  DropTableCmdContext *_localctx = _tracker.createInstance<DropTableCmdContext>(_ctx, getState());
  enterRule(_localctx, 70, sqlParser::RuleDropTableCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(806);
    match(sqlParser::DROP);
    setState(807);
    match(sqlParser::TABLE);
    setState(808);
    dynamic_cast<DropTableCmdContext *>(_localctx)->name = tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DropIndexCmdContext ------------------------------------------------------------------

sqlParser::DropIndexCmdContext::DropIndexCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DropIndexCmdContext::DROP() {
  return getToken(sqlParser::DROP, 0);
}

tree::TerminalNode* sqlParser::DropIndexCmdContext::INDEX() {
  return getToken(sqlParser::INDEX, 0);
}

tree::TerminalNode* sqlParser::DropIndexCmdContext::ON() {
  return getToken(sqlParser::ON, 0);
}

sqlParser::IndexNameContext* sqlParser::DropIndexCmdContext::indexName() {
  return getRuleContext<sqlParser::IndexNameContext>(0);
}

sqlParser::TableNameContext* sqlParser::DropIndexCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}


size_t sqlParser::DropIndexCmdContext::getRuleIndex() const {
  return sqlParser::RuleDropIndexCmd;
}


antlrcpp::Any sqlParser::DropIndexCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDropIndexCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DropIndexCmdContext* sqlParser::dropIndexCmd() {
  DropIndexCmdContext *_localctx = _tracker.createInstance<DropIndexCmdContext>(_ctx, getState());
  enterRule(_localctx, 72, sqlParser::RuleDropIndexCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(810);
    match(sqlParser::DROP);
    setState(811);
    match(sqlParser::INDEX);
    setState(819);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 78, _ctx)) {
    case 1: {
      setState(812);
      dynamic_cast<DropIndexCmdContext *>(_localctx)->idxName = indexName();
      setState(813);
      match(sqlParser::ON);
      setState(814);
      dynamic_cast<DropIndexCmdContext *>(_localctx)->tblName = tableName();
      break;
    }

    case 2: {
      setState(816);
      dynamic_cast<DropIndexCmdContext *>(_localctx)->tblName = tableName();
      setState(817);
      dynamic_cast<DropIndexCmdContext *>(_localctx)->idxName = indexName();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EmptyCmdContext ------------------------------------------------------------------

sqlParser::EmptyCmdContext::EmptyCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t sqlParser::EmptyCmdContext::getRuleIndex() const {
  return sqlParser::RuleEmptyCmd;
}


antlrcpp::Any sqlParser::EmptyCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitEmptyCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::EmptyCmdContext* sqlParser::emptyCmd() {
  EmptyCmdContext *_localctx = _tracker.createInstance<EmptyCmdContext>(_ctx, getState());
  enterRule(_localctx, 74, sqlParser::RuleEmptyCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);

   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExplainCmdContext ------------------------------------------------------------------

sqlParser::ExplainCmdContext::ExplainCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::ExplainCmdContext::EXPLAIN() {
  return getToken(sqlParser::EXPLAIN, 0);
}

sqlParser::StatementChoiceContext* sqlParser::ExplainCmdContext::statementChoice() {
  return getRuleContext<sqlParser::StatementChoiceContext>(0);
}

std::vector<sqlParser::QueryHintContext *> sqlParser::ExplainCmdContext::queryHint() {
  return getRuleContexts<sqlParser::QueryHintContext>();
}

sqlParser::QueryHintContext* sqlParser::ExplainCmdContext::queryHint(size_t i) {
  return getRuleContext<sqlParser::QueryHintContext>(i);
}

std::vector<sqlParser::ExplainOptionsContext *> sqlParser::ExplainCmdContext::explainOptions() {
  return getRuleContexts<sqlParser::ExplainOptionsContext>();
}

sqlParser::ExplainOptionsContext* sqlParser::ExplainCmdContext::explainOptions(size_t i) {
  return getRuleContext<sqlParser::ExplainOptionsContext>(i);
}


size_t sqlParser::ExplainCmdContext::getRuleIndex() const {
  return sqlParser::RuleExplainCmd;
}


antlrcpp::Any sqlParser::ExplainCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitExplainCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ExplainCmdContext* sqlParser::explainCmd() {
  ExplainCmdContext *_localctx = _tracker.createInstance<ExplainCmdContext>(_ctx, getState());
  enterRule(_localctx, 76, sqlParser::RuleExplainCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(826);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(823);
      queryHint();
      setState(828);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(829);
    match(sqlParser::EXPLAIN);
    setState(833);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::FORMAT || ((((_la - 200) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 200)) & ((1ULL << (sqlParser::STAGE - 200))
      | (1ULL << (sqlParser::WITH_NAMES - 200))
      | (1ULL << (sqlParser::WITH_TYPES - 200)))) != 0)) {
      setState(830);
      explainOptions();
      setState(835);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(836);
    statementChoice();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExplainOptionsContext ------------------------------------------------------------------

sqlParser::ExplainOptionsContext::ExplainOptionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::ExplainStageListContext* sqlParser::ExplainOptionsContext::explainStageList() {
  return getRuleContext<sqlParser::ExplainStageListContext>(0);
}

sqlParser::ExplainFormatContext* sqlParser::ExplainOptionsContext::explainFormat() {
  return getRuleContext<sqlParser::ExplainFormatContext>(0);
}

tree::TerminalNode* sqlParser::ExplainOptionsContext::WITH_NAMES() {
  return getToken(sqlParser::WITH_NAMES, 0);
}

tree::TerminalNode* sqlParser::ExplainOptionsContext::WITH_TYPES() {
  return getToken(sqlParser::WITH_TYPES, 0);
}


size_t sqlParser::ExplainOptionsContext::getRuleIndex() const {
  return sqlParser::RuleExplainOptions;
}


antlrcpp::Any sqlParser::ExplainOptionsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitExplainOptions(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ExplainOptionsContext* sqlParser::explainOptions() {
  ExplainOptionsContext *_localctx = _tracker.createInstance<ExplainOptionsContext>(_ctx, getState());
  enterRule(_localctx, 78, sqlParser::RuleExplainOptions);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(842);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::STAGE: {
        enterOuterAlt(_localctx, 1);
        setState(838);
        explainStageList();
        break;
      }

      case sqlParser::FORMAT: {
        enterOuterAlt(_localctx, 2);
        setState(839);
        explainFormat();
        break;
      }

      case sqlParser::WITH_NAMES: {
        enterOuterAlt(_localctx, 3);
        setState(840);
        match(sqlParser::WITH_NAMES);
        break;
      }

      case sqlParser::WITH_TYPES: {
        enterOuterAlt(_localctx, 4);
        setState(841);
        match(sqlParser::WITH_TYPES);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExplainStageListContext ------------------------------------------------------------------

sqlParser::ExplainStageListContext::ExplainStageListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::ExplainStageListContext::STAGE() {
  return getToken(sqlParser::STAGE, 0);
}

std::vector<sqlParser::ExplainRangeContext *> sqlParser::ExplainStageListContext::explainRange() {
  return getRuleContexts<sqlParser::ExplainRangeContext>();
}

sqlParser::ExplainRangeContext* sqlParser::ExplainStageListContext::explainRange(size_t i) {
  return getRuleContext<sqlParser::ExplainRangeContext>(i);
}


size_t sqlParser::ExplainStageListContext::getRuleIndex() const {
  return sqlParser::RuleExplainStageList;
}


antlrcpp::Any sqlParser::ExplainStageListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitExplainStageList(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ExplainStageListContext* sqlParser::explainStageList() {
  ExplainStageListContext *_localctx = _tracker.createInstance<ExplainStageListContext>(_ctx, getState());
  enterRule(_localctx, 80, sqlParser::RuleExplainStageList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(844);
    match(sqlParser::STAGE);
    setState(845);
    explainRange();
    setState(850);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(846);
      match(sqlParser::T__5);
      setState(847);
      explainRange();
      setState(852);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExplainRangeContext ------------------------------------------------------------------

sqlParser::ExplainRangeContext::ExplainRangeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<sqlParser::ExplainRangeChoiceContext *> sqlParser::ExplainRangeContext::explainRangeChoice() {
  return getRuleContexts<sqlParser::ExplainRangeChoiceContext>();
}

sqlParser::ExplainRangeChoiceContext* sqlParser::ExplainRangeContext::explainRangeChoice(size_t i) {
  return getRuleContext<sqlParser::ExplainRangeChoiceContext>(i);
}

tree::TerminalNode* sqlParser::ExplainRangeContext::TO() {
  return getToken(sqlParser::TO, 0);
}


size_t sqlParser::ExplainRangeContext::getRuleIndex() const {
  return sqlParser::RuleExplainRange;
}


antlrcpp::Any sqlParser::ExplainRangeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitExplainRange(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ExplainRangeContext* sqlParser::explainRange() {
  ExplainRangeContext *_localctx = _tracker.createInstance<ExplainRangeContext>(_ctx, getState());
  enterRule(_localctx, 82, sqlParser::RuleExplainRange);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(853);
    dynamic_cast<ExplainRangeContext *>(_localctx)->begin = explainRangeChoice();
    setState(856);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__10 || _la == sqlParser::TO) {
      setState(854);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__10 || _la == sqlParser::TO)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(855);
      dynamic_cast<ExplainRangeContext *>(_localctx)->end = explainRangeChoice();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExplainRangeChoiceContext ------------------------------------------------------------------

sqlParser::ExplainRangeChoiceContext::ExplainRangeChoiceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::ExplainRangeChoiceContext::ALL() {
  return getToken(sqlParser::ALL, 0);
}

tree::TerminalNode* sqlParser::ExplainRangeChoiceContext::INTEGER_LITERAL() {
  return getToken(sqlParser::INTEGER_LITERAL, 0);
}

sqlParser::IdentifierContext* sqlParser::ExplainRangeChoiceContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::ExplainRangeChoiceContext::getRuleIndex() const {
  return sqlParser::RuleExplainRangeChoice;
}


antlrcpp::Any sqlParser::ExplainRangeChoiceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitExplainRangeChoice(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ExplainRangeChoiceContext* sqlParser::explainRangeChoice() {
  ExplainRangeChoiceContext *_localctx = _tracker.createInstance<ExplainRangeChoiceContext>(_ctx, getState());
  enterRule(_localctx, 84, sqlParser::RuleExplainRangeChoice);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(861);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__11:
      case sqlParser::ALL: {
        enterOuterAlt(_localctx, 1);
        setState(858);
        dynamic_cast<ExplainRangeChoiceContext *>(_localctx)->all = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__11

        || _la == sqlParser::ALL)) {
          dynamic_cast<ExplainRangeChoiceContext *>(_localctx)->all = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(859);
        dynamic_cast<ExplainRangeChoiceContext *>(_localctx)->num = match(sqlParser::INTEGER_LITERAL);
        break;
      }

      case sqlParser::ANALYSE:
      case sqlParser::ANALYZE:
      case sqlParser::ANIDX:
      case sqlParser::ANTBL:
      case sqlParser::ASC:
      case sqlParser::BIT:
      case sqlParser::BITS:
      case sqlParser::BLOCK:
      case sqlParser::BLOOM:
      case sqlParser::BPA:
      case sqlParser::BTREE:
      case sqlParser::BYE:
      case sqlParser::BYTE:
      case sqlParser::BYTES:
      case sqlParser::CATALOG:
      case sqlParser::CHARS:
      case sqlParser::COMPACTIDX:
      case sqlParser::CONCAT:
      case sqlParser::DATA:
      case sqlParser::DATE_FORMAT:
      case sqlParser::DAYS:
      case sqlParser::DECODE:
      case sqlParser::DESC:
      case sqlParser::DETAIL:
      case sqlParser::DIGITS:
      case sqlParser::DLOAD:
      case sqlParser::DOCOL:
      case sqlParser::ECHO:
      case sqlParser::ESCAPE_CHAR:
      case sqlParser::EXIT_TOKEN:
      case sqlParser::EXPLAIN:
      case sqlParser::EXPORT:
      case sqlParser::SQL_FALSE:
      case sqlParser::FEDERATION:
      case sqlParser::FIELD_DELIMITER:
      case sqlParser::FILE:
      case sqlParser::FORCE:
      case sqlParser::FORMAT:
      case sqlParser::HASH:
      case sqlParser::HASHKEY:
      case sqlParser::HOURS:
      case sqlParser::IF:
      case sqlParser::IFNULL:
      case sqlParser::INDEX:
      case sqlParser::INTERVAL_DT:
      case sqlParser::INTERVAL_YM:
      case sqlParser::KEY:
      case sqlParser::LIMIT:
      case sqlParser::LOADREPLICATED:
      case sqlParser::LOAD:
      case sqlParser::LOG:
      case sqlParser::LOOP:
      case sqlParser::LSIDX:
      case sqlParser::LSPTNCOL:
      case sqlParser::LSTBL:
      case sqlParser::MEMORYLEAKS:
      case sqlParser::MILLIS:
      case sqlParser::MILLISECOND:
      case sqlParser::MILLISECONDS:
      case sqlParser::MINUTES:
      case sqlParser::MONOTONIC:
      case sqlParser::MONTHS:
      case sqlParser::MS:
      case sqlParser::NODES:
      case sqlParser::NODE:
      case sqlParser::NOOP:
      case sqlParser::NOP:
      case sqlParser::NOW:
      case sqlParser::OBJECTS:
      case sqlParser::OFF:
      case sqlParser::OFFSET:
      case sqlParser::ORIGIN:
      case sqlParser::PARTITIONS:
      case sqlParser::PATHS:
      case sqlParser::PIPELINE:
      case sqlParser::PLACING:
      case sqlParser::PRETTY:
      case sqlParser::QUARTER:
      case sqlParser::QUARTERS:
      case sqlParser::RECORD_ENDS:
      case sqlParser::REFERENCE:
      case sqlParser::REGIONS:
      case sqlParser::REPLICATED:
      case sqlParser::ROWSIZE:
      case sqlParser::SCHEMA:
      case sqlParser::SEC:
      case sqlParser::SECONDS:
      case sqlParser::SKIPLIST:
      case sqlParser::SLEEP:
      case sqlParser::SPACE:
      case sqlParser::STAGE:
      case sqlParser::STATS:
      case sqlParser::STATISTICS:
      case sqlParser::STATUS:
      case sqlParser::SUBSTR:
      case sqlParser::SZIDX:
      case sqlParser::SZTBL:
      case sqlParser::S_ONLY:
      case sqlParser::TABLESIZE:
      case sqlParser::TEXT:
      case sqlParser::TOP:
      case sqlParser::TRACKMEMORY:
      case sqlParser::TREE:
      case sqlParser::TRUNCATE:
      case sqlParser::TYPE:
      case sqlParser::TYPEOF:
      case sqlParser::UNLOAD:
      case sqlParser::VALUE:
      case sqlParser::WEEK:
      case sqlParser::WEEKS:
      case sqlParser::WITH_NAMES:
      case sqlParser::WITH_TYPES:
      case sqlParser::XOR:
      case sqlParser::YEARS:
      case sqlParser::PLAIN_IDENTIFIER:
      case sqlParser::BAKTIKD_IDENTIFIER:
      case sqlParser::DUBLQUOTED_IDENTIFIER: {
        enterOuterAlt(_localctx, 3);
        setState(860);
        dynamic_cast<ExplainRangeChoiceContext *>(_localctx)->id = identifier();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExplainFormatContext ------------------------------------------------------------------

sqlParser::ExplainFormatContext::ExplainFormatContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::ExplainFormatContext::FORMAT() {
  return getToken(sqlParser::FORMAT, 0);
}

tree::TerminalNode* sqlParser::ExplainFormatContext::PRETTY() {
  return getToken(sqlParser::PRETTY, 0);
}

tree::TerminalNode* sqlParser::ExplainFormatContext::ROW() {
  return getToken(sqlParser::ROW, 0);
}


size_t sqlParser::ExplainFormatContext::getRuleIndex() const {
  return sqlParser::RuleExplainFormat;
}


antlrcpp::Any sqlParser::ExplainFormatContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitExplainFormat(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ExplainFormatContext* sqlParser::explainFormat() {
  ExplainFormatContext *_localctx = _tracker.createInstance<ExplainFormatContext>(_ctx, getState());
  enterRule(_localctx, 86, sqlParser::RuleExplainFormat);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(863);
    match(sqlParser::FORMAT);
    setState(864);
    _la = _input->LA(1);
    if (!(_la == sqlParser::PRETTY

    || _la == sqlParser::ROW)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExportCmdContext ------------------------------------------------------------------

sqlParser::ExportCmdContext::ExportCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::ExportCmdContext::EXPORT() {
  return getToken(sqlParser::EXPORT, 0);
}

sqlParser::TableNameContext* sqlParser::ExportCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::StringLiteralContext* sqlParser::ExportCmdContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}


size_t sqlParser::ExportCmdContext::getRuleIndex() const {
  return sqlParser::RuleExportCmd;
}


antlrcpp::Any sqlParser::ExportCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitExportCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ExportCmdContext* sqlParser::exportCmd() {
  ExportCmdContext *_localctx = _tracker.createInstance<ExportCmdContext>(_ctx, getState());
  enterRule(_localctx, 88, sqlParser::RuleExportCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(866);
    match(sqlParser::EXPORT);
    setState(867);
    tableName();
    setState(868);
    dynamic_cast<ExportCmdContext *>(_localctx)->filePath = stringLiteral();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FileCmdContext ------------------------------------------------------------------

sqlParser::FileCmdContext::FileCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::FileCmdContext::FILE() {
  return getToken(sqlParser::FILE, 0);
}

sqlParser::IdentifierContext* sqlParser::FileCmdContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}

sqlParser::StringLiteralContext* sqlParser::FileCmdContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}


size_t sqlParser::FileCmdContext::getRuleIndex() const {
  return sqlParser::RuleFileCmd;
}


antlrcpp::Any sqlParser::FileCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitFileCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::FileCmdContext* sqlParser::fileCmd() {
  FileCmdContext *_localctx = _tracker.createInstance<FileCmdContext>(_ctx, getState());
  enterRule(_localctx, 90, sqlParser::RuleFileCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(870);
    match(sqlParser::FILE);
    setState(873);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 85, _ctx)) {
    case 1: {
      setState(871);
      identifier();
      break;
    }

    case 2: {
      setState(872);
      stringLiteral();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InsertCmdContext ------------------------------------------------------------------

sqlParser::InsertCmdContext::InsertCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::InsertCmdContext::INSERT() {
  return getToken(sqlParser::INSERT, 0);
}

sqlParser::TableReferenceContext* sqlParser::InsertCmdContext::tableReference() {
  return getRuleContext<sqlParser::TableReferenceContext>(0);
}

sqlParser::ValuesExprContext* sqlParser::InsertCmdContext::valuesExpr() {
  return getRuleContext<sqlParser::ValuesExprContext>(0);
}

sqlParser::SelectQueryContext* sqlParser::InsertCmdContext::selectQuery() {
  return getRuleContext<sqlParser::SelectQueryContext>(0);
}

sqlParser::SetQueryContext* sqlParser::InsertCmdContext::setQuery() {
  return getRuleContext<sqlParser::SetQueryContext>(0);
}

tree::TerminalNode* sqlParser::InsertCmdContext::WITH() {
  return getToken(sqlParser::WITH, 0);
}

std::vector<sqlParser::WithQueryContext *> sqlParser::InsertCmdContext::withQuery() {
  return getRuleContexts<sqlParser::WithQueryContext>();
}

sqlParser::WithQueryContext* sqlParser::InsertCmdContext::withQuery(size_t i) {
  return getRuleContext<sqlParser::WithQueryContext>(i);
}

tree::TerminalNode* sqlParser::InsertCmdContext::INTO() {
  return getToken(sqlParser::INTO, 0);
}

std::vector<sqlParser::ColumnNameContext *> sqlParser::InsertCmdContext::columnName() {
  return getRuleContexts<sqlParser::ColumnNameContext>();
}

sqlParser::ColumnNameContext* sqlParser::InsertCmdContext::columnName(size_t i) {
  return getRuleContext<sqlParser::ColumnNameContext>(i);
}


size_t sqlParser::InsertCmdContext::getRuleIndex() const {
  return sqlParser::RuleInsertCmd;
}


antlrcpp::Any sqlParser::InsertCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitInsertCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::InsertCmdContext* sqlParser::insertCmd() {
  InsertCmdContext *_localctx = _tracker.createInstance<InsertCmdContext>(_ctx, getState());
  enterRule(_localctx, 92, sqlParser::RuleInsertCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(884);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(875);
      match(sqlParser::WITH);
      setState(876);
      withQuery();
      setState(881);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(877);
        match(sqlParser::T__5);
        setState(878);
        withQuery();
        setState(883);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(886);
    match(sqlParser::INSERT);
    setState(888);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::INTO) {
      setState(887);
      match(sqlParser::INTO);
    }
    setState(890);
    dynamic_cast<InsertCmdContext *>(_localctx)->ref = tableReference();
    setState(902);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 90, _ctx)) {
    case 1: {
      setState(891);
      match(sqlParser::T__6);
      setState(892);
      columnName();
      setState(897);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(893);
        match(sqlParser::T__5);
        setState(894);
        columnName();
        setState(899);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(900);
      match(sqlParser::T__7);
      break;
    }

    }
    setState(907);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 91, _ctx)) {
    case 1: {
      setState(904);
      valuesExpr();
      break;
    }

    case 2: {
      setState(905);
      selectQuery();
      break;
    }

    case 3: {
      setState(906);
      setQuery();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LoadCmdContext ------------------------------------------------------------------

sqlParser::LoadCmdContext::LoadCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LoadCmdContext::LOAD() {
  return getToken(sqlParser::LOAD, 0);
}

sqlParser::TableNameContext* sqlParser::LoadCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::StringLiteralContext* sqlParser::LoadCmdContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}

std::vector<sqlParser::LoadOptionContext *> sqlParser::LoadCmdContext::loadOption() {
  return getRuleContexts<sqlParser::LoadOptionContext>();
}

sqlParser::LoadOptionContext* sqlParser::LoadCmdContext::loadOption(size_t i) {
  return getRuleContext<sqlParser::LoadOptionContext>(i);
}


size_t sqlParser::LoadCmdContext::getRuleIndex() const {
  return sqlParser::RuleLoadCmd;
}


antlrcpp::Any sqlParser::LoadCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLoadCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LoadCmdContext* sqlParser::loadCmd() {
  LoadCmdContext *_localctx = _tracker.createInstance<LoadCmdContext>(_ctx, getState());
  enterRule(_localctx, 94, sqlParser::RuleLoadCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(909);
    match(sqlParser::LOAD);
    setState(910);
    tableName();
    setState(911);
    dynamic_cast<LoadCmdContext *>(_localctx)->filePath = stringLiteral();
    setState(922);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__1 || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (sqlParser::DATE_FORMAT - 70))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 70))
      | (1ULL << (sqlParser::FIELD_DELIMITER - 70)))) != 0) || _la == sqlParser::RECORD_ENDS) {
      setState(912);
      loadOption();
      setState(919);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__1

      || _la == sqlParser::T__5 || ((((_la - 70) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 70)) & ((1ULL << (sqlParser::DATE_FORMAT - 70))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 70))
        | (1ULL << (sqlParser::FIELD_DELIMITER - 70)))) != 0) || _la == sqlParser::RECORD_ENDS) {
        setState(914);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(913);
          match(sqlParser::T__5);
        }
        setState(916);
        loadOption();
        setState(921);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LoadReplicatedCmdContext ------------------------------------------------------------------

sqlParser::LoadReplicatedCmdContext::LoadReplicatedCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LoadReplicatedCmdContext::LOADREPLICATED() {
  return getToken(sqlParser::LOADREPLICATED, 0);
}

sqlParser::TableNameContext* sqlParser::LoadReplicatedCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::StringLiteralContext* sqlParser::LoadReplicatedCmdContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}

std::vector<sqlParser::LoadOptionContext *> sqlParser::LoadReplicatedCmdContext::loadOption() {
  return getRuleContexts<sqlParser::LoadOptionContext>();
}

sqlParser::LoadOptionContext* sqlParser::LoadReplicatedCmdContext::loadOption(size_t i) {
  return getRuleContext<sqlParser::LoadOptionContext>(i);
}


size_t sqlParser::LoadReplicatedCmdContext::getRuleIndex() const {
  return sqlParser::RuleLoadReplicatedCmd;
}


antlrcpp::Any sqlParser::LoadReplicatedCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLoadReplicatedCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LoadReplicatedCmdContext* sqlParser::loadReplicatedCmd() {
  LoadReplicatedCmdContext *_localctx = _tracker.createInstance<LoadReplicatedCmdContext>(_ctx, getState());
  enterRule(_localctx, 96, sqlParser::RuleLoadReplicatedCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(924);
    match(sqlParser::LOADREPLICATED);
    setState(925);
    tableName();
    setState(926);
    dynamic_cast<LoadReplicatedCmdContext *>(_localctx)->filePath = stringLiteral();
    setState(937);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__1 || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (sqlParser::DATE_FORMAT - 70))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 70))
      | (1ULL << (sqlParser::FIELD_DELIMITER - 70)))) != 0) || _la == sqlParser::RECORD_ENDS) {
      setState(927);
      loadOption();
      setState(934);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__1

      || _la == sqlParser::T__5 || ((((_la - 70) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 70)) & ((1ULL << (sqlParser::DATE_FORMAT - 70))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 70))
        | (1ULL << (sqlParser::FIELD_DELIMITER - 70)))) != 0) || _la == sqlParser::RECORD_ENDS) {
        setState(929);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(928);
          match(sqlParser::T__5);
        }
        setState(931);
        loadOption();
        setState(936);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LoadOptionContext ------------------------------------------------------------------

sqlParser::LoadOptionContext::LoadOptionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::StringLiteralContext* sqlParser::LoadOptionContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}

sqlParser::LoadOptionKeysContext* sqlParser::LoadOptionContext::loadOptionKeys() {
  return getRuleContext<sqlParser::LoadOptionKeysContext>(0);
}


size_t sqlParser::LoadOptionContext::getRuleIndex() const {
  return sqlParser::RuleLoadOption;
}


antlrcpp::Any sqlParser::LoadOptionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLoadOption(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LoadOptionContext* sqlParser::loadOption() {
  LoadOptionContext *_localctx = _tracker.createInstance<LoadOptionContext>(_ctx, getState());
  enterRule(_localctx, 98, sqlParser::RuleLoadOption);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(944);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__1: {
        setState(939);
        match(sqlParser::T__1);
        setState(940);
        loadOptionKeys();
        setState(941);
        match(sqlParser::T__2);
        break;
      }

      case sqlParser::DATE_FORMAT:
      case sqlParser::ESCAPE_CHAR:
      case sqlParser::FIELD_DELIMITER:
      case sqlParser::RECORD_ENDS: {
        setState(943);
        loadOptionKeys();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(946);
    match(sqlParser::T__3);
    setState(947);
    dynamic_cast<LoadOptionContext *>(_localctx)->loadValue = stringLiteral();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LoadOptionKeysContext ------------------------------------------------------------------

sqlParser::LoadOptionKeysContext::LoadOptionKeysContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LoadOptionKeysContext::FIELD_DELIMITER() {
  return getToken(sqlParser::FIELD_DELIMITER, 0);
}

tree::TerminalNode* sqlParser::LoadOptionKeysContext::ESCAPE_CHAR() {
  return getToken(sqlParser::ESCAPE_CHAR, 0);
}

tree::TerminalNode* sqlParser::LoadOptionKeysContext::DATE_FORMAT() {
  return getToken(sqlParser::DATE_FORMAT, 0);
}

tree::TerminalNode* sqlParser::LoadOptionKeysContext::RECORD_ENDS() {
  return getToken(sqlParser::RECORD_ENDS, 0);
}


size_t sqlParser::LoadOptionKeysContext::getRuleIndex() const {
  return sqlParser::RuleLoadOptionKeys;
}


antlrcpp::Any sqlParser::LoadOptionKeysContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLoadOptionKeys(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LoadOptionKeysContext* sqlParser::loadOptionKeys() {
  LoadOptionKeysContext *_localctx = _tracker.createInstance<LoadOptionKeysContext>(_ctx, getState());
  enterRule(_localctx, 100, sqlParser::RuleLoadOptionKeys);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(949);
    _la = _input->LA(1);
    if (!(((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (sqlParser::DATE_FORMAT - 70))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 70))
      | (1ULL << (sqlParser::FIELD_DELIMITER - 70)))) != 0) || _la == sqlParser::RECORD_ENDS)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogCmdContext ------------------------------------------------------------------

sqlParser::LogCmdContext::LogCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LogCmdContext::LOG() {
  return getToken(sqlParser::LOG, 0);
}

sqlParser::StringLiteralContext* sqlParser::LogCmdContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}

tree::TerminalNode* sqlParser::LogCmdContext::ECHO() {
  return getToken(sqlParser::ECHO, 0);
}

tree::TerminalNode* sqlParser::LogCmdContext::SQL_TRUE() {
  return getToken(sqlParser::SQL_TRUE, 0);
}

tree::TerminalNode* sqlParser::LogCmdContext::ON() {
  return getToken(sqlParser::ON, 0);
}

tree::TerminalNode* sqlParser::LogCmdContext::SQL_FALSE() {
  return getToken(sqlParser::SQL_FALSE, 0);
}

tree::TerminalNode* sqlParser::LogCmdContext::OFF() {
  return getToken(sqlParser::OFF, 0);
}


size_t sqlParser::LogCmdContext::getRuleIndex() const {
  return sqlParser::RuleLogCmd;
}


antlrcpp::Any sqlParser::LogCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLogCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LogCmdContext* sqlParser::logCmd() {
  LogCmdContext *_localctx = _tracker.createInstance<LogCmdContext>(_ctx, getState());
  enterRule(_localctx, 102, sqlParser::RuleLogCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(951);
    match(sqlParser::LOG);
    setState(952);
    dynamic_cast<LogCmdContext *>(_localctx)->msg = stringLiteral();
    setState(955);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ECHO) {
      setState(953);
      match(sqlParser::ECHO);
      setState(954);
      _la = _input->LA(1);
      if (!(_la == sqlParser::SQL_FALSE || ((((_la - 163) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 163)) & ((1ULL << (sqlParser::OFF - 163))
        | (1ULL << (sqlParser::ON - 163))
        | (1ULL << (sqlParser::SQL_TRUE - 163)))) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LscolCmdContext ------------------------------------------------------------------

sqlParser::LscolCmdContext::LscolCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LscolCmdContext::LSCOL() {
  return getToken(sqlParser::LSCOL, 0);
}

sqlParser::TableNameContext* sqlParser::LscolCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}


size_t sqlParser::LscolCmdContext::getRuleIndex() const {
  return sqlParser::RuleLscolCmd;
}


antlrcpp::Any sqlParser::LscolCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLscolCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LscolCmdContext* sqlParser::lscolCmd() {
  LscolCmdContext *_localctx = _tracker.createInstance<LscolCmdContext>(_ctx, getState());
  enterRule(_localctx, 104, sqlParser::RuleLscolCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(957);
    match(sqlParser::LSCOL);
    setState(958);
    tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LsidxCmdContext ------------------------------------------------------------------

sqlParser::LsidxCmdContext::LsidxCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LsidxCmdContext::LSIDX() {
  return getToken(sqlParser::LSIDX, 0);
}

sqlParser::TableNameContext* sqlParser::LsidxCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}


size_t sqlParser::LsidxCmdContext::getRuleIndex() const {
  return sqlParser::RuleLsidxCmd;
}


antlrcpp::Any sqlParser::LsidxCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLsidxCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LsidxCmdContext* sqlParser::lsidxCmd() {
  LsidxCmdContext *_localctx = _tracker.createInstance<LsidxCmdContext>(_ctx, getState());
  enterRule(_localctx, 106, sqlParser::RuleLsidxCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(960);
    match(sqlParser::LSIDX);
    setState(961);
    tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LsidxcolCmdContext ------------------------------------------------------------------

sqlParser::LsidxcolCmdContext::LsidxcolCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LsidxcolCmdContext::LSIDXCOL() {
  return getToken(sqlParser::LSIDXCOL, 0);
}

sqlParser::TableNameContext* sqlParser::LsidxcolCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::IndexNameContext* sqlParser::LsidxcolCmdContext::indexName() {
  return getRuleContext<sqlParser::IndexNameContext>(0);
}


size_t sqlParser::LsidxcolCmdContext::getRuleIndex() const {
  return sqlParser::RuleLsidxcolCmd;
}


antlrcpp::Any sqlParser::LsidxcolCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLsidxcolCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LsidxcolCmdContext* sqlParser::lsidxcolCmd() {
  LsidxcolCmdContext *_localctx = _tracker.createInstance<LsidxcolCmdContext>(_ctx, getState());
  enterRule(_localctx, 108, sqlParser::RuleLsidxcolCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(963);
    match(sqlParser::LSIDXCOL);
    setState(964);
    tableName();
    setState(965);
    indexName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LsptncolCmdContext ------------------------------------------------------------------

sqlParser::LsptncolCmdContext::LsptncolCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LsptncolCmdContext::LSPTNCOL() {
  return getToken(sqlParser::LSPTNCOL, 0);
}

sqlParser::TableNameContext* sqlParser::LsptncolCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}


size_t sqlParser::LsptncolCmdContext::getRuleIndex() const {
  return sqlParser::RuleLsptncolCmd;
}


antlrcpp::Any sqlParser::LsptncolCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLsptncolCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LsptncolCmdContext* sqlParser::lsptncolCmd() {
  LsptncolCmdContext *_localctx = _tracker.createInstance<LsptncolCmdContext>(_ctx, getState());
  enterRule(_localctx, 110, sqlParser::RuleLsptncolCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(967);
    match(sqlParser::LSPTNCOL);
    setState(968);
    tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LstblCmdContext ------------------------------------------------------------------

sqlParser::LstblCmdContext::LstblCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LstblCmdContext::LSTBL() {
  return getToken(sqlParser::LSTBL, 0);
}


size_t sqlParser::LstblCmdContext::getRuleIndex() const {
  return sqlParser::RuleLstblCmd;
}


antlrcpp::Any sqlParser::LstblCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLstblCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LstblCmdContext* sqlParser::lstblCmd() {
  LstblCmdContext *_localctx = _tracker.createInstance<LstblCmdContext>(_ctx, getState());
  enterRule(_localctx, 112, sqlParser::RuleLstblCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(970);
    match(sqlParser::LSTBL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MemoryleaksCmdContext ------------------------------------------------------------------

sqlParser::MemoryleaksCmdContext::MemoryleaksCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::MemoryleaksCmdContext::MEMORYLEAKS() {
  return getToken(sqlParser::MEMORYLEAKS, 0);
}


size_t sqlParser::MemoryleaksCmdContext::getRuleIndex() const {
  return sqlParser::RuleMemoryleaksCmd;
}


antlrcpp::Any sqlParser::MemoryleaksCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitMemoryleaksCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::MemoryleaksCmdContext* sqlParser::memoryleaksCmd() {
  MemoryleaksCmdContext *_localctx = _tracker.createInstance<MemoryleaksCmdContext>(_ctx, getState());
  enterRule(_localctx, 114, sqlParser::RuleMemoryleaksCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(972);
    match(sqlParser::MEMORYLEAKS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NoopCmdContext ------------------------------------------------------------------

sqlParser::NoopCmdContext::NoopCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::NoopCmdContext::NOOP() {
  return getToken(sqlParser::NOOP, 0);
}

tree::TerminalNode* sqlParser::NoopCmdContext::NOP() {
  return getToken(sqlParser::NOP, 0);
}


size_t sqlParser::NoopCmdContext::getRuleIndex() const {
  return sqlParser::RuleNoopCmd;
}


antlrcpp::Any sqlParser::NoopCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitNoopCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::NoopCmdContext* sqlParser::noopCmd() {
  NoopCmdContext *_localctx = _tracker.createInstance<NoopCmdContext>(_ctx, getState());
  enterRule(_localctx, 116, sqlParser::RuleNoopCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(974);
    _la = _input->LA(1);
    if (!(_la == sqlParser::NOOP

    || _la == sqlParser::NOP)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PipelineCmdContext ------------------------------------------------------------------

sqlParser::PipelineCmdContext::PipelineCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::PipelineCmdContext::PIPELINE() {
  return getToken(sqlParser::PIPELINE, 0);
}

tree::TerminalNode* sqlParser::PipelineCmdContext::BEGIN() {
  return getToken(sqlParser::BEGIN, 0);
}

tree::TerminalNode* sqlParser::PipelineCmdContext::END() {
  return getToken(sqlParser::END, 0);
}

sqlParser::IdentifierContext* sqlParser::PipelineCmdContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}

std::vector<sqlParser::PipelineStageContext *> sqlParser::PipelineCmdContext::pipelineStage() {
  return getRuleContexts<sqlParser::PipelineStageContext>();
}

sqlParser::PipelineStageContext* sqlParser::PipelineCmdContext::pipelineStage(size_t i) {
  return getRuleContext<sqlParser::PipelineStageContext>(i);
}


size_t sqlParser::PipelineCmdContext::getRuleIndex() const {
  return sqlParser::RulePipelineCmd;
}


antlrcpp::Any sqlParser::PipelineCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitPipelineCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::PipelineCmdContext* sqlParser::pipelineCmd() {
  PipelineCmdContext *_localctx = _tracker.createInstance<PipelineCmdContext>(_ctx, getState());
  enterRule(_localctx, 118, sqlParser::RulePipelineCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(976);
    match(sqlParser::PIPELINE);
    setState(977);
    dynamic_cast<PipelineCmdContext *>(_localctx)->pipelineName = identifier();
    setState(978);
    match(sqlParser::BEGIN);
    setState(980); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(979);
      pipelineStage();
      setState(982); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == sqlParser::STAGE);
    setState(984);
    match(sqlParser::END);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PipelineStageContext ------------------------------------------------------------------

sqlParser::PipelineStageContext::PipelineStageContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::PipelineStageContext::STAGE() {
  return getToken(sqlParser::STAGE, 0);
}

tree::TerminalNode* sqlParser::PipelineStageContext::ON() {
  return getToken(sqlParser::ON, 0);
}

sqlParser::PipelineOpSeqContext* sqlParser::PipelineStageContext::pipelineOpSeq() {
  return getRuleContext<sqlParser::PipelineOpSeqContext>(0);
}

sqlParser::IdentifierContext* sqlParser::PipelineStageContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}

tree::TerminalNode* sqlParser::PipelineStageContext::ORIGIN() {
  return getToken(sqlParser::ORIGIN, 0);
}

tree::TerminalNode* sqlParser::PipelineStageContext::ALL() {
  return getToken(sqlParser::ALL, 0);
}

tree::TerminalNode* sqlParser::PipelineStageContext::NODES() {
  return getToken(sqlParser::NODES, 0);
}

tree::TerminalNode* sqlParser::PipelineStageContext::PARTITIONS() {
  return getToken(sqlParser::PARTITIONS, 0);
}


size_t sqlParser::PipelineStageContext::getRuleIndex() const {
  return sqlParser::RulePipelineStage;
}


antlrcpp::Any sqlParser::PipelineStageContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitPipelineStage(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::PipelineStageContext* sqlParser::pipelineStage() {
  PipelineStageContext *_localctx = _tracker.createInstance<PipelineStageContext>(_ctx, getState());
  enterRule(_localctx, 120, sqlParser::RulePipelineStage);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(986);
    match(sqlParser::STAGE);
    setState(987);
    dynamic_cast<PipelineStageContext *>(_localctx)->stageName = identifier();
    setState(988);
    match(sqlParser::ON);
    setState(994);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 101, _ctx)) {
    case 1: {
      setState(989);
      match(sqlParser::ORIGIN);
      break;
    }

    case 2: {
      setState(990);
      match(sqlParser::ALL);
      setState(991);
      match(sqlParser::NODES);
      break;
    }

    case 3: {
      setState(992);
      match(sqlParser::ALL);
      setState(993);
      match(sqlParser::PARTITIONS);
      break;
    }

    }
    setState(996);
    match(sqlParser::T__12);
    setState(997);
    pipelineOpSeq();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PipelineOpSeqContext ------------------------------------------------------------------

sqlParser::PipelineOpSeqContext::PipelineOpSeqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<sqlParser::PipelineOperatorContext *> sqlParser::PipelineOpSeqContext::pipelineOperator() {
  return getRuleContexts<sqlParser::PipelineOperatorContext>();
}

sqlParser::PipelineOperatorContext* sqlParser::PipelineOpSeqContext::pipelineOperator(size_t i) {
  return getRuleContext<sqlParser::PipelineOperatorContext>(i);
}


size_t sqlParser::PipelineOpSeqContext::getRuleIndex() const {
  return sqlParser::RulePipelineOpSeq;
}


antlrcpp::Any sqlParser::PipelineOpSeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitPipelineOpSeq(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::PipelineOpSeqContext* sqlParser::pipelineOpSeq() {
  PipelineOpSeqContext *_localctx = _tracker.createInstance<PipelineOpSeqContext>(_ctx, getState());
  enterRule(_localctx, 122, sqlParser::RulePipelineOpSeq);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(999);
    pipelineOperator();
    setState(1004);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__13) {
      setState(1000);
      match(sqlParser::T__13);
      setState(1001);
      pipelineOperator();
      setState(1006);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PipelineOperatorContext ------------------------------------------------------------------

sqlParser::PipelineOperatorContext::PipelineOperatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::PipelineOperatorContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}

std::vector<sqlParser::PipelineOpParamContext *> sqlParser::PipelineOperatorContext::pipelineOpParam() {
  return getRuleContexts<sqlParser::PipelineOpParamContext>();
}

sqlParser::PipelineOpParamContext* sqlParser::PipelineOperatorContext::pipelineOpParam(size_t i) {
  return getRuleContext<sqlParser::PipelineOpParamContext>(i);
}


size_t sqlParser::PipelineOperatorContext::getRuleIndex() const {
  return sqlParser::RulePipelineOperator;
}


antlrcpp::Any sqlParser::PipelineOperatorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitPipelineOperator(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::PipelineOperatorContext* sqlParser::pipelineOperator() {
  PipelineOperatorContext *_localctx = _tracker.createInstance<PipelineOperatorContext>(_ctx, getState());
  enterRule(_localctx, 124, sqlParser::RulePipelineOperator);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1007);
    dynamic_cast<PipelineOperatorContext *>(_localctx)->opName = identifier();
    setState(1018);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 105, _ctx)) {
    case 1: {
      setState(1008);
      pipelineOpParam();
      setState(1015);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 104, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(1010);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__5) {
            setState(1009);
            match(sqlParser::T__5);
          }
          setState(1012);
          pipelineOpParam(); 
        }
        setState(1017);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 104, _ctx);
      }
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PipelineOpParamContext ------------------------------------------------------------------

sqlParser::PipelineOpParamContext::PipelineOpParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::PipelineOpParamContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}

tree::TerminalNode* sqlParser::PipelineOpParamContext::TYPE() {
  return getToken(sqlParser::TYPE, 0);
}

tree::TerminalNode* sqlParser::PipelineOpParamContext::AS() {
  return getToken(sqlParser::AS, 0);
}

tree::TerminalNode* sqlParser::PipelineOpParamContext::SUM() {
  return getToken(sqlParser::SUM, 0);
}

tree::TerminalNode* sqlParser::PipelineOpParamContext::AVG() {
  return getToken(sqlParser::AVG, 0);
}

tree::TerminalNode* sqlParser::PipelineOpParamContext::MIN() {
  return getToken(sqlParser::MIN, 0);
}

tree::TerminalNode* sqlParser::PipelineOpParamContext::MAX() {
  return getToken(sqlParser::MAX, 0);
}

tree::TerminalNode* sqlParser::PipelineOpParamContext::PARAM_IDENTIFIER() {
  return getToken(sqlParser::PARAM_IDENTIFIER, 0);
}

sqlParser::PipelineTypeNamesContext* sqlParser::PipelineOpParamContext::pipelineTypeNames() {
  return getRuleContext<sqlParser::PipelineTypeNamesContext>(0);
}

sqlParser::BooleanLiteralContext* sqlParser::PipelineOpParamContext::booleanLiteral() {
  return getRuleContext<sqlParser::BooleanLiteralContext>(0);
}

sqlParser::SignedIntegerLiteralContext* sqlParser::PipelineOpParamContext::signedIntegerLiteral() {
  return getRuleContext<sqlParser::SignedIntegerLiteralContext>(0);
}

sqlParser::SignedDecimalLiteralContext* sqlParser::PipelineOpParamContext::signedDecimalLiteral() {
  return getRuleContext<sqlParser::SignedDecimalLiteralContext>(0);
}

sqlParser::SignedFloatLiteralContext* sqlParser::PipelineOpParamContext::signedFloatLiteral() {
  return getRuleContext<sqlParser::SignedFloatLiteralContext>(0);
}

sqlParser::DateLiteralContext* sqlParser::PipelineOpParamContext::dateLiteral() {
  return getRuleContext<sqlParser::DateLiteralContext>(0);
}

sqlParser::TimeLiteralContext* sqlParser::PipelineOpParamContext::timeLiteral() {
  return getRuleContext<sqlParser::TimeLiteralContext>(0);
}

tree::TerminalNode* sqlParser::PipelineOpParamContext::DUBLQUOTED_IDENTIFIER() {
  return getToken(sqlParser::DUBLQUOTED_IDENTIFIER, 0);
}

tree::TerminalNode* sqlParser::PipelineOpParamContext::DUBLQUOTED_STRING() {
  return getToken(sqlParser::DUBLQUOTED_STRING, 0);
}

sqlParser::TimestampLiteralContext* sqlParser::PipelineOpParamContext::timestampLiteral() {
  return getRuleContext<sqlParser::TimestampLiteralContext>(0);
}

sqlParser::IntervalLiteralContext* sqlParser::PipelineOpParamContext::intervalLiteral() {
  return getRuleContext<sqlParser::IntervalLiteralContext>(0);
}


size_t sqlParser::PipelineOpParamContext::getRuleIndex() const {
  return sqlParser::RulePipelineOpParam;
}


antlrcpp::Any sqlParser::PipelineOpParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitPipelineOpParam(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::PipelineOpParamContext* sqlParser::pipelineOpParam() {
  PipelineOpParamContext *_localctx = _tracker.createInstance<PipelineOpParamContext>(_ctx, getState());
  enterRule(_localctx, 126, sqlParser::RulePipelineOpParam);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1046);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 106, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1020);
      identifier();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1021);
      match(sqlParser::TYPE);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1022);
      match(sqlParser::AS);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(1023);
      match(sqlParser::SUM);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1024);
      match(sqlParser::AVG);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1025);
      match(sqlParser::MIN);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(1026);
      match(sqlParser::MAX);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(1027);
      match(sqlParser::T__7);
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(1028);
      match(sqlParser::T__10);
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(1029);
      match(sqlParser::T__14);
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(1030);
      match(sqlParser::T__11);
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(1031);
      match(sqlParser::T__4);
      break;
    }

    case 13: {
      enterOuterAlt(_localctx, 13);
      setState(1032);
      match(sqlParser::T__15);
      break;
    }

    case 14: {
      enterOuterAlt(_localctx, 14);
      setState(1033);
      match(sqlParser::T__16);
      break;
    }

    case 15: {
      enterOuterAlt(_localctx, 15);
      setState(1034);
      match(sqlParser::PARAM_IDENTIFIER);
      break;
    }

    case 16: {
      enterOuterAlt(_localctx, 16);
      setState(1035);
      pipelineTypeNames();
      break;
    }

    case 17: {
      enterOuterAlt(_localctx, 17);
      setState(1036);
      booleanLiteral();
      break;
    }

    case 18: {
      enterOuterAlt(_localctx, 18);
      setState(1037);
      signedIntegerLiteral();
      break;
    }

    case 19: {
      enterOuterAlt(_localctx, 19);
      setState(1038);
      signedDecimalLiteral();
      break;
    }

    case 20: {
      enterOuterAlt(_localctx, 20);
      setState(1039);
      signedFloatLiteral();
      break;
    }

    case 21: {
      enterOuterAlt(_localctx, 21);
      setState(1040);
      dateLiteral();
      break;
    }

    case 22: {
      enterOuterAlt(_localctx, 22);
      setState(1041);
      timeLiteral();
      break;
    }

    case 23: {
      enterOuterAlt(_localctx, 23);
      setState(1042);
      match(sqlParser::DUBLQUOTED_IDENTIFIER);
      break;
    }

    case 24: {
      enterOuterAlt(_localctx, 24);
      setState(1043);
      match(sqlParser::DUBLQUOTED_STRING);
      break;
    }

    case 25: {
      enterOuterAlt(_localctx, 25);
      setState(1044);
      timestampLiteral();
      break;
    }

    case 26: {
      enterOuterAlt(_localctx, 26);
      setState(1045);
      intervalLiteral();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PipelineTypeNamesContext ------------------------------------------------------------------

sqlParser::PipelineTypeNamesContext::PipelineTypeNamesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::PipelineTypeNamesContext::BOOLEAN() {
  return getToken(sqlParser::BOOLEAN, 0);
}

tree::TerminalNode* sqlParser::PipelineTypeNamesContext::CHAR() {
  return getToken(sqlParser::CHAR, 0);
}

tree::TerminalNode* sqlParser::PipelineTypeNamesContext::DATE() {
  return getToken(sqlParser::DATE, 0);
}

tree::TerminalNode* sqlParser::PipelineTypeNamesContext::DECIMAL() {
  return getToken(sqlParser::DECIMAL, 0);
}

tree::TerminalNode* sqlParser::PipelineTypeNamesContext::FLOAT() {
  return getToken(sqlParser::FLOAT, 0);
}

tree::TerminalNode* sqlParser::PipelineTypeNamesContext::INTEGER() {
  return getToken(sqlParser::INTEGER, 0);
}

tree::TerminalNode* sqlParser::PipelineTypeNamesContext::INTERVAL_DT() {
  return getToken(sqlParser::INTERVAL_DT, 0);
}

tree::TerminalNode* sqlParser::PipelineTypeNamesContext::INTERVAL_YM() {
  return getToken(sqlParser::INTERVAL_YM, 0);
}

tree::TerminalNode* sqlParser::PipelineTypeNamesContext::TIME() {
  return getToken(sqlParser::TIME, 0);
}

tree::TerminalNode* sqlParser::PipelineTypeNamesContext::TIMESTAMP() {
  return getToken(sqlParser::TIMESTAMP, 0);
}

tree::TerminalNode* sqlParser::PipelineTypeNamesContext::VARCHAR() {
  return getToken(sqlParser::VARCHAR, 0);
}

tree::TerminalNode* sqlParser::PipelineTypeNamesContext::SQL_NULL() {
  return getToken(sqlParser::SQL_NULL, 0);
}


size_t sqlParser::PipelineTypeNamesContext::getRuleIndex() const {
  return sqlParser::RulePipelineTypeNames;
}


antlrcpp::Any sqlParser::PipelineTypeNamesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitPipelineTypeNames(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::PipelineTypeNamesContext* sqlParser::pipelineTypeNames() {
  PipelineTypeNamesContext *_localctx = _tracker.createInstance<PipelineTypeNamesContext>(_ctx, getState());
  enterRule(_localctx, 128, sqlParser::RulePipelineTypeNames);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1048);
    _la = _input->LA(1);
    if (!(((((_la - 43) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 43)) & ((1ULL << (sqlParser::BOOLEAN - 43))
      | (1ULL << (sqlParser::CHAR - 43))
      | (1ULL << (sqlParser::DATE - 43))
      | (1ULL << (sqlParser::DECIMAL - 43))
      | (1ULL << (sqlParser::FLOAT - 43)))) != 0) || ((((_la - 118) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 118)) & ((1ULL << (sqlParser::INTEGER - 118))
      | (1ULL << (sqlParser::INTERVAL_DT - 118))
      | (1ULL << (sqlParser::INTERVAL_YM - 118))
      | (1ULL << (sqlParser::SQL_NULL - 118)))) != 0) || ((((_la - 215) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 215)) & ((1ULL << (sqlParser::TIME - 215))
      | (1ULL << (sqlParser::TIMESTAMP - 215))
      | (1ULL << (sqlParser::VARCHAR - 215)))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SleepCmdContext ------------------------------------------------------------------

sqlParser::SleepCmdContext::SleepCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SleepCmdContext::SLEEP() {
  return getToken(sqlParser::SLEEP, 0);
}

sqlParser::IntegerLiteralContext* sqlParser::SleepCmdContext::integerLiteral() {
  return getRuleContext<sqlParser::IntegerLiteralContext>(0);
}

sqlParser::SleepUnitsContext* sqlParser::SleepCmdContext::sleepUnits() {
  return getRuleContext<sqlParser::SleepUnitsContext>(0);
}


size_t sqlParser::SleepCmdContext::getRuleIndex() const {
  return sqlParser::RuleSleepCmd;
}


antlrcpp::Any sqlParser::SleepCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSleepCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SleepCmdContext* sqlParser::sleepCmd() {
  SleepCmdContext *_localctx = _tracker.createInstance<SleepCmdContext>(_ctx, getState());
  enterRule(_localctx, 130, sqlParser::RuleSleepCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1050);
    match(sqlParser::SLEEP);
    setState(1051);
    integerLiteral();
    setState(1053);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 142) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 142)) & ((1ULL << (sqlParser::MILLIS - 142))
      | (1ULL << (sqlParser::MILLISECOND - 142))
      | (1ULL << (sqlParser::MILLISECONDS - 142))
      | (1ULL << (sqlParser::MS - 142))
      | (1ULL << (sqlParser::SEC - 142))
      | (1ULL << (sqlParser::SECOND - 142))
      | (1ULL << (sqlParser::SECONDS - 142)))) != 0) || _la == sqlParser::S_ONLY) {
      setState(1052);
      sleepUnits();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SleepUnitsContext ------------------------------------------------------------------

sqlParser::SleepUnitsContext::SleepUnitsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SleepUnitsContext::S_ONLY() {
  return getToken(sqlParser::S_ONLY, 0);
}

tree::TerminalNode* sqlParser::SleepUnitsContext::SEC() {
  return getToken(sqlParser::SEC, 0);
}

tree::TerminalNode* sqlParser::SleepUnitsContext::SECOND() {
  return getToken(sqlParser::SECOND, 0);
}

tree::TerminalNode* sqlParser::SleepUnitsContext::SECONDS() {
  return getToken(sqlParser::SECONDS, 0);
}

tree::TerminalNode* sqlParser::SleepUnitsContext::MS() {
  return getToken(sqlParser::MS, 0);
}

tree::TerminalNode* sqlParser::SleepUnitsContext::MILLIS() {
  return getToken(sqlParser::MILLIS, 0);
}

tree::TerminalNode* sqlParser::SleepUnitsContext::MILLISECOND() {
  return getToken(sqlParser::MILLISECOND, 0);
}

tree::TerminalNode* sqlParser::SleepUnitsContext::MILLISECONDS() {
  return getToken(sqlParser::MILLISECONDS, 0);
}


size_t sqlParser::SleepUnitsContext::getRuleIndex() const {
  return sqlParser::RuleSleepUnits;
}


antlrcpp::Any sqlParser::SleepUnitsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSleepUnits(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SleepUnitsContext* sqlParser::sleepUnits() {
  SleepUnitsContext *_localctx = _tracker.createInstance<SleepUnitsContext>(_ctx, getState());
  enterRule(_localctx, 132, sqlParser::RuleSleepUnits);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1057);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::SEC:
      case sqlParser::SECOND:
      case sqlParser::SECONDS:
      case sqlParser::S_ONLY: {
        enterOuterAlt(_localctx, 1);
        setState(1055);
        dynamic_cast<SleepUnitsContext *>(_localctx)->seconds = _input->LT(1);
        _la = _input->LA(1);
        if (!(((((_la - 191) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 191)) & ((1ULL << (sqlParser::SEC - 191))
          | (1ULL << (sqlParser::SECOND - 191))
          | (1ULL << (sqlParser::SECONDS - 191))
          | (1ULL << (sqlParser::S_ONLY - 191)))) != 0))) {
          dynamic_cast<SleepUnitsContext *>(_localctx)->seconds = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        break;
      }

      case sqlParser::MILLIS:
      case sqlParser::MILLISECOND:
      case sqlParser::MILLISECONDS:
      case sqlParser::MS: {
        enterOuterAlt(_localctx, 2);
        setState(1056);
        dynamic_cast<SleepUnitsContext *>(_localctx)->millis = _input->LT(1);
        _la = _input->LA(1);
        if (!(((((_la - 142) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 142)) & ((1ULL << (sqlParser::MILLIS - 142))
          | (1ULL << (sqlParser::MILLISECOND - 142))
          | (1ULL << (sqlParser::MILLISECONDS - 142))
          | (1ULL << (sqlParser::MS - 142)))) != 0))) {
          dynamic_cast<SleepUnitsContext *>(_localctx)->millis = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SpaceCmdContext ------------------------------------------------------------------

sqlParser::SpaceCmdContext::SpaceCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SpaceCmdContext::SPACE() {
  return getToken(sqlParser::SPACE, 0);
}


size_t sqlParser::SpaceCmdContext::getRuleIndex() const {
  return sqlParser::RuleSpaceCmd;
}


antlrcpp::Any sqlParser::SpaceCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSpaceCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SpaceCmdContext* sqlParser::spaceCmd() {
  SpaceCmdContext *_localctx = _tracker.createInstance<SpaceCmdContext>(_ctx, getState());
  enterRule(_localctx, 134, sqlParser::RuleSpaceCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1059);
    match(sqlParser::SPACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatusCmdContext ------------------------------------------------------------------

sqlParser::StatusCmdContext::StatusCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::StatusCmdContext::STATUS() {
  return getToken(sqlParser::STATUS, 0);
}


size_t sqlParser::StatusCmdContext::getRuleIndex() const {
  return sqlParser::RuleStatusCmd;
}


antlrcpp::Any sqlParser::StatusCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitStatusCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::StatusCmdContext* sqlParser::statusCmd() {
  StatusCmdContext *_localctx = _tracker.createInstance<StatusCmdContext>(_ctx, getState());
  enterRule(_localctx, 136, sqlParser::RuleStatusCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1061);
    match(sqlParser::STATUS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SzidxCmdContext ------------------------------------------------------------------

sqlParser::SzidxCmdContext::SzidxCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SzidxCmdContext::SZIDX() {
  return getToken(sqlParser::SZIDX, 0);
}

sqlParser::TableNameContext* sqlParser::SzidxCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::IndexNameContext* sqlParser::SzidxCmdContext::indexName() {
  return getRuleContext<sqlParser::IndexNameContext>(0);
}


size_t sqlParser::SzidxCmdContext::getRuleIndex() const {
  return sqlParser::RuleSzidxCmd;
}


antlrcpp::Any sqlParser::SzidxCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSzidxCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SzidxCmdContext* sqlParser::szidxCmd() {
  SzidxCmdContext *_localctx = _tracker.createInstance<SzidxCmdContext>(_ctx, getState());
  enterRule(_localctx, 138, sqlParser::RuleSzidxCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1063);
    match(sqlParser::SZIDX);
    setState(1064);
    tableName();
    setState(1065);
    indexName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SztblCmdContext ------------------------------------------------------------------

sqlParser::SztblCmdContext::SztblCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SztblCmdContext::SZTBL() {
  return getToken(sqlParser::SZTBL, 0);
}

sqlParser::TableNameContext* sqlParser::SztblCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}


size_t sqlParser::SztblCmdContext::getRuleIndex() const {
  return sqlParser::RuleSztblCmd;
}


antlrcpp::Any sqlParser::SztblCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSztblCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SztblCmdContext* sqlParser::sztblCmd() {
  SztblCmdContext *_localctx = _tracker.createInstance<SztblCmdContext>(_ctx, getState());
  enterRule(_localctx, 140, sqlParser::RuleSztblCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1067);
    match(sqlParser::SZTBL);
    setState(1068);
    tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TableSizesCmdContext ------------------------------------------------------------------

sqlParser::TableSizesCmdContext::TableSizesCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::TableSizesCmdContext::TABLESIZES() {
  return getToken(sqlParser::TABLESIZES, 0);
}


size_t sqlParser::TableSizesCmdContext::getRuleIndex() const {
  return sqlParser::RuleTableSizesCmd;
}


antlrcpp::Any sqlParser::TableSizesCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTableSizesCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TableSizesCmdContext* sqlParser::tableSizesCmd() {
  TableSizesCmdContext *_localctx = _tracker.createInstance<TableSizesCmdContext>(_ctx, getState());
  enterRule(_localctx, 142, sqlParser::RuleTableSizesCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1070);
    match(sqlParser::TABLESIZES);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TruncateCmdContext ------------------------------------------------------------------

sqlParser::TruncateCmdContext::TruncateCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::TruncateCmdContext::TRUNCATE() {
  return getToken(sqlParser::TRUNCATE, 0);
}

sqlParser::TableNameContext* sqlParser::TruncateCmdContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

tree::TerminalNode* sqlParser::TruncateCmdContext::TABLE() {
  return getToken(sqlParser::TABLE, 0);
}


size_t sqlParser::TruncateCmdContext::getRuleIndex() const {
  return sqlParser::RuleTruncateCmd;
}


antlrcpp::Any sqlParser::TruncateCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTruncateCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TruncateCmdContext* sqlParser::truncateCmd() {
  TruncateCmdContext *_localctx = _tracker.createInstance<TruncateCmdContext>(_ctx, getState());
  enterRule(_localctx, 144, sqlParser::RuleTruncateCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1072);
    match(sqlParser::TRUNCATE);
    setState(1074);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::TABLE) {
      setState(1073);
      match(sqlParser::TABLE);
    }
    setState(1076);
    dynamic_cast<TruncateCmdContext *>(_localctx)->name = tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TrackmemoryCmdContext ------------------------------------------------------------------

sqlParser::TrackmemoryCmdContext::TrackmemoryCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::TrackmemoryCmdContext::TRACKMEMORY() {
  return getToken(sqlParser::TRACKMEMORY, 0);
}

tree::TerminalNode* sqlParser::TrackmemoryCmdContext::FORCE() {
  return getToken(sqlParser::FORCE, 0);
}


size_t sqlParser::TrackmemoryCmdContext::getRuleIndex() const {
  return sqlParser::RuleTrackmemoryCmd;
}


antlrcpp::Any sqlParser::TrackmemoryCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTrackmemoryCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TrackmemoryCmdContext* sqlParser::trackmemoryCmd() {
  TrackmemoryCmdContext *_localctx = _tracker.createInstance<TrackmemoryCmdContext>(_ctx, getState());
  enterRule(_localctx, 146, sqlParser::RuleTrackmemoryCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1078);
    match(sqlParser::TRACKMEMORY);
    setState(1080);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::FORCE) {
      setState(1079);
      match(sqlParser::FORCE);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UpdateCmdContext ------------------------------------------------------------------

sqlParser::UpdateCmdContext::UpdateCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::UpdateCmdContext::UPDATE() {
  return getToken(sqlParser::UPDATE, 0);
}

tree::TerminalNode* sqlParser::UpdateCmdContext::SET() {
  return getToken(sqlParser::SET, 0);
}

std::vector<sqlParser::ColumnNameContext *> sqlParser::UpdateCmdContext::columnName() {
  return getRuleContexts<sqlParser::ColumnNameContext>();
}

sqlParser::ColumnNameContext* sqlParser::UpdateCmdContext::columnName(size_t i) {
  return getRuleContext<sqlParser::ColumnNameContext>(i);
}

std::vector<sqlParser::ExprContext *> sqlParser::UpdateCmdContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::UpdateCmdContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::TableReferenceContext* sqlParser::UpdateCmdContext::tableReference() {
  return getRuleContext<sqlParser::TableReferenceContext>(0);
}

tree::TerminalNode* sqlParser::UpdateCmdContext::WITH() {
  return getToken(sqlParser::WITH, 0);
}

std::vector<sqlParser::WithQueryContext *> sqlParser::UpdateCmdContext::withQuery() {
  return getRuleContexts<sqlParser::WithQueryContext>();
}

sqlParser::WithQueryContext* sqlParser::UpdateCmdContext::withQuery(size_t i) {
  return getRuleContext<sqlParser::WithQueryContext>(i);
}

tree::TerminalNode* sqlParser::UpdateCmdContext::WHERE() {
  return getToken(sqlParser::WHERE, 0);
}

sqlParser::PredicateContext* sqlParser::UpdateCmdContext::predicate() {
  return getRuleContext<sqlParser::PredicateContext>(0);
}

tree::TerminalNode* sqlParser::UpdateCmdContext::FROM() {
  return getToken(sqlParser::FROM, 0);
}

sqlParser::TableExprContext* sqlParser::UpdateCmdContext::tableExpr() {
  return getRuleContext<sqlParser::TableExprContext>(0);
}


size_t sqlParser::UpdateCmdContext::getRuleIndex() const {
  return sqlParser::RuleUpdateCmd;
}


antlrcpp::Any sqlParser::UpdateCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitUpdateCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::UpdateCmdContext* sqlParser::updateCmd() {
  UpdateCmdContext *_localctx = _tracker.createInstance<UpdateCmdContext>(_ctx, getState());
  enterRule(_localctx, 148, sqlParser::RuleUpdateCmd);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1091);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(1082);
      match(sqlParser::WITH);
      setState(1083);
      withQuery();
      setState(1088);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(1084);
        match(sqlParser::T__5);
        setState(1085);
        withQuery();
        setState(1090);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(1093);
    match(sqlParser::UPDATE);
    setState(1094);
    dynamic_cast<UpdateCmdContext *>(_localctx)->ref = tableReference();
    setState(1095);
    match(sqlParser::SET);
    setState(1096);
    columnName();
    setState(1097);
    match(sqlParser::T__3);
    setState(1098);
    expr(0);
    setState(1106);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1099);
      match(sqlParser::T__5);
      setState(1100);
      columnName();
      setState(1101);
      match(sqlParser::T__3);
      setState(1102);
      expr(0);
      setState(1108);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(1113);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(1109);
        match(sqlParser::WHERE);
        setState(1110);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(1111);
        match(sqlParser::FROM);
        setState(1112);
        tableExpr(0);
        break;
      }

      case sqlParser::EOF: {
        break;
      }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnloadCmdContext ------------------------------------------------------------------

sqlParser::UnloadCmdContext::UnloadCmdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::UnloadCmdContext::UNLOAD() {
  return getToken(sqlParser::UNLOAD, 0);
}

sqlParser::IdentifierContext* sqlParser::UnloadCmdContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}

sqlParser::StringLiteralContext* sqlParser::UnloadCmdContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}


size_t sqlParser::UnloadCmdContext::getRuleIndex() const {
  return sqlParser::RuleUnloadCmd;
}


antlrcpp::Any sqlParser::UnloadCmdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitUnloadCmd(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::UnloadCmdContext* sqlParser::unloadCmd() {
  UnloadCmdContext *_localctx = _tracker.createInstance<UnloadCmdContext>(_ctx, getState());
  enterRule(_localctx, 150, sqlParser::RuleUnloadCmd);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1115);
    match(sqlParser::UNLOAD);
    setState(1118);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 115, _ctx)) {
    case 1: {
      setState(1116);
      identifier();
      break;
    }

    case 2: {
      setState(1117);
      stringLiteral();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TableDefinitionContext ------------------------------------------------------------------

sqlParser::TableDefinitionContext::TableDefinitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<sqlParser::ColumnDefinitionContext *> sqlParser::TableDefinitionContext::columnDefinition() {
  return getRuleContexts<sqlParser::ColumnDefinitionContext>();
}

sqlParser::ColumnDefinitionContext* sqlParser::TableDefinitionContext::columnDefinition(size_t i) {
  return getRuleContext<sqlParser::ColumnDefinitionContext>(i);
}

std::vector<sqlParser::TablePartitioningContext *> sqlParser::TableDefinitionContext::tablePartitioning() {
  return getRuleContexts<sqlParser::TablePartitioningContext>();
}

sqlParser::TablePartitioningContext* sqlParser::TableDefinitionContext::tablePartitioning(size_t i) {
  return getRuleContext<sqlParser::TablePartitioningContext>(i);
}

std::vector<tree::TerminalNode *> sqlParser::TableDefinitionContext::REPLICATED() {
  return getTokens(sqlParser::REPLICATED);
}

tree::TerminalNode* sqlParser::TableDefinitionContext::REPLICATED(size_t i) {
  return getToken(sqlParser::REPLICATED, i);
}

std::vector<tree::TerminalNode *> sqlParser::TableDefinitionContext::ROWSIZE() {
  return getTokens(sqlParser::ROWSIZE);
}

tree::TerminalNode* sqlParser::TableDefinitionContext::ROWSIZE(size_t i) {
  return getToken(sqlParser::ROWSIZE, i);
}

std::vector<sqlParser::IntegerLiteralContext *> sqlParser::TableDefinitionContext::integerLiteral() {
  return getRuleContexts<sqlParser::IntegerLiteralContext>();
}

sqlParser::IntegerLiteralContext* sqlParser::TableDefinitionContext::integerLiteral(size_t i) {
  return getRuleContext<sqlParser::IntegerLiteralContext>(i);
}

std::vector<tree::TerminalNode *> sqlParser::TableDefinitionContext::TABLESIZE() {
  return getTokens(sqlParser::TABLESIZE);
}

tree::TerminalNode* sqlParser::TableDefinitionContext::TABLESIZE(size_t i) {
  return getToken(sqlParser::TABLESIZE, i);
}


size_t sqlParser::TableDefinitionContext::getRuleIndex() const {
  return sqlParser::RuleTableDefinition;
}


antlrcpp::Any sqlParser::TableDefinitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTableDefinition(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TableDefinitionContext* sqlParser::tableDefinition() {
  TableDefinitionContext *_localctx = _tracker.createInstance<TableDefinitionContext>(_ctx, getState());
  enterRule(_localctx, 152, sqlParser::RuleTableDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1120);
    match(sqlParser::T__6);
    setState(1123);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::ANALYSE:
      case sqlParser::ANALYZE:
      case sqlParser::ANIDX:
      case sqlParser::ANTBL:
      case sqlParser::ASC:
      case sqlParser::BIT:
      case sqlParser::BITS:
      case sqlParser::BLOCK:
      case sqlParser::BLOOM:
      case sqlParser::BPA:
      case sqlParser::BTREE:
      case sqlParser::BYE:
      case sqlParser::BYTE:
      case sqlParser::BYTES:
      case sqlParser::CATALOG:
      case sqlParser::CHARS:
      case sqlParser::COMPACTIDX:
      case sqlParser::CONCAT:
      case sqlParser::DATA:
      case sqlParser::DATE_FORMAT:
      case sqlParser::DAYS:
      case sqlParser::DECODE:
      case sqlParser::DESC:
      case sqlParser::DETAIL:
      case sqlParser::DIGITS:
      case sqlParser::DLOAD:
      case sqlParser::DOCOL:
      case sqlParser::ECHO:
      case sqlParser::ESCAPE_CHAR:
      case sqlParser::EXIT_TOKEN:
      case sqlParser::EXPLAIN:
      case sqlParser::EXPORT:
      case sqlParser::SQL_FALSE:
      case sqlParser::FEDERATION:
      case sqlParser::FIELD_DELIMITER:
      case sqlParser::FILE:
      case sqlParser::FORCE:
      case sqlParser::FORMAT:
      case sqlParser::HASH:
      case sqlParser::HASHKEY:
      case sqlParser::HOURS:
      case sqlParser::IF:
      case sqlParser::IFNULL:
      case sqlParser::INDEX:
      case sqlParser::INTERVAL_DT:
      case sqlParser::INTERVAL_YM:
      case sqlParser::KEY:
      case sqlParser::LIMIT:
      case sqlParser::LOADREPLICATED:
      case sqlParser::LOAD:
      case sqlParser::LOG:
      case sqlParser::LOOP:
      case sqlParser::LSIDX:
      case sqlParser::LSPTNCOL:
      case sqlParser::LSTBL:
      case sqlParser::MEMORYLEAKS:
      case sqlParser::MILLIS:
      case sqlParser::MILLISECOND:
      case sqlParser::MILLISECONDS:
      case sqlParser::MINUTES:
      case sqlParser::MONOTONIC:
      case sqlParser::MONTHS:
      case sqlParser::MS:
      case sqlParser::NODES:
      case sqlParser::NODE:
      case sqlParser::NOOP:
      case sqlParser::NOP:
      case sqlParser::NOW:
      case sqlParser::OBJECTS:
      case sqlParser::OFF:
      case sqlParser::OFFSET:
      case sqlParser::ORIGIN:
      case sqlParser::PARTITIONS:
      case sqlParser::PATHS:
      case sqlParser::PIPELINE:
      case sqlParser::PLACING:
      case sqlParser::PRETTY:
      case sqlParser::QUARTER:
      case sqlParser::QUARTERS:
      case sqlParser::RECORD_ENDS:
      case sqlParser::REFERENCE:
      case sqlParser::REGIONS:
      case sqlParser::REPLICATED:
      case sqlParser::ROWSIZE:
      case sqlParser::SCHEMA:
      case sqlParser::SEC:
      case sqlParser::SECONDS:
      case sqlParser::SKIPLIST:
      case sqlParser::SLEEP:
      case sqlParser::SPACE:
      case sqlParser::STAGE:
      case sqlParser::STATS:
      case sqlParser::STATISTICS:
      case sqlParser::STATUS:
      case sqlParser::SUBSTR:
      case sqlParser::SZIDX:
      case sqlParser::SZTBL:
      case sqlParser::S_ONLY:
      case sqlParser::TABLESIZE:
      case sqlParser::TEXT:
      case sqlParser::TOP:
      case sqlParser::TRACKMEMORY:
      case sqlParser::TREE:
      case sqlParser::TRUNCATE:
      case sqlParser::TYPE:
      case sqlParser::TYPEOF:
      case sqlParser::UNLOAD:
      case sqlParser::VALUE:
      case sqlParser::WEEK:
      case sqlParser::WEEKS:
      case sqlParser::WITH_NAMES:
      case sqlParser::WITH_TYPES:
      case sqlParser::XOR:
      case sqlParser::YEARS:
      case sqlParser::PLAIN_IDENTIFIER:
      case sqlParser::BAKTIKD_IDENTIFIER:
      case sqlParser::DUBLQUOTED_IDENTIFIER: {
        setState(1121);
        columnDefinition();
        break;
      }

      case sqlParser::T__5:
      case sqlParser::T__7: {
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(1132);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1125);
      match(sqlParser::T__5);
      setState(1128);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ANALYSE:
        case sqlParser::ANALYZE:
        case sqlParser::ANIDX:
        case sqlParser::ANTBL:
        case sqlParser::ASC:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::COMPACTIDX:
        case sqlParser::CONCAT:
        case sqlParser::DATA:
        case sqlParser::DATE_FORMAT:
        case sqlParser::DAYS:
        case sqlParser::DECODE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DIGITS:
        case sqlParser::DLOAD:
        case sqlParser::DOCOL:
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXIT_TOKEN:
        case sqlParser::EXPLAIN:
        case sqlParser::EXPORT:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
        case sqlParser::FILE:
        case sqlParser::FORCE:
        case sqlParser::FORMAT:
        case sqlParser::HASH:
        case sqlParser::HASHKEY:
        case sqlParser::HOURS:
        case sqlParser::IF:
        case sqlParser::IFNULL:
        case sqlParser::INDEX:
        case sqlParser::INTERVAL_DT:
        case sqlParser::INTERVAL_YM:
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOADREPLICATED:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSPTNCOL:
        case sqlParser::LSTBL:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MILLIS:
        case sqlParser::MILLISECOND:
        case sqlParser::MILLISECONDS:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::MS:
        case sqlParser::NODES:
        case sqlParser::NODE:
        case sqlParser::NOOP:
        case sqlParser::NOP:
        case sqlParser::NOW:
        case sqlParser::OBJECTS:
        case sqlParser::OFF:
        case sqlParser::OFFSET:
        case sqlParser::ORIGIN:
        case sqlParser::PARTITIONS:
        case sqlParser::PATHS:
        case sqlParser::PIPELINE:
        case sqlParser::PLACING:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::RECORD_ENDS:
        case sqlParser::REFERENCE:
        case sqlParser::REGIONS:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SEC:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
        case sqlParser::SLEEP:
        case sqlParser::SPACE:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::STATUS:
        case sqlParser::SUBSTR:
        case sqlParser::SZIDX:
        case sqlParser::SZTBL:
        case sqlParser::S_ONLY:
        case sqlParser::TABLESIZE:
        case sqlParser::TEXT:
        case sqlParser::TOP:
        case sqlParser::TRACKMEMORY:
        case sqlParser::TREE:
        case sqlParser::TRUNCATE:
        case sqlParser::TYPE:
        case sqlParser::TYPEOF:
        case sqlParser::UNLOAD:
        case sqlParser::VALUE:
        case sqlParser::WEEK:
        case sqlParser::WEEKS:
        case sqlParser::WITH_NAMES:
        case sqlParser::WITH_TYPES:
        case sqlParser::XOR:
        case sqlParser::YEARS:
        case sqlParser::PLAIN_IDENTIFIER:
        case sqlParser::BAKTIKD_IDENTIFIER:
        case sqlParser::DUBLQUOTED_IDENTIFIER: {
          setState(1126);
          columnDefinition();
          break;
        }

        case sqlParser::T__5:
        case sqlParser::T__7: {
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(1134);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(1135);
    match(sqlParser::T__7);
    setState(1144);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 173) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 173)) & ((1ULL << (sqlParser::PARTITION - 173))
      | (1ULL << (sqlParser::REPLICATED - 173))
      | (1ULL << (sqlParser::ROWSIZE - 173))
      | (1ULL << (sqlParser::TABLESIZE - 173)))) != 0)) {
      setState(1142);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::PARTITION: {
          setState(1136);
          tablePartitioning();
          break;
        }

        case sqlParser::REPLICATED: {
          setState(1137);
          match(sqlParser::REPLICATED);
          break;
        }

        case sqlParser::ROWSIZE: {
          setState(1138);
          match(sqlParser::ROWSIZE);
          setState(1139);
          integerLiteral();
          break;
        }

        case sqlParser::TABLESIZE: {
          setState(1140);
          match(sqlParser::TABLESIZE);
          setState(1141);
          integerLiteral();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(1146);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TablePartitioningContext ------------------------------------------------------------------

sqlParser::TablePartitioningContext::TablePartitioningContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::TablePartitioningContext::PARTITION() {
  return getToken(sqlParser::PARTITION, 0);
}

tree::TerminalNode* sqlParser::TablePartitioningContext::BY() {
  return getToken(sqlParser::BY, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::TablePartitioningContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::TablePartitioningContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}


size_t sqlParser::TablePartitioningContext::getRuleIndex() const {
  return sqlParser::RuleTablePartitioning;
}


antlrcpp::Any sqlParser::TablePartitioningContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTablePartitioning(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TablePartitioningContext* sqlParser::tablePartitioning() {
  TablePartitioningContext *_localctx = _tracker.createInstance<TablePartitioningContext>(_ctx, getState());
  enterRule(_localctx, 154, sqlParser::RuleTablePartitioning);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1147);
    match(sqlParser::PARTITION);
    setState(1148);
    match(sqlParser::BY);
    setState(1149);
    match(sqlParser::T__6);
    setState(1150);
    expr(0);
    setState(1155);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1151);
      match(sqlParser::T__5);
      setState(1152);
      expr(0);
      setState(1157);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(1158);
    match(sqlParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ColumnDefinitionContext ------------------------------------------------------------------

sqlParser::ColumnDefinitionContext::ColumnDefinitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::ColumnNameContext* sqlParser::ColumnDefinitionContext::columnName() {
  return getRuleContext<sqlParser::ColumnNameContext>(0);
}

sqlParser::DataTypeContext* sqlParser::ColumnDefinitionContext::dataType() {
  return getRuleContext<sqlParser::DataTypeContext>(0);
}

std::vector<tree::TerminalNode *> sqlParser::ColumnDefinitionContext::NOT() {
  return getTokens(sqlParser::NOT);
}

tree::TerminalNode* sqlParser::ColumnDefinitionContext::NOT(size_t i) {
  return getToken(sqlParser::NOT, i);
}

std::vector<tree::TerminalNode *> sqlParser::ColumnDefinitionContext::SQL_NULL() {
  return getTokens(sqlParser::SQL_NULL);
}

tree::TerminalNode* sqlParser::ColumnDefinitionContext::SQL_NULL(size_t i) {
  return getToken(sqlParser::SQL_NULL, i);
}


size_t sqlParser::ColumnDefinitionContext::getRuleIndex() const {
  return sqlParser::RuleColumnDefinition;
}


antlrcpp::Any sqlParser::ColumnDefinitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitColumnDefinition(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ColumnDefinitionContext* sqlParser::columnDefinition() {
  ColumnDefinitionContext *_localctx = _tracker.createInstance<ColumnDefinitionContext>(_ctx, getState());
  enterRule(_localctx, 156, sqlParser::RuleColumnDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1160);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->name = columnName();
    setState(1161);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->type = dataType();
    setState(1166);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::NOT) {
      setState(1162);
      dynamic_cast<ColumnDefinitionContext *>(_localctx)->notNullable = match(sqlParser::NOT);
      setState(1163);
      dynamic_cast<ColumnDefinitionContext *>(_localctx)->nullable = match(sqlParser::SQL_NULL);
      setState(1168);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ColumnNameListContext ------------------------------------------------------------------

sqlParser::ColumnNameListContext::ColumnNameListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<sqlParser::ColumnNameContext *> sqlParser::ColumnNameListContext::columnName() {
  return getRuleContexts<sqlParser::ColumnNameContext>();
}

sqlParser::ColumnNameContext* sqlParser::ColumnNameListContext::columnName(size_t i) {
  return getRuleContext<sqlParser::ColumnNameContext>(i);
}


size_t sqlParser::ColumnNameListContext::getRuleIndex() const {
  return sqlParser::RuleColumnNameList;
}


antlrcpp::Any sqlParser::ColumnNameListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitColumnNameList(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ColumnNameListContext* sqlParser::columnNameList() {
  ColumnNameListContext *_localctx = _tracker.createInstance<ColumnNameListContext>(_ctx, getState());
  enterRule(_localctx, 158, sqlParser::RuleColumnNameList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1169);
    match(sqlParser::T__6);
    setState(1170);
    columnName();
    setState(1175);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1171);
      match(sqlParser::T__5);
      setState(1172);
      columnName();
      setState(1177);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(1178);
    match(sqlParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IndexDefinitionContext ------------------------------------------------------------------

sqlParser::IndexDefinitionContext::IndexDefinitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::IndexDefinitionContext::INDEX() {
  return getToken(sqlParser::INDEX, 0);
}

tree::TerminalNode* sqlParser::IndexDefinitionContext::KEY() {
  return getToken(sqlParser::KEY, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::IndexDefinitionContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::IndexDefinitionContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::IndexDefinitionContext::ON() {
  return getToken(sqlParser::ON, 0);
}

tree::TerminalNode* sqlParser::IndexDefinitionContext::USING() {
  return getToken(sqlParser::USING, 0);
}

tree::TerminalNode* sqlParser::IndexDefinitionContext::WITH() {
  return getToken(sqlParser::WITH, 0);
}

tree::TerminalNode* sqlParser::IndexDefinitionContext::UNIQUE() {
  return getToken(sqlParser::UNIQUE, 0);
}

sqlParser::IndexNameContext* sqlParser::IndexDefinitionContext::indexName() {
  return getRuleContext<sqlParser::IndexNameContext>(0);
}

sqlParser::IndexMethodContext* sqlParser::IndexDefinitionContext::indexMethod() {
  return getRuleContext<sqlParser::IndexMethodContext>(0);
}

sqlParser::PropertyListContext* sqlParser::IndexDefinitionContext::propertyList() {
  return getRuleContext<sqlParser::PropertyListContext>(0);
}


size_t sqlParser::IndexDefinitionContext::getRuleIndex() const {
  return sqlParser::RuleIndexDefinition;
}


antlrcpp::Any sqlParser::IndexDefinitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitIndexDefinition(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::IndexDefinitionContext* sqlParser::indexDefinition() {
  IndexDefinitionContext *_localctx = _tracker.createInstance<IndexDefinitionContext>(_ctx, getState());
  enterRule(_localctx, 160, sqlParser::RuleIndexDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1181);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(1180);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
    }
    setState(1183);
    _la = _input->LA(1);
    if (!(_la == sqlParser::INDEX

    || _la == sqlParser::KEY)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(1185);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 125, _ctx)) {
    case 1: {
      setState(1184);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->name = indexName();
      break;
    }

    }
    setState(1188);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ON) {
      setState(1187);
      match(sqlParser::ON);
    }
    setState(1202);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 128, _ctx)) {
    case 1: {
      setState(1190);
      expr(0);
      break;
    }

    case 2: {
      setState(1191);
      match(sqlParser::T__6);
      setState(1192);
      expr(0);
      setState(1197);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(1193);
        match(sqlParser::T__5);
        setState(1194);
        expr(0);
        setState(1199);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(1200);
      match(sqlParser::T__7);
      break;
    }

    }
    setState(1206);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::USING) {
      setState(1204);
      match(sqlParser::USING);
      setState(1205);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->method = indexMethod();
    }
    setState(1210);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(1208);
      match(sqlParser::WITH);
      setState(1209);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->props = propertyList();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IndexMethodContext ------------------------------------------------------------------

sqlParser::IndexMethodContext::IndexMethodContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::IndexMethodContext::HASH() {
  return getToken(sqlParser::HASH, 0);
}

tree::TerminalNode* sqlParser::IndexMethodContext::TREE() {
  return getToken(sqlParser::TREE, 0);
}

tree::TerminalNode* sqlParser::IndexMethodContext::BTREE() {
  return getToken(sqlParser::BTREE, 0);
}

tree::TerminalNode* sqlParser::IndexMethodContext::SKIPLIST() {
  return getToken(sqlParser::SKIPLIST, 0);
}


size_t sqlParser::IndexMethodContext::getRuleIndex() const {
  return sqlParser::RuleIndexMethod;
}


antlrcpp::Any sqlParser::IndexMethodContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitIndexMethod(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::IndexMethodContext* sqlParser::indexMethod() {
  IndexMethodContext *_localctx = _tracker.createInstance<IndexMethodContext>(_ctx, getState());
  enterRule(_localctx, 162, sqlParser::RuleIndexMethod);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1212);
    _la = _input->LA(1);
    if (!(_la == sqlParser::BTREE

    || _la == sqlParser::HASH || _la == sqlParser::SKIPLIST

    || _la == sqlParser::TREE)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DataTypeContext ------------------------------------------------------------------

sqlParser::DataTypeContext::DataTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::SqlDataTypeContext* sqlParser::DataTypeContext::sqlDataType() {
  return getRuleContext<sqlParser::SqlDataTypeContext>(0);
}


size_t sqlParser::DataTypeContext::getRuleIndex() const {
  return sqlParser::RuleDataType;
}


antlrcpp::Any sqlParser::DataTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDataType(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DataTypeContext* sqlParser::dataType() {
  DataTypeContext *_localctx = _tracker.createInstance<DataTypeContext>(_ctx, getState());
  enterRule(_localctx, 164, sqlParser::RuleDataType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1214);
    sqlDataType();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SqlDataTypeContext ------------------------------------------------------------------

sqlParser::SqlDataTypeContext::SqlDataTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::BOOLEAN() {
  return getToken(sqlParser::BOOLEAN, 0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::INTEGER() {
  return getToken(sqlParser::INTEGER, 0);
}

sqlParser::PrecisionContext* sqlParser::SqlDataTypeContext::precision() {
  return getRuleContext<sqlParser::PrecisionContext>(0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::BITS() {
  return getToken(sqlParser::BITS, 0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::BYTES() {
  return getToken(sqlParser::BYTES, 0);
}

std::vector<tree::TerminalNode *> sqlParser::SqlDataTypeContext::DIGITS() {
  return getTokens(sqlParser::DIGITS);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::DIGITS(size_t i) {
  return getToken(sqlParser::DIGITS, i);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::DECIMAL() {
  return getToken(sqlParser::DECIMAL, 0);
}

sqlParser::ScaleContext* sqlParser::SqlDataTypeContext::scale() {
  return getRuleContext<sqlParser::ScaleContext>(0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::FLOAT() {
  return getToken(sqlParser::FLOAT, 0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::DATE() {
  return getToken(sqlParser::DATE, 0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::TIME() {
  return getToken(sqlParser::TIME, 0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::TIMESTAMP() {
  return getToken(sqlParser::TIMESTAMP, 0);
}

sqlParser::IntervalTypeContext* sqlParser::SqlDataTypeContext::intervalType() {
  return getRuleContext<sqlParser::IntervalTypeContext>(0);
}

sqlParser::FixedLenCharContext* sqlParser::SqlDataTypeContext::fixedLenChar() {
  return getRuleContext<sqlParser::FixedLenCharContext>(0);
}

sqlParser::VaryingLenCharContext* sqlParser::SqlDataTypeContext::varyingLenChar() {
  return getRuleContext<sqlParser::VaryingLenCharContext>(0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::BINARY() {
  return getToken(sqlParser::BINARY, 0);
}

sqlParser::SizeContext* sqlParser::SqlDataTypeContext::size() {
  return getRuleContext<sqlParser::SizeContext>(0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::BYTE() {
  return getToken(sqlParser::BYTE, 0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::VARBINARY() {
  return getToken(sqlParser::VARBINARY, 0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::BLOB() {
  return getToken(sqlParser::BLOB, 0);
}


size_t sqlParser::SqlDataTypeContext::getRuleIndex() const {
  return sqlParser::RuleSqlDataType;
}


antlrcpp::Any sqlParser::SqlDataTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSqlDataType(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SqlDataTypeContext* sqlParser::sqlDataType() {
  SqlDataTypeContext *_localctx = _tracker.createInstance<SqlDataTypeContext>(_ctx, getState());
  enterRule(_localctx, 166, sqlParser::RuleSqlDataType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1279);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 142, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1216);
      match(sqlParser::BOOLEAN);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1217);
      match(sqlParser::INTEGER);
      setState(1225);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1218);
        match(sqlParser::T__6);
        setState(1219);
        precision();
        setState(1221);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 38) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 38)) & ((1ULL << (sqlParser::BITS - 38))
          | (1ULL << (sqlParser::BYTES - 38))
          | (1ULL << (sqlParser::DIGITS - 38)))) != 0)) {
          setState(1220);
          _la = _input->LA(1);
          if (!(((((_la - 38) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 38)) & ((1ULL << (sqlParser::BITS - 38))
            | (1ULL << (sqlParser::BYTES - 38))
            | (1ULL << (sqlParser::DIGITS - 38)))) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        setState(1223);
        match(sqlParser::T__7);
      }
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1227);
      match(sqlParser::DECIMAL);
      setState(1242);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1228);
        match(sqlParser::T__6);
        setState(1229);
        precision();
        setState(1231);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::DIGITS) {
          setState(1230);
          match(sqlParser::DIGITS);
        }
        setState(1238);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1233);
          match(sqlParser::T__5);
          setState(1234);
          scale();
          setState(1236);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::DIGITS) {
            setState(1235);
            match(sqlParser::DIGITS);
          }
        }
        setState(1240);
        match(sqlParser::T__7);
      }
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(1244);
      match(sqlParser::FLOAT);
      setState(1252);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1245);
        match(sqlParser::T__6);
        setState(1246);
        precision();
        setState(1248);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 38) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 38)) & ((1ULL << (sqlParser::BITS - 38))
          | (1ULL << (sqlParser::BYTES - 38))
          | (1ULL << (sqlParser::DIGITS - 38)))) != 0)) {
          setState(1247);
          _la = _input->LA(1);
          if (!(((((_la - 38) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 38)) & ((1ULL << (sqlParser::BITS - 38))
            | (1ULL << (sqlParser::BYTES - 38))
            | (1ULL << (sqlParser::DIGITS - 38)))) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        setState(1250);
        match(sqlParser::T__7);
      }
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1254);
      match(sqlParser::DATE);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1255);
      match(sqlParser::TIME);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(1256);
      match(sqlParser::TIMESTAMP);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(1257);
      intervalType();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(1258);
      fixedLenChar();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(1259);
      varyingLenChar();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(1260);
      match(sqlParser::BINARY);

      setState(1261);
      match(sqlParser::T__6);
      setState(1262);
      size();
      setState(1264);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::BYTE

      || _la == sqlParser::BYTES) {
        setState(1263);
        _la = _input->LA(1);
        if (!(_la == sqlParser::BYTE

        || _la == sqlParser::BYTES)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
      }
      setState(1266);
      match(sqlParser::T__7);
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(1268);
      match(sqlParser::VARBINARY);
      setState(1276);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1269);
        match(sqlParser::T__6);
        setState(1270);
        size();
        setState(1272);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::BYTE

        || _la == sqlParser::BYTES) {
          setState(1271);
          _la = _input->LA(1);
          if (!(_la == sqlParser::BYTE

          || _la == sqlParser::BYTES)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        setState(1274);
        match(sqlParser::T__7);
      }
      break;
    }

    case 13: {
      enterOuterAlt(_localctx, 13);
      setState(1278);
      match(sqlParser::BLOB);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntervalTypeContext ------------------------------------------------------------------

sqlParser::IntervalTypeContext::IntervalTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::IntervalTypeContext::INTERVAL() {
  return getToken(sqlParser::INTERVAL, 0);
}

sqlParser::YearMonthIntervalTypeContext* sqlParser::IntervalTypeContext::yearMonthIntervalType() {
  return getRuleContext<sqlParser::YearMonthIntervalTypeContext>(0);
}

sqlParser::DayTimeIntervalTypeContext* sqlParser::IntervalTypeContext::dayTimeIntervalType() {
  return getRuleContext<sqlParser::DayTimeIntervalTypeContext>(0);
}


size_t sqlParser::IntervalTypeContext::getRuleIndex() const {
  return sqlParser::RuleIntervalType;
}


antlrcpp::Any sqlParser::IntervalTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitIntervalType(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::IntervalTypeContext* sqlParser::intervalType() {
  IntervalTypeContext *_localctx = _tracker.createInstance<IntervalTypeContext>(_ctx, getState());
  enterRule(_localctx, 168, sqlParser::RuleIntervalType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1281);
    match(sqlParser::INTERVAL);
    setState(1284);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::MONTH:
      case sqlParser::YEAR: {
        setState(1282);
        yearMonthIntervalType();
        break;
      }

      case sqlParser::DAY:
      case sqlParser::HOUR:
      case sqlParser::MINUTE:
      case sqlParser::SECOND: {
        setState(1283);
        dayTimeIntervalType();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- YearMonthIntervalTypeContext ------------------------------------------------------------------

sqlParser::YearMonthIntervalTypeContext::YearMonthIntervalTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::YearMonthIntervalTypeContext::YEAR() {
  return getToken(sqlParser::YEAR, 0);
}

sqlParser::PrecisionContext* sqlParser::YearMonthIntervalTypeContext::precision() {
  return getRuleContext<sqlParser::PrecisionContext>(0);
}

tree::TerminalNode* sqlParser::YearMonthIntervalTypeContext::TO() {
  return getToken(sqlParser::TO, 0);
}

tree::TerminalNode* sqlParser::YearMonthIntervalTypeContext::MONTH() {
  return getToken(sqlParser::MONTH, 0);
}


size_t sqlParser::YearMonthIntervalTypeContext::getRuleIndex() const {
  return sqlParser::RuleYearMonthIntervalType;
}


antlrcpp::Any sqlParser::YearMonthIntervalTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitYearMonthIntervalType(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::YearMonthIntervalTypeContext* sqlParser::yearMonthIntervalType() {
  YearMonthIntervalTypeContext *_localctx = _tracker.createInstance<YearMonthIntervalTypeContext>(_ctx, getState());
  enterRule(_localctx, 170, sqlParser::RuleYearMonthIntervalType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1309);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 147, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1286);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1291);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1287);
        match(sqlParser::T__6);
        setState(1288);
        precision();
        setState(1289);
        match(sqlParser::T__7);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1293);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1298);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1294);
        match(sqlParser::T__6);
        setState(1295);
        precision();
        setState(1296);
        match(sqlParser::T__7);
      }
      setState(1300);
      match(sqlParser::TO);
      setState(1301);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->month = match(sqlParser::MONTH);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1302);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->month = match(sqlParser::MONTH);
      setState(1307);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1303);
        match(sqlParser::T__6);
        setState(1304);
        precision();
        setState(1305);
        match(sqlParser::T__7);
      }
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DayTimeIntervalTypeContext ------------------------------------------------------------------

sqlParser::DayTimeIntervalTypeContext::DayTimeIntervalTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DayTimeIntervalTypeContext::DAY() {
  return getToken(sqlParser::DAY, 0);
}

sqlParser::PrecisionContext* sqlParser::DayTimeIntervalTypeContext::precision() {
  return getRuleContext<sqlParser::PrecisionContext>(0);
}

tree::TerminalNode* sqlParser::DayTimeIntervalTypeContext::TO() {
  return getToken(sqlParser::TO, 0);
}

tree::TerminalNode* sqlParser::DayTimeIntervalTypeContext::HOUR() {
  return getToken(sqlParser::HOUR, 0);
}

tree::TerminalNode* sqlParser::DayTimeIntervalTypeContext::MINUTE() {
  return getToken(sqlParser::MINUTE, 0);
}

tree::TerminalNode* sqlParser::DayTimeIntervalTypeContext::SECOND() {
  return getToken(sqlParser::SECOND, 0);
}

sqlParser::ScaleContext* sqlParser::DayTimeIntervalTypeContext::scale() {
  return getRuleContext<sqlParser::ScaleContext>(0);
}


size_t sqlParser::DayTimeIntervalTypeContext::getRuleIndex() const {
  return sqlParser::RuleDayTimeIntervalType;
}


antlrcpp::Any sqlParser::DayTimeIntervalTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDayTimeIntervalType(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DayTimeIntervalTypeContext* sqlParser::dayTimeIntervalType() {
  DayTimeIntervalTypeContext *_localctx = _tracker.createInstance<DayTimeIntervalTypeContext>(_ctx, getState());
  enterRule(_localctx, 172, sqlParser::RuleDayTimeIntervalType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1395);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 161, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1311);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1316);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1312);
        match(sqlParser::T__6);
        setState(1313);
        precision();
        setState(1314);
        match(sqlParser::T__7);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1318);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1323);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1319);
        match(sqlParser::T__6);
        setState(1320);
        precision();
        setState(1321);
        match(sqlParser::T__7);
      }
      setState(1325);
      match(sqlParser::TO);
      setState(1335);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::HOUR: {
          setState(1326);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->hour = match(sqlParser::HOUR);
          break;
        }

        case sqlParser::MINUTE: {
          setState(1327);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1328);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1333);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__6) {
            setState(1329);
            match(sqlParser::T__6);
            setState(1330);
            scale();
            setState(1331);
            match(sqlParser::T__7);
          }
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1337);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1342);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1338);
        match(sqlParser::T__6);
        setState(1339);
        precision();
        setState(1340);
        match(sqlParser::T__7);
      }
      setState(1344);
      match(sqlParser::TO);
      setState(1353);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::MINUTE: {
          setState(1345);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1346);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1351);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__6) {
            setState(1347);
            match(sqlParser::T__6);
            setState(1348);
            scale();
            setState(1349);
            match(sqlParser::T__7);
          }
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(1355);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1360);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1356);
        match(sqlParser::T__6);
        setState(1357);
        precision();
        setState(1358);
        match(sqlParser::T__7);
      }
      setState(1362);
      match(sqlParser::TO);

      setState(1363);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1368);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1364);
        match(sqlParser::T__6);
        setState(1365);
        scale();
        setState(1366);
        match(sqlParser::T__7);
      }
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1370);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1375);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1371);
        match(sqlParser::T__6);
        setState(1372);
        precision();
        setState(1373);
        match(sqlParser::T__7);
      }
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1377);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1382);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1378);
        match(sqlParser::T__6);
        setState(1379);
        precision();
        setState(1380);
        match(sqlParser::T__7);
      }
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(1384);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1393);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1385);
        match(sqlParser::T__6);
        setState(1386);
        precision();
        setState(1389);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1387);
          match(sqlParser::T__5);
          setState(1388);
          scale();
        }
        setState(1391);
        match(sqlParser::T__7);
      }
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FixedLenCharContext ------------------------------------------------------------------

sqlParser::FixedLenCharContext::FixedLenCharContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::SizeContext* sqlParser::FixedLenCharContext::size() {
  return getRuleContext<sqlParser::SizeContext>(0);
}

std::vector<tree::TerminalNode *> sqlParser::FixedLenCharContext::CHAR() {
  return getTokens(sqlParser::CHAR);
}

tree::TerminalNode* sqlParser::FixedLenCharContext::CHAR(size_t i) {
  return getToken(sqlParser::CHAR, i);
}

tree::TerminalNode* sqlParser::FixedLenCharContext::CHARACTER() {
  return getToken(sqlParser::CHARACTER, 0);
}

tree::TerminalNode* sqlParser::FixedLenCharContext::COLLATE() {
  return getToken(sqlParser::COLLATE, 0);
}

tree::TerminalNode* sqlParser::FixedLenCharContext::BYTE() {
  return getToken(sqlParser::BYTE, 0);
}

tree::TerminalNode* sqlParser::FixedLenCharContext::BYTES() {
  return getToken(sqlParser::BYTES, 0);
}

tree::TerminalNode* sqlParser::FixedLenCharContext::CHARS() {
  return getToken(sqlParser::CHARS, 0);
}

sqlParser::CollationNameContext* sqlParser::FixedLenCharContext::collationName() {
  return getRuleContext<sqlParser::CollationNameContext>(0);
}


size_t sqlParser::FixedLenCharContext::getRuleIndex() const {
  return sqlParser::RuleFixedLenChar;
}


antlrcpp::Any sqlParser::FixedLenCharContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitFixedLenChar(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::FixedLenCharContext* sqlParser::fixedLenChar() {
  FixedLenCharContext *_localctx = _tracker.createInstance<FixedLenCharContext>(_ctx, getState());
  enterRule(_localctx, 174, sqlParser::RuleFixedLenChar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1397);
    _la = _input->LA(1);
    if (!(_la == sqlParser::CHAR

    || _la == sqlParser::CHARACTER)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(1398);
    match(sqlParser::T__6);
    setState(1399);
    size();
    setState(1401);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::BYTE)
      | (1ULL << sqlParser::BYTES)
      | (1ULL << sqlParser::CHAR)
      | (1ULL << sqlParser::CHARS))) != 0)) {
      setState(1400);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::BYTE)
        | (1ULL << sqlParser::BYTES)
        | (1ULL << sqlParser::CHAR)
        | (1ULL << sqlParser::CHARS))) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(1403);
    match(sqlParser::T__7);
    setState(1406);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::COLLATE) {
      setState(1404);
      match(sqlParser::COLLATE);
      setState(1405);
      dynamic_cast<FixedLenCharContext *>(_localctx)->collate = collationName();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VaryingLenCharContext ------------------------------------------------------------------

sqlParser::VaryingLenCharContext::VaryingLenCharContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::VaryingLenCharContext::TEXT() {
  return getToken(sqlParser::TEXT, 0);
}

tree::TerminalNode* sqlParser::VaryingLenCharContext::COLLATE() {
  return getToken(sqlParser::COLLATE, 0);
}

tree::TerminalNode* sqlParser::VaryingLenCharContext::VARCHAR() {
  return getToken(sqlParser::VARCHAR, 0);
}

sqlParser::CollationNameContext* sqlParser::VaryingLenCharContext::collationName() {
  return getRuleContext<sqlParser::CollationNameContext>(0);
}

sqlParser::SizeContext* sqlParser::VaryingLenCharContext::size() {
  return getRuleContext<sqlParser::SizeContext>(0);
}

std::vector<tree::TerminalNode *> sqlParser::VaryingLenCharContext::CHAR() {
  return getTokens(sqlParser::CHAR);
}

tree::TerminalNode* sqlParser::VaryingLenCharContext::CHAR(size_t i) {
  return getToken(sqlParser::CHAR, i);
}

tree::TerminalNode* sqlParser::VaryingLenCharContext::CHARACTER() {
  return getToken(sqlParser::CHARACTER, 0);
}

tree::TerminalNode* sqlParser::VaryingLenCharContext::VARYING() {
  return getToken(sqlParser::VARYING, 0);
}

tree::TerminalNode* sqlParser::VaryingLenCharContext::BYTE() {
  return getToken(sqlParser::BYTE, 0);
}

tree::TerminalNode* sqlParser::VaryingLenCharContext::BYTES() {
  return getToken(sqlParser::BYTES, 0);
}

tree::TerminalNode* sqlParser::VaryingLenCharContext::CHARS() {
  return getToken(sqlParser::CHARS, 0);
}


size_t sqlParser::VaryingLenCharContext::getRuleIndex() const {
  return sqlParser::RuleVaryingLenChar;
}


antlrcpp::Any sqlParser::VaryingLenCharContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitVaryingLenChar(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::VaryingLenCharContext* sqlParser::varyingLenChar() {
  VaryingLenCharContext *_localctx = _tracker.createInstance<VaryingLenCharContext>(_ctx, getState());
  enterRule(_localctx, 176, sqlParser::RuleVaryingLenChar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1425);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::CHAR:
      case sqlParser::CHARACTER:
      case sqlParser::VARCHAR: {
        setState(1413);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::CHAR:
          case sqlParser::CHARACTER: {
            setState(1408);
            _la = _input->LA(1);
            if (!(_la == sqlParser::CHAR

            || _la == sqlParser::CHARACTER)) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
            setState(1410);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == sqlParser::VARYING) {
              setState(1409);
              match(sqlParser::VARYING);
            }
            break;
          }

          case sqlParser::VARCHAR: {
            setState(1412);
            match(sqlParser::VARCHAR);
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(1422);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__6) {
          setState(1415);
          match(sqlParser::T__6);
          setState(1416);
          size();
          setState(1418);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::BYTE)
            | (1ULL << sqlParser::BYTES)
            | (1ULL << sqlParser::CHAR)
            | (1ULL << sqlParser::CHARS))) != 0)) {
            setState(1417);
            _la = _input->LA(1);
            if (!((((_la & ~ 0x3fULL) == 0) &&
              ((1ULL << _la) & ((1ULL << sqlParser::BYTE)
              | (1ULL << sqlParser::BYTES)
              | (1ULL << sqlParser::CHAR)
              | (1ULL << sqlParser::CHARS))) != 0))) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
          }
          setState(1420);
          match(sqlParser::T__7);
        }
        break;
      }

      case sqlParser::TEXT: {
        setState(1424);
        match(sqlParser::TEXT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(1429);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::COLLATE) {
      setState(1427);
      match(sqlParser::COLLATE);
      setState(1428);
      dynamic_cast<VaryingLenCharContext *>(_localctx)->collate = collationName();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AsSubqueryContext ------------------------------------------------------------------

sqlParser::AsSubqueryContext::AsSubqueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::SelectOrValuesContext* sqlParser::AsSubqueryContext::selectOrValues() {
  return getRuleContext<sqlParser::SelectOrValuesContext>(0);
}


size_t sqlParser::AsSubqueryContext::getRuleIndex() const {
  return sqlParser::RuleAsSubquery;
}


antlrcpp::Any sqlParser::AsSubqueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitAsSubquery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::AsSubqueryContext* sqlParser::asSubquery() {
  AsSubqueryContext *_localctx = _tracker.createInstance<AsSubqueryContext>(_ctx, getState());
  enterRule(_localctx, 178, sqlParser::RuleAsSubquery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1431);
    selectOrValues();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectOrValuesContext ------------------------------------------------------------------

sqlParser::SelectOrValuesContext::SelectOrValuesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::SelectOrValuesContext* sqlParser::SelectOrValuesContext::selectOrValues() {
  return getRuleContext<sqlParser::SelectOrValuesContext>(0);
}

std::vector<sqlParser::QueryHintContext *> sqlParser::SelectOrValuesContext::queryHint() {
  return getRuleContexts<sqlParser::QueryHintContext>();
}

sqlParser::QueryHintContext* sqlParser::SelectOrValuesContext::queryHint(size_t i) {
  return getRuleContext<sqlParser::QueryHintContext>(i);
}

sqlParser::SelectQueryContext* sqlParser::SelectOrValuesContext::selectQuery() {
  return getRuleContext<sqlParser::SelectQueryContext>(0);
}

sqlParser::ValuesExprContext* sqlParser::SelectOrValuesContext::valuesExpr() {
  return getRuleContext<sqlParser::ValuesExprContext>(0);
}

sqlParser::SetQueryContext* sqlParser::SelectOrValuesContext::setQuery() {
  return getRuleContext<sqlParser::SetQueryContext>(0);
}


size_t sqlParser::SelectOrValuesContext::getRuleIndex() const {
  return sqlParser::RuleSelectOrValues;
}


antlrcpp::Any sqlParser::SelectOrValuesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSelectOrValues(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SelectOrValuesContext* sqlParser::selectOrValues() {
  SelectOrValuesContext *_localctx = _tracker.createInstance<SelectOrValuesContext>(_ctx, getState());
  enterRule(_localctx, 180, sqlParser::RuleSelectOrValues);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    setState(1454);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 173, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1433);
      match(sqlParser::T__6);
      setState(1437);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 170, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(1434);
          queryHint(); 
        }
        setState(1439);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 170, _ctx);
      }
      setState(1440);
      selectOrValues();
      setState(1441);
      match(sqlParser::T__7);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1446);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::HINT_START) {
        setState(1443);
        queryHint();
        setState(1448);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(1452);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 172, _ctx)) {
      case 1: {
        setState(1449);
        selectQuery();
        break;
      }

      case 2: {
        setState(1450);
        valuesExpr();
        break;
      }

      case 3: {
        setState(1451);
        setQuery();
        break;
      }

      }
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectListContext ------------------------------------------------------------------

sqlParser::SelectListContext::SelectListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<sqlParser::SelectListItemContext *> sqlParser::SelectListContext::selectListItem() {
  return getRuleContexts<sqlParser::SelectListItemContext>();
}

sqlParser::SelectListItemContext* sqlParser::SelectListContext::selectListItem(size_t i) {
  return getRuleContext<sqlParser::SelectListItemContext>(i);
}


size_t sqlParser::SelectListContext::getRuleIndex() const {
  return sqlParser::RuleSelectList;
}


antlrcpp::Any sqlParser::SelectListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSelectList(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SelectListContext* sqlParser::selectList() {
  SelectListContext *_localctx = _tracker.createInstance<SelectListContext>(_ctx, getState());
  enterRule(_localctx, 182, sqlParser::RuleSelectList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1456);
    selectListItem();
    setState(1461);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 174, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1457);
        match(sqlParser::T__5);
        setState(1458);
        selectListItem(); 
      }
      setState(1463);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 174, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectListItemContext ------------------------------------------------------------------

sqlParser::SelectListItemContext::SelectListItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::ExprContext* sqlParser::SelectListItemContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}

sqlParser::IdentifierContext* sqlParser::SelectListItemContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}

tree::TerminalNode* sqlParser::SelectListItemContext::AS() {
  return getToken(sqlParser::AS, 0);
}

sqlParser::ColumnWildcardContext* sqlParser::SelectListItemContext::columnWildcard() {
  return getRuleContext<sqlParser::ColumnWildcardContext>(0);
}


size_t sqlParser::SelectListItemContext::getRuleIndex() const {
  return sqlParser::RuleSelectListItem;
}


antlrcpp::Any sqlParser::SelectListItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSelectListItem(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SelectListItemContext* sqlParser::selectListItem() {
  SelectListItemContext *_localctx = _tracker.createInstance<SelectListItemContext>(_ctx, getState());
  enterRule(_localctx, 184, sqlParser::RuleSelectListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1472);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 177, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1464);
      expr(0);
      setState(1469);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 176, _ctx)) {
      case 1: {
        setState(1466);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(1465);
          match(sqlParser::AS);
        }
        setState(1468);
        dynamic_cast<SelectListItemContext *>(_localctx)->as = identifier();
        break;
      }

      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1471);
      dynamic_cast<SelectListItemContext *>(_localctx)->wild = columnWildcard();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupByListContext ------------------------------------------------------------------

sqlParser::GroupByListContext::GroupByListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<sqlParser::GroupByListItemContext *> sqlParser::GroupByListContext::groupByListItem() {
  return getRuleContexts<sqlParser::GroupByListItemContext>();
}

sqlParser::GroupByListItemContext* sqlParser::GroupByListContext::groupByListItem(size_t i) {
  return getRuleContext<sqlParser::GroupByListItemContext>(i);
}


size_t sqlParser::GroupByListContext::getRuleIndex() const {
  return sqlParser::RuleGroupByList;
}


antlrcpp::Any sqlParser::GroupByListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitGroupByList(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::GroupByListContext* sqlParser::groupByList() {
  GroupByListContext *_localctx = _tracker.createInstance<GroupByListContext>(_ctx, getState());
  enterRule(_localctx, 186, sqlParser::RuleGroupByList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1474);
    groupByListItem();
    setState(1479);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 178, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1475);
        match(sqlParser::T__5);
        setState(1476);
        groupByListItem(); 
      }
      setState(1481);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 178, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupByListItemContext ------------------------------------------------------------------

sqlParser::GroupByListItemContext::GroupByListItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::ExprContext* sqlParser::GroupByListItemContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}


size_t sqlParser::GroupByListItemContext::getRuleIndex() const {
  return sqlParser::RuleGroupByListItem;
}


antlrcpp::Any sqlParser::GroupByListItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitGroupByListItem(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::GroupByListItemContext* sqlParser::groupByListItem() {
  GroupByListItemContext *_localctx = _tracker.createInstance<GroupByListItemContext>(_ctx, getState());
  enterRule(_localctx, 188, sqlParser::RuleGroupByListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1482);
    expr(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrderByListContext ------------------------------------------------------------------

sqlParser::OrderByListContext::OrderByListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<sqlParser::OrderByListItemContext *> sqlParser::OrderByListContext::orderByListItem() {
  return getRuleContexts<sqlParser::OrderByListItemContext>();
}

sqlParser::OrderByListItemContext* sqlParser::OrderByListContext::orderByListItem(size_t i) {
  return getRuleContext<sqlParser::OrderByListItemContext>(i);
}


size_t sqlParser::OrderByListContext::getRuleIndex() const {
  return sqlParser::RuleOrderByList;
}


antlrcpp::Any sqlParser::OrderByListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitOrderByList(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::OrderByListContext* sqlParser::orderByList() {
  OrderByListContext *_localctx = _tracker.createInstance<OrderByListContext>(_ctx, getState());
  enterRule(_localctx, 190, sqlParser::RuleOrderByList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1484);
    orderByListItem();
    setState(1489);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1485);
      match(sqlParser::T__5);
      setState(1486);
      orderByListItem();
      setState(1491);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrderByListItemContext ------------------------------------------------------------------

sqlParser::OrderByListItemContext::OrderByListItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::ExprContext* sqlParser::OrderByListItemContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}

tree::TerminalNode* sqlParser::OrderByListItemContext::ASC() {
  return getToken(sqlParser::ASC, 0);
}

tree::TerminalNode* sqlParser::OrderByListItemContext::DESC() {
  return getToken(sqlParser::DESC, 0);
}


size_t sqlParser::OrderByListItemContext::getRuleIndex() const {
  return sqlParser::RuleOrderByListItem;
}


antlrcpp::Any sqlParser::OrderByListItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitOrderByListItem(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::OrderByListItemContext* sqlParser::orderByListItem() {
  OrderByListItemContext *_localctx = _tracker.createInstance<OrderByListItemContext>(_ctx, getState());
  enterRule(_localctx, 192, sqlParser::RuleOrderByListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1492);
    expr(0);
    setState(1494);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ASC

    || _la == sqlParser::DESC) {
      setState(1493);
      dynamic_cast<OrderByListItemContext *>(_localctx)->desc = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == sqlParser::ASC

      || _la == sqlParser::DESC)) {
        dynamic_cast<OrderByListItemContext *>(_localctx)->desc = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- JoinUsingListContext ------------------------------------------------------------------

sqlParser::JoinUsingListContext::JoinUsingListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<sqlParser::JoinUsingListItemContext *> sqlParser::JoinUsingListContext::joinUsingListItem() {
  return getRuleContexts<sqlParser::JoinUsingListItemContext>();
}

sqlParser::JoinUsingListItemContext* sqlParser::JoinUsingListContext::joinUsingListItem(size_t i) {
  return getRuleContext<sqlParser::JoinUsingListItemContext>(i);
}


size_t sqlParser::JoinUsingListContext::getRuleIndex() const {
  return sqlParser::RuleJoinUsingList;
}


antlrcpp::Any sqlParser::JoinUsingListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitJoinUsingList(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::JoinUsingListContext* sqlParser::joinUsingList() {
  JoinUsingListContext *_localctx = _tracker.createInstance<JoinUsingListContext>(_ctx, getState());
  enterRule(_localctx, 194, sqlParser::RuleJoinUsingList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1496);
    joinUsingListItem();
    setState(1501);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1497);
      match(sqlParser::T__5);
      setState(1498);
      joinUsingListItem();
      setState(1503);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- JoinUsingListItemContext ------------------------------------------------------------------

sqlParser::JoinUsingListItemContext::JoinUsingListItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::JoinUsingListItemContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::JoinUsingListItemContext::getRuleIndex() const {
  return sqlParser::RuleJoinUsingListItem;
}


antlrcpp::Any sqlParser::JoinUsingListItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitJoinUsingListItem(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::JoinUsingListItemContext* sqlParser::joinUsingListItem() {
  JoinUsingListItemContext *_localctx = _tracker.createInstance<JoinUsingListItemContext>(_ctx, getState());
  enterRule(_localctx, 196, sqlParser::RuleJoinUsingListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1504);
    dynamic_cast<JoinUsingListItemContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CorrListContext ------------------------------------------------------------------

sqlParser::CorrListContext::CorrListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<sqlParser::CorrListItemContext *> sqlParser::CorrListContext::corrListItem() {
  return getRuleContexts<sqlParser::CorrListItemContext>();
}

sqlParser::CorrListItemContext* sqlParser::CorrListContext::corrListItem(size_t i) {
  return getRuleContext<sqlParser::CorrListItemContext>(i);
}


size_t sqlParser::CorrListContext::getRuleIndex() const {
  return sqlParser::RuleCorrList;
}


antlrcpp::Any sqlParser::CorrListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCorrList(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CorrListContext* sqlParser::corrList() {
  CorrListContext *_localctx = _tracker.createInstance<CorrListContext>(_ctx, getState());
  enterRule(_localctx, 198, sqlParser::RuleCorrList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1506);
    corrListItem();
    setState(1511);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1507);
      match(sqlParser::T__5);
      setState(1508);
      corrListItem();
      setState(1513);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CorrListItemContext ------------------------------------------------------------------

sqlParser::CorrListItemContext::CorrListItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::CorrListItemContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::CorrListItemContext::getRuleIndex() const {
  return sqlParser::RuleCorrListItem;
}


antlrcpp::Any sqlParser::CorrListItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCorrListItem(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CorrListItemContext* sqlParser::corrListItem() {
  CorrListItemContext *_localctx = _tracker.createInstance<CorrListItemContext>(_ctx, getState());
  enterRule(_localctx, 200, sqlParser::RuleCorrListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1514);
    dynamic_cast<CorrListItemContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ColAliasesContext ------------------------------------------------------------------

sqlParser::ColAliasesContext::ColAliasesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::ColAliasListContext* sqlParser::ColAliasesContext::colAliasList() {
  return getRuleContext<sqlParser::ColAliasListContext>(0);
}


size_t sqlParser::ColAliasesContext::getRuleIndex() const {
  return sqlParser::RuleColAliases;
}


antlrcpp::Any sqlParser::ColAliasesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitColAliases(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ColAliasesContext* sqlParser::colAliases() {
  ColAliasesContext *_localctx = _tracker.createInstance<ColAliasesContext>(_ctx, getState());
  enterRule(_localctx, 202, sqlParser::RuleColAliases);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1516);
    match(sqlParser::T__6);
    setState(1517);
    colAliasList();
    setState(1518);
    match(sqlParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ColAliasListContext ------------------------------------------------------------------

sqlParser::ColAliasListContext::ColAliasListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<sqlParser::ColAliasListItemContext *> sqlParser::ColAliasListContext::colAliasListItem() {
  return getRuleContexts<sqlParser::ColAliasListItemContext>();
}

sqlParser::ColAliasListItemContext* sqlParser::ColAliasListContext::colAliasListItem(size_t i) {
  return getRuleContext<sqlParser::ColAliasListItemContext>(i);
}


size_t sqlParser::ColAliasListContext::getRuleIndex() const {
  return sqlParser::RuleColAliasList;
}


antlrcpp::Any sqlParser::ColAliasListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitColAliasList(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ColAliasListContext* sqlParser::colAliasList() {
  ColAliasListContext *_localctx = _tracker.createInstance<ColAliasListContext>(_ctx, getState());
  enterRule(_localctx, 204, sqlParser::RuleColAliasList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1520);
    colAliasListItem();
    setState(1525);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1521);
      match(sqlParser::T__5);
      setState(1522);
      colAliasListItem();
      setState(1527);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ColAliasListItemContext ------------------------------------------------------------------

sqlParser::ColAliasListItemContext::ColAliasListItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::ColAliasListItemContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::ColAliasListItemContext::getRuleIndex() const {
  return sqlParser::RuleColAliasListItem;
}


antlrcpp::Any sqlParser::ColAliasListItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitColAliasListItem(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ColAliasListItemContext* sqlParser::colAliasListItem() {
  ColAliasListItemContext *_localctx = _tracker.createInstance<ColAliasListItemContext>(_ctx, getState());
  enterRule(_localctx, 206, sqlParser::RuleColAliasListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1528);
    dynamic_cast<ColAliasListItemContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyListContext ------------------------------------------------------------------

sqlParser::PropertyListContext::PropertyListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<sqlParser::PropertyContext *> sqlParser::PropertyListContext::property() {
  return getRuleContexts<sqlParser::PropertyContext>();
}

sqlParser::PropertyContext* sqlParser::PropertyListContext::property(size_t i) {
  return getRuleContext<sqlParser::PropertyContext>(i);
}


size_t sqlParser::PropertyListContext::getRuleIndex() const {
  return sqlParser::RulePropertyList;
}


antlrcpp::Any sqlParser::PropertyListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitPropertyList(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::PropertyListContext* sqlParser::propertyList() {
  PropertyListContext *_localctx = _tracker.createInstance<PropertyListContext>(_ctx, getState());
  enterRule(_localctx, 208, sqlParser::RulePropertyList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1530);
    property();
    setState(1535);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1531);
      match(sqlParser::T__5);
      setState(1532);
      property();
      setState(1537);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyContext ------------------------------------------------------------------

sqlParser::PropertyContext::PropertyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::PropertyNameContext* sqlParser::PropertyContext::propertyName() {
  return getRuleContext<sqlParser::PropertyNameContext>(0);
}

sqlParser::PropertyValueContext* sqlParser::PropertyContext::propertyValue() {
  return getRuleContext<sqlParser::PropertyValueContext>(0);
}


size_t sqlParser::PropertyContext::getRuleIndex() const {
  return sqlParser::RuleProperty;
}


antlrcpp::Any sqlParser::PropertyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitProperty(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::PropertyContext* sqlParser::property() {
  PropertyContext *_localctx = _tracker.createInstance<PropertyContext>(_ctx, getState());
  enterRule(_localctx, 210, sqlParser::RuleProperty);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1538);
    dynamic_cast<PropertyContext *>(_localctx)->name = propertyName();
    setState(1541);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__3) {
      setState(1539);
      match(sqlParser::T__3);
      setState(1540);
      dynamic_cast<PropertyContext *>(_localctx)->value = propertyValue();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueListContext ------------------------------------------------------------------

sqlParser::ValueListContext::ValueListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<sqlParser::ExprContext *> sqlParser::ValueListContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::ValueListContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}


size_t sqlParser::ValueListContext::getRuleIndex() const {
  return sqlParser::RuleValueList;
}


antlrcpp::Any sqlParser::ValueListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitValueList(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ValueListContext* sqlParser::valueList() {
  ValueListContext *_localctx = _tracker.createInstance<ValueListContext>(_ctx, getState());
  enterRule(_localctx, 212, sqlParser::RuleValueList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1543);
    expr(0);
    setState(1548);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1544);
      match(sqlParser::T__5);
      setState(1545);
      expr(0);
      setState(1550);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PredicateContext ------------------------------------------------------------------

sqlParser::PredicateContext::PredicateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::ExprContext* sqlParser::PredicateContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}


size_t sqlParser::PredicateContext::getRuleIndex() const {
  return sqlParser::RulePredicate;
}


antlrcpp::Any sqlParser::PredicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitPredicate(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::PredicateContext* sqlParser::predicate() {
  PredicateContext *_localctx = _tracker.createInstance<PredicateContext>(_ctx, getState());
  enterRule(_localctx, 214, sqlParser::RulePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1551);
    expr(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- JoinPredicateContext ------------------------------------------------------------------

sqlParser::JoinPredicateContext::JoinPredicateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::ExprContext* sqlParser::JoinPredicateContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}


size_t sqlParser::JoinPredicateContext::getRuleIndex() const {
  return sqlParser::RuleJoinPredicate;
}


antlrcpp::Any sqlParser::JoinPredicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitJoinPredicate(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::JoinPredicateContext* sqlParser::joinPredicate() {
  JoinPredicateContext *_localctx = _tracker.createInstance<JoinPredicateContext>(_ctx, getState());
  enterRule(_localctx, 216, sqlParser::RuleJoinPredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1553);
    expr(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CasePredicateContext ------------------------------------------------------------------

sqlParser::CasePredicateContext::CasePredicateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::ExprContext* sqlParser::CasePredicateContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}


size_t sqlParser::CasePredicateContext::getRuleIndex() const {
  return sqlParser::RuleCasePredicate;
}


antlrcpp::Any sqlParser::CasePredicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCasePredicate(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CasePredicateContext* sqlParser::casePredicate() {
  CasePredicateContext *_localctx = _tracker.createInstance<CasePredicateContext>(_ctx, getState());
  enterRule(_localctx, 218, sqlParser::RuleCasePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1555);
    expr(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

sqlParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t sqlParser::ExprContext::getRuleIndex() const {
  return sqlParser::RuleExpr;
}

void sqlParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- CountAllContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::CountAllContext::COUNT() {
  return getToken(sqlParser::COUNT, 0);
}

tree::TerminalNode* sqlParser::CountAllContext::ALL() {
  return getToken(sqlParser::ALL, 0);
}

sqlParser::CountAllContext::CountAllContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::CountAllContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCountAll(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExtractFuncContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::ExtractFuncContext::EXTRACT() {
  return getToken(sqlParser::EXTRACT, 0);
}

tree::TerminalNode* sqlParser::ExtractFuncContext::FROM() {
  return getToken(sqlParser::FROM, 0);
}

sqlParser::ExprContext* sqlParser::ExtractFuncContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}

tree::TerminalNode* sqlParser::ExtractFuncContext::SECOND() {
  return getToken(sqlParser::SECOND, 0);
}

tree::TerminalNode* sqlParser::ExtractFuncContext::MINUTE() {
  return getToken(sqlParser::MINUTE, 0);
}

tree::TerminalNode* sqlParser::ExtractFuncContext::HOUR() {
  return getToken(sqlParser::HOUR, 0);
}

tree::TerminalNode* sqlParser::ExtractFuncContext::DAY() {
  return getToken(sqlParser::DAY, 0);
}

tree::TerminalNode* sqlParser::ExtractFuncContext::MONTH() {
  return getToken(sqlParser::MONTH, 0);
}

tree::TerminalNode* sqlParser::ExtractFuncContext::YEAR() {
  return getToken(sqlParser::YEAR, 0);
}

sqlParser::ExtractFuncContext::ExtractFuncContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::ExtractFuncContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitExtractFunc(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ScalarFuncContext ------------------------------------------------------------------

sqlParser::FunctionNameContext* sqlParser::ScalarFuncContext::functionName() {
  return getRuleContext<sqlParser::FunctionNameContext>(0);
}

std::vector<sqlParser::ExprContext *> sqlParser::ScalarFuncContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::ScalarFuncContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::ScalarFuncContext::ScalarFuncContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::ScalarFuncContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitScalarFunc(this);
  else
    return visitor->visitChildren(this);
}
//----------------- UnaryOpContext ------------------------------------------------------------------

sqlParser::ExprContext* sqlParser::UnaryOpContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}

sqlParser::UnaryOpContext::UnaryOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::UnaryOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitUnaryOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CoalesceOpContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::CoalesceOpContext::COALESCE() {
  return getToken(sqlParser::COALESCE, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::CoalesceOpContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::CoalesceOpContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::CoalesceOpContext::CoalesceOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::CoalesceOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCoalesceOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- VarRefContext ------------------------------------------------------------------

sqlParser::VariableNameContext* sqlParser::VarRefContext::variableName() {
  return getRuleContext<sqlParser::VariableNameContext>(0);
}

sqlParser::VarRefContext::VarRefContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::VarRefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitVarRef(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PositionFuncContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::PositionFuncContext::POSITION() {
  return getToken(sqlParser::POSITION, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::PositionFuncContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::PositionFuncContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::PositionFuncContext::IN() {
  return getToken(sqlParser::IN, 0);
}

sqlParser::PositionFuncContext::PositionFuncContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::PositionFuncContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitPositionFunc(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ColRefContext ------------------------------------------------------------------

sqlParser::ColumnRefContext* sqlParser::ColRefContext::columnRef() {
  return getRuleContext<sqlParser::ColumnRefContext>(0);
}

sqlParser::ColRefContext::ColRefContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::ColRefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitColRef(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CaseOpContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::CaseOpContext::CASE() {
  return getToken(sqlParser::CASE, 0);
}

tree::TerminalNode* sqlParser::CaseOpContext::END() {
  return getToken(sqlParser::END, 0);
}

tree::TerminalNode* sqlParser::CaseOpContext::ELSE() {
  return getToken(sqlParser::ELSE, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::CaseOpContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::CaseOpContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> sqlParser::CaseOpContext::WHEN() {
  return getTokens(sqlParser::WHEN);
}

tree::TerminalNode* sqlParser::CaseOpContext::WHEN(size_t i) {
  return getToken(sqlParser::WHEN, i);
}

std::vector<sqlParser::CasePredicateContext *> sqlParser::CaseOpContext::casePredicate() {
  return getRuleContexts<sqlParser::CasePredicateContext>();
}

sqlParser::CasePredicateContext* sqlParser::CaseOpContext::casePredicate(size_t i) {
  return getRuleContext<sqlParser::CasePredicateContext>(i);
}

std::vector<tree::TerminalNode *> sqlParser::CaseOpContext::THEN() {
  return getTokens(sqlParser::THEN);
}

tree::TerminalNode* sqlParser::CaseOpContext::THEN(size_t i) {
  return getToken(sqlParser::THEN, i);
}

sqlParser::CaseOpContext::CaseOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::CaseOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCaseOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LiteralValContext ------------------------------------------------------------------

sqlParser::LiteralContext* sqlParser::LiteralValContext::literal() {
  return getRuleContext<sqlParser::LiteralContext>(0);
}

sqlParser::LiteralValContext::LiteralValContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::LiteralValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLiteralVal(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OverlayFuncContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::OverlayFuncContext::OVERLAY() {
  return getToken(sqlParser::OVERLAY, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::OverlayFuncContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::OverlayFuncContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::OverlayFuncContext::PLACING() {
  return getToken(sqlParser::PLACING, 0);
}

tree::TerminalNode* sqlParser::OverlayFuncContext::FROM() {
  return getToken(sqlParser::FROM, 0);
}

tree::TerminalNode* sqlParser::OverlayFuncContext::FOR() {
  return getToken(sqlParser::FOR, 0);
}

sqlParser::OverlayFuncContext::OverlayFuncContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::OverlayFuncContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitOverlayFunc(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfNullOpContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::IfNullOpContext::IFNULL() {
  return getToken(sqlParser::IFNULL, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::IfNullOpContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::IfNullOpContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::IfNullOpContext::IfNullOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::IfNullOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitIfNullOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- SubqueryExistsOpContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::SubqueryExistsOpContext::EXISTS() {
  return getToken(sqlParser::EXISTS, 0);
}

sqlParser::QueryContext* sqlParser::SubqueryExistsOpContext::query() {
  return getRuleContext<sqlParser::QueryContext>(0);
}

tree::TerminalNode* sqlParser::SubqueryExistsOpContext::NOT() {
  return getToken(sqlParser::NOT, 0);
}

sqlParser::SubqueryExistsOpContext::SubqueryExistsOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::SubqueryExistsOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSubqueryExistsOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IsNullOpContext ------------------------------------------------------------------

sqlParser::ExprContext* sqlParser::IsNullOpContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}

tree::TerminalNode* sqlParser::IsNullOpContext::IS() {
  return getToken(sqlParser::IS, 0);
}

tree::TerminalNode* sqlParser::IsNullOpContext::SQL_NULL() {
  return getToken(sqlParser::SQL_NULL, 0);
}

tree::TerminalNode* sqlParser::IsNullOpContext::NOT() {
  return getToken(sqlParser::NOT, 0);
}

sqlParser::IsNullOpContext::IsNullOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::IsNullOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitIsNullOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InOpContext ------------------------------------------------------------------

std::vector<sqlParser::ExprContext *> sqlParser::InOpContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::InOpContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::InOpContext::IN() {
  return getToken(sqlParser::IN, 0);
}

tree::TerminalNode* sqlParser::InOpContext::NOT() {
  return getToken(sqlParser::NOT, 0);
}

sqlParser::InOpContext::InOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::InOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitInOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TrimFuncContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::TrimFuncContext::TRIM() {
  return getToken(sqlParser::TRIM, 0);
}

tree::TerminalNode* sqlParser::TrimFuncContext::FROM() {
  return getToken(sqlParser::FROM, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::TrimFuncContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::TrimFuncContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::TrimFuncContext::LEADING() {
  return getToken(sqlParser::LEADING, 0);
}

tree::TerminalNode* sqlParser::TrimFuncContext::TRAILING() {
  return getToken(sqlParser::TRAILING, 0);
}

tree::TerminalNode* sqlParser::TrimFuncContext::BOTH() {
  return getToken(sqlParser::BOTH, 0);
}

sqlParser::TrimFuncContext::TrimFuncContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::TrimFuncContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTrimFunc(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LikeOpContext ------------------------------------------------------------------

std::vector<sqlParser::ExprContext *> sqlParser::LikeOpContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::LikeOpContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::LikeOpContext::LIKE() {
  return getToken(sqlParser::LIKE, 0);
}

tree::TerminalNode* sqlParser::LikeOpContext::NOT() {
  return getToken(sqlParser::NOT, 0);
}

sqlParser::LikeOpContext::LikeOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::LikeOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLikeOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TypeOfOpContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::TypeOfOpContext::TYPEOF() {
  return getToken(sqlParser::TYPEOF, 0);
}

sqlParser::ExprContext* sqlParser::TypeOfOpContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}

sqlParser::TypeOfOpContext::TypeOfOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::TypeOfOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTypeOfOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NullIfOpContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::NullIfOpContext::NULLIF() {
  return getToken(sqlParser::NULLIF, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::NullIfOpContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::NullIfOpContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::NullIfOpContext::NullIfOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::NullIfOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitNullIfOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InlineCastContext ------------------------------------------------------------------

sqlParser::DataTypeContext* sqlParser::InlineCastContext::dataType() {
  return getRuleContext<sqlParser::DataTypeContext>(0);
}

sqlParser::ExprContext* sqlParser::InlineCastContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}

sqlParser::InlineCastContext::InlineCastContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::InlineCastContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitInlineCast(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BinaryOpContext ------------------------------------------------------------------

std::vector<sqlParser::ExprContext *> sqlParser::BinaryOpContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::BinaryOpContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::BinaryOpContext::BinaryOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::BinaryOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitBinaryOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BetweenOpContext ------------------------------------------------------------------

std::vector<sqlParser::ExprContext *> sqlParser::BetweenOpContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::BetweenOpContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::BetweenOpContext::BETWEEN() {
  return getToken(sqlParser::BETWEEN, 0);
}

tree::TerminalNode* sqlParser::BetweenOpContext::AND() {
  return getToken(sqlParser::AND, 0);
}

tree::TerminalNode* sqlParser::BetweenOpContext::NOT() {
  return getToken(sqlParser::NOT, 0);
}

sqlParser::BetweenOpContext::BetweenOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::BetweenOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitBetweenOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AggFuncContext ------------------------------------------------------------------

sqlParser::ExprContext* sqlParser::AggFuncContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}

tree::TerminalNode* sqlParser::AggFuncContext::COUNT() {
  return getToken(sqlParser::COUNT, 0);
}

tree::TerminalNode* sqlParser::AggFuncContext::ALL() {
  return getToken(sqlParser::ALL, 0);
}

tree::TerminalNode* sqlParser::AggFuncContext::DISTINCT() {
  return getToken(sqlParser::DISTINCT, 0);
}

tree::TerminalNode* sqlParser::AggFuncContext::SUM() {
  return getToken(sqlParser::SUM, 0);
}

tree::TerminalNode* sqlParser::AggFuncContext::AVG() {
  return getToken(sqlParser::AVG, 0);
}

tree::TerminalNode* sqlParser::AggFuncContext::MIN() {
  return getToken(sqlParser::MIN, 0);
}

tree::TerminalNode* sqlParser::AggFuncContext::MAX() {
  return getToken(sqlParser::MAX, 0);
}

sqlParser::AggFuncContext::AggFuncContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::AggFuncContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitAggFunc(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DecodeOpContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::DecodeOpContext::DECODE() {
  return getToken(sqlParser::DECODE, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::DecodeOpContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::DecodeOpContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::DecodeOpContext::DecodeOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::DecodeOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDecodeOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ConcatOpContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::ConcatOpContext::CONCAT() {
  return getToken(sqlParser::CONCAT, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::ConcatOpContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::ConcatOpContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::ConcatOpContext::ConcatOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::ConcatOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitConcatOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- SubqueryExprContext ------------------------------------------------------------------

sqlParser::QueryContext* sqlParser::SubqueryExprContext::query() {
  return getRuleContext<sqlParser::QueryContext>(0);
}

sqlParser::SubqueryExprContext::SubqueryExprContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::SubqueryExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSubqueryExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfOpContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::IfOpContext::IF() {
  return getToken(sqlParser::IF, 0);
}

sqlParser::CasePredicateContext* sqlParser::IfOpContext::casePredicate() {
  return getRuleContext<sqlParser::CasePredicateContext>(0);
}

std::vector<sqlParser::ExprContext *> sqlParser::IfOpContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::IfOpContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::IfOpContext::IfOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::IfOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitIfOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LikeEscapeOpContext ------------------------------------------------------------------

std::vector<sqlParser::ExprContext *> sqlParser::LikeEscapeOpContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::LikeEscapeOpContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::LikeEscapeOpContext::LIKE() {
  return getToken(sqlParser::LIKE, 0);
}

tree::TerminalNode* sqlParser::LikeEscapeOpContext::ESCAPE() {
  return getToken(sqlParser::ESCAPE, 0);
}

tree::TerminalNode* sqlParser::LikeEscapeOpContext::NOT() {
  return getToken(sqlParser::NOT, 0);
}

sqlParser::LikeEscapeOpContext::LikeEscapeOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::LikeEscapeOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLikeEscapeOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- SubStringFuncContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::SubStringFuncContext::SUBSTRING() {
  return getToken(sqlParser::SUBSTRING, 0);
}

tree::TerminalNode* sqlParser::SubStringFuncContext::SUBSTR() {
  return getToken(sqlParser::SUBSTR, 0);
}

tree::TerminalNode* sqlParser::SubStringFuncContext::FROM() {
  return getToken(sqlParser::FROM, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::SubStringFuncContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::SubStringFuncContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::SubStringFuncContext::FOR() {
  return getToken(sqlParser::FOR, 0);
}

sqlParser::SubStringFuncContext::SubStringFuncContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::SubStringFuncContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSubStringFunc(this);
  else
    return visitor->visitChildren(this);
}
//----------------- SubqueryInOpContext ------------------------------------------------------------------

sqlParser::ExprContext* sqlParser::SubqueryInOpContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}

tree::TerminalNode* sqlParser::SubqueryInOpContext::IN() {
  return getToken(sqlParser::IN, 0);
}

sqlParser::QueryContext* sqlParser::SubqueryInOpContext::query() {
  return getRuleContext<sqlParser::QueryContext>(0);
}

tree::TerminalNode* sqlParser::SubqueryInOpContext::NOT() {
  return getToken(sqlParser::NOT, 0);
}

sqlParser::SubqueryInOpContext::SubqueryInOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::SubqueryInOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSubqueryInOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LogicOpContext ------------------------------------------------------------------

std::vector<sqlParser::ExprContext *> sqlParser::LogicOpContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::LogicOpContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::LogicOpContext::NOT() {
  return getToken(sqlParser::NOT, 0);
}

tree::TerminalNode* sqlParser::LogicOpContext::AND() {
  return getToken(sqlParser::AND, 0);
}

tree::TerminalNode* sqlParser::LogicOpContext::XOR() {
  return getToken(sqlParser::XOR, 0);
}

tree::TerminalNode* sqlParser::LogicOpContext::OR() {
  return getToken(sqlParser::OR, 0);
}

sqlParser::LogicOpContext::LogicOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::LogicOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLogicOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CastOpContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::CastOpContext::CAST() {
  return getToken(sqlParser::CAST, 0);
}

sqlParser::ExprContext* sqlParser::CastOpContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}

sqlParser::DataTypeContext* sqlParser::CastOpContext::dataType() {
  return getRuleContext<sqlParser::DataTypeContext>(0);
}

tree::TerminalNode* sqlParser::CastOpContext::AS() {
  return getToken(sqlParser::AS, 0);
}

sqlParser::CastOpContext::CastOpContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::CastOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCastOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParentheticContext ------------------------------------------------------------------

sqlParser::ExprContext* sqlParser::ParentheticContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}

sqlParser::ParentheticContext::ParentheticContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::ParentheticContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitParenthetic(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MethodCallContext ------------------------------------------------------------------

std::vector<sqlParser::ExprContext *> sqlParser::MethodCallContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::MethodCallContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::MethodNameContext* sqlParser::MethodCallContext::methodName() {
  return getRuleContext<sqlParser::MethodNameContext>(0);
}

sqlParser::MethodCallContext::MethodCallContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::MethodCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitMethodCall(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ExprContext* sqlParser::expr() {
   return expr(0);
}

sqlParser::ExprContext* sqlParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  sqlParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  sqlParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 220;
  enterRecursionRule(_localctx, 220, sqlParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1820);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 214, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<LiteralValContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(1558);
      literal();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<VarRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1559);
      variableName();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ColRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1560);
      columnRef();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<InlineCastContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1561);
      match(sqlParser::T__6);
      setState(1562);
      dataType();
      setState(1563);
      match(sqlParser::T__7);
      setState(1564);
      expr(40);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<UnaryOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1566);
      dynamic_cast<UnaryOpContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__10

      || _la == sqlParser::T__14)) {
        dynamic_cast<UnaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1567);
      expr(39);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<CastOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1568);
      match(sqlParser::CAST);
      setState(1569);
      match(sqlParser::T__6);
      setState(1570);
      expr(0);
      setState(1571);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5

      || _la == sqlParser::AS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1572);
      dataType();
      setState(1573);
      match(sqlParser::T__7);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<TypeOfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1575);
      match(sqlParser::TYPEOF);
      setState(1576);
      match(sqlParser::T__6);
      setState(1577);
      expr(0);
      setState(1578);
      match(sqlParser::T__7);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<CaseOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1580);
      match(sqlParser::CASE);
      setState(1600);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::T__6:
        case sqlParser::T__10:
        case sqlParser::T__14:
        case sqlParser::ANALYSE:
        case sqlParser::ANALYZE:
        case sqlParser::ANIDX:
        case sqlParser::ANTBL:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::COALESCE:
        case sqlParser::COMPACTIDX:
        case sqlParser::CONCAT:
        case sqlParser::COUNT:
        case sqlParser::CURRENT_CATALOG:
        case sqlParser::CURRENT_SCHEMA:
        case sqlParser::CURRENT_TIMESTAMP:
        case sqlParser::CURRENT_USER:
        case sqlParser::DATA:
        case sqlParser::DATE:
        case sqlParser::DATE_FORMAT:
        case sqlParser::DAYS:
        case sqlParser::DECODE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DIGITS:
        case sqlParser::DLOAD:
        case sqlParser::DOCOL:
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXIT_TOKEN:
        case sqlParser::EXPLAIN:
        case sqlParser::EXPORT:
        case sqlParser::EXTRACT:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
        case sqlParser::FILE:
        case sqlParser::FORCE:
        case sqlParser::FORMAT:
        case sqlParser::HASH:
        case sqlParser::HASHKEY:
        case sqlParser::HOURS:
        case sqlParser::IF:
        case sqlParser::IFNULL:
        case sqlParser::INDEX:
        case sqlParser::INTERVAL_DT:
        case sqlParser::INTERVAL_YM:
        case sqlParser::INTERVAL:
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOADREPLICATED:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSPTNCOL:
        case sqlParser::LSTBL:
        case sqlParser::MAX:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MILLIS:
        case sqlParser::MILLISECOND:
        case sqlParser::MILLISECONDS:
        case sqlParser::MIN:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::MS:
        case sqlParser::NODES:
        case sqlParser::NODE:
        case sqlParser::NOOP:
        case sqlParser::NOP:
        case sqlParser::NOT:
        case sqlParser::NOW:
        case sqlParser::SQL_NULL:
        case sqlParser::NULLIF:
        case sqlParser::OBJECTS:
        case sqlParser::OFF:
        case sqlParser::OFFSET:
        case sqlParser::ORIGIN:
        case sqlParser::OVERLAY:
        case sqlParser::PARTITIONS:
        case sqlParser::PATHS:
        case sqlParser::PIPELINE:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::RECORD_ENDS:
        case sqlParser::REFERENCE:
        case sqlParser::REGIONS:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SEC:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
        case sqlParser::SLEEP:
        case sqlParser::SPACE:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::STATUS:
        case sqlParser::SUBSTR:
        case sqlParser::SUBSTRING:
        case sqlParser::SUM:
        case sqlParser::SZIDX:
        case sqlParser::SZTBL:
        case sqlParser::S_ONLY:
        case sqlParser::TABLESIZE:
        case sqlParser::TEXT:
        case sqlParser::TIME:
        case sqlParser::TIMESTAMP:
        case sqlParser::TOP:
        case sqlParser::TRACKMEMORY:
        case sqlParser::TREE:
        case sqlParser::TRIM:
        case sqlParser::TRUNCATE:
        case sqlParser::SQL_TRUE:
        case sqlParser::TYPE:
        case sqlParser::TYPEOF:
        case sqlParser::UNLOAD:
        case sqlParser::VALUE:
        case sqlParser::WEEK:
        case sqlParser::WEEKS:
        case sqlParser::WITH_NAMES:
        case sqlParser::WITH_TYPES:
        case sqlParser::XOR:
        case sqlParser::YEARS:
        case sqlParser::PLAIN_IDENTIFIER:
        case sqlParser::BAKTIKD_IDENTIFIER:
        case sqlParser::DUBLQUOTED_IDENTIFIER:
        case sqlParser::SINGLQUOTED_STRING:
        case sqlParser::DUBLQUOTED_STRING:
        case sqlParser::FLOAT_LITERAL:
        case sqlParser::DECIMAL_LITERAL:
        case sqlParser::INTEGER_LITERAL: {
          setState(1581);
          dynamic_cast<CaseOpContext *>(_localctx)->caseExpr = expr(0);
          setState(1587); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1582);
            match(sqlParser::WHEN);
            setState(1583);
            expr(0);
            setState(1584);
            match(sqlParser::THEN);
            setState(1585);
            expr(0);
            setState(1589); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

        case sqlParser::WHEN: {
          setState(1596); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1591);
            match(sqlParser::WHEN);
            setState(1592);
            casePredicate();
            setState(1593);
            match(sqlParser::THEN);
            setState(1594);
            expr(0);
            setState(1598); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(1604);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ELSE) {
        setState(1602);
        match(sqlParser::ELSE);
        setState(1603);
        dynamic_cast<CaseOpContext *>(_localctx)->elseExpr = expr(0);
      }
      setState(1606);
      match(sqlParser::END);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<IfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1608);
      match(sqlParser::IF);
      setState(1609);
      match(sqlParser::T__6);
      setState(1610);
      casePredicate();
      setState(1611);
      match(sqlParser::T__5);
      setState(1612);
      expr(0);
      setState(1613);
      match(sqlParser::T__5);
      setState(1614);
      expr(0);
      setState(1615);
      match(sqlParser::T__7);
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<IfNullOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1617);
      match(sqlParser::IFNULL);
      setState(1618);
      match(sqlParser::T__6);
      setState(1619);
      expr(0);
      setState(1620);
      match(sqlParser::T__5);
      setState(1621);
      expr(0);
      setState(1622);
      match(sqlParser::T__7);
      break;
    }

    case 11: {
      _localctx = _tracker.createInstance<NullIfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1624);
      match(sqlParser::NULLIF);
      setState(1625);
      match(sqlParser::T__6);
      setState(1626);
      expr(0);
      setState(1627);
      match(sqlParser::T__5);
      setState(1628);
      expr(0);
      setState(1629);
      match(sqlParser::T__7);
      break;
    }

    case 12: {
      _localctx = _tracker.createInstance<CoalesceOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1631);
      match(sqlParser::COALESCE);
      setState(1632);
      match(sqlParser::T__6);
      setState(1641);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__6)
        | (1ULL << sqlParser::T__10)
        | (1ULL << sqlParser::T__14)
        | (1ULL << sqlParser::ANALYSE)
        | (1ULL << sqlParser::ANALYZE)
        | (1ULL << sqlParser::ANIDX)
        | (1ULL << sqlParser::ANTBL)
        | (1ULL << sqlParser::ASC)
        | (1ULL << sqlParser::AVG)
        | (1ULL << sqlParser::BIT)
        | (1ULL << sqlParser::BITS)
        | (1ULL << sqlParser::BLOCK)
        | (1ULL << sqlParser::BLOOM)
        | (1ULL << sqlParser::BPA)
        | (1ULL << sqlParser::BTREE)
        | (1ULL << sqlParser::BYE)
        | (1ULL << sqlParser::BYTE)
        | (1ULL << sqlParser::BYTES)
        | (1ULL << sqlParser::CASE)
        | (1ULL << sqlParser::CAST)
        | (1ULL << sqlParser::CATALOG)
        | (1ULL << sqlParser::CHARS)
        | (1ULL << sqlParser::COALESCE)
        | (1ULL << sqlParser::COMPACTIDX)
        | (1ULL << sqlParser::CONCAT)
        | (1ULL << sqlParser::COUNT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::CURRENT_CATALOG - 64))
        | (1ULL << (sqlParser::CURRENT_SCHEMA - 64))
        | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 64))
        | (1ULL << (sqlParser::CURRENT_USER - 64))
        | (1ULL << (sqlParser::DATA - 64))
        | (1ULL << (sqlParser::DATE - 64))
        | (1ULL << (sqlParser::DATE_FORMAT - 64))
        | (1ULL << (sqlParser::DAYS - 64))
        | (1ULL << (sqlParser::DECODE - 64))
        | (1ULL << (sqlParser::DESC - 64))
        | (1ULL << (sqlParser::DETAIL - 64))
        | (1ULL << (sqlParser::DIGITS - 64))
        | (1ULL << (sqlParser::DLOAD - 64))
        | (1ULL << (sqlParser::DOCOL - 64))
        | (1ULL << (sqlParser::ECHO - 64))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
        | (1ULL << (sqlParser::EXISTS - 64))
        | (1ULL << (sqlParser::EXIT_TOKEN - 64))
        | (1ULL << (sqlParser::EXPLAIN - 64))
        | (1ULL << (sqlParser::EXPORT - 64))
        | (1ULL << (sqlParser::EXTRACT - 64))
        | (1ULL << (sqlParser::SQL_FALSE - 64))
        | (1ULL << (sqlParser::FEDERATION - 64))
        | (1ULL << (sqlParser::FIELD_DELIMITER - 64))
        | (1ULL << (sqlParser::FILE - 64))
        | (1ULL << (sqlParser::FORCE - 64))
        | (1ULL << (sqlParser::FORMAT - 64))
        | (1ULL << (sqlParser::HASH - 64))
        | (1ULL << (sqlParser::HASHKEY - 64))
        | (1ULL << (sqlParser::HOURS - 64))
        | (1ULL << (sqlParser::IF - 64))
        | (1ULL << (sqlParser::IFNULL - 64))
        | (1ULL << (sqlParser::INDEX - 64))
        | (1ULL << (sqlParser::INTERVAL_DT - 64))
        | (1ULL << (sqlParser::INTERVAL_YM - 64))
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::KEY - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::LIMIT - 130))
        | (1ULL << (sqlParser::LOADREPLICATED - 130))
        | (1ULL << (sqlParser::LOAD - 130))
        | (1ULL << (sqlParser::LOG - 130))
        | (1ULL << (sqlParser::LOOP - 130))
        | (1ULL << (sqlParser::LSIDX - 130))
        | (1ULL << (sqlParser::LSPTNCOL - 130))
        | (1ULL << (sqlParser::LSTBL - 130))
        | (1ULL << (sqlParser::MAX - 130))
        | (1ULL << (sqlParser::MEMORYLEAKS - 130))
        | (1ULL << (sqlParser::MILLIS - 130))
        | (1ULL << (sqlParser::MILLISECOND - 130))
        | (1ULL << (sqlParser::MILLISECONDS - 130))
        | (1ULL << (sqlParser::MIN - 130))
        | (1ULL << (sqlParser::MINUTES - 130))
        | (1ULL << (sqlParser::MONOTONIC - 130))
        | (1ULL << (sqlParser::MONTHS - 130))
        | (1ULL << (sqlParser::MS - 130))
        | (1ULL << (sqlParser::NODES - 130))
        | (1ULL << (sqlParser::NODE - 130))
        | (1ULL << (sqlParser::NOOP - 130))
        | (1ULL << (sqlParser::NOP - 130))
        | (1ULL << (sqlParser::NOT - 130))
        | (1ULL << (sqlParser::NOW - 130))
        | (1ULL << (sqlParser::SQL_NULL - 130))
        | (1ULL << (sqlParser::NULLIF - 130))
        | (1ULL << (sqlParser::OBJECTS - 130))
        | (1ULL << (sqlParser::OFF - 130))
        | (1ULL << (sqlParser::OFFSET - 130))
        | (1ULL << (sqlParser::ORIGIN - 130))
        | (1ULL << (sqlParser::OVERLAY - 130))
        | (1ULL << (sqlParser::PARTITIONS - 130))
        | (1ULL << (sqlParser::PATHS - 130))
        | (1ULL << (sqlParser::PIPELINE - 130))
        | (1ULL << (sqlParser::PLACING - 130))
        | (1ULL << (sqlParser::POSITION - 130))
        | (1ULL << (sqlParser::PRETTY - 130))
        | (1ULL << (sqlParser::QUARTER - 130))
        | (1ULL << (sqlParser::QUARTERS - 130))
        | (1ULL << (sqlParser::RECORD_ENDS - 130))
        | (1ULL << (sqlParser::REFERENCE - 130))
        | (1ULL << (sqlParser::REGIONS - 130))
        | (1ULL << (sqlParser::REPLICATED - 130))
        | (1ULL << (sqlParser::ROWSIZE - 130))
        | (1ULL << (sqlParser::SCHEMA - 130))
        | (1ULL << (sqlParser::SEC - 130))
        | (1ULL << (sqlParser::SECONDS - 130)))) != 0) || ((((_la - 197) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 197)) & ((1ULL << (sqlParser::SKIPLIST - 197))
        | (1ULL << (sqlParser::SLEEP - 197))
        | (1ULL << (sqlParser::SPACE - 197))
        | (1ULL << (sqlParser::STAGE - 197))
        | (1ULL << (sqlParser::STATS - 197))
        | (1ULL << (sqlParser::STATISTICS - 197))
        | (1ULL << (sqlParser::STATUS - 197))
        | (1ULL << (sqlParser::SUBSTR - 197))
        | (1ULL << (sqlParser::SUBSTRING - 197))
        | (1ULL << (sqlParser::SUM - 197))
        | (1ULL << (sqlParser::SZIDX - 197))
        | (1ULL << (sqlParser::SZTBL - 197))
        | (1ULL << (sqlParser::S_ONLY - 197))
        | (1ULL << (sqlParser::TABLESIZE - 197))
        | (1ULL << (sqlParser::TEXT - 197))
        | (1ULL << (sqlParser::TIME - 197))
        | (1ULL << (sqlParser::TIMESTAMP - 197))
        | (1ULL << (sqlParser::TOP - 197))
        | (1ULL << (sqlParser::TRACKMEMORY - 197))
        | (1ULL << (sqlParser::TREE - 197))
        | (1ULL << (sqlParser::TRIM - 197))
        | (1ULL << (sqlParser::TRUNCATE - 197))
        | (1ULL << (sqlParser::SQL_TRUE - 197))
        | (1ULL << (sqlParser::TYPE - 197))
        | (1ULL << (sqlParser::TYPEOF - 197))
        | (1ULL << (sqlParser::UNLOAD - 197))
        | (1ULL << (sqlParser::VALUE - 197))
        | (1ULL << (sqlParser::WEEK - 197))
        | (1ULL << (sqlParser::WEEKS - 197))
        | (1ULL << (sqlParser::WITH_NAMES - 197))
        | (1ULL << (sqlParser::WITH_TYPES - 197))
        | (1ULL << (sqlParser::XOR - 197))
        | (1ULL << (sqlParser::YEARS - 197))
        | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 197))
        | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 197))
        | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 197))
        | (1ULL << (sqlParser::SINGLQUOTED_STRING - 197))
        | (1ULL << (sqlParser::DUBLQUOTED_STRING - 197))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 197))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 197))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 197)))) != 0)) {
        setState(1633);
        expr(0);
        setState(1638);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1634);
          match(sqlParser::T__5);
          setState(1635);
          expr(0);
          setState(1640);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1643);
      match(sqlParser::T__7);
      break;
    }

    case 13: {
      _localctx = _tracker.createInstance<DecodeOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1644);
      match(sqlParser::DECODE);
      setState(1645);
      match(sqlParser::T__6);
      setState(1654);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__6)
        | (1ULL << sqlParser::T__10)
        | (1ULL << sqlParser::T__14)
        | (1ULL << sqlParser::ANALYSE)
        | (1ULL << sqlParser::ANALYZE)
        | (1ULL << sqlParser::ANIDX)
        | (1ULL << sqlParser::ANTBL)
        | (1ULL << sqlParser::ASC)
        | (1ULL << sqlParser::AVG)
        | (1ULL << sqlParser::BIT)
        | (1ULL << sqlParser::BITS)
        | (1ULL << sqlParser::BLOCK)
        | (1ULL << sqlParser::BLOOM)
        | (1ULL << sqlParser::BPA)
        | (1ULL << sqlParser::BTREE)
        | (1ULL << sqlParser::BYE)
        | (1ULL << sqlParser::BYTE)
        | (1ULL << sqlParser::BYTES)
        | (1ULL << sqlParser::CASE)
        | (1ULL << sqlParser::CAST)
        | (1ULL << sqlParser::CATALOG)
        | (1ULL << sqlParser::CHARS)
        | (1ULL << sqlParser::COALESCE)
        | (1ULL << sqlParser::COMPACTIDX)
        | (1ULL << sqlParser::CONCAT)
        | (1ULL << sqlParser::COUNT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::CURRENT_CATALOG - 64))
        | (1ULL << (sqlParser::CURRENT_SCHEMA - 64))
        | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 64))
        | (1ULL << (sqlParser::CURRENT_USER - 64))
        | (1ULL << (sqlParser::DATA - 64))
        | (1ULL << (sqlParser::DATE - 64))
        | (1ULL << (sqlParser::DATE_FORMAT - 64))
        | (1ULL << (sqlParser::DAYS - 64))
        | (1ULL << (sqlParser::DECODE - 64))
        | (1ULL << (sqlParser::DESC - 64))
        | (1ULL << (sqlParser::DETAIL - 64))
        | (1ULL << (sqlParser::DIGITS - 64))
        | (1ULL << (sqlParser::DLOAD - 64))
        | (1ULL << (sqlParser::DOCOL - 64))
        | (1ULL << (sqlParser::ECHO - 64))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
        | (1ULL << (sqlParser::EXISTS - 64))
        | (1ULL << (sqlParser::EXIT_TOKEN - 64))
        | (1ULL << (sqlParser::EXPLAIN - 64))
        | (1ULL << (sqlParser::EXPORT - 64))
        | (1ULL << (sqlParser::EXTRACT - 64))
        | (1ULL << (sqlParser::SQL_FALSE - 64))
        | (1ULL << (sqlParser::FEDERATION - 64))
        | (1ULL << (sqlParser::FIELD_DELIMITER - 64))
        | (1ULL << (sqlParser::FILE - 64))
        | (1ULL << (sqlParser::FORCE - 64))
        | (1ULL << (sqlParser::FORMAT - 64))
        | (1ULL << (sqlParser::HASH - 64))
        | (1ULL << (sqlParser::HASHKEY - 64))
        | (1ULL << (sqlParser::HOURS - 64))
        | (1ULL << (sqlParser::IF - 64))
        | (1ULL << (sqlParser::IFNULL - 64))
        | (1ULL << (sqlParser::INDEX - 64))
        | (1ULL << (sqlParser::INTERVAL_DT - 64))
        | (1ULL << (sqlParser::INTERVAL_YM - 64))
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::KEY - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::LIMIT - 130))
        | (1ULL << (sqlParser::LOADREPLICATED - 130))
        | (1ULL << (sqlParser::LOAD - 130))
        | (1ULL << (sqlParser::LOG - 130))
        | (1ULL << (sqlParser::LOOP - 130))
        | (1ULL << (sqlParser::LSIDX - 130))
        | (1ULL << (sqlParser::LSPTNCOL - 130))
        | (1ULL << (sqlParser::LSTBL - 130))
        | (1ULL << (sqlParser::MAX - 130))
        | (1ULL << (sqlParser::MEMORYLEAKS - 130))
        | (1ULL << (sqlParser::MILLIS - 130))
        | (1ULL << (sqlParser::MILLISECOND - 130))
        | (1ULL << (sqlParser::MILLISECONDS - 130))
        | (1ULL << (sqlParser::MIN - 130))
        | (1ULL << (sqlParser::MINUTES - 130))
        | (1ULL << (sqlParser::MONOTONIC - 130))
        | (1ULL << (sqlParser::MONTHS - 130))
        | (1ULL << (sqlParser::MS - 130))
        | (1ULL << (sqlParser::NODES - 130))
        | (1ULL << (sqlParser::NODE - 130))
        | (1ULL << (sqlParser::NOOP - 130))
        | (1ULL << (sqlParser::NOP - 130))
        | (1ULL << (sqlParser::NOT - 130))
        | (1ULL << (sqlParser::NOW - 130))
        | (1ULL << (sqlParser::SQL_NULL - 130))
        | (1ULL << (sqlParser::NULLIF - 130))
        | (1ULL << (sqlParser::OBJECTS - 130))
        | (1ULL << (sqlParser::OFF - 130))
        | (1ULL << (sqlParser::OFFSET - 130))
        | (1ULL << (sqlParser::ORIGIN - 130))
        | (1ULL << (sqlParser::OVERLAY - 130))
        | (1ULL << (sqlParser::PARTITIONS - 130))
        | (1ULL << (sqlParser::PATHS - 130))
        | (1ULL << (sqlParser::PIPELINE - 130))
        | (1ULL << (sqlParser::PLACING - 130))
        | (1ULL << (sqlParser::POSITION - 130))
        | (1ULL << (sqlParser::PRETTY - 130))
        | (1ULL << (sqlParser::QUARTER - 130))
        | (1ULL << (sqlParser::QUARTERS - 130))
        | (1ULL << (sqlParser::RECORD_ENDS - 130))
        | (1ULL << (sqlParser::REFERENCE - 130))
        | (1ULL << (sqlParser::REGIONS - 130))
        | (1ULL << (sqlParser::REPLICATED - 130))
        | (1ULL << (sqlParser::ROWSIZE - 130))
        | (1ULL << (sqlParser::SCHEMA - 130))
        | (1ULL << (sqlParser::SEC - 130))
        | (1ULL << (sqlParser::SECONDS - 130)))) != 0) || ((((_la - 197) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 197)) & ((1ULL << (sqlParser::SKIPLIST - 197))
        | (1ULL << (sqlParser::SLEEP - 197))
        | (1ULL << (sqlParser::SPACE - 197))
        | (1ULL << (sqlParser::STAGE - 197))
        | (1ULL << (sqlParser::STATS - 197))
        | (1ULL << (sqlParser::STATISTICS - 197))
        | (1ULL << (sqlParser::STATUS - 197))
        | (1ULL << (sqlParser::SUBSTR - 197))
        | (1ULL << (sqlParser::SUBSTRING - 197))
        | (1ULL << (sqlParser::SUM - 197))
        | (1ULL << (sqlParser::SZIDX - 197))
        | (1ULL << (sqlParser::SZTBL - 197))
        | (1ULL << (sqlParser::S_ONLY - 197))
        | (1ULL << (sqlParser::TABLESIZE - 197))
        | (1ULL << (sqlParser::TEXT - 197))
        | (1ULL << (sqlParser::TIME - 197))
        | (1ULL << (sqlParser::TIMESTAMP - 197))
        | (1ULL << (sqlParser::TOP - 197))
        | (1ULL << (sqlParser::TRACKMEMORY - 197))
        | (1ULL << (sqlParser::TREE - 197))
        | (1ULL << (sqlParser::TRIM - 197))
        | (1ULL << (sqlParser::TRUNCATE - 197))
        | (1ULL << (sqlParser::SQL_TRUE - 197))
        | (1ULL << (sqlParser::TYPE - 197))
        | (1ULL << (sqlParser::TYPEOF - 197))
        | (1ULL << (sqlParser::UNLOAD - 197))
        | (1ULL << (sqlParser::VALUE - 197))
        | (1ULL << (sqlParser::WEEK - 197))
        | (1ULL << (sqlParser::WEEKS - 197))
        | (1ULL << (sqlParser::WITH_NAMES - 197))
        | (1ULL << (sqlParser::WITH_TYPES - 197))
        | (1ULL << (sqlParser::XOR - 197))
        | (1ULL << (sqlParser::YEARS - 197))
        | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 197))
        | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 197))
        | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 197))
        | (1ULL << (sqlParser::SINGLQUOTED_STRING - 197))
        | (1ULL << (sqlParser::DUBLQUOTED_STRING - 197))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 197))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 197))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 197)))) != 0)) {
        setState(1646);
        expr(0);
        setState(1651);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1647);
          match(sqlParser::T__5);
          setState(1648);
          expr(0);
          setState(1653);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1656);
      match(sqlParser::T__7);
      break;
    }

    case 14: {
      _localctx = _tracker.createInstance<ConcatOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1657);
      match(sqlParser::CONCAT);
      setState(1658);
      match(sqlParser::T__6);
      setState(1667);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__6)
        | (1ULL << sqlParser::T__10)
        | (1ULL << sqlParser::T__14)
        | (1ULL << sqlParser::ANALYSE)
        | (1ULL << sqlParser::ANALYZE)
        | (1ULL << sqlParser::ANIDX)
        | (1ULL << sqlParser::ANTBL)
        | (1ULL << sqlParser::ASC)
        | (1ULL << sqlParser::AVG)
        | (1ULL << sqlParser::BIT)
        | (1ULL << sqlParser::BITS)
        | (1ULL << sqlParser::BLOCK)
        | (1ULL << sqlParser::BLOOM)
        | (1ULL << sqlParser::BPA)
        | (1ULL << sqlParser::BTREE)
        | (1ULL << sqlParser::BYE)
        | (1ULL << sqlParser::BYTE)
        | (1ULL << sqlParser::BYTES)
        | (1ULL << sqlParser::CASE)
        | (1ULL << sqlParser::CAST)
        | (1ULL << sqlParser::CATALOG)
        | (1ULL << sqlParser::CHARS)
        | (1ULL << sqlParser::COALESCE)
        | (1ULL << sqlParser::COMPACTIDX)
        | (1ULL << sqlParser::CONCAT)
        | (1ULL << sqlParser::COUNT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::CURRENT_CATALOG - 64))
        | (1ULL << (sqlParser::CURRENT_SCHEMA - 64))
        | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 64))
        | (1ULL << (sqlParser::CURRENT_USER - 64))
        | (1ULL << (sqlParser::DATA - 64))
        | (1ULL << (sqlParser::DATE - 64))
        | (1ULL << (sqlParser::DATE_FORMAT - 64))
        | (1ULL << (sqlParser::DAYS - 64))
        | (1ULL << (sqlParser::DECODE - 64))
        | (1ULL << (sqlParser::DESC - 64))
        | (1ULL << (sqlParser::DETAIL - 64))
        | (1ULL << (sqlParser::DIGITS - 64))
        | (1ULL << (sqlParser::DLOAD - 64))
        | (1ULL << (sqlParser::DOCOL - 64))
        | (1ULL << (sqlParser::ECHO - 64))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
        | (1ULL << (sqlParser::EXISTS - 64))
        | (1ULL << (sqlParser::EXIT_TOKEN - 64))
        | (1ULL << (sqlParser::EXPLAIN - 64))
        | (1ULL << (sqlParser::EXPORT - 64))
        | (1ULL << (sqlParser::EXTRACT - 64))
        | (1ULL << (sqlParser::SQL_FALSE - 64))
        | (1ULL << (sqlParser::FEDERATION - 64))
        | (1ULL << (sqlParser::FIELD_DELIMITER - 64))
        | (1ULL << (sqlParser::FILE - 64))
        | (1ULL << (sqlParser::FORCE - 64))
        | (1ULL << (sqlParser::FORMAT - 64))
        | (1ULL << (sqlParser::HASH - 64))
        | (1ULL << (sqlParser::HASHKEY - 64))
        | (1ULL << (sqlParser::HOURS - 64))
        | (1ULL << (sqlParser::IF - 64))
        | (1ULL << (sqlParser::IFNULL - 64))
        | (1ULL << (sqlParser::INDEX - 64))
        | (1ULL << (sqlParser::INTERVAL_DT - 64))
        | (1ULL << (sqlParser::INTERVAL_YM - 64))
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::KEY - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::LIMIT - 130))
        | (1ULL << (sqlParser::LOADREPLICATED - 130))
        | (1ULL << (sqlParser::LOAD - 130))
        | (1ULL << (sqlParser::LOG - 130))
        | (1ULL << (sqlParser::LOOP - 130))
        | (1ULL << (sqlParser::LSIDX - 130))
        | (1ULL << (sqlParser::LSPTNCOL - 130))
        | (1ULL << (sqlParser::LSTBL - 130))
        | (1ULL << (sqlParser::MAX - 130))
        | (1ULL << (sqlParser::MEMORYLEAKS - 130))
        | (1ULL << (sqlParser::MILLIS - 130))
        | (1ULL << (sqlParser::MILLISECOND - 130))
        | (1ULL << (sqlParser::MILLISECONDS - 130))
        | (1ULL << (sqlParser::MIN - 130))
        | (1ULL << (sqlParser::MINUTES - 130))
        | (1ULL << (sqlParser::MONOTONIC - 130))
        | (1ULL << (sqlParser::MONTHS - 130))
        | (1ULL << (sqlParser::MS - 130))
        | (1ULL << (sqlParser::NODES - 130))
        | (1ULL << (sqlParser::NODE - 130))
        | (1ULL << (sqlParser::NOOP - 130))
        | (1ULL << (sqlParser::NOP - 130))
        | (1ULL << (sqlParser::NOT - 130))
        | (1ULL << (sqlParser::NOW - 130))
        | (1ULL << (sqlParser::SQL_NULL - 130))
        | (1ULL << (sqlParser::NULLIF - 130))
        | (1ULL << (sqlParser::OBJECTS - 130))
        | (1ULL << (sqlParser::OFF - 130))
        | (1ULL << (sqlParser::OFFSET - 130))
        | (1ULL << (sqlParser::ORIGIN - 130))
        | (1ULL << (sqlParser::OVERLAY - 130))
        | (1ULL << (sqlParser::PARTITIONS - 130))
        | (1ULL << (sqlParser::PATHS - 130))
        | (1ULL << (sqlParser::PIPELINE - 130))
        | (1ULL << (sqlParser::PLACING - 130))
        | (1ULL << (sqlParser::POSITION - 130))
        | (1ULL << (sqlParser::PRETTY - 130))
        | (1ULL << (sqlParser::QUARTER - 130))
        | (1ULL << (sqlParser::QUARTERS - 130))
        | (1ULL << (sqlParser::RECORD_ENDS - 130))
        | (1ULL << (sqlParser::REFERENCE - 130))
        | (1ULL << (sqlParser::REGIONS - 130))
        | (1ULL << (sqlParser::REPLICATED - 130))
        | (1ULL << (sqlParser::ROWSIZE - 130))
        | (1ULL << (sqlParser::SCHEMA - 130))
        | (1ULL << (sqlParser::SEC - 130))
        | (1ULL << (sqlParser::SECONDS - 130)))) != 0) || ((((_la - 197) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 197)) & ((1ULL << (sqlParser::SKIPLIST - 197))
        | (1ULL << (sqlParser::SLEEP - 197))
        | (1ULL << (sqlParser::SPACE - 197))
        | (1ULL << (sqlParser::STAGE - 197))
        | (1ULL << (sqlParser::STATS - 197))
        | (1ULL << (sqlParser::STATISTICS - 197))
        | (1ULL << (sqlParser::STATUS - 197))
        | (1ULL << (sqlParser::SUBSTR - 197))
        | (1ULL << (sqlParser::SUBSTRING - 197))
        | (1ULL << (sqlParser::SUM - 197))
        | (1ULL << (sqlParser::SZIDX - 197))
        | (1ULL << (sqlParser::SZTBL - 197))
        | (1ULL << (sqlParser::S_ONLY - 197))
        | (1ULL << (sqlParser::TABLESIZE - 197))
        | (1ULL << (sqlParser::TEXT - 197))
        | (1ULL << (sqlParser::TIME - 197))
        | (1ULL << (sqlParser::TIMESTAMP - 197))
        | (1ULL << (sqlParser::TOP - 197))
        | (1ULL << (sqlParser::TRACKMEMORY - 197))
        | (1ULL << (sqlParser::TREE - 197))
        | (1ULL << (sqlParser::TRIM - 197))
        | (1ULL << (sqlParser::TRUNCATE - 197))
        | (1ULL << (sqlParser::SQL_TRUE - 197))
        | (1ULL << (sqlParser::TYPE - 197))
        | (1ULL << (sqlParser::TYPEOF - 197))
        | (1ULL << (sqlParser::UNLOAD - 197))
        | (1ULL << (sqlParser::VALUE - 197))
        | (1ULL << (sqlParser::WEEK - 197))
        | (1ULL << (sqlParser::WEEKS - 197))
        | (1ULL << (sqlParser::WITH_NAMES - 197))
        | (1ULL << (sqlParser::WITH_TYPES - 197))
        | (1ULL << (sqlParser::XOR - 197))
        | (1ULL << (sqlParser::YEARS - 197))
        | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 197))
        | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 197))
        | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 197))
        | (1ULL << (sqlParser::SINGLQUOTED_STRING - 197))
        | (1ULL << (sqlParser::DUBLQUOTED_STRING - 197))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 197))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 197))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 197)))) != 0)) {
        setState(1659);
        expr(0);
        setState(1664);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1660);
          match(sqlParser::T__5);
          setState(1661);
          expr(0);
          setState(1666);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1669);
      match(sqlParser::T__7);
      break;
    }

    case 15: {
      _localctx = _tracker.createInstance<OverlayFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1670);
      match(sqlParser::OVERLAY);
      setState(1671);
      match(sqlParser::T__6);
      setState(1672);
      expr(0);
      setState(1673);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5 || _la == sqlParser::PLACING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1674);
      expr(0);
      setState(1675);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5 || _la == sqlParser::FROM)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1676);
      expr(0);
      setState(1679);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__5 || _la == sqlParser::FOR) {
        setState(1677);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__5 || _la == sqlParser::FOR)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(1678);
        expr(0);
      }
      setState(1681);
      match(sqlParser::T__7);
      break;
    }

    case 16: {
      _localctx = _tracker.createInstance<PositionFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1683);
      match(sqlParser::POSITION);
      setState(1684);
      match(sqlParser::T__6);
      setState(1685);
      expr(0);
      setState(1686);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5 || _la == sqlParser::IN)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1687);
      expr(0);
      setState(1688);
      match(sqlParser::T__7);
      break;
    }

    case 17: {
      _localctx = _tracker.createInstance<SubStringFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1690);
      _la = _input->LA(1);
      if (!(_la == sqlParser::SUBSTR

      || _la == sqlParser::SUBSTRING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1691);
      match(sqlParser::T__6);
      setState(1706);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 200, _ctx)) {
      case 1: {
        setState(1692);
        dynamic_cast<SubStringFuncContext *>(_localctx)->src = expr(0);
        setState(1693);
        match(sqlParser::T__5);
        setState(1694);
        dynamic_cast<SubStringFuncContext *>(_localctx)->start = expr(0);
        setState(1697);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1695);
          match(sqlParser::T__5);
          setState(1696);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      case 2: {
        setState(1699);
        dynamic_cast<SubStringFuncContext *>(_localctx)->src = expr(0);
        setState(1700);
        match(sqlParser::FROM);
        setState(1701);
        dynamic_cast<SubStringFuncContext *>(_localctx)->start = expr(0);
        setState(1704);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::FOR) {
          setState(1702);
          match(sqlParser::FOR);
          setState(1703);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      }
      setState(1708);
      match(sqlParser::T__7);
      break;
    }

    case 18: {
      _localctx = _tracker.createInstance<TrimFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1710);
      match(sqlParser::TRIM);
      setState(1711);
      match(sqlParser::T__6);
      setState(1725);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 204, _ctx)) {
      case 1: {
        setState(1712);
        expr(0);
        setState(1715);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1713);
          match(sqlParser::T__5);
          setState(1714);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        break;
      }

      case 2: {
        setState(1718);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::BOTH || _la == sqlParser::LEADING || _la == sqlParser::TRAILING) {
          setState(1717);
          _la = _input->LA(1);
          if (!(_la == sqlParser::BOTH || _la == sqlParser::LEADING || _la == sqlParser::TRAILING)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        setState(1721);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << sqlParser::T__6)
          | (1ULL << sqlParser::T__10)
          | (1ULL << sqlParser::T__14)
          | (1ULL << sqlParser::ANALYSE)
          | (1ULL << sqlParser::ANALYZE)
          | (1ULL << sqlParser::ANIDX)
          | (1ULL << sqlParser::ANTBL)
          | (1ULL << sqlParser::ASC)
          | (1ULL << sqlParser::AVG)
          | (1ULL << sqlParser::BIT)
          | (1ULL << sqlParser::BITS)
          | (1ULL << sqlParser::BLOCK)
          | (1ULL << sqlParser::BLOOM)
          | (1ULL << sqlParser::BPA)
          | (1ULL << sqlParser::BTREE)
          | (1ULL << sqlParser::BYE)
          | (1ULL << sqlParser::BYTE)
          | (1ULL << sqlParser::BYTES)
          | (1ULL << sqlParser::CASE)
          | (1ULL << sqlParser::CAST)
          | (1ULL << sqlParser::CATALOG)
          | (1ULL << sqlParser::CHARS)
          | (1ULL << sqlParser::COALESCE)
          | (1ULL << sqlParser::COMPACTIDX)
          | (1ULL << sqlParser::CONCAT)
          | (1ULL << sqlParser::COUNT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::CURRENT_CATALOG - 64))
          | (1ULL << (sqlParser::CURRENT_SCHEMA - 64))
          | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 64))
          | (1ULL << (sqlParser::CURRENT_USER - 64))
          | (1ULL << (sqlParser::DATA - 64))
          | (1ULL << (sqlParser::DATE - 64))
          | (1ULL << (sqlParser::DATE_FORMAT - 64))
          | (1ULL << (sqlParser::DAYS - 64))
          | (1ULL << (sqlParser::DECODE - 64))
          | (1ULL << (sqlParser::DESC - 64))
          | (1ULL << (sqlParser::DETAIL - 64))
          | (1ULL << (sqlParser::DIGITS - 64))
          | (1ULL << (sqlParser::DLOAD - 64))
          | (1ULL << (sqlParser::DOCOL - 64))
          | (1ULL << (sqlParser::ECHO - 64))
          | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
          | (1ULL << (sqlParser::EXISTS - 64))
          | (1ULL << (sqlParser::EXIT_TOKEN - 64))
          | (1ULL << (sqlParser::EXPLAIN - 64))
          | (1ULL << (sqlParser::EXPORT - 64))
          | (1ULL << (sqlParser::EXTRACT - 64))
          | (1ULL << (sqlParser::SQL_FALSE - 64))
          | (1ULL << (sqlParser::FEDERATION - 64))
          | (1ULL << (sqlParser::FIELD_DELIMITER - 64))
          | (1ULL << (sqlParser::FILE - 64))
          | (1ULL << (sqlParser::FORCE - 64))
          | (1ULL << (sqlParser::FORMAT - 64))
          | (1ULL << (sqlParser::HASH - 64))
          | (1ULL << (sqlParser::HASHKEY - 64))
          | (1ULL << (sqlParser::HOURS - 64))
          | (1ULL << (sqlParser::IF - 64))
          | (1ULL << (sqlParser::IFNULL - 64))
          | (1ULL << (sqlParser::INDEX - 64))
          | (1ULL << (sqlParser::INTERVAL_DT - 64))
          | (1ULL << (sqlParser::INTERVAL_YM - 64))
          | (1ULL << (sqlParser::INTERVAL - 64))
          | (1ULL << (sqlParser::KEY - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::LIMIT - 130))
          | (1ULL << (sqlParser::LOADREPLICATED - 130))
          | (1ULL << (sqlParser::LOAD - 130))
          | (1ULL << (sqlParser::LOG - 130))
          | (1ULL << (sqlParser::LOOP - 130))
          | (1ULL << (sqlParser::LSIDX - 130))
          | (1ULL << (sqlParser::LSPTNCOL - 130))
          | (1ULL << (sqlParser::LSTBL - 130))
          | (1ULL << (sqlParser::MAX - 130))
          | (1ULL << (sqlParser::MEMORYLEAKS - 130))
          | (1ULL << (sqlParser::MILLIS - 130))
          | (1ULL << (sqlParser::MILLISECOND - 130))
          | (1ULL << (sqlParser::MILLISECONDS - 130))
          | (1ULL << (sqlParser::MIN - 130))
          | (1ULL << (sqlParser::MINUTES - 130))
          | (1ULL << (sqlParser::MONOTONIC - 130))
          | (1ULL << (sqlParser::MONTHS - 130))
          | (1ULL << (sqlParser::MS - 130))
          | (1ULL << (sqlParser::NODES - 130))
          | (1ULL << (sqlParser::NODE - 130))
          | (1ULL << (sqlParser::NOOP - 130))
          | (1ULL << (sqlParser::NOP - 130))
          | (1ULL << (sqlParser::NOT - 130))
          | (1ULL << (sqlParser::NOW - 130))
          | (1ULL << (sqlParser::SQL_NULL - 130))
          | (1ULL << (sqlParser::NULLIF - 130))
          | (1ULL << (sqlParser::OBJECTS - 130))
          | (1ULL << (sqlParser::OFF - 130))
          | (1ULL << (sqlParser::OFFSET - 130))
          | (1ULL << (sqlParser::ORIGIN - 130))
          | (1ULL << (sqlParser::OVERLAY - 130))
          | (1ULL << (sqlParser::PARTITIONS - 130))
          | (1ULL << (sqlParser::PATHS - 130))
          | (1ULL << (sqlParser::PIPELINE - 130))
          | (1ULL << (sqlParser::PLACING - 130))
          | (1ULL << (sqlParser::POSITION - 130))
          | (1ULL << (sqlParser::PRETTY - 130))
          | (1ULL << (sqlParser::QUARTER - 130))
          | (1ULL << (sqlParser::QUARTERS - 130))
          | (1ULL << (sqlParser::RECORD_ENDS - 130))
          | (1ULL << (sqlParser::REFERENCE - 130))
          | (1ULL << (sqlParser::REGIONS - 130))
          | (1ULL << (sqlParser::REPLICATED - 130))
          | (1ULL << (sqlParser::ROWSIZE - 130))
          | (1ULL << (sqlParser::SCHEMA - 130))
          | (1ULL << (sqlParser::SEC - 130))
          | (1ULL << (sqlParser::SECONDS - 130)))) != 0) || ((((_la - 197) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 197)) & ((1ULL << (sqlParser::SKIPLIST - 197))
          | (1ULL << (sqlParser::SLEEP - 197))
          | (1ULL << (sqlParser::SPACE - 197))
          | (1ULL << (sqlParser::STAGE - 197))
          | (1ULL << (sqlParser::STATS - 197))
          | (1ULL << (sqlParser::STATISTICS - 197))
          | (1ULL << (sqlParser::STATUS - 197))
          | (1ULL << (sqlParser::SUBSTR - 197))
          | (1ULL << (sqlParser::SUBSTRING - 197))
          | (1ULL << (sqlParser::SUM - 197))
          | (1ULL << (sqlParser::SZIDX - 197))
          | (1ULL << (sqlParser::SZTBL - 197))
          | (1ULL << (sqlParser::S_ONLY - 197))
          | (1ULL << (sqlParser::TABLESIZE - 197))
          | (1ULL << (sqlParser::TEXT - 197))
          | (1ULL << (sqlParser::TIME - 197))
          | (1ULL << (sqlParser::TIMESTAMP - 197))
          | (1ULL << (sqlParser::TOP - 197))
          | (1ULL << (sqlParser::TRACKMEMORY - 197))
          | (1ULL << (sqlParser::TREE - 197))
          | (1ULL << (sqlParser::TRIM - 197))
          | (1ULL << (sqlParser::TRUNCATE - 197))
          | (1ULL << (sqlParser::SQL_TRUE - 197))
          | (1ULL << (sqlParser::TYPE - 197))
          | (1ULL << (sqlParser::TYPEOF - 197))
          | (1ULL << (sqlParser::UNLOAD - 197))
          | (1ULL << (sqlParser::VALUE - 197))
          | (1ULL << (sqlParser::WEEK - 197))
          | (1ULL << (sqlParser::WEEKS - 197))
          | (1ULL << (sqlParser::WITH_NAMES - 197))
          | (1ULL << (sqlParser::WITH_TYPES - 197))
          | (1ULL << (sqlParser::XOR - 197))
          | (1ULL << (sqlParser::YEARS - 197))
          | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 197))
          | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 197))
          | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 197))
          | (1ULL << (sqlParser::SINGLQUOTED_STRING - 197))
          | (1ULL << (sqlParser::DUBLQUOTED_STRING - 197))
          | (1ULL << (sqlParser::FLOAT_LITERAL - 197))
          | (1ULL << (sqlParser::DECIMAL_LITERAL - 197))
          | (1ULL << (sqlParser::INTEGER_LITERAL - 197)))) != 0)) {
          setState(1720);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        setState(1723);
        match(sqlParser::FROM);
        setState(1724);
        expr(0);
        break;
      }

      }
      setState(1727);
      match(sqlParser::T__7);
      break;
    }

    case 19: {
      _localctx = _tracker.createInstance<ExtractFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1729);
      match(sqlParser::EXTRACT);
      setState(1730);
      match(sqlParser::T__6);
      setState(1731);
      _la = _input->LA(1);
      if (!(_la == sqlParser::DAY

      || _la == sqlParser::HOUR || ((((_la - 146) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 146)) & ((1ULL << (sqlParser::MINUTE - 146))
        | (1ULL << (sqlParser::MONTH - 146))
        | (1ULL << (sqlParser::SECOND - 146)))) != 0) || _la == sqlParser::YEAR)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1732);
      match(sqlParser::FROM);
      setState(1733);
      expr(0);
      setState(1734);
      match(sqlParser::T__7);
      break;
    }

    case 20: {
      _localctx = _tracker.createInstance<CountAllContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1736);
      match(sqlParser::COUNT);
      setState(1737);
      match(sqlParser::T__6);
      setState(1739);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ALL) {
        setState(1738);
        match(sqlParser::ALL);
      }
      setState(1741);
      match(sqlParser::T__11);
      setState(1742);
      match(sqlParser::T__7);
      break;
    }

    case 21: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1743);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::COUNT);
      setState(1744);
      match(sqlParser::T__6);
      setState(1747);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1745);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1746);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__6:
        case sqlParser::T__10:
        case sqlParser::T__14:
        case sqlParser::ANALYSE:
        case sqlParser::ANALYZE:
        case sqlParser::ANIDX:
        case sqlParser::ANTBL:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::COALESCE:
        case sqlParser::COMPACTIDX:
        case sqlParser::CONCAT:
        case sqlParser::COUNT:
        case sqlParser::CURRENT_CATALOG:
        case sqlParser::CURRENT_SCHEMA:
        case sqlParser::CURRENT_TIMESTAMP:
        case sqlParser::CURRENT_USER:
        case sqlParser::DATA:
        case sqlParser::DATE:
        case sqlParser::DATE_FORMAT:
        case sqlParser::DAYS:
        case sqlParser::DECODE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DIGITS:
        case sqlParser::DLOAD:
        case sqlParser::DOCOL:
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXIT_TOKEN:
        case sqlParser::EXPLAIN:
        case sqlParser::EXPORT:
        case sqlParser::EXTRACT:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
        case sqlParser::FILE:
        case sqlParser::FORCE:
        case sqlParser::FORMAT:
        case sqlParser::HASH:
        case sqlParser::HASHKEY:
        case sqlParser::HOURS:
        case sqlParser::IF:
        case sqlParser::IFNULL:
        case sqlParser::INDEX:
        case sqlParser::INTERVAL_DT:
        case sqlParser::INTERVAL_YM:
        case sqlParser::INTERVAL:
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOADREPLICATED:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSPTNCOL:
        case sqlParser::LSTBL:
        case sqlParser::MAX:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MILLIS:
        case sqlParser::MILLISECOND:
        case sqlParser::MILLISECONDS:
        case sqlParser::MIN:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::MS:
        case sqlParser::NODES:
        case sqlParser::NODE:
        case sqlParser::NOOP:
        case sqlParser::NOP:
        case sqlParser::NOT:
        case sqlParser::NOW:
        case sqlParser::SQL_NULL:
        case sqlParser::NULLIF:
        case sqlParser::OBJECTS:
        case sqlParser::OFF:
        case sqlParser::OFFSET:
        case sqlParser::ORIGIN:
        case sqlParser::OVERLAY:
        case sqlParser::PARTITIONS:
        case sqlParser::PATHS:
        case sqlParser::PIPELINE:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::RECORD_ENDS:
        case sqlParser::REFERENCE:
        case sqlParser::REGIONS:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SEC:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
        case sqlParser::SLEEP:
        case sqlParser::SPACE:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::STATUS:
        case sqlParser::SUBSTR:
        case sqlParser::SUBSTRING:
        case sqlParser::SUM:
        case sqlParser::SZIDX:
        case sqlParser::SZTBL:
        case sqlParser::S_ONLY:
        case sqlParser::TABLESIZE:
        case sqlParser::TEXT:
        case sqlParser::TIME:
        case sqlParser::TIMESTAMP:
        case sqlParser::TOP:
        case sqlParser::TRACKMEMORY:
        case sqlParser::TREE:
        case sqlParser::TRIM:
        case sqlParser::TRUNCATE:
        case sqlParser::SQL_TRUE:
        case sqlParser::TYPE:
        case sqlParser::TYPEOF:
        case sqlParser::UNLOAD:
        case sqlParser::VALUE:
        case sqlParser::WEEK:
        case sqlParser::WEEKS:
        case sqlParser::WITH_NAMES:
        case sqlParser::WITH_TYPES:
        case sqlParser::XOR:
        case sqlParser::YEARS:
        case sqlParser::PLAIN_IDENTIFIER:
        case sqlParser::BAKTIKD_IDENTIFIER:
        case sqlParser::DUBLQUOTED_IDENTIFIER:
        case sqlParser::SINGLQUOTED_STRING:
        case sqlParser::DUBLQUOTED_STRING:
        case sqlParser::FLOAT_LITERAL:
        case sqlParser::DECIMAL_LITERAL:
        case sqlParser::INTEGER_LITERAL: {
          break;
        }

      default:
        break;
      }
      setState(1749);
      expr(0);
      setState(1750);
      match(sqlParser::T__7);
      break;
    }

    case 22: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1752);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::SUM);
      setState(1753);
      match(sqlParser::T__6);
      setState(1756);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1754);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1755);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__6:
        case sqlParser::T__10:
        case sqlParser::T__14:
        case sqlParser::ANALYSE:
        case sqlParser::ANALYZE:
        case sqlParser::ANIDX:
        case sqlParser::ANTBL:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::COALESCE:
        case sqlParser::COMPACTIDX:
        case sqlParser::CONCAT:
        case sqlParser::COUNT:
        case sqlParser::CURRENT_CATALOG:
        case sqlParser::CURRENT_SCHEMA:
        case sqlParser::CURRENT_TIMESTAMP:
        case sqlParser::CURRENT_USER:
        case sqlParser::DATA:
        case sqlParser::DATE:
        case sqlParser::DATE_FORMAT:
        case sqlParser::DAYS:
        case sqlParser::DECODE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DIGITS:
        case sqlParser::DLOAD:
        case sqlParser::DOCOL:
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXIT_TOKEN:
        case sqlParser::EXPLAIN:
        case sqlParser::EXPORT:
        case sqlParser::EXTRACT:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
        case sqlParser::FILE:
        case sqlParser::FORCE:
        case sqlParser::FORMAT:
        case sqlParser::HASH:
        case sqlParser::HASHKEY:
        case sqlParser::HOURS:
        case sqlParser::IF:
        case sqlParser::IFNULL:
        case sqlParser::INDEX:
        case sqlParser::INTERVAL_DT:
        case sqlParser::INTERVAL_YM:
        case sqlParser::INTERVAL:
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOADREPLICATED:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSPTNCOL:
        case sqlParser::LSTBL:
        case sqlParser::MAX:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MILLIS:
        case sqlParser::MILLISECOND:
        case sqlParser::MILLISECONDS:
        case sqlParser::MIN:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::MS:
        case sqlParser::NODES:
        case sqlParser::NODE:
        case sqlParser::NOOP:
        case sqlParser::NOP:
        case sqlParser::NOT:
        case sqlParser::NOW:
        case sqlParser::SQL_NULL:
        case sqlParser::NULLIF:
        case sqlParser::OBJECTS:
        case sqlParser::OFF:
        case sqlParser::OFFSET:
        case sqlParser::ORIGIN:
        case sqlParser::OVERLAY:
        case sqlParser::PARTITIONS:
        case sqlParser::PATHS:
        case sqlParser::PIPELINE:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::RECORD_ENDS:
        case sqlParser::REFERENCE:
        case sqlParser::REGIONS:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SEC:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
        case sqlParser::SLEEP:
        case sqlParser::SPACE:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::STATUS:
        case sqlParser::SUBSTR:
        case sqlParser::SUBSTRING:
        case sqlParser::SUM:
        case sqlParser::SZIDX:
        case sqlParser::SZTBL:
        case sqlParser::S_ONLY:
        case sqlParser::TABLESIZE:
        case sqlParser::TEXT:
        case sqlParser::TIME:
        case sqlParser::TIMESTAMP:
        case sqlParser::TOP:
        case sqlParser::TRACKMEMORY:
        case sqlParser::TREE:
        case sqlParser::TRIM:
        case sqlParser::TRUNCATE:
        case sqlParser::SQL_TRUE:
        case sqlParser::TYPE:
        case sqlParser::TYPEOF:
        case sqlParser::UNLOAD:
        case sqlParser::VALUE:
        case sqlParser::WEEK:
        case sqlParser::WEEKS:
        case sqlParser::WITH_NAMES:
        case sqlParser::WITH_TYPES:
        case sqlParser::XOR:
        case sqlParser::YEARS:
        case sqlParser::PLAIN_IDENTIFIER:
        case sqlParser::BAKTIKD_IDENTIFIER:
        case sqlParser::DUBLQUOTED_IDENTIFIER:
        case sqlParser::SINGLQUOTED_STRING:
        case sqlParser::DUBLQUOTED_STRING:
        case sqlParser::FLOAT_LITERAL:
        case sqlParser::DECIMAL_LITERAL:
        case sqlParser::INTEGER_LITERAL: {
          break;
        }

      default:
        break;
      }
      setState(1758);
      expr(0);
      setState(1759);
      match(sqlParser::T__7);
      break;
    }

    case 23: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1761);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::AVG);
      setState(1762);
      match(sqlParser::T__6);
      setState(1765);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1763);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1764);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__6:
        case sqlParser::T__10:
        case sqlParser::T__14:
        case sqlParser::ANALYSE:
        case sqlParser::ANALYZE:
        case sqlParser::ANIDX:
        case sqlParser::ANTBL:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::COALESCE:
        case sqlParser::COMPACTIDX:
        case sqlParser::CONCAT:
        case sqlParser::COUNT:
        case sqlParser::CURRENT_CATALOG:
        case sqlParser::CURRENT_SCHEMA:
        case sqlParser::CURRENT_TIMESTAMP:
        case sqlParser::CURRENT_USER:
        case sqlParser::DATA:
        case sqlParser::DATE:
        case sqlParser::DATE_FORMAT:
        case sqlParser::DAYS:
        case sqlParser::DECODE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DIGITS:
        case sqlParser::DLOAD:
        case sqlParser::DOCOL:
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXIT_TOKEN:
        case sqlParser::EXPLAIN:
        case sqlParser::EXPORT:
        case sqlParser::EXTRACT:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
        case sqlParser::FILE:
        case sqlParser::FORCE:
        case sqlParser::FORMAT:
        case sqlParser::HASH:
        case sqlParser::HASHKEY:
        case sqlParser::HOURS:
        case sqlParser::IF:
        case sqlParser::IFNULL:
        case sqlParser::INDEX:
        case sqlParser::INTERVAL_DT:
        case sqlParser::INTERVAL_YM:
        case sqlParser::INTERVAL:
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOADREPLICATED:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSPTNCOL:
        case sqlParser::LSTBL:
        case sqlParser::MAX:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MILLIS:
        case sqlParser::MILLISECOND:
        case sqlParser::MILLISECONDS:
        case sqlParser::MIN:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::MS:
        case sqlParser::NODES:
        case sqlParser::NODE:
        case sqlParser::NOOP:
        case sqlParser::NOP:
        case sqlParser::NOT:
        case sqlParser::NOW:
        case sqlParser::SQL_NULL:
        case sqlParser::NULLIF:
        case sqlParser::OBJECTS:
        case sqlParser::OFF:
        case sqlParser::OFFSET:
        case sqlParser::ORIGIN:
        case sqlParser::OVERLAY:
        case sqlParser::PARTITIONS:
        case sqlParser::PATHS:
        case sqlParser::PIPELINE:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::RECORD_ENDS:
        case sqlParser::REFERENCE:
        case sqlParser::REGIONS:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SEC:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
        case sqlParser::SLEEP:
        case sqlParser::SPACE:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::STATUS:
        case sqlParser::SUBSTR:
        case sqlParser::SUBSTRING:
        case sqlParser::SUM:
        case sqlParser::SZIDX:
        case sqlParser::SZTBL:
        case sqlParser::S_ONLY:
        case sqlParser::TABLESIZE:
        case sqlParser::TEXT:
        case sqlParser::TIME:
        case sqlParser::TIMESTAMP:
        case sqlParser::TOP:
        case sqlParser::TRACKMEMORY:
        case sqlParser::TREE:
        case sqlParser::TRIM:
        case sqlParser::TRUNCATE:
        case sqlParser::SQL_TRUE:
        case sqlParser::TYPE:
        case sqlParser::TYPEOF:
        case sqlParser::UNLOAD:
        case sqlParser::VALUE:
        case sqlParser::WEEK:
        case sqlParser::WEEKS:
        case sqlParser::WITH_NAMES:
        case sqlParser::WITH_TYPES:
        case sqlParser::XOR:
        case sqlParser::YEARS:
        case sqlParser::PLAIN_IDENTIFIER:
        case sqlParser::BAKTIKD_IDENTIFIER:
        case sqlParser::DUBLQUOTED_IDENTIFIER:
        case sqlParser::SINGLQUOTED_STRING:
        case sqlParser::DUBLQUOTED_STRING:
        case sqlParser::FLOAT_LITERAL:
        case sqlParser::DECIMAL_LITERAL:
        case sqlParser::INTEGER_LITERAL: {
          break;
        }

      default:
        break;
      }
      setState(1767);
      expr(0);
      setState(1768);
      match(sqlParser::T__7);
      break;
    }

    case 24: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1770);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MIN);
      setState(1771);
      match(sqlParser::T__6);
      setState(1774);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1772);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1773);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__6:
        case sqlParser::T__10:
        case sqlParser::T__14:
        case sqlParser::ANALYSE:
        case sqlParser::ANALYZE:
        case sqlParser::ANIDX:
        case sqlParser::ANTBL:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::COALESCE:
        case sqlParser::COMPACTIDX:
        case sqlParser::CONCAT:
        case sqlParser::COUNT:
        case sqlParser::CURRENT_CATALOG:
        case sqlParser::CURRENT_SCHEMA:
        case sqlParser::CURRENT_TIMESTAMP:
        case sqlParser::CURRENT_USER:
        case sqlParser::DATA:
        case sqlParser::DATE:
        case sqlParser::DATE_FORMAT:
        case sqlParser::DAYS:
        case sqlParser::DECODE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DIGITS:
        case sqlParser::DLOAD:
        case sqlParser::DOCOL:
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXIT_TOKEN:
        case sqlParser::EXPLAIN:
        case sqlParser::EXPORT:
        case sqlParser::EXTRACT:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
        case sqlParser::FILE:
        case sqlParser::FORCE:
        case sqlParser::FORMAT:
        case sqlParser::HASH:
        case sqlParser::HASHKEY:
        case sqlParser::HOURS:
        case sqlParser::IF:
        case sqlParser::IFNULL:
        case sqlParser::INDEX:
        case sqlParser::INTERVAL_DT:
        case sqlParser::INTERVAL_YM:
        case sqlParser::INTERVAL:
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOADREPLICATED:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSPTNCOL:
        case sqlParser::LSTBL:
        case sqlParser::MAX:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MILLIS:
        case sqlParser::MILLISECOND:
        case sqlParser::MILLISECONDS:
        case sqlParser::MIN:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::MS:
        case sqlParser::NODES:
        case sqlParser::NODE:
        case sqlParser::NOOP:
        case sqlParser::NOP:
        case sqlParser::NOT:
        case sqlParser::NOW:
        case sqlParser::SQL_NULL:
        case sqlParser::NULLIF:
        case sqlParser::OBJECTS:
        case sqlParser::OFF:
        case sqlParser::OFFSET:
        case sqlParser::ORIGIN:
        case sqlParser::OVERLAY:
        case sqlParser::PARTITIONS:
        case sqlParser::PATHS:
        case sqlParser::PIPELINE:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::RECORD_ENDS:
        case sqlParser::REFERENCE:
        case sqlParser::REGIONS:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SEC:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
        case sqlParser::SLEEP:
        case sqlParser::SPACE:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::STATUS:
        case sqlParser::SUBSTR:
        case sqlParser::SUBSTRING:
        case sqlParser::SUM:
        case sqlParser::SZIDX:
        case sqlParser::SZTBL:
        case sqlParser::S_ONLY:
        case sqlParser::TABLESIZE:
        case sqlParser::TEXT:
        case sqlParser::TIME:
        case sqlParser::TIMESTAMP:
        case sqlParser::TOP:
        case sqlParser::TRACKMEMORY:
        case sqlParser::TREE:
        case sqlParser::TRIM:
        case sqlParser::TRUNCATE:
        case sqlParser::SQL_TRUE:
        case sqlParser::TYPE:
        case sqlParser::TYPEOF:
        case sqlParser::UNLOAD:
        case sqlParser::VALUE:
        case sqlParser::WEEK:
        case sqlParser::WEEKS:
        case sqlParser::WITH_NAMES:
        case sqlParser::WITH_TYPES:
        case sqlParser::XOR:
        case sqlParser::YEARS:
        case sqlParser::PLAIN_IDENTIFIER:
        case sqlParser::BAKTIKD_IDENTIFIER:
        case sqlParser::DUBLQUOTED_IDENTIFIER:
        case sqlParser::SINGLQUOTED_STRING:
        case sqlParser::DUBLQUOTED_STRING:
        case sqlParser::FLOAT_LITERAL:
        case sqlParser::DECIMAL_LITERAL:
        case sqlParser::INTEGER_LITERAL: {
          break;
        }

      default:
        break;
      }
      setState(1776);
      expr(0);
      setState(1777);
      match(sqlParser::T__7);
      break;
    }

    case 25: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1779);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MAX);
      setState(1780);
      match(sqlParser::T__6);
      setState(1783);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1781);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1782);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__6:
        case sqlParser::T__10:
        case sqlParser::T__14:
        case sqlParser::ANALYSE:
        case sqlParser::ANALYZE:
        case sqlParser::ANIDX:
        case sqlParser::ANTBL:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::COALESCE:
        case sqlParser::COMPACTIDX:
        case sqlParser::CONCAT:
        case sqlParser::COUNT:
        case sqlParser::CURRENT_CATALOG:
        case sqlParser::CURRENT_SCHEMA:
        case sqlParser::CURRENT_TIMESTAMP:
        case sqlParser::CURRENT_USER:
        case sqlParser::DATA:
        case sqlParser::DATE:
        case sqlParser::DATE_FORMAT:
        case sqlParser::DAYS:
        case sqlParser::DECODE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DIGITS:
        case sqlParser::DLOAD:
        case sqlParser::DOCOL:
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXIT_TOKEN:
        case sqlParser::EXPLAIN:
        case sqlParser::EXPORT:
        case sqlParser::EXTRACT:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
        case sqlParser::FILE:
        case sqlParser::FORCE:
        case sqlParser::FORMAT:
        case sqlParser::HASH:
        case sqlParser::HASHKEY:
        case sqlParser::HOURS:
        case sqlParser::IF:
        case sqlParser::IFNULL:
        case sqlParser::INDEX:
        case sqlParser::INTERVAL_DT:
        case sqlParser::INTERVAL_YM:
        case sqlParser::INTERVAL:
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOADREPLICATED:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSPTNCOL:
        case sqlParser::LSTBL:
        case sqlParser::MAX:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MILLIS:
        case sqlParser::MILLISECOND:
        case sqlParser::MILLISECONDS:
        case sqlParser::MIN:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::MS:
        case sqlParser::NODES:
        case sqlParser::NODE:
        case sqlParser::NOOP:
        case sqlParser::NOP:
        case sqlParser::NOT:
        case sqlParser::NOW:
        case sqlParser::SQL_NULL:
        case sqlParser::NULLIF:
        case sqlParser::OBJECTS:
        case sqlParser::OFF:
        case sqlParser::OFFSET:
        case sqlParser::ORIGIN:
        case sqlParser::OVERLAY:
        case sqlParser::PARTITIONS:
        case sqlParser::PATHS:
        case sqlParser::PIPELINE:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::RECORD_ENDS:
        case sqlParser::REFERENCE:
        case sqlParser::REGIONS:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SEC:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
        case sqlParser::SLEEP:
        case sqlParser::SPACE:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::STATUS:
        case sqlParser::SUBSTR:
        case sqlParser::SUBSTRING:
        case sqlParser::SUM:
        case sqlParser::SZIDX:
        case sqlParser::SZTBL:
        case sqlParser::S_ONLY:
        case sqlParser::TABLESIZE:
        case sqlParser::TEXT:
        case sqlParser::TIME:
        case sqlParser::TIMESTAMP:
        case sqlParser::TOP:
        case sqlParser::TRACKMEMORY:
        case sqlParser::TREE:
        case sqlParser::TRIM:
        case sqlParser::TRUNCATE:
        case sqlParser::SQL_TRUE:
        case sqlParser::TYPE:
        case sqlParser::TYPEOF:
        case sqlParser::UNLOAD:
        case sqlParser::VALUE:
        case sqlParser::WEEK:
        case sqlParser::WEEKS:
        case sqlParser::WITH_NAMES:
        case sqlParser::WITH_TYPES:
        case sqlParser::XOR:
        case sqlParser::YEARS:
        case sqlParser::PLAIN_IDENTIFIER:
        case sqlParser::BAKTIKD_IDENTIFIER:
        case sqlParser::DUBLQUOTED_IDENTIFIER:
        case sqlParser::SINGLQUOTED_STRING:
        case sqlParser::DUBLQUOTED_STRING:
        case sqlParser::FLOAT_LITERAL:
        case sqlParser::DECIMAL_LITERAL:
        case sqlParser::INTEGER_LITERAL: {
          break;
        }

      default:
        break;
      }
      setState(1785);
      expr(0);
      setState(1786);
      match(sqlParser::T__7);
      break;
    }

    case 26: {
      _localctx = _tracker.createInstance<ScalarFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1788);
      dynamic_cast<ScalarFuncContext *>(_localctx)->func = functionName();
      setState(1789);
      match(sqlParser::T__6);
      setState(1798);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__6)
        | (1ULL << sqlParser::T__10)
        | (1ULL << sqlParser::T__14)
        | (1ULL << sqlParser::ANALYSE)
        | (1ULL << sqlParser::ANALYZE)
        | (1ULL << sqlParser::ANIDX)
        | (1ULL << sqlParser::ANTBL)
        | (1ULL << sqlParser::ASC)
        | (1ULL << sqlParser::AVG)
        | (1ULL << sqlParser::BIT)
        | (1ULL << sqlParser::BITS)
        | (1ULL << sqlParser::BLOCK)
        | (1ULL << sqlParser::BLOOM)
        | (1ULL << sqlParser::BPA)
        | (1ULL << sqlParser::BTREE)
        | (1ULL << sqlParser::BYE)
        | (1ULL << sqlParser::BYTE)
        | (1ULL << sqlParser::BYTES)
        | (1ULL << sqlParser::CASE)
        | (1ULL << sqlParser::CAST)
        | (1ULL << sqlParser::CATALOG)
        | (1ULL << sqlParser::CHARS)
        | (1ULL << sqlParser::COALESCE)
        | (1ULL << sqlParser::COMPACTIDX)
        | (1ULL << sqlParser::CONCAT)
        | (1ULL << sqlParser::COUNT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::CURRENT_CATALOG - 64))
        | (1ULL << (sqlParser::CURRENT_SCHEMA - 64))
        | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 64))
        | (1ULL << (sqlParser::CURRENT_USER - 64))
        | (1ULL << (sqlParser::DATA - 64))
        | (1ULL << (sqlParser::DATE - 64))
        | (1ULL << (sqlParser::DATE_FORMAT - 64))
        | (1ULL << (sqlParser::DAYS - 64))
        | (1ULL << (sqlParser::DECODE - 64))
        | (1ULL << (sqlParser::DESC - 64))
        | (1ULL << (sqlParser::DETAIL - 64))
        | (1ULL << (sqlParser::DIGITS - 64))
        | (1ULL << (sqlParser::DLOAD - 64))
        | (1ULL << (sqlParser::DOCOL - 64))
        | (1ULL << (sqlParser::ECHO - 64))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
        | (1ULL << (sqlParser::EXISTS - 64))
        | (1ULL << (sqlParser::EXIT_TOKEN - 64))
        | (1ULL << (sqlParser::EXPLAIN - 64))
        | (1ULL << (sqlParser::EXPORT - 64))
        | (1ULL << (sqlParser::EXTRACT - 64))
        | (1ULL << (sqlParser::SQL_FALSE - 64))
        | (1ULL << (sqlParser::FEDERATION - 64))
        | (1ULL << (sqlParser::FIELD_DELIMITER - 64))
        | (1ULL << (sqlParser::FILE - 64))
        | (1ULL << (sqlParser::FORCE - 64))
        | (1ULL << (sqlParser::FORMAT - 64))
        | (1ULL << (sqlParser::HASH - 64))
        | (1ULL << (sqlParser::HASHKEY - 64))
        | (1ULL << (sqlParser::HOURS - 64))
        | (1ULL << (sqlParser::IF - 64))
        | (1ULL << (sqlParser::IFNULL - 64))
        | (1ULL << (sqlParser::INDEX - 64))
        | (1ULL << (sqlParser::INTERVAL_DT - 64))
        | (1ULL << (sqlParser::INTERVAL_YM - 64))
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::KEY - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::LIMIT - 130))
        | (1ULL << (sqlParser::LOADREPLICATED - 130))
        | (1ULL << (sqlParser::LOAD - 130))
        | (1ULL << (sqlParser::LOG - 130))
        | (1ULL << (sqlParser::LOOP - 130))
        | (1ULL << (sqlParser::LSIDX - 130))
        | (1ULL << (sqlParser::LSPTNCOL - 130))
        | (1ULL << (sqlParser::LSTBL - 130))
        | (1ULL << (sqlParser::MAX - 130))
        | (1ULL << (sqlParser::MEMORYLEAKS - 130))
        | (1ULL << (sqlParser::MILLIS - 130))
        | (1ULL << (sqlParser::MILLISECOND - 130))
        | (1ULL << (sqlParser::MILLISECONDS - 130))
        | (1ULL << (sqlParser::MIN - 130))
        | (1ULL << (sqlParser::MINUTES - 130))
        | (1ULL << (sqlParser::MONOTONIC - 130))
        | (1ULL << (sqlParser::MONTHS - 130))
        | (1ULL << (sqlParser::MS - 130))
        | (1ULL << (sqlParser::NODES - 130))
        | (1ULL << (sqlParser::NODE - 130))
        | (1ULL << (sqlParser::NOOP - 130))
        | (1ULL << (sqlParser::NOP - 130))
        | (1ULL << (sqlParser::NOT - 130))
        | (1ULL << (sqlParser::NOW - 130))
        | (1ULL << (sqlParser::SQL_NULL - 130))
        | (1ULL << (sqlParser::NULLIF - 130))
        | (1ULL << (sqlParser::OBJECTS - 130))
        | (1ULL << (sqlParser::OFF - 130))
        | (1ULL << (sqlParser::OFFSET - 130))
        | (1ULL << (sqlParser::ORIGIN - 130))
        | (1ULL << (sqlParser::OVERLAY - 130))
        | (1ULL << (sqlParser::PARTITIONS - 130))
        | (1ULL << (sqlParser::PATHS - 130))
        | (1ULL << (sqlParser::PIPELINE - 130))
        | (1ULL << (sqlParser::PLACING - 130))
        | (1ULL << (sqlParser::POSITION - 130))
        | (1ULL << (sqlParser::PRETTY - 130))
        | (1ULL << (sqlParser::QUARTER - 130))
        | (1ULL << (sqlParser::QUARTERS - 130))
        | (1ULL << (sqlParser::RECORD_ENDS - 130))
        | (1ULL << (sqlParser::REFERENCE - 130))
        | (1ULL << (sqlParser::REGIONS - 130))
        | (1ULL << (sqlParser::REPLICATED - 130))
        | (1ULL << (sqlParser::ROWSIZE - 130))
        | (1ULL << (sqlParser::SCHEMA - 130))
        | (1ULL << (sqlParser::SEC - 130))
        | (1ULL << (sqlParser::SECONDS - 130)))) != 0) || ((((_la - 197) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 197)) & ((1ULL << (sqlParser::SKIPLIST - 197))
        | (1ULL << (sqlParser::SLEEP - 197))
        | (1ULL << (sqlParser::SPACE - 197))
        | (1ULL << (sqlParser::STAGE - 197))
        | (1ULL << (sqlParser::STATS - 197))
        | (1ULL << (sqlParser::STATISTICS - 197))
        | (1ULL << (sqlParser::STATUS - 197))
        | (1ULL << (sqlParser::SUBSTR - 197))
        | (1ULL << (sqlParser::SUBSTRING - 197))
        | (1ULL << (sqlParser::SUM - 197))
        | (1ULL << (sqlParser::SZIDX - 197))
        | (1ULL << (sqlParser::SZTBL - 197))
        | (1ULL << (sqlParser::S_ONLY - 197))
        | (1ULL << (sqlParser::TABLESIZE - 197))
        | (1ULL << (sqlParser::TEXT - 197))
        | (1ULL << (sqlParser::TIME - 197))
        | (1ULL << (sqlParser::TIMESTAMP - 197))
        | (1ULL << (sqlParser::TOP - 197))
        | (1ULL << (sqlParser::TRACKMEMORY - 197))
        | (1ULL << (sqlParser::TREE - 197))
        | (1ULL << (sqlParser::TRIM - 197))
        | (1ULL << (sqlParser::TRUNCATE - 197))
        | (1ULL << (sqlParser::SQL_TRUE - 197))
        | (1ULL << (sqlParser::TYPE - 197))
        | (1ULL << (sqlParser::TYPEOF - 197))
        | (1ULL << (sqlParser::UNLOAD - 197))
        | (1ULL << (sqlParser::VALUE - 197))
        | (1ULL << (sqlParser::WEEK - 197))
        | (1ULL << (sqlParser::WEEKS - 197))
        | (1ULL << (sqlParser::WITH_NAMES - 197))
        | (1ULL << (sqlParser::WITH_TYPES - 197))
        | (1ULL << (sqlParser::XOR - 197))
        | (1ULL << (sqlParser::YEARS - 197))
        | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 197))
        | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 197))
        | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 197))
        | (1ULL << (sqlParser::SINGLQUOTED_STRING - 197))
        | (1ULL << (sqlParser::DUBLQUOTED_STRING - 197))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 197))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 197))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 197)))) != 0)) {
        setState(1790);
        expr(0);
        setState(1795);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1791);
          match(sqlParser::T__5);
          setState(1792);
          expr(0);
          setState(1797);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1800);
      match(sqlParser::T__7);
      break;
    }

    case 27: {
      _localctx = _tracker.createInstance<ParentheticContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1802);
      match(sqlParser::T__6);
      setState(1803);
      expr(0);
      setState(1804);
      match(sqlParser::T__7);
      break;
    }

    case 28: {
      _localctx = _tracker.createInstance<SubqueryExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1806);
      match(sqlParser::T__6);
      setState(1807);
      query();
      setState(1808);
      match(sqlParser::T__7);
      break;
    }

    case 29: {
      _localctx = _tracker.createInstance<SubqueryExistsOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1811);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::NOT) {
        setState(1810);
        match(sqlParser::NOT);
      }
      setState(1813);
      match(sqlParser::EXISTS);

      setState(1814);
      match(sqlParser::T__6);
      setState(1815);
      query();
      setState(1816);
      match(sqlParser::T__7);
      break;
    }

    case 30: {
      _localctx = _tracker.createInstance<LogicOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1818);
      dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::NOT);
      setState(1819);
      expr(4);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(1915);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 226, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(1913);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 225, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1822);

          if (!(precpred(_ctx, 37))) throw FailedPredicateException(this, "precpred(_ctx, 37)");
          setState(1823);
          dynamic_cast<BinaryOpContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__4)
            | (1ULL << sqlParser::T__11)
            | (1ULL << sqlParser::T__15))) != 0))) {
            dynamic_cast<BinaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1824);
          expr(38);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1825);

          if (!(precpred(_ctx, 36))) throw FailedPredicateException(this, "precpred(_ctx, 36)");
          setState(1826);
          dynamic_cast<BinaryOpContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__10)
            | (1ULL << sqlParser::T__14)
            | (1ULL << sqlParser::T__17))) != 0))) {
            dynamic_cast<BinaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1827);
          expr(37);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1828);

          if (!(precpred(_ctx, 35))) throw FailedPredicateException(this, "precpred(_ctx, 35)");
          setState(1829);
          dynamic_cast<BinaryOpContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__1)
            | (1ULL << sqlParser::T__2)
            | (1ULL << sqlParser::T__3)
            | (1ULL << sqlParser::T__18)
            | (1ULL << sqlParser::T__19)
            | (1ULL << sqlParser::T__20)
            | (1ULL << sqlParser::T__21))) != 0))) {
            dynamic_cast<BinaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1830);
          expr(36);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<LikeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1831);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(1833);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1832);
            match(sqlParser::NOT);
          }
          setState(1835);
          match(sqlParser::LIKE);
          setState(1836);
          expr(11);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<LikeEscapeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1837);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(1839);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1838);
            match(sqlParser::NOT);
          }
          setState(1841);
          match(sqlParser::LIKE);
          setState(1842);
          expr(0);
          setState(1843);
          match(sqlParser::ESCAPE);
          setState(1844);
          expr(10);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1846);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(1847);

          if (!(!(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent)))) throw FailedPredicateException(this, "!(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent))");
          setState(1848);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::AND);
          setState(1849);
          expr(4);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1850);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(1851);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::XOR);
          setState(1852);
          expr(3);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1853);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(1854);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::OR);
          setState(1855);
          expr(2);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<MethodCallContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1856);

          if (!(precpred(_ctx, 38))) throw FailedPredicateException(this, "precpred(_ctx, 38)");
          setState(1857);
          match(sqlParser::T__9);
          setState(1858);
          dynamic_cast<MethodCallContext *>(_localctx)->func = methodName();
          setState(1859);
          match(sqlParser::T__6);
          setState(1868);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__6)
            | (1ULL << sqlParser::T__10)
            | (1ULL << sqlParser::T__14)
            | (1ULL << sqlParser::ANALYSE)
            | (1ULL << sqlParser::ANALYZE)
            | (1ULL << sqlParser::ANIDX)
            | (1ULL << sqlParser::ANTBL)
            | (1ULL << sqlParser::ASC)
            | (1ULL << sqlParser::AVG)
            | (1ULL << sqlParser::BIT)
            | (1ULL << sqlParser::BITS)
            | (1ULL << sqlParser::BLOCK)
            | (1ULL << sqlParser::BLOOM)
            | (1ULL << sqlParser::BPA)
            | (1ULL << sqlParser::BTREE)
            | (1ULL << sqlParser::BYE)
            | (1ULL << sqlParser::BYTE)
            | (1ULL << sqlParser::BYTES)
            | (1ULL << sqlParser::CASE)
            | (1ULL << sqlParser::CAST)
            | (1ULL << sqlParser::CATALOG)
            | (1ULL << sqlParser::CHARS)
            | (1ULL << sqlParser::COALESCE)
            | (1ULL << sqlParser::COMPACTIDX)
            | (1ULL << sqlParser::CONCAT)
            | (1ULL << sqlParser::COUNT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::CURRENT_CATALOG - 64))
            | (1ULL << (sqlParser::CURRENT_SCHEMA - 64))
            | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 64))
            | (1ULL << (sqlParser::CURRENT_USER - 64))
            | (1ULL << (sqlParser::DATA - 64))
            | (1ULL << (sqlParser::DATE - 64))
            | (1ULL << (sqlParser::DATE_FORMAT - 64))
            | (1ULL << (sqlParser::DAYS - 64))
            | (1ULL << (sqlParser::DECODE - 64))
            | (1ULL << (sqlParser::DESC - 64))
            | (1ULL << (sqlParser::DETAIL - 64))
            | (1ULL << (sqlParser::DIGITS - 64))
            | (1ULL << (sqlParser::DLOAD - 64))
            | (1ULL << (sqlParser::DOCOL - 64))
            | (1ULL << (sqlParser::ECHO - 64))
            | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
            | (1ULL << (sqlParser::EXISTS - 64))
            | (1ULL << (sqlParser::EXIT_TOKEN - 64))
            | (1ULL << (sqlParser::EXPLAIN - 64))
            | (1ULL << (sqlParser::EXPORT - 64))
            | (1ULL << (sqlParser::EXTRACT - 64))
            | (1ULL << (sqlParser::SQL_FALSE - 64))
            | (1ULL << (sqlParser::FEDERATION - 64))
            | (1ULL << (sqlParser::FIELD_DELIMITER - 64))
            | (1ULL << (sqlParser::FILE - 64))
            | (1ULL << (sqlParser::FORCE - 64))
            | (1ULL << (sqlParser::FORMAT - 64))
            | (1ULL << (sqlParser::HASH - 64))
            | (1ULL << (sqlParser::HASHKEY - 64))
            | (1ULL << (sqlParser::HOURS - 64))
            | (1ULL << (sqlParser::IF - 64))
            | (1ULL << (sqlParser::IFNULL - 64))
            | (1ULL << (sqlParser::INDEX - 64))
            | (1ULL << (sqlParser::INTERVAL_DT - 64))
            | (1ULL << (sqlParser::INTERVAL_YM - 64))
            | (1ULL << (sqlParser::INTERVAL - 64))
            | (1ULL << (sqlParser::KEY - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::LIMIT - 130))
            | (1ULL << (sqlParser::LOADREPLICATED - 130))
            | (1ULL << (sqlParser::LOAD - 130))
            | (1ULL << (sqlParser::LOG - 130))
            | (1ULL << (sqlParser::LOOP - 130))
            | (1ULL << (sqlParser::LSIDX - 130))
            | (1ULL << (sqlParser::LSPTNCOL - 130))
            | (1ULL << (sqlParser::LSTBL - 130))
            | (1ULL << (sqlParser::MAX - 130))
            | (1ULL << (sqlParser::MEMORYLEAKS - 130))
            | (1ULL << (sqlParser::MILLIS - 130))
            | (1ULL << (sqlParser::MILLISECOND - 130))
            | (1ULL << (sqlParser::MILLISECONDS - 130))
            | (1ULL << (sqlParser::MIN - 130))
            | (1ULL << (sqlParser::MINUTES - 130))
            | (1ULL << (sqlParser::MONOTONIC - 130))
            | (1ULL << (sqlParser::MONTHS - 130))
            | (1ULL << (sqlParser::MS - 130))
            | (1ULL << (sqlParser::NODES - 130))
            | (1ULL << (sqlParser::NODE - 130))
            | (1ULL << (sqlParser::NOOP - 130))
            | (1ULL << (sqlParser::NOP - 130))
            | (1ULL << (sqlParser::NOT - 130))
            | (1ULL << (sqlParser::NOW - 130))
            | (1ULL << (sqlParser::SQL_NULL - 130))
            | (1ULL << (sqlParser::NULLIF - 130))
            | (1ULL << (sqlParser::OBJECTS - 130))
            | (1ULL << (sqlParser::OFF - 130))
            | (1ULL << (sqlParser::OFFSET - 130))
            | (1ULL << (sqlParser::ORIGIN - 130))
            | (1ULL << (sqlParser::OVERLAY - 130))
            | (1ULL << (sqlParser::PARTITIONS - 130))
            | (1ULL << (sqlParser::PATHS - 130))
            | (1ULL << (sqlParser::PIPELINE - 130))
            | (1ULL << (sqlParser::PLACING - 130))
            | (1ULL << (sqlParser::POSITION - 130))
            | (1ULL << (sqlParser::PRETTY - 130))
            | (1ULL << (sqlParser::QUARTER - 130))
            | (1ULL << (sqlParser::QUARTERS - 130))
            | (1ULL << (sqlParser::RECORD_ENDS - 130))
            | (1ULL << (sqlParser::REFERENCE - 130))
            | (1ULL << (sqlParser::REGIONS - 130))
            | (1ULL << (sqlParser::REPLICATED - 130))
            | (1ULL << (sqlParser::ROWSIZE - 130))
            | (1ULL << (sqlParser::SCHEMA - 130))
            | (1ULL << (sqlParser::SEC - 130))
            | (1ULL << (sqlParser::SECONDS - 130)))) != 0) || ((((_la - 197) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 197)) & ((1ULL << (sqlParser::SKIPLIST - 197))
            | (1ULL << (sqlParser::SLEEP - 197))
            | (1ULL << (sqlParser::SPACE - 197))
            | (1ULL << (sqlParser::STAGE - 197))
            | (1ULL << (sqlParser::STATS - 197))
            | (1ULL << (sqlParser::STATISTICS - 197))
            | (1ULL << (sqlParser::STATUS - 197))
            | (1ULL << (sqlParser::SUBSTR - 197))
            | (1ULL << (sqlParser::SUBSTRING - 197))
            | (1ULL << (sqlParser::SUM - 197))
            | (1ULL << (sqlParser::SZIDX - 197))
            | (1ULL << (sqlParser::SZTBL - 197))
            | (1ULL << (sqlParser::S_ONLY - 197))
            | (1ULL << (sqlParser::TABLESIZE - 197))
            | (1ULL << (sqlParser::TEXT - 197))
            | (1ULL << (sqlParser::TIME - 197))
            | (1ULL << (sqlParser::TIMESTAMP - 197))
            | (1ULL << (sqlParser::TOP - 197))
            | (1ULL << (sqlParser::TRACKMEMORY - 197))
            | (1ULL << (sqlParser::TREE - 197))
            | (1ULL << (sqlParser::TRIM - 197))
            | (1ULL << (sqlParser::TRUNCATE - 197))
            | (1ULL << (sqlParser::SQL_TRUE - 197))
            | (1ULL << (sqlParser::TYPE - 197))
            | (1ULL << (sqlParser::TYPEOF - 197))
            | (1ULL << (sqlParser::UNLOAD - 197))
            | (1ULL << (sqlParser::VALUE - 197))
            | (1ULL << (sqlParser::WEEK - 197))
            | (1ULL << (sqlParser::WEEKS - 197))
            | (1ULL << (sqlParser::WITH_NAMES - 197))
            | (1ULL << (sqlParser::WITH_TYPES - 197))
            | (1ULL << (sqlParser::XOR - 197))
            | (1ULL << (sqlParser::YEARS - 197))
            | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 197))
            | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 197))
            | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 197))
            | (1ULL << (sqlParser::SINGLQUOTED_STRING - 197))
            | (1ULL << (sqlParser::DUBLQUOTED_STRING - 197))
            | (1ULL << (sqlParser::FLOAT_LITERAL - 197))
            | (1ULL << (sqlParser::DECIMAL_LITERAL - 197))
            | (1ULL << (sqlParser::INTEGER_LITERAL - 197)))) != 0)) {
            setState(1860);
            expr(0);
            setState(1865);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__5) {
              setState(1861);
              match(sqlParser::T__5);
              setState(1862);
              expr(0);
              setState(1867);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1870);
          match(sqlParser::T__7);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<IsNullOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1872);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(1873);
          match(sqlParser::IS);
          setState(1875);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1874);
            match(sqlParser::NOT);
          }
          setState(1877);
          match(sqlParser::SQL_NULL);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<SubqueryInOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1878);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(1880);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1879);
            match(sqlParser::NOT);
          }
          setState(1882);
          match(sqlParser::IN);

          setState(1883);
          match(sqlParser::T__6);
          setState(1884);
          query();
          setState(1885);
          match(sqlParser::T__7);
          break;
        }

        case 12: {
          auto newContext = _tracker.createInstance<InOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1887);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(1889);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1888);
            match(sqlParser::NOT);
          }
          setState(1891);
          match(sqlParser::IN);

          setState(1892);
          match(sqlParser::T__6);
          setState(1901);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__6)
            | (1ULL << sqlParser::T__10)
            | (1ULL << sqlParser::T__14)
            | (1ULL << sqlParser::ANALYSE)
            | (1ULL << sqlParser::ANALYZE)
            | (1ULL << sqlParser::ANIDX)
            | (1ULL << sqlParser::ANTBL)
            | (1ULL << sqlParser::ASC)
            | (1ULL << sqlParser::AVG)
            | (1ULL << sqlParser::BIT)
            | (1ULL << sqlParser::BITS)
            | (1ULL << sqlParser::BLOCK)
            | (1ULL << sqlParser::BLOOM)
            | (1ULL << sqlParser::BPA)
            | (1ULL << sqlParser::BTREE)
            | (1ULL << sqlParser::BYE)
            | (1ULL << sqlParser::BYTE)
            | (1ULL << sqlParser::BYTES)
            | (1ULL << sqlParser::CASE)
            | (1ULL << sqlParser::CAST)
            | (1ULL << sqlParser::CATALOG)
            | (1ULL << sqlParser::CHARS)
            | (1ULL << sqlParser::COALESCE)
            | (1ULL << sqlParser::COMPACTIDX)
            | (1ULL << sqlParser::CONCAT)
            | (1ULL << sqlParser::COUNT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::CURRENT_CATALOG - 64))
            | (1ULL << (sqlParser::CURRENT_SCHEMA - 64))
            | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 64))
            | (1ULL << (sqlParser::CURRENT_USER - 64))
            | (1ULL << (sqlParser::DATA - 64))
            | (1ULL << (sqlParser::DATE - 64))
            | (1ULL << (sqlParser::DATE_FORMAT - 64))
            | (1ULL << (sqlParser::DAYS - 64))
            | (1ULL << (sqlParser::DECODE - 64))
            | (1ULL << (sqlParser::DESC - 64))
            | (1ULL << (sqlParser::DETAIL - 64))
            | (1ULL << (sqlParser::DIGITS - 64))
            | (1ULL << (sqlParser::DLOAD - 64))
            | (1ULL << (sqlParser::DOCOL - 64))
            | (1ULL << (sqlParser::ECHO - 64))
            | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
            | (1ULL << (sqlParser::EXISTS - 64))
            | (1ULL << (sqlParser::EXIT_TOKEN - 64))
            | (1ULL << (sqlParser::EXPLAIN - 64))
            | (1ULL << (sqlParser::EXPORT - 64))
            | (1ULL << (sqlParser::EXTRACT - 64))
            | (1ULL << (sqlParser::SQL_FALSE - 64))
            | (1ULL << (sqlParser::FEDERATION - 64))
            | (1ULL << (sqlParser::FIELD_DELIMITER - 64))
            | (1ULL << (sqlParser::FILE - 64))
            | (1ULL << (sqlParser::FORCE - 64))
            | (1ULL << (sqlParser::FORMAT - 64))
            | (1ULL << (sqlParser::HASH - 64))
            | (1ULL << (sqlParser::HASHKEY - 64))
            | (1ULL << (sqlParser::HOURS - 64))
            | (1ULL << (sqlParser::IF - 64))
            | (1ULL << (sqlParser::IFNULL - 64))
            | (1ULL << (sqlParser::INDEX - 64))
            | (1ULL << (sqlParser::INTERVAL_DT - 64))
            | (1ULL << (sqlParser::INTERVAL_YM - 64))
            | (1ULL << (sqlParser::INTERVAL - 64))
            | (1ULL << (sqlParser::KEY - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::LIMIT - 130))
            | (1ULL << (sqlParser::LOADREPLICATED - 130))
            | (1ULL << (sqlParser::LOAD - 130))
            | (1ULL << (sqlParser::LOG - 130))
            | (1ULL << (sqlParser::LOOP - 130))
            | (1ULL << (sqlParser::LSIDX - 130))
            | (1ULL << (sqlParser::LSPTNCOL - 130))
            | (1ULL << (sqlParser::LSTBL - 130))
            | (1ULL << (sqlParser::MAX - 130))
            | (1ULL << (sqlParser::MEMORYLEAKS - 130))
            | (1ULL << (sqlParser::MILLIS - 130))
            | (1ULL << (sqlParser::MILLISECOND - 130))
            | (1ULL << (sqlParser::MILLISECONDS - 130))
            | (1ULL << (sqlParser::MIN - 130))
            | (1ULL << (sqlParser::MINUTES - 130))
            | (1ULL << (sqlParser::MONOTONIC - 130))
            | (1ULL << (sqlParser::MONTHS - 130))
            | (1ULL << (sqlParser::MS - 130))
            | (1ULL << (sqlParser::NODES - 130))
            | (1ULL << (sqlParser::NODE - 130))
            | (1ULL << (sqlParser::NOOP - 130))
            | (1ULL << (sqlParser::NOP - 130))
            | (1ULL << (sqlParser::NOT - 130))
            | (1ULL << (sqlParser::NOW - 130))
            | (1ULL << (sqlParser::SQL_NULL - 130))
            | (1ULL << (sqlParser::NULLIF - 130))
            | (1ULL << (sqlParser::OBJECTS - 130))
            | (1ULL << (sqlParser::OFF - 130))
            | (1ULL << (sqlParser::OFFSET - 130))
            | (1ULL << (sqlParser::ORIGIN - 130))
            | (1ULL << (sqlParser::OVERLAY - 130))
            | (1ULL << (sqlParser::PARTITIONS - 130))
            | (1ULL << (sqlParser::PATHS - 130))
            | (1ULL << (sqlParser::PIPELINE - 130))
            | (1ULL << (sqlParser::PLACING - 130))
            | (1ULL << (sqlParser::POSITION - 130))
            | (1ULL << (sqlParser::PRETTY - 130))
            | (1ULL << (sqlParser::QUARTER - 130))
            | (1ULL << (sqlParser::QUARTERS - 130))
            | (1ULL << (sqlParser::RECORD_ENDS - 130))
            | (1ULL << (sqlParser::REFERENCE - 130))
            | (1ULL << (sqlParser::REGIONS - 130))
            | (1ULL << (sqlParser::REPLICATED - 130))
            | (1ULL << (sqlParser::ROWSIZE - 130))
            | (1ULL << (sqlParser::SCHEMA - 130))
            | (1ULL << (sqlParser::SEC - 130))
            | (1ULL << (sqlParser::SECONDS - 130)))) != 0) || ((((_la - 197) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 197)) & ((1ULL << (sqlParser::SKIPLIST - 197))
            | (1ULL << (sqlParser::SLEEP - 197))
            | (1ULL << (sqlParser::SPACE - 197))
            | (1ULL << (sqlParser::STAGE - 197))
            | (1ULL << (sqlParser::STATS - 197))
            | (1ULL << (sqlParser::STATISTICS - 197))
            | (1ULL << (sqlParser::STATUS - 197))
            | (1ULL << (sqlParser::SUBSTR - 197))
            | (1ULL << (sqlParser::SUBSTRING - 197))
            | (1ULL << (sqlParser::SUM - 197))
            | (1ULL << (sqlParser::SZIDX - 197))
            | (1ULL << (sqlParser::SZTBL - 197))
            | (1ULL << (sqlParser::S_ONLY - 197))
            | (1ULL << (sqlParser::TABLESIZE - 197))
            | (1ULL << (sqlParser::TEXT - 197))
            | (1ULL << (sqlParser::TIME - 197))
            | (1ULL << (sqlParser::TIMESTAMP - 197))
            | (1ULL << (sqlParser::TOP - 197))
            | (1ULL << (sqlParser::TRACKMEMORY - 197))
            | (1ULL << (sqlParser::TREE - 197))
            | (1ULL << (sqlParser::TRIM - 197))
            | (1ULL << (sqlParser::TRUNCATE - 197))
            | (1ULL << (sqlParser::SQL_TRUE - 197))
            | (1ULL << (sqlParser::TYPE - 197))
            | (1ULL << (sqlParser::TYPEOF - 197))
            | (1ULL << (sqlParser::UNLOAD - 197))
            | (1ULL << (sqlParser::VALUE - 197))
            | (1ULL << (sqlParser::WEEK - 197))
            | (1ULL << (sqlParser::WEEKS - 197))
            | (1ULL << (sqlParser::WITH_NAMES - 197))
            | (1ULL << (sqlParser::WITH_TYPES - 197))
            | (1ULL << (sqlParser::XOR - 197))
            | (1ULL << (sqlParser::YEARS - 197))
            | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 197))
            | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 197))
            | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 197))
            | (1ULL << (sqlParser::SINGLQUOTED_STRING - 197))
            | (1ULL << (sqlParser::DUBLQUOTED_STRING - 197))
            | (1ULL << (sqlParser::FLOAT_LITERAL - 197))
            | (1ULL << (sqlParser::DECIMAL_LITERAL - 197))
            | (1ULL << (sqlParser::INTEGER_LITERAL - 197)))) != 0)) {
            setState(1893);
            expr(0);
            setState(1898);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__5) {
              setState(1894);
              match(sqlParser::T__5);
              setState(1895);
              expr(0);
              setState(1900);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1903);
          match(sqlParser::T__7);
          break;
        }

        case 13: {
          auto newContext = _tracker.createInstance<BetweenOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1904);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(1906);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1905);
            match(sqlParser::NOT);
          }
          setState(1908);
          match(sqlParser::BETWEEN);

          setState(1909);
          expr(0);
          setState(1910);
          match(sqlParser::AND);
          setState(1911);
          expr(0);
          break;
        }

        } 
      }
      setState(1917);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 226, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- VariableNameContext ------------------------------------------------------------------

sqlParser::VariableNameContext::VariableNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::VariableNameContext::CURRENT_CATALOG() {
  return getToken(sqlParser::CURRENT_CATALOG, 0);
}

tree::TerminalNode* sqlParser::VariableNameContext::CURRENT_SCHEMA() {
  return getToken(sqlParser::CURRENT_SCHEMA, 0);
}

tree::TerminalNode* sqlParser::VariableNameContext::CURRENT_TIMESTAMP() {
  return getToken(sqlParser::CURRENT_TIMESTAMP, 0);
}

tree::TerminalNode* sqlParser::VariableNameContext::CURRENT_USER() {
  return getToken(sqlParser::CURRENT_USER, 0);
}


size_t sqlParser::VariableNameContext::getRuleIndex() const {
  return sqlParser::RuleVariableName;
}


antlrcpp::Any sqlParser::VariableNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitVariableName(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::VariableNameContext* sqlParser::variableName() {
  VariableNameContext *_localctx = _tracker.createInstance<VariableNameContext>(_ctx, getState());
  enterRule(_localctx, 222, sqlParser::RuleVariableName);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1918);
    _la = _input->LA(1);
    if (!(((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::CURRENT_CATALOG - 64))
      | (1ULL << (sqlParser::CURRENT_SCHEMA - 64))
      | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 64))
      | (1ULL << (sqlParser::CURRENT_USER - 64)))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ColumnRefContext ------------------------------------------------------------------

sqlParser::ColumnRefContext::ColumnRefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::ColumnNameContext* sqlParser::ColumnRefContext::columnName() {
  return getRuleContext<sqlParser::ColumnNameContext>(0);
}

sqlParser::TableNameContext* sqlParser::ColumnRefContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::SchemaNameContext* sqlParser::ColumnRefContext::schemaName() {
  return getRuleContext<sqlParser::SchemaNameContext>(0);
}

sqlParser::CatalogNameContext* sqlParser::ColumnRefContext::catalogName() {
  return getRuleContext<sqlParser::CatalogNameContext>(0);
}


size_t sqlParser::ColumnRefContext::getRuleIndex() const {
  return sqlParser::RuleColumnRef;
}


antlrcpp::Any sqlParser::ColumnRefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitColumnRef(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ColumnRefContext* sqlParser::columnRef() {
  ColumnRefContext *_localctx = _tracker.createInstance<ColumnRefContext>(_ctx, getState());
  enterRule(_localctx, 224, sqlParser::RuleColumnRef);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1933);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 229, _ctx)) {
    case 1: {
      setState(1928);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 228, _ctx)) {
      case 1: {
        setState(1923);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 227, _ctx)) {
        case 1: {
          setState(1920);
          catalogName();
          setState(1921);
          match(sqlParser::T__9);
          break;
        }

        }

        setState(1925);
        schemaName();
        setState(1926);
        match(sqlParser::T__9);
        break;
      }

      }

      setState(1930);
      tableName();
      setState(1931);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(1935);
    columnName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ColumnWildcardContext ------------------------------------------------------------------

sqlParser::ColumnWildcardContext::ColumnWildcardContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::TableNameContext* sqlParser::ColumnWildcardContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::SchemaNameContext* sqlParser::ColumnWildcardContext::schemaName() {
  return getRuleContext<sqlParser::SchemaNameContext>(0);
}

sqlParser::CatalogNameContext* sqlParser::ColumnWildcardContext::catalogName() {
  return getRuleContext<sqlParser::CatalogNameContext>(0);
}


size_t sqlParser::ColumnWildcardContext::getRuleIndex() const {
  return sqlParser::RuleColumnWildcard;
}


antlrcpp::Any sqlParser::ColumnWildcardContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitColumnWildcard(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ColumnWildcardContext* sqlParser::columnWildcard() {
  ColumnWildcardContext *_localctx = _tracker.createInstance<ColumnWildcardContext>(_ctx, getState());
  enterRule(_localctx, 226, sqlParser::RuleColumnWildcard);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1950);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::ANALYSE)
      | (1ULL << sqlParser::ANALYZE)
      | (1ULL << sqlParser::ANIDX)
      | (1ULL << sqlParser::ANTBL)
      | (1ULL << sqlParser::ASC)
      | (1ULL << sqlParser::BIT)
      | (1ULL << sqlParser::BITS)
      | (1ULL << sqlParser::BLOCK)
      | (1ULL << sqlParser::BLOOM)
      | (1ULL << sqlParser::BPA)
      | (1ULL << sqlParser::BTREE)
      | (1ULL << sqlParser::BYE)
      | (1ULL << sqlParser::BYTE)
      | (1ULL << sqlParser::BYTES)
      | (1ULL << sqlParser::CATALOG)
      | (1ULL << sqlParser::CHARS)
      | (1ULL << sqlParser::COMPACTIDX)
      | (1ULL << sqlParser::CONCAT))) != 0) || ((((_la - 68) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 68)) & ((1ULL << (sqlParser::DATA - 68))
      | (1ULL << (sqlParser::DATE_FORMAT - 68))
      | (1ULL << (sqlParser::DAYS - 68))
      | (1ULL << (sqlParser::DECODE - 68))
      | (1ULL << (sqlParser::DESC - 68))
      | (1ULL << (sqlParser::DETAIL - 68))
      | (1ULL << (sqlParser::DIGITS - 68))
      | (1ULL << (sqlParser::DLOAD - 68))
      | (1ULL << (sqlParser::DOCOL - 68))
      | (1ULL << (sqlParser::ECHO - 68))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 68))
      | (1ULL << (sqlParser::EXIT_TOKEN - 68))
      | (1ULL << (sqlParser::EXPLAIN - 68))
      | (1ULL << (sqlParser::EXPORT - 68))
      | (1ULL << (sqlParser::SQL_FALSE - 68))
      | (1ULL << (sqlParser::FEDERATION - 68))
      | (1ULL << (sqlParser::FIELD_DELIMITER - 68))
      | (1ULL << (sqlParser::FILE - 68))
      | (1ULL << (sqlParser::FORCE - 68))
      | (1ULL << (sqlParser::FORMAT - 68))
      | (1ULL << (sqlParser::HASH - 68))
      | (1ULL << (sqlParser::HASHKEY - 68))
      | (1ULL << (sqlParser::HOURS - 68))
      | (1ULL << (sqlParser::IF - 68))
      | (1ULL << (sqlParser::IFNULL - 68))
      | (1ULL << (sqlParser::INDEX - 68))
      | (1ULL << (sqlParser::INTERVAL_DT - 68))
      | (1ULL << (sqlParser::INTERVAL_YM - 68))
      | (1ULL << (sqlParser::KEY - 68))
      | (1ULL << (sqlParser::LIMIT - 68))
      | (1ULL << (sqlParser::LOADREPLICATED - 68)))) != 0) || ((((_la - 132) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 132)) & ((1ULL << (sqlParser::LOAD - 132))
      | (1ULL << (sqlParser::LOG - 132))
      | (1ULL << (sqlParser::LOOP - 132))
      | (1ULL << (sqlParser::LSIDX - 132))
      | (1ULL << (sqlParser::LSPTNCOL - 132))
      | (1ULL << (sqlParser::LSTBL - 132))
      | (1ULL << (sqlParser::MEMORYLEAKS - 132))
      | (1ULL << (sqlParser::MILLIS - 132))
      | (1ULL << (sqlParser::MILLISECOND - 132))
      | (1ULL << (sqlParser::MILLISECONDS - 132))
      | (1ULL << (sqlParser::MINUTES - 132))
      | (1ULL << (sqlParser::MONOTONIC - 132))
      | (1ULL << (sqlParser::MONTHS - 132))
      | (1ULL << (sqlParser::MS - 132))
      | (1ULL << (sqlParser::NODES - 132))
      | (1ULL << (sqlParser::NODE - 132))
      | (1ULL << (sqlParser::NOOP - 132))
      | (1ULL << (sqlParser::NOP - 132))
      | (1ULL << (sqlParser::NOW - 132))
      | (1ULL << (sqlParser::OBJECTS - 132))
      | (1ULL << (sqlParser::OFF - 132))
      | (1ULL << (sqlParser::OFFSET - 132))
      | (1ULL << (sqlParser::ORIGIN - 132))
      | (1ULL << (sqlParser::PARTITIONS - 132))
      | (1ULL << (sqlParser::PATHS - 132))
      | (1ULL << (sqlParser::PIPELINE - 132))
      | (1ULL << (sqlParser::PLACING - 132))
      | (1ULL << (sqlParser::PRETTY - 132))
      | (1ULL << (sqlParser::QUARTER - 132))
      | (1ULL << (sqlParser::QUARTERS - 132))
      | (1ULL << (sqlParser::RECORD_ENDS - 132))
      | (1ULL << (sqlParser::REFERENCE - 132))
      | (1ULL << (sqlParser::REGIONS - 132))
      | (1ULL << (sqlParser::REPLICATED - 132))
      | (1ULL << (sqlParser::ROWSIZE - 132))
      | (1ULL << (sqlParser::SCHEMA - 132))
      | (1ULL << (sqlParser::SEC - 132))
      | (1ULL << (sqlParser::SECONDS - 132)))) != 0) || ((((_la - 197) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 197)) & ((1ULL << (sqlParser::SKIPLIST - 197))
      | (1ULL << (sqlParser::SLEEP - 197))
      | (1ULL << (sqlParser::SPACE - 197))
      | (1ULL << (sqlParser::STAGE - 197))
      | (1ULL << (sqlParser::STATS - 197))
      | (1ULL << (sqlParser::STATISTICS - 197))
      | (1ULL << (sqlParser::STATUS - 197))
      | (1ULL << (sqlParser::SUBSTR - 197))
      | (1ULL << (sqlParser::SZIDX - 197))
      | (1ULL << (sqlParser::SZTBL - 197))
      | (1ULL << (sqlParser::S_ONLY - 197))
      | (1ULL << (sqlParser::TABLESIZE - 197))
      | (1ULL << (sqlParser::TEXT - 197))
      | (1ULL << (sqlParser::TOP - 197))
      | (1ULL << (sqlParser::TRACKMEMORY - 197))
      | (1ULL << (sqlParser::TREE - 197))
      | (1ULL << (sqlParser::TRUNCATE - 197))
      | (1ULL << (sqlParser::TYPE - 197))
      | (1ULL << (sqlParser::TYPEOF - 197))
      | (1ULL << (sqlParser::UNLOAD - 197))
      | (1ULL << (sqlParser::VALUE - 197))
      | (1ULL << (sqlParser::WEEK - 197))
      | (1ULL << (sqlParser::WEEKS - 197))
      | (1ULL << (sqlParser::WITH_NAMES - 197))
      | (1ULL << (sqlParser::WITH_TYPES - 197))
      | (1ULL << (sqlParser::XOR - 197))
      | (1ULL << (sqlParser::YEARS - 197))
      | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 197))
      | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 197))
      | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 197)))) != 0)) {
      setState(1945);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 231, _ctx)) {
      case 1: {
        setState(1940);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 230, _ctx)) {
        case 1: {
          setState(1937);
          catalogName();
          setState(1938);
          match(sqlParser::T__9);
          break;
        }

        }

        setState(1942);
        schemaName();
        setState(1943);
        match(sqlParser::T__9);
        break;
      }

      }

      setState(1947);
      tableName();
      setState(1948);
      match(sqlParser::T__9);
    }
    setState(1952);
    match(sqlParser::T__11);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassNameContext ------------------------------------------------------------------

sqlParser::ClassNameContext::ClassNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::ClassNameContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}

std::vector<sqlParser::QualifierContext *> sqlParser::ClassNameContext::qualifier() {
  return getRuleContexts<sqlParser::QualifierContext>();
}

sqlParser::QualifierContext* sqlParser::ClassNameContext::qualifier(size_t i) {
  return getRuleContext<sqlParser::QualifierContext>(i);
}


size_t sqlParser::ClassNameContext::getRuleIndex() const {
  return sqlParser::RuleClassName;
}


antlrcpp::Any sqlParser::ClassNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitClassName(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ClassNameContext* sqlParser::className() {
  ClassNameContext *_localctx = _tracker.createInstance<ClassNameContext>(_ctx, getState());
  enterRule(_localctx, 228, sqlParser::RuleClassName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1959);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 233, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1954);
        qualifier();
        setState(1955);
        match(sqlParser::T__9); 
      }
      setState(1961);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 233, _ctx);
    }
    setState(1962);
    dynamic_cast<ClassNameContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionNameContext ------------------------------------------------------------------

sqlParser::FunctionNameContext::FunctionNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::FunctionNameContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::FunctionNameContext::getRuleIndex() const {
  return sqlParser::RuleFunctionName;
}


antlrcpp::Any sqlParser::FunctionNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitFunctionName(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::FunctionNameContext* sqlParser::functionName() {
  FunctionNameContext *_localctx = _tracker.createInstance<FunctionNameContext>(_ctx, getState());
  enterRule(_localctx, 230, sqlParser::RuleFunctionName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1964);
    dynamic_cast<FunctionNameContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MethodNameContext ------------------------------------------------------------------

sqlParser::MethodNameContext::MethodNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::MethodNameContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::MethodNameContext::getRuleIndex() const {
  return sqlParser::RuleMethodName;
}


antlrcpp::Any sqlParser::MethodNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitMethodName(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::MethodNameContext* sqlParser::methodName() {
  MethodNameContext *_localctx = _tracker.createInstance<MethodNameContext>(_ctx, getState());
  enterRule(_localctx, 232, sqlParser::RuleMethodName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1966);
    dynamic_cast<MethodNameContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CatalogNameContext ------------------------------------------------------------------

sqlParser::CatalogNameContext::CatalogNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::CatalogNameContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::CatalogNameContext::getRuleIndex() const {
  return sqlParser::RuleCatalogName;
}


antlrcpp::Any sqlParser::CatalogNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCatalogName(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CatalogNameContext* sqlParser::catalogName() {
  CatalogNameContext *_localctx = _tracker.createInstance<CatalogNameContext>(_ctx, getState());
  enterRule(_localctx, 234, sqlParser::RuleCatalogName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1968);
    dynamic_cast<CatalogNameContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SchemaNameContext ------------------------------------------------------------------

sqlParser::SchemaNameContext::SchemaNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::SchemaNameContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::SchemaNameContext::getRuleIndex() const {
  return sqlParser::RuleSchemaName;
}


antlrcpp::Any sqlParser::SchemaNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSchemaName(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SchemaNameContext* sqlParser::schemaName() {
  SchemaNameContext *_localctx = _tracker.createInstance<SchemaNameContext>(_ctx, getState());
  enterRule(_localctx, 236, sqlParser::RuleSchemaName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1970);
    dynamic_cast<SchemaNameContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TableNameContext ------------------------------------------------------------------

sqlParser::TableNameContext::TableNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::TableNameContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::TableNameContext::getRuleIndex() const {
  return sqlParser::RuleTableName;
}


antlrcpp::Any sqlParser::TableNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTableName(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TableNameContext* sqlParser::tableName() {
  TableNameContext *_localctx = _tracker.createInstance<TableNameContext>(_ctx, getState());
  enterRule(_localctx, 238, sqlParser::RuleTableName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1972);
    dynamic_cast<TableNameContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TableAliasContext ------------------------------------------------------------------

sqlParser::TableAliasContext::TableAliasContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::TableAliasContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::TableAliasContext::getRuleIndex() const {
  return sqlParser::RuleTableAlias;
}


antlrcpp::Any sqlParser::TableAliasContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTableAlias(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TableAliasContext* sqlParser::tableAlias() {
  TableAliasContext *_localctx = _tracker.createInstance<TableAliasContext>(_ctx, getState());
  enterRule(_localctx, 240, sqlParser::RuleTableAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1974);
    dynamic_cast<TableAliasContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ColumnNameContext ------------------------------------------------------------------

sqlParser::ColumnNameContext::ColumnNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::ColumnNameContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::ColumnNameContext::getRuleIndex() const {
  return sqlParser::RuleColumnName;
}


antlrcpp::Any sqlParser::ColumnNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitColumnName(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ColumnNameContext* sqlParser::columnName() {
  ColumnNameContext *_localctx = _tracker.createInstance<ColumnNameContext>(_ctx, getState());
  enterRule(_localctx, 242, sqlParser::RuleColumnName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1976);
    dynamic_cast<ColumnNameContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ColumnAliasContext ------------------------------------------------------------------

sqlParser::ColumnAliasContext::ColumnAliasContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::ColumnAliasContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::ColumnAliasContext::getRuleIndex() const {
  return sqlParser::RuleColumnAlias;
}


antlrcpp::Any sqlParser::ColumnAliasContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitColumnAlias(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ColumnAliasContext* sqlParser::columnAlias() {
  ColumnAliasContext *_localctx = _tracker.createInstance<ColumnAliasContext>(_ctx, getState());
  enterRule(_localctx, 244, sqlParser::RuleColumnAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1978);
    dynamic_cast<ColumnAliasContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CollationNameContext ------------------------------------------------------------------

sqlParser::CollationNameContext::CollationNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::CollationNameContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::CollationNameContext::getRuleIndex() const {
  return sqlParser::RuleCollationName;
}


antlrcpp::Any sqlParser::CollationNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCollationName(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CollationNameContext* sqlParser::collationName() {
  CollationNameContext *_localctx = _tracker.createInstance<CollationNameContext>(_ctx, getState());
  enterRule(_localctx, 246, sqlParser::RuleCollationName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1980);
    dynamic_cast<CollationNameContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IndexNameContext ------------------------------------------------------------------

sqlParser::IndexNameContext::IndexNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::IndexNameContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::IndexNameContext::getRuleIndex() const {
  return sqlParser::RuleIndexName;
}


antlrcpp::Any sqlParser::IndexNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitIndexName(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::IndexNameContext* sqlParser::indexName() {
  IndexNameContext *_localctx = _tracker.createInstance<IndexNameContext>(_ctx, getState());
  enterRule(_localctx, 248, sqlParser::RuleIndexName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1982);
    dynamic_cast<IndexNameContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyNameContext ------------------------------------------------------------------

sqlParser::PropertyNameContext::PropertyNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::PropertyNameContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::PropertyNameContext::getRuleIndex() const {
  return sqlParser::RulePropertyName;
}


antlrcpp::Any sqlParser::PropertyNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitPropertyName(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::PropertyNameContext* sqlParser::propertyName() {
  PropertyNameContext *_localctx = _tracker.createInstance<PropertyNameContext>(_ctx, getState());
  enterRule(_localctx, 250, sqlParser::RulePropertyName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1984);
    dynamic_cast<PropertyNameContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- HintNameContext ------------------------------------------------------------------

sqlParser::HintNameContext::HintNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::HintNameContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::HintNameContext::getRuleIndex() const {
  return sqlParser::RuleHintName;
}


antlrcpp::Any sqlParser::HintNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitHintName(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::HintNameContext* sqlParser::hintName() {
  HintNameContext *_localctx = _tracker.createInstance<HintNameContext>(_ctx, getState());
  enterRule(_localctx, 252, sqlParser::RuleHintName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1986);
    dynamic_cast<HintNameContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprHintContext ------------------------------------------------------------------

sqlParser::ExprHintContext::ExprHintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::ExprHintContext::HASHKEY() {
  return getToken(sqlParser::HASHKEY, 0);
}


size_t sqlParser::ExprHintContext::getRuleIndex() const {
  return sqlParser::RuleExprHint;
}


antlrcpp::Any sqlParser::ExprHintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitExprHint(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ExprHintContext* sqlParser::exprHint() {
  ExprHintContext *_localctx = _tracker.createInstance<ExprHintContext>(_ctx, getState());
  enterRule(_localctx, 254, sqlParser::RuleExprHint);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1988);
    match(sqlParser::HASHKEY);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropertyValueContext ------------------------------------------------------------------

sqlParser::PropertyValueContext::PropertyValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::SimpleLiteralContext* sqlParser::PropertyValueContext::simpleLiteral() {
  return getRuleContext<sqlParser::SimpleLiteralContext>(0);
}

sqlParser::IdentifierContext* sqlParser::PropertyValueContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::PropertyValueContext::getRuleIndex() const {
  return sqlParser::RulePropertyValue;
}


antlrcpp::Any sqlParser::PropertyValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitPropertyValue(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::PropertyValueContext* sqlParser::propertyValue() {
  PropertyValueContext *_localctx = _tracker.createInstance<PropertyValueContext>(_ctx, getState());
  enterRule(_localctx, 256, sqlParser::RulePropertyValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1992);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 234, _ctx)) {
    case 1: {
      setState(1990);
      dynamic_cast<PropertyValueContext *>(_localctx)->lit = simpleLiteral();
      break;
    }

    case 2: {
      setState(1991);
      dynamic_cast<PropertyValueContext *>(_localctx)->id = identifier();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- HintValueContext ------------------------------------------------------------------

sqlParser::HintValueContext::HintValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::SimpleLiteralContext* sqlParser::HintValueContext::simpleLiteral() {
  return getRuleContext<sqlParser::SimpleLiteralContext>(0);
}

sqlParser::IdentifierContext* sqlParser::HintValueContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}

sqlParser::ExprContext* sqlParser::HintValueContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}


size_t sqlParser::HintValueContext::getRuleIndex() const {
  return sqlParser::RuleHintValue;
}


antlrcpp::Any sqlParser::HintValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitHintValue(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::HintValueContext* sqlParser::hintValue() {
  HintValueContext *_localctx = _tracker.createInstance<HintValueContext>(_ctx, getState());
  enterRule(_localctx, 258, sqlParser::RuleHintValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1997);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 235, _ctx)) {
    case 1: {
      setState(1994);
      dynamic_cast<HintValueContext *>(_localctx)->lit = simpleLiteral();
      break;
    }

    case 2: {
      setState(1995);
      dynamic_cast<HintValueContext *>(_localctx)->id = identifier();
      break;
    }

    case 3: {
      setState(1996);
      dynamic_cast<HintValueContext *>(_localctx)->expression = expr(0);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdentifierContext ------------------------------------------------------------------

sqlParser::IdentifierContext::IdentifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::IdentifierContext::PLAIN_IDENTIFIER() {
  return getToken(sqlParser::PLAIN_IDENTIFIER, 0);
}

tree::TerminalNode* sqlParser::IdentifierContext::BAKTIKD_IDENTIFIER() {
  return getToken(sqlParser::BAKTIKD_IDENTIFIER, 0);
}

tree::TerminalNode* sqlParser::IdentifierContext::DUBLQUOTED_IDENTIFIER() {
  return getToken(sqlParser::DUBLQUOTED_IDENTIFIER, 0);
}

sqlParser::NonReservedWordContext* sqlParser::IdentifierContext::nonReservedWord() {
  return getRuleContext<sqlParser::NonReservedWordContext>(0);
}


size_t sqlParser::IdentifierContext::getRuleIndex() const {
  return sqlParser::RuleIdentifier;
}


antlrcpp::Any sqlParser::IdentifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitIdentifier(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::IdentifierContext* sqlParser::identifier() {
  IdentifierContext *_localctx = _tracker.createInstance<IdentifierContext>(_ctx, getState());
  enterRule(_localctx, 260, sqlParser::RuleIdentifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(2003);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::PLAIN_IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(1999);
        match(sqlParser::PLAIN_IDENTIFIER);
        break;
      }

      case sqlParser::BAKTIKD_IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(2000);
        match(sqlParser::BAKTIKD_IDENTIFIER);
        break;
      }

      case sqlParser::DUBLQUOTED_IDENTIFIER: {
        enterOuterAlt(_localctx, 3);
        setState(2001);
        match(sqlParser::DUBLQUOTED_IDENTIFIER);
        break;
      }

      case sqlParser::ANALYSE:
      case sqlParser::ANALYZE:
      case sqlParser::ANIDX:
      case sqlParser::ANTBL:
      case sqlParser::ASC:
      case sqlParser::BIT:
      case sqlParser::BITS:
      case sqlParser::BLOCK:
      case sqlParser::BLOOM:
      case sqlParser::BPA:
      case sqlParser::BTREE:
      case sqlParser::BYE:
      case sqlParser::BYTE:
      case sqlParser::BYTES:
      case sqlParser::CATALOG:
      case sqlParser::CHARS:
      case sqlParser::COMPACTIDX:
      case sqlParser::CONCAT:
      case sqlParser::DATA:
      case sqlParser::DATE_FORMAT:
      case sqlParser::DAYS:
      case sqlParser::DECODE:
      case sqlParser::DESC:
      case sqlParser::DETAIL:
      case sqlParser::DIGITS:
      case sqlParser::DLOAD:
      case sqlParser::DOCOL:
      case sqlParser::ECHO:
      case sqlParser::ESCAPE_CHAR:
      case sqlParser::EXIT_TOKEN:
      case sqlParser::EXPLAIN:
      case sqlParser::EXPORT:
      case sqlParser::SQL_FALSE:
      case sqlParser::FEDERATION:
      case sqlParser::FIELD_DELIMITER:
      case sqlParser::FILE:
      case sqlParser::FORCE:
      case sqlParser::FORMAT:
      case sqlParser::HASH:
      case sqlParser::HASHKEY:
      case sqlParser::HOURS:
      case sqlParser::IF:
      case sqlParser::IFNULL:
      case sqlParser::INDEX:
      case sqlParser::INTERVAL_DT:
      case sqlParser::INTERVAL_YM:
      case sqlParser::KEY:
      case sqlParser::LIMIT:
      case sqlParser::LOADREPLICATED:
      case sqlParser::LOAD:
      case sqlParser::LOG:
      case sqlParser::LOOP:
      case sqlParser::LSIDX:
      case sqlParser::LSPTNCOL:
      case sqlParser::LSTBL:
      case sqlParser::MEMORYLEAKS:
      case sqlParser::MILLIS:
      case sqlParser::MILLISECOND:
      case sqlParser::MILLISECONDS:
      case sqlParser::MINUTES:
      case sqlParser::MONOTONIC:
      case sqlParser::MONTHS:
      case sqlParser::MS:
      case sqlParser::NODES:
      case sqlParser::NODE:
      case sqlParser::NOOP:
      case sqlParser::NOP:
      case sqlParser::NOW:
      case sqlParser::OBJECTS:
      case sqlParser::OFF:
      case sqlParser::OFFSET:
      case sqlParser::ORIGIN:
      case sqlParser::PARTITIONS:
      case sqlParser::PATHS:
      case sqlParser::PIPELINE:
      case sqlParser::PLACING:
      case sqlParser::PRETTY:
      case sqlParser::QUARTER:
      case sqlParser::QUARTERS:
      case sqlParser::RECORD_ENDS:
      case sqlParser::REFERENCE:
      case sqlParser::REGIONS:
      case sqlParser::REPLICATED:
      case sqlParser::ROWSIZE:
      case sqlParser::SCHEMA:
      case sqlParser::SEC:
      case sqlParser::SECONDS:
      case sqlParser::SKIPLIST:
      case sqlParser::SLEEP:
      case sqlParser::SPACE:
      case sqlParser::STAGE:
      case sqlParser::STATS:
      case sqlParser::STATISTICS:
      case sqlParser::STATUS:
      case sqlParser::SUBSTR:
      case sqlParser::SZIDX:
      case sqlParser::SZTBL:
      case sqlParser::S_ONLY:
      case sqlParser::TABLESIZE:
      case sqlParser::TEXT:
      case sqlParser::TOP:
      case sqlParser::TRACKMEMORY:
      case sqlParser::TREE:
      case sqlParser::TRUNCATE:
      case sqlParser::TYPE:
      case sqlParser::TYPEOF:
      case sqlParser::UNLOAD:
      case sqlParser::VALUE:
      case sqlParser::WEEK:
      case sqlParser::WEEKS:
      case sqlParser::WITH_NAMES:
      case sqlParser::WITH_TYPES:
      case sqlParser::XOR:
      case sqlParser::YEARS: {
        enterOuterAlt(_localctx, 4);
        setState(2002);
        nonReservedWord();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QualifierContext ------------------------------------------------------------------

sqlParser::QualifierContext::QualifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IdentifierContext* sqlParser::QualifierContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}


size_t sqlParser::QualifierContext::getRuleIndex() const {
  return sqlParser::RuleQualifier;
}


antlrcpp::Any sqlParser::QualifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitQualifier(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::QualifierContext* sqlParser::qualifier() {
  QualifierContext *_localctx = _tracker.createInstance<QualifierContext>(_ctx, getState());
  enterRule(_localctx, 262, sqlParser::RuleQualifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2005);
    dynamic_cast<QualifierContext *>(_localctx)->id = identifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrecisionContext ------------------------------------------------------------------

sqlParser::PrecisionContext::PrecisionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IntegerLiteralContext* sqlParser::PrecisionContext::integerLiteral() {
  return getRuleContext<sqlParser::IntegerLiteralContext>(0);
}


size_t sqlParser::PrecisionContext::getRuleIndex() const {
  return sqlParser::RulePrecision;
}


antlrcpp::Any sqlParser::PrecisionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitPrecision(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::PrecisionContext* sqlParser::precision() {
  PrecisionContext *_localctx = _tracker.createInstance<PrecisionContext>(_ctx, getState());
  enterRule(_localctx, 264, sqlParser::RulePrecision);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2007);
    integerLiteral();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ScaleContext ------------------------------------------------------------------

sqlParser::ScaleContext::ScaleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IntegerLiteralContext* sqlParser::ScaleContext::integerLiteral() {
  return getRuleContext<sqlParser::IntegerLiteralContext>(0);
}


size_t sqlParser::ScaleContext::getRuleIndex() const {
  return sqlParser::RuleScale;
}


antlrcpp::Any sqlParser::ScaleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitScale(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ScaleContext* sqlParser::scale() {
  ScaleContext *_localctx = _tracker.createInstance<ScaleContext>(_ctx, getState());
  enterRule(_localctx, 266, sqlParser::RuleScale);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2009);
    integerLiteral();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SizeContext ------------------------------------------------------------------

sqlParser::SizeContext::SizeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::IntegerLiteralContext* sqlParser::SizeContext::integerLiteral() {
  return getRuleContext<sqlParser::IntegerLiteralContext>(0);
}


size_t sqlParser::SizeContext::getRuleIndex() const {
  return sqlParser::RuleSize;
}


antlrcpp::Any sqlParser::SizeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSize(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SizeContext* sqlParser::size() {
  SizeContext *_localctx = _tracker.createInstance<SizeContext>(_ctx, getState());
  enterRule(_localctx, 268, sqlParser::RuleSize);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2011);
    integerLiteral();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LiteralContext ------------------------------------------------------------------

sqlParser::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LiteralContext::SQL_NULL() {
  return getToken(sqlParser::SQL_NULL, 0);
}

sqlParser::BooleanLiteralContext* sqlParser::LiteralContext::booleanLiteral() {
  return getRuleContext<sqlParser::BooleanLiteralContext>(0);
}

sqlParser::SignedIntegerLiteralContext* sqlParser::LiteralContext::signedIntegerLiteral() {
  return getRuleContext<sqlParser::SignedIntegerLiteralContext>(0);
}

sqlParser::SignedDecimalLiteralContext* sqlParser::LiteralContext::signedDecimalLiteral() {
  return getRuleContext<sqlParser::SignedDecimalLiteralContext>(0);
}

sqlParser::SignedFloatLiteralContext* sqlParser::LiteralContext::signedFloatLiteral() {
  return getRuleContext<sqlParser::SignedFloatLiteralContext>(0);
}

sqlParser::DateLiteralContext* sqlParser::LiteralContext::dateLiteral() {
  return getRuleContext<sqlParser::DateLiteralContext>(0);
}

sqlParser::TimeLiteralContext* sqlParser::LiteralContext::timeLiteral() {
  return getRuleContext<sqlParser::TimeLiteralContext>(0);
}

sqlParser::StringLiteralContext* sqlParser::LiteralContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}

sqlParser::TimestampLiteralContext* sqlParser::LiteralContext::timestampLiteral() {
  return getRuleContext<sqlParser::TimestampLiteralContext>(0);
}

sqlParser::IntervalLiteralContext* sqlParser::LiteralContext::intervalLiteral() {
  return getRuleContext<sqlParser::IntervalLiteralContext>(0);
}


size_t sqlParser::LiteralContext::getRuleIndex() const {
  return sqlParser::RuleLiteral;
}


antlrcpp::Any sqlParser::LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LiteralContext* sqlParser::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 270, sqlParser::RuleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(2023);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 237, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(2013);
      match(sqlParser::SQL_NULL);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(2014);
      booleanLiteral();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(2015);
      signedIntegerLiteral();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(2016);
      signedDecimalLiteral();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(2017);
      signedFloatLiteral();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(2018);
      dateLiteral();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(2019);
      timeLiteral();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(2020);
      stringLiteral();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(2021);
      timestampLiteral();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(2022);
      intervalLiteral();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleLiteralContext ------------------------------------------------------------------

sqlParser::SimpleLiteralContext::SimpleLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SimpleLiteralContext::SQL_NULL() {
  return getToken(sqlParser::SQL_NULL, 0);
}

sqlParser::BooleanLiteralContext* sqlParser::SimpleLiteralContext::booleanLiteral() {
  return getRuleContext<sqlParser::BooleanLiteralContext>(0);
}

sqlParser::SignedIntegerLiteralContext* sqlParser::SimpleLiteralContext::signedIntegerLiteral() {
  return getRuleContext<sqlParser::SignedIntegerLiteralContext>(0);
}

sqlParser::SignedDecimalLiteralContext* sqlParser::SimpleLiteralContext::signedDecimalLiteral() {
  return getRuleContext<sqlParser::SignedDecimalLiteralContext>(0);
}

sqlParser::SignedFloatLiteralContext* sqlParser::SimpleLiteralContext::signedFloatLiteral() {
  return getRuleContext<sqlParser::SignedFloatLiteralContext>(0);
}

sqlParser::StringLiteralContext* sqlParser::SimpleLiteralContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}


size_t sqlParser::SimpleLiteralContext::getRuleIndex() const {
  return sqlParser::RuleSimpleLiteral;
}


antlrcpp::Any sqlParser::SimpleLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSimpleLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SimpleLiteralContext* sqlParser::simpleLiteral() {
  SimpleLiteralContext *_localctx = _tracker.createInstance<SimpleLiteralContext>(_ctx, getState());
  enterRule(_localctx, 272, sqlParser::RuleSimpleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(2031);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 238, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(2025);
      match(sqlParser::SQL_NULL);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(2026);
      booleanLiteral();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(2027);
      signedIntegerLiteral();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(2028);
      signedDecimalLiteral();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(2029);
      signedFloatLiteral();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(2030);
      stringLiteral();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BooleanLiteralContext ------------------------------------------------------------------

sqlParser::BooleanLiteralContext::BooleanLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::BooleanLiteralContext::SQL_TRUE() {
  return getToken(sqlParser::SQL_TRUE, 0);
}

tree::TerminalNode* sqlParser::BooleanLiteralContext::SQL_FALSE() {
  return getToken(sqlParser::SQL_FALSE, 0);
}


size_t sqlParser::BooleanLiteralContext::getRuleIndex() const {
  return sqlParser::RuleBooleanLiteral;
}


antlrcpp::Any sqlParser::BooleanLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitBooleanLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::BooleanLiteralContext* sqlParser::booleanLiteral() {
  BooleanLiteralContext *_localctx = _tracker.createInstance<BooleanLiteralContext>(_ctx, getState());
  enterRule(_localctx, 274, sqlParser::RuleBooleanLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2033);
    _la = _input->LA(1);
    if (!(_la == sqlParser::SQL_FALSE || _la == sqlParser::SQL_TRUE)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntegerLiteralContext ------------------------------------------------------------------

sqlParser::IntegerLiteralContext::IntegerLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::IntegerLiteralContext::INTEGER_LITERAL() {
  return getToken(sqlParser::INTEGER_LITERAL, 0);
}


size_t sqlParser::IntegerLiteralContext::getRuleIndex() const {
  return sqlParser::RuleIntegerLiteral;
}


antlrcpp::Any sqlParser::IntegerLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitIntegerLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::IntegerLiteralContext* sqlParser::integerLiteral() {
  IntegerLiteralContext *_localctx = _tracker.createInstance<IntegerLiteralContext>(_ctx, getState());
  enterRule(_localctx, 276, sqlParser::RuleIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2035);
    match(sqlParser::INTEGER_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SignedIntegerLiteralContext ------------------------------------------------------------------

sqlParser::SignedIntegerLiteralContext::SignedIntegerLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SignedIntegerLiteralContext::INTEGER_LITERAL() {
  return getToken(sqlParser::INTEGER_LITERAL, 0);
}


size_t sqlParser::SignedIntegerLiteralContext::getRuleIndex() const {
  return sqlParser::RuleSignedIntegerLiteral;
}


antlrcpp::Any sqlParser::SignedIntegerLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSignedIntegerLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SignedIntegerLiteralContext* sqlParser::signedIntegerLiteral() {
  SignedIntegerLiteralContext *_localctx = _tracker.createInstance<SignedIntegerLiteralContext>(_ctx, getState());
  enterRule(_localctx, 278, sqlParser::RuleSignedIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2039);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__14: {
        setState(2037);
        match(sqlParser::T__14);
        break;
      }

      case sqlParser::T__10: {
        setState(2038);
        dynamic_cast<SignedIntegerLiteralContext *>(_localctx)->neg = match(sqlParser::T__10);
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(2041);
    match(sqlParser::INTEGER_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DecimalLiteralContext ------------------------------------------------------------------

sqlParser::DecimalLiteralContext::DecimalLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DecimalLiteralContext::DECIMAL_LITERAL() {
  return getToken(sqlParser::DECIMAL_LITERAL, 0);
}


size_t sqlParser::DecimalLiteralContext::getRuleIndex() const {
  return sqlParser::RuleDecimalLiteral;
}


antlrcpp::Any sqlParser::DecimalLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDecimalLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DecimalLiteralContext* sqlParser::decimalLiteral() {
  DecimalLiteralContext *_localctx = _tracker.createInstance<DecimalLiteralContext>(_ctx, getState());
  enterRule(_localctx, 280, sqlParser::RuleDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2043);
    match(sqlParser::DECIMAL_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SignedDecimalLiteralContext ------------------------------------------------------------------

sqlParser::SignedDecimalLiteralContext::SignedDecimalLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SignedDecimalLiteralContext::DECIMAL_LITERAL() {
  return getToken(sqlParser::DECIMAL_LITERAL, 0);
}


size_t sqlParser::SignedDecimalLiteralContext::getRuleIndex() const {
  return sqlParser::RuleSignedDecimalLiteral;
}


antlrcpp::Any sqlParser::SignedDecimalLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSignedDecimalLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SignedDecimalLiteralContext* sqlParser::signedDecimalLiteral() {
  SignedDecimalLiteralContext *_localctx = _tracker.createInstance<SignedDecimalLiteralContext>(_ctx, getState());
  enterRule(_localctx, 282, sqlParser::RuleSignedDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2047);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__14: {
        setState(2045);
        match(sqlParser::T__14);
        break;
      }

      case sqlParser::T__10: {
        setState(2046);
        dynamic_cast<SignedDecimalLiteralContext *>(_localctx)->neg = match(sqlParser::T__10);
        break;
      }

      case sqlParser::DECIMAL_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(2049);
    match(sqlParser::DECIMAL_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FloatLiteralContext ------------------------------------------------------------------

sqlParser::FloatLiteralContext::FloatLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::FloatLiteralContext::FLOAT_LITERAL() {
  return getToken(sqlParser::FLOAT_LITERAL, 0);
}


size_t sqlParser::FloatLiteralContext::getRuleIndex() const {
  return sqlParser::RuleFloatLiteral;
}


antlrcpp::Any sqlParser::FloatLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitFloatLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::FloatLiteralContext* sqlParser::floatLiteral() {
  FloatLiteralContext *_localctx = _tracker.createInstance<FloatLiteralContext>(_ctx, getState());
  enterRule(_localctx, 284, sqlParser::RuleFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2051);
    match(sqlParser::FLOAT_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SignedFloatLiteralContext ------------------------------------------------------------------

sqlParser::SignedFloatLiteralContext::SignedFloatLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SignedFloatLiteralContext::FLOAT_LITERAL() {
  return getToken(sqlParser::FLOAT_LITERAL, 0);
}


size_t sqlParser::SignedFloatLiteralContext::getRuleIndex() const {
  return sqlParser::RuleSignedFloatLiteral;
}


antlrcpp::Any sqlParser::SignedFloatLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSignedFloatLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SignedFloatLiteralContext* sqlParser::signedFloatLiteral() {
  SignedFloatLiteralContext *_localctx = _tracker.createInstance<SignedFloatLiteralContext>(_ctx, getState());
  enterRule(_localctx, 286, sqlParser::RuleSignedFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2055);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__14: {
        setState(2053);
        match(sqlParser::T__14);
        break;
      }

      case sqlParser::T__10: {
        setState(2054);
        dynamic_cast<SignedFloatLiteralContext *>(_localctx)->neg = match(sqlParser::T__10);
        break;
      }

      case sqlParser::FLOAT_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(2057);
    match(sqlParser::FLOAT_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DateLiteralContext ------------------------------------------------------------------

sqlParser::DateLiteralContext::DateLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DateLiteralContext::DATE() {
  return getToken(sqlParser::DATE, 0);
}

sqlParser::StringLiteralContext* sqlParser::DateLiteralContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}


size_t sqlParser::DateLiteralContext::getRuleIndex() const {
  return sqlParser::RuleDateLiteral;
}


antlrcpp::Any sqlParser::DateLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDateLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DateLiteralContext* sqlParser::dateLiteral() {
  DateLiteralContext *_localctx = _tracker.createInstance<DateLiteralContext>(_ctx, getState());
  enterRule(_localctx, 288, sqlParser::RuleDateLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2059);
    match(sqlParser::DATE);
    setState(2060);
    dynamic_cast<DateLiteralContext *>(_localctx)->str = stringLiteral();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TimeLiteralContext ------------------------------------------------------------------

sqlParser::TimeLiteralContext::TimeLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::TimeLiteralContext::TIME() {
  return getToken(sqlParser::TIME, 0);
}

sqlParser::StringLiteralContext* sqlParser::TimeLiteralContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}


size_t sqlParser::TimeLiteralContext::getRuleIndex() const {
  return sqlParser::RuleTimeLiteral;
}


antlrcpp::Any sqlParser::TimeLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTimeLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TimeLiteralContext* sqlParser::timeLiteral() {
  TimeLiteralContext *_localctx = _tracker.createInstance<TimeLiteralContext>(_ctx, getState());
  enterRule(_localctx, 290, sqlParser::RuleTimeLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2062);
    match(sqlParser::TIME);
    setState(2063);
    dynamic_cast<TimeLiteralContext *>(_localctx)->str = stringLiteral();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TimestampLiteralContext ------------------------------------------------------------------

sqlParser::TimestampLiteralContext::TimestampLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::TimestampLiteralContext::TIMESTAMP() {
  return getToken(sqlParser::TIMESTAMP, 0);
}

sqlParser::StringLiteralContext* sqlParser::TimestampLiteralContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}


size_t sqlParser::TimestampLiteralContext::getRuleIndex() const {
  return sqlParser::RuleTimestampLiteral;
}


antlrcpp::Any sqlParser::TimestampLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTimestampLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TimestampLiteralContext* sqlParser::timestampLiteral() {
  TimestampLiteralContext *_localctx = _tracker.createInstance<TimestampLiteralContext>(_ctx, getState());
  enterRule(_localctx, 292, sqlParser::RuleTimestampLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2065);
    match(sqlParser::TIMESTAMP);
    setState(2066);
    stringLiteral();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StringLiteralContext ------------------------------------------------------------------

sqlParser::StringLiteralContext::StringLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::StringLiteralContext::SINGLQUOTED_STRING() {
  return getToken(sqlParser::SINGLQUOTED_STRING, 0);
}

tree::TerminalNode* sqlParser::StringLiteralContext::DUBLQUOTED_STRING() {
  return getToken(sqlParser::DUBLQUOTED_STRING, 0);
}

tree::TerminalNode* sqlParser::StringLiteralContext::DUBLQUOTED_IDENTIFIER() {
  return getToken(sqlParser::DUBLQUOTED_IDENTIFIER, 0);
}


size_t sqlParser::StringLiteralContext::getRuleIndex() const {
  return sqlParser::RuleStringLiteral;
}


antlrcpp::Any sqlParser::StringLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitStringLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::StringLiteralContext* sqlParser::stringLiteral() {
  StringLiteralContext *_localctx = _tracker.createInstance<StringLiteralContext>(_ctx, getState());
  enterRule(_localctx, 294, sqlParser::RuleStringLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2068);
    _la = _input->LA(1);
    if (!(((((_la - 249) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 249)) & ((1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 249))
      | (1ULL << (sqlParser::SINGLQUOTED_STRING - 249))
      | (1ULL << (sqlParser::DUBLQUOTED_STRING - 249)))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntervalLiteralContext ------------------------------------------------------------------

sqlParser::IntervalLiteralContext::IntervalLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::YearMonthIntervalContext* sqlParser::IntervalLiteralContext::yearMonthInterval() {
  return getRuleContext<sqlParser::YearMonthIntervalContext>(0);
}

sqlParser::DayTimeIntervalContext* sqlParser::IntervalLiteralContext::dayTimeInterval() {
  return getRuleContext<sqlParser::DayTimeIntervalContext>(0);
}


size_t sqlParser::IntervalLiteralContext::getRuleIndex() const {
  return sqlParser::RuleIntervalLiteral;
}


antlrcpp::Any sqlParser::IntervalLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitIntervalLiteral(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::IntervalLiteralContext* sqlParser::intervalLiteral() {
  IntervalLiteralContext *_localctx = _tracker.createInstance<IntervalLiteralContext>(_ctx, getState());
  enterRule(_localctx, 296, sqlParser::RuleIntervalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2072);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 242, _ctx)) {
    case 1: {
      setState(2070);
      yearMonthInterval();
      break;
    }

    case 2: {
      setState(2071);
      dayTimeInterval();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- YearMonthIntervalContext ------------------------------------------------------------------

sqlParser::YearMonthIntervalContext::YearMonthIntervalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::YearMonthIntervalContext::INTERVAL() {
  return getToken(sqlParser::INTERVAL, 0);
}

sqlParser::StringLiteralContext* sqlParser::YearMonthIntervalContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}

tree::TerminalNode* sqlParser::YearMonthIntervalContext::TO() {
  return getToken(sqlParser::TO, 0);
}

tree::TerminalNode* sqlParser::YearMonthIntervalContext::YEAR() {
  return getToken(sqlParser::YEAR, 0);
}

tree::TerminalNode* sqlParser::YearMonthIntervalContext::MONTH() {
  return getToken(sqlParser::MONTH, 0);
}

sqlParser::PrecisionContext* sqlParser::YearMonthIntervalContext::precision() {
  return getRuleContext<sqlParser::PrecisionContext>(0);
}


size_t sqlParser::YearMonthIntervalContext::getRuleIndex() const {
  return sqlParser::RuleYearMonthInterval;
}


antlrcpp::Any sqlParser::YearMonthIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitYearMonthInterval(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::YearMonthIntervalContext* sqlParser::yearMonthInterval() {
  YearMonthIntervalContext *_localctx = _tracker.createInstance<YearMonthIntervalContext>(_ctx, getState());
  enterRule(_localctx, 298, sqlParser::RuleYearMonthInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2074);
    match(sqlParser::INTERVAL);
    setState(2077);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__14: {
        setState(2075);
        match(sqlParser::T__14);
        break;
      }

      case sqlParser::T__10: {
        setState(2076);
        dynamic_cast<YearMonthIntervalContext *>(_localctx)->neg = match(sqlParser::T__10);
        break;
      }

      case sqlParser::DUBLQUOTED_IDENTIFIER:
      case sqlParser::SINGLQUOTED_STRING:
      case sqlParser::DUBLQUOTED_STRING: {
        break;
      }

    default:
      break;
    }
    setState(2079);
    stringLiteral();
    setState(2103);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 247, _ctx)) {
    case 1: {
      setState(2080);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(2085);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 244, _ctx)) {
      case 1: {
        setState(2081);
        match(sqlParser::T__6);
        setState(2082);
        precision();
        setState(2083);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(2087);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(2092);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(2088);
        match(sqlParser::T__6);
        setState(2089);
        precision();
        setState(2090);
        match(sqlParser::T__7);
      }
      setState(2094);
      match(sqlParser::TO);
      setState(2095);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      break;
    }

    case 3: {
      setState(2096);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      setState(2101);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 246, _ctx)) {
      case 1: {
        setState(2097);
        match(sqlParser::T__6);
        setState(2098);
        precision();
        setState(2099);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DayTimeIntervalContext ------------------------------------------------------------------

sqlParser::DayTimeIntervalContext::DayTimeIntervalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DayTimeIntervalContext::INTERVAL() {
  return getToken(sqlParser::INTERVAL, 0);
}

sqlParser::StringLiteralContext* sqlParser::DayTimeIntervalContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}

tree::TerminalNode* sqlParser::DayTimeIntervalContext::TO() {
  return getToken(sqlParser::TO, 0);
}

tree::TerminalNode* sqlParser::DayTimeIntervalContext::DAY() {
  return getToken(sqlParser::DAY, 0);
}

tree::TerminalNode* sqlParser::DayTimeIntervalContext::HOUR() {
  return getToken(sqlParser::HOUR, 0);
}

tree::TerminalNode* sqlParser::DayTimeIntervalContext::MINUTE() {
  return getToken(sqlParser::MINUTE, 0);
}

tree::TerminalNode* sqlParser::DayTimeIntervalContext::SECOND() {
  return getToken(sqlParser::SECOND, 0);
}

sqlParser::PrecisionContext* sqlParser::DayTimeIntervalContext::precision() {
  return getRuleContext<sqlParser::PrecisionContext>(0);
}

sqlParser::ScaleContext* sqlParser::DayTimeIntervalContext::scale() {
  return getRuleContext<sqlParser::ScaleContext>(0);
}


size_t sqlParser::DayTimeIntervalContext::getRuleIndex() const {
  return sqlParser::RuleDayTimeInterval;
}


antlrcpp::Any sqlParser::DayTimeIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDayTimeInterval(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DayTimeIntervalContext* sqlParser::dayTimeInterval() {
  DayTimeIntervalContext *_localctx = _tracker.createInstance<DayTimeIntervalContext>(_ctx, getState());
  enterRule(_localctx, 300, sqlParser::RuleDayTimeInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2105);
    match(sqlParser::INTERVAL);
    setState(2108);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__14: {
        setState(2106);
        match(sqlParser::T__14);
        break;
      }

      case sqlParser::T__10: {
        setState(2107);
        dynamic_cast<DayTimeIntervalContext *>(_localctx)->neg = match(sqlParser::T__10);
        break;
      }

      case sqlParser::DUBLQUOTED_IDENTIFIER:
      case sqlParser::SINGLQUOTED_STRING:
      case sqlParser::DUBLQUOTED_STRING: {
        break;
      }

    default:
      break;
    }
    setState(2110);
    stringLiteral();
    setState(2195);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 262, _ctx)) {
    case 1: {
      setState(2111);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(2116);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 249, _ctx)) {
      case 1: {
        setState(2112);
        match(sqlParser::T__6);
        setState(2113);
        precision();
        setState(2114);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(2118);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(2123);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(2119);
        match(sqlParser::T__6);
        setState(2120);
        precision();
        setState(2121);
        match(sqlParser::T__7);
      }
      setState(2125);
      match(sqlParser::TO);
      setState(2135);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::HOUR: {
          setState(2126);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
          break;
        }

        case sqlParser::MINUTE: {
          setState(2127);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(2128);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(2133);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 251, _ctx)) {
          case 1: {
            setState(2129);
            match(sqlParser::T__6);
            setState(2130);
            scale();
            setState(2131);
            match(sqlParser::T__7);
            break;
          }

          }
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      break;
    }

    case 3: {
      setState(2137);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(2142);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(2138);
        match(sqlParser::T__6);
        setState(2139);
        precision();
        setState(2140);
        match(sqlParser::T__7);
      }
      setState(2144);
      match(sqlParser::TO);
      setState(2153);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::MINUTE: {
          setState(2145);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(2146);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(2151);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 254, _ctx)) {
          case 1: {
            setState(2147);
            match(sqlParser::T__6);
            setState(2148);
            scale();
            setState(2149);
            match(sqlParser::T__7);
            break;
          }

          }
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      break;
    }

    case 4: {
      setState(2155);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(2160);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(2156);
        match(sqlParser::T__6);
        setState(2157);
        precision();
        setState(2158);
        match(sqlParser::T__7);
      }
      setState(2162);
      match(sqlParser::TO);

      setState(2163);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(2168);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 257, _ctx)) {
      case 1: {
        setState(2164);
        match(sqlParser::T__6);
        setState(2165);
        scale();
        setState(2166);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 5: {
      setState(2170);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(2175);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 258, _ctx)) {
      case 1: {
        setState(2171);
        match(sqlParser::T__6);
        setState(2172);
        precision();
        setState(2173);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 6: {
      setState(2177);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(2182);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 259, _ctx)) {
      case 1: {
        setState(2178);
        match(sqlParser::T__6);
        setState(2179);
        precision();
        setState(2180);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 7: {
      setState(2184);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(2193);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 261, _ctx)) {
      case 1: {
        setState(2185);
        match(sqlParser::T__6);
        setState(2186);
        precision();
        setState(2189);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(2187);
          match(sqlParser::T__5);
          setState(2188);
          scale();
        }
        setState(2191);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NonReservedWordContext ------------------------------------------------------------------

sqlParser::NonReservedWordContext::NonReservedWordContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::NonReservedWordContext::ANALYSE() {
  return getToken(sqlParser::ANALYSE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::ANALYZE() {
  return getToken(sqlParser::ANALYZE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::ANIDX() {
  return getToken(sqlParser::ANIDX, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::ANTBL() {
  return getToken(sqlParser::ANTBL, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::ASC() {
  return getToken(sqlParser::ASC, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::BIT() {
  return getToken(sqlParser::BIT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::BITS() {
  return getToken(sqlParser::BITS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::BLOCK() {
  return getToken(sqlParser::BLOCK, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::BLOOM() {
  return getToken(sqlParser::BLOOM, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::BPA() {
  return getToken(sqlParser::BPA, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::BTREE() {
  return getToken(sqlParser::BTREE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::BYE() {
  return getToken(sqlParser::BYE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::BYTE() {
  return getToken(sqlParser::BYTE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::BYTES() {
  return getToken(sqlParser::BYTES, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::CATALOG() {
  return getToken(sqlParser::CATALOG, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::CHARS() {
  return getToken(sqlParser::CHARS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::COMPACTIDX() {
  return getToken(sqlParser::COMPACTIDX, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::CONCAT() {
  return getToken(sqlParser::CONCAT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DATA() {
  return getToken(sqlParser::DATA, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DATE_FORMAT() {
  return getToken(sqlParser::DATE_FORMAT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DAYS() {
  return getToken(sqlParser::DAYS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DECODE() {
  return getToken(sqlParser::DECODE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DESC() {
  return getToken(sqlParser::DESC, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DETAIL() {
  return getToken(sqlParser::DETAIL, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DIGITS() {
  return getToken(sqlParser::DIGITS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DLOAD() {
  return getToken(sqlParser::DLOAD, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DOCOL() {
  return getToken(sqlParser::DOCOL, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::ECHO() {
  return getToken(sqlParser::ECHO, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::ESCAPE_CHAR() {
  return getToken(sqlParser::ESCAPE_CHAR, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::EXIT_TOKEN() {
  return getToken(sqlParser::EXIT_TOKEN, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::EXPLAIN() {
  return getToken(sqlParser::EXPLAIN, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::EXPORT() {
  return getToken(sqlParser::EXPORT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SQL_FALSE() {
  return getToken(sqlParser::SQL_FALSE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::FEDERATION() {
  return getToken(sqlParser::FEDERATION, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::FIELD_DELIMITER() {
  return getToken(sqlParser::FIELD_DELIMITER, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::FILE() {
  return getToken(sqlParser::FILE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::FORCE() {
  return getToken(sqlParser::FORCE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::FORMAT() {
  return getToken(sqlParser::FORMAT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::HASH() {
  return getToken(sqlParser::HASH, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::HASHKEY() {
  return getToken(sqlParser::HASHKEY, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::HOURS() {
  return getToken(sqlParser::HOURS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::IF() {
  return getToken(sqlParser::IF, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::IFNULL() {
  return getToken(sqlParser::IFNULL, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::INDEX() {
  return getToken(sqlParser::INDEX, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::INTERVAL_DT() {
  return getToken(sqlParser::INTERVAL_DT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::INTERVAL_YM() {
  return getToken(sqlParser::INTERVAL_YM, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::KEY() {
  return getToken(sqlParser::KEY, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::LIMIT() {
  return getToken(sqlParser::LIMIT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::LOAD() {
  return getToken(sqlParser::LOAD, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::LOADREPLICATED() {
  return getToken(sqlParser::LOADREPLICATED, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::LOG() {
  return getToken(sqlParser::LOG, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::LOOP() {
  return getToken(sqlParser::LOOP, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::LSIDX() {
  return getToken(sqlParser::LSIDX, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::LSPTNCOL() {
  return getToken(sqlParser::LSPTNCOL, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::LSTBL() {
  return getToken(sqlParser::LSTBL, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MEMORYLEAKS() {
  return getToken(sqlParser::MEMORYLEAKS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MILLIS() {
  return getToken(sqlParser::MILLIS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MILLISECOND() {
  return getToken(sqlParser::MILLISECOND, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MILLISECONDS() {
  return getToken(sqlParser::MILLISECONDS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MINUTES() {
  return getToken(sqlParser::MINUTES, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MONOTONIC() {
  return getToken(sqlParser::MONOTONIC, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MONTHS() {
  return getToken(sqlParser::MONTHS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MS() {
  return getToken(sqlParser::MS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::NODE() {
  return getToken(sqlParser::NODE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::NODES() {
  return getToken(sqlParser::NODES, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::NOOP() {
  return getToken(sqlParser::NOOP, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::NOP() {
  return getToken(sqlParser::NOP, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::NOW() {
  return getToken(sqlParser::NOW, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::OBJECTS() {
  return getToken(sqlParser::OBJECTS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::OFF() {
  return getToken(sqlParser::OFF, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::OFFSET() {
  return getToken(sqlParser::OFFSET, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::ORIGIN() {
  return getToken(sqlParser::ORIGIN, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::PARTITIONS() {
  return getToken(sqlParser::PARTITIONS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::PATHS() {
  return getToken(sqlParser::PATHS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::PIPELINE() {
  return getToken(sqlParser::PIPELINE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::PLACING() {
  return getToken(sqlParser::PLACING, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::PRETTY() {
  return getToken(sqlParser::PRETTY, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::QUARTER() {
  return getToken(sqlParser::QUARTER, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::QUARTERS() {
  return getToken(sqlParser::QUARTERS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::RECORD_ENDS() {
  return getToken(sqlParser::RECORD_ENDS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::REFERENCE() {
  return getToken(sqlParser::REFERENCE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::REGIONS() {
  return getToken(sqlParser::REGIONS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::REPLICATED() {
  return getToken(sqlParser::REPLICATED, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::ROWSIZE() {
  return getToken(sqlParser::ROWSIZE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::S_ONLY() {
  return getToken(sqlParser::S_ONLY, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SCHEMA() {
  return getToken(sqlParser::SCHEMA, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SEC() {
  return getToken(sqlParser::SEC, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SECONDS() {
  return getToken(sqlParser::SECONDS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SKIPLIST() {
  return getToken(sqlParser::SKIPLIST, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SLEEP() {
  return getToken(sqlParser::SLEEP, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SPACE() {
  return getToken(sqlParser::SPACE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::STAGE() {
  return getToken(sqlParser::STAGE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::STATS() {
  return getToken(sqlParser::STATS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::STATISTICS() {
  return getToken(sqlParser::STATISTICS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::STATUS() {
  return getToken(sqlParser::STATUS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SUBSTR() {
  return getToken(sqlParser::SUBSTR, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SZIDX() {
  return getToken(sqlParser::SZIDX, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SZTBL() {
  return getToken(sqlParser::SZTBL, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::TABLESIZE() {
  return getToken(sqlParser::TABLESIZE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::TEXT() {
  return getToken(sqlParser::TEXT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::TOP() {
  return getToken(sqlParser::TOP, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::TRACKMEMORY() {
  return getToken(sqlParser::TRACKMEMORY, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::TREE() {
  return getToken(sqlParser::TREE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::TRUNCATE() {
  return getToken(sqlParser::TRUNCATE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::TYPE() {
  return getToken(sqlParser::TYPE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::TYPEOF() {
  return getToken(sqlParser::TYPEOF, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::UNLOAD() {
  return getToken(sqlParser::UNLOAD, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::VALUE() {
  return getToken(sqlParser::VALUE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::WEEK() {
  return getToken(sqlParser::WEEK, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::WEEKS() {
  return getToken(sqlParser::WEEKS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::WITH_NAMES() {
  return getToken(sqlParser::WITH_NAMES, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::WITH_TYPES() {
  return getToken(sqlParser::WITH_TYPES, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::XOR() {
  return getToken(sqlParser::XOR, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::YEARS() {
  return getToken(sqlParser::YEARS, 0);
}


size_t sqlParser::NonReservedWordContext::getRuleIndex() const {
  return sqlParser::RuleNonReservedWord;
}


antlrcpp::Any sqlParser::NonReservedWordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitNonReservedWord(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::NonReservedWordContext* sqlParser::nonReservedWord() {
  NonReservedWordContext *_localctx = _tracker.createInstance<NonReservedWordContext>(_ctx, getState());
  enterRule(_localctx, 302, sqlParser::RuleNonReservedWord);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2197);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::ANALYSE)
      | (1ULL << sqlParser::ANALYZE)
      | (1ULL << sqlParser::ANIDX)
      | (1ULL << sqlParser::ANTBL)
      | (1ULL << sqlParser::ASC)
      | (1ULL << sqlParser::BIT)
      | (1ULL << sqlParser::BITS)
      | (1ULL << sqlParser::BLOCK)
      | (1ULL << sqlParser::BLOOM)
      | (1ULL << sqlParser::BPA)
      | (1ULL << sqlParser::BTREE)
      | (1ULL << sqlParser::BYE)
      | (1ULL << sqlParser::BYTE)
      | (1ULL << sqlParser::BYTES)
      | (1ULL << sqlParser::CATALOG)
      | (1ULL << sqlParser::CHARS)
      | (1ULL << sqlParser::COMPACTIDX)
      | (1ULL << sqlParser::CONCAT))) != 0) || ((((_la - 68) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 68)) & ((1ULL << (sqlParser::DATA - 68))
      | (1ULL << (sqlParser::DATE_FORMAT - 68))
      | (1ULL << (sqlParser::DAYS - 68))
      | (1ULL << (sqlParser::DECODE - 68))
      | (1ULL << (sqlParser::DESC - 68))
      | (1ULL << (sqlParser::DETAIL - 68))
      | (1ULL << (sqlParser::DIGITS - 68))
      | (1ULL << (sqlParser::DLOAD - 68))
      | (1ULL << (sqlParser::DOCOL - 68))
      | (1ULL << (sqlParser::ECHO - 68))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 68))
      | (1ULL << (sqlParser::EXIT_TOKEN - 68))
      | (1ULL << (sqlParser::EXPLAIN - 68))
      | (1ULL << (sqlParser::EXPORT - 68))
      | (1ULL << (sqlParser::SQL_FALSE - 68))
      | (1ULL << (sqlParser::FEDERATION - 68))
      | (1ULL << (sqlParser::FIELD_DELIMITER - 68))
      | (1ULL << (sqlParser::FILE - 68))
      | (1ULL << (sqlParser::FORCE - 68))
      | (1ULL << (sqlParser::FORMAT - 68))
      | (1ULL << (sqlParser::HASH - 68))
      | (1ULL << (sqlParser::HASHKEY - 68))
      | (1ULL << (sqlParser::HOURS - 68))
      | (1ULL << (sqlParser::IF - 68))
      | (1ULL << (sqlParser::IFNULL - 68))
      | (1ULL << (sqlParser::INDEX - 68))
      | (1ULL << (sqlParser::INTERVAL_DT - 68))
      | (1ULL << (sqlParser::INTERVAL_YM - 68))
      | (1ULL << (sqlParser::KEY - 68))
      | (1ULL << (sqlParser::LIMIT - 68))
      | (1ULL << (sqlParser::LOADREPLICATED - 68)))) != 0) || ((((_la - 132) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 132)) & ((1ULL << (sqlParser::LOAD - 132))
      | (1ULL << (sqlParser::LOG - 132))
      | (1ULL << (sqlParser::LOOP - 132))
      | (1ULL << (sqlParser::LSIDX - 132))
      | (1ULL << (sqlParser::LSPTNCOL - 132))
      | (1ULL << (sqlParser::LSTBL - 132))
      | (1ULL << (sqlParser::MEMORYLEAKS - 132))
      | (1ULL << (sqlParser::MILLIS - 132))
      | (1ULL << (sqlParser::MILLISECOND - 132))
      | (1ULL << (sqlParser::MILLISECONDS - 132))
      | (1ULL << (sqlParser::MINUTES - 132))
      | (1ULL << (sqlParser::MONOTONIC - 132))
      | (1ULL << (sqlParser::MONTHS - 132))
      | (1ULL << (sqlParser::MS - 132))
      | (1ULL << (sqlParser::NODES - 132))
      | (1ULL << (sqlParser::NODE - 132))
      | (1ULL << (sqlParser::NOOP - 132))
      | (1ULL << (sqlParser::NOP - 132))
      | (1ULL << (sqlParser::NOW - 132))
      | (1ULL << (sqlParser::OBJECTS - 132))
      | (1ULL << (sqlParser::OFF - 132))
      | (1ULL << (sqlParser::OFFSET - 132))
      | (1ULL << (sqlParser::ORIGIN - 132))
      | (1ULL << (sqlParser::PARTITIONS - 132))
      | (1ULL << (sqlParser::PATHS - 132))
      | (1ULL << (sqlParser::PIPELINE - 132))
      | (1ULL << (sqlParser::PLACING - 132))
      | (1ULL << (sqlParser::PRETTY - 132))
      | (1ULL << (sqlParser::QUARTER - 132))
      | (1ULL << (sqlParser::QUARTERS - 132))
      | (1ULL << (sqlParser::RECORD_ENDS - 132))
      | (1ULL << (sqlParser::REFERENCE - 132))
      | (1ULL << (sqlParser::REGIONS - 132))
      | (1ULL << (sqlParser::REPLICATED - 132))
      | (1ULL << (sqlParser::ROWSIZE - 132))
      | (1ULL << (sqlParser::SCHEMA - 132))
      | (1ULL << (sqlParser::SEC - 132))
      | (1ULL << (sqlParser::SECONDS - 132)))) != 0) || ((((_la - 197) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 197)) & ((1ULL << (sqlParser::SKIPLIST - 197))
      | (1ULL << (sqlParser::SLEEP - 197))
      | (1ULL << (sqlParser::SPACE - 197))
      | (1ULL << (sqlParser::STAGE - 197))
      | (1ULL << (sqlParser::STATS - 197))
      | (1ULL << (sqlParser::STATISTICS - 197))
      | (1ULL << (sqlParser::STATUS - 197))
      | (1ULL << (sqlParser::SUBSTR - 197))
      | (1ULL << (sqlParser::SZIDX - 197))
      | (1ULL << (sqlParser::SZTBL - 197))
      | (1ULL << (sqlParser::S_ONLY - 197))
      | (1ULL << (sqlParser::TABLESIZE - 197))
      | (1ULL << (sqlParser::TEXT - 197))
      | (1ULL << (sqlParser::TOP - 197))
      | (1ULL << (sqlParser::TRACKMEMORY - 197))
      | (1ULL << (sqlParser::TREE - 197))
      | (1ULL << (sqlParser::TRUNCATE - 197))
      | (1ULL << (sqlParser::TYPE - 197))
      | (1ULL << (sqlParser::TYPEOF - 197))
      | (1ULL << (sqlParser::UNLOAD - 197))
      | (1ULL << (sqlParser::VALUE - 197))
      | (1ULL << (sqlParser::WEEK - 197))
      | (1ULL << (sqlParser::WEEKS - 197))
      | (1ULL << (sqlParser::WITH_NAMES - 197))
      | (1ULL << (sqlParser::WITH_TYPES - 197))
      | (1ULL << (sqlParser::XOR - 197))
      | (1ULL << (sqlParser::YEARS - 197)))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool sqlParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 14: return setExprSempred(dynamic_cast<SetExprContext *>(context), predicateIndex);
    case 17: return tableExprSempred(dynamic_cast<TableExprContext *>(context), predicateIndex);
    case 110: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool sqlParser::setExprSempred(SetExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

bool sqlParser::tableExprSempred(TableExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 1: return precpred(_ctx, 6);
    case 2: return precpred(_ctx, 5);
    case 3: return precpred(_ctx, 4);
    case 4: return precpred(_ctx, 3);

  default:
    break;
  }
  return true;
}

bool sqlParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 5: return precpred(_ctx, 37);
    case 6: return precpred(_ctx, 36);
    case 7: return precpred(_ctx, 35);
    case 8: return precpred(_ctx, 10);
    case 9: return precpred(_ctx, 9);
    case 10: return precpred(_ctx, 3);
    case 11: return !(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent));
    case 12: return precpred(_ctx, 2);
    case 13: return precpred(_ctx, 1);
    case 14: return precpred(_ctx, 38);
    case 15: return precpred(_ctx, 11);
    case 16: return precpred(_ctx, 7);
    case 17: return precpred(_ctx, 6);
    case 18: return precpred(_ctx, 5);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> sqlParser::_decisionToDFA;
atn::PredictionContextCache sqlParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN sqlParser::_atn;
std::vector<uint16_t> sqlParser::_serializedATN;

std::vector<std::string> sqlParser::_ruleNames = {
  "statements", "statement", "statementChoice", "tags", "keyValueTag", "keyOnlyTag", 
  "tagName", "queryCmd", "query", "queryHint", "hint", "selectQuery", "selectExpr", 
  "setQuery", "setExpr", "corrClause", "withQuery", "tableExpr", "tableReference", 
  "joinType", "extendedJoinType", "valuesExpr", "anidxCmd", "antblCmd", 
  "bpaCmd", "compactIdxCmd", "countCmd", "createCmd", "createTableCmd", 
  "createIndexCmd", "deleteCmd", "disconnectCmd", "dloadCmd", "docolCmd", 
  "dropCmd", "dropTableCmd", "dropIndexCmd", "emptyCmd", "explainCmd", "explainOptions", 
  "explainStageList", "explainRange", "explainRangeChoice", "explainFormat", 
  "exportCmd", "fileCmd", "insertCmd", "loadCmd", "loadReplicatedCmd", "loadOption", 
  "loadOptionKeys", "logCmd", "lscolCmd", "lsidxCmd", "lsidxcolCmd", "lsptncolCmd", 
  "lstblCmd", "memoryleaksCmd", "noopCmd", "pipelineCmd", "pipelineStage", 
  "pipelineOpSeq", "pipelineOperator", "pipelineOpParam", "pipelineTypeNames", 
  "sleepCmd", "sleepUnits", "spaceCmd", "statusCmd", "szidxCmd", "sztblCmd", 
  "tableSizesCmd", "truncateCmd", "trackmemoryCmd", "updateCmd", "unloadCmd", 
  "tableDefinition", "tablePartitioning", "columnDefinition", "columnNameList", 
  "indexDefinition", "indexMethod", "dataType", "sqlDataType", "intervalType", 
  "yearMonthIntervalType", "dayTimeIntervalType", "fixedLenChar", "varyingLenChar", 
  "asSubquery", "selectOrValues", "selectList", "selectListItem", "groupByList", 
  "groupByListItem", "orderByList", "orderByListItem", "joinUsingList", 
  "joinUsingListItem", "corrList", "corrListItem", "colAliases", "colAliasList", 
  "colAliasListItem", "propertyList", "property", "valueList", "predicate", 
  "joinPredicate", "casePredicate", "expr", "variableName", "columnRef", 
  "columnWildcard", "className", "functionName", "methodName", "catalogName", 
  "schemaName", "tableName", "tableAlias", "columnName", "columnAlias", 
  "collationName", "indexName", "propertyName", "hintName", "exprHint", 
  "propertyValue", "hintValue", "identifier", "qualifier", "precision", 
  "scale", "size", "literal", "simpleLiteral", "booleanLiteral", "integerLiteral", 
  "signedIntegerLiteral", "decimalLiteral", "signedDecimalLiteral", "floatLiteral", 
  "signedFloatLiteral", "dateLiteral", "timeLiteral", "timestampLiteral", 
  "stringLiteral", "intervalLiteral", "yearMonthInterval", "dayTimeInterval", 
  "nonReservedWord"
};

std::vector<std::string> sqlParser::_literalNames = {
  "", "';'", "'<'", "'>'", "'='", "'/'", "','", "'('", "')'", "'?'", "'.'", 
  "'-'", "'*'", "':'", "'|'", "'+'", "'%'", "'=='", "'||'", "'<='", "'!='", 
  "'<>'", "'>='", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "'/*+'", "'*/'"
};

std::vector<std::string> sqlParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "ALL", "ANALYSE", "ANALYZE", "AND", "ANIDX", "ANTBL", 
  "ANTI", "ANY", "AS", "ASC", "AVG", "BEGIN", "BETWEEN", "BINARY", "BIT", 
  "BITS", "BLOB", "BLOCK", "BLOOM", "BOTH", "BOOLEAN", "BPA", "BTREE", "BYE", 
  "BY", "BYTE", "BYTES", "CASE", "CAST", "CATALOG", "CHAR", "CHARS", "CHARACTER", 
  "COALESCE", "COLLATE", "COMPACTIDX", "CONCAT", "CORRESPONDING", "COUNT", 
  "CREATE", "CROSS", "CURRENT_CATALOG", "CURRENT_SCHEMA", "CURRENT_TIMESTAMP", 
  "CURRENT_USER", "DATA", "DATE", "DATE_FORMAT", "DAY", "DAYS", "DECIMAL", 
  "DECODE", "DELETE", "DESC", "DETAIL", "DIGITS", "DISCONNECT", "DISTINCT", 
  "DLOAD", "DOCOL", "DROP", "ECHO", "ELSE", "END", "ESCAPE", "ESCAPE_CHAR", 
  "EXCEPT", "EXISTS", "EXIT_TOKEN", "EXPLAIN", "EXPORT", "EXTRACT", "SQL_FALSE", 
  "FEDERATION", "FIELD_DELIMITER", "FILE", "FLOAT", "FOR", "FORCE", "FORMAT", 
  "FULL", "FROM", "GROUP", "HASH", "HASHKEY", "HAVING", "HEAP", "HOUR", 
  "HOURS", "IF", "IFNULL", "IN", "INDEX", "INNER", "INSERT", "INTEGER", 
  "INTERSECT", "INTERVAL_DT", "INTERVAL_YM", "INTERVAL", "INTO", "IS", "JOIN", 
  "KEY", "LEADING", "LEFT", "LIKE", "LIMIT", "LOADREPLICATED", "LOAD", "LOG", 
  "LOOP", "LSCOL", "LSIDXCOL", "LSIDX", "LSPTNCOL", "LSTBL", "MAX", "MEMORYLEAKS", 
  "MILLIS", "MILLISECOND", "MILLISECONDS", "MIN", "MINUTE", "MINUTES", "MONOTONIC", 
  "MONTH", "MONTHS", "MS", "NATURAL", "NO", "NODES", "NODE", "NOOP", "NOP", 
  "NOT", "NOW", "SQL_NULL", "NULLIF", "OBJECTS", "OFF", "OFFSET", "ON", 
  "OR", "ORDER", "ORIGIN", "OUTER", "OVER", "OVERLAY", "PARTITIONS", "PARTITION", 
  "PATHS", "PIPELINE", "PLACING", "POSITION", "PRETTY", "PRIMARY", "QUARTER", 
  "QUARTERS", "RECORD_ENDS", "RECURSIVE", "REFERENCE", "REGIONS", "REPLICATED", 
  "RIGHT", "ROW", "ROWSIZE", "SCHEMA", "SEC", "SECOND", "SECONDS", "SELECT", 
  "SEMI", "SET", "SKIPLIST", "SLEEP", "SPACE", "STAGE", "STATS", "STATISTICS", 
  "STATUS", "SUBSTR", "SUBSTRING", "SUM", "SZIDX", "SZTBL", "S_ONLY", "TABLE", 
  "TABLESIZES", "TABLESIZE", "TEXT", "THEN", "TIME", "TIMESTAMP", "TO", 
  "TOP", "TRACKMEMORY", "TREE", "TRIM", "TRAILING", "TRUNCATE", "SQL_TRUE", 
  "TYPE", "TYPEOF", "UNION", "UNIQUE", "UNLOAD", "UPDATE", "USING", "VALUE", 
  "VALUES", "VARBINARY", "VARCHAR", "VARYING", "WEEK", "WEEKS", "WHEN", 
  "WHERE", "WITH_NAMES", "WITH_TYPES", "WITH", "XOR", "YEAR", "YEARS", "PLAIN_IDENTIFIER", 
  "BAKTIKD_IDENTIFIER", "DUBLQUOTED_IDENTIFIER", "PARAM_IDENTIFIER", "SINGLQUOTED_STRING", 
  "DUBLQUOTED_STRING", "FLOAT_LITERAL", "DECIMAL_LITERAL", "INTEGER_LITERAL", 
  "HINT_START", "HINT_END", "SLASH_COMMENT", "SQL_COMMENT", "BLOCK_COMMENT", 
  "WHITESPACE"
};

dfa::Vocabulary sqlParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> sqlParser::_tokenNames;

sqlParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x107, 0x89a, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 
    0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15, 
    0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 0x18, 0x9, 
    0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 
    0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4, 
    0x1f, 0x9, 0x1f, 0x4, 0x20, 0x9, 0x20, 0x4, 0x21, 0x9, 0x21, 0x4, 0x22, 
    0x9, 0x22, 0x4, 0x23, 0x9, 0x23, 0x4, 0x24, 0x9, 0x24, 0x4, 0x25, 0x9, 
    0x25, 0x4, 0x26, 0x9, 0x26, 0x4, 0x27, 0x9, 0x27, 0x4, 0x28, 0x9, 0x28, 
    0x4, 0x29, 0x9, 0x29, 0x4, 0x2a, 0x9, 0x2a, 0x4, 0x2b, 0x9, 0x2b, 0x4, 
    0x2c, 0x9, 0x2c, 0x4, 0x2d, 0x9, 0x2d, 0x4, 0x2e, 0x9, 0x2e, 0x4, 0x2f, 
    0x9, 0x2f, 0x4, 0x30, 0x9, 0x30, 0x4, 0x31, 0x9, 0x31, 0x4, 0x32, 0x9, 
    0x32, 0x4, 0x33, 0x9, 0x33, 0x4, 0x34, 0x9, 0x34, 0x4, 0x35, 0x9, 0x35, 
    0x4, 0x36, 0x9, 0x36, 0x4, 0x37, 0x9, 0x37, 0x4, 0x38, 0x9, 0x38, 0x4, 
    0x39, 0x9, 0x39, 0x4, 0x3a, 0x9, 0x3a, 0x4, 0x3b, 0x9, 0x3b, 0x4, 0x3c, 
    0x9, 0x3c, 0x4, 0x3d, 0x9, 0x3d, 0x4, 0x3e, 0x9, 0x3e, 0x4, 0x3f, 0x9, 
    0x3f, 0x4, 0x40, 0x9, 0x40, 0x4, 0x41, 0x9, 0x41, 0x4, 0x42, 0x9, 0x42, 
    0x4, 0x43, 0x9, 0x43, 0x4, 0x44, 0x9, 0x44, 0x4, 0x45, 0x9, 0x45, 0x4, 
    0x46, 0x9, 0x46, 0x4, 0x47, 0x9, 0x47, 0x4, 0x48, 0x9, 0x48, 0x4, 0x49, 
    0x9, 0x49, 0x4, 0x4a, 0x9, 0x4a, 0x4, 0x4b, 0x9, 0x4b, 0x4, 0x4c, 0x9, 
    0x4c, 0x4, 0x4d, 0x9, 0x4d, 0x4, 0x4e, 0x9, 0x4e, 0x4, 0x4f, 0x9, 0x4f, 
    0x4, 0x50, 0x9, 0x50, 0x4, 0x51, 0x9, 0x51, 0x4, 0x52, 0x9, 0x52, 0x4, 
    0x53, 0x9, 0x53, 0x4, 0x54, 0x9, 0x54, 0x4, 0x55, 0x9, 0x55, 0x4, 0x56, 
    0x9, 0x56, 0x4, 0x57, 0x9, 0x57, 0x4, 0x58, 0x9, 0x58, 0x4, 0x59, 0x9, 
    0x59, 0x4, 0x5a, 0x9, 0x5a, 0x4, 0x5b, 0x9, 0x5b, 0x4, 0x5c, 0x9, 0x5c, 
    0x4, 0x5d, 0x9, 0x5d, 0x4, 0x5e, 0x9, 0x5e, 0x4, 0x5f, 0x9, 0x5f, 0x4, 
    0x60, 0x9, 0x60, 0x4, 0x61, 0x9, 0x61, 0x4, 0x62, 0x9, 0x62, 0x4, 0x63, 
    0x9, 0x63, 0x4, 0x64, 0x9, 0x64, 0x4, 0x65, 0x9, 0x65, 0x4, 0x66, 0x9, 
    0x66, 0x4, 0x67, 0x9, 0x67, 0x4, 0x68, 0x9, 0x68, 0x4, 0x69, 0x9, 0x69, 
    0x4, 0x6a, 0x9, 0x6a, 0x4, 0x6b, 0x9, 0x6b, 0x4, 0x6c, 0x9, 0x6c, 0x4, 
    0x6d, 0x9, 0x6d, 0x4, 0x6e, 0x9, 0x6e, 0x4, 0x6f, 0x9, 0x6f, 0x4, 0x70, 
    0x9, 0x70, 0x4, 0x71, 0x9, 0x71, 0x4, 0x72, 0x9, 0x72, 0x4, 0x73, 0x9, 
    0x73, 0x4, 0x74, 0x9, 0x74, 0x4, 0x75, 0x9, 0x75, 0x4, 0x76, 0x9, 0x76, 
    0x4, 0x77, 0x9, 0x77, 0x4, 0x78, 0x9, 0x78, 0x4, 0x79, 0x9, 0x79, 0x4, 
    0x7a, 0x9, 0x7a, 0x4, 0x7b, 0x9, 0x7b, 0x4, 0x7c, 0x9, 0x7c, 0x4, 0x7d, 
    0x9, 0x7d, 0x4, 0x7e, 0x9, 0x7e, 0x4, 0x7f, 0x9, 0x7f, 0x4, 0x80, 0x9, 
    0x80, 0x4, 0x81, 0x9, 0x81, 0x4, 0x82, 0x9, 0x82, 0x4, 0x83, 0x9, 0x83, 
    0x4, 0x84, 0x9, 0x84, 0x4, 0x85, 0x9, 0x85, 0x4, 0x86, 0x9, 0x86, 0x4, 
    0x87, 0x9, 0x87, 0x4, 0x88, 0x9, 0x88, 0x4, 0x89, 0x9, 0x89, 0x4, 0x8a, 
    0x9, 0x8a, 0x4, 0x8b, 0x9, 0x8b, 0x4, 0x8c, 0x9, 0x8c, 0x4, 0x8d, 0x9, 
    0x8d, 0x4, 0x8e, 0x9, 0x8e, 0x4, 0x8f, 0x9, 0x8f, 0x4, 0x90, 0x9, 0x90, 
    0x4, 0x91, 0x9, 0x91, 0x4, 0x92, 0x9, 0x92, 0x4, 0x93, 0x9, 0x93, 0x4, 
    0x94, 0x9, 0x94, 0x4, 0x95, 0x9, 0x95, 0x4, 0x96, 0x9, 0x96, 0x4, 0x97, 
    0x9, 0x97, 0x4, 0x98, 0x9, 0x98, 0x4, 0x99, 0x9, 0x99, 0x3, 0x2, 0x6, 
    0x2, 0x134, 0xa, 0x2, 0xd, 0x2, 0xe, 0x2, 0x135, 0x3, 0x2, 0x3, 0x2, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x162, 0xa, 
    0x4, 0x3, 0x5, 0x3, 0x5, 0x7, 0x5, 0x166, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 
    0x169, 0xb, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x5, 0x6, 0x171, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
    0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 0x179, 0xa, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 
    0x9, 0x3, 0x9, 0x3, 0xa, 0x7, 0xa, 0x180, 0xa, 0xa, 0xc, 0xa, 0xe, 0xa, 
    0x183, 0xb, 0xa, 0x3, 0xa, 0x3, 0xa, 0x5, 0xa, 0x187, 0xa, 0xa, 0x3, 
    0xb, 0x3, 0xb, 0x5, 0xb, 0x18b, 0xa, 0xb, 0x3, 0xb, 0x3, 0xb, 0x7, 0xb, 
    0x18f, 0xa, 0xb, 0xc, 0xb, 0xe, 0xb, 0x192, 0xb, 0xb, 0x3, 0xb, 0x3, 
    0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 
    0xc, 0x5, 0xc, 0x19d, 0xa, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 
    0x7, 0xc, 0x1a3, 0xa, 0xc, 0xc, 0xc, 0xe, 0xc, 0x1a6, 0xb, 0xc, 0x3, 
    0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 
    0xc, 0x5, 0xc, 0x1b0, 0xa, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 
    0x7, 0xc, 0x1b6, 0xa, 0xc, 0xc, 0xc, 0xe, 0xc, 0x1b9, 0xb, 0xc, 0x3, 
    0xc, 0x3, 0xc, 0x5, 0xc, 0x1bd, 0xa, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
    0x3, 0xd, 0x7, 0xd, 0x1c3, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 0x1c6, 0xb, 
    0xd, 0x5, 0xd, 0x1c8, 0xa, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
    0x5, 0xd, 0x1ce, 0xa, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x5, 
    0xd, 0x1d4, 0xa, 0xd, 0x5, 0xd, 0x1d6, 0xa, 0xd, 0x3, 0xe, 0x3, 0xe, 
    0x3, 0xe, 0x5, 0xe, 0x1db, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 
    0xe, 0x1e0, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0x1e4, 0xa, 0xe, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0x1e9, 0xa, 0xe, 0x3, 0xe, 0x3, 
    0xe, 0x5, 0xe, 0x1ed, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x5, 0xe, 0x1f3, 0xa, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x7, 
    0xf, 0x1f9, 0xa, 0xf, 0xc, 0xf, 0xe, 0xf, 0x1fc, 0xb, 0xf, 0x5, 0xf, 
    0x1fe, 0xa, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0x204, 
    0xa, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0x20a, 0xa, 
    0xf, 0x5, 0xf, 0x20c, 0xa, 0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 
    0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x214, 0xa, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x21a, 0xa, 0x10, 0x3, 0x10, 
    0x5, 0x10, 0x21d, 0xa, 0x10, 0x3, 0x10, 0x7, 0x10, 0x220, 0xa, 0x10, 
    0xc, 0x10, 0xe, 0x10, 0x223, 0xb, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0x22b, 0xa, 0x11, 0x3, 0x12, 
    0x5, 0x12, 0x22e, 0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x232, 
    0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 
    0x13, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0x23c, 0xa, 0x13, 0x3, 0x13, 
    0x5, 0x13, 0x23f, 0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x5, 0x13, 0x245, 0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0x249, 
    0xa, 0x13, 0x5, 0x13, 0x24b, 0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x5, 0x13, 0x251, 0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 
    0x255, 0xa, 0x13, 0x5, 0x13, 0x257, 0xa, 0x13, 0x5, 0x13, 0x259, 0xa, 
    0x13, 0x3, 0x13, 0x3, 0x13, 0x7, 0x13, 0x25d, 0xa, 0x13, 0xc, 0x13, 
    0xe, 0x13, 0x260, 0xb, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 
    0x265, 0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x7, 0x13, 0x26a, 
    0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0x26d, 0xb, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x5, 0x13, 0x271, 0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x7, 0x13, 0x277, 0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0x27a, 0xb, 0x13, 
    0x3, 0x13, 0x5, 0x13, 0x27d, 0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x7, 0x13, 0x286, 0xa, 0x13, 
    0xc, 0x13, 0xe, 0x13, 0x289, 0xb, 0x13, 0x3, 0x13, 0x5, 0x13, 0x28c, 
    0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 
    0x13, 0x3, 0x13, 0x7, 0x13, 0x295, 0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 
    0x298, 0xb, 0x13, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x29d, 
    0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x2a2, 0xa, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x2a9, 
    0xa, 0x15, 0x5, 0x15, 0x2ab, 0xa, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 
    0x5, 0x16, 0x2b0, 0xa, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 
    0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x7, 0x17, 0x2bb, 
    0xa, 0x17, 0xc, 0x17, 0xe, 0x17, 0x2be, 0xb, 0x17, 0x3, 0x18, 0x3, 0x18, 
    0x3, 0x18, 0x5, 0x18, 0x2c3, 0xa, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 
    0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x2cb, 0xa, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 0x2d1, 0xa, 0x1a, 0x3, 0x1b, 
    0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 
    0x1d, 0x3, 0x1d, 0x5, 0x1d, 0x2dc, 0xa, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2e5, 
    0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 
    0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x7, 0x1f, 0x2ef, 0xa, 0x1f, 0xc, 0x1f, 
    0xe, 0x1f, 0x2f2, 0xb, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x20, 0x3, 0x20, 
    0x3, 0x20, 0x3, 0x20, 0x7, 0x20, 0x2fa, 0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 
    0x2fd, 0xb, 0x20, 0x5, 0x20, 0x2ff, 0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x5, 0x20, 0x303, 0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x3, 0x20, 0x5, 0x20, 0x30a, 0xa, 0x20, 0x3, 0x21, 0x3, 0x21, 0x3, 0x22, 
    0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x6, 0x22, 0x312, 0xa, 0x22, 0xd, 0x22, 
    0xe, 0x22, 0x313, 0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x318, 0xa, 0x22, 
    0x3, 0x22, 0x7, 0x22, 0x31b, 0xa, 0x22, 0xc, 0x22, 0xe, 0x22, 0x31e, 
    0xb, 0x22, 0x5, 0x22, 0x320, 0xa, 0x22, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 
    0x3, 0x24, 0x3, 0x24, 0x5, 0x24, 0x327, 0xa, 0x24, 0x3, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 
    0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x5, 0x26, 0x336, 
    0xa, 0x26, 0x3, 0x27, 0x3, 0x27, 0x3, 0x28, 0x7, 0x28, 0x33b, 0xa, 0x28, 
    0xc, 0x28, 0xe, 0x28, 0x33e, 0xb, 0x28, 0x3, 0x28, 0x3, 0x28, 0x7, 0x28, 
    0x342, 0xa, 0x28, 0xc, 0x28, 0xe, 0x28, 0x345, 0xb, 0x28, 0x3, 0x28, 
    0x3, 0x28, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x5, 0x29, 0x34d, 
    0xa, 0x29, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x7, 0x2a, 0x353, 
    0xa, 0x2a, 0xc, 0x2a, 0xe, 0x2a, 0x356, 0xb, 0x2a, 0x3, 0x2b, 0x3, 0x2b, 
    0x3, 0x2b, 0x5, 0x2b, 0x35b, 0xa, 0x2b, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 
    0x5, 0x2c, 0x360, 0xa, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2e, 
    0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x5, 
    0x2f, 0x36c, 0xa, 0x2f, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 
    0x7, 0x30, 0x372, 0xa, 0x30, 0xc, 0x30, 0xe, 0x30, 0x375, 0xb, 0x30, 
    0x5, 0x30, 0x377, 0xa, 0x30, 0x3, 0x30, 0x3, 0x30, 0x5, 0x30, 0x37b, 
    0xa, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x7, 
    0x30, 0x382, 0xa, 0x30, 0xc, 0x30, 0xe, 0x30, 0x385, 0xb, 0x30, 0x3, 
    0x30, 0x3, 0x30, 0x5, 0x30, 0x389, 0xa, 0x30, 0x3, 0x30, 0x3, 0x30, 
    0x3, 0x30, 0x5, 0x30, 0x38e, 0xa, 0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 
    0x3, 0x31, 0x3, 0x31, 0x5, 0x31, 0x395, 0xa, 0x31, 0x3, 0x31, 0x7, 0x31, 
    0x398, 0xa, 0x31, 0xc, 0x31, 0xe, 0x31, 0x39b, 0xb, 0x31, 0x5, 0x31, 
    0x39d, 0xa, 0x31, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 
    0x5, 0x32, 0x3a4, 0xa, 0x32, 0x3, 0x32, 0x7, 0x32, 0x3a7, 0xa, 0x32, 
    0xc, 0x32, 0xe, 0x32, 0x3aa, 0xb, 0x32, 0x5, 0x32, 0x3ac, 0xa, 0x32, 
    0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x5, 0x33, 0x3b3, 
    0xa, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x34, 0x3, 0x34, 0x3, 
    0x35, 0x3, 0x35, 0x3, 0x35, 0x3, 0x35, 0x5, 0x35, 0x3be, 0xa, 0x35, 
    0x3, 0x36, 0x3, 0x36, 0x3, 0x36, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 
    0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3c, 0x3, 0x3c, 0x3, 
    0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x6, 0x3d, 0x3d7, 0xa, 0x3d, 
    0xd, 0x3d, 0xe, 0x3d, 0x3d8, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 
    0x3e, 0x3e5, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3f, 
    0x3, 0x3f, 0x3, 0x3f, 0x7, 0x3f, 0x3ed, 0xa, 0x3f, 0xc, 0x3f, 0xe, 0x3f, 
    0x3f0, 0xb, 0x3f, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 0x3f5, 
    0xa, 0x40, 0x3, 0x40, 0x7, 0x40, 0x3f8, 0xa, 0x40, 0xc, 0x40, 0xe, 0x40, 
    0x3fb, 0xb, 0x40, 0x5, 0x40, 0x3fd, 0xa, 0x40, 0x3, 0x41, 0x3, 0x41, 
    0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 
    0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 
    0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 
    0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x5, 0x41, 0x419, 
    0xa, 0x41, 0x3, 0x42, 0x3, 0x42, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 
    0x43, 0x420, 0xa, 0x43, 0x3, 0x44, 0x3, 0x44, 0x5, 0x44, 0x424, 0xa, 
    0x44, 0x3, 0x45, 0x3, 0x45, 0x3, 0x46, 0x3, 0x46, 0x3, 0x47, 0x3, 0x47, 
    0x3, 0x47, 0x3, 0x47, 0x3, 0x48, 0x3, 0x48, 0x3, 0x48, 0x3, 0x49, 0x3, 
    0x49, 0x3, 0x4a, 0x3, 0x4a, 0x5, 0x4a, 0x435, 0xa, 0x4a, 0x3, 0x4a, 
    0x3, 0x4a, 0x3, 0x4b, 0x3, 0x4b, 0x5, 0x4b, 0x43b, 0xa, 0x4b, 0x3, 0x4c, 
    0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4c, 0x7, 0x4c, 0x441, 0xa, 0x4c, 0xc, 0x4c, 
    0xe, 0x4c, 0x444, 0xb, 0x4c, 0x5, 0x4c, 0x446, 0xa, 0x4c, 0x3, 0x4c, 
    0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4c, 0x3, 
    0x4c, 0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4c, 0x7, 0x4c, 0x453, 0xa, 0x4c, 
    0xc, 0x4c, 0xe, 0x4c, 0x456, 0xb, 0x4c, 0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4c, 
    0x3, 0x4c, 0x5, 0x4c, 0x45c, 0xa, 0x4c, 0x3, 0x4d, 0x3, 0x4d, 0x3, 0x4d, 
    0x5, 0x4d, 0x461, 0xa, 0x4d, 0x3, 0x4e, 0x3, 0x4e, 0x3, 0x4e, 0x5, 0x4e, 
    0x466, 0xa, 0x4e, 0x3, 0x4e, 0x3, 0x4e, 0x3, 0x4e, 0x5, 0x4e, 0x46b, 
    0xa, 0x4e, 0x7, 0x4e, 0x46d, 0xa, 0x4e, 0xc, 0x4e, 0xe, 0x4e, 0x470, 
    0xb, 0x4e, 0x3, 0x4e, 0x3, 0x4e, 0x3, 0x4e, 0x3, 0x4e, 0x3, 0x4e, 0x3, 
    0x4e, 0x3, 0x4e, 0x7, 0x4e, 0x479, 0xa, 0x4e, 0xc, 0x4e, 0xe, 0x4e, 
    0x47c, 0xb, 0x4e, 0x3, 0x4f, 0x3, 0x4f, 0x3, 0x4f, 0x3, 0x4f, 0x3, 0x4f, 
    0x3, 0x4f, 0x7, 0x4f, 0x484, 0xa, 0x4f, 0xc, 0x4f, 0xe, 0x4f, 0x487, 
    0xb, 0x4f, 0x3, 0x4f, 0x3, 0x4f, 0x3, 0x50, 0x3, 0x50, 0x3, 0x50, 0x3, 
    0x50, 0x7, 0x50, 0x48f, 0xa, 0x50, 0xc, 0x50, 0xe, 0x50, 0x492, 0xb, 
    0x50, 0x3, 0x51, 0x3, 0x51, 0x3, 0x51, 0x3, 0x51, 0x7, 0x51, 0x498, 
    0xa, 0x51, 0xc, 0x51, 0xe, 0x51, 0x49b, 0xb, 0x51, 0x3, 0x51, 0x3, 0x51, 
    0x3, 0x52, 0x5, 0x52, 0x4a0, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 
    0x4a4, 0xa, 0x52, 0x3, 0x52, 0x5, 0x52, 0x4a7, 0xa, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x7, 0x52, 0x4ae, 0xa, 0x52, 
    0xc, 0x52, 0xe, 0x52, 0x4b1, 0xb, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 
    0x4b5, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 0x4b9, 0xa, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 0x4bd, 0xa, 0x52, 0x3, 0x53, 0x3, 0x53, 
    0x3, 0x54, 0x3, 0x54, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x5, 0x55, 0x4c8, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 
    0x4cc, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 
    0x4d2, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 0x4d7, 
    0xa, 0x55, 0x5, 0x55, 0x4d9, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 
    0x4dd, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 
    0x4e3, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 0x4e7, 0xa, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 0x4f3, 0xa, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 
    0x55, 0x4fb, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 0x4ff, 0xa, 
    0x55, 0x3, 0x55, 0x5, 0x55, 0x502, 0xa, 0x55, 0x3, 0x56, 0x3, 0x56, 
    0x3, 0x56, 0x5, 0x56, 0x507, 0xa, 0x56, 0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 
    0x3, 0x57, 0x3, 0x57, 0x5, 0x57, 0x50e, 0xa, 0x57, 0x3, 0x57, 0x3, 0x57, 
    0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 0x5, 0x57, 0x515, 0xa, 0x57, 0x3, 0x57, 
    0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 0x5, 
    0x57, 0x51e, 0xa, 0x57, 0x5, 0x57, 0x520, 0xa, 0x57, 0x3, 0x58, 0x3, 
    0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x5, 0x58, 0x527, 0xa, 0x58, 
    0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x5, 0x58, 0x52e, 
    0xa, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 
    0x58, 0x3, 0x58, 0x3, 0x58, 0x5, 0x58, 0x538, 0xa, 0x58, 0x5, 0x58, 
    0x53a, 0xa, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 
    0x5, 0x58, 0x541, 0xa, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 
    0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x5, 0x58, 0x54a, 0xa, 0x58, 0x5, 0x58, 
    0x54c, 0xa, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 
    0x5, 0x58, 0x553, 0xa, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 
    0x3, 0x58, 0x3, 0x58, 0x5, 0x58, 0x55b, 0xa, 0x58, 0x3, 0x58, 0x3, 0x58, 
    0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x5, 0x58, 0x562, 0xa, 0x58, 0x3, 0x58, 
    0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x5, 0x58, 0x569, 0xa, 0x58, 
    0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x5, 0x58, 0x570, 
    0xa, 0x58, 0x3, 0x58, 0x3, 0x58, 0x5, 0x58, 0x574, 0xa, 0x58, 0x5, 0x58, 
    0x576, 0xa, 0x58, 0x3, 0x59, 0x3, 0x59, 0x3, 0x59, 0x3, 0x59, 0x5, 0x59, 
    0x57c, 0xa, 0x59, 0x3, 0x59, 0x3, 0x59, 0x3, 0x59, 0x5, 0x59, 0x581, 
    0xa, 0x59, 0x3, 0x5a, 0x3, 0x5a, 0x5, 0x5a, 0x585, 0xa, 0x5a, 0x3, 0x5a, 
    0x5, 0x5a, 0x588, 0xa, 0x5a, 0x3, 0x5a, 0x3, 0x5a, 0x3, 0x5a, 0x5, 0x5a, 
    0x58d, 0xa, 0x5a, 0x3, 0x5a, 0x3, 0x5a, 0x5, 0x5a, 0x591, 0xa, 0x5a, 
    0x3, 0x5a, 0x5, 0x5a, 0x594, 0xa, 0x5a, 0x3, 0x5a, 0x3, 0x5a, 0x5, 0x5a, 
    0x598, 0xa, 0x5a, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5c, 0x3, 0x5c, 0x7, 0x5c, 
    0x59e, 0xa, 0x5c, 0xc, 0x5c, 0xe, 0x5c, 0x5a1, 0xb, 0x5c, 0x3, 0x5c, 
    0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x7, 0x5c, 0x5a7, 0xa, 0x5c, 0xc, 0x5c, 
    0xe, 0x5c, 0x5aa, 0xb, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x5, 0x5c, 
    0x5af, 0xa, 0x5c, 0x5, 0x5c, 0x5b1, 0xa, 0x5c, 0x3, 0x5d, 0x3, 0x5d, 
    0x3, 0x5d, 0x7, 0x5d, 0x5b6, 0xa, 0x5d, 0xc, 0x5d, 0xe, 0x5d, 0x5b9, 
    0xb, 0x5d, 0x3, 0x5e, 0x3, 0x5e, 0x5, 0x5e, 0x5bd, 0xa, 0x5e, 0x3, 0x5e, 
    0x5, 0x5e, 0x5c0, 0xa, 0x5e, 0x3, 0x5e, 0x5, 0x5e, 0x5c3, 0xa, 0x5e, 
    0x3, 0x5f, 0x3, 0x5f, 0x3, 0x5f, 0x7, 0x5f, 0x5c8, 0xa, 0x5f, 0xc, 0x5f, 
    0xe, 0x5f, 0x5cb, 0xb, 0x5f, 0x3, 0x60, 0x3, 0x60, 0x3, 0x61, 0x3, 0x61, 
    0x3, 0x61, 0x7, 0x61, 0x5d2, 0xa, 0x61, 0xc, 0x61, 0xe, 0x61, 0x5d5, 
    0xb, 0x61, 0x3, 0x62, 0x3, 0x62, 0x5, 0x62, 0x5d9, 0xa, 0x62, 0x3, 0x63, 
    0x3, 0x63, 0x3, 0x63, 0x7, 0x63, 0x5de, 0xa, 0x63, 0xc, 0x63, 0xe, 0x63, 
    0x5e1, 0xb, 0x63, 0x3, 0x64, 0x3, 0x64, 0x3, 0x65, 0x3, 0x65, 0x3, 0x65, 
    0x7, 0x65, 0x5e8, 0xa, 0x65, 0xc, 0x65, 0xe, 0x65, 0x5eb, 0xb, 0x65, 
    0x3, 0x66, 0x3, 0x66, 0x3, 0x67, 0x3, 0x67, 0x3, 0x67, 0x3, 0x67, 0x3, 
    0x68, 0x3, 0x68, 0x3, 0x68, 0x7, 0x68, 0x5f6, 0xa, 0x68, 0xc, 0x68, 
    0xe, 0x68, 0x5f9, 0xb, 0x68, 0x3, 0x69, 0x3, 0x69, 0x3, 0x6a, 0x3, 0x6a, 
    0x3, 0x6a, 0x7, 0x6a, 0x600, 0xa, 0x6a, 0xc, 0x6a, 0xe, 0x6a, 0x603, 
    0xb, 0x6a, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x608, 0xa, 0x6b, 
    0x3, 0x6c, 0x3, 0x6c, 0x3, 0x6c, 0x7, 0x6c, 0x60d, 0xa, 0x6c, 0xc, 0x6c, 
    0xe, 0x6c, 0x610, 0xb, 0x6c, 0x3, 0x6d, 0x3, 0x6d, 0x3, 0x6e, 0x3, 0x6e, 
    0x3, 0x6f, 0x3, 0x6f, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x6, 
    0x70, 0x636, 0xa, 0x70, 0xd, 0x70, 0xe, 0x70, 0x637, 0x3, 0x70, 0x3, 
    0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x6, 0x70, 0x63f, 0xa, 0x70, 
    0xd, 0x70, 0xe, 0x70, 0x640, 0x5, 0x70, 0x643, 0xa, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x5, 0x70, 0x647, 0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x7, 0x70, 0x667, 0xa, 0x70, 0xc, 0x70, 0xe, 0x70, 0x66a, 
    0xb, 0x70, 0x5, 0x70, 0x66c, 0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x7, 0x70, 0x674, 0xa, 0x70, 0xc, 0x70, 
    0xe, 0x70, 0x677, 0xb, 0x70, 0x5, 0x70, 0x679, 0xa, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x7, 0x70, 0x681, 
    0xa, 0x70, 0xc, 0x70, 0xe, 0x70, 0x684, 0xb, 0x70, 0x5, 0x70, 0x686, 
    0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x692, 
    0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x6a4, 0xa, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x6ab, 
    0xa, 0x70, 0x5, 0x70, 0x6ad, 0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x6b6, 0xa, 0x70, 
    0x3, 0x70, 0x5, 0x70, 0x6b9, 0xa, 0x70, 0x3, 0x70, 0x5, 0x70, 0x6bc, 
    0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x6c0, 0xa, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x6ce, 
    0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x5, 0x70, 0x6d6, 0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x6df, 0xa, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x5, 0x70, 0x6e8, 0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x6f1, 0xa, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x5, 0x70, 0x6fa, 0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x7, 0x70, 0x704, 
    0xa, 0x70, 0xc, 0x70, 0xe, 0x70, 0x707, 0xb, 0x70, 0x5, 0x70, 0x709, 
    0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 
    0x716, 0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x71f, 0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x72c, 0xa, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x732, 0xa, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 
    0x70, 0x3, 0x70, 0x7, 0x70, 0x74a, 0xa, 0x70, 0xc, 0x70, 0xe, 0x70, 
    0x74d, 0xb, 0x70, 0x5, 0x70, 0x74f, 0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x756, 0xa, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x75b, 0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 0x764, 
    0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x7, 
    0x70, 0x76b, 0xa, 0x70, 0xc, 0x70, 0xe, 0x70, 0x76e, 0xb, 0x70, 0x5, 
    0x70, 0x770, 0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x5, 0x70, 
    0x775, 0xa, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x7, 0x70, 0x77c, 0xa, 0x70, 0xc, 0x70, 0xe, 0x70, 0x77f, 0xb, 0x70, 
    0x3, 0x71, 0x3, 0x71, 0x3, 0x72, 0x3, 0x72, 0x3, 0x72, 0x5, 0x72, 0x786, 
    0xa, 0x72, 0x3, 0x72, 0x3, 0x72, 0x3, 0x72, 0x5, 0x72, 0x78b, 0xa, 0x72, 
    0x3, 0x72, 0x3, 0x72, 0x3, 0x72, 0x5, 0x72, 0x790, 0xa, 0x72, 0x3, 0x72, 
    0x3, 0x72, 0x3, 0x73, 0x3, 0x73, 0x3, 0x73, 0x5, 0x73, 0x797, 0xa, 0x73, 
    0x3, 0x73, 0x3, 0x73, 0x3, 0x73, 0x5, 0x73, 0x79c, 0xa, 0x73, 0x3, 0x73, 
    0x3, 0x73, 0x3, 0x73, 0x5, 0x73, 0x7a1, 0xa, 0x73, 0x3, 0x73, 0x3, 0x73, 
    0x3, 0x74, 0x3, 0x74, 0x3, 0x74, 0x7, 0x74, 0x7a8, 0xa, 0x74, 0xc, 0x74, 
    0xe, 0x74, 0x7ab, 0xb, 0x74, 0x3, 0x74, 0x3, 0x74, 0x3, 0x75, 0x3, 0x75, 
    0x3, 0x76, 0x3, 0x76, 0x3, 0x77, 0x3, 0x77, 0x3, 0x78, 0x3, 0x78, 0x3, 
    0x79, 0x3, 0x79, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7c, 
    0x3, 0x7c, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7e, 0x3, 0x7e, 0x3, 0x7f, 0x3, 
    0x7f, 0x3, 0x80, 0x3, 0x80, 0x3, 0x81, 0x3, 0x81, 0x3, 0x82, 0x3, 0x82, 
    0x5, 0x82, 0x7cb, 0xa, 0x82, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x5, 0x83, 
    0x7d0, 0xa, 0x83, 0x3, 0x84, 0x3, 0x84, 0x3, 0x84, 0x3, 0x84, 0x5, 0x84, 
    0x7d6, 0xa, 0x84, 0x3, 0x85, 0x3, 0x85, 0x3, 0x86, 0x3, 0x86, 0x3, 0x87, 
    0x3, 0x87, 0x3, 0x88, 0x3, 0x88, 0x3, 0x89, 0x3, 0x89, 0x3, 0x89, 0x3, 
    0x89, 0x3, 0x89, 0x3, 0x89, 0x3, 0x89, 0x3, 0x89, 0x3, 0x89, 0x3, 0x89, 
    0x5, 0x89, 0x7ea, 0xa, 0x89, 0x3, 0x8a, 0x3, 0x8a, 0x3, 0x8a, 0x3, 0x8a, 
    0x3, 0x8a, 0x3, 0x8a, 0x5, 0x8a, 0x7f2, 0xa, 0x8a, 0x3, 0x8b, 0x3, 0x8b, 
    0x3, 0x8c, 0x3, 0x8c, 0x3, 0x8d, 0x3, 0x8d, 0x5, 0x8d, 0x7fa, 0xa, 0x8d, 
    0x3, 0x8d, 0x3, 0x8d, 0x3, 0x8e, 0x3, 0x8e, 0x3, 0x8f, 0x3, 0x8f, 0x5, 
    0x8f, 0x802, 0xa, 0x8f, 0x3, 0x8f, 0x3, 0x8f, 0x3, 0x90, 0x3, 0x90, 
    0x3, 0x91, 0x3, 0x91, 0x5, 0x91, 0x80a, 0xa, 0x91, 0x3, 0x91, 0x3, 0x91, 
    0x3, 0x92, 0x3, 0x92, 0x3, 0x92, 0x3, 0x93, 0x3, 0x93, 0x3, 0x93, 0x3, 
    0x94, 0x3, 0x94, 0x3, 0x94, 0x3, 0x95, 0x3, 0x95, 0x3, 0x96, 0x3, 0x96, 
    0x5, 0x96, 0x81b, 0xa, 0x96, 0x3, 0x97, 0x3, 0x97, 0x3, 0x97, 0x5, 0x97, 
    0x820, 0xa, 0x97, 0x3, 0x97, 0x3, 0x97, 0x3, 0x97, 0x3, 0x97, 0x3, 0x97, 
    0x3, 0x97, 0x5, 0x97, 0x828, 0xa, 0x97, 0x3, 0x97, 0x3, 0x97, 0x3, 0x97, 
    0x3, 0x97, 0x3, 0x97, 0x5, 0x97, 0x82f, 0xa, 0x97, 0x3, 0x97, 0x3, 0x97, 
    0x3, 0x97, 0x3, 0x97, 0x3, 0x97, 0x3, 0x97, 0x3, 0x97, 0x5, 0x97, 0x838, 
    0xa, 0x97, 0x5, 0x97, 0x83a, 0xa, 0x97, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 
    0x5, 0x98, 0x83f, 0xa, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 
    0x3, 0x98, 0x3, 0x98, 0x5, 0x98, 0x847, 0xa, 0x98, 0x3, 0x98, 0x3, 0x98, 
    0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x5, 0x98, 0x84e, 0xa, 0x98, 0x3, 0x98, 
    0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 
    0x98, 0x5, 0x98, 0x858, 0xa, 0x98, 0x5, 0x98, 0x85a, 0xa, 0x98, 0x3, 
    0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x5, 0x98, 0x861, 
    0xa, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 
    0x98, 0x3, 0x98, 0x5, 0x98, 0x86a, 0xa, 0x98, 0x5, 0x98, 0x86c, 0xa, 
    0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x5, 0x98, 
    0x873, 0xa, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 
    0x3, 0x98, 0x5, 0x98, 0x87b, 0xa, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 
    0x3, 0x98, 0x3, 0x98, 0x5, 0x98, 0x882, 0xa, 0x98, 0x3, 0x98, 0x3, 0x98, 
    0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x5, 0x98, 0x889, 0xa, 0x98, 0x3, 0x98, 
    0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x3, 0x98, 0x5, 0x98, 0x890, 0xa, 0x98, 
    0x3, 0x98, 0x3, 0x98, 0x5, 0x98, 0x894, 0xa, 0x98, 0x5, 0x98, 0x896, 
    0xa, 0x98, 0x3, 0x99, 0x3, 0x99, 0x3, 0x99, 0x2, 0x5, 0x1e, 0x24, 0xde, 
    0x9a, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 
    0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 
    0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 0x48, 
    0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 0x60, 
    0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 0x78, 
    0x7a, 0x7c, 0x7e, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 0x90, 
    0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, 0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 
    0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe, 0xc0, 
    0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 
    0xda, 0xdc, 0xde, 0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee, 0xf0, 
    0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe, 0x100, 0x102, 0x104, 0x106, 
    0x108, 0x10a, 0x10c, 0x10e, 0x110, 0x112, 0x114, 0x116, 0x118, 0x11a, 
    0x11c, 0x11e, 0x120, 0x122, 0x124, 0x126, 0x128, 0x12a, 0x12c, 0x12e, 
    0x130, 0x2, 0x29, 0x4, 0x2, 0xd9, 0xd9, 0xf9, 0xf9, 0x4, 0x2, 0xb, 0xb, 
    0xc4, 0xc4, 0x5, 0x2, 0x5b, 0x5b, 0x79, 0x79, 0xe5, 0xe5, 0x5, 0x2, 
    0x69, 0x69, 0x82, 0x82, 0xbd, 0xbd, 0x3, 0x2, 0x1a, 0x1b, 0x5, 0x2, 
    0x19, 0x19, 0xa4, 0xa4, 0xbb, 0xbb, 0x5, 0x2, 0x30, 0x30, 0x51, 0x51, 
    0x5d, 0x5d, 0x4, 0x2, 0xd, 0xd, 0xdb, 0xdb, 0x4, 0x2, 0xe, 0xe, 0x19, 
    0x19, 0x4, 0x2, 0xb4, 0xb4, 0xbe, 0xbe, 0x6, 0x2, 0x48, 0x48, 0x5a, 
    0x5a, 0x63, 0x63, 0xb8, 0xb8, 0x6, 0x2, 0x61, 0x61, 0xa5, 0xa5, 0xa7, 
    0xa7, 0xe2, 0xe2, 0x3, 0x2, 0x9e, 0x9f, 0xc, 0x2, 0x2d, 0x2d, 0x37, 
    0x37, 0x47, 0x47, 0x4b, 0x4b, 0x65, 0x65, 0x78, 0x78, 0x7a, 0x7b, 0xa2, 
    0xa2, 0xd9, 0xda, 0xed, 0xed, 0x4, 0x2, 0xc1, 0xc3, 0xd3, 0xd3, 0x4, 
    0x2, 0x90, 0x92, 0x99, 0x99, 0x4, 0x2, 0x75, 0x75, 0x80, 0x80, 0x6, 
    0x2, 0x2f, 0x2f, 0x6c, 0x6c, 0xc7, 0xc7, 0xde, 0xde, 0x5, 0x2, 0x28, 
    0x28, 0x33, 0x33, 0x50, 0x50, 0x3, 0x2, 0x32, 0x33, 0x4, 0x2, 0x37, 
    0x37, 0x39, 0x39, 0x4, 0x2, 0x32, 0x33, 0x37, 0x38, 0x4, 0x2, 0x22, 
    0x22, 0x4e, 0x4e, 0x4, 0x2, 0xd, 0xd, 0x11, 0x11, 0x4, 0x2, 0x8, 0x8, 
    0x21, 0x21, 0x4, 0x2, 0x8, 0x8, 0xb2, 0xb2, 0x4, 0x2, 0x8, 0x8, 0x6a, 
    0x6a, 0x4, 0x2, 0x8, 0x8, 0x66, 0x66, 0x4, 0x2, 0x8, 0x8, 0x74, 0x74, 
    0x3, 0x2, 0xce, 0xcf, 0x5, 0x2, 0x2c, 0x2c, 0x81, 0x81, 0xe0, 0xe0, 
    0x8, 0x2, 0x49, 0x49, 0x70, 0x70, 0x94, 0x94, 0x97, 0x97, 0xc2, 0xc2, 
    0xf7, 0xf7, 0x5, 0x2, 0x7, 0x7, 0xe, 0xe, 0x12, 0x12, 0x5, 0x2, 0xd, 
    0xd, 0x11, 0x11, 0x14, 0x14, 0x4, 0x2, 0x4, 0x6, 0x15, 0x18, 0x3, 0x2, 
    0x42, 0x45, 0x4, 0x2, 0x61, 0x61, 0xe2, 0xe2, 0x4, 0x2, 0xfb, 0xfb, 
    0xfd, 0xfe, 0x38, 0x2, 0x1a, 0x1b, 0x1d, 0x1e, 0x22, 0x22, 0x27, 0x28, 
    0x2a, 0x2b, 0x2e, 0x30, 0x32, 0x33, 0x36, 0x36, 0x38, 0x38, 0x3c, 0x3d, 
    0x46, 0x46, 0x48, 0x48, 0x4a, 0x4a, 0x4c, 0x4c, 0x4e, 0x50, 0x53, 0x54, 
    0x56, 0x56, 0x5a, 0x5a, 0x5d, 0x5f, 0x61, 0x64, 0x67, 0x68, 0x6c, 0x6d, 
    0x71, 0x73, 0x75, 0x75, 0x7a, 0x7b, 0x80, 0x80, 0x84, 0x88, 0x8b, 0x8d, 
    0x8f, 0x92, 0x95, 0x96, 0x98, 0x99, 0x9c, 0x9f, 0xa1, 0xa1, 0xa4, 0xa6, 
    0xaa, 0xaa, 0xae, 0xae, 0xb0, 0xb2, 0xb4, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 
    0xbf, 0xc1, 0xc3, 0xc3, 0xc7, 0xce, 0xd1, 0xd3, 0xd6, 0xd7, 0xdc, 0xde, 
    0xe1, 0xe1, 0xe3, 0xe4, 0xe7, 0xe7, 0xea, 0xea, 0xef, 0xf0, 0xf3, 0xf4, 
    0xf6, 0xf6, 0xf8, 0xf8, 0x2, 0x9ae, 0x2, 0x133, 0x3, 0x2, 0x2, 0x2, 
    0x4, 0x139, 0x3, 0x2, 0x2, 0x2, 0x6, 0x161, 0x3, 0x2, 0x2, 0x2, 0x8, 
    0x167, 0x3, 0x2, 0x2, 0x2, 0xa, 0x16a, 0x3, 0x2, 0x2, 0x2, 0xc, 0x178, 
    0x3, 0x2, 0x2, 0x2, 0xe, 0x17a, 0x3, 0x2, 0x2, 0x2, 0x10, 0x17c, 0x3, 
    0x2, 0x2, 0x2, 0x12, 0x181, 0x3, 0x2, 0x2, 0x2, 0x14, 0x188, 0x3, 0x2, 
    0x2, 0x2, 0x16, 0x1bc, 0x3, 0x2, 0x2, 0x2, 0x18, 0x1c7, 0x3, 0x2, 0x2, 
    0x2, 0x1a, 0x1f2, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x1fd, 0x3, 0x2, 0x2, 0x2, 
    0x1e, 0x213, 0x3, 0x2, 0x2, 0x2, 0x20, 0x224, 0x3, 0x2, 0x2, 0x2, 0x22, 
    0x22d, 0x3, 0x2, 0x2, 0x2, 0x24, 0x258, 0x3, 0x2, 0x2, 0x2, 0x26, 0x2a1, 
    0x3, 0x2, 0x2, 0x2, 0x28, 0x2aa, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x2af, 0x3, 
    0x2, 0x2, 0x2, 0x2c, 0x2b1, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x2c2, 0x3, 0x2, 
    0x2, 0x2, 0x30, 0x2ca, 0x3, 0x2, 0x2, 0x2, 0x32, 0x2ce, 0x3, 0x2, 0x2, 
    0x2, 0x34, 0x2d2, 0x3, 0x2, 0x2, 0x2, 0x36, 0x2d6, 0x3, 0x2, 0x2, 0x2, 
    0x38, 0x2db, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x2dd, 0x3, 0x2, 0x2, 0x2, 0x3c, 
    0x2e2, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x2fe, 0x3, 0x2, 0x2, 0x2, 0x40, 0x30b, 
    0x3, 0x2, 0x2, 0x2, 0x42, 0x30d, 0x3, 0x2, 0x2, 0x2, 0x44, 0x321, 0x3, 
    0x2, 0x2, 0x2, 0x46, 0x326, 0x3, 0x2, 0x2, 0x2, 0x48, 0x328, 0x3, 0x2, 
    0x2, 0x2, 0x4a, 0x32c, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x337, 0x3, 0x2, 0x2, 
    0x2, 0x4e, 0x33c, 0x3, 0x2, 0x2, 0x2, 0x50, 0x34c, 0x3, 0x2, 0x2, 0x2, 
    0x52, 0x34e, 0x3, 0x2, 0x2, 0x2, 0x54, 0x357, 0x3, 0x2, 0x2, 0x2, 0x56, 
    0x35f, 0x3, 0x2, 0x2, 0x2, 0x58, 0x361, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x364, 
    0x3, 0x2, 0x2, 0x2, 0x5c, 0x368, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x376, 0x3, 
    0x2, 0x2, 0x2, 0x60, 0x38f, 0x3, 0x2, 0x2, 0x2, 0x62, 0x39e, 0x3, 0x2, 
    0x2, 0x2, 0x64, 0x3b2, 0x3, 0x2, 0x2, 0x2, 0x66, 0x3b7, 0x3, 0x2, 0x2, 
    0x2, 0x68, 0x3b9, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x3bf, 0x3, 0x2, 0x2, 0x2, 
    0x6c, 0x3c2, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x3c5, 0x3, 0x2, 0x2, 0x2, 0x70, 
    0x3c9, 0x3, 0x2, 0x2, 0x2, 0x72, 0x3cc, 0x3, 0x2, 0x2, 0x2, 0x74, 0x3ce, 
    0x3, 0x2, 0x2, 0x2, 0x76, 0x3d0, 0x3, 0x2, 0x2, 0x2, 0x78, 0x3d2, 0x3, 
    0x2, 0x2, 0x2, 0x7a, 0x3dc, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x3e9, 0x3, 0x2, 
    0x2, 0x2, 0x7e, 0x3f1, 0x3, 0x2, 0x2, 0x2, 0x80, 0x418, 0x3, 0x2, 0x2, 
    0x2, 0x82, 0x41a, 0x3, 0x2, 0x2, 0x2, 0x84, 0x41c, 0x3, 0x2, 0x2, 0x2, 
    0x86, 0x423, 0x3, 0x2, 0x2, 0x2, 0x88, 0x425, 0x3, 0x2, 0x2, 0x2, 0x8a, 
    0x427, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x429, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x42d, 
    0x3, 0x2, 0x2, 0x2, 0x90, 0x430, 0x3, 0x2, 0x2, 0x2, 0x92, 0x432, 0x3, 
    0x2, 0x2, 0x2, 0x94, 0x438, 0x3, 0x2, 0x2, 0x2, 0x96, 0x445, 0x3, 0x2, 
    0x2, 0x2, 0x98, 0x45d, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x462, 0x3, 0x2, 0x2, 
    0x2, 0x9c, 0x47d, 0x3, 0x2, 0x2, 0x2, 0x9e, 0x48a, 0x3, 0x2, 0x2, 0x2, 
    0xa0, 0x493, 0x3, 0x2, 0x2, 0x2, 0xa2, 0x49f, 0x3, 0x2, 0x2, 0x2, 0xa4, 
    0x4be, 0x3, 0x2, 0x2, 0x2, 0xa6, 0x4c0, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x501, 
    0x3, 0x2, 0x2, 0x2, 0xaa, 0x503, 0x3, 0x2, 0x2, 0x2, 0xac, 0x51f, 0x3, 
    0x2, 0x2, 0x2, 0xae, 0x575, 0x3, 0x2, 0x2, 0x2, 0xb0, 0x577, 0x3, 0x2, 
    0x2, 0x2, 0xb2, 0x593, 0x3, 0x2, 0x2, 0x2, 0xb4, 0x599, 0x3, 0x2, 0x2, 
    0x2, 0xb6, 0x5b0, 0x3, 0x2, 0x2, 0x2, 0xb8, 0x5b2, 0x3, 0x2, 0x2, 0x2, 
    0xba, 0x5c2, 0x3, 0x2, 0x2, 0x2, 0xbc, 0x5c4, 0x3, 0x2, 0x2, 0x2, 0xbe, 
    0x5cc, 0x3, 0x2, 0x2, 0x2, 0xc0, 0x5ce, 0x3, 0x2, 0x2, 0x2, 0xc2, 0x5d6, 
    0x3, 0x2, 0x2, 0x2, 0xc4, 0x5da, 0x3, 0x2, 0x2, 0x2, 0xc6, 0x5e2, 0x3, 
    0x2, 0x2, 0x2, 0xc8, 0x5e4, 0x3, 0x2, 0x2, 0x2, 0xca, 0x5ec, 0x3, 0x2, 
    0x2, 0x2, 0xcc, 0x5ee, 0x3, 0x2, 0x2, 0x2, 0xce, 0x5f2, 0x3, 0x2, 0x2, 
    0x2, 0xd0, 0x5fa, 0x3, 0x2, 0x2, 0x2, 0xd2, 0x5fc, 0x3, 0x2, 0x2, 0x2, 
    0xd4, 0x604, 0x3, 0x2, 0x2, 0x2, 0xd6, 0x609, 0x3, 0x2, 0x2, 0x2, 0xd8, 
    0x611, 0x3, 0x2, 0x2, 0x2, 0xda, 0x613, 0x3, 0x2, 0x2, 0x2, 0xdc, 0x615, 
    0x3, 0x2, 0x2, 0x2, 0xde, 0x71e, 0x3, 0x2, 0x2, 0x2, 0xe0, 0x780, 0x3, 
    0x2, 0x2, 0x2, 0xe2, 0x78f, 0x3, 0x2, 0x2, 0x2, 0xe4, 0x7a0, 0x3, 0x2, 
    0x2, 0x2, 0xe6, 0x7a9, 0x3, 0x2, 0x2, 0x2, 0xe8, 0x7ae, 0x3, 0x2, 0x2, 
    0x2, 0xea, 0x7b0, 0x3, 0x2, 0x2, 0x2, 0xec, 0x7b2, 0x3, 0x2, 0x2, 0x2, 
    0xee, 0x7b4, 0x3, 0x2, 0x2, 0x2, 0xf0, 0x7b6, 0x3, 0x2, 0x2, 0x2, 0xf2, 
    0x7b8, 0x3, 0x2, 0x2, 0x2, 0xf4, 0x7ba, 0x3, 0x2, 0x2, 0x2, 0xf6, 0x7bc, 
    0x3, 0x2, 0x2, 0x2, 0xf8, 0x7be, 0x3, 0x2, 0x2, 0x2, 0xfa, 0x7c0, 0x3, 
    0x2, 0x2, 0x2, 0xfc, 0x7c2, 0x3, 0x2, 0x2, 0x2, 0xfe, 0x7c4, 0x3, 0x2, 
    0x2, 0x2, 0x100, 0x7c6, 0x3, 0x2, 0x2, 0x2, 0x102, 0x7ca, 0x3, 0x2, 
    0x2, 0x2, 0x104, 0x7cf, 0x3, 0x2, 0x2, 0x2, 0x106, 0x7d5, 0x3, 0x2, 
    0x2, 0x2, 0x108, 0x7d7, 0x3, 0x2, 0x2, 0x2, 0x10a, 0x7d9, 0x3, 0x2, 
    0x2, 0x2, 0x10c, 0x7db, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x7dd, 0x3, 0x2, 
    0x2, 0x2, 0x110, 0x7e9, 0x3, 0x2, 0x2, 0x2, 0x112, 0x7f1, 0x3, 0x2, 
    0x2, 0x2, 0x114, 0x7f3, 0x3, 0x2, 0x2, 0x2, 0x116, 0x7f5, 0x3, 0x2, 
    0x2, 0x2, 0x118, 0x7f9, 0x3, 0x2, 0x2, 0x2, 0x11a, 0x7fd, 0x3, 0x2, 
    0x2, 0x2, 0x11c, 0x801, 0x3, 0x2, 0x2, 0x2, 0x11e, 0x805, 0x3, 0x2, 
    0x2, 0x2, 0x120, 0x809, 0x3, 0x2, 0x2, 0x2, 0x122, 0x80d, 0x3, 0x2, 
    0x2, 0x2, 0x124, 0x810, 0x3, 0x2, 0x2, 0x2, 0x126, 0x813, 0x3, 0x2, 
    0x2, 0x2, 0x128, 0x816, 0x3, 0x2, 0x2, 0x2, 0x12a, 0x81a, 0x3, 0x2, 
    0x2, 0x2, 0x12c, 0x81c, 0x3, 0x2, 0x2, 0x2, 0x12e, 0x83b, 0x3, 0x2, 
    0x2, 0x2, 0x130, 0x897, 0x3, 0x2, 0x2, 0x2, 0x132, 0x134, 0x5, 0x4, 
    0x3, 0x2, 0x133, 0x132, 0x3, 0x2, 0x2, 0x2, 0x134, 0x135, 0x3, 0x2, 
    0x2, 0x2, 0x135, 0x133, 0x3, 0x2, 0x2, 0x2, 0x135, 0x136, 0x3, 0x2, 
    0x2, 0x2, 0x136, 0x137, 0x3, 0x2, 0x2, 0x2, 0x137, 0x138, 0x7, 0x2, 
    0x2, 0x3, 0x138, 0x3, 0x3, 0x2, 0x2, 0x2, 0x139, 0x13a, 0x5, 0x8, 0x5, 
    0x2, 0x13a, 0x13b, 0x5, 0x6, 0x4, 0x2, 0x13b, 0x13c, 0x7, 0x3, 0x2, 
    0x2, 0x13c, 0x5, 0x3, 0x2, 0x2, 0x2, 0x13d, 0x162, 0x3, 0x2, 0x2, 0x2, 
    0x13e, 0x162, 0x5, 0x2e, 0x18, 0x2, 0x13f, 0x162, 0x5, 0x30, 0x19, 0x2, 
    0x140, 0x162, 0x5, 0x34, 0x1b, 0x2, 0x141, 0x162, 0x5, 0x36, 0x1c, 0x2, 
    0x142, 0x162, 0x5, 0x38, 0x1d, 0x2, 0x143, 0x162, 0x5, 0x40, 0x21, 0x2, 
    0x144, 0x162, 0x5, 0x42, 0x22, 0x2, 0x145, 0x162, 0x5, 0x44, 0x23, 0x2, 
    0x146, 0x162, 0x5, 0x46, 0x24, 0x2, 0x147, 0x162, 0x5, 0x4e, 0x28, 0x2, 
    0x148, 0x162, 0x5, 0x5a, 0x2e, 0x2, 0x149, 0x162, 0x5, 0x60, 0x31, 0x2, 
    0x14a, 0x162, 0x5, 0x62, 0x32, 0x2, 0x14b, 0x162, 0x5, 0x68, 0x35, 0x2, 
    0x14c, 0x162, 0x5, 0x6a, 0x36, 0x2, 0x14d, 0x162, 0x5, 0x6c, 0x37, 0x2, 
    0x14e, 0x162, 0x5, 0x6e, 0x38, 0x2, 0x14f, 0x162, 0x5, 0x70, 0x39, 0x2, 
    0x150, 0x162, 0x5, 0x72, 0x3a, 0x2, 0x151, 0x162, 0x5, 0x78, 0x3d, 0x2, 
    0x152, 0x162, 0x5, 0x10, 0x9, 0x2, 0x153, 0x162, 0x5, 0x84, 0x43, 0x2, 
    0x154, 0x162, 0x5, 0x88, 0x45, 0x2, 0x155, 0x162, 0x5, 0x8a, 0x46, 0x2, 
    0x156, 0x162, 0x5, 0x8c, 0x47, 0x2, 0x157, 0x162, 0x5, 0x8e, 0x48, 0x2, 
    0x158, 0x162, 0x5, 0x90, 0x49, 0x2, 0x159, 0x162, 0x5, 0x92, 0x4a, 0x2, 
    0x15a, 0x162, 0x5, 0x98, 0x4d, 0x2, 0x15b, 0x162, 0x5, 0x94, 0x4b, 0x2, 
    0x15c, 0x162, 0x5, 0x74, 0x3b, 0x2, 0x15d, 0x162, 0x5, 0x32, 0x1a, 0x2, 
    0x15e, 0x162, 0x5, 0x5c, 0x2f, 0x2, 0x15f, 0x162, 0x5, 0x76, 0x3c, 0x2, 
    0x160, 0x162, 0x5, 0x4c, 0x27, 0x2, 0x161, 0x13d, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x13e, 0x3, 0x2, 0x2, 0x2, 0x161, 0x13f, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x140, 0x3, 0x2, 0x2, 0x2, 0x161, 0x141, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x142, 0x3, 0x2, 0x2, 0x2, 0x161, 0x143, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x144, 0x3, 0x2, 0x2, 0x2, 0x161, 0x145, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x146, 0x3, 0x2, 0x2, 0x2, 0x161, 0x147, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x148, 0x3, 0x2, 0x2, 0x2, 0x161, 0x149, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x14a, 0x3, 0x2, 0x2, 0x2, 0x161, 0x14b, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x14c, 0x3, 0x2, 0x2, 0x2, 0x161, 0x14d, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x14e, 0x3, 0x2, 0x2, 0x2, 0x161, 0x14f, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x150, 0x3, 0x2, 0x2, 0x2, 0x161, 0x151, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x152, 0x3, 0x2, 0x2, 0x2, 0x161, 0x153, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x154, 0x3, 0x2, 0x2, 0x2, 0x161, 0x155, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x156, 0x3, 0x2, 0x2, 0x2, 0x161, 0x157, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x158, 0x3, 0x2, 0x2, 0x2, 0x161, 0x159, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x15a, 0x3, 0x2, 0x2, 0x2, 0x161, 0x15b, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x15c, 0x3, 0x2, 0x2, 0x2, 0x161, 0x15d, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x15e, 0x3, 0x2, 0x2, 0x2, 0x161, 0x15f, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x160, 0x3, 0x2, 0x2, 0x2, 0x162, 0x7, 0x3, 0x2, 0x2, 0x2, 0x163, 
    0x166, 0x5, 0xc, 0x7, 0x2, 0x164, 0x166, 0x5, 0xa, 0x6, 0x2, 0x165, 
    0x163, 0x3, 0x2, 0x2, 0x2, 0x165, 0x164, 0x3, 0x2, 0x2, 0x2, 0x166, 
    0x169, 0x3, 0x2, 0x2, 0x2, 0x167, 0x165, 0x3, 0x2, 0x2, 0x2, 0x167, 
    0x168, 0x3, 0x2, 0x2, 0x2, 0x168, 0x9, 0x3, 0x2, 0x2, 0x2, 0x169, 0x167, 
    0x3, 0x2, 0x2, 0x2, 0x16a, 0x16b, 0x7, 0x4, 0x2, 0x2, 0x16b, 0x16c, 
    0x5, 0xe, 0x8, 0x2, 0x16c, 0x16d, 0x7, 0x5, 0x2, 0x2, 0x16d, 0x170, 
    0x7, 0x6, 0x2, 0x2, 0x16e, 0x171, 0x5, 0x128, 0x95, 0x2, 0x16f, 0x171, 
    0x7, 0x101, 0x2, 0x2, 0x170, 0x16e, 0x3, 0x2, 0x2, 0x2, 0x170, 0x16f, 
    0x3, 0x2, 0x2, 0x2, 0x171, 0xb, 0x3, 0x2, 0x2, 0x2, 0x172, 0x173, 0x7, 
    0x7, 0x2, 0x2, 0x173, 0x179, 0x5, 0xe, 0x8, 0x2, 0x174, 0x175, 0x7, 
    0x4, 0x2, 0x2, 0x175, 0x176, 0x5, 0xe, 0x8, 0x2, 0x176, 0x177, 0x7, 
    0x5, 0x2, 0x2, 0x177, 0x179, 0x3, 0x2, 0x2, 0x2, 0x178, 0x172, 0x3, 
    0x2, 0x2, 0x2, 0x178, 0x174, 0x3, 0x2, 0x2, 0x2, 0x179, 0xd, 0x3, 0x2, 
    0x2, 0x2, 0x17a, 0x17b, 0x9, 0x2, 0x2, 0x2, 0x17b, 0xf, 0x3, 0x2, 0x2, 
    0x2, 0x17c, 0x17d, 0x5, 0x12, 0xa, 0x2, 0x17d, 0x11, 0x3, 0x2, 0x2, 
    0x2, 0x17e, 0x180, 0x5, 0x14, 0xb, 0x2, 0x17f, 0x17e, 0x3, 0x2, 0x2, 
    0x2, 0x180, 0x183, 0x3, 0x2, 0x2, 0x2, 0x181, 0x17f, 0x3, 0x2, 0x2, 
    0x2, 0x181, 0x182, 0x3, 0x2, 0x2, 0x2, 0x182, 0x186, 0x3, 0x2, 0x2, 
    0x2, 0x183, 0x181, 0x3, 0x2, 0x2, 0x2, 0x184, 0x187, 0x5, 0x18, 0xd, 
    0x2, 0x185, 0x187, 0x5, 0x1c, 0xf, 0x2, 0x186, 0x184, 0x3, 0x2, 0x2, 
    0x2, 0x186, 0x185, 0x3, 0x2, 0x2, 0x2, 0x187, 0x13, 0x3, 0x2, 0x2, 0x2, 
    0x188, 0x18a, 0x7, 0x102, 0x2, 0x2, 0x189, 0x18b, 0x5, 0x16, 0xc, 0x2, 
    0x18a, 0x189, 0x3, 0x2, 0x2, 0x2, 0x18a, 0x18b, 0x3, 0x2, 0x2, 0x2, 
    0x18b, 0x190, 0x3, 0x2, 0x2, 0x2, 0x18c, 0x18d, 0x7, 0x8, 0x2, 0x2, 
    0x18d, 0x18f, 0x5, 0x16, 0xc, 0x2, 0x18e, 0x18c, 0x3, 0x2, 0x2, 0x2, 
    0x18f, 0x192, 0x3, 0x2, 0x2, 0x2, 0x190, 0x18e, 0x3, 0x2, 0x2, 0x2, 
    0x190, 0x191, 0x3, 0x2, 0x2, 0x2, 0x191, 0x193, 0x3, 0x2, 0x2, 0x2, 
    0x192, 0x190, 0x3, 0x2, 0x2, 0x2, 0x193, 0x194, 0x7, 0x103, 0x2, 0x2, 
    0x194, 0x15, 0x3, 0x2, 0x2, 0x2, 0x195, 0x1bd, 0x5, 0xfe, 0x80, 0x2, 
    0x196, 0x197, 0x5, 0x100, 0x81, 0x2, 0x197, 0x198, 0x7, 0x6, 0x2, 0x2, 
    0x198, 0x199, 0x5, 0xde, 0x70, 0x2, 0x199, 0x1bd, 0x3, 0x2, 0x2, 0x2, 
    0x19a, 0x19c, 0x5, 0x100, 0x81, 0x2, 0x19b, 0x19d, 0x7, 0x6, 0x2, 0x2, 
    0x19c, 0x19b, 0x3, 0x2, 0x2, 0x2, 0x19c, 0x19d, 0x3, 0x2, 0x2, 0x2, 
    0x19d, 0x19e, 0x3, 0x2, 0x2, 0x2, 0x19e, 0x19f, 0x7, 0x9, 0x2, 0x2, 
    0x19f, 0x1a4, 0x5, 0xde, 0x70, 0x2, 0x1a0, 0x1a1, 0x7, 0x8, 0x2, 0x2, 
    0x1a1, 0x1a3, 0x5, 0xde, 0x70, 0x2, 0x1a2, 0x1a0, 0x3, 0x2, 0x2, 0x2, 
    0x1a3, 0x1a6, 0x3, 0x2, 0x2, 0x2, 0x1a4, 0x1a2, 0x3, 0x2, 0x2, 0x2, 
    0x1a4, 0x1a5, 0x3, 0x2, 0x2, 0x2, 0x1a5, 0x1a7, 0x3, 0x2, 0x2, 0x2, 
    0x1a6, 0x1a4, 0x3, 0x2, 0x2, 0x2, 0x1a7, 0x1a8, 0x7, 0xa, 0x2, 0x2, 
    0x1a8, 0x1bd, 0x3, 0x2, 0x2, 0x2, 0x1a9, 0x1aa, 0x5, 0xfe, 0x80, 0x2, 
    0x1aa, 0x1ab, 0x7, 0x6, 0x2, 0x2, 0x1ab, 0x1ac, 0x5, 0x104, 0x83, 0x2, 
    0x1ac, 0x1bd, 0x3, 0x2, 0x2, 0x2, 0x1ad, 0x1af, 0x5, 0xfe, 0x80, 0x2, 
    0x1ae, 0x1b0, 0x7, 0x6, 0x2, 0x2, 0x1af, 0x1ae, 0x3, 0x2, 0x2, 0x2, 
    0x1af, 0x1b0, 0x3, 0x2, 0x2, 0x2, 0x1b0, 0x1b1, 0x3, 0x2, 0x2, 0x2, 
    0x1b1, 0x1b2, 0x7, 0x9, 0x2, 0x2, 0x1b2, 0x1b7, 0x5, 0x104, 0x83, 0x2, 
    0x1b3, 0x1b4, 0x7, 0x8, 0x2, 0x2, 0x1b4, 0x1b6, 0x5, 0x104, 0x83, 0x2, 
    0x1b5, 0x1b3, 0x3, 0x2, 0x2, 0x2, 0x1b6, 0x1b9, 0x3, 0x2, 0x2, 0x2, 
    0x1b7, 0x1b5, 0x3, 0x2, 0x2, 0x2, 0x1b7, 0x1b8, 0x3, 0x2, 0x2, 0x2, 
    0x1b8, 0x1ba, 0x3, 0x2, 0x2, 0x2, 0x1b9, 0x1b7, 0x3, 0x2, 0x2, 0x2, 
    0x1ba, 0x1bb, 0x7, 0xa, 0x2, 0x2, 0x1bb, 0x1bd, 0x3, 0x2, 0x2, 0x2, 
    0x1bc, 0x195, 0x3, 0x2, 0x2, 0x2, 0x1bc, 0x196, 0x3, 0x2, 0x2, 0x2, 
    0x1bc, 0x19a, 0x3, 0x2, 0x2, 0x2, 0x1bc, 0x1a9, 0x3, 0x2, 0x2, 0x2, 
    0x1bc, 0x1ad, 0x3, 0x2, 0x2, 0x2, 0x1bd, 0x17, 0x3, 0x2, 0x2, 0x2, 0x1be, 
    0x1bf, 0x7, 0xf5, 0x2, 0x2, 0x1bf, 0x1c4, 0x5, 0x22, 0x12, 0x2, 0x1c0, 
    0x1c1, 0x7, 0x8, 0x2, 0x2, 0x1c1, 0x1c3, 0x5, 0x22, 0x12, 0x2, 0x1c2, 
    0x1c0, 0x3, 0x2, 0x2, 0x2, 0x1c3, 0x1c6, 0x3, 0x2, 0x2, 0x2, 0x1c4, 
    0x1c2, 0x3, 0x2, 0x2, 0x2, 0x1c4, 0x1c5, 0x3, 0x2, 0x2, 0x2, 0x1c5, 
    0x1c8, 0x3, 0x2, 0x2, 0x2, 0x1c6, 0x1c4, 0x3, 0x2, 0x2, 0x2, 0x1c7, 
    0x1be, 0x3, 0x2, 0x2, 0x2, 0x1c7, 0x1c8, 0x3, 0x2, 0x2, 0x2, 0x1c8, 
    0x1c9, 0x3, 0x2, 0x2, 0x2, 0x1c9, 0x1cd, 0x5, 0x1a, 0xe, 0x2, 0x1ca, 
    0x1cb, 0x7, 0xa9, 0x2, 0x2, 0x1cb, 0x1cc, 0x7, 0x31, 0x2, 0x2, 0x1cc, 
    0x1ce, 0x5, 0xc0, 0x61, 0x2, 0x1cd, 0x1ca, 0x3, 0x2, 0x2, 0x2, 0x1cd, 
    0x1ce, 0x3, 0x2, 0x2, 0x2, 0x1ce, 0x1d5, 0x3, 0x2, 0x2, 0x2, 0x1cf, 
    0x1d0, 0x7, 0x84, 0x2, 0x2, 0x1d0, 0x1d3, 0x7, 0x101, 0x2, 0x2, 0x1d1, 
    0x1d2, 0x7, 0xa6, 0x2, 0x2, 0x1d2, 0x1d4, 0x7, 0x101, 0x2, 0x2, 0x1d3, 
    0x1d1, 0x3, 0x2, 0x2, 0x2, 0x1d3, 0x1d4, 0x3, 0x2, 0x2, 0x2, 0x1d4, 
    0x1d6, 0x3, 0x2, 0x2, 0x2, 0x1d5, 0x1cf, 0x3, 0x2, 0x2, 0x2, 0x1d5, 
    0x1d6, 0x3, 0x2, 0x2, 0x2, 0x1d6, 0x19, 0x3, 0x2, 0x2, 0x2, 0x1d7, 0x1da, 
    0x9, 0x3, 0x2, 0x2, 0x1d8, 0x1db, 0x7, 0x19, 0x2, 0x2, 0x1d9, 0x1db, 
    0x7, 0x52, 0x2, 0x2, 0x1da, 0x1d8, 0x3, 0x2, 0x2, 0x2, 0x1da, 0x1d9, 
    0x3, 0x2, 0x2, 0x2, 0x1da, 0x1db, 0x3, 0x2, 0x2, 0x2, 0x1db, 0x1dc, 
    0x3, 0x2, 0x2, 0x2, 0x1dc, 0x1df, 0x5, 0xb8, 0x5d, 0x2, 0x1dd, 0x1de, 
    0x7, 0x6a, 0x2, 0x2, 0x1de, 0x1e0, 0x5, 0x24, 0x13, 0x2, 0x1df, 0x1dd, 
    0x3, 0x2, 0x2, 0x2, 0x1df, 0x1e0, 0x3, 0x2, 0x2, 0x2, 0x1e0, 0x1e3, 
    0x3, 0x2, 0x2, 0x2, 0x1e1, 0x1e2, 0x7, 0xf2, 0x2, 0x2, 0x1e2, 0x1e4, 
    0x5, 0xd8, 0x6d, 0x2, 0x1e3, 0x1e1, 0x3, 0x2, 0x2, 0x2, 0x1e3, 0x1e4, 
    0x3, 0x2, 0x2, 0x2, 0x1e4, 0x1e8, 0x3, 0x2, 0x2, 0x2, 0x1e5, 0x1e6, 
    0x7, 0x6b, 0x2, 0x2, 0x1e6, 0x1e7, 0x7, 0x31, 0x2, 0x2, 0x1e7, 0x1e9, 
    0x5, 0xbc, 0x5f, 0x2, 0x1e8, 0x1e5, 0x3, 0x2, 0x2, 0x2, 0x1e8, 0x1e9, 
    0x3, 0x2, 0x2, 0x2, 0x1e9, 0x1ec, 0x3, 0x2, 0x2, 0x2, 0x1ea, 0x1eb, 
    0x7, 0x6e, 0x2, 0x2, 0x1eb, 0x1ed, 0x5, 0xd8, 0x6d, 0x2, 0x1ec, 0x1ea, 
    0x3, 0x2, 0x2, 0x2, 0x1ec, 0x1ed, 0x3, 0x2, 0x2, 0x2, 0x1ed, 0x1f3, 
    0x3, 0x2, 0x2, 0x2, 0x1ee, 0x1ef, 0x7, 0x9, 0x2, 0x2, 0x1ef, 0x1f0, 
    0x5, 0x1a, 0xe, 0x2, 0x1f0, 0x1f1, 0x7, 0xa, 0x2, 0x2, 0x1f1, 0x1f3, 
    0x3, 0x2, 0x2, 0x2, 0x1f2, 0x1d7, 0x3, 0x2, 0x2, 0x2, 0x1f2, 0x1ee, 
    0x3, 0x2, 0x2, 0x2, 0x1f3, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x1f4, 0x1f5, 0x7, 
    0xf5, 0x2, 0x2, 0x1f5, 0x1fa, 0x5, 0x22, 0x12, 0x2, 0x1f6, 0x1f7, 0x7, 
    0x8, 0x2, 0x2, 0x1f7, 0x1f9, 0x5, 0x22, 0x12, 0x2, 0x1f8, 0x1f6, 0x3, 
    0x2, 0x2, 0x2, 0x1f9, 0x1fc, 0x3, 0x2, 0x2, 0x2, 0x1fa, 0x1f8, 0x3, 
    0x2, 0x2, 0x2, 0x1fa, 0x1fb, 0x3, 0x2, 0x2, 0x2, 0x1fb, 0x1fe, 0x3, 
    0x2, 0x2, 0x2, 0x1fc, 0x1fa, 0x3, 0x2, 0x2, 0x2, 0x1fd, 0x1f4, 0x3, 
    0x2, 0x2, 0x2, 0x1fd, 0x1fe, 0x3, 0x2, 0x2, 0x2, 0x1fe, 0x1ff, 0x3, 
    0x2, 0x2, 0x2, 0x1ff, 0x203, 0x5, 0x1e, 0x10, 0x2, 0x200, 0x201, 0x7, 
    0xa9, 0x2, 0x2, 0x201, 0x202, 0x7, 0x31, 0x2, 0x2, 0x202, 0x204, 0x5, 
    0xc0, 0x61, 0x2, 0x203, 0x200, 0x3, 0x2, 0x2, 0x2, 0x203, 0x204, 0x3, 
    0x2, 0x2, 0x2, 0x204, 0x20b, 0x3, 0x2, 0x2, 0x2, 0x205, 0x206, 0x7, 
    0x84, 0x2, 0x2, 0x206, 0x209, 0x7, 0x101, 0x2, 0x2, 0x207, 0x208, 0x7, 
    0xa6, 0x2, 0x2, 0x208, 0x20a, 0x7, 0x101, 0x2, 0x2, 0x209, 0x207, 0x3, 
    0x2, 0x2, 0x2, 0x209, 0x20a, 0x3, 0x2, 0x2, 0x2, 0x20a, 0x20c, 0x3, 
    0x2, 0x2, 0x2, 0x20b, 0x205, 0x3, 0x2, 0x2, 0x2, 0x20b, 0x20c, 0x3, 
    0x2, 0x2, 0x2, 0x20c, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x20d, 0x20e, 0x8, 0x10, 
    0x1, 0x2, 0x20e, 0x214, 0x5, 0x1a, 0xe, 0x2, 0x20f, 0x210, 0x7, 0x9, 
    0x2, 0x2, 0x210, 0x211, 0x5, 0x1e, 0x10, 0x2, 0x211, 0x212, 0x7, 0xa, 
    0x2, 0x2, 0x212, 0x214, 0x3, 0x2, 0x2, 0x2, 0x213, 0x20d, 0x3, 0x2, 
    0x2, 0x2, 0x213, 0x20f, 0x3, 0x2, 0x2, 0x2, 0x214, 0x221, 0x3, 0x2, 
    0x2, 0x2, 0x215, 0x216, 0xc, 0x4, 0x2, 0x2, 0x216, 0x219, 0x9, 0x4, 
    0x2, 0x2, 0x217, 0x21a, 0x7, 0x52, 0x2, 0x2, 0x218, 0x21a, 0x7, 0x19, 
    0x2, 0x2, 0x219, 0x217, 0x3, 0x2, 0x2, 0x2, 0x219, 0x218, 0x3, 0x2, 
    0x2, 0x2, 0x219, 0x21a, 0x3, 0x2, 0x2, 0x2, 0x21a, 0x21c, 0x3, 0x2, 
    0x2, 0x2, 0x21b, 0x21d, 0x5, 0x20, 0x11, 0x2, 0x21c, 0x21b, 0x3, 0x2, 
    0x2, 0x2, 0x21c, 0x21d, 0x3, 0x2, 0x2, 0x2, 0x21d, 0x21e, 0x3, 0x2, 
    0x2, 0x2, 0x21e, 0x220, 0x5, 0x1e, 0x10, 0x5, 0x21f, 0x215, 0x3, 0x2, 
    0x2, 0x2, 0x220, 0x223, 0x3, 0x2, 0x2, 0x2, 0x221, 0x21f, 0x3, 0x2, 
    0x2, 0x2, 0x221, 0x222, 0x3, 0x2, 0x2, 0x2, 0x222, 0x1f, 0x3, 0x2, 0x2, 
    0x2, 0x223, 0x221, 0x3, 0x2, 0x2, 0x2, 0x224, 0x22a, 0x7, 0x3e, 0x2, 
    0x2, 0x225, 0x226, 0x7, 0x31, 0x2, 0x2, 0x226, 0x227, 0x7, 0x9, 0x2, 
    0x2, 0x227, 0x228, 0x5, 0xc8, 0x65, 0x2, 0x228, 0x229, 0x7, 0xa, 0x2, 
    0x2, 0x229, 0x22b, 0x3, 0x2, 0x2, 0x2, 0x22a, 0x225, 0x3, 0x2, 0x2, 
    0x2, 0x22a, 0x22b, 0x3, 0x2, 0x2, 0x2, 0x22b, 0x21, 0x3, 0x2, 0x2, 0x2, 
    0x22c, 0x22e, 0x7, 0xb9, 0x2, 0x2, 0x22d, 0x22c, 0x3, 0x2, 0x2, 0x2, 
    0x22d, 0x22e, 0x3, 0x2, 0x2, 0x2, 0x22e, 0x22f, 0x3, 0x2, 0x2, 0x2, 
    0x22f, 0x231, 0x5, 0xf0, 0x79, 0x2, 0x230, 0x232, 0x5, 0xcc, 0x67, 0x2, 
    0x231, 0x230, 0x3, 0x2, 0x2, 0x2, 0x231, 0x232, 0x3, 0x2, 0x2, 0x2, 
    0x232, 0x233, 0x3, 0x2, 0x2, 0x2, 0x233, 0x234, 0x7, 0x21, 0x2, 0x2, 
    0x234, 0x235, 0x7, 0x9, 0x2, 0x2, 0x235, 0x236, 0x5, 0x12, 0xa, 0x2, 
    0x236, 0x237, 0x7, 0xa, 0x2, 0x2, 0x237, 0x23, 0x3, 0x2, 0x2, 0x2, 0x238, 
    0x239, 0x8, 0x13, 0x1, 0x2, 0x239, 0x23e, 0x5, 0x26, 0x14, 0x2, 0x23a, 
    0x23c, 0x7, 0x21, 0x2, 0x2, 0x23b, 0x23a, 0x3, 0x2, 0x2, 0x2, 0x23b, 
    0x23c, 0x3, 0x2, 0x2, 0x2, 0x23c, 0x23d, 0x3, 0x2, 0x2, 0x2, 0x23d, 
    0x23f, 0x5, 0xf2, 0x7a, 0x2, 0x23e, 0x23b, 0x3, 0x2, 0x2, 0x2, 0x23e, 
    0x23f, 0x3, 0x2, 0x2, 0x2, 0x23f, 0x259, 0x3, 0x2, 0x2, 0x2, 0x240, 
    0x241, 0x7, 0x9, 0x2, 0x2, 0x241, 0x242, 0x5, 0x12, 0xa, 0x2, 0x242, 
    0x24a, 0x7, 0xa, 0x2, 0x2, 0x243, 0x245, 0x7, 0x21, 0x2, 0x2, 0x244, 
    0x243, 0x3, 0x2, 0x2, 0x2, 0x244, 0x245, 0x3, 0x2, 0x2, 0x2, 0x245, 
    0x246, 0x3, 0x2, 0x2, 0x2, 0x246, 0x248, 0x5, 0xf2, 0x7a, 0x2, 0x247, 
    0x249, 0x5, 0xcc, 0x67, 0x2, 0x248, 0x247, 0x3, 0x2, 0x2, 0x2, 0x248, 
    0x249, 0x3, 0x2, 0x2, 0x2, 0x249, 0x24b, 0x3, 0x2, 0x2, 0x2, 0x24a, 
    0x244, 0x3, 0x2, 0x2, 0x2, 0x24a, 0x24b, 0x3, 0x2, 0x2, 0x2, 0x24b, 
    0x259, 0x3, 0x2, 0x2, 0x2, 0x24c, 0x24d, 0x7, 0x9, 0x2, 0x2, 0x24d, 
    0x24e, 0x5, 0x2c, 0x17, 0x2, 0x24e, 0x256, 0x7, 0xa, 0x2, 0x2, 0x24f, 
    0x251, 0x7, 0x21, 0x2, 0x2, 0x250, 0x24f, 0x3, 0x2, 0x2, 0x2, 0x250, 
    0x251, 0x3, 0x2, 0x2, 0x2, 0x251, 0x252, 0x3, 0x2, 0x2, 0x2, 0x252, 
    0x254, 0x5, 0xf2, 0x7a, 0x2, 0x253, 0x255, 0x5, 0xcc, 0x67, 0x2, 0x254, 
    0x253, 0x3, 0x2, 0x2, 0x2, 0x254, 0x255, 0x3, 0x2, 0x2, 0x2, 0x255, 
    0x257, 0x3, 0x2, 0x2, 0x2, 0x256, 0x250, 0x3, 0x2, 0x2, 0x2, 0x256, 
    0x257, 0x3, 0x2, 0x2, 0x2, 0x257, 0x259, 0x3, 0x2, 0x2, 0x2, 0x258, 
    0x238, 0x3, 0x2, 0x2, 0x2, 0x258, 0x240, 0x3, 0x2, 0x2, 0x2, 0x258, 
    0x24c, 0x3, 0x2, 0x2, 0x2, 0x259, 0x296, 0x3, 0x2, 0x2, 0x2, 0x25a, 
    0x25e, 0xc, 0x8, 0x2, 0x2, 0x25b, 0x25d, 0x5, 0x14, 0xb, 0x2, 0x25c, 
    0x25b, 0x3, 0x2, 0x2, 0x2, 0x25d, 0x260, 0x3, 0x2, 0x2, 0x2, 0x25e, 
    0x25c, 0x3, 0x2, 0x2, 0x2, 0x25e, 0x25f, 0x3, 0x2, 0x2, 0x2, 0x25f, 
    0x264, 0x3, 0x2, 0x2, 0x2, 0x260, 0x25e, 0x3, 0x2, 0x2, 0x2, 0x261, 
    0x265, 0x7, 0x8, 0x2, 0x2, 0x262, 0x263, 0x7, 0x41, 0x2, 0x2, 0x263, 
    0x265, 0x7, 0x7f, 0x2, 0x2, 0x264, 0x261, 0x3, 0x2, 0x2, 0x2, 0x264, 
    0x262, 0x3, 0x2, 0x2, 0x2, 0x265, 0x266, 0x3, 0x2, 0x2, 0x2, 0x266, 
    0x295, 0x5, 0x24, 0x13, 0x9, 0x267, 0x26b, 0xc, 0x7, 0x2, 0x2, 0x268, 
    0x26a, 0x5, 0x14, 0xb, 0x2, 0x269, 0x268, 0x3, 0x2, 0x2, 0x2, 0x26a, 
    0x26d, 0x3, 0x2, 0x2, 0x2, 0x26b, 0x269, 0x3, 0x2, 0x2, 0x2, 0x26b, 
    0x26c, 0x3, 0x2, 0x2, 0x2, 0x26c, 0x26e, 0x3, 0x2, 0x2, 0x2, 0x26d, 
    0x26b, 0x3, 0x2, 0x2, 0x2, 0x26e, 0x270, 0x7, 0x9a, 0x2, 0x2, 0x26f, 
    0x271, 0x5, 0x28, 0x15, 0x2, 0x270, 0x26f, 0x3, 0x2, 0x2, 0x2, 0x270, 
    0x271, 0x3, 0x2, 0x2, 0x2, 0x271, 0x272, 0x3, 0x2, 0x2, 0x2, 0x272, 
    0x273, 0x7, 0x7f, 0x2, 0x2, 0x273, 0x295, 0x5, 0x24, 0x13, 0x8, 0x274, 
    0x278, 0xc, 0x6, 0x2, 0x2, 0x275, 0x277, 0x5, 0x14, 0xb, 0x2, 0x276, 
    0x275, 0x3, 0x2, 0x2, 0x2, 0x277, 0x27a, 0x3, 0x2, 0x2, 0x2, 0x278, 
    0x276, 0x3, 0x2, 0x2, 0x2, 0x278, 0x279, 0x3, 0x2, 0x2, 0x2, 0x279, 
    0x27c, 0x3, 0x2, 0x2, 0x2, 0x27a, 0x278, 0x3, 0x2, 0x2, 0x2, 0x27b, 
    0x27d, 0x5, 0x2a, 0x16, 0x2, 0x27c, 0x27b, 0x3, 0x2, 0x2, 0x2, 0x27c, 
    0x27d, 0x3, 0x2, 0x2, 0x2, 0x27d, 0x27e, 0x3, 0x2, 0x2, 0x2, 0x27e, 
    0x27f, 0x7, 0x7f, 0x2, 0x2, 0x27f, 0x280, 0x5, 0x24, 0x13, 0x2, 0x280, 
    0x281, 0x7, 0xa7, 0x2, 0x2, 0x281, 0x282, 0x5, 0xda, 0x6e, 0x2, 0x282, 
    0x295, 0x3, 0x2, 0x2, 0x2, 0x283, 0x287, 0xc, 0x5, 0x2, 0x2, 0x284, 
    0x286, 0x5, 0x14, 0xb, 0x2, 0x285, 0x284, 0x3, 0x2, 0x2, 0x2, 0x286, 
    0x289, 0x3, 0x2, 0x2, 0x2, 0x287, 0x285, 0x3, 0x2, 0x2, 0x2, 0x287, 
    0x288, 0x3, 0x2, 0x2, 0x2, 0x288, 0x28b, 0x3, 0x2, 0x2, 0x2, 0x289, 
    0x287, 0x3, 0x2, 0x2, 0x2, 0x28a, 0x28c, 0x5, 0x2a, 0x16, 0x2, 0x28b, 
    0x28a, 0x3, 0x2, 0x2, 0x2, 0x28b, 0x28c, 0x3, 0x2, 0x2, 0x2, 0x28c, 
    0x28d, 0x3, 0x2, 0x2, 0x2, 0x28d, 0x28e, 0x7, 0x7f, 0x2, 0x2, 0x28e, 
    0x28f, 0x5, 0x24, 0x13, 0x2, 0x28f, 0x290, 0x7, 0xe9, 0x2, 0x2, 0x290, 
    0x291, 0x7, 0x9, 0x2, 0x2, 0x291, 0x292, 0x5, 0xc4, 0x63, 0x2, 0x292, 
    0x293, 0x7, 0xa, 0x2, 0x2, 0x293, 0x295, 0x3, 0x2, 0x2, 0x2, 0x294, 
    0x25a, 0x3, 0x2, 0x2, 0x2, 0x294, 0x267, 0x3, 0x2, 0x2, 0x2, 0x294, 
    0x274, 0x3, 0x2, 0x2, 0x2, 0x294, 0x283, 0x3, 0x2, 0x2, 0x2, 0x295, 
    0x298, 0x3, 0x2, 0x2, 0x2, 0x296, 0x294, 0x3, 0x2, 0x2, 0x2, 0x296, 
    0x297, 0x3, 0x2, 0x2, 0x2, 0x297, 0x25, 0x3, 0x2, 0x2, 0x2, 0x298, 0x296, 
    0x3, 0x2, 0x2, 0x2, 0x299, 0x29a, 0x5, 0xec, 0x77, 0x2, 0x29a, 0x29b, 
    0x7, 0xc, 0x2, 0x2, 0x29b, 0x29d, 0x3, 0x2, 0x2, 0x2, 0x29c, 0x299, 
    0x3, 0x2, 0x2, 0x2, 0x29c, 0x29d, 0x3, 0x2, 0x2, 0x2, 0x29d, 0x29e, 
    0x3, 0x2, 0x2, 0x2, 0x29e, 0x29f, 0x5, 0xee, 0x78, 0x2, 0x29f, 0x2a0, 
    0x7, 0xc, 0x2, 0x2, 0x2a0, 0x2a2, 0x3, 0x2, 0x2, 0x2, 0x2a1, 0x29c, 
    0x3, 0x2, 0x2, 0x2, 0x2a1, 0x2a2, 0x3, 0x2, 0x2, 0x2, 0x2a2, 0x2a3, 
    0x3, 0x2, 0x2, 0x2, 0x2a3, 0x2a4, 0x5, 0xf0, 0x79, 0x2, 0x2a4, 0x27, 
    0x3, 0x2, 0x2, 0x2, 0x2a5, 0x2ab, 0x7, 0x76, 0x2, 0x2, 0x2a6, 0x2a8, 
    0x9, 0x5, 0x2, 0x2, 0x2a7, 0x2a9, 0x7, 0xab, 0x2, 0x2, 0x2a8, 0x2a7, 
    0x3, 0x2, 0x2, 0x2, 0x2a8, 0x2a9, 0x3, 0x2, 0x2, 0x2, 0x2a9, 0x2ab, 
    0x3, 0x2, 0x2, 0x2, 0x2aa, 0x2a5, 0x3, 0x2, 0x2, 0x2, 0x2aa, 0x2a6, 
    0x3, 0x2, 0x2, 0x2, 0x2ab, 0x29, 0x3, 0x2, 0x2, 0x2, 0x2ac, 0x2b0, 0x5, 
    0x28, 0x15, 0x2, 0x2ad, 0x2b0, 0x7, 0xc5, 0x2, 0x2, 0x2ae, 0x2b0, 0x7, 
    0x1f, 0x2, 0x2, 0x2af, 0x2ac, 0x3, 0x2, 0x2, 0x2, 0x2af, 0x2ad, 0x3, 
    0x2, 0x2, 0x2, 0x2af, 0x2ae, 0x3, 0x2, 0x2, 0x2, 0x2b0, 0x2b, 0x3, 0x2, 
    0x2, 0x2, 0x2b1, 0x2b2, 0x7, 0xeb, 0x2, 0x2, 0x2b2, 0x2b3, 0x7, 0x9, 
    0x2, 0x2, 0x2b3, 0x2b4, 0x5, 0xd6, 0x6c, 0x2, 0x2b4, 0x2bc, 0x7, 0xa, 
    0x2, 0x2, 0x2b5, 0x2b6, 0x7, 0x8, 0x2, 0x2, 0x2b6, 0x2b7, 0x7, 0x9, 
    0x2, 0x2, 0x2b7, 0x2b8, 0x5, 0xd6, 0x6c, 0x2, 0x2b8, 0x2b9, 0x7, 0xa, 
    0x2, 0x2, 0x2b9, 0x2bb, 0x3, 0x2, 0x2, 0x2, 0x2ba, 0x2b5, 0x3, 0x2, 
    0x2, 0x2, 0x2bb, 0x2be, 0x3, 0x2, 0x2, 0x2, 0x2bc, 0x2ba, 0x3, 0x2, 
    0x2, 0x2, 0x2bc, 0x2bd, 0x3, 0x2, 0x2, 0x2, 0x2bd, 0x2d, 0x3, 0x2, 0x2, 
    0x2, 0x2be, 0x2bc, 0x3, 0x2, 0x2, 0x2, 0x2bf, 0x2c3, 0x7, 0x1d, 0x2, 
    0x2, 0x2c0, 0x2c1, 0x9, 0x6, 0x2, 0x2, 0x2c1, 0x2c3, 0x7, 0xd4, 0x2, 
    0x2, 0x2c2, 0x2bf, 0x3, 0x2, 0x2, 0x2, 0x2c2, 0x2c0, 0x3, 0x2, 0x2, 
    0x2, 0x2c3, 0x2c4, 0x3, 0x2, 0x2, 0x2, 0x2c4, 0x2c5, 0x5, 0xf0, 0x79, 
    0x2, 0x2c5, 0x2c6, 0x5, 0xfa, 0x7e, 0x2, 0x2c6, 0x2f, 0x3, 0x2, 0x2, 
    0x2, 0x2c7, 0x2cb, 0x7, 0x1e, 0x2, 0x2, 0x2c8, 0x2c9, 0x9, 0x6, 0x2, 
    0x2, 0x2c9, 0x2cb, 0x7, 0x75, 0x2, 0x2, 0x2ca, 0x2c7, 0x3, 0x2, 0x2, 
    0x2, 0x2ca, 0x2c8, 0x3, 0x2, 0x2, 0x2, 0x2cb, 0x2cc, 0x3, 0x2, 0x2, 
    0x2, 0x2cc, 0x2cd, 0x5, 0xf0, 0x79, 0x2, 0x2cd, 0x31, 0x3, 0x2, 0x2, 
    0x2, 0x2ce, 0x2d0, 0x7, 0x2e, 0x2, 0x2, 0x2cf, 0x2d1, 0x9, 0x7, 0x2, 
    0x2, 0x2d0, 0x2cf, 0x3, 0x2, 0x2, 0x2, 0x2d0, 0x2d1, 0x3, 0x2, 0x2, 
    0x2, 0x2d1, 0x33, 0x3, 0x2, 0x2, 0x2, 0x2d2, 0x2d3, 0x7, 0x3c, 0x2, 
    0x2, 0x2d3, 0x2d4, 0x5, 0xf0, 0x79, 0x2, 0x2d4, 0x2d5, 0x5, 0xfa, 0x7e, 
    0x2, 0x2d5, 0x35, 0x3, 0x2, 0x2, 0x2, 0x2d6, 0x2d7, 0x7, 0x3f, 0x2, 
    0x2, 0x2d7, 0x2d8, 0x5, 0xf0, 0x79, 0x2, 0x2d8, 0x37, 0x3, 0x2, 0x2, 
    0x2, 0x2d9, 0x2dc, 0x5, 0x3a, 0x1e, 0x2, 0x2da, 0x2dc, 0x5, 0x3c, 0x1f, 
    0x2, 0x2db, 0x2d9, 0x3, 0x2, 0x2, 0x2, 0x2db, 0x2da, 0x3, 0x2, 0x2, 
    0x2, 0x2dc, 0x39, 0x3, 0x2, 0x2, 0x2, 0x2dd, 0x2de, 0x7, 0x40, 0x2, 
    0x2, 0x2de, 0x2df, 0x7, 0xd4, 0x2, 0x2, 0x2df, 0x2e0, 0x5, 0xf0, 0x79, 
    0x2, 0x2e0, 0x2e1, 0x5, 0x9a, 0x4e, 0x2, 0x2e1, 0x3b, 0x3, 0x2, 0x2, 
    0x2, 0x2e2, 0x2e4, 0x7, 0x40, 0x2, 0x2, 0x2e3, 0x2e5, 0x7, 0xe6, 0x2, 
    0x2, 0x2e4, 0x2e3, 0x3, 0x2, 0x2, 0x2, 0x2e4, 0x2e5, 0x3, 0x2, 0x2, 
    0x2, 0x2e5, 0x2e6, 0x3, 0x2, 0x2, 0x2, 0x2e6, 0x2e7, 0x7, 0x75, 0x2, 
    0x2, 0x2e7, 0x2e8, 0x5, 0xfa, 0x7e, 0x2, 0x2e8, 0x2e9, 0x7, 0xa7, 0x2, 
    0x2, 0x2e9, 0x2ea, 0x5, 0xf0, 0x79, 0x2, 0x2ea, 0x2eb, 0x7, 0x9, 0x2, 
    0x2, 0x2eb, 0x2f0, 0x5, 0xde, 0x70, 0x2, 0x2ec, 0x2ed, 0x7, 0x8, 0x2, 
    0x2, 0x2ed, 0x2ef, 0x5, 0xde, 0x70, 0x2, 0x2ee, 0x2ec, 0x3, 0x2, 0x2, 
    0x2, 0x2ef, 0x2f2, 0x3, 0x2, 0x2, 0x2, 0x2f0, 0x2ee, 0x3, 0x2, 0x2, 
    0x2, 0x2f0, 0x2f1, 0x3, 0x2, 0x2, 0x2, 0x2f1, 0x2f3, 0x3, 0x2, 0x2, 
    0x2, 0x2f2, 0x2f0, 0x3, 0x2, 0x2, 0x2, 0x2f3, 0x2f4, 0x7, 0xa, 0x2, 
    0x2, 0x2f4, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x2f5, 0x2f6, 0x7, 0xf5, 0x2, 
    0x2, 0x2f6, 0x2fb, 0x5, 0x22, 0x12, 0x2, 0x2f7, 0x2f8, 0x7, 0x8, 0x2, 
    0x2, 0x2f8, 0x2fa, 0x5, 0x22, 0x12, 0x2, 0x2f9, 0x2f7, 0x3, 0x2, 0x2, 
    0x2, 0x2fa, 0x2fd, 0x3, 0x2, 0x2, 0x2, 0x2fb, 0x2f9, 0x3, 0x2, 0x2, 
    0x2, 0x2fb, 0x2fc, 0x3, 0x2, 0x2, 0x2, 0x2fc, 0x2ff, 0x3, 0x2, 0x2, 
    0x2, 0x2fd, 0x2fb, 0x3, 0x2, 0x2, 0x2, 0x2fe, 0x2f5, 0x3, 0x2, 0x2, 
    0x2, 0x2fe, 0x2ff, 0x3, 0x2, 0x2, 0x2, 0x2ff, 0x300, 0x3, 0x2, 0x2, 
    0x2, 0x300, 0x302, 0x7, 0x4d, 0x2, 0x2, 0x301, 0x303, 0x7, 0x6a, 0x2, 
    0x2, 0x302, 0x301, 0x3, 0x2, 0x2, 0x2, 0x302, 0x303, 0x3, 0x2, 0x2, 
    0x2, 0x303, 0x304, 0x3, 0x2, 0x2, 0x2, 0x304, 0x309, 0x5, 0x26, 0x14, 
    0x2, 0x305, 0x306, 0x7, 0xf2, 0x2, 0x2, 0x306, 0x30a, 0x5, 0xd8, 0x6d, 
    0x2, 0x307, 0x308, 0x7, 0x6a, 0x2, 0x2, 0x308, 0x30a, 0x5, 0x24, 0x13, 
    0x2, 0x309, 0x305, 0x3, 0x2, 0x2, 0x2, 0x309, 0x307, 0x3, 0x2, 0x2, 
    0x2, 0x309, 0x30a, 0x3, 0x2, 0x2, 0x2, 0x30a, 0x3f, 0x3, 0x2, 0x2, 0x2, 
    0x30b, 0x30c, 0x9, 0x8, 0x2, 0x2, 0x30c, 0x41, 0x3, 0x2, 0x2, 0x2, 0x30d, 
    0x30e, 0x7, 0x53, 0x2, 0x2, 0x30e, 0x30f, 0x5, 0xf0, 0x79, 0x2, 0x30f, 
    0x311, 0x7, 0xb0, 0x2, 0x2, 0x310, 0x312, 0x5, 0x128, 0x95, 0x2, 0x311, 
    0x310, 0x3, 0x2, 0x2, 0x2, 0x312, 0x313, 0x3, 0x2, 0x2, 0x2, 0x313, 
    0x311, 0x3, 0x2, 0x2, 0x2, 0x313, 0x314, 0x3, 0x2, 0x2, 0x2, 0x314, 
    0x31f, 0x3, 0x2, 0x2, 0x2, 0x315, 0x31c, 0x5, 0x64, 0x33, 0x2, 0x316, 
    0x318, 0x7, 0x8, 0x2, 0x2, 0x317, 0x316, 0x3, 0x2, 0x2, 0x2, 0x317, 
    0x318, 0x3, 0x2, 0x2, 0x2, 0x318, 0x319, 0x3, 0x2, 0x2, 0x2, 0x319, 
    0x31b, 0x5, 0x64, 0x33, 0x2, 0x31a, 0x317, 0x3, 0x2, 0x2, 0x2, 0x31b, 
    0x31e, 0x3, 0x2, 0x2, 0x2, 0x31c, 0x31a, 0x3, 0x2, 0x2, 0x2, 0x31c, 
    0x31d, 0x3, 0x2, 0x2, 0x2, 0x31d, 0x320, 0x3, 0x2, 0x2, 0x2, 0x31e, 
    0x31c, 0x3, 0x2, 0x2, 0x2, 0x31f, 0x315, 0x3, 0x2, 0x2, 0x2, 0x31f, 
    0x320, 0x3, 0x2, 0x2, 0x2, 0x320, 0x43, 0x3, 0x2, 0x2, 0x2, 0x321, 0x322, 
    0x7, 0x54, 0x2, 0x2, 0x322, 0x323, 0x5, 0x106, 0x84, 0x2, 0x323, 0x45, 
    0x3, 0x2, 0x2, 0x2, 0x324, 0x327, 0x5, 0x48, 0x25, 0x2, 0x325, 0x327, 
    0x5, 0x4a, 0x26, 0x2, 0x326, 0x324, 0x3, 0x2, 0x2, 0x2, 0x326, 0x325, 
    0x3, 0x2, 0x2, 0x2, 0x327, 0x47, 0x3, 0x2, 0x2, 0x2, 0x328, 0x329, 0x7, 
    0x55, 0x2, 0x2, 0x329, 0x32a, 0x7, 0xd4, 0x2, 0x2, 0x32a, 0x32b, 0x5, 
    0xf0, 0x79, 0x2, 0x32b, 0x49, 0x3, 0x2, 0x2, 0x2, 0x32c, 0x32d, 0x7, 
    0x55, 0x2, 0x2, 0x32d, 0x335, 0x7, 0x75, 0x2, 0x2, 0x32e, 0x32f, 0x5, 
    0xfa, 0x7e, 0x2, 0x32f, 0x330, 0x7, 0xa7, 0x2, 0x2, 0x330, 0x331, 0x5, 
    0xf0, 0x79, 0x2, 0x331, 0x336, 0x3, 0x2, 0x2, 0x2, 0x332, 0x333, 0x5, 
    0xf0, 0x79, 0x2, 0x333, 0x334, 0x5, 0xfa, 0x7e, 0x2, 0x334, 0x336, 0x3, 
    0x2, 0x2, 0x2, 0x335, 0x32e, 0x3, 0x2, 0x2, 0x2, 0x335, 0x332, 0x3, 
    0x2, 0x2, 0x2, 0x336, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x337, 0x338, 0x3, 0x2, 
    0x2, 0x2, 0x338, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x339, 0x33b, 0x5, 0x14, 
    0xb, 0x2, 0x33a, 0x339, 0x3, 0x2, 0x2, 0x2, 0x33b, 0x33e, 0x3, 0x2, 
    0x2, 0x2, 0x33c, 0x33a, 0x3, 0x2, 0x2, 0x2, 0x33c, 0x33d, 0x3, 0x2, 
    0x2, 0x2, 0x33d, 0x33f, 0x3, 0x2, 0x2, 0x2, 0x33e, 0x33c, 0x3, 0x2, 
    0x2, 0x2, 0x33f, 0x343, 0x7, 0x5e, 0x2, 0x2, 0x340, 0x342, 0x5, 0x50, 
    0x29, 0x2, 0x341, 0x340, 0x3, 0x2, 0x2, 0x2, 0x342, 0x345, 0x3, 0x2, 
    0x2, 0x2, 0x343, 0x341, 0x3, 0x2, 0x2, 0x2, 0x343, 0x344, 0x3, 0x2, 
    0x2, 0x2, 0x344, 0x346, 0x3, 0x2, 0x2, 0x2, 0x345, 0x343, 0x3, 0x2, 
    0x2, 0x2, 0x346, 0x347, 0x5, 0x6, 0x4, 0x2, 0x347, 0x4f, 0x3, 0x2, 0x2, 
    0x2, 0x348, 0x34d, 0x5, 0x52, 0x2a, 0x2, 0x349, 0x34d, 0x5, 0x58, 0x2d, 
    0x2, 0x34a, 0x34d, 0x7, 0xf3, 0x2, 0x2, 0x34b, 0x34d, 0x7, 0xf4, 0x2, 
    0x2, 0x34c, 0x348, 0x3, 0x2, 0x2, 0x2, 0x34c, 0x349, 0x3, 0x2, 0x2, 
    0x2, 0x34c, 0x34a, 0x3, 0x2, 0x2, 0x2, 0x34c, 0x34b, 0x3, 0x2, 0x2, 
    0x2, 0x34d, 0x51, 0x3, 0x2, 0x2, 0x2, 0x34e, 0x34f, 0x7, 0xca, 0x2, 
    0x2, 0x34f, 0x354, 0x5, 0x54, 0x2b, 0x2, 0x350, 0x351, 0x7, 0x8, 0x2, 
    0x2, 0x351, 0x353, 0x5, 0x54, 0x2b, 0x2, 0x352, 0x350, 0x3, 0x2, 0x2, 
    0x2, 0x353, 0x356, 0x3, 0x2, 0x2, 0x2, 0x354, 0x352, 0x3, 0x2, 0x2, 
    0x2, 0x354, 0x355, 0x3, 0x2, 0x2, 0x2, 0x355, 0x53, 0x3, 0x2, 0x2, 0x2, 
    0x356, 0x354, 0x3, 0x2, 0x2, 0x2, 0x357, 0x35a, 0x5, 0x56, 0x2c, 0x2, 
    0x358, 0x359, 0x9, 0x9, 0x2, 0x2, 0x359, 0x35b, 0x5, 0x56, 0x2c, 0x2, 
    0x35a, 0x358, 0x3, 0x2, 0x2, 0x2, 0x35a, 0x35b, 0x3, 0x2, 0x2, 0x2, 
    0x35b, 0x55, 0x3, 0x2, 0x2, 0x2, 0x35c, 0x360, 0x9, 0xa, 0x2, 0x2, 0x35d, 
    0x360, 0x7, 0x101, 0x2, 0x2, 0x35e, 0x360, 0x5, 0x106, 0x84, 0x2, 0x35f, 
    0x35c, 0x3, 0x2, 0x2, 0x2, 0x35f, 0x35d, 0x3, 0x2, 0x2, 0x2, 0x35f, 
    0x35e, 0x3, 0x2, 0x2, 0x2, 0x360, 0x57, 0x3, 0x2, 0x2, 0x2, 0x361, 0x362, 
    0x7, 0x68, 0x2, 0x2, 0x362, 0x363, 0x9, 0xb, 0x2, 0x2, 0x363, 0x59, 
    0x3, 0x2, 0x2, 0x2, 0x364, 0x365, 0x7, 0x5f, 0x2, 0x2, 0x365, 0x366, 
    0x5, 0xf0, 0x79, 0x2, 0x366, 0x367, 0x5, 0x128, 0x95, 0x2, 0x367, 0x5b, 
    0x3, 0x2, 0x2, 0x2, 0x368, 0x36b, 0x7, 0x64, 0x2, 0x2, 0x369, 0x36c, 
    0x5, 0x106, 0x84, 0x2, 0x36a, 0x36c, 0x5, 0x128, 0x95, 0x2, 0x36b, 0x369, 
    0x3, 0x2, 0x2, 0x2, 0x36b, 0x36a, 0x3, 0x2, 0x2, 0x2, 0x36c, 0x5d, 0x3, 
    0x2, 0x2, 0x2, 0x36d, 0x36e, 0x7, 0xf5, 0x2, 0x2, 0x36e, 0x373, 0x5, 
    0x22, 0x12, 0x2, 0x36f, 0x370, 0x7, 0x8, 0x2, 0x2, 0x370, 0x372, 0x5, 
    0x22, 0x12, 0x2, 0x371, 0x36f, 0x3, 0x2, 0x2, 0x2, 0x372, 0x375, 0x3, 
    0x2, 0x2, 0x2, 0x373, 0x371, 0x3, 0x2, 0x2, 0x2, 0x373, 0x374, 0x3, 
    0x2, 0x2, 0x2, 0x374, 0x377, 0x3, 0x2, 0x2, 0x2, 0x375, 0x373, 0x3, 
    0x2, 0x2, 0x2, 0x376, 0x36d, 0x3, 0x2, 0x2, 0x2, 0x376, 0x377, 0x3, 
    0x2, 0x2, 0x2, 0x377, 0x378, 0x3, 0x2, 0x2, 0x2, 0x378, 0x37a, 0x7, 
    0x77, 0x2, 0x2, 0x379, 0x37b, 0x7, 0x7d, 0x2, 0x2, 0x37a, 0x379, 0x3, 
    0x2, 0x2, 0x2, 0x37a, 0x37b, 0x3, 0x2, 0x2, 0x2, 0x37b, 0x37c, 0x3, 
    0x2, 0x2, 0x2, 0x37c, 0x388, 0x5, 0x26, 0x14, 0x2, 0x37d, 0x37e, 0x7, 
    0x9, 0x2, 0x2, 0x37e, 0x383, 0x5, 0xf4, 0x7b, 0x2, 0x37f, 0x380, 0x7, 
    0x8, 0x2, 0x2, 0x380, 0x382, 0x5, 0xf4, 0x7b, 0x2, 0x381, 0x37f, 0x3, 
    0x2, 0x2, 0x2, 0x382, 0x385, 0x3, 0x2, 0x2, 0x2, 0x383, 0x381, 0x3, 
    0x2, 0x2, 0x2, 0x383, 0x384, 0x3, 0x2, 0x2, 0x2, 0x384, 0x386, 0x3, 
    0x2, 0x2, 0x2, 0x385, 0x383, 0x3, 0x2, 0x2, 0x2, 0x386, 0x387, 0x7, 
    0xa, 0x2, 0x2, 0x387, 0x389, 0x3, 0x2, 0x2, 0x2, 0x388, 0x37d, 0x3, 
    0x2, 0x2, 0x2, 0x388, 0x389, 0x3, 0x2, 0x2, 0x2, 0x389, 0x38d, 0x3, 
    0x2, 0x2, 0x2, 0x38a, 0x38e, 0x5, 0x2c, 0x17, 0x2, 0x38b, 0x38e, 0x5, 
    0x18, 0xd, 0x2, 0x38c, 0x38e, 0x5, 0x1c, 0xf, 0x2, 0x38d, 0x38a, 0x3, 
    0x2, 0x2, 0x2, 0x38d, 0x38b, 0x3, 0x2, 0x2, 0x2, 0x38d, 0x38c, 0x3, 
    0x2, 0x2, 0x2, 0x38e, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x38f, 0x390, 0x7, 0x86, 
    0x2, 0x2, 0x390, 0x391, 0x5, 0xf0, 0x79, 0x2, 0x391, 0x39c, 0x5, 0x128, 
    0x95, 0x2, 0x392, 0x399, 0x5, 0x64, 0x33, 0x2, 0x393, 0x395, 0x7, 0x8, 
    0x2, 0x2, 0x394, 0x393, 0x3, 0x2, 0x2, 0x2, 0x394, 0x395, 0x3, 0x2, 
    0x2, 0x2, 0x395, 0x396, 0x3, 0x2, 0x2, 0x2, 0x396, 0x398, 0x5, 0x64, 
    0x33, 0x2, 0x397, 0x394, 0x3, 0x2, 0x2, 0x2, 0x398, 0x39b, 0x3, 0x2, 
    0x2, 0x2, 0x399, 0x397, 0x3, 0x2, 0x2, 0x2, 0x399, 0x39a, 0x3, 0x2, 
    0x2, 0x2, 0x39a, 0x39d, 0x3, 0x2, 0x2, 0x2, 0x39b, 0x399, 0x3, 0x2, 
    0x2, 0x2, 0x39c, 0x392, 0x3, 0x2, 0x2, 0x2, 0x39c, 0x39d, 0x3, 0x2, 
    0x2, 0x2, 0x39d, 0x61, 0x3, 0x2, 0x2, 0x2, 0x39e, 0x39f, 0x7, 0x85, 
    0x2, 0x2, 0x39f, 0x3a0, 0x5, 0xf0, 0x79, 0x2, 0x3a0, 0x3ab, 0x5, 0x128, 
    0x95, 0x2, 0x3a1, 0x3a8, 0x5, 0x64, 0x33, 0x2, 0x3a2, 0x3a4, 0x7, 0x8, 
    0x2, 0x2, 0x3a3, 0x3a2, 0x3, 0x2, 0x2, 0x2, 0x3a3, 0x3a4, 0x3, 0x2, 
    0x2, 0x2, 0x3a4, 0x3a5, 0x3, 0x2, 0x2, 0x2, 0x3a5, 0x3a7, 0x5, 0x64, 
    0x33, 0x2, 0x3a6, 0x3a3, 0x3, 0x2, 0x2, 0x2, 0x3a7, 0x3aa, 0x3, 0x2, 
    0x2, 0x2, 0x3a8, 0x3a6, 0x3, 0x2, 0x2, 0x2, 0x3a8, 0x3a9, 0x3, 0x2, 
    0x2, 0x2, 0x3a9, 0x3ac, 0x3, 0x2, 0x2, 0x2, 0x3aa, 0x3a8, 0x3, 0x2, 
    0x2, 0x2, 0x3ab, 0x3a1, 0x3, 0x2, 0x2, 0x2, 0x3ab, 0x3ac, 0x3, 0x2, 
    0x2, 0x2, 0x3ac, 0x63, 0x3, 0x2, 0x2, 0x2, 0x3ad, 0x3ae, 0x7, 0x4, 0x2, 
    0x2, 0x3ae, 0x3af, 0x5, 0x66, 0x34, 0x2, 0x3af, 0x3b0, 0x7, 0x5, 0x2, 
    0x2, 0x3b0, 0x3b3, 0x3, 0x2, 0x2, 0x2, 0x3b1, 0x3b3, 0x5, 0x66, 0x34, 
    0x2, 0x3b2, 0x3ad, 0x3, 0x2, 0x2, 0x2, 0x3b2, 0x3b1, 0x3, 0x2, 0x2, 
    0x2, 0x3b3, 0x3b4, 0x3, 0x2, 0x2, 0x2, 0x3b4, 0x3b5, 0x7, 0x6, 0x2, 
    0x2, 0x3b5, 0x3b6, 0x5, 0x128, 0x95, 0x2, 0x3b6, 0x65, 0x3, 0x2, 0x2, 
    0x2, 0x3b7, 0x3b8, 0x9, 0xc, 0x2, 0x2, 0x3b8, 0x67, 0x3, 0x2, 0x2, 0x2, 
    0x3b9, 0x3ba, 0x7, 0x87, 0x2, 0x2, 0x3ba, 0x3bd, 0x5, 0x128, 0x95, 0x2, 
    0x3bb, 0x3bc, 0x7, 0x56, 0x2, 0x2, 0x3bc, 0x3be, 0x9, 0xd, 0x2, 0x2, 
    0x3bd, 0x3bb, 0x3, 0x2, 0x2, 0x2, 0x3bd, 0x3be, 0x3, 0x2, 0x2, 0x2, 
    0x3be, 0x69, 0x3, 0x2, 0x2, 0x2, 0x3bf, 0x3c0, 0x7, 0x89, 0x2, 0x2, 
    0x3c0, 0x3c1, 0x5, 0xf0, 0x79, 0x2, 0x3c1, 0x6b, 0x3, 0x2, 0x2, 0x2, 
    0x3c2, 0x3c3, 0x7, 0x8b, 0x2, 0x2, 0x3c3, 0x3c4, 0x5, 0xf0, 0x79, 0x2, 
    0x3c4, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x3c5, 0x3c6, 0x7, 0x8a, 0x2, 0x2, 
    0x3c6, 0x3c7, 0x5, 0xf0, 0x79, 0x2, 0x3c7, 0x3c8, 0x5, 0xfa, 0x7e, 0x2, 
    0x3c8, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x3c9, 0x3ca, 0x7, 0x8c, 0x2, 0x2, 
    0x3ca, 0x3cb, 0x5, 0xf0, 0x79, 0x2, 0x3cb, 0x71, 0x3, 0x2, 0x2, 0x2, 
    0x3cc, 0x3cd, 0x7, 0x8d, 0x2, 0x2, 0x3cd, 0x73, 0x3, 0x2, 0x2, 0x2, 
    0x3ce, 0x3cf, 0x7, 0x8f, 0x2, 0x2, 0x3cf, 0x75, 0x3, 0x2, 0x2, 0x2, 
    0x3d0, 0x3d1, 0x9, 0xe, 0x2, 0x2, 0x3d1, 0x77, 0x3, 0x2, 0x2, 0x2, 0x3d2, 
    0x3d3, 0x7, 0xb1, 0x2, 0x2, 0x3d3, 0x3d4, 0x5, 0x106, 0x84, 0x2, 0x3d4, 
    0x3d6, 0x7, 0x24, 0x2, 0x2, 0x3d5, 0x3d7, 0x5, 0x7a, 0x3e, 0x2, 0x3d6, 
    0x3d5, 0x3, 0x2, 0x2, 0x2, 0x3d7, 0x3d8, 0x3, 0x2, 0x2, 0x2, 0x3d8, 
    0x3d6, 0x3, 0x2, 0x2, 0x2, 0x3d8, 0x3d9, 0x3, 0x2, 0x2, 0x2, 0x3d9, 
    0x3da, 0x3, 0x2, 0x2, 0x2, 0x3da, 0x3db, 0x7, 0x58, 0x2, 0x2, 0x3db, 
    0x79, 0x3, 0x2, 0x2, 0x2, 0x3dc, 0x3dd, 0x7, 0xca, 0x2, 0x2, 0x3dd, 
    0x3de, 0x5, 0x106, 0x84, 0x2, 0x3de, 0x3e4, 0x7, 0xa7, 0x2, 0x2, 0x3df, 
    0x3e5, 0x7, 0xaa, 0x2, 0x2, 0x3e0, 0x3e1, 0x7, 0x19, 0x2, 0x2, 0x3e1, 
    0x3e5, 0x7, 0x9c, 0x2, 0x2, 0x3e2, 0x3e3, 0x7, 0x19, 0x2, 0x2, 0x3e3, 
    0x3e5, 0x7, 0xae, 0x2, 0x2, 0x3e4, 0x3df, 0x3, 0x2, 0x2, 0x2, 0x3e4, 
    0x3e0, 0x3, 0x2, 0x2, 0x2, 0x3e4, 0x3e2, 0x3, 0x2, 0x2, 0x2, 0x3e5, 
    0x3e6, 0x3, 0x2, 0x2, 0x2, 0x3e6, 0x3e7, 0x7, 0xf, 0x2, 0x2, 0x3e7, 
    0x3e8, 0x5, 0x7c, 0x3f, 0x2, 0x3e8, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x3e9, 
    0x3ee, 0x5, 0x7e, 0x40, 0x2, 0x3ea, 0x3eb, 0x7, 0x10, 0x2, 0x2, 0x3eb, 
    0x3ed, 0x5, 0x7e, 0x40, 0x2, 0x3ec, 0x3ea, 0x3, 0x2, 0x2, 0x2, 0x3ed, 
    0x3f0, 0x3, 0x2, 0x2, 0x2, 0x3ee, 0x3ec, 0x3, 0x2, 0x2, 0x2, 0x3ee, 
    0x3ef, 0x3, 0x2, 0x2, 0x2, 0x3ef, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x3f0, 0x3ee, 
    0x3, 0x2, 0x2, 0x2, 0x3f1, 0x3fc, 0x5, 0x106, 0x84, 0x2, 0x3f2, 0x3f9, 
    0x5, 0x80, 0x41, 0x2, 0x3f3, 0x3f5, 0x7, 0x8, 0x2, 0x2, 0x3f4, 0x3f3, 
    0x3, 0x2, 0x2, 0x2, 0x3f4, 0x3f5, 0x3, 0x2, 0x2, 0x2, 0x3f5, 0x3f6, 
    0x3, 0x2, 0x2, 0x2, 0x3f6, 0x3f8, 0x5, 0x80, 0x41, 0x2, 0x3f7, 0x3f4, 
    0x3, 0x2, 0x2, 0x2, 0x3f8, 0x3fb, 0x3, 0x2, 0x2, 0x2, 0x3f9, 0x3f7, 
    0x3, 0x2, 0x2, 0x2, 0x3f9, 0x3fa, 0x3, 0x2, 0x2, 0x2, 0x3fa, 0x3fd, 
    0x3, 0x2, 0x2, 0x2, 0x3fb, 0x3f9, 0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3f2, 
    0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3fd, 0x3, 0x2, 0x2, 0x2, 0x3fd, 0x7f, 0x3, 
    0x2, 0x2, 0x2, 0x3fe, 0x419, 0x5, 0x106, 0x84, 0x2, 0x3ff, 0x419, 0x7, 
    0xe3, 0x2, 0x2, 0x400, 0x419, 0x7, 0x21, 0x2, 0x2, 0x401, 0x419, 0x7, 
    0xd0, 0x2, 0x2, 0x402, 0x419, 0x7, 0x23, 0x2, 0x2, 0x403, 0x419, 0x7, 
    0x93, 0x2, 0x2, 0x404, 0x419, 0x7, 0x8e, 0x2, 0x2, 0x405, 0x419, 0x7, 
    0xa, 0x2, 0x2, 0x406, 0x419, 0x7, 0xd, 0x2, 0x2, 0x407, 0x419, 0x7, 
    0x11, 0x2, 0x2, 0x408, 0x419, 0x7, 0xe, 0x2, 0x2, 0x409, 0x419, 0x7, 
    0x7, 0x2, 0x2, 0x40a, 0x419, 0x7, 0x12, 0x2, 0x2, 0x40b, 0x419, 0x7, 
    0x13, 0x2, 0x2, 0x40c, 0x419, 0x7, 0xfc, 0x2, 0x2, 0x40d, 0x419, 0x5, 
    0x82, 0x42, 0x2, 0x40e, 0x419, 0x5, 0x114, 0x8b, 0x2, 0x40f, 0x419, 
    0x5, 0x118, 0x8d, 0x2, 0x410, 0x419, 0x5, 0x11c, 0x8f, 0x2, 0x411, 0x419, 
    0x5, 0x120, 0x91, 0x2, 0x412, 0x419, 0x5, 0x122, 0x92, 0x2, 0x413, 0x419, 
    0x5, 0x124, 0x93, 0x2, 0x414, 0x419, 0x7, 0xfb, 0x2, 0x2, 0x415, 0x419, 
    0x7, 0xfe, 0x2, 0x2, 0x416, 0x419, 0x5, 0x126, 0x94, 0x2, 0x417, 0x419, 
    0x5, 0x12a, 0x96, 0x2, 0x418, 0x3fe, 0x3, 0x2, 0x2, 0x2, 0x418, 0x3ff, 
    0x3, 0x2, 0x2, 0x2, 0x418, 0x400, 0x3, 0x2, 0x2, 0x2, 0x418, 0x401, 
    0x3, 0x2, 0x2, 0x2, 0x418, 0x402, 0x3, 0x2, 0x2, 0x2, 0x418, 0x403, 
    0x3, 0x2, 0x2, 0x2, 0x418, 0x404, 0x3, 0x2, 0x2, 0x2, 0x418, 0x405, 
    0x3, 0x2, 0x2, 0x2, 0x418, 0x406, 0x3, 0x2, 0x2, 0x2, 0x418, 0x407, 
    0x3, 0x2, 0x2, 0x2, 0x418, 0x408, 0x3, 0x2, 0x2, 0x2, 0x418, 0x409, 
    0x3, 0x2, 0x2, 0x2, 0x418, 0x40a, 0x3, 0x2, 0x2, 0x2, 0x418, 0x40b, 
    0x3, 0x2, 0x2, 0x2, 0x418, 0x40c, 0x3, 0x2, 0x2, 0x2, 0x418, 0x40d, 
    0x3, 0x2, 0x2, 0x2, 0x418, 0x40e, 0x3, 0x2, 0x2, 0x2, 0x418, 0x40f, 
    0x3, 0x2, 0x2, 0x2, 0x418, 0x410, 0x3, 0x2, 0x2, 0x2, 0x418, 0x411, 
    0x3, 0x2, 0x2, 0x2, 0x418, 0x412, 0x3, 0x2, 0x2, 0x2, 0x418, 0x413, 
    0x3, 0x2, 0x2, 0x2, 0x418, 0x414, 0x3, 0x2, 0x2, 0x2, 0x418, 0x415, 
    0x3, 0x2, 0x2, 0x2, 0x418, 0x416, 0x3, 0x2, 0x2, 0x2, 0x418, 0x417, 
    0x3, 0x2, 0x2, 0x2, 0x419, 0x81, 0x3, 0x2, 0x2, 0x2, 0x41a, 0x41b, 0x9, 
    0xf, 0x2, 0x2, 0x41b, 0x83, 0x3, 0x2, 0x2, 0x2, 0x41c, 0x41d, 0x7, 0xc8, 
    0x2, 0x2, 0x41d, 0x41f, 0x5, 0x116, 0x8c, 0x2, 0x41e, 0x420, 0x5, 0x86, 
    0x44, 0x2, 0x41f, 0x41e, 0x3, 0x2, 0x2, 0x2, 0x41f, 0x420, 0x3, 0x2, 
    0x2, 0x2, 0x420, 0x85, 0x3, 0x2, 0x2, 0x2, 0x421, 0x424, 0x9, 0x10, 
    0x2, 0x2, 0x422, 0x424, 0x9, 0x11, 0x2, 0x2, 0x423, 0x421, 0x3, 0x2, 
    0x2, 0x2, 0x423, 0x422, 0x3, 0x2, 0x2, 0x2, 0x424, 0x87, 0x3, 0x2, 0x2, 
    0x2, 0x425, 0x426, 0x7, 0xc9, 0x2, 0x2, 0x426, 0x89, 0x3, 0x2, 0x2, 
    0x2, 0x427, 0x428, 0x7, 0xcd, 0x2, 0x2, 0x428, 0x8b, 0x3, 0x2, 0x2, 
    0x2, 0x429, 0x42a, 0x7, 0xd1, 0x2, 0x2, 0x42a, 0x42b, 0x5, 0xf0, 0x79, 
    0x2, 0x42b, 0x42c, 0x5, 0xfa, 0x7e, 0x2, 0x42c, 0x8d, 0x3, 0x2, 0x2, 
    0x2, 0x42d, 0x42e, 0x7, 0xd2, 0x2, 0x2, 0x42e, 0x42f, 0x5, 0xf0, 0x79, 
    0x2, 0x42f, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x430, 0x431, 0x7, 0xd5, 0x2, 
    0x2, 0x431, 0x91, 0x3, 0x2, 0x2, 0x2, 0x432, 0x434, 0x7, 0xe1, 0x2, 
    0x2, 0x433, 0x435, 0x7, 0xd4, 0x2, 0x2, 0x434, 0x433, 0x3, 0x2, 0x2, 
    0x2, 0x434, 0x435, 0x3, 0x2, 0x2, 0x2, 0x435, 0x436, 0x3, 0x2, 0x2, 
    0x2, 0x436, 0x437, 0x5, 0xf0, 0x79, 0x2, 0x437, 0x93, 0x3, 0x2, 0x2, 
    0x2, 0x438, 0x43a, 0x7, 0xdd, 0x2, 0x2, 0x439, 0x43b, 0x7, 0x67, 0x2, 
    0x2, 0x43a, 0x439, 0x3, 0x2, 0x2, 0x2, 0x43a, 0x43b, 0x3, 0x2, 0x2, 
    0x2, 0x43b, 0x95, 0x3, 0x2, 0x2, 0x2, 0x43c, 0x43d, 0x7, 0xf5, 0x2, 
    0x2, 0x43d, 0x442, 0x5, 0x22, 0x12, 0x2, 0x43e, 0x43f, 0x7, 0x8, 0x2, 
    0x2, 0x43f, 0x441, 0x5, 0x22, 0x12, 0x2, 0x440, 0x43e, 0x3, 0x2, 0x2, 
    0x2, 0x441, 0x444, 0x3, 0x2, 0x2, 0x2, 0x442, 0x440, 0x3, 0x2, 0x2, 
    0x2, 0x442, 0x443, 0x3, 0x2, 0x2, 0x2, 0x443, 0x446, 0x3, 0x2, 0x2, 
    0x2, 0x444, 0x442, 0x3, 0x2, 0x2, 0x2, 0x445, 0x43c, 0x3, 0x2, 0x2, 
    0x2, 0x445, 0x446, 0x3, 0x2, 0x2, 0x2, 0x446, 0x447, 0x3, 0x2, 0x2, 
    0x2, 0x447, 0x448, 0x7, 0xe8, 0x2, 0x2, 0x448, 0x449, 0x5, 0x26, 0x14, 
    0x2, 0x449, 0x44a, 0x7, 0xc6, 0x2, 0x2, 0x44a, 0x44b, 0x5, 0xf4, 0x7b, 
    0x2, 0x44b, 0x44c, 0x7, 0x6, 0x2, 0x2, 0x44c, 0x454, 0x5, 0xde, 0x70, 
    0x2, 0x44d, 0x44e, 0x7, 0x8, 0x2, 0x2, 0x44e, 0x44f, 0x5, 0xf4, 0x7b, 
    0x2, 0x44f, 0x450, 0x7, 0x6, 0x2, 0x2, 0x450, 0x451, 0x5, 0xde, 0x70, 
    0x2, 0x451, 0x453, 0x3, 0x2, 0x2, 0x2, 0x452, 0x44d, 0x3, 0x2, 0x2, 
    0x2, 0x453, 0x456, 0x3, 0x2, 0x2, 0x2, 0x454, 0x452, 0x3, 0x2, 0x2, 
    0x2, 0x454, 0x455, 0x3, 0x2, 0x2, 0x2, 0x455, 0x45b, 0x3, 0x2, 0x2, 
    0x2, 0x456, 0x454, 0x3, 0x2, 0x2, 0x2, 0x457, 0x458, 0x7, 0xf2, 0x2, 
    0x2, 0x458, 0x45c, 0x5, 0xd8, 0x6d, 0x2, 0x459, 0x45a, 0x7, 0x6a, 0x2, 
    0x2, 0x45a, 0x45c, 0x5, 0x24, 0x13, 0x2, 0x45b, 0x457, 0x3, 0x2, 0x2, 
    0x2, 0x45b, 0x459, 0x3, 0x2, 0x2, 0x2, 0x45b, 0x45c, 0x3, 0x2, 0x2, 
    0x2, 0x45c, 0x97, 0x3, 0x2, 0x2, 0x2, 0x45d, 0x460, 0x7, 0xe7, 0x2, 
    0x2, 0x45e, 0x461, 0x5, 0x106, 0x84, 0x2, 0x45f, 0x461, 0x5, 0x128, 
    0x95, 0x2, 0x460, 0x45e, 0x3, 0x2, 0x2, 0x2, 0x460, 0x45f, 0x3, 0x2, 
    0x2, 0x2, 0x461, 0x99, 0x3, 0x2, 0x2, 0x2, 0x462, 0x465, 0x7, 0x9, 0x2, 
    0x2, 0x463, 0x466, 0x5, 0x9e, 0x50, 0x2, 0x464, 0x466, 0x3, 0x2, 0x2, 
    0x2, 0x465, 0x463, 0x3, 0x2, 0x2, 0x2, 0x465, 0x464, 0x3, 0x2, 0x2, 
    0x2, 0x466, 0x46e, 0x3, 0x2, 0x2, 0x2, 0x467, 0x46a, 0x7, 0x8, 0x2, 
    0x2, 0x468, 0x46b, 0x5, 0x9e, 0x50, 0x2, 0x469, 0x46b, 0x3, 0x2, 0x2, 
    0x2, 0x46a, 0x468, 0x3, 0x2, 0x2, 0x2, 0x46a, 0x469, 0x3, 0x2, 0x2, 
    0x2, 0x46b, 0x46d, 0x3, 0x2, 0x2, 0x2, 0x46c, 0x467, 0x3, 0x2, 0x2, 
    0x2, 0x46d, 0x470, 0x3, 0x2, 0x2, 0x2, 0x46e, 0x46c, 0x3, 0x2, 0x2, 
    0x2, 0x46e, 0x46f, 0x3, 0x2, 0x2, 0x2, 0x46f, 0x471, 0x3, 0x2, 0x2, 
    0x2, 0x470, 0x46e, 0x3, 0x2, 0x2, 0x2, 0x471, 0x47a, 0x7, 0xa, 0x2, 
    0x2, 0x472, 0x479, 0x5, 0x9c, 0x4f, 0x2, 0x473, 0x479, 0x7, 0xbc, 0x2, 
    0x2, 0x474, 0x475, 0x7, 0xbf, 0x2, 0x2, 0x475, 0x479, 0x5, 0x116, 0x8c, 
    0x2, 0x476, 0x477, 0x7, 0xd6, 0x2, 0x2, 0x477, 0x479, 0x5, 0x116, 0x8c, 
    0x2, 0x478, 0x472, 0x3, 0x2, 0x2, 0x2, 0x478, 0x473, 0x3, 0x2, 0x2, 
    0x2, 0x478, 0x474, 0x3, 0x2, 0x2, 0x2, 0x478, 0x476, 0x3, 0x2, 0x2, 
    0x2, 0x479, 0x47c, 0x3, 0x2, 0x2, 0x2, 0x47a, 0x478, 0x3, 0x2, 0x2, 
    0x2, 0x47a, 0x47b, 0x3, 0x2, 0x2, 0x2, 0x47b, 0x9b, 0x3, 0x2, 0x2, 0x2, 
    0x47c, 0x47a, 0x3, 0x2, 0x2, 0x2, 0x47d, 0x47e, 0x7, 0xaf, 0x2, 0x2, 
    0x47e, 0x47f, 0x7, 0x31, 0x2, 0x2, 0x47f, 0x480, 0x7, 0x9, 0x2, 0x2, 
    0x480, 0x485, 0x5, 0xde, 0x70, 0x2, 0x481, 0x482, 0x7, 0x8, 0x2, 0x2, 
    0x482, 0x484, 0x5, 0xde, 0x70, 0x2, 0x483, 0x481, 0x3, 0x2, 0x2, 0x2, 
    0x484, 0x487, 0x3, 0x2, 0x2, 0x2, 0x485, 0x483, 0x3, 0x2, 0x2, 0x2, 
    0x485, 0x486, 0x3, 0x2, 0x2, 0x2, 0x486, 0x488, 0x3, 0x2, 0x2, 0x2, 
    0x487, 0x485, 0x3, 0x2, 0x2, 0x2, 0x488, 0x489, 0x7, 0xa, 0x2, 0x2, 
    0x489, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x48a, 0x48b, 0x5, 0xf4, 0x7b, 0x2, 
    0x48b, 0x490, 0x5, 0xa6, 0x54, 0x2, 0x48c, 0x48d, 0x7, 0xa0, 0x2, 0x2, 
    0x48d, 0x48f, 0x7, 0xa2, 0x2, 0x2, 0x48e, 0x48c, 0x3, 0x2, 0x2, 0x2, 
    0x48f, 0x492, 0x3, 0x2, 0x2, 0x2, 0x490, 0x48e, 0x3, 0x2, 0x2, 0x2, 
    0x490, 0x491, 0x3, 0x2, 0x2, 0x2, 0x491, 0x9f, 0x3, 0x2, 0x2, 0x2, 0x492, 
    0x490, 0x3, 0x2, 0x2, 0x2, 0x493, 0x494, 0x7, 0x9, 0x2, 0x2, 0x494, 
    0x499, 0x5, 0xf4, 0x7b, 0x2, 0x495, 0x496, 0x7, 0x8, 0x2, 0x2, 0x496, 
    0x498, 0x5, 0xf4, 0x7b, 0x2, 0x497, 0x495, 0x3, 0x2, 0x2, 0x2, 0x498, 
    0x49b, 0x3, 0x2, 0x2, 0x2, 0x499, 0x497, 0x3, 0x2, 0x2, 0x2, 0x499, 
    0x49a, 0x3, 0x2, 0x2, 0x2, 0x49a, 0x49c, 0x3, 0x2, 0x2, 0x2, 0x49b, 
    0x499, 0x3, 0x2, 0x2, 0x2, 0x49c, 0x49d, 0x7, 0xa, 0x2, 0x2, 0x49d, 
    0xa1, 0x3, 0x2, 0x2, 0x2, 0x49e, 0x4a0, 0x7, 0xe6, 0x2, 0x2, 0x49f, 
    0x49e, 0x3, 0x2, 0x2, 0x2, 0x49f, 0x4a0, 0x3, 0x2, 0x2, 0x2, 0x4a0, 
    0x4a1, 0x3, 0x2, 0x2, 0x2, 0x4a1, 0x4a3, 0x9, 0x12, 0x2, 0x2, 0x4a2, 
    0x4a4, 0x5, 0xfa, 0x7e, 0x2, 0x4a3, 0x4a2, 0x3, 0x2, 0x2, 0x2, 0x4a3, 
    0x4a4, 0x3, 0x2, 0x2, 0x2, 0x4a4, 0x4a6, 0x3, 0x2, 0x2, 0x2, 0x4a5, 
    0x4a7, 0x7, 0xa7, 0x2, 0x2, 0x4a6, 0x4a5, 0x3, 0x2, 0x2, 0x2, 0x4a6, 
    0x4a7, 0x3, 0x2, 0x2, 0x2, 0x4a7, 0x4b4, 0x3, 0x2, 0x2, 0x2, 0x4a8, 
    0x4b5, 0x5, 0xde, 0x70, 0x2, 0x4a9, 0x4aa, 0x7, 0x9, 0x2, 0x2, 0x4aa, 
    0x4af, 0x5, 0xde, 0x70, 0x2, 0x4ab, 0x4ac, 0x7, 0x8, 0x2, 0x2, 0x4ac, 
    0x4ae, 0x5, 0xde, 0x70, 0x2, 0x4ad, 0x4ab, 0x3, 0x2, 0x2, 0x2, 0x4ae, 
    0x4b1, 0x3, 0x2, 0x2, 0x2, 0x4af, 0x4ad, 0x3, 0x2, 0x2, 0x2, 0x4af, 
    0x4b0, 0x3, 0x2, 0x2, 0x2, 0x4b0, 0x4b2, 0x3, 0x2, 0x2, 0x2, 0x4b1, 
    0x4af, 0x3, 0x2, 0x2, 0x2, 0x4b2, 0x4b3, 0x7, 0xa, 0x2, 0x2, 0x4b3, 
    0x4b5, 0x3, 0x2, 0x2, 0x2, 0x4b4, 0x4a8, 0x3, 0x2, 0x2, 0x2, 0x4b4, 
    0x4a9, 0x3, 0x2, 0x2, 0x2, 0x4b5, 0x4b8, 0x3, 0x2, 0x2, 0x2, 0x4b6, 
    0x4b7, 0x7, 0xe9, 0x2, 0x2, 0x4b7, 0x4b9, 0x5, 0xa4, 0x53, 0x2, 0x4b8, 
    0x4b6, 0x3, 0x2, 0x2, 0x2, 0x4b8, 0x4b9, 0x3, 0x2, 0x2, 0x2, 0x4b9, 
    0x4bc, 0x3, 0x2, 0x2, 0x2, 0x4ba, 0x4bb, 0x7, 0xf5, 0x2, 0x2, 0x4bb, 
    0x4bd, 0x5, 0xd2, 0x6a, 0x2, 0x4bc, 0x4ba, 0x3, 0x2, 0x2, 0x2, 0x4bc, 
    0x4bd, 0x3, 0x2, 0x2, 0x2, 0x4bd, 0xa3, 0x3, 0x2, 0x2, 0x2, 0x4be, 0x4bf, 
    0x9, 0x13, 0x2, 0x2, 0x4bf, 0xa5, 0x3, 0x2, 0x2, 0x2, 0x4c0, 0x4c1, 
    0x5, 0xa8, 0x55, 0x2, 0x4c1, 0xa7, 0x3, 0x2, 0x2, 0x2, 0x4c2, 0x502, 
    0x7, 0x2d, 0x2, 0x2, 0x4c3, 0x4cb, 0x7, 0x78, 0x2, 0x2, 0x4c4, 0x4c5, 
    0x7, 0x9, 0x2, 0x2, 0x4c5, 0x4c7, 0x5, 0x10a, 0x86, 0x2, 0x4c6, 0x4c8, 
    0x9, 0x14, 0x2, 0x2, 0x4c7, 0x4c6, 0x3, 0x2, 0x2, 0x2, 0x4c7, 0x4c8, 
    0x3, 0x2, 0x2, 0x2, 0x4c8, 0x4c9, 0x3, 0x2, 0x2, 0x2, 0x4c9, 0x4ca, 
    0x7, 0xa, 0x2, 0x2, 0x4ca, 0x4cc, 0x3, 0x2, 0x2, 0x2, 0x4cb, 0x4c4, 
    0x3, 0x2, 0x2, 0x2, 0x4cb, 0x4cc, 0x3, 0x2, 0x2, 0x2, 0x4cc, 0x502, 
    0x3, 0x2, 0x2, 0x2, 0x4cd, 0x4dc, 0x7, 0x4b, 0x2, 0x2, 0x4ce, 0x4cf, 
    0x7, 0x9, 0x2, 0x2, 0x4cf, 0x4d1, 0x5, 0x10a, 0x86, 0x2, 0x4d0, 0x4d2, 
    0x7, 0x50, 0x2, 0x2, 0x4d1, 0x4d0, 0x3, 0x2, 0x2, 0x2, 0x4d1, 0x4d2, 
    0x3, 0x2, 0x2, 0x2, 0x4d2, 0x4d8, 0x3, 0x2, 0x2, 0x2, 0x4d3, 0x4d4, 
    0x7, 0x8, 0x2, 0x2, 0x4d4, 0x4d6, 0x5, 0x10c, 0x87, 0x2, 0x4d5, 0x4d7, 
    0x7, 0x50, 0x2, 0x2, 0x4d6, 0x4d5, 0x3, 0x2, 0x2, 0x2, 0x4d6, 0x4d7, 
    0x3, 0x2, 0x2, 0x2, 0x4d7, 0x4d9, 0x3, 0x2, 0x2, 0x2, 0x4d8, 0x4d3, 
    0x3, 0x2, 0x2, 0x2, 0x4d8, 0x4d9, 0x3, 0x2, 0x2, 0x2, 0x4d9, 0x4da, 
    0x3, 0x2, 0x2, 0x2, 0x4da, 0x4db, 0x7, 0xa, 0x2, 0x2, 0x4db, 0x4dd, 
    0x3, 0x2, 0x2, 0x2, 0x4dc, 0x4ce, 0x3, 0x2, 0x2, 0x2, 0x4dc, 0x4dd, 
    0x3, 0x2, 0x2, 0x2, 0x4dd, 0x502, 0x3, 0x2, 0x2, 0x2, 0x4de, 0x4e6, 
    0x7, 0x65, 0x2, 0x2, 0x4df, 0x4e0, 0x7, 0x9, 0x2, 0x2, 0x4e0, 0x4e2, 
    0x5, 0x10a, 0x86, 0x2, 0x4e1, 0x4e3, 0x9, 0x14, 0x2, 0x2, 0x4e2, 0x4e1, 
    0x3, 0x2, 0x2, 0x2, 0x4e2, 0x4e3, 0x3, 0x2, 0x2, 0x2, 0x4e3, 0x4e4, 
    0x3, 0x2, 0x2, 0x2, 0x4e4, 0x4e5, 0x7, 0xa, 0x2, 0x2, 0x4e5, 0x4e7, 
    0x3, 0x2, 0x2, 0x2, 0x4e6, 0x4df, 0x3, 0x2, 0x2, 0x2, 0x4e6, 0x4e7, 
    0x3, 0x2, 0x2, 0x2, 0x4e7, 0x502, 0x3, 0x2, 0x2, 0x2, 0x4e8, 0x502, 
    0x7, 0x47, 0x2, 0x2, 0x4e9, 0x502, 0x7, 0xd9, 0x2, 0x2, 0x4ea, 0x502, 
    0x7, 0xda, 0x2, 0x2, 0x4eb, 0x502, 0x5, 0xaa, 0x56, 0x2, 0x4ec, 0x502, 
    0x5, 0xb0, 0x59, 0x2, 0x4ed, 0x502, 0x5, 0xb2, 0x5a, 0x2, 0x4ee, 0x4ef, 
    0x7, 0x26, 0x2, 0x2, 0x4ef, 0x4f0, 0x7, 0x9, 0x2, 0x2, 0x4f0, 0x4f2, 
    0x5, 0x10e, 0x88, 0x2, 0x4f1, 0x4f3, 0x9, 0x15, 0x2, 0x2, 0x4f2, 0x4f1, 
    0x3, 0x2, 0x2, 0x2, 0x4f2, 0x4f3, 0x3, 0x2, 0x2, 0x2, 0x4f3, 0x4f4, 
    0x3, 0x2, 0x2, 0x2, 0x4f4, 0x4f5, 0x7, 0xa, 0x2, 0x2, 0x4f5, 0x502, 
    0x3, 0x2, 0x2, 0x2, 0x4f6, 0x4fe, 0x7, 0xec, 0x2, 0x2, 0x4f7, 0x4f8, 
    0x7, 0x9, 0x2, 0x2, 0x4f8, 0x4fa, 0x5, 0x10e, 0x88, 0x2, 0x4f9, 0x4fb, 
    0x9, 0x15, 0x2, 0x2, 0x4fa, 0x4f9, 0x3, 0x2, 0x2, 0x2, 0x4fa, 0x4fb, 
    0x3, 0x2, 0x2, 0x2, 0x4fb, 0x4fc, 0x3, 0x2, 0x2, 0x2, 0x4fc, 0x4fd, 
    0x7, 0xa, 0x2, 0x2, 0x4fd, 0x4ff, 0x3, 0x2, 0x2, 0x2, 0x4fe, 0x4f7, 
    0x3, 0x2, 0x2, 0x2, 0x4fe, 0x4ff, 0x3, 0x2, 0x2, 0x2, 0x4ff, 0x502, 
    0x3, 0x2, 0x2, 0x2, 0x500, 0x502, 0x7, 0x29, 0x2, 0x2, 0x501, 0x4c2, 
    0x3, 0x2, 0x2, 0x2, 0x501, 0x4c3, 0x3, 0x2, 0x2, 0x2, 0x501, 0x4cd, 
    0x3, 0x2, 0x2, 0x2, 0x501, 0x4de, 0x3, 0x2, 0x2, 0x2, 0x501, 0x4e8, 
    0x3, 0x2, 0x2, 0x2, 0x501, 0x4e9, 0x3, 0x2, 0x2, 0x2, 0x501, 0x4ea, 
    0x3, 0x2, 0x2, 0x2, 0x501, 0x4eb, 0x3, 0x2, 0x2, 0x2, 0x501, 0x4ec, 
    0x3, 0x2, 0x2, 0x2, 0x501, 0x4ed, 0x3, 0x2, 0x2, 0x2, 0x501, 0x4ee, 
    0x3, 0x2, 0x2, 0x2, 0x501, 0x4f6, 0x3, 0x2, 0x2, 0x2, 0x501, 0x500, 
    0x3, 0x2, 0x2, 0x2, 0x502, 0xa9, 0x3, 0x2, 0x2, 0x2, 0x503, 0x506, 0x7, 
    0x7c, 0x2, 0x2, 0x504, 0x507, 0x5, 0xac, 0x57, 0x2, 0x505, 0x507, 0x5, 
    0xae, 0x58, 0x2, 0x506, 0x504, 0x3, 0x2, 0x2, 0x2, 0x506, 0x505, 0x3, 
    0x2, 0x2, 0x2, 0x507, 0xab, 0x3, 0x2, 0x2, 0x2, 0x508, 0x50d, 0x7, 0xf7, 
    0x2, 0x2, 0x509, 0x50a, 0x7, 0x9, 0x2, 0x2, 0x50a, 0x50b, 0x5, 0x10a, 
    0x86, 0x2, 0x50b, 0x50c, 0x7, 0xa, 0x2, 0x2, 0x50c, 0x50e, 0x3, 0x2, 
    0x2, 0x2, 0x50d, 0x509, 0x3, 0x2, 0x2, 0x2, 0x50d, 0x50e, 0x3, 0x2, 
    0x2, 0x2, 0x50e, 0x520, 0x3, 0x2, 0x2, 0x2, 0x50f, 0x514, 0x7, 0xf7, 
    0x2, 0x2, 0x510, 0x511, 0x7, 0x9, 0x2, 0x2, 0x511, 0x512, 0x5, 0x10a, 
    0x86, 0x2, 0x512, 0x513, 0x7, 0xa, 0x2, 0x2, 0x513, 0x515, 0x3, 0x2, 
    0x2, 0x2, 0x514, 0x510, 0x3, 0x2, 0x2, 0x2, 0x514, 0x515, 0x3, 0x2, 
    0x2, 0x2, 0x515, 0x516, 0x3, 0x2, 0x2, 0x2, 0x516, 0x517, 0x7, 0xdb, 
    0x2, 0x2, 0x517, 0x520, 0x7, 0x97, 0x2, 0x2, 0x518, 0x51d, 0x7, 0x97, 
    0x2, 0x2, 0x519, 0x51a, 0x7, 0x9, 0x2, 0x2, 0x51a, 0x51b, 0x5, 0x10a, 
    0x86, 0x2, 0x51b, 0x51c, 0x7, 0xa, 0x2, 0x2, 0x51c, 0x51e, 0x3, 0x2, 
    0x2, 0x2, 0x51d, 0x519, 0x3, 0x2, 0x2, 0x2, 0x51d, 0x51e, 0x3, 0x2, 
    0x2, 0x2, 0x51e, 0x520, 0x3, 0x2, 0x2, 0x2, 0x51f, 0x508, 0x3, 0x2, 
    0x2, 0x2, 0x51f, 0x50f, 0x3, 0x2, 0x2, 0x2, 0x51f, 0x518, 0x3, 0x2, 
    0x2, 0x2, 0x520, 0xad, 0x3, 0x2, 0x2, 0x2, 0x521, 0x526, 0x7, 0x49, 
    0x2, 0x2, 0x522, 0x523, 0x7, 0x9, 0x2, 0x2, 0x523, 0x524, 0x5, 0x10a, 
    0x86, 0x2, 0x524, 0x525, 0x7, 0xa, 0x2, 0x2, 0x525, 0x527, 0x3, 0x2, 
    0x2, 0x2, 0x526, 0x522, 0x3, 0x2, 0x2, 0x2, 0x526, 0x527, 0x3, 0x2, 
    0x2, 0x2, 0x527, 0x576, 0x3, 0x2, 0x2, 0x2, 0x528, 0x52d, 0x7, 0x49, 
    0x2, 0x2, 0x529, 0x52a, 0x7, 0x9, 0x2, 0x2, 0x52a, 0x52b, 0x5, 0x10a, 
    0x86, 0x2, 0x52b, 0x52c, 0x7, 0xa, 0x2, 0x2, 0x52c, 0x52e, 0x3, 0x2, 
    0x2, 0x2, 0x52d, 0x529, 0x3, 0x2, 0x2, 0x2, 0x52d, 0x52e, 0x3, 0x2, 
    0x2, 0x2, 0x52e, 0x52f, 0x3, 0x2, 0x2, 0x2, 0x52f, 0x539, 0x7, 0xdb, 
    0x2, 0x2, 0x530, 0x53a, 0x7, 0x70, 0x2, 0x2, 0x531, 0x53a, 0x7, 0x94, 
    0x2, 0x2, 0x532, 0x537, 0x7, 0xc2, 0x2, 0x2, 0x533, 0x534, 0x7, 0x9, 
    0x2, 0x2, 0x534, 0x535, 0x5, 0x10c, 0x87, 0x2, 0x535, 0x536, 0x7, 0xa, 
    0x2, 0x2, 0x536, 0x538, 0x3, 0x2, 0x2, 0x2, 0x537, 0x533, 0x3, 0x2, 
    0x2, 0x2, 0x537, 0x538, 0x3, 0x2, 0x2, 0x2, 0x538, 0x53a, 0x3, 0x2, 
    0x2, 0x2, 0x539, 0x530, 0x3, 0x2, 0x2, 0x2, 0x539, 0x531, 0x3, 0x2, 
    0x2, 0x2, 0x539, 0x532, 0x3, 0x2, 0x2, 0x2, 0x53a, 0x576, 0x3, 0x2, 
    0x2, 0x2, 0x53b, 0x540, 0x7, 0x70, 0x2, 0x2, 0x53c, 0x53d, 0x7, 0x9, 
    0x2, 0x2, 0x53d, 0x53e, 0x5, 0x10a, 0x86, 0x2, 0x53e, 0x53f, 0x7, 0xa, 
    0x2, 0x2, 0x53f, 0x541, 0x3, 0x2, 0x2, 0x2, 0x540, 0x53c, 0x3, 0x2, 
    0x2, 0x2, 0x540, 0x541, 0x3, 0x2, 0x2, 0x2, 0x541, 0x542, 0x3, 0x2, 
    0x2, 0x2, 0x542, 0x54b, 0x7, 0xdb, 0x2, 0x2, 0x543, 0x54c, 0x7, 0x94, 
    0x2, 0x2, 0x544, 0x549, 0x7, 0xc2, 0x2, 0x2, 0x545, 0x546, 0x7, 0x9, 
    0x2, 0x2, 0x546, 0x547, 0x5, 0x10c, 0x87, 0x2, 0x547, 0x548, 0x7, 0xa, 
    0x2, 0x2, 0x548, 0x54a, 0x3, 0x2, 0x2, 0x2, 0x549, 0x545, 0x3, 0x2, 
    0x2, 0x2, 0x549, 0x54a, 0x3, 0x2, 0x2, 0x2, 0x54a, 0x54c, 0x3, 0x2, 
    0x2, 0x2, 0x54b, 0x543, 0x3, 0x2, 0x2, 0x2, 0x54b, 0x544, 0x3, 0x2, 
    0x2, 0x2, 0x54c, 0x576, 0x3, 0x2, 0x2, 0x2, 0x54d, 0x552, 0x7, 0x94, 
    0x2, 0x2, 0x54e, 0x54f, 0x7, 0x9, 0x2, 0x2, 0x54f, 0x550, 0x5, 0x10a, 
    0x86, 0x2, 0x550, 0x551, 0x7, 0xa, 0x2, 0x2, 0x551, 0x553, 0x3, 0x2, 
    0x2, 0x2, 0x552, 0x54e, 0x3, 0x2, 0x2, 0x2, 0x552, 0x553, 0x3, 0x2, 
    0x2, 0x2, 0x553, 0x554, 0x3, 0x2, 0x2, 0x2, 0x554, 0x555, 0x7, 0xdb, 
    0x2, 0x2, 0x555, 0x55a, 0x7, 0xc2, 0x2, 0x2, 0x556, 0x557, 0x7, 0x9, 
    0x2, 0x2, 0x557, 0x558, 0x5, 0x10c, 0x87, 0x2, 0x558, 0x559, 0x7, 0xa, 
    0x2, 0x2, 0x559, 0x55b, 0x3, 0x2, 0x2, 0x2, 0x55a, 0x556, 0x3, 0x2, 
    0x2, 0x2, 0x55a, 0x55b, 0x3, 0x2, 0x2, 0x2, 0x55b, 0x576, 0x3, 0x2, 
    0x2, 0x2, 0x55c, 0x561, 0x7, 0x70, 0x2, 0x2, 0x55d, 0x55e, 0x7, 0x9, 
    0x2, 0x2, 0x55e, 0x55f, 0x5, 0x10a, 0x86, 0x2, 0x55f, 0x560, 0x7, 0xa, 
    0x2, 0x2, 0x560, 0x562, 0x3, 0x2, 0x2, 0x2, 0x561, 0x55d, 0x3, 0x2, 
    0x2, 0x2, 0x561, 0x562, 0x3, 0x2, 0x2, 0x2, 0x562, 0x576, 0x3, 0x2, 
    0x2, 0x2, 0x563, 0x568, 0x7, 0x94, 0x2, 0x2, 0x564, 0x565, 0x7, 0x9, 
    0x2, 0x2, 0x565, 0x566, 0x5, 0x10a, 0x86, 0x2, 0x566, 0x567, 0x7, 0xa, 
    0x2, 0x2, 0x567, 0x569, 0x3, 0x2, 0x2, 0x2, 0x568, 0x564, 0x3, 0x2, 
    0x2, 0x2, 0x568, 0x569, 0x3, 0x2, 0x2, 0x2, 0x569, 0x576, 0x3, 0x2, 
    0x2, 0x2, 0x56a, 0x573, 0x7, 0xc2, 0x2, 0x2, 0x56b, 0x56c, 0x7, 0x9, 
    0x2, 0x2, 0x56c, 0x56f, 0x5, 0x10a, 0x86, 0x2, 0x56d, 0x56e, 0x7, 0x8, 
    0x2, 0x2, 0x56e, 0x570, 0x5, 0x10c, 0x87, 0x2, 0x56f, 0x56d, 0x3, 0x2, 
    0x2, 0x2, 0x56f, 0x570, 0x3, 0x2, 0x2, 0x2, 0x570, 0x571, 0x3, 0x2, 
    0x2, 0x2, 0x571, 0x572, 0x7, 0xa, 0x2, 0x2, 0x572, 0x574, 0x3, 0x2, 
    0x2, 0x2, 0x573, 0x56b, 0x3, 0x2, 0x2, 0x2, 0x573, 0x574, 0x3, 0x2, 
    0x2, 0x2, 0x574, 0x576, 0x3, 0x2, 0x2, 0x2, 0x575, 0x521, 0x3, 0x2, 
    0x2, 0x2, 0x575, 0x528, 0x3, 0x2, 0x2, 0x2, 0x575, 0x53b, 0x3, 0x2, 
    0x2, 0x2, 0x575, 0x54d, 0x3, 0x2, 0x2, 0x2, 0x575, 0x55c, 0x3, 0x2, 
    0x2, 0x2, 0x575, 0x563, 0x3, 0x2, 0x2, 0x2, 0x575, 0x56a, 0x3, 0x2, 
    0x2, 0x2, 0x576, 0xaf, 0x3, 0x2, 0x2, 0x2, 0x577, 0x578, 0x9, 0x16, 
    0x2, 0x2, 0x578, 0x579, 0x7, 0x9, 0x2, 0x2, 0x579, 0x57b, 0x5, 0x10e, 
    0x88, 0x2, 0x57a, 0x57c, 0x9, 0x17, 0x2, 0x2, 0x57b, 0x57a, 0x3, 0x2, 
    0x2, 0x2, 0x57b, 0x57c, 0x3, 0x2, 0x2, 0x2, 0x57c, 0x57d, 0x3, 0x2, 
    0x2, 0x2, 0x57d, 0x580, 0x7, 0xa, 0x2, 0x2, 0x57e, 0x57f, 0x7, 0x3b, 
    0x2, 0x2, 0x57f, 0x581, 0x5, 0xf8, 0x7d, 0x2, 0x580, 0x57e, 0x3, 0x2, 
    0x2, 0x2, 0x580, 0x581, 0x3, 0x2, 0x2, 0x2, 0x581, 0xb1, 0x3, 0x2, 0x2, 
    0x2, 0x582, 0x584, 0x9, 0x16, 0x2, 0x2, 0x583, 0x585, 0x7, 0xee, 0x2, 
    0x2, 0x584, 0x583, 0x3, 0x2, 0x2, 0x2, 0x584, 0x585, 0x3, 0x2, 0x2, 
    0x2, 0x585, 0x588, 0x3, 0x2, 0x2, 0x2, 0x586, 0x588, 0x7, 0xed, 0x2, 
    0x2, 0x587, 0x582, 0x3, 0x2, 0x2, 0x2, 0x587, 0x586, 0x3, 0x2, 0x2, 
    0x2, 0x588, 0x590, 0x3, 0x2, 0x2, 0x2, 0x589, 0x58a, 0x7, 0x9, 0x2, 
    0x2, 0x58a, 0x58c, 0x5, 0x10e, 0x88, 0x2, 0x58b, 0x58d, 0x9, 0x17, 0x2, 
    0x2, 0x58c, 0x58b, 0x3, 0x2, 0x2, 0x2, 0x58c, 0x58d, 0x3, 0x2, 0x2, 
    0x2, 0x58d, 0x58e, 0x3, 0x2, 0x2, 0x2, 0x58e, 0x58f, 0x7, 0xa, 0x2, 
    0x2, 0x58f, 0x591, 0x3, 0x2, 0x2, 0x2, 0x590, 0x589, 0x3, 0x2, 0x2, 
    0x2, 0x590, 0x591, 0x3, 0x2, 0x2, 0x2, 0x591, 0x594, 0x3, 0x2, 0x2, 
    0x2, 0x592, 0x594, 0x7, 0xd7, 0x2, 0x2, 0x593, 0x587, 0x3, 0x2, 0x2, 
    0x2, 0x593, 0x592, 0x3, 0x2, 0x2, 0x2, 0x594, 0x597, 0x3, 0x2, 0x2, 
    0x2, 0x595, 0x596, 0x7, 0x3b, 0x2, 0x2, 0x596, 0x598, 0x5, 0xf8, 0x7d, 
    0x2, 0x597, 0x595, 0x3, 0x2, 0x2, 0x2, 0x597, 0x598, 0x3, 0x2, 0x2, 
    0x2, 0x598, 0xb3, 0x3, 0x2, 0x2, 0x2, 0x599, 0x59a, 0x5, 0xb6, 0x5c, 
    0x2, 0x59a, 0xb5, 0x3, 0x2, 0x2, 0x2, 0x59b, 0x59f, 0x7, 0x9, 0x2, 0x2, 
    0x59c, 0x59e, 0x5, 0x14, 0xb, 0x2, 0x59d, 0x59c, 0x3, 0x2, 0x2, 0x2, 
    0x59e, 0x5a1, 0x3, 0x2, 0x2, 0x2, 0x59f, 0x59d, 0x3, 0x2, 0x2, 0x2, 
    0x59f, 0x5a0, 0x3, 0x2, 0x2, 0x2, 0x5a0, 0x5a2, 0x3, 0x2, 0x2, 0x2, 
    0x5a1, 0x59f, 0x3, 0x2, 0x2, 0x2, 0x5a2, 0x5a3, 0x5, 0xb6, 0x5c, 0x2, 
    0x5a3, 0x5a4, 0x7, 0xa, 0x2, 0x2, 0x5a4, 0x5b1, 0x3, 0x2, 0x2, 0x2, 
    0x5a5, 0x5a7, 0x5, 0x14, 0xb, 0x2, 0x5a6, 0x5a5, 0x3, 0x2, 0x2, 0x2, 
    0x5a7, 0x5aa, 0x3, 0x2, 0x2, 0x2, 0x5a8, 0x5a6, 0x3, 0x2, 0x2, 0x2, 
    0x5a8, 0x5a9, 0x3, 0x2, 0x2, 0x2, 0x5a9, 0x5ae, 0x3, 0x2, 0x2, 0x2, 
    0x5aa, 0x5a8, 0x3, 0x2, 0x2, 0x2, 0x5ab, 0x5af, 0x5, 0x18, 0xd, 0x2, 
    0x5ac, 0x5af, 0x5, 0x2c, 0x17, 0x2, 0x5ad, 0x5af, 0x5, 0x1c, 0xf, 0x2, 
    0x5ae, 0x5ab, 0x3, 0x2, 0x2, 0x2, 0x5ae, 0x5ac, 0x3, 0x2, 0x2, 0x2, 
    0x5ae, 0x5ad, 0x3, 0x2, 0x2, 0x2, 0x5af, 0x5b1, 0x3, 0x2, 0x2, 0x2, 
    0x5b0, 0x59b, 0x3, 0x2, 0x2, 0x2, 0x5b0, 0x5a8, 0x3, 0x2, 0x2, 0x2, 
    0x5b1, 0xb7, 0x3, 0x2, 0x2, 0x2, 0x5b2, 0x5b7, 0x5, 0xba, 0x5e, 0x2, 
    0x5b3, 0x5b4, 0x7, 0x8, 0x2, 0x2, 0x5b4, 0x5b6, 0x5, 0xba, 0x5e, 0x2, 
    0x5b5, 0x5b3, 0x3, 0x2, 0x2, 0x2, 0x5b6, 0x5b9, 0x3, 0x2, 0x2, 0x2, 
    0x5b7, 0x5b5, 0x3, 0x2, 0x2, 0x2, 0x5b7, 0x5b8, 0x3, 0x2, 0x2, 0x2, 
    0x5b8, 0xb9, 0x3, 0x2, 0x2, 0x2, 0x5b9, 0x5b7, 0x3, 0x2, 0x2, 0x2, 0x5ba, 
    0x5bf, 0x5, 0xde, 0x70, 0x2, 0x5bb, 0x5bd, 0x7, 0x21, 0x2, 0x2, 0x5bc, 
    0x5bb, 0x3, 0x2, 0x2, 0x2, 0x5bc, 0x5bd, 0x3, 0x2, 0x2, 0x2, 0x5bd, 
    0x5be, 0x3, 0x2, 0x2, 0x2, 0x5be, 0x5c0, 0x5, 0x106, 0x84, 0x2, 0x5bf, 
    0x5bc, 0x3, 0x2, 0x2, 0x2, 0x5bf, 0x5c0, 0x3, 0x2, 0x2, 0x2, 0x5c0, 
    0x5c3, 0x3, 0x2, 0x2, 0x2, 0x5c1, 0x5c3, 0x5, 0xe4, 0x73, 0x2, 0x5c2, 
    0x5ba, 0x3, 0x2, 0x2, 0x2, 0x5c2, 0x5c1, 0x3, 0x2, 0x2, 0x2, 0x5c3, 
    0xbb, 0x3, 0x2, 0x2, 0x2, 0x5c4, 0x5c9, 0x5, 0xbe, 0x60, 0x2, 0x5c5, 
    0x5c6, 0x7, 0x8, 0x2, 0x2, 0x5c6, 0x5c8, 0x5, 0xbe, 0x60, 0x2, 0x5c7, 
    0x5c5, 0x3, 0x2, 0x2, 0x2, 0x5c8, 0x5cb, 0x3, 0x2, 0x2, 0x2, 0x5c9, 
    0x5c7, 0x3, 0x2, 0x2, 0x2, 0x5c9, 0x5ca, 0x3, 0x2, 0x2, 0x2, 0x5ca, 
    0xbd, 0x3, 0x2, 0x2, 0x2, 0x5cb, 0x5c9, 0x3, 0x2, 0x2, 0x2, 0x5cc, 0x5cd, 
    0x5, 0xde, 0x70, 0x2, 0x5cd, 0xbf, 0x3, 0x2, 0x2, 0x2, 0x5ce, 0x5d3, 
    0x5, 0xc2, 0x62, 0x2, 0x5cf, 0x5d0, 0x7, 0x8, 0x2, 0x2, 0x5d0, 0x5d2, 
    0x5, 0xc2, 0x62, 0x2, 0x5d1, 0x5cf, 0x3, 0x2, 0x2, 0x2, 0x5d2, 0x5d5, 
    0x3, 0x2, 0x2, 0x2, 0x5d3, 0x5d1, 0x3, 0x2, 0x2, 0x2, 0x5d3, 0x5d4, 
    0x3, 0x2, 0x2, 0x2, 0x5d4, 0xc1, 0x3, 0x2, 0x2, 0x2, 0x5d5, 0x5d3, 0x3, 
    0x2, 0x2, 0x2, 0x5d6, 0x5d8, 0x5, 0xde, 0x70, 0x2, 0x5d7, 0x5d9, 0x9, 
    0x18, 0x2, 0x2, 0x5d8, 0x5d7, 0x3, 0x2, 0x2, 0x2, 0x5d8, 0x5d9, 0x3, 
    0x2, 0x2, 0x2, 0x5d9, 0xc3, 0x3, 0x2, 0x2, 0x2, 0x5da, 0x5df, 0x5, 0xc6, 
    0x64, 0x2, 0x5db, 0x5dc, 0x7, 0x8, 0x2, 0x2, 0x5dc, 0x5de, 0x5, 0xc6, 
    0x64, 0x2, 0x5dd, 0x5db, 0x3, 0x2, 0x2, 0x2, 0x5de, 0x5e1, 0x3, 0x2, 
    0x2, 0x2, 0x5df, 0x5dd, 0x3, 0x2, 0x2, 0x2, 0x5df, 0x5e0, 0x3, 0x2, 
    0x2, 0x2, 0x5e0, 0xc5, 0x3, 0x2, 0x2, 0x2, 0x5e1, 0x5df, 0x3, 0x2, 0x2, 
    0x2, 0x5e2, 0x5e3, 0x5, 0x106, 0x84, 0x2, 0x5e3, 0xc7, 0x3, 0x2, 0x2, 
    0x2, 0x5e4, 0x5e9, 0x5, 0xca, 0x66, 0x2, 0x5e5, 0x5e6, 0x7, 0x8, 0x2, 
    0x2, 0x5e6, 0x5e8, 0x5, 0xca, 0x66, 0x2, 0x5e7, 0x5e5, 0x3, 0x2, 0x2, 
    0x2, 0x5e8, 0x5eb, 0x3, 0x2, 0x2, 0x2, 0x5e9, 0x5e7, 0x3, 0x2, 0x2, 
    0x2, 0x5e9, 0x5ea, 0x3, 0x2, 0x2, 0x2, 0x5ea, 0xc9, 0x3, 0x2, 0x2, 0x2, 
    0x5eb, 0x5e9, 0x3, 0x2, 0x2, 0x2, 0x5ec, 0x5ed, 0x5, 0x106, 0x84, 0x2, 
    0x5ed, 0xcb, 0x3, 0x2, 0x2, 0x2, 0x5ee, 0x5ef, 0x7, 0x9, 0x2, 0x2, 0x5ef, 
    0x5f0, 0x5, 0xce, 0x68, 0x2, 0x5f0, 0x5f1, 0x7, 0xa, 0x2, 0x2, 0x5f1, 
    0xcd, 0x3, 0x2, 0x2, 0x2, 0x5f2, 0x5f7, 0x5, 0xd0, 0x69, 0x2, 0x5f3, 
    0x5f4, 0x7, 0x8, 0x2, 0x2, 0x5f4, 0x5f6, 0x5, 0xd0, 0x69, 0x2, 0x5f5, 
    0x5f3, 0x3, 0x2, 0x2, 0x2, 0x5f6, 0x5f9, 0x3, 0x2, 0x2, 0x2, 0x5f7, 
    0x5f5, 0x3, 0x2, 0x2, 0x2, 0x5f7, 0x5f8, 0x3, 0x2, 0x2, 0x2, 0x5f8, 
    0xcf, 0x3, 0x2, 0x2, 0x2, 0x5f9, 0x5f7, 0x3, 0x2, 0x2, 0x2, 0x5fa, 0x5fb, 
    0x5, 0x106, 0x84, 0x2, 0x5fb, 0xd1, 0x3, 0x2, 0x2, 0x2, 0x5fc, 0x601, 
    0x5, 0xd4, 0x6b, 0x2, 0x5fd, 0x5fe, 0x7, 0x8, 0x2, 0x2, 0x5fe, 0x600, 
    0x5, 0xd4, 0x6b, 0x2, 0x5ff, 0x5fd, 0x3, 0x2, 0x2, 0x2, 0x600, 0x603, 
    0x3, 0x2, 0x2, 0x2, 0x601, 0x5ff, 0x3, 0x2, 0x2, 0x2, 0x601, 0x602, 
    0x3, 0x2, 0x2, 0x2, 0x602, 0xd3, 0x3, 0x2, 0x2, 0x2, 0x603, 0x601, 0x3, 
    0x2, 0x2, 0x2, 0x604, 0x607, 0x5, 0xfc, 0x7f, 0x2, 0x605, 0x606, 0x7, 
    0x6, 0x2, 0x2, 0x606, 0x608, 0x5, 0x102, 0x82, 0x2, 0x607, 0x605, 0x3, 
    0x2, 0x2, 0x2, 0x607, 0x608, 0x3, 0x2, 0x2, 0x2, 0x608, 0xd5, 0x3, 0x2, 
    0x2, 0x2, 0x609, 0x60e, 0x5, 0xde, 0x70, 0x2, 0x60a, 0x60b, 0x7, 0x8, 
    0x2, 0x2, 0x60b, 0x60d, 0x5, 0xde, 0x70, 0x2, 0x60c, 0x60a, 0x3, 0x2, 
    0x2, 0x2, 0x60d, 0x610, 0x3, 0x2, 0x2, 0x2, 0x60e, 0x60c, 0x3, 0x2, 
    0x2, 0x2, 0x60e, 0x60f, 0x3, 0x2, 0x2, 0x2, 0x60f, 0xd7, 0x3, 0x2, 0x2, 
    0x2, 0x610, 0x60e, 0x3, 0x2, 0x2, 0x2, 0x611, 0x612, 0x5, 0xde, 0x70, 
    0x2, 0x612, 0xd9, 0x3, 0x2, 0x2, 0x2, 0x613, 0x614, 0x5, 0xde, 0x70, 
    0x2, 0x614, 0xdb, 0x3, 0x2, 0x2, 0x2, 0x615, 0x616, 0x5, 0xde, 0x70, 
    0x2, 0x616, 0xdd, 0x3, 0x2, 0x2, 0x2, 0x617, 0x618, 0x8, 0x70, 0x1, 
    0x2, 0x618, 0x71f, 0x5, 0x110, 0x89, 0x2, 0x619, 0x71f, 0x5, 0xe0, 0x71, 
    0x2, 0x61a, 0x71f, 0x5, 0xe2, 0x72, 0x2, 0x61b, 0x61c, 0x7, 0x9, 0x2, 
    0x2, 0x61c, 0x61d, 0x5, 0xa6, 0x54, 0x2, 0x61d, 0x61e, 0x7, 0xa, 0x2, 
    0x2, 0x61e, 0x61f, 0x5, 0xde, 0x70, 0x2a, 0x61f, 0x71f, 0x3, 0x2, 0x2, 
    0x2, 0x620, 0x621, 0x9, 0x19, 0x2, 0x2, 0x621, 0x71f, 0x5, 0xde, 0x70, 
    0x29, 0x622, 0x623, 0x7, 0x35, 0x2, 0x2, 0x623, 0x624, 0x7, 0x9, 0x2, 
    0x2, 0x624, 0x625, 0x5, 0xde, 0x70, 0x2, 0x625, 0x626, 0x9, 0x1a, 0x2, 
    0x2, 0x626, 0x627, 0x5, 0xa6, 0x54, 0x2, 0x627, 0x628, 0x7, 0xa, 0x2, 
    0x2, 0x628, 0x71f, 0x3, 0x2, 0x2, 0x2, 0x629, 0x62a, 0x7, 0xe4, 0x2, 
    0x2, 0x62a, 0x62b, 0x7, 0x9, 0x2, 0x2, 0x62b, 0x62c, 0x5, 0xde, 0x70, 
    0x2, 0x62c, 0x62d, 0x7, 0xa, 0x2, 0x2, 0x62d, 0x71f, 0x3, 0x2, 0x2, 
    0x2, 0x62e, 0x642, 0x7, 0x34, 0x2, 0x2, 0x62f, 0x635, 0x5, 0xde, 0x70, 
    0x2, 0x630, 0x631, 0x7, 0xf1, 0x2, 0x2, 0x631, 0x632, 0x5, 0xde, 0x70, 
    0x2, 0x632, 0x633, 0x7, 0xd8, 0x2, 0x2, 0x633, 0x634, 0x5, 0xde, 0x70, 
    0x2, 0x634, 0x636, 0x3, 0x2, 0x2, 0x2, 0x635, 0x630, 0x3, 0x2, 0x2, 
    0x2, 0x636, 0x637, 0x3, 0x2, 0x2, 0x2, 0x637, 0x635, 0x3, 0x2, 0x2, 
    0x2, 0x637, 0x638, 0x3, 0x2, 0x2, 0x2, 0x638, 0x643, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x63a, 0x7, 0xf1, 0x2, 0x2, 0x63a, 0x63b, 0x5, 0xdc, 0x6f, 
    0x2, 0x63b, 0x63c, 0x7, 0xd8, 0x2, 0x2, 0x63c, 0x63d, 0x5, 0xde, 0x70, 
    0x2, 0x63d, 0x63f, 0x3, 0x2, 0x2, 0x2, 0x63e, 0x639, 0x3, 0x2, 0x2, 
    0x2, 0x63f, 0x640, 0x3, 0x2, 0x2, 0x2, 0x640, 0x63e, 0x3, 0x2, 0x2, 
    0x2, 0x640, 0x641, 0x3, 0x2, 0x2, 0x2, 0x641, 0x643, 0x3, 0x2, 0x2, 
    0x2, 0x642, 0x62f, 0x3, 0x2, 0x2, 0x2, 0x642, 0x63e, 0x3, 0x2, 0x2, 
    0x2, 0x643, 0x646, 0x3, 0x2, 0x2, 0x2, 0x644, 0x645, 0x7, 0x57, 0x2, 
    0x2, 0x645, 0x647, 0x5, 0xde, 0x70, 0x2, 0x646, 0x644, 0x3, 0x2, 0x2, 
    0x2, 0x646, 0x647, 0x3, 0x2, 0x2, 0x2, 0x647, 0x648, 0x3, 0x2, 0x2, 
    0x2, 0x648, 0x649, 0x7, 0x58, 0x2, 0x2, 0x649, 0x71f, 0x3, 0x2, 0x2, 
    0x2, 0x64a, 0x64b, 0x7, 0x72, 0x2, 0x2, 0x64b, 0x64c, 0x7, 0x9, 0x2, 
    0x2, 0x64c, 0x64d, 0x5, 0xdc, 0x6f, 0x2, 0x64d, 0x64e, 0x7, 0x8, 0x2, 
    0x2, 0x64e, 0x64f, 0x5, 0xde, 0x70, 0x2, 0x64f, 0x650, 0x7, 0x8, 0x2, 
    0x2, 0x650, 0x651, 0x5, 0xde, 0x70, 0x2, 0x651, 0x652, 0x7, 0xa, 0x2, 
    0x2, 0x652, 0x71f, 0x3, 0x2, 0x2, 0x2, 0x653, 0x654, 0x7, 0x73, 0x2, 
    0x2, 0x654, 0x655, 0x7, 0x9, 0x2, 0x2, 0x655, 0x656, 0x5, 0xde, 0x70, 
    0x2, 0x656, 0x657, 0x7, 0x8, 0x2, 0x2, 0x657, 0x658, 0x5, 0xde, 0x70, 
    0x2, 0x658, 0x659, 0x7, 0xa, 0x2, 0x2, 0x659, 0x71f, 0x3, 0x2, 0x2, 
    0x2, 0x65a, 0x65b, 0x7, 0xa3, 0x2, 0x2, 0x65b, 0x65c, 0x7, 0x9, 0x2, 
    0x2, 0x65c, 0x65d, 0x5, 0xde, 0x70, 0x2, 0x65d, 0x65e, 0x7, 0x8, 0x2, 
    0x2, 0x65e, 0x65f, 0x5, 0xde, 0x70, 0x2, 0x65f, 0x660, 0x7, 0xa, 0x2, 
    0x2, 0x660, 0x71f, 0x3, 0x2, 0x2, 0x2, 0x661, 0x662, 0x7, 0x3a, 0x2, 
    0x2, 0x662, 0x66b, 0x7, 0x9, 0x2, 0x2, 0x663, 0x668, 0x5, 0xde, 0x70, 
    0x2, 0x664, 0x665, 0x7, 0x8, 0x2, 0x2, 0x665, 0x667, 0x5, 0xde, 0x70, 
    0x2, 0x666, 0x664, 0x3, 0x2, 0x2, 0x2, 0x667, 0x66a, 0x3, 0x2, 0x2, 
    0x2, 0x668, 0x666, 0x3, 0x2, 0x2, 0x2, 0x668, 0x669, 0x3, 0x2, 0x2, 
    0x2, 0x669, 0x66c, 0x3, 0x2, 0x2, 0x2, 0x66a, 0x668, 0x3, 0x2, 0x2, 
    0x2, 0x66b, 0x663, 0x3, 0x2, 0x2, 0x2, 0x66b, 0x66c, 0x3, 0x2, 0x2, 
    0x2, 0x66c, 0x66d, 0x3, 0x2, 0x2, 0x2, 0x66d, 0x71f, 0x7, 0xa, 0x2, 
    0x2, 0x66e, 0x66f, 0x7, 0x4c, 0x2, 0x2, 0x66f, 0x678, 0x7, 0x9, 0x2, 
    0x2, 0x670, 0x675, 0x5, 0xde, 0x70, 0x2, 0x671, 0x672, 0x7, 0x8, 0x2, 
    0x2, 0x672, 0x674, 0x5, 0xde, 0x70, 0x2, 0x673, 0x671, 0x3, 0x2, 0x2, 
    0x2, 0x674, 0x677, 0x3, 0x2, 0x2, 0x2, 0x675, 0x673, 0x3, 0x2, 0x2, 
    0x2, 0x675, 0x676, 0x3, 0x2, 0x2, 0x2, 0x676, 0x679, 0x3, 0x2, 0x2, 
    0x2, 0x677, 0x675, 0x3, 0x2, 0x2, 0x2, 0x678, 0x670, 0x3, 0x2, 0x2, 
    0x2, 0x678, 0x679, 0x3, 0x2, 0x2, 0x2, 0x679, 0x67a, 0x3, 0x2, 0x2, 
    0x2, 0x67a, 0x71f, 0x7, 0xa, 0x2, 0x2, 0x67b, 0x67c, 0x7, 0x3d, 0x2, 
    0x2, 0x67c, 0x685, 0x7, 0x9, 0x2, 0x2, 0x67d, 0x682, 0x5, 0xde, 0x70, 
    0x2, 0x67e, 0x67f, 0x7, 0x8, 0x2, 0x2, 0x67f, 0x681, 0x5, 0xde, 0x70, 
    0x2, 0x680, 0x67e, 0x3, 0x2, 0x2, 0x2, 0x681, 0x684, 0x3, 0x2, 0x2, 
    0x2, 0x682, 0x680, 0x3, 0x2, 0x2, 0x2, 0x682, 0x683, 0x3, 0x2, 0x2, 
    0x2, 0x683, 0x686, 0x3, 0x2, 0x2, 0x2, 0x684, 0x682, 0x3, 0x2, 0x2, 
    0x2, 0x685, 0x67d, 0x3, 0x2, 0x2, 0x2, 0x685, 0x686, 0x3, 0x2, 0x2, 
    0x2, 0x686, 0x687, 0x3, 0x2, 0x2, 0x2, 0x687, 0x71f, 0x7, 0xa, 0x2, 
    0x2, 0x688, 0x689, 0x7, 0xad, 0x2, 0x2, 0x689, 0x68a, 0x7, 0x9, 0x2, 
    0x2, 0x68a, 0x68b, 0x5, 0xde, 0x70, 0x2, 0x68b, 0x68c, 0x9, 0x1b, 0x2, 
    0x2, 0x68c, 0x68d, 0x5, 0xde, 0x70, 0x2, 0x68d, 0x68e, 0x9, 0x1c, 0x2, 
    0x2, 0x68e, 0x691, 0x5, 0xde, 0x70, 0x2, 0x68f, 0x690, 0x9, 0x1d, 0x2, 
    0x2, 0x690, 0x692, 0x5, 0xde, 0x70, 0x2, 0x691, 0x68f, 0x3, 0x2, 0x2, 
    0x2, 0x691, 0x692, 0x3, 0x2, 0x2, 0x2, 0x692, 0x693, 0x3, 0x2, 0x2, 
    0x2, 0x693, 0x694, 0x7, 0xa, 0x2, 0x2, 0x694, 0x71f, 0x3, 0x2, 0x2, 
    0x2, 0x695, 0x696, 0x7, 0xb3, 0x2, 0x2, 0x696, 0x697, 0x7, 0x9, 0x2, 
    0x2, 0x697, 0x698, 0x5, 0xde, 0x70, 0x2, 0x698, 0x699, 0x9, 0x1e, 0x2, 
    0x2, 0x699, 0x69a, 0x5, 0xde, 0x70, 0x2, 0x69a, 0x69b, 0x7, 0xa, 0x2, 
    0x2, 0x69b, 0x71f, 0x3, 0x2, 0x2, 0x2, 0x69c, 0x69d, 0x9, 0x1f, 0x2, 
    0x2, 0x69d, 0x6ac, 0x7, 0x9, 0x2, 0x2, 0x69e, 0x69f, 0x5, 0xde, 0x70, 
    0x2, 0x69f, 0x6a0, 0x7, 0x8, 0x2, 0x2, 0x6a0, 0x6a3, 0x5, 0xde, 0x70, 
    0x2, 0x6a1, 0x6a2, 0x7, 0x8, 0x2, 0x2, 0x6a2, 0x6a4, 0x5, 0xde, 0x70, 
    0x2, 0x6a3, 0x6a1, 0x3, 0x2, 0x2, 0x2, 0x6a3, 0x6a4, 0x3, 0x2, 0x2, 
    0x2, 0x6a4, 0x6ad, 0x3, 0x2, 0x2, 0x2, 0x6a5, 0x6a6, 0x5, 0xde, 0x70, 
    0x2, 0x6a6, 0x6a7, 0x7, 0x6a, 0x2, 0x2, 0x6a7, 0x6aa, 0x5, 0xde, 0x70, 
    0x2, 0x6a8, 0x6a9, 0x7, 0x66, 0x2, 0x2, 0x6a9, 0x6ab, 0x5, 0xde, 0x70, 
    0x2, 0x6aa, 0x6a8, 0x3, 0x2, 0x2, 0x2, 0x6aa, 0x6ab, 0x3, 0x2, 0x2, 
    0x2, 0x6ab, 0x6ad, 0x3, 0x2, 0x2, 0x2, 0x6ac, 0x69e, 0x3, 0x2, 0x2, 
    0x2, 0x6ac, 0x6a5, 0x3, 0x2, 0x2, 0x2, 0x6ad, 0x6ae, 0x3, 0x2, 0x2, 
    0x2, 0x6ae, 0x6af, 0x7, 0xa, 0x2, 0x2, 0x6af, 0x71f, 0x3, 0x2, 0x2, 
    0x2, 0x6b0, 0x6b1, 0x7, 0xdf, 0x2, 0x2, 0x6b1, 0x6bf, 0x7, 0x9, 0x2, 
    0x2, 0x6b2, 0x6b5, 0x5, 0xde, 0x70, 0x2, 0x6b3, 0x6b4, 0x7, 0x8, 0x2, 
    0x2, 0x6b4, 0x6b6, 0x5, 0xde, 0x70, 0x2, 0x6b5, 0x6b3, 0x3, 0x2, 0x2, 
    0x2, 0x6b5, 0x6b6, 0x3, 0x2, 0x2, 0x2, 0x6b6, 0x6c0, 0x3, 0x2, 0x2, 
    0x2, 0x6b7, 0x6b9, 0x9, 0x20, 0x2, 0x2, 0x6b8, 0x6b7, 0x3, 0x2, 0x2, 
    0x2, 0x6b8, 0x6b9, 0x3, 0x2, 0x2, 0x2, 0x6b9, 0x6bb, 0x3, 0x2, 0x2, 
    0x2, 0x6ba, 0x6bc, 0x5, 0xde, 0x70, 0x2, 0x6bb, 0x6ba, 0x3, 0x2, 0x2, 
    0x2, 0x6bb, 0x6bc, 0x3, 0x2, 0x2, 0x2, 0x6bc, 0x6bd, 0x3, 0x2, 0x2, 
    0x2, 0x6bd, 0x6be, 0x7, 0x6a, 0x2, 0x2, 0x6be, 0x6c0, 0x5, 0xde, 0x70, 
    0x2, 0x6bf, 0x6b2, 0x3, 0x2, 0x2, 0x2, 0x6bf, 0x6b8, 0x3, 0x2, 0x2, 
    0x2, 0x6c0, 0x6c1, 0x3, 0x2, 0x2, 0x2, 0x6c1, 0x6c2, 0x7, 0xa, 0x2, 
    0x2, 0x6c2, 0x71f, 0x3, 0x2, 0x2, 0x2, 0x6c3, 0x6c4, 0x7, 0x60, 0x2, 
    0x2, 0x6c4, 0x6c5, 0x7, 0x9, 0x2, 0x2, 0x6c5, 0x6c6, 0x9, 0x21, 0x2, 
    0x2, 0x6c6, 0x6c7, 0x7, 0x6a, 0x2, 0x2, 0x6c7, 0x6c8, 0x5, 0xde, 0x70, 
    0x2, 0x6c8, 0x6c9, 0x7, 0xa, 0x2, 0x2, 0x6c9, 0x71f, 0x3, 0x2, 0x2, 
    0x2, 0x6ca, 0x6cb, 0x7, 0x3f, 0x2, 0x2, 0x6cb, 0x6cd, 0x7, 0x9, 0x2, 
    0x2, 0x6cc, 0x6ce, 0x7, 0x19, 0x2, 0x2, 0x6cd, 0x6cc, 0x3, 0x2, 0x2, 
    0x2, 0x6cd, 0x6ce, 0x3, 0x2, 0x2, 0x2, 0x6ce, 0x6cf, 0x3, 0x2, 0x2, 
    0x2, 0x6cf, 0x6d0, 0x7, 0xe, 0x2, 0x2, 0x6d0, 0x71f, 0x7, 0xa, 0x2, 
    0x2, 0x6d1, 0x6d2, 0x7, 0x3f, 0x2, 0x2, 0x6d2, 0x6d5, 0x7, 0x9, 0x2, 
    0x2, 0x6d3, 0x6d6, 0x7, 0x19, 0x2, 0x2, 0x6d4, 0x6d6, 0x7, 0x52, 0x2, 
    0x2, 0x6d5, 0x6d3, 0x3, 0x2, 0x2, 0x2, 0x6d5, 0x6d4, 0x3, 0x2, 0x2, 
    0x2, 0x6d5, 0x6d6, 0x3, 0x2, 0x2, 0x2, 0x6d6, 0x6d7, 0x3, 0x2, 0x2, 
    0x2, 0x6d7, 0x6d8, 0x5, 0xde, 0x70, 0x2, 0x6d8, 0x6d9, 0x7, 0xa, 0x2, 
    0x2, 0x6d9, 0x71f, 0x3, 0x2, 0x2, 0x2, 0x6da, 0x6db, 0x7, 0xd0, 0x2, 
    0x2, 0x6db, 0x6de, 0x7, 0x9, 0x2, 0x2, 0x6dc, 0x6df, 0x7, 0x19, 0x2, 
    0x2, 0x6dd, 0x6df, 0x7, 0x52, 0x2, 0x2, 0x6de, 0x6dc, 0x3, 0x2, 0x2, 
    0x2, 0x6de, 0x6dd, 0x3, 0x2, 0x2, 0x2, 0x6de, 0x6df, 0x3, 0x2, 0x2, 
    0x2, 0x6df, 0x6e0, 0x3, 0x2, 0x2, 0x2, 0x6e0, 0x6e1, 0x5, 0xde, 0x70, 
    0x2, 0x6e1, 0x6e2, 0x7, 0xa, 0x2, 0x2, 0x6e2, 0x71f, 0x3, 0x2, 0x2, 
    0x2, 0x6e3, 0x6e4, 0x7, 0x23, 0x2, 0x2, 0x6e4, 0x6e7, 0x7, 0x9, 0x2, 
    0x2, 0x6e5, 0x6e8, 0x7, 0x19, 0x2, 0x2, 0x6e6, 0x6e8, 0x7, 0x52, 0x2, 
    0x2, 0x6e7, 0x6e5, 0x3, 0x2, 0x2, 0x2, 0x6e7, 0x6e6, 0x3, 0x2, 0x2, 
    0x2, 0x6e7, 0x6e8, 0x3, 0x2, 0x2, 0x2, 0x6e8, 0x6e9, 0x3, 0x2, 0x2, 
    0x2, 0x6e9, 0x6ea, 0x5, 0xde, 0x70, 0x2, 0x6ea, 0x6eb, 0x7, 0xa, 0x2, 
    0x2, 0x6eb, 0x71f, 0x3, 0x2, 0x2, 0x2, 0x6ec, 0x6ed, 0x7, 0x93, 0x2, 
    0x2, 0x6ed, 0x6f0, 0x7, 0x9, 0x2, 0x2, 0x6ee, 0x6f1, 0x7, 0x19, 0x2, 
    0x2, 0x6ef, 0x6f1, 0x7, 0x52, 0x2, 0x2, 0x6f0, 0x6ee, 0x3, 0x2, 0x2, 
    0x2, 0x6f0, 0x6ef, 0x3, 0x2, 0x2, 0x2, 0x6f0, 0x6f1, 0x3, 0x2, 0x2, 
    0x2, 0x6f1, 0x6f2, 0x3, 0x2, 0x2, 0x2, 0x6f2, 0x6f3, 0x5, 0xde, 0x70, 
    0x2, 0x6f3, 0x6f4, 0x7, 0xa, 0x2, 0x2, 0x6f4, 0x71f, 0x3, 0x2, 0x2, 
    0x2, 0x6f5, 0x6f6, 0x7, 0x8e, 0x2, 0x2, 0x6f6, 0x6f9, 0x7, 0x9, 0x2, 
    0x2, 0x6f7, 0x6fa, 0x7, 0x19, 0x2, 0x2, 0x6f8, 0x6fa, 0x7, 0x52, 0x2, 
    0x2, 0x6f9, 0x6f7, 0x3, 0x2, 0x2, 0x2, 0x6f9, 0x6f8, 0x3, 0x2, 0x2, 
    0x2, 0x6f9, 0x6fa, 0x3, 0x2, 0x2, 0x2, 0x6fa, 0x6fb, 0x3, 0x2, 0x2, 
    0x2, 0x6fb, 0x6fc, 0x5, 0xde, 0x70, 0x2, 0x6fc, 0x6fd, 0x7, 0xa, 0x2, 
    0x2, 0x6fd, 0x71f, 0x3, 0x2, 0x2, 0x2, 0x6fe, 0x6ff, 0x5, 0xe8, 0x75, 
    0x2, 0x6ff, 0x708, 0x7, 0x9, 0x2, 0x2, 0x700, 0x705, 0x5, 0xde, 0x70, 
    0x2, 0x701, 0x702, 0x7, 0x8, 0x2, 0x2, 0x702, 0x704, 0x5, 0xde, 0x70, 
    0x2, 0x703, 0x701, 0x3, 0x2, 0x2, 0x2, 0x704, 0x707, 0x3, 0x2, 0x2, 
    0x2, 0x705, 0x703, 0x3, 0x2, 0x2, 0x2, 0x705, 0x706, 0x3, 0x2, 0x2, 
    0x2, 0x706, 0x709, 0x3, 0x2, 0x2, 0x2, 0x707, 0x705, 0x3, 0x2, 0x2, 
    0x2, 0x708, 0x700, 0x3, 0x2, 0x2, 0x2, 0x708, 0x709, 0x3, 0x2, 0x2, 
    0x2, 0x709, 0x70a, 0x3, 0x2, 0x2, 0x2, 0x70a, 0x70b, 0x7, 0xa, 0x2, 
    0x2, 0x70b, 0x71f, 0x3, 0x2, 0x2, 0x2, 0x70c, 0x70d, 0x7, 0x9, 0x2, 
    0x2, 0x70d, 0x70e, 0x5, 0xde, 0x70, 0x2, 0x70e, 0x70f, 0x7, 0xa, 0x2, 
    0x2, 0x70f, 0x71f, 0x3, 0x2, 0x2, 0x2, 0x710, 0x711, 0x7, 0x9, 0x2, 
    0x2, 0x711, 0x712, 0x5, 0x12, 0xa, 0x2, 0x712, 0x713, 0x7, 0xa, 0x2, 
    0x2, 0x713, 0x71f, 0x3, 0x2, 0x2, 0x2, 0x714, 0x716, 0x7, 0xa0, 0x2, 
    0x2, 0x715, 0x714, 0x3, 0x2, 0x2, 0x2, 0x715, 0x716, 0x3, 0x2, 0x2, 
    0x2, 0x716, 0x717, 0x3, 0x2, 0x2, 0x2, 0x717, 0x718, 0x7, 0x5c, 0x2, 
    0x2, 0x718, 0x719, 0x7, 0x9, 0x2, 0x2, 0x719, 0x71a, 0x5, 0x12, 0xa, 
    0x2, 0x71a, 0x71b, 0x7, 0xa, 0x2, 0x2, 0x71b, 0x71f, 0x3, 0x2, 0x2, 
    0x2, 0x71c, 0x71d, 0x7, 0xa0, 0x2, 0x2, 0x71d, 0x71f, 0x5, 0xde, 0x70, 
    0x6, 0x71e, 0x617, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x619, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x61a, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x61b, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x620, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x622, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x629, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x62e, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x64a, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x653, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x65a, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x661, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x66e, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x67b, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x688, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x695, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x69c, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x6b0, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x6c3, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x6ca, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x6d1, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x6da, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x6e3, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x6ec, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x6f5, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x6fe, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x70c, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x710, 0x3, 0x2, 0x2, 
    0x2, 0x71e, 0x715, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x71c, 0x3, 0x2, 0x2, 
    0x2, 0x71f, 0x77d, 0x3, 0x2, 0x2, 0x2, 0x720, 0x721, 0xc, 0x27, 0x2, 
    0x2, 0x721, 0x722, 0x9, 0x22, 0x2, 0x2, 0x722, 0x77c, 0x5, 0xde, 0x70, 
    0x28, 0x723, 0x724, 0xc, 0x26, 0x2, 0x2, 0x724, 0x725, 0x9, 0x23, 0x2, 
    0x2, 0x725, 0x77c, 0x5, 0xde, 0x70, 0x27, 0x726, 0x727, 0xc, 0x25, 0x2, 
    0x2, 0x727, 0x728, 0x9, 0x24, 0x2, 0x2, 0x728, 0x77c, 0x5, 0xde, 0x70, 
    0x26, 0x729, 0x72b, 0xc, 0xc, 0x2, 0x2, 0x72a, 0x72c, 0x7, 0xa0, 0x2, 
    0x2, 0x72b, 0x72a, 0x3, 0x2, 0x2, 0x2, 0x72b, 0x72c, 0x3, 0x2, 0x2, 
    0x2, 0x72c, 0x72d, 0x3, 0x2, 0x2, 0x2, 0x72d, 0x72e, 0x7, 0x83, 0x2, 
    0x2, 0x72e, 0x77c, 0x5, 0xde, 0x70, 0xd, 0x72f, 0x731, 0xc, 0xb, 0x2, 
    0x2, 0x730, 0x732, 0x7, 0xa0, 0x2, 0x2, 0x731, 0x730, 0x3, 0x2, 0x2, 
    0x2, 0x731, 0x732, 0x3, 0x2, 0x2, 0x2, 0x732, 0x733, 0x3, 0x2, 0x2, 
    0x2, 0x733, 0x734, 0x7, 0x83, 0x2, 0x2, 0x734, 0x735, 0x5, 0xde, 0x70, 
    0x2, 0x735, 0x736, 0x7, 0x59, 0x2, 0x2, 0x736, 0x737, 0x5, 0xde, 0x70, 
    0xc, 0x737, 0x77c, 0x3, 0x2, 0x2, 0x2, 0x738, 0x739, 0xc, 0x5, 0x2, 
    0x2, 0x739, 0x73a, 0x6, 0x70, 0xd, 0x2, 0x73a, 0x73b, 0x7, 0x1c, 0x2, 
    0x2, 0x73b, 0x77c, 0x5, 0xde, 0x70, 0x6, 0x73c, 0x73d, 0xc, 0x4, 0x2, 
    0x2, 0x73d, 0x73e, 0x7, 0xf6, 0x2, 0x2, 0x73e, 0x77c, 0x5, 0xde, 0x70, 
    0x5, 0x73f, 0x740, 0xc, 0x3, 0x2, 0x2, 0x740, 0x741, 0x7, 0xa8, 0x2, 
    0x2, 0x741, 0x77c, 0x5, 0xde, 0x70, 0x4, 0x742, 0x743, 0xc, 0x28, 0x2, 
    0x2, 0x743, 0x744, 0x7, 0xc, 0x2, 0x2, 0x744, 0x745, 0x5, 0xea, 0x76, 
    0x2, 0x745, 0x74e, 0x7, 0x9, 0x2, 0x2, 0x746, 0x74b, 0x5, 0xde, 0x70, 
    0x2, 0x747, 0x748, 0x7, 0x8, 0x2, 0x2, 0x748, 0x74a, 0x5, 0xde, 0x70, 
    0x2, 0x749, 0x747, 0x3, 0x2, 0x2, 0x2, 0x74a, 0x74d, 0x3, 0x2, 0x2, 
    0x2, 0x74b, 0x749, 0x3, 0x2, 0x2, 0x2, 0x74b, 0x74c, 0x3, 0x2, 0x2, 
    0x2, 0x74c, 0x74f, 0x3, 0x2, 0x2, 0x2, 0x74d, 0x74b, 0x3, 0x2, 0x2, 
    0x2, 0x74e, 0x746, 0x3, 0x2, 0x2, 0x2, 0x74e, 0x74f, 0x3, 0x2, 0x2, 
    0x2, 0x74f, 0x750, 0x3, 0x2, 0x2, 0x2, 0x750, 0x751, 0x7, 0xa, 0x2, 
    0x2, 0x751, 0x77c, 0x3, 0x2, 0x2, 0x2, 0x752, 0x753, 0xc, 0xd, 0x2, 
    0x2, 0x753, 0x755, 0x7, 0x7e, 0x2, 0x2, 0x754, 0x756, 0x7, 0xa0, 0x2, 
    0x2, 0x755, 0x754, 0x3, 0x2, 0x2, 0x2, 0x755, 0x756, 0x3, 0x2, 0x2, 
    0x2, 0x756, 0x757, 0x3, 0x2, 0x2, 0x2, 0x757, 0x77c, 0x7, 0xa2, 0x2, 
    0x2, 0x758, 0x75a, 0xc, 0x9, 0x2, 0x2, 0x759, 0x75b, 0x7, 0xa0, 0x2, 
    0x2, 0x75a, 0x759, 0x3, 0x2, 0x2, 0x2, 0x75a, 0x75b, 0x3, 0x2, 0x2, 
    0x2, 0x75b, 0x75c, 0x3, 0x2, 0x2, 0x2, 0x75c, 0x75d, 0x7, 0x74, 0x2, 
    0x2, 0x75d, 0x75e, 0x7, 0x9, 0x2, 0x2, 0x75e, 0x75f, 0x5, 0x12, 0xa, 
    0x2, 0x75f, 0x760, 0x7, 0xa, 0x2, 0x2, 0x760, 0x77c, 0x3, 0x2, 0x2, 
    0x2, 0x761, 0x763, 0xc, 0x8, 0x2, 0x2, 0x762, 0x764, 0x7, 0xa0, 0x2, 
    0x2, 0x763, 0x762, 0x3, 0x2, 0x2, 0x2, 0x763, 0x764, 0x3, 0x2, 0x2, 
    0x2, 0x764, 0x765, 0x3, 0x2, 0x2, 0x2, 0x765, 0x766, 0x7, 0x74, 0x2, 
    0x2, 0x766, 0x76f, 0x7, 0x9, 0x2, 0x2, 0x767, 0x76c, 0x5, 0xde, 0x70, 
    0x2, 0x768, 0x769, 0x7, 0x8, 0x2, 0x2, 0x769, 0x76b, 0x5, 0xde, 0x70, 
    0x2, 0x76a, 0x768, 0x3, 0x2, 0x2, 0x2, 0x76b, 0x76e, 0x3, 0x2, 0x2, 
    0x2, 0x76c, 0x76a, 0x3, 0x2, 0x2, 0x2, 0x76c, 0x76d, 0x3, 0x2, 0x2, 
    0x2, 0x76d, 0x770, 0x3, 0x2, 0x2, 0x2, 0x76e, 0x76c, 0x3, 0x2, 0x2, 
    0x2, 0x76f, 0x767, 0x3, 0x2, 0x2, 0x2, 0x76f, 0x770, 0x3, 0x2, 0x2, 
    0x2, 0x770, 0x771, 0x3, 0x2, 0x2, 0x2, 0x771, 0x77c, 0x7, 0xa, 0x2, 
    0x2, 0x772, 0x774, 0xc, 0x7, 0x2, 0x2, 0x773, 0x775, 0x7, 0xa0, 0x2, 
    0x2, 0x774, 0x773, 0x3, 0x2, 0x2, 0x2, 0x774, 0x775, 0x3, 0x2, 0x2, 
    0x2, 0x775, 0x776, 0x3, 0x2, 0x2, 0x2, 0x776, 0x777, 0x7, 0x25, 0x2, 
    0x2, 0x777, 0x778, 0x5, 0xde, 0x70, 0x2, 0x778, 0x779, 0x7, 0x1c, 0x2, 
    0x2, 0x779, 0x77a, 0x5, 0xde, 0x70, 0x2, 0x77a, 0x77c, 0x3, 0x2, 0x2, 
    0x2, 0x77b, 0x720, 0x3, 0x2, 0x2, 0x2, 0x77b, 0x723, 0x3, 0x2, 0x2, 
    0x2, 0x77b, 0x726, 0x3, 0x2, 0x2, 0x2, 0x77b, 0x729, 0x3, 0x2, 0x2, 
    0x2, 0x77b, 0x72f, 0x3, 0x2, 0x2, 0x2, 0x77b, 0x738, 0x3, 0x2, 0x2, 
    0x2, 0x77b, 0x73c, 0x3, 0x2, 0x2, 0x2, 0x77b, 0x73f, 0x3, 0x2, 0x2, 
    0x2, 0x77b, 0x742, 0x3, 0x2, 0x2, 0x2, 0x77b, 0x752, 0x3, 0x2, 0x2, 
    0x2, 0x77b, 0x758, 0x3, 0x2, 0x2, 0x2, 0x77b, 0x761, 0x3, 0x2, 0x2, 
    0x2, 0x77b, 0x772, 0x3, 0x2, 0x2, 0x2, 0x77c, 0x77f, 0x3, 0x2, 0x2, 
    0x2, 0x77d, 0x77b, 0x3, 0x2, 0x2, 0x2, 0x77d, 0x77e, 0x3, 0x2, 0x2, 
    0x2, 0x77e, 0xdf, 0x3, 0x2, 0x2, 0x2, 0x77f, 0x77d, 0x3, 0x2, 0x2, 0x2, 
    0x780, 0x781, 0x9, 0x25, 0x2, 0x2, 0x781, 0xe1, 0x3, 0x2, 0x2, 0x2, 
    0x782, 0x783, 0x5, 0xec, 0x77, 0x2, 0x783, 0x784, 0x7, 0xc, 0x2, 0x2, 
    0x784, 0x786, 0x3, 0x2, 0x2, 0x2, 0x785, 0x782, 0x3, 0x2, 0x2, 0x2, 
    0x785, 0x786, 0x3, 0x2, 0x2, 0x2, 0x786, 0x787, 0x3, 0x2, 0x2, 0x2, 
    0x787, 0x788, 0x5, 0xee, 0x78, 0x2, 0x788, 0x789, 0x7, 0xc, 0x2, 0x2, 
    0x789, 0x78b, 0x3, 0x2, 0x2, 0x2, 0x78a, 0x785, 0x3, 0x2, 0x2, 0x2, 
    0x78a, 0x78b, 0x3, 0x2, 0x2, 0x2, 0x78b, 0x78c, 0x3, 0x2, 0x2, 0x2, 
    0x78c, 0x78d, 0x5, 0xf0, 0x79, 0x2, 0x78d, 0x78e, 0x7, 0xc, 0x2, 0x2, 
    0x78e, 0x790, 0x3, 0x2, 0x2, 0x2, 0x78f, 0x78a, 0x3, 0x2, 0x2, 0x2, 
    0x78f, 0x790, 0x3, 0x2, 0x2, 0x2, 0x790, 0x791, 0x3, 0x2, 0x2, 0x2, 
    0x791, 0x792, 0x5, 0xf4, 0x7b, 0x2, 0x792, 0xe3, 0x3, 0x2, 0x2, 0x2, 
    0x793, 0x794, 0x5, 0xec, 0x77, 0x2, 0x794, 0x795, 0x7, 0xc, 0x2, 0x2, 
    0x795, 0x797, 0x3, 0x2, 0x2, 0x2, 0x796, 0x793, 0x3, 0x2, 0x2, 0x2, 
    0x796, 0x797, 0x3, 0x2, 0x2, 0x2, 0x797, 0x798, 0x3, 0x2, 0x2, 0x2, 
    0x798, 0x799, 0x5, 0xee, 0x78, 0x2, 0x799, 0x79a, 0x7, 0xc, 0x2, 0x2, 
    0x79a, 0x79c, 0x3, 0x2, 0x2, 0x2, 0x79b, 0x796, 0x3, 0x2, 0x2, 0x2, 
    0x79b, 0x79c, 0x3, 0x2, 0x2, 0x2, 0x79c, 0x79d, 0x3, 0x2, 0x2, 0x2, 
    0x79d, 0x79e, 0x5, 0xf0, 0x79, 0x2, 0x79e, 0x79f, 0x7, 0xc, 0x2, 0x2, 
    0x79f, 0x7a1, 0x3, 0x2, 0x2, 0x2, 0x7a0, 0x79b, 0x3, 0x2, 0x2, 0x2, 
    0x7a0, 0x7a1, 0x3, 0x2, 0x2, 0x2, 0x7a1, 0x7a2, 0x3, 0x2, 0x2, 0x2, 
    0x7a2, 0x7a3, 0x7, 0xe, 0x2, 0x2, 0x7a3, 0xe5, 0x3, 0x2, 0x2, 0x2, 0x7a4, 
    0x7a5, 0x5, 0x108, 0x85, 0x2, 0x7a5, 0x7a6, 0x7, 0xc, 0x2, 0x2, 0x7a6, 
    0x7a8, 0x3, 0x2, 0x2, 0x2, 0x7a7, 0x7a4, 0x3, 0x2, 0x2, 0x2, 0x7a8, 
    0x7ab, 0x3, 0x2, 0x2, 0x2, 0x7a9, 0x7a7, 0x3, 0x2, 0x2, 0x2, 0x7a9, 
    0x7aa, 0x3, 0x2, 0x2, 0x2, 0x7aa, 0x7ac, 0x3, 0x2, 0x2, 0x2, 0x7ab, 
    0x7a9, 0x3, 0x2, 0x2, 0x2, 0x7ac, 0x7ad, 0x5, 0x106, 0x84, 0x2, 0x7ad, 
    0xe7, 0x3, 0x2, 0x2, 0x2, 0x7ae, 0x7af, 0x5, 0x106, 0x84, 0x2, 0x7af, 
    0xe9, 0x3, 0x2, 0x2, 0x2, 0x7b0, 0x7b1, 0x5, 0x106, 0x84, 0x2, 0x7b1, 
    0xeb, 0x3, 0x2, 0x2, 0x2, 0x7b2, 0x7b3, 0x5, 0x106, 0x84, 0x2, 0x7b3, 
    0xed, 0x3, 0x2, 0x2, 0x2, 0x7b4, 0x7b5, 0x5, 0x106, 0x84, 0x2, 0x7b5, 
    0xef, 0x3, 0x2, 0x2, 0x2, 0x7b6, 0x7b7, 0x5, 0x106, 0x84, 0x2, 0x7b7, 
    0xf1, 0x3, 0x2, 0x2, 0x2, 0x7b8, 0x7b9, 0x5, 0x106, 0x84, 0x2, 0x7b9, 
    0xf3, 0x3, 0x2, 0x2, 0x2, 0x7ba, 0x7bb, 0x5, 0x106, 0x84, 0x2, 0x7bb, 
    0xf5, 0x3, 0x2, 0x2, 0x2, 0x7bc, 0x7bd, 0x5, 0x106, 0x84, 0x2, 0x7bd, 
    0xf7, 0x3, 0x2, 0x2, 0x2, 0x7be, 0x7bf, 0x5, 0x106, 0x84, 0x2, 0x7bf, 
    0xf9, 0x3, 0x2, 0x2, 0x2, 0x7c0, 0x7c1, 0x5, 0x106, 0x84, 0x2, 0x7c1, 
    0xfb, 0x3, 0x2, 0x2, 0x2, 0x7c2, 0x7c3, 0x5, 0x106, 0x84, 0x2, 0x7c3, 
    0xfd, 0x3, 0x2, 0x2, 0x2, 0x7c4, 0x7c5, 0x5, 0x106, 0x84, 0x2, 0x7c5, 
    0xff, 0x3, 0x2, 0x2, 0x2, 0x7c6, 0x7c7, 0x7, 0x6d, 0x2, 0x2, 0x7c7, 
    0x101, 0x3, 0x2, 0x2, 0x2, 0x7c8, 0x7cb, 0x5, 0x112, 0x8a, 0x2, 0x7c9, 
    0x7cb, 0x5, 0x106, 0x84, 0x2, 0x7ca, 0x7c8, 0x3, 0x2, 0x2, 0x2, 0x7ca, 
    0x7c9, 0x3, 0x2, 0x2, 0x2, 0x7cb, 0x103, 0x3, 0x2, 0x2, 0x2, 0x7cc, 
    0x7d0, 0x5, 0x112, 0x8a, 0x2, 0x7cd, 0x7d0, 0x5, 0x106, 0x84, 0x2, 0x7ce, 
    0x7d0, 0x5, 0xde, 0x70, 0x2, 0x7cf, 0x7cc, 0x3, 0x2, 0x2, 0x2, 0x7cf, 
    0x7cd, 0x3, 0x2, 0x2, 0x2, 0x7cf, 0x7ce, 0x3, 0x2, 0x2, 0x2, 0x7d0, 
    0x105, 0x3, 0x2, 0x2, 0x2, 0x7d1, 0x7d6, 0x7, 0xf9, 0x2, 0x2, 0x7d2, 
    0x7d6, 0x7, 0xfa, 0x2, 0x2, 0x7d3, 0x7d6, 0x7, 0xfb, 0x2, 0x2, 0x7d4, 
    0x7d6, 0x5, 0x130, 0x99, 0x2, 0x7d5, 0x7d1, 0x3, 0x2, 0x2, 0x2, 0x7d5, 
    0x7d2, 0x3, 0x2, 0x2, 0x2, 0x7d5, 0x7d3, 0x3, 0x2, 0x2, 0x2, 0x7d5, 
    0x7d4, 0x3, 0x2, 0x2, 0x2, 0x7d6, 0x107, 0x3, 0x2, 0x2, 0x2, 0x7d7, 
    0x7d8, 0x5, 0x106, 0x84, 0x2, 0x7d8, 0x109, 0x3, 0x2, 0x2, 0x2, 0x7d9, 
    0x7da, 0x5, 0x116, 0x8c, 0x2, 0x7da, 0x10b, 0x3, 0x2, 0x2, 0x2, 0x7db, 
    0x7dc, 0x5, 0x116, 0x8c, 0x2, 0x7dc, 0x10d, 0x3, 0x2, 0x2, 0x2, 0x7dd, 
    0x7de, 0x5, 0x116, 0x8c, 0x2, 0x7de, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x7df, 
    0x7ea, 0x7, 0xa2, 0x2, 0x2, 0x7e0, 0x7ea, 0x5, 0x114, 0x8b, 0x2, 0x7e1, 
    0x7ea, 0x5, 0x118, 0x8d, 0x2, 0x7e2, 0x7ea, 0x5, 0x11c, 0x8f, 0x2, 0x7e3, 
    0x7ea, 0x5, 0x120, 0x91, 0x2, 0x7e4, 0x7ea, 0x5, 0x122, 0x92, 0x2, 0x7e5, 
    0x7ea, 0x5, 0x124, 0x93, 0x2, 0x7e6, 0x7ea, 0x5, 0x128, 0x95, 0x2, 0x7e7, 
    0x7ea, 0x5, 0x126, 0x94, 0x2, 0x7e8, 0x7ea, 0x5, 0x12a, 0x96, 0x2, 0x7e9, 
    0x7df, 0x3, 0x2, 0x2, 0x2, 0x7e9, 0x7e0, 0x3, 0x2, 0x2, 0x2, 0x7e9, 
    0x7e1, 0x3, 0x2, 0x2, 0x2, 0x7e9, 0x7e2, 0x3, 0x2, 0x2, 0x2, 0x7e9, 
    0x7e3, 0x3, 0x2, 0x2, 0x2, 0x7e9, 0x7e4, 0x3, 0x2, 0x2, 0x2, 0x7e9, 
    0x7e5, 0x3, 0x2, 0x2, 0x2, 0x7e9, 0x7e6, 0x3, 0x2, 0x2, 0x2, 0x7e9, 
    0x7e7, 0x3, 0x2, 0x2, 0x2, 0x7e9, 0x7e8, 0x3, 0x2, 0x2, 0x2, 0x7ea, 
    0x111, 0x3, 0x2, 0x2, 0x2, 0x7eb, 0x7f2, 0x7, 0xa2, 0x2, 0x2, 0x7ec, 
    0x7f2, 0x5, 0x114, 0x8b, 0x2, 0x7ed, 0x7f2, 0x5, 0x118, 0x8d, 0x2, 0x7ee, 
    0x7f2, 0x5, 0x11c, 0x8f, 0x2, 0x7ef, 0x7f2, 0x5, 0x120, 0x91, 0x2, 0x7f0, 
    0x7f2, 0x5, 0x128, 0x95, 0x2, 0x7f1, 0x7eb, 0x3, 0x2, 0x2, 0x2, 0x7f1, 
    0x7ec, 0x3, 0x2, 0x2, 0x2, 0x7f1, 0x7ed, 0x3, 0x2, 0x2, 0x2, 0x7f1, 
    0x7ee, 0x3, 0x2, 0x2, 0x2, 0x7f1, 0x7ef, 0x3, 0x2, 0x2, 0x2, 0x7f1, 
    0x7f0, 0x3, 0x2, 0x2, 0x2, 0x7f2, 0x113, 0x3, 0x2, 0x2, 0x2, 0x7f3, 
    0x7f4, 0x9, 0x26, 0x2, 0x2, 0x7f4, 0x115, 0x3, 0x2, 0x2, 0x2, 0x7f5, 
    0x7f6, 0x7, 0x101, 0x2, 0x2, 0x7f6, 0x117, 0x3, 0x2, 0x2, 0x2, 0x7f7, 
    0x7fa, 0x7, 0x11, 0x2, 0x2, 0x7f8, 0x7fa, 0x7, 0xd, 0x2, 0x2, 0x7f9, 
    0x7f7, 0x3, 0x2, 0x2, 0x2, 0x7f9, 0x7f8, 0x3, 0x2, 0x2, 0x2, 0x7f9, 
    0x7fa, 0x3, 0x2, 0x2, 0x2, 0x7fa, 0x7fb, 0x3, 0x2, 0x2, 0x2, 0x7fb, 
    0x7fc, 0x7, 0x101, 0x2, 0x2, 0x7fc, 0x119, 0x3, 0x2, 0x2, 0x2, 0x7fd, 
    0x7fe, 0x7, 0x100, 0x2, 0x2, 0x7fe, 0x11b, 0x3, 0x2, 0x2, 0x2, 0x7ff, 
    0x802, 0x7, 0x11, 0x2, 0x2, 0x800, 0x802, 0x7, 0xd, 0x2, 0x2, 0x801, 
    0x7ff, 0x3, 0x2, 0x2, 0x2, 0x801, 0x800, 0x3, 0x2, 0x2, 0x2, 0x801, 
    0x802, 0x3, 0x2, 0x2, 0x2, 0x802, 0x803, 0x3, 0x2, 0x2, 0x2, 0x803, 
    0x804, 0x7, 0x100, 0x2, 0x2, 0x804, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x805, 
    0x806, 0x7, 0xff, 0x2, 0x2, 0x806, 0x11f, 0x3, 0x2, 0x2, 0x2, 0x807, 
    0x80a, 0x7, 0x11, 0x2, 0x2, 0x808, 0x80a, 0x7, 0xd, 0x2, 0x2, 0x809, 
    0x807, 0x3, 0x2, 0x2, 0x2, 0x809, 0x808, 0x3, 0x2, 0x2, 0x2, 0x809, 
    0x80a, 0x3, 0x2, 0x2, 0x2, 0x80a, 0x80b, 0x3, 0x2, 0x2, 0x2, 0x80b, 
    0x80c, 0x7, 0xff, 0x2, 0x2, 0x80c, 0x121, 0x3, 0x2, 0x2, 0x2, 0x80d, 
    0x80e, 0x7, 0x47, 0x2, 0x2, 0x80e, 0x80f, 0x5, 0x128, 0x95, 0x2, 0x80f, 
    0x123, 0x3, 0x2, 0x2, 0x2, 0x810, 0x811, 0x7, 0xd9, 0x2, 0x2, 0x811, 
    0x812, 0x5, 0x128, 0x95, 0x2, 0x812, 0x125, 0x3, 0x2, 0x2, 0x2, 0x813, 
    0x814, 0x7, 0xda, 0x2, 0x2, 0x814, 0x815, 0x5, 0x128, 0x95, 0x2, 0x815, 
    0x127, 0x3, 0x2, 0x2, 0x2, 0x816, 0x817, 0x9, 0x27, 0x2, 0x2, 0x817, 
    0x129, 0x3, 0x2, 0x2, 0x2, 0x818, 0x81b, 0x5, 0x12c, 0x97, 0x2, 0x819, 
    0x81b, 0x5, 0x12e, 0x98, 0x2, 0x81a, 0x818, 0x3, 0x2, 0x2, 0x2, 0x81a, 
    0x819, 0x3, 0x2, 0x2, 0x2, 0x81b, 0x12b, 0x3, 0x2, 0x2, 0x2, 0x81c, 
    0x81f, 0x7, 0x7c, 0x2, 0x2, 0x81d, 0x820, 0x7, 0x11, 0x2, 0x2, 0x81e, 
    0x820, 0x7, 0xd, 0x2, 0x2, 0x81f, 0x81d, 0x3, 0x2, 0x2, 0x2, 0x81f, 
    0x81e, 0x3, 0x2, 0x2, 0x2, 0x81f, 0x820, 0x3, 0x2, 0x2, 0x2, 0x820, 
    0x821, 0x3, 0x2, 0x2, 0x2, 0x821, 0x839, 0x5, 0x128, 0x95, 0x2, 0x822, 
    0x827, 0x7, 0xf7, 0x2, 0x2, 0x823, 0x824, 0x7, 0x9, 0x2, 0x2, 0x824, 
    0x825, 0x5, 0x10a, 0x86, 0x2, 0x825, 0x826, 0x7, 0xa, 0x2, 0x2, 0x826, 
    0x828, 0x3, 0x2, 0x2, 0x2, 0x827, 0x823, 0x3, 0x2, 0x2, 0x2, 0x827, 
    0x828, 0x3, 0x2, 0x2, 0x2, 0x828, 0x83a, 0x3, 0x2, 0x2, 0x2, 0x829, 
    0x82e, 0x7, 0xf7, 0x2, 0x2, 0x82a, 0x82b, 0x7, 0x9, 0x2, 0x2, 0x82b, 
    0x82c, 0x5, 0x10a, 0x86, 0x2, 0x82c, 0x82d, 0x7, 0xa, 0x2, 0x2, 0x82d, 
    0x82f, 0x3, 0x2, 0x2, 0x2, 0x82e, 0x82a, 0x3, 0x2, 0x2, 0x2, 0x82e, 
    0x82f, 0x3, 0x2, 0x2, 0x2, 0x82f, 0x830, 0x3, 0x2, 0x2, 0x2, 0x830, 
    0x831, 0x7, 0xdb, 0x2, 0x2, 0x831, 0x83a, 0x7, 0x97, 0x2, 0x2, 0x832, 
    0x837, 0x7, 0x97, 0x2, 0x2, 0x833, 0x834, 0x7, 0x9, 0x2, 0x2, 0x834, 
    0x835, 0x5, 0x10a, 0x86, 0x2, 0x835, 0x836, 0x7, 0xa, 0x2, 0x2, 0x836, 
    0x838, 0x3, 0x2, 0x2, 0x2, 0x837, 0x833, 0x3, 0x2, 0x2, 0x2, 0x837, 
    0x838, 0x3, 0x2, 0x2, 0x2, 0x838, 0x83a, 0x3, 0x2, 0x2, 0x2, 0x839, 
    0x822, 0x3, 0x2, 0x2, 0x2, 0x839, 0x829, 0x3, 0x2, 0x2, 0x2, 0x839, 
    0x832, 0x3, 0x2, 0x2, 0x2, 0x83a, 0x12d, 0x3, 0x2, 0x2, 0x2, 0x83b, 
    0x83e, 0x7, 0x7c, 0x2, 0x2, 0x83c, 0x83f, 0x7, 0x11, 0x2, 0x2, 0x83d, 
    0x83f, 0x7, 0xd, 0x2, 0x2, 0x83e, 0x83c, 0x3, 0x2, 0x2, 0x2, 0x83e, 
    0x83d, 0x3, 0x2, 0x2, 0x2, 0x83e, 0x83f, 0x3, 0x2, 0x2, 0x2, 0x83f, 
    0x840, 0x3, 0x2, 0x2, 0x2, 0x840, 0x895, 0x5, 0x128, 0x95, 0x2, 0x841, 
    0x846, 0x7, 0x49, 0x2, 0x2, 0x842, 0x843, 0x7, 0x9, 0x2, 0x2, 0x843, 
    0x844, 0x5, 0x10a, 0x86, 0x2, 0x844, 0x845, 0x7, 0xa, 0x2, 0x2, 0x845, 
    0x847, 0x3, 0x2, 0x2, 0x2, 0x846, 0x842, 0x3, 0x2, 0x2, 0x2, 0x846, 
    0x847, 0x3, 0x2, 0x2, 0x2, 0x847, 0x896, 0x3, 0x2, 0x2, 0x2, 0x848, 
    0x84d, 0x7, 0x49, 0x2, 0x2, 0x849, 0x84a, 0x7, 0x9, 0x2, 0x2, 0x84a, 
    0x84b, 0x5, 0x10a, 0x86, 0x2, 0x84b, 0x84c, 0x7, 0xa, 0x2, 0x2, 0x84c, 
    0x84e, 0x3, 0x2, 0x2, 0x2, 0x84d, 0x849, 0x3, 0x2, 0x2, 0x2, 0x84d, 
    0x84e, 0x3, 0x2, 0x2, 0x2, 0x84e, 0x84f, 0x3, 0x2, 0x2, 0x2, 0x84f, 
    0x859, 0x7, 0xdb, 0x2, 0x2, 0x850, 0x85a, 0x7, 0x70, 0x2, 0x2, 0x851, 
    0x85a, 0x7, 0x94, 0x2, 0x2, 0x852, 0x857, 0x7, 0xc2, 0x2, 0x2, 0x853, 
    0x854, 0x7, 0x9, 0x2, 0x2, 0x854, 0x855, 0x5, 0x10c, 0x87, 0x2, 0x855, 
    0x856, 0x7, 0xa, 0x2, 0x2, 0x856, 0x858, 0x3, 0x2, 0x2, 0x2, 0x857, 
    0x853, 0x3, 0x2, 0x2, 0x2, 0x857, 0x858, 0x3, 0x2, 0x2, 0x2, 0x858, 
    0x85a, 0x3, 0x2, 0x2, 0x2, 0x859, 0x850, 0x3, 0x2, 0x2, 0x2, 0x859, 
    0x851, 0x3, 0x2, 0x2, 0x2, 0x859, 0x852, 0x3, 0x2, 0x2, 0x2, 0x85a, 
    0x896, 0x3, 0x2, 0x2, 0x2, 0x85b, 0x860, 0x7, 0x70, 0x2, 0x2, 0x85c, 
    0x85d, 0x7, 0x9, 0x2, 0x2, 0x85d, 0x85e, 0x5, 0x10a, 0x86, 0x2, 0x85e, 
    0x85f, 0x7, 0xa, 0x2, 0x2, 0x85f, 0x861, 0x3, 0x2, 0x2, 0x2, 0x860, 
    0x85c, 0x3, 0x2, 0x2, 0x2, 0x860, 0x861, 0x3, 0x2, 0x2, 0x2, 0x861, 
    0x862, 0x3, 0x2, 0x2, 0x2, 0x862, 0x86b, 0x7, 0xdb, 0x2, 0x2, 0x863, 
    0x86c, 0x7, 0x94, 0x2, 0x2, 0x864, 0x869, 0x7, 0xc2, 0x2, 0x2, 0x865, 
    0x866, 0x7, 0x9, 0x2, 0x2, 0x866, 0x867, 0x5, 0x10c, 0x87, 0x2, 0x867, 
    0x868, 0x7, 0xa, 0x2, 0x2, 0x868, 0x86a, 0x3, 0x2, 0x2, 0x2, 0x869, 
    0x865, 0x3, 0x2, 0x2, 0x2, 0x869, 0x86a, 0x3, 0x2, 0x2, 0x2, 0x86a, 
    0x86c, 0x3, 0x2, 0x2, 0x2, 0x86b, 0x863, 0x3, 0x2, 0x2, 0x2, 0x86b, 
    0x864, 0x3, 0x2, 0x2, 0x2, 0x86c, 0x896, 0x3, 0x2, 0x2, 0x2, 0x86d, 
    0x872, 0x7, 0x94, 0x2, 0x2, 0x86e, 0x86f, 0x7, 0x9, 0x2, 0x2, 0x86f, 
    0x870, 0x5, 0x10a, 0x86, 0x2, 0x870, 0x871, 0x7, 0xa, 0x2, 0x2, 0x871, 
    0x873, 0x3, 0x2, 0x2, 0x2, 0x872, 0x86e, 0x3, 0x2, 0x2, 0x2, 0x872, 
    0x873, 0x3, 0x2, 0x2, 0x2, 0x873, 0x874, 0x3, 0x2, 0x2, 0x2, 0x874, 
    0x875, 0x7, 0xdb, 0x2, 0x2, 0x875, 0x87a, 0x7, 0xc2, 0x2, 0x2, 0x876, 
    0x877, 0x7, 0x9, 0x2, 0x2, 0x877, 0x878, 0x5, 0x10c, 0x87, 0x2, 0x878, 
    0x879, 0x7, 0xa, 0x2, 0x2, 0x879, 0x87b, 0x3, 0x2, 0x2, 0x2, 0x87a, 
    0x876, 0x3, 0x2, 0x2, 0x2, 0x87a, 0x87b, 0x3, 0x2, 0x2, 0x2, 0x87b, 
    0x896, 0x3, 0x2, 0x2, 0x2, 0x87c, 0x881, 0x7, 0x70, 0x2, 0x2, 0x87d, 
    0x87e, 0x7, 0x9, 0x2, 0x2, 0x87e, 0x87f, 0x5, 0x10a, 0x86, 0x2, 0x87f, 
    0x880, 0x7, 0xa, 0x2, 0x2, 0x880, 0x882, 0x3, 0x2, 0x2, 0x2, 0x881, 
    0x87d, 0x3, 0x2, 0x2, 0x2, 0x881, 0x882, 0x3, 0x2, 0x2, 0x2, 0x882, 
    0x896, 0x3, 0x2, 0x2, 0x2, 0x883, 0x888, 0x7, 0x94, 0x2, 0x2, 0x884, 
    0x885, 0x7, 0x9, 0x2, 0x2, 0x885, 0x886, 0x5, 0x10a, 0x86, 0x2, 0x886, 
    0x887, 0x7, 0xa, 0x2, 0x2, 0x887, 0x889, 0x3, 0x2, 0x2, 0x2, 0x888, 
    0x884, 0x3, 0x2, 0x2, 0x2, 0x888, 0x889, 0x3, 0x2, 0x2, 0x2, 0x889, 
    0x896, 0x3, 0x2, 0x2, 0x2, 0x88a, 0x893, 0x7, 0xc2, 0x2, 0x2, 0x88b, 
    0x88c, 0x7, 0x9, 0x2, 0x2, 0x88c, 0x88f, 0x5, 0x10a, 0x86, 0x2, 0x88d, 
    0x88e, 0x7, 0x8, 0x2, 0x2, 0x88e, 0x890, 0x5, 0x10c, 0x87, 0x2, 0x88f, 
    0x88d, 0x3, 0x2, 0x2, 0x2, 0x88f, 0x890, 0x3, 0x2, 0x2, 0x2, 0x890, 
    0x891, 0x3, 0x2, 0x2, 0x2, 0x891, 0x892, 0x7, 0xa, 0x2, 0x2, 0x892, 
    0x894, 0x3, 0x2, 0x2, 0x2, 0x893, 0x88b, 0x3, 0x2, 0x2, 0x2, 0x893, 
    0x894, 0x3, 0x2, 0x2, 0x2, 0x894, 0x896, 0x3, 0x2, 0x2, 0x2, 0x895, 
    0x841, 0x3, 0x2, 0x2, 0x2, 0x895, 0x848, 0x3, 0x2, 0x2, 0x2, 0x895, 
    0x85b, 0x3, 0x2, 0x2, 0x2, 0x895, 0x86d, 0x3, 0x2, 0x2, 0x2, 0x895, 
    0x87c, 0x3, 0x2, 0x2, 0x2, 0x895, 0x883, 0x3, 0x2, 0x2, 0x2, 0x895, 
    0x88a, 0x3, 0x2, 0x2, 0x2, 0x896, 0x12f, 0x3, 0x2, 0x2, 0x2, 0x897, 
    0x898, 0x9, 0x28, 0x2, 0x2, 0x898, 0x131, 0x3, 0x2, 0x2, 0x2, 0x109, 
    0x135, 0x161, 0x165, 0x167, 0x170, 0x178, 0x181, 0x186, 0x18a, 0x190, 
    0x19c, 0x1a4, 0x1af, 0x1b7, 0x1bc, 0x1c4, 0x1c7, 0x1cd, 0x1d3, 0x1d5, 
    0x1da, 0x1df, 0x1e3, 0x1e8, 0x1ec, 0x1f2, 0x1fa, 0x1fd, 0x203, 0x209, 
    0x20b, 0x213, 0x219, 0x21c, 0x221, 0x22a, 0x22d, 0x231, 0x23b, 0x23e, 
    0x244, 0x248, 0x24a, 0x250, 0x254, 0x256, 0x258, 0x25e, 0x264, 0x26b, 
    0x270, 0x278, 0x27c, 0x287, 0x28b, 0x294, 0x296, 0x29c, 0x2a1, 0x2a8, 
    0x2aa, 0x2af, 0x2bc, 0x2c2, 0x2ca, 0x2d0, 0x2db, 0x2e4, 0x2f0, 0x2fb, 
    0x2fe, 0x302, 0x309, 0x313, 0x317, 0x31c, 0x31f, 0x326, 0x335, 0x33c, 
    0x343, 0x34c, 0x354, 0x35a, 0x35f, 0x36b, 0x373, 0x376, 0x37a, 0x383, 
    0x388, 0x38d, 0x394, 0x399, 0x39c, 0x3a3, 0x3a8, 0x3ab, 0x3b2, 0x3bd, 
    0x3d8, 0x3e4, 0x3ee, 0x3f4, 0x3f9, 0x3fc, 0x418, 0x41f, 0x423, 0x434, 
    0x43a, 0x442, 0x445, 0x454, 0x45b, 0x460, 0x465, 0x46a, 0x46e, 0x478, 
    0x47a, 0x485, 0x490, 0x499, 0x49f, 0x4a3, 0x4a6, 0x4af, 0x4b4, 0x4b8, 
    0x4bc, 0x4c7, 0x4cb, 0x4d1, 0x4d6, 0x4d8, 0x4dc, 0x4e2, 0x4e6, 0x4f2, 
    0x4fa, 0x4fe, 0x501, 0x506, 0x50d, 0x514, 0x51d, 0x51f, 0x526, 0x52d, 
    0x537, 0x539, 0x540, 0x549, 0x54b, 0x552, 0x55a, 0x561, 0x568, 0x56f, 
    0x573, 0x575, 0x57b, 0x580, 0x584, 0x587, 0x58c, 0x590, 0x593, 0x597, 
    0x59f, 0x5a8, 0x5ae, 0x5b0, 0x5b7, 0x5bc, 0x5bf, 0x5c2, 0x5c9, 0x5d3, 
    0x5d8, 0x5df, 0x5e9, 0x5f7, 0x601, 0x607, 0x60e, 0x637, 0x640, 0x642, 
    0x646, 0x668, 0x66b, 0x675, 0x678, 0x682, 0x685, 0x691, 0x6a3, 0x6aa, 
    0x6ac, 0x6b5, 0x6b8, 0x6bb, 0x6bf, 0x6cd, 0x6d5, 0x6de, 0x6e7, 0x6f0, 
    0x6f9, 0x705, 0x708, 0x715, 0x71e, 0x72b, 0x731, 0x74b, 0x74e, 0x755, 
    0x75a, 0x763, 0x76c, 0x76f, 0x774, 0x77b, 0x77d, 0x785, 0x78a, 0x78f, 
    0x796, 0x79b, 0x7a0, 0x7a9, 0x7ca, 0x7cf, 0x7d5, 0x7e9, 0x7f1, 0x7f9, 
    0x801, 0x809, 0x81a, 0x81f, 0x827, 0x82e, 0x837, 0x839, 0x83e, 0x846, 
    0x84d, 0x857, 0x859, 0x860, 0x869, 0x86b, 0x872, 0x87a, 0x881, 0x888, 
    0x88f, 0x893, 0x895, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

sqlParser::Initializer sqlParser::_init;
