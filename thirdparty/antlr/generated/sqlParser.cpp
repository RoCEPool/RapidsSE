
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
    setState(273); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(272);
      statement();
      setState(275); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::T__0)
      | (1ULL << sqlParser::T__1)
      | (1ULL << sqlParser::T__4)
      | (1ULL << sqlParser::T__8)
      | (1ULL << sqlParser::T__10)
      | (1ULL << sqlParser::BPA)
      | (1ULL << sqlParser::COUNT)
      | (1ULL << sqlParser::CREATE))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (sqlParser::DLOAD - 70))
      | (1ULL << (sqlParser::DROP - 70))
      | (1ULL << (sqlParser::EXPLAIN - 70))
      | (1ULL << (sqlParser::FILE - 70))
      | (1ULL << (sqlParser::LOAD - 70))
      | (1ULL << (sqlParser::LOG - 70))
      | (1ULL << (sqlParser::LSCOL - 70))
      | (1ULL << (sqlParser::LSIDX - 70))
      | (1ULL << (sqlParser::LSIDXCOL - 70))
      | (1ULL << (sqlParser::LSTBL - 70))
      | (1ULL << (sqlParser::MEMORYLEAKS - 70)))) != 0) || ((((_la - 137) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 137)) & ((1ULL << (sqlParser::NOOP - 137))
      | (1ULL << (sqlParser::NOP - 137))
      | (1ULL << (sqlParser::SELECT - 137))
      | (1ULL << (sqlParser::SLEEP - 137))
      | (1ULL << (sqlParser::SPACE - 137))
      | (1ULL << (sqlParser::STATUS - 137))
      | (1ULL << (sqlParser::SZIDX - 137))
      | (1ULL << (sqlParser::SZTBL - 137))
      | (1ULL << (sqlParser::TRACKMEMORY - 137))
      | (1ULL << (sqlParser::TRUNCATE - 137)))) != 0) || _la == sqlParser::WITH

    || _la == sqlParser::HINT_START);
    setState(277);
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
    setState(279);
    tags();
    setState(280);
    statementChoice();
    setState(281);
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

sqlParser::QueryStatementContext* sqlParser::StatementChoiceContext::queryStatement() {
  return getRuleContext<sqlParser::QueryStatementContext>(0);
}

sqlParser::ExplainStatementContext* sqlParser::StatementChoiceContext::explainStatement() {
  return getRuleContext<sqlParser::ExplainStatementContext>(0);
}

sqlParser::CreateQueryContext* sqlParser::StatementChoiceContext::createQuery() {
  return getRuleContext<sqlParser::CreateQueryContext>(0);
}

sqlParser::DropQueryContext* sqlParser::StatementChoiceContext::dropQuery() {
  return getRuleContext<sqlParser::DropQueryContext>(0);
}

sqlParser::TruncateQueryContext* sqlParser::StatementChoiceContext::truncateQuery() {
  return getRuleContext<sqlParser::TruncateQueryContext>(0);
}

sqlParser::DloadContext* sqlParser::StatementChoiceContext::dload() {
  return getRuleContext<sqlParser::DloadContext>(0);
}

sqlParser::LoadContext* sqlParser::StatementChoiceContext::load() {
  return getRuleContext<sqlParser::LoadContext>(0);
}

sqlParser::LogContext* sqlParser::StatementChoiceContext::log() {
  return getRuleContext<sqlParser::LogContext>(0);
}

sqlParser::LscolContext* sqlParser::StatementChoiceContext::lscol() {
  return getRuleContext<sqlParser::LscolContext>(0);
}

sqlParser::LsidxContext* sqlParser::StatementChoiceContext::lsidx() {
  return getRuleContext<sqlParser::LsidxContext>(0);
}

sqlParser::LsidxcolContext* sqlParser::StatementChoiceContext::lsidxcol() {
  return getRuleContext<sqlParser::LsidxcolContext>(0);
}

sqlParser::LstblContext* sqlParser::StatementChoiceContext::lstbl() {
  return getRuleContext<sqlParser::LstblContext>(0);
}

sqlParser::CountContext* sqlParser::StatementChoiceContext::count() {
  return getRuleContext<sqlParser::CountContext>(0);
}

sqlParser::SleepContext* sqlParser::StatementChoiceContext::sleep() {
  return getRuleContext<sqlParser::SleepContext>(0);
}

sqlParser::SpaceContext* sqlParser::StatementChoiceContext::space() {
  return getRuleContext<sqlParser::SpaceContext>(0);
}

sqlParser::StatusContext* sqlParser::StatementChoiceContext::status() {
  return getRuleContext<sqlParser::StatusContext>(0);
}

sqlParser::SzidxContext* sqlParser::StatementChoiceContext::szidx() {
  return getRuleContext<sqlParser::SzidxContext>(0);
}

sqlParser::SztblContext* sqlParser::StatementChoiceContext::sztbl() {
  return getRuleContext<sqlParser::SztblContext>(0);
}

sqlParser::TrackmemoryContext* sqlParser::StatementChoiceContext::trackmemory() {
  return getRuleContext<sqlParser::TrackmemoryContext>(0);
}

sqlParser::MemoryleaksContext* sqlParser::StatementChoiceContext::memoryleaks() {
  return getRuleContext<sqlParser::MemoryleaksContext>(0);
}

sqlParser::BpaContext* sqlParser::StatementChoiceContext::bpa() {
  return getRuleContext<sqlParser::BpaContext>(0);
}

sqlParser::FileContext* sqlParser::StatementChoiceContext::file() {
  return getRuleContext<sqlParser::FileContext>(0);
}

sqlParser::NoopContext* sqlParser::StatementChoiceContext::noop() {
  return getRuleContext<sqlParser::NoopContext>(0);
}

sqlParser::EmptyCommandContext* sqlParser::StatementChoiceContext::emptyCommand() {
  return getRuleContext<sqlParser::EmptyCommandContext>(0);
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
    setState(307);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(283);
      queryStatement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(284);
      explainStatement();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(285);
      createQuery();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(286);
      dropQuery();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(287);
      truncateQuery();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(288);
      dload();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(289);
      load();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(290);
      log();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(291);
      lscol();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(292);
      lsidx();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(293);
      lsidxcol();
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(294);
      lstbl();
      break;
    }

    case 13: {
      enterOuterAlt(_localctx, 13);
      setState(295);
      count();
      break;
    }

    case 14: {
      enterOuterAlt(_localctx, 14);
      setState(296);
      sleep();
      break;
    }

    case 15: {
      enterOuterAlt(_localctx, 15);
      setState(297);
      space();
      break;
    }

    case 16: {
      enterOuterAlt(_localctx, 16);
      setState(298);
      status();
      break;
    }

    case 17: {
      enterOuterAlt(_localctx, 17);
      setState(299);
      szidx();
      break;
    }

    case 18: {
      enterOuterAlt(_localctx, 18);
      setState(300);
      sztbl();
      break;
    }

    case 19: {
      enterOuterAlt(_localctx, 19);
      setState(301);
      trackmemory();
      break;
    }

    case 20: {
      enterOuterAlt(_localctx, 20);
      setState(302);
      memoryleaks();
      break;
    }

    case 21: {
      enterOuterAlt(_localctx, 21);
      setState(303);
      bpa();
      break;
    }

    case 22: {
      enterOuterAlt(_localctx, 22);
      setState(304);
      file();
      break;
    }

    case 23: {
      enterOuterAlt(_localctx, 23);
      setState(305);
      noop();
      break;
    }

    case 24: {
      enterOuterAlt(_localctx, 24);
      setState(306);
      emptyCommand();
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
    setState(313);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__1

    || _la == sqlParser::T__4) {
      setState(311);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
      case 1: {
        setState(309);
        keyOnlyTag();
        break;
      }

      case 2: {
        setState(310);
        keyValueTag();
        break;
      }

      }
      setState(315);
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

tree::TerminalNode* sqlParser::KeyValueTagContext::PLAIN_IDENTIFIER() {
  return getToken(sqlParser::PLAIN_IDENTIFIER, 0);
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
    setState(316);
    match(sqlParser::T__1);
    setState(317);
    dynamic_cast<KeyValueTagContext *>(_localctx)->keyName = match(sqlParser::PLAIN_IDENTIFIER);
    setState(318);
    match(sqlParser::T__2);
    setState(319);
    match(sqlParser::T__3);
    setState(322);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::DUBLQUOTED_IDENTIFIER:
      case sqlParser::SINGLQUOTED_STRING:
      case sqlParser::DUBLQUOTED_STRING: {
        setState(320);
        stringLiteral();
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        setState(321);
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

tree::TerminalNode* sqlParser::KeyOnlyTagContext::PLAIN_IDENTIFIER() {
  return getToken(sqlParser::PLAIN_IDENTIFIER, 0);
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
    setState(329);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__4: {
        enterOuterAlt(_localctx, 1);
        setState(324);
        match(sqlParser::T__4);
        setState(325);
        match(sqlParser::PLAIN_IDENTIFIER);
        break;
      }

      case sqlParser::T__1: {
        enterOuterAlt(_localctx, 2);
        setState(326);
        match(sqlParser::T__1);
        setState(327);
        match(sqlParser::PLAIN_IDENTIFIER);
        setState(328);
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

//----------------- ExplainStatementContext ------------------------------------------------------------------

sqlParser::ExplainStatementContext::ExplainStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::ExplainStatementContext::EXPLAIN() {
  return getToken(sqlParser::EXPLAIN, 0);
}

sqlParser::StatementChoiceContext* sqlParser::ExplainStatementContext::statementChoice() {
  return getRuleContext<sqlParser::StatementChoiceContext>(0);
}

std::vector<sqlParser::QueryHintContext *> sqlParser::ExplainStatementContext::queryHint() {
  return getRuleContexts<sqlParser::QueryHintContext>();
}

sqlParser::QueryHintContext* sqlParser::ExplainStatementContext::queryHint(size_t i) {
  return getRuleContext<sqlParser::QueryHintContext>(i);
}

std::vector<sqlParser::ExplainOptionsContext *> sqlParser::ExplainStatementContext::explainOptions() {
  return getRuleContexts<sqlParser::ExplainOptionsContext>();
}

sqlParser::ExplainOptionsContext* sqlParser::ExplainStatementContext::explainOptions(size_t i) {
  return getRuleContext<sqlParser::ExplainOptionsContext>(i);
}


size_t sqlParser::ExplainStatementContext::getRuleIndex() const {
  return sqlParser::RuleExplainStatement;
}


antlrcpp::Any sqlParser::ExplainStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitExplainStatement(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::ExplainStatementContext* sqlParser::explainStatement() {
  ExplainStatementContext *_localctx = _tracker.createInstance<ExplainStatementContext>(_ctx, getState());
  enterRule(_localctx, 12, sqlParser::RuleExplainStatement);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(334);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(331);
      queryHint();
      setState(336);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(337);
    match(sqlParser::EXPLAIN);
    setState(341);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::FORMAT || ((((_la - 177) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 177)) & ((1ULL << (sqlParser::STAGE - 177))
      | (1ULL << (sqlParser::WITH_NAMES - 177))
      | (1ULL << (sqlParser::WITH_TYPES - 177)))) != 0)) {
      setState(338);
      explainOptions();
      setState(343);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(344);
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
  enterRule(_localctx, 14, sqlParser::RuleExplainOptions);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(350);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::STAGE: {
        enterOuterAlt(_localctx, 1);
        setState(346);
        explainStageList();
        break;
      }

      case sqlParser::FORMAT: {
        enterOuterAlt(_localctx, 2);
        setState(347);
        explainFormat();
        break;
      }

      case sqlParser::WITH_NAMES: {
        enterOuterAlt(_localctx, 3);
        setState(348);
        match(sqlParser::WITH_NAMES);
        break;
      }

      case sqlParser::WITH_TYPES: {
        enterOuterAlt(_localctx, 4);
        setState(349);
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
  enterRule(_localctx, 16, sqlParser::RuleExplainStageList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(352);
    match(sqlParser::STAGE);
    setState(353);
    explainRange();
    setState(358);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(354);
      match(sqlParser::T__5);
      setState(355);
      explainRange();
      setState(360);
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
  enterRule(_localctx, 18, sqlParser::RuleExplainRange);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(361);
    dynamic_cast<ExplainRangeContext *>(_localctx)->begin = explainRangeChoice();
    setState(364);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__6 || _la == sqlParser::TO) {
      setState(362);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__6 || _la == sqlParser::TO)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(363);
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
  enterRule(_localctx, 20, sqlParser::RuleExplainRangeChoice);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(369);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__7:
      case sqlParser::ALL: {
        enterOuterAlt(_localctx, 1);
        setState(366);
        dynamic_cast<ExplainRangeChoiceContext *>(_localctx)->all = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__7

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
        setState(367);
        dynamic_cast<ExplainRangeChoiceContext *>(_localctx)->num = match(sqlParser::INTEGER_LITERAL);
        break;
      }

      case sqlParser::ASC:
      case sqlParser::BIT:
      case sqlParser::BITS:
      case sqlParser::BLOCK:
      case sqlParser::BLOOM:
      case sqlParser::BPA:
      case sqlParser::BTREE:
      case sqlParser::BYTE:
      case sqlParser::BYTES:
      case sqlParser::CATALOG:
      case sqlParser::CHARS:
      case sqlParser::CONCAT:
      case sqlParser::DATA:
      case sqlParser::DATE_FORMAT:
      case sqlParser::DAYS:
      case sqlParser::DECODE:
      case sqlParser::DESC:
      case sqlParser::DETAIL:
      case sqlParser::DIGITS:
      case sqlParser::DLOAD:
      case sqlParser::ECHO:
      case sqlParser::ESCAPE_CHAR:
      case sqlParser::EXPLAIN:
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
      case sqlParser::KEY:
      case sqlParser::LIMIT:
      case sqlParser::LOAD:
      case sqlParser::LOG:
      case sqlParser::LOOP:
      case sqlParser::LSIDX:
      case sqlParser::LSTBL:
      case sqlParser::MEMORYLEAKS:
      case sqlParser::MILLIS:
      case sqlParser::MILLISECOND:
      case sqlParser::MILLISECONDS:
      case sqlParser::MINUTES:
      case sqlParser::MONOTONIC:
      case sqlParser::MONTHS:
      case sqlParser::MS:
      case sqlParser::NODE:
      case sqlParser::NOOP:
      case sqlParser::NOP:
      case sqlParser::NOW:
      case sqlParser::OBJECTS:
      case sqlParser::OFF:
      case sqlParser::OFFSET:
      case sqlParser::PATHS:
      case sqlParser::PLACING:
      case sqlParser::PRETTY:
      case sqlParser::QUARTER:
      case sqlParser::QUARTERS:
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
        setState(368);
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
  enterRule(_localctx, 22, sqlParser::RuleExplainFormat);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(371);
    match(sqlParser::FORMAT);
    setState(372);
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

//----------------- StatisticsStatementContext ------------------------------------------------------------------

sqlParser::StatisticsStatementContext::StatisticsStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::StatementChoiceContext* sqlParser::StatisticsStatementContext::statementChoice() {
  return getRuleContext<sqlParser::StatementChoiceContext>(0);
}

tree::TerminalNode* sqlParser::StatisticsStatementContext::STATS() {
  return getToken(sqlParser::STATS, 0);
}

tree::TerminalNode* sqlParser::StatisticsStatementContext::STATISTICS() {
  return getToken(sqlParser::STATISTICS, 0);
}

std::vector<sqlParser::QueryHintContext *> sqlParser::StatisticsStatementContext::queryHint() {
  return getRuleContexts<sqlParser::QueryHintContext>();
}

sqlParser::QueryHintContext* sqlParser::StatisticsStatementContext::queryHint(size_t i) {
  return getRuleContext<sqlParser::QueryHintContext>(i);
}


size_t sqlParser::StatisticsStatementContext::getRuleIndex() const {
  return sqlParser::RuleStatisticsStatement;
}


antlrcpp::Any sqlParser::StatisticsStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitStatisticsStatement(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::StatisticsStatementContext* sqlParser::statisticsStatement() {
  StatisticsStatementContext *_localctx = _tracker.createInstance<StatisticsStatementContext>(_ctx, getState());
  enterRule(_localctx, 24, sqlParser::RuleStatisticsStatement);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(377);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(374);
      queryHint();
      setState(379);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(380);
    _la = _input->LA(1);
    if (!(_la == sqlParser::STATS

    || _la == sqlParser::STATISTICS)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(381);
    statementChoice();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QueryStatementContext ------------------------------------------------------------------

sqlParser::QueryStatementContext::QueryStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::QueryContext* sqlParser::QueryStatementContext::query() {
  return getRuleContext<sqlParser::QueryContext>(0);
}


size_t sqlParser::QueryStatementContext::getRuleIndex() const {
  return sqlParser::RuleQueryStatement;
}


antlrcpp::Any sqlParser::QueryStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitQueryStatement(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::QueryStatementContext* sqlParser::queryStatement() {
  QueryStatementContext *_localctx = _tracker.createInstance<QueryStatementContext>(_ctx, getState());
  enterRule(_localctx, 26, sqlParser::RuleQueryStatement);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(383);
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
  enterRule(_localctx, 28, sqlParser::RuleQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(388);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(385);
      queryHint();
      setState(390);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(393);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      setState(391);
      selectQuery();
      break;
    }

    case 2: {
      setState(392);
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
  enterRule(_localctx, 30, sqlParser::RuleQueryHint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(395);
    match(sqlParser::HINT_START);
    setState(397);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::ASC)
      | (1ULL << sqlParser::BIT)
      | (1ULL << sqlParser::BITS)
      | (1ULL << sqlParser::BLOCK)
      | (1ULL << sqlParser::BLOOM)
      | (1ULL << sqlParser::BPA)
      | (1ULL << sqlParser::BTREE)
      | (1ULL << sqlParser::BYTE)
      | (1ULL << sqlParser::BYTES)
      | (1ULL << sqlParser::CATALOG)
      | (1ULL << sqlParser::CHARS)
      | (1ULL << sqlParser::CONCAT)
      | (1ULL << sqlParser::DATA)
      | (1ULL << sqlParser::DATE_FORMAT)
      | (1ULL << sqlParser::DAYS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::DECODE - 64))
      | (1ULL << (sqlParser::DESC - 64))
      | (1ULL << (sqlParser::DETAIL - 64))
      | (1ULL << (sqlParser::DIGITS - 64))
      | (1ULL << (sqlParser::DLOAD - 64))
      | (1ULL << (sqlParser::ECHO - 64))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
      | (1ULL << (sqlParser::EXPLAIN - 64))
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
      | (1ULL << (sqlParser::KEY - 64))
      | (1ULL << (sqlParser::LIMIT - 64))
      | (1ULL << (sqlParser::LOAD - 64))
      | (1ULL << (sqlParser::LOG - 64))
      | (1ULL << (sqlParser::LOOP - 64))
      | (1ULL << (sqlParser::LSIDX - 64))
      | (1ULL << (sqlParser::LSTBL - 64))
      | (1ULL << (sqlParser::MEMORYLEAKS - 64))
      | (1ULL << (sqlParser::MILLIS - 64))
      | (1ULL << (sqlParser::MILLISECOND - 64))
      | (1ULL << (sqlParser::MILLISECONDS - 64)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::MINUTES - 129))
      | (1ULL << (sqlParser::MONOTONIC - 129))
      | (1ULL << (sqlParser::MONTHS - 129))
      | (1ULL << (sqlParser::MS - 129))
      | (1ULL << (sqlParser::NODE - 129))
      | (1ULL << (sqlParser::NOOP - 129))
      | (1ULL << (sqlParser::NOP - 129))
      | (1ULL << (sqlParser::NOW - 129))
      | (1ULL << (sqlParser::OBJECTS - 129))
      | (1ULL << (sqlParser::OFF - 129))
      | (1ULL << (sqlParser::OFFSET - 129))
      | (1ULL << (sqlParser::PATHS - 129))
      | (1ULL << (sqlParser::PLACING - 129))
      | (1ULL << (sqlParser::PRETTY - 129))
      | (1ULL << (sqlParser::QUARTER - 129))
      | (1ULL << (sqlParser::QUARTERS - 129))
      | (1ULL << (sqlParser::REFERENCE - 129))
      | (1ULL << (sqlParser::REGIONS - 129))
      | (1ULL << (sqlParser::REPLICATED - 129))
      | (1ULL << (sqlParser::ROWSIZE - 129))
      | (1ULL << (sqlParser::SCHEMA - 129))
      | (1ULL << (sqlParser::SEC - 129))
      | (1ULL << (sqlParser::SECONDS - 129))
      | (1ULL << (sqlParser::SKIPLIST - 129))
      | (1ULL << (sqlParser::SLEEP - 129))
      | (1ULL << (sqlParser::SPACE - 129))
      | (1ULL << (sqlParser::STAGE - 129))
      | (1ULL << (sqlParser::STATS - 129))
      | (1ULL << (sqlParser::STATISTICS - 129))
      | (1ULL << (sqlParser::STATUS - 129))
      | (1ULL << (sqlParser::SUBSTR - 129))
      | (1ULL << (sqlParser::SZIDX - 129))
      | (1ULL << (sqlParser::SZTBL - 129))
      | (1ULL << (sqlParser::S_ONLY - 129))
      | (1ULL << (sqlParser::TABLESIZE - 129))
      | (1ULL << (sqlParser::TEXT - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::TOP - 194))
      | (1ULL << (sqlParser::TRACKMEMORY - 194))
      | (1ULL << (sqlParser::TREE - 194))
      | (1ULL << (sqlParser::TRUNCATE - 194))
      | (1ULL << (sqlParser::TYPE - 194))
      | (1ULL << (sqlParser::TYPEOF - 194))
      | (1ULL << (sqlParser::VALUE - 194))
      | (1ULL << (sqlParser::WEEK - 194))
      | (1ULL << (sqlParser::WEEKS - 194))
      | (1ULL << (sqlParser::WITH_NAMES - 194))
      | (1ULL << (sqlParser::WITH_TYPES - 194))
      | (1ULL << (sqlParser::XOR - 194))
      | (1ULL << (sqlParser::YEARS - 194))
      | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 194))
      | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 194))
      | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 194)))) != 0)) {
      setState(396);
      hint();
    }
    setState(403);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(399);
      match(sqlParser::T__5);
      setState(400);
      hint();
      setState(405);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(406);
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
  enterRule(_localctx, 32, sqlParser::RuleHint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(447);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(408);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(409);
      dynamic_cast<HintContext *>(_localctx)->exprName = exprHint();
      setState(410);
      match(sqlParser::T__3);
      setState(411);
      expr(0);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(413);
      dynamic_cast<HintContext *>(_localctx)->exprName = exprHint();
      setState(415);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__3) {
        setState(414);
        match(sqlParser::T__3);
      }
      setState(417);
      match(sqlParser::T__8);
      setState(418);
      expr(0);
      setState(423);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(419);
        match(sqlParser::T__5);
        setState(420);
        expr(0);
        setState(425);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(426);
      match(sqlParser::T__9);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(428);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(429);
      match(sqlParser::T__3);
      setState(430);
      hintValue();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(432);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(434);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__3) {
        setState(433);
        match(sqlParser::T__3);
      }
      setState(436);
      match(sqlParser::T__8);
      setState(437);
      hintValue();
      setState(442);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(438);
        match(sqlParser::T__5);
        setState(439);
        hintValue();
        setState(444);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(445);
      match(sqlParser::T__9);
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
  enterRule(_localctx, 34, sqlParser::RuleSelectQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(458);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(449);
      match(sqlParser::WITH);
      setState(450);
      withQuery();
      setState(455);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(451);
        match(sqlParser::T__5);
        setState(452);
        withQuery();
        setState(457);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(460);
    selectExpr();
    setState(464);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(461);
      match(sqlParser::ORDER);
      setState(462);
      match(sqlParser::BY);
      setState(463);
      dynamic_cast<SelectQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(472);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(466);
      match(sqlParser::LIMIT);
      setState(467);
      dynamic_cast<SelectQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(470);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(468);
        match(sqlParser::OFFSET);
        setState(469);
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
  enterRule(_localctx, 36, sqlParser::RuleSelectExpr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(501);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__10:
      case sqlParser::SELECT: {
        enterOuterAlt(_localctx, 1);
        setState(474);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__10 || _la == sqlParser::SELECT)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(477);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::ALL: {
            setState(475);
            match(sqlParser::ALL);
            break;
          }

          case sqlParser::DISTINCT: {
            setState(476);
            dynamic_cast<SelectExprContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
            break;
          }

          case sqlParser::T__6:
          case sqlParser::T__7:
          case sqlParser::T__8:
          case sqlParser::T__12:
          case sqlParser::ASC:
          case sqlParser::AVG:
          case sqlParser::BIT:
          case sqlParser::BITS:
          case sqlParser::BLOCK:
          case sqlParser::BLOOM:
          case sqlParser::BPA:
          case sqlParser::BTREE:
          case sqlParser::BYTE:
          case sqlParser::BYTES:
          case sqlParser::CASE:
          case sqlParser::CAST:
          case sqlParser::CATALOG:
          case sqlParser::CHARS:
          case sqlParser::COALESCE:
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
          case sqlParser::ECHO:
          case sqlParser::ESCAPE_CHAR:
          case sqlParser::EXISTS:
          case sqlParser::EXPLAIN:
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
          case sqlParser::INTERVAL:
          case sqlParser::KEY:
          case sqlParser::LIMIT:
          case sqlParser::LOAD:
          case sqlParser::LOG:
          case sqlParser::LOOP:
          case sqlParser::LSIDX:
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
          case sqlParser::OVERLAY:
          case sqlParser::PATHS:
          case sqlParser::PLACING:
          case sqlParser::POSITION:
          case sqlParser::PRETTY:
          case sqlParser::QUARTER:
          case sqlParser::QUARTERS:
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
        setState(479);
        selectList();
        setState(482);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
        case 1: {
          setState(480);
          match(sqlParser::FROM);
          setState(481);
          tableExpr(0);
          break;
        }

        }
        setState(486);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
        case 1: {
          setState(484);
          match(sqlParser::WHERE);
          setState(485);
          dynamic_cast<SelectExprContext *>(_localctx)->where = predicate();
          break;
        }

        }
        setState(491);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
        case 1: {
          setState(488);
          match(sqlParser::GROUP);
          setState(489);
          match(sqlParser::BY);
          setState(490);
          dynamic_cast<SelectExprContext *>(_localctx)->groupBy = groupByList();
          break;
        }

        }
        setState(495);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
        case 1: {
          setState(493);
          match(sqlParser::HAVING);
          setState(494);
          dynamic_cast<SelectExprContext *>(_localctx)->having = predicate();
          break;
        }

        }
        break;
      }

      case sqlParser::T__8: {
        enterOuterAlt(_localctx, 2);
        setState(497);
        match(sqlParser::T__8);
        setState(498);
        selectExpr();
        setState(499);
        match(sqlParser::T__9);
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
  enterRule(_localctx, 38, sqlParser::RuleSetQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(512);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(503);
      match(sqlParser::WITH);
      setState(504);
      withQuery();
      setState(509);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(505);
        match(sqlParser::T__5);
        setState(506);
        withQuery();
        setState(511);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(514);
    setExpr(0);
    setState(518);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(515);
      match(sqlParser::ORDER);
      setState(516);
      match(sqlParser::BY);
      setState(517);
      dynamic_cast<SetQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(526);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(520);
      match(sqlParser::LIMIT);
      setState(521);
      dynamic_cast<SetQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(524);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(522);
        match(sqlParser::OFFSET);
        setState(523);
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
  size_t startState = 40;
  enterRecursionRule(_localctx, 40, sqlParser::RuleSetExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(534);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      setState(529);
      selectExpr();
      break;
    }

    case 2: {
      setState(530);
      match(sqlParser::T__8);
      setState(531);
      setExpr(0);
      setState(532);
      match(sqlParser::T__9);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(548);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<SetExprContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleSetExpr);
        setState(536);

        if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
        setState(537);
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
        setState(540);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::DISTINCT: {
            setState(538);
            match(sqlParser::DISTINCT);
            break;
          }

          case sqlParser::ALL: {
            setState(539);
            dynamic_cast<SetExprContext *>(_localctx)->all = match(sqlParser::ALL);
            break;
          }

          case sqlParser::T__8:
          case sqlParser::T__10:
          case sqlParser::CORRESPONDING:
          case sqlParser::SELECT: {
            break;
          }

        default:
          break;
        }
        setState(543);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::CORRESPONDING) {
          setState(542);
          corrClause();
        }
        setState(545);
        setExpr(3); 
      }
      setState(550);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx);
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
  enterRule(_localctx, 42, sqlParser::RuleCorrClause);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(551);
    dynamic_cast<CorrClauseContext *>(_localctx)->corr = match(sqlParser::CORRESPONDING);
    setState(557);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::BY) {
      setState(552);
      match(sqlParser::BY);
      setState(553);
      match(sqlParser::T__8);
      setState(554);
      corrList();
      setState(555);
      match(sqlParser::T__9);
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
  enterRule(_localctx, 44, sqlParser::RuleWithQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(560);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::RECURSIVE) {
      setState(559);
      match(sqlParser::RECURSIVE);
    }
    setState(562);
    dynamic_cast<WithQueryContext *>(_localctx)->name = tableName();
    setState(564);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__8) {
      setState(563);
      colAliases();
    }
    setState(566);
    match(sqlParser::AS);
    setState(567);
    match(sqlParser::T__8);
    setState(568);
    query();
    setState(569);
    match(sqlParser::T__9);
   
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
  size_t startState = 46;
  enterRecursionRule(_localctx, 46, sqlParser::RuleTableExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(603);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<FetchContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(572);
      dynamic_cast<FetchContext *>(_localctx)->ref = tableReference();
      setState(577);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx)) {
      case 1: {
        setState(574);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(573);
          match(sqlParser::AS);
        }
        setState(576);
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
      setState(579);
      match(sqlParser::T__8);
      setState(580);
      query();
      setState(581);
      match(sqlParser::T__9);
      setState(589);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 49, _ctx)) {
      case 1: {
        setState(583);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(582);
          match(sqlParser::AS);
        }
        setState(585);
        dynamic_cast<SubqueryContext *>(_localctx)->as = tableAlias();
        setState(587);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 48, _ctx)) {
        case 1: {
          setState(586);
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
      setState(591);
      match(sqlParser::T__8);
      setState(592);
      valuesExpr();
      setState(593);
      match(sqlParser::T__9);
      setState(601);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 52, _ctx)) {
      case 1: {
        setState(595);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(594);
          match(sqlParser::AS);
        }
        setState(597);
        dynamic_cast<ValuesSubqueryContext *>(_localctx)->as = tableAlias();
        setState(599);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx)) {
        case 1: {
          setState(598);
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
    setState(665);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 63, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(663);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 62, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<CrossJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(605);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(609);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(606);
            queryHint();
            setState(611);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(615);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case sqlParser::T__5: {
              setState(612);
              match(sqlParser::T__5);
              break;
            }

            case sqlParser::CROSS: {
              setState(613);
              match(sqlParser::CROSS);
              setState(614);
              match(sqlParser::JOIN);
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(617);
          tableExpr(7);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<NaturalJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(618);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(622);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(619);
            queryHint();
            setState(624);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(625);
          match(sqlParser::NATURAL);

          setState(627);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 89) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 89)) & ((1ULL << (sqlParser::FULL - 89))
            | (1ULL << (sqlParser::INNER - 89))
            | (1ULL << (sqlParser::LEFT - 89)))) != 0) || _la == sqlParser::RIGHT) {
            setState(626);
            dynamic_cast<NaturalJoinContext *>(_localctx)->type = joinType();
          }
          setState(629);
          match(sqlParser::JOIN);
          setState(630);
          tableExpr(6);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<JoinOnContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(631);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(635);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(632);
            queryHint();
            setState(637);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(639);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::ANTI || ((((_la - 89) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 89)) & ((1ULL << (sqlParser::FULL - 89))
            | (1ULL << (sqlParser::INNER - 89))
            | (1ULL << (sqlParser::LEFT - 89)))) != 0) || _la == sqlParser::RIGHT

          || _la == sqlParser::SEMI) {
            setState(638);
            dynamic_cast<JoinOnContext *>(_localctx)->type = extendedJoinType();
          }
          setState(641);
          match(sqlParser::JOIN);
          setState(642);
          tableExpr(0);
          setState(643);
          match(sqlParser::ON);
          setState(644);
          joinPredicate();
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<JoinUsingContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(646);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(650);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(647);
            queryHint();
            setState(652);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(654);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::ANTI || ((((_la - 89) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 89)) & ((1ULL << (sqlParser::FULL - 89))
            | (1ULL << (sqlParser::INNER - 89))
            | (1ULL << (sqlParser::LEFT - 89)))) != 0) || _la == sqlParser::RIGHT

          || _la == sqlParser::SEMI) {
            setState(653);
            dynamic_cast<JoinUsingContext *>(_localctx)->type = extendedJoinType();
          }
          setState(656);
          match(sqlParser::JOIN);
          setState(657);
          tableExpr(0);
          setState(658);
          match(sqlParser::USING);
          setState(659);
          match(sqlParser::T__8);
          setState(660);
          joinUsingList();
          setState(661);
          match(sqlParser::T__9);
          break;
        }

        } 
      }
      setState(667);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 63, _ctx);
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
  enterRule(_localctx, 48, sqlParser::RuleTableReference);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(676);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 65, _ctx)) {
    case 1: {
      setState(671);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 64, _ctx)) {
      case 1: {
        setState(668);
        dynamic_cast<TableReferenceContext *>(_localctx)->catalog = catalogName();
        setState(669);
        match(sqlParser::T__11);
        break;
      }

      }
      setState(673);
      dynamic_cast<TableReferenceContext *>(_localctx)->schema = schemaName();
      setState(674);
      match(sqlParser::T__11);
      break;
    }

    }
    setState(678);
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
  enterRule(_localctx, 50, sqlParser::RuleJoinType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(685);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::INNER: {
        enterOuterAlt(_localctx, 1);
        setState(680);
        match(sqlParser::INNER);
        break;
      }

      case sqlParser::FULL:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 2);
        setState(681);
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
        setState(683);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::OUTER) {
          setState(682);
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
  enterRule(_localctx, 52, sqlParser::RuleExtendedJoinType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(690);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::FULL:
      case sqlParser::INNER:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 1);
        setState(687);
        joinType();
        break;
      }

      case sqlParser::SEMI: {
        enterOuterAlt(_localctx, 2);
        setState(688);
        match(sqlParser::SEMI);
        break;
      }

      case sqlParser::ANTI: {
        enterOuterAlt(_localctx, 3);
        setState(689);
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
  enterRule(_localctx, 54, sqlParser::RuleValuesExpr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(692);
    match(sqlParser::VALUES);
    setState(693);
    match(sqlParser::T__8);
    setState(694);
    valueList();
    setState(695);
    match(sqlParser::T__9);
    setState(703);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(696);
      match(sqlParser::T__5);
      setState(697);
      match(sqlParser::T__8);
      setState(698);
      valueList();
      setState(699);
      match(sqlParser::T__9);
      setState(705);
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

//----------------- InsertQueryContext ------------------------------------------------------------------

sqlParser::InsertQueryContext::InsertQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::InsertQueryContext::INSERT() {
  return getToken(sqlParser::INSERT, 0);
}

sqlParser::TableReferenceContext* sqlParser::InsertQueryContext::tableReference() {
  return getRuleContext<sqlParser::TableReferenceContext>(0);
}

sqlParser::ValuesExprContext* sqlParser::InsertQueryContext::valuesExpr() {
  return getRuleContext<sqlParser::ValuesExprContext>(0);
}

sqlParser::SelectQueryContext* sqlParser::InsertQueryContext::selectQuery() {
  return getRuleContext<sqlParser::SelectQueryContext>(0);
}

sqlParser::SetQueryContext* sqlParser::InsertQueryContext::setQuery() {
  return getRuleContext<sqlParser::SetQueryContext>(0);
}

tree::TerminalNode* sqlParser::InsertQueryContext::WITH() {
  return getToken(sqlParser::WITH, 0);
}

std::vector<sqlParser::WithQueryContext *> sqlParser::InsertQueryContext::withQuery() {
  return getRuleContexts<sqlParser::WithQueryContext>();
}

sqlParser::WithQueryContext* sqlParser::InsertQueryContext::withQuery(size_t i) {
  return getRuleContext<sqlParser::WithQueryContext>(i);
}

tree::TerminalNode* sqlParser::InsertQueryContext::INTO() {
  return getToken(sqlParser::INTO, 0);
}

std::vector<sqlParser::ColumnNameContext *> sqlParser::InsertQueryContext::columnName() {
  return getRuleContexts<sqlParser::ColumnNameContext>();
}

sqlParser::ColumnNameContext* sqlParser::InsertQueryContext::columnName(size_t i) {
  return getRuleContext<sqlParser::ColumnNameContext>(i);
}


size_t sqlParser::InsertQueryContext::getRuleIndex() const {
  return sqlParser::RuleInsertQuery;
}


antlrcpp::Any sqlParser::InsertQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitInsertQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::InsertQueryContext* sqlParser::insertQuery() {
  InsertQueryContext *_localctx = _tracker.createInstance<InsertQueryContext>(_ctx, getState());
  enterRule(_localctx, 56, sqlParser::RuleInsertQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(715);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(706);
      match(sqlParser::WITH);
      setState(707);
      withQuery();
      setState(712);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(708);
        match(sqlParser::T__5);
        setState(709);
        withQuery();
        setState(714);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(717);
    match(sqlParser::INSERT);
    setState(719);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::INTO) {
      setState(718);
      match(sqlParser::INTO);
    }
    setState(721);
    dynamic_cast<InsertQueryContext *>(_localctx)->ref = tableReference();
    setState(733);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 74, _ctx)) {
    case 1: {
      setState(722);
      match(sqlParser::T__8);
      setState(723);
      columnName();
      setState(728);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(724);
        match(sqlParser::T__5);
        setState(725);
        columnName();
        setState(730);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(731);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(738);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx)) {
    case 1: {
      setState(735);
      valuesExpr();
      break;
    }

    case 2: {
      setState(736);
      selectQuery();
      break;
    }

    case 3: {
      setState(737);
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

//----------------- DeleteQueryContext ------------------------------------------------------------------

sqlParser::DeleteQueryContext::DeleteQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DeleteQueryContext::DELETE() {
  return getToken(sqlParser::DELETE, 0);
}

sqlParser::TableReferenceContext* sqlParser::DeleteQueryContext::tableReference() {
  return getRuleContext<sqlParser::TableReferenceContext>(0);
}

tree::TerminalNode* sqlParser::DeleteQueryContext::WITH() {
  return getToken(sqlParser::WITH, 0);
}

std::vector<sqlParser::WithQueryContext *> sqlParser::DeleteQueryContext::withQuery() {
  return getRuleContexts<sqlParser::WithQueryContext>();
}

sqlParser::WithQueryContext* sqlParser::DeleteQueryContext::withQuery(size_t i) {
  return getRuleContext<sqlParser::WithQueryContext>(i);
}

std::vector<tree::TerminalNode *> sqlParser::DeleteQueryContext::FROM() {
  return getTokens(sqlParser::FROM);
}

tree::TerminalNode* sqlParser::DeleteQueryContext::FROM(size_t i) {
  return getToken(sqlParser::FROM, i);
}

tree::TerminalNode* sqlParser::DeleteQueryContext::WHERE() {
  return getToken(sqlParser::WHERE, 0);
}

sqlParser::PredicateContext* sqlParser::DeleteQueryContext::predicate() {
  return getRuleContext<sqlParser::PredicateContext>(0);
}

sqlParser::TableExprContext* sqlParser::DeleteQueryContext::tableExpr() {
  return getRuleContext<sqlParser::TableExprContext>(0);
}


size_t sqlParser::DeleteQueryContext::getRuleIndex() const {
  return sqlParser::RuleDeleteQuery;
}


antlrcpp::Any sqlParser::DeleteQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDeleteQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DeleteQueryContext* sqlParser::deleteQuery() {
  DeleteQueryContext *_localctx = _tracker.createInstance<DeleteQueryContext>(_ctx, getState());
  enterRule(_localctx, 58, sqlParser::RuleDeleteQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(749);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(740);
      match(sqlParser::WITH);
      setState(741);
      withQuery();
      setState(746);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(742);
        match(sqlParser::T__5);
        setState(743);
        withQuery();
        setState(748);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(751);
    match(sqlParser::DELETE);
    setState(753);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::FROM) {
      setState(752);
      match(sqlParser::FROM);
    }
    setState(755);
    dynamic_cast<DeleteQueryContext *>(_localctx)->ref = tableReference();
    setState(760);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(756);
        match(sqlParser::WHERE);
        setState(757);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(758);
        match(sqlParser::FROM);
        setState(759);
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

//----------------- UpdateQueryContext ------------------------------------------------------------------

sqlParser::UpdateQueryContext::UpdateQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::UpdateQueryContext::UPDATE() {
  return getToken(sqlParser::UPDATE, 0);
}

tree::TerminalNode* sqlParser::UpdateQueryContext::SET() {
  return getToken(sqlParser::SET, 0);
}

std::vector<sqlParser::ColumnNameContext *> sqlParser::UpdateQueryContext::columnName() {
  return getRuleContexts<sqlParser::ColumnNameContext>();
}

sqlParser::ColumnNameContext* sqlParser::UpdateQueryContext::columnName(size_t i) {
  return getRuleContext<sqlParser::ColumnNameContext>(i);
}

std::vector<sqlParser::ExprContext *> sqlParser::UpdateQueryContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::UpdateQueryContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::TableReferenceContext* sqlParser::UpdateQueryContext::tableReference() {
  return getRuleContext<sqlParser::TableReferenceContext>(0);
}

tree::TerminalNode* sqlParser::UpdateQueryContext::WITH() {
  return getToken(sqlParser::WITH, 0);
}

std::vector<sqlParser::WithQueryContext *> sqlParser::UpdateQueryContext::withQuery() {
  return getRuleContexts<sqlParser::WithQueryContext>();
}

sqlParser::WithQueryContext* sqlParser::UpdateQueryContext::withQuery(size_t i) {
  return getRuleContext<sqlParser::WithQueryContext>(i);
}

tree::TerminalNode* sqlParser::UpdateQueryContext::WHERE() {
  return getToken(sqlParser::WHERE, 0);
}

sqlParser::PredicateContext* sqlParser::UpdateQueryContext::predicate() {
  return getRuleContext<sqlParser::PredicateContext>(0);
}

tree::TerminalNode* sqlParser::UpdateQueryContext::FROM() {
  return getToken(sqlParser::FROM, 0);
}

sqlParser::TableExprContext* sqlParser::UpdateQueryContext::tableExpr() {
  return getRuleContext<sqlParser::TableExprContext>(0);
}


size_t sqlParser::UpdateQueryContext::getRuleIndex() const {
  return sqlParser::RuleUpdateQuery;
}


antlrcpp::Any sqlParser::UpdateQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitUpdateQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::UpdateQueryContext* sqlParser::updateQuery() {
  UpdateQueryContext *_localctx = _tracker.createInstance<UpdateQueryContext>(_ctx, getState());
  enterRule(_localctx, 60, sqlParser::RuleUpdateQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(771);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(762);
      match(sqlParser::WITH);
      setState(763);
      withQuery();
      setState(768);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(764);
        match(sqlParser::T__5);
        setState(765);
        withQuery();
        setState(770);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(773);
    match(sqlParser::UPDATE);
    setState(774);
    dynamic_cast<UpdateQueryContext *>(_localctx)->ref = tableReference();
    setState(775);
    match(sqlParser::SET);
    setState(776);
    columnName();
    setState(777);
    match(sqlParser::T__3);
    setState(778);
    expr(0);
    setState(786);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(779);
      match(sqlParser::T__5);
      setState(780);
      columnName();
      setState(781);
      match(sqlParser::T__3);
      setState(782);
      expr(0);
      setState(788);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(793);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(789);
        match(sqlParser::WHERE);
        setState(790);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(791);
        match(sqlParser::FROM);
        setState(792);
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

//----------------- CreateQueryContext ------------------------------------------------------------------

sqlParser::CreateQueryContext::CreateQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::CreateTableQueryContext* sqlParser::CreateQueryContext::createTableQuery() {
  return getRuleContext<sqlParser::CreateTableQueryContext>(0);
}

sqlParser::CreateIndexQueryContext* sqlParser::CreateQueryContext::createIndexQuery() {
  return getRuleContext<sqlParser::CreateIndexQueryContext>(0);
}


size_t sqlParser::CreateQueryContext::getRuleIndex() const {
  return sqlParser::RuleCreateQuery;
}


antlrcpp::Any sqlParser::CreateQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCreateQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CreateQueryContext* sqlParser::createQuery() {
  CreateQueryContext *_localctx = _tracker.createInstance<CreateQueryContext>(_ctx, getState());
  enterRule(_localctx, 62, sqlParser::RuleCreateQuery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(797);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 84, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(795);
      createTableQuery();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(796);
      createIndexQuery();
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

//----------------- CreateTableQueryContext ------------------------------------------------------------------

sqlParser::CreateTableQueryContext::CreateTableQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::CreateTableQueryContext::CREATE() {
  return getToken(sqlParser::CREATE, 0);
}

tree::TerminalNode* sqlParser::CreateTableQueryContext::TABLE() {
  return getToken(sqlParser::TABLE, 0);
}

sqlParser::TableNameContext* sqlParser::CreateTableQueryContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::TableDefinitionContext* sqlParser::CreateTableQueryContext::tableDefinition() {
  return getRuleContext<sqlParser::TableDefinitionContext>(0);
}


size_t sqlParser::CreateTableQueryContext::getRuleIndex() const {
  return sqlParser::RuleCreateTableQuery;
}


antlrcpp::Any sqlParser::CreateTableQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCreateTableQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CreateTableQueryContext* sqlParser::createTableQuery() {
  CreateTableQueryContext *_localctx = _tracker.createInstance<CreateTableQueryContext>(_ctx, getState());
  enterRule(_localctx, 64, sqlParser::RuleCreateTableQuery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(799);
    match(sqlParser::CREATE);
    setState(800);
    match(sqlParser::TABLE);
    setState(801);
    tableName();

    setState(802);
    tableDefinition();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CreateIndexQueryContext ------------------------------------------------------------------

sqlParser::CreateIndexQueryContext::CreateIndexQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::CREATE() {
  return getToken(sqlParser::CREATE, 0);
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::INDEX() {
  return getToken(sqlParser::INDEX, 0);
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::ON() {
  return getToken(sqlParser::ON, 0);
}

sqlParser::TableNameContext* sqlParser::CreateIndexQueryContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

std::vector<sqlParser::ExprContext *> sqlParser::CreateIndexQueryContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::CreateIndexQueryContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

sqlParser::IndexNameContext* sqlParser::CreateIndexQueryContext::indexName() {
  return getRuleContext<sqlParser::IndexNameContext>(0);
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::UNIQUE() {
  return getToken(sqlParser::UNIQUE, 0);
}


size_t sqlParser::CreateIndexQueryContext::getRuleIndex() const {
  return sqlParser::RuleCreateIndexQuery;
}


antlrcpp::Any sqlParser::CreateIndexQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCreateIndexQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CreateIndexQueryContext* sqlParser::createIndexQuery() {
  CreateIndexQueryContext *_localctx = _tracker.createInstance<CreateIndexQueryContext>(_ctx, getState());
  enterRule(_localctx, 66, sqlParser::RuleCreateIndexQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(804);
    match(sqlParser::CREATE);
    setState(806);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(805);
      match(sqlParser::UNIQUE);
    }
    setState(808);
    match(sqlParser::INDEX);
    setState(809);
    dynamic_cast<CreateIndexQueryContext *>(_localctx)->idxName = indexName();
    setState(810);
    match(sqlParser::ON);
    setState(811);
    tableName();
    setState(812);
    match(sqlParser::T__8);
    setState(813);
    expr(0);
    setState(818);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(814);
      match(sqlParser::T__5);
      setState(815);
      expr(0);
      setState(820);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(821);
    match(sqlParser::T__9);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DropQueryContext ------------------------------------------------------------------

sqlParser::DropQueryContext::DropQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::DropTableQueryContext* sqlParser::DropQueryContext::dropTableQuery() {
  return getRuleContext<sqlParser::DropTableQueryContext>(0);
}

sqlParser::DropIndexQueryContext* sqlParser::DropQueryContext::dropIndexQuery() {
  return getRuleContext<sqlParser::DropIndexQueryContext>(0);
}


size_t sqlParser::DropQueryContext::getRuleIndex() const {
  return sqlParser::RuleDropQuery;
}


antlrcpp::Any sqlParser::DropQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDropQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DropQueryContext* sqlParser::dropQuery() {
  DropQueryContext *_localctx = _tracker.createInstance<DropQueryContext>(_ctx, getState());
  enterRule(_localctx, 68, sqlParser::RuleDropQuery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(825);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 87, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(823);
      dropTableQuery();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(824);
      dropIndexQuery();
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

//----------------- DropTableQueryContext ------------------------------------------------------------------

sqlParser::DropTableQueryContext::DropTableQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DropTableQueryContext::DROP() {
  return getToken(sqlParser::DROP, 0);
}

tree::TerminalNode* sqlParser::DropTableQueryContext::TABLE() {
  return getToken(sqlParser::TABLE, 0);
}

sqlParser::TableNameContext* sqlParser::DropTableQueryContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}


size_t sqlParser::DropTableQueryContext::getRuleIndex() const {
  return sqlParser::RuleDropTableQuery;
}


antlrcpp::Any sqlParser::DropTableQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDropTableQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DropTableQueryContext* sqlParser::dropTableQuery() {
  DropTableQueryContext *_localctx = _tracker.createInstance<DropTableQueryContext>(_ctx, getState());
  enterRule(_localctx, 70, sqlParser::RuleDropTableQuery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(827);
    match(sqlParser::DROP);
    setState(828);
    match(sqlParser::TABLE);
    setState(829);
    dynamic_cast<DropTableQueryContext *>(_localctx)->name = tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DropIndexQueryContext ------------------------------------------------------------------

sqlParser::DropIndexQueryContext::DropIndexQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DropIndexQueryContext::DROP() {
  return getToken(sqlParser::DROP, 0);
}

tree::TerminalNode* sqlParser::DropIndexQueryContext::INDEX() {
  return getToken(sqlParser::INDEX, 0);
}

tree::TerminalNode* sqlParser::DropIndexQueryContext::ON() {
  return getToken(sqlParser::ON, 0);
}

sqlParser::IndexNameContext* sqlParser::DropIndexQueryContext::indexName() {
  return getRuleContext<sqlParser::IndexNameContext>(0);
}

sqlParser::TableNameContext* sqlParser::DropIndexQueryContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}


size_t sqlParser::DropIndexQueryContext::getRuleIndex() const {
  return sqlParser::RuleDropIndexQuery;
}


antlrcpp::Any sqlParser::DropIndexQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDropIndexQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DropIndexQueryContext* sqlParser::dropIndexQuery() {
  DropIndexQueryContext *_localctx = _tracker.createInstance<DropIndexQueryContext>(_ctx, getState());
  enterRule(_localctx, 72, sqlParser::RuleDropIndexQuery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(831);
    match(sqlParser::DROP);
    setState(832);
    match(sqlParser::INDEX);
    setState(840);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 88, _ctx)) {
    case 1: {
      setState(833);
      dynamic_cast<DropIndexQueryContext *>(_localctx)->idxName = indexName();
      setState(834);
      match(sqlParser::ON);
      setState(835);
      dynamic_cast<DropIndexQueryContext *>(_localctx)->tblName = tableName();
      break;
    }

    case 2: {
      setState(837);
      dynamic_cast<DropIndexQueryContext *>(_localctx)->tblName = tableName();
      setState(838);
      dynamic_cast<DropIndexQueryContext *>(_localctx)->idxName = indexName();
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

//----------------- TruncateQueryContext ------------------------------------------------------------------

sqlParser::TruncateQueryContext::TruncateQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::TruncateQueryContext::TRUNCATE() {
  return getToken(sqlParser::TRUNCATE, 0);
}

sqlParser::TableNameContext* sqlParser::TruncateQueryContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

tree::TerminalNode* sqlParser::TruncateQueryContext::TABLE() {
  return getToken(sqlParser::TABLE, 0);
}


size_t sqlParser::TruncateQueryContext::getRuleIndex() const {
  return sqlParser::RuleTruncateQuery;
}


antlrcpp::Any sqlParser::TruncateQueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTruncateQuery(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TruncateQueryContext* sqlParser::truncateQuery() {
  TruncateQueryContext *_localctx = _tracker.createInstance<TruncateQueryContext>(_ctx, getState());
  enterRule(_localctx, 74, sqlParser::RuleTruncateQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(842);
    match(sqlParser::TRUNCATE);
    setState(844);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::TABLE) {
      setState(843);
      match(sqlParser::TABLE);
    }
    setState(846);
    dynamic_cast<TruncateQueryContext *>(_localctx)->name = tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LoadContext ------------------------------------------------------------------

sqlParser::LoadContext::LoadContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LoadContext::LOAD() {
  return getToken(sqlParser::LOAD, 0);
}

sqlParser::TableNameContext* sqlParser::LoadContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::StringLiteralContext* sqlParser::LoadContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}

std::vector<sqlParser::LoadOptionContext *> sqlParser::LoadContext::loadOption() {
  return getRuleContexts<sqlParser::LoadOptionContext>();
}

sqlParser::LoadOptionContext* sqlParser::LoadContext::loadOption(size_t i) {
  return getRuleContext<sqlParser::LoadOptionContext>(i);
}


size_t sqlParser::LoadContext::getRuleIndex() const {
  return sqlParser::RuleLoad;
}


antlrcpp::Any sqlParser::LoadContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLoad(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LoadContext* sqlParser::load() {
  LoadContext *_localctx = _tracker.createInstance<LoadContext>(_ctx, getState());
  enterRule(_localctx, 76, sqlParser::RuleLoad);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(848);
    match(sqlParser::LOAD);
    setState(849);
    tableName();
    setState(850);
    dynamic_cast<LoadContext *>(_localctx)->filePath = stringLiteral();
    setState(861);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__1

    || _la == sqlParser::DATE_FORMAT || _la == sqlParser::ESCAPE_CHAR

    || _la == sqlParser::FIELD_DELIMITER) {
      setState(851);
      loadOption();
      setState(858);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__1)
        | (1ULL << sqlParser::T__5)
        | (1ULL << sqlParser::DATE_FORMAT))) != 0) || _la == sqlParser::ESCAPE_CHAR

      || _la == sqlParser::FIELD_DELIMITER) {
        setState(853);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(852);
          match(sqlParser::T__5);
        }
        setState(855);
        loadOption();
        setState(860);
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

//----------------- DloadContext ------------------------------------------------------------------

sqlParser::DloadContext::DloadContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::DloadContext::DLOAD() {
  return getToken(sqlParser::DLOAD, 0);
}

sqlParser::TableNameContext* sqlParser::DloadContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

tree::TerminalNode* sqlParser::DloadContext::PATHS() {
  return getToken(sqlParser::PATHS, 0);
}

std::vector<sqlParser::StringLiteralContext *> sqlParser::DloadContext::stringLiteral() {
  return getRuleContexts<sqlParser::StringLiteralContext>();
}

sqlParser::StringLiteralContext* sqlParser::DloadContext::stringLiteral(size_t i) {
  return getRuleContext<sqlParser::StringLiteralContext>(i);
}

std::vector<sqlParser::LoadOptionContext *> sqlParser::DloadContext::loadOption() {
  return getRuleContexts<sqlParser::LoadOptionContext>();
}

sqlParser::LoadOptionContext* sqlParser::DloadContext::loadOption(size_t i) {
  return getRuleContext<sqlParser::LoadOptionContext>(i);
}


size_t sqlParser::DloadContext::getRuleIndex() const {
  return sqlParser::RuleDload;
}


antlrcpp::Any sqlParser::DloadContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDload(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::DloadContext* sqlParser::dload() {
  DloadContext *_localctx = _tracker.createInstance<DloadContext>(_ctx, getState());
  enterRule(_localctx, 78, sqlParser::RuleDload);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(863);
    match(sqlParser::DLOAD);
    setState(864);
    tableName();
    setState(865);
    match(sqlParser::PATHS);
    setState(867); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(866);
      stringLiteral();
      setState(869); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (((((_la - 224) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 224)) & ((1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 224))
      | (1ULL << (sqlParser::SINGLQUOTED_STRING - 224))
      | (1ULL << (sqlParser::DUBLQUOTED_STRING - 224)))) != 0));
    setState(881);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__1

    || _la == sqlParser::DATE_FORMAT || _la == sqlParser::ESCAPE_CHAR

    || _la == sqlParser::FIELD_DELIMITER) {
      setState(871);
      loadOption();
      setState(878);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__1)
        | (1ULL << sqlParser::T__5)
        | (1ULL << sqlParser::DATE_FORMAT))) != 0) || _la == sqlParser::ESCAPE_CHAR

      || _la == sqlParser::FIELD_DELIMITER) {
        setState(873);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(872);
          match(sqlParser::T__5);
        }
        setState(875);
        loadOption();
        setState(880);
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
  enterRule(_localctx, 80, sqlParser::RuleLoadOption);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(888);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__1: {
        setState(883);
        match(sqlParser::T__1);
        setState(884);
        loadOptionKeys();
        setState(885);
        match(sqlParser::T__2);
        break;
      }

      case sqlParser::DATE_FORMAT:
      case sqlParser::ESCAPE_CHAR:
      case sqlParser::FIELD_DELIMITER: {
        setState(887);
        loadOptionKeys();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(890);
    match(sqlParser::T__3);
    setState(891);
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
  enterRule(_localctx, 82, sqlParser::RuleLoadOptionKeys);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(893);
    _la = _input->LA(1);
    if (!(((((_la - 60) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 60)) & ((1ULL << (sqlParser::DATE_FORMAT - 60))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 60))
      | (1ULL << (sqlParser::FIELD_DELIMITER - 60)))) != 0))) {
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

//----------------- CountContext ------------------------------------------------------------------

sqlParser::CountContext::CountContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::CountContext::COUNT() {
  return getToken(sqlParser::COUNT, 0);
}

sqlParser::TableNameContext* sqlParser::CountContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}


size_t sqlParser::CountContext::getRuleIndex() const {
  return sqlParser::RuleCount;
}


antlrcpp::Any sqlParser::CountContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitCount(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::CountContext* sqlParser::count() {
  CountContext *_localctx = _tracker.createInstance<CountContext>(_ctx, getState());
  enterRule(_localctx, 84, sqlParser::RuleCount);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(895);
    match(sqlParser::COUNT);
    setState(896);
    tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SleepContext ------------------------------------------------------------------

sqlParser::SleepContext::SleepContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SleepContext::SLEEP() {
  return getToken(sqlParser::SLEEP, 0);
}

sqlParser::IntegerLiteralContext* sqlParser::SleepContext::integerLiteral() {
  return getRuleContext<sqlParser::IntegerLiteralContext>(0);
}

sqlParser::SleepUnitsContext* sqlParser::SleepContext::sleepUnits() {
  return getRuleContext<sqlParser::SleepUnitsContext>(0);
}


size_t sqlParser::SleepContext::getRuleIndex() const {
  return sqlParser::RuleSleep;
}


antlrcpp::Any sqlParser::SleepContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSleep(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SleepContext* sqlParser::sleep() {
  SleepContext *_localctx = _tracker.createInstance<SleepContext>(_ctx, getState());
  enterRule(_localctx, 86, sqlParser::RuleSleep);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(898);
    match(sqlParser::SLEEP);
    setState(899);
    integerLiteral();
    setState(901);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 124) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 124)) & ((1ULL << (sqlParser::MILLIS - 124))
      | (1ULL << (sqlParser::MILLISECOND - 124))
      | (1ULL << (sqlParser::MILLISECONDS - 124))
      | (1ULL << (sqlParser::MS - 124))
      | (1ULL << (sqlParser::SEC - 124))
      | (1ULL << (sqlParser::SECOND - 124))
      | (1ULL << (sqlParser::SECONDS - 124))
      | (1ULL << (sqlParser::S_ONLY - 124)))) != 0)) {
      setState(900);
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
  enterRule(_localctx, 88, sqlParser::RuleSleepUnits);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(905);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::SEC:
      case sqlParser::SECOND:
      case sqlParser::SECONDS:
      case sqlParser::S_ONLY: {
        enterOuterAlt(_localctx, 1);
        setState(903);
        dynamic_cast<SleepUnitsContext *>(_localctx)->seconds = _input->LT(1);
        _la = _input->LA(1);
        if (!(((((_la - 168) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 168)) & ((1ULL << (sqlParser::SEC - 168))
          | (1ULL << (sqlParser::SECOND - 168))
          | (1ULL << (sqlParser::SECONDS - 168))
          | (1ULL << (sqlParser::S_ONLY - 168)))) != 0))) {
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
        setState(904);
        dynamic_cast<SleepUnitsContext *>(_localctx)->millis = _input->LT(1);
        _la = _input->LA(1);
        if (!(((((_la - 124) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 124)) & ((1ULL << (sqlParser::MILLIS - 124))
          | (1ULL << (sqlParser::MILLISECOND - 124))
          | (1ULL << (sqlParser::MILLISECONDS - 124))
          | (1ULL << (sqlParser::MS - 124)))) != 0))) {
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

//----------------- SpaceContext ------------------------------------------------------------------

sqlParser::SpaceContext::SpaceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SpaceContext::SPACE() {
  return getToken(sqlParser::SPACE, 0);
}


size_t sqlParser::SpaceContext::getRuleIndex() const {
  return sqlParser::RuleSpace;
}


antlrcpp::Any sqlParser::SpaceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSpace(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SpaceContext* sqlParser::space() {
  SpaceContext *_localctx = _tracker.createInstance<SpaceContext>(_ctx, getState());
  enterRule(_localctx, 90, sqlParser::RuleSpace);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(907);
    match(sqlParser::SPACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatusContext ------------------------------------------------------------------

sqlParser::StatusContext::StatusContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::StatusContext::STATUS() {
  return getToken(sqlParser::STATUS, 0);
}


size_t sqlParser::StatusContext::getRuleIndex() const {
  return sqlParser::RuleStatus;
}


antlrcpp::Any sqlParser::StatusContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitStatus(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::StatusContext* sqlParser::status() {
  StatusContext *_localctx = _tracker.createInstance<StatusContext>(_ctx, getState());
  enterRule(_localctx, 92, sqlParser::RuleStatus);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(909);
    match(sqlParser::STATUS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SzidxContext ------------------------------------------------------------------

sqlParser::SzidxContext::SzidxContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SzidxContext::SZIDX() {
  return getToken(sqlParser::SZIDX, 0);
}

sqlParser::TableNameContext* sqlParser::SzidxContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::IndexNameContext* sqlParser::SzidxContext::indexName() {
  return getRuleContext<sqlParser::IndexNameContext>(0);
}


size_t sqlParser::SzidxContext::getRuleIndex() const {
  return sqlParser::RuleSzidx;
}


antlrcpp::Any sqlParser::SzidxContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSzidx(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SzidxContext* sqlParser::szidx() {
  SzidxContext *_localctx = _tracker.createInstance<SzidxContext>(_ctx, getState());
  enterRule(_localctx, 94, sqlParser::RuleSzidx);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(911);
    match(sqlParser::SZIDX);
    setState(912);
    tableName();
    setState(913);
    indexName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SztblContext ------------------------------------------------------------------

sqlParser::SztblContext::SztblContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::SztblContext::SZTBL() {
  return getToken(sqlParser::SZTBL, 0);
}

sqlParser::TableNameContext* sqlParser::SztblContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}


size_t sqlParser::SztblContext::getRuleIndex() const {
  return sqlParser::RuleSztbl;
}


antlrcpp::Any sqlParser::SztblContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSztbl(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::SztblContext* sqlParser::sztbl() {
  SztblContext *_localctx = _tracker.createInstance<SztblContext>(_ctx, getState());
  enterRule(_localctx, 96, sqlParser::RuleSztbl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(915);
    match(sqlParser::SZTBL);
    setState(916);
    tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TrackmemoryContext ------------------------------------------------------------------

sqlParser::TrackmemoryContext::TrackmemoryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::TrackmemoryContext::TRACKMEMORY() {
  return getToken(sqlParser::TRACKMEMORY, 0);
}

tree::TerminalNode* sqlParser::TrackmemoryContext::FORCE() {
  return getToken(sqlParser::FORCE, 0);
}


size_t sqlParser::TrackmemoryContext::getRuleIndex() const {
  return sqlParser::RuleTrackmemory;
}


antlrcpp::Any sqlParser::TrackmemoryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTrackmemory(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TrackmemoryContext* sqlParser::trackmemory() {
  TrackmemoryContext *_localctx = _tracker.createInstance<TrackmemoryContext>(_ctx, getState());
  enterRule(_localctx, 98, sqlParser::RuleTrackmemory);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(918);
    match(sqlParser::TRACKMEMORY);
    setState(920);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::FORCE) {
      setState(919);
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

//----------------- MemoryleaksContext ------------------------------------------------------------------

sqlParser::MemoryleaksContext::MemoryleaksContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::MemoryleaksContext::MEMORYLEAKS() {
  return getToken(sqlParser::MEMORYLEAKS, 0);
}


size_t sqlParser::MemoryleaksContext::getRuleIndex() const {
  return sqlParser::RuleMemoryleaks;
}


antlrcpp::Any sqlParser::MemoryleaksContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitMemoryleaks(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::MemoryleaksContext* sqlParser::memoryleaks() {
  MemoryleaksContext *_localctx = _tracker.createInstance<MemoryleaksContext>(_ctx, getState());
  enterRule(_localctx, 100, sqlParser::RuleMemoryleaks);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(922);
    match(sqlParser::MEMORYLEAKS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BpaContext ------------------------------------------------------------------

sqlParser::BpaContext::BpaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::BpaContext::BPA() {
  return getToken(sqlParser::BPA, 0);
}

tree::TerminalNode* sqlParser::BpaContext::REGIONS() {
  return getToken(sqlParser::REGIONS, 0);
}

tree::TerminalNode* sqlParser::BpaContext::OBJECTS() {
  return getToken(sqlParser::OBJECTS, 0);
}

tree::TerminalNode* sqlParser::BpaContext::ALL() {
  return getToken(sqlParser::ALL, 0);
}


size_t sqlParser::BpaContext::getRuleIndex() const {
  return sqlParser::RuleBpa;
}


antlrcpp::Any sqlParser::BpaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitBpa(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::BpaContext* sqlParser::bpa() {
  BpaContext *_localctx = _tracker.createInstance<BpaContext>(_ctx, getState());
  enterRule(_localctx, 102, sqlParser::RuleBpa);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(924);
    match(sqlParser::BPA);
    setState(926);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ALL || _la == sqlParser::OBJECTS

    || _la == sqlParser::REGIONS) {
      setState(925);
      dynamic_cast<BpaContext *>(_localctx)->type = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == sqlParser::ALL || _la == sqlParser::OBJECTS

      || _la == sqlParser::REGIONS)) {
        dynamic_cast<BpaContext *>(_localctx)->type = _errHandler->recoverInline(this);
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

//----------------- FileContext ------------------------------------------------------------------

sqlParser::FileContext::FileContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::FileContext::FILE() {
  return getToken(sqlParser::FILE, 0);
}

sqlParser::IdentifierContext* sqlParser::FileContext::identifier() {
  return getRuleContext<sqlParser::IdentifierContext>(0);
}

sqlParser::StringLiteralContext* sqlParser::FileContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}


size_t sqlParser::FileContext::getRuleIndex() const {
  return sqlParser::RuleFile;
}


antlrcpp::Any sqlParser::FileContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitFile(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::FileContext* sqlParser::file() {
  FileContext *_localctx = _tracker.createInstance<FileContext>(_ctx, getState());
  enterRule(_localctx, 104, sqlParser::RuleFile);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(928);
    match(sqlParser::FILE);
    setState(931);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 102, _ctx)) {
    case 1: {
      setState(929);
      identifier();
      break;
    }

    case 2: {
      setState(930);
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

//----------------- NoopContext ------------------------------------------------------------------

sqlParser::NoopContext::NoopContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::NoopContext::NOOP() {
  return getToken(sqlParser::NOOP, 0);
}

tree::TerminalNode* sqlParser::NoopContext::NOP() {
  return getToken(sqlParser::NOP, 0);
}


size_t sqlParser::NoopContext::getRuleIndex() const {
  return sqlParser::RuleNoop;
}


antlrcpp::Any sqlParser::NoopContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitNoop(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::NoopContext* sqlParser::noop() {
  NoopContext *_localctx = _tracker.createInstance<NoopContext>(_ctx, getState());
  enterRule(_localctx, 106, sqlParser::RuleNoop);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(933);
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

//----------------- EmptyCommandContext ------------------------------------------------------------------

sqlParser::EmptyCommandContext::EmptyCommandContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t sqlParser::EmptyCommandContext::getRuleIndex() const {
  return sqlParser::RuleEmptyCommand;
}


antlrcpp::Any sqlParser::EmptyCommandContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitEmptyCommand(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::EmptyCommandContext* sqlParser::emptyCommand() {
  EmptyCommandContext *_localctx = _tracker.createInstance<EmptyCommandContext>(_ctx, getState());
  enterRule(_localctx, 108, sqlParser::RuleEmptyCommand);

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
  enterRule(_localctx, 110, sqlParser::RuleTableDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(937);
    match(sqlParser::T__8);
    setState(940);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::ASC:
      case sqlParser::BIT:
      case sqlParser::BITS:
      case sqlParser::BLOCK:
      case sqlParser::BLOOM:
      case sqlParser::BPA:
      case sqlParser::BTREE:
      case sqlParser::BYTE:
      case sqlParser::BYTES:
      case sqlParser::CATALOG:
      case sqlParser::CHARS:
      case sqlParser::CONCAT:
      case sqlParser::DATA:
      case sqlParser::DATE_FORMAT:
      case sqlParser::DAYS:
      case sqlParser::DECODE:
      case sqlParser::DESC:
      case sqlParser::DETAIL:
      case sqlParser::DIGITS:
      case sqlParser::DLOAD:
      case sqlParser::ECHO:
      case sqlParser::ESCAPE_CHAR:
      case sqlParser::EXPLAIN:
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
      case sqlParser::KEY:
      case sqlParser::LIMIT:
      case sqlParser::LOAD:
      case sqlParser::LOG:
      case sqlParser::LOOP:
      case sqlParser::LSIDX:
      case sqlParser::LSTBL:
      case sqlParser::MEMORYLEAKS:
      case sqlParser::MILLIS:
      case sqlParser::MILLISECOND:
      case sqlParser::MILLISECONDS:
      case sqlParser::MINUTES:
      case sqlParser::MONOTONIC:
      case sqlParser::MONTHS:
      case sqlParser::MS:
      case sqlParser::NODE:
      case sqlParser::NOOP:
      case sqlParser::NOP:
      case sqlParser::NOW:
      case sqlParser::OBJECTS:
      case sqlParser::OFF:
      case sqlParser::OFFSET:
      case sqlParser::PATHS:
      case sqlParser::PLACING:
      case sqlParser::PRETTY:
      case sqlParser::QUARTER:
      case sqlParser::QUARTERS:
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
        setState(938);
        columnDefinition();
        break;
      }

      case sqlParser::T__5:
      case sqlParser::T__9: {
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(949);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(942);
      match(sqlParser::T__5);
      setState(945);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ASC:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::CONCAT:
        case sqlParser::DATA:
        case sqlParser::DATE_FORMAT:
        case sqlParser::DAYS:
        case sqlParser::DECODE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DIGITS:
        case sqlParser::DLOAD:
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXPLAIN:
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
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSTBL:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MILLIS:
        case sqlParser::MILLISECOND:
        case sqlParser::MILLISECONDS:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::MS:
        case sqlParser::NODE:
        case sqlParser::NOOP:
        case sqlParser::NOP:
        case sqlParser::NOW:
        case sqlParser::OBJECTS:
        case sqlParser::OFF:
        case sqlParser::OFFSET:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
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
          setState(943);
          columnDefinition();
          break;
        }

        case sqlParser::T__5:
        case sqlParser::T__9: {
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(951);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(952);
    match(sqlParser::T__9);
    setState(961);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 152) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 152)) & ((1ULL << (sqlParser::PARTITION - 152))
      | (1ULL << (sqlParser::REPLICATED - 152))
      | (1ULL << (sqlParser::ROWSIZE - 152))
      | (1ULL << (sqlParser::TABLESIZE - 152)))) != 0)) {
      setState(959);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::PARTITION: {
          setState(953);
          tablePartitioning();
          break;
        }

        case sqlParser::REPLICATED: {
          setState(954);
          match(sqlParser::REPLICATED);
          break;
        }

        case sqlParser::ROWSIZE: {
          setState(955);
          match(sqlParser::ROWSIZE);
          setState(956);
          integerLiteral();
          break;
        }

        case sqlParser::TABLESIZE: {
          setState(957);
          match(sqlParser::TABLESIZE);
          setState(958);
          integerLiteral();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(963);
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
  enterRule(_localctx, 112, sqlParser::RuleTablePartitioning);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(964);
    match(sqlParser::PARTITION);
    setState(965);
    match(sqlParser::BY);
    setState(966);
    match(sqlParser::T__8);
    setState(967);
    expr(0);
    setState(972);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(968);
      match(sqlParser::T__5);
      setState(969);
      expr(0);
      setState(974);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(975);
    match(sqlParser::T__9);
   
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
  enterRule(_localctx, 114, sqlParser::RuleColumnDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(977);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->name = columnName();
    setState(978);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->type = dataType();
    setState(983);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::NOT) {
      setState(979);
      dynamic_cast<ColumnDefinitionContext *>(_localctx)->notNullable = match(sqlParser::NOT);
      setState(980);
      dynamic_cast<ColumnDefinitionContext *>(_localctx)->nullable = match(sqlParser::SQL_NULL);
      setState(985);
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
  enterRule(_localctx, 116, sqlParser::RuleColumnNameList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(986);
    match(sqlParser::T__8);
    setState(987);
    columnName();
    setState(992);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(988);
      match(sqlParser::T__5);
      setState(989);
      columnName();
      setState(994);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(995);
    match(sqlParser::T__9);
   
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
  enterRule(_localctx, 118, sqlParser::RuleIndexDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(998);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(997);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
    }
    setState(1000);
    _la = _input->LA(1);
    if (!(_la == sqlParser::INDEX

    || _la == sqlParser::KEY)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(1002);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 112, _ctx)) {
    case 1: {
      setState(1001);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->name = indexName();
      break;
    }

    }
    setState(1005);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ON) {
      setState(1004);
      match(sqlParser::ON);
    }
    setState(1019);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 115, _ctx)) {
    case 1: {
      setState(1007);
      expr(0);
      break;
    }

    case 2: {
      setState(1008);
      match(sqlParser::T__8);
      setState(1009);
      expr(0);
      setState(1014);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(1010);
        match(sqlParser::T__5);
        setState(1011);
        expr(0);
        setState(1016);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(1017);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(1023);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::USING) {
      setState(1021);
      match(sqlParser::USING);
      setState(1022);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->method = indexMethod();
    }
    setState(1027);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(1025);
      match(sqlParser::WITH);
      setState(1026);
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
  enterRule(_localctx, 120, sqlParser::RuleIndexMethod);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1029);
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
  enterRule(_localctx, 122, sqlParser::RuleDataType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1031);
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
  enterRule(_localctx, 124, sqlParser::RuleSqlDataType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1096);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 129, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1033);
      match(sqlParser::BOOLEAN);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1034);
      match(sqlParser::INTEGER);
      setState(1042);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1035);
        match(sqlParser::T__8);
        setState(1036);
        precision();
        setState(1038);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 30) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 30)) & ((1ULL << (sqlParser::BITS - 30))
          | (1ULL << (sqlParser::BYTES - 30))
          | (1ULL << (sqlParser::DIGITS - 30)))) != 0)) {
          setState(1037);
          _la = _input->LA(1);
          if (!(((((_la - 30) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 30)) & ((1ULL << (sqlParser::BITS - 30))
            | (1ULL << (sqlParser::BYTES - 30))
            | (1ULL << (sqlParser::DIGITS - 30)))) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        setState(1040);
        match(sqlParser::T__9);
      }
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1044);
      match(sqlParser::DECIMAL);
      setState(1059);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1045);
        match(sqlParser::T__8);
        setState(1046);
        precision();
        setState(1048);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::DIGITS) {
          setState(1047);
          match(sqlParser::DIGITS);
        }
        setState(1055);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1050);
          match(sqlParser::T__5);
          setState(1051);
          scale();
          setState(1053);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::DIGITS) {
            setState(1052);
            match(sqlParser::DIGITS);
          }
        }
        setState(1057);
        match(sqlParser::T__9);
      }
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(1061);
      match(sqlParser::FLOAT);
      setState(1069);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1062);
        match(sqlParser::T__8);
        setState(1063);
        precision();
        setState(1065);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 30) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 30)) & ((1ULL << (sqlParser::BITS - 30))
          | (1ULL << (sqlParser::BYTES - 30))
          | (1ULL << (sqlParser::DIGITS - 30)))) != 0)) {
          setState(1064);
          _la = _input->LA(1);
          if (!(((((_la - 30) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 30)) & ((1ULL << (sqlParser::BITS - 30))
            | (1ULL << (sqlParser::BYTES - 30))
            | (1ULL << (sqlParser::DIGITS - 30)))) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        setState(1067);
        match(sqlParser::T__9);
      }
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1071);
      match(sqlParser::DATE);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1072);
      match(sqlParser::TIME);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(1073);
      match(sqlParser::TIMESTAMP);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(1074);
      intervalType();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(1075);
      fixedLenChar();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(1076);
      varyingLenChar();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(1077);
      match(sqlParser::BINARY);

      setState(1078);
      match(sqlParser::T__8);
      setState(1079);
      size();
      setState(1081);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::BYTE

      || _la == sqlParser::BYTES) {
        setState(1080);
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
      setState(1083);
      match(sqlParser::T__9);
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(1085);
      match(sqlParser::VARBINARY);
      setState(1093);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1086);
        match(sqlParser::T__8);
        setState(1087);
        size();
        setState(1089);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::BYTE

        || _la == sqlParser::BYTES) {
          setState(1088);
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
        setState(1091);
        match(sqlParser::T__9);
      }
      break;
    }

    case 13: {
      enterOuterAlt(_localctx, 13);
      setState(1095);
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
  enterRule(_localctx, 126, sqlParser::RuleIntervalType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1098);
    match(sqlParser::INTERVAL);
    setState(1101);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::MONTH:
      case sqlParser::YEAR: {
        setState(1099);
        yearMonthIntervalType();
        break;
      }

      case sqlParser::DAY:
      case sqlParser::HOUR:
      case sqlParser::MINUTE:
      case sqlParser::SECOND: {
        setState(1100);
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
  enterRule(_localctx, 128, sqlParser::RuleYearMonthIntervalType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1126);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 134, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1103);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1108);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1104);
        match(sqlParser::T__8);
        setState(1105);
        precision();
        setState(1106);
        match(sqlParser::T__9);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1110);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1115);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1111);
        match(sqlParser::T__8);
        setState(1112);
        precision();
        setState(1113);
        match(sqlParser::T__9);
      }
      setState(1117);
      match(sqlParser::TO);
      setState(1118);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->month = match(sqlParser::MONTH);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1119);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->month = match(sqlParser::MONTH);
      setState(1124);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1120);
        match(sqlParser::T__8);
        setState(1121);
        precision();
        setState(1122);
        match(sqlParser::T__9);
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
  enterRule(_localctx, 130, sqlParser::RuleDayTimeIntervalType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1212);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 148, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1128);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1133);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1129);
        match(sqlParser::T__8);
        setState(1130);
        precision();
        setState(1131);
        match(sqlParser::T__9);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1135);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1140);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1136);
        match(sqlParser::T__8);
        setState(1137);
        precision();
        setState(1138);
        match(sqlParser::T__9);
      }
      setState(1142);
      match(sqlParser::TO);
      setState(1152);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::HOUR: {
          setState(1143);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->hour = match(sqlParser::HOUR);
          break;
        }

        case sqlParser::MINUTE: {
          setState(1144);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1145);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1150);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__8) {
            setState(1146);
            match(sqlParser::T__8);
            setState(1147);
            scale();
            setState(1148);
            match(sqlParser::T__9);
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
      setState(1154);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1159);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1155);
        match(sqlParser::T__8);
        setState(1156);
        precision();
        setState(1157);
        match(sqlParser::T__9);
      }
      setState(1161);
      match(sqlParser::TO);
      setState(1170);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::MINUTE: {
          setState(1162);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1163);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1168);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__8) {
            setState(1164);
            match(sqlParser::T__8);
            setState(1165);
            scale();
            setState(1166);
            match(sqlParser::T__9);
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
      setState(1172);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1177);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1173);
        match(sqlParser::T__8);
        setState(1174);
        precision();
        setState(1175);
        match(sqlParser::T__9);
      }
      setState(1179);
      match(sqlParser::TO);

      setState(1180);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1185);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1181);
        match(sqlParser::T__8);
        setState(1182);
        scale();
        setState(1183);
        match(sqlParser::T__9);
      }
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1187);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1192);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1188);
        match(sqlParser::T__8);
        setState(1189);
        precision();
        setState(1190);
        match(sqlParser::T__9);
      }
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1194);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1199);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1195);
        match(sqlParser::T__8);
        setState(1196);
        precision();
        setState(1197);
        match(sqlParser::T__9);
      }
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(1201);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1210);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1202);
        match(sqlParser::T__8);
        setState(1203);
        precision();
        setState(1206);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1204);
          match(sqlParser::T__5);
          setState(1205);
          scale();
        }
        setState(1208);
        match(sqlParser::T__9);
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
  enterRule(_localctx, 132, sqlParser::RuleFixedLenChar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1214);
    _la = _input->LA(1);
    if (!(_la == sqlParser::CHAR

    || _la == sqlParser::CHARACTER)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(1215);
    match(sqlParser::T__8);
    setState(1216);
    size();
    setState(1218);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::BYTE)
      | (1ULL << sqlParser::BYTES)
      | (1ULL << sqlParser::CHAR)
      | (1ULL << sqlParser::CHARS))) != 0)) {
      setState(1217);
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
    setState(1220);
    match(sqlParser::T__9);
    setState(1223);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::COLLATE) {
      setState(1221);
      match(sqlParser::COLLATE);
      setState(1222);
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
  enterRule(_localctx, 134, sqlParser::RuleVaryingLenChar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1242);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::CHAR:
      case sqlParser::CHARACTER:
      case sqlParser::VARCHAR: {
        setState(1230);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::CHAR:
          case sqlParser::CHARACTER: {
            setState(1225);
            _la = _input->LA(1);
            if (!(_la == sqlParser::CHAR

            || _la == sqlParser::CHARACTER)) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
            setState(1227);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == sqlParser::VARYING) {
              setState(1226);
              match(sqlParser::VARYING);
            }
            break;
          }

          case sqlParser::VARCHAR: {
            setState(1229);
            match(sqlParser::VARCHAR);
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(1239);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__8) {
          setState(1232);
          match(sqlParser::T__8);
          setState(1233);
          size();
          setState(1235);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::BYTE)
            | (1ULL << sqlParser::BYTES)
            | (1ULL << sqlParser::CHAR)
            | (1ULL << sqlParser::CHARS))) != 0)) {
            setState(1234);
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
          setState(1237);
          match(sqlParser::T__9);
        }
        break;
      }

      case sqlParser::TEXT: {
        setState(1241);
        match(sqlParser::TEXT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(1246);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::COLLATE) {
      setState(1244);
      match(sqlParser::COLLATE);
      setState(1245);
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
  enterRule(_localctx, 136, sqlParser::RuleAsSubquery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1248);
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
  enterRule(_localctx, 138, sqlParser::RuleSelectOrValues);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    setState(1271);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 160, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1250);
      match(sqlParser::T__8);
      setState(1254);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 157, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(1251);
          queryHint(); 
        }
        setState(1256);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 157, _ctx);
      }
      setState(1257);
      selectOrValues();
      setState(1258);
      match(sqlParser::T__9);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1263);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::HINT_START) {
        setState(1260);
        queryHint();
        setState(1265);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(1269);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 159, _ctx)) {
      case 1: {
        setState(1266);
        selectQuery();
        break;
      }

      case 2: {
        setState(1267);
        valuesExpr();
        break;
      }

      case 3: {
        setState(1268);
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
  enterRule(_localctx, 140, sqlParser::RuleSelectList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1273);
    selectListItem();
    setState(1278);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 161, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1274);
        match(sqlParser::T__5);
        setState(1275);
        selectListItem(); 
      }
      setState(1280);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 161, _ctx);
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
  enterRule(_localctx, 142, sqlParser::RuleSelectListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1289);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 164, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1281);
      expr(0);
      setState(1286);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 163, _ctx)) {
      case 1: {
        setState(1283);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(1282);
          match(sqlParser::AS);
        }
        setState(1285);
        dynamic_cast<SelectListItemContext *>(_localctx)->as = identifier();
        break;
      }

      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1288);
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
  enterRule(_localctx, 144, sqlParser::RuleGroupByList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1291);
    groupByListItem();
    setState(1296);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 165, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1292);
        match(sqlParser::T__5);
        setState(1293);
        groupByListItem(); 
      }
      setState(1298);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 165, _ctx);
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
  enterRule(_localctx, 146, sqlParser::RuleGroupByListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1299);
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
  enterRule(_localctx, 148, sqlParser::RuleOrderByList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1301);
    orderByListItem();
    setState(1306);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1302);
      match(sqlParser::T__5);
      setState(1303);
      orderByListItem();
      setState(1308);
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
  enterRule(_localctx, 150, sqlParser::RuleOrderByListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1309);
    expr(0);
    setState(1311);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ASC

    || _la == sqlParser::DESC) {
      setState(1310);
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
  enterRule(_localctx, 152, sqlParser::RuleJoinUsingList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1313);
    joinUsingListItem();
    setState(1318);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1314);
      match(sqlParser::T__5);
      setState(1315);
      joinUsingListItem();
      setState(1320);
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
  enterRule(_localctx, 154, sqlParser::RuleJoinUsingListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1321);
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
  enterRule(_localctx, 156, sqlParser::RuleCorrList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1323);
    corrListItem();
    setState(1328);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1324);
      match(sqlParser::T__5);
      setState(1325);
      corrListItem();
      setState(1330);
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
  enterRule(_localctx, 158, sqlParser::RuleCorrListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1331);
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
  enterRule(_localctx, 160, sqlParser::RuleColAliases);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1333);
    match(sqlParser::T__8);
    setState(1334);
    colAliasList();
    setState(1335);
    match(sqlParser::T__9);
   
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
  enterRule(_localctx, 162, sqlParser::RuleColAliasList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1337);
    colAliasListItem();
    setState(1342);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1338);
      match(sqlParser::T__5);
      setState(1339);
      colAliasListItem();
      setState(1344);
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
  enterRule(_localctx, 164, sqlParser::RuleColAliasListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1345);
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
  enterRule(_localctx, 166, sqlParser::RulePropertyList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1347);
    property();
    setState(1352);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1348);
      match(sqlParser::T__5);
      setState(1349);
      property();
      setState(1354);
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
  enterRule(_localctx, 168, sqlParser::RuleProperty);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1355);
    dynamic_cast<PropertyContext *>(_localctx)->name = propertyName();
    setState(1358);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__3) {
      setState(1356);
      match(sqlParser::T__3);
      setState(1357);
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
  enterRule(_localctx, 170, sqlParser::RuleValueList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1360);
    expr(0);
    setState(1365);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1361);
      match(sqlParser::T__5);
      setState(1362);
      expr(0);
      setState(1367);
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
  enterRule(_localctx, 172, sqlParser::RulePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1368);
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
  enterRule(_localctx, 174, sqlParser::RuleJoinPredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1370);
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
  enterRule(_localctx, 176, sqlParser::RuleCasePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1372);
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
  size_t startState = 178;
  enterRecursionRule(_localctx, 178, sqlParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1637);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 201, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<LiteralValContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(1375);
      literal();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<VarRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1376);
      variableName();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ColRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1377);
      columnRef();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<InlineCastContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1378);
      match(sqlParser::T__8);
      setState(1379);
      dataType();
      setState(1380);
      match(sqlParser::T__9);
      setState(1381);
      expr(40);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<UnaryOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1383);
      dynamic_cast<UnaryOpContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__6

      || _la == sqlParser::T__12)) {
        dynamic_cast<UnaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1384);
      expr(39);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<CastOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1385);
      match(sqlParser::CAST);
      setState(1386);
      match(sqlParser::T__8);
      setState(1387);
      expr(0);
      setState(1388);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5

      || _la == sqlParser::AS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1389);
      dataType();
      setState(1390);
      match(sqlParser::T__9);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<TypeOfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1392);
      match(sqlParser::TYPEOF);
      setState(1393);
      match(sqlParser::T__8);
      setState(1394);
      expr(0);
      setState(1395);
      match(sqlParser::T__9);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<CaseOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1397);
      match(sqlParser::CASE);
      setState(1417);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::T__6:
        case sqlParser::T__8:
        case sqlParser::T__12:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::COALESCE:
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
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXPLAIN:
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
        case sqlParser::INTERVAL:
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
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
        case sqlParser::OVERLAY:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
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
          setState(1398);
          dynamic_cast<CaseOpContext *>(_localctx)->caseExpr = expr(0);
          setState(1404); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1399);
            match(sqlParser::WHEN);
            setState(1400);
            expr(0);
            setState(1401);
            match(sqlParser::THEN);
            setState(1402);
            expr(0);
            setState(1406); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

        case sqlParser::WHEN: {
          setState(1413); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1408);
            match(sqlParser::WHEN);
            setState(1409);
            casePredicate();
            setState(1410);
            match(sqlParser::THEN);
            setState(1411);
            expr(0);
            setState(1415); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(1421);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ELSE) {
        setState(1419);
        match(sqlParser::ELSE);
        setState(1420);
        dynamic_cast<CaseOpContext *>(_localctx)->elseExpr = expr(0);
      }
      setState(1423);
      match(sqlParser::END);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<IfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1425);
      match(sqlParser::IF);
      setState(1426);
      match(sqlParser::T__8);
      setState(1427);
      casePredicate();
      setState(1428);
      match(sqlParser::T__5);
      setState(1429);
      expr(0);
      setState(1430);
      match(sqlParser::T__5);
      setState(1431);
      expr(0);
      setState(1432);
      match(sqlParser::T__9);
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<IfNullOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1434);
      match(sqlParser::IFNULL);
      setState(1435);
      match(sqlParser::T__8);
      setState(1436);
      expr(0);
      setState(1437);
      match(sqlParser::T__5);
      setState(1438);
      expr(0);
      setState(1439);
      match(sqlParser::T__9);
      break;
    }

    case 11: {
      _localctx = _tracker.createInstance<NullIfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1441);
      match(sqlParser::NULLIF);
      setState(1442);
      match(sqlParser::T__8);
      setState(1443);
      expr(0);
      setState(1444);
      match(sqlParser::T__5);
      setState(1445);
      expr(0);
      setState(1446);
      match(sqlParser::T__9);
      break;
    }

    case 12: {
      _localctx = _tracker.createInstance<CoalesceOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1448);
      match(sqlParser::COALESCE);
      setState(1449);
      match(sqlParser::T__8);
      setState(1458);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__6)
        | (1ULL << sqlParser::T__8)
        | (1ULL << sqlParser::T__12)
        | (1ULL << sqlParser::ASC)
        | (1ULL << sqlParser::AVG)
        | (1ULL << sqlParser::BIT)
        | (1ULL << sqlParser::BITS)
        | (1ULL << sqlParser::BLOCK)
        | (1ULL << sqlParser::BLOOM)
        | (1ULL << sqlParser::BPA)
        | (1ULL << sqlParser::BTREE)
        | (1ULL << sqlParser::BYTE)
        | (1ULL << sqlParser::BYTES)
        | (1ULL << sqlParser::CASE)
        | (1ULL << sqlParser::CAST)
        | (1ULL << sqlParser::CATALOG)
        | (1ULL << sqlParser::CHARS)
        | (1ULL << sqlParser::COALESCE)
        | (1ULL << sqlParser::CONCAT)
        | (1ULL << sqlParser::COUNT)
        | (1ULL << sqlParser::CURRENT_CATALOG)
        | (1ULL << sqlParser::CURRENT_SCHEMA)
        | (1ULL << sqlParser::CURRENT_TIMESTAMP)
        | (1ULL << sqlParser::CURRENT_USER)
        | (1ULL << sqlParser::DATA)
        | (1ULL << sqlParser::DATE)
        | (1ULL << sqlParser::DATE_FORMAT)
        | (1ULL << sqlParser::DAYS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::DECODE - 64))
        | (1ULL << (sqlParser::DESC - 64))
        | (1ULL << (sqlParser::DETAIL - 64))
        | (1ULL << (sqlParser::DIGITS - 64))
        | (1ULL << (sqlParser::DLOAD - 64))
        | (1ULL << (sqlParser::ECHO - 64))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
        | (1ULL << (sqlParser::EXISTS - 64))
        | (1ULL << (sqlParser::EXPLAIN - 64))
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
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::KEY - 64))
        | (1ULL << (sqlParser::LIMIT - 64))
        | (1ULL << (sqlParser::LOAD - 64))
        | (1ULL << (sqlParser::LOG - 64))
        | (1ULL << (sqlParser::LOOP - 64))
        | (1ULL << (sqlParser::LSIDX - 64))
        | (1ULL << (sqlParser::LSTBL - 64))
        | (1ULL << (sqlParser::MAX - 64))
        | (1ULL << (sqlParser::MEMORYLEAKS - 64))
        | (1ULL << (sqlParser::MILLIS - 64))
        | (1ULL << (sqlParser::MILLISECOND - 64))
        | (1ULL << (sqlParser::MILLISECONDS - 64))
        | (1ULL << (sqlParser::MIN - 64)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::MINUTES - 129))
        | (1ULL << (sqlParser::MONOTONIC - 129))
        | (1ULL << (sqlParser::MONTHS - 129))
        | (1ULL << (sqlParser::MS - 129))
        | (1ULL << (sqlParser::NODE - 129))
        | (1ULL << (sqlParser::NOOP - 129))
        | (1ULL << (sqlParser::NOP - 129))
        | (1ULL << (sqlParser::NOT - 129))
        | (1ULL << (sqlParser::NOW - 129))
        | (1ULL << (sqlParser::SQL_NULL - 129))
        | (1ULL << (sqlParser::NULLIF - 129))
        | (1ULL << (sqlParser::OBJECTS - 129))
        | (1ULL << (sqlParser::OFF - 129))
        | (1ULL << (sqlParser::OFFSET - 129))
        | (1ULL << (sqlParser::OVERLAY - 129))
        | (1ULL << (sqlParser::PATHS - 129))
        | (1ULL << (sqlParser::PLACING - 129))
        | (1ULL << (sqlParser::POSITION - 129))
        | (1ULL << (sqlParser::PRETTY - 129))
        | (1ULL << (sqlParser::QUARTER - 129))
        | (1ULL << (sqlParser::QUARTERS - 129))
        | (1ULL << (sqlParser::REFERENCE - 129))
        | (1ULL << (sqlParser::REGIONS - 129))
        | (1ULL << (sqlParser::REPLICATED - 129))
        | (1ULL << (sqlParser::ROWSIZE - 129))
        | (1ULL << (sqlParser::SCHEMA - 129))
        | (1ULL << (sqlParser::SEC - 129))
        | (1ULL << (sqlParser::SECONDS - 129))
        | (1ULL << (sqlParser::SKIPLIST - 129))
        | (1ULL << (sqlParser::SLEEP - 129))
        | (1ULL << (sqlParser::SPACE - 129))
        | (1ULL << (sqlParser::STAGE - 129))
        | (1ULL << (sqlParser::STATS - 129))
        | (1ULL << (sqlParser::STATISTICS - 129))
        | (1ULL << (sqlParser::STATUS - 129))
        | (1ULL << (sqlParser::SUBSTR - 129))
        | (1ULL << (sqlParser::SUBSTRING - 129))
        | (1ULL << (sqlParser::SUM - 129))
        | (1ULL << (sqlParser::SZIDX - 129))
        | (1ULL << (sqlParser::SZTBL - 129))
        | (1ULL << (sqlParser::S_ONLY - 129))
        | (1ULL << (sqlParser::TABLESIZE - 129))
        | (1ULL << (sqlParser::TEXT - 129))
        | (1ULL << (sqlParser::TIME - 129))
        | (1ULL << (sqlParser::TIMESTAMP - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::TOP - 194))
        | (1ULL << (sqlParser::TRACKMEMORY - 194))
        | (1ULL << (sqlParser::TREE - 194))
        | (1ULL << (sqlParser::TRIM - 194))
        | (1ULL << (sqlParser::TRUNCATE - 194))
        | (1ULL << (sqlParser::SQL_TRUE - 194))
        | (1ULL << (sqlParser::TYPE - 194))
        | (1ULL << (sqlParser::TYPEOF - 194))
        | (1ULL << (sqlParser::VALUE - 194))
        | (1ULL << (sqlParser::WEEK - 194))
        | (1ULL << (sqlParser::WEEKS - 194))
        | (1ULL << (sqlParser::WITH_NAMES - 194))
        | (1ULL << (sqlParser::WITH_TYPES - 194))
        | (1ULL << (sqlParser::XOR - 194))
        | (1ULL << (sqlParser::YEARS - 194))
        | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 194))
        | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 194))
        | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 194))
        | (1ULL << (sqlParser::SINGLQUOTED_STRING - 194))
        | (1ULL << (sqlParser::DUBLQUOTED_STRING - 194))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 194))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 194))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 194)))) != 0)) {
        setState(1450);
        expr(0);
        setState(1455);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1451);
          match(sqlParser::T__5);
          setState(1452);
          expr(0);
          setState(1457);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1460);
      match(sqlParser::T__9);
      break;
    }

    case 13: {
      _localctx = _tracker.createInstance<DecodeOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1461);
      match(sqlParser::DECODE);
      setState(1462);
      match(sqlParser::T__8);
      setState(1471);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__6)
        | (1ULL << sqlParser::T__8)
        | (1ULL << sqlParser::T__12)
        | (1ULL << sqlParser::ASC)
        | (1ULL << sqlParser::AVG)
        | (1ULL << sqlParser::BIT)
        | (1ULL << sqlParser::BITS)
        | (1ULL << sqlParser::BLOCK)
        | (1ULL << sqlParser::BLOOM)
        | (1ULL << sqlParser::BPA)
        | (1ULL << sqlParser::BTREE)
        | (1ULL << sqlParser::BYTE)
        | (1ULL << sqlParser::BYTES)
        | (1ULL << sqlParser::CASE)
        | (1ULL << sqlParser::CAST)
        | (1ULL << sqlParser::CATALOG)
        | (1ULL << sqlParser::CHARS)
        | (1ULL << sqlParser::COALESCE)
        | (1ULL << sqlParser::CONCAT)
        | (1ULL << sqlParser::COUNT)
        | (1ULL << sqlParser::CURRENT_CATALOG)
        | (1ULL << sqlParser::CURRENT_SCHEMA)
        | (1ULL << sqlParser::CURRENT_TIMESTAMP)
        | (1ULL << sqlParser::CURRENT_USER)
        | (1ULL << sqlParser::DATA)
        | (1ULL << sqlParser::DATE)
        | (1ULL << sqlParser::DATE_FORMAT)
        | (1ULL << sqlParser::DAYS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::DECODE - 64))
        | (1ULL << (sqlParser::DESC - 64))
        | (1ULL << (sqlParser::DETAIL - 64))
        | (1ULL << (sqlParser::DIGITS - 64))
        | (1ULL << (sqlParser::DLOAD - 64))
        | (1ULL << (sqlParser::ECHO - 64))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
        | (1ULL << (sqlParser::EXISTS - 64))
        | (1ULL << (sqlParser::EXPLAIN - 64))
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
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::KEY - 64))
        | (1ULL << (sqlParser::LIMIT - 64))
        | (1ULL << (sqlParser::LOAD - 64))
        | (1ULL << (sqlParser::LOG - 64))
        | (1ULL << (sqlParser::LOOP - 64))
        | (1ULL << (sqlParser::LSIDX - 64))
        | (1ULL << (sqlParser::LSTBL - 64))
        | (1ULL << (sqlParser::MAX - 64))
        | (1ULL << (sqlParser::MEMORYLEAKS - 64))
        | (1ULL << (sqlParser::MILLIS - 64))
        | (1ULL << (sqlParser::MILLISECOND - 64))
        | (1ULL << (sqlParser::MILLISECONDS - 64))
        | (1ULL << (sqlParser::MIN - 64)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::MINUTES - 129))
        | (1ULL << (sqlParser::MONOTONIC - 129))
        | (1ULL << (sqlParser::MONTHS - 129))
        | (1ULL << (sqlParser::MS - 129))
        | (1ULL << (sqlParser::NODE - 129))
        | (1ULL << (sqlParser::NOOP - 129))
        | (1ULL << (sqlParser::NOP - 129))
        | (1ULL << (sqlParser::NOT - 129))
        | (1ULL << (sqlParser::NOW - 129))
        | (1ULL << (sqlParser::SQL_NULL - 129))
        | (1ULL << (sqlParser::NULLIF - 129))
        | (1ULL << (sqlParser::OBJECTS - 129))
        | (1ULL << (sqlParser::OFF - 129))
        | (1ULL << (sqlParser::OFFSET - 129))
        | (1ULL << (sqlParser::OVERLAY - 129))
        | (1ULL << (sqlParser::PATHS - 129))
        | (1ULL << (sqlParser::PLACING - 129))
        | (1ULL << (sqlParser::POSITION - 129))
        | (1ULL << (sqlParser::PRETTY - 129))
        | (1ULL << (sqlParser::QUARTER - 129))
        | (1ULL << (sqlParser::QUARTERS - 129))
        | (1ULL << (sqlParser::REFERENCE - 129))
        | (1ULL << (sqlParser::REGIONS - 129))
        | (1ULL << (sqlParser::REPLICATED - 129))
        | (1ULL << (sqlParser::ROWSIZE - 129))
        | (1ULL << (sqlParser::SCHEMA - 129))
        | (1ULL << (sqlParser::SEC - 129))
        | (1ULL << (sqlParser::SECONDS - 129))
        | (1ULL << (sqlParser::SKIPLIST - 129))
        | (1ULL << (sqlParser::SLEEP - 129))
        | (1ULL << (sqlParser::SPACE - 129))
        | (1ULL << (sqlParser::STAGE - 129))
        | (1ULL << (sqlParser::STATS - 129))
        | (1ULL << (sqlParser::STATISTICS - 129))
        | (1ULL << (sqlParser::STATUS - 129))
        | (1ULL << (sqlParser::SUBSTR - 129))
        | (1ULL << (sqlParser::SUBSTRING - 129))
        | (1ULL << (sqlParser::SUM - 129))
        | (1ULL << (sqlParser::SZIDX - 129))
        | (1ULL << (sqlParser::SZTBL - 129))
        | (1ULL << (sqlParser::S_ONLY - 129))
        | (1ULL << (sqlParser::TABLESIZE - 129))
        | (1ULL << (sqlParser::TEXT - 129))
        | (1ULL << (sqlParser::TIME - 129))
        | (1ULL << (sqlParser::TIMESTAMP - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::TOP - 194))
        | (1ULL << (sqlParser::TRACKMEMORY - 194))
        | (1ULL << (sqlParser::TREE - 194))
        | (1ULL << (sqlParser::TRIM - 194))
        | (1ULL << (sqlParser::TRUNCATE - 194))
        | (1ULL << (sqlParser::SQL_TRUE - 194))
        | (1ULL << (sqlParser::TYPE - 194))
        | (1ULL << (sqlParser::TYPEOF - 194))
        | (1ULL << (sqlParser::VALUE - 194))
        | (1ULL << (sqlParser::WEEK - 194))
        | (1ULL << (sqlParser::WEEKS - 194))
        | (1ULL << (sqlParser::WITH_NAMES - 194))
        | (1ULL << (sqlParser::WITH_TYPES - 194))
        | (1ULL << (sqlParser::XOR - 194))
        | (1ULL << (sqlParser::YEARS - 194))
        | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 194))
        | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 194))
        | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 194))
        | (1ULL << (sqlParser::SINGLQUOTED_STRING - 194))
        | (1ULL << (sqlParser::DUBLQUOTED_STRING - 194))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 194))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 194))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 194)))) != 0)) {
        setState(1463);
        expr(0);
        setState(1468);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1464);
          match(sqlParser::T__5);
          setState(1465);
          expr(0);
          setState(1470);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1473);
      match(sqlParser::T__9);
      break;
    }

    case 14: {
      _localctx = _tracker.createInstance<ConcatOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1474);
      match(sqlParser::CONCAT);
      setState(1475);
      match(sqlParser::T__8);
      setState(1484);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__6)
        | (1ULL << sqlParser::T__8)
        | (1ULL << sqlParser::T__12)
        | (1ULL << sqlParser::ASC)
        | (1ULL << sqlParser::AVG)
        | (1ULL << sqlParser::BIT)
        | (1ULL << sqlParser::BITS)
        | (1ULL << sqlParser::BLOCK)
        | (1ULL << sqlParser::BLOOM)
        | (1ULL << sqlParser::BPA)
        | (1ULL << sqlParser::BTREE)
        | (1ULL << sqlParser::BYTE)
        | (1ULL << sqlParser::BYTES)
        | (1ULL << sqlParser::CASE)
        | (1ULL << sqlParser::CAST)
        | (1ULL << sqlParser::CATALOG)
        | (1ULL << sqlParser::CHARS)
        | (1ULL << sqlParser::COALESCE)
        | (1ULL << sqlParser::CONCAT)
        | (1ULL << sqlParser::COUNT)
        | (1ULL << sqlParser::CURRENT_CATALOG)
        | (1ULL << sqlParser::CURRENT_SCHEMA)
        | (1ULL << sqlParser::CURRENT_TIMESTAMP)
        | (1ULL << sqlParser::CURRENT_USER)
        | (1ULL << sqlParser::DATA)
        | (1ULL << sqlParser::DATE)
        | (1ULL << sqlParser::DATE_FORMAT)
        | (1ULL << sqlParser::DAYS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::DECODE - 64))
        | (1ULL << (sqlParser::DESC - 64))
        | (1ULL << (sqlParser::DETAIL - 64))
        | (1ULL << (sqlParser::DIGITS - 64))
        | (1ULL << (sqlParser::DLOAD - 64))
        | (1ULL << (sqlParser::ECHO - 64))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
        | (1ULL << (sqlParser::EXISTS - 64))
        | (1ULL << (sqlParser::EXPLAIN - 64))
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
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::KEY - 64))
        | (1ULL << (sqlParser::LIMIT - 64))
        | (1ULL << (sqlParser::LOAD - 64))
        | (1ULL << (sqlParser::LOG - 64))
        | (1ULL << (sqlParser::LOOP - 64))
        | (1ULL << (sqlParser::LSIDX - 64))
        | (1ULL << (sqlParser::LSTBL - 64))
        | (1ULL << (sqlParser::MAX - 64))
        | (1ULL << (sqlParser::MEMORYLEAKS - 64))
        | (1ULL << (sqlParser::MILLIS - 64))
        | (1ULL << (sqlParser::MILLISECOND - 64))
        | (1ULL << (sqlParser::MILLISECONDS - 64))
        | (1ULL << (sqlParser::MIN - 64)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::MINUTES - 129))
        | (1ULL << (sqlParser::MONOTONIC - 129))
        | (1ULL << (sqlParser::MONTHS - 129))
        | (1ULL << (sqlParser::MS - 129))
        | (1ULL << (sqlParser::NODE - 129))
        | (1ULL << (sqlParser::NOOP - 129))
        | (1ULL << (sqlParser::NOP - 129))
        | (1ULL << (sqlParser::NOT - 129))
        | (1ULL << (sqlParser::NOW - 129))
        | (1ULL << (sqlParser::SQL_NULL - 129))
        | (1ULL << (sqlParser::NULLIF - 129))
        | (1ULL << (sqlParser::OBJECTS - 129))
        | (1ULL << (sqlParser::OFF - 129))
        | (1ULL << (sqlParser::OFFSET - 129))
        | (1ULL << (sqlParser::OVERLAY - 129))
        | (1ULL << (sqlParser::PATHS - 129))
        | (1ULL << (sqlParser::PLACING - 129))
        | (1ULL << (sqlParser::POSITION - 129))
        | (1ULL << (sqlParser::PRETTY - 129))
        | (1ULL << (sqlParser::QUARTER - 129))
        | (1ULL << (sqlParser::QUARTERS - 129))
        | (1ULL << (sqlParser::REFERENCE - 129))
        | (1ULL << (sqlParser::REGIONS - 129))
        | (1ULL << (sqlParser::REPLICATED - 129))
        | (1ULL << (sqlParser::ROWSIZE - 129))
        | (1ULL << (sqlParser::SCHEMA - 129))
        | (1ULL << (sqlParser::SEC - 129))
        | (1ULL << (sqlParser::SECONDS - 129))
        | (1ULL << (sqlParser::SKIPLIST - 129))
        | (1ULL << (sqlParser::SLEEP - 129))
        | (1ULL << (sqlParser::SPACE - 129))
        | (1ULL << (sqlParser::STAGE - 129))
        | (1ULL << (sqlParser::STATS - 129))
        | (1ULL << (sqlParser::STATISTICS - 129))
        | (1ULL << (sqlParser::STATUS - 129))
        | (1ULL << (sqlParser::SUBSTR - 129))
        | (1ULL << (sqlParser::SUBSTRING - 129))
        | (1ULL << (sqlParser::SUM - 129))
        | (1ULL << (sqlParser::SZIDX - 129))
        | (1ULL << (sqlParser::SZTBL - 129))
        | (1ULL << (sqlParser::S_ONLY - 129))
        | (1ULL << (sqlParser::TABLESIZE - 129))
        | (1ULL << (sqlParser::TEXT - 129))
        | (1ULL << (sqlParser::TIME - 129))
        | (1ULL << (sqlParser::TIMESTAMP - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::TOP - 194))
        | (1ULL << (sqlParser::TRACKMEMORY - 194))
        | (1ULL << (sqlParser::TREE - 194))
        | (1ULL << (sqlParser::TRIM - 194))
        | (1ULL << (sqlParser::TRUNCATE - 194))
        | (1ULL << (sqlParser::SQL_TRUE - 194))
        | (1ULL << (sqlParser::TYPE - 194))
        | (1ULL << (sqlParser::TYPEOF - 194))
        | (1ULL << (sqlParser::VALUE - 194))
        | (1ULL << (sqlParser::WEEK - 194))
        | (1ULL << (sqlParser::WEEKS - 194))
        | (1ULL << (sqlParser::WITH_NAMES - 194))
        | (1ULL << (sqlParser::WITH_TYPES - 194))
        | (1ULL << (sqlParser::XOR - 194))
        | (1ULL << (sqlParser::YEARS - 194))
        | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 194))
        | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 194))
        | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 194))
        | (1ULL << (sqlParser::SINGLQUOTED_STRING - 194))
        | (1ULL << (sqlParser::DUBLQUOTED_STRING - 194))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 194))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 194))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 194)))) != 0)) {
        setState(1476);
        expr(0);
        setState(1481);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1477);
          match(sqlParser::T__5);
          setState(1478);
          expr(0);
          setState(1483);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1486);
      match(sqlParser::T__9);
      break;
    }

    case 15: {
      _localctx = _tracker.createInstance<OverlayFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1487);
      match(sqlParser::OVERLAY);
      setState(1488);
      match(sqlParser::T__8);
      setState(1489);
      expr(0);
      setState(1490);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5 || _la == sqlParser::PLACING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1491);
      expr(0);
      setState(1492);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5 || _la == sqlParser::FROM)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1493);
      expr(0);
      setState(1496);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__5 || _la == sqlParser::FOR) {
        setState(1494);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__5 || _la == sqlParser::FOR)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(1495);
        expr(0);
      }
      setState(1498);
      match(sqlParser::T__9);
      break;
    }

    case 16: {
      _localctx = _tracker.createInstance<PositionFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1500);
      match(sqlParser::POSITION);
      setState(1501);
      match(sqlParser::T__8);
      setState(1502);
      expr(0);
      setState(1503);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5 || _la == sqlParser::IN)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1504);
      expr(0);
      setState(1505);
      match(sqlParser::T__9);
      break;
    }

    case 17: {
      _localctx = _tracker.createInstance<SubStringFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1507);
      _la = _input->LA(1);
      if (!(_la == sqlParser::SUBSTR

      || _la == sqlParser::SUBSTRING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1508);
      match(sqlParser::T__8);
      setState(1523);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 187, _ctx)) {
      case 1: {
        setState(1509);
        dynamic_cast<SubStringFuncContext *>(_localctx)->src = expr(0);
        setState(1510);
        match(sqlParser::T__5);
        setState(1511);
        dynamic_cast<SubStringFuncContext *>(_localctx)->start = expr(0);
        setState(1514);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1512);
          match(sqlParser::T__5);
          setState(1513);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      case 2: {
        setState(1516);
        dynamic_cast<SubStringFuncContext *>(_localctx)->src = expr(0);
        setState(1517);
        match(sqlParser::FROM);
        setState(1518);
        dynamic_cast<SubStringFuncContext *>(_localctx)->start = expr(0);
        setState(1521);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::FOR) {
          setState(1519);
          match(sqlParser::FOR);
          setState(1520);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      }
      setState(1525);
      match(sqlParser::T__9);
      break;
    }

    case 18: {
      _localctx = _tracker.createInstance<TrimFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1527);
      match(sqlParser::TRIM);
      setState(1528);
      match(sqlParser::T__8);
      setState(1542);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 191, _ctx)) {
      case 1: {
        setState(1529);
        expr(0);
        setState(1532);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1530);
          match(sqlParser::T__5);
          setState(1531);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        break;
      }

      case 2: {
        setState(1535);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::BOTH || _la == sqlParser::LEADING || _la == sqlParser::TRAILING) {
          setState(1534);
          _la = _input->LA(1);
          if (!(_la == sqlParser::BOTH || _la == sqlParser::LEADING || _la == sqlParser::TRAILING)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        setState(1538);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << sqlParser::T__6)
          | (1ULL << sqlParser::T__8)
          | (1ULL << sqlParser::T__12)
          | (1ULL << sqlParser::ASC)
          | (1ULL << sqlParser::AVG)
          | (1ULL << sqlParser::BIT)
          | (1ULL << sqlParser::BITS)
          | (1ULL << sqlParser::BLOCK)
          | (1ULL << sqlParser::BLOOM)
          | (1ULL << sqlParser::BPA)
          | (1ULL << sqlParser::BTREE)
          | (1ULL << sqlParser::BYTE)
          | (1ULL << sqlParser::BYTES)
          | (1ULL << sqlParser::CASE)
          | (1ULL << sqlParser::CAST)
          | (1ULL << sqlParser::CATALOG)
          | (1ULL << sqlParser::CHARS)
          | (1ULL << sqlParser::COALESCE)
          | (1ULL << sqlParser::CONCAT)
          | (1ULL << sqlParser::COUNT)
          | (1ULL << sqlParser::CURRENT_CATALOG)
          | (1ULL << sqlParser::CURRENT_SCHEMA)
          | (1ULL << sqlParser::CURRENT_TIMESTAMP)
          | (1ULL << sqlParser::CURRENT_USER)
          | (1ULL << sqlParser::DATA)
          | (1ULL << sqlParser::DATE)
          | (1ULL << sqlParser::DATE_FORMAT)
          | (1ULL << sqlParser::DAYS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::DECODE - 64))
          | (1ULL << (sqlParser::DESC - 64))
          | (1ULL << (sqlParser::DETAIL - 64))
          | (1ULL << (sqlParser::DIGITS - 64))
          | (1ULL << (sqlParser::DLOAD - 64))
          | (1ULL << (sqlParser::ECHO - 64))
          | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
          | (1ULL << (sqlParser::EXISTS - 64))
          | (1ULL << (sqlParser::EXPLAIN - 64))
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
          | (1ULL << (sqlParser::INTERVAL - 64))
          | (1ULL << (sqlParser::KEY - 64))
          | (1ULL << (sqlParser::LIMIT - 64))
          | (1ULL << (sqlParser::LOAD - 64))
          | (1ULL << (sqlParser::LOG - 64))
          | (1ULL << (sqlParser::LOOP - 64))
          | (1ULL << (sqlParser::LSIDX - 64))
          | (1ULL << (sqlParser::LSTBL - 64))
          | (1ULL << (sqlParser::MAX - 64))
          | (1ULL << (sqlParser::MEMORYLEAKS - 64))
          | (1ULL << (sqlParser::MILLIS - 64))
          | (1ULL << (sqlParser::MILLISECOND - 64))
          | (1ULL << (sqlParser::MILLISECONDS - 64))
          | (1ULL << (sqlParser::MIN - 64)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::MINUTES - 129))
          | (1ULL << (sqlParser::MONOTONIC - 129))
          | (1ULL << (sqlParser::MONTHS - 129))
          | (1ULL << (sqlParser::MS - 129))
          | (1ULL << (sqlParser::NODE - 129))
          | (1ULL << (sqlParser::NOOP - 129))
          | (1ULL << (sqlParser::NOP - 129))
          | (1ULL << (sqlParser::NOT - 129))
          | (1ULL << (sqlParser::NOW - 129))
          | (1ULL << (sqlParser::SQL_NULL - 129))
          | (1ULL << (sqlParser::NULLIF - 129))
          | (1ULL << (sqlParser::OBJECTS - 129))
          | (1ULL << (sqlParser::OFF - 129))
          | (1ULL << (sqlParser::OFFSET - 129))
          | (1ULL << (sqlParser::OVERLAY - 129))
          | (1ULL << (sqlParser::PATHS - 129))
          | (1ULL << (sqlParser::PLACING - 129))
          | (1ULL << (sqlParser::POSITION - 129))
          | (1ULL << (sqlParser::PRETTY - 129))
          | (1ULL << (sqlParser::QUARTER - 129))
          | (1ULL << (sqlParser::QUARTERS - 129))
          | (1ULL << (sqlParser::REFERENCE - 129))
          | (1ULL << (sqlParser::REGIONS - 129))
          | (1ULL << (sqlParser::REPLICATED - 129))
          | (1ULL << (sqlParser::ROWSIZE - 129))
          | (1ULL << (sqlParser::SCHEMA - 129))
          | (1ULL << (sqlParser::SEC - 129))
          | (1ULL << (sqlParser::SECONDS - 129))
          | (1ULL << (sqlParser::SKIPLIST - 129))
          | (1ULL << (sqlParser::SLEEP - 129))
          | (1ULL << (sqlParser::SPACE - 129))
          | (1ULL << (sqlParser::STAGE - 129))
          | (1ULL << (sqlParser::STATS - 129))
          | (1ULL << (sqlParser::STATISTICS - 129))
          | (1ULL << (sqlParser::STATUS - 129))
          | (1ULL << (sqlParser::SUBSTR - 129))
          | (1ULL << (sqlParser::SUBSTRING - 129))
          | (1ULL << (sqlParser::SUM - 129))
          | (1ULL << (sqlParser::SZIDX - 129))
          | (1ULL << (sqlParser::SZTBL - 129))
          | (1ULL << (sqlParser::S_ONLY - 129))
          | (1ULL << (sqlParser::TABLESIZE - 129))
          | (1ULL << (sqlParser::TEXT - 129))
          | (1ULL << (sqlParser::TIME - 129))
          | (1ULL << (sqlParser::TIMESTAMP - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::TOP - 194))
          | (1ULL << (sqlParser::TRACKMEMORY - 194))
          | (1ULL << (sqlParser::TREE - 194))
          | (1ULL << (sqlParser::TRIM - 194))
          | (1ULL << (sqlParser::TRUNCATE - 194))
          | (1ULL << (sqlParser::SQL_TRUE - 194))
          | (1ULL << (sqlParser::TYPE - 194))
          | (1ULL << (sqlParser::TYPEOF - 194))
          | (1ULL << (sqlParser::VALUE - 194))
          | (1ULL << (sqlParser::WEEK - 194))
          | (1ULL << (sqlParser::WEEKS - 194))
          | (1ULL << (sqlParser::WITH_NAMES - 194))
          | (1ULL << (sqlParser::WITH_TYPES - 194))
          | (1ULL << (sqlParser::XOR - 194))
          | (1ULL << (sqlParser::YEARS - 194))
          | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 194))
          | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 194))
          | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 194))
          | (1ULL << (sqlParser::SINGLQUOTED_STRING - 194))
          | (1ULL << (sqlParser::DUBLQUOTED_STRING - 194))
          | (1ULL << (sqlParser::FLOAT_LITERAL - 194))
          | (1ULL << (sqlParser::DECIMAL_LITERAL - 194))
          | (1ULL << (sqlParser::INTEGER_LITERAL - 194)))) != 0)) {
          setState(1537);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        setState(1540);
        match(sqlParser::FROM);
        setState(1541);
        expr(0);
        break;
      }

      }
      setState(1544);
      match(sqlParser::T__9);
      break;
    }

    case 19: {
      _localctx = _tracker.createInstance<ExtractFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1546);
      match(sqlParser::EXTRACT);
      setState(1547);
      match(sqlParser::T__8);
      setState(1548);
      _la = _input->LA(1);
      if (!(_la == sqlParser::DAY || ((((_la - 96) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 96)) & ((1ULL << (sqlParser::HOUR - 96))
        | (1ULL << (sqlParser::MINUTE - 96))
        | (1ULL << (sqlParser::MONTH - 96)))) != 0) || _la == sqlParser::SECOND

      || _la == sqlParser::YEAR)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1549);
      match(sqlParser::FROM);
      setState(1550);
      expr(0);
      setState(1551);
      match(sqlParser::T__9);
      break;
    }

    case 20: {
      _localctx = _tracker.createInstance<CountAllContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1553);
      match(sqlParser::COUNT);
      setState(1554);
      match(sqlParser::T__8);
      setState(1556);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ALL) {
        setState(1555);
        match(sqlParser::ALL);
      }
      setState(1558);
      match(sqlParser::T__7);
      setState(1559);
      match(sqlParser::T__9);
      break;
    }

    case 21: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1560);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::COUNT);
      setState(1561);
      match(sqlParser::T__8);
      setState(1564);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1562);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1563);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__6:
        case sqlParser::T__8:
        case sqlParser::T__12:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::COALESCE:
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
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXPLAIN:
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
        case sqlParser::INTERVAL:
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
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
        case sqlParser::OVERLAY:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
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
      setState(1566);
      expr(0);
      setState(1567);
      match(sqlParser::T__9);
      break;
    }

    case 22: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1569);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::SUM);
      setState(1570);
      match(sqlParser::T__8);
      setState(1573);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1571);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1572);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__6:
        case sqlParser::T__8:
        case sqlParser::T__12:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::COALESCE:
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
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXPLAIN:
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
        case sqlParser::INTERVAL:
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
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
        case sqlParser::OVERLAY:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
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
      setState(1575);
      expr(0);
      setState(1576);
      match(sqlParser::T__9);
      break;
    }

    case 23: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1578);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::AVG);
      setState(1579);
      match(sqlParser::T__8);
      setState(1582);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1580);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1581);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__6:
        case sqlParser::T__8:
        case sqlParser::T__12:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::COALESCE:
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
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXPLAIN:
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
        case sqlParser::INTERVAL:
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
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
        case sqlParser::OVERLAY:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
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
      setState(1584);
      expr(0);
      setState(1585);
      match(sqlParser::T__9);
      break;
    }

    case 24: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1587);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MIN);
      setState(1588);
      match(sqlParser::T__8);
      setState(1591);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1589);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1590);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__6:
        case sqlParser::T__8:
        case sqlParser::T__12:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::COALESCE:
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
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXPLAIN:
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
        case sqlParser::INTERVAL:
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
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
        case sqlParser::OVERLAY:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
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
      setState(1593);
      expr(0);
      setState(1594);
      match(sqlParser::T__9);
      break;
    }

    case 25: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1596);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MAX);
      setState(1597);
      match(sqlParser::T__8);
      setState(1600);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1598);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1599);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__6:
        case sqlParser::T__8:
        case sqlParser::T__12:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BPA:
        case sqlParser::BTREE:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::CATALOG:
        case sqlParser::CHARS:
        case sqlParser::COALESCE:
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
        case sqlParser::ECHO:
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXPLAIN:
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
        case sqlParser::INTERVAL:
        case sqlParser::KEY:
        case sqlParser::LIMIT:
        case sqlParser::LOAD:
        case sqlParser::LOG:
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
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
        case sqlParser::OVERLAY:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
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
      setState(1602);
      expr(0);
      setState(1603);
      match(sqlParser::T__9);
      break;
    }

    case 26: {
      _localctx = _tracker.createInstance<ScalarFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1605);
      dynamic_cast<ScalarFuncContext *>(_localctx)->func = functionName();
      setState(1606);
      match(sqlParser::T__8);
      setState(1615);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__6)
        | (1ULL << sqlParser::T__8)
        | (1ULL << sqlParser::T__12)
        | (1ULL << sqlParser::ASC)
        | (1ULL << sqlParser::AVG)
        | (1ULL << sqlParser::BIT)
        | (1ULL << sqlParser::BITS)
        | (1ULL << sqlParser::BLOCK)
        | (1ULL << sqlParser::BLOOM)
        | (1ULL << sqlParser::BPA)
        | (1ULL << sqlParser::BTREE)
        | (1ULL << sqlParser::BYTE)
        | (1ULL << sqlParser::BYTES)
        | (1ULL << sqlParser::CASE)
        | (1ULL << sqlParser::CAST)
        | (1ULL << sqlParser::CATALOG)
        | (1ULL << sqlParser::CHARS)
        | (1ULL << sqlParser::COALESCE)
        | (1ULL << sqlParser::CONCAT)
        | (1ULL << sqlParser::COUNT)
        | (1ULL << sqlParser::CURRENT_CATALOG)
        | (1ULL << sqlParser::CURRENT_SCHEMA)
        | (1ULL << sqlParser::CURRENT_TIMESTAMP)
        | (1ULL << sqlParser::CURRENT_USER)
        | (1ULL << sqlParser::DATA)
        | (1ULL << sqlParser::DATE)
        | (1ULL << sqlParser::DATE_FORMAT)
        | (1ULL << sqlParser::DAYS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::DECODE - 64))
        | (1ULL << (sqlParser::DESC - 64))
        | (1ULL << (sqlParser::DETAIL - 64))
        | (1ULL << (sqlParser::DIGITS - 64))
        | (1ULL << (sqlParser::DLOAD - 64))
        | (1ULL << (sqlParser::ECHO - 64))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
        | (1ULL << (sqlParser::EXISTS - 64))
        | (1ULL << (sqlParser::EXPLAIN - 64))
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
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::KEY - 64))
        | (1ULL << (sqlParser::LIMIT - 64))
        | (1ULL << (sqlParser::LOAD - 64))
        | (1ULL << (sqlParser::LOG - 64))
        | (1ULL << (sqlParser::LOOP - 64))
        | (1ULL << (sqlParser::LSIDX - 64))
        | (1ULL << (sqlParser::LSTBL - 64))
        | (1ULL << (sqlParser::MAX - 64))
        | (1ULL << (sqlParser::MEMORYLEAKS - 64))
        | (1ULL << (sqlParser::MILLIS - 64))
        | (1ULL << (sqlParser::MILLISECOND - 64))
        | (1ULL << (sqlParser::MILLISECONDS - 64))
        | (1ULL << (sqlParser::MIN - 64)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::MINUTES - 129))
        | (1ULL << (sqlParser::MONOTONIC - 129))
        | (1ULL << (sqlParser::MONTHS - 129))
        | (1ULL << (sqlParser::MS - 129))
        | (1ULL << (sqlParser::NODE - 129))
        | (1ULL << (sqlParser::NOOP - 129))
        | (1ULL << (sqlParser::NOP - 129))
        | (1ULL << (sqlParser::NOT - 129))
        | (1ULL << (sqlParser::NOW - 129))
        | (1ULL << (sqlParser::SQL_NULL - 129))
        | (1ULL << (sqlParser::NULLIF - 129))
        | (1ULL << (sqlParser::OBJECTS - 129))
        | (1ULL << (sqlParser::OFF - 129))
        | (1ULL << (sqlParser::OFFSET - 129))
        | (1ULL << (sqlParser::OVERLAY - 129))
        | (1ULL << (sqlParser::PATHS - 129))
        | (1ULL << (sqlParser::PLACING - 129))
        | (1ULL << (sqlParser::POSITION - 129))
        | (1ULL << (sqlParser::PRETTY - 129))
        | (1ULL << (sqlParser::QUARTER - 129))
        | (1ULL << (sqlParser::QUARTERS - 129))
        | (1ULL << (sqlParser::REFERENCE - 129))
        | (1ULL << (sqlParser::REGIONS - 129))
        | (1ULL << (sqlParser::REPLICATED - 129))
        | (1ULL << (sqlParser::ROWSIZE - 129))
        | (1ULL << (sqlParser::SCHEMA - 129))
        | (1ULL << (sqlParser::SEC - 129))
        | (1ULL << (sqlParser::SECONDS - 129))
        | (1ULL << (sqlParser::SKIPLIST - 129))
        | (1ULL << (sqlParser::SLEEP - 129))
        | (1ULL << (sqlParser::SPACE - 129))
        | (1ULL << (sqlParser::STAGE - 129))
        | (1ULL << (sqlParser::STATS - 129))
        | (1ULL << (sqlParser::STATISTICS - 129))
        | (1ULL << (sqlParser::STATUS - 129))
        | (1ULL << (sqlParser::SUBSTR - 129))
        | (1ULL << (sqlParser::SUBSTRING - 129))
        | (1ULL << (sqlParser::SUM - 129))
        | (1ULL << (sqlParser::SZIDX - 129))
        | (1ULL << (sqlParser::SZTBL - 129))
        | (1ULL << (sqlParser::S_ONLY - 129))
        | (1ULL << (sqlParser::TABLESIZE - 129))
        | (1ULL << (sqlParser::TEXT - 129))
        | (1ULL << (sqlParser::TIME - 129))
        | (1ULL << (sqlParser::TIMESTAMP - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::TOP - 194))
        | (1ULL << (sqlParser::TRACKMEMORY - 194))
        | (1ULL << (sqlParser::TREE - 194))
        | (1ULL << (sqlParser::TRIM - 194))
        | (1ULL << (sqlParser::TRUNCATE - 194))
        | (1ULL << (sqlParser::SQL_TRUE - 194))
        | (1ULL << (sqlParser::TYPE - 194))
        | (1ULL << (sqlParser::TYPEOF - 194))
        | (1ULL << (sqlParser::VALUE - 194))
        | (1ULL << (sqlParser::WEEK - 194))
        | (1ULL << (sqlParser::WEEKS - 194))
        | (1ULL << (sqlParser::WITH_NAMES - 194))
        | (1ULL << (sqlParser::WITH_TYPES - 194))
        | (1ULL << (sqlParser::XOR - 194))
        | (1ULL << (sqlParser::YEARS - 194))
        | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 194))
        | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 194))
        | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 194))
        | (1ULL << (sqlParser::SINGLQUOTED_STRING - 194))
        | (1ULL << (sqlParser::DUBLQUOTED_STRING - 194))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 194))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 194))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 194)))) != 0)) {
        setState(1607);
        expr(0);
        setState(1612);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1608);
          match(sqlParser::T__5);
          setState(1609);
          expr(0);
          setState(1614);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1617);
      match(sqlParser::T__9);
      break;
    }

    case 27: {
      _localctx = _tracker.createInstance<ParentheticContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1619);
      match(sqlParser::T__8);
      setState(1620);
      expr(0);
      setState(1621);
      match(sqlParser::T__9);
      break;
    }

    case 28: {
      _localctx = _tracker.createInstance<SubqueryExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1623);
      match(sqlParser::T__8);
      setState(1624);
      query();
      setState(1625);
      match(sqlParser::T__9);
      break;
    }

    case 29: {
      _localctx = _tracker.createInstance<SubqueryExistsOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1628);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::NOT) {
        setState(1627);
        match(sqlParser::NOT);
      }
      setState(1630);
      match(sqlParser::EXISTS);

      setState(1631);
      match(sqlParser::T__8);
      setState(1632);
      query();
      setState(1633);
      match(sqlParser::T__9);
      break;
    }

    case 30: {
      _localctx = _tracker.createInstance<LogicOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1635);
      dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::NOT);
      setState(1636);
      expr(4);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(1732);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 213, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(1730);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 212, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1639);

          if (!(precpred(_ctx, 37))) throw FailedPredicateException(this, "precpred(_ctx, 37)");
          setState(1640);
          dynamic_cast<BinaryOpContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__4)
            | (1ULL << sqlParser::T__7)
            | (1ULL << sqlParser::T__13))) != 0))) {
            dynamic_cast<BinaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1641);
          expr(38);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1642);

          if (!(precpred(_ctx, 36))) throw FailedPredicateException(this, "precpred(_ctx, 36)");
          setState(1643);
          dynamic_cast<BinaryOpContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__6)
            | (1ULL << sqlParser::T__12)
            | (1ULL << sqlParser::T__14))) != 0))) {
            dynamic_cast<BinaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1644);
          expr(37);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1645);

          if (!(precpred(_ctx, 35))) throw FailedPredicateException(this, "precpred(_ctx, 35)");
          setState(1646);
          dynamic_cast<BinaryOpContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__1)
            | (1ULL << sqlParser::T__2)
            | (1ULL << sqlParser::T__3)
            | (1ULL << sqlParser::T__15)
            | (1ULL << sqlParser::T__16)
            | (1ULL << sqlParser::T__17)
            | (1ULL << sqlParser::T__18))) != 0))) {
            dynamic_cast<BinaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1647);
          expr(36);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<LikeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1648);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(1650);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1649);
            match(sqlParser::NOT);
          }
          setState(1652);
          match(sqlParser::LIKE);
          setState(1653);
          expr(11);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<LikeEscapeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1654);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(1656);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1655);
            match(sqlParser::NOT);
          }
          setState(1658);
          match(sqlParser::LIKE);
          setState(1659);
          expr(0);
          setState(1660);
          match(sqlParser::ESCAPE);
          setState(1661);
          expr(10);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1663);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(1664);

          if (!(!(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent)))) throw FailedPredicateException(this, "!(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent))");
          setState(1665);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::AND);
          setState(1666);
          expr(4);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1667);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(1668);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::XOR);
          setState(1669);
          expr(3);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1670);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(1671);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::OR);
          setState(1672);
          expr(2);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<MethodCallContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1673);

          if (!(precpred(_ctx, 38))) throw FailedPredicateException(this, "precpred(_ctx, 38)");
          setState(1674);
          match(sqlParser::T__11);
          setState(1675);
          dynamic_cast<MethodCallContext *>(_localctx)->func = methodName();
          setState(1676);
          match(sqlParser::T__8);
          setState(1685);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__6)
            | (1ULL << sqlParser::T__8)
            | (1ULL << sqlParser::T__12)
            | (1ULL << sqlParser::ASC)
            | (1ULL << sqlParser::AVG)
            | (1ULL << sqlParser::BIT)
            | (1ULL << sqlParser::BITS)
            | (1ULL << sqlParser::BLOCK)
            | (1ULL << sqlParser::BLOOM)
            | (1ULL << sqlParser::BPA)
            | (1ULL << sqlParser::BTREE)
            | (1ULL << sqlParser::BYTE)
            | (1ULL << sqlParser::BYTES)
            | (1ULL << sqlParser::CASE)
            | (1ULL << sqlParser::CAST)
            | (1ULL << sqlParser::CATALOG)
            | (1ULL << sqlParser::CHARS)
            | (1ULL << sqlParser::COALESCE)
            | (1ULL << sqlParser::CONCAT)
            | (1ULL << sqlParser::COUNT)
            | (1ULL << sqlParser::CURRENT_CATALOG)
            | (1ULL << sqlParser::CURRENT_SCHEMA)
            | (1ULL << sqlParser::CURRENT_TIMESTAMP)
            | (1ULL << sqlParser::CURRENT_USER)
            | (1ULL << sqlParser::DATA)
            | (1ULL << sqlParser::DATE)
            | (1ULL << sqlParser::DATE_FORMAT)
            | (1ULL << sqlParser::DAYS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::DECODE - 64))
            | (1ULL << (sqlParser::DESC - 64))
            | (1ULL << (sqlParser::DETAIL - 64))
            | (1ULL << (sqlParser::DIGITS - 64))
            | (1ULL << (sqlParser::DLOAD - 64))
            | (1ULL << (sqlParser::ECHO - 64))
            | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
            | (1ULL << (sqlParser::EXISTS - 64))
            | (1ULL << (sqlParser::EXPLAIN - 64))
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
            | (1ULL << (sqlParser::INTERVAL - 64))
            | (1ULL << (sqlParser::KEY - 64))
            | (1ULL << (sqlParser::LIMIT - 64))
            | (1ULL << (sqlParser::LOAD - 64))
            | (1ULL << (sqlParser::LOG - 64))
            | (1ULL << (sqlParser::LOOP - 64))
            | (1ULL << (sqlParser::LSIDX - 64))
            | (1ULL << (sqlParser::LSTBL - 64))
            | (1ULL << (sqlParser::MAX - 64))
            | (1ULL << (sqlParser::MEMORYLEAKS - 64))
            | (1ULL << (sqlParser::MILLIS - 64))
            | (1ULL << (sqlParser::MILLISECOND - 64))
            | (1ULL << (sqlParser::MILLISECONDS - 64))
            | (1ULL << (sqlParser::MIN - 64)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::MINUTES - 129))
            | (1ULL << (sqlParser::MONOTONIC - 129))
            | (1ULL << (sqlParser::MONTHS - 129))
            | (1ULL << (sqlParser::MS - 129))
            | (1ULL << (sqlParser::NODE - 129))
            | (1ULL << (sqlParser::NOOP - 129))
            | (1ULL << (sqlParser::NOP - 129))
            | (1ULL << (sqlParser::NOT - 129))
            | (1ULL << (sqlParser::NOW - 129))
            | (1ULL << (sqlParser::SQL_NULL - 129))
            | (1ULL << (sqlParser::NULLIF - 129))
            | (1ULL << (sqlParser::OBJECTS - 129))
            | (1ULL << (sqlParser::OFF - 129))
            | (1ULL << (sqlParser::OFFSET - 129))
            | (1ULL << (sqlParser::OVERLAY - 129))
            | (1ULL << (sqlParser::PATHS - 129))
            | (1ULL << (sqlParser::PLACING - 129))
            | (1ULL << (sqlParser::POSITION - 129))
            | (1ULL << (sqlParser::PRETTY - 129))
            | (1ULL << (sqlParser::QUARTER - 129))
            | (1ULL << (sqlParser::QUARTERS - 129))
            | (1ULL << (sqlParser::REFERENCE - 129))
            | (1ULL << (sqlParser::REGIONS - 129))
            | (1ULL << (sqlParser::REPLICATED - 129))
            | (1ULL << (sqlParser::ROWSIZE - 129))
            | (1ULL << (sqlParser::SCHEMA - 129))
            | (1ULL << (sqlParser::SEC - 129))
            | (1ULL << (sqlParser::SECONDS - 129))
            | (1ULL << (sqlParser::SKIPLIST - 129))
            | (1ULL << (sqlParser::SLEEP - 129))
            | (1ULL << (sqlParser::SPACE - 129))
            | (1ULL << (sqlParser::STAGE - 129))
            | (1ULL << (sqlParser::STATS - 129))
            | (1ULL << (sqlParser::STATISTICS - 129))
            | (1ULL << (sqlParser::STATUS - 129))
            | (1ULL << (sqlParser::SUBSTR - 129))
            | (1ULL << (sqlParser::SUBSTRING - 129))
            | (1ULL << (sqlParser::SUM - 129))
            | (1ULL << (sqlParser::SZIDX - 129))
            | (1ULL << (sqlParser::SZTBL - 129))
            | (1ULL << (sqlParser::S_ONLY - 129))
            | (1ULL << (sqlParser::TABLESIZE - 129))
            | (1ULL << (sqlParser::TEXT - 129))
            | (1ULL << (sqlParser::TIME - 129))
            | (1ULL << (sqlParser::TIMESTAMP - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::TOP - 194))
            | (1ULL << (sqlParser::TRACKMEMORY - 194))
            | (1ULL << (sqlParser::TREE - 194))
            | (1ULL << (sqlParser::TRIM - 194))
            | (1ULL << (sqlParser::TRUNCATE - 194))
            | (1ULL << (sqlParser::SQL_TRUE - 194))
            | (1ULL << (sqlParser::TYPE - 194))
            | (1ULL << (sqlParser::TYPEOF - 194))
            | (1ULL << (sqlParser::VALUE - 194))
            | (1ULL << (sqlParser::WEEK - 194))
            | (1ULL << (sqlParser::WEEKS - 194))
            | (1ULL << (sqlParser::WITH_NAMES - 194))
            | (1ULL << (sqlParser::WITH_TYPES - 194))
            | (1ULL << (sqlParser::XOR - 194))
            | (1ULL << (sqlParser::YEARS - 194))
            | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 194))
            | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 194))
            | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 194))
            | (1ULL << (sqlParser::SINGLQUOTED_STRING - 194))
            | (1ULL << (sqlParser::DUBLQUOTED_STRING - 194))
            | (1ULL << (sqlParser::FLOAT_LITERAL - 194))
            | (1ULL << (sqlParser::DECIMAL_LITERAL - 194))
            | (1ULL << (sqlParser::INTEGER_LITERAL - 194)))) != 0)) {
            setState(1677);
            expr(0);
            setState(1682);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__5) {
              setState(1678);
              match(sqlParser::T__5);
              setState(1679);
              expr(0);
              setState(1684);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1687);
          match(sqlParser::T__9);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<IsNullOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1689);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(1690);
          match(sqlParser::IS);
          setState(1692);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1691);
            match(sqlParser::NOT);
          }
          setState(1694);
          match(sqlParser::SQL_NULL);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<SubqueryInOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1695);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(1697);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1696);
            match(sqlParser::NOT);
          }
          setState(1699);
          match(sqlParser::IN);

          setState(1700);
          match(sqlParser::T__8);
          setState(1701);
          query();
          setState(1702);
          match(sqlParser::T__9);
          break;
        }

        case 12: {
          auto newContext = _tracker.createInstance<InOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1704);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(1706);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1705);
            match(sqlParser::NOT);
          }
          setState(1708);
          match(sqlParser::IN);

          setState(1709);
          match(sqlParser::T__8);
          setState(1718);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__6)
            | (1ULL << sqlParser::T__8)
            | (1ULL << sqlParser::T__12)
            | (1ULL << sqlParser::ASC)
            | (1ULL << sqlParser::AVG)
            | (1ULL << sqlParser::BIT)
            | (1ULL << sqlParser::BITS)
            | (1ULL << sqlParser::BLOCK)
            | (1ULL << sqlParser::BLOOM)
            | (1ULL << sqlParser::BPA)
            | (1ULL << sqlParser::BTREE)
            | (1ULL << sqlParser::BYTE)
            | (1ULL << sqlParser::BYTES)
            | (1ULL << sqlParser::CASE)
            | (1ULL << sqlParser::CAST)
            | (1ULL << sqlParser::CATALOG)
            | (1ULL << sqlParser::CHARS)
            | (1ULL << sqlParser::COALESCE)
            | (1ULL << sqlParser::CONCAT)
            | (1ULL << sqlParser::COUNT)
            | (1ULL << sqlParser::CURRENT_CATALOG)
            | (1ULL << sqlParser::CURRENT_SCHEMA)
            | (1ULL << sqlParser::CURRENT_TIMESTAMP)
            | (1ULL << sqlParser::CURRENT_USER)
            | (1ULL << sqlParser::DATA)
            | (1ULL << sqlParser::DATE)
            | (1ULL << sqlParser::DATE_FORMAT)
            | (1ULL << sqlParser::DAYS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::DECODE - 64))
            | (1ULL << (sqlParser::DESC - 64))
            | (1ULL << (sqlParser::DETAIL - 64))
            | (1ULL << (sqlParser::DIGITS - 64))
            | (1ULL << (sqlParser::DLOAD - 64))
            | (1ULL << (sqlParser::ECHO - 64))
            | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
            | (1ULL << (sqlParser::EXISTS - 64))
            | (1ULL << (sqlParser::EXPLAIN - 64))
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
            | (1ULL << (sqlParser::INTERVAL - 64))
            | (1ULL << (sqlParser::KEY - 64))
            | (1ULL << (sqlParser::LIMIT - 64))
            | (1ULL << (sqlParser::LOAD - 64))
            | (1ULL << (sqlParser::LOG - 64))
            | (1ULL << (sqlParser::LOOP - 64))
            | (1ULL << (sqlParser::LSIDX - 64))
            | (1ULL << (sqlParser::LSTBL - 64))
            | (1ULL << (sqlParser::MAX - 64))
            | (1ULL << (sqlParser::MEMORYLEAKS - 64))
            | (1ULL << (sqlParser::MILLIS - 64))
            | (1ULL << (sqlParser::MILLISECOND - 64))
            | (1ULL << (sqlParser::MILLISECONDS - 64))
            | (1ULL << (sqlParser::MIN - 64)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::MINUTES - 129))
            | (1ULL << (sqlParser::MONOTONIC - 129))
            | (1ULL << (sqlParser::MONTHS - 129))
            | (1ULL << (sqlParser::MS - 129))
            | (1ULL << (sqlParser::NODE - 129))
            | (1ULL << (sqlParser::NOOP - 129))
            | (1ULL << (sqlParser::NOP - 129))
            | (1ULL << (sqlParser::NOT - 129))
            | (1ULL << (sqlParser::NOW - 129))
            | (1ULL << (sqlParser::SQL_NULL - 129))
            | (1ULL << (sqlParser::NULLIF - 129))
            | (1ULL << (sqlParser::OBJECTS - 129))
            | (1ULL << (sqlParser::OFF - 129))
            | (1ULL << (sqlParser::OFFSET - 129))
            | (1ULL << (sqlParser::OVERLAY - 129))
            | (1ULL << (sqlParser::PATHS - 129))
            | (1ULL << (sqlParser::PLACING - 129))
            | (1ULL << (sqlParser::POSITION - 129))
            | (1ULL << (sqlParser::PRETTY - 129))
            | (1ULL << (sqlParser::QUARTER - 129))
            | (1ULL << (sqlParser::QUARTERS - 129))
            | (1ULL << (sqlParser::REFERENCE - 129))
            | (1ULL << (sqlParser::REGIONS - 129))
            | (1ULL << (sqlParser::REPLICATED - 129))
            | (1ULL << (sqlParser::ROWSIZE - 129))
            | (1ULL << (sqlParser::SCHEMA - 129))
            | (1ULL << (sqlParser::SEC - 129))
            | (1ULL << (sqlParser::SECONDS - 129))
            | (1ULL << (sqlParser::SKIPLIST - 129))
            | (1ULL << (sqlParser::SLEEP - 129))
            | (1ULL << (sqlParser::SPACE - 129))
            | (1ULL << (sqlParser::STAGE - 129))
            | (1ULL << (sqlParser::STATS - 129))
            | (1ULL << (sqlParser::STATISTICS - 129))
            | (1ULL << (sqlParser::STATUS - 129))
            | (1ULL << (sqlParser::SUBSTR - 129))
            | (1ULL << (sqlParser::SUBSTRING - 129))
            | (1ULL << (sqlParser::SUM - 129))
            | (1ULL << (sqlParser::SZIDX - 129))
            | (1ULL << (sqlParser::SZTBL - 129))
            | (1ULL << (sqlParser::S_ONLY - 129))
            | (1ULL << (sqlParser::TABLESIZE - 129))
            | (1ULL << (sqlParser::TEXT - 129))
            | (1ULL << (sqlParser::TIME - 129))
            | (1ULL << (sqlParser::TIMESTAMP - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::TOP - 194))
            | (1ULL << (sqlParser::TRACKMEMORY - 194))
            | (1ULL << (sqlParser::TREE - 194))
            | (1ULL << (sqlParser::TRIM - 194))
            | (1ULL << (sqlParser::TRUNCATE - 194))
            | (1ULL << (sqlParser::SQL_TRUE - 194))
            | (1ULL << (sqlParser::TYPE - 194))
            | (1ULL << (sqlParser::TYPEOF - 194))
            | (1ULL << (sqlParser::VALUE - 194))
            | (1ULL << (sqlParser::WEEK - 194))
            | (1ULL << (sqlParser::WEEKS - 194))
            | (1ULL << (sqlParser::WITH_NAMES - 194))
            | (1ULL << (sqlParser::WITH_TYPES - 194))
            | (1ULL << (sqlParser::XOR - 194))
            | (1ULL << (sqlParser::YEARS - 194))
            | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 194))
            | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 194))
            | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 194))
            | (1ULL << (sqlParser::SINGLQUOTED_STRING - 194))
            | (1ULL << (sqlParser::DUBLQUOTED_STRING - 194))
            | (1ULL << (sqlParser::FLOAT_LITERAL - 194))
            | (1ULL << (sqlParser::DECIMAL_LITERAL - 194))
            | (1ULL << (sqlParser::INTEGER_LITERAL - 194)))) != 0)) {
            setState(1710);
            expr(0);
            setState(1715);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__5) {
              setState(1711);
              match(sqlParser::T__5);
              setState(1712);
              expr(0);
              setState(1717);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1720);
          match(sqlParser::T__9);
          break;
        }

        case 13: {
          auto newContext = _tracker.createInstance<BetweenOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1721);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(1723);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1722);
            match(sqlParser::NOT);
          }
          setState(1725);
          match(sqlParser::BETWEEN);

          setState(1726);
          expr(0);
          setState(1727);
          match(sqlParser::AND);
          setState(1728);
          expr(0);
          break;
        }

        } 
      }
      setState(1734);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 213, _ctx);
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
  enterRule(_localctx, 180, sqlParser::RuleVariableName);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1735);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::CURRENT_CATALOG)
      | (1ULL << sqlParser::CURRENT_SCHEMA)
      | (1ULL << sqlParser::CURRENT_TIMESTAMP)
      | (1ULL << sqlParser::CURRENT_USER))) != 0))) {
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
  enterRule(_localctx, 182, sqlParser::RuleColumnRef);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1750);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 216, _ctx)) {
    case 1: {
      setState(1745);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 215, _ctx)) {
      case 1: {
        setState(1740);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 214, _ctx)) {
        case 1: {
          setState(1737);
          catalogName();
          setState(1738);
          match(sqlParser::T__11);
          break;
        }

        }

        setState(1742);
        schemaName();
        setState(1743);
        match(sqlParser::T__11);
        break;
      }

      }

      setState(1747);
      tableName();
      setState(1748);
      match(sqlParser::T__11);
      break;
    }

    }
    setState(1752);
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
  enterRule(_localctx, 184, sqlParser::RuleColumnWildcard);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1767);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::ASC)
      | (1ULL << sqlParser::BIT)
      | (1ULL << sqlParser::BITS)
      | (1ULL << sqlParser::BLOCK)
      | (1ULL << sqlParser::BLOOM)
      | (1ULL << sqlParser::BPA)
      | (1ULL << sqlParser::BTREE)
      | (1ULL << sqlParser::BYTE)
      | (1ULL << sqlParser::BYTES)
      | (1ULL << sqlParser::CATALOG)
      | (1ULL << sqlParser::CHARS)
      | (1ULL << sqlParser::CONCAT)
      | (1ULL << sqlParser::DATA)
      | (1ULL << sqlParser::DATE_FORMAT)
      | (1ULL << sqlParser::DAYS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::DECODE - 64))
      | (1ULL << (sqlParser::DESC - 64))
      | (1ULL << (sqlParser::DETAIL - 64))
      | (1ULL << (sqlParser::DIGITS - 64))
      | (1ULL << (sqlParser::DLOAD - 64))
      | (1ULL << (sqlParser::ECHO - 64))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
      | (1ULL << (sqlParser::EXPLAIN - 64))
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
      | (1ULL << (sqlParser::KEY - 64))
      | (1ULL << (sqlParser::LIMIT - 64))
      | (1ULL << (sqlParser::LOAD - 64))
      | (1ULL << (sqlParser::LOG - 64))
      | (1ULL << (sqlParser::LOOP - 64))
      | (1ULL << (sqlParser::LSIDX - 64))
      | (1ULL << (sqlParser::LSTBL - 64))
      | (1ULL << (sqlParser::MEMORYLEAKS - 64))
      | (1ULL << (sqlParser::MILLIS - 64))
      | (1ULL << (sqlParser::MILLISECOND - 64))
      | (1ULL << (sqlParser::MILLISECONDS - 64)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::MINUTES - 129))
      | (1ULL << (sqlParser::MONOTONIC - 129))
      | (1ULL << (sqlParser::MONTHS - 129))
      | (1ULL << (sqlParser::MS - 129))
      | (1ULL << (sqlParser::NODE - 129))
      | (1ULL << (sqlParser::NOOP - 129))
      | (1ULL << (sqlParser::NOP - 129))
      | (1ULL << (sqlParser::NOW - 129))
      | (1ULL << (sqlParser::OBJECTS - 129))
      | (1ULL << (sqlParser::OFF - 129))
      | (1ULL << (sqlParser::OFFSET - 129))
      | (1ULL << (sqlParser::PATHS - 129))
      | (1ULL << (sqlParser::PLACING - 129))
      | (1ULL << (sqlParser::PRETTY - 129))
      | (1ULL << (sqlParser::QUARTER - 129))
      | (1ULL << (sqlParser::QUARTERS - 129))
      | (1ULL << (sqlParser::REFERENCE - 129))
      | (1ULL << (sqlParser::REGIONS - 129))
      | (1ULL << (sqlParser::REPLICATED - 129))
      | (1ULL << (sqlParser::ROWSIZE - 129))
      | (1ULL << (sqlParser::SCHEMA - 129))
      | (1ULL << (sqlParser::SEC - 129))
      | (1ULL << (sqlParser::SECONDS - 129))
      | (1ULL << (sqlParser::SKIPLIST - 129))
      | (1ULL << (sqlParser::SLEEP - 129))
      | (1ULL << (sqlParser::SPACE - 129))
      | (1ULL << (sqlParser::STAGE - 129))
      | (1ULL << (sqlParser::STATS - 129))
      | (1ULL << (sqlParser::STATISTICS - 129))
      | (1ULL << (sqlParser::STATUS - 129))
      | (1ULL << (sqlParser::SUBSTR - 129))
      | (1ULL << (sqlParser::SZIDX - 129))
      | (1ULL << (sqlParser::SZTBL - 129))
      | (1ULL << (sqlParser::S_ONLY - 129))
      | (1ULL << (sqlParser::TABLESIZE - 129))
      | (1ULL << (sqlParser::TEXT - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::TOP - 194))
      | (1ULL << (sqlParser::TRACKMEMORY - 194))
      | (1ULL << (sqlParser::TREE - 194))
      | (1ULL << (sqlParser::TRUNCATE - 194))
      | (1ULL << (sqlParser::TYPE - 194))
      | (1ULL << (sqlParser::TYPEOF - 194))
      | (1ULL << (sqlParser::VALUE - 194))
      | (1ULL << (sqlParser::WEEK - 194))
      | (1ULL << (sqlParser::WEEKS - 194))
      | (1ULL << (sqlParser::WITH_NAMES - 194))
      | (1ULL << (sqlParser::WITH_TYPES - 194))
      | (1ULL << (sqlParser::XOR - 194))
      | (1ULL << (sqlParser::YEARS - 194))
      | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 194))
      | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 194))
      | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 194)))) != 0)) {
      setState(1762);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 218, _ctx)) {
      case 1: {
        setState(1757);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 217, _ctx)) {
        case 1: {
          setState(1754);
          catalogName();
          setState(1755);
          match(sqlParser::T__11);
          break;
        }

        }

        setState(1759);
        schemaName();
        setState(1760);
        match(sqlParser::T__11);
        break;
      }

      }

      setState(1764);
      tableName();
      setState(1765);
      match(sqlParser::T__11);
    }
    setState(1769);
    match(sqlParser::T__7);
   
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
  enterRule(_localctx, 186, sqlParser::RuleClassName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1776);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 220, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1771);
        qualifier();
        setState(1772);
        match(sqlParser::T__11); 
      }
      setState(1778);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 220, _ctx);
    }
    setState(1779);
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
  enterRule(_localctx, 188, sqlParser::RuleFunctionName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1781);
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
  enterRule(_localctx, 190, sqlParser::RuleMethodName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1783);
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
  enterRule(_localctx, 192, sqlParser::RuleCatalogName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1785);
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
  enterRule(_localctx, 194, sqlParser::RuleSchemaName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1787);
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
  enterRule(_localctx, 196, sqlParser::RuleTableName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1789);
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
  enterRule(_localctx, 198, sqlParser::RuleTableAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1791);
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
  enterRule(_localctx, 200, sqlParser::RuleColumnName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1793);
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
  enterRule(_localctx, 202, sqlParser::RuleColumnAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1795);
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
  enterRule(_localctx, 204, sqlParser::RuleCollationName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1797);
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
  enterRule(_localctx, 206, sqlParser::RuleIndexName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1799);
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
  enterRule(_localctx, 208, sqlParser::RulePropertyName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1801);
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
  enterRule(_localctx, 210, sqlParser::RuleHintName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1803);
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
  enterRule(_localctx, 212, sqlParser::RuleExprHint);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1805);
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
  enterRule(_localctx, 214, sqlParser::RulePropertyValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1809);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 221, _ctx)) {
    case 1: {
      setState(1807);
      dynamic_cast<PropertyValueContext *>(_localctx)->lit = simpleLiteral();
      break;
    }

    case 2: {
      setState(1808);
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
  enterRule(_localctx, 216, sqlParser::RuleHintValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1814);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 222, _ctx)) {
    case 1: {
      setState(1811);
      dynamic_cast<HintValueContext *>(_localctx)->lit = simpleLiteral();
      break;
    }

    case 2: {
      setState(1812);
      dynamic_cast<HintValueContext *>(_localctx)->id = identifier();
      break;
    }

    case 3: {
      setState(1813);
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
  enterRule(_localctx, 218, sqlParser::RuleIdentifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1820);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::PLAIN_IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(1816);
        match(sqlParser::PLAIN_IDENTIFIER);
        break;
      }

      case sqlParser::BAKTIKD_IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(1817);
        match(sqlParser::BAKTIKD_IDENTIFIER);
        break;
      }

      case sqlParser::DUBLQUOTED_IDENTIFIER: {
        enterOuterAlt(_localctx, 3);
        setState(1818);
        match(sqlParser::DUBLQUOTED_IDENTIFIER);
        break;
      }

      case sqlParser::ASC:
      case sqlParser::BIT:
      case sqlParser::BITS:
      case sqlParser::BLOCK:
      case sqlParser::BLOOM:
      case sqlParser::BPA:
      case sqlParser::BTREE:
      case sqlParser::BYTE:
      case sqlParser::BYTES:
      case sqlParser::CATALOG:
      case sqlParser::CHARS:
      case sqlParser::CONCAT:
      case sqlParser::DATA:
      case sqlParser::DATE_FORMAT:
      case sqlParser::DAYS:
      case sqlParser::DECODE:
      case sqlParser::DESC:
      case sqlParser::DETAIL:
      case sqlParser::DIGITS:
      case sqlParser::DLOAD:
      case sqlParser::ECHO:
      case sqlParser::ESCAPE_CHAR:
      case sqlParser::EXPLAIN:
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
      case sqlParser::KEY:
      case sqlParser::LIMIT:
      case sqlParser::LOAD:
      case sqlParser::LOG:
      case sqlParser::LOOP:
      case sqlParser::LSIDX:
      case sqlParser::LSTBL:
      case sqlParser::MEMORYLEAKS:
      case sqlParser::MILLIS:
      case sqlParser::MILLISECOND:
      case sqlParser::MILLISECONDS:
      case sqlParser::MINUTES:
      case sqlParser::MONOTONIC:
      case sqlParser::MONTHS:
      case sqlParser::MS:
      case sqlParser::NODE:
      case sqlParser::NOOP:
      case sqlParser::NOP:
      case sqlParser::NOW:
      case sqlParser::OBJECTS:
      case sqlParser::OFF:
      case sqlParser::OFFSET:
      case sqlParser::PATHS:
      case sqlParser::PLACING:
      case sqlParser::PRETTY:
      case sqlParser::QUARTER:
      case sqlParser::QUARTERS:
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
      case sqlParser::VALUE:
      case sqlParser::WEEK:
      case sqlParser::WEEKS:
      case sqlParser::WITH_NAMES:
      case sqlParser::WITH_TYPES:
      case sqlParser::XOR:
      case sqlParser::YEARS: {
        enterOuterAlt(_localctx, 4);
        setState(1819);
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
  enterRule(_localctx, 220, sqlParser::RuleQualifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1822);
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
  enterRule(_localctx, 222, sqlParser::RulePrecision);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1824);
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
  enterRule(_localctx, 224, sqlParser::RuleScale);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1826);
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
  enterRule(_localctx, 226, sqlParser::RuleSize);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1828);
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
  enterRule(_localctx, 228, sqlParser::RuleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1840);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 224, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1830);
      match(sqlParser::SQL_NULL);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1831);
      booleanLiteral();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1832);
      signedIntegerLiteral();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(1833);
      signedDecimalLiteral();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1834);
      signedFloatLiteral();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1835);
      dateLiteral();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(1836);
      timeLiteral();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(1837);
      stringLiteral();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(1838);
      timestampLiteral();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(1839);
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
  enterRule(_localctx, 230, sqlParser::RuleSimpleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1848);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 225, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1842);
      match(sqlParser::SQL_NULL);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1843);
      booleanLiteral();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1844);
      signedIntegerLiteral();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(1845);
      signedDecimalLiteral();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1846);
      signedFloatLiteral();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1847);
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
  enterRule(_localctx, 232, sqlParser::RuleBooleanLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1850);
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
  enterRule(_localctx, 234, sqlParser::RuleIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1852);
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
  enterRule(_localctx, 236, sqlParser::RuleSignedIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1856);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1854);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1855);
        dynamic_cast<SignedIntegerLiteralContext *>(_localctx)->neg = match(sqlParser::T__6);
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1858);
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
  enterRule(_localctx, 238, sqlParser::RuleDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1860);
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
  enterRule(_localctx, 240, sqlParser::RuleSignedDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1864);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1862);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1863);
        dynamic_cast<SignedDecimalLiteralContext *>(_localctx)->neg = match(sqlParser::T__6);
        break;
      }

      case sqlParser::DECIMAL_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1866);
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
  enterRule(_localctx, 242, sqlParser::RuleFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1868);
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
  enterRule(_localctx, 244, sqlParser::RuleSignedFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1872);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1870);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1871);
        dynamic_cast<SignedFloatLiteralContext *>(_localctx)->neg = match(sqlParser::T__6);
        break;
      }

      case sqlParser::FLOAT_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1874);
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
  enterRule(_localctx, 246, sqlParser::RuleDateLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1876);
    match(sqlParser::DATE);
    setState(1877);
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
  enterRule(_localctx, 248, sqlParser::RuleTimeLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1879);
    match(sqlParser::TIME);
    setState(1880);
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
  enterRule(_localctx, 250, sqlParser::RuleTimestampLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1882);
    match(sqlParser::TIMESTAMP);
    setState(1883);
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
  enterRule(_localctx, 252, sqlParser::RuleStringLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1885);
    _la = _input->LA(1);
    if (!(((((_la - 224) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 224)) & ((1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 224))
      | (1ULL << (sqlParser::SINGLQUOTED_STRING - 224))
      | (1ULL << (sqlParser::DUBLQUOTED_STRING - 224)))) != 0))) {
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
  enterRule(_localctx, 254, sqlParser::RuleIntervalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1889);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 229, _ctx)) {
    case 1: {
      setState(1887);
      yearMonthInterval();
      break;
    }

    case 2: {
      setState(1888);
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
  enterRule(_localctx, 256, sqlParser::RuleYearMonthInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1891);
    match(sqlParser::INTERVAL);
    setState(1894);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1892);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1893);
        dynamic_cast<YearMonthIntervalContext *>(_localctx)->neg = match(sqlParser::T__6);
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
    setState(1896);
    stringLiteral();
    setState(1920);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 234, _ctx)) {
    case 1: {
      setState(1897);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1902);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 231, _ctx)) {
      case 1: {
        setState(1898);
        match(sqlParser::T__8);
        setState(1899);
        precision();
        setState(1900);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1904);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1909);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1905);
        match(sqlParser::T__8);
        setState(1906);
        precision();
        setState(1907);
        match(sqlParser::T__9);
      }
      setState(1911);
      match(sqlParser::TO);
      setState(1912);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      break;
    }

    case 3: {
      setState(1913);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      setState(1918);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 233, _ctx)) {
      case 1: {
        setState(1914);
        match(sqlParser::T__8);
        setState(1915);
        precision();
        setState(1916);
        match(sqlParser::T__9);
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
  enterRule(_localctx, 258, sqlParser::RuleDayTimeInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1922);
    match(sqlParser::INTERVAL);
    setState(1925);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1923);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1924);
        dynamic_cast<DayTimeIntervalContext *>(_localctx)->neg = match(sqlParser::T__6);
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
    setState(1927);
    stringLiteral();
    setState(2012);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 249, _ctx)) {
    case 1: {
      setState(1928);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1933);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 236, _ctx)) {
      case 1: {
        setState(1929);
        match(sqlParser::T__8);
        setState(1930);
        precision();
        setState(1931);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1935);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1940);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1936);
        match(sqlParser::T__8);
        setState(1937);
        precision();
        setState(1938);
        match(sqlParser::T__9);
      }
      setState(1942);
      match(sqlParser::TO);
      setState(1952);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::HOUR: {
          setState(1943);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
          break;
        }

        case sqlParser::MINUTE: {
          setState(1944);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1945);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1950);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 238, _ctx)) {
          case 1: {
            setState(1946);
            match(sqlParser::T__8);
            setState(1947);
            scale();
            setState(1948);
            match(sqlParser::T__9);
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
      setState(1954);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1959);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1955);
        match(sqlParser::T__8);
        setState(1956);
        precision();
        setState(1957);
        match(sqlParser::T__9);
      }
      setState(1961);
      match(sqlParser::TO);
      setState(1970);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::MINUTE: {
          setState(1962);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1963);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1968);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 241, _ctx)) {
          case 1: {
            setState(1964);
            match(sqlParser::T__8);
            setState(1965);
            scale();
            setState(1966);
            match(sqlParser::T__9);
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
      setState(1972);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1977);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1973);
        match(sqlParser::T__8);
        setState(1974);
        precision();
        setState(1975);
        match(sqlParser::T__9);
      }
      setState(1979);
      match(sqlParser::TO);

      setState(1980);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1985);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 244, _ctx)) {
      case 1: {
        setState(1981);
        match(sqlParser::T__8);
        setState(1982);
        scale();
        setState(1983);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 5: {
      setState(1987);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1992);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 245, _ctx)) {
      case 1: {
        setState(1988);
        match(sqlParser::T__8);
        setState(1989);
        precision();
        setState(1990);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 6: {
      setState(1994);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1999);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 246, _ctx)) {
      case 1: {
        setState(1995);
        match(sqlParser::T__8);
        setState(1996);
        precision();
        setState(1997);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 7: {
      setState(2001);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(2010);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 248, _ctx)) {
      case 1: {
        setState(2002);
        match(sqlParser::T__8);
        setState(2003);
        precision();
        setState(2006);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(2004);
          match(sqlParser::T__5);
          setState(2005);
          scale();
        }
        setState(2008);
        match(sqlParser::T__9);
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

//----------------- LogContext ------------------------------------------------------------------

sqlParser::LogContext::LogContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LogContext::LOG() {
  return getToken(sqlParser::LOG, 0);
}

sqlParser::StringLiteralContext* sqlParser::LogContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}

tree::TerminalNode* sqlParser::LogContext::ECHO() {
  return getToken(sqlParser::ECHO, 0);
}

tree::TerminalNode* sqlParser::LogContext::SQL_TRUE() {
  return getToken(sqlParser::SQL_TRUE, 0);
}

tree::TerminalNode* sqlParser::LogContext::ON() {
  return getToken(sqlParser::ON, 0);
}

tree::TerminalNode* sqlParser::LogContext::SQL_FALSE() {
  return getToken(sqlParser::SQL_FALSE, 0);
}

tree::TerminalNode* sqlParser::LogContext::OFF() {
  return getToken(sqlParser::OFF, 0);
}


size_t sqlParser::LogContext::getRuleIndex() const {
  return sqlParser::RuleLog;
}


antlrcpp::Any sqlParser::LogContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLog(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LogContext* sqlParser::log() {
  LogContext *_localctx = _tracker.createInstance<LogContext>(_ctx, getState());
  enterRule(_localctx, 260, sqlParser::RuleLog);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2014);
    match(sqlParser::LOG);
    setState(2015);
    dynamic_cast<LogContext *>(_localctx)->msg = stringLiteral();
    setState(2018);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ECHO) {
      setState(2016);
      match(sqlParser::ECHO);
      setState(2017);
      _la = _input->LA(1);
      if (!(_la == sqlParser::SQL_FALSE

      || _la == sqlParser::OFF || _la == sqlParser::ON

      || _la == sqlParser::SQL_TRUE)) {
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

//----------------- LscolContext ------------------------------------------------------------------

sqlParser::LscolContext::LscolContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LscolContext::LSCOL() {
  return getToken(sqlParser::LSCOL, 0);
}

sqlParser::TableNameContext* sqlParser::LscolContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}


size_t sqlParser::LscolContext::getRuleIndex() const {
  return sqlParser::RuleLscol;
}


antlrcpp::Any sqlParser::LscolContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLscol(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LscolContext* sqlParser::lscol() {
  LscolContext *_localctx = _tracker.createInstance<LscolContext>(_ctx, getState());
  enterRule(_localctx, 262, sqlParser::RuleLscol);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2020);
    match(sqlParser::LSCOL);
    setState(2021);
    tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LsidxContext ------------------------------------------------------------------

sqlParser::LsidxContext::LsidxContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LsidxContext::LSIDX() {
  return getToken(sqlParser::LSIDX, 0);
}

sqlParser::TableNameContext* sqlParser::LsidxContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}


size_t sqlParser::LsidxContext::getRuleIndex() const {
  return sqlParser::RuleLsidx;
}


antlrcpp::Any sqlParser::LsidxContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLsidx(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LsidxContext* sqlParser::lsidx() {
  LsidxContext *_localctx = _tracker.createInstance<LsidxContext>(_ctx, getState());
  enterRule(_localctx, 264, sqlParser::RuleLsidx);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2023);
    match(sqlParser::LSIDX);
    setState(2024);
    tableName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LsidxcolContext ------------------------------------------------------------------

sqlParser::LsidxcolContext::LsidxcolContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LsidxcolContext::LSIDXCOL() {
  return getToken(sqlParser::LSIDXCOL, 0);
}

sqlParser::TableNameContext* sqlParser::LsidxcolContext::tableName() {
  return getRuleContext<sqlParser::TableNameContext>(0);
}

sqlParser::IndexNameContext* sqlParser::LsidxcolContext::indexName() {
  return getRuleContext<sqlParser::IndexNameContext>(0);
}


size_t sqlParser::LsidxcolContext::getRuleIndex() const {
  return sqlParser::RuleLsidxcol;
}


antlrcpp::Any sqlParser::LsidxcolContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLsidxcol(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LsidxcolContext* sqlParser::lsidxcol() {
  LsidxcolContext *_localctx = _tracker.createInstance<LsidxcolContext>(_ctx, getState());
  enterRule(_localctx, 266, sqlParser::RuleLsidxcol);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2026);
    match(sqlParser::LSIDXCOL);
    setState(2027);
    tableName();
    setState(2028);
    indexName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LstblContext ------------------------------------------------------------------

sqlParser::LstblContext::LstblContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LstblContext::LSTBL() {
  return getToken(sqlParser::LSTBL, 0);
}


size_t sqlParser::LstblContext::getRuleIndex() const {
  return sqlParser::RuleLstbl;
}


antlrcpp::Any sqlParser::LstblContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLstbl(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LstblContext* sqlParser::lstbl() {
  LstblContext *_localctx = _tracker.createInstance<LstblContext>(_ctx, getState());
  enterRule(_localctx, 268, sqlParser::RuleLstbl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2030);
    match(sqlParser::LSTBL);
   
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

tree::TerminalNode* sqlParser::NonReservedWordContext::ECHO() {
  return getToken(sqlParser::ECHO, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::ESCAPE_CHAR() {
  return getToken(sqlParser::ESCAPE_CHAR, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::EXPLAIN() {
  return getToken(sqlParser::EXPLAIN, 0);
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

tree::TerminalNode* sqlParser::NonReservedWordContext::KEY() {
  return getToken(sqlParser::KEY, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::LIMIT() {
  return getToken(sqlParser::LIMIT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::LOAD() {
  return getToken(sqlParser::LOAD, 0);
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

tree::TerminalNode* sqlParser::NonReservedWordContext::PATHS() {
  return getToken(sqlParser::PATHS, 0);
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
  enterRule(_localctx, 270, sqlParser::RuleNonReservedWord);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(2032);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::ASC)
      | (1ULL << sqlParser::BIT)
      | (1ULL << sqlParser::BITS)
      | (1ULL << sqlParser::BLOCK)
      | (1ULL << sqlParser::BLOOM)
      | (1ULL << sqlParser::BPA)
      | (1ULL << sqlParser::BTREE)
      | (1ULL << sqlParser::BYTE)
      | (1ULL << sqlParser::BYTES)
      | (1ULL << sqlParser::CATALOG)
      | (1ULL << sqlParser::CHARS)
      | (1ULL << sqlParser::CONCAT)
      | (1ULL << sqlParser::DATA)
      | (1ULL << sqlParser::DATE_FORMAT)
      | (1ULL << sqlParser::DAYS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::DECODE - 64))
      | (1ULL << (sqlParser::DESC - 64))
      | (1ULL << (sqlParser::DETAIL - 64))
      | (1ULL << (sqlParser::DIGITS - 64))
      | (1ULL << (sqlParser::DLOAD - 64))
      | (1ULL << (sqlParser::ECHO - 64))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 64))
      | (1ULL << (sqlParser::EXPLAIN - 64))
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
      | (1ULL << (sqlParser::KEY - 64))
      | (1ULL << (sqlParser::LIMIT - 64))
      | (1ULL << (sqlParser::LOAD - 64))
      | (1ULL << (sqlParser::LOG - 64))
      | (1ULL << (sqlParser::LOOP - 64))
      | (1ULL << (sqlParser::LSIDX - 64))
      | (1ULL << (sqlParser::LSTBL - 64))
      | (1ULL << (sqlParser::MEMORYLEAKS - 64))
      | (1ULL << (sqlParser::MILLIS - 64))
      | (1ULL << (sqlParser::MILLISECOND - 64))
      | (1ULL << (sqlParser::MILLISECONDS - 64)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::MINUTES - 129))
      | (1ULL << (sqlParser::MONOTONIC - 129))
      | (1ULL << (sqlParser::MONTHS - 129))
      | (1ULL << (sqlParser::MS - 129))
      | (1ULL << (sqlParser::NODE - 129))
      | (1ULL << (sqlParser::NOOP - 129))
      | (1ULL << (sqlParser::NOP - 129))
      | (1ULL << (sqlParser::NOW - 129))
      | (1ULL << (sqlParser::OBJECTS - 129))
      | (1ULL << (sqlParser::OFF - 129))
      | (1ULL << (sqlParser::OFFSET - 129))
      | (1ULL << (sqlParser::PATHS - 129))
      | (1ULL << (sqlParser::PLACING - 129))
      | (1ULL << (sqlParser::PRETTY - 129))
      | (1ULL << (sqlParser::QUARTER - 129))
      | (1ULL << (sqlParser::QUARTERS - 129))
      | (1ULL << (sqlParser::REFERENCE - 129))
      | (1ULL << (sqlParser::REGIONS - 129))
      | (1ULL << (sqlParser::REPLICATED - 129))
      | (1ULL << (sqlParser::ROWSIZE - 129))
      | (1ULL << (sqlParser::SCHEMA - 129))
      | (1ULL << (sqlParser::SEC - 129))
      | (1ULL << (sqlParser::SECONDS - 129))
      | (1ULL << (sqlParser::SKIPLIST - 129))
      | (1ULL << (sqlParser::SLEEP - 129))
      | (1ULL << (sqlParser::SPACE - 129))
      | (1ULL << (sqlParser::STAGE - 129))
      | (1ULL << (sqlParser::STATS - 129))
      | (1ULL << (sqlParser::STATISTICS - 129))
      | (1ULL << (sqlParser::STATUS - 129))
      | (1ULL << (sqlParser::SUBSTR - 129))
      | (1ULL << (sqlParser::SZIDX - 129))
      | (1ULL << (sqlParser::SZTBL - 129))
      | (1ULL << (sqlParser::S_ONLY - 129))
      | (1ULL << (sqlParser::TABLESIZE - 129))
      | (1ULL << (sqlParser::TEXT - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::TOP - 194))
      | (1ULL << (sqlParser::TRACKMEMORY - 194))
      | (1ULL << (sqlParser::TREE - 194))
      | (1ULL << (sqlParser::TRUNCATE - 194))
      | (1ULL << (sqlParser::TYPE - 194))
      | (1ULL << (sqlParser::TYPEOF - 194))
      | (1ULL << (sqlParser::VALUE - 194))
      | (1ULL << (sqlParser::WEEK - 194))
      | (1ULL << (sqlParser::WEEKS - 194))
      | (1ULL << (sqlParser::WITH_NAMES - 194))
      | (1ULL << (sqlParser::WITH_TYPES - 194))
      | (1ULL << (sqlParser::XOR - 194))
      | (1ULL << (sqlParser::YEARS - 194)))) != 0))) {
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
    case 20: return setExprSempred(dynamic_cast<SetExprContext *>(context), predicateIndex);
    case 23: return tableExprSempred(dynamic_cast<TableExprContext *>(context), predicateIndex);
    case 89: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

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
  "explainStatement", "explainOptions", "explainStageList", "explainRange", 
  "explainRangeChoice", "explainFormat", "statisticsStatement", "queryStatement", 
  "query", "queryHint", "hint", "selectQuery", "selectExpr", "setQuery", 
  "setExpr", "corrClause", "withQuery", "tableExpr", "tableReference", "joinType", 
  "extendedJoinType", "valuesExpr", "insertQuery", "deleteQuery", "updateQuery", 
  "createQuery", "createTableQuery", "createIndexQuery", "dropQuery", "dropTableQuery", 
  "dropIndexQuery", "truncateQuery", "load", "dload", "loadOption", "loadOptionKeys", 
  "count", "sleep", "sleepUnits", "space", "status", "szidx", "sztbl", "trackmemory", 
  "memoryleaks", "bpa", "file", "noop", "emptyCommand", "tableDefinition", 
  "tablePartitioning", "columnDefinition", "columnNameList", "indexDefinition", 
  "indexMethod", "dataType", "sqlDataType", "intervalType", "yearMonthIntervalType", 
  "dayTimeIntervalType", "fixedLenChar", "varyingLenChar", "asSubquery", 
  "selectOrValues", "selectList", "selectListItem", "groupByList", "groupByListItem", 
  "orderByList", "orderByListItem", "joinUsingList", "joinUsingListItem", 
  "corrList", "corrListItem", "colAliases", "colAliasList", "colAliasListItem", 
  "propertyList", "property", "valueList", "predicate", "joinPredicate", 
  "casePredicate", "expr", "variableName", "columnRef", "columnWildcard", 
  "className", "functionName", "methodName", "catalogName", "schemaName", 
  "tableName", "tableAlias", "columnName", "columnAlias", "collationName", 
  "indexName", "propertyName", "hintName", "exprHint", "propertyValue", 
  "hintValue", "identifier", "qualifier", "precision", "scale", "size", 
  "literal", "simpleLiteral", "booleanLiteral", "integerLiteral", "signedIntegerLiteral", 
  "decimalLiteral", "signedDecimalLiteral", "floatLiteral", "signedFloatLiteral", 
  "dateLiteral", "timeLiteral", "timestampLiteral", "stringLiteral", "intervalLiteral", 
  "yearMonthInterval", "dayTimeInterval", "log", "lscol", "lsidx", "lsidxcol", 
  "lstbl", "nonReservedWord"
};

std::vector<std::string> sqlParser::_literalNames = {
  "", "';'", "'<'", "'>'", "'='", "'/'", "','", "'-'", "'*'", "'('", "')'", 
  "'?'", "'.'", "'+'", "'%'", "'||'", "'<='", "'!='", "'<>'", "'>='", "", 
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
  "", "", "", "", "", "", "", "", "", "", "", "'/*+'", "'*/'"
};

std::vector<std::string> sqlParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "ALL", "AND", "ANTI", "ANY", "AS", "ASC", "AVG", "BETWEEN", "BINARY", 
  "BIT", "BITS", "BLOB", "BLOCK", "BLOOM", "BOTH", "BOOLEAN", "BPA", "BTREE", 
  "BY", "BYTE", "BYTES", "CASE", "CAST", "CATALOG", "CHAR", "CHARS", "CHARACTER", 
  "COALESCE", "COLLATE", "CONCAT", "CORRESPONDING", "COUNT", "CREATE", "CROSS", 
  "CURRENT_CATALOG", "CURRENT_SCHEMA", "CURRENT_TIMESTAMP", "CURRENT_USER", 
  "DATA", "DATE", "DATE_FORMAT", "DAY", "DAYS", "DECIMAL", "DECODE", "DELETE", 
  "DESC", "DETAIL", "DIGITS", "DISTINCT", "DLOAD", "DROP", "ECHO", "ELSE", 
  "END", "ESCAPE", "ESCAPE_CHAR", "EXCEPT", "EXISTS", "EXPLAIN", "EXTRACT", 
  "SQL_FALSE", "FEDERATION", "FIELD_DELIMITER", "FILE", "FLOAT", "FOR", 
  "FORCE", "FORMAT", "FULL", "FROM", "GROUP", "HASH", "HASHKEY", "HAVING", 
  "HEAP", "HOUR", "HOURS", "IF", "IFNULL", "IN", "INDEX", "INNER", "INSERT", 
  "INTEGER", "INTERSECT", "INTERVAL", "INTO", "IS", "JOIN", "KEY", "LEADING", 
  "LEFT", "LIKE", "LIMIT", "LOAD", "LOG", "LOOP", "LSCOL", "LSIDX", "LSIDXCOL", 
  "LSTBL", "MAX", "MEMORYLEAKS", "MILLIS", "MILLISECOND", "MILLISECONDS", 
  "MIN", "MINUTE", "MINUTES", "MONOTONIC", "MONTH", "MONTHS", "MS", "NATURAL", 
  "NO", "NODE", "NOOP", "NOP", "NOT", "NOW", "SQL_NULL", "NULLIF", "OBJECTS", 
  "OFF", "OFFSET", "ON", "OR", "ORDER", "OUTER", "OVER", "OVERLAY", "PARTITION", 
  "PATHS", "PLACING", "POSITION", "PRETTY", "PRIMARY", "QUARTER", "QUARTERS", 
  "RECURSIVE", "REFERENCE", "REGIONS", "REPLICATED", "RIGHT", "ROW", "ROWSIZE", 
  "SCHEMA", "SEC", "SECOND", "SECONDS", "SELECT", "SEMI", "SET", "SKIPLIST", 
  "SLEEP", "SPACE", "STAGE", "STATS", "STATISTICS", "STATUS", "SUBSTR", 
  "SUBSTRING", "SUM", "SZIDX", "SZTBL", "S_ONLY", "TABLE", "TABLESIZE", 
  "TEXT", "THEN", "TIME", "TIMESTAMP", "TO", "TOP", "TRACKMEMORY", "TREE", 
  "TRIM", "TRAILING", "TRUNCATE", "SQL_TRUE", "TYPE", "TYPEOF", "UNION", 
  "UNIQUE", "UPDATE", "USING", "VALUE", "VALUES", "VARBINARY", "VARCHAR", 
  "VARYING", "WEEK", "WEEKS", "WHEN", "WHERE", "WITH_NAMES", "WITH_TYPES", 
  "WITH", "XOR", "YEAR", "YEARS", "PLAIN_IDENTIFIER", "BAKTIKD_IDENTIFIER", 
  "DUBLQUOTED_IDENTIFIER", "SINGLQUOTED_STRING", "DUBLQUOTED_STRING", "FLOAT_LITERAL", 
  "DECIMAL_LITERAL", "INTEGER_LITERAL", "HINT_START", "HINT_END", "SLASH_COMMENT", 
  "SQL_COMMENT", "BLOCK_COMMENT", "WHITESPACE"
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
    0x3, 0xed, 0x7f5, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
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
    0x87, 0x9, 0x87, 0x4, 0x88, 0x9, 0x88, 0x4, 0x89, 0x9, 0x89, 0x3, 0x2, 
    0x6, 0x2, 0x114, 0xa, 0x2, 0xd, 0x2, 0xe, 0x2, 0x115, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x5, 0x4, 0x136, 0xa, 0x4, 0x3, 0x5, 0x3, 0x5, 0x7, 0x5, 0x13a, 
    0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0x13d, 0xb, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x145, 0xa, 0x6, 0x3, 0x7, 
    0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 0x14c, 0xa, 0x7, 0x3, 
    0x8, 0x7, 0x8, 0x14f, 0xa, 0x8, 0xc, 0x8, 0xe, 0x8, 0x152, 0xb, 0x8, 
    0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 0x156, 0xa, 0x8, 0xc, 0x8, 0xe, 0x8, 0x159, 
    0xb, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
    0x5, 0x9, 0x161, 0xa, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x7, 
    0xa, 0x167, 0xa, 0xa, 0xc, 0xa, 0xe, 0xa, 0x16a, 0xb, 0xa, 0x3, 0xb, 
    0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x16f, 0xa, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 
    0xc, 0x5, 0xc, 0x174, 0xa, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 
    0x7, 0xe, 0x17a, 0xa, 0xe, 0xc, 0xe, 0xe, 0xe, 0x17d, 0xb, 0xe, 0x3, 
    0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x7, 0x10, 0x185, 
    0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 0x188, 0xb, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x5, 0x10, 0x18c, 0xa, 0x10, 0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0x190, 
    0xa, 0x11, 0x3, 0x11, 0x3, 0x11, 0x7, 0x11, 0x194, 0xa, 0x11, 0xc, 0x11, 
    0xe, 0x11, 0x197, 0xb, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x1a2, 
    0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x7, 0x12, 0x1a8, 
    0xa, 0x12, 0xc, 0x12, 0xe, 0x12, 0x1ab, 0xb, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 
    0x12, 0x1b5, 0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x7, 0x12, 0x1bb, 0xa, 0x12, 0xc, 0x12, 0xe, 0x12, 0x1be, 0xb, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x1c2, 0xa, 0x12, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x3, 0x13, 0x7, 0x13, 0x1c8, 0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 
    0x1cb, 0xb, 0x13, 0x5, 0x13, 0x1cd, 0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0x1d3, 0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0x1d9, 0xa, 0x13, 0x5, 0x13, 0x1db, 
    0xa, 0x13, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1e0, 0xa, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1e5, 0xa, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x5, 0x14, 0x1e9, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x5, 0x14, 0x1ee, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1f2, 
    0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1f8, 
    0xa, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x7, 0x15, 0x1fe, 
    0xa, 0x15, 0xc, 0x15, 0xe, 0x15, 0x201, 0xb, 0x15, 0x5, 0x15, 0x203, 
    0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x209, 
    0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x20f, 
    0xa, 0x15, 0x5, 0x15, 0x211, 0xa, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 
    0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x219, 0xa, 0x16, 0x3, 0x16, 
    0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x21f, 0xa, 0x16, 0x3, 0x16, 
    0x5, 0x16, 0x222, 0xa, 0x16, 0x3, 0x16, 0x7, 0x16, 0x225, 0xa, 0x16, 
    0xc, 0x16, 0xe, 0x16, 0x228, 0xb, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 
    0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0x230, 0xa, 0x17, 0x3, 0x18, 
    0x5, 0x18, 0x233, 0xa, 0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0x237, 
    0xa, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 
    0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x241, 0xa, 0x19, 0x3, 0x19, 
    0x5, 0x19, 0x244, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 
    0x5, 0x19, 0x24a, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x24e, 
    0xa, 0x19, 0x5, 0x19, 0x250, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x5, 0x19, 0x256, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 
    0x25a, 0xa, 0x19, 0x5, 0x19, 0x25c, 0xa, 0x19, 0x5, 0x19, 0x25e, 0xa, 
    0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 0x19, 0x262, 0xa, 0x19, 0xc, 0x19, 
    0xe, 0x19, 0x265, 0xb, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 
    0x26a, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 0x19, 0x26f, 
    0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 0x272, 0xb, 0x19, 0x3, 0x19, 0x3, 0x19, 
    0x5, 0x19, 0x276, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 
    0x7, 0x19, 0x27c, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 0x27f, 0xb, 0x19, 
    0x3, 0x19, 0x5, 0x19, 0x282, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 0x19, 0x28b, 0xa, 0x19, 
    0xc, 0x19, 0xe, 0x19, 0x28e, 0xb, 0x19, 0x3, 0x19, 0x5, 0x19, 0x291, 
    0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 
    0x19, 0x3, 0x19, 0x7, 0x19, 0x29a, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 
    0x29d, 0xb, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 0x2a2, 
    0xa, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 0x2a7, 0xa, 0x1a, 
    0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x2ae, 
    0xa, 0x1b, 0x5, 0x1b, 0x2b0, 0xa, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 
    0x5, 0x1c, 0x2b5, 0xa, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 
    0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x7, 0x1d, 0x2c0, 
    0xa, 0x1d, 0xc, 0x1d, 0xe, 0x1d, 0x2c3, 0xb, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x7, 0x1e, 0x2c9, 0xa, 0x1e, 0xc, 0x1e, 0xe, 0x1e, 
    0x2cc, 0xb, 0x1e, 0x5, 0x1e, 0x2ce, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x5, 0x1e, 0x2d2, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x7, 0x1e, 0x2d9, 0xa, 0x1e, 0xc, 0x1e, 0xe, 0x1e, 0x2dc, 
    0xb, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x2e0, 0xa, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x2e5, 0xa, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 
    0x3, 0x1f, 0x3, 0x1f, 0x7, 0x1f, 0x2eb, 0xa, 0x1f, 0xc, 0x1f, 0xe, 0x1f, 
    0x2ee, 0xb, 0x1f, 0x5, 0x1f, 0x2f0, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
    0x5, 0x1f, 0x2f4, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
    0x3, 0x1f, 0x5, 0x1f, 0x2fb, 0xa, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x3, 0x20, 0x7, 0x20, 0x301, 0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 0x304, 
    0xb, 0x20, 0x5, 0x20, 0x306, 0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 
    0x20, 0x3, 0x20, 0x7, 0x20, 0x313, 0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 
    0x316, 0xb, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x5, 0x20, 
    0x31c, 0xa, 0x20, 0x3, 0x21, 0x3, 0x21, 0x5, 0x21, 0x320, 0xa, 0x21, 
    0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x23, 0x3, 
    0x23, 0x5, 0x23, 0x329, 0xa, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 
    0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x7, 0x23, 0x333, 
    0xa, 0x23, 0xc, 0x23, 0xe, 0x23, 0x336, 0xb, 0x23, 0x3, 0x23, 0x3, 0x23, 
    0x3, 0x24, 0x3, 0x24, 0x5, 0x24, 0x33c, 0xa, 0x24, 0x3, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 
    0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x5, 0x26, 0x34b, 
    0xa, 0x26, 0x3, 0x27, 0x3, 0x27, 0x5, 0x27, 0x34f, 0xa, 0x27, 0x3, 0x27, 
    0x3, 0x27, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x5, 
    0x28, 0x358, 0xa, 0x28, 0x3, 0x28, 0x7, 0x28, 0x35b, 0xa, 0x28, 0xc, 
    0x28, 0xe, 0x28, 0x35e, 0xb, 0x28, 0x5, 0x28, 0x360, 0xa, 0x28, 0x3, 
    0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x6, 0x29, 0x366, 0xa, 0x29, 
    0xd, 0x29, 0xe, 0x29, 0x367, 0x3, 0x29, 0x3, 0x29, 0x5, 0x29, 0x36c, 
    0xa, 0x29, 0x3, 0x29, 0x7, 0x29, 0x36f, 0xa, 0x29, 0xc, 0x29, 0xe, 0x29, 
    0x372, 0xb, 0x29, 0x5, 0x29, 0x374, 0xa, 0x29, 0x3, 0x2a, 0x3, 0x2a, 
    0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x37b, 0xa, 0x2a, 0x3, 0x2a, 
    0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2c, 0x3, 0x2c, 0x3, 
    0x2c, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x5, 0x2d, 0x388, 0xa, 0x2d, 
    0x3, 0x2e, 0x3, 0x2e, 0x5, 0x2e, 0x38c, 0xa, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 
    0x3, 0x30, 0x3, 0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 
    0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x33, 0x3, 0x33, 0x5, 0x33, 0x39b, 
    0xa, 0x33, 0x3, 0x34, 0x3, 0x34, 0x3, 0x35, 0x3, 0x35, 0x5, 0x35, 0x3a1, 
    0xa, 0x35, 0x3, 0x36, 0x3, 0x36, 0x3, 0x36, 0x5, 0x36, 0x3a6, 0xa, 0x36, 
    0x3, 0x37, 0x3, 0x37, 0x3, 0x38, 0x3, 0x38, 0x3, 0x39, 0x3, 0x39, 0x3, 
    0x39, 0x5, 0x39, 0x3af, 0xa, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 
    0x5, 0x39, 0x3b4, 0xa, 0x39, 0x7, 0x39, 0x3b6, 0xa, 0x39, 0xc, 0x39, 
    0xe, 0x39, 0x3b9, 0xb, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 
    0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x7, 0x39, 0x3c2, 0xa, 0x39, 0xc, 0x39, 
    0xe, 0x39, 0x3c5, 0xb, 0x39, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x7, 0x3a, 0x3cd, 0xa, 0x3a, 0xc, 0x3a, 0xe, 0x3a, 
    0x3d0, 0xb, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 
    0x3, 0x3b, 0x7, 0x3b, 0x3d8, 0xa, 0x3b, 0xc, 0x3b, 0xe, 0x3b, 0x3db, 
    0xb, 0x3b, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x7, 0x3c, 0x3e1, 
    0xa, 0x3c, 0xc, 0x3c, 0xe, 0x3c, 0x3e4, 0xb, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 
    0x3, 0x3d, 0x5, 0x3d, 0x3e9, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 
    0x3ed, 0xa, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x3f0, 0xa, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x7, 0x3d, 0x3f7, 0xa, 0x3d, 
    0xc, 0x3d, 0xe, 0x3d, 0x3fa, 0xb, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 
    0x3fe, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x402, 0xa, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x406, 0xa, 0x3d, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3f, 0x3, 0x3f, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 
    0x40, 0x5, 0x40, 0x411, 0xa, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 
    0x415, 0xa, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 
    0x41b, 0xa, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 0x420, 
    0xa, 0x40, 0x5, 0x40, 0x422, 0xa, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 
    0x426, 0xa, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 
    0x42c, 0xa, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 0x430, 0xa, 0x40, 
    0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 
    0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 0x43c, 0xa, 0x40, 
    0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 
    0x40, 0x444, 0xa, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 0x448, 0xa, 
    0x40, 0x3, 0x40, 0x5, 0x40, 0x44b, 0xa, 0x40, 0x3, 0x41, 0x3, 0x41, 
    0x3, 0x41, 0x5, 0x41, 0x450, 0xa, 0x41, 0x3, 0x42, 0x3, 0x42, 0x3, 0x42, 
    0x3, 0x42, 0x3, 0x42, 0x5, 0x42, 0x457, 0xa, 0x42, 0x3, 0x42, 0x3, 0x42, 
    0x3, 0x42, 0x3, 0x42, 0x3, 0x42, 0x5, 0x42, 0x45e, 0xa, 0x42, 0x3, 0x42, 
    0x3, 0x42, 0x3, 0x42, 0x3, 0x42, 0x3, 0x42, 0x3, 0x42, 0x3, 0x42, 0x5, 
    0x42, 0x467, 0xa, 0x42, 0x5, 0x42, 0x469, 0xa, 0x42, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x470, 0xa, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x477, 
    0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x481, 0xa, 0x43, 0x5, 0x43, 
    0x483, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x5, 0x43, 0x48a, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x493, 0xa, 0x43, 0x5, 0x43, 
    0x495, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x5, 0x43, 0x49c, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x4a4, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x4ab, 0xa, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x4b2, 0xa, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x4b9, 
    0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x4bd, 0xa, 0x43, 0x5, 0x43, 
    0x4bf, 0xa, 0x43, 0x3, 0x44, 0x3, 0x44, 0x3, 0x44, 0x3, 0x44, 0x5, 0x44, 
    0x4c5, 0xa, 0x44, 0x3, 0x44, 0x3, 0x44, 0x3, 0x44, 0x5, 0x44, 0x4ca, 
    0xa, 0x44, 0x3, 0x45, 0x3, 0x45, 0x5, 0x45, 0x4ce, 0xa, 0x45, 0x3, 0x45, 
    0x5, 0x45, 0x4d1, 0xa, 0x45, 0x3, 0x45, 0x3, 0x45, 0x3, 0x45, 0x5, 0x45, 
    0x4d6, 0xa, 0x45, 0x3, 0x45, 0x3, 0x45, 0x5, 0x45, 0x4da, 0xa, 0x45, 
    0x3, 0x45, 0x5, 0x45, 0x4dd, 0xa, 0x45, 0x3, 0x45, 0x3, 0x45, 0x5, 0x45, 
    0x4e1, 0xa, 0x45, 0x3, 0x46, 0x3, 0x46, 0x3, 0x47, 0x3, 0x47, 0x7, 0x47, 
    0x4e7, 0xa, 0x47, 0xc, 0x47, 0xe, 0x47, 0x4ea, 0xb, 0x47, 0x3, 0x47, 
    0x3, 0x47, 0x3, 0x47, 0x3, 0x47, 0x7, 0x47, 0x4f0, 0xa, 0x47, 0xc, 0x47, 
    0xe, 0x47, 0x4f3, 0xb, 0x47, 0x3, 0x47, 0x3, 0x47, 0x3, 0x47, 0x5, 0x47, 
    0x4f8, 0xa, 0x47, 0x5, 0x47, 0x4fa, 0xa, 0x47, 0x3, 0x48, 0x3, 0x48, 
    0x3, 0x48, 0x7, 0x48, 0x4ff, 0xa, 0x48, 0xc, 0x48, 0xe, 0x48, 0x502, 
    0xb, 0x48, 0x3, 0x49, 0x3, 0x49, 0x5, 0x49, 0x506, 0xa, 0x49, 0x3, 0x49, 
    0x5, 0x49, 0x509, 0xa, 0x49, 0x3, 0x49, 0x5, 0x49, 0x50c, 0xa, 0x49, 
    0x3, 0x4a, 0x3, 0x4a, 0x3, 0x4a, 0x7, 0x4a, 0x511, 0xa, 0x4a, 0xc, 0x4a, 
    0xe, 0x4a, 0x514, 0xb, 0x4a, 0x3, 0x4b, 0x3, 0x4b, 0x3, 0x4c, 0x3, 0x4c, 
    0x3, 0x4c, 0x7, 0x4c, 0x51b, 0xa, 0x4c, 0xc, 0x4c, 0xe, 0x4c, 0x51e, 
    0xb, 0x4c, 0x3, 0x4d, 0x3, 0x4d, 0x5, 0x4d, 0x522, 0xa, 0x4d, 0x3, 0x4e, 
    0x3, 0x4e, 0x3, 0x4e, 0x7, 0x4e, 0x527, 0xa, 0x4e, 0xc, 0x4e, 0xe, 0x4e, 
    0x52a, 0xb, 0x4e, 0x3, 0x4f, 0x3, 0x4f, 0x3, 0x50, 0x3, 0x50, 0x3, 0x50, 
    0x7, 0x50, 0x531, 0xa, 0x50, 0xc, 0x50, 0xe, 0x50, 0x534, 0xb, 0x50, 
    0x3, 0x51, 0x3, 0x51, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 
    0x53, 0x3, 0x53, 0x3, 0x53, 0x7, 0x53, 0x53f, 0xa, 0x53, 0xc, 0x53, 
    0xe, 0x53, 0x542, 0xb, 0x53, 0x3, 0x54, 0x3, 0x54, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x7, 0x55, 0x549, 0xa, 0x55, 0xc, 0x55, 0xe, 0x55, 0x54c, 
    0xb, 0x55, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 0x5, 0x56, 0x551, 0xa, 0x56, 
    0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 0x7, 0x57, 0x556, 0xa, 0x57, 0xc, 0x57, 
    0xe, 0x57, 0x559, 0xb, 0x57, 0x3, 0x58, 0x3, 0x58, 0x3, 0x59, 0x3, 0x59, 
    0x3, 0x5a, 0x3, 0x5a, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x6, 
    0x5b, 0x57f, 0xa, 0x5b, 0xd, 0x5b, 0xe, 0x5b, 0x580, 0x3, 0x5b, 0x3, 
    0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x6, 0x5b, 0x588, 0xa, 0x5b, 
    0xd, 0x5b, 0xe, 0x5b, 0x589, 0x5, 0x5b, 0x58c, 0xa, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x5, 0x5b, 0x590, 0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x7, 0x5b, 0x5b0, 0xa, 0x5b, 0xc, 0x5b, 0xe, 0x5b, 0x5b3, 
    0xb, 0x5b, 0x5, 0x5b, 0x5b5, 0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x7, 0x5b, 0x5bd, 0xa, 0x5b, 0xc, 0x5b, 
    0xe, 0x5b, 0x5c0, 0xb, 0x5b, 0x5, 0x5b, 0x5c2, 0xa, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x7, 0x5b, 0x5ca, 
    0xa, 0x5b, 0xc, 0x5b, 0xe, 0x5b, 0x5cd, 0xb, 0x5b, 0x5, 0x5b, 0x5cf, 
    0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x5db, 
    0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x5ed, 0xa, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x5f4, 
    0xa, 0x5b, 0x5, 0x5b, 0x5f6, 0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x5ff, 0xa, 0x5b, 
    0x3, 0x5b, 0x5, 0x5b, 0x602, 0xa, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x605, 
    0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x609, 0xa, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x617, 
    0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x5, 0x5b, 0x61f, 0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x628, 0xa, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x5, 0x5b, 0x631, 0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x63a, 0xa, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x5, 0x5b, 0x643, 0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x7, 0x5b, 0x64d, 
    0xa, 0x5b, 0xc, 0x5b, 0xe, 0x5b, 0x650, 0xb, 0x5b, 0x5, 0x5b, 0x652, 
    0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 
    0x65f, 0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x668, 0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x675, 0xa, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x67b, 0xa, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x3, 0x5b, 0x7, 0x5b, 0x693, 0xa, 0x5b, 0xc, 0x5b, 0xe, 0x5b, 
    0x696, 0xb, 0x5b, 0x5, 0x5b, 0x698, 0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x69f, 0xa, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x6a4, 0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x6ad, 
    0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x7, 
    0x5b, 0x6b4, 0xa, 0x5b, 0xc, 0x5b, 0xe, 0x5b, 0x6b7, 0xb, 0x5b, 0x5, 
    0x5b, 0x6b9, 0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 
    0x6be, 0xa, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x7, 0x5b, 0x6c5, 0xa, 0x5b, 0xc, 0x5b, 0xe, 0x5b, 0x6c8, 0xb, 0x5b, 
    0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x6cf, 
    0xa, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x6d4, 0xa, 0x5d, 
    0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x6d9, 0xa, 0x5d, 0x3, 0x5d, 
    0x3, 0x5d, 0x3, 0x5e, 0x3, 0x5e, 0x3, 0x5e, 0x5, 0x5e, 0x6e0, 0xa, 0x5e, 
    0x3, 0x5e, 0x3, 0x5e, 0x3, 0x5e, 0x5, 0x5e, 0x6e5, 0xa, 0x5e, 0x3, 0x5e, 
    0x3, 0x5e, 0x3, 0x5e, 0x5, 0x5e, 0x6ea, 0xa, 0x5e, 0x3, 0x5e, 0x3, 0x5e, 
    0x3, 0x5f, 0x3, 0x5f, 0x3, 0x5f, 0x7, 0x5f, 0x6f1, 0xa, 0x5f, 0xc, 0x5f, 
    0xe, 0x5f, 0x6f4, 0xb, 0x5f, 0x3, 0x5f, 0x3, 0x5f, 0x3, 0x60, 0x3, 0x60, 
    0x3, 0x61, 0x3, 0x61, 0x3, 0x62, 0x3, 0x62, 0x3, 0x63, 0x3, 0x63, 0x3, 
    0x64, 0x3, 0x64, 0x3, 0x65, 0x3, 0x65, 0x3, 0x66, 0x3, 0x66, 0x3, 0x67, 
    0x3, 0x67, 0x3, 0x68, 0x3, 0x68, 0x3, 0x69, 0x3, 0x69, 0x3, 0x6a, 0x3, 
    0x6a, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6c, 0x3, 0x6c, 0x3, 0x6d, 0x3, 0x6d, 
    0x5, 0x6d, 0x714, 0xa, 0x6d, 0x3, 0x6e, 0x3, 0x6e, 0x3, 0x6e, 0x5, 0x6e, 
    0x719, 0xa, 0x6e, 0x3, 0x6f, 0x3, 0x6f, 0x3, 0x6f, 0x3, 0x6f, 0x5, 0x6f, 
    0x71f, 0xa, 0x6f, 0x3, 0x70, 0x3, 0x70, 0x3, 0x71, 0x3, 0x71, 0x3, 0x72, 
    0x3, 0x72, 0x3, 0x73, 0x3, 0x73, 0x3, 0x74, 0x3, 0x74, 0x3, 0x74, 0x3, 
    0x74, 0x3, 0x74, 0x3, 0x74, 0x3, 0x74, 0x3, 0x74, 0x3, 0x74, 0x3, 0x74, 
    0x5, 0x74, 0x733, 0xa, 0x74, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 
    0x3, 0x75, 0x3, 0x75, 0x5, 0x75, 0x73b, 0xa, 0x75, 0x3, 0x76, 0x3, 0x76, 
    0x3, 0x77, 0x3, 0x77, 0x3, 0x78, 0x3, 0x78, 0x5, 0x78, 0x743, 0xa, 0x78, 
    0x3, 0x78, 0x3, 0x78, 0x3, 0x79, 0x3, 0x79, 0x3, 0x7a, 0x3, 0x7a, 0x5, 
    0x7a, 0x74b, 0xa, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7b, 0x3, 0x7b, 
    0x3, 0x7c, 0x3, 0x7c, 0x5, 0x7c, 0x753, 0xa, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 
    0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7e, 0x3, 0x7e, 0x3, 0x7e, 0x3, 
    0x7f, 0x3, 0x7f, 0x3, 0x7f, 0x3, 0x80, 0x3, 0x80, 0x3, 0x81, 0x3, 0x81, 
    0x5, 0x81, 0x764, 0xa, 0x81, 0x3, 0x82, 0x3, 0x82, 0x3, 0x82, 0x5, 0x82, 
    0x769, 0xa, 0x82, 0x3, 0x82, 0x3, 0x82, 0x3, 0x82, 0x3, 0x82, 0x3, 0x82, 
    0x3, 0x82, 0x5, 0x82, 0x771, 0xa, 0x82, 0x3, 0x82, 0x3, 0x82, 0x3, 0x82, 
    0x3, 0x82, 0x3, 0x82, 0x5, 0x82, 0x778, 0xa, 0x82, 0x3, 0x82, 0x3, 0x82, 
    0x3, 0x82, 0x3, 0x82, 0x3, 0x82, 0x3, 0x82, 0x3, 0x82, 0x5, 0x82, 0x781, 
    0xa, 0x82, 0x5, 0x82, 0x783, 0xa, 0x82, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 
    0x5, 0x83, 0x788, 0xa, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 
    0x3, 0x83, 0x3, 0x83, 0x5, 0x83, 0x790, 0xa, 0x83, 0x3, 0x83, 0x3, 0x83, 
    0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x5, 0x83, 0x797, 0xa, 0x83, 0x3, 0x83, 
    0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 
    0x83, 0x5, 0x83, 0x7a1, 0xa, 0x83, 0x5, 0x83, 0x7a3, 0xa, 0x83, 0x3, 
    0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x5, 0x83, 0x7aa, 
    0xa, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 
    0x83, 0x3, 0x83, 0x5, 0x83, 0x7b3, 0xa, 0x83, 0x5, 0x83, 0x7b5, 0xa, 
    0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x5, 0x83, 
    0x7bc, 0xa, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 
    0x3, 0x83, 0x5, 0x83, 0x7c4, 0xa, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 
    0x3, 0x83, 0x3, 0x83, 0x5, 0x83, 0x7cb, 0xa, 0x83, 0x3, 0x83, 0x3, 0x83, 
    0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x5, 0x83, 0x7d2, 0xa, 0x83, 0x3, 0x83, 
    0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x3, 0x83, 0x5, 0x83, 0x7d9, 0xa, 0x83, 
    0x3, 0x83, 0x3, 0x83, 0x5, 0x83, 0x7dd, 0xa, 0x83, 0x5, 0x83, 0x7df, 
    0xa, 0x83, 0x3, 0x84, 0x3, 0x84, 0x3, 0x84, 0x3, 0x84, 0x5, 0x84, 0x7e5, 
    0xa, 0x84, 0x3, 0x85, 0x3, 0x85, 0x3, 0x85, 0x3, 0x86, 0x3, 0x86, 0x3, 
    0x86, 0x3, 0x87, 0x3, 0x87, 0x3, 0x87, 0x3, 0x87, 0x3, 0x88, 0x3, 0x88, 
    0x3, 0x89, 0x3, 0x89, 0x3, 0x89, 0x2, 0x5, 0x2a, 0x30, 0xb4, 0x8a, 0x2, 
    0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 
    0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 
    0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 
    0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 0x60, 0x62, 0x64, 
    0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c, 
    0x7e, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 0x90, 0x92, 0x94, 
    0x96, 0x98, 0x9a, 0x9c, 0x9e, 0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 
    0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe, 0xc0, 0xc2, 0xc4, 
    0xc6, 0xc8, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 0xda, 0xdc, 
    0xde, 0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee, 0xf0, 0xf2, 0xf4, 
    0xf6, 0xf8, 0xfa, 0xfc, 0xfe, 0x100, 0x102, 0x104, 0x106, 0x108, 0x10a, 
    0x10c, 0x10e, 0x110, 0x2, 0x26, 0x4, 0x2, 0x9, 0x9, 0xc3, 0xc3, 0x4, 
    0x2, 0xa, 0xa, 0x16, 0x16, 0x4, 0x2, 0x9e, 0x9e, 0xa7, 0xa7, 0x3, 0x2, 
    0xb4, 0xb5, 0x4, 0x2, 0xd, 0xd, 0xad, 0xad, 0x5, 0x2, 0x4f, 0x4f, 0x6b, 
    0x6b, 0xcd, 0xcd, 0x5, 0x2, 0x5b, 0x5b, 0x72, 0x72, 0xa6, 0xa6, 0x5, 
    0x2, 0x3e, 0x3e, 0x4e, 0x4e, 0x55, 0x55, 0x4, 0x2, 0xaa, 0xac, 0xbc, 
    0xbc, 0x4, 0x2, 0x7e, 0x80, 0x87, 0x87, 0x5, 0x2, 0x16, 0x16, 0x91, 
    0x91, 0xa4, 0xa4, 0x3, 0x2, 0x8b, 0x8c, 0x4, 0x2, 0x67, 0x67, 0x70, 
    0x70, 0x6, 0x2, 0x27, 0x27, 0x5e, 0x5e, 0xb0, 0xb0, 0xc6, 0xc6, 0x5, 
    0x2, 0x20, 0x20, 0x2a, 0x2a, 0x46, 0x46, 0x3, 0x2, 0x29, 0x2a, 0x4, 
    0x2, 0x2e, 0x2e, 0x30, 0x30, 0x4, 0x2, 0x29, 0x2a, 0x2e, 0x2f, 0x4, 
    0x2, 0x1b, 0x1b, 0x44, 0x44, 0x4, 0x2, 0x9, 0x9, 0xf, 0xf, 0x4, 0x2, 
    0x8, 0x8, 0x1a, 0x1a, 0x4, 0x2, 0x8, 0x8, 0x9c, 0x9c, 0x4, 0x2, 0x8, 
    0x8, 0x5c, 0x5c, 0x4, 0x2, 0x8, 0x8, 0x58, 0x58, 0x4, 0x2, 0x8, 0x8, 
    0x66, 0x66, 0x3, 0x2, 0xb7, 0xb8, 0x5, 0x2, 0x24, 0x24, 0x71, 0x71, 
    0xc8, 0xc8, 0x8, 0x2, 0x3f, 0x3f, 0x62, 0x62, 0x82, 0x82, 0x85, 0x85, 
    0xab, 0xab, 0xde, 0xde, 0x5, 0x2, 0x7, 0x7, 0xa, 0xa, 0x10, 0x10, 0x5, 
    0x2, 0x9, 0x9, 0xf, 0xf, 0x11, 0x11, 0x4, 0x2, 0x4, 0x6, 0x12, 0x15, 
    0x3, 0x2, 0x38, 0x3b, 0x4, 0x2, 0x53, 0x53, 0xca, 0xca, 0x3, 0x2, 0xe2, 
    0xe4, 0x6, 0x2, 0x53, 0x53, 0x92, 0x92, 0x94, 0x94, 0xca, 0xca, 0x33, 
    0x2, 0x1b, 0x1b, 0x1f, 0x20, 0x22, 0x23, 0x26, 0x27, 0x29, 0x2a, 0x2d, 
    0x2d, 0x2f, 0x2f, 0x33, 0x33, 0x3c, 0x3c, 0x3e, 0x3e, 0x40, 0x40, 0x42, 
    0x42, 0x44, 0x46, 0x48, 0x48, 0x4a, 0x4a, 0x4e, 0x4e, 0x51, 0x51, 0x53, 
    0x56, 0x59, 0x5a, 0x5e, 0x5f, 0x63, 0x65, 0x67, 0x67, 0x70, 0x70, 0x74, 
    0x77, 0x79, 0x79, 0x7b, 0x7b, 0x7d, 0x80, 0x83, 0x84, 0x86, 0x87, 0x8a, 
    0x8c, 0x8e, 0x8e, 0x91, 0x93, 0x9b, 0x9c, 0x9e, 0x9e, 0xa0, 0xa1, 0xa3, 
    0xa5, 0xa8, 0xaa, 0xac, 0xac, 0xb0, 0xb7, 0xba, 0xbc, 0xbe, 0xbf, 0xc4, 
    0xc6, 0xc9, 0xc9, 0xcb, 0xcc, 0xd1, 0xd1, 0xd6, 0xd7, 0xda, 0xdb, 0xdd, 
    0xdd, 0xdf, 0xdf, 0x2, 0x8e8, 0x2, 0x113, 0x3, 0x2, 0x2, 0x2, 0x4, 0x119, 
    0x3, 0x2, 0x2, 0x2, 0x6, 0x135, 0x3, 0x2, 0x2, 0x2, 0x8, 0x13b, 0x3, 
    0x2, 0x2, 0x2, 0xa, 0x13e, 0x3, 0x2, 0x2, 0x2, 0xc, 0x14b, 0x3, 0x2, 
    0x2, 0x2, 0xe, 0x150, 0x3, 0x2, 0x2, 0x2, 0x10, 0x160, 0x3, 0x2, 0x2, 
    0x2, 0x12, 0x162, 0x3, 0x2, 0x2, 0x2, 0x14, 0x16b, 0x3, 0x2, 0x2, 0x2, 
    0x16, 0x173, 0x3, 0x2, 0x2, 0x2, 0x18, 0x175, 0x3, 0x2, 0x2, 0x2, 0x1a, 
    0x17b, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x181, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x186, 
    0x3, 0x2, 0x2, 0x2, 0x20, 0x18d, 0x3, 0x2, 0x2, 0x2, 0x22, 0x1c1, 0x3, 
    0x2, 0x2, 0x2, 0x24, 0x1cc, 0x3, 0x2, 0x2, 0x2, 0x26, 0x1f7, 0x3, 0x2, 
    0x2, 0x2, 0x28, 0x202, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x218, 0x3, 0x2, 0x2, 
    0x2, 0x2c, 0x229, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x232, 0x3, 0x2, 0x2, 0x2, 
    0x30, 0x25d, 0x3, 0x2, 0x2, 0x2, 0x32, 0x2a6, 0x3, 0x2, 0x2, 0x2, 0x34, 
    0x2af, 0x3, 0x2, 0x2, 0x2, 0x36, 0x2b4, 0x3, 0x2, 0x2, 0x2, 0x38, 0x2b6, 
    0x3, 0x2, 0x2, 0x2, 0x3a, 0x2cd, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x2ef, 0x3, 
    0x2, 0x2, 0x2, 0x3e, 0x305, 0x3, 0x2, 0x2, 0x2, 0x40, 0x31f, 0x3, 0x2, 
    0x2, 0x2, 0x42, 0x321, 0x3, 0x2, 0x2, 0x2, 0x44, 0x326, 0x3, 0x2, 0x2, 
    0x2, 0x46, 0x33b, 0x3, 0x2, 0x2, 0x2, 0x48, 0x33d, 0x3, 0x2, 0x2, 0x2, 
    0x4a, 0x341, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x34c, 0x3, 0x2, 0x2, 0x2, 0x4e, 
    0x352, 0x3, 0x2, 0x2, 0x2, 0x50, 0x361, 0x3, 0x2, 0x2, 0x2, 0x52, 0x37a, 
    0x3, 0x2, 0x2, 0x2, 0x54, 0x37f, 0x3, 0x2, 0x2, 0x2, 0x56, 0x381, 0x3, 
    0x2, 0x2, 0x2, 0x58, 0x384, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x38b, 0x3, 0x2, 
    0x2, 0x2, 0x5c, 0x38d, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x38f, 0x3, 0x2, 0x2, 
    0x2, 0x60, 0x391, 0x3, 0x2, 0x2, 0x2, 0x62, 0x395, 0x3, 0x2, 0x2, 0x2, 
    0x64, 0x398, 0x3, 0x2, 0x2, 0x2, 0x66, 0x39c, 0x3, 0x2, 0x2, 0x2, 0x68, 
    0x39e, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x3a2, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x3a7, 
    0x3, 0x2, 0x2, 0x2, 0x6e, 0x3a9, 0x3, 0x2, 0x2, 0x2, 0x70, 0x3ab, 0x3, 
    0x2, 0x2, 0x2, 0x72, 0x3c6, 0x3, 0x2, 0x2, 0x2, 0x74, 0x3d3, 0x3, 0x2, 
    0x2, 0x2, 0x76, 0x3dc, 0x3, 0x2, 0x2, 0x2, 0x78, 0x3e8, 0x3, 0x2, 0x2, 
    0x2, 0x7a, 0x407, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x409, 0x3, 0x2, 0x2, 0x2, 
    0x7e, 0x44a, 0x3, 0x2, 0x2, 0x2, 0x80, 0x44c, 0x3, 0x2, 0x2, 0x2, 0x82, 
    0x468, 0x3, 0x2, 0x2, 0x2, 0x84, 0x4be, 0x3, 0x2, 0x2, 0x2, 0x86, 0x4c0, 
    0x3, 0x2, 0x2, 0x2, 0x88, 0x4dc, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x4e2, 0x3, 
    0x2, 0x2, 0x2, 0x8c, 0x4f9, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x4fb, 0x3, 0x2, 
    0x2, 0x2, 0x90, 0x50b, 0x3, 0x2, 0x2, 0x2, 0x92, 0x50d, 0x3, 0x2, 0x2, 
    0x2, 0x94, 0x515, 0x3, 0x2, 0x2, 0x2, 0x96, 0x517, 0x3, 0x2, 0x2, 0x2, 
    0x98, 0x51f, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x523, 0x3, 0x2, 0x2, 0x2, 0x9c, 
    0x52b, 0x3, 0x2, 0x2, 0x2, 0x9e, 0x52d, 0x3, 0x2, 0x2, 0x2, 0xa0, 0x535, 
    0x3, 0x2, 0x2, 0x2, 0xa2, 0x537, 0x3, 0x2, 0x2, 0x2, 0xa4, 0x53b, 0x3, 
    0x2, 0x2, 0x2, 0xa6, 0x543, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x545, 0x3, 0x2, 
    0x2, 0x2, 0xaa, 0x54d, 0x3, 0x2, 0x2, 0x2, 0xac, 0x552, 0x3, 0x2, 0x2, 
    0x2, 0xae, 0x55a, 0x3, 0x2, 0x2, 0x2, 0xb0, 0x55c, 0x3, 0x2, 0x2, 0x2, 
    0xb2, 0x55e, 0x3, 0x2, 0x2, 0x2, 0xb4, 0x667, 0x3, 0x2, 0x2, 0x2, 0xb6, 
    0x6c9, 0x3, 0x2, 0x2, 0x2, 0xb8, 0x6d8, 0x3, 0x2, 0x2, 0x2, 0xba, 0x6e9, 
    0x3, 0x2, 0x2, 0x2, 0xbc, 0x6f2, 0x3, 0x2, 0x2, 0x2, 0xbe, 0x6f7, 0x3, 
    0x2, 0x2, 0x2, 0xc0, 0x6f9, 0x3, 0x2, 0x2, 0x2, 0xc2, 0x6fb, 0x3, 0x2, 
    0x2, 0x2, 0xc4, 0x6fd, 0x3, 0x2, 0x2, 0x2, 0xc6, 0x6ff, 0x3, 0x2, 0x2, 
    0x2, 0xc8, 0x701, 0x3, 0x2, 0x2, 0x2, 0xca, 0x703, 0x3, 0x2, 0x2, 0x2, 
    0xcc, 0x705, 0x3, 0x2, 0x2, 0x2, 0xce, 0x707, 0x3, 0x2, 0x2, 0x2, 0xd0, 
    0x709, 0x3, 0x2, 0x2, 0x2, 0xd2, 0x70b, 0x3, 0x2, 0x2, 0x2, 0xd4, 0x70d, 
    0x3, 0x2, 0x2, 0x2, 0xd6, 0x70f, 0x3, 0x2, 0x2, 0x2, 0xd8, 0x713, 0x3, 
    0x2, 0x2, 0x2, 0xda, 0x718, 0x3, 0x2, 0x2, 0x2, 0xdc, 0x71e, 0x3, 0x2, 
    0x2, 0x2, 0xde, 0x720, 0x3, 0x2, 0x2, 0x2, 0xe0, 0x722, 0x3, 0x2, 0x2, 
    0x2, 0xe2, 0x724, 0x3, 0x2, 0x2, 0x2, 0xe4, 0x726, 0x3, 0x2, 0x2, 0x2, 
    0xe6, 0x732, 0x3, 0x2, 0x2, 0x2, 0xe8, 0x73a, 0x3, 0x2, 0x2, 0x2, 0xea, 
    0x73c, 0x3, 0x2, 0x2, 0x2, 0xec, 0x73e, 0x3, 0x2, 0x2, 0x2, 0xee, 0x742, 
    0x3, 0x2, 0x2, 0x2, 0xf0, 0x746, 0x3, 0x2, 0x2, 0x2, 0xf2, 0x74a, 0x3, 
    0x2, 0x2, 0x2, 0xf4, 0x74e, 0x3, 0x2, 0x2, 0x2, 0xf6, 0x752, 0x3, 0x2, 
    0x2, 0x2, 0xf8, 0x756, 0x3, 0x2, 0x2, 0x2, 0xfa, 0x759, 0x3, 0x2, 0x2, 
    0x2, 0xfc, 0x75c, 0x3, 0x2, 0x2, 0x2, 0xfe, 0x75f, 0x3, 0x2, 0x2, 0x2, 
    0x100, 0x763, 0x3, 0x2, 0x2, 0x2, 0x102, 0x765, 0x3, 0x2, 0x2, 0x2, 
    0x104, 0x784, 0x3, 0x2, 0x2, 0x2, 0x106, 0x7e0, 0x3, 0x2, 0x2, 0x2, 
    0x108, 0x7e6, 0x3, 0x2, 0x2, 0x2, 0x10a, 0x7e9, 0x3, 0x2, 0x2, 0x2, 
    0x10c, 0x7ec, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x7f0, 0x3, 0x2, 0x2, 0x2, 
    0x110, 0x7f2, 0x3, 0x2, 0x2, 0x2, 0x112, 0x114, 0x5, 0x4, 0x3, 0x2, 
    0x113, 0x112, 0x3, 0x2, 0x2, 0x2, 0x114, 0x115, 0x3, 0x2, 0x2, 0x2, 
    0x115, 0x113, 0x3, 0x2, 0x2, 0x2, 0x115, 0x116, 0x3, 0x2, 0x2, 0x2, 
    0x116, 0x117, 0x3, 0x2, 0x2, 0x2, 0x117, 0x118, 0x7, 0x2, 0x2, 0x3, 
    0x118, 0x3, 0x3, 0x2, 0x2, 0x2, 0x119, 0x11a, 0x5, 0x8, 0x5, 0x2, 0x11a, 
    0x11b, 0x5, 0x6, 0x4, 0x2, 0x11b, 0x11c, 0x7, 0x3, 0x2, 0x2, 0x11c, 
    0x5, 0x3, 0x2, 0x2, 0x2, 0x11d, 0x136, 0x5, 0x1c, 0xf, 0x2, 0x11e, 0x136, 
    0x5, 0xe, 0x8, 0x2, 0x11f, 0x136, 0x5, 0x40, 0x21, 0x2, 0x120, 0x136, 
    0x5, 0x46, 0x24, 0x2, 0x121, 0x136, 0x5, 0x4c, 0x27, 0x2, 0x122, 0x136, 
    0x5, 0x50, 0x29, 0x2, 0x123, 0x136, 0x5, 0x4e, 0x28, 0x2, 0x124, 0x136, 
    0x5, 0x106, 0x84, 0x2, 0x125, 0x136, 0x5, 0x108, 0x85, 0x2, 0x126, 0x136, 
    0x5, 0x10a, 0x86, 0x2, 0x127, 0x136, 0x5, 0x10c, 0x87, 0x2, 0x128, 0x136, 
    0x5, 0x10e, 0x88, 0x2, 0x129, 0x136, 0x5, 0x56, 0x2c, 0x2, 0x12a, 0x136, 
    0x5, 0x58, 0x2d, 0x2, 0x12b, 0x136, 0x5, 0x5c, 0x2f, 0x2, 0x12c, 0x136, 
    0x5, 0x5e, 0x30, 0x2, 0x12d, 0x136, 0x5, 0x60, 0x31, 0x2, 0x12e, 0x136, 
    0x5, 0x62, 0x32, 0x2, 0x12f, 0x136, 0x5, 0x64, 0x33, 0x2, 0x130, 0x136, 
    0x5, 0x66, 0x34, 0x2, 0x131, 0x136, 0x5, 0x68, 0x35, 0x2, 0x132, 0x136, 
    0x5, 0x6a, 0x36, 0x2, 0x133, 0x136, 0x5, 0x6c, 0x37, 0x2, 0x134, 0x136, 
    0x5, 0x6e, 0x38, 0x2, 0x135, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x135, 0x11e, 
    0x3, 0x2, 0x2, 0x2, 0x135, 0x11f, 0x3, 0x2, 0x2, 0x2, 0x135, 0x120, 
    0x3, 0x2, 0x2, 0x2, 0x135, 0x121, 0x3, 0x2, 0x2, 0x2, 0x135, 0x122, 
    0x3, 0x2, 0x2, 0x2, 0x135, 0x123, 0x3, 0x2, 0x2, 0x2, 0x135, 0x124, 
    0x3, 0x2, 0x2, 0x2, 0x135, 0x125, 0x3, 0x2, 0x2, 0x2, 0x135, 0x126, 
    0x3, 0x2, 0x2, 0x2, 0x135, 0x127, 0x3, 0x2, 0x2, 0x2, 0x135, 0x128, 
    0x3, 0x2, 0x2, 0x2, 0x135, 0x129, 0x3, 0x2, 0x2, 0x2, 0x135, 0x12a, 
    0x3, 0x2, 0x2, 0x2, 0x135, 0x12b, 0x3, 0x2, 0x2, 0x2, 0x135, 0x12c, 
    0x3, 0x2, 0x2, 0x2, 0x135, 0x12d, 0x3, 0x2, 0x2, 0x2, 0x135, 0x12e, 
    0x3, 0x2, 0x2, 0x2, 0x135, 0x12f, 0x3, 0x2, 0x2, 0x2, 0x135, 0x130, 
    0x3, 0x2, 0x2, 0x2, 0x135, 0x131, 0x3, 0x2, 0x2, 0x2, 0x135, 0x132, 
    0x3, 0x2, 0x2, 0x2, 0x135, 0x133, 0x3, 0x2, 0x2, 0x2, 0x135, 0x134, 
    0x3, 0x2, 0x2, 0x2, 0x136, 0x7, 0x3, 0x2, 0x2, 0x2, 0x137, 0x13a, 0x5, 
    0xc, 0x7, 0x2, 0x138, 0x13a, 0x5, 0xa, 0x6, 0x2, 0x139, 0x137, 0x3, 
    0x2, 0x2, 0x2, 0x139, 0x138, 0x3, 0x2, 0x2, 0x2, 0x13a, 0x13d, 0x3, 
    0x2, 0x2, 0x2, 0x13b, 0x139, 0x3, 0x2, 0x2, 0x2, 0x13b, 0x13c, 0x3, 
    0x2, 0x2, 0x2, 0x13c, 0x9, 0x3, 0x2, 0x2, 0x2, 0x13d, 0x13b, 0x3, 0x2, 
    0x2, 0x2, 0x13e, 0x13f, 0x7, 0x4, 0x2, 0x2, 0x13f, 0x140, 0x7, 0xe0, 
    0x2, 0x2, 0x140, 0x141, 0x7, 0x5, 0x2, 0x2, 0x141, 0x144, 0x7, 0x6, 
    0x2, 0x2, 0x142, 0x145, 0x5, 0xfe, 0x80, 0x2, 0x143, 0x145, 0x7, 0xe7, 
    0x2, 0x2, 0x144, 0x142, 0x3, 0x2, 0x2, 0x2, 0x144, 0x143, 0x3, 0x2, 
    0x2, 0x2, 0x145, 0xb, 0x3, 0x2, 0x2, 0x2, 0x146, 0x147, 0x7, 0x7, 0x2, 
    0x2, 0x147, 0x14c, 0x7, 0xe0, 0x2, 0x2, 0x148, 0x149, 0x7, 0x4, 0x2, 
    0x2, 0x149, 0x14a, 0x7, 0xe0, 0x2, 0x2, 0x14a, 0x14c, 0x7, 0x5, 0x2, 
    0x2, 0x14b, 0x146, 0x3, 0x2, 0x2, 0x2, 0x14b, 0x148, 0x3, 0x2, 0x2, 
    0x2, 0x14c, 0xd, 0x3, 0x2, 0x2, 0x2, 0x14d, 0x14f, 0x5, 0x20, 0x11, 
    0x2, 0x14e, 0x14d, 0x3, 0x2, 0x2, 0x2, 0x14f, 0x152, 0x3, 0x2, 0x2, 
    0x2, 0x150, 0x14e, 0x3, 0x2, 0x2, 0x2, 0x150, 0x151, 0x3, 0x2, 0x2, 
    0x2, 0x151, 0x153, 0x3, 0x2, 0x2, 0x2, 0x152, 0x150, 0x3, 0x2, 0x2, 
    0x2, 0x153, 0x157, 0x7, 0x51, 0x2, 0x2, 0x154, 0x156, 0x5, 0x10, 0x9, 
    0x2, 0x155, 0x154, 0x3, 0x2, 0x2, 0x2, 0x156, 0x159, 0x3, 0x2, 0x2, 
    0x2, 0x157, 0x155, 0x3, 0x2, 0x2, 0x2, 0x157, 0x158, 0x3, 0x2, 0x2, 
    0x2, 0x158, 0x15a, 0x3, 0x2, 0x2, 0x2, 0x159, 0x157, 0x3, 0x2, 0x2, 
    0x2, 0x15a, 0x15b, 0x5, 0x6, 0x4, 0x2, 0x15b, 0xf, 0x3, 0x2, 0x2, 0x2, 
    0x15c, 0x161, 0x5, 0x12, 0xa, 0x2, 0x15d, 0x161, 0x5, 0x18, 0xd, 0x2, 
    0x15e, 0x161, 0x7, 0xda, 0x2, 0x2, 0x15f, 0x161, 0x7, 0xdb, 0x2, 0x2, 
    0x160, 0x15c, 0x3, 0x2, 0x2, 0x2, 0x160, 0x15d, 0x3, 0x2, 0x2, 0x2, 
    0x160, 0x15e, 0x3, 0x2, 0x2, 0x2, 0x160, 0x15f, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x11, 0x3, 0x2, 0x2, 0x2, 0x162, 0x163, 0x7, 0xb3, 0x2, 0x2, 
    0x163, 0x168, 0x5, 0x14, 0xb, 0x2, 0x164, 0x165, 0x7, 0x8, 0x2, 0x2, 
    0x165, 0x167, 0x5, 0x14, 0xb, 0x2, 0x166, 0x164, 0x3, 0x2, 0x2, 0x2, 
    0x167, 0x16a, 0x3, 0x2, 0x2, 0x2, 0x168, 0x166, 0x3, 0x2, 0x2, 0x2, 
    0x168, 0x169, 0x3, 0x2, 0x2, 0x2, 0x169, 0x13, 0x3, 0x2, 0x2, 0x2, 0x16a, 
    0x168, 0x3, 0x2, 0x2, 0x2, 0x16b, 0x16e, 0x5, 0x16, 0xc, 0x2, 0x16c, 
    0x16d, 0x9, 0x2, 0x2, 0x2, 0x16d, 0x16f, 0x5, 0x16, 0xc, 0x2, 0x16e, 
    0x16c, 0x3, 0x2, 0x2, 0x2, 0x16e, 0x16f, 0x3, 0x2, 0x2, 0x2, 0x16f, 
    0x15, 0x3, 0x2, 0x2, 0x2, 0x170, 0x174, 0x9, 0x3, 0x2, 0x2, 0x171, 0x174, 
    0x7, 0xe7, 0x2, 0x2, 0x172, 0x174, 0x5, 0xdc, 0x6f, 0x2, 0x173, 0x170, 
    0x3, 0x2, 0x2, 0x2, 0x173, 0x171, 0x3, 0x2, 0x2, 0x2, 0x173, 0x172, 
    0x3, 0x2, 0x2, 0x2, 0x174, 0x17, 0x3, 0x2, 0x2, 0x2, 0x175, 0x176, 0x7, 
    0x5a, 0x2, 0x2, 0x176, 0x177, 0x9, 0x4, 0x2, 0x2, 0x177, 0x19, 0x3, 
    0x2, 0x2, 0x2, 0x178, 0x17a, 0x5, 0x20, 0x11, 0x2, 0x179, 0x178, 0x3, 
    0x2, 0x2, 0x2, 0x17a, 0x17d, 0x3, 0x2, 0x2, 0x2, 0x17b, 0x179, 0x3, 
    0x2, 0x2, 0x2, 0x17b, 0x17c, 0x3, 0x2, 0x2, 0x2, 0x17c, 0x17e, 0x3, 
    0x2, 0x2, 0x2, 0x17d, 0x17b, 0x3, 0x2, 0x2, 0x2, 0x17e, 0x17f, 0x9, 
    0x5, 0x2, 0x2, 0x17f, 0x180, 0x5, 0x6, 0x4, 0x2, 0x180, 0x1b, 0x3, 0x2, 
    0x2, 0x2, 0x181, 0x182, 0x5, 0x1e, 0x10, 0x2, 0x182, 0x1d, 0x3, 0x2, 
    0x2, 0x2, 0x183, 0x185, 0x5, 0x20, 0x11, 0x2, 0x184, 0x183, 0x3, 0x2, 
    0x2, 0x2, 0x185, 0x188, 0x3, 0x2, 0x2, 0x2, 0x186, 0x184, 0x3, 0x2, 
    0x2, 0x2, 0x186, 0x187, 0x3, 0x2, 0x2, 0x2, 0x187, 0x18b, 0x3, 0x2, 
    0x2, 0x2, 0x188, 0x186, 0x3, 0x2, 0x2, 0x2, 0x189, 0x18c, 0x5, 0x24, 
    0x13, 0x2, 0x18a, 0x18c, 0x5, 0x28, 0x15, 0x2, 0x18b, 0x189, 0x3, 0x2, 
    0x2, 0x2, 0x18b, 0x18a, 0x3, 0x2, 0x2, 0x2, 0x18c, 0x1f, 0x3, 0x2, 0x2, 
    0x2, 0x18d, 0x18f, 0x7, 0xe8, 0x2, 0x2, 0x18e, 0x190, 0x5, 0x22, 0x12, 
    0x2, 0x18f, 0x18e, 0x3, 0x2, 0x2, 0x2, 0x18f, 0x190, 0x3, 0x2, 0x2, 
    0x2, 0x190, 0x195, 0x3, 0x2, 0x2, 0x2, 0x191, 0x192, 0x7, 0x8, 0x2, 
    0x2, 0x192, 0x194, 0x5, 0x22, 0x12, 0x2, 0x193, 0x191, 0x3, 0x2, 0x2, 
    0x2, 0x194, 0x197, 0x3, 0x2, 0x2, 0x2, 0x195, 0x193, 0x3, 0x2, 0x2, 
    0x2, 0x195, 0x196, 0x3, 0x2, 0x2, 0x2, 0x196, 0x198, 0x3, 0x2, 0x2, 
    0x2, 0x197, 0x195, 0x3, 0x2, 0x2, 0x2, 0x198, 0x199, 0x7, 0xe9, 0x2, 
    0x2, 0x199, 0x21, 0x3, 0x2, 0x2, 0x2, 0x19a, 0x1c2, 0x5, 0xd4, 0x6b, 
    0x2, 0x19b, 0x19c, 0x5, 0xd6, 0x6c, 0x2, 0x19c, 0x19d, 0x7, 0x6, 0x2, 
    0x2, 0x19d, 0x19e, 0x5, 0xb4, 0x5b, 0x2, 0x19e, 0x1c2, 0x3, 0x2, 0x2, 
    0x2, 0x19f, 0x1a1, 0x5, 0xd6, 0x6c, 0x2, 0x1a0, 0x1a2, 0x7, 0x6, 0x2, 
    0x2, 0x1a1, 0x1a0, 0x3, 0x2, 0x2, 0x2, 0x1a1, 0x1a2, 0x3, 0x2, 0x2, 
    0x2, 0x1a2, 0x1a3, 0x3, 0x2, 0x2, 0x2, 0x1a3, 0x1a4, 0x7, 0xb, 0x2, 
    0x2, 0x1a4, 0x1a9, 0x5, 0xb4, 0x5b, 0x2, 0x1a5, 0x1a6, 0x7, 0x8, 0x2, 
    0x2, 0x1a6, 0x1a8, 0x5, 0xb4, 0x5b, 0x2, 0x1a7, 0x1a5, 0x3, 0x2, 0x2, 
    0x2, 0x1a8, 0x1ab, 0x3, 0x2, 0x2, 0x2, 0x1a9, 0x1a7, 0x3, 0x2, 0x2, 
    0x2, 0x1a9, 0x1aa, 0x3, 0x2, 0x2, 0x2, 0x1aa, 0x1ac, 0x3, 0x2, 0x2, 
    0x2, 0x1ab, 0x1a9, 0x3, 0x2, 0x2, 0x2, 0x1ac, 0x1ad, 0x7, 0xc, 0x2, 
    0x2, 0x1ad, 0x1c2, 0x3, 0x2, 0x2, 0x2, 0x1ae, 0x1af, 0x5, 0xd4, 0x6b, 
    0x2, 0x1af, 0x1b0, 0x7, 0x6, 0x2, 0x2, 0x1b0, 0x1b1, 0x5, 0xda, 0x6e, 
    0x2, 0x1b1, 0x1c2, 0x3, 0x2, 0x2, 0x2, 0x1b2, 0x1b4, 0x5, 0xd4, 0x6b, 
    0x2, 0x1b3, 0x1b5, 0x7, 0x6, 0x2, 0x2, 0x1b4, 0x1b3, 0x3, 0x2, 0x2, 
    0x2, 0x1b4, 0x1b5, 0x3, 0x2, 0x2, 0x2, 0x1b5, 0x1b6, 0x3, 0x2, 0x2, 
    0x2, 0x1b6, 0x1b7, 0x7, 0xb, 0x2, 0x2, 0x1b7, 0x1bc, 0x5, 0xda, 0x6e, 
    0x2, 0x1b8, 0x1b9, 0x7, 0x8, 0x2, 0x2, 0x1b9, 0x1bb, 0x5, 0xda, 0x6e, 
    0x2, 0x1ba, 0x1b8, 0x3, 0x2, 0x2, 0x2, 0x1bb, 0x1be, 0x3, 0x2, 0x2, 
    0x2, 0x1bc, 0x1ba, 0x3, 0x2, 0x2, 0x2, 0x1bc, 0x1bd, 0x3, 0x2, 0x2, 
    0x2, 0x1bd, 0x1bf, 0x3, 0x2, 0x2, 0x2, 0x1be, 0x1bc, 0x3, 0x2, 0x2, 
    0x2, 0x1bf, 0x1c0, 0x7, 0xc, 0x2, 0x2, 0x1c0, 0x1c2, 0x3, 0x2, 0x2, 
    0x2, 0x1c1, 0x19a, 0x3, 0x2, 0x2, 0x2, 0x1c1, 0x19b, 0x3, 0x2, 0x2, 
    0x2, 0x1c1, 0x19f, 0x3, 0x2, 0x2, 0x2, 0x1c1, 0x1ae, 0x3, 0x2, 0x2, 
    0x2, 0x1c1, 0x1b2, 0x3, 0x2, 0x2, 0x2, 0x1c2, 0x23, 0x3, 0x2, 0x2, 0x2, 
    0x1c3, 0x1c4, 0x7, 0xdc, 0x2, 0x2, 0x1c4, 0x1c9, 0x5, 0x2e, 0x18, 0x2, 
    0x1c5, 0x1c6, 0x7, 0x8, 0x2, 0x2, 0x1c6, 0x1c8, 0x5, 0x2e, 0x18, 0x2, 
    0x1c7, 0x1c5, 0x3, 0x2, 0x2, 0x2, 0x1c8, 0x1cb, 0x3, 0x2, 0x2, 0x2, 
    0x1c9, 0x1c7, 0x3, 0x2, 0x2, 0x2, 0x1c9, 0x1ca, 0x3, 0x2, 0x2, 0x2, 
    0x1ca, 0x1cd, 0x3, 0x2, 0x2, 0x2, 0x1cb, 0x1c9, 0x3, 0x2, 0x2, 0x2, 
    0x1cc, 0x1c3, 0x3, 0x2, 0x2, 0x2, 0x1cc, 0x1cd, 0x3, 0x2, 0x2, 0x2, 
    0x1cd, 0x1ce, 0x3, 0x2, 0x2, 0x2, 0x1ce, 0x1d2, 0x5, 0x26, 0x14, 0x2, 
    0x1cf, 0x1d0, 0x7, 0x96, 0x2, 0x2, 0x1d0, 0x1d1, 0x7, 0x28, 0x2, 0x2, 
    0x1d1, 0x1d3, 0x5, 0x96, 0x4c, 0x2, 0x1d2, 0x1cf, 0x3, 0x2, 0x2, 0x2, 
    0x1d2, 0x1d3, 0x3, 0x2, 0x2, 0x2, 0x1d3, 0x1da, 0x3, 0x2, 0x2, 0x2, 
    0x1d4, 0x1d5, 0x7, 0x74, 0x2, 0x2, 0x1d5, 0x1d8, 0x7, 0xe7, 0x2, 0x2, 
    0x1d6, 0x1d7, 0x7, 0x93, 0x2, 0x2, 0x1d7, 0x1d9, 0x7, 0xe7, 0x2, 0x2, 
    0x1d8, 0x1d6, 0x3, 0x2, 0x2, 0x2, 0x1d8, 0x1d9, 0x3, 0x2, 0x2, 0x2, 
    0x1d9, 0x1db, 0x3, 0x2, 0x2, 0x2, 0x1da, 0x1d4, 0x3, 0x2, 0x2, 0x2, 
    0x1da, 0x1db, 0x3, 0x2, 0x2, 0x2, 0x1db, 0x25, 0x3, 0x2, 0x2, 0x2, 0x1dc, 
    0x1df, 0x9, 0x6, 0x2, 0x2, 0x1dd, 0x1e0, 0x7, 0x16, 0x2, 0x2, 0x1de, 
    0x1e0, 0x7, 0x47, 0x2, 0x2, 0x1df, 0x1dd, 0x3, 0x2, 0x2, 0x2, 0x1df, 
    0x1de, 0x3, 0x2, 0x2, 0x2, 0x1df, 0x1e0, 0x3, 0x2, 0x2, 0x2, 0x1e0, 
    0x1e1, 0x3, 0x2, 0x2, 0x2, 0x1e1, 0x1e4, 0x5, 0x8e, 0x48, 0x2, 0x1e2, 
    0x1e3, 0x7, 0x5c, 0x2, 0x2, 0x1e3, 0x1e5, 0x5, 0x30, 0x19, 0x2, 0x1e4, 
    0x1e2, 0x3, 0x2, 0x2, 0x2, 0x1e4, 0x1e5, 0x3, 0x2, 0x2, 0x2, 0x1e5, 
    0x1e8, 0x3, 0x2, 0x2, 0x2, 0x1e6, 0x1e7, 0x7, 0xd9, 0x2, 0x2, 0x1e7, 
    0x1e9, 0x5, 0xae, 0x58, 0x2, 0x1e8, 0x1e6, 0x3, 0x2, 0x2, 0x2, 0x1e8, 
    0x1e9, 0x3, 0x2, 0x2, 0x2, 0x1e9, 0x1ed, 0x3, 0x2, 0x2, 0x2, 0x1ea, 
    0x1eb, 0x7, 0x5d, 0x2, 0x2, 0x1eb, 0x1ec, 0x7, 0x28, 0x2, 0x2, 0x1ec, 
    0x1ee, 0x5, 0x92, 0x4a, 0x2, 0x1ed, 0x1ea, 0x3, 0x2, 0x2, 0x2, 0x1ed, 
    0x1ee, 0x3, 0x2, 0x2, 0x2, 0x1ee, 0x1f1, 0x3, 0x2, 0x2, 0x2, 0x1ef, 
    0x1f0, 0x7, 0x60, 0x2, 0x2, 0x1f0, 0x1f2, 0x5, 0xae, 0x58, 0x2, 0x1f1, 
    0x1ef, 0x3, 0x2, 0x2, 0x2, 0x1f1, 0x1f2, 0x3, 0x2, 0x2, 0x2, 0x1f2, 
    0x1f8, 0x3, 0x2, 0x2, 0x2, 0x1f3, 0x1f4, 0x7, 0xb, 0x2, 0x2, 0x1f4, 
    0x1f5, 0x5, 0x26, 0x14, 0x2, 0x1f5, 0x1f6, 0x7, 0xc, 0x2, 0x2, 0x1f6, 
    0x1f8, 0x3, 0x2, 0x2, 0x2, 0x1f7, 0x1dc, 0x3, 0x2, 0x2, 0x2, 0x1f7, 
    0x1f3, 0x3, 0x2, 0x2, 0x2, 0x1f8, 0x27, 0x3, 0x2, 0x2, 0x2, 0x1f9, 0x1fa, 
    0x7, 0xdc, 0x2, 0x2, 0x1fa, 0x1ff, 0x5, 0x2e, 0x18, 0x2, 0x1fb, 0x1fc, 
    0x7, 0x8, 0x2, 0x2, 0x1fc, 0x1fe, 0x5, 0x2e, 0x18, 0x2, 0x1fd, 0x1fb, 
    0x3, 0x2, 0x2, 0x2, 0x1fe, 0x201, 0x3, 0x2, 0x2, 0x2, 0x1ff, 0x1fd, 
    0x3, 0x2, 0x2, 0x2, 0x1ff, 0x200, 0x3, 0x2, 0x2, 0x2, 0x200, 0x203, 
    0x3, 0x2, 0x2, 0x2, 0x201, 0x1ff, 0x3, 0x2, 0x2, 0x2, 0x202, 0x1f9, 
    0x3, 0x2, 0x2, 0x2, 0x202, 0x203, 0x3, 0x2, 0x2, 0x2, 0x203, 0x204, 
    0x3, 0x2, 0x2, 0x2, 0x204, 0x208, 0x5, 0x2a, 0x16, 0x2, 0x205, 0x206, 
    0x7, 0x96, 0x2, 0x2, 0x206, 0x207, 0x7, 0x28, 0x2, 0x2, 0x207, 0x209, 
    0x5, 0x96, 0x4c, 0x2, 0x208, 0x205, 0x3, 0x2, 0x2, 0x2, 0x208, 0x209, 
    0x3, 0x2, 0x2, 0x2, 0x209, 0x210, 0x3, 0x2, 0x2, 0x2, 0x20a, 0x20b, 
    0x7, 0x74, 0x2, 0x2, 0x20b, 0x20e, 0x7, 0xe7, 0x2, 0x2, 0x20c, 0x20d, 
    0x7, 0x93, 0x2, 0x2, 0x20d, 0x20f, 0x7, 0xe7, 0x2, 0x2, 0x20e, 0x20c, 
    0x3, 0x2, 0x2, 0x2, 0x20e, 0x20f, 0x3, 0x2, 0x2, 0x2, 0x20f, 0x211, 
    0x3, 0x2, 0x2, 0x2, 0x210, 0x20a, 0x3, 0x2, 0x2, 0x2, 0x210, 0x211, 
    0x3, 0x2, 0x2, 0x2, 0x211, 0x29, 0x3, 0x2, 0x2, 0x2, 0x212, 0x213, 0x8, 
    0x16, 0x1, 0x2, 0x213, 0x219, 0x5, 0x26, 0x14, 0x2, 0x214, 0x215, 0x7, 
    0xb, 0x2, 0x2, 0x215, 0x216, 0x5, 0x2a, 0x16, 0x2, 0x216, 0x217, 0x7, 
    0xc, 0x2, 0x2, 0x217, 0x219, 0x3, 0x2, 0x2, 0x2, 0x218, 0x212, 0x3, 
    0x2, 0x2, 0x2, 0x218, 0x214, 0x3, 0x2, 0x2, 0x2, 0x219, 0x226, 0x3, 
    0x2, 0x2, 0x2, 0x21a, 0x21b, 0xc, 0x4, 0x2, 0x2, 0x21b, 0x21e, 0x9, 
    0x7, 0x2, 0x2, 0x21c, 0x21f, 0x7, 0x47, 0x2, 0x2, 0x21d, 0x21f, 0x7, 
    0x16, 0x2, 0x2, 0x21e, 0x21c, 0x3, 0x2, 0x2, 0x2, 0x21e, 0x21d, 0x3, 
    0x2, 0x2, 0x2, 0x21e, 0x21f, 0x3, 0x2, 0x2, 0x2, 0x21f, 0x221, 0x3, 
    0x2, 0x2, 0x2, 0x220, 0x222, 0x5, 0x2c, 0x17, 0x2, 0x221, 0x220, 0x3, 
    0x2, 0x2, 0x2, 0x221, 0x222, 0x3, 0x2, 0x2, 0x2, 0x222, 0x223, 0x3, 
    0x2, 0x2, 0x2, 0x223, 0x225, 0x5, 0x2a, 0x16, 0x5, 0x224, 0x21a, 0x3, 
    0x2, 0x2, 0x2, 0x225, 0x228, 0x3, 0x2, 0x2, 0x2, 0x226, 0x224, 0x3, 
    0x2, 0x2, 0x2, 0x226, 0x227, 0x3, 0x2, 0x2, 0x2, 0x227, 0x2b, 0x3, 0x2, 
    0x2, 0x2, 0x228, 0x226, 0x3, 0x2, 0x2, 0x2, 0x229, 0x22f, 0x7, 0x34, 
    0x2, 0x2, 0x22a, 0x22b, 0x7, 0x28, 0x2, 0x2, 0x22b, 0x22c, 0x7, 0xb, 
    0x2, 0x2, 0x22c, 0x22d, 0x5, 0x9e, 0x50, 0x2, 0x22d, 0x22e, 0x7, 0xc, 
    0x2, 0x2, 0x22e, 0x230, 0x3, 0x2, 0x2, 0x2, 0x22f, 0x22a, 0x3, 0x2, 
    0x2, 0x2, 0x22f, 0x230, 0x3, 0x2, 0x2, 0x2, 0x230, 0x2d, 0x3, 0x2, 0x2, 
    0x2, 0x231, 0x233, 0x7, 0xa2, 0x2, 0x2, 0x232, 0x231, 0x3, 0x2, 0x2, 
    0x2, 0x232, 0x233, 0x3, 0x2, 0x2, 0x2, 0x233, 0x234, 0x3, 0x2, 0x2, 
    0x2, 0x234, 0x236, 0x5, 0xc6, 0x64, 0x2, 0x235, 0x237, 0x5, 0xa2, 0x52, 
    0x2, 0x236, 0x235, 0x3, 0x2, 0x2, 0x2, 0x236, 0x237, 0x3, 0x2, 0x2, 
    0x2, 0x237, 0x238, 0x3, 0x2, 0x2, 0x2, 0x238, 0x239, 0x7, 0x1a, 0x2, 
    0x2, 0x239, 0x23a, 0x7, 0xb, 0x2, 0x2, 0x23a, 0x23b, 0x5, 0x1e, 0x10, 
    0x2, 0x23b, 0x23c, 0x7, 0xc, 0x2, 0x2, 0x23c, 0x2f, 0x3, 0x2, 0x2, 0x2, 
    0x23d, 0x23e, 0x8, 0x19, 0x1, 0x2, 0x23e, 0x243, 0x5, 0x32, 0x1a, 0x2, 
    0x23f, 0x241, 0x7, 0x1a, 0x2, 0x2, 0x240, 0x23f, 0x3, 0x2, 0x2, 0x2, 
    0x240, 0x241, 0x3, 0x2, 0x2, 0x2, 0x241, 0x242, 0x3, 0x2, 0x2, 0x2, 
    0x242, 0x244, 0x5, 0xc8, 0x65, 0x2, 0x243, 0x240, 0x3, 0x2, 0x2, 0x2, 
    0x243, 0x244, 0x3, 0x2, 0x2, 0x2, 0x244, 0x25e, 0x3, 0x2, 0x2, 0x2, 
    0x245, 0x246, 0x7, 0xb, 0x2, 0x2, 0x246, 0x247, 0x5, 0x1e, 0x10, 0x2, 
    0x247, 0x24f, 0x7, 0xc, 0x2, 0x2, 0x248, 0x24a, 0x7, 0x1a, 0x2, 0x2, 
    0x249, 0x248, 0x3, 0x2, 0x2, 0x2, 0x249, 0x24a, 0x3, 0x2, 0x2, 0x2, 
    0x24a, 0x24b, 0x3, 0x2, 0x2, 0x2, 0x24b, 0x24d, 0x5, 0xc8, 0x65, 0x2, 
    0x24c, 0x24e, 0x5, 0xa2, 0x52, 0x2, 0x24d, 0x24c, 0x3, 0x2, 0x2, 0x2, 
    0x24d, 0x24e, 0x3, 0x2, 0x2, 0x2, 0x24e, 0x250, 0x3, 0x2, 0x2, 0x2, 
    0x24f, 0x249, 0x3, 0x2, 0x2, 0x2, 0x24f, 0x250, 0x3, 0x2, 0x2, 0x2, 
    0x250, 0x25e, 0x3, 0x2, 0x2, 0x2, 0x251, 0x252, 0x7, 0xb, 0x2, 0x2, 
    0x252, 0x253, 0x5, 0x38, 0x1d, 0x2, 0x253, 0x25b, 0x7, 0xc, 0x2, 0x2, 
    0x254, 0x256, 0x7, 0x1a, 0x2, 0x2, 0x255, 0x254, 0x3, 0x2, 0x2, 0x2, 
    0x255, 0x256, 0x3, 0x2, 0x2, 0x2, 0x256, 0x257, 0x3, 0x2, 0x2, 0x2, 
    0x257, 0x259, 0x5, 0xc8, 0x65, 0x2, 0x258, 0x25a, 0x5, 0xa2, 0x52, 0x2, 
    0x259, 0x258, 0x3, 0x2, 0x2, 0x2, 0x259, 0x25a, 0x3, 0x2, 0x2, 0x2, 
    0x25a, 0x25c, 0x3, 0x2, 0x2, 0x2, 0x25b, 0x255, 0x3, 0x2, 0x2, 0x2, 
    0x25b, 0x25c, 0x3, 0x2, 0x2, 0x2, 0x25c, 0x25e, 0x3, 0x2, 0x2, 0x2, 
    0x25d, 0x23d, 0x3, 0x2, 0x2, 0x2, 0x25d, 0x245, 0x3, 0x2, 0x2, 0x2, 
    0x25d, 0x251, 0x3, 0x2, 0x2, 0x2, 0x25e, 0x29b, 0x3, 0x2, 0x2, 0x2, 
    0x25f, 0x263, 0xc, 0x8, 0x2, 0x2, 0x260, 0x262, 0x5, 0x20, 0x11, 0x2, 
    0x261, 0x260, 0x3, 0x2, 0x2, 0x2, 0x262, 0x265, 0x3, 0x2, 0x2, 0x2, 
    0x263, 0x261, 0x3, 0x2, 0x2, 0x2, 0x263, 0x264, 0x3, 0x2, 0x2, 0x2, 
    0x264, 0x269, 0x3, 0x2, 0x2, 0x2, 0x265, 0x263, 0x3, 0x2, 0x2, 0x2, 
    0x266, 0x26a, 0x7, 0x8, 0x2, 0x2, 0x267, 0x268, 0x7, 0x37, 0x2, 0x2, 
    0x268, 0x26a, 0x7, 0x6f, 0x2, 0x2, 0x269, 0x266, 0x3, 0x2, 0x2, 0x2, 
    0x269, 0x267, 0x3, 0x2, 0x2, 0x2, 0x26a, 0x26b, 0x3, 0x2, 0x2, 0x2, 
    0x26b, 0x29a, 0x5, 0x30, 0x19, 0x9, 0x26c, 0x270, 0xc, 0x7, 0x2, 0x2, 
    0x26d, 0x26f, 0x5, 0x20, 0x11, 0x2, 0x26e, 0x26d, 0x3, 0x2, 0x2, 0x2, 
    0x26f, 0x272, 0x3, 0x2, 0x2, 0x2, 0x270, 0x26e, 0x3, 0x2, 0x2, 0x2, 
    0x270, 0x271, 0x3, 0x2, 0x2, 0x2, 0x271, 0x273, 0x3, 0x2, 0x2, 0x2, 
    0x272, 0x270, 0x3, 0x2, 0x2, 0x2, 0x273, 0x275, 0x7, 0x88, 0x2, 0x2, 
    0x274, 0x276, 0x5, 0x34, 0x1b, 0x2, 0x275, 0x274, 0x3, 0x2, 0x2, 0x2, 
    0x275, 0x276, 0x3, 0x2, 0x2, 0x2, 0x276, 0x277, 0x3, 0x2, 0x2, 0x2, 
    0x277, 0x278, 0x7, 0x6f, 0x2, 0x2, 0x278, 0x29a, 0x5, 0x30, 0x19, 0x8, 
    0x279, 0x27d, 0xc, 0x6, 0x2, 0x2, 0x27a, 0x27c, 0x5, 0x20, 0x11, 0x2, 
    0x27b, 0x27a, 0x3, 0x2, 0x2, 0x2, 0x27c, 0x27f, 0x3, 0x2, 0x2, 0x2, 
    0x27d, 0x27b, 0x3, 0x2, 0x2, 0x2, 0x27d, 0x27e, 0x3, 0x2, 0x2, 0x2, 
    0x27e, 0x281, 0x3, 0x2, 0x2, 0x2, 0x27f, 0x27d, 0x3, 0x2, 0x2, 0x2, 
    0x280, 0x282, 0x5, 0x36, 0x1c, 0x2, 0x281, 0x280, 0x3, 0x2, 0x2, 0x2, 
    0x281, 0x282, 0x3, 0x2, 0x2, 0x2, 0x282, 0x283, 0x3, 0x2, 0x2, 0x2, 
    0x283, 0x284, 0x7, 0x6f, 0x2, 0x2, 0x284, 0x285, 0x5, 0x30, 0x19, 0x2, 
    0x285, 0x286, 0x7, 0x94, 0x2, 0x2, 0x286, 0x287, 0x5, 0xb0, 0x59, 0x2, 
    0x287, 0x29a, 0x3, 0x2, 0x2, 0x2, 0x288, 0x28c, 0xc, 0x5, 0x2, 0x2, 
    0x289, 0x28b, 0x5, 0x20, 0x11, 0x2, 0x28a, 0x289, 0x3, 0x2, 0x2, 0x2, 
    0x28b, 0x28e, 0x3, 0x2, 0x2, 0x2, 0x28c, 0x28a, 0x3, 0x2, 0x2, 0x2, 
    0x28c, 0x28d, 0x3, 0x2, 0x2, 0x2, 0x28d, 0x290, 0x3, 0x2, 0x2, 0x2, 
    0x28e, 0x28c, 0x3, 0x2, 0x2, 0x2, 0x28f, 0x291, 0x5, 0x36, 0x1c, 0x2, 
    0x290, 0x28f, 0x3, 0x2, 0x2, 0x2, 0x290, 0x291, 0x3, 0x2, 0x2, 0x2, 
    0x291, 0x292, 0x3, 0x2, 0x2, 0x2, 0x292, 0x293, 0x7, 0x6f, 0x2, 0x2, 
    0x293, 0x294, 0x5, 0x30, 0x19, 0x2, 0x294, 0x295, 0x7, 0xd0, 0x2, 0x2, 
    0x295, 0x296, 0x7, 0xb, 0x2, 0x2, 0x296, 0x297, 0x5, 0x9a, 0x4e, 0x2, 
    0x297, 0x298, 0x7, 0xc, 0x2, 0x2, 0x298, 0x29a, 0x3, 0x2, 0x2, 0x2, 
    0x299, 0x25f, 0x3, 0x2, 0x2, 0x2, 0x299, 0x26c, 0x3, 0x2, 0x2, 0x2, 
    0x299, 0x279, 0x3, 0x2, 0x2, 0x2, 0x299, 0x288, 0x3, 0x2, 0x2, 0x2, 
    0x29a, 0x29d, 0x3, 0x2, 0x2, 0x2, 0x29b, 0x299, 0x3, 0x2, 0x2, 0x2, 
    0x29b, 0x29c, 0x3, 0x2, 0x2, 0x2, 0x29c, 0x31, 0x3, 0x2, 0x2, 0x2, 0x29d, 
    0x29b, 0x3, 0x2, 0x2, 0x2, 0x29e, 0x29f, 0x5, 0xc2, 0x62, 0x2, 0x29f, 
    0x2a0, 0x7, 0xe, 0x2, 0x2, 0x2a0, 0x2a2, 0x3, 0x2, 0x2, 0x2, 0x2a1, 
    0x29e, 0x3, 0x2, 0x2, 0x2, 0x2a1, 0x2a2, 0x3, 0x2, 0x2, 0x2, 0x2a2, 
    0x2a3, 0x3, 0x2, 0x2, 0x2, 0x2a3, 0x2a4, 0x5, 0xc4, 0x63, 0x2, 0x2a4, 
    0x2a5, 0x7, 0xe, 0x2, 0x2, 0x2a5, 0x2a7, 0x3, 0x2, 0x2, 0x2, 0x2a6, 
    0x2a1, 0x3, 0x2, 0x2, 0x2, 0x2a6, 0x2a7, 0x3, 0x2, 0x2, 0x2, 0x2a7, 
    0x2a8, 0x3, 0x2, 0x2, 0x2, 0x2a8, 0x2a9, 0x5, 0xc6, 0x64, 0x2, 0x2a9, 
    0x33, 0x3, 0x2, 0x2, 0x2, 0x2aa, 0x2b0, 0x7, 0x68, 0x2, 0x2, 0x2ab, 
    0x2ad, 0x9, 0x8, 0x2, 0x2, 0x2ac, 0x2ae, 0x7, 0x97, 0x2, 0x2, 0x2ad, 
    0x2ac, 0x3, 0x2, 0x2, 0x2, 0x2ad, 0x2ae, 0x3, 0x2, 0x2, 0x2, 0x2ae, 
    0x2b0, 0x3, 0x2, 0x2, 0x2, 0x2af, 0x2aa, 0x3, 0x2, 0x2, 0x2, 0x2af, 
    0x2ab, 0x3, 0x2, 0x2, 0x2, 0x2b0, 0x35, 0x3, 0x2, 0x2, 0x2, 0x2b1, 0x2b5, 
    0x5, 0x34, 0x1b, 0x2, 0x2b2, 0x2b5, 0x7, 0xae, 0x2, 0x2, 0x2b3, 0x2b5, 
    0x7, 0x18, 0x2, 0x2, 0x2b4, 0x2b1, 0x3, 0x2, 0x2, 0x2, 0x2b4, 0x2b2, 
    0x3, 0x2, 0x2, 0x2, 0x2b4, 0x2b3, 0x3, 0x2, 0x2, 0x2, 0x2b5, 0x37, 0x3, 
    0x2, 0x2, 0x2, 0x2b6, 0x2b7, 0x7, 0xd2, 0x2, 0x2, 0x2b7, 0x2b8, 0x7, 
    0xb, 0x2, 0x2, 0x2b8, 0x2b9, 0x5, 0xac, 0x57, 0x2, 0x2b9, 0x2c1, 0x7, 
    0xc, 0x2, 0x2, 0x2ba, 0x2bb, 0x7, 0x8, 0x2, 0x2, 0x2bb, 0x2bc, 0x7, 
    0xb, 0x2, 0x2, 0x2bc, 0x2bd, 0x5, 0xac, 0x57, 0x2, 0x2bd, 0x2be, 0x7, 
    0xc, 0x2, 0x2, 0x2be, 0x2c0, 0x3, 0x2, 0x2, 0x2, 0x2bf, 0x2ba, 0x3, 
    0x2, 0x2, 0x2, 0x2c0, 0x2c3, 0x3, 0x2, 0x2, 0x2, 0x2c1, 0x2bf, 0x3, 
    0x2, 0x2, 0x2, 0x2c1, 0x2c2, 0x3, 0x2, 0x2, 0x2, 0x2c2, 0x39, 0x3, 0x2, 
    0x2, 0x2, 0x2c3, 0x2c1, 0x3, 0x2, 0x2, 0x2, 0x2c4, 0x2c5, 0x7, 0xdc, 
    0x2, 0x2, 0x2c5, 0x2ca, 0x5, 0x2e, 0x18, 0x2, 0x2c6, 0x2c7, 0x7, 0x8, 
    0x2, 0x2, 0x2c7, 0x2c9, 0x5, 0x2e, 0x18, 0x2, 0x2c8, 0x2c6, 0x3, 0x2, 
    0x2, 0x2, 0x2c9, 0x2cc, 0x3, 0x2, 0x2, 0x2, 0x2ca, 0x2c8, 0x3, 0x2, 
    0x2, 0x2, 0x2ca, 0x2cb, 0x3, 0x2, 0x2, 0x2, 0x2cb, 0x2ce, 0x3, 0x2, 
    0x2, 0x2, 0x2cc, 0x2ca, 0x3, 0x2, 0x2, 0x2, 0x2cd, 0x2c4, 0x3, 0x2, 
    0x2, 0x2, 0x2cd, 0x2ce, 0x3, 0x2, 0x2, 0x2, 0x2ce, 0x2cf, 0x3, 0x2, 
    0x2, 0x2, 0x2cf, 0x2d1, 0x7, 0x69, 0x2, 0x2, 0x2d0, 0x2d2, 0x7, 0x6d, 
    0x2, 0x2, 0x2d1, 0x2d0, 0x3, 0x2, 0x2, 0x2, 0x2d1, 0x2d2, 0x3, 0x2, 
    0x2, 0x2, 0x2d2, 0x2d3, 0x3, 0x2, 0x2, 0x2, 0x2d3, 0x2df, 0x5, 0x32, 
    0x1a, 0x2, 0x2d4, 0x2d5, 0x7, 0xb, 0x2, 0x2, 0x2d5, 0x2da, 0x5, 0xca, 
    0x66, 0x2, 0x2d6, 0x2d7, 0x7, 0x8, 0x2, 0x2, 0x2d7, 0x2d9, 0x5, 0xca, 
    0x66, 0x2, 0x2d8, 0x2d6, 0x3, 0x2, 0x2, 0x2, 0x2d9, 0x2dc, 0x3, 0x2, 
    0x2, 0x2, 0x2da, 0x2d8, 0x3, 0x2, 0x2, 0x2, 0x2da, 0x2db, 0x3, 0x2, 
    0x2, 0x2, 0x2db, 0x2dd, 0x3, 0x2, 0x2, 0x2, 0x2dc, 0x2da, 0x3, 0x2, 
    0x2, 0x2, 0x2dd, 0x2de, 0x7, 0xc, 0x2, 0x2, 0x2de, 0x2e0, 0x3, 0x2, 
    0x2, 0x2, 0x2df, 0x2d4, 0x3, 0x2, 0x2, 0x2, 0x2df, 0x2e0, 0x3, 0x2, 
    0x2, 0x2, 0x2e0, 0x2e4, 0x3, 0x2, 0x2, 0x2, 0x2e1, 0x2e5, 0x5, 0x38, 
    0x1d, 0x2, 0x2e2, 0x2e5, 0x5, 0x24, 0x13, 0x2, 0x2e3, 0x2e5, 0x5, 0x28, 
    0x15, 0x2, 0x2e4, 0x2e1, 0x3, 0x2, 0x2, 0x2, 0x2e4, 0x2e2, 0x3, 0x2, 
    0x2, 0x2, 0x2e4, 0x2e3, 0x3, 0x2, 0x2, 0x2, 0x2e5, 0x3b, 0x3, 0x2, 0x2, 
    0x2, 0x2e6, 0x2e7, 0x7, 0xdc, 0x2, 0x2, 0x2e7, 0x2ec, 0x5, 0x2e, 0x18, 
    0x2, 0x2e8, 0x2e9, 0x7, 0x8, 0x2, 0x2, 0x2e9, 0x2eb, 0x5, 0x2e, 0x18, 
    0x2, 0x2ea, 0x2e8, 0x3, 0x2, 0x2, 0x2, 0x2eb, 0x2ee, 0x3, 0x2, 0x2, 
    0x2, 0x2ec, 0x2ea, 0x3, 0x2, 0x2, 0x2, 0x2ec, 0x2ed, 0x3, 0x2, 0x2, 
    0x2, 0x2ed, 0x2f0, 0x3, 0x2, 0x2, 0x2, 0x2ee, 0x2ec, 0x3, 0x2, 0x2, 
    0x2, 0x2ef, 0x2e6, 0x3, 0x2, 0x2, 0x2, 0x2ef, 0x2f0, 0x3, 0x2, 0x2, 
    0x2, 0x2f0, 0x2f1, 0x3, 0x2, 0x2, 0x2, 0x2f1, 0x2f3, 0x7, 0x43, 0x2, 
    0x2, 0x2f2, 0x2f4, 0x7, 0x5c, 0x2, 0x2, 0x2f3, 0x2f2, 0x3, 0x2, 0x2, 
    0x2, 0x2f3, 0x2f4, 0x3, 0x2, 0x2, 0x2, 0x2f4, 0x2f5, 0x3, 0x2, 0x2, 
    0x2, 0x2f5, 0x2fa, 0x5, 0x32, 0x1a, 0x2, 0x2f6, 0x2f7, 0x7, 0xd9, 0x2, 
    0x2, 0x2f7, 0x2fb, 0x5, 0xae, 0x58, 0x2, 0x2f8, 0x2f9, 0x7, 0x5c, 0x2, 
    0x2, 0x2f9, 0x2fb, 0x5, 0x30, 0x19, 0x2, 0x2fa, 0x2f6, 0x3, 0x2, 0x2, 
    0x2, 0x2fa, 0x2f8, 0x3, 0x2, 0x2, 0x2, 0x2fa, 0x2fb, 0x3, 0x2, 0x2, 
    0x2, 0x2fb, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x2fc, 0x2fd, 0x7, 0xdc, 0x2, 
    0x2, 0x2fd, 0x302, 0x5, 0x2e, 0x18, 0x2, 0x2fe, 0x2ff, 0x7, 0x8, 0x2, 
    0x2, 0x2ff, 0x301, 0x5, 0x2e, 0x18, 0x2, 0x300, 0x2fe, 0x3, 0x2, 0x2, 
    0x2, 0x301, 0x304, 0x3, 0x2, 0x2, 0x2, 0x302, 0x300, 0x3, 0x2, 0x2, 
    0x2, 0x302, 0x303, 0x3, 0x2, 0x2, 0x2, 0x303, 0x306, 0x3, 0x2, 0x2, 
    0x2, 0x304, 0x302, 0x3, 0x2, 0x2, 0x2, 0x305, 0x2fc, 0x3, 0x2, 0x2, 
    0x2, 0x305, 0x306, 0x3, 0x2, 0x2, 0x2, 0x306, 0x307, 0x3, 0x2, 0x2, 
    0x2, 0x307, 0x308, 0x7, 0xcf, 0x2, 0x2, 0x308, 0x309, 0x5, 0x32, 0x1a, 
    0x2, 0x309, 0x30a, 0x7, 0xaf, 0x2, 0x2, 0x30a, 0x30b, 0x5, 0xca, 0x66, 
    0x2, 0x30b, 0x30c, 0x7, 0x6, 0x2, 0x2, 0x30c, 0x314, 0x5, 0xb4, 0x5b, 
    0x2, 0x30d, 0x30e, 0x7, 0x8, 0x2, 0x2, 0x30e, 0x30f, 0x5, 0xca, 0x66, 
    0x2, 0x30f, 0x310, 0x7, 0x6, 0x2, 0x2, 0x310, 0x311, 0x5, 0xb4, 0x5b, 
    0x2, 0x311, 0x313, 0x3, 0x2, 0x2, 0x2, 0x312, 0x30d, 0x3, 0x2, 0x2, 
    0x2, 0x313, 0x316, 0x3, 0x2, 0x2, 0x2, 0x314, 0x312, 0x3, 0x2, 0x2, 
    0x2, 0x314, 0x315, 0x3, 0x2, 0x2, 0x2, 0x315, 0x31b, 0x3, 0x2, 0x2, 
    0x2, 0x316, 0x314, 0x3, 0x2, 0x2, 0x2, 0x317, 0x318, 0x7, 0xd9, 0x2, 
    0x2, 0x318, 0x31c, 0x5, 0xae, 0x58, 0x2, 0x319, 0x31a, 0x7, 0x5c, 0x2, 
    0x2, 0x31a, 0x31c, 0x5, 0x30, 0x19, 0x2, 0x31b, 0x317, 0x3, 0x2, 0x2, 
    0x2, 0x31b, 0x319, 0x3, 0x2, 0x2, 0x2, 0x31b, 0x31c, 0x3, 0x2, 0x2, 
    0x2, 0x31c, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x31d, 0x320, 0x5, 0x42, 0x22, 
    0x2, 0x31e, 0x320, 0x5, 0x44, 0x23, 0x2, 0x31f, 0x31d, 0x3, 0x2, 0x2, 
    0x2, 0x31f, 0x31e, 0x3, 0x2, 0x2, 0x2, 0x320, 0x41, 0x3, 0x2, 0x2, 0x2, 
    0x321, 0x322, 0x7, 0x36, 0x2, 0x2, 0x322, 0x323, 0x7, 0xbd, 0x2, 0x2, 
    0x323, 0x324, 0x5, 0xc6, 0x64, 0x2, 0x324, 0x325, 0x5, 0x70, 0x39, 0x2, 
    0x325, 0x43, 0x3, 0x2, 0x2, 0x2, 0x326, 0x328, 0x7, 0x36, 0x2, 0x2, 
    0x327, 0x329, 0x7, 0xce, 0x2, 0x2, 0x328, 0x327, 0x3, 0x2, 0x2, 0x2, 
    0x328, 0x329, 0x3, 0x2, 0x2, 0x2, 0x329, 0x32a, 0x3, 0x2, 0x2, 0x2, 
    0x32a, 0x32b, 0x7, 0x67, 0x2, 0x2, 0x32b, 0x32c, 0x5, 0xd0, 0x69, 0x2, 
    0x32c, 0x32d, 0x7, 0x94, 0x2, 0x2, 0x32d, 0x32e, 0x5, 0xc6, 0x64, 0x2, 
    0x32e, 0x32f, 0x7, 0xb, 0x2, 0x2, 0x32f, 0x334, 0x5, 0xb4, 0x5b, 0x2, 
    0x330, 0x331, 0x7, 0x8, 0x2, 0x2, 0x331, 0x333, 0x5, 0xb4, 0x5b, 0x2, 
    0x332, 0x330, 0x3, 0x2, 0x2, 0x2, 0x333, 0x336, 0x3, 0x2, 0x2, 0x2, 
    0x334, 0x332, 0x3, 0x2, 0x2, 0x2, 0x334, 0x335, 0x3, 0x2, 0x2, 0x2, 
    0x335, 0x337, 0x3, 0x2, 0x2, 0x2, 0x336, 0x334, 0x3, 0x2, 0x2, 0x2, 
    0x337, 0x338, 0x7, 0xc, 0x2, 0x2, 0x338, 0x45, 0x3, 0x2, 0x2, 0x2, 0x339, 
    0x33c, 0x5, 0x48, 0x25, 0x2, 0x33a, 0x33c, 0x5, 0x4a, 0x26, 0x2, 0x33b, 
    0x339, 0x3, 0x2, 0x2, 0x2, 0x33b, 0x33a, 0x3, 0x2, 0x2, 0x2, 0x33c, 
    0x47, 0x3, 0x2, 0x2, 0x2, 0x33d, 0x33e, 0x7, 0x49, 0x2, 0x2, 0x33e, 
    0x33f, 0x7, 0xbd, 0x2, 0x2, 0x33f, 0x340, 0x5, 0xc6, 0x64, 0x2, 0x340, 
    0x49, 0x3, 0x2, 0x2, 0x2, 0x341, 0x342, 0x7, 0x49, 0x2, 0x2, 0x342, 
    0x34a, 0x7, 0x67, 0x2, 0x2, 0x343, 0x344, 0x5, 0xd0, 0x69, 0x2, 0x344, 
    0x345, 0x7, 0x94, 0x2, 0x2, 0x345, 0x346, 0x5, 0xc6, 0x64, 0x2, 0x346, 
    0x34b, 0x3, 0x2, 0x2, 0x2, 0x347, 0x348, 0x5, 0xc6, 0x64, 0x2, 0x348, 
    0x349, 0x5, 0xd0, 0x69, 0x2, 0x349, 0x34b, 0x3, 0x2, 0x2, 0x2, 0x34a, 
    0x343, 0x3, 0x2, 0x2, 0x2, 0x34a, 0x347, 0x3, 0x2, 0x2, 0x2, 0x34b, 
    0x4b, 0x3, 0x2, 0x2, 0x2, 0x34c, 0x34e, 0x7, 0xc9, 0x2, 0x2, 0x34d, 
    0x34f, 0x7, 0xbd, 0x2, 0x2, 0x34e, 0x34d, 0x3, 0x2, 0x2, 0x2, 0x34e, 
    0x34f, 0x3, 0x2, 0x2, 0x2, 0x34f, 0x350, 0x3, 0x2, 0x2, 0x2, 0x350, 
    0x351, 0x5, 0xc6, 0x64, 0x2, 0x351, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x352, 
    0x353, 0x7, 0x75, 0x2, 0x2, 0x353, 0x354, 0x5, 0xc6, 0x64, 0x2, 0x354, 
    0x35f, 0x5, 0xfe, 0x80, 0x2, 0x355, 0x35c, 0x5, 0x52, 0x2a, 0x2, 0x356, 
    0x358, 0x7, 0x8, 0x2, 0x2, 0x357, 0x356, 0x3, 0x2, 0x2, 0x2, 0x357, 
    0x358, 0x3, 0x2, 0x2, 0x2, 0x358, 0x359, 0x3, 0x2, 0x2, 0x2, 0x359, 
    0x35b, 0x5, 0x52, 0x2a, 0x2, 0x35a, 0x357, 0x3, 0x2, 0x2, 0x2, 0x35b, 
    0x35e, 0x3, 0x2, 0x2, 0x2, 0x35c, 0x35a, 0x3, 0x2, 0x2, 0x2, 0x35c, 
    0x35d, 0x3, 0x2, 0x2, 0x2, 0x35d, 0x360, 0x3, 0x2, 0x2, 0x2, 0x35e, 
    0x35c, 0x3, 0x2, 0x2, 0x2, 0x35f, 0x355, 0x3, 0x2, 0x2, 0x2, 0x35f, 
    0x360, 0x3, 0x2, 0x2, 0x2, 0x360, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x361, 0x362, 
    0x7, 0x48, 0x2, 0x2, 0x362, 0x363, 0x5, 0xc6, 0x64, 0x2, 0x363, 0x365, 
    0x7, 0x9b, 0x2, 0x2, 0x364, 0x366, 0x5, 0xfe, 0x80, 0x2, 0x365, 0x364, 
    0x3, 0x2, 0x2, 0x2, 0x366, 0x367, 0x3, 0x2, 0x2, 0x2, 0x367, 0x365, 
    0x3, 0x2, 0x2, 0x2, 0x367, 0x368, 0x3, 0x2, 0x2, 0x2, 0x368, 0x373, 
    0x3, 0x2, 0x2, 0x2, 0x369, 0x370, 0x5, 0x52, 0x2a, 0x2, 0x36a, 0x36c, 
    0x7, 0x8, 0x2, 0x2, 0x36b, 0x36a, 0x3, 0x2, 0x2, 0x2, 0x36b, 0x36c, 
    0x3, 0x2, 0x2, 0x2, 0x36c, 0x36d, 0x3, 0x2, 0x2, 0x2, 0x36d, 0x36f, 
    0x5, 0x52, 0x2a, 0x2, 0x36e, 0x36b, 0x3, 0x2, 0x2, 0x2, 0x36f, 0x372, 
    0x3, 0x2, 0x2, 0x2, 0x370, 0x36e, 0x3, 0x2, 0x2, 0x2, 0x370, 0x371, 
    0x3, 0x2, 0x2, 0x2, 0x371, 0x374, 0x3, 0x2, 0x2, 0x2, 0x372, 0x370, 
    0x3, 0x2, 0x2, 0x2, 0x373, 0x369, 0x3, 0x2, 0x2, 0x2, 0x373, 0x374, 
    0x3, 0x2, 0x2, 0x2, 0x374, 0x51, 0x3, 0x2, 0x2, 0x2, 0x375, 0x376, 0x7, 
    0x4, 0x2, 0x2, 0x376, 0x377, 0x5, 0x54, 0x2b, 0x2, 0x377, 0x378, 0x7, 
    0x5, 0x2, 0x2, 0x378, 0x37b, 0x3, 0x2, 0x2, 0x2, 0x379, 0x37b, 0x5, 
    0x54, 0x2b, 0x2, 0x37a, 0x375, 0x3, 0x2, 0x2, 0x2, 0x37a, 0x379, 0x3, 
    0x2, 0x2, 0x2, 0x37b, 0x37c, 0x3, 0x2, 0x2, 0x2, 0x37c, 0x37d, 0x7, 
    0x6, 0x2, 0x2, 0x37d, 0x37e, 0x5, 0xfe, 0x80, 0x2, 0x37e, 0x53, 0x3, 
    0x2, 0x2, 0x2, 0x37f, 0x380, 0x9, 0x9, 0x2, 0x2, 0x380, 0x55, 0x3, 0x2, 
    0x2, 0x2, 0x381, 0x382, 0x7, 0x35, 0x2, 0x2, 0x382, 0x383, 0x5, 0xc6, 
    0x64, 0x2, 0x383, 0x57, 0x3, 0x2, 0x2, 0x2, 0x384, 0x385, 0x7, 0xb1, 
    0x2, 0x2, 0x385, 0x387, 0x5, 0xec, 0x77, 0x2, 0x386, 0x388, 0x5, 0x5a, 
    0x2e, 0x2, 0x387, 0x386, 0x3, 0x2, 0x2, 0x2, 0x387, 0x388, 0x3, 0x2, 
    0x2, 0x2, 0x388, 0x59, 0x3, 0x2, 0x2, 0x2, 0x389, 0x38c, 0x9, 0xa, 0x2, 
    0x2, 0x38a, 0x38c, 0x9, 0xb, 0x2, 0x2, 0x38b, 0x389, 0x3, 0x2, 0x2, 
    0x2, 0x38b, 0x38a, 0x3, 0x2, 0x2, 0x2, 0x38c, 0x5b, 0x3, 0x2, 0x2, 0x2, 
    0x38d, 0x38e, 0x7, 0xb2, 0x2, 0x2, 0x38e, 0x5d, 0x3, 0x2, 0x2, 0x2, 
    0x38f, 0x390, 0x7, 0xb6, 0x2, 0x2, 0x390, 0x5f, 0x3, 0x2, 0x2, 0x2, 
    0x391, 0x392, 0x7, 0xba, 0x2, 0x2, 0x392, 0x393, 0x5, 0xc6, 0x64, 0x2, 
    0x393, 0x394, 0x5, 0xd0, 0x69, 0x2, 0x394, 0x61, 0x3, 0x2, 0x2, 0x2, 
    0x395, 0x396, 0x7, 0xbb, 0x2, 0x2, 0x396, 0x397, 0x5, 0xc6, 0x64, 0x2, 
    0x397, 0x63, 0x3, 0x2, 0x2, 0x2, 0x398, 0x39a, 0x7, 0xc5, 0x2, 0x2, 
    0x399, 0x39b, 0x7, 0x59, 0x2, 0x2, 0x39a, 0x399, 0x3, 0x2, 0x2, 0x2, 
    0x39a, 0x39b, 0x3, 0x2, 0x2, 0x2, 0x39b, 0x65, 0x3, 0x2, 0x2, 0x2, 0x39c, 
    0x39d, 0x7, 0x7d, 0x2, 0x2, 0x39d, 0x67, 0x3, 0x2, 0x2, 0x2, 0x39e, 
    0x3a0, 0x7, 0x26, 0x2, 0x2, 0x39f, 0x3a1, 0x9, 0xc, 0x2, 0x2, 0x3a0, 
    0x39f, 0x3, 0x2, 0x2, 0x2, 0x3a0, 0x3a1, 0x3, 0x2, 0x2, 0x2, 0x3a1, 
    0x69, 0x3, 0x2, 0x2, 0x2, 0x3a2, 0x3a5, 0x7, 0x56, 0x2, 0x2, 0x3a3, 
    0x3a6, 0x5, 0xdc, 0x6f, 0x2, 0x3a4, 0x3a6, 0x5, 0xfe, 0x80, 0x2, 0x3a5, 
    0x3a3, 0x3, 0x2, 0x2, 0x2, 0x3a5, 0x3a4, 0x3, 0x2, 0x2, 0x2, 0x3a6, 
    0x6b, 0x3, 0x2, 0x2, 0x2, 0x3a7, 0x3a8, 0x9, 0xd, 0x2, 0x2, 0x3a8, 0x6d, 
    0x3, 0x2, 0x2, 0x2, 0x3a9, 0x3aa, 0x3, 0x2, 0x2, 0x2, 0x3aa, 0x6f, 0x3, 
    0x2, 0x2, 0x2, 0x3ab, 0x3ae, 0x7, 0xb, 0x2, 0x2, 0x3ac, 0x3af, 0x5, 
    0x74, 0x3b, 0x2, 0x3ad, 0x3af, 0x3, 0x2, 0x2, 0x2, 0x3ae, 0x3ac, 0x3, 
    0x2, 0x2, 0x2, 0x3ae, 0x3ad, 0x3, 0x2, 0x2, 0x2, 0x3af, 0x3b7, 0x3, 
    0x2, 0x2, 0x2, 0x3b0, 0x3b3, 0x7, 0x8, 0x2, 0x2, 0x3b1, 0x3b4, 0x5, 
    0x74, 0x3b, 0x2, 0x3b2, 0x3b4, 0x3, 0x2, 0x2, 0x2, 0x3b3, 0x3b1, 0x3, 
    0x2, 0x2, 0x2, 0x3b3, 0x3b2, 0x3, 0x2, 0x2, 0x2, 0x3b4, 0x3b6, 0x3, 
    0x2, 0x2, 0x2, 0x3b5, 0x3b0, 0x3, 0x2, 0x2, 0x2, 0x3b6, 0x3b9, 0x3, 
    0x2, 0x2, 0x2, 0x3b7, 0x3b5, 0x3, 0x2, 0x2, 0x2, 0x3b7, 0x3b8, 0x3, 
    0x2, 0x2, 0x2, 0x3b8, 0x3ba, 0x3, 0x2, 0x2, 0x2, 0x3b9, 0x3b7, 0x3, 
    0x2, 0x2, 0x2, 0x3ba, 0x3c3, 0x7, 0xc, 0x2, 0x2, 0x3bb, 0x3c2, 0x5, 
    0x72, 0x3a, 0x2, 0x3bc, 0x3c2, 0x7, 0xa5, 0x2, 0x2, 0x3bd, 0x3be, 0x7, 
    0xa8, 0x2, 0x2, 0x3be, 0x3c2, 0x5, 0xec, 0x77, 0x2, 0x3bf, 0x3c0, 0x7, 
    0xbe, 0x2, 0x2, 0x3c0, 0x3c2, 0x5, 0xec, 0x77, 0x2, 0x3c1, 0x3bb, 0x3, 
    0x2, 0x2, 0x2, 0x3c1, 0x3bc, 0x3, 0x2, 0x2, 0x2, 0x3c1, 0x3bd, 0x3, 
    0x2, 0x2, 0x2, 0x3c1, 0x3bf, 0x3, 0x2, 0x2, 0x2, 0x3c2, 0x3c5, 0x3, 
    0x2, 0x2, 0x2, 0x3c3, 0x3c1, 0x3, 0x2, 0x2, 0x2, 0x3c3, 0x3c4, 0x3, 
    0x2, 0x2, 0x2, 0x3c4, 0x71, 0x3, 0x2, 0x2, 0x2, 0x3c5, 0x3c3, 0x3, 0x2, 
    0x2, 0x2, 0x3c6, 0x3c7, 0x7, 0x9a, 0x2, 0x2, 0x3c7, 0x3c8, 0x7, 0x28, 
    0x2, 0x2, 0x3c8, 0x3c9, 0x7, 0xb, 0x2, 0x2, 0x3c9, 0x3ce, 0x5, 0xb4, 
    0x5b, 0x2, 0x3ca, 0x3cb, 0x7, 0x8, 0x2, 0x2, 0x3cb, 0x3cd, 0x5, 0xb4, 
    0x5b, 0x2, 0x3cc, 0x3ca, 0x3, 0x2, 0x2, 0x2, 0x3cd, 0x3d0, 0x3, 0x2, 
    0x2, 0x2, 0x3ce, 0x3cc, 0x3, 0x2, 0x2, 0x2, 0x3ce, 0x3cf, 0x3, 0x2, 
    0x2, 0x2, 0x3cf, 0x3d1, 0x3, 0x2, 0x2, 0x2, 0x3d0, 0x3ce, 0x3, 0x2, 
    0x2, 0x2, 0x3d1, 0x3d2, 0x7, 0xc, 0x2, 0x2, 0x3d2, 0x73, 0x3, 0x2, 0x2, 
    0x2, 0x3d3, 0x3d4, 0x5, 0xca, 0x66, 0x2, 0x3d4, 0x3d9, 0x5, 0x7c, 0x3f, 
    0x2, 0x3d5, 0x3d6, 0x7, 0x8d, 0x2, 0x2, 0x3d6, 0x3d8, 0x7, 0x8f, 0x2, 
    0x2, 0x3d7, 0x3d5, 0x3, 0x2, 0x2, 0x2, 0x3d8, 0x3db, 0x3, 0x2, 0x2, 
    0x2, 0x3d9, 0x3d7, 0x3, 0x2, 0x2, 0x2, 0x3d9, 0x3da, 0x3, 0x2, 0x2, 
    0x2, 0x3da, 0x75, 0x3, 0x2, 0x2, 0x2, 0x3db, 0x3d9, 0x3, 0x2, 0x2, 0x2, 
    0x3dc, 0x3dd, 0x7, 0xb, 0x2, 0x2, 0x3dd, 0x3e2, 0x5, 0xca, 0x66, 0x2, 
    0x3de, 0x3df, 0x7, 0x8, 0x2, 0x2, 0x3df, 0x3e1, 0x5, 0xca, 0x66, 0x2, 
    0x3e0, 0x3de, 0x3, 0x2, 0x2, 0x2, 0x3e1, 0x3e4, 0x3, 0x2, 0x2, 0x2, 
    0x3e2, 0x3e0, 0x3, 0x2, 0x2, 0x2, 0x3e2, 0x3e3, 0x3, 0x2, 0x2, 0x2, 
    0x3e3, 0x3e5, 0x3, 0x2, 0x2, 0x2, 0x3e4, 0x3e2, 0x3, 0x2, 0x2, 0x2, 
    0x3e5, 0x3e6, 0x7, 0xc, 0x2, 0x2, 0x3e6, 0x77, 0x3, 0x2, 0x2, 0x2, 0x3e7, 
    0x3e9, 0x7, 0xce, 0x2, 0x2, 0x3e8, 0x3e7, 0x3, 0x2, 0x2, 0x2, 0x3e8, 
    0x3e9, 0x3, 0x2, 0x2, 0x2, 0x3e9, 0x3ea, 0x3, 0x2, 0x2, 0x2, 0x3ea, 
    0x3ec, 0x9, 0xe, 0x2, 0x2, 0x3eb, 0x3ed, 0x5, 0xd0, 0x69, 0x2, 0x3ec, 
    0x3eb, 0x3, 0x2, 0x2, 0x2, 0x3ec, 0x3ed, 0x3, 0x2, 0x2, 0x2, 0x3ed, 
    0x3ef, 0x3, 0x2, 0x2, 0x2, 0x3ee, 0x3f0, 0x7, 0x94, 0x2, 0x2, 0x3ef, 
    0x3ee, 0x3, 0x2, 0x2, 0x2, 0x3ef, 0x3f0, 0x3, 0x2, 0x2, 0x2, 0x3f0, 
    0x3fd, 0x3, 0x2, 0x2, 0x2, 0x3f1, 0x3fe, 0x5, 0xb4, 0x5b, 0x2, 0x3f2, 
    0x3f3, 0x7, 0xb, 0x2, 0x2, 0x3f3, 0x3f8, 0x5, 0xb4, 0x5b, 0x2, 0x3f4, 
    0x3f5, 0x7, 0x8, 0x2, 0x2, 0x3f5, 0x3f7, 0x5, 0xb4, 0x5b, 0x2, 0x3f6, 
    0x3f4, 0x3, 0x2, 0x2, 0x2, 0x3f7, 0x3fa, 0x3, 0x2, 0x2, 0x2, 0x3f8, 
    0x3f6, 0x3, 0x2, 0x2, 0x2, 0x3f8, 0x3f9, 0x3, 0x2, 0x2, 0x2, 0x3f9, 
    0x3fb, 0x3, 0x2, 0x2, 0x2, 0x3fa, 0x3f8, 0x3, 0x2, 0x2, 0x2, 0x3fb, 
    0x3fc, 0x7, 0xc, 0x2, 0x2, 0x3fc, 0x3fe, 0x3, 0x2, 0x2, 0x2, 0x3fd, 
    0x3f1, 0x3, 0x2, 0x2, 0x2, 0x3fd, 0x3f2, 0x3, 0x2, 0x2, 0x2, 0x3fe, 
    0x401, 0x3, 0x2, 0x2, 0x2, 0x3ff, 0x400, 0x7, 0xd0, 0x2, 0x2, 0x400, 
    0x402, 0x5, 0x7a, 0x3e, 0x2, 0x401, 0x3ff, 0x3, 0x2, 0x2, 0x2, 0x401, 
    0x402, 0x3, 0x2, 0x2, 0x2, 0x402, 0x405, 0x3, 0x2, 0x2, 0x2, 0x403, 
    0x404, 0x7, 0xdc, 0x2, 0x2, 0x404, 0x406, 0x5, 0xa8, 0x55, 0x2, 0x405, 
    0x403, 0x3, 0x2, 0x2, 0x2, 0x405, 0x406, 0x3, 0x2, 0x2, 0x2, 0x406, 
    0x79, 0x3, 0x2, 0x2, 0x2, 0x407, 0x408, 0x9, 0xf, 0x2, 0x2, 0x408, 0x7b, 
    0x3, 0x2, 0x2, 0x2, 0x409, 0x40a, 0x5, 0x7e, 0x40, 0x2, 0x40a, 0x7d, 
    0x3, 0x2, 0x2, 0x2, 0x40b, 0x44b, 0x7, 0x25, 0x2, 0x2, 0x40c, 0x414, 
    0x7, 0x6a, 0x2, 0x2, 0x40d, 0x40e, 0x7, 0xb, 0x2, 0x2, 0x40e, 0x410, 
    0x5, 0xe0, 0x71, 0x2, 0x40f, 0x411, 0x9, 0x10, 0x2, 0x2, 0x410, 0x40f, 
    0x3, 0x2, 0x2, 0x2, 0x410, 0x411, 0x3, 0x2, 0x2, 0x2, 0x411, 0x412, 
    0x3, 0x2, 0x2, 0x2, 0x412, 0x413, 0x7, 0xc, 0x2, 0x2, 0x413, 0x415, 
    0x3, 0x2, 0x2, 0x2, 0x414, 0x40d, 0x3, 0x2, 0x2, 0x2, 0x414, 0x415, 
    0x3, 0x2, 0x2, 0x2, 0x415, 0x44b, 0x3, 0x2, 0x2, 0x2, 0x416, 0x425, 
    0x7, 0x41, 0x2, 0x2, 0x417, 0x418, 0x7, 0xb, 0x2, 0x2, 0x418, 0x41a, 
    0x5, 0xe0, 0x71, 0x2, 0x419, 0x41b, 0x7, 0x46, 0x2, 0x2, 0x41a, 0x419, 
    0x3, 0x2, 0x2, 0x2, 0x41a, 0x41b, 0x3, 0x2, 0x2, 0x2, 0x41b, 0x421, 
    0x3, 0x2, 0x2, 0x2, 0x41c, 0x41d, 0x7, 0x8, 0x2, 0x2, 0x41d, 0x41f, 
    0x5, 0xe2, 0x72, 0x2, 0x41e, 0x420, 0x7, 0x46, 0x2, 0x2, 0x41f, 0x41e, 
    0x3, 0x2, 0x2, 0x2, 0x41f, 0x420, 0x3, 0x2, 0x2, 0x2, 0x420, 0x422, 
    0x3, 0x2, 0x2, 0x2, 0x421, 0x41c, 0x3, 0x2, 0x2, 0x2, 0x421, 0x422, 
    0x3, 0x2, 0x2, 0x2, 0x422, 0x423, 0x3, 0x2, 0x2, 0x2, 0x423, 0x424, 
    0x7, 0xc, 0x2, 0x2, 0x424, 0x426, 0x3, 0x2, 0x2, 0x2, 0x425, 0x417, 
    0x3, 0x2, 0x2, 0x2, 0x425, 0x426, 0x3, 0x2, 0x2, 0x2, 0x426, 0x44b, 
    0x3, 0x2, 0x2, 0x2, 0x427, 0x42f, 0x7, 0x57, 0x2, 0x2, 0x428, 0x429, 
    0x7, 0xb, 0x2, 0x2, 0x429, 0x42b, 0x5, 0xe0, 0x71, 0x2, 0x42a, 0x42c, 
    0x9, 0x10, 0x2, 0x2, 0x42b, 0x42a, 0x3, 0x2, 0x2, 0x2, 0x42b, 0x42c, 
    0x3, 0x2, 0x2, 0x2, 0x42c, 0x42d, 0x3, 0x2, 0x2, 0x2, 0x42d, 0x42e, 
    0x7, 0xc, 0x2, 0x2, 0x42e, 0x430, 0x3, 0x2, 0x2, 0x2, 0x42f, 0x428, 
    0x3, 0x2, 0x2, 0x2, 0x42f, 0x430, 0x3, 0x2, 0x2, 0x2, 0x430, 0x44b, 
    0x3, 0x2, 0x2, 0x2, 0x431, 0x44b, 0x7, 0x3d, 0x2, 0x2, 0x432, 0x44b, 
    0x7, 0xc1, 0x2, 0x2, 0x433, 0x44b, 0x7, 0xc2, 0x2, 0x2, 0x434, 0x44b, 
    0x5, 0x80, 0x41, 0x2, 0x435, 0x44b, 0x5, 0x86, 0x44, 0x2, 0x436, 0x44b, 
    0x5, 0x88, 0x45, 0x2, 0x437, 0x438, 0x7, 0x1e, 0x2, 0x2, 0x438, 0x439, 
    0x7, 0xb, 0x2, 0x2, 0x439, 0x43b, 0x5, 0xe4, 0x73, 0x2, 0x43a, 0x43c, 
    0x9, 0x11, 0x2, 0x2, 0x43b, 0x43a, 0x3, 0x2, 0x2, 0x2, 0x43b, 0x43c, 
    0x3, 0x2, 0x2, 0x2, 0x43c, 0x43d, 0x3, 0x2, 0x2, 0x2, 0x43d, 0x43e, 
    0x7, 0xc, 0x2, 0x2, 0x43e, 0x44b, 0x3, 0x2, 0x2, 0x2, 0x43f, 0x447, 
    0x7, 0xd3, 0x2, 0x2, 0x440, 0x441, 0x7, 0xb, 0x2, 0x2, 0x441, 0x443, 
    0x5, 0xe4, 0x73, 0x2, 0x442, 0x444, 0x9, 0x11, 0x2, 0x2, 0x443, 0x442, 
    0x3, 0x2, 0x2, 0x2, 0x443, 0x444, 0x3, 0x2, 0x2, 0x2, 0x444, 0x445, 
    0x3, 0x2, 0x2, 0x2, 0x445, 0x446, 0x7, 0xc, 0x2, 0x2, 0x446, 0x448, 
    0x3, 0x2, 0x2, 0x2, 0x447, 0x440, 0x3, 0x2, 0x2, 0x2, 0x447, 0x448, 
    0x3, 0x2, 0x2, 0x2, 0x448, 0x44b, 0x3, 0x2, 0x2, 0x2, 0x449, 0x44b, 
    0x7, 0x21, 0x2, 0x2, 0x44a, 0x40b, 0x3, 0x2, 0x2, 0x2, 0x44a, 0x40c, 
    0x3, 0x2, 0x2, 0x2, 0x44a, 0x416, 0x3, 0x2, 0x2, 0x2, 0x44a, 0x427, 
    0x3, 0x2, 0x2, 0x2, 0x44a, 0x431, 0x3, 0x2, 0x2, 0x2, 0x44a, 0x432, 
    0x3, 0x2, 0x2, 0x2, 0x44a, 0x433, 0x3, 0x2, 0x2, 0x2, 0x44a, 0x434, 
    0x3, 0x2, 0x2, 0x2, 0x44a, 0x435, 0x3, 0x2, 0x2, 0x2, 0x44a, 0x436, 
    0x3, 0x2, 0x2, 0x2, 0x44a, 0x437, 0x3, 0x2, 0x2, 0x2, 0x44a, 0x43f, 
    0x3, 0x2, 0x2, 0x2, 0x44a, 0x449, 0x3, 0x2, 0x2, 0x2, 0x44b, 0x7f, 0x3, 
    0x2, 0x2, 0x2, 0x44c, 0x44f, 0x7, 0x6c, 0x2, 0x2, 0x44d, 0x450, 0x5, 
    0x82, 0x42, 0x2, 0x44e, 0x450, 0x5, 0x84, 0x43, 0x2, 0x44f, 0x44d, 0x3, 
    0x2, 0x2, 0x2, 0x44f, 0x44e, 0x3, 0x2, 0x2, 0x2, 0x450, 0x81, 0x3, 0x2, 
    0x2, 0x2, 0x451, 0x456, 0x7, 0xde, 0x2, 0x2, 0x452, 0x453, 0x7, 0xb, 
    0x2, 0x2, 0x453, 0x454, 0x5, 0xe0, 0x71, 0x2, 0x454, 0x455, 0x7, 0xc, 
    0x2, 0x2, 0x455, 0x457, 0x3, 0x2, 0x2, 0x2, 0x456, 0x452, 0x3, 0x2, 
    0x2, 0x2, 0x456, 0x457, 0x3, 0x2, 0x2, 0x2, 0x457, 0x469, 0x3, 0x2, 
    0x2, 0x2, 0x458, 0x45d, 0x7, 0xde, 0x2, 0x2, 0x459, 0x45a, 0x7, 0xb, 
    0x2, 0x2, 0x45a, 0x45b, 0x5, 0xe0, 0x71, 0x2, 0x45b, 0x45c, 0x7, 0xc, 
    0x2, 0x2, 0x45c, 0x45e, 0x3, 0x2, 0x2, 0x2, 0x45d, 0x459, 0x3, 0x2, 
    0x2, 0x2, 0x45d, 0x45e, 0x3, 0x2, 0x2, 0x2, 0x45e, 0x45f, 0x3, 0x2, 
    0x2, 0x2, 0x45f, 0x460, 0x7, 0xc3, 0x2, 0x2, 0x460, 0x469, 0x7, 0x85, 
    0x2, 0x2, 0x461, 0x466, 0x7, 0x85, 0x2, 0x2, 0x462, 0x463, 0x7, 0xb, 
    0x2, 0x2, 0x463, 0x464, 0x5, 0xe0, 0x71, 0x2, 0x464, 0x465, 0x7, 0xc, 
    0x2, 0x2, 0x465, 0x467, 0x3, 0x2, 0x2, 0x2, 0x466, 0x462, 0x3, 0x2, 
    0x2, 0x2, 0x466, 0x467, 0x3, 0x2, 0x2, 0x2, 0x467, 0x469, 0x3, 0x2, 
    0x2, 0x2, 0x468, 0x451, 0x3, 0x2, 0x2, 0x2, 0x468, 0x458, 0x3, 0x2, 
    0x2, 0x2, 0x468, 0x461, 0x3, 0x2, 0x2, 0x2, 0x469, 0x83, 0x3, 0x2, 0x2, 
    0x2, 0x46a, 0x46f, 0x7, 0x3f, 0x2, 0x2, 0x46b, 0x46c, 0x7, 0xb, 0x2, 
    0x2, 0x46c, 0x46d, 0x5, 0xe0, 0x71, 0x2, 0x46d, 0x46e, 0x7, 0xc, 0x2, 
    0x2, 0x46e, 0x470, 0x3, 0x2, 0x2, 0x2, 0x46f, 0x46b, 0x3, 0x2, 0x2, 
    0x2, 0x46f, 0x470, 0x3, 0x2, 0x2, 0x2, 0x470, 0x4bf, 0x3, 0x2, 0x2, 
    0x2, 0x471, 0x476, 0x7, 0x3f, 0x2, 0x2, 0x472, 0x473, 0x7, 0xb, 0x2, 
    0x2, 0x473, 0x474, 0x5, 0xe0, 0x71, 0x2, 0x474, 0x475, 0x7, 0xc, 0x2, 
    0x2, 0x475, 0x477, 0x3, 0x2, 0x2, 0x2, 0x476, 0x472, 0x3, 0x2, 0x2, 
    0x2, 0x476, 0x477, 0x3, 0x2, 0x2, 0x2, 0x477, 0x478, 0x3, 0x2, 0x2, 
    0x2, 0x478, 0x482, 0x7, 0xc3, 0x2, 0x2, 0x479, 0x483, 0x7, 0x62, 0x2, 
    0x2, 0x47a, 0x483, 0x7, 0x82, 0x2, 0x2, 0x47b, 0x480, 0x7, 0xab, 0x2, 
    0x2, 0x47c, 0x47d, 0x7, 0xb, 0x2, 0x2, 0x47d, 0x47e, 0x5, 0xe2, 0x72, 
    0x2, 0x47e, 0x47f, 0x7, 0xc, 0x2, 0x2, 0x47f, 0x481, 0x3, 0x2, 0x2, 
    0x2, 0x480, 0x47c, 0x3, 0x2, 0x2, 0x2, 0x480, 0x481, 0x3, 0x2, 0x2, 
    0x2, 0x481, 0x483, 0x3, 0x2, 0x2, 0x2, 0x482, 0x479, 0x3, 0x2, 0x2, 
    0x2, 0x482, 0x47a, 0x3, 0x2, 0x2, 0x2, 0x482, 0x47b, 0x3, 0x2, 0x2, 
    0x2, 0x483, 0x4bf, 0x3, 0x2, 0x2, 0x2, 0x484, 0x489, 0x7, 0x62, 0x2, 
    0x2, 0x485, 0x486, 0x7, 0xb, 0x2, 0x2, 0x486, 0x487, 0x5, 0xe0, 0x71, 
    0x2, 0x487, 0x488, 0x7, 0xc, 0x2, 0x2, 0x488, 0x48a, 0x3, 0x2, 0x2, 
    0x2, 0x489, 0x485, 0x3, 0x2, 0x2, 0x2, 0x489, 0x48a, 0x3, 0x2, 0x2, 
    0x2, 0x48a, 0x48b, 0x3, 0x2, 0x2, 0x2, 0x48b, 0x494, 0x7, 0xc3, 0x2, 
    0x2, 0x48c, 0x495, 0x7, 0x82, 0x2, 0x2, 0x48d, 0x492, 0x7, 0xab, 0x2, 
    0x2, 0x48e, 0x48f, 0x7, 0xb, 0x2, 0x2, 0x48f, 0x490, 0x5, 0xe2, 0x72, 
    0x2, 0x490, 0x491, 0x7, 0xc, 0x2, 0x2, 0x491, 0x493, 0x3, 0x2, 0x2, 
    0x2, 0x492, 0x48e, 0x3, 0x2, 0x2, 0x2, 0x492, 0x493, 0x3, 0x2, 0x2, 
    0x2, 0x493, 0x495, 0x3, 0x2, 0x2, 0x2, 0x494, 0x48c, 0x3, 0x2, 0x2, 
    0x2, 0x494, 0x48d, 0x3, 0x2, 0x2, 0x2, 0x495, 0x4bf, 0x3, 0x2, 0x2, 
    0x2, 0x496, 0x49b, 0x7, 0x82, 0x2, 0x2, 0x497, 0x498, 0x7, 0xb, 0x2, 
    0x2, 0x498, 0x499, 0x5, 0xe0, 0x71, 0x2, 0x499, 0x49a, 0x7, 0xc, 0x2, 
    0x2, 0x49a, 0x49c, 0x3, 0x2, 0x2, 0x2, 0x49b, 0x497, 0x3, 0x2, 0x2, 
    0x2, 0x49b, 0x49c, 0x3, 0x2, 0x2, 0x2, 0x49c, 0x49d, 0x3, 0x2, 0x2, 
    0x2, 0x49d, 0x49e, 0x7, 0xc3, 0x2, 0x2, 0x49e, 0x4a3, 0x7, 0xab, 0x2, 
    0x2, 0x49f, 0x4a0, 0x7, 0xb, 0x2, 0x2, 0x4a0, 0x4a1, 0x5, 0xe2, 0x72, 
    0x2, 0x4a1, 0x4a2, 0x7, 0xc, 0x2, 0x2, 0x4a2, 0x4a4, 0x3, 0x2, 0x2, 
    0x2, 0x4a3, 0x49f, 0x3, 0x2, 0x2, 0x2, 0x4a3, 0x4a4, 0x3, 0x2, 0x2, 
    0x2, 0x4a4, 0x4bf, 0x3, 0x2, 0x2, 0x2, 0x4a5, 0x4aa, 0x7, 0x62, 0x2, 
    0x2, 0x4a6, 0x4a7, 0x7, 0xb, 0x2, 0x2, 0x4a7, 0x4a8, 0x5, 0xe0, 0x71, 
    0x2, 0x4a8, 0x4a9, 0x7, 0xc, 0x2, 0x2, 0x4a9, 0x4ab, 0x3, 0x2, 0x2, 
    0x2, 0x4aa, 0x4a6, 0x3, 0x2, 0x2, 0x2, 0x4aa, 0x4ab, 0x3, 0x2, 0x2, 
    0x2, 0x4ab, 0x4bf, 0x3, 0x2, 0x2, 0x2, 0x4ac, 0x4b1, 0x7, 0x82, 0x2, 
    0x2, 0x4ad, 0x4ae, 0x7, 0xb, 0x2, 0x2, 0x4ae, 0x4af, 0x5, 0xe0, 0x71, 
    0x2, 0x4af, 0x4b0, 0x7, 0xc, 0x2, 0x2, 0x4b0, 0x4b2, 0x3, 0x2, 0x2, 
    0x2, 0x4b1, 0x4ad, 0x3, 0x2, 0x2, 0x2, 0x4b1, 0x4b2, 0x3, 0x2, 0x2, 
    0x2, 0x4b2, 0x4bf, 0x3, 0x2, 0x2, 0x2, 0x4b3, 0x4bc, 0x7, 0xab, 0x2, 
    0x2, 0x4b4, 0x4b5, 0x7, 0xb, 0x2, 0x2, 0x4b5, 0x4b8, 0x5, 0xe0, 0x71, 
    0x2, 0x4b6, 0x4b7, 0x7, 0x8, 0x2, 0x2, 0x4b7, 0x4b9, 0x5, 0xe2, 0x72, 
    0x2, 0x4b8, 0x4b6, 0x3, 0x2, 0x2, 0x2, 0x4b8, 0x4b9, 0x3, 0x2, 0x2, 
    0x2, 0x4b9, 0x4ba, 0x3, 0x2, 0x2, 0x2, 0x4ba, 0x4bb, 0x7, 0xc, 0x2, 
    0x2, 0x4bb, 0x4bd, 0x3, 0x2, 0x2, 0x2, 0x4bc, 0x4b4, 0x3, 0x2, 0x2, 
    0x2, 0x4bc, 0x4bd, 0x3, 0x2, 0x2, 0x2, 0x4bd, 0x4bf, 0x3, 0x2, 0x2, 
    0x2, 0x4be, 0x46a, 0x3, 0x2, 0x2, 0x2, 0x4be, 0x471, 0x3, 0x2, 0x2, 
    0x2, 0x4be, 0x484, 0x3, 0x2, 0x2, 0x2, 0x4be, 0x496, 0x3, 0x2, 0x2, 
    0x2, 0x4be, 0x4a5, 0x3, 0x2, 0x2, 0x2, 0x4be, 0x4ac, 0x3, 0x2, 0x2, 
    0x2, 0x4be, 0x4b3, 0x3, 0x2, 0x2, 0x2, 0x4bf, 0x85, 0x3, 0x2, 0x2, 0x2, 
    0x4c0, 0x4c1, 0x9, 0x12, 0x2, 0x2, 0x4c1, 0x4c2, 0x7, 0xb, 0x2, 0x2, 
    0x4c2, 0x4c4, 0x5, 0xe4, 0x73, 0x2, 0x4c3, 0x4c5, 0x9, 0x13, 0x2, 0x2, 
    0x4c4, 0x4c3, 0x3, 0x2, 0x2, 0x2, 0x4c4, 0x4c5, 0x3, 0x2, 0x2, 0x2, 
    0x4c5, 0x4c6, 0x3, 0x2, 0x2, 0x2, 0x4c6, 0x4c9, 0x7, 0xc, 0x2, 0x2, 
    0x4c7, 0x4c8, 0x7, 0x32, 0x2, 0x2, 0x4c8, 0x4ca, 0x5, 0xce, 0x68, 0x2, 
    0x4c9, 0x4c7, 0x3, 0x2, 0x2, 0x2, 0x4c9, 0x4ca, 0x3, 0x2, 0x2, 0x2, 
    0x4ca, 0x87, 0x3, 0x2, 0x2, 0x2, 0x4cb, 0x4cd, 0x9, 0x12, 0x2, 0x2, 
    0x4cc, 0x4ce, 0x7, 0xd5, 0x2, 0x2, 0x4cd, 0x4cc, 0x3, 0x2, 0x2, 0x2, 
    0x4cd, 0x4ce, 0x3, 0x2, 0x2, 0x2, 0x4ce, 0x4d1, 0x3, 0x2, 0x2, 0x2, 
    0x4cf, 0x4d1, 0x7, 0xd4, 0x2, 0x2, 0x4d0, 0x4cb, 0x3, 0x2, 0x2, 0x2, 
    0x4d0, 0x4cf, 0x3, 0x2, 0x2, 0x2, 0x4d1, 0x4d9, 0x3, 0x2, 0x2, 0x2, 
    0x4d2, 0x4d3, 0x7, 0xb, 0x2, 0x2, 0x4d3, 0x4d5, 0x5, 0xe4, 0x73, 0x2, 
    0x4d4, 0x4d6, 0x9, 0x13, 0x2, 0x2, 0x4d5, 0x4d4, 0x3, 0x2, 0x2, 0x2, 
    0x4d5, 0x4d6, 0x3, 0x2, 0x2, 0x2, 0x4d6, 0x4d7, 0x3, 0x2, 0x2, 0x2, 
    0x4d7, 0x4d8, 0x7, 0xc, 0x2, 0x2, 0x4d8, 0x4da, 0x3, 0x2, 0x2, 0x2, 
    0x4d9, 0x4d2, 0x3, 0x2, 0x2, 0x2, 0x4d9, 0x4da, 0x3, 0x2, 0x2, 0x2, 
    0x4da, 0x4dd, 0x3, 0x2, 0x2, 0x2, 0x4db, 0x4dd, 0x7, 0xbf, 0x2, 0x2, 
    0x4dc, 0x4d0, 0x3, 0x2, 0x2, 0x2, 0x4dc, 0x4db, 0x3, 0x2, 0x2, 0x2, 
    0x4dd, 0x4e0, 0x3, 0x2, 0x2, 0x2, 0x4de, 0x4df, 0x7, 0x32, 0x2, 0x2, 
    0x4df, 0x4e1, 0x5, 0xce, 0x68, 0x2, 0x4e0, 0x4de, 0x3, 0x2, 0x2, 0x2, 
    0x4e0, 0x4e1, 0x3, 0x2, 0x2, 0x2, 0x4e1, 0x89, 0x3, 0x2, 0x2, 0x2, 0x4e2, 
    0x4e3, 0x5, 0x8c, 0x47, 0x2, 0x4e3, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x4e4, 
    0x4e8, 0x7, 0xb, 0x2, 0x2, 0x4e5, 0x4e7, 0x5, 0x20, 0x11, 0x2, 0x4e6, 
    0x4e5, 0x3, 0x2, 0x2, 0x2, 0x4e7, 0x4ea, 0x3, 0x2, 0x2, 0x2, 0x4e8, 
    0x4e6, 0x3, 0x2, 0x2, 0x2, 0x4e8, 0x4e9, 0x3, 0x2, 0x2, 0x2, 0x4e9, 
    0x4eb, 0x3, 0x2, 0x2, 0x2, 0x4ea, 0x4e8, 0x3, 0x2, 0x2, 0x2, 0x4eb, 
    0x4ec, 0x5, 0x8c, 0x47, 0x2, 0x4ec, 0x4ed, 0x7, 0xc, 0x2, 0x2, 0x4ed, 
    0x4fa, 0x3, 0x2, 0x2, 0x2, 0x4ee, 0x4f0, 0x5, 0x20, 0x11, 0x2, 0x4ef, 
    0x4ee, 0x3, 0x2, 0x2, 0x2, 0x4f0, 0x4f3, 0x3, 0x2, 0x2, 0x2, 0x4f1, 
    0x4ef, 0x3, 0x2, 0x2, 0x2, 0x4f1, 0x4f2, 0x3, 0x2, 0x2, 0x2, 0x4f2, 
    0x4f7, 0x3, 0x2, 0x2, 0x2, 0x4f3, 0x4f1, 0x3, 0x2, 0x2, 0x2, 0x4f4, 
    0x4f8, 0x5, 0x24, 0x13, 0x2, 0x4f5, 0x4f8, 0x5, 0x38, 0x1d, 0x2, 0x4f6, 
    0x4f8, 0x5, 0x28, 0x15, 0x2, 0x4f7, 0x4f4, 0x3, 0x2, 0x2, 0x2, 0x4f7, 
    0x4f5, 0x3, 0x2, 0x2, 0x2, 0x4f7, 0x4f6, 0x3, 0x2, 0x2, 0x2, 0x4f8, 
    0x4fa, 0x3, 0x2, 0x2, 0x2, 0x4f9, 0x4e4, 0x3, 0x2, 0x2, 0x2, 0x4f9, 
    0x4f1, 0x3, 0x2, 0x2, 0x2, 0x4fa, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x4fb, 0x500, 
    0x5, 0x90, 0x49, 0x2, 0x4fc, 0x4fd, 0x7, 0x8, 0x2, 0x2, 0x4fd, 0x4ff, 
    0x5, 0x90, 0x49, 0x2, 0x4fe, 0x4fc, 0x3, 0x2, 0x2, 0x2, 0x4ff, 0x502, 
    0x3, 0x2, 0x2, 0x2, 0x500, 0x4fe, 0x3, 0x2, 0x2, 0x2, 0x500, 0x501, 
    0x3, 0x2, 0x2, 0x2, 0x501, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x502, 0x500, 0x3, 
    0x2, 0x2, 0x2, 0x503, 0x508, 0x5, 0xb4, 0x5b, 0x2, 0x504, 0x506, 0x7, 
    0x1a, 0x2, 0x2, 0x505, 0x504, 0x3, 0x2, 0x2, 0x2, 0x505, 0x506, 0x3, 
    0x2, 0x2, 0x2, 0x506, 0x507, 0x3, 0x2, 0x2, 0x2, 0x507, 0x509, 0x5, 
    0xdc, 0x6f, 0x2, 0x508, 0x505, 0x3, 0x2, 0x2, 0x2, 0x508, 0x509, 0x3, 
    0x2, 0x2, 0x2, 0x509, 0x50c, 0x3, 0x2, 0x2, 0x2, 0x50a, 0x50c, 0x5, 
    0xba, 0x5e, 0x2, 0x50b, 0x503, 0x3, 0x2, 0x2, 0x2, 0x50b, 0x50a, 0x3, 
    0x2, 0x2, 0x2, 0x50c, 0x91, 0x3, 0x2, 0x2, 0x2, 0x50d, 0x512, 0x5, 0x94, 
    0x4b, 0x2, 0x50e, 0x50f, 0x7, 0x8, 0x2, 0x2, 0x50f, 0x511, 0x5, 0x94, 
    0x4b, 0x2, 0x510, 0x50e, 0x3, 0x2, 0x2, 0x2, 0x511, 0x514, 0x3, 0x2, 
    0x2, 0x2, 0x512, 0x510, 0x3, 0x2, 0x2, 0x2, 0x512, 0x513, 0x3, 0x2, 
    0x2, 0x2, 0x513, 0x93, 0x3, 0x2, 0x2, 0x2, 0x514, 0x512, 0x3, 0x2, 0x2, 
    0x2, 0x515, 0x516, 0x5, 0xb4, 0x5b, 0x2, 0x516, 0x95, 0x3, 0x2, 0x2, 
    0x2, 0x517, 0x51c, 0x5, 0x98, 0x4d, 0x2, 0x518, 0x519, 0x7, 0x8, 0x2, 
    0x2, 0x519, 0x51b, 0x5, 0x98, 0x4d, 0x2, 0x51a, 0x518, 0x3, 0x2, 0x2, 
    0x2, 0x51b, 0x51e, 0x3, 0x2, 0x2, 0x2, 0x51c, 0x51a, 0x3, 0x2, 0x2, 
    0x2, 0x51c, 0x51d, 0x3, 0x2, 0x2, 0x2, 0x51d, 0x97, 0x3, 0x2, 0x2, 0x2, 
    0x51e, 0x51c, 0x3, 0x2, 0x2, 0x2, 0x51f, 0x521, 0x5, 0xb4, 0x5b, 0x2, 
    0x520, 0x522, 0x9, 0x14, 0x2, 0x2, 0x521, 0x520, 0x3, 0x2, 0x2, 0x2, 
    0x521, 0x522, 0x3, 0x2, 0x2, 0x2, 0x522, 0x99, 0x3, 0x2, 0x2, 0x2, 0x523, 
    0x528, 0x5, 0x9c, 0x4f, 0x2, 0x524, 0x525, 0x7, 0x8, 0x2, 0x2, 0x525, 
    0x527, 0x5, 0x9c, 0x4f, 0x2, 0x526, 0x524, 0x3, 0x2, 0x2, 0x2, 0x527, 
    0x52a, 0x3, 0x2, 0x2, 0x2, 0x528, 0x526, 0x3, 0x2, 0x2, 0x2, 0x528, 
    0x529, 0x3, 0x2, 0x2, 0x2, 0x529, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x52a, 0x528, 
    0x3, 0x2, 0x2, 0x2, 0x52b, 0x52c, 0x5, 0xdc, 0x6f, 0x2, 0x52c, 0x9d, 
    0x3, 0x2, 0x2, 0x2, 0x52d, 0x532, 0x5, 0xa0, 0x51, 0x2, 0x52e, 0x52f, 
    0x7, 0x8, 0x2, 0x2, 0x52f, 0x531, 0x5, 0xa0, 0x51, 0x2, 0x530, 0x52e, 
    0x3, 0x2, 0x2, 0x2, 0x531, 0x534, 0x3, 0x2, 0x2, 0x2, 0x532, 0x530, 
    0x3, 0x2, 0x2, 0x2, 0x532, 0x533, 0x3, 0x2, 0x2, 0x2, 0x533, 0x9f, 0x3, 
    0x2, 0x2, 0x2, 0x534, 0x532, 0x3, 0x2, 0x2, 0x2, 0x535, 0x536, 0x5, 
    0xdc, 0x6f, 0x2, 0x536, 0xa1, 0x3, 0x2, 0x2, 0x2, 0x537, 0x538, 0x7, 
    0xb, 0x2, 0x2, 0x538, 0x539, 0x5, 0xa4, 0x53, 0x2, 0x539, 0x53a, 0x7, 
    0xc, 0x2, 0x2, 0x53a, 0xa3, 0x3, 0x2, 0x2, 0x2, 0x53b, 0x540, 0x5, 0xa6, 
    0x54, 0x2, 0x53c, 0x53d, 0x7, 0x8, 0x2, 0x2, 0x53d, 0x53f, 0x5, 0xa6, 
    0x54, 0x2, 0x53e, 0x53c, 0x3, 0x2, 0x2, 0x2, 0x53f, 0x542, 0x3, 0x2, 
    0x2, 0x2, 0x540, 0x53e, 0x3, 0x2, 0x2, 0x2, 0x540, 0x541, 0x3, 0x2, 
    0x2, 0x2, 0x541, 0xa5, 0x3, 0x2, 0x2, 0x2, 0x542, 0x540, 0x3, 0x2, 0x2, 
    0x2, 0x543, 0x544, 0x5, 0xdc, 0x6f, 0x2, 0x544, 0xa7, 0x3, 0x2, 0x2, 
    0x2, 0x545, 0x54a, 0x5, 0xaa, 0x56, 0x2, 0x546, 0x547, 0x7, 0x8, 0x2, 
    0x2, 0x547, 0x549, 0x5, 0xaa, 0x56, 0x2, 0x548, 0x546, 0x3, 0x2, 0x2, 
    0x2, 0x549, 0x54c, 0x3, 0x2, 0x2, 0x2, 0x54a, 0x548, 0x3, 0x2, 0x2, 
    0x2, 0x54a, 0x54b, 0x3, 0x2, 0x2, 0x2, 0x54b, 0xa9, 0x3, 0x2, 0x2, 0x2, 
    0x54c, 0x54a, 0x3, 0x2, 0x2, 0x2, 0x54d, 0x550, 0x5, 0xd2, 0x6a, 0x2, 
    0x54e, 0x54f, 0x7, 0x6, 0x2, 0x2, 0x54f, 0x551, 0x5, 0xd8, 0x6d, 0x2, 
    0x550, 0x54e, 0x3, 0x2, 0x2, 0x2, 0x550, 0x551, 0x3, 0x2, 0x2, 0x2, 
    0x551, 0xab, 0x3, 0x2, 0x2, 0x2, 0x552, 0x557, 0x5, 0xb4, 0x5b, 0x2, 
    0x553, 0x554, 0x7, 0x8, 0x2, 0x2, 0x554, 0x556, 0x5, 0xb4, 0x5b, 0x2, 
    0x555, 0x553, 0x3, 0x2, 0x2, 0x2, 0x556, 0x559, 0x3, 0x2, 0x2, 0x2, 
    0x557, 0x555, 0x3, 0x2, 0x2, 0x2, 0x557, 0x558, 0x3, 0x2, 0x2, 0x2, 
    0x558, 0xad, 0x3, 0x2, 0x2, 0x2, 0x559, 0x557, 0x3, 0x2, 0x2, 0x2, 0x55a, 
    0x55b, 0x5, 0xb4, 0x5b, 0x2, 0x55b, 0xaf, 0x3, 0x2, 0x2, 0x2, 0x55c, 
    0x55d, 0x5, 0xb4, 0x5b, 0x2, 0x55d, 0xb1, 0x3, 0x2, 0x2, 0x2, 0x55e, 
    0x55f, 0x5, 0xb4, 0x5b, 0x2, 0x55f, 0xb3, 0x3, 0x2, 0x2, 0x2, 0x560, 
    0x561, 0x8, 0x5b, 0x1, 0x2, 0x561, 0x668, 0x5, 0xe6, 0x74, 0x2, 0x562, 
    0x668, 0x5, 0xb6, 0x5c, 0x2, 0x563, 0x668, 0x5, 0xb8, 0x5d, 0x2, 0x564, 
    0x565, 0x7, 0xb, 0x2, 0x2, 0x565, 0x566, 0x5, 0x7c, 0x3f, 0x2, 0x566, 
    0x567, 0x7, 0xc, 0x2, 0x2, 0x567, 0x568, 0x5, 0xb4, 0x5b, 0x2a, 0x568, 
    0x668, 0x3, 0x2, 0x2, 0x2, 0x569, 0x56a, 0x9, 0x15, 0x2, 0x2, 0x56a, 
    0x668, 0x5, 0xb4, 0x5b, 0x29, 0x56b, 0x56c, 0x7, 0x2c, 0x2, 0x2, 0x56c, 
    0x56d, 0x7, 0xb, 0x2, 0x2, 0x56d, 0x56e, 0x5, 0xb4, 0x5b, 0x2, 0x56e, 
    0x56f, 0x9, 0x16, 0x2, 0x2, 0x56f, 0x570, 0x5, 0x7c, 0x3f, 0x2, 0x570, 
    0x571, 0x7, 0xc, 0x2, 0x2, 0x571, 0x668, 0x3, 0x2, 0x2, 0x2, 0x572, 
    0x573, 0x7, 0xcc, 0x2, 0x2, 0x573, 0x574, 0x7, 0xb, 0x2, 0x2, 0x574, 
    0x575, 0x5, 0xb4, 0x5b, 0x2, 0x575, 0x576, 0x7, 0xc, 0x2, 0x2, 0x576, 
    0x668, 0x3, 0x2, 0x2, 0x2, 0x577, 0x58b, 0x7, 0x2b, 0x2, 0x2, 0x578, 
    0x57e, 0x5, 0xb4, 0x5b, 0x2, 0x579, 0x57a, 0x7, 0xd8, 0x2, 0x2, 0x57a, 
    0x57b, 0x5, 0xb4, 0x5b, 0x2, 0x57b, 0x57c, 0x7, 0xc0, 0x2, 0x2, 0x57c, 
    0x57d, 0x5, 0xb4, 0x5b, 0x2, 0x57d, 0x57f, 0x3, 0x2, 0x2, 0x2, 0x57e, 
    0x579, 0x3, 0x2, 0x2, 0x2, 0x57f, 0x580, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x57e, 0x3, 0x2, 0x2, 0x2, 0x580, 0x581, 0x3, 0x2, 0x2, 0x2, 0x581, 
    0x58c, 0x3, 0x2, 0x2, 0x2, 0x582, 0x583, 0x7, 0xd8, 0x2, 0x2, 0x583, 
    0x584, 0x5, 0xb2, 0x5a, 0x2, 0x584, 0x585, 0x7, 0xc0, 0x2, 0x2, 0x585, 
    0x586, 0x5, 0xb4, 0x5b, 0x2, 0x586, 0x588, 0x3, 0x2, 0x2, 0x2, 0x587, 
    0x582, 0x3, 0x2, 0x2, 0x2, 0x588, 0x589, 0x3, 0x2, 0x2, 0x2, 0x589, 
    0x587, 0x3, 0x2, 0x2, 0x2, 0x589, 0x58a, 0x3, 0x2, 0x2, 0x2, 0x58a, 
    0x58c, 0x3, 0x2, 0x2, 0x2, 0x58b, 0x578, 0x3, 0x2, 0x2, 0x2, 0x58b, 
    0x587, 0x3, 0x2, 0x2, 0x2, 0x58c, 0x58f, 0x3, 0x2, 0x2, 0x2, 0x58d, 
    0x58e, 0x7, 0x4b, 0x2, 0x2, 0x58e, 0x590, 0x5, 0xb4, 0x5b, 0x2, 0x58f, 
    0x58d, 0x3, 0x2, 0x2, 0x2, 0x58f, 0x590, 0x3, 0x2, 0x2, 0x2, 0x590, 
    0x591, 0x3, 0x2, 0x2, 0x2, 0x591, 0x592, 0x7, 0x4c, 0x2, 0x2, 0x592, 
    0x668, 0x3, 0x2, 0x2, 0x2, 0x593, 0x594, 0x7, 0x64, 0x2, 0x2, 0x594, 
    0x595, 0x7, 0xb, 0x2, 0x2, 0x595, 0x596, 0x5, 0xb2, 0x5a, 0x2, 0x596, 
    0x597, 0x7, 0x8, 0x2, 0x2, 0x597, 0x598, 0x5, 0xb4, 0x5b, 0x2, 0x598, 
    0x599, 0x7, 0x8, 0x2, 0x2, 0x599, 0x59a, 0x5, 0xb4, 0x5b, 0x2, 0x59a, 
    0x59b, 0x7, 0xc, 0x2, 0x2, 0x59b, 0x668, 0x3, 0x2, 0x2, 0x2, 0x59c, 
    0x59d, 0x7, 0x65, 0x2, 0x2, 0x59d, 0x59e, 0x7, 0xb, 0x2, 0x2, 0x59e, 
    0x59f, 0x5, 0xb4, 0x5b, 0x2, 0x59f, 0x5a0, 0x7, 0x8, 0x2, 0x2, 0x5a0, 
    0x5a1, 0x5, 0xb4, 0x5b, 0x2, 0x5a1, 0x5a2, 0x7, 0xc, 0x2, 0x2, 0x5a2, 
    0x668, 0x3, 0x2, 0x2, 0x2, 0x5a3, 0x5a4, 0x7, 0x90, 0x2, 0x2, 0x5a4, 
    0x5a5, 0x7, 0xb, 0x2, 0x2, 0x5a5, 0x5a6, 0x5, 0xb4, 0x5b, 0x2, 0x5a6, 
    0x5a7, 0x7, 0x8, 0x2, 0x2, 0x5a7, 0x5a8, 0x5, 0xb4, 0x5b, 0x2, 0x5a8, 
    0x5a9, 0x7, 0xc, 0x2, 0x2, 0x5a9, 0x668, 0x3, 0x2, 0x2, 0x2, 0x5aa, 
    0x5ab, 0x7, 0x31, 0x2, 0x2, 0x5ab, 0x5b4, 0x7, 0xb, 0x2, 0x2, 0x5ac, 
    0x5b1, 0x5, 0xb4, 0x5b, 0x2, 0x5ad, 0x5ae, 0x7, 0x8, 0x2, 0x2, 0x5ae, 
    0x5b0, 0x5, 0xb4, 0x5b, 0x2, 0x5af, 0x5ad, 0x3, 0x2, 0x2, 0x2, 0x5b0, 
    0x5b3, 0x3, 0x2, 0x2, 0x2, 0x5b1, 0x5af, 0x3, 0x2, 0x2, 0x2, 0x5b1, 
    0x5b2, 0x3, 0x2, 0x2, 0x2, 0x5b2, 0x5b5, 0x3, 0x2, 0x2, 0x2, 0x5b3, 
    0x5b1, 0x3, 0x2, 0x2, 0x2, 0x5b4, 0x5ac, 0x3, 0x2, 0x2, 0x2, 0x5b4, 
    0x5b5, 0x3, 0x2, 0x2, 0x2, 0x5b5, 0x5b6, 0x3, 0x2, 0x2, 0x2, 0x5b6, 
    0x668, 0x7, 0xc, 0x2, 0x2, 0x5b7, 0x5b8, 0x7, 0x42, 0x2, 0x2, 0x5b8, 
    0x5c1, 0x7, 0xb, 0x2, 0x2, 0x5b9, 0x5be, 0x5, 0xb4, 0x5b, 0x2, 0x5ba, 
    0x5bb, 0x7, 0x8, 0x2, 0x2, 0x5bb, 0x5bd, 0x5, 0xb4, 0x5b, 0x2, 0x5bc, 
    0x5ba, 0x3, 0x2, 0x2, 0x2, 0x5bd, 0x5c0, 0x3, 0x2, 0x2, 0x2, 0x5be, 
    0x5bc, 0x3, 0x2, 0x2, 0x2, 0x5be, 0x5bf, 0x3, 0x2, 0x2, 0x2, 0x5bf, 
    0x5c2, 0x3, 0x2, 0x2, 0x2, 0x5c0, 0x5be, 0x3, 0x2, 0x2, 0x2, 0x5c1, 
    0x5b9, 0x3, 0x2, 0x2, 0x2, 0x5c1, 0x5c2, 0x3, 0x2, 0x2, 0x2, 0x5c2, 
    0x5c3, 0x3, 0x2, 0x2, 0x2, 0x5c3, 0x668, 0x7, 0xc, 0x2, 0x2, 0x5c4, 
    0x5c5, 0x7, 0x33, 0x2, 0x2, 0x5c5, 0x5ce, 0x7, 0xb, 0x2, 0x2, 0x5c6, 
    0x5cb, 0x5, 0xb4, 0x5b, 0x2, 0x5c7, 0x5c8, 0x7, 0x8, 0x2, 0x2, 0x5c8, 
    0x5ca, 0x5, 0xb4, 0x5b, 0x2, 0x5c9, 0x5c7, 0x3, 0x2, 0x2, 0x2, 0x5ca, 
    0x5cd, 0x3, 0x2, 0x2, 0x2, 0x5cb, 0x5c9, 0x3, 0x2, 0x2, 0x2, 0x5cb, 
    0x5cc, 0x3, 0x2, 0x2, 0x2, 0x5cc, 0x5cf, 0x3, 0x2, 0x2, 0x2, 0x5cd, 
    0x5cb, 0x3, 0x2, 0x2, 0x2, 0x5ce, 0x5c6, 0x3, 0x2, 0x2, 0x2, 0x5ce, 
    0x5cf, 0x3, 0x2, 0x2, 0x2, 0x5cf, 0x5d0, 0x3, 0x2, 0x2, 0x2, 0x5d0, 
    0x668, 0x7, 0xc, 0x2, 0x2, 0x5d1, 0x5d2, 0x7, 0x99, 0x2, 0x2, 0x5d2, 
    0x5d3, 0x7, 0xb, 0x2, 0x2, 0x5d3, 0x5d4, 0x5, 0xb4, 0x5b, 0x2, 0x5d4, 
    0x5d5, 0x9, 0x17, 0x2, 0x2, 0x5d5, 0x5d6, 0x5, 0xb4, 0x5b, 0x2, 0x5d6, 
    0x5d7, 0x9, 0x18, 0x2, 0x2, 0x5d7, 0x5da, 0x5, 0xb4, 0x5b, 0x2, 0x5d8, 
    0x5d9, 0x9, 0x19, 0x2, 0x2, 0x5d9, 0x5db, 0x5, 0xb4, 0x5b, 0x2, 0x5da, 
    0x5d8, 0x3, 0x2, 0x2, 0x2, 0x5da, 0x5db, 0x3, 0x2, 0x2, 0x2, 0x5db, 
    0x5dc, 0x3, 0x2, 0x2, 0x2, 0x5dc, 0x5dd, 0x7, 0xc, 0x2, 0x2, 0x5dd, 
    0x668, 0x3, 0x2, 0x2, 0x2, 0x5de, 0x5df, 0x7, 0x9d, 0x2, 0x2, 0x5df, 
    0x5e0, 0x7, 0xb, 0x2, 0x2, 0x5e0, 0x5e1, 0x5, 0xb4, 0x5b, 0x2, 0x5e1, 
    0x5e2, 0x9, 0x1a, 0x2, 0x2, 0x5e2, 0x5e3, 0x5, 0xb4, 0x5b, 0x2, 0x5e3, 
    0x5e4, 0x7, 0xc, 0x2, 0x2, 0x5e4, 0x668, 0x3, 0x2, 0x2, 0x2, 0x5e5, 
    0x5e6, 0x9, 0x1b, 0x2, 0x2, 0x5e6, 0x5f5, 0x7, 0xb, 0x2, 0x2, 0x5e7, 
    0x5e8, 0x5, 0xb4, 0x5b, 0x2, 0x5e8, 0x5e9, 0x7, 0x8, 0x2, 0x2, 0x5e9, 
    0x5ec, 0x5, 0xb4, 0x5b, 0x2, 0x5ea, 0x5eb, 0x7, 0x8, 0x2, 0x2, 0x5eb, 
    0x5ed, 0x5, 0xb4, 0x5b, 0x2, 0x5ec, 0x5ea, 0x3, 0x2, 0x2, 0x2, 0x5ec, 
    0x5ed, 0x3, 0x2, 0x2, 0x2, 0x5ed, 0x5f6, 0x3, 0x2, 0x2, 0x2, 0x5ee, 
    0x5ef, 0x5, 0xb4, 0x5b, 0x2, 0x5ef, 0x5f0, 0x7, 0x5c, 0x2, 0x2, 0x5f0, 
    0x5f3, 0x5, 0xb4, 0x5b, 0x2, 0x5f1, 0x5f2, 0x7, 0x58, 0x2, 0x2, 0x5f2, 
    0x5f4, 0x5, 0xb4, 0x5b, 0x2, 0x5f3, 0x5f1, 0x3, 0x2, 0x2, 0x2, 0x5f3, 
    0x5f4, 0x3, 0x2, 0x2, 0x2, 0x5f4, 0x5f6, 0x3, 0x2, 0x2, 0x2, 0x5f5, 
    0x5e7, 0x3, 0x2, 0x2, 0x2, 0x5f5, 0x5ee, 0x3, 0x2, 0x2, 0x2, 0x5f6, 
    0x5f7, 0x3, 0x2, 0x2, 0x2, 0x5f7, 0x5f8, 0x7, 0xc, 0x2, 0x2, 0x5f8, 
    0x668, 0x3, 0x2, 0x2, 0x2, 0x5f9, 0x5fa, 0x7, 0xc7, 0x2, 0x2, 0x5fa, 
    0x608, 0x7, 0xb, 0x2, 0x2, 0x5fb, 0x5fe, 0x5, 0xb4, 0x5b, 0x2, 0x5fc, 
    0x5fd, 0x7, 0x8, 0x2, 0x2, 0x5fd, 0x5ff, 0x5, 0xb4, 0x5b, 0x2, 0x5fe, 
    0x5fc, 0x3, 0x2, 0x2, 0x2, 0x5fe, 0x5ff, 0x3, 0x2, 0x2, 0x2, 0x5ff, 
    0x609, 0x3, 0x2, 0x2, 0x2, 0x600, 0x602, 0x9, 0x1c, 0x2, 0x2, 0x601, 
    0x600, 0x3, 0x2, 0x2, 0x2, 0x601, 0x602, 0x3, 0x2, 0x2, 0x2, 0x602, 
    0x604, 0x3, 0x2, 0x2, 0x2, 0x603, 0x605, 0x5, 0xb4, 0x5b, 0x2, 0x604, 
    0x603, 0x3, 0x2, 0x2, 0x2, 0x604, 0x605, 0x3, 0x2, 0x2, 0x2, 0x605, 
    0x606, 0x3, 0x2, 0x2, 0x2, 0x606, 0x607, 0x7, 0x5c, 0x2, 0x2, 0x607, 
    0x609, 0x5, 0xb4, 0x5b, 0x2, 0x608, 0x5fb, 0x3, 0x2, 0x2, 0x2, 0x608, 
    0x601, 0x3, 0x2, 0x2, 0x2, 0x609, 0x60a, 0x3, 0x2, 0x2, 0x2, 0x60a, 
    0x60b, 0x7, 0xc, 0x2, 0x2, 0x60b, 0x668, 0x3, 0x2, 0x2, 0x2, 0x60c, 
    0x60d, 0x7, 0x52, 0x2, 0x2, 0x60d, 0x60e, 0x7, 0xb, 0x2, 0x2, 0x60e, 
    0x60f, 0x9, 0x1d, 0x2, 0x2, 0x60f, 0x610, 0x7, 0x5c, 0x2, 0x2, 0x610, 
    0x611, 0x5, 0xb4, 0x5b, 0x2, 0x611, 0x612, 0x7, 0xc, 0x2, 0x2, 0x612, 
    0x668, 0x3, 0x2, 0x2, 0x2, 0x613, 0x614, 0x7, 0x35, 0x2, 0x2, 0x614, 
    0x616, 0x7, 0xb, 0x2, 0x2, 0x615, 0x617, 0x7, 0x16, 0x2, 0x2, 0x616, 
    0x615, 0x3, 0x2, 0x2, 0x2, 0x616, 0x617, 0x3, 0x2, 0x2, 0x2, 0x617, 
    0x618, 0x3, 0x2, 0x2, 0x2, 0x618, 0x619, 0x7, 0xa, 0x2, 0x2, 0x619, 
    0x668, 0x7, 0xc, 0x2, 0x2, 0x61a, 0x61b, 0x7, 0x35, 0x2, 0x2, 0x61b, 
    0x61e, 0x7, 0xb, 0x2, 0x2, 0x61c, 0x61f, 0x7, 0x16, 0x2, 0x2, 0x61d, 
    0x61f, 0x7, 0x47, 0x2, 0x2, 0x61e, 0x61c, 0x3, 0x2, 0x2, 0x2, 0x61e, 
    0x61d, 0x3, 0x2, 0x2, 0x2, 0x61e, 0x61f, 0x3, 0x2, 0x2, 0x2, 0x61f, 
    0x620, 0x3, 0x2, 0x2, 0x2, 0x620, 0x621, 0x5, 0xb4, 0x5b, 0x2, 0x621, 
    0x622, 0x7, 0xc, 0x2, 0x2, 0x622, 0x668, 0x3, 0x2, 0x2, 0x2, 0x623, 
    0x624, 0x7, 0xb9, 0x2, 0x2, 0x624, 0x627, 0x7, 0xb, 0x2, 0x2, 0x625, 
    0x628, 0x7, 0x16, 0x2, 0x2, 0x626, 0x628, 0x7, 0x47, 0x2, 0x2, 0x627, 
    0x625, 0x3, 0x2, 0x2, 0x2, 0x627, 0x626, 0x3, 0x2, 0x2, 0x2, 0x627, 
    0x628, 0x3, 0x2, 0x2, 0x2, 0x628, 0x629, 0x3, 0x2, 0x2, 0x2, 0x629, 
    0x62a, 0x5, 0xb4, 0x5b, 0x2, 0x62a, 0x62b, 0x7, 0xc, 0x2, 0x2, 0x62b, 
    0x668, 0x3, 0x2, 0x2, 0x2, 0x62c, 0x62d, 0x7, 0x1c, 0x2, 0x2, 0x62d, 
    0x630, 0x7, 0xb, 0x2, 0x2, 0x62e, 0x631, 0x7, 0x16, 0x2, 0x2, 0x62f, 
    0x631, 0x7, 0x47, 0x2, 0x2, 0x630, 0x62e, 0x3, 0x2, 0x2, 0x2, 0x630, 
    0x62f, 0x3, 0x2, 0x2, 0x2, 0x630, 0x631, 0x3, 0x2, 0x2, 0x2, 0x631, 
    0x632, 0x3, 0x2, 0x2, 0x2, 0x632, 0x633, 0x5, 0xb4, 0x5b, 0x2, 0x633, 
    0x634, 0x7, 0xc, 0x2, 0x2, 0x634, 0x668, 0x3, 0x2, 0x2, 0x2, 0x635, 
    0x636, 0x7, 0x81, 0x2, 0x2, 0x636, 0x639, 0x7, 0xb, 0x2, 0x2, 0x637, 
    0x63a, 0x7, 0x16, 0x2, 0x2, 0x638, 0x63a, 0x7, 0x47, 0x2, 0x2, 0x639, 
    0x637, 0x3, 0x2, 0x2, 0x2, 0x639, 0x638, 0x3, 0x2, 0x2, 0x2, 0x639, 
    0x63a, 0x3, 0x2, 0x2, 0x2, 0x63a, 0x63b, 0x3, 0x2, 0x2, 0x2, 0x63b, 
    0x63c, 0x5, 0xb4, 0x5b, 0x2, 0x63c, 0x63d, 0x7, 0xc, 0x2, 0x2, 0x63d, 
    0x668, 0x3, 0x2, 0x2, 0x2, 0x63e, 0x63f, 0x7, 0x7c, 0x2, 0x2, 0x63f, 
    0x642, 0x7, 0xb, 0x2, 0x2, 0x640, 0x643, 0x7, 0x16, 0x2, 0x2, 0x641, 
    0x643, 0x7, 0x47, 0x2, 0x2, 0x642, 0x640, 0x3, 0x2, 0x2, 0x2, 0x642, 
    0x641, 0x3, 0x2, 0x2, 0x2, 0x642, 0x643, 0x3, 0x2, 0x2, 0x2, 0x643, 
    0x644, 0x3, 0x2, 0x2, 0x2, 0x644, 0x645, 0x5, 0xb4, 0x5b, 0x2, 0x645, 
    0x646, 0x7, 0xc, 0x2, 0x2, 0x646, 0x668, 0x3, 0x2, 0x2, 0x2, 0x647, 
    0x648, 0x5, 0xbe, 0x60, 0x2, 0x648, 0x651, 0x7, 0xb, 0x2, 0x2, 0x649, 
    0x64e, 0x5, 0xb4, 0x5b, 0x2, 0x64a, 0x64b, 0x7, 0x8, 0x2, 0x2, 0x64b, 
    0x64d, 0x5, 0xb4, 0x5b, 0x2, 0x64c, 0x64a, 0x3, 0x2, 0x2, 0x2, 0x64d, 
    0x650, 0x3, 0x2, 0x2, 0x2, 0x64e, 0x64c, 0x3, 0x2, 0x2, 0x2, 0x64e, 
    0x64f, 0x3, 0x2, 0x2, 0x2, 0x64f, 0x652, 0x3, 0x2, 0x2, 0x2, 0x650, 
    0x64e, 0x3, 0x2, 0x2, 0x2, 0x651, 0x649, 0x3, 0x2, 0x2, 0x2, 0x651, 
    0x652, 0x3, 0x2, 0x2, 0x2, 0x652, 0x653, 0x3, 0x2, 0x2, 0x2, 0x653, 
    0x654, 0x7, 0xc, 0x2, 0x2, 0x654, 0x668, 0x3, 0x2, 0x2, 0x2, 0x655, 
    0x656, 0x7, 0xb, 0x2, 0x2, 0x656, 0x657, 0x5, 0xb4, 0x5b, 0x2, 0x657, 
    0x658, 0x7, 0xc, 0x2, 0x2, 0x658, 0x668, 0x3, 0x2, 0x2, 0x2, 0x659, 
    0x65a, 0x7, 0xb, 0x2, 0x2, 0x65a, 0x65b, 0x5, 0x1e, 0x10, 0x2, 0x65b, 
    0x65c, 0x7, 0xc, 0x2, 0x2, 0x65c, 0x668, 0x3, 0x2, 0x2, 0x2, 0x65d, 
    0x65f, 0x7, 0x8d, 0x2, 0x2, 0x65e, 0x65d, 0x3, 0x2, 0x2, 0x2, 0x65e, 
    0x65f, 0x3, 0x2, 0x2, 0x2, 0x65f, 0x660, 0x3, 0x2, 0x2, 0x2, 0x660, 
    0x661, 0x7, 0x50, 0x2, 0x2, 0x661, 0x662, 0x7, 0xb, 0x2, 0x2, 0x662, 
    0x663, 0x5, 0x1e, 0x10, 0x2, 0x663, 0x664, 0x7, 0xc, 0x2, 0x2, 0x664, 
    0x668, 0x3, 0x2, 0x2, 0x2, 0x665, 0x666, 0x7, 0x8d, 0x2, 0x2, 0x666, 
    0x668, 0x5, 0xb4, 0x5b, 0x6, 0x667, 0x560, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x562, 0x3, 0x2, 0x2, 0x2, 0x667, 0x563, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x564, 0x3, 0x2, 0x2, 0x2, 0x667, 0x569, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x56b, 0x3, 0x2, 0x2, 0x2, 0x667, 0x572, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x577, 0x3, 0x2, 0x2, 0x2, 0x667, 0x593, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x59c, 0x3, 0x2, 0x2, 0x2, 0x667, 0x5a3, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x5aa, 0x3, 0x2, 0x2, 0x2, 0x667, 0x5b7, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x5c4, 0x3, 0x2, 0x2, 0x2, 0x667, 0x5d1, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x5de, 0x3, 0x2, 0x2, 0x2, 0x667, 0x5e5, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x5f9, 0x3, 0x2, 0x2, 0x2, 0x667, 0x60c, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x613, 0x3, 0x2, 0x2, 0x2, 0x667, 0x61a, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x623, 0x3, 0x2, 0x2, 0x2, 0x667, 0x62c, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x635, 0x3, 0x2, 0x2, 0x2, 0x667, 0x63e, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x647, 0x3, 0x2, 0x2, 0x2, 0x667, 0x655, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x659, 0x3, 0x2, 0x2, 0x2, 0x667, 0x65e, 0x3, 0x2, 0x2, 0x2, 0x667, 
    0x665, 0x3, 0x2, 0x2, 0x2, 0x668, 0x6c6, 0x3, 0x2, 0x2, 0x2, 0x669, 
    0x66a, 0xc, 0x27, 0x2, 0x2, 0x66a, 0x66b, 0x9, 0x1e, 0x2, 0x2, 0x66b, 
    0x6c5, 0x5, 0xb4, 0x5b, 0x28, 0x66c, 0x66d, 0xc, 0x26, 0x2, 0x2, 0x66d, 
    0x66e, 0x9, 0x1f, 0x2, 0x2, 0x66e, 0x6c5, 0x5, 0xb4, 0x5b, 0x27, 0x66f, 
    0x670, 0xc, 0x25, 0x2, 0x2, 0x670, 0x671, 0x9, 0x20, 0x2, 0x2, 0x671, 
    0x6c5, 0x5, 0xb4, 0x5b, 0x26, 0x672, 0x674, 0xc, 0xc, 0x2, 0x2, 0x673, 
    0x675, 0x7, 0x8d, 0x2, 0x2, 0x674, 0x673, 0x3, 0x2, 0x2, 0x2, 0x674, 
    0x675, 0x3, 0x2, 0x2, 0x2, 0x675, 0x676, 0x3, 0x2, 0x2, 0x2, 0x676, 
    0x677, 0x7, 0x73, 0x2, 0x2, 0x677, 0x6c5, 0x5, 0xb4, 0x5b, 0xd, 0x678, 
    0x67a, 0xc, 0xb, 0x2, 0x2, 0x679, 0x67b, 0x7, 0x8d, 0x2, 0x2, 0x67a, 
    0x679, 0x3, 0x2, 0x2, 0x2, 0x67a, 0x67b, 0x3, 0x2, 0x2, 0x2, 0x67b, 
    0x67c, 0x3, 0x2, 0x2, 0x2, 0x67c, 0x67d, 0x7, 0x73, 0x2, 0x2, 0x67d, 
    0x67e, 0x5, 0xb4, 0x5b, 0x2, 0x67e, 0x67f, 0x7, 0x4d, 0x2, 0x2, 0x67f, 
    0x680, 0x5, 0xb4, 0x5b, 0xc, 0x680, 0x6c5, 0x3, 0x2, 0x2, 0x2, 0x681, 
    0x682, 0xc, 0x5, 0x2, 0x2, 0x682, 0x683, 0x6, 0x5b, 0xd, 0x2, 0x683, 
    0x684, 0x7, 0x17, 0x2, 0x2, 0x684, 0x6c5, 0x5, 0xb4, 0x5b, 0x6, 0x685, 
    0x686, 0xc, 0x4, 0x2, 0x2, 0x686, 0x687, 0x7, 0xdd, 0x2, 0x2, 0x687, 
    0x6c5, 0x5, 0xb4, 0x5b, 0x5, 0x688, 0x689, 0xc, 0x3, 0x2, 0x2, 0x689, 
    0x68a, 0x7, 0x95, 0x2, 0x2, 0x68a, 0x6c5, 0x5, 0xb4, 0x5b, 0x4, 0x68b, 
    0x68c, 0xc, 0x28, 0x2, 0x2, 0x68c, 0x68d, 0x7, 0xe, 0x2, 0x2, 0x68d, 
    0x68e, 0x5, 0xc0, 0x61, 0x2, 0x68e, 0x697, 0x7, 0xb, 0x2, 0x2, 0x68f, 
    0x694, 0x5, 0xb4, 0x5b, 0x2, 0x690, 0x691, 0x7, 0x8, 0x2, 0x2, 0x691, 
    0x693, 0x5, 0xb4, 0x5b, 0x2, 0x692, 0x690, 0x3, 0x2, 0x2, 0x2, 0x693, 
    0x696, 0x3, 0x2, 0x2, 0x2, 0x694, 0x692, 0x3, 0x2, 0x2, 0x2, 0x694, 
    0x695, 0x3, 0x2, 0x2, 0x2, 0x695, 0x698, 0x3, 0x2, 0x2, 0x2, 0x696, 
    0x694, 0x3, 0x2, 0x2, 0x2, 0x697, 0x68f, 0x3, 0x2, 0x2, 0x2, 0x697, 
    0x698, 0x3, 0x2, 0x2, 0x2, 0x698, 0x699, 0x3, 0x2, 0x2, 0x2, 0x699, 
    0x69a, 0x7, 0xc, 0x2, 0x2, 0x69a, 0x6c5, 0x3, 0x2, 0x2, 0x2, 0x69b, 
    0x69c, 0xc, 0xd, 0x2, 0x2, 0x69c, 0x69e, 0x7, 0x6e, 0x2, 0x2, 0x69d, 
    0x69f, 0x7, 0x8d, 0x2, 0x2, 0x69e, 0x69d, 0x3, 0x2, 0x2, 0x2, 0x69e, 
    0x69f, 0x3, 0x2, 0x2, 0x2, 0x69f, 0x6a0, 0x3, 0x2, 0x2, 0x2, 0x6a0, 
    0x6c5, 0x7, 0x8f, 0x2, 0x2, 0x6a1, 0x6a3, 0xc, 0x9, 0x2, 0x2, 0x6a2, 
    0x6a4, 0x7, 0x8d, 0x2, 0x2, 0x6a3, 0x6a2, 0x3, 0x2, 0x2, 0x2, 0x6a3, 
    0x6a4, 0x3, 0x2, 0x2, 0x2, 0x6a4, 0x6a5, 0x3, 0x2, 0x2, 0x2, 0x6a5, 
    0x6a6, 0x7, 0x66, 0x2, 0x2, 0x6a6, 0x6a7, 0x7, 0xb, 0x2, 0x2, 0x6a7, 
    0x6a8, 0x5, 0x1e, 0x10, 0x2, 0x6a8, 0x6a9, 0x7, 0xc, 0x2, 0x2, 0x6a9, 
    0x6c5, 0x3, 0x2, 0x2, 0x2, 0x6aa, 0x6ac, 0xc, 0x8, 0x2, 0x2, 0x6ab, 
    0x6ad, 0x7, 0x8d, 0x2, 0x2, 0x6ac, 0x6ab, 0x3, 0x2, 0x2, 0x2, 0x6ac, 
    0x6ad, 0x3, 0x2, 0x2, 0x2, 0x6ad, 0x6ae, 0x3, 0x2, 0x2, 0x2, 0x6ae, 
    0x6af, 0x7, 0x66, 0x2, 0x2, 0x6af, 0x6b8, 0x7, 0xb, 0x2, 0x2, 0x6b0, 
    0x6b5, 0x5, 0xb4, 0x5b, 0x2, 0x6b1, 0x6b2, 0x7, 0x8, 0x2, 0x2, 0x6b2, 
    0x6b4, 0x5, 0xb4, 0x5b, 0x2, 0x6b3, 0x6b1, 0x3, 0x2, 0x2, 0x2, 0x6b4, 
    0x6b7, 0x3, 0x2, 0x2, 0x2, 0x6b5, 0x6b3, 0x3, 0x2, 0x2, 0x2, 0x6b5, 
    0x6b6, 0x3, 0x2, 0x2, 0x2, 0x6b6, 0x6b9, 0x3, 0x2, 0x2, 0x2, 0x6b7, 
    0x6b5, 0x3, 0x2, 0x2, 0x2, 0x6b8, 0x6b0, 0x3, 0x2, 0x2, 0x2, 0x6b8, 
    0x6b9, 0x3, 0x2, 0x2, 0x2, 0x6b9, 0x6ba, 0x3, 0x2, 0x2, 0x2, 0x6ba, 
    0x6c5, 0x7, 0xc, 0x2, 0x2, 0x6bb, 0x6bd, 0xc, 0x7, 0x2, 0x2, 0x6bc, 
    0x6be, 0x7, 0x8d, 0x2, 0x2, 0x6bd, 0x6bc, 0x3, 0x2, 0x2, 0x2, 0x6bd, 
    0x6be, 0x3, 0x2, 0x2, 0x2, 0x6be, 0x6bf, 0x3, 0x2, 0x2, 0x2, 0x6bf, 
    0x6c0, 0x7, 0x1d, 0x2, 0x2, 0x6c0, 0x6c1, 0x5, 0xb4, 0x5b, 0x2, 0x6c1, 
    0x6c2, 0x7, 0x17, 0x2, 0x2, 0x6c2, 0x6c3, 0x5, 0xb4, 0x5b, 0x2, 0x6c3, 
    0x6c5, 0x3, 0x2, 0x2, 0x2, 0x6c4, 0x669, 0x3, 0x2, 0x2, 0x2, 0x6c4, 
    0x66c, 0x3, 0x2, 0x2, 0x2, 0x6c4, 0x66f, 0x3, 0x2, 0x2, 0x2, 0x6c4, 
    0x672, 0x3, 0x2, 0x2, 0x2, 0x6c4, 0x678, 0x3, 0x2, 0x2, 0x2, 0x6c4, 
    0x681, 0x3, 0x2, 0x2, 0x2, 0x6c4, 0x685, 0x3, 0x2, 0x2, 0x2, 0x6c4, 
    0x688, 0x3, 0x2, 0x2, 0x2, 0x6c4, 0x68b, 0x3, 0x2, 0x2, 0x2, 0x6c4, 
    0x69b, 0x3, 0x2, 0x2, 0x2, 0x6c4, 0x6a1, 0x3, 0x2, 0x2, 0x2, 0x6c4, 
    0x6aa, 0x3, 0x2, 0x2, 0x2, 0x6c4, 0x6bb, 0x3, 0x2, 0x2, 0x2, 0x6c5, 
    0x6c8, 0x3, 0x2, 0x2, 0x2, 0x6c6, 0x6c4, 0x3, 0x2, 0x2, 0x2, 0x6c6, 
    0x6c7, 0x3, 0x2, 0x2, 0x2, 0x6c7, 0xb5, 0x3, 0x2, 0x2, 0x2, 0x6c8, 0x6c6, 
    0x3, 0x2, 0x2, 0x2, 0x6c9, 0x6ca, 0x9, 0x21, 0x2, 0x2, 0x6ca, 0xb7, 
    0x3, 0x2, 0x2, 0x2, 0x6cb, 0x6cc, 0x5, 0xc2, 0x62, 0x2, 0x6cc, 0x6cd, 
    0x7, 0xe, 0x2, 0x2, 0x6cd, 0x6cf, 0x3, 0x2, 0x2, 0x2, 0x6ce, 0x6cb, 
    0x3, 0x2, 0x2, 0x2, 0x6ce, 0x6cf, 0x3, 0x2, 0x2, 0x2, 0x6cf, 0x6d0, 
    0x3, 0x2, 0x2, 0x2, 0x6d0, 0x6d1, 0x5, 0xc4, 0x63, 0x2, 0x6d1, 0x6d2, 
    0x7, 0xe, 0x2, 0x2, 0x6d2, 0x6d4, 0x3, 0x2, 0x2, 0x2, 0x6d3, 0x6ce, 
    0x3, 0x2, 0x2, 0x2, 0x6d3, 0x6d4, 0x3, 0x2, 0x2, 0x2, 0x6d4, 0x6d5, 
    0x3, 0x2, 0x2, 0x2, 0x6d5, 0x6d6, 0x5, 0xc6, 0x64, 0x2, 0x6d6, 0x6d7, 
    0x7, 0xe, 0x2, 0x2, 0x6d7, 0x6d9, 0x3, 0x2, 0x2, 0x2, 0x6d8, 0x6d3, 
    0x3, 0x2, 0x2, 0x2, 0x6d8, 0x6d9, 0x3, 0x2, 0x2, 0x2, 0x6d9, 0x6da, 
    0x3, 0x2, 0x2, 0x2, 0x6da, 0x6db, 0x5, 0xca, 0x66, 0x2, 0x6db, 0xb9, 
    0x3, 0x2, 0x2, 0x2, 0x6dc, 0x6dd, 0x5, 0xc2, 0x62, 0x2, 0x6dd, 0x6de, 
    0x7, 0xe, 0x2, 0x2, 0x6de, 0x6e0, 0x3, 0x2, 0x2, 0x2, 0x6df, 0x6dc, 
    0x3, 0x2, 0x2, 0x2, 0x6df, 0x6e0, 0x3, 0x2, 0x2, 0x2, 0x6e0, 0x6e1, 
    0x3, 0x2, 0x2, 0x2, 0x6e1, 0x6e2, 0x5, 0xc4, 0x63, 0x2, 0x6e2, 0x6e3, 
    0x7, 0xe, 0x2, 0x2, 0x6e3, 0x6e5, 0x3, 0x2, 0x2, 0x2, 0x6e4, 0x6df, 
    0x3, 0x2, 0x2, 0x2, 0x6e4, 0x6e5, 0x3, 0x2, 0x2, 0x2, 0x6e5, 0x6e6, 
    0x3, 0x2, 0x2, 0x2, 0x6e6, 0x6e7, 0x5, 0xc6, 0x64, 0x2, 0x6e7, 0x6e8, 
    0x7, 0xe, 0x2, 0x2, 0x6e8, 0x6ea, 0x3, 0x2, 0x2, 0x2, 0x6e9, 0x6e4, 
    0x3, 0x2, 0x2, 0x2, 0x6e9, 0x6ea, 0x3, 0x2, 0x2, 0x2, 0x6ea, 0x6eb, 
    0x3, 0x2, 0x2, 0x2, 0x6eb, 0x6ec, 0x7, 0xa, 0x2, 0x2, 0x6ec, 0xbb, 0x3, 
    0x2, 0x2, 0x2, 0x6ed, 0x6ee, 0x5, 0xde, 0x70, 0x2, 0x6ee, 0x6ef, 0x7, 
    0xe, 0x2, 0x2, 0x6ef, 0x6f1, 0x3, 0x2, 0x2, 0x2, 0x6f0, 0x6ed, 0x3, 
    0x2, 0x2, 0x2, 0x6f1, 0x6f4, 0x3, 0x2, 0x2, 0x2, 0x6f2, 0x6f0, 0x3, 
    0x2, 0x2, 0x2, 0x6f2, 0x6f3, 0x3, 0x2, 0x2, 0x2, 0x6f3, 0x6f5, 0x3, 
    0x2, 0x2, 0x2, 0x6f4, 0x6f2, 0x3, 0x2, 0x2, 0x2, 0x6f5, 0x6f6, 0x5, 
    0xdc, 0x6f, 0x2, 0x6f6, 0xbd, 0x3, 0x2, 0x2, 0x2, 0x6f7, 0x6f8, 0x5, 
    0xdc, 0x6f, 0x2, 0x6f8, 0xbf, 0x3, 0x2, 0x2, 0x2, 0x6f9, 0x6fa, 0x5, 
    0xdc, 0x6f, 0x2, 0x6fa, 0xc1, 0x3, 0x2, 0x2, 0x2, 0x6fb, 0x6fc, 0x5, 
    0xdc, 0x6f, 0x2, 0x6fc, 0xc3, 0x3, 0x2, 0x2, 0x2, 0x6fd, 0x6fe, 0x5, 
    0xdc, 0x6f, 0x2, 0x6fe, 0xc5, 0x3, 0x2, 0x2, 0x2, 0x6ff, 0x700, 0x5, 
    0xdc, 0x6f, 0x2, 0x700, 0xc7, 0x3, 0x2, 0x2, 0x2, 0x701, 0x702, 0x5, 
    0xdc, 0x6f, 0x2, 0x702, 0xc9, 0x3, 0x2, 0x2, 0x2, 0x703, 0x704, 0x5, 
    0xdc, 0x6f, 0x2, 0x704, 0xcb, 0x3, 0x2, 0x2, 0x2, 0x705, 0x706, 0x5, 
    0xdc, 0x6f, 0x2, 0x706, 0xcd, 0x3, 0x2, 0x2, 0x2, 0x707, 0x708, 0x5, 
    0xdc, 0x6f, 0x2, 0x708, 0xcf, 0x3, 0x2, 0x2, 0x2, 0x709, 0x70a, 0x5, 
    0xdc, 0x6f, 0x2, 0x70a, 0xd1, 0x3, 0x2, 0x2, 0x2, 0x70b, 0x70c, 0x5, 
    0xdc, 0x6f, 0x2, 0x70c, 0xd3, 0x3, 0x2, 0x2, 0x2, 0x70d, 0x70e, 0x5, 
    0xdc, 0x6f, 0x2, 0x70e, 0xd5, 0x3, 0x2, 0x2, 0x2, 0x70f, 0x710, 0x7, 
    0x5f, 0x2, 0x2, 0x710, 0xd7, 0x3, 0x2, 0x2, 0x2, 0x711, 0x714, 0x5, 
    0xe8, 0x75, 0x2, 0x712, 0x714, 0x5, 0xdc, 0x6f, 0x2, 0x713, 0x711, 0x3, 
    0x2, 0x2, 0x2, 0x713, 0x712, 0x3, 0x2, 0x2, 0x2, 0x714, 0xd9, 0x3, 0x2, 
    0x2, 0x2, 0x715, 0x719, 0x5, 0xe8, 0x75, 0x2, 0x716, 0x719, 0x5, 0xdc, 
    0x6f, 0x2, 0x717, 0x719, 0x5, 0xb4, 0x5b, 0x2, 0x718, 0x715, 0x3, 0x2, 
    0x2, 0x2, 0x718, 0x716, 0x3, 0x2, 0x2, 0x2, 0x718, 0x717, 0x3, 0x2, 
    0x2, 0x2, 0x719, 0xdb, 0x3, 0x2, 0x2, 0x2, 0x71a, 0x71f, 0x7, 0xe0, 
    0x2, 0x2, 0x71b, 0x71f, 0x7, 0xe1, 0x2, 0x2, 0x71c, 0x71f, 0x7, 0xe2, 
    0x2, 0x2, 0x71d, 0x71f, 0x5, 0x110, 0x89, 0x2, 0x71e, 0x71a, 0x3, 0x2, 
    0x2, 0x2, 0x71e, 0x71b, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x71c, 0x3, 0x2, 
    0x2, 0x2, 0x71e, 0x71d, 0x3, 0x2, 0x2, 0x2, 0x71f, 0xdd, 0x3, 0x2, 0x2, 
    0x2, 0x720, 0x721, 0x5, 0xdc, 0x6f, 0x2, 0x721, 0xdf, 0x3, 0x2, 0x2, 
    0x2, 0x722, 0x723, 0x5, 0xec, 0x77, 0x2, 0x723, 0xe1, 0x3, 0x2, 0x2, 
    0x2, 0x724, 0x725, 0x5, 0xec, 0x77, 0x2, 0x725, 0xe3, 0x3, 0x2, 0x2, 
    0x2, 0x726, 0x727, 0x5, 0xec, 0x77, 0x2, 0x727, 0xe5, 0x3, 0x2, 0x2, 
    0x2, 0x728, 0x733, 0x7, 0x8f, 0x2, 0x2, 0x729, 0x733, 0x5, 0xea, 0x76, 
    0x2, 0x72a, 0x733, 0x5, 0xee, 0x78, 0x2, 0x72b, 0x733, 0x5, 0xf2, 0x7a, 
    0x2, 0x72c, 0x733, 0x5, 0xf6, 0x7c, 0x2, 0x72d, 0x733, 0x5, 0xf8, 0x7d, 
    0x2, 0x72e, 0x733, 0x5, 0xfa, 0x7e, 0x2, 0x72f, 0x733, 0x5, 0xfe, 0x80, 
    0x2, 0x730, 0x733, 0x5, 0xfc, 0x7f, 0x2, 0x731, 0x733, 0x5, 0x100, 0x81, 
    0x2, 0x732, 0x728, 0x3, 0x2, 0x2, 0x2, 0x732, 0x729, 0x3, 0x2, 0x2, 
    0x2, 0x732, 0x72a, 0x3, 0x2, 0x2, 0x2, 0x732, 0x72b, 0x3, 0x2, 0x2, 
    0x2, 0x732, 0x72c, 0x3, 0x2, 0x2, 0x2, 0x732, 0x72d, 0x3, 0x2, 0x2, 
    0x2, 0x732, 0x72e, 0x3, 0x2, 0x2, 0x2, 0x732, 0x72f, 0x3, 0x2, 0x2, 
    0x2, 0x732, 0x730, 0x3, 0x2, 0x2, 0x2, 0x732, 0x731, 0x3, 0x2, 0x2, 
    0x2, 0x733, 0xe7, 0x3, 0x2, 0x2, 0x2, 0x734, 0x73b, 0x7, 0x8f, 0x2, 
    0x2, 0x735, 0x73b, 0x5, 0xea, 0x76, 0x2, 0x736, 0x73b, 0x5, 0xee, 0x78, 
    0x2, 0x737, 0x73b, 0x5, 0xf2, 0x7a, 0x2, 0x738, 0x73b, 0x5, 0xf6, 0x7c, 
    0x2, 0x739, 0x73b, 0x5, 0xfe, 0x80, 0x2, 0x73a, 0x734, 0x3, 0x2, 0x2, 
    0x2, 0x73a, 0x735, 0x3, 0x2, 0x2, 0x2, 0x73a, 0x736, 0x3, 0x2, 0x2, 
    0x2, 0x73a, 0x737, 0x3, 0x2, 0x2, 0x2, 0x73a, 0x738, 0x3, 0x2, 0x2, 
    0x2, 0x73a, 0x739, 0x3, 0x2, 0x2, 0x2, 0x73b, 0xe9, 0x3, 0x2, 0x2, 0x2, 
    0x73c, 0x73d, 0x9, 0x22, 0x2, 0x2, 0x73d, 0xeb, 0x3, 0x2, 0x2, 0x2, 
    0x73e, 0x73f, 0x7, 0xe7, 0x2, 0x2, 0x73f, 0xed, 0x3, 0x2, 0x2, 0x2, 
    0x740, 0x743, 0x7, 0xf, 0x2, 0x2, 0x741, 0x743, 0x7, 0x9, 0x2, 0x2, 
    0x742, 0x740, 0x3, 0x2, 0x2, 0x2, 0x742, 0x741, 0x3, 0x2, 0x2, 0x2, 
    0x742, 0x743, 0x3, 0x2, 0x2, 0x2, 0x743, 0x744, 0x3, 0x2, 0x2, 0x2, 
    0x744, 0x745, 0x7, 0xe7, 0x2, 0x2, 0x745, 0xef, 0x3, 0x2, 0x2, 0x2, 
    0x746, 0x747, 0x7, 0xe6, 0x2, 0x2, 0x747, 0xf1, 0x3, 0x2, 0x2, 0x2, 
    0x748, 0x74b, 0x7, 0xf, 0x2, 0x2, 0x749, 0x74b, 0x7, 0x9, 0x2, 0x2, 
    0x74a, 0x748, 0x3, 0x2, 0x2, 0x2, 0x74a, 0x749, 0x3, 0x2, 0x2, 0x2, 
    0x74a, 0x74b, 0x3, 0x2, 0x2, 0x2, 0x74b, 0x74c, 0x3, 0x2, 0x2, 0x2, 
    0x74c, 0x74d, 0x7, 0xe6, 0x2, 0x2, 0x74d, 0xf3, 0x3, 0x2, 0x2, 0x2, 
    0x74e, 0x74f, 0x7, 0xe5, 0x2, 0x2, 0x74f, 0xf5, 0x3, 0x2, 0x2, 0x2, 
    0x750, 0x753, 0x7, 0xf, 0x2, 0x2, 0x751, 0x753, 0x7, 0x9, 0x2, 0x2, 
    0x752, 0x750, 0x3, 0x2, 0x2, 0x2, 0x752, 0x751, 0x3, 0x2, 0x2, 0x2, 
    0x752, 0x753, 0x3, 0x2, 0x2, 0x2, 0x753, 0x754, 0x3, 0x2, 0x2, 0x2, 
    0x754, 0x755, 0x7, 0xe5, 0x2, 0x2, 0x755, 0xf7, 0x3, 0x2, 0x2, 0x2, 
    0x756, 0x757, 0x7, 0x3d, 0x2, 0x2, 0x757, 0x758, 0x5, 0xfe, 0x80, 0x2, 
    0x758, 0xf9, 0x3, 0x2, 0x2, 0x2, 0x759, 0x75a, 0x7, 0xc1, 0x2, 0x2, 
    0x75a, 0x75b, 0x5, 0xfe, 0x80, 0x2, 0x75b, 0xfb, 0x3, 0x2, 0x2, 0x2, 
    0x75c, 0x75d, 0x7, 0xc2, 0x2, 0x2, 0x75d, 0x75e, 0x5, 0xfe, 0x80, 0x2, 
    0x75e, 0xfd, 0x3, 0x2, 0x2, 0x2, 0x75f, 0x760, 0x9, 0x23, 0x2, 0x2, 
    0x760, 0xff, 0x3, 0x2, 0x2, 0x2, 0x761, 0x764, 0x5, 0x102, 0x82, 0x2, 
    0x762, 0x764, 0x5, 0x104, 0x83, 0x2, 0x763, 0x761, 0x3, 0x2, 0x2, 0x2, 
    0x763, 0x762, 0x3, 0x2, 0x2, 0x2, 0x764, 0x101, 0x3, 0x2, 0x2, 0x2, 
    0x765, 0x768, 0x7, 0x6c, 0x2, 0x2, 0x766, 0x769, 0x7, 0xf, 0x2, 0x2, 
    0x767, 0x769, 0x7, 0x9, 0x2, 0x2, 0x768, 0x766, 0x3, 0x2, 0x2, 0x2, 
    0x768, 0x767, 0x3, 0x2, 0x2, 0x2, 0x768, 0x769, 0x3, 0x2, 0x2, 0x2, 
    0x769, 0x76a, 0x3, 0x2, 0x2, 0x2, 0x76a, 0x782, 0x5, 0xfe, 0x80, 0x2, 
    0x76b, 0x770, 0x7, 0xde, 0x2, 0x2, 0x76c, 0x76d, 0x7, 0xb, 0x2, 0x2, 
    0x76d, 0x76e, 0x5, 0xe0, 0x71, 0x2, 0x76e, 0x76f, 0x7, 0xc, 0x2, 0x2, 
    0x76f, 0x771, 0x3, 0x2, 0x2, 0x2, 0x770, 0x76c, 0x3, 0x2, 0x2, 0x2, 
    0x770, 0x771, 0x3, 0x2, 0x2, 0x2, 0x771, 0x783, 0x3, 0x2, 0x2, 0x2, 
    0x772, 0x777, 0x7, 0xde, 0x2, 0x2, 0x773, 0x774, 0x7, 0xb, 0x2, 0x2, 
    0x774, 0x775, 0x5, 0xe0, 0x71, 0x2, 0x775, 0x776, 0x7, 0xc, 0x2, 0x2, 
    0x776, 0x778, 0x3, 0x2, 0x2, 0x2, 0x777, 0x773, 0x3, 0x2, 0x2, 0x2, 
    0x777, 0x778, 0x3, 0x2, 0x2, 0x2, 0x778, 0x779, 0x3, 0x2, 0x2, 0x2, 
    0x779, 0x77a, 0x7, 0xc3, 0x2, 0x2, 0x77a, 0x783, 0x7, 0x85, 0x2, 0x2, 
    0x77b, 0x780, 0x7, 0x85, 0x2, 0x2, 0x77c, 0x77d, 0x7, 0xb, 0x2, 0x2, 
    0x77d, 0x77e, 0x5, 0xe0, 0x71, 0x2, 0x77e, 0x77f, 0x7, 0xc, 0x2, 0x2, 
    0x77f, 0x781, 0x3, 0x2, 0x2, 0x2, 0x780, 0x77c, 0x3, 0x2, 0x2, 0x2, 
    0x780, 0x781, 0x3, 0x2, 0x2, 0x2, 0x781, 0x783, 0x3, 0x2, 0x2, 0x2, 
    0x782, 0x76b, 0x3, 0x2, 0x2, 0x2, 0x782, 0x772, 0x3, 0x2, 0x2, 0x2, 
    0x782, 0x77b, 0x3, 0x2, 0x2, 0x2, 0x783, 0x103, 0x3, 0x2, 0x2, 0x2, 
    0x784, 0x787, 0x7, 0x6c, 0x2, 0x2, 0x785, 0x788, 0x7, 0xf, 0x2, 0x2, 
    0x786, 0x788, 0x7, 0x9, 0x2, 0x2, 0x787, 0x785, 0x3, 0x2, 0x2, 0x2, 
    0x787, 0x786, 0x3, 0x2, 0x2, 0x2, 0x787, 0x788, 0x3, 0x2, 0x2, 0x2, 
    0x788, 0x789, 0x3, 0x2, 0x2, 0x2, 0x789, 0x7de, 0x5, 0xfe, 0x80, 0x2, 
    0x78a, 0x78f, 0x7, 0x3f, 0x2, 0x2, 0x78b, 0x78c, 0x7, 0xb, 0x2, 0x2, 
    0x78c, 0x78d, 0x5, 0xe0, 0x71, 0x2, 0x78d, 0x78e, 0x7, 0xc, 0x2, 0x2, 
    0x78e, 0x790, 0x3, 0x2, 0x2, 0x2, 0x78f, 0x78b, 0x3, 0x2, 0x2, 0x2, 
    0x78f, 0x790, 0x3, 0x2, 0x2, 0x2, 0x790, 0x7df, 0x3, 0x2, 0x2, 0x2, 
    0x791, 0x796, 0x7, 0x3f, 0x2, 0x2, 0x792, 0x793, 0x7, 0xb, 0x2, 0x2, 
    0x793, 0x794, 0x5, 0xe0, 0x71, 0x2, 0x794, 0x795, 0x7, 0xc, 0x2, 0x2, 
    0x795, 0x797, 0x3, 0x2, 0x2, 0x2, 0x796, 0x792, 0x3, 0x2, 0x2, 0x2, 
    0x796, 0x797, 0x3, 0x2, 0x2, 0x2, 0x797, 0x798, 0x3, 0x2, 0x2, 0x2, 
    0x798, 0x7a2, 0x7, 0xc3, 0x2, 0x2, 0x799, 0x7a3, 0x7, 0x62, 0x2, 0x2, 
    0x79a, 0x7a3, 0x7, 0x82, 0x2, 0x2, 0x79b, 0x7a0, 0x7, 0xab, 0x2, 0x2, 
    0x79c, 0x79d, 0x7, 0xb, 0x2, 0x2, 0x79d, 0x79e, 0x5, 0xe2, 0x72, 0x2, 
    0x79e, 0x79f, 0x7, 0xc, 0x2, 0x2, 0x79f, 0x7a1, 0x3, 0x2, 0x2, 0x2, 
    0x7a0, 0x79c, 0x3, 0x2, 0x2, 0x2, 0x7a0, 0x7a1, 0x3, 0x2, 0x2, 0x2, 
    0x7a1, 0x7a3, 0x3, 0x2, 0x2, 0x2, 0x7a2, 0x799, 0x3, 0x2, 0x2, 0x2, 
    0x7a2, 0x79a, 0x3, 0x2, 0x2, 0x2, 0x7a2, 0x79b, 0x3, 0x2, 0x2, 0x2, 
    0x7a3, 0x7df, 0x3, 0x2, 0x2, 0x2, 0x7a4, 0x7a9, 0x7, 0x62, 0x2, 0x2, 
    0x7a5, 0x7a6, 0x7, 0xb, 0x2, 0x2, 0x7a6, 0x7a7, 0x5, 0xe0, 0x71, 0x2, 
    0x7a7, 0x7a8, 0x7, 0xc, 0x2, 0x2, 0x7a8, 0x7aa, 0x3, 0x2, 0x2, 0x2, 
    0x7a9, 0x7a5, 0x3, 0x2, 0x2, 0x2, 0x7a9, 0x7aa, 0x3, 0x2, 0x2, 0x2, 
    0x7aa, 0x7ab, 0x3, 0x2, 0x2, 0x2, 0x7ab, 0x7b4, 0x7, 0xc3, 0x2, 0x2, 
    0x7ac, 0x7b5, 0x7, 0x82, 0x2, 0x2, 0x7ad, 0x7b2, 0x7, 0xab, 0x2, 0x2, 
    0x7ae, 0x7af, 0x7, 0xb, 0x2, 0x2, 0x7af, 0x7b0, 0x5, 0xe2, 0x72, 0x2, 
    0x7b0, 0x7b1, 0x7, 0xc, 0x2, 0x2, 0x7b1, 0x7b3, 0x3, 0x2, 0x2, 0x2, 
    0x7b2, 0x7ae, 0x3, 0x2, 0x2, 0x2, 0x7b2, 0x7b3, 0x3, 0x2, 0x2, 0x2, 
    0x7b3, 0x7b5, 0x3, 0x2, 0x2, 0x2, 0x7b4, 0x7ac, 0x3, 0x2, 0x2, 0x2, 
    0x7b4, 0x7ad, 0x3, 0x2, 0x2, 0x2, 0x7b5, 0x7df, 0x3, 0x2, 0x2, 0x2, 
    0x7b6, 0x7bb, 0x7, 0x82, 0x2, 0x2, 0x7b7, 0x7b8, 0x7, 0xb, 0x2, 0x2, 
    0x7b8, 0x7b9, 0x5, 0xe0, 0x71, 0x2, 0x7b9, 0x7ba, 0x7, 0xc, 0x2, 0x2, 
    0x7ba, 0x7bc, 0x3, 0x2, 0x2, 0x2, 0x7bb, 0x7b7, 0x3, 0x2, 0x2, 0x2, 
    0x7bb, 0x7bc, 0x3, 0x2, 0x2, 0x2, 0x7bc, 0x7bd, 0x3, 0x2, 0x2, 0x2, 
    0x7bd, 0x7be, 0x7, 0xc3, 0x2, 0x2, 0x7be, 0x7c3, 0x7, 0xab, 0x2, 0x2, 
    0x7bf, 0x7c0, 0x7, 0xb, 0x2, 0x2, 0x7c0, 0x7c1, 0x5, 0xe2, 0x72, 0x2, 
    0x7c1, 0x7c2, 0x7, 0xc, 0x2, 0x2, 0x7c2, 0x7c4, 0x3, 0x2, 0x2, 0x2, 
    0x7c3, 0x7bf, 0x3, 0x2, 0x2, 0x2, 0x7c3, 0x7c4, 0x3, 0x2, 0x2, 0x2, 
    0x7c4, 0x7df, 0x3, 0x2, 0x2, 0x2, 0x7c5, 0x7ca, 0x7, 0x62, 0x2, 0x2, 
    0x7c6, 0x7c7, 0x7, 0xb, 0x2, 0x2, 0x7c7, 0x7c8, 0x5, 0xe0, 0x71, 0x2, 
    0x7c8, 0x7c9, 0x7, 0xc, 0x2, 0x2, 0x7c9, 0x7cb, 0x3, 0x2, 0x2, 0x2, 
    0x7ca, 0x7c6, 0x3, 0x2, 0x2, 0x2, 0x7ca, 0x7cb, 0x3, 0x2, 0x2, 0x2, 
    0x7cb, 0x7df, 0x3, 0x2, 0x2, 0x2, 0x7cc, 0x7d1, 0x7, 0x82, 0x2, 0x2, 
    0x7cd, 0x7ce, 0x7, 0xb, 0x2, 0x2, 0x7ce, 0x7cf, 0x5, 0xe0, 0x71, 0x2, 
    0x7cf, 0x7d0, 0x7, 0xc, 0x2, 0x2, 0x7d0, 0x7d2, 0x3, 0x2, 0x2, 0x2, 
    0x7d1, 0x7cd, 0x3, 0x2, 0x2, 0x2, 0x7d1, 0x7d2, 0x3, 0x2, 0x2, 0x2, 
    0x7d2, 0x7df, 0x3, 0x2, 0x2, 0x2, 0x7d3, 0x7dc, 0x7, 0xab, 0x2, 0x2, 
    0x7d4, 0x7d5, 0x7, 0xb, 0x2, 0x2, 0x7d5, 0x7d8, 0x5, 0xe0, 0x71, 0x2, 
    0x7d6, 0x7d7, 0x7, 0x8, 0x2, 0x2, 0x7d7, 0x7d9, 0x5, 0xe2, 0x72, 0x2, 
    0x7d8, 0x7d6, 0x3, 0x2, 0x2, 0x2, 0x7d8, 0x7d9, 0x3, 0x2, 0x2, 0x2, 
    0x7d9, 0x7da, 0x3, 0x2, 0x2, 0x2, 0x7da, 0x7db, 0x7, 0xc, 0x2, 0x2, 
    0x7db, 0x7dd, 0x3, 0x2, 0x2, 0x2, 0x7dc, 0x7d4, 0x3, 0x2, 0x2, 0x2, 
    0x7dc, 0x7dd, 0x3, 0x2, 0x2, 0x2, 0x7dd, 0x7df, 0x3, 0x2, 0x2, 0x2, 
    0x7de, 0x78a, 0x3, 0x2, 0x2, 0x2, 0x7de, 0x791, 0x3, 0x2, 0x2, 0x2, 
    0x7de, 0x7a4, 0x3, 0x2, 0x2, 0x2, 0x7de, 0x7b6, 0x3, 0x2, 0x2, 0x2, 
    0x7de, 0x7c5, 0x3, 0x2, 0x2, 0x2, 0x7de, 0x7cc, 0x3, 0x2, 0x2, 0x2, 
    0x7de, 0x7d3, 0x3, 0x2, 0x2, 0x2, 0x7df, 0x105, 0x3, 0x2, 0x2, 0x2, 
    0x7e0, 0x7e1, 0x7, 0x76, 0x2, 0x2, 0x7e1, 0x7e4, 0x5, 0xfe, 0x80, 0x2, 
    0x7e2, 0x7e3, 0x7, 0x4a, 0x2, 0x2, 0x7e3, 0x7e5, 0x9, 0x24, 0x2, 0x2, 
    0x7e4, 0x7e2, 0x3, 0x2, 0x2, 0x2, 0x7e4, 0x7e5, 0x3, 0x2, 0x2, 0x2, 
    0x7e5, 0x107, 0x3, 0x2, 0x2, 0x2, 0x7e6, 0x7e7, 0x7, 0x78, 0x2, 0x2, 
    0x7e7, 0x7e8, 0x5, 0xc6, 0x64, 0x2, 0x7e8, 0x109, 0x3, 0x2, 0x2, 0x2, 
    0x7e9, 0x7ea, 0x7, 0x79, 0x2, 0x2, 0x7ea, 0x7eb, 0x5, 0xc6, 0x64, 0x2, 
    0x7eb, 0x10b, 0x3, 0x2, 0x2, 0x2, 0x7ec, 0x7ed, 0x7, 0x7a, 0x2, 0x2, 
    0x7ed, 0x7ee, 0x5, 0xc6, 0x64, 0x2, 0x7ee, 0x7ef, 0x5, 0xd0, 0x69, 0x2, 
    0x7ef, 0x10d, 0x3, 0x2, 0x2, 0x2, 0x7f0, 0x7f1, 0x7, 0x7b, 0x2, 0x2, 
    0x7f1, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x7f2, 0x7f3, 0x9, 0x25, 0x2, 0x2, 
    0x7f3, 0x111, 0x3, 0x2, 0x2, 0x2, 0xfd, 0x115, 0x135, 0x139, 0x13b, 
    0x144, 0x14b, 0x150, 0x157, 0x160, 0x168, 0x16e, 0x173, 0x17b, 0x186, 
    0x18b, 0x18f, 0x195, 0x1a1, 0x1a9, 0x1b4, 0x1bc, 0x1c1, 0x1c9, 0x1cc, 
    0x1d2, 0x1d8, 0x1da, 0x1df, 0x1e4, 0x1e8, 0x1ed, 0x1f1, 0x1f7, 0x1ff, 
    0x202, 0x208, 0x20e, 0x210, 0x218, 0x21e, 0x221, 0x226, 0x22f, 0x232, 
    0x236, 0x240, 0x243, 0x249, 0x24d, 0x24f, 0x255, 0x259, 0x25b, 0x25d, 
    0x263, 0x269, 0x270, 0x275, 0x27d, 0x281, 0x28c, 0x290, 0x299, 0x29b, 
    0x2a1, 0x2a6, 0x2ad, 0x2af, 0x2b4, 0x2c1, 0x2ca, 0x2cd, 0x2d1, 0x2da, 
    0x2df, 0x2e4, 0x2ec, 0x2ef, 0x2f3, 0x2fa, 0x302, 0x305, 0x314, 0x31b, 
    0x31f, 0x328, 0x334, 0x33b, 0x34a, 0x34e, 0x357, 0x35c, 0x35f, 0x367, 
    0x36b, 0x370, 0x373, 0x37a, 0x387, 0x38b, 0x39a, 0x3a0, 0x3a5, 0x3ae, 
    0x3b3, 0x3b7, 0x3c1, 0x3c3, 0x3ce, 0x3d9, 0x3e2, 0x3e8, 0x3ec, 0x3ef, 
    0x3f8, 0x3fd, 0x401, 0x405, 0x410, 0x414, 0x41a, 0x41f, 0x421, 0x425, 
    0x42b, 0x42f, 0x43b, 0x443, 0x447, 0x44a, 0x44f, 0x456, 0x45d, 0x466, 
    0x468, 0x46f, 0x476, 0x480, 0x482, 0x489, 0x492, 0x494, 0x49b, 0x4a3, 
    0x4aa, 0x4b1, 0x4b8, 0x4bc, 0x4be, 0x4c4, 0x4c9, 0x4cd, 0x4d0, 0x4d5, 
    0x4d9, 0x4dc, 0x4e0, 0x4e8, 0x4f1, 0x4f7, 0x4f9, 0x500, 0x505, 0x508, 
    0x50b, 0x512, 0x51c, 0x521, 0x528, 0x532, 0x540, 0x54a, 0x550, 0x557, 
    0x580, 0x589, 0x58b, 0x58f, 0x5b1, 0x5b4, 0x5be, 0x5c1, 0x5cb, 0x5ce, 
    0x5da, 0x5ec, 0x5f3, 0x5f5, 0x5fe, 0x601, 0x604, 0x608, 0x616, 0x61e, 
    0x627, 0x630, 0x639, 0x642, 0x64e, 0x651, 0x65e, 0x667, 0x674, 0x67a, 
    0x694, 0x697, 0x69e, 0x6a3, 0x6ac, 0x6b5, 0x6b8, 0x6bd, 0x6c4, 0x6c6, 
    0x6ce, 0x6d3, 0x6d8, 0x6df, 0x6e4, 0x6e9, 0x6f2, 0x713, 0x718, 0x71e, 
    0x732, 0x73a, 0x742, 0x74a, 0x752, 0x763, 0x768, 0x770, 0x777, 0x780, 
    0x782, 0x787, 0x78f, 0x796, 0x7a0, 0x7a2, 0x7a9, 0x7b2, 0x7b4, 0x7bb, 
    0x7c3, 0x7ca, 0x7d1, 0x7d8, 0x7dc, 0x7de, 0x7e4, 
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
