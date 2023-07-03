
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
    setState(259); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(258);
      statement();
      setState(261); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::T__1)
      | (1ULL << sqlParser::T__4)
      | (1ULL << sqlParser::T__8)
      | (1ULL << sqlParser::T__10)
      | (1ULL << sqlParser::COUNT)
      | (1ULL << sqlParser::CREATE))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::DLOAD - 69))
      | (1ULL << (sqlParser::DROP - 69))
      | (1ULL << (sqlParser::EXPLAIN - 69))
      | (1ULL << (sqlParser::LOAD - 69))
      | (1ULL << (sqlParser::LSCOL - 69))
      | (1ULL << (sqlParser::LSIDX - 69))
      | (1ULL << (sqlParser::LSIDXCOL - 69))
      | (1ULL << (sqlParser::LSTBL - 69))
      | (1ULL << (sqlParser::MEMORYLEAKS - 69)))) != 0) || ((((_la - 155) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 155)) & ((1ULL << (sqlParser::SELECT - 155))
      | (1ULL << (sqlParser::SPACE - 155))
      | (1ULL << (sqlParser::STATUS - 155))
      | (1ULL << (sqlParser::SZIDX - 155))
      | (1ULL << (sqlParser::SZTBL - 155))
      | (1ULL << (sqlParser::TRACKMEMORY - 155))
      | (1ULL << (sqlParser::TRUNCATE - 155))
      | (1ULL << (sqlParser::WITH - 155))
      | (1ULL << (sqlParser::HINT_START - 155)))) != 0));
   
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
    setState(263);
    tags();
    setState(264);
    statementChoice();
    setState(265);
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
    setState(285);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(267);
      queryStatement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(268);
      explainStatement();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(269);
      createQuery();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(270);
      dropQuery();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(271);
      truncateQuery();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(272);
      dload();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(273);
      load();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(274);
      lscol();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(275);
      lsidx();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(276);
      lsidxcol();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(277);
      lstbl();
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(278);
      count();
      break;
    }

    case 13: {
      enterOuterAlt(_localctx, 13);
      setState(279);
      space();
      break;
    }

    case 14: {
      enterOuterAlt(_localctx, 14);
      setState(280);
      status();
      break;
    }

    case 15: {
      enterOuterAlt(_localctx, 15);
      setState(281);
      szidx();
      break;
    }

    case 16: {
      enterOuterAlt(_localctx, 16);
      setState(282);
      sztbl();
      break;
    }

    case 17: {
      enterOuterAlt(_localctx, 17);
      setState(283);
      trackmemory();
      break;
    }

    case 18: {
      enterOuterAlt(_localctx, 18);
      setState(284);
      memoryleaks();
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
    setState(291);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__1

    || _la == sqlParser::T__4) {
      setState(289);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
      case 1: {
        setState(287);
        keyOnlyTag();
        break;
      }

      case 2: {
        setState(288);
        keyValueTag();
        break;
      }

      }
      setState(293);
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
    setState(294);
    match(sqlParser::T__1);
    setState(295);
    dynamic_cast<KeyValueTagContext *>(_localctx)->keyName = match(sqlParser::PLAIN_IDENTIFIER);
    setState(296);
    match(sqlParser::T__2);
    setState(297);
    match(sqlParser::T__3);
    setState(300);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::DUBLQUOTED_IDENTIFIER:
      case sqlParser::SINGLQUOTED_STRING:
      case sqlParser::DUBLQUOTED_STRING: {
        setState(298);
        stringLiteral();
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        setState(299);
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
    setState(307);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__4: {
        enterOuterAlt(_localctx, 1);
        setState(302);
        match(sqlParser::T__4);
        setState(303);
        match(sqlParser::PLAIN_IDENTIFIER);
        break;
      }

      case sqlParser::T__1: {
        enterOuterAlt(_localctx, 2);
        setState(304);
        match(sqlParser::T__1);
        setState(305);
        match(sqlParser::PLAIN_IDENTIFIER);
        setState(306);
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
    setState(312);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(309);
      queryHint();
      setState(314);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(315);
    match(sqlParser::EXPLAIN);
    setState(319);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::FORMAT || ((((_la - 160) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 160)) & ((1ULL << (sqlParser::STAGE - 160))
      | (1ULL << (sqlParser::WITH_NAMES - 160))
      | (1ULL << (sqlParser::WITH_TYPES - 160)))) != 0)) {
      setState(316);
      explainOptions();
      setState(321);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(322);
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
    setState(328);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::STAGE: {
        enterOuterAlt(_localctx, 1);
        setState(324);
        explainStageList();
        break;
      }

      case sqlParser::FORMAT: {
        enterOuterAlt(_localctx, 2);
        setState(325);
        explainFormat();
        break;
      }

      case sqlParser::WITH_NAMES: {
        enterOuterAlt(_localctx, 3);
        setState(326);
        match(sqlParser::WITH_NAMES);
        break;
      }

      case sqlParser::WITH_TYPES: {
        enterOuterAlt(_localctx, 4);
        setState(327);
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
    setState(330);
    match(sqlParser::STAGE);
    setState(331);
    explainRange();
    setState(336);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(332);
      match(sqlParser::T__5);
      setState(333);
      explainRange();
      setState(338);
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
    setState(339);
    dynamic_cast<ExplainRangeContext *>(_localctx)->begin = explainRangeChoice();
    setState(342);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__6 || _la == sqlParser::TO) {
      setState(340);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__6 || _la == sqlParser::TO)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(341);
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
    setState(347);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__7:
      case sqlParser::ALL: {
        enterOuterAlt(_localctx, 1);
        setState(344);
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
        setState(345);
        dynamic_cast<ExplainRangeChoiceContext *>(_localctx)->num = match(sqlParser::INTEGER_LITERAL);
        break;
      }

      case sqlParser::ASC:
      case sqlParser::BIT:
      case sqlParser::BITS:
      case sqlParser::BLOCK:
      case sqlParser::BLOOM:
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
      case sqlParser::ESCAPE_CHAR:
      case sqlParser::EXPLAIN:
      case sqlParser::SQL_FALSE:
      case sqlParser::FEDERATION:
      case sqlParser::FIELD_DELIMITER:
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
      case sqlParser::LOOP:
      case sqlParser::LSIDX:
      case sqlParser::LSTBL:
      case sqlParser::MEMORYLEAKS:
      case sqlParser::MINUTES:
      case sqlParser::MONOTONIC:
      case sqlParser::MONTHS:
      case sqlParser::NODE:
      case sqlParser::NOW:
      case sqlParser::OFFSET:
      case sqlParser::PATHS:
      case sqlParser::PLACING:
      case sqlParser::PRETTY:
      case sqlParser::QUARTER:
      case sqlParser::QUARTERS:
      case sqlParser::REFERENCE:
      case sqlParser::REPLICATED:
      case sqlParser::ROWSIZE:
      case sqlParser::SCHEMA:
      case sqlParser::SECONDS:
      case sqlParser::SKIPLIST:
      case sqlParser::SPACE:
      case sqlParser::STAGE:
      case sqlParser::STATS:
      case sqlParser::STATISTICS:
      case sqlParser::STATUS:
      case sqlParser::SUBSTR:
      case sqlParser::SZIDX:
      case sqlParser::SZTBL:
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
        setState(346);
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
    setState(349);
    match(sqlParser::FORMAT);
    setState(350);
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
    setState(355);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(352);
      queryHint();
      setState(357);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(358);
    _la = _input->LA(1);
    if (!(_la == sqlParser::STATS

    || _la == sqlParser::STATISTICS)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(359);
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
    setState(361);
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
    setState(366);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(363);
      queryHint();
      setState(368);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(371);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      setState(369);
      selectQuery();
      break;
    }

    case 2: {
      setState(370);
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
    setState(373);
    match(sqlParser::HINT_START);
    setState(375);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 25) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 25)) & ((1ULL << (sqlParser::ASC - 25))
      | (1ULL << (sqlParser::BIT - 25))
      | (1ULL << (sqlParser::BITS - 25))
      | (1ULL << (sqlParser::BLOCK - 25))
      | (1ULL << (sqlParser::BLOOM - 25))
      | (1ULL << (sqlParser::BTREE - 25))
      | (1ULL << (sqlParser::BYTE - 25))
      | (1ULL << (sqlParser::BYTES - 25))
      | (1ULL << (sqlParser::CATALOG - 25))
      | (1ULL << (sqlParser::CHARS - 25))
      | (1ULL << (sqlParser::CONCAT - 25))
      | (1ULL << (sqlParser::DATA - 25))
      | (1ULL << (sqlParser::DATE_FORMAT - 25))
      | (1ULL << (sqlParser::DAYS - 25))
      | (1ULL << (sqlParser::DECODE - 25))
      | (1ULL << (sqlParser::DESC - 25))
      | (1ULL << (sqlParser::DETAIL - 25))
      | (1ULL << (sqlParser::DIGITS - 25))
      | (1ULL << (sqlParser::DLOAD - 25))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 25))
      | (1ULL << (sqlParser::EXPLAIN - 25))
      | (1ULL << (sqlParser::SQL_FALSE - 25))
      | (1ULL << (sqlParser::FEDERATION - 25))
      | (1ULL << (sqlParser::FIELD_DELIMITER - 25))
      | (1ULL << (sqlParser::FORMAT - 25))
      | (1ULL << (sqlParser::HASH - 25)))) != 0) || ((((_la - 89) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 89)) & ((1ULL << (sqlParser::HASHKEY - 89))
      | (1ULL << (sqlParser::HOURS - 89))
      | (1ULL << (sqlParser::IF - 89))
      | (1ULL << (sqlParser::IFNULL - 89))
      | (1ULL << (sqlParser::INDEX - 89))
      | (1ULL << (sqlParser::KEY - 89))
      | (1ULL << (sqlParser::LIMIT - 89))
      | (1ULL << (sqlParser::LOAD - 89))
      | (1ULL << (sqlParser::LOOP - 89))
      | (1ULL << (sqlParser::LSIDX - 89))
      | (1ULL << (sqlParser::LSTBL - 89))
      | (1ULL << (sqlParser::MEMORYLEAKS - 89))
      | (1ULL << (sqlParser::MINUTES - 89))
      | (1ULL << (sqlParser::MONOTONIC - 89))
      | (1ULL << (sqlParser::MONTHS - 89))
      | (1ULL << (sqlParser::NODE - 89))
      | (1ULL << (sqlParser::NOW - 89))
      | (1ULL << (sqlParser::OFFSET - 89))
      | (1ULL << (sqlParser::PATHS - 89))
      | (1ULL << (sqlParser::PLACING - 89))
      | (1ULL << (sqlParser::PRETTY - 89))
      | (1ULL << (sqlParser::QUARTER - 89))
      | (1ULL << (sqlParser::QUARTERS - 89))
      | (1ULL << (sqlParser::REFERENCE - 89))
      | (1ULL << (sqlParser::REPLICATED - 89))
      | (1ULL << (sqlParser::ROWSIZE - 89))
      | (1ULL << (sqlParser::SCHEMA - 89)))) != 0) || ((((_la - 154) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 154)) & ((1ULL << (sqlParser::SECONDS - 154))
      | (1ULL << (sqlParser::SKIPLIST - 154))
      | (1ULL << (sqlParser::SPACE - 154))
      | (1ULL << (sqlParser::STAGE - 154))
      | (1ULL << (sqlParser::STATS - 154))
      | (1ULL << (sqlParser::STATISTICS - 154))
      | (1ULL << (sqlParser::STATUS - 154))
      | (1ULL << (sqlParser::SUBSTR - 154))
      | (1ULL << (sqlParser::SZIDX - 154))
      | (1ULL << (sqlParser::SZTBL - 154))
      | (1ULL << (sqlParser::TABLESIZE - 154))
      | (1ULL << (sqlParser::TEXT - 154))
      | (1ULL << (sqlParser::TOP - 154))
      | (1ULL << (sqlParser::TRACKMEMORY - 154))
      | (1ULL << (sqlParser::TREE - 154))
      | (1ULL << (sqlParser::TRUNCATE - 154))
      | (1ULL << (sqlParser::TYPE - 154))
      | (1ULL << (sqlParser::TYPEOF - 154))
      | (1ULL << (sqlParser::VALUE - 154))
      | (1ULL << (sqlParser::WEEK - 154))
      | (1ULL << (sqlParser::WEEKS - 154))
      | (1ULL << (sqlParser::WITH_NAMES - 154))
      | (1ULL << (sqlParser::WITH_TYPES - 154))
      | (1ULL << (sqlParser::XOR - 154))
      | (1ULL << (sqlParser::YEARS - 154))
      | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 154))
      | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 154))
      | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 154)))) != 0)) {
      setState(374);
      hint();
    }
    setState(381);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(377);
      match(sqlParser::T__5);
      setState(378);
      hint();
      setState(383);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(384);
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
    setState(425);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(386);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(387);
      dynamic_cast<HintContext *>(_localctx)->exprName = exprHint();
      setState(388);
      match(sqlParser::T__3);
      setState(389);
      expr(0);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(391);
      dynamic_cast<HintContext *>(_localctx)->exprName = exprHint();
      setState(393);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__3) {
        setState(392);
        match(sqlParser::T__3);
      }
      setState(395);
      match(sqlParser::T__8);
      setState(396);
      expr(0);
      setState(401);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(397);
        match(sqlParser::T__5);
        setState(398);
        expr(0);
        setState(403);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(404);
      match(sqlParser::T__9);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(406);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(407);
      match(sqlParser::T__3);
      setState(408);
      hintValue();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(410);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(412);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__3) {
        setState(411);
        match(sqlParser::T__3);
      }
      setState(414);
      match(sqlParser::T__8);
      setState(415);
      hintValue();
      setState(420);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(416);
        match(sqlParser::T__5);
        setState(417);
        hintValue();
        setState(422);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(423);
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
    setState(436);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(427);
      match(sqlParser::WITH);
      setState(428);
      withQuery();
      setState(433);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(429);
        match(sqlParser::T__5);
        setState(430);
        withQuery();
        setState(435);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(438);
    selectExpr();
    setState(442);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(439);
      match(sqlParser::ORDER);
      setState(440);
      match(sqlParser::BY);
      setState(441);
      dynamic_cast<SelectQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(450);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(444);
      match(sqlParser::LIMIT);
      setState(445);
      dynamic_cast<SelectQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(448);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(446);
        match(sqlParser::OFFSET);
        setState(447);
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
    setState(479);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__10:
      case sqlParser::SELECT: {
        enterOuterAlt(_localctx, 1);
        setState(452);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__10 || _la == sqlParser::SELECT)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(455);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::ALL: {
            setState(453);
            match(sqlParser::ALL);
            break;
          }

          case sqlParser::DISTINCT: {
            setState(454);
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
          case sqlParser::ESCAPE_CHAR:
          case sqlParser::EXISTS:
          case sqlParser::EXPLAIN:
          case sqlParser::EXTRACT:
          case sqlParser::SQL_FALSE:
          case sqlParser::FEDERATION:
          case sqlParser::FIELD_DELIMITER:
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
          case sqlParser::LOOP:
          case sqlParser::LSIDX:
          case sqlParser::LSTBL:
          case sqlParser::MAX:
          case sqlParser::MEMORYLEAKS:
          case sqlParser::MIN:
          case sqlParser::MINUTES:
          case sqlParser::MONOTONIC:
          case sqlParser::MONTHS:
          case sqlParser::NODE:
          case sqlParser::NOT:
          case sqlParser::NOW:
          case sqlParser::SQL_NULL:
          case sqlParser::NULLIF:
          case sqlParser::OFFSET:
          case sqlParser::OVERLAY:
          case sqlParser::PATHS:
          case sqlParser::PLACING:
          case sqlParser::POSITION:
          case sqlParser::PRETTY:
          case sqlParser::QUARTER:
          case sqlParser::QUARTERS:
          case sqlParser::REFERENCE:
          case sqlParser::REPLICATED:
          case sqlParser::ROWSIZE:
          case sqlParser::SCHEMA:
          case sqlParser::SECONDS:
          case sqlParser::SKIPLIST:
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
        setState(457);
        selectList();
        setState(460);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
        case 1: {
          setState(458);
          match(sqlParser::FROM);
          setState(459);
          tableExpr(0);
          break;
        }

        }
        setState(464);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
        case 1: {
          setState(462);
          match(sqlParser::WHERE);
          setState(463);
          dynamic_cast<SelectExprContext *>(_localctx)->where = predicate();
          break;
        }

        }
        setState(469);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
        case 1: {
          setState(466);
          match(sqlParser::GROUP);
          setState(467);
          match(sqlParser::BY);
          setState(468);
          dynamic_cast<SelectExprContext *>(_localctx)->groupBy = groupByList();
          break;
        }

        }
        setState(473);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
        case 1: {
          setState(471);
          match(sqlParser::HAVING);
          setState(472);
          dynamic_cast<SelectExprContext *>(_localctx)->having = predicate();
          break;
        }

        }
        break;
      }

      case sqlParser::T__8: {
        enterOuterAlt(_localctx, 2);
        setState(475);
        match(sqlParser::T__8);
        setState(476);
        selectExpr();
        setState(477);
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
    setState(490);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(481);
      match(sqlParser::WITH);
      setState(482);
      withQuery();
      setState(487);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(483);
        match(sqlParser::T__5);
        setState(484);
        withQuery();
        setState(489);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(492);
    setExpr(0);
    setState(496);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(493);
      match(sqlParser::ORDER);
      setState(494);
      match(sqlParser::BY);
      setState(495);
      dynamic_cast<SetQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(504);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(498);
      match(sqlParser::LIMIT);
      setState(499);
      dynamic_cast<SetQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(502);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(500);
        match(sqlParser::OFFSET);
        setState(501);
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
    setState(512);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      setState(507);
      selectExpr();
      break;
    }

    case 2: {
      setState(508);
      match(sqlParser::T__8);
      setState(509);
      setExpr(0);
      setState(510);
      match(sqlParser::T__9);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(526);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<SetExprContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleSetExpr);
        setState(514);

        if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
        setState(515);
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
        setState(518);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::DISTINCT: {
            setState(516);
            match(sqlParser::DISTINCT);
            break;
          }

          case sqlParser::ALL: {
            setState(517);
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
        setState(521);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::CORRESPONDING) {
          setState(520);
          corrClause();
        }
        setState(523);
        setExpr(3); 
      }
      setState(528);
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
    setState(529);
    dynamic_cast<CorrClauseContext *>(_localctx)->corr = match(sqlParser::CORRESPONDING);
    setState(535);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::BY) {
      setState(530);
      match(sqlParser::BY);
      setState(531);
      match(sqlParser::T__8);
      setState(532);
      corrList();
      setState(533);
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
    setState(538);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::RECURSIVE) {
      setState(537);
      match(sqlParser::RECURSIVE);
    }
    setState(540);
    dynamic_cast<WithQueryContext *>(_localctx)->name = tableName();
    setState(542);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__8) {
      setState(541);
      colAliases();
    }
    setState(544);
    match(sqlParser::AS);
    setState(545);
    match(sqlParser::T__8);
    setState(546);
    query();
    setState(547);
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
    setState(581);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<FetchContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(550);
      dynamic_cast<FetchContext *>(_localctx)->ref = tableReference();
      setState(555);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx)) {
      case 1: {
        setState(552);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(551);
          match(sqlParser::AS);
        }
        setState(554);
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
      setState(557);
      match(sqlParser::T__8);
      setState(558);
      query();
      setState(559);
      match(sqlParser::T__9);
      setState(567);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 49, _ctx)) {
      case 1: {
        setState(561);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(560);
          match(sqlParser::AS);
        }
        setState(563);
        dynamic_cast<SubqueryContext *>(_localctx)->as = tableAlias();
        setState(565);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 48, _ctx)) {
        case 1: {
          setState(564);
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
      setState(569);
      match(sqlParser::T__8);
      setState(570);
      valuesExpr();
      setState(571);
      match(sqlParser::T__9);
      setState(579);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 52, _ctx)) {
      case 1: {
        setState(573);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(572);
          match(sqlParser::AS);
        }
        setState(575);
        dynamic_cast<ValuesSubqueryContext *>(_localctx)->as = tableAlias();
        setState(577);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx)) {
        case 1: {
          setState(576);
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
    setState(643);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 63, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(641);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 62, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<CrossJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(583);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(587);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(584);
            queryHint();
            setState(589);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(593);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case sqlParser::T__5: {
              setState(590);
              match(sqlParser::T__5);
              break;
            }

            case sqlParser::CROSS: {
              setState(591);
              match(sqlParser::CROSS);
              setState(592);
              match(sqlParser::JOIN);
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(595);
          tableExpr(7);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<NaturalJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(596);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(600);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(597);
            queryHint();
            setState(602);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(603);
          match(sqlParser::NATURAL);

          setState(605);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 85) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 85)) & ((1ULL << (sqlParser::FULL - 85))
            | (1ULL << (sqlParser::INNER - 85))
            | (1ULL << (sqlParser::LEFT - 85)))) != 0) || _la == sqlParser::RIGHT) {
            setState(604);
            dynamic_cast<NaturalJoinContext *>(_localctx)->type = joinType();
          }
          setState(607);
          match(sqlParser::JOIN);
          setState(608);
          tableExpr(6);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<JoinOnContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(609);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(613);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(610);
            queryHint();
            setState(615);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(617);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::ANTI

          || _la == sqlParser::FULL || ((((_la - 97) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 97)) & ((1ULL << (sqlParser::INNER - 97))
            | (1ULL << (sqlParser::LEFT - 97))
            | (1ULL << (sqlParser::RIGHT - 97))
            | (1ULL << (sqlParser::SEMI - 97)))) != 0)) {
            setState(616);
            dynamic_cast<JoinOnContext *>(_localctx)->type = extendedJoinType();
          }
          setState(619);
          match(sqlParser::JOIN);
          setState(620);
          tableExpr(0);
          setState(621);
          match(sqlParser::ON);
          setState(622);
          joinPredicate();
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<JoinUsingContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(624);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(628);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(625);
            queryHint();
            setState(630);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(632);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::ANTI

          || _la == sqlParser::FULL || ((((_la - 97) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 97)) & ((1ULL << (sqlParser::INNER - 97))
            | (1ULL << (sqlParser::LEFT - 97))
            | (1ULL << (sqlParser::RIGHT - 97))
            | (1ULL << (sqlParser::SEMI - 97)))) != 0)) {
            setState(631);
            dynamic_cast<JoinUsingContext *>(_localctx)->type = extendedJoinType();
          }
          setState(634);
          match(sqlParser::JOIN);
          setState(635);
          tableExpr(0);
          setState(636);
          match(sqlParser::USING);
          setState(637);
          match(sqlParser::T__8);
          setState(638);
          joinUsingList();
          setState(639);
          match(sqlParser::T__9);
          break;
        }

        } 
      }
      setState(645);
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
    setState(654);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 65, _ctx)) {
    case 1: {
      setState(649);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 64, _ctx)) {
      case 1: {
        setState(646);
        dynamic_cast<TableReferenceContext *>(_localctx)->catalog = catalogName();
        setState(647);
        match(sqlParser::T__11);
        break;
      }

      }
      setState(651);
      dynamic_cast<TableReferenceContext *>(_localctx)->schema = schemaName();
      setState(652);
      match(sqlParser::T__11);
      break;
    }

    }
    setState(656);
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
    setState(663);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::INNER: {
        enterOuterAlt(_localctx, 1);
        setState(658);
        match(sqlParser::INNER);
        break;
      }

      case sqlParser::FULL:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 2);
        setState(659);
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
        setState(661);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::OUTER) {
          setState(660);
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
    setState(668);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::FULL:
      case sqlParser::INNER:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 1);
        setState(665);
        joinType();
        break;
      }

      case sqlParser::SEMI: {
        enterOuterAlt(_localctx, 2);
        setState(666);
        match(sqlParser::SEMI);
        break;
      }

      case sqlParser::ANTI: {
        enterOuterAlt(_localctx, 3);
        setState(667);
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
    setState(670);
    match(sqlParser::VALUES);
    setState(671);
    match(sqlParser::T__8);
    setState(672);
    valueList();
    setState(673);
    match(sqlParser::T__9);
    setState(681);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(674);
      match(sqlParser::T__5);
      setState(675);
      match(sqlParser::T__8);
      setState(676);
      valueList();
      setState(677);
      match(sqlParser::T__9);
      setState(683);
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
    setState(693);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(684);
      match(sqlParser::WITH);
      setState(685);
      withQuery();
      setState(690);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(686);
        match(sqlParser::T__5);
        setState(687);
        withQuery();
        setState(692);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(695);
    match(sqlParser::INSERT);
    setState(697);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::INTO) {
      setState(696);
      match(sqlParser::INTO);
    }
    setState(699);
    dynamic_cast<InsertQueryContext *>(_localctx)->ref = tableReference();
    setState(711);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 74, _ctx)) {
    case 1: {
      setState(700);
      match(sqlParser::T__8);
      setState(701);
      columnName();
      setState(706);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(702);
        match(sqlParser::T__5);
        setState(703);
        columnName();
        setState(708);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(709);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(716);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx)) {
    case 1: {
      setState(713);
      valuesExpr();
      break;
    }

    case 2: {
      setState(714);
      selectQuery();
      break;
    }

    case 3: {
      setState(715);
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
    setState(727);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(718);
      match(sqlParser::WITH);
      setState(719);
      withQuery();
      setState(724);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(720);
        match(sqlParser::T__5);
        setState(721);
        withQuery();
        setState(726);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(729);
    match(sqlParser::DELETE);
    setState(731);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::FROM) {
      setState(730);
      match(sqlParser::FROM);
    }
    setState(733);
    dynamic_cast<DeleteQueryContext *>(_localctx)->ref = tableReference();
    setState(738);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(734);
        match(sqlParser::WHERE);
        setState(735);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(736);
        match(sqlParser::FROM);
        setState(737);
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
    match(sqlParser::UPDATE);
    setState(752);
    dynamic_cast<UpdateQueryContext *>(_localctx)->ref = tableReference();
    setState(753);
    match(sqlParser::SET);
    setState(754);
    columnName();
    setState(755);
    match(sqlParser::T__3);
    setState(756);
    expr(0);
    setState(764);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(757);
      match(sqlParser::T__5);
      setState(758);
      columnName();
      setState(759);
      match(sqlParser::T__3);
      setState(760);
      expr(0);
      setState(766);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(771);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(767);
        match(sqlParser::WHERE);
        setState(768);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(769);
        match(sqlParser::FROM);
        setState(770);
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
    setState(775);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 84, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(773);
      createTableQuery();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(774);
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
    setState(777);
    match(sqlParser::CREATE);
    setState(778);
    match(sqlParser::TABLE);
    setState(779);
    tableName();

    setState(780);
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
    setState(782);
    match(sqlParser::CREATE);
    setState(784);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(783);
      match(sqlParser::UNIQUE);
    }
    setState(786);
    match(sqlParser::INDEX);
    setState(787);
    dynamic_cast<CreateIndexQueryContext *>(_localctx)->idxName = indexName();
    setState(788);
    match(sqlParser::ON);
    setState(789);
    tableName();
    setState(790);
    match(sqlParser::T__8);
    setState(791);
    expr(0);
    setState(796);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(792);
      match(sqlParser::T__5);
      setState(793);
      expr(0);
      setState(798);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(799);
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
    setState(803);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 87, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(801);
      dropTableQuery();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(802);
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
    setState(805);
    match(sqlParser::DROP);
    setState(806);
    match(sqlParser::TABLE);
    setState(807);
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
    setState(809);
    match(sqlParser::DROP);
    setState(810);
    match(sqlParser::INDEX);
    setState(818);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 88, _ctx)) {
    case 1: {
      setState(811);
      dynamic_cast<DropIndexQueryContext *>(_localctx)->idxName = indexName();
      setState(812);
      match(sqlParser::ON);
      setState(813);
      dynamic_cast<DropIndexQueryContext *>(_localctx)->tblName = tableName();
      break;
    }

    case 2: {
      setState(815);
      dynamic_cast<DropIndexQueryContext *>(_localctx)->tblName = tableName();
      setState(816);
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
    setState(820);
    match(sqlParser::TRUNCATE);
    setState(822);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::TABLE) {
      setState(821);
      match(sqlParser::TABLE);
    }
    setState(824);
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
    setState(826);
    match(sqlParser::LOAD);
    setState(827);
    tableName();
    setState(828);
    dynamic_cast<LoadContext *>(_localctx)->filePath = stringLiteral();
    setState(839);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__1

    || _la == sqlParser::DATE_FORMAT || _la == sqlParser::ESCAPE_CHAR

    || _la == sqlParser::FIELD_DELIMITER) {
      setState(829);
      loadOption();
      setState(836);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__1)
        | (1ULL << sqlParser::T__5)
        | (1ULL << sqlParser::DATE_FORMAT))) != 0) || _la == sqlParser::ESCAPE_CHAR

      || _la == sqlParser::FIELD_DELIMITER) {
        setState(831);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(830);
          match(sqlParser::T__5);
        }
        setState(833);
        loadOption();
        setState(838);
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
    setState(841);
    match(sqlParser::DLOAD);
    setState(842);
    tableName();
    setState(843);
    match(sqlParser::PATHS);
    setState(845); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(844);
      stringLiteral();
      setState(847); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (((((_la - 206) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 206)) & ((1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 206))
      | (1ULL << (sqlParser::SINGLQUOTED_STRING - 206))
      | (1ULL << (sqlParser::DUBLQUOTED_STRING - 206)))) != 0));
    setState(859);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__1

    || _la == sqlParser::DATE_FORMAT || _la == sqlParser::ESCAPE_CHAR

    || _la == sqlParser::FIELD_DELIMITER) {
      setState(849);
      loadOption();
      setState(856);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__1)
        | (1ULL << sqlParser::T__5)
        | (1ULL << sqlParser::DATE_FORMAT))) != 0) || _la == sqlParser::ESCAPE_CHAR

      || _la == sqlParser::FIELD_DELIMITER) {
        setState(851);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(850);
          match(sqlParser::T__5);
        }
        setState(853);
        loadOption();
        setState(858);
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
    setState(866);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__1: {
        setState(861);
        match(sqlParser::T__1);
        setState(862);
        loadOptionKeys();
        setState(863);
        match(sqlParser::T__2);
        break;
      }

      case sqlParser::DATE_FORMAT:
      case sqlParser::ESCAPE_CHAR:
      case sqlParser::FIELD_DELIMITER: {
        setState(865);
        loadOptionKeys();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(868);
    match(sqlParser::T__3);
    setState(869);
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
    setState(871);
    _la = _input->LA(1);
    if (!(((((_la - 59) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 59)) & ((1ULL << (sqlParser::DATE_FORMAT - 59))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 59))
      | (1ULL << (sqlParser::FIELD_DELIMITER - 59)))) != 0))) {
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
    setState(873);
    match(sqlParser::COUNT);
    setState(874);
    tableName();
   
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
  enterRule(_localctx, 86, sqlParser::RuleSpace);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(876);
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
  enterRule(_localctx, 88, sqlParser::RuleStatus);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(878);
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
  enterRule(_localctx, 90, sqlParser::RuleSzidx);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(880);
    match(sqlParser::SZIDX);
    setState(881);
    tableName();
    setState(882);
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
  enterRule(_localctx, 92, sqlParser::RuleSztbl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(884);
    match(sqlParser::SZTBL);
    setState(885);
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
  enterRule(_localctx, 94, sqlParser::RuleTrackmemory);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(887);
    match(sqlParser::TRACKMEMORY);
   
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
  enterRule(_localctx, 96, sqlParser::RuleMemoryleaks);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(889);
    match(sqlParser::MEMORYLEAKS);
   
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
  enterRule(_localctx, 98, sqlParser::RuleTableDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(891);
    match(sqlParser::T__8);
    setState(894);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::ASC:
      case sqlParser::BIT:
      case sqlParser::BITS:
      case sqlParser::BLOCK:
      case sqlParser::BLOOM:
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
      case sqlParser::ESCAPE_CHAR:
      case sqlParser::EXPLAIN:
      case sqlParser::SQL_FALSE:
      case sqlParser::FEDERATION:
      case sqlParser::FIELD_DELIMITER:
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
      case sqlParser::LOOP:
      case sqlParser::LSIDX:
      case sqlParser::LSTBL:
      case sqlParser::MEMORYLEAKS:
      case sqlParser::MINUTES:
      case sqlParser::MONOTONIC:
      case sqlParser::MONTHS:
      case sqlParser::NODE:
      case sqlParser::NOW:
      case sqlParser::OFFSET:
      case sqlParser::PATHS:
      case sqlParser::PLACING:
      case sqlParser::PRETTY:
      case sqlParser::QUARTER:
      case sqlParser::QUARTERS:
      case sqlParser::REFERENCE:
      case sqlParser::REPLICATED:
      case sqlParser::ROWSIZE:
      case sqlParser::SCHEMA:
      case sqlParser::SECONDS:
      case sqlParser::SKIPLIST:
      case sqlParser::SPACE:
      case sqlParser::STAGE:
      case sqlParser::STATS:
      case sqlParser::STATISTICS:
      case sqlParser::STATUS:
      case sqlParser::SUBSTR:
      case sqlParser::SZIDX:
      case sqlParser::SZTBL:
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
        setState(892);
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
    setState(903);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(896);
      match(sqlParser::T__5);
      setState(899);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ASC:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
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
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXPLAIN:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
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
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSTBL:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::NODE:
        case sqlParser::NOW:
        case sqlParser::OFFSET:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::REFERENCE:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
        case sqlParser::SPACE:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::STATUS:
        case sqlParser::SUBSTR:
        case sqlParser::SZIDX:
        case sqlParser::SZTBL:
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
          setState(897);
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
      setState(905);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(906);
    match(sqlParser::T__9);
    setState(915);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 138) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 138)) & ((1ULL << (sqlParser::PARTITION - 138))
      | (1ULL << (sqlParser::REPLICATED - 138))
      | (1ULL << (sqlParser::ROWSIZE - 138))
      | (1ULL << (sqlParser::TABLESIZE - 138)))) != 0)) {
      setState(913);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::PARTITION: {
          setState(907);
          tablePartitioning();
          break;
        }

        case sqlParser::REPLICATED: {
          setState(908);
          match(sqlParser::REPLICATED);
          break;
        }

        case sqlParser::ROWSIZE: {
          setState(909);
          match(sqlParser::ROWSIZE);
          setState(910);
          integerLiteral();
          break;
        }

        case sqlParser::TABLESIZE: {
          setState(911);
          match(sqlParser::TABLESIZE);
          setState(912);
          integerLiteral();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(917);
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
  enterRule(_localctx, 100, sqlParser::RuleTablePartitioning);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(918);
    match(sqlParser::PARTITION);
    setState(919);
    match(sqlParser::BY);
    setState(920);
    match(sqlParser::T__8);
    setState(921);
    expr(0);
    setState(926);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(922);
      match(sqlParser::T__5);
      setState(923);
      expr(0);
      setState(928);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(929);
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
  enterRule(_localctx, 102, sqlParser::RuleColumnDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(931);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->name = columnName();
    setState(932);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->type = dataType();
    setState(937);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::NOT) {
      setState(933);
      dynamic_cast<ColumnDefinitionContext *>(_localctx)->notNullable = match(sqlParser::NOT);
      setState(934);
      dynamic_cast<ColumnDefinitionContext *>(_localctx)->nullable = match(sqlParser::SQL_NULL);
      setState(939);
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
  enterRule(_localctx, 104, sqlParser::RuleColumnNameList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(940);
    match(sqlParser::T__8);
    setState(941);
    columnName();
    setState(946);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(942);
      match(sqlParser::T__5);
      setState(943);
      columnName();
      setState(948);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(949);
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
  enterRule(_localctx, 106, sqlParser::RuleIndexDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(952);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(951);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
    }
    setState(954);
    _la = _input->LA(1);
    if (!(_la == sqlParser::INDEX

    || _la == sqlParser::KEY)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(956);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 107, _ctx)) {
    case 1: {
      setState(955);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->name = indexName();
      break;
    }

    }
    setState(959);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ON) {
      setState(958);
      match(sqlParser::ON);
    }
    setState(973);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 110, _ctx)) {
    case 1: {
      setState(961);
      expr(0);
      break;
    }

    case 2: {
      setState(962);
      match(sqlParser::T__8);
      setState(963);
      expr(0);
      setState(968);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(964);
        match(sqlParser::T__5);
        setState(965);
        expr(0);
        setState(970);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(971);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(977);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::USING) {
      setState(975);
      match(sqlParser::USING);
      setState(976);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->method = indexMethod();
    }
    setState(981);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(979);
      match(sqlParser::WITH);
      setState(980);
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
  enterRule(_localctx, 108, sqlParser::RuleIndexMethod);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(983);
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
  enterRule(_localctx, 110, sqlParser::RuleDataType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(985);
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
  enterRule(_localctx, 112, sqlParser::RuleSqlDataType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1050);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 124, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(987);
      match(sqlParser::BOOLEAN);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(988);
      match(sqlParser::INTEGER);
      setState(996);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(989);
        match(sqlParser::T__8);
        setState(990);
        precision();
        setState(992);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 30) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 30)) & ((1ULL << (sqlParser::BITS - 30))
          | (1ULL << (sqlParser::BYTES - 30))
          | (1ULL << (sqlParser::DIGITS - 30)))) != 0)) {
          setState(991);
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
        setState(994);
        match(sqlParser::T__9);
      }
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(998);
      match(sqlParser::DECIMAL);
      setState(1013);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(999);
        match(sqlParser::T__8);
        setState(1000);
        precision();
        setState(1002);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::DIGITS) {
          setState(1001);
          match(sqlParser::DIGITS);
        }
        setState(1009);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1004);
          match(sqlParser::T__5);
          setState(1005);
          scale();
          setState(1007);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::DIGITS) {
            setState(1006);
            match(sqlParser::DIGITS);
          }
        }
        setState(1011);
        match(sqlParser::T__9);
      }
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(1015);
      match(sqlParser::FLOAT);
      setState(1023);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1016);
        match(sqlParser::T__8);
        setState(1017);
        precision();
        setState(1019);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 30) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 30)) & ((1ULL << (sqlParser::BITS - 30))
          | (1ULL << (sqlParser::BYTES - 30))
          | (1ULL << (sqlParser::DIGITS - 30)))) != 0)) {
          setState(1018);
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
        setState(1021);
        match(sqlParser::T__9);
      }
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1025);
      match(sqlParser::DATE);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1026);
      match(sqlParser::TIME);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(1027);
      match(sqlParser::TIMESTAMP);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(1028);
      intervalType();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(1029);
      fixedLenChar();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(1030);
      varyingLenChar();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(1031);
      match(sqlParser::BINARY);

      setState(1032);
      match(sqlParser::T__8);
      setState(1033);
      size();
      setState(1035);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::BYTE

      || _la == sqlParser::BYTES) {
        setState(1034);
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
      setState(1037);
      match(sqlParser::T__9);
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(1039);
      match(sqlParser::VARBINARY);
      setState(1047);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1040);
        match(sqlParser::T__8);
        setState(1041);
        size();
        setState(1043);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::BYTE

        || _la == sqlParser::BYTES) {
          setState(1042);
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
        setState(1045);
        match(sqlParser::T__9);
      }
      break;
    }

    case 13: {
      enterOuterAlt(_localctx, 13);
      setState(1049);
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
  enterRule(_localctx, 114, sqlParser::RuleIntervalType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1052);
    match(sqlParser::INTERVAL);
    setState(1055);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::MONTH:
      case sqlParser::YEAR: {
        setState(1053);
        yearMonthIntervalType();
        break;
      }

      case sqlParser::DAY:
      case sqlParser::HOUR:
      case sqlParser::MINUTE:
      case sqlParser::SECOND: {
        setState(1054);
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
  enterRule(_localctx, 116, sqlParser::RuleYearMonthIntervalType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1080);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 129, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1057);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1062);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1058);
        match(sqlParser::T__8);
        setState(1059);
        precision();
        setState(1060);
        match(sqlParser::T__9);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1064);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1069);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1065);
        match(sqlParser::T__8);
        setState(1066);
        precision();
        setState(1067);
        match(sqlParser::T__9);
      }
      setState(1071);
      match(sqlParser::TO);
      setState(1072);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->month = match(sqlParser::MONTH);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1073);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->month = match(sqlParser::MONTH);
      setState(1078);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1074);
        match(sqlParser::T__8);
        setState(1075);
        precision();
        setState(1076);
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
  enterRule(_localctx, 118, sqlParser::RuleDayTimeIntervalType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1166);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 143, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1082);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1087);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1083);
        match(sqlParser::T__8);
        setState(1084);
        precision();
        setState(1085);
        match(sqlParser::T__9);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1089);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1094);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1090);
        match(sqlParser::T__8);
        setState(1091);
        precision();
        setState(1092);
        match(sqlParser::T__9);
      }
      setState(1096);
      match(sqlParser::TO);
      setState(1106);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::HOUR: {
          setState(1097);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->hour = match(sqlParser::HOUR);
          break;
        }

        case sqlParser::MINUTE: {
          setState(1098);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1099);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1104);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__8) {
            setState(1100);
            match(sqlParser::T__8);
            setState(1101);
            scale();
            setState(1102);
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
      setState(1108);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1113);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1109);
        match(sqlParser::T__8);
        setState(1110);
        precision();
        setState(1111);
        match(sqlParser::T__9);
      }
      setState(1115);
      match(sqlParser::TO);
      setState(1124);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::MINUTE: {
          setState(1116);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1117);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1122);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__8) {
            setState(1118);
            match(sqlParser::T__8);
            setState(1119);
            scale();
            setState(1120);
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
      setState(1126);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1131);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1127);
        match(sqlParser::T__8);
        setState(1128);
        precision();
        setState(1129);
        match(sqlParser::T__9);
      }
      setState(1133);
      match(sqlParser::TO);

      setState(1134);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1139);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1135);
        match(sqlParser::T__8);
        setState(1136);
        scale();
        setState(1137);
        match(sqlParser::T__9);
      }
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1141);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1146);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1142);
        match(sqlParser::T__8);
        setState(1143);
        precision();
        setState(1144);
        match(sqlParser::T__9);
      }
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1148);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1153);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1149);
        match(sqlParser::T__8);
        setState(1150);
        precision();
        setState(1151);
        match(sqlParser::T__9);
      }
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(1155);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1164);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1156);
        match(sqlParser::T__8);
        setState(1157);
        precision();
        setState(1160);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1158);
          match(sqlParser::T__5);
          setState(1159);
          scale();
        }
        setState(1162);
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
  enterRule(_localctx, 120, sqlParser::RuleFixedLenChar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1168);
    _la = _input->LA(1);
    if (!(_la == sqlParser::CHAR

    || _la == sqlParser::CHARACTER)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(1169);
    match(sqlParser::T__8);
    setState(1170);
    size();
    setState(1172);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::BYTE)
      | (1ULL << sqlParser::BYTES)
      | (1ULL << sqlParser::CHAR)
      | (1ULL << sqlParser::CHARS))) != 0)) {
      setState(1171);
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
    setState(1174);
    match(sqlParser::T__9);
    setState(1177);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::COLLATE) {
      setState(1175);
      match(sqlParser::COLLATE);
      setState(1176);
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
  enterRule(_localctx, 122, sqlParser::RuleVaryingLenChar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1196);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::CHAR:
      case sqlParser::CHARACTER:
      case sqlParser::VARCHAR: {
        setState(1184);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::CHAR:
          case sqlParser::CHARACTER: {
            setState(1179);
            _la = _input->LA(1);
            if (!(_la == sqlParser::CHAR

            || _la == sqlParser::CHARACTER)) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
            setState(1181);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == sqlParser::VARYING) {
              setState(1180);
              match(sqlParser::VARYING);
            }
            break;
          }

          case sqlParser::VARCHAR: {
            setState(1183);
            match(sqlParser::VARCHAR);
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(1193);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__8) {
          setState(1186);
          match(sqlParser::T__8);
          setState(1187);
          size();
          setState(1189);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::BYTE)
            | (1ULL << sqlParser::BYTES)
            | (1ULL << sqlParser::CHAR)
            | (1ULL << sqlParser::CHARS))) != 0)) {
            setState(1188);
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
          setState(1191);
          match(sqlParser::T__9);
        }
        break;
      }

      case sqlParser::TEXT: {
        setState(1195);
        match(sqlParser::TEXT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(1200);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::COLLATE) {
      setState(1198);
      match(sqlParser::COLLATE);
      setState(1199);
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
  enterRule(_localctx, 124, sqlParser::RuleAsSubquery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1202);
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
  enterRule(_localctx, 126, sqlParser::RuleSelectOrValues);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    setState(1225);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 155, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1204);
      match(sqlParser::T__8);
      setState(1208);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 152, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(1205);
          queryHint(); 
        }
        setState(1210);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 152, _ctx);
      }
      setState(1211);
      selectOrValues();
      setState(1212);
      match(sqlParser::T__9);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1217);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::HINT_START) {
        setState(1214);
        queryHint();
        setState(1219);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(1223);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 154, _ctx)) {
      case 1: {
        setState(1220);
        selectQuery();
        break;
      }

      case 2: {
        setState(1221);
        valuesExpr();
        break;
      }

      case 3: {
        setState(1222);
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
  enterRule(_localctx, 128, sqlParser::RuleSelectList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1227);
    selectListItem();
    setState(1232);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 156, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1228);
        match(sqlParser::T__5);
        setState(1229);
        selectListItem(); 
      }
      setState(1234);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 156, _ctx);
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
  enterRule(_localctx, 130, sqlParser::RuleSelectListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1243);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 159, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1235);
      expr(0);
      setState(1240);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 158, _ctx)) {
      case 1: {
        setState(1237);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(1236);
          match(sqlParser::AS);
        }
        setState(1239);
        dynamic_cast<SelectListItemContext *>(_localctx)->as = identifier();
        break;
      }

      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1242);
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
  enterRule(_localctx, 132, sqlParser::RuleGroupByList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1245);
    groupByListItem();
    setState(1250);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 160, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1246);
        match(sqlParser::T__5);
        setState(1247);
        groupByListItem(); 
      }
      setState(1252);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 160, _ctx);
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
  enterRule(_localctx, 134, sqlParser::RuleGroupByListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1253);
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
  enterRule(_localctx, 136, sqlParser::RuleOrderByList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1255);
    orderByListItem();
    setState(1260);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1256);
      match(sqlParser::T__5);
      setState(1257);
      orderByListItem();
      setState(1262);
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
  enterRule(_localctx, 138, sqlParser::RuleOrderByListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1263);
    expr(0);
    setState(1265);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ASC

    || _la == sqlParser::DESC) {
      setState(1264);
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
  enterRule(_localctx, 140, sqlParser::RuleJoinUsingList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1267);
    joinUsingListItem();
    setState(1272);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1268);
      match(sqlParser::T__5);
      setState(1269);
      joinUsingListItem();
      setState(1274);
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
  enterRule(_localctx, 142, sqlParser::RuleJoinUsingListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1275);
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
  enterRule(_localctx, 144, sqlParser::RuleCorrList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1277);
    corrListItem();
    setState(1282);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1278);
      match(sqlParser::T__5);
      setState(1279);
      corrListItem();
      setState(1284);
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
  enterRule(_localctx, 146, sqlParser::RuleCorrListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1285);
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
  enterRule(_localctx, 148, sqlParser::RuleColAliases);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1287);
    match(sqlParser::T__8);
    setState(1288);
    colAliasList();
    setState(1289);
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
  enterRule(_localctx, 150, sqlParser::RuleColAliasList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1291);
    colAliasListItem();
    setState(1296);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1292);
      match(sqlParser::T__5);
      setState(1293);
      colAliasListItem();
      setState(1298);
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
  enterRule(_localctx, 152, sqlParser::RuleColAliasListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1299);
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
  enterRule(_localctx, 154, sqlParser::RulePropertyList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1301);
    property();
    setState(1306);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1302);
      match(sqlParser::T__5);
      setState(1303);
      property();
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
  enterRule(_localctx, 156, sqlParser::RuleProperty);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1309);
    dynamic_cast<PropertyContext *>(_localctx)->name = propertyName();
    setState(1312);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__3) {
      setState(1310);
      match(sqlParser::T__3);
      setState(1311);
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
  enterRule(_localctx, 158, sqlParser::RuleValueList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1314);
    expr(0);
    setState(1319);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1315);
      match(sqlParser::T__5);
      setState(1316);
      expr(0);
      setState(1321);
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
  enterRule(_localctx, 160, sqlParser::RulePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1322);
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
  enterRule(_localctx, 162, sqlParser::RuleJoinPredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1324);
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
  enterRule(_localctx, 164, sqlParser::RuleCasePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1326);
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
  size_t startState = 166;
  enterRecursionRule(_localctx, 166, sqlParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1591);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 196, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<LiteralValContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(1329);
      literal();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<VarRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1330);
      variableName();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ColRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1331);
      columnRef();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<InlineCastContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1332);
      match(sqlParser::T__8);
      setState(1333);
      dataType();
      setState(1334);
      match(sqlParser::T__9);
      setState(1335);
      expr(40);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<UnaryOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1337);
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
      setState(1338);
      expr(39);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<CastOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1339);
      match(sqlParser::CAST);
      setState(1340);
      match(sqlParser::T__8);
      setState(1341);
      expr(0);
      setState(1342);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5

      || _la == sqlParser::AS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1343);
      dataType();
      setState(1344);
      match(sqlParser::T__9);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<TypeOfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1346);
      match(sqlParser::TYPEOF);
      setState(1347);
      match(sqlParser::T__8);
      setState(1348);
      expr(0);
      setState(1349);
      match(sqlParser::T__9);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<CaseOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1351);
      match(sqlParser::CASE);
      setState(1371);
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
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXPLAIN:
        case sqlParser::EXTRACT:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
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
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSTBL:
        case sqlParser::MAX:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MIN:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::NODE:
        case sqlParser::NOT:
        case sqlParser::NOW:
        case sqlParser::SQL_NULL:
        case sqlParser::NULLIF:
        case sqlParser::OFFSET:
        case sqlParser::OVERLAY:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::REFERENCE:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
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
          setState(1352);
          dynamic_cast<CaseOpContext *>(_localctx)->caseExpr = expr(0);
          setState(1358); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1353);
            match(sqlParser::WHEN);
            setState(1354);
            expr(0);
            setState(1355);
            match(sqlParser::THEN);
            setState(1356);
            expr(0);
            setState(1360); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

        case sqlParser::WHEN: {
          setState(1367); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1362);
            match(sqlParser::WHEN);
            setState(1363);
            casePredicate();
            setState(1364);
            match(sqlParser::THEN);
            setState(1365);
            expr(0);
            setState(1369); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(1375);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ELSE) {
        setState(1373);
        match(sqlParser::ELSE);
        setState(1374);
        dynamic_cast<CaseOpContext *>(_localctx)->elseExpr = expr(0);
      }
      setState(1377);
      match(sqlParser::END);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<IfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1379);
      match(sqlParser::IF);
      setState(1380);
      match(sqlParser::T__8);
      setState(1381);
      casePredicate();
      setState(1382);
      match(sqlParser::T__5);
      setState(1383);
      expr(0);
      setState(1384);
      match(sqlParser::T__5);
      setState(1385);
      expr(0);
      setState(1386);
      match(sqlParser::T__9);
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<IfNullOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1388);
      match(sqlParser::IFNULL);
      setState(1389);
      match(sqlParser::T__8);
      setState(1390);
      expr(0);
      setState(1391);
      match(sqlParser::T__5);
      setState(1392);
      expr(0);
      setState(1393);
      match(sqlParser::T__9);
      break;
    }

    case 11: {
      _localctx = _tracker.createInstance<NullIfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1395);
      match(sqlParser::NULLIF);
      setState(1396);
      match(sqlParser::T__8);
      setState(1397);
      expr(0);
      setState(1398);
      match(sqlParser::T__5);
      setState(1399);
      expr(0);
      setState(1400);
      match(sqlParser::T__9);
      break;
    }

    case 12: {
      _localctx = _tracker.createInstance<CoalesceOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1402);
      match(sqlParser::COALESCE);
      setState(1403);
      match(sqlParser::T__8);
      setState(1412);
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
        | (1ULL << sqlParser::DAYS)
        | (1ULL << sqlParser::DECODE))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 65)) & ((1ULL << (sqlParser::DESC - 65))
        | (1ULL << (sqlParser::DETAIL - 65))
        | (1ULL << (sqlParser::DIGITS - 65))
        | (1ULL << (sqlParser::DLOAD - 65))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 65))
        | (1ULL << (sqlParser::EXISTS - 65))
        | (1ULL << (sqlParser::EXPLAIN - 65))
        | (1ULL << (sqlParser::EXTRACT - 65))
        | (1ULL << (sqlParser::SQL_FALSE - 65))
        | (1ULL << (sqlParser::FEDERATION - 65))
        | (1ULL << (sqlParser::FIELD_DELIMITER - 65))
        | (1ULL << (sqlParser::FORMAT - 65))
        | (1ULL << (sqlParser::HASH - 65))
        | (1ULL << (sqlParser::HASHKEY - 65))
        | (1ULL << (sqlParser::HOURS - 65))
        | (1ULL << (sqlParser::IF - 65))
        | (1ULL << (sqlParser::IFNULL - 65))
        | (1ULL << (sqlParser::INDEX - 65))
        | (1ULL << (sqlParser::INTERVAL - 65))
        | (1ULL << (sqlParser::KEY - 65))
        | (1ULL << (sqlParser::LIMIT - 65))
        | (1ULL << (sqlParser::LOAD - 65))
        | (1ULL << (sqlParser::LOOP - 65))
        | (1ULL << (sqlParser::LSIDX - 65))
        | (1ULL << (sqlParser::LSTBL - 65))
        | (1ULL << (sqlParser::MAX - 65))
        | (1ULL << (sqlParser::MEMORYLEAKS - 65))
        | (1ULL << (sqlParser::MIN - 65))
        | (1ULL << (sqlParser::MINUTES - 65))
        | (1ULL << (sqlParser::MONOTONIC - 65))
        | (1ULL << (sqlParser::MONTHS - 65))
        | (1ULL << (sqlParser::NODE - 65))
        | (1ULL << (sqlParser::NOT - 65))
        | (1ULL << (sqlParser::NOW - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::SQL_NULL - 129))
        | (1ULL << (sqlParser::NULLIF - 129))
        | (1ULL << (sqlParser::OFFSET - 129))
        | (1ULL << (sqlParser::OVERLAY - 129))
        | (1ULL << (sqlParser::PATHS - 129))
        | (1ULL << (sqlParser::PLACING - 129))
        | (1ULL << (sqlParser::POSITION - 129))
        | (1ULL << (sqlParser::PRETTY - 129))
        | (1ULL << (sqlParser::QUARTER - 129))
        | (1ULL << (sqlParser::QUARTERS - 129))
        | (1ULL << (sqlParser::REFERENCE - 129))
        | (1ULL << (sqlParser::REPLICATED - 129))
        | (1ULL << (sqlParser::ROWSIZE - 129))
        | (1ULL << (sqlParser::SCHEMA - 129))
        | (1ULL << (sqlParser::SECONDS - 129))
        | (1ULL << (sqlParser::SKIPLIST - 129))
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
        | (1ULL << (sqlParser::TABLESIZE - 129))
        | (1ULL << (sqlParser::TEXT - 129))
        | (1ULL << (sqlParser::TIME - 129))
        | (1ULL << (sqlParser::TIMESTAMP - 129))
        | (1ULL << (sqlParser::TOP - 129))
        | (1ULL << (sqlParser::TRACKMEMORY - 129))
        | (1ULL << (sqlParser::TREE - 129))
        | (1ULL << (sqlParser::TRIM - 129))
        | (1ULL << (sqlParser::TRUNCATE - 129))
        | (1ULL << (sqlParser::SQL_TRUE - 129))
        | (1ULL << (sqlParser::TYPE - 129))
        | (1ULL << (sqlParser::TYPEOF - 129))
        | (1ULL << (sqlParser::VALUE - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::WEEK - 194))
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
        setState(1404);
        expr(0);
        setState(1409);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1405);
          match(sqlParser::T__5);
          setState(1406);
          expr(0);
          setState(1411);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1414);
      match(sqlParser::T__9);
      break;
    }

    case 13: {
      _localctx = _tracker.createInstance<DecodeOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1415);
      match(sqlParser::DECODE);
      setState(1416);
      match(sqlParser::T__8);
      setState(1425);
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
        | (1ULL << sqlParser::DAYS)
        | (1ULL << sqlParser::DECODE))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 65)) & ((1ULL << (sqlParser::DESC - 65))
        | (1ULL << (sqlParser::DETAIL - 65))
        | (1ULL << (sqlParser::DIGITS - 65))
        | (1ULL << (sqlParser::DLOAD - 65))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 65))
        | (1ULL << (sqlParser::EXISTS - 65))
        | (1ULL << (sqlParser::EXPLAIN - 65))
        | (1ULL << (sqlParser::EXTRACT - 65))
        | (1ULL << (sqlParser::SQL_FALSE - 65))
        | (1ULL << (sqlParser::FEDERATION - 65))
        | (1ULL << (sqlParser::FIELD_DELIMITER - 65))
        | (1ULL << (sqlParser::FORMAT - 65))
        | (1ULL << (sqlParser::HASH - 65))
        | (1ULL << (sqlParser::HASHKEY - 65))
        | (1ULL << (sqlParser::HOURS - 65))
        | (1ULL << (sqlParser::IF - 65))
        | (1ULL << (sqlParser::IFNULL - 65))
        | (1ULL << (sqlParser::INDEX - 65))
        | (1ULL << (sqlParser::INTERVAL - 65))
        | (1ULL << (sqlParser::KEY - 65))
        | (1ULL << (sqlParser::LIMIT - 65))
        | (1ULL << (sqlParser::LOAD - 65))
        | (1ULL << (sqlParser::LOOP - 65))
        | (1ULL << (sqlParser::LSIDX - 65))
        | (1ULL << (sqlParser::LSTBL - 65))
        | (1ULL << (sqlParser::MAX - 65))
        | (1ULL << (sqlParser::MEMORYLEAKS - 65))
        | (1ULL << (sqlParser::MIN - 65))
        | (1ULL << (sqlParser::MINUTES - 65))
        | (1ULL << (sqlParser::MONOTONIC - 65))
        | (1ULL << (sqlParser::MONTHS - 65))
        | (1ULL << (sqlParser::NODE - 65))
        | (1ULL << (sqlParser::NOT - 65))
        | (1ULL << (sqlParser::NOW - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::SQL_NULL - 129))
        | (1ULL << (sqlParser::NULLIF - 129))
        | (1ULL << (sqlParser::OFFSET - 129))
        | (1ULL << (sqlParser::OVERLAY - 129))
        | (1ULL << (sqlParser::PATHS - 129))
        | (1ULL << (sqlParser::PLACING - 129))
        | (1ULL << (sqlParser::POSITION - 129))
        | (1ULL << (sqlParser::PRETTY - 129))
        | (1ULL << (sqlParser::QUARTER - 129))
        | (1ULL << (sqlParser::QUARTERS - 129))
        | (1ULL << (sqlParser::REFERENCE - 129))
        | (1ULL << (sqlParser::REPLICATED - 129))
        | (1ULL << (sqlParser::ROWSIZE - 129))
        | (1ULL << (sqlParser::SCHEMA - 129))
        | (1ULL << (sqlParser::SECONDS - 129))
        | (1ULL << (sqlParser::SKIPLIST - 129))
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
        | (1ULL << (sqlParser::TABLESIZE - 129))
        | (1ULL << (sqlParser::TEXT - 129))
        | (1ULL << (sqlParser::TIME - 129))
        | (1ULL << (sqlParser::TIMESTAMP - 129))
        | (1ULL << (sqlParser::TOP - 129))
        | (1ULL << (sqlParser::TRACKMEMORY - 129))
        | (1ULL << (sqlParser::TREE - 129))
        | (1ULL << (sqlParser::TRIM - 129))
        | (1ULL << (sqlParser::TRUNCATE - 129))
        | (1ULL << (sqlParser::SQL_TRUE - 129))
        | (1ULL << (sqlParser::TYPE - 129))
        | (1ULL << (sqlParser::TYPEOF - 129))
        | (1ULL << (sqlParser::VALUE - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::WEEK - 194))
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
        setState(1417);
        expr(0);
        setState(1422);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1418);
          match(sqlParser::T__5);
          setState(1419);
          expr(0);
          setState(1424);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1427);
      match(sqlParser::T__9);
      break;
    }

    case 14: {
      _localctx = _tracker.createInstance<ConcatOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1428);
      match(sqlParser::CONCAT);
      setState(1429);
      match(sqlParser::T__8);
      setState(1438);
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
        | (1ULL << sqlParser::DAYS)
        | (1ULL << sqlParser::DECODE))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 65)) & ((1ULL << (sqlParser::DESC - 65))
        | (1ULL << (sqlParser::DETAIL - 65))
        | (1ULL << (sqlParser::DIGITS - 65))
        | (1ULL << (sqlParser::DLOAD - 65))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 65))
        | (1ULL << (sqlParser::EXISTS - 65))
        | (1ULL << (sqlParser::EXPLAIN - 65))
        | (1ULL << (sqlParser::EXTRACT - 65))
        | (1ULL << (sqlParser::SQL_FALSE - 65))
        | (1ULL << (sqlParser::FEDERATION - 65))
        | (1ULL << (sqlParser::FIELD_DELIMITER - 65))
        | (1ULL << (sqlParser::FORMAT - 65))
        | (1ULL << (sqlParser::HASH - 65))
        | (1ULL << (sqlParser::HASHKEY - 65))
        | (1ULL << (sqlParser::HOURS - 65))
        | (1ULL << (sqlParser::IF - 65))
        | (1ULL << (sqlParser::IFNULL - 65))
        | (1ULL << (sqlParser::INDEX - 65))
        | (1ULL << (sqlParser::INTERVAL - 65))
        | (1ULL << (sqlParser::KEY - 65))
        | (1ULL << (sqlParser::LIMIT - 65))
        | (1ULL << (sqlParser::LOAD - 65))
        | (1ULL << (sqlParser::LOOP - 65))
        | (1ULL << (sqlParser::LSIDX - 65))
        | (1ULL << (sqlParser::LSTBL - 65))
        | (1ULL << (sqlParser::MAX - 65))
        | (1ULL << (sqlParser::MEMORYLEAKS - 65))
        | (1ULL << (sqlParser::MIN - 65))
        | (1ULL << (sqlParser::MINUTES - 65))
        | (1ULL << (sqlParser::MONOTONIC - 65))
        | (1ULL << (sqlParser::MONTHS - 65))
        | (1ULL << (sqlParser::NODE - 65))
        | (1ULL << (sqlParser::NOT - 65))
        | (1ULL << (sqlParser::NOW - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::SQL_NULL - 129))
        | (1ULL << (sqlParser::NULLIF - 129))
        | (1ULL << (sqlParser::OFFSET - 129))
        | (1ULL << (sqlParser::OVERLAY - 129))
        | (1ULL << (sqlParser::PATHS - 129))
        | (1ULL << (sqlParser::PLACING - 129))
        | (1ULL << (sqlParser::POSITION - 129))
        | (1ULL << (sqlParser::PRETTY - 129))
        | (1ULL << (sqlParser::QUARTER - 129))
        | (1ULL << (sqlParser::QUARTERS - 129))
        | (1ULL << (sqlParser::REFERENCE - 129))
        | (1ULL << (sqlParser::REPLICATED - 129))
        | (1ULL << (sqlParser::ROWSIZE - 129))
        | (1ULL << (sqlParser::SCHEMA - 129))
        | (1ULL << (sqlParser::SECONDS - 129))
        | (1ULL << (sqlParser::SKIPLIST - 129))
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
        | (1ULL << (sqlParser::TABLESIZE - 129))
        | (1ULL << (sqlParser::TEXT - 129))
        | (1ULL << (sqlParser::TIME - 129))
        | (1ULL << (sqlParser::TIMESTAMP - 129))
        | (1ULL << (sqlParser::TOP - 129))
        | (1ULL << (sqlParser::TRACKMEMORY - 129))
        | (1ULL << (sqlParser::TREE - 129))
        | (1ULL << (sqlParser::TRIM - 129))
        | (1ULL << (sqlParser::TRUNCATE - 129))
        | (1ULL << (sqlParser::SQL_TRUE - 129))
        | (1ULL << (sqlParser::TYPE - 129))
        | (1ULL << (sqlParser::TYPEOF - 129))
        | (1ULL << (sqlParser::VALUE - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::WEEK - 194))
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
        setState(1430);
        expr(0);
        setState(1435);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1431);
          match(sqlParser::T__5);
          setState(1432);
          expr(0);
          setState(1437);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1440);
      match(sqlParser::T__9);
      break;
    }

    case 15: {
      _localctx = _tracker.createInstance<OverlayFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1441);
      match(sqlParser::OVERLAY);
      setState(1442);
      match(sqlParser::T__8);
      setState(1443);
      expr(0);
      setState(1444);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5 || _la == sqlParser::PLACING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1445);
      expr(0);
      setState(1446);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5 || _la == sqlParser::FROM)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1447);
      expr(0);
      setState(1450);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__5 || _la == sqlParser::FOR) {
        setState(1448);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__5 || _la == sqlParser::FOR)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(1449);
        expr(0);
      }
      setState(1452);
      match(sqlParser::T__9);
      break;
    }

    case 16: {
      _localctx = _tracker.createInstance<PositionFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1454);
      match(sqlParser::POSITION);
      setState(1455);
      match(sqlParser::T__8);
      setState(1456);
      expr(0);
      setState(1457);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5 || _la == sqlParser::IN)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1458);
      expr(0);
      setState(1459);
      match(sqlParser::T__9);
      break;
    }

    case 17: {
      _localctx = _tracker.createInstance<SubStringFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1461);
      _la = _input->LA(1);
      if (!(_la == sqlParser::SUBSTR

      || _la == sqlParser::SUBSTRING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1462);
      match(sqlParser::T__8);
      setState(1477);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 182, _ctx)) {
      case 1: {
        setState(1463);
        dynamic_cast<SubStringFuncContext *>(_localctx)->src = expr(0);
        setState(1464);
        match(sqlParser::T__5);
        setState(1465);
        dynamic_cast<SubStringFuncContext *>(_localctx)->start = expr(0);
        setState(1468);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1466);
          match(sqlParser::T__5);
          setState(1467);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      case 2: {
        setState(1470);
        dynamic_cast<SubStringFuncContext *>(_localctx)->src = expr(0);
        setState(1471);
        match(sqlParser::FROM);
        setState(1472);
        dynamic_cast<SubStringFuncContext *>(_localctx)->start = expr(0);
        setState(1475);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::FOR) {
          setState(1473);
          match(sqlParser::FOR);
          setState(1474);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      }
      setState(1479);
      match(sqlParser::T__9);
      break;
    }

    case 18: {
      _localctx = _tracker.createInstance<TrimFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1481);
      match(sqlParser::TRIM);
      setState(1482);
      match(sqlParser::T__8);
      setState(1496);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 186, _ctx)) {
      case 1: {
        setState(1483);
        expr(0);
        setState(1486);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1484);
          match(sqlParser::T__5);
          setState(1485);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        break;
      }

      case 2: {
        setState(1489);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::BOTH || _la == sqlParser::LEADING || _la == sqlParser::TRAILING) {
          setState(1488);
          _la = _input->LA(1);
          if (!(_la == sqlParser::BOTH || _la == sqlParser::LEADING || _la == sqlParser::TRAILING)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        setState(1492);
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
          | (1ULL << sqlParser::DAYS)
          | (1ULL << sqlParser::DECODE))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 65)) & ((1ULL << (sqlParser::DESC - 65))
          | (1ULL << (sqlParser::DETAIL - 65))
          | (1ULL << (sqlParser::DIGITS - 65))
          | (1ULL << (sqlParser::DLOAD - 65))
          | (1ULL << (sqlParser::ESCAPE_CHAR - 65))
          | (1ULL << (sqlParser::EXISTS - 65))
          | (1ULL << (sqlParser::EXPLAIN - 65))
          | (1ULL << (sqlParser::EXTRACT - 65))
          | (1ULL << (sqlParser::SQL_FALSE - 65))
          | (1ULL << (sqlParser::FEDERATION - 65))
          | (1ULL << (sqlParser::FIELD_DELIMITER - 65))
          | (1ULL << (sqlParser::FORMAT - 65))
          | (1ULL << (sqlParser::HASH - 65))
          | (1ULL << (sqlParser::HASHKEY - 65))
          | (1ULL << (sqlParser::HOURS - 65))
          | (1ULL << (sqlParser::IF - 65))
          | (1ULL << (sqlParser::IFNULL - 65))
          | (1ULL << (sqlParser::INDEX - 65))
          | (1ULL << (sqlParser::INTERVAL - 65))
          | (1ULL << (sqlParser::KEY - 65))
          | (1ULL << (sqlParser::LIMIT - 65))
          | (1ULL << (sqlParser::LOAD - 65))
          | (1ULL << (sqlParser::LOOP - 65))
          | (1ULL << (sqlParser::LSIDX - 65))
          | (1ULL << (sqlParser::LSTBL - 65))
          | (1ULL << (sqlParser::MAX - 65))
          | (1ULL << (sqlParser::MEMORYLEAKS - 65))
          | (1ULL << (sqlParser::MIN - 65))
          | (1ULL << (sqlParser::MINUTES - 65))
          | (1ULL << (sqlParser::MONOTONIC - 65))
          | (1ULL << (sqlParser::MONTHS - 65))
          | (1ULL << (sqlParser::NODE - 65))
          | (1ULL << (sqlParser::NOT - 65))
          | (1ULL << (sqlParser::NOW - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::SQL_NULL - 129))
          | (1ULL << (sqlParser::NULLIF - 129))
          | (1ULL << (sqlParser::OFFSET - 129))
          | (1ULL << (sqlParser::OVERLAY - 129))
          | (1ULL << (sqlParser::PATHS - 129))
          | (1ULL << (sqlParser::PLACING - 129))
          | (1ULL << (sqlParser::POSITION - 129))
          | (1ULL << (sqlParser::PRETTY - 129))
          | (1ULL << (sqlParser::QUARTER - 129))
          | (1ULL << (sqlParser::QUARTERS - 129))
          | (1ULL << (sqlParser::REFERENCE - 129))
          | (1ULL << (sqlParser::REPLICATED - 129))
          | (1ULL << (sqlParser::ROWSIZE - 129))
          | (1ULL << (sqlParser::SCHEMA - 129))
          | (1ULL << (sqlParser::SECONDS - 129))
          | (1ULL << (sqlParser::SKIPLIST - 129))
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
          | (1ULL << (sqlParser::TABLESIZE - 129))
          | (1ULL << (sqlParser::TEXT - 129))
          | (1ULL << (sqlParser::TIME - 129))
          | (1ULL << (sqlParser::TIMESTAMP - 129))
          | (1ULL << (sqlParser::TOP - 129))
          | (1ULL << (sqlParser::TRACKMEMORY - 129))
          | (1ULL << (sqlParser::TREE - 129))
          | (1ULL << (sqlParser::TRIM - 129))
          | (1ULL << (sqlParser::TRUNCATE - 129))
          | (1ULL << (sqlParser::SQL_TRUE - 129))
          | (1ULL << (sqlParser::TYPE - 129))
          | (1ULL << (sqlParser::TYPEOF - 129))
          | (1ULL << (sqlParser::VALUE - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::WEEK - 194))
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
          setState(1491);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        setState(1494);
        match(sqlParser::FROM);
        setState(1495);
        expr(0);
        break;
      }

      }
      setState(1498);
      match(sqlParser::T__9);
      break;
    }

    case 19: {
      _localctx = _tracker.createInstance<ExtractFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1500);
      match(sqlParser::EXTRACT);
      setState(1501);
      match(sqlParser::T__8);
      setState(1502);
      _la = _input->LA(1);
      if (!(((((_la - 60) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 60)) & ((1ULL << (sqlParser::DAY - 60))
        | (1ULL << (sqlParser::HOUR - 60))
        | (1ULL << (sqlParser::MINUTE - 60))
        | (1ULL << (sqlParser::MONTH - 60)))) != 0) || _la == sqlParser::SECOND

      || _la == sqlParser::YEAR)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1503);
      match(sqlParser::FROM);
      setState(1504);
      expr(0);
      setState(1505);
      match(sqlParser::T__9);
      break;
    }

    case 20: {
      _localctx = _tracker.createInstance<CountAllContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1507);
      match(sqlParser::COUNT);
      setState(1508);
      match(sqlParser::T__8);
      setState(1510);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ALL) {
        setState(1509);
        match(sqlParser::ALL);
      }
      setState(1512);
      match(sqlParser::T__7);
      setState(1513);
      match(sqlParser::T__9);
      break;
    }

    case 21: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1514);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::COUNT);
      setState(1515);
      match(sqlParser::T__8);
      setState(1518);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1516);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1517);
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
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXPLAIN:
        case sqlParser::EXTRACT:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
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
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSTBL:
        case sqlParser::MAX:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MIN:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::NODE:
        case sqlParser::NOT:
        case sqlParser::NOW:
        case sqlParser::SQL_NULL:
        case sqlParser::NULLIF:
        case sqlParser::OFFSET:
        case sqlParser::OVERLAY:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::REFERENCE:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
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
      setState(1520);
      expr(0);
      setState(1521);
      match(sqlParser::T__9);
      break;
    }

    case 22: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1523);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::SUM);
      setState(1524);
      match(sqlParser::T__8);
      setState(1527);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1525);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1526);
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
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXPLAIN:
        case sqlParser::EXTRACT:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
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
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSTBL:
        case sqlParser::MAX:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MIN:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::NODE:
        case sqlParser::NOT:
        case sqlParser::NOW:
        case sqlParser::SQL_NULL:
        case sqlParser::NULLIF:
        case sqlParser::OFFSET:
        case sqlParser::OVERLAY:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::REFERENCE:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
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
      setState(1529);
      expr(0);
      setState(1530);
      match(sqlParser::T__9);
      break;
    }

    case 23: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1532);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::AVG);
      setState(1533);
      match(sqlParser::T__8);
      setState(1536);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1534);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1535);
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
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXPLAIN:
        case sqlParser::EXTRACT:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
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
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSTBL:
        case sqlParser::MAX:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MIN:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::NODE:
        case sqlParser::NOT:
        case sqlParser::NOW:
        case sqlParser::SQL_NULL:
        case sqlParser::NULLIF:
        case sqlParser::OFFSET:
        case sqlParser::OVERLAY:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::REFERENCE:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
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
      setState(1538);
      expr(0);
      setState(1539);
      match(sqlParser::T__9);
      break;
    }

    case 24: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1541);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MIN);
      setState(1542);
      match(sqlParser::T__8);
      setState(1545);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1543);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1544);
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
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXPLAIN:
        case sqlParser::EXTRACT:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
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
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSTBL:
        case sqlParser::MAX:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MIN:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::NODE:
        case sqlParser::NOT:
        case sqlParser::NOW:
        case sqlParser::SQL_NULL:
        case sqlParser::NULLIF:
        case sqlParser::OFFSET:
        case sqlParser::OVERLAY:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::REFERENCE:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
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
      setState(1547);
      expr(0);
      setState(1548);
      match(sqlParser::T__9);
      break;
    }

    case 25: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1550);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MAX);
      setState(1551);
      match(sqlParser::T__8);
      setState(1554);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1552);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1553);
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
        case sqlParser::ESCAPE_CHAR:
        case sqlParser::EXISTS:
        case sqlParser::EXPLAIN:
        case sqlParser::EXTRACT:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FIELD_DELIMITER:
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
        case sqlParser::LOOP:
        case sqlParser::LSIDX:
        case sqlParser::LSTBL:
        case sqlParser::MAX:
        case sqlParser::MEMORYLEAKS:
        case sqlParser::MIN:
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::NODE:
        case sqlParser::NOT:
        case sqlParser::NOW:
        case sqlParser::SQL_NULL:
        case sqlParser::NULLIF:
        case sqlParser::OFFSET:
        case sqlParser::OVERLAY:
        case sqlParser::PATHS:
        case sqlParser::PLACING:
        case sqlParser::POSITION:
        case sqlParser::PRETTY:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::REFERENCE:
        case sqlParser::REPLICATED:
        case sqlParser::ROWSIZE:
        case sqlParser::SCHEMA:
        case sqlParser::SECONDS:
        case sqlParser::SKIPLIST:
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
      setState(1556);
      expr(0);
      setState(1557);
      match(sqlParser::T__9);
      break;
    }

    case 26: {
      _localctx = _tracker.createInstance<ScalarFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1559);
      dynamic_cast<ScalarFuncContext *>(_localctx)->func = functionName();
      setState(1560);
      match(sqlParser::T__8);
      setState(1569);
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
        | (1ULL << sqlParser::DAYS)
        | (1ULL << sqlParser::DECODE))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 65)) & ((1ULL << (sqlParser::DESC - 65))
        | (1ULL << (sqlParser::DETAIL - 65))
        | (1ULL << (sqlParser::DIGITS - 65))
        | (1ULL << (sqlParser::DLOAD - 65))
        | (1ULL << (sqlParser::ESCAPE_CHAR - 65))
        | (1ULL << (sqlParser::EXISTS - 65))
        | (1ULL << (sqlParser::EXPLAIN - 65))
        | (1ULL << (sqlParser::EXTRACT - 65))
        | (1ULL << (sqlParser::SQL_FALSE - 65))
        | (1ULL << (sqlParser::FEDERATION - 65))
        | (1ULL << (sqlParser::FIELD_DELIMITER - 65))
        | (1ULL << (sqlParser::FORMAT - 65))
        | (1ULL << (sqlParser::HASH - 65))
        | (1ULL << (sqlParser::HASHKEY - 65))
        | (1ULL << (sqlParser::HOURS - 65))
        | (1ULL << (sqlParser::IF - 65))
        | (1ULL << (sqlParser::IFNULL - 65))
        | (1ULL << (sqlParser::INDEX - 65))
        | (1ULL << (sqlParser::INTERVAL - 65))
        | (1ULL << (sqlParser::KEY - 65))
        | (1ULL << (sqlParser::LIMIT - 65))
        | (1ULL << (sqlParser::LOAD - 65))
        | (1ULL << (sqlParser::LOOP - 65))
        | (1ULL << (sqlParser::LSIDX - 65))
        | (1ULL << (sqlParser::LSTBL - 65))
        | (1ULL << (sqlParser::MAX - 65))
        | (1ULL << (sqlParser::MEMORYLEAKS - 65))
        | (1ULL << (sqlParser::MIN - 65))
        | (1ULL << (sqlParser::MINUTES - 65))
        | (1ULL << (sqlParser::MONOTONIC - 65))
        | (1ULL << (sqlParser::MONTHS - 65))
        | (1ULL << (sqlParser::NODE - 65))
        | (1ULL << (sqlParser::NOT - 65))
        | (1ULL << (sqlParser::NOW - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::SQL_NULL - 129))
        | (1ULL << (sqlParser::NULLIF - 129))
        | (1ULL << (sqlParser::OFFSET - 129))
        | (1ULL << (sqlParser::OVERLAY - 129))
        | (1ULL << (sqlParser::PATHS - 129))
        | (1ULL << (sqlParser::PLACING - 129))
        | (1ULL << (sqlParser::POSITION - 129))
        | (1ULL << (sqlParser::PRETTY - 129))
        | (1ULL << (sqlParser::QUARTER - 129))
        | (1ULL << (sqlParser::QUARTERS - 129))
        | (1ULL << (sqlParser::REFERENCE - 129))
        | (1ULL << (sqlParser::REPLICATED - 129))
        | (1ULL << (sqlParser::ROWSIZE - 129))
        | (1ULL << (sqlParser::SCHEMA - 129))
        | (1ULL << (sqlParser::SECONDS - 129))
        | (1ULL << (sqlParser::SKIPLIST - 129))
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
        | (1ULL << (sqlParser::TABLESIZE - 129))
        | (1ULL << (sqlParser::TEXT - 129))
        | (1ULL << (sqlParser::TIME - 129))
        | (1ULL << (sqlParser::TIMESTAMP - 129))
        | (1ULL << (sqlParser::TOP - 129))
        | (1ULL << (sqlParser::TRACKMEMORY - 129))
        | (1ULL << (sqlParser::TREE - 129))
        | (1ULL << (sqlParser::TRIM - 129))
        | (1ULL << (sqlParser::TRUNCATE - 129))
        | (1ULL << (sqlParser::SQL_TRUE - 129))
        | (1ULL << (sqlParser::TYPE - 129))
        | (1ULL << (sqlParser::TYPEOF - 129))
        | (1ULL << (sqlParser::VALUE - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::WEEK - 194))
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
        setState(1561);
        expr(0);
        setState(1566);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1562);
          match(sqlParser::T__5);
          setState(1563);
          expr(0);
          setState(1568);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1571);
      match(sqlParser::T__9);
      break;
    }

    case 27: {
      _localctx = _tracker.createInstance<ParentheticContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1573);
      match(sqlParser::T__8);
      setState(1574);
      expr(0);
      setState(1575);
      match(sqlParser::T__9);
      break;
    }

    case 28: {
      _localctx = _tracker.createInstance<SubqueryExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1577);
      match(sqlParser::T__8);
      setState(1578);
      query();
      setState(1579);
      match(sqlParser::T__9);
      break;
    }

    case 29: {
      _localctx = _tracker.createInstance<SubqueryExistsOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1582);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::NOT) {
        setState(1581);
        match(sqlParser::NOT);
      }
      setState(1584);
      match(sqlParser::EXISTS);

      setState(1585);
      match(sqlParser::T__8);
      setState(1586);
      query();
      setState(1587);
      match(sqlParser::T__9);
      break;
    }

    case 30: {
      _localctx = _tracker.createInstance<LogicOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1589);
      dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::NOT);
      setState(1590);
      expr(4);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(1686);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 208, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(1684);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 207, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1593);

          if (!(precpred(_ctx, 37))) throw FailedPredicateException(this, "precpred(_ctx, 37)");
          setState(1594);
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
          setState(1595);
          expr(38);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1596);

          if (!(precpred(_ctx, 36))) throw FailedPredicateException(this, "precpred(_ctx, 36)");
          setState(1597);
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
          setState(1598);
          expr(37);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1599);

          if (!(precpred(_ctx, 35))) throw FailedPredicateException(this, "precpred(_ctx, 35)");
          setState(1600);
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
          setState(1601);
          expr(36);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<LikeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1602);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(1604);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1603);
            match(sqlParser::NOT);
          }
          setState(1606);
          match(sqlParser::LIKE);
          setState(1607);
          expr(11);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<LikeEscapeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1608);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(1610);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1609);
            match(sqlParser::NOT);
          }
          setState(1612);
          match(sqlParser::LIKE);
          setState(1613);
          expr(0);
          setState(1614);
          match(sqlParser::ESCAPE);
          setState(1615);
          expr(10);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1617);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(1618);

          if (!(!(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent)))) throw FailedPredicateException(this, "!(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent))");
          setState(1619);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::AND);
          setState(1620);
          expr(4);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1621);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(1622);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::XOR);
          setState(1623);
          expr(3);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1624);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(1625);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::OR);
          setState(1626);
          expr(2);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<MethodCallContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1627);

          if (!(precpred(_ctx, 38))) throw FailedPredicateException(this, "precpred(_ctx, 38)");
          setState(1628);
          match(sqlParser::T__11);
          setState(1629);
          dynamic_cast<MethodCallContext *>(_localctx)->func = methodName();
          setState(1630);
          match(sqlParser::T__8);
          setState(1639);
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
            | (1ULL << sqlParser::DAYS)
            | (1ULL << sqlParser::DECODE))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 65)) & ((1ULL << (sqlParser::DESC - 65))
            | (1ULL << (sqlParser::DETAIL - 65))
            | (1ULL << (sqlParser::DIGITS - 65))
            | (1ULL << (sqlParser::DLOAD - 65))
            | (1ULL << (sqlParser::ESCAPE_CHAR - 65))
            | (1ULL << (sqlParser::EXISTS - 65))
            | (1ULL << (sqlParser::EXPLAIN - 65))
            | (1ULL << (sqlParser::EXTRACT - 65))
            | (1ULL << (sqlParser::SQL_FALSE - 65))
            | (1ULL << (sqlParser::FEDERATION - 65))
            | (1ULL << (sqlParser::FIELD_DELIMITER - 65))
            | (1ULL << (sqlParser::FORMAT - 65))
            | (1ULL << (sqlParser::HASH - 65))
            | (1ULL << (sqlParser::HASHKEY - 65))
            | (1ULL << (sqlParser::HOURS - 65))
            | (1ULL << (sqlParser::IF - 65))
            | (1ULL << (sqlParser::IFNULL - 65))
            | (1ULL << (sqlParser::INDEX - 65))
            | (1ULL << (sqlParser::INTERVAL - 65))
            | (1ULL << (sqlParser::KEY - 65))
            | (1ULL << (sqlParser::LIMIT - 65))
            | (1ULL << (sqlParser::LOAD - 65))
            | (1ULL << (sqlParser::LOOP - 65))
            | (1ULL << (sqlParser::LSIDX - 65))
            | (1ULL << (sqlParser::LSTBL - 65))
            | (1ULL << (sqlParser::MAX - 65))
            | (1ULL << (sqlParser::MEMORYLEAKS - 65))
            | (1ULL << (sqlParser::MIN - 65))
            | (1ULL << (sqlParser::MINUTES - 65))
            | (1ULL << (sqlParser::MONOTONIC - 65))
            | (1ULL << (sqlParser::MONTHS - 65))
            | (1ULL << (sqlParser::NODE - 65))
            | (1ULL << (sqlParser::NOT - 65))
            | (1ULL << (sqlParser::NOW - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::SQL_NULL - 129))
            | (1ULL << (sqlParser::NULLIF - 129))
            | (1ULL << (sqlParser::OFFSET - 129))
            | (1ULL << (sqlParser::OVERLAY - 129))
            | (1ULL << (sqlParser::PATHS - 129))
            | (1ULL << (sqlParser::PLACING - 129))
            | (1ULL << (sqlParser::POSITION - 129))
            | (1ULL << (sqlParser::PRETTY - 129))
            | (1ULL << (sqlParser::QUARTER - 129))
            | (1ULL << (sqlParser::QUARTERS - 129))
            | (1ULL << (sqlParser::REFERENCE - 129))
            | (1ULL << (sqlParser::REPLICATED - 129))
            | (1ULL << (sqlParser::ROWSIZE - 129))
            | (1ULL << (sqlParser::SCHEMA - 129))
            | (1ULL << (sqlParser::SECONDS - 129))
            | (1ULL << (sqlParser::SKIPLIST - 129))
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
            | (1ULL << (sqlParser::TABLESIZE - 129))
            | (1ULL << (sqlParser::TEXT - 129))
            | (1ULL << (sqlParser::TIME - 129))
            | (1ULL << (sqlParser::TIMESTAMP - 129))
            | (1ULL << (sqlParser::TOP - 129))
            | (1ULL << (sqlParser::TRACKMEMORY - 129))
            | (1ULL << (sqlParser::TREE - 129))
            | (1ULL << (sqlParser::TRIM - 129))
            | (1ULL << (sqlParser::TRUNCATE - 129))
            | (1ULL << (sqlParser::SQL_TRUE - 129))
            | (1ULL << (sqlParser::TYPE - 129))
            | (1ULL << (sqlParser::TYPEOF - 129))
            | (1ULL << (sqlParser::VALUE - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::WEEK - 194))
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
            setState(1631);
            expr(0);
            setState(1636);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__5) {
              setState(1632);
              match(sqlParser::T__5);
              setState(1633);
              expr(0);
              setState(1638);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1641);
          match(sqlParser::T__9);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<IsNullOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1643);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(1644);
          match(sqlParser::IS);
          setState(1646);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1645);
            match(sqlParser::NOT);
          }
          setState(1648);
          match(sqlParser::SQL_NULL);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<SubqueryInOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1649);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(1651);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1650);
            match(sqlParser::NOT);
          }
          setState(1653);
          match(sqlParser::IN);

          setState(1654);
          match(sqlParser::T__8);
          setState(1655);
          query();
          setState(1656);
          match(sqlParser::T__9);
          break;
        }

        case 12: {
          auto newContext = _tracker.createInstance<InOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1658);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(1660);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1659);
            match(sqlParser::NOT);
          }
          setState(1662);
          match(sqlParser::IN);

          setState(1663);
          match(sqlParser::T__8);
          setState(1672);
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
            | (1ULL << sqlParser::DAYS)
            | (1ULL << sqlParser::DECODE))) != 0) || ((((_la - 65) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 65)) & ((1ULL << (sqlParser::DESC - 65))
            | (1ULL << (sqlParser::DETAIL - 65))
            | (1ULL << (sqlParser::DIGITS - 65))
            | (1ULL << (sqlParser::DLOAD - 65))
            | (1ULL << (sqlParser::ESCAPE_CHAR - 65))
            | (1ULL << (sqlParser::EXISTS - 65))
            | (1ULL << (sqlParser::EXPLAIN - 65))
            | (1ULL << (sqlParser::EXTRACT - 65))
            | (1ULL << (sqlParser::SQL_FALSE - 65))
            | (1ULL << (sqlParser::FEDERATION - 65))
            | (1ULL << (sqlParser::FIELD_DELIMITER - 65))
            | (1ULL << (sqlParser::FORMAT - 65))
            | (1ULL << (sqlParser::HASH - 65))
            | (1ULL << (sqlParser::HASHKEY - 65))
            | (1ULL << (sqlParser::HOURS - 65))
            | (1ULL << (sqlParser::IF - 65))
            | (1ULL << (sqlParser::IFNULL - 65))
            | (1ULL << (sqlParser::INDEX - 65))
            | (1ULL << (sqlParser::INTERVAL - 65))
            | (1ULL << (sqlParser::KEY - 65))
            | (1ULL << (sqlParser::LIMIT - 65))
            | (1ULL << (sqlParser::LOAD - 65))
            | (1ULL << (sqlParser::LOOP - 65))
            | (1ULL << (sqlParser::LSIDX - 65))
            | (1ULL << (sqlParser::LSTBL - 65))
            | (1ULL << (sqlParser::MAX - 65))
            | (1ULL << (sqlParser::MEMORYLEAKS - 65))
            | (1ULL << (sqlParser::MIN - 65))
            | (1ULL << (sqlParser::MINUTES - 65))
            | (1ULL << (sqlParser::MONOTONIC - 65))
            | (1ULL << (sqlParser::MONTHS - 65))
            | (1ULL << (sqlParser::NODE - 65))
            | (1ULL << (sqlParser::NOT - 65))
            | (1ULL << (sqlParser::NOW - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::SQL_NULL - 129))
            | (1ULL << (sqlParser::NULLIF - 129))
            | (1ULL << (sqlParser::OFFSET - 129))
            | (1ULL << (sqlParser::OVERLAY - 129))
            | (1ULL << (sqlParser::PATHS - 129))
            | (1ULL << (sqlParser::PLACING - 129))
            | (1ULL << (sqlParser::POSITION - 129))
            | (1ULL << (sqlParser::PRETTY - 129))
            | (1ULL << (sqlParser::QUARTER - 129))
            | (1ULL << (sqlParser::QUARTERS - 129))
            | (1ULL << (sqlParser::REFERENCE - 129))
            | (1ULL << (sqlParser::REPLICATED - 129))
            | (1ULL << (sqlParser::ROWSIZE - 129))
            | (1ULL << (sqlParser::SCHEMA - 129))
            | (1ULL << (sqlParser::SECONDS - 129))
            | (1ULL << (sqlParser::SKIPLIST - 129))
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
            | (1ULL << (sqlParser::TABLESIZE - 129))
            | (1ULL << (sqlParser::TEXT - 129))
            | (1ULL << (sqlParser::TIME - 129))
            | (1ULL << (sqlParser::TIMESTAMP - 129))
            | (1ULL << (sqlParser::TOP - 129))
            | (1ULL << (sqlParser::TRACKMEMORY - 129))
            | (1ULL << (sqlParser::TREE - 129))
            | (1ULL << (sqlParser::TRIM - 129))
            | (1ULL << (sqlParser::TRUNCATE - 129))
            | (1ULL << (sqlParser::SQL_TRUE - 129))
            | (1ULL << (sqlParser::TYPE - 129))
            | (1ULL << (sqlParser::TYPEOF - 129))
            | (1ULL << (sqlParser::VALUE - 129)))) != 0) || ((((_la - 194) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 194)) & ((1ULL << (sqlParser::WEEK - 194))
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
            setState(1664);
            expr(0);
            setState(1669);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__5) {
              setState(1665);
              match(sqlParser::T__5);
              setState(1666);
              expr(0);
              setState(1671);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1674);
          match(sqlParser::T__9);
          break;
        }

        case 13: {
          auto newContext = _tracker.createInstance<BetweenOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1675);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(1677);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1676);
            match(sqlParser::NOT);
          }
          setState(1679);
          match(sqlParser::BETWEEN);

          setState(1680);
          expr(0);
          setState(1681);
          match(sqlParser::AND);
          setState(1682);
          expr(0);
          break;
        }

        } 
      }
      setState(1688);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 208, _ctx);
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
  enterRule(_localctx, 168, sqlParser::RuleVariableName);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1689);
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
  enterRule(_localctx, 170, sqlParser::RuleColumnRef);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1704);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 211, _ctx)) {
    case 1: {
      setState(1699);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 210, _ctx)) {
      case 1: {
        setState(1694);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 209, _ctx)) {
        case 1: {
          setState(1691);
          catalogName();
          setState(1692);
          match(sqlParser::T__11);
          break;
        }

        }

        setState(1696);
        schemaName();
        setState(1697);
        match(sqlParser::T__11);
        break;
      }

      }

      setState(1701);
      tableName();
      setState(1702);
      match(sqlParser::T__11);
      break;
    }

    }
    setState(1706);
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
  enterRule(_localctx, 172, sqlParser::RuleColumnWildcard);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1721);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 25) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 25)) & ((1ULL << (sqlParser::ASC - 25))
      | (1ULL << (sqlParser::BIT - 25))
      | (1ULL << (sqlParser::BITS - 25))
      | (1ULL << (sqlParser::BLOCK - 25))
      | (1ULL << (sqlParser::BLOOM - 25))
      | (1ULL << (sqlParser::BTREE - 25))
      | (1ULL << (sqlParser::BYTE - 25))
      | (1ULL << (sqlParser::BYTES - 25))
      | (1ULL << (sqlParser::CATALOG - 25))
      | (1ULL << (sqlParser::CHARS - 25))
      | (1ULL << (sqlParser::CONCAT - 25))
      | (1ULL << (sqlParser::DATA - 25))
      | (1ULL << (sqlParser::DATE_FORMAT - 25))
      | (1ULL << (sqlParser::DAYS - 25))
      | (1ULL << (sqlParser::DECODE - 25))
      | (1ULL << (sqlParser::DESC - 25))
      | (1ULL << (sqlParser::DETAIL - 25))
      | (1ULL << (sqlParser::DIGITS - 25))
      | (1ULL << (sqlParser::DLOAD - 25))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 25))
      | (1ULL << (sqlParser::EXPLAIN - 25))
      | (1ULL << (sqlParser::SQL_FALSE - 25))
      | (1ULL << (sqlParser::FEDERATION - 25))
      | (1ULL << (sqlParser::FIELD_DELIMITER - 25))
      | (1ULL << (sqlParser::FORMAT - 25))
      | (1ULL << (sqlParser::HASH - 25)))) != 0) || ((((_la - 89) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 89)) & ((1ULL << (sqlParser::HASHKEY - 89))
      | (1ULL << (sqlParser::HOURS - 89))
      | (1ULL << (sqlParser::IF - 89))
      | (1ULL << (sqlParser::IFNULL - 89))
      | (1ULL << (sqlParser::INDEX - 89))
      | (1ULL << (sqlParser::KEY - 89))
      | (1ULL << (sqlParser::LIMIT - 89))
      | (1ULL << (sqlParser::LOAD - 89))
      | (1ULL << (sqlParser::LOOP - 89))
      | (1ULL << (sqlParser::LSIDX - 89))
      | (1ULL << (sqlParser::LSTBL - 89))
      | (1ULL << (sqlParser::MEMORYLEAKS - 89))
      | (1ULL << (sqlParser::MINUTES - 89))
      | (1ULL << (sqlParser::MONOTONIC - 89))
      | (1ULL << (sqlParser::MONTHS - 89))
      | (1ULL << (sqlParser::NODE - 89))
      | (1ULL << (sqlParser::NOW - 89))
      | (1ULL << (sqlParser::OFFSET - 89))
      | (1ULL << (sqlParser::PATHS - 89))
      | (1ULL << (sqlParser::PLACING - 89))
      | (1ULL << (sqlParser::PRETTY - 89))
      | (1ULL << (sqlParser::QUARTER - 89))
      | (1ULL << (sqlParser::QUARTERS - 89))
      | (1ULL << (sqlParser::REFERENCE - 89))
      | (1ULL << (sqlParser::REPLICATED - 89))
      | (1ULL << (sqlParser::ROWSIZE - 89))
      | (1ULL << (sqlParser::SCHEMA - 89)))) != 0) || ((((_la - 154) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 154)) & ((1ULL << (sqlParser::SECONDS - 154))
      | (1ULL << (sqlParser::SKIPLIST - 154))
      | (1ULL << (sqlParser::SPACE - 154))
      | (1ULL << (sqlParser::STAGE - 154))
      | (1ULL << (sqlParser::STATS - 154))
      | (1ULL << (sqlParser::STATISTICS - 154))
      | (1ULL << (sqlParser::STATUS - 154))
      | (1ULL << (sqlParser::SUBSTR - 154))
      | (1ULL << (sqlParser::SZIDX - 154))
      | (1ULL << (sqlParser::SZTBL - 154))
      | (1ULL << (sqlParser::TABLESIZE - 154))
      | (1ULL << (sqlParser::TEXT - 154))
      | (1ULL << (sqlParser::TOP - 154))
      | (1ULL << (sqlParser::TRACKMEMORY - 154))
      | (1ULL << (sqlParser::TREE - 154))
      | (1ULL << (sqlParser::TRUNCATE - 154))
      | (1ULL << (sqlParser::TYPE - 154))
      | (1ULL << (sqlParser::TYPEOF - 154))
      | (1ULL << (sqlParser::VALUE - 154))
      | (1ULL << (sqlParser::WEEK - 154))
      | (1ULL << (sqlParser::WEEKS - 154))
      | (1ULL << (sqlParser::WITH_NAMES - 154))
      | (1ULL << (sqlParser::WITH_TYPES - 154))
      | (1ULL << (sqlParser::XOR - 154))
      | (1ULL << (sqlParser::YEARS - 154))
      | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 154))
      | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 154))
      | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 154)))) != 0)) {
      setState(1716);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 213, _ctx)) {
      case 1: {
        setState(1711);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 212, _ctx)) {
        case 1: {
          setState(1708);
          catalogName();
          setState(1709);
          match(sqlParser::T__11);
          break;
        }

        }

        setState(1713);
        schemaName();
        setState(1714);
        match(sqlParser::T__11);
        break;
      }

      }

      setState(1718);
      tableName();
      setState(1719);
      match(sqlParser::T__11);
    }
    setState(1723);
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
  enterRule(_localctx, 174, sqlParser::RuleClassName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1730);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 215, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1725);
        qualifier();
        setState(1726);
        match(sqlParser::T__11); 
      }
      setState(1732);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 215, _ctx);
    }
    setState(1733);
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
  enterRule(_localctx, 176, sqlParser::RuleFunctionName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1735);
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
  enterRule(_localctx, 178, sqlParser::RuleMethodName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1737);
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
  enterRule(_localctx, 180, sqlParser::RuleCatalogName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1739);
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
  enterRule(_localctx, 182, sqlParser::RuleSchemaName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1741);
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
  enterRule(_localctx, 184, sqlParser::RuleTableName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1743);
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
  enterRule(_localctx, 186, sqlParser::RuleTableAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1745);
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
  enterRule(_localctx, 188, sqlParser::RuleColumnName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1747);
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
  enterRule(_localctx, 190, sqlParser::RuleColumnAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1749);
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
  enterRule(_localctx, 192, sqlParser::RuleCollationName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1751);
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
  enterRule(_localctx, 194, sqlParser::RuleIndexName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1753);
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
  enterRule(_localctx, 196, sqlParser::RulePropertyName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1755);
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
  enterRule(_localctx, 198, sqlParser::RuleHintName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1757);
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
  enterRule(_localctx, 200, sqlParser::RuleExprHint);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1759);
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
  enterRule(_localctx, 202, sqlParser::RulePropertyValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1763);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 216, _ctx)) {
    case 1: {
      setState(1761);
      dynamic_cast<PropertyValueContext *>(_localctx)->lit = simpleLiteral();
      break;
    }

    case 2: {
      setState(1762);
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
  enterRule(_localctx, 204, sqlParser::RuleHintValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1768);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 217, _ctx)) {
    case 1: {
      setState(1765);
      dynamic_cast<HintValueContext *>(_localctx)->lit = simpleLiteral();
      break;
    }

    case 2: {
      setState(1766);
      dynamic_cast<HintValueContext *>(_localctx)->id = identifier();
      break;
    }

    case 3: {
      setState(1767);
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
  enterRule(_localctx, 206, sqlParser::RuleIdentifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1774);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::PLAIN_IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(1770);
        match(sqlParser::PLAIN_IDENTIFIER);
        break;
      }

      case sqlParser::BAKTIKD_IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(1771);
        match(sqlParser::BAKTIKD_IDENTIFIER);
        break;
      }

      case sqlParser::DUBLQUOTED_IDENTIFIER: {
        enterOuterAlt(_localctx, 3);
        setState(1772);
        match(sqlParser::DUBLQUOTED_IDENTIFIER);
        break;
      }

      case sqlParser::ASC:
      case sqlParser::BIT:
      case sqlParser::BITS:
      case sqlParser::BLOCK:
      case sqlParser::BLOOM:
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
      case sqlParser::ESCAPE_CHAR:
      case sqlParser::EXPLAIN:
      case sqlParser::SQL_FALSE:
      case sqlParser::FEDERATION:
      case sqlParser::FIELD_DELIMITER:
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
      case sqlParser::LOOP:
      case sqlParser::LSIDX:
      case sqlParser::LSTBL:
      case sqlParser::MEMORYLEAKS:
      case sqlParser::MINUTES:
      case sqlParser::MONOTONIC:
      case sqlParser::MONTHS:
      case sqlParser::NODE:
      case sqlParser::NOW:
      case sqlParser::OFFSET:
      case sqlParser::PATHS:
      case sqlParser::PLACING:
      case sqlParser::PRETTY:
      case sqlParser::QUARTER:
      case sqlParser::QUARTERS:
      case sqlParser::REFERENCE:
      case sqlParser::REPLICATED:
      case sqlParser::ROWSIZE:
      case sqlParser::SCHEMA:
      case sqlParser::SECONDS:
      case sqlParser::SKIPLIST:
      case sqlParser::SPACE:
      case sqlParser::STAGE:
      case sqlParser::STATS:
      case sqlParser::STATISTICS:
      case sqlParser::STATUS:
      case sqlParser::SUBSTR:
      case sqlParser::SZIDX:
      case sqlParser::SZTBL:
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
        setState(1773);
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
  enterRule(_localctx, 208, sqlParser::RuleQualifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1776);
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
  enterRule(_localctx, 210, sqlParser::RulePrecision);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1778);
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
  enterRule(_localctx, 212, sqlParser::RuleScale);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1780);
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
  enterRule(_localctx, 214, sqlParser::RuleSize);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1782);
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
  enterRule(_localctx, 216, sqlParser::RuleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1794);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 219, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1784);
      match(sqlParser::SQL_NULL);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1785);
      booleanLiteral();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1786);
      signedIntegerLiteral();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(1787);
      signedDecimalLiteral();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1788);
      signedFloatLiteral();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1789);
      dateLiteral();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(1790);
      timeLiteral();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(1791);
      stringLiteral();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(1792);
      timestampLiteral();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(1793);
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
  enterRule(_localctx, 218, sqlParser::RuleSimpleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1802);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 220, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1796);
      match(sqlParser::SQL_NULL);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1797);
      booleanLiteral();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1798);
      signedIntegerLiteral();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(1799);
      signedDecimalLiteral();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1800);
      signedFloatLiteral();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1801);
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
  enterRule(_localctx, 220, sqlParser::RuleBooleanLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1804);
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
  enterRule(_localctx, 222, sqlParser::RuleIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1806);
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
  enterRule(_localctx, 224, sqlParser::RuleSignedIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1810);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1808);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1809);
        dynamic_cast<SignedIntegerLiteralContext *>(_localctx)->neg = match(sqlParser::T__6);
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1812);
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
  enterRule(_localctx, 226, sqlParser::RuleDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1814);
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
  enterRule(_localctx, 228, sqlParser::RuleSignedDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1818);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1816);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1817);
        dynamic_cast<SignedDecimalLiteralContext *>(_localctx)->neg = match(sqlParser::T__6);
        break;
      }

      case sqlParser::DECIMAL_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1820);
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
  enterRule(_localctx, 230, sqlParser::RuleFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1822);
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
  enterRule(_localctx, 232, sqlParser::RuleSignedFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1826);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1824);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1825);
        dynamic_cast<SignedFloatLiteralContext *>(_localctx)->neg = match(sqlParser::T__6);
        break;
      }

      case sqlParser::FLOAT_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1828);
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
  enterRule(_localctx, 234, sqlParser::RuleDateLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1830);
    match(sqlParser::DATE);
    setState(1831);
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
  enterRule(_localctx, 236, sqlParser::RuleTimeLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1833);
    match(sqlParser::TIME);
    setState(1834);
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
  enterRule(_localctx, 238, sqlParser::RuleTimestampLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1836);
    match(sqlParser::TIMESTAMP);
    setState(1837);
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
  enterRule(_localctx, 240, sqlParser::RuleStringLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1839);
    _la = _input->LA(1);
    if (!(((((_la - 206) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 206)) & ((1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 206))
      | (1ULL << (sqlParser::SINGLQUOTED_STRING - 206))
      | (1ULL << (sqlParser::DUBLQUOTED_STRING - 206)))) != 0))) {
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
  enterRule(_localctx, 242, sqlParser::RuleIntervalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1843);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 224, _ctx)) {
    case 1: {
      setState(1841);
      yearMonthInterval();
      break;
    }

    case 2: {
      setState(1842);
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
  enterRule(_localctx, 244, sqlParser::RuleYearMonthInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1845);
    match(sqlParser::INTERVAL);
    setState(1848);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1846);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1847);
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
    setState(1850);
    stringLiteral();
    setState(1874);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 229, _ctx)) {
    case 1: {
      setState(1851);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1856);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 226, _ctx)) {
      case 1: {
        setState(1852);
        match(sqlParser::T__8);
        setState(1853);
        precision();
        setState(1854);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1858);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1863);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1859);
        match(sqlParser::T__8);
        setState(1860);
        precision();
        setState(1861);
        match(sqlParser::T__9);
      }
      setState(1865);
      match(sqlParser::TO);
      setState(1866);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      break;
    }

    case 3: {
      setState(1867);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      setState(1872);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 228, _ctx)) {
      case 1: {
        setState(1868);
        match(sqlParser::T__8);
        setState(1869);
        precision();
        setState(1870);
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
  enterRule(_localctx, 246, sqlParser::RuleDayTimeInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1876);
    match(sqlParser::INTERVAL);
    setState(1879);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1877);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1878);
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
    setState(1881);
    stringLiteral();
    setState(1966);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 244, _ctx)) {
    case 1: {
      setState(1882);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1887);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 231, _ctx)) {
      case 1: {
        setState(1883);
        match(sqlParser::T__8);
        setState(1884);
        precision();
        setState(1885);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1889);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1894);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1890);
        match(sqlParser::T__8);
        setState(1891);
        precision();
        setState(1892);
        match(sqlParser::T__9);
      }
      setState(1896);
      match(sqlParser::TO);
      setState(1906);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::HOUR: {
          setState(1897);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
          break;
        }

        case sqlParser::MINUTE: {
          setState(1898);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1899);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1904);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 233, _ctx)) {
          case 1: {
            setState(1900);
            match(sqlParser::T__8);
            setState(1901);
            scale();
            setState(1902);
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
      setState(1908);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1913);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1909);
        match(sqlParser::T__8);
        setState(1910);
        precision();
        setState(1911);
        match(sqlParser::T__9);
      }
      setState(1915);
      match(sqlParser::TO);
      setState(1924);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::MINUTE: {
          setState(1916);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1917);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1922);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 236, _ctx)) {
          case 1: {
            setState(1918);
            match(sqlParser::T__8);
            setState(1919);
            scale();
            setState(1920);
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
      setState(1926);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1931);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1927);
        match(sqlParser::T__8);
        setState(1928);
        precision();
        setState(1929);
        match(sqlParser::T__9);
      }
      setState(1933);
      match(sqlParser::TO);

      setState(1934);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1939);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 239, _ctx)) {
      case 1: {
        setState(1935);
        match(sqlParser::T__8);
        setState(1936);
        scale();
        setState(1937);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 5: {
      setState(1941);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1946);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 240, _ctx)) {
      case 1: {
        setState(1942);
        match(sqlParser::T__8);
        setState(1943);
        precision();
        setState(1944);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 6: {
      setState(1948);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1953);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 241, _ctx)) {
      case 1: {
        setState(1949);
        match(sqlParser::T__8);
        setState(1950);
        precision();
        setState(1951);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 7: {
      setState(1955);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1964);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 243, _ctx)) {
      case 1: {
        setState(1956);
        match(sqlParser::T__8);
        setState(1957);
        precision();
        setState(1960);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1958);
          match(sqlParser::T__5);
          setState(1959);
          scale();
        }
        setState(1962);
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
  enterRule(_localctx, 248, sqlParser::RuleLscol);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1968);
    match(sqlParser::LSCOL);
    setState(1969);
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
  enterRule(_localctx, 250, sqlParser::RuleLsidx);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1971);
    match(sqlParser::LSIDX);
    setState(1972);
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
  enterRule(_localctx, 252, sqlParser::RuleLsidxcol);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1974);
    match(sqlParser::LSIDXCOL);
    setState(1975);
    tableName();
    setState(1976);
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
  enterRule(_localctx, 254, sqlParser::RuleLstbl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1978);
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

tree::TerminalNode* sqlParser::NonReservedWordContext::MINUTES() {
  return getToken(sqlParser::MINUTES, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MONOTONIC() {
  return getToken(sqlParser::MONOTONIC, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MONTHS() {
  return getToken(sqlParser::MONTHS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::NODE() {
  return getToken(sqlParser::NODE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::NOW() {
  return getToken(sqlParser::NOW, 0);
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

tree::TerminalNode* sqlParser::NonReservedWordContext::REPLICATED() {
  return getToken(sqlParser::REPLICATED, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::ROWSIZE() {
  return getToken(sqlParser::ROWSIZE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SCHEMA() {
  return getToken(sqlParser::SCHEMA, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SECONDS() {
  return getToken(sqlParser::SECONDS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SKIPLIST() {
  return getToken(sqlParser::SKIPLIST, 0);
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
  enterRule(_localctx, 256, sqlParser::RuleNonReservedWord);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1980);
    _la = _input->LA(1);
    if (!(((((_la - 25) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 25)) & ((1ULL << (sqlParser::ASC - 25))
      | (1ULL << (sqlParser::BIT - 25))
      | (1ULL << (sqlParser::BITS - 25))
      | (1ULL << (sqlParser::BLOCK - 25))
      | (1ULL << (sqlParser::BLOOM - 25))
      | (1ULL << (sqlParser::BTREE - 25))
      | (1ULL << (sqlParser::BYTE - 25))
      | (1ULL << (sqlParser::BYTES - 25))
      | (1ULL << (sqlParser::CATALOG - 25))
      | (1ULL << (sqlParser::CHARS - 25))
      | (1ULL << (sqlParser::CONCAT - 25))
      | (1ULL << (sqlParser::DATA - 25))
      | (1ULL << (sqlParser::DATE_FORMAT - 25))
      | (1ULL << (sqlParser::DAYS - 25))
      | (1ULL << (sqlParser::DECODE - 25))
      | (1ULL << (sqlParser::DESC - 25))
      | (1ULL << (sqlParser::DETAIL - 25))
      | (1ULL << (sqlParser::DIGITS - 25))
      | (1ULL << (sqlParser::DLOAD - 25))
      | (1ULL << (sqlParser::ESCAPE_CHAR - 25))
      | (1ULL << (sqlParser::EXPLAIN - 25))
      | (1ULL << (sqlParser::SQL_FALSE - 25))
      | (1ULL << (sqlParser::FEDERATION - 25))
      | (1ULL << (sqlParser::FIELD_DELIMITER - 25))
      | (1ULL << (sqlParser::FORMAT - 25))
      | (1ULL << (sqlParser::HASH - 25)))) != 0) || ((((_la - 89) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 89)) & ((1ULL << (sqlParser::HASHKEY - 89))
      | (1ULL << (sqlParser::HOURS - 89))
      | (1ULL << (sqlParser::IF - 89))
      | (1ULL << (sqlParser::IFNULL - 89))
      | (1ULL << (sqlParser::INDEX - 89))
      | (1ULL << (sqlParser::KEY - 89))
      | (1ULL << (sqlParser::LIMIT - 89))
      | (1ULL << (sqlParser::LOAD - 89))
      | (1ULL << (sqlParser::LOOP - 89))
      | (1ULL << (sqlParser::LSIDX - 89))
      | (1ULL << (sqlParser::LSTBL - 89))
      | (1ULL << (sqlParser::MEMORYLEAKS - 89))
      | (1ULL << (sqlParser::MINUTES - 89))
      | (1ULL << (sqlParser::MONOTONIC - 89))
      | (1ULL << (sqlParser::MONTHS - 89))
      | (1ULL << (sqlParser::NODE - 89))
      | (1ULL << (sqlParser::NOW - 89))
      | (1ULL << (sqlParser::OFFSET - 89))
      | (1ULL << (sqlParser::PATHS - 89))
      | (1ULL << (sqlParser::PLACING - 89))
      | (1ULL << (sqlParser::PRETTY - 89))
      | (1ULL << (sqlParser::QUARTER - 89))
      | (1ULL << (sqlParser::QUARTERS - 89))
      | (1ULL << (sqlParser::REFERENCE - 89))
      | (1ULL << (sqlParser::REPLICATED - 89))
      | (1ULL << (sqlParser::ROWSIZE - 89))
      | (1ULL << (sqlParser::SCHEMA - 89)))) != 0) || ((((_la - 154) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 154)) & ((1ULL << (sqlParser::SECONDS - 154))
      | (1ULL << (sqlParser::SKIPLIST - 154))
      | (1ULL << (sqlParser::SPACE - 154))
      | (1ULL << (sqlParser::STAGE - 154))
      | (1ULL << (sqlParser::STATS - 154))
      | (1ULL << (sqlParser::STATISTICS - 154))
      | (1ULL << (sqlParser::STATUS - 154))
      | (1ULL << (sqlParser::SUBSTR - 154))
      | (1ULL << (sqlParser::SZIDX - 154))
      | (1ULL << (sqlParser::SZTBL - 154))
      | (1ULL << (sqlParser::TABLESIZE - 154))
      | (1ULL << (sqlParser::TEXT - 154))
      | (1ULL << (sqlParser::TOP - 154))
      | (1ULL << (sqlParser::TRACKMEMORY - 154))
      | (1ULL << (sqlParser::TREE - 154))
      | (1ULL << (sqlParser::TRUNCATE - 154))
      | (1ULL << (sqlParser::TYPE - 154))
      | (1ULL << (sqlParser::TYPEOF - 154))
      | (1ULL << (sqlParser::VALUE - 154))
      | (1ULL << (sqlParser::WEEK - 154))
      | (1ULL << (sqlParser::WEEKS - 154))
      | (1ULL << (sqlParser::WITH_NAMES - 154))
      | (1ULL << (sqlParser::WITH_TYPES - 154))
      | (1ULL << (sqlParser::XOR - 154))
      | (1ULL << (sqlParser::YEARS - 154)))) != 0))) {
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
    case 83: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

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
  "count", "space", "status", "szidx", "sztbl", "trackmemory", "memoryleaks", 
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
  "lscol", "lsidx", "lsidxcol", "lstbl", "nonReservedWord"
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
  "", "", "", "", "", "", "", "", "", "", "", "'/*+'", "'*/'"
};

std::vector<std::string> sqlParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "ALL", "AND", "ANTI", "ANY", "AS", "ASC", "AVG", "BETWEEN", "BINARY", 
  "BIT", "BITS", "BLOB", "BLOCK", "BLOOM", "BOTH", "BOOLEAN", "BTREE", "BY", 
  "BYTE", "BYTES", "CASE", "CAST", "CATALOG", "CHAR", "CHARS", "CHARACTER", 
  "COALESCE", "COLLATE", "CONCAT", "CORRESPONDING", "COUNT", "CREATE", "CROSS", 
  "CURRENT_CATALOG", "CURRENT_SCHEMA", "CURRENT_TIMESTAMP", "CURRENT_USER", 
  "DATA", "DATE", "DATE_FORMAT", "DAY", "DAYS", "DECIMAL", "DECODE", "DELETE", 
  "DESC", "DETAIL", "DIGITS", "DISTINCT", "DLOAD", "DROP", "ELSE", "END", 
  "ESCAPE", "ESCAPE_CHAR", "EXCEPT", "EXISTS", "EXPLAIN", "EXTRACT", "SQL_FALSE", 
  "FEDERATION", "FIELD_DELIMITER", "FLOAT", "FOR", "FORMAT", "FULL", "FROM", 
  "GROUP", "HASH", "HASHKEY", "HAVING", "HOUR", "HOURS", "IF", "IFNULL", 
  "IN", "INDEX", "INNER", "INSERT", "INTEGER", "INTERSECT", "INTERVAL", 
  "INTO", "IS", "JOIN", "KEY", "LEADING", "LEFT", "LIKE", "LIMIT", "LOAD", 
  "LOOP", "LSCOL", "LSIDX", "LSIDXCOL", "LSTBL", "MAX", "MEMORYLEAKS", "MIN", 
  "MINUTE", "MINUTES", "MONOTONIC", "MONTH", "MONTHS", "NATURAL", "NO", 
  "NODE", "NOT", "NOW", "SQL_NULL", "NULLIF", "OFFSET", "ON", "OR", "ORDER", 
  "OUTER", "OVER", "OVERLAY", "PARTITION", "PATHS", "PLACING", "POSITION", 
  "PRETTY", "PRIMARY", "QUARTER", "QUARTERS", "RECURSIVE", "REFERENCE", 
  "REPLICATED", "RIGHT", "ROW", "ROWSIZE", "SCHEMA", "SECOND", "SECONDS", 
  "SELECT", "SEMI", "SET", "SKIPLIST", "SPACE", "STAGE", "STATS", "STATISTICS", 
  "STATUS", "SUBSTR", "SUBSTRING", "SUM", "SZIDX", "SZTBL", "TABLE", "TABLESIZE", 
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
    0x3, 0xdb, 0x7c1, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
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
    0x80, 0x4, 0x81, 0x9, 0x81, 0x4, 0x82, 0x9, 0x82, 0x3, 0x2, 0x6, 0x2, 
    0x106, 0xa, 0x2, 0xd, 0x2, 0xe, 0x2, 0x107, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x120, 
    0xa, 0x4, 0x3, 0x5, 0x3, 0x5, 0x7, 0x5, 0x124, 0xa, 0x5, 0xc, 0x5, 0xe, 
    0x5, 0x127, 0xb, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 
    0x3, 0x6, 0x5, 0x6, 0x12f, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x7, 0x3, 0x7, 0x5, 0x7, 0x136, 0xa, 0x7, 0x3, 0x8, 0x7, 0x8, 0x139, 
    0xa, 0x8, 0xc, 0x8, 0xe, 0x8, 0x13c, 0xb, 0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 
    0x8, 0x140, 0xa, 0x8, 0xc, 0x8, 0xe, 0x8, 0x143, 0xb, 0x8, 0x3, 0x8, 
    0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 0x14b, 0xa, 
    0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x7, 0xa, 0x151, 0xa, 0xa, 
    0xc, 0xa, 0xe, 0xa, 0x154, 0xb, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 
    0xb, 0x159, 0xa, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0x15e, 
    0xa, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x7, 0xe, 0x164, 0xa, 
    0xe, 0xc, 0xe, 0xe, 0xe, 0x167, 0xb, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x7, 0x10, 0x16f, 0xa, 0x10, 0xc, 0x10, 
    0xe, 0x10, 0x172, 0xb, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x176, 
    0xa, 0x10, 0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0x17a, 0xa, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x7, 0x11, 0x17e, 0xa, 0x11, 0xc, 0x11, 0xe, 0x11, 0x181, 
    0xb, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 
    0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x18c, 0xa, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x7, 0x12, 0x192, 0xa, 0x12, 
    0xc, 0x12, 0xe, 0x12, 0x195, 0xb, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x19f, 
    0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x7, 0x12, 0x1a5, 
    0xa, 0x12, 0xc, 0x12, 0xe, 0x12, 0x1a8, 0xb, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x5, 0x12, 0x1ac, 0xa, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x7, 0x13, 0x1b2, 0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0x1b5, 0xb, 0x13, 
    0x5, 0x13, 0x1b7, 0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x5, 0x13, 0x1bd, 0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x5, 0x13, 0x1c3, 0xa, 0x13, 0x5, 0x13, 0x1c5, 0xa, 0x13, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1ca, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x5, 0x14, 0x1cf, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 
    0x1d3, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1d8, 
    0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1dc, 0xa, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1e2, 0xa, 0x14, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x7, 0x15, 0x1e8, 0xa, 0x15, 0xc, 0x15, 
    0xe, 0x15, 0x1eb, 0xb, 0x15, 0x5, 0x15, 0x1ed, 0xa, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1f3, 0xa, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1f9, 0xa, 0x15, 0x5, 0x15, 
    0x1fb, 0xa, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 
    0x3, 0x16, 0x5, 0x16, 0x203, 0xa, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 
    0x3, 0x16, 0x5, 0x16, 0x209, 0xa, 0x16, 0x3, 0x16, 0x5, 0x16, 0x20c, 
    0xa, 0x16, 0x3, 0x16, 0x7, 0x16, 0x20f, 0xa, 0x16, 0xc, 0x16, 0xe, 0x16, 
    0x212, 0xb, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 
    0x3, 0x17, 0x5, 0x17, 0x21a, 0xa, 0x17, 0x3, 0x18, 0x5, 0x18, 0x21d, 
    0xa, 0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0x221, 0xa, 0x18, 0x3, 0x18, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 
    0x19, 0x5, 0x19, 0x22b, 0xa, 0x19, 0x3, 0x19, 0x5, 0x19, 0x22e, 0xa, 
    0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x234, 
    0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x238, 0xa, 0x19, 0x5, 0x19, 
    0x23a, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 
    0x240, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x244, 0xa, 0x19, 
    0x5, 0x19, 0x246, 0xa, 0x19, 0x5, 0x19, 0x248, 0xa, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x7, 0x19, 0x24c, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 0x24f, 
    0xb, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x254, 0xa, 0x19, 
    0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 0x19, 0x259, 0xa, 0x19, 0xc, 0x19, 
    0xe, 0x19, 0x25c, 0xb, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x260, 
    0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 0x19, 0x266, 
    0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 0x269, 0xb, 0x19, 0x3, 0x19, 0x5, 0x19, 
    0x26c, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x3, 0x19, 0x7, 0x19, 0x275, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 
    0x278, 0xb, 0x19, 0x3, 0x19, 0x5, 0x19, 0x27b, 0xa, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 
    0x19, 0x284, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 0x287, 0xb, 0x19, 0x3, 
    0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 0x28c, 0xa, 0x1a, 0x3, 0x1a, 
    0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 0x291, 0xa, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 
    0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x298, 0xa, 0x1b, 0x5, 0x1b, 
    0x29a, 0xa, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x29f, 
    0xa, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 
    0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x7, 0x1d, 0x2aa, 0xa, 0x1d, 
    0xc, 0x1d, 0xe, 0x1d, 0x2ad, 0xb, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x7, 0x1e, 0x2b3, 0xa, 0x1e, 0xc, 0x1e, 0xe, 0x1e, 0x2b6, 
    0xb, 0x1e, 0x5, 0x1e, 0x2b8, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 
    0x2bc, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x7, 0x1e, 0x2c3, 0xa, 0x1e, 0xc, 0x1e, 0xe, 0x1e, 0x2c6, 0xb, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x2ca, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x5, 0x1e, 0x2cf, 0xa, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
    0x3, 0x1f, 0x7, 0x1f, 0x2d5, 0xa, 0x1f, 0xc, 0x1f, 0xe, 0x1f, 0x2d8, 
    0xb, 0x1f, 0x5, 0x1f, 0x2da, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 
    0x2de, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
    0x5, 0x1f, 0x2e5, 0xa, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x7, 0x20, 0x2eb, 0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 0x2ee, 0xb, 0x20, 
    0x5, 0x20, 0x2f0, 0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 
    0x20, 0x7, 0x20, 0x2fd, 0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 0x300, 0xb, 
    0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x5, 0x20, 0x306, 
    0xa, 0x20, 0x3, 0x21, 0x3, 0x21, 0x5, 0x21, 0x30a, 0xa, 0x21, 0x3, 0x22, 
    0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x23, 0x3, 0x23, 0x5, 
    0x23, 0x313, 0xa, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 
    0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x7, 0x23, 0x31d, 0xa, 0x23, 
    0xc, 0x23, 0xe, 0x23, 0x320, 0xb, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x24, 
    0x3, 0x24, 0x5, 0x24, 0x326, 0xa, 0x24, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 
    0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x5, 0x26, 0x335, 0xa, 0x26, 
    0x3, 0x27, 0x3, 0x27, 0x5, 0x27, 0x339, 0xa, 0x27, 0x3, 0x27, 0x3, 0x27, 
    0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 0x342, 
    0xa, 0x28, 0x3, 0x28, 0x7, 0x28, 0x345, 0xa, 0x28, 0xc, 0x28, 0xe, 0x28, 
    0x348, 0xb, 0x28, 0x5, 0x28, 0x34a, 0xa, 0x28, 0x3, 0x29, 0x3, 0x29, 
    0x3, 0x29, 0x3, 0x29, 0x6, 0x29, 0x350, 0xa, 0x29, 0xd, 0x29, 0xe, 0x29, 
    0x351, 0x3, 0x29, 0x3, 0x29, 0x5, 0x29, 0x356, 0xa, 0x29, 0x3, 0x29, 
    0x7, 0x29, 0x359, 0xa, 0x29, 0xc, 0x29, 0xe, 0x29, 0x35c, 0xb, 0x29, 
    0x5, 0x29, 0x35e, 0xa, 0x29, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 
    0x3, 0x2a, 0x5, 0x2a, 0x365, 0xa, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 
    0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2d, 0x3, 
    0x2d, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 
    0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x32, 0x3, 
    0x32, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x5, 0x33, 0x381, 0xa, 0x33, 
    0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x5, 0x33, 0x386, 0xa, 0x33, 0x7, 0x33, 
    0x388, 0xa, 0x33, 0xc, 0x33, 0xe, 0x33, 0x38b, 0xb, 0x33, 0x3, 0x33, 
    0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x7, 
    0x33, 0x394, 0xa, 0x33, 0xc, 0x33, 0xe, 0x33, 0x397, 0xb, 0x33, 0x3, 
    0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x7, 0x34, 
    0x39f, 0xa, 0x34, 0xc, 0x34, 0xe, 0x34, 0x3a2, 0xb, 0x34, 0x3, 0x34, 
    0x3, 0x34, 0x3, 0x35, 0x3, 0x35, 0x3, 0x35, 0x3, 0x35, 0x7, 0x35, 0x3aa, 
    0xa, 0x35, 0xc, 0x35, 0xe, 0x35, 0x3ad, 0xb, 0x35, 0x3, 0x36, 0x3, 0x36, 
    0x3, 0x36, 0x3, 0x36, 0x7, 0x36, 0x3b3, 0xa, 0x36, 0xc, 0x36, 0xe, 0x36, 
    0x3b6, 0xb, 0x36, 0x3, 0x36, 0x3, 0x36, 0x3, 0x37, 0x5, 0x37, 0x3bb, 
    0xa, 0x37, 0x3, 0x37, 0x3, 0x37, 0x5, 0x37, 0x3bf, 0xa, 0x37, 0x3, 0x37, 
    0x5, 0x37, 0x3c2, 0xa, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 
    0x3, 0x37, 0x7, 0x37, 0x3c9, 0xa, 0x37, 0xc, 0x37, 0xe, 0x37, 0x3cc, 
    0xb, 0x37, 0x3, 0x37, 0x3, 0x37, 0x5, 0x37, 0x3d0, 0xa, 0x37, 0x3, 0x37, 
    0x3, 0x37, 0x5, 0x37, 0x3d4, 0xa, 0x37, 0x3, 0x37, 0x3, 0x37, 0x5, 0x37, 
    0x3d8, 0xa, 0x37, 0x3, 0x38, 0x3, 0x38, 0x3, 0x39, 0x3, 0x39, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x3e3, 0xa, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x3e7, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x3ed, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x5, 0x3a, 0x3f2, 0xa, 0x3a, 0x5, 0x3a, 0x3f4, 0xa, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x3f8, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x3fe, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x5, 0x3a, 0x402, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 
    0x3a, 0x40e, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x416, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x5, 0x3a, 0x41a, 0xa, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x41d, 0xa, 0x3a, 
    0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x5, 0x3b, 0x422, 0xa, 0x3b, 0x3, 0x3c, 
    0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x5, 0x3c, 0x429, 0xa, 0x3c, 
    0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x5, 0x3c, 0x430, 
    0xa, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x3, 
    0x3c, 0x3, 0x3c, 0x5, 0x3c, 0x439, 0xa, 0x3c, 0x5, 0x3c, 0x43b, 0xa, 
    0x3c, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 
    0x442, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x5, 0x3d, 0x449, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x453, 0xa, 0x3d, 
    0x5, 0x3d, 0x455, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x5, 0x3d, 0x45c, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x465, 0xa, 0x3d, 
    0x5, 0x3d, 0x467, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x5, 0x3d, 0x46e, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x476, 0xa, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x47d, 0xa, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x484, 
    0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 
    0x3d, 0x48b, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x48f, 0xa, 
    0x3d, 0x5, 0x3d, 0x491, 0xa, 0x3d, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x5, 0x3e, 0x497, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x5, 0x3e, 0x49c, 0xa, 0x3e, 0x3, 0x3f, 0x3, 0x3f, 0x5, 0x3f, 0x4a0, 
    0xa, 0x3f, 0x3, 0x3f, 0x5, 0x3f, 0x4a3, 0xa, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 
    0x3, 0x3f, 0x5, 0x3f, 0x4a8, 0xa, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x5, 0x3f, 
    0x4ac, 0xa, 0x3f, 0x3, 0x3f, 0x5, 0x3f, 0x4af, 0xa, 0x3f, 0x3, 0x3f, 
    0x3, 0x3f, 0x5, 0x3f, 0x4b3, 0xa, 0x3f, 0x3, 0x40, 0x3, 0x40, 0x3, 0x41, 
    0x3, 0x41, 0x7, 0x41, 0x4b9, 0xa, 0x41, 0xc, 0x41, 0xe, 0x41, 0x4bc, 
    0xb, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x7, 0x41, 0x4c2, 
    0xa, 0x41, 0xc, 0x41, 0xe, 0x41, 0x4c5, 0xb, 0x41, 0x3, 0x41, 0x3, 0x41, 
    0x3, 0x41, 0x5, 0x41, 0x4ca, 0xa, 0x41, 0x5, 0x41, 0x4cc, 0xa, 0x41, 
    0x3, 0x42, 0x3, 0x42, 0x3, 0x42, 0x7, 0x42, 0x4d1, 0xa, 0x42, 0xc, 0x42, 
    0xe, 0x42, 0x4d4, 0xb, 0x42, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x4d8, 
    0xa, 0x43, 0x3, 0x43, 0x5, 0x43, 0x4db, 0xa, 0x43, 0x3, 0x43, 0x5, 0x43, 
    0x4de, 0xa, 0x43, 0x3, 0x44, 0x3, 0x44, 0x3, 0x44, 0x7, 0x44, 0x4e3, 
    0xa, 0x44, 0xc, 0x44, 0xe, 0x44, 0x4e6, 0xb, 0x44, 0x3, 0x45, 0x3, 0x45, 
    0x3, 0x46, 0x3, 0x46, 0x3, 0x46, 0x7, 0x46, 0x4ed, 0xa, 0x46, 0xc, 0x46, 
    0xe, 0x46, 0x4f0, 0xb, 0x46, 0x3, 0x47, 0x3, 0x47, 0x5, 0x47, 0x4f4, 
    0xa, 0x47, 0x3, 0x48, 0x3, 0x48, 0x3, 0x48, 0x7, 0x48, 0x4f9, 0xa, 0x48, 
    0xc, 0x48, 0xe, 0x48, 0x4fc, 0xb, 0x48, 0x3, 0x49, 0x3, 0x49, 0x3, 0x4a, 
    0x3, 0x4a, 0x3, 0x4a, 0x7, 0x4a, 0x503, 0xa, 0x4a, 0xc, 0x4a, 0xe, 0x4a, 
    0x506, 0xb, 0x4a, 0x3, 0x4b, 0x3, 0x4b, 0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4c, 
    0x3, 0x4c, 0x3, 0x4d, 0x3, 0x4d, 0x3, 0x4d, 0x7, 0x4d, 0x511, 0xa, 0x4d, 
    0xc, 0x4d, 0xe, 0x4d, 0x514, 0xb, 0x4d, 0x3, 0x4e, 0x3, 0x4e, 0x3, 0x4f, 
    0x3, 0x4f, 0x3, 0x4f, 0x7, 0x4f, 0x51b, 0xa, 0x4f, 0xc, 0x4f, 0xe, 0x4f, 
    0x51e, 0xb, 0x4f, 0x3, 0x50, 0x3, 0x50, 0x3, 0x50, 0x5, 0x50, 0x523, 
    0xa, 0x50, 0x3, 0x51, 0x3, 0x51, 0x3, 0x51, 0x7, 0x51, 0x528, 0xa, 0x51, 
    0xc, 0x51, 0xe, 0x51, 0x52b, 0xb, 0x51, 0x3, 0x52, 0x3, 0x52, 0x3, 0x53, 
    0x3, 0x53, 0x3, 0x54, 0x3, 0x54, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x6, 0x55, 0x551, 0xa, 0x55, 0xd, 0x55, 0xe, 0x55, 0x552, 0x3, 
    0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x6, 0x55, 0x55a, 
    0xa, 0x55, 0xd, 0x55, 0xe, 0x55, 0x55b, 0x5, 0x55, 0x55e, 0xa, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 0x562, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x7, 0x55, 0x582, 0xa, 0x55, 0xc, 0x55, 0xe, 0x55, 
    0x585, 0xb, 0x55, 0x5, 0x55, 0x587, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x7, 0x55, 0x58f, 0xa, 0x55, 
    0xc, 0x55, 0xe, 0x55, 0x592, 0xb, 0x55, 0x5, 0x55, 0x594, 0xa, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x7, 
    0x55, 0x59c, 0xa, 0x55, 0xc, 0x55, 0xe, 0x55, 0x59f, 0xb, 0x55, 0x5, 
    0x55, 0x5a1, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 
    0x55, 0x5ad, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 
    0x5bf, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x5, 0x55, 0x5c6, 0xa, 0x55, 0x5, 0x55, 0x5c8, 0xa, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 
    0x55, 0x5d1, 0xa, 0x55, 0x3, 0x55, 0x5, 0x55, 0x5d4, 0xa, 0x55, 0x3, 
    0x55, 0x5, 0x55, 0x5d7, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 
    0x5db, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x5, 0x55, 0x5e9, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 0x5f1, 0xa, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 
    0x55, 0x5fa, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 0x603, 0xa, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 
    0x55, 0x60c, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 0x615, 0xa, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x7, 0x55, 0x61f, 0xa, 0x55, 0xc, 0x55, 0xe, 0x55, 0x622, 0xb, 
    0x55, 0x5, 0x55, 0x624, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x3, 0x55, 0x5, 0x55, 0x631, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 0x63a, 
    0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 
    0x647, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 
    0x64d, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x7, 0x55, 0x665, 0xa, 0x55, 
    0xc, 0x55, 0xe, 0x55, 0x668, 0xb, 0x55, 0x5, 0x55, 0x66a, 0xa, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 0x671, 
    0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 0x676, 0xa, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x55, 0x5, 0x55, 0x67f, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x7, 0x55, 0x686, 0xa, 0x55, 0xc, 0x55, 0xe, 0x55, 
    0x689, 0xb, 0x55, 0x5, 0x55, 0x68b, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x5, 0x55, 0x690, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x7, 0x55, 0x697, 0xa, 0x55, 0xc, 0x55, 0xe, 0x55, 
    0x69a, 0xb, 0x55, 0x3, 0x56, 0x3, 0x56, 0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 
    0x5, 0x57, 0x6a1, 0xa, 0x57, 0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 0x5, 0x57, 
    0x6a6, 0xa, 0x57, 0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 0x5, 0x57, 0x6ab, 
    0xa, 0x57, 0x3, 0x57, 0x3, 0x57, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x5, 
    0x58, 0x6b2, 0xa, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x5, 0x58, 
    0x6b7, 0xa, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x5, 0x58, 0x6bc, 
    0xa, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x59, 0x3, 0x59, 0x3, 0x59, 0x7, 
    0x59, 0x6c3, 0xa, 0x59, 0xc, 0x59, 0xe, 0x59, 0x6c6, 0xb, 0x59, 0x3, 
    0x59, 0x3, 0x59, 0x3, 0x5a, 0x3, 0x5a, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5c, 
    0x3, 0x5c, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5e, 0x3, 0x5e, 0x3, 0x5f, 0x3, 
    0x5f, 0x3, 0x60, 0x3, 0x60, 0x3, 0x61, 0x3, 0x61, 0x3, 0x62, 0x3, 0x62, 
    0x3, 0x63, 0x3, 0x63, 0x3, 0x64, 0x3, 0x64, 0x3, 0x65, 0x3, 0x65, 0x3, 
    0x66, 0x3, 0x66, 0x3, 0x67, 0x3, 0x67, 0x5, 0x67, 0x6e6, 0xa, 0x67, 
    0x3, 0x68, 0x3, 0x68, 0x3, 0x68, 0x5, 0x68, 0x6eb, 0xa, 0x68, 0x3, 0x69, 
    0x3, 0x69, 0x3, 0x69, 0x3, 0x69, 0x5, 0x69, 0x6f1, 0xa, 0x69, 0x3, 0x6a, 
    0x3, 0x6a, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6c, 0x3, 0x6c, 0x3, 0x6d, 0x3, 
    0x6d, 0x3, 0x6e, 0x3, 0x6e, 0x3, 0x6e, 0x3, 0x6e, 0x3, 0x6e, 0x3, 0x6e, 
    0x3, 0x6e, 0x3, 0x6e, 0x3, 0x6e, 0x3, 0x6e, 0x5, 0x6e, 0x705, 0xa, 0x6e, 
    0x3, 0x6f, 0x3, 0x6f, 0x3, 0x6f, 0x3, 0x6f, 0x3, 0x6f, 0x3, 0x6f, 0x5, 
    0x6f, 0x70d, 0xa, 0x6f, 0x3, 0x70, 0x3, 0x70, 0x3, 0x71, 0x3, 0x71, 
    0x3, 0x72, 0x3, 0x72, 0x5, 0x72, 0x715, 0xa, 0x72, 0x3, 0x72, 0x3, 0x72, 
    0x3, 0x73, 0x3, 0x73, 0x3, 0x74, 0x3, 0x74, 0x5, 0x74, 0x71d, 0xa, 0x74, 
    0x3, 0x74, 0x3, 0x74, 0x3, 0x75, 0x3, 0x75, 0x3, 0x76, 0x3, 0x76, 0x5, 
    0x76, 0x725, 0xa, 0x76, 0x3, 0x76, 0x3, 0x76, 0x3, 0x77, 0x3, 0x77, 
    0x3, 0x77, 0x3, 0x78, 0x3, 0x78, 0x3, 0x78, 0x3, 0x79, 0x3, 0x79, 0x3, 
    0x79, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7b, 0x3, 0x7b, 0x5, 0x7b, 0x736, 
    0xa, 0x7b, 0x3, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 0x5, 0x7c, 0x73b, 0xa, 0x7c, 
    0x3, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 0x5, 
    0x7c, 0x743, 0xa, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 
    0x3, 0x7c, 0x5, 0x7c, 0x74a, 0xa, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 
    0x3, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 0x5, 0x7c, 0x753, 0xa, 0x7c, 
    0x5, 0x7c, 0x755, 0xa, 0x7c, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x5, 0x7d, 
    0x75a, 0xa, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 
    0x3, 0x7d, 0x5, 0x7d, 0x762, 0xa, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 
    0x3, 0x7d, 0x3, 0x7d, 0x5, 0x7d, 0x769, 0xa, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 
    0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x5, 
    0x7d, 0x773, 0xa, 0x7d, 0x5, 0x7d, 0x775, 0xa, 0x7d, 0x3, 0x7d, 0x3, 
    0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x5, 0x7d, 0x77c, 0xa, 0x7d, 
    0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 
    0x7d, 0x5, 0x7d, 0x785, 0xa, 0x7d, 0x5, 0x7d, 0x787, 0xa, 0x7d, 0x3, 
    0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x5, 0x7d, 0x78e, 
    0xa, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 
    0x7d, 0x5, 0x7d, 0x796, 0xa, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 
    0x3, 0x7d, 0x3, 0x7d, 0x5, 0x7d, 0x79d, 0xa, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 
    0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x5, 0x7d, 0x7a4, 0xa, 0x7d, 0x3, 0x7d, 
    0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x5, 0x7d, 0x7ab, 0xa, 0x7d, 
    0x3, 0x7d, 0x3, 0x7d, 0x5, 0x7d, 0x7af, 0xa, 0x7d, 0x5, 0x7d, 0x7b1, 
    0xa, 0x7d, 0x3, 0x7e, 0x3, 0x7e, 0x3, 0x7e, 0x3, 0x7f, 0x3, 0x7f, 0x3, 
    0x7f, 0x3, 0x80, 0x3, 0x80, 0x3, 0x80, 0x3, 0x80, 0x3, 0x81, 0x3, 0x81, 
    0x3, 0x82, 0x3, 0x82, 0x3, 0x82, 0x2, 0x5, 0x2a, 0x30, 0xa8, 0x83, 0x2, 
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
    0xf6, 0xf8, 0xfa, 0xfc, 0xfe, 0x100, 0x102, 0x2, 0x21, 0x4, 0x2, 0x9, 
    0x9, 0xb1, 0xb1, 0x4, 0x2, 0xa, 0xa, 0x16, 0x16, 0x4, 0x2, 0x90, 0x90, 
    0x98, 0x98, 0x3, 0x2, 0xa3, 0xa4, 0x4, 0x2, 0xd, 0xd, 0x9d, 0x9d, 0x5, 
    0x2, 0x4d, 0x4d, 0x66, 0x66, 0xbb, 0xbb, 0x5, 0x2, 0x57, 0x57, 0x6d, 
    0x6d, 0x97, 0x97, 0x5, 0x2, 0x3d, 0x3d, 0x4c, 0x4c, 0x53, 0x53, 0x4, 
    0x2, 0x62, 0x62, 0x6b, 0x6b, 0x6, 0x2, 0x26, 0x26, 0x5a, 0x5a, 0xa0, 
    0xa0, 0xb4, 0xb4, 0x5, 0x2, 0x20, 0x20, 0x29, 0x29, 0x45, 0x45, 0x3, 
    0x2, 0x28, 0x29, 0x4, 0x2, 0x2d, 0x2d, 0x2f, 0x2f, 0x4, 0x2, 0x28, 0x29, 
    0x2d, 0x2e, 0x4, 0x2, 0x1b, 0x1b, 0x43, 0x43, 0x4, 0x2, 0x9, 0x9, 0xf, 
    0xf, 0x4, 0x2, 0x8, 0x8, 0x1a, 0x1a, 0x4, 0x2, 0x8, 0x8, 0x8e, 0x8e, 
    0x4, 0x2, 0x8, 0x8, 0x58, 0x58, 0x4, 0x2, 0x8, 0x8, 0x55, 0x55, 0x4, 
    0x2, 0x8, 0x8, 0x61, 0x61, 0x3, 0x2, 0xa6, 0xa7, 0x5, 0x2, 0x24, 0x24, 
    0x6c, 0x6c, 0xb6, 0xb6, 0x8, 0x2, 0x3e, 0x3e, 0x5d, 0x5d, 0x79, 0x79, 
    0x7c, 0x7c, 0x9b, 0x9b, 0xcc, 0xcc, 0x5, 0x2, 0x7, 0x7, 0xa, 0xa, 0x10, 
    0x10, 0x5, 0x2, 0x9, 0x9, 0xf, 0xf, 0x11, 0x11, 0x4, 0x2, 0x4, 0x6, 
    0x12, 0x15, 0x3, 0x2, 0x37, 0x3a, 0x4, 0x2, 0x51, 0x51, 0xb8, 0xb8, 
    0x3, 0x2, 0xd0, 0xd2, 0x32, 0x2, 0x1b, 0x1b, 0x1f, 0x20, 0x22, 0x23, 
    0x26, 0x26, 0x28, 0x29, 0x2c, 0x2c, 0x2e, 0x2e, 0x32, 0x32, 0x3b, 0x3b, 
    0x3d, 0x3d, 0x3f, 0x3f, 0x41, 0x41, 0x43, 0x45, 0x47, 0x47, 0x4c, 0x4c, 
    0x4f, 0x4f, 0x51, 0x53, 0x56, 0x56, 0x5a, 0x5b, 0x5e, 0x60, 0x62, 0x62, 
    0x6b, 0x6b, 0x6f, 0x71, 0x73, 0x73, 0x75, 0x75, 0x77, 0x77, 0x7a, 0x7b, 
    0x7d, 0x7d, 0x80, 0x80, 0x82, 0x82, 0x85, 0x85, 0x8d, 0x8e, 0x90, 0x90, 
    0x92, 0x93, 0x95, 0x96, 0x99, 0x9a, 0x9c, 0x9c, 0xa0, 0xa6, 0xa9, 0xaa, 
    0xac, 0xad, 0xb2, 0xb4, 0xb7, 0xb7, 0xb9, 0xba, 0xbf, 0xbf, 0xc4, 0xc5, 
    0xc8, 0xc9, 0xcb, 0xcb, 0xcd, 0xcd, 0x2, 0x8af, 0x2, 0x105, 0x3, 0x2, 
    0x2, 0x2, 0x4, 0x109, 0x3, 0x2, 0x2, 0x2, 0x6, 0x11f, 0x3, 0x2, 0x2, 
    0x2, 0x8, 0x125, 0x3, 0x2, 0x2, 0x2, 0xa, 0x128, 0x3, 0x2, 0x2, 0x2, 
    0xc, 0x135, 0x3, 0x2, 0x2, 0x2, 0xe, 0x13a, 0x3, 0x2, 0x2, 0x2, 0x10, 
    0x14a, 0x3, 0x2, 0x2, 0x2, 0x12, 0x14c, 0x3, 0x2, 0x2, 0x2, 0x14, 0x155, 
    0x3, 0x2, 0x2, 0x2, 0x16, 0x15d, 0x3, 0x2, 0x2, 0x2, 0x18, 0x15f, 0x3, 
    0x2, 0x2, 0x2, 0x1a, 0x165, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x16b, 0x3, 0x2, 
    0x2, 0x2, 0x1e, 0x170, 0x3, 0x2, 0x2, 0x2, 0x20, 0x177, 0x3, 0x2, 0x2, 
    0x2, 0x22, 0x1ab, 0x3, 0x2, 0x2, 0x2, 0x24, 0x1b6, 0x3, 0x2, 0x2, 0x2, 
    0x26, 0x1e1, 0x3, 0x2, 0x2, 0x2, 0x28, 0x1ec, 0x3, 0x2, 0x2, 0x2, 0x2a, 
    0x202, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x213, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x21c, 
    0x3, 0x2, 0x2, 0x2, 0x30, 0x247, 0x3, 0x2, 0x2, 0x2, 0x32, 0x290, 0x3, 
    0x2, 0x2, 0x2, 0x34, 0x299, 0x3, 0x2, 0x2, 0x2, 0x36, 0x29e, 0x3, 0x2, 
    0x2, 0x2, 0x38, 0x2a0, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x2b7, 0x3, 0x2, 0x2, 
    0x2, 0x3c, 0x2d9, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x2ef, 0x3, 0x2, 0x2, 0x2, 
    0x40, 0x309, 0x3, 0x2, 0x2, 0x2, 0x42, 0x30b, 0x3, 0x2, 0x2, 0x2, 0x44, 
    0x310, 0x3, 0x2, 0x2, 0x2, 0x46, 0x325, 0x3, 0x2, 0x2, 0x2, 0x48, 0x327, 
    0x3, 0x2, 0x2, 0x2, 0x4a, 0x32b, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x336, 0x3, 
    0x2, 0x2, 0x2, 0x4e, 0x33c, 0x3, 0x2, 0x2, 0x2, 0x50, 0x34b, 0x3, 0x2, 
    0x2, 0x2, 0x52, 0x364, 0x3, 0x2, 0x2, 0x2, 0x54, 0x369, 0x3, 0x2, 0x2, 
    0x2, 0x56, 0x36b, 0x3, 0x2, 0x2, 0x2, 0x58, 0x36e, 0x3, 0x2, 0x2, 0x2, 
    0x5a, 0x370, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x372, 0x3, 0x2, 0x2, 0x2, 0x5e, 
    0x376, 0x3, 0x2, 0x2, 0x2, 0x60, 0x379, 0x3, 0x2, 0x2, 0x2, 0x62, 0x37b, 
    0x3, 0x2, 0x2, 0x2, 0x64, 0x37d, 0x3, 0x2, 0x2, 0x2, 0x66, 0x398, 0x3, 
    0x2, 0x2, 0x2, 0x68, 0x3a5, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x3ae, 0x3, 0x2, 
    0x2, 0x2, 0x6c, 0x3ba, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x3d9, 0x3, 0x2, 0x2, 
    0x2, 0x70, 0x3db, 0x3, 0x2, 0x2, 0x2, 0x72, 0x41c, 0x3, 0x2, 0x2, 0x2, 
    0x74, 0x41e, 0x3, 0x2, 0x2, 0x2, 0x76, 0x43a, 0x3, 0x2, 0x2, 0x2, 0x78, 
    0x490, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x492, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x4ae, 
    0x3, 0x2, 0x2, 0x2, 0x7e, 0x4b4, 0x3, 0x2, 0x2, 0x2, 0x80, 0x4cb, 0x3, 
    0x2, 0x2, 0x2, 0x82, 0x4cd, 0x3, 0x2, 0x2, 0x2, 0x84, 0x4dd, 0x3, 0x2, 
    0x2, 0x2, 0x86, 0x4df, 0x3, 0x2, 0x2, 0x2, 0x88, 0x4e7, 0x3, 0x2, 0x2, 
    0x2, 0x8a, 0x4e9, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x4f1, 0x3, 0x2, 0x2, 0x2, 
    0x8e, 0x4f5, 0x3, 0x2, 0x2, 0x2, 0x90, 0x4fd, 0x3, 0x2, 0x2, 0x2, 0x92, 
    0x4ff, 0x3, 0x2, 0x2, 0x2, 0x94, 0x507, 0x3, 0x2, 0x2, 0x2, 0x96, 0x509, 
    0x3, 0x2, 0x2, 0x2, 0x98, 0x50d, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x515, 0x3, 
    0x2, 0x2, 0x2, 0x9c, 0x517, 0x3, 0x2, 0x2, 0x2, 0x9e, 0x51f, 0x3, 0x2, 
    0x2, 0x2, 0xa0, 0x524, 0x3, 0x2, 0x2, 0x2, 0xa2, 0x52c, 0x3, 0x2, 0x2, 
    0x2, 0xa4, 0x52e, 0x3, 0x2, 0x2, 0x2, 0xa6, 0x530, 0x3, 0x2, 0x2, 0x2, 
    0xa8, 0x639, 0x3, 0x2, 0x2, 0x2, 0xaa, 0x69b, 0x3, 0x2, 0x2, 0x2, 0xac, 
    0x6aa, 0x3, 0x2, 0x2, 0x2, 0xae, 0x6bb, 0x3, 0x2, 0x2, 0x2, 0xb0, 0x6c4, 
    0x3, 0x2, 0x2, 0x2, 0xb2, 0x6c9, 0x3, 0x2, 0x2, 0x2, 0xb4, 0x6cb, 0x3, 
    0x2, 0x2, 0x2, 0xb6, 0x6cd, 0x3, 0x2, 0x2, 0x2, 0xb8, 0x6cf, 0x3, 0x2, 
    0x2, 0x2, 0xba, 0x6d1, 0x3, 0x2, 0x2, 0x2, 0xbc, 0x6d3, 0x3, 0x2, 0x2, 
    0x2, 0xbe, 0x6d5, 0x3, 0x2, 0x2, 0x2, 0xc0, 0x6d7, 0x3, 0x2, 0x2, 0x2, 
    0xc2, 0x6d9, 0x3, 0x2, 0x2, 0x2, 0xc4, 0x6db, 0x3, 0x2, 0x2, 0x2, 0xc6, 
    0x6dd, 0x3, 0x2, 0x2, 0x2, 0xc8, 0x6df, 0x3, 0x2, 0x2, 0x2, 0xca, 0x6e1, 
    0x3, 0x2, 0x2, 0x2, 0xcc, 0x6e5, 0x3, 0x2, 0x2, 0x2, 0xce, 0x6ea, 0x3, 
    0x2, 0x2, 0x2, 0xd0, 0x6f0, 0x3, 0x2, 0x2, 0x2, 0xd2, 0x6f2, 0x3, 0x2, 
    0x2, 0x2, 0xd4, 0x6f4, 0x3, 0x2, 0x2, 0x2, 0xd6, 0x6f6, 0x3, 0x2, 0x2, 
    0x2, 0xd8, 0x6f8, 0x3, 0x2, 0x2, 0x2, 0xda, 0x704, 0x3, 0x2, 0x2, 0x2, 
    0xdc, 0x70c, 0x3, 0x2, 0x2, 0x2, 0xde, 0x70e, 0x3, 0x2, 0x2, 0x2, 0xe0, 
    0x710, 0x3, 0x2, 0x2, 0x2, 0xe2, 0x714, 0x3, 0x2, 0x2, 0x2, 0xe4, 0x718, 
    0x3, 0x2, 0x2, 0x2, 0xe6, 0x71c, 0x3, 0x2, 0x2, 0x2, 0xe8, 0x720, 0x3, 
    0x2, 0x2, 0x2, 0xea, 0x724, 0x3, 0x2, 0x2, 0x2, 0xec, 0x728, 0x3, 0x2, 
    0x2, 0x2, 0xee, 0x72b, 0x3, 0x2, 0x2, 0x2, 0xf0, 0x72e, 0x3, 0x2, 0x2, 
    0x2, 0xf2, 0x731, 0x3, 0x2, 0x2, 0x2, 0xf4, 0x735, 0x3, 0x2, 0x2, 0x2, 
    0xf6, 0x737, 0x3, 0x2, 0x2, 0x2, 0xf8, 0x756, 0x3, 0x2, 0x2, 0x2, 0xfa, 
    0x7b2, 0x3, 0x2, 0x2, 0x2, 0xfc, 0x7b5, 0x3, 0x2, 0x2, 0x2, 0xfe, 0x7b8, 
    0x3, 0x2, 0x2, 0x2, 0x100, 0x7bc, 0x3, 0x2, 0x2, 0x2, 0x102, 0x7be, 
    0x3, 0x2, 0x2, 0x2, 0x104, 0x106, 0x5, 0x4, 0x3, 0x2, 0x105, 0x104, 
    0x3, 0x2, 0x2, 0x2, 0x106, 0x107, 0x3, 0x2, 0x2, 0x2, 0x107, 0x105, 
    0x3, 0x2, 0x2, 0x2, 0x107, 0x108, 0x3, 0x2, 0x2, 0x2, 0x108, 0x3, 0x3, 
    0x2, 0x2, 0x2, 0x109, 0x10a, 0x5, 0x8, 0x5, 0x2, 0x10a, 0x10b, 0x5, 
    0x6, 0x4, 0x2, 0x10b, 0x10c, 0x7, 0x3, 0x2, 0x2, 0x10c, 0x5, 0x3, 0x2, 
    0x2, 0x2, 0x10d, 0x120, 0x5, 0x1c, 0xf, 0x2, 0x10e, 0x120, 0x5, 0xe, 
    0x8, 0x2, 0x10f, 0x120, 0x5, 0x40, 0x21, 0x2, 0x110, 0x120, 0x5, 0x46, 
    0x24, 0x2, 0x111, 0x120, 0x5, 0x4c, 0x27, 0x2, 0x112, 0x120, 0x5, 0x50, 
    0x29, 0x2, 0x113, 0x120, 0x5, 0x4e, 0x28, 0x2, 0x114, 0x120, 0x5, 0xfa, 
    0x7e, 0x2, 0x115, 0x120, 0x5, 0xfc, 0x7f, 0x2, 0x116, 0x120, 0x5, 0xfe, 
    0x80, 0x2, 0x117, 0x120, 0x5, 0x100, 0x81, 0x2, 0x118, 0x120, 0x5, 0x56, 
    0x2c, 0x2, 0x119, 0x120, 0x5, 0x58, 0x2d, 0x2, 0x11a, 0x120, 0x5, 0x5a, 
    0x2e, 0x2, 0x11b, 0x120, 0x5, 0x5c, 0x2f, 0x2, 0x11c, 0x120, 0x5, 0x5e, 
    0x30, 0x2, 0x11d, 0x120, 0x5, 0x60, 0x31, 0x2, 0x11e, 0x120, 0x5, 0x62, 
    0x32, 0x2, 0x11f, 0x10d, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x10e, 0x3, 0x2, 
    0x2, 0x2, 0x11f, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x110, 0x3, 0x2, 
    0x2, 0x2, 0x11f, 0x111, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x112, 0x3, 0x2, 
    0x2, 0x2, 0x11f, 0x113, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x114, 0x3, 0x2, 
    0x2, 0x2, 0x11f, 0x115, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x116, 0x3, 0x2, 
    0x2, 0x2, 0x11f, 0x117, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x118, 0x3, 0x2, 
    0x2, 0x2, 0x11f, 0x119, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x11a, 0x3, 0x2, 
    0x2, 0x2, 0x11f, 0x11b, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x11c, 0x3, 0x2, 
    0x2, 0x2, 0x11f, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x11e, 0x3, 0x2, 
    0x2, 0x2, 0x120, 0x7, 0x3, 0x2, 0x2, 0x2, 0x121, 0x124, 0x5, 0xc, 0x7, 
    0x2, 0x122, 0x124, 0x5, 0xa, 0x6, 0x2, 0x123, 0x121, 0x3, 0x2, 0x2, 
    0x2, 0x123, 0x122, 0x3, 0x2, 0x2, 0x2, 0x124, 0x127, 0x3, 0x2, 0x2, 
    0x2, 0x125, 0x123, 0x3, 0x2, 0x2, 0x2, 0x125, 0x126, 0x3, 0x2, 0x2, 
    0x2, 0x126, 0x9, 0x3, 0x2, 0x2, 0x2, 0x127, 0x125, 0x3, 0x2, 0x2, 0x2, 
    0x128, 0x129, 0x7, 0x4, 0x2, 0x2, 0x129, 0x12a, 0x7, 0xce, 0x2, 0x2, 
    0x12a, 0x12b, 0x7, 0x5, 0x2, 0x2, 0x12b, 0x12e, 0x7, 0x6, 0x2, 0x2, 
    0x12c, 0x12f, 0x5, 0xf2, 0x7a, 0x2, 0x12d, 0x12f, 0x7, 0xd5, 0x2, 0x2, 
    0x12e, 0x12c, 0x3, 0x2, 0x2, 0x2, 0x12e, 0x12d, 0x3, 0x2, 0x2, 0x2, 
    0x12f, 0xb, 0x3, 0x2, 0x2, 0x2, 0x130, 0x131, 0x7, 0x7, 0x2, 0x2, 0x131, 
    0x136, 0x7, 0xce, 0x2, 0x2, 0x132, 0x133, 0x7, 0x4, 0x2, 0x2, 0x133, 
    0x134, 0x7, 0xce, 0x2, 0x2, 0x134, 0x136, 0x7, 0x5, 0x2, 0x2, 0x135, 
    0x130, 0x3, 0x2, 0x2, 0x2, 0x135, 0x132, 0x3, 0x2, 0x2, 0x2, 0x136, 
    0xd, 0x3, 0x2, 0x2, 0x2, 0x137, 0x139, 0x5, 0x20, 0x11, 0x2, 0x138, 
    0x137, 0x3, 0x2, 0x2, 0x2, 0x139, 0x13c, 0x3, 0x2, 0x2, 0x2, 0x13a, 
    0x138, 0x3, 0x2, 0x2, 0x2, 0x13a, 0x13b, 0x3, 0x2, 0x2, 0x2, 0x13b, 
    0x13d, 0x3, 0x2, 0x2, 0x2, 0x13c, 0x13a, 0x3, 0x2, 0x2, 0x2, 0x13d, 
    0x141, 0x7, 0x4f, 0x2, 0x2, 0x13e, 0x140, 0x5, 0x10, 0x9, 0x2, 0x13f, 
    0x13e, 0x3, 0x2, 0x2, 0x2, 0x140, 0x143, 0x3, 0x2, 0x2, 0x2, 0x141, 
    0x13f, 0x3, 0x2, 0x2, 0x2, 0x141, 0x142, 0x3, 0x2, 0x2, 0x2, 0x142, 
    0x144, 0x3, 0x2, 0x2, 0x2, 0x143, 0x141, 0x3, 0x2, 0x2, 0x2, 0x144, 
    0x145, 0x5, 0x6, 0x4, 0x2, 0x145, 0xf, 0x3, 0x2, 0x2, 0x2, 0x146, 0x14b, 
    0x5, 0x12, 0xa, 0x2, 0x147, 0x14b, 0x5, 0x18, 0xd, 0x2, 0x148, 0x14b, 
    0x7, 0xc8, 0x2, 0x2, 0x149, 0x14b, 0x7, 0xc9, 0x2, 0x2, 0x14a, 0x146, 
    0x3, 0x2, 0x2, 0x2, 0x14a, 0x147, 0x3, 0x2, 0x2, 0x2, 0x14a, 0x148, 
    0x3, 0x2, 0x2, 0x2, 0x14a, 0x149, 0x3, 0x2, 0x2, 0x2, 0x14b, 0x11, 0x3, 
    0x2, 0x2, 0x2, 0x14c, 0x14d, 0x7, 0xa2, 0x2, 0x2, 0x14d, 0x152, 0x5, 
    0x14, 0xb, 0x2, 0x14e, 0x14f, 0x7, 0x8, 0x2, 0x2, 0x14f, 0x151, 0x5, 
    0x14, 0xb, 0x2, 0x150, 0x14e, 0x3, 0x2, 0x2, 0x2, 0x151, 0x154, 0x3, 
    0x2, 0x2, 0x2, 0x152, 0x150, 0x3, 0x2, 0x2, 0x2, 0x152, 0x153, 0x3, 
    0x2, 0x2, 0x2, 0x153, 0x13, 0x3, 0x2, 0x2, 0x2, 0x154, 0x152, 0x3, 0x2, 
    0x2, 0x2, 0x155, 0x158, 0x5, 0x16, 0xc, 0x2, 0x156, 0x157, 0x9, 0x2, 
    0x2, 0x2, 0x157, 0x159, 0x5, 0x16, 0xc, 0x2, 0x158, 0x156, 0x3, 0x2, 
    0x2, 0x2, 0x158, 0x159, 0x3, 0x2, 0x2, 0x2, 0x159, 0x15, 0x3, 0x2, 0x2, 
    0x2, 0x15a, 0x15e, 0x9, 0x3, 0x2, 0x2, 0x15b, 0x15e, 0x7, 0xd5, 0x2, 
    0x2, 0x15c, 0x15e, 0x5, 0xd0, 0x69, 0x2, 0x15d, 0x15a, 0x3, 0x2, 0x2, 
    0x2, 0x15d, 0x15b, 0x3, 0x2, 0x2, 0x2, 0x15d, 0x15c, 0x3, 0x2, 0x2, 
    0x2, 0x15e, 0x17, 0x3, 0x2, 0x2, 0x2, 0x15f, 0x160, 0x7, 0x56, 0x2, 
    0x2, 0x160, 0x161, 0x9, 0x4, 0x2, 0x2, 0x161, 0x19, 0x3, 0x2, 0x2, 0x2, 
    0x162, 0x164, 0x5, 0x20, 0x11, 0x2, 0x163, 0x162, 0x3, 0x2, 0x2, 0x2, 
    0x164, 0x167, 0x3, 0x2, 0x2, 0x2, 0x165, 0x163, 0x3, 0x2, 0x2, 0x2, 
    0x165, 0x166, 0x3, 0x2, 0x2, 0x2, 0x166, 0x168, 0x3, 0x2, 0x2, 0x2, 
    0x167, 0x165, 0x3, 0x2, 0x2, 0x2, 0x168, 0x169, 0x9, 0x5, 0x2, 0x2, 
    0x169, 0x16a, 0x5, 0x6, 0x4, 0x2, 0x16a, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x16b, 
    0x16c, 0x5, 0x1e, 0x10, 0x2, 0x16c, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x16d, 
    0x16f, 0x5, 0x20, 0x11, 0x2, 0x16e, 0x16d, 0x3, 0x2, 0x2, 0x2, 0x16f, 
    0x172, 0x3, 0x2, 0x2, 0x2, 0x170, 0x16e, 0x3, 0x2, 0x2, 0x2, 0x170, 
    0x171, 0x3, 0x2, 0x2, 0x2, 0x171, 0x175, 0x3, 0x2, 0x2, 0x2, 0x172, 
    0x170, 0x3, 0x2, 0x2, 0x2, 0x173, 0x176, 0x5, 0x24, 0x13, 0x2, 0x174, 
    0x176, 0x5, 0x28, 0x15, 0x2, 0x175, 0x173, 0x3, 0x2, 0x2, 0x2, 0x175, 
    0x174, 0x3, 0x2, 0x2, 0x2, 0x176, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x177, 0x179, 
    0x7, 0xd6, 0x2, 0x2, 0x178, 0x17a, 0x5, 0x22, 0x12, 0x2, 0x179, 0x178, 
    0x3, 0x2, 0x2, 0x2, 0x179, 0x17a, 0x3, 0x2, 0x2, 0x2, 0x17a, 0x17f, 
    0x3, 0x2, 0x2, 0x2, 0x17b, 0x17c, 0x7, 0x8, 0x2, 0x2, 0x17c, 0x17e, 
    0x5, 0x22, 0x12, 0x2, 0x17d, 0x17b, 0x3, 0x2, 0x2, 0x2, 0x17e, 0x181, 
    0x3, 0x2, 0x2, 0x2, 0x17f, 0x17d, 0x3, 0x2, 0x2, 0x2, 0x17f, 0x180, 
    0x3, 0x2, 0x2, 0x2, 0x180, 0x182, 0x3, 0x2, 0x2, 0x2, 0x181, 0x17f, 
    0x3, 0x2, 0x2, 0x2, 0x182, 0x183, 0x7, 0xd7, 0x2, 0x2, 0x183, 0x21, 
    0x3, 0x2, 0x2, 0x2, 0x184, 0x1ac, 0x5, 0xc8, 0x65, 0x2, 0x185, 0x186, 
    0x5, 0xca, 0x66, 0x2, 0x186, 0x187, 0x7, 0x6, 0x2, 0x2, 0x187, 0x188, 
    0x5, 0xa8, 0x55, 0x2, 0x188, 0x1ac, 0x3, 0x2, 0x2, 0x2, 0x189, 0x18b, 
    0x5, 0xca, 0x66, 0x2, 0x18a, 0x18c, 0x7, 0x6, 0x2, 0x2, 0x18b, 0x18a, 
    0x3, 0x2, 0x2, 0x2, 0x18b, 0x18c, 0x3, 0x2, 0x2, 0x2, 0x18c, 0x18d, 
    0x3, 0x2, 0x2, 0x2, 0x18d, 0x18e, 0x7, 0xb, 0x2, 0x2, 0x18e, 0x193, 
    0x5, 0xa8, 0x55, 0x2, 0x18f, 0x190, 0x7, 0x8, 0x2, 0x2, 0x190, 0x192, 
    0x5, 0xa8, 0x55, 0x2, 0x191, 0x18f, 0x3, 0x2, 0x2, 0x2, 0x192, 0x195, 
    0x3, 0x2, 0x2, 0x2, 0x193, 0x191, 0x3, 0x2, 0x2, 0x2, 0x193, 0x194, 
    0x3, 0x2, 0x2, 0x2, 0x194, 0x196, 0x3, 0x2, 0x2, 0x2, 0x195, 0x193, 
    0x3, 0x2, 0x2, 0x2, 0x196, 0x197, 0x7, 0xc, 0x2, 0x2, 0x197, 0x1ac, 
    0x3, 0x2, 0x2, 0x2, 0x198, 0x199, 0x5, 0xc8, 0x65, 0x2, 0x199, 0x19a, 
    0x7, 0x6, 0x2, 0x2, 0x19a, 0x19b, 0x5, 0xce, 0x68, 0x2, 0x19b, 0x1ac, 
    0x3, 0x2, 0x2, 0x2, 0x19c, 0x19e, 0x5, 0xc8, 0x65, 0x2, 0x19d, 0x19f, 
    0x7, 0x6, 0x2, 0x2, 0x19e, 0x19d, 0x3, 0x2, 0x2, 0x2, 0x19e, 0x19f, 
    0x3, 0x2, 0x2, 0x2, 0x19f, 0x1a0, 0x3, 0x2, 0x2, 0x2, 0x1a0, 0x1a1, 
    0x7, 0xb, 0x2, 0x2, 0x1a1, 0x1a6, 0x5, 0xce, 0x68, 0x2, 0x1a2, 0x1a3, 
    0x7, 0x8, 0x2, 0x2, 0x1a3, 0x1a5, 0x5, 0xce, 0x68, 0x2, 0x1a4, 0x1a2, 
    0x3, 0x2, 0x2, 0x2, 0x1a5, 0x1a8, 0x3, 0x2, 0x2, 0x2, 0x1a6, 0x1a4, 
    0x3, 0x2, 0x2, 0x2, 0x1a6, 0x1a7, 0x3, 0x2, 0x2, 0x2, 0x1a7, 0x1a9, 
    0x3, 0x2, 0x2, 0x2, 0x1a8, 0x1a6, 0x3, 0x2, 0x2, 0x2, 0x1a9, 0x1aa, 
    0x7, 0xc, 0x2, 0x2, 0x1aa, 0x1ac, 0x3, 0x2, 0x2, 0x2, 0x1ab, 0x184, 
    0x3, 0x2, 0x2, 0x2, 0x1ab, 0x185, 0x3, 0x2, 0x2, 0x2, 0x1ab, 0x189, 
    0x3, 0x2, 0x2, 0x2, 0x1ab, 0x198, 0x3, 0x2, 0x2, 0x2, 0x1ab, 0x19c, 
    0x3, 0x2, 0x2, 0x2, 0x1ac, 0x23, 0x3, 0x2, 0x2, 0x2, 0x1ad, 0x1ae, 0x7, 
    0xca, 0x2, 0x2, 0x1ae, 0x1b3, 0x5, 0x2e, 0x18, 0x2, 0x1af, 0x1b0, 0x7, 
    0x8, 0x2, 0x2, 0x1b0, 0x1b2, 0x5, 0x2e, 0x18, 0x2, 0x1b1, 0x1af, 0x3, 
    0x2, 0x2, 0x2, 0x1b2, 0x1b5, 0x3, 0x2, 0x2, 0x2, 0x1b3, 0x1b1, 0x3, 
    0x2, 0x2, 0x2, 0x1b3, 0x1b4, 0x3, 0x2, 0x2, 0x2, 0x1b4, 0x1b7, 0x3, 
    0x2, 0x2, 0x2, 0x1b5, 0x1b3, 0x3, 0x2, 0x2, 0x2, 0x1b6, 0x1ad, 0x3, 
    0x2, 0x2, 0x2, 0x1b6, 0x1b7, 0x3, 0x2, 0x2, 0x2, 0x1b7, 0x1b8, 0x3, 
    0x2, 0x2, 0x2, 0x1b8, 0x1bc, 0x5, 0x26, 0x14, 0x2, 0x1b9, 0x1ba, 0x7, 
    0x88, 0x2, 0x2, 0x1ba, 0x1bb, 0x7, 0x27, 0x2, 0x2, 0x1bb, 0x1bd, 0x5, 
    0x8a, 0x46, 0x2, 0x1bc, 0x1b9, 0x3, 0x2, 0x2, 0x2, 0x1bc, 0x1bd, 0x3, 
    0x2, 0x2, 0x2, 0x1bd, 0x1c4, 0x3, 0x2, 0x2, 0x2, 0x1be, 0x1bf, 0x7, 
    0x6f, 0x2, 0x2, 0x1bf, 0x1c2, 0x7, 0xd5, 0x2, 0x2, 0x1c0, 0x1c1, 0x7, 
    0x85, 0x2, 0x2, 0x1c1, 0x1c3, 0x7, 0xd5, 0x2, 0x2, 0x1c2, 0x1c0, 0x3, 
    0x2, 0x2, 0x2, 0x1c2, 0x1c3, 0x3, 0x2, 0x2, 0x2, 0x1c3, 0x1c5, 0x3, 
    0x2, 0x2, 0x2, 0x1c4, 0x1be, 0x3, 0x2, 0x2, 0x2, 0x1c4, 0x1c5, 0x3, 
    0x2, 0x2, 0x2, 0x1c5, 0x25, 0x3, 0x2, 0x2, 0x2, 0x1c6, 0x1c9, 0x9, 0x6, 
    0x2, 0x2, 0x1c7, 0x1ca, 0x7, 0x16, 0x2, 0x2, 0x1c8, 0x1ca, 0x7, 0x46, 
    0x2, 0x2, 0x1c9, 0x1c7, 0x3, 0x2, 0x2, 0x2, 0x1c9, 0x1c8, 0x3, 0x2, 
    0x2, 0x2, 0x1c9, 0x1ca, 0x3, 0x2, 0x2, 0x2, 0x1ca, 0x1cb, 0x3, 0x2, 
    0x2, 0x2, 0x1cb, 0x1ce, 0x5, 0x82, 0x42, 0x2, 0x1cc, 0x1cd, 0x7, 0x58, 
    0x2, 0x2, 0x1cd, 0x1cf, 0x5, 0x30, 0x19, 0x2, 0x1ce, 0x1cc, 0x3, 0x2, 
    0x2, 0x2, 0x1ce, 0x1cf, 0x3, 0x2, 0x2, 0x2, 0x1cf, 0x1d2, 0x3, 0x2, 
    0x2, 0x2, 0x1d0, 0x1d1, 0x7, 0xc7, 0x2, 0x2, 0x1d1, 0x1d3, 0x5, 0xa2, 
    0x52, 0x2, 0x1d2, 0x1d0, 0x3, 0x2, 0x2, 0x2, 0x1d2, 0x1d3, 0x3, 0x2, 
    0x2, 0x2, 0x1d3, 0x1d7, 0x3, 0x2, 0x2, 0x2, 0x1d4, 0x1d5, 0x7, 0x59, 
    0x2, 0x2, 0x1d5, 0x1d6, 0x7, 0x27, 0x2, 0x2, 0x1d6, 0x1d8, 0x5, 0x86, 
    0x44, 0x2, 0x1d7, 0x1d4, 0x3, 0x2, 0x2, 0x2, 0x1d7, 0x1d8, 0x3, 0x2, 
    0x2, 0x2, 0x1d8, 0x1db, 0x3, 0x2, 0x2, 0x2, 0x1d9, 0x1da, 0x7, 0x5c, 
    0x2, 0x2, 0x1da, 0x1dc, 0x5, 0xa2, 0x52, 0x2, 0x1db, 0x1d9, 0x3, 0x2, 
    0x2, 0x2, 0x1db, 0x1dc, 0x3, 0x2, 0x2, 0x2, 0x1dc, 0x1e2, 0x3, 0x2, 
    0x2, 0x2, 0x1dd, 0x1de, 0x7, 0xb, 0x2, 0x2, 0x1de, 0x1df, 0x5, 0x26, 
    0x14, 0x2, 0x1df, 0x1e0, 0x7, 0xc, 0x2, 0x2, 0x1e0, 0x1e2, 0x3, 0x2, 
    0x2, 0x2, 0x1e1, 0x1c6, 0x3, 0x2, 0x2, 0x2, 0x1e1, 0x1dd, 0x3, 0x2, 
    0x2, 0x2, 0x1e2, 0x27, 0x3, 0x2, 0x2, 0x2, 0x1e3, 0x1e4, 0x7, 0xca, 
    0x2, 0x2, 0x1e4, 0x1e9, 0x5, 0x2e, 0x18, 0x2, 0x1e5, 0x1e6, 0x7, 0x8, 
    0x2, 0x2, 0x1e6, 0x1e8, 0x5, 0x2e, 0x18, 0x2, 0x1e7, 0x1e5, 0x3, 0x2, 
    0x2, 0x2, 0x1e8, 0x1eb, 0x3, 0x2, 0x2, 0x2, 0x1e9, 0x1e7, 0x3, 0x2, 
    0x2, 0x2, 0x1e9, 0x1ea, 0x3, 0x2, 0x2, 0x2, 0x1ea, 0x1ed, 0x3, 0x2, 
    0x2, 0x2, 0x1eb, 0x1e9, 0x3, 0x2, 0x2, 0x2, 0x1ec, 0x1e3, 0x3, 0x2, 
    0x2, 0x2, 0x1ec, 0x1ed, 0x3, 0x2, 0x2, 0x2, 0x1ed, 0x1ee, 0x3, 0x2, 
    0x2, 0x2, 0x1ee, 0x1f2, 0x5, 0x2a, 0x16, 0x2, 0x1ef, 0x1f0, 0x7, 0x88, 
    0x2, 0x2, 0x1f0, 0x1f1, 0x7, 0x27, 0x2, 0x2, 0x1f1, 0x1f3, 0x5, 0x8a, 
    0x46, 0x2, 0x1f2, 0x1ef, 0x3, 0x2, 0x2, 0x2, 0x1f2, 0x1f3, 0x3, 0x2, 
    0x2, 0x2, 0x1f3, 0x1fa, 0x3, 0x2, 0x2, 0x2, 0x1f4, 0x1f5, 0x7, 0x6f, 
    0x2, 0x2, 0x1f5, 0x1f8, 0x7, 0xd5, 0x2, 0x2, 0x1f6, 0x1f7, 0x7, 0x85, 
    0x2, 0x2, 0x1f7, 0x1f9, 0x7, 0xd5, 0x2, 0x2, 0x1f8, 0x1f6, 0x3, 0x2, 
    0x2, 0x2, 0x1f8, 0x1f9, 0x3, 0x2, 0x2, 0x2, 0x1f9, 0x1fb, 0x3, 0x2, 
    0x2, 0x2, 0x1fa, 0x1f4, 0x3, 0x2, 0x2, 0x2, 0x1fa, 0x1fb, 0x3, 0x2, 
    0x2, 0x2, 0x1fb, 0x29, 0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1fd, 0x8, 0x16, 
    0x1, 0x2, 0x1fd, 0x203, 0x5, 0x26, 0x14, 0x2, 0x1fe, 0x1ff, 0x7, 0xb, 
    0x2, 0x2, 0x1ff, 0x200, 0x5, 0x2a, 0x16, 0x2, 0x200, 0x201, 0x7, 0xc, 
    0x2, 0x2, 0x201, 0x203, 0x3, 0x2, 0x2, 0x2, 0x202, 0x1fc, 0x3, 0x2, 
    0x2, 0x2, 0x202, 0x1fe, 0x3, 0x2, 0x2, 0x2, 0x203, 0x210, 0x3, 0x2, 
    0x2, 0x2, 0x204, 0x205, 0xc, 0x4, 0x2, 0x2, 0x205, 0x208, 0x9, 0x7, 
    0x2, 0x2, 0x206, 0x209, 0x7, 0x46, 0x2, 0x2, 0x207, 0x209, 0x7, 0x16, 
    0x2, 0x2, 0x208, 0x206, 0x3, 0x2, 0x2, 0x2, 0x208, 0x207, 0x3, 0x2, 
    0x2, 0x2, 0x208, 0x209, 0x3, 0x2, 0x2, 0x2, 0x209, 0x20b, 0x3, 0x2, 
    0x2, 0x2, 0x20a, 0x20c, 0x5, 0x2c, 0x17, 0x2, 0x20b, 0x20a, 0x3, 0x2, 
    0x2, 0x2, 0x20b, 0x20c, 0x3, 0x2, 0x2, 0x2, 0x20c, 0x20d, 0x3, 0x2, 
    0x2, 0x2, 0x20d, 0x20f, 0x5, 0x2a, 0x16, 0x5, 0x20e, 0x204, 0x3, 0x2, 
    0x2, 0x2, 0x20f, 0x212, 0x3, 0x2, 0x2, 0x2, 0x210, 0x20e, 0x3, 0x2, 
    0x2, 0x2, 0x210, 0x211, 0x3, 0x2, 0x2, 0x2, 0x211, 0x2b, 0x3, 0x2, 0x2, 
    0x2, 0x212, 0x210, 0x3, 0x2, 0x2, 0x2, 0x213, 0x219, 0x7, 0x33, 0x2, 
    0x2, 0x214, 0x215, 0x7, 0x27, 0x2, 0x2, 0x215, 0x216, 0x7, 0xb, 0x2, 
    0x2, 0x216, 0x217, 0x5, 0x92, 0x4a, 0x2, 0x217, 0x218, 0x7, 0xc, 0x2, 
    0x2, 0x218, 0x21a, 0x3, 0x2, 0x2, 0x2, 0x219, 0x214, 0x3, 0x2, 0x2, 
    0x2, 0x219, 0x21a, 0x3, 0x2, 0x2, 0x2, 0x21a, 0x2d, 0x3, 0x2, 0x2, 0x2, 
    0x21b, 0x21d, 0x7, 0x94, 0x2, 0x2, 0x21c, 0x21b, 0x3, 0x2, 0x2, 0x2, 
    0x21c, 0x21d, 0x3, 0x2, 0x2, 0x2, 0x21d, 0x21e, 0x3, 0x2, 0x2, 0x2, 
    0x21e, 0x220, 0x5, 0xba, 0x5e, 0x2, 0x21f, 0x221, 0x5, 0x96, 0x4c, 0x2, 
    0x220, 0x21f, 0x3, 0x2, 0x2, 0x2, 0x220, 0x221, 0x3, 0x2, 0x2, 0x2, 
    0x221, 0x222, 0x3, 0x2, 0x2, 0x2, 0x222, 0x223, 0x7, 0x1a, 0x2, 0x2, 
    0x223, 0x224, 0x7, 0xb, 0x2, 0x2, 0x224, 0x225, 0x5, 0x1e, 0x10, 0x2, 
    0x225, 0x226, 0x7, 0xc, 0x2, 0x2, 0x226, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x227, 
    0x228, 0x8, 0x19, 0x1, 0x2, 0x228, 0x22d, 0x5, 0x32, 0x1a, 0x2, 0x229, 
    0x22b, 0x7, 0x1a, 0x2, 0x2, 0x22a, 0x229, 0x3, 0x2, 0x2, 0x2, 0x22a, 
    0x22b, 0x3, 0x2, 0x2, 0x2, 0x22b, 0x22c, 0x3, 0x2, 0x2, 0x2, 0x22c, 
    0x22e, 0x5, 0xbc, 0x5f, 0x2, 0x22d, 0x22a, 0x3, 0x2, 0x2, 0x2, 0x22d, 
    0x22e, 0x3, 0x2, 0x2, 0x2, 0x22e, 0x248, 0x3, 0x2, 0x2, 0x2, 0x22f, 
    0x230, 0x7, 0xb, 0x2, 0x2, 0x230, 0x231, 0x5, 0x1e, 0x10, 0x2, 0x231, 
    0x239, 0x7, 0xc, 0x2, 0x2, 0x232, 0x234, 0x7, 0x1a, 0x2, 0x2, 0x233, 
    0x232, 0x3, 0x2, 0x2, 0x2, 0x233, 0x234, 0x3, 0x2, 0x2, 0x2, 0x234, 
    0x235, 0x3, 0x2, 0x2, 0x2, 0x235, 0x237, 0x5, 0xbc, 0x5f, 0x2, 0x236, 
    0x238, 0x5, 0x96, 0x4c, 0x2, 0x237, 0x236, 0x3, 0x2, 0x2, 0x2, 0x237, 
    0x238, 0x3, 0x2, 0x2, 0x2, 0x238, 0x23a, 0x3, 0x2, 0x2, 0x2, 0x239, 
    0x233, 0x3, 0x2, 0x2, 0x2, 0x239, 0x23a, 0x3, 0x2, 0x2, 0x2, 0x23a, 
    0x248, 0x3, 0x2, 0x2, 0x2, 0x23b, 0x23c, 0x7, 0xb, 0x2, 0x2, 0x23c, 
    0x23d, 0x5, 0x38, 0x1d, 0x2, 0x23d, 0x245, 0x7, 0xc, 0x2, 0x2, 0x23e, 
    0x240, 0x7, 0x1a, 0x2, 0x2, 0x23f, 0x23e, 0x3, 0x2, 0x2, 0x2, 0x23f, 
    0x240, 0x3, 0x2, 0x2, 0x2, 0x240, 0x241, 0x3, 0x2, 0x2, 0x2, 0x241, 
    0x243, 0x5, 0xbc, 0x5f, 0x2, 0x242, 0x244, 0x5, 0x96, 0x4c, 0x2, 0x243, 
    0x242, 0x3, 0x2, 0x2, 0x2, 0x243, 0x244, 0x3, 0x2, 0x2, 0x2, 0x244, 
    0x246, 0x3, 0x2, 0x2, 0x2, 0x245, 0x23f, 0x3, 0x2, 0x2, 0x2, 0x245, 
    0x246, 0x3, 0x2, 0x2, 0x2, 0x246, 0x248, 0x3, 0x2, 0x2, 0x2, 0x247, 
    0x227, 0x3, 0x2, 0x2, 0x2, 0x247, 0x22f, 0x3, 0x2, 0x2, 0x2, 0x247, 
    0x23b, 0x3, 0x2, 0x2, 0x2, 0x248, 0x285, 0x3, 0x2, 0x2, 0x2, 0x249, 
    0x24d, 0xc, 0x8, 0x2, 0x2, 0x24a, 0x24c, 0x5, 0x20, 0x11, 0x2, 0x24b, 
    0x24a, 0x3, 0x2, 0x2, 0x2, 0x24c, 0x24f, 0x3, 0x2, 0x2, 0x2, 0x24d, 
    0x24b, 0x3, 0x2, 0x2, 0x2, 0x24d, 0x24e, 0x3, 0x2, 0x2, 0x2, 0x24e, 
    0x253, 0x3, 0x2, 0x2, 0x2, 0x24f, 0x24d, 0x3, 0x2, 0x2, 0x2, 0x250, 
    0x254, 0x7, 0x8, 0x2, 0x2, 0x251, 0x252, 0x7, 0x36, 0x2, 0x2, 0x252, 
    0x254, 0x7, 0x6a, 0x2, 0x2, 0x253, 0x250, 0x3, 0x2, 0x2, 0x2, 0x253, 
    0x251, 0x3, 0x2, 0x2, 0x2, 0x254, 0x255, 0x3, 0x2, 0x2, 0x2, 0x255, 
    0x284, 0x5, 0x30, 0x19, 0x9, 0x256, 0x25a, 0xc, 0x7, 0x2, 0x2, 0x257, 
    0x259, 0x5, 0x20, 0x11, 0x2, 0x258, 0x257, 0x3, 0x2, 0x2, 0x2, 0x259, 
    0x25c, 0x3, 0x2, 0x2, 0x2, 0x25a, 0x258, 0x3, 0x2, 0x2, 0x2, 0x25a, 
    0x25b, 0x3, 0x2, 0x2, 0x2, 0x25b, 0x25d, 0x3, 0x2, 0x2, 0x2, 0x25c, 
    0x25a, 0x3, 0x2, 0x2, 0x2, 0x25d, 0x25f, 0x7, 0x7e, 0x2, 0x2, 0x25e, 
    0x260, 0x5, 0x34, 0x1b, 0x2, 0x25f, 0x25e, 0x3, 0x2, 0x2, 0x2, 0x25f, 
    0x260, 0x3, 0x2, 0x2, 0x2, 0x260, 0x261, 0x3, 0x2, 0x2, 0x2, 0x261, 
    0x262, 0x7, 0x6a, 0x2, 0x2, 0x262, 0x284, 0x5, 0x30, 0x19, 0x8, 0x263, 
    0x267, 0xc, 0x6, 0x2, 0x2, 0x264, 0x266, 0x5, 0x20, 0x11, 0x2, 0x265, 
    0x264, 0x3, 0x2, 0x2, 0x2, 0x266, 0x269, 0x3, 0x2, 0x2, 0x2, 0x267, 
    0x265, 0x3, 0x2, 0x2, 0x2, 0x267, 0x268, 0x3, 0x2, 0x2, 0x2, 0x268, 
    0x26b, 0x3, 0x2, 0x2, 0x2, 0x269, 0x267, 0x3, 0x2, 0x2, 0x2, 0x26a, 
    0x26c, 0x5, 0x36, 0x1c, 0x2, 0x26b, 0x26a, 0x3, 0x2, 0x2, 0x2, 0x26b, 
    0x26c, 0x3, 0x2, 0x2, 0x2, 0x26c, 0x26d, 0x3, 0x2, 0x2, 0x2, 0x26d, 
    0x26e, 0x7, 0x6a, 0x2, 0x2, 0x26e, 0x26f, 0x5, 0x30, 0x19, 0x2, 0x26f, 
    0x270, 0x7, 0x86, 0x2, 0x2, 0x270, 0x271, 0x5, 0xa4, 0x53, 0x2, 0x271, 
    0x284, 0x3, 0x2, 0x2, 0x2, 0x272, 0x276, 0xc, 0x5, 0x2, 0x2, 0x273, 
    0x275, 0x5, 0x20, 0x11, 0x2, 0x274, 0x273, 0x3, 0x2, 0x2, 0x2, 0x275, 
    0x278, 0x3, 0x2, 0x2, 0x2, 0x276, 0x274, 0x3, 0x2, 0x2, 0x2, 0x276, 
    0x277, 0x3, 0x2, 0x2, 0x2, 0x277, 0x27a, 0x3, 0x2, 0x2, 0x2, 0x278, 
    0x276, 0x3, 0x2, 0x2, 0x2, 0x279, 0x27b, 0x5, 0x36, 0x1c, 0x2, 0x27a, 
    0x279, 0x3, 0x2, 0x2, 0x2, 0x27a, 0x27b, 0x3, 0x2, 0x2, 0x2, 0x27b, 
    0x27c, 0x3, 0x2, 0x2, 0x2, 0x27c, 0x27d, 0x7, 0x6a, 0x2, 0x2, 0x27d, 
    0x27e, 0x5, 0x30, 0x19, 0x2, 0x27e, 0x27f, 0x7, 0xbe, 0x2, 0x2, 0x27f, 
    0x280, 0x7, 0xb, 0x2, 0x2, 0x280, 0x281, 0x5, 0x8e, 0x48, 0x2, 0x281, 
    0x282, 0x7, 0xc, 0x2, 0x2, 0x282, 0x284, 0x3, 0x2, 0x2, 0x2, 0x283, 
    0x249, 0x3, 0x2, 0x2, 0x2, 0x283, 0x256, 0x3, 0x2, 0x2, 0x2, 0x283, 
    0x263, 0x3, 0x2, 0x2, 0x2, 0x283, 0x272, 0x3, 0x2, 0x2, 0x2, 0x284, 
    0x287, 0x3, 0x2, 0x2, 0x2, 0x285, 0x283, 0x3, 0x2, 0x2, 0x2, 0x285, 
    0x286, 0x3, 0x2, 0x2, 0x2, 0x286, 0x31, 0x3, 0x2, 0x2, 0x2, 0x287, 0x285, 
    0x3, 0x2, 0x2, 0x2, 0x288, 0x289, 0x5, 0xb6, 0x5c, 0x2, 0x289, 0x28a, 
    0x7, 0xe, 0x2, 0x2, 0x28a, 0x28c, 0x3, 0x2, 0x2, 0x2, 0x28b, 0x288, 
    0x3, 0x2, 0x2, 0x2, 0x28b, 0x28c, 0x3, 0x2, 0x2, 0x2, 0x28c, 0x28d, 
    0x3, 0x2, 0x2, 0x2, 0x28d, 0x28e, 0x5, 0xb8, 0x5d, 0x2, 0x28e, 0x28f, 
    0x7, 0xe, 0x2, 0x2, 0x28f, 0x291, 0x3, 0x2, 0x2, 0x2, 0x290, 0x28b, 
    0x3, 0x2, 0x2, 0x2, 0x290, 0x291, 0x3, 0x2, 0x2, 0x2, 0x291, 0x292, 
    0x3, 0x2, 0x2, 0x2, 0x292, 0x293, 0x5, 0xba, 0x5e, 0x2, 0x293, 0x33, 
    0x3, 0x2, 0x2, 0x2, 0x294, 0x29a, 0x7, 0x63, 0x2, 0x2, 0x295, 0x297, 
    0x9, 0x8, 0x2, 0x2, 0x296, 0x298, 0x7, 0x89, 0x2, 0x2, 0x297, 0x296, 
    0x3, 0x2, 0x2, 0x2, 0x297, 0x298, 0x3, 0x2, 0x2, 0x2, 0x298, 0x29a, 
    0x3, 0x2, 0x2, 0x2, 0x299, 0x294, 0x3, 0x2, 0x2, 0x2, 0x299, 0x295, 
    0x3, 0x2, 0x2, 0x2, 0x29a, 0x35, 0x3, 0x2, 0x2, 0x2, 0x29b, 0x29f, 0x5, 
    0x34, 0x1b, 0x2, 0x29c, 0x29f, 0x7, 0x9e, 0x2, 0x2, 0x29d, 0x29f, 0x7, 
    0x18, 0x2, 0x2, 0x29e, 0x29b, 0x3, 0x2, 0x2, 0x2, 0x29e, 0x29c, 0x3, 
    0x2, 0x2, 0x2, 0x29e, 0x29d, 0x3, 0x2, 0x2, 0x2, 0x29f, 0x37, 0x3, 0x2, 
    0x2, 0x2, 0x2a0, 0x2a1, 0x7, 0xc0, 0x2, 0x2, 0x2a1, 0x2a2, 0x7, 0xb, 
    0x2, 0x2, 0x2a2, 0x2a3, 0x5, 0xa0, 0x51, 0x2, 0x2a3, 0x2ab, 0x7, 0xc, 
    0x2, 0x2, 0x2a4, 0x2a5, 0x7, 0x8, 0x2, 0x2, 0x2a5, 0x2a6, 0x7, 0xb, 
    0x2, 0x2, 0x2a6, 0x2a7, 0x5, 0xa0, 0x51, 0x2, 0x2a7, 0x2a8, 0x7, 0xc, 
    0x2, 0x2, 0x2a8, 0x2aa, 0x3, 0x2, 0x2, 0x2, 0x2a9, 0x2a4, 0x3, 0x2, 
    0x2, 0x2, 0x2aa, 0x2ad, 0x3, 0x2, 0x2, 0x2, 0x2ab, 0x2a9, 0x3, 0x2, 
    0x2, 0x2, 0x2ab, 0x2ac, 0x3, 0x2, 0x2, 0x2, 0x2ac, 0x39, 0x3, 0x2, 0x2, 
    0x2, 0x2ad, 0x2ab, 0x3, 0x2, 0x2, 0x2, 0x2ae, 0x2af, 0x7, 0xca, 0x2, 
    0x2, 0x2af, 0x2b4, 0x5, 0x2e, 0x18, 0x2, 0x2b0, 0x2b1, 0x7, 0x8, 0x2, 
    0x2, 0x2b1, 0x2b3, 0x5, 0x2e, 0x18, 0x2, 0x2b2, 0x2b0, 0x3, 0x2, 0x2, 
    0x2, 0x2b3, 0x2b6, 0x3, 0x2, 0x2, 0x2, 0x2b4, 0x2b2, 0x3, 0x2, 0x2, 
    0x2, 0x2b4, 0x2b5, 0x3, 0x2, 0x2, 0x2, 0x2b5, 0x2b8, 0x3, 0x2, 0x2, 
    0x2, 0x2b6, 0x2b4, 0x3, 0x2, 0x2, 0x2, 0x2b7, 0x2ae, 0x3, 0x2, 0x2, 
    0x2, 0x2b7, 0x2b8, 0x3, 0x2, 0x2, 0x2, 0x2b8, 0x2b9, 0x3, 0x2, 0x2, 
    0x2, 0x2b9, 0x2bb, 0x7, 0x64, 0x2, 0x2, 0x2ba, 0x2bc, 0x7, 0x68, 0x2, 
    0x2, 0x2bb, 0x2ba, 0x3, 0x2, 0x2, 0x2, 0x2bb, 0x2bc, 0x3, 0x2, 0x2, 
    0x2, 0x2bc, 0x2bd, 0x3, 0x2, 0x2, 0x2, 0x2bd, 0x2c9, 0x5, 0x32, 0x1a, 
    0x2, 0x2be, 0x2bf, 0x7, 0xb, 0x2, 0x2, 0x2bf, 0x2c4, 0x5, 0xbe, 0x60, 
    0x2, 0x2c0, 0x2c1, 0x7, 0x8, 0x2, 0x2, 0x2c1, 0x2c3, 0x5, 0xbe, 0x60, 
    0x2, 0x2c2, 0x2c0, 0x3, 0x2, 0x2, 0x2, 0x2c3, 0x2c6, 0x3, 0x2, 0x2, 
    0x2, 0x2c4, 0x2c2, 0x3, 0x2, 0x2, 0x2, 0x2c4, 0x2c5, 0x3, 0x2, 0x2, 
    0x2, 0x2c5, 0x2c7, 0x3, 0x2, 0x2, 0x2, 0x2c6, 0x2c4, 0x3, 0x2, 0x2, 
    0x2, 0x2c7, 0x2c8, 0x7, 0xc, 0x2, 0x2, 0x2c8, 0x2ca, 0x3, 0x2, 0x2, 
    0x2, 0x2c9, 0x2be, 0x3, 0x2, 0x2, 0x2, 0x2c9, 0x2ca, 0x3, 0x2, 0x2, 
    0x2, 0x2ca, 0x2ce, 0x3, 0x2, 0x2, 0x2, 0x2cb, 0x2cf, 0x5, 0x38, 0x1d, 
    0x2, 0x2cc, 0x2cf, 0x5, 0x24, 0x13, 0x2, 0x2cd, 0x2cf, 0x5, 0x28, 0x15, 
    0x2, 0x2ce, 0x2cb, 0x3, 0x2, 0x2, 0x2, 0x2ce, 0x2cc, 0x3, 0x2, 0x2, 
    0x2, 0x2ce, 0x2cd, 0x3, 0x2, 0x2, 0x2, 0x2cf, 0x3b, 0x3, 0x2, 0x2, 0x2, 
    0x2d0, 0x2d1, 0x7, 0xca, 0x2, 0x2, 0x2d1, 0x2d6, 0x5, 0x2e, 0x18, 0x2, 
    0x2d2, 0x2d3, 0x7, 0x8, 0x2, 0x2, 0x2d3, 0x2d5, 0x5, 0x2e, 0x18, 0x2, 
    0x2d4, 0x2d2, 0x3, 0x2, 0x2, 0x2, 0x2d5, 0x2d8, 0x3, 0x2, 0x2, 0x2, 
    0x2d6, 0x2d4, 0x3, 0x2, 0x2, 0x2, 0x2d6, 0x2d7, 0x3, 0x2, 0x2, 0x2, 
    0x2d7, 0x2da, 0x3, 0x2, 0x2, 0x2, 0x2d8, 0x2d6, 0x3, 0x2, 0x2, 0x2, 
    0x2d9, 0x2d0, 0x3, 0x2, 0x2, 0x2, 0x2d9, 0x2da, 0x3, 0x2, 0x2, 0x2, 
    0x2da, 0x2db, 0x3, 0x2, 0x2, 0x2, 0x2db, 0x2dd, 0x7, 0x42, 0x2, 0x2, 
    0x2dc, 0x2de, 0x7, 0x58, 0x2, 0x2, 0x2dd, 0x2dc, 0x3, 0x2, 0x2, 0x2, 
    0x2dd, 0x2de, 0x3, 0x2, 0x2, 0x2, 0x2de, 0x2df, 0x3, 0x2, 0x2, 0x2, 
    0x2df, 0x2e4, 0x5, 0x32, 0x1a, 0x2, 0x2e0, 0x2e1, 0x7, 0xc7, 0x2, 0x2, 
    0x2e1, 0x2e5, 0x5, 0xa2, 0x52, 0x2, 0x2e2, 0x2e3, 0x7, 0x58, 0x2, 0x2, 
    0x2e3, 0x2e5, 0x5, 0x30, 0x19, 0x2, 0x2e4, 0x2e0, 0x3, 0x2, 0x2, 0x2, 
    0x2e4, 0x2e2, 0x3, 0x2, 0x2, 0x2, 0x2e4, 0x2e5, 0x3, 0x2, 0x2, 0x2, 
    0x2e5, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x2e6, 0x2e7, 0x7, 0xca, 0x2, 0x2, 
    0x2e7, 0x2ec, 0x5, 0x2e, 0x18, 0x2, 0x2e8, 0x2e9, 0x7, 0x8, 0x2, 0x2, 
    0x2e9, 0x2eb, 0x5, 0x2e, 0x18, 0x2, 0x2ea, 0x2e8, 0x3, 0x2, 0x2, 0x2, 
    0x2eb, 0x2ee, 0x3, 0x2, 0x2, 0x2, 0x2ec, 0x2ea, 0x3, 0x2, 0x2, 0x2, 
    0x2ec, 0x2ed, 0x3, 0x2, 0x2, 0x2, 0x2ed, 0x2f0, 0x3, 0x2, 0x2, 0x2, 
    0x2ee, 0x2ec, 0x3, 0x2, 0x2, 0x2, 0x2ef, 0x2e6, 0x3, 0x2, 0x2, 0x2, 
    0x2ef, 0x2f0, 0x3, 0x2, 0x2, 0x2, 0x2f0, 0x2f1, 0x3, 0x2, 0x2, 0x2, 
    0x2f1, 0x2f2, 0x7, 0xbd, 0x2, 0x2, 0x2f2, 0x2f3, 0x5, 0x32, 0x1a, 0x2, 
    0x2f3, 0x2f4, 0x7, 0x9f, 0x2, 0x2, 0x2f4, 0x2f5, 0x5, 0xbe, 0x60, 0x2, 
    0x2f5, 0x2f6, 0x7, 0x6, 0x2, 0x2, 0x2f6, 0x2fe, 0x5, 0xa8, 0x55, 0x2, 
    0x2f7, 0x2f8, 0x7, 0x8, 0x2, 0x2, 0x2f8, 0x2f9, 0x5, 0xbe, 0x60, 0x2, 
    0x2f9, 0x2fa, 0x7, 0x6, 0x2, 0x2, 0x2fa, 0x2fb, 0x5, 0xa8, 0x55, 0x2, 
    0x2fb, 0x2fd, 0x3, 0x2, 0x2, 0x2, 0x2fc, 0x2f7, 0x3, 0x2, 0x2, 0x2, 
    0x2fd, 0x300, 0x3, 0x2, 0x2, 0x2, 0x2fe, 0x2fc, 0x3, 0x2, 0x2, 0x2, 
    0x2fe, 0x2ff, 0x3, 0x2, 0x2, 0x2, 0x2ff, 0x305, 0x3, 0x2, 0x2, 0x2, 
    0x300, 0x2fe, 0x3, 0x2, 0x2, 0x2, 0x301, 0x302, 0x7, 0xc7, 0x2, 0x2, 
    0x302, 0x306, 0x5, 0xa2, 0x52, 0x2, 0x303, 0x304, 0x7, 0x58, 0x2, 0x2, 
    0x304, 0x306, 0x5, 0x30, 0x19, 0x2, 0x305, 0x301, 0x3, 0x2, 0x2, 0x2, 
    0x305, 0x303, 0x3, 0x2, 0x2, 0x2, 0x305, 0x306, 0x3, 0x2, 0x2, 0x2, 
    0x306, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x307, 0x30a, 0x5, 0x42, 0x22, 0x2, 
    0x308, 0x30a, 0x5, 0x44, 0x23, 0x2, 0x309, 0x307, 0x3, 0x2, 0x2, 0x2, 
    0x309, 0x308, 0x3, 0x2, 0x2, 0x2, 0x30a, 0x41, 0x3, 0x2, 0x2, 0x2, 0x30b, 
    0x30c, 0x7, 0x35, 0x2, 0x2, 0x30c, 0x30d, 0x7, 0xab, 0x2, 0x2, 0x30d, 
    0x30e, 0x5, 0xba, 0x5e, 0x2, 0x30e, 0x30f, 0x5, 0x64, 0x33, 0x2, 0x30f, 
    0x43, 0x3, 0x2, 0x2, 0x2, 0x310, 0x312, 0x7, 0x35, 0x2, 0x2, 0x311, 
    0x313, 0x7, 0xbc, 0x2, 0x2, 0x312, 0x311, 0x3, 0x2, 0x2, 0x2, 0x312, 
    0x313, 0x3, 0x2, 0x2, 0x2, 0x313, 0x314, 0x3, 0x2, 0x2, 0x2, 0x314, 
    0x315, 0x7, 0x62, 0x2, 0x2, 0x315, 0x316, 0x5, 0xc4, 0x63, 0x2, 0x316, 
    0x317, 0x7, 0x86, 0x2, 0x2, 0x317, 0x318, 0x5, 0xba, 0x5e, 0x2, 0x318, 
    0x319, 0x7, 0xb, 0x2, 0x2, 0x319, 0x31e, 0x5, 0xa8, 0x55, 0x2, 0x31a, 
    0x31b, 0x7, 0x8, 0x2, 0x2, 0x31b, 0x31d, 0x5, 0xa8, 0x55, 0x2, 0x31c, 
    0x31a, 0x3, 0x2, 0x2, 0x2, 0x31d, 0x320, 0x3, 0x2, 0x2, 0x2, 0x31e, 
    0x31c, 0x3, 0x2, 0x2, 0x2, 0x31e, 0x31f, 0x3, 0x2, 0x2, 0x2, 0x31f, 
    0x321, 0x3, 0x2, 0x2, 0x2, 0x320, 0x31e, 0x3, 0x2, 0x2, 0x2, 0x321, 
    0x322, 0x7, 0xc, 0x2, 0x2, 0x322, 0x45, 0x3, 0x2, 0x2, 0x2, 0x323, 0x326, 
    0x5, 0x48, 0x25, 0x2, 0x324, 0x326, 0x5, 0x4a, 0x26, 0x2, 0x325, 0x323, 
    0x3, 0x2, 0x2, 0x2, 0x325, 0x324, 0x3, 0x2, 0x2, 0x2, 0x326, 0x47, 0x3, 
    0x2, 0x2, 0x2, 0x327, 0x328, 0x7, 0x48, 0x2, 0x2, 0x328, 0x329, 0x7, 
    0xab, 0x2, 0x2, 0x329, 0x32a, 0x5, 0xba, 0x5e, 0x2, 0x32a, 0x49, 0x3, 
    0x2, 0x2, 0x2, 0x32b, 0x32c, 0x7, 0x48, 0x2, 0x2, 0x32c, 0x334, 0x7, 
    0x62, 0x2, 0x2, 0x32d, 0x32e, 0x5, 0xc4, 0x63, 0x2, 0x32e, 0x32f, 0x7, 
    0x86, 0x2, 0x2, 0x32f, 0x330, 0x5, 0xba, 0x5e, 0x2, 0x330, 0x335, 0x3, 
    0x2, 0x2, 0x2, 0x331, 0x332, 0x5, 0xba, 0x5e, 0x2, 0x332, 0x333, 0x5, 
    0xc4, 0x63, 0x2, 0x333, 0x335, 0x3, 0x2, 0x2, 0x2, 0x334, 0x32d, 0x3, 
    0x2, 0x2, 0x2, 0x334, 0x331, 0x3, 0x2, 0x2, 0x2, 0x335, 0x4b, 0x3, 0x2, 
    0x2, 0x2, 0x336, 0x338, 0x7, 0xb7, 0x2, 0x2, 0x337, 0x339, 0x7, 0xab, 
    0x2, 0x2, 0x338, 0x337, 0x3, 0x2, 0x2, 0x2, 0x338, 0x339, 0x3, 0x2, 
    0x2, 0x2, 0x339, 0x33a, 0x3, 0x2, 0x2, 0x2, 0x33a, 0x33b, 0x5, 0xba, 
    0x5e, 0x2, 0x33b, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x33c, 0x33d, 0x7, 0x70, 
    0x2, 0x2, 0x33d, 0x33e, 0x5, 0xba, 0x5e, 0x2, 0x33e, 0x349, 0x5, 0xf2, 
    0x7a, 0x2, 0x33f, 0x346, 0x5, 0x52, 0x2a, 0x2, 0x340, 0x342, 0x7, 0x8, 
    0x2, 0x2, 0x341, 0x340, 0x3, 0x2, 0x2, 0x2, 0x341, 0x342, 0x3, 0x2, 
    0x2, 0x2, 0x342, 0x343, 0x3, 0x2, 0x2, 0x2, 0x343, 0x345, 0x5, 0x52, 
    0x2a, 0x2, 0x344, 0x341, 0x3, 0x2, 0x2, 0x2, 0x345, 0x348, 0x3, 0x2, 
    0x2, 0x2, 0x346, 0x344, 0x3, 0x2, 0x2, 0x2, 0x346, 0x347, 0x3, 0x2, 
    0x2, 0x2, 0x347, 0x34a, 0x3, 0x2, 0x2, 0x2, 0x348, 0x346, 0x3, 0x2, 
    0x2, 0x2, 0x349, 0x33f, 0x3, 0x2, 0x2, 0x2, 0x349, 0x34a, 0x3, 0x2, 
    0x2, 0x2, 0x34a, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x34b, 0x34c, 0x7, 0x47, 
    0x2, 0x2, 0x34c, 0x34d, 0x5, 0xba, 0x5e, 0x2, 0x34d, 0x34f, 0x7, 0x8d, 
    0x2, 0x2, 0x34e, 0x350, 0x5, 0xf2, 0x7a, 0x2, 0x34f, 0x34e, 0x3, 0x2, 
    0x2, 0x2, 0x350, 0x351, 0x3, 0x2, 0x2, 0x2, 0x351, 0x34f, 0x3, 0x2, 
    0x2, 0x2, 0x351, 0x352, 0x3, 0x2, 0x2, 0x2, 0x352, 0x35d, 0x3, 0x2, 
    0x2, 0x2, 0x353, 0x35a, 0x5, 0x52, 0x2a, 0x2, 0x354, 0x356, 0x7, 0x8, 
    0x2, 0x2, 0x355, 0x354, 0x3, 0x2, 0x2, 0x2, 0x355, 0x356, 0x3, 0x2, 
    0x2, 0x2, 0x356, 0x357, 0x3, 0x2, 0x2, 0x2, 0x357, 0x359, 0x5, 0x52, 
    0x2a, 0x2, 0x358, 0x355, 0x3, 0x2, 0x2, 0x2, 0x359, 0x35c, 0x3, 0x2, 
    0x2, 0x2, 0x35a, 0x358, 0x3, 0x2, 0x2, 0x2, 0x35a, 0x35b, 0x3, 0x2, 
    0x2, 0x2, 0x35b, 0x35e, 0x3, 0x2, 0x2, 0x2, 0x35c, 0x35a, 0x3, 0x2, 
    0x2, 0x2, 0x35d, 0x353, 0x3, 0x2, 0x2, 0x2, 0x35d, 0x35e, 0x3, 0x2, 
    0x2, 0x2, 0x35e, 0x51, 0x3, 0x2, 0x2, 0x2, 0x35f, 0x360, 0x7, 0x4, 0x2, 
    0x2, 0x360, 0x361, 0x5, 0x54, 0x2b, 0x2, 0x361, 0x362, 0x7, 0x5, 0x2, 
    0x2, 0x362, 0x365, 0x3, 0x2, 0x2, 0x2, 0x363, 0x365, 0x5, 0x54, 0x2b, 
    0x2, 0x364, 0x35f, 0x3, 0x2, 0x2, 0x2, 0x364, 0x363, 0x3, 0x2, 0x2, 
    0x2, 0x365, 0x366, 0x3, 0x2, 0x2, 0x2, 0x366, 0x367, 0x7, 0x6, 0x2, 
    0x2, 0x367, 0x368, 0x5, 0xf2, 0x7a, 0x2, 0x368, 0x53, 0x3, 0x2, 0x2, 
    0x2, 0x369, 0x36a, 0x9, 0x9, 0x2, 0x2, 0x36a, 0x55, 0x3, 0x2, 0x2, 0x2, 
    0x36b, 0x36c, 0x7, 0x34, 0x2, 0x2, 0x36c, 0x36d, 0x5, 0xba, 0x5e, 0x2, 
    0x36d, 0x57, 0x3, 0x2, 0x2, 0x2, 0x36e, 0x36f, 0x7, 0xa1, 0x2, 0x2, 
    0x36f, 0x59, 0x3, 0x2, 0x2, 0x2, 0x370, 0x371, 0x7, 0xa5, 0x2, 0x2, 
    0x371, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x372, 0x373, 0x7, 0xa9, 0x2, 0x2, 
    0x373, 0x374, 0x5, 0xba, 0x5e, 0x2, 0x374, 0x375, 0x5, 0xc4, 0x63, 0x2, 
    0x375, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x376, 0x377, 0x7, 0xaa, 0x2, 0x2, 
    0x377, 0x378, 0x5, 0xba, 0x5e, 0x2, 0x378, 0x5f, 0x3, 0x2, 0x2, 0x2, 
    0x379, 0x37a, 0x7, 0xb3, 0x2, 0x2, 0x37a, 0x61, 0x3, 0x2, 0x2, 0x2, 
    0x37b, 0x37c, 0x7, 0x77, 0x2, 0x2, 0x37c, 0x63, 0x3, 0x2, 0x2, 0x2, 
    0x37d, 0x380, 0x7, 0xb, 0x2, 0x2, 0x37e, 0x381, 0x5, 0x68, 0x35, 0x2, 
    0x37f, 0x381, 0x3, 0x2, 0x2, 0x2, 0x380, 0x37e, 0x3, 0x2, 0x2, 0x2, 
    0x380, 0x37f, 0x3, 0x2, 0x2, 0x2, 0x381, 0x389, 0x3, 0x2, 0x2, 0x2, 
    0x382, 0x385, 0x7, 0x8, 0x2, 0x2, 0x383, 0x386, 0x5, 0x68, 0x35, 0x2, 
    0x384, 0x386, 0x3, 0x2, 0x2, 0x2, 0x385, 0x383, 0x3, 0x2, 0x2, 0x2, 
    0x385, 0x384, 0x3, 0x2, 0x2, 0x2, 0x386, 0x388, 0x3, 0x2, 0x2, 0x2, 
    0x387, 0x382, 0x3, 0x2, 0x2, 0x2, 0x388, 0x38b, 0x3, 0x2, 0x2, 0x2, 
    0x389, 0x387, 0x3, 0x2, 0x2, 0x2, 0x389, 0x38a, 0x3, 0x2, 0x2, 0x2, 
    0x38a, 0x38c, 0x3, 0x2, 0x2, 0x2, 0x38b, 0x389, 0x3, 0x2, 0x2, 0x2, 
    0x38c, 0x395, 0x7, 0xc, 0x2, 0x2, 0x38d, 0x394, 0x5, 0x66, 0x34, 0x2, 
    0x38e, 0x394, 0x7, 0x96, 0x2, 0x2, 0x38f, 0x390, 0x7, 0x99, 0x2, 0x2, 
    0x390, 0x394, 0x5, 0xe0, 0x71, 0x2, 0x391, 0x392, 0x7, 0xac, 0x2, 0x2, 
    0x392, 0x394, 0x5, 0xe0, 0x71, 0x2, 0x393, 0x38d, 0x3, 0x2, 0x2, 0x2, 
    0x393, 0x38e, 0x3, 0x2, 0x2, 0x2, 0x393, 0x38f, 0x3, 0x2, 0x2, 0x2, 
    0x393, 0x391, 0x3, 0x2, 0x2, 0x2, 0x394, 0x397, 0x3, 0x2, 0x2, 0x2, 
    0x395, 0x393, 0x3, 0x2, 0x2, 0x2, 0x395, 0x396, 0x3, 0x2, 0x2, 0x2, 
    0x396, 0x65, 0x3, 0x2, 0x2, 0x2, 0x397, 0x395, 0x3, 0x2, 0x2, 0x2, 0x398, 
    0x399, 0x7, 0x8c, 0x2, 0x2, 0x399, 0x39a, 0x7, 0x27, 0x2, 0x2, 0x39a, 
    0x39b, 0x7, 0xb, 0x2, 0x2, 0x39b, 0x3a0, 0x5, 0xa8, 0x55, 0x2, 0x39c, 
    0x39d, 0x7, 0x8, 0x2, 0x2, 0x39d, 0x39f, 0x5, 0xa8, 0x55, 0x2, 0x39e, 
    0x39c, 0x3, 0x2, 0x2, 0x2, 0x39f, 0x3a2, 0x3, 0x2, 0x2, 0x2, 0x3a0, 
    0x39e, 0x3, 0x2, 0x2, 0x2, 0x3a0, 0x3a1, 0x3, 0x2, 0x2, 0x2, 0x3a1, 
    0x3a3, 0x3, 0x2, 0x2, 0x2, 0x3a2, 0x3a0, 0x3, 0x2, 0x2, 0x2, 0x3a3, 
    0x3a4, 0x7, 0xc, 0x2, 0x2, 0x3a4, 0x67, 0x3, 0x2, 0x2, 0x2, 0x3a5, 0x3a6, 
    0x5, 0xbe, 0x60, 0x2, 0x3a6, 0x3ab, 0x5, 0x70, 0x39, 0x2, 0x3a7, 0x3a8, 
    0x7, 0x81, 0x2, 0x2, 0x3a8, 0x3aa, 0x7, 0x83, 0x2, 0x2, 0x3a9, 0x3a7, 
    0x3, 0x2, 0x2, 0x2, 0x3aa, 0x3ad, 0x3, 0x2, 0x2, 0x2, 0x3ab, 0x3a9, 
    0x3, 0x2, 0x2, 0x2, 0x3ab, 0x3ac, 0x3, 0x2, 0x2, 0x2, 0x3ac, 0x69, 0x3, 
    0x2, 0x2, 0x2, 0x3ad, 0x3ab, 0x3, 0x2, 0x2, 0x2, 0x3ae, 0x3af, 0x7, 
    0xb, 0x2, 0x2, 0x3af, 0x3b4, 0x5, 0xbe, 0x60, 0x2, 0x3b0, 0x3b1, 0x7, 
    0x8, 0x2, 0x2, 0x3b1, 0x3b3, 0x5, 0xbe, 0x60, 0x2, 0x3b2, 0x3b0, 0x3, 
    0x2, 0x2, 0x2, 0x3b3, 0x3b6, 0x3, 0x2, 0x2, 0x2, 0x3b4, 0x3b2, 0x3, 
    0x2, 0x2, 0x2, 0x3b4, 0x3b5, 0x3, 0x2, 0x2, 0x2, 0x3b5, 0x3b7, 0x3, 
    0x2, 0x2, 0x2, 0x3b6, 0x3b4, 0x3, 0x2, 0x2, 0x2, 0x3b7, 0x3b8, 0x7, 
    0xc, 0x2, 0x2, 0x3b8, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x3b9, 0x3bb, 0x7, 0xbc, 
    0x2, 0x2, 0x3ba, 0x3b9, 0x3, 0x2, 0x2, 0x2, 0x3ba, 0x3bb, 0x3, 0x2, 
    0x2, 0x2, 0x3bb, 0x3bc, 0x3, 0x2, 0x2, 0x2, 0x3bc, 0x3be, 0x9, 0xa, 
    0x2, 0x2, 0x3bd, 0x3bf, 0x5, 0xc4, 0x63, 0x2, 0x3be, 0x3bd, 0x3, 0x2, 
    0x2, 0x2, 0x3be, 0x3bf, 0x3, 0x2, 0x2, 0x2, 0x3bf, 0x3c1, 0x3, 0x2, 
    0x2, 0x2, 0x3c0, 0x3c2, 0x7, 0x86, 0x2, 0x2, 0x3c1, 0x3c0, 0x3, 0x2, 
    0x2, 0x2, 0x3c1, 0x3c2, 0x3, 0x2, 0x2, 0x2, 0x3c2, 0x3cf, 0x3, 0x2, 
    0x2, 0x2, 0x3c3, 0x3d0, 0x5, 0xa8, 0x55, 0x2, 0x3c4, 0x3c5, 0x7, 0xb, 
    0x2, 0x2, 0x3c5, 0x3ca, 0x5, 0xa8, 0x55, 0x2, 0x3c6, 0x3c7, 0x7, 0x8, 
    0x2, 0x2, 0x3c7, 0x3c9, 0x5, 0xa8, 0x55, 0x2, 0x3c8, 0x3c6, 0x3, 0x2, 
    0x2, 0x2, 0x3c9, 0x3cc, 0x3, 0x2, 0x2, 0x2, 0x3ca, 0x3c8, 0x3, 0x2, 
    0x2, 0x2, 0x3ca, 0x3cb, 0x3, 0x2, 0x2, 0x2, 0x3cb, 0x3cd, 0x3, 0x2, 
    0x2, 0x2, 0x3cc, 0x3ca, 0x3, 0x2, 0x2, 0x2, 0x3cd, 0x3ce, 0x7, 0xc, 
    0x2, 0x2, 0x3ce, 0x3d0, 0x3, 0x2, 0x2, 0x2, 0x3cf, 0x3c3, 0x3, 0x2, 
    0x2, 0x2, 0x3cf, 0x3c4, 0x3, 0x2, 0x2, 0x2, 0x3d0, 0x3d3, 0x3, 0x2, 
    0x2, 0x2, 0x3d1, 0x3d2, 0x7, 0xbe, 0x2, 0x2, 0x3d2, 0x3d4, 0x5, 0x6e, 
    0x38, 0x2, 0x3d3, 0x3d1, 0x3, 0x2, 0x2, 0x2, 0x3d3, 0x3d4, 0x3, 0x2, 
    0x2, 0x2, 0x3d4, 0x3d7, 0x3, 0x2, 0x2, 0x2, 0x3d5, 0x3d6, 0x7, 0xca, 
    0x2, 0x2, 0x3d6, 0x3d8, 0x5, 0x9c, 0x4f, 0x2, 0x3d7, 0x3d5, 0x3, 0x2, 
    0x2, 0x2, 0x3d7, 0x3d8, 0x3, 0x2, 0x2, 0x2, 0x3d8, 0x6d, 0x3, 0x2, 0x2, 
    0x2, 0x3d9, 0x3da, 0x9, 0xb, 0x2, 0x2, 0x3da, 0x6f, 0x3, 0x2, 0x2, 0x2, 
    0x3db, 0x3dc, 0x5, 0x72, 0x3a, 0x2, 0x3dc, 0x71, 0x3, 0x2, 0x2, 0x2, 
    0x3dd, 0x41d, 0x7, 0x25, 0x2, 0x2, 0x3de, 0x3e6, 0x7, 0x65, 0x2, 0x2, 
    0x3df, 0x3e0, 0x7, 0xb, 0x2, 0x2, 0x3e0, 0x3e2, 0x5, 0xd4, 0x6b, 0x2, 
    0x3e1, 0x3e3, 0x9, 0xc, 0x2, 0x2, 0x3e2, 0x3e1, 0x3, 0x2, 0x2, 0x2, 
    0x3e2, 0x3e3, 0x3, 0x2, 0x2, 0x2, 0x3e3, 0x3e4, 0x3, 0x2, 0x2, 0x2, 
    0x3e4, 0x3e5, 0x7, 0xc, 0x2, 0x2, 0x3e5, 0x3e7, 0x3, 0x2, 0x2, 0x2, 
    0x3e6, 0x3df, 0x3, 0x2, 0x2, 0x2, 0x3e6, 0x3e7, 0x3, 0x2, 0x2, 0x2, 
    0x3e7, 0x41d, 0x3, 0x2, 0x2, 0x2, 0x3e8, 0x3f7, 0x7, 0x40, 0x2, 0x2, 
    0x3e9, 0x3ea, 0x7, 0xb, 0x2, 0x2, 0x3ea, 0x3ec, 0x5, 0xd4, 0x6b, 0x2, 
    0x3eb, 0x3ed, 0x7, 0x45, 0x2, 0x2, 0x3ec, 0x3eb, 0x3, 0x2, 0x2, 0x2, 
    0x3ec, 0x3ed, 0x3, 0x2, 0x2, 0x2, 0x3ed, 0x3f3, 0x3, 0x2, 0x2, 0x2, 
    0x3ee, 0x3ef, 0x7, 0x8, 0x2, 0x2, 0x3ef, 0x3f1, 0x5, 0xd6, 0x6c, 0x2, 
    0x3f0, 0x3f2, 0x7, 0x45, 0x2, 0x2, 0x3f1, 0x3f0, 0x3, 0x2, 0x2, 0x2, 
    0x3f1, 0x3f2, 0x3, 0x2, 0x2, 0x2, 0x3f2, 0x3f4, 0x3, 0x2, 0x2, 0x2, 
    0x3f3, 0x3ee, 0x3, 0x2, 0x2, 0x2, 0x3f3, 0x3f4, 0x3, 0x2, 0x2, 0x2, 
    0x3f4, 0x3f5, 0x3, 0x2, 0x2, 0x2, 0x3f5, 0x3f6, 0x7, 0xc, 0x2, 0x2, 
    0x3f6, 0x3f8, 0x3, 0x2, 0x2, 0x2, 0x3f7, 0x3e9, 0x3, 0x2, 0x2, 0x2, 
    0x3f7, 0x3f8, 0x3, 0x2, 0x2, 0x2, 0x3f8, 0x41d, 0x3, 0x2, 0x2, 0x2, 
    0x3f9, 0x401, 0x7, 0x54, 0x2, 0x2, 0x3fa, 0x3fb, 0x7, 0xb, 0x2, 0x2, 
    0x3fb, 0x3fd, 0x5, 0xd4, 0x6b, 0x2, 0x3fc, 0x3fe, 0x9, 0xc, 0x2, 0x2, 
    0x3fd, 0x3fc, 0x3, 0x2, 0x2, 0x2, 0x3fd, 0x3fe, 0x3, 0x2, 0x2, 0x2, 
    0x3fe, 0x3ff, 0x3, 0x2, 0x2, 0x2, 0x3ff, 0x400, 0x7, 0xc, 0x2, 0x2, 
    0x400, 0x402, 0x3, 0x2, 0x2, 0x2, 0x401, 0x3fa, 0x3, 0x2, 0x2, 0x2, 
    0x401, 0x402, 0x3, 0x2, 0x2, 0x2, 0x402, 0x41d, 0x3, 0x2, 0x2, 0x2, 
    0x403, 0x41d, 0x7, 0x3c, 0x2, 0x2, 0x404, 0x41d, 0x7, 0xaf, 0x2, 0x2, 
    0x405, 0x41d, 0x7, 0xb0, 0x2, 0x2, 0x406, 0x41d, 0x5, 0x74, 0x3b, 0x2, 
    0x407, 0x41d, 0x5, 0x7a, 0x3e, 0x2, 0x408, 0x41d, 0x5, 0x7c, 0x3f, 0x2, 
    0x409, 0x40a, 0x7, 0x1e, 0x2, 0x2, 0x40a, 0x40b, 0x7, 0xb, 0x2, 0x2, 
    0x40b, 0x40d, 0x5, 0xd8, 0x6d, 0x2, 0x40c, 0x40e, 0x9, 0xd, 0x2, 0x2, 
    0x40d, 0x40c, 0x3, 0x2, 0x2, 0x2, 0x40d, 0x40e, 0x3, 0x2, 0x2, 0x2, 
    0x40e, 0x40f, 0x3, 0x2, 0x2, 0x2, 0x40f, 0x410, 0x7, 0xc, 0x2, 0x2, 
    0x410, 0x41d, 0x3, 0x2, 0x2, 0x2, 0x411, 0x419, 0x7, 0xc1, 0x2, 0x2, 
    0x412, 0x413, 0x7, 0xb, 0x2, 0x2, 0x413, 0x415, 0x5, 0xd8, 0x6d, 0x2, 
    0x414, 0x416, 0x9, 0xd, 0x2, 0x2, 0x415, 0x414, 0x3, 0x2, 0x2, 0x2, 
    0x415, 0x416, 0x3, 0x2, 0x2, 0x2, 0x416, 0x417, 0x3, 0x2, 0x2, 0x2, 
    0x417, 0x418, 0x7, 0xc, 0x2, 0x2, 0x418, 0x41a, 0x3, 0x2, 0x2, 0x2, 
    0x419, 0x412, 0x3, 0x2, 0x2, 0x2, 0x419, 0x41a, 0x3, 0x2, 0x2, 0x2, 
    0x41a, 0x41d, 0x3, 0x2, 0x2, 0x2, 0x41b, 0x41d, 0x7, 0x21, 0x2, 0x2, 
    0x41c, 0x3dd, 0x3, 0x2, 0x2, 0x2, 0x41c, 0x3de, 0x3, 0x2, 0x2, 0x2, 
    0x41c, 0x3e8, 0x3, 0x2, 0x2, 0x2, 0x41c, 0x3f9, 0x3, 0x2, 0x2, 0x2, 
    0x41c, 0x403, 0x3, 0x2, 0x2, 0x2, 0x41c, 0x404, 0x3, 0x2, 0x2, 0x2, 
    0x41c, 0x405, 0x3, 0x2, 0x2, 0x2, 0x41c, 0x406, 0x3, 0x2, 0x2, 0x2, 
    0x41c, 0x407, 0x3, 0x2, 0x2, 0x2, 0x41c, 0x408, 0x3, 0x2, 0x2, 0x2, 
    0x41c, 0x409, 0x3, 0x2, 0x2, 0x2, 0x41c, 0x411, 0x3, 0x2, 0x2, 0x2, 
    0x41c, 0x41b, 0x3, 0x2, 0x2, 0x2, 0x41d, 0x73, 0x3, 0x2, 0x2, 0x2, 0x41e, 
    0x421, 0x7, 0x67, 0x2, 0x2, 0x41f, 0x422, 0x5, 0x76, 0x3c, 0x2, 0x420, 
    0x422, 0x5, 0x78, 0x3d, 0x2, 0x421, 0x41f, 0x3, 0x2, 0x2, 0x2, 0x421, 
    0x420, 0x3, 0x2, 0x2, 0x2, 0x422, 0x75, 0x3, 0x2, 0x2, 0x2, 0x423, 0x428, 
    0x7, 0xcc, 0x2, 0x2, 0x424, 0x425, 0x7, 0xb, 0x2, 0x2, 0x425, 0x426, 
    0x5, 0xd4, 0x6b, 0x2, 0x426, 0x427, 0x7, 0xc, 0x2, 0x2, 0x427, 0x429, 
    0x3, 0x2, 0x2, 0x2, 0x428, 0x424, 0x3, 0x2, 0x2, 0x2, 0x428, 0x429, 
    0x3, 0x2, 0x2, 0x2, 0x429, 0x43b, 0x3, 0x2, 0x2, 0x2, 0x42a, 0x42f, 
    0x7, 0xcc, 0x2, 0x2, 0x42b, 0x42c, 0x7, 0xb, 0x2, 0x2, 0x42c, 0x42d, 
    0x5, 0xd4, 0x6b, 0x2, 0x42d, 0x42e, 0x7, 0xc, 0x2, 0x2, 0x42e, 0x430, 
    0x3, 0x2, 0x2, 0x2, 0x42f, 0x42b, 0x3, 0x2, 0x2, 0x2, 0x42f, 0x430, 
    0x3, 0x2, 0x2, 0x2, 0x430, 0x431, 0x3, 0x2, 0x2, 0x2, 0x431, 0x432, 
    0x7, 0xb1, 0x2, 0x2, 0x432, 0x43b, 0x7, 0x7c, 0x2, 0x2, 0x433, 0x438, 
    0x7, 0x7c, 0x2, 0x2, 0x434, 0x435, 0x7, 0xb, 0x2, 0x2, 0x435, 0x436, 
    0x5, 0xd4, 0x6b, 0x2, 0x436, 0x437, 0x7, 0xc, 0x2, 0x2, 0x437, 0x439, 
    0x3, 0x2, 0x2, 0x2, 0x438, 0x434, 0x3, 0x2, 0x2, 0x2, 0x438, 0x439, 
    0x3, 0x2, 0x2, 0x2, 0x439, 0x43b, 0x3, 0x2, 0x2, 0x2, 0x43a, 0x423, 
    0x3, 0x2, 0x2, 0x2, 0x43a, 0x42a, 0x3, 0x2, 0x2, 0x2, 0x43a, 0x433, 
    0x3, 0x2, 0x2, 0x2, 0x43b, 0x77, 0x3, 0x2, 0x2, 0x2, 0x43c, 0x441, 0x7, 
    0x3e, 0x2, 0x2, 0x43d, 0x43e, 0x7, 0xb, 0x2, 0x2, 0x43e, 0x43f, 0x5, 
    0xd4, 0x6b, 0x2, 0x43f, 0x440, 0x7, 0xc, 0x2, 0x2, 0x440, 0x442, 0x3, 
    0x2, 0x2, 0x2, 0x441, 0x43d, 0x3, 0x2, 0x2, 0x2, 0x441, 0x442, 0x3, 
    0x2, 0x2, 0x2, 0x442, 0x491, 0x3, 0x2, 0x2, 0x2, 0x443, 0x448, 0x7, 
    0x3e, 0x2, 0x2, 0x444, 0x445, 0x7, 0xb, 0x2, 0x2, 0x445, 0x446, 0x5, 
    0xd4, 0x6b, 0x2, 0x446, 0x447, 0x7, 0xc, 0x2, 0x2, 0x447, 0x449, 0x3, 
    0x2, 0x2, 0x2, 0x448, 0x444, 0x3, 0x2, 0x2, 0x2, 0x448, 0x449, 0x3, 
    0x2, 0x2, 0x2, 0x449, 0x44a, 0x3, 0x2, 0x2, 0x2, 0x44a, 0x454, 0x7, 
    0xb1, 0x2, 0x2, 0x44b, 0x455, 0x7, 0x5d, 0x2, 0x2, 0x44c, 0x455, 0x7, 
    0x79, 0x2, 0x2, 0x44d, 0x452, 0x7, 0x9b, 0x2, 0x2, 0x44e, 0x44f, 0x7, 
    0xb, 0x2, 0x2, 0x44f, 0x450, 0x5, 0xd6, 0x6c, 0x2, 0x450, 0x451, 0x7, 
    0xc, 0x2, 0x2, 0x451, 0x453, 0x3, 0x2, 0x2, 0x2, 0x452, 0x44e, 0x3, 
    0x2, 0x2, 0x2, 0x452, 0x453, 0x3, 0x2, 0x2, 0x2, 0x453, 0x455, 0x3, 
    0x2, 0x2, 0x2, 0x454, 0x44b, 0x3, 0x2, 0x2, 0x2, 0x454, 0x44c, 0x3, 
    0x2, 0x2, 0x2, 0x454, 0x44d, 0x3, 0x2, 0x2, 0x2, 0x455, 0x491, 0x3, 
    0x2, 0x2, 0x2, 0x456, 0x45b, 0x7, 0x5d, 0x2, 0x2, 0x457, 0x458, 0x7, 
    0xb, 0x2, 0x2, 0x458, 0x459, 0x5, 0xd4, 0x6b, 0x2, 0x459, 0x45a, 0x7, 
    0xc, 0x2, 0x2, 0x45a, 0x45c, 0x3, 0x2, 0x2, 0x2, 0x45b, 0x457, 0x3, 
    0x2, 0x2, 0x2, 0x45b, 0x45c, 0x3, 0x2, 0x2, 0x2, 0x45c, 0x45d, 0x3, 
    0x2, 0x2, 0x2, 0x45d, 0x466, 0x7, 0xb1, 0x2, 0x2, 0x45e, 0x467, 0x7, 
    0x79, 0x2, 0x2, 0x45f, 0x464, 0x7, 0x9b, 0x2, 0x2, 0x460, 0x461, 0x7, 
    0xb, 0x2, 0x2, 0x461, 0x462, 0x5, 0xd6, 0x6c, 0x2, 0x462, 0x463, 0x7, 
    0xc, 0x2, 0x2, 0x463, 0x465, 0x3, 0x2, 0x2, 0x2, 0x464, 0x460, 0x3, 
    0x2, 0x2, 0x2, 0x464, 0x465, 0x3, 0x2, 0x2, 0x2, 0x465, 0x467, 0x3, 
    0x2, 0x2, 0x2, 0x466, 0x45e, 0x3, 0x2, 0x2, 0x2, 0x466, 0x45f, 0x3, 
    0x2, 0x2, 0x2, 0x467, 0x491, 0x3, 0x2, 0x2, 0x2, 0x468, 0x46d, 0x7, 
    0x79, 0x2, 0x2, 0x469, 0x46a, 0x7, 0xb, 0x2, 0x2, 0x46a, 0x46b, 0x5, 
    0xd4, 0x6b, 0x2, 0x46b, 0x46c, 0x7, 0xc, 0x2, 0x2, 0x46c, 0x46e, 0x3, 
    0x2, 0x2, 0x2, 0x46d, 0x469, 0x3, 0x2, 0x2, 0x2, 0x46d, 0x46e, 0x3, 
    0x2, 0x2, 0x2, 0x46e, 0x46f, 0x3, 0x2, 0x2, 0x2, 0x46f, 0x470, 0x7, 
    0xb1, 0x2, 0x2, 0x470, 0x475, 0x7, 0x9b, 0x2, 0x2, 0x471, 0x472, 0x7, 
    0xb, 0x2, 0x2, 0x472, 0x473, 0x5, 0xd6, 0x6c, 0x2, 0x473, 0x474, 0x7, 
    0xc, 0x2, 0x2, 0x474, 0x476, 0x3, 0x2, 0x2, 0x2, 0x475, 0x471, 0x3, 
    0x2, 0x2, 0x2, 0x475, 0x476, 0x3, 0x2, 0x2, 0x2, 0x476, 0x491, 0x3, 
    0x2, 0x2, 0x2, 0x477, 0x47c, 0x7, 0x5d, 0x2, 0x2, 0x478, 0x479, 0x7, 
    0xb, 0x2, 0x2, 0x479, 0x47a, 0x5, 0xd4, 0x6b, 0x2, 0x47a, 0x47b, 0x7, 
    0xc, 0x2, 0x2, 0x47b, 0x47d, 0x3, 0x2, 0x2, 0x2, 0x47c, 0x478, 0x3, 
    0x2, 0x2, 0x2, 0x47c, 0x47d, 0x3, 0x2, 0x2, 0x2, 0x47d, 0x491, 0x3, 
    0x2, 0x2, 0x2, 0x47e, 0x483, 0x7, 0x79, 0x2, 0x2, 0x47f, 0x480, 0x7, 
    0xb, 0x2, 0x2, 0x480, 0x481, 0x5, 0xd4, 0x6b, 0x2, 0x481, 0x482, 0x7, 
    0xc, 0x2, 0x2, 0x482, 0x484, 0x3, 0x2, 0x2, 0x2, 0x483, 0x47f, 0x3, 
    0x2, 0x2, 0x2, 0x483, 0x484, 0x3, 0x2, 0x2, 0x2, 0x484, 0x491, 0x3, 
    0x2, 0x2, 0x2, 0x485, 0x48e, 0x7, 0x9b, 0x2, 0x2, 0x486, 0x487, 0x7, 
    0xb, 0x2, 0x2, 0x487, 0x48a, 0x5, 0xd4, 0x6b, 0x2, 0x488, 0x489, 0x7, 
    0x8, 0x2, 0x2, 0x489, 0x48b, 0x5, 0xd6, 0x6c, 0x2, 0x48a, 0x488, 0x3, 
    0x2, 0x2, 0x2, 0x48a, 0x48b, 0x3, 0x2, 0x2, 0x2, 0x48b, 0x48c, 0x3, 
    0x2, 0x2, 0x2, 0x48c, 0x48d, 0x7, 0xc, 0x2, 0x2, 0x48d, 0x48f, 0x3, 
    0x2, 0x2, 0x2, 0x48e, 0x486, 0x3, 0x2, 0x2, 0x2, 0x48e, 0x48f, 0x3, 
    0x2, 0x2, 0x2, 0x48f, 0x491, 0x3, 0x2, 0x2, 0x2, 0x490, 0x43c, 0x3, 
    0x2, 0x2, 0x2, 0x490, 0x443, 0x3, 0x2, 0x2, 0x2, 0x490, 0x456, 0x3, 
    0x2, 0x2, 0x2, 0x490, 0x468, 0x3, 0x2, 0x2, 0x2, 0x490, 0x477, 0x3, 
    0x2, 0x2, 0x2, 0x490, 0x47e, 0x3, 0x2, 0x2, 0x2, 0x490, 0x485, 0x3, 
    0x2, 0x2, 0x2, 0x491, 0x79, 0x3, 0x2, 0x2, 0x2, 0x492, 0x493, 0x9, 0xe, 
    0x2, 0x2, 0x493, 0x494, 0x7, 0xb, 0x2, 0x2, 0x494, 0x496, 0x5, 0xd8, 
    0x6d, 0x2, 0x495, 0x497, 0x9, 0xf, 0x2, 0x2, 0x496, 0x495, 0x3, 0x2, 
    0x2, 0x2, 0x496, 0x497, 0x3, 0x2, 0x2, 0x2, 0x497, 0x498, 0x3, 0x2, 
    0x2, 0x2, 0x498, 0x49b, 0x7, 0xc, 0x2, 0x2, 0x499, 0x49a, 0x7, 0x31, 
    0x2, 0x2, 0x49a, 0x49c, 0x5, 0xc2, 0x62, 0x2, 0x49b, 0x499, 0x3, 0x2, 
    0x2, 0x2, 0x49b, 0x49c, 0x3, 0x2, 0x2, 0x2, 0x49c, 0x7b, 0x3, 0x2, 0x2, 
    0x2, 0x49d, 0x49f, 0x9, 0xe, 0x2, 0x2, 0x49e, 0x4a0, 0x7, 0xc3, 0x2, 
    0x2, 0x49f, 0x49e, 0x3, 0x2, 0x2, 0x2, 0x49f, 0x4a0, 0x3, 0x2, 0x2, 
    0x2, 0x4a0, 0x4a3, 0x3, 0x2, 0x2, 0x2, 0x4a1, 0x4a3, 0x7, 0xc2, 0x2, 
    0x2, 0x4a2, 0x49d, 0x3, 0x2, 0x2, 0x2, 0x4a2, 0x4a1, 0x3, 0x2, 0x2, 
    0x2, 0x4a3, 0x4ab, 0x3, 0x2, 0x2, 0x2, 0x4a4, 0x4a5, 0x7, 0xb, 0x2, 
    0x2, 0x4a5, 0x4a7, 0x5, 0xd8, 0x6d, 0x2, 0x4a6, 0x4a8, 0x9, 0xf, 0x2, 
    0x2, 0x4a7, 0x4a6, 0x3, 0x2, 0x2, 0x2, 0x4a7, 0x4a8, 0x3, 0x2, 0x2, 
    0x2, 0x4a8, 0x4a9, 0x3, 0x2, 0x2, 0x2, 0x4a9, 0x4aa, 0x7, 0xc, 0x2, 
    0x2, 0x4aa, 0x4ac, 0x3, 0x2, 0x2, 0x2, 0x4ab, 0x4a4, 0x3, 0x2, 0x2, 
    0x2, 0x4ab, 0x4ac, 0x3, 0x2, 0x2, 0x2, 0x4ac, 0x4af, 0x3, 0x2, 0x2, 
    0x2, 0x4ad, 0x4af, 0x7, 0xad, 0x2, 0x2, 0x4ae, 0x4a2, 0x3, 0x2, 0x2, 
    0x2, 0x4ae, 0x4ad, 0x3, 0x2, 0x2, 0x2, 0x4af, 0x4b2, 0x3, 0x2, 0x2, 
    0x2, 0x4b0, 0x4b1, 0x7, 0x31, 0x2, 0x2, 0x4b1, 0x4b3, 0x5, 0xc2, 0x62, 
    0x2, 0x4b2, 0x4b0, 0x3, 0x2, 0x2, 0x2, 0x4b2, 0x4b3, 0x3, 0x2, 0x2, 
    0x2, 0x4b3, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x4b4, 0x4b5, 0x5, 0x80, 0x41, 
    0x2, 0x4b5, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x4b6, 0x4ba, 0x7, 0xb, 0x2, 0x2, 
    0x4b7, 0x4b9, 0x5, 0x20, 0x11, 0x2, 0x4b8, 0x4b7, 0x3, 0x2, 0x2, 0x2, 
    0x4b9, 0x4bc, 0x3, 0x2, 0x2, 0x2, 0x4ba, 0x4b8, 0x3, 0x2, 0x2, 0x2, 
    0x4ba, 0x4bb, 0x3, 0x2, 0x2, 0x2, 0x4bb, 0x4bd, 0x3, 0x2, 0x2, 0x2, 
    0x4bc, 0x4ba, 0x3, 0x2, 0x2, 0x2, 0x4bd, 0x4be, 0x5, 0x80, 0x41, 0x2, 
    0x4be, 0x4bf, 0x7, 0xc, 0x2, 0x2, 0x4bf, 0x4cc, 0x3, 0x2, 0x2, 0x2, 
    0x4c0, 0x4c2, 0x5, 0x20, 0x11, 0x2, 0x4c1, 0x4c0, 0x3, 0x2, 0x2, 0x2, 
    0x4c2, 0x4c5, 0x3, 0x2, 0x2, 0x2, 0x4c3, 0x4c1, 0x3, 0x2, 0x2, 0x2, 
    0x4c3, 0x4c4, 0x3, 0x2, 0x2, 0x2, 0x4c4, 0x4c9, 0x3, 0x2, 0x2, 0x2, 
    0x4c5, 0x4c3, 0x3, 0x2, 0x2, 0x2, 0x4c6, 0x4ca, 0x5, 0x24, 0x13, 0x2, 
    0x4c7, 0x4ca, 0x5, 0x38, 0x1d, 0x2, 0x4c8, 0x4ca, 0x5, 0x28, 0x15, 0x2, 
    0x4c9, 0x4c6, 0x3, 0x2, 0x2, 0x2, 0x4c9, 0x4c7, 0x3, 0x2, 0x2, 0x2, 
    0x4c9, 0x4c8, 0x3, 0x2, 0x2, 0x2, 0x4ca, 0x4cc, 0x3, 0x2, 0x2, 0x2, 
    0x4cb, 0x4b6, 0x3, 0x2, 0x2, 0x2, 0x4cb, 0x4c3, 0x3, 0x2, 0x2, 0x2, 
    0x4cc, 0x81, 0x3, 0x2, 0x2, 0x2, 0x4cd, 0x4d2, 0x5, 0x84, 0x43, 0x2, 
    0x4ce, 0x4cf, 0x7, 0x8, 0x2, 0x2, 0x4cf, 0x4d1, 0x5, 0x84, 0x43, 0x2, 
    0x4d0, 0x4ce, 0x3, 0x2, 0x2, 0x2, 0x4d1, 0x4d4, 0x3, 0x2, 0x2, 0x2, 
    0x4d2, 0x4d0, 0x3, 0x2, 0x2, 0x2, 0x4d2, 0x4d3, 0x3, 0x2, 0x2, 0x2, 
    0x4d3, 0x83, 0x3, 0x2, 0x2, 0x2, 0x4d4, 0x4d2, 0x3, 0x2, 0x2, 0x2, 0x4d5, 
    0x4da, 0x5, 0xa8, 0x55, 0x2, 0x4d6, 0x4d8, 0x7, 0x1a, 0x2, 0x2, 0x4d7, 
    0x4d6, 0x3, 0x2, 0x2, 0x2, 0x4d7, 0x4d8, 0x3, 0x2, 0x2, 0x2, 0x4d8, 
    0x4d9, 0x3, 0x2, 0x2, 0x2, 0x4d9, 0x4db, 0x5, 0xd0, 0x69, 0x2, 0x4da, 
    0x4d7, 0x3, 0x2, 0x2, 0x2, 0x4da, 0x4db, 0x3, 0x2, 0x2, 0x2, 0x4db, 
    0x4de, 0x3, 0x2, 0x2, 0x2, 0x4dc, 0x4de, 0x5, 0xae, 0x58, 0x2, 0x4dd, 
    0x4d5, 0x3, 0x2, 0x2, 0x2, 0x4dd, 0x4dc, 0x3, 0x2, 0x2, 0x2, 0x4de, 
    0x85, 0x3, 0x2, 0x2, 0x2, 0x4df, 0x4e4, 0x5, 0x88, 0x45, 0x2, 0x4e0, 
    0x4e1, 0x7, 0x8, 0x2, 0x2, 0x4e1, 0x4e3, 0x5, 0x88, 0x45, 0x2, 0x4e2, 
    0x4e0, 0x3, 0x2, 0x2, 0x2, 0x4e3, 0x4e6, 0x3, 0x2, 0x2, 0x2, 0x4e4, 
    0x4e2, 0x3, 0x2, 0x2, 0x2, 0x4e4, 0x4e5, 0x3, 0x2, 0x2, 0x2, 0x4e5, 
    0x87, 0x3, 0x2, 0x2, 0x2, 0x4e6, 0x4e4, 0x3, 0x2, 0x2, 0x2, 0x4e7, 0x4e8, 
    0x5, 0xa8, 0x55, 0x2, 0x4e8, 0x89, 0x3, 0x2, 0x2, 0x2, 0x4e9, 0x4ee, 
    0x5, 0x8c, 0x47, 0x2, 0x4ea, 0x4eb, 0x7, 0x8, 0x2, 0x2, 0x4eb, 0x4ed, 
    0x5, 0x8c, 0x47, 0x2, 0x4ec, 0x4ea, 0x3, 0x2, 0x2, 0x2, 0x4ed, 0x4f0, 
    0x3, 0x2, 0x2, 0x2, 0x4ee, 0x4ec, 0x3, 0x2, 0x2, 0x2, 0x4ee, 0x4ef, 
    0x3, 0x2, 0x2, 0x2, 0x4ef, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x4f0, 0x4ee, 0x3, 
    0x2, 0x2, 0x2, 0x4f1, 0x4f3, 0x5, 0xa8, 0x55, 0x2, 0x4f2, 0x4f4, 0x9, 
    0x10, 0x2, 0x2, 0x4f3, 0x4f2, 0x3, 0x2, 0x2, 0x2, 0x4f3, 0x4f4, 0x3, 
    0x2, 0x2, 0x2, 0x4f4, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x4f5, 0x4fa, 0x5, 0x90, 
    0x49, 0x2, 0x4f6, 0x4f7, 0x7, 0x8, 0x2, 0x2, 0x4f7, 0x4f9, 0x5, 0x90, 
    0x49, 0x2, 0x4f8, 0x4f6, 0x3, 0x2, 0x2, 0x2, 0x4f9, 0x4fc, 0x3, 0x2, 
    0x2, 0x2, 0x4fa, 0x4f8, 0x3, 0x2, 0x2, 0x2, 0x4fa, 0x4fb, 0x3, 0x2, 
    0x2, 0x2, 0x4fb, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x4fc, 0x4fa, 0x3, 0x2, 0x2, 
    0x2, 0x4fd, 0x4fe, 0x5, 0xd0, 0x69, 0x2, 0x4fe, 0x91, 0x3, 0x2, 0x2, 
    0x2, 0x4ff, 0x504, 0x5, 0x94, 0x4b, 0x2, 0x500, 0x501, 0x7, 0x8, 0x2, 
    0x2, 0x501, 0x503, 0x5, 0x94, 0x4b, 0x2, 0x502, 0x500, 0x3, 0x2, 0x2, 
    0x2, 0x503, 0x506, 0x3, 0x2, 0x2, 0x2, 0x504, 0x502, 0x3, 0x2, 0x2, 
    0x2, 0x504, 0x505, 0x3, 0x2, 0x2, 0x2, 0x505, 0x93, 0x3, 0x2, 0x2, 0x2, 
    0x506, 0x504, 0x3, 0x2, 0x2, 0x2, 0x507, 0x508, 0x5, 0xd0, 0x69, 0x2, 
    0x508, 0x95, 0x3, 0x2, 0x2, 0x2, 0x509, 0x50a, 0x7, 0xb, 0x2, 0x2, 0x50a, 
    0x50b, 0x5, 0x98, 0x4d, 0x2, 0x50b, 0x50c, 0x7, 0xc, 0x2, 0x2, 0x50c, 
    0x97, 0x3, 0x2, 0x2, 0x2, 0x50d, 0x512, 0x5, 0x9a, 0x4e, 0x2, 0x50e, 
    0x50f, 0x7, 0x8, 0x2, 0x2, 0x50f, 0x511, 0x5, 0x9a, 0x4e, 0x2, 0x510, 
    0x50e, 0x3, 0x2, 0x2, 0x2, 0x511, 0x514, 0x3, 0x2, 0x2, 0x2, 0x512, 
    0x510, 0x3, 0x2, 0x2, 0x2, 0x512, 0x513, 0x3, 0x2, 0x2, 0x2, 0x513, 
    0x99, 0x3, 0x2, 0x2, 0x2, 0x514, 0x512, 0x3, 0x2, 0x2, 0x2, 0x515, 0x516, 
    0x5, 0xd0, 0x69, 0x2, 0x516, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x517, 0x51c, 
    0x5, 0x9e, 0x50, 0x2, 0x518, 0x519, 0x7, 0x8, 0x2, 0x2, 0x519, 0x51b, 
    0x5, 0x9e, 0x50, 0x2, 0x51a, 0x518, 0x3, 0x2, 0x2, 0x2, 0x51b, 0x51e, 
    0x3, 0x2, 0x2, 0x2, 0x51c, 0x51a, 0x3, 0x2, 0x2, 0x2, 0x51c, 0x51d, 
    0x3, 0x2, 0x2, 0x2, 0x51d, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x51e, 0x51c, 0x3, 
    0x2, 0x2, 0x2, 0x51f, 0x522, 0x5, 0xc6, 0x64, 0x2, 0x520, 0x521, 0x7, 
    0x6, 0x2, 0x2, 0x521, 0x523, 0x5, 0xcc, 0x67, 0x2, 0x522, 0x520, 0x3, 
    0x2, 0x2, 0x2, 0x522, 0x523, 0x3, 0x2, 0x2, 0x2, 0x523, 0x9f, 0x3, 0x2, 
    0x2, 0x2, 0x524, 0x529, 0x5, 0xa8, 0x55, 0x2, 0x525, 0x526, 0x7, 0x8, 
    0x2, 0x2, 0x526, 0x528, 0x5, 0xa8, 0x55, 0x2, 0x527, 0x525, 0x3, 0x2, 
    0x2, 0x2, 0x528, 0x52b, 0x3, 0x2, 0x2, 0x2, 0x529, 0x527, 0x3, 0x2, 
    0x2, 0x2, 0x529, 0x52a, 0x3, 0x2, 0x2, 0x2, 0x52a, 0xa1, 0x3, 0x2, 0x2, 
    0x2, 0x52b, 0x529, 0x3, 0x2, 0x2, 0x2, 0x52c, 0x52d, 0x5, 0xa8, 0x55, 
    0x2, 0x52d, 0xa3, 0x3, 0x2, 0x2, 0x2, 0x52e, 0x52f, 0x5, 0xa8, 0x55, 
    0x2, 0x52f, 0xa5, 0x3, 0x2, 0x2, 0x2, 0x530, 0x531, 0x5, 0xa8, 0x55, 
    0x2, 0x531, 0xa7, 0x3, 0x2, 0x2, 0x2, 0x532, 0x533, 0x8, 0x55, 0x1, 
    0x2, 0x533, 0x63a, 0x5, 0xda, 0x6e, 0x2, 0x534, 0x63a, 0x5, 0xaa, 0x56, 
    0x2, 0x535, 0x63a, 0x5, 0xac, 0x57, 0x2, 0x536, 0x537, 0x7, 0xb, 0x2, 
    0x2, 0x537, 0x538, 0x5, 0x70, 0x39, 0x2, 0x538, 0x539, 0x7, 0xc, 0x2, 
    0x2, 0x539, 0x53a, 0x5, 0xa8, 0x55, 0x2a, 0x53a, 0x63a, 0x3, 0x2, 0x2, 
    0x2, 0x53b, 0x53c, 0x9, 0x11, 0x2, 0x2, 0x53c, 0x63a, 0x5, 0xa8, 0x55, 
    0x29, 0x53d, 0x53e, 0x7, 0x2b, 0x2, 0x2, 0x53e, 0x53f, 0x7, 0xb, 0x2, 
    0x2, 0x53f, 0x540, 0x5, 0xa8, 0x55, 0x2, 0x540, 0x541, 0x9, 0x12, 0x2, 
    0x2, 0x541, 0x542, 0x5, 0x70, 0x39, 0x2, 0x542, 0x543, 0x7, 0xc, 0x2, 
    0x2, 0x543, 0x63a, 0x3, 0x2, 0x2, 0x2, 0x544, 0x545, 0x7, 0xba, 0x2, 
    0x2, 0x545, 0x546, 0x7, 0xb, 0x2, 0x2, 0x546, 0x547, 0x5, 0xa8, 0x55, 
    0x2, 0x547, 0x548, 0x7, 0xc, 0x2, 0x2, 0x548, 0x63a, 0x3, 0x2, 0x2, 
    0x2, 0x549, 0x55d, 0x7, 0x2a, 0x2, 0x2, 0x54a, 0x550, 0x5, 0xa8, 0x55, 
    0x2, 0x54b, 0x54c, 0x7, 0xc6, 0x2, 0x2, 0x54c, 0x54d, 0x5, 0xa8, 0x55, 
    0x2, 0x54d, 0x54e, 0x7, 0xae, 0x2, 0x2, 0x54e, 0x54f, 0x5, 0xa8, 0x55, 
    0x2, 0x54f, 0x551, 0x3, 0x2, 0x2, 0x2, 0x550, 0x54b, 0x3, 0x2, 0x2, 
    0x2, 0x551, 0x552, 0x3, 0x2, 0x2, 0x2, 0x552, 0x550, 0x3, 0x2, 0x2, 
    0x2, 0x552, 0x553, 0x3, 0x2, 0x2, 0x2, 0x553, 0x55e, 0x3, 0x2, 0x2, 
    0x2, 0x554, 0x555, 0x7, 0xc6, 0x2, 0x2, 0x555, 0x556, 0x5, 0xa6, 0x54, 
    0x2, 0x556, 0x557, 0x7, 0xae, 0x2, 0x2, 0x557, 0x558, 0x5, 0xa8, 0x55, 
    0x2, 0x558, 0x55a, 0x3, 0x2, 0x2, 0x2, 0x559, 0x554, 0x3, 0x2, 0x2, 
    0x2, 0x55a, 0x55b, 0x3, 0x2, 0x2, 0x2, 0x55b, 0x559, 0x3, 0x2, 0x2, 
    0x2, 0x55b, 0x55c, 0x3, 0x2, 0x2, 0x2, 0x55c, 0x55e, 0x3, 0x2, 0x2, 
    0x2, 0x55d, 0x54a, 0x3, 0x2, 0x2, 0x2, 0x55d, 0x559, 0x3, 0x2, 0x2, 
    0x2, 0x55e, 0x561, 0x3, 0x2, 0x2, 0x2, 0x55f, 0x560, 0x7, 0x49, 0x2, 
    0x2, 0x560, 0x562, 0x5, 0xa8, 0x55, 0x2, 0x561, 0x55f, 0x3, 0x2, 0x2, 
    0x2, 0x561, 0x562, 0x3, 0x2, 0x2, 0x2, 0x562, 0x563, 0x3, 0x2, 0x2, 
    0x2, 0x563, 0x564, 0x7, 0x4a, 0x2, 0x2, 0x564, 0x63a, 0x3, 0x2, 0x2, 
    0x2, 0x565, 0x566, 0x7, 0x5f, 0x2, 0x2, 0x566, 0x567, 0x7, 0xb, 0x2, 
    0x2, 0x567, 0x568, 0x5, 0xa6, 0x54, 0x2, 0x568, 0x569, 0x7, 0x8, 0x2, 
    0x2, 0x569, 0x56a, 0x5, 0xa8, 0x55, 0x2, 0x56a, 0x56b, 0x7, 0x8, 0x2, 
    0x2, 0x56b, 0x56c, 0x5, 0xa8, 0x55, 0x2, 0x56c, 0x56d, 0x7, 0xc, 0x2, 
    0x2, 0x56d, 0x63a, 0x3, 0x2, 0x2, 0x2, 0x56e, 0x56f, 0x7, 0x60, 0x2, 
    0x2, 0x56f, 0x570, 0x7, 0xb, 0x2, 0x2, 0x570, 0x571, 0x5, 0xa8, 0x55, 
    0x2, 0x571, 0x572, 0x7, 0x8, 0x2, 0x2, 0x572, 0x573, 0x5, 0xa8, 0x55, 
    0x2, 0x573, 0x574, 0x7, 0xc, 0x2, 0x2, 0x574, 0x63a, 0x3, 0x2, 0x2, 
    0x2, 0x575, 0x576, 0x7, 0x84, 0x2, 0x2, 0x576, 0x577, 0x7, 0xb, 0x2, 
    0x2, 0x577, 0x578, 0x5, 0xa8, 0x55, 0x2, 0x578, 0x579, 0x7, 0x8, 0x2, 
    0x2, 0x579, 0x57a, 0x5, 0xa8, 0x55, 0x2, 0x57a, 0x57b, 0x7, 0xc, 0x2, 
    0x2, 0x57b, 0x63a, 0x3, 0x2, 0x2, 0x2, 0x57c, 0x57d, 0x7, 0x30, 0x2, 
    0x2, 0x57d, 0x586, 0x7, 0xb, 0x2, 0x2, 0x57e, 0x583, 0x5, 0xa8, 0x55, 
    0x2, 0x57f, 0x580, 0x7, 0x8, 0x2, 0x2, 0x580, 0x582, 0x5, 0xa8, 0x55, 
    0x2, 0x581, 0x57f, 0x3, 0x2, 0x2, 0x2, 0x582, 0x585, 0x3, 0x2, 0x2, 
    0x2, 0x583, 0x581, 0x3, 0x2, 0x2, 0x2, 0x583, 0x584, 0x3, 0x2, 0x2, 
    0x2, 0x584, 0x587, 0x3, 0x2, 0x2, 0x2, 0x585, 0x583, 0x3, 0x2, 0x2, 
    0x2, 0x586, 0x57e, 0x3, 0x2, 0x2, 0x2, 0x586, 0x587, 0x3, 0x2, 0x2, 
    0x2, 0x587, 0x588, 0x3, 0x2, 0x2, 0x2, 0x588, 0x63a, 0x7, 0xc, 0x2, 
    0x2, 0x589, 0x58a, 0x7, 0x41, 0x2, 0x2, 0x58a, 0x593, 0x7, 0xb, 0x2, 
    0x2, 0x58b, 0x590, 0x5, 0xa8, 0x55, 0x2, 0x58c, 0x58d, 0x7, 0x8, 0x2, 
    0x2, 0x58d, 0x58f, 0x5, 0xa8, 0x55, 0x2, 0x58e, 0x58c, 0x3, 0x2, 0x2, 
    0x2, 0x58f, 0x592, 0x3, 0x2, 0x2, 0x2, 0x590, 0x58e, 0x3, 0x2, 0x2, 
    0x2, 0x590, 0x591, 0x3, 0x2, 0x2, 0x2, 0x591, 0x594, 0x3, 0x2, 0x2, 
    0x2, 0x592, 0x590, 0x3, 0x2, 0x2, 0x2, 0x593, 0x58b, 0x3, 0x2, 0x2, 
    0x2, 0x593, 0x594, 0x3, 0x2, 0x2, 0x2, 0x594, 0x595, 0x3, 0x2, 0x2, 
    0x2, 0x595, 0x63a, 0x7, 0xc, 0x2, 0x2, 0x596, 0x597, 0x7, 0x32, 0x2, 
    0x2, 0x597, 0x5a0, 0x7, 0xb, 0x2, 0x2, 0x598, 0x59d, 0x5, 0xa8, 0x55, 
    0x2, 0x599, 0x59a, 0x7, 0x8, 0x2, 0x2, 0x59a, 0x59c, 0x5, 0xa8, 0x55, 
    0x2, 0x59b, 0x599, 0x3, 0x2, 0x2, 0x2, 0x59c, 0x59f, 0x3, 0x2, 0x2, 
    0x2, 0x59d, 0x59b, 0x3, 0x2, 0x2, 0x2, 0x59d, 0x59e, 0x3, 0x2, 0x2, 
    0x2, 0x59e, 0x5a1, 0x3, 0x2, 0x2, 0x2, 0x59f, 0x59d, 0x3, 0x2, 0x2, 
    0x2, 0x5a0, 0x598, 0x3, 0x2, 0x2, 0x2, 0x5a0, 0x5a1, 0x3, 0x2, 0x2, 
    0x2, 0x5a1, 0x5a2, 0x3, 0x2, 0x2, 0x2, 0x5a2, 0x63a, 0x7, 0xc, 0x2, 
    0x2, 0x5a3, 0x5a4, 0x7, 0x8b, 0x2, 0x2, 0x5a4, 0x5a5, 0x7, 0xb, 0x2, 
    0x2, 0x5a5, 0x5a6, 0x5, 0xa8, 0x55, 0x2, 0x5a6, 0x5a7, 0x9, 0x13, 0x2, 
    0x2, 0x5a7, 0x5a8, 0x5, 0xa8, 0x55, 0x2, 0x5a8, 0x5a9, 0x9, 0x14, 0x2, 
    0x2, 0x5a9, 0x5ac, 0x5, 0xa8, 0x55, 0x2, 0x5aa, 0x5ab, 0x9, 0x15, 0x2, 
    0x2, 0x5ab, 0x5ad, 0x5, 0xa8, 0x55, 0x2, 0x5ac, 0x5aa, 0x3, 0x2, 0x2, 
    0x2, 0x5ac, 0x5ad, 0x3, 0x2, 0x2, 0x2, 0x5ad, 0x5ae, 0x3, 0x2, 0x2, 
    0x2, 0x5ae, 0x5af, 0x7, 0xc, 0x2, 0x2, 0x5af, 0x63a, 0x3, 0x2, 0x2, 
    0x2, 0x5b0, 0x5b1, 0x7, 0x8f, 0x2, 0x2, 0x5b1, 0x5b2, 0x7, 0xb, 0x2, 
    0x2, 0x5b2, 0x5b3, 0x5, 0xa8, 0x55, 0x2, 0x5b3, 0x5b4, 0x9, 0x16, 0x2, 
    0x2, 0x5b4, 0x5b5, 0x5, 0xa8, 0x55, 0x2, 0x5b5, 0x5b6, 0x7, 0xc, 0x2, 
    0x2, 0x5b6, 0x63a, 0x3, 0x2, 0x2, 0x2, 0x5b7, 0x5b8, 0x9, 0x17, 0x2, 
    0x2, 0x5b8, 0x5c7, 0x7, 0xb, 0x2, 0x2, 0x5b9, 0x5ba, 0x5, 0xa8, 0x55, 
    0x2, 0x5ba, 0x5bb, 0x7, 0x8, 0x2, 0x2, 0x5bb, 0x5be, 0x5, 0xa8, 0x55, 
    0x2, 0x5bc, 0x5bd, 0x7, 0x8, 0x2, 0x2, 0x5bd, 0x5bf, 0x5, 0xa8, 0x55, 
    0x2, 0x5be, 0x5bc, 0x3, 0x2, 0x2, 0x2, 0x5be, 0x5bf, 0x3, 0x2, 0x2, 
    0x2, 0x5bf, 0x5c8, 0x3, 0x2, 0x2, 0x2, 0x5c0, 0x5c1, 0x5, 0xa8, 0x55, 
    0x2, 0x5c1, 0x5c2, 0x7, 0x58, 0x2, 0x2, 0x5c2, 0x5c5, 0x5, 0xa8, 0x55, 
    0x2, 0x5c3, 0x5c4, 0x7, 0x55, 0x2, 0x2, 0x5c4, 0x5c6, 0x5, 0xa8, 0x55, 
    0x2, 0x5c5, 0x5c3, 0x3, 0x2, 0x2, 0x2, 0x5c5, 0x5c6, 0x3, 0x2, 0x2, 
    0x2, 0x5c6, 0x5c8, 0x3, 0x2, 0x2, 0x2, 0x5c7, 0x5b9, 0x3, 0x2, 0x2, 
    0x2, 0x5c7, 0x5c0, 0x3, 0x2, 0x2, 0x2, 0x5c8, 0x5c9, 0x3, 0x2, 0x2, 
    0x2, 0x5c9, 0x5ca, 0x7, 0xc, 0x2, 0x2, 0x5ca, 0x63a, 0x3, 0x2, 0x2, 
    0x2, 0x5cb, 0x5cc, 0x7, 0xb5, 0x2, 0x2, 0x5cc, 0x5da, 0x7, 0xb, 0x2, 
    0x2, 0x5cd, 0x5d0, 0x5, 0xa8, 0x55, 0x2, 0x5ce, 0x5cf, 0x7, 0x8, 0x2, 
    0x2, 0x5cf, 0x5d1, 0x5, 0xa8, 0x55, 0x2, 0x5d0, 0x5ce, 0x3, 0x2, 0x2, 
    0x2, 0x5d0, 0x5d1, 0x3, 0x2, 0x2, 0x2, 0x5d1, 0x5db, 0x3, 0x2, 0x2, 
    0x2, 0x5d2, 0x5d4, 0x9, 0x18, 0x2, 0x2, 0x5d3, 0x5d2, 0x3, 0x2, 0x2, 
    0x2, 0x5d3, 0x5d4, 0x3, 0x2, 0x2, 0x2, 0x5d4, 0x5d6, 0x3, 0x2, 0x2, 
    0x2, 0x5d5, 0x5d7, 0x5, 0xa8, 0x55, 0x2, 0x5d6, 0x5d5, 0x3, 0x2, 0x2, 
    0x2, 0x5d6, 0x5d7, 0x3, 0x2, 0x2, 0x2, 0x5d7, 0x5d8, 0x3, 0x2, 0x2, 
    0x2, 0x5d8, 0x5d9, 0x7, 0x58, 0x2, 0x2, 0x5d9, 0x5db, 0x5, 0xa8, 0x55, 
    0x2, 0x5da, 0x5cd, 0x3, 0x2, 0x2, 0x2, 0x5da, 0x5d3, 0x3, 0x2, 0x2, 
    0x2, 0x5db, 0x5dc, 0x3, 0x2, 0x2, 0x2, 0x5dc, 0x5dd, 0x7, 0xc, 0x2, 
    0x2, 0x5dd, 0x63a, 0x3, 0x2, 0x2, 0x2, 0x5de, 0x5df, 0x7, 0x50, 0x2, 
    0x2, 0x5df, 0x5e0, 0x7, 0xb, 0x2, 0x2, 0x5e0, 0x5e1, 0x9, 0x19, 0x2, 
    0x2, 0x5e1, 0x5e2, 0x7, 0x58, 0x2, 0x2, 0x5e2, 0x5e3, 0x5, 0xa8, 0x55, 
    0x2, 0x5e3, 0x5e4, 0x7, 0xc, 0x2, 0x2, 0x5e4, 0x63a, 0x3, 0x2, 0x2, 
    0x2, 0x5e5, 0x5e6, 0x7, 0x34, 0x2, 0x2, 0x5e6, 0x5e8, 0x7, 0xb, 0x2, 
    0x2, 0x5e7, 0x5e9, 0x7, 0x16, 0x2, 0x2, 0x5e8, 0x5e7, 0x3, 0x2, 0x2, 
    0x2, 0x5e8, 0x5e9, 0x3, 0x2, 0x2, 0x2, 0x5e9, 0x5ea, 0x3, 0x2, 0x2, 
    0x2, 0x5ea, 0x5eb, 0x7, 0xa, 0x2, 0x2, 0x5eb, 0x63a, 0x7, 0xc, 0x2, 
    0x2, 0x5ec, 0x5ed, 0x7, 0x34, 0x2, 0x2, 0x5ed, 0x5f0, 0x7, 0xb, 0x2, 
    0x2, 0x5ee, 0x5f1, 0x7, 0x16, 0x2, 0x2, 0x5ef, 0x5f1, 0x7, 0x46, 0x2, 
    0x2, 0x5f0, 0x5ee, 0x3, 0x2, 0x2, 0x2, 0x5f0, 0x5ef, 0x3, 0x2, 0x2, 
    0x2, 0x5f0, 0x5f1, 0x3, 0x2, 0x2, 0x2, 0x5f1, 0x5f2, 0x3, 0x2, 0x2, 
    0x2, 0x5f2, 0x5f3, 0x5, 0xa8, 0x55, 0x2, 0x5f3, 0x5f4, 0x7, 0xc, 0x2, 
    0x2, 0x5f4, 0x63a, 0x3, 0x2, 0x2, 0x2, 0x5f5, 0x5f6, 0x7, 0xa8, 0x2, 
    0x2, 0x5f6, 0x5f9, 0x7, 0xb, 0x2, 0x2, 0x5f7, 0x5fa, 0x7, 0x16, 0x2, 
    0x2, 0x5f8, 0x5fa, 0x7, 0x46, 0x2, 0x2, 0x5f9, 0x5f7, 0x3, 0x2, 0x2, 
    0x2, 0x5f9, 0x5f8, 0x3, 0x2, 0x2, 0x2, 0x5f9, 0x5fa, 0x3, 0x2, 0x2, 
    0x2, 0x5fa, 0x5fb, 0x3, 0x2, 0x2, 0x2, 0x5fb, 0x5fc, 0x5, 0xa8, 0x55, 
    0x2, 0x5fc, 0x5fd, 0x7, 0xc, 0x2, 0x2, 0x5fd, 0x63a, 0x3, 0x2, 0x2, 
    0x2, 0x5fe, 0x5ff, 0x7, 0x1c, 0x2, 0x2, 0x5ff, 0x602, 0x7, 0xb, 0x2, 
    0x2, 0x600, 0x603, 0x7, 0x16, 0x2, 0x2, 0x601, 0x603, 0x7, 0x46, 0x2, 
    0x2, 0x602, 0x600, 0x3, 0x2, 0x2, 0x2, 0x602, 0x601, 0x3, 0x2, 0x2, 
    0x2, 0x602, 0x603, 0x3, 0x2, 0x2, 0x2, 0x603, 0x604, 0x3, 0x2, 0x2, 
    0x2, 0x604, 0x605, 0x5, 0xa8, 0x55, 0x2, 0x605, 0x606, 0x7, 0xc, 0x2, 
    0x2, 0x606, 0x63a, 0x3, 0x2, 0x2, 0x2, 0x607, 0x608, 0x7, 0x78, 0x2, 
    0x2, 0x608, 0x60b, 0x7, 0xb, 0x2, 0x2, 0x609, 0x60c, 0x7, 0x16, 0x2, 
    0x2, 0x60a, 0x60c, 0x7, 0x46, 0x2, 0x2, 0x60b, 0x609, 0x3, 0x2, 0x2, 
    0x2, 0x60b, 0x60a, 0x3, 0x2, 0x2, 0x2, 0x60b, 0x60c, 0x3, 0x2, 0x2, 
    0x2, 0x60c, 0x60d, 0x3, 0x2, 0x2, 0x2, 0x60d, 0x60e, 0x5, 0xa8, 0x55, 
    0x2, 0x60e, 0x60f, 0x7, 0xc, 0x2, 0x2, 0x60f, 0x63a, 0x3, 0x2, 0x2, 
    0x2, 0x610, 0x611, 0x7, 0x76, 0x2, 0x2, 0x611, 0x614, 0x7, 0xb, 0x2, 
    0x2, 0x612, 0x615, 0x7, 0x16, 0x2, 0x2, 0x613, 0x615, 0x7, 0x46, 0x2, 
    0x2, 0x614, 0x612, 0x3, 0x2, 0x2, 0x2, 0x614, 0x613, 0x3, 0x2, 0x2, 
    0x2, 0x614, 0x615, 0x3, 0x2, 0x2, 0x2, 0x615, 0x616, 0x3, 0x2, 0x2, 
    0x2, 0x616, 0x617, 0x5, 0xa8, 0x55, 0x2, 0x617, 0x618, 0x7, 0xc, 0x2, 
    0x2, 0x618, 0x63a, 0x3, 0x2, 0x2, 0x2, 0x619, 0x61a, 0x5, 0xb2, 0x5a, 
    0x2, 0x61a, 0x623, 0x7, 0xb, 0x2, 0x2, 0x61b, 0x620, 0x5, 0xa8, 0x55, 
    0x2, 0x61c, 0x61d, 0x7, 0x8, 0x2, 0x2, 0x61d, 0x61f, 0x5, 0xa8, 0x55, 
    0x2, 0x61e, 0x61c, 0x3, 0x2, 0x2, 0x2, 0x61f, 0x622, 0x3, 0x2, 0x2, 
    0x2, 0x620, 0x61e, 0x3, 0x2, 0x2, 0x2, 0x620, 0x621, 0x3, 0x2, 0x2, 
    0x2, 0x621, 0x624, 0x3, 0x2, 0x2, 0x2, 0x622, 0x620, 0x3, 0x2, 0x2, 
    0x2, 0x623, 0x61b, 0x3, 0x2, 0x2, 0x2, 0x623, 0x624, 0x3, 0x2, 0x2, 
    0x2, 0x624, 0x625, 0x3, 0x2, 0x2, 0x2, 0x625, 0x626, 0x7, 0xc, 0x2, 
    0x2, 0x626, 0x63a, 0x3, 0x2, 0x2, 0x2, 0x627, 0x628, 0x7, 0xb, 0x2, 
    0x2, 0x628, 0x629, 0x5, 0xa8, 0x55, 0x2, 0x629, 0x62a, 0x7, 0xc, 0x2, 
    0x2, 0x62a, 0x63a, 0x3, 0x2, 0x2, 0x2, 0x62b, 0x62c, 0x7, 0xb, 0x2, 
    0x2, 0x62c, 0x62d, 0x5, 0x1e, 0x10, 0x2, 0x62d, 0x62e, 0x7, 0xc, 0x2, 
    0x2, 0x62e, 0x63a, 0x3, 0x2, 0x2, 0x2, 0x62f, 0x631, 0x7, 0x81, 0x2, 
    0x2, 0x630, 0x62f, 0x3, 0x2, 0x2, 0x2, 0x630, 0x631, 0x3, 0x2, 0x2, 
    0x2, 0x631, 0x632, 0x3, 0x2, 0x2, 0x2, 0x632, 0x633, 0x7, 0x4e, 0x2, 
    0x2, 0x633, 0x634, 0x7, 0xb, 0x2, 0x2, 0x634, 0x635, 0x5, 0x1e, 0x10, 
    0x2, 0x635, 0x636, 0x7, 0xc, 0x2, 0x2, 0x636, 0x63a, 0x3, 0x2, 0x2, 
    0x2, 0x637, 0x638, 0x7, 0x81, 0x2, 0x2, 0x638, 0x63a, 0x5, 0xa8, 0x55, 
    0x6, 0x639, 0x532, 0x3, 0x2, 0x2, 0x2, 0x639, 0x534, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x535, 0x3, 0x2, 0x2, 0x2, 0x639, 0x536, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x53b, 0x3, 0x2, 0x2, 0x2, 0x639, 0x53d, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x544, 0x3, 0x2, 0x2, 0x2, 0x639, 0x549, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x565, 0x3, 0x2, 0x2, 0x2, 0x639, 0x56e, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x575, 0x3, 0x2, 0x2, 0x2, 0x639, 0x57c, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x589, 0x3, 0x2, 0x2, 0x2, 0x639, 0x596, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x5a3, 0x3, 0x2, 0x2, 0x2, 0x639, 0x5b0, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x5b7, 0x3, 0x2, 0x2, 0x2, 0x639, 0x5cb, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x5de, 0x3, 0x2, 0x2, 0x2, 0x639, 0x5e5, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x5ec, 0x3, 0x2, 0x2, 0x2, 0x639, 0x5f5, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x5fe, 0x3, 0x2, 0x2, 0x2, 0x639, 0x607, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x610, 0x3, 0x2, 0x2, 0x2, 0x639, 0x619, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x627, 0x3, 0x2, 0x2, 0x2, 0x639, 0x62b, 0x3, 0x2, 0x2, 
    0x2, 0x639, 0x630, 0x3, 0x2, 0x2, 0x2, 0x639, 0x637, 0x3, 0x2, 0x2, 
    0x2, 0x63a, 0x698, 0x3, 0x2, 0x2, 0x2, 0x63b, 0x63c, 0xc, 0x27, 0x2, 
    0x2, 0x63c, 0x63d, 0x9, 0x1a, 0x2, 0x2, 0x63d, 0x697, 0x5, 0xa8, 0x55, 
    0x28, 0x63e, 0x63f, 0xc, 0x26, 0x2, 0x2, 0x63f, 0x640, 0x9, 0x1b, 0x2, 
    0x2, 0x640, 0x697, 0x5, 0xa8, 0x55, 0x27, 0x641, 0x642, 0xc, 0x25, 0x2, 
    0x2, 0x642, 0x643, 0x9, 0x1c, 0x2, 0x2, 0x643, 0x697, 0x5, 0xa8, 0x55, 
    0x26, 0x644, 0x646, 0xc, 0xc, 0x2, 0x2, 0x645, 0x647, 0x7, 0x81, 0x2, 
    0x2, 0x646, 0x645, 0x3, 0x2, 0x2, 0x2, 0x646, 0x647, 0x3, 0x2, 0x2, 
    0x2, 0x647, 0x648, 0x3, 0x2, 0x2, 0x2, 0x648, 0x649, 0x7, 0x6e, 0x2, 
    0x2, 0x649, 0x697, 0x5, 0xa8, 0x55, 0xd, 0x64a, 0x64c, 0xc, 0xb, 0x2, 
    0x2, 0x64b, 0x64d, 0x7, 0x81, 0x2, 0x2, 0x64c, 0x64b, 0x3, 0x2, 0x2, 
    0x2, 0x64c, 0x64d, 0x3, 0x2, 0x2, 0x2, 0x64d, 0x64e, 0x3, 0x2, 0x2, 
    0x2, 0x64e, 0x64f, 0x7, 0x6e, 0x2, 0x2, 0x64f, 0x650, 0x5, 0xa8, 0x55, 
    0x2, 0x650, 0x651, 0x7, 0x4b, 0x2, 0x2, 0x651, 0x652, 0x5, 0xa8, 0x55, 
    0xc, 0x652, 0x697, 0x3, 0x2, 0x2, 0x2, 0x653, 0x654, 0xc, 0x5, 0x2, 
    0x2, 0x654, 0x655, 0x6, 0x55, 0xd, 0x2, 0x655, 0x656, 0x7, 0x17, 0x2, 
    0x2, 0x656, 0x697, 0x5, 0xa8, 0x55, 0x6, 0x657, 0x658, 0xc, 0x4, 0x2, 
    0x2, 0x658, 0x659, 0x7, 0xcb, 0x2, 0x2, 0x659, 0x697, 0x5, 0xa8, 0x55, 
    0x5, 0x65a, 0x65b, 0xc, 0x3, 0x2, 0x2, 0x65b, 0x65c, 0x7, 0x87, 0x2, 
    0x2, 0x65c, 0x697, 0x5, 0xa8, 0x55, 0x4, 0x65d, 0x65e, 0xc, 0x28, 0x2, 
    0x2, 0x65e, 0x65f, 0x7, 0xe, 0x2, 0x2, 0x65f, 0x660, 0x5, 0xb4, 0x5b, 
    0x2, 0x660, 0x669, 0x7, 0xb, 0x2, 0x2, 0x661, 0x666, 0x5, 0xa8, 0x55, 
    0x2, 0x662, 0x663, 0x7, 0x8, 0x2, 0x2, 0x663, 0x665, 0x5, 0xa8, 0x55, 
    0x2, 0x664, 0x662, 0x3, 0x2, 0x2, 0x2, 0x665, 0x668, 0x3, 0x2, 0x2, 
    0x2, 0x666, 0x664, 0x3, 0x2, 0x2, 0x2, 0x666, 0x667, 0x3, 0x2, 0x2, 
    0x2, 0x667, 0x66a, 0x3, 0x2, 0x2, 0x2, 0x668, 0x666, 0x3, 0x2, 0x2, 
    0x2, 0x669, 0x661, 0x3, 0x2, 0x2, 0x2, 0x669, 0x66a, 0x3, 0x2, 0x2, 
    0x2, 0x66a, 0x66b, 0x3, 0x2, 0x2, 0x2, 0x66b, 0x66c, 0x7, 0xc, 0x2, 
    0x2, 0x66c, 0x697, 0x3, 0x2, 0x2, 0x2, 0x66d, 0x66e, 0xc, 0xd, 0x2, 
    0x2, 0x66e, 0x670, 0x7, 0x69, 0x2, 0x2, 0x66f, 0x671, 0x7, 0x81, 0x2, 
    0x2, 0x670, 0x66f, 0x3, 0x2, 0x2, 0x2, 0x670, 0x671, 0x3, 0x2, 0x2, 
    0x2, 0x671, 0x672, 0x3, 0x2, 0x2, 0x2, 0x672, 0x697, 0x7, 0x83, 0x2, 
    0x2, 0x673, 0x675, 0xc, 0x9, 0x2, 0x2, 0x674, 0x676, 0x7, 0x81, 0x2, 
    0x2, 0x675, 0x674, 0x3, 0x2, 0x2, 0x2, 0x675, 0x676, 0x3, 0x2, 0x2, 
    0x2, 0x676, 0x677, 0x3, 0x2, 0x2, 0x2, 0x677, 0x678, 0x7, 0x61, 0x2, 
    0x2, 0x678, 0x679, 0x7, 0xb, 0x2, 0x2, 0x679, 0x67a, 0x5, 0x1e, 0x10, 
    0x2, 0x67a, 0x67b, 0x7, 0xc, 0x2, 0x2, 0x67b, 0x697, 0x3, 0x2, 0x2, 
    0x2, 0x67c, 0x67e, 0xc, 0x8, 0x2, 0x2, 0x67d, 0x67f, 0x7, 0x81, 0x2, 
    0x2, 0x67e, 0x67d, 0x3, 0x2, 0x2, 0x2, 0x67e, 0x67f, 0x3, 0x2, 0x2, 
    0x2, 0x67f, 0x680, 0x3, 0x2, 0x2, 0x2, 0x680, 0x681, 0x7, 0x61, 0x2, 
    0x2, 0x681, 0x68a, 0x7, 0xb, 0x2, 0x2, 0x682, 0x687, 0x5, 0xa8, 0x55, 
    0x2, 0x683, 0x684, 0x7, 0x8, 0x2, 0x2, 0x684, 0x686, 0x5, 0xa8, 0x55, 
    0x2, 0x685, 0x683, 0x3, 0x2, 0x2, 0x2, 0x686, 0x689, 0x3, 0x2, 0x2, 
    0x2, 0x687, 0x685, 0x3, 0x2, 0x2, 0x2, 0x687, 0x688, 0x3, 0x2, 0x2, 
    0x2, 0x688, 0x68b, 0x3, 0x2, 0x2, 0x2, 0x689, 0x687, 0x3, 0x2, 0x2, 
    0x2, 0x68a, 0x682, 0x3, 0x2, 0x2, 0x2, 0x68a, 0x68b, 0x3, 0x2, 0x2, 
    0x2, 0x68b, 0x68c, 0x3, 0x2, 0x2, 0x2, 0x68c, 0x697, 0x7, 0xc, 0x2, 
    0x2, 0x68d, 0x68f, 0xc, 0x7, 0x2, 0x2, 0x68e, 0x690, 0x7, 0x81, 0x2, 
    0x2, 0x68f, 0x68e, 0x3, 0x2, 0x2, 0x2, 0x68f, 0x690, 0x3, 0x2, 0x2, 
    0x2, 0x690, 0x691, 0x3, 0x2, 0x2, 0x2, 0x691, 0x692, 0x7, 0x1d, 0x2, 
    0x2, 0x692, 0x693, 0x5, 0xa8, 0x55, 0x2, 0x693, 0x694, 0x7, 0x17, 0x2, 
    0x2, 0x694, 0x695, 0x5, 0xa8, 0x55, 0x2, 0x695, 0x697, 0x3, 0x2, 0x2, 
    0x2, 0x696, 0x63b, 0x3, 0x2, 0x2, 0x2, 0x696, 0x63e, 0x3, 0x2, 0x2, 
    0x2, 0x696, 0x641, 0x3, 0x2, 0x2, 0x2, 0x696, 0x644, 0x3, 0x2, 0x2, 
    0x2, 0x696, 0x64a, 0x3, 0x2, 0x2, 0x2, 0x696, 0x653, 0x3, 0x2, 0x2, 
    0x2, 0x696, 0x657, 0x3, 0x2, 0x2, 0x2, 0x696, 0x65a, 0x3, 0x2, 0x2, 
    0x2, 0x696, 0x65d, 0x3, 0x2, 0x2, 0x2, 0x696, 0x66d, 0x3, 0x2, 0x2, 
    0x2, 0x696, 0x673, 0x3, 0x2, 0x2, 0x2, 0x696, 0x67c, 0x3, 0x2, 0x2, 
    0x2, 0x696, 0x68d, 0x3, 0x2, 0x2, 0x2, 0x697, 0x69a, 0x3, 0x2, 0x2, 
    0x2, 0x698, 0x696, 0x3, 0x2, 0x2, 0x2, 0x698, 0x699, 0x3, 0x2, 0x2, 
    0x2, 0x699, 0xa9, 0x3, 0x2, 0x2, 0x2, 0x69a, 0x698, 0x3, 0x2, 0x2, 0x2, 
    0x69b, 0x69c, 0x9, 0x1d, 0x2, 0x2, 0x69c, 0xab, 0x3, 0x2, 0x2, 0x2, 
    0x69d, 0x69e, 0x5, 0xb6, 0x5c, 0x2, 0x69e, 0x69f, 0x7, 0xe, 0x2, 0x2, 
    0x69f, 0x6a1, 0x3, 0x2, 0x2, 0x2, 0x6a0, 0x69d, 0x3, 0x2, 0x2, 0x2, 
    0x6a0, 0x6a1, 0x3, 0x2, 0x2, 0x2, 0x6a1, 0x6a2, 0x3, 0x2, 0x2, 0x2, 
    0x6a2, 0x6a3, 0x5, 0xb8, 0x5d, 0x2, 0x6a3, 0x6a4, 0x7, 0xe, 0x2, 0x2, 
    0x6a4, 0x6a6, 0x3, 0x2, 0x2, 0x2, 0x6a5, 0x6a0, 0x3, 0x2, 0x2, 0x2, 
    0x6a5, 0x6a6, 0x3, 0x2, 0x2, 0x2, 0x6a6, 0x6a7, 0x3, 0x2, 0x2, 0x2, 
    0x6a7, 0x6a8, 0x5, 0xba, 0x5e, 0x2, 0x6a8, 0x6a9, 0x7, 0xe, 0x2, 0x2, 
    0x6a9, 0x6ab, 0x3, 0x2, 0x2, 0x2, 0x6aa, 0x6a5, 0x3, 0x2, 0x2, 0x2, 
    0x6aa, 0x6ab, 0x3, 0x2, 0x2, 0x2, 0x6ab, 0x6ac, 0x3, 0x2, 0x2, 0x2, 
    0x6ac, 0x6ad, 0x5, 0xbe, 0x60, 0x2, 0x6ad, 0xad, 0x3, 0x2, 0x2, 0x2, 
    0x6ae, 0x6af, 0x5, 0xb6, 0x5c, 0x2, 0x6af, 0x6b0, 0x7, 0xe, 0x2, 0x2, 
    0x6b0, 0x6b2, 0x3, 0x2, 0x2, 0x2, 0x6b1, 0x6ae, 0x3, 0x2, 0x2, 0x2, 
    0x6b1, 0x6b2, 0x3, 0x2, 0x2, 0x2, 0x6b2, 0x6b3, 0x3, 0x2, 0x2, 0x2, 
    0x6b3, 0x6b4, 0x5, 0xb8, 0x5d, 0x2, 0x6b4, 0x6b5, 0x7, 0xe, 0x2, 0x2, 
    0x6b5, 0x6b7, 0x3, 0x2, 0x2, 0x2, 0x6b6, 0x6b1, 0x3, 0x2, 0x2, 0x2, 
    0x6b6, 0x6b7, 0x3, 0x2, 0x2, 0x2, 0x6b7, 0x6b8, 0x3, 0x2, 0x2, 0x2, 
    0x6b8, 0x6b9, 0x5, 0xba, 0x5e, 0x2, 0x6b9, 0x6ba, 0x7, 0xe, 0x2, 0x2, 
    0x6ba, 0x6bc, 0x3, 0x2, 0x2, 0x2, 0x6bb, 0x6b6, 0x3, 0x2, 0x2, 0x2, 
    0x6bb, 0x6bc, 0x3, 0x2, 0x2, 0x2, 0x6bc, 0x6bd, 0x3, 0x2, 0x2, 0x2, 
    0x6bd, 0x6be, 0x7, 0xa, 0x2, 0x2, 0x6be, 0xaf, 0x3, 0x2, 0x2, 0x2, 0x6bf, 
    0x6c0, 0x5, 0xd2, 0x6a, 0x2, 0x6c0, 0x6c1, 0x7, 0xe, 0x2, 0x2, 0x6c1, 
    0x6c3, 0x3, 0x2, 0x2, 0x2, 0x6c2, 0x6bf, 0x3, 0x2, 0x2, 0x2, 0x6c3, 
    0x6c6, 0x3, 0x2, 0x2, 0x2, 0x6c4, 0x6c2, 0x3, 0x2, 0x2, 0x2, 0x6c4, 
    0x6c5, 0x3, 0x2, 0x2, 0x2, 0x6c5, 0x6c7, 0x3, 0x2, 0x2, 0x2, 0x6c6, 
    0x6c4, 0x3, 0x2, 0x2, 0x2, 0x6c7, 0x6c8, 0x5, 0xd0, 0x69, 0x2, 0x6c8, 
    0xb1, 0x3, 0x2, 0x2, 0x2, 0x6c9, 0x6ca, 0x5, 0xd0, 0x69, 0x2, 0x6ca, 
    0xb3, 0x3, 0x2, 0x2, 0x2, 0x6cb, 0x6cc, 0x5, 0xd0, 0x69, 0x2, 0x6cc, 
    0xb5, 0x3, 0x2, 0x2, 0x2, 0x6cd, 0x6ce, 0x5, 0xd0, 0x69, 0x2, 0x6ce, 
    0xb7, 0x3, 0x2, 0x2, 0x2, 0x6cf, 0x6d0, 0x5, 0xd0, 0x69, 0x2, 0x6d0, 
    0xb9, 0x3, 0x2, 0x2, 0x2, 0x6d1, 0x6d2, 0x5, 0xd0, 0x69, 0x2, 0x6d2, 
    0xbb, 0x3, 0x2, 0x2, 0x2, 0x6d3, 0x6d4, 0x5, 0xd0, 0x69, 0x2, 0x6d4, 
    0xbd, 0x3, 0x2, 0x2, 0x2, 0x6d5, 0x6d6, 0x5, 0xd0, 0x69, 0x2, 0x6d6, 
    0xbf, 0x3, 0x2, 0x2, 0x2, 0x6d7, 0x6d8, 0x5, 0xd0, 0x69, 0x2, 0x6d8, 
    0xc1, 0x3, 0x2, 0x2, 0x2, 0x6d9, 0x6da, 0x5, 0xd0, 0x69, 0x2, 0x6da, 
    0xc3, 0x3, 0x2, 0x2, 0x2, 0x6db, 0x6dc, 0x5, 0xd0, 0x69, 0x2, 0x6dc, 
    0xc5, 0x3, 0x2, 0x2, 0x2, 0x6dd, 0x6de, 0x5, 0xd0, 0x69, 0x2, 0x6de, 
    0xc7, 0x3, 0x2, 0x2, 0x2, 0x6df, 0x6e0, 0x5, 0xd0, 0x69, 0x2, 0x6e0, 
    0xc9, 0x3, 0x2, 0x2, 0x2, 0x6e1, 0x6e2, 0x7, 0x5b, 0x2, 0x2, 0x6e2, 
    0xcb, 0x3, 0x2, 0x2, 0x2, 0x6e3, 0x6e6, 0x5, 0xdc, 0x6f, 0x2, 0x6e4, 
    0x6e6, 0x5, 0xd0, 0x69, 0x2, 0x6e5, 0x6e3, 0x3, 0x2, 0x2, 0x2, 0x6e5, 
    0x6e4, 0x3, 0x2, 0x2, 0x2, 0x6e6, 0xcd, 0x3, 0x2, 0x2, 0x2, 0x6e7, 0x6eb, 
    0x5, 0xdc, 0x6f, 0x2, 0x6e8, 0x6eb, 0x5, 0xd0, 0x69, 0x2, 0x6e9, 0x6eb, 
    0x5, 0xa8, 0x55, 0x2, 0x6ea, 0x6e7, 0x3, 0x2, 0x2, 0x2, 0x6ea, 0x6e8, 
    0x3, 0x2, 0x2, 0x2, 0x6ea, 0x6e9, 0x3, 0x2, 0x2, 0x2, 0x6eb, 0xcf, 0x3, 
    0x2, 0x2, 0x2, 0x6ec, 0x6f1, 0x7, 0xce, 0x2, 0x2, 0x6ed, 0x6f1, 0x7, 
    0xcf, 0x2, 0x2, 0x6ee, 0x6f1, 0x7, 0xd0, 0x2, 0x2, 0x6ef, 0x6f1, 0x5, 
    0x102, 0x82, 0x2, 0x6f0, 0x6ec, 0x3, 0x2, 0x2, 0x2, 0x6f0, 0x6ed, 0x3, 
    0x2, 0x2, 0x2, 0x6f0, 0x6ee, 0x3, 0x2, 0x2, 0x2, 0x6f0, 0x6ef, 0x3, 
    0x2, 0x2, 0x2, 0x6f1, 0xd1, 0x3, 0x2, 0x2, 0x2, 0x6f2, 0x6f3, 0x5, 0xd0, 
    0x69, 0x2, 0x6f3, 0xd3, 0x3, 0x2, 0x2, 0x2, 0x6f4, 0x6f5, 0x5, 0xe0, 
    0x71, 0x2, 0x6f5, 0xd5, 0x3, 0x2, 0x2, 0x2, 0x6f6, 0x6f7, 0x5, 0xe0, 
    0x71, 0x2, 0x6f7, 0xd7, 0x3, 0x2, 0x2, 0x2, 0x6f8, 0x6f9, 0x5, 0xe0, 
    0x71, 0x2, 0x6f9, 0xd9, 0x3, 0x2, 0x2, 0x2, 0x6fa, 0x705, 0x7, 0x83, 
    0x2, 0x2, 0x6fb, 0x705, 0x5, 0xde, 0x70, 0x2, 0x6fc, 0x705, 0x5, 0xe2, 
    0x72, 0x2, 0x6fd, 0x705, 0x5, 0xe6, 0x74, 0x2, 0x6fe, 0x705, 0x5, 0xea, 
    0x76, 0x2, 0x6ff, 0x705, 0x5, 0xec, 0x77, 0x2, 0x700, 0x705, 0x5, 0xee, 
    0x78, 0x2, 0x701, 0x705, 0x5, 0xf2, 0x7a, 0x2, 0x702, 0x705, 0x5, 0xf0, 
    0x79, 0x2, 0x703, 0x705, 0x5, 0xf4, 0x7b, 0x2, 0x704, 0x6fa, 0x3, 0x2, 
    0x2, 0x2, 0x704, 0x6fb, 0x3, 0x2, 0x2, 0x2, 0x704, 0x6fc, 0x3, 0x2, 
    0x2, 0x2, 0x704, 0x6fd, 0x3, 0x2, 0x2, 0x2, 0x704, 0x6fe, 0x3, 0x2, 
    0x2, 0x2, 0x704, 0x6ff, 0x3, 0x2, 0x2, 0x2, 0x704, 0x700, 0x3, 0x2, 
    0x2, 0x2, 0x704, 0x701, 0x3, 0x2, 0x2, 0x2, 0x704, 0x702, 0x3, 0x2, 
    0x2, 0x2, 0x704, 0x703, 0x3, 0x2, 0x2, 0x2, 0x705, 0xdb, 0x3, 0x2, 0x2, 
    0x2, 0x706, 0x70d, 0x7, 0x83, 0x2, 0x2, 0x707, 0x70d, 0x5, 0xde, 0x70, 
    0x2, 0x708, 0x70d, 0x5, 0xe2, 0x72, 0x2, 0x709, 0x70d, 0x5, 0xe6, 0x74, 
    0x2, 0x70a, 0x70d, 0x5, 0xea, 0x76, 0x2, 0x70b, 0x70d, 0x5, 0xf2, 0x7a, 
    0x2, 0x70c, 0x706, 0x3, 0x2, 0x2, 0x2, 0x70c, 0x707, 0x3, 0x2, 0x2, 
    0x2, 0x70c, 0x708, 0x3, 0x2, 0x2, 0x2, 0x70c, 0x709, 0x3, 0x2, 0x2, 
    0x2, 0x70c, 0x70a, 0x3, 0x2, 0x2, 0x2, 0x70c, 0x70b, 0x3, 0x2, 0x2, 
    0x2, 0x70d, 0xdd, 0x3, 0x2, 0x2, 0x2, 0x70e, 0x70f, 0x9, 0x1e, 0x2, 
    0x2, 0x70f, 0xdf, 0x3, 0x2, 0x2, 0x2, 0x710, 0x711, 0x7, 0xd5, 0x2, 
    0x2, 0x711, 0xe1, 0x3, 0x2, 0x2, 0x2, 0x712, 0x715, 0x7, 0xf, 0x2, 0x2, 
    0x713, 0x715, 0x7, 0x9, 0x2, 0x2, 0x714, 0x712, 0x3, 0x2, 0x2, 0x2, 
    0x714, 0x713, 0x3, 0x2, 0x2, 0x2, 0x714, 0x715, 0x3, 0x2, 0x2, 0x2, 
    0x715, 0x716, 0x3, 0x2, 0x2, 0x2, 0x716, 0x717, 0x7, 0xd5, 0x2, 0x2, 
    0x717, 0xe3, 0x3, 0x2, 0x2, 0x2, 0x718, 0x719, 0x7, 0xd4, 0x2, 0x2, 
    0x719, 0xe5, 0x3, 0x2, 0x2, 0x2, 0x71a, 0x71d, 0x7, 0xf, 0x2, 0x2, 0x71b, 
    0x71d, 0x7, 0x9, 0x2, 0x2, 0x71c, 0x71a, 0x3, 0x2, 0x2, 0x2, 0x71c, 
    0x71b, 0x3, 0x2, 0x2, 0x2, 0x71c, 0x71d, 0x3, 0x2, 0x2, 0x2, 0x71d, 
    0x71e, 0x3, 0x2, 0x2, 0x2, 0x71e, 0x71f, 0x7, 0xd4, 0x2, 0x2, 0x71f, 
    0xe7, 0x3, 0x2, 0x2, 0x2, 0x720, 0x721, 0x7, 0xd3, 0x2, 0x2, 0x721, 
    0xe9, 0x3, 0x2, 0x2, 0x2, 0x722, 0x725, 0x7, 0xf, 0x2, 0x2, 0x723, 0x725, 
    0x7, 0x9, 0x2, 0x2, 0x724, 0x722, 0x3, 0x2, 0x2, 0x2, 0x724, 0x723, 
    0x3, 0x2, 0x2, 0x2, 0x724, 0x725, 0x3, 0x2, 0x2, 0x2, 0x725, 0x726, 
    0x3, 0x2, 0x2, 0x2, 0x726, 0x727, 0x7, 0xd3, 0x2, 0x2, 0x727, 0xeb, 
    0x3, 0x2, 0x2, 0x2, 0x728, 0x729, 0x7, 0x3c, 0x2, 0x2, 0x729, 0x72a, 
    0x5, 0xf2, 0x7a, 0x2, 0x72a, 0xed, 0x3, 0x2, 0x2, 0x2, 0x72b, 0x72c, 
    0x7, 0xaf, 0x2, 0x2, 0x72c, 0x72d, 0x5, 0xf2, 0x7a, 0x2, 0x72d, 0xef, 
    0x3, 0x2, 0x2, 0x2, 0x72e, 0x72f, 0x7, 0xb0, 0x2, 0x2, 0x72f, 0x730, 
    0x5, 0xf2, 0x7a, 0x2, 0x730, 0xf1, 0x3, 0x2, 0x2, 0x2, 0x731, 0x732, 
    0x9, 0x1f, 0x2, 0x2, 0x732, 0xf3, 0x3, 0x2, 0x2, 0x2, 0x733, 0x736, 
    0x5, 0xf6, 0x7c, 0x2, 0x734, 0x736, 0x5, 0xf8, 0x7d, 0x2, 0x735, 0x733, 
    0x3, 0x2, 0x2, 0x2, 0x735, 0x734, 0x3, 0x2, 0x2, 0x2, 0x736, 0xf5, 0x3, 
    0x2, 0x2, 0x2, 0x737, 0x73a, 0x7, 0x67, 0x2, 0x2, 0x738, 0x73b, 0x7, 
    0xf, 0x2, 0x2, 0x739, 0x73b, 0x7, 0x9, 0x2, 0x2, 0x73a, 0x738, 0x3, 
    0x2, 0x2, 0x2, 0x73a, 0x739, 0x3, 0x2, 0x2, 0x2, 0x73a, 0x73b, 0x3, 
    0x2, 0x2, 0x2, 0x73b, 0x73c, 0x3, 0x2, 0x2, 0x2, 0x73c, 0x754, 0x5, 
    0xf2, 0x7a, 0x2, 0x73d, 0x742, 0x7, 0xcc, 0x2, 0x2, 0x73e, 0x73f, 0x7, 
    0xb, 0x2, 0x2, 0x73f, 0x740, 0x5, 0xd4, 0x6b, 0x2, 0x740, 0x741, 0x7, 
    0xc, 0x2, 0x2, 0x741, 0x743, 0x3, 0x2, 0x2, 0x2, 0x742, 0x73e, 0x3, 
    0x2, 0x2, 0x2, 0x742, 0x743, 0x3, 0x2, 0x2, 0x2, 0x743, 0x755, 0x3, 
    0x2, 0x2, 0x2, 0x744, 0x749, 0x7, 0xcc, 0x2, 0x2, 0x745, 0x746, 0x7, 
    0xb, 0x2, 0x2, 0x746, 0x747, 0x5, 0xd4, 0x6b, 0x2, 0x747, 0x748, 0x7, 
    0xc, 0x2, 0x2, 0x748, 0x74a, 0x3, 0x2, 0x2, 0x2, 0x749, 0x745, 0x3, 
    0x2, 0x2, 0x2, 0x749, 0x74a, 0x3, 0x2, 0x2, 0x2, 0x74a, 0x74b, 0x3, 
    0x2, 0x2, 0x2, 0x74b, 0x74c, 0x7, 0xb1, 0x2, 0x2, 0x74c, 0x755, 0x7, 
    0x7c, 0x2, 0x2, 0x74d, 0x752, 0x7, 0x7c, 0x2, 0x2, 0x74e, 0x74f, 0x7, 
    0xb, 0x2, 0x2, 0x74f, 0x750, 0x5, 0xd4, 0x6b, 0x2, 0x750, 0x751, 0x7, 
    0xc, 0x2, 0x2, 0x751, 0x753, 0x3, 0x2, 0x2, 0x2, 0x752, 0x74e, 0x3, 
    0x2, 0x2, 0x2, 0x752, 0x753, 0x3, 0x2, 0x2, 0x2, 0x753, 0x755, 0x3, 
    0x2, 0x2, 0x2, 0x754, 0x73d, 0x3, 0x2, 0x2, 0x2, 0x754, 0x744, 0x3, 
    0x2, 0x2, 0x2, 0x754, 0x74d, 0x3, 0x2, 0x2, 0x2, 0x755, 0xf7, 0x3, 0x2, 
    0x2, 0x2, 0x756, 0x759, 0x7, 0x67, 0x2, 0x2, 0x757, 0x75a, 0x7, 0xf, 
    0x2, 0x2, 0x758, 0x75a, 0x7, 0x9, 0x2, 0x2, 0x759, 0x757, 0x3, 0x2, 
    0x2, 0x2, 0x759, 0x758, 0x3, 0x2, 0x2, 0x2, 0x759, 0x75a, 0x3, 0x2, 
    0x2, 0x2, 0x75a, 0x75b, 0x3, 0x2, 0x2, 0x2, 0x75b, 0x7b0, 0x5, 0xf2, 
    0x7a, 0x2, 0x75c, 0x761, 0x7, 0x3e, 0x2, 0x2, 0x75d, 0x75e, 0x7, 0xb, 
    0x2, 0x2, 0x75e, 0x75f, 0x5, 0xd4, 0x6b, 0x2, 0x75f, 0x760, 0x7, 0xc, 
    0x2, 0x2, 0x760, 0x762, 0x3, 0x2, 0x2, 0x2, 0x761, 0x75d, 0x3, 0x2, 
    0x2, 0x2, 0x761, 0x762, 0x3, 0x2, 0x2, 0x2, 0x762, 0x7b1, 0x3, 0x2, 
    0x2, 0x2, 0x763, 0x768, 0x7, 0x3e, 0x2, 0x2, 0x764, 0x765, 0x7, 0xb, 
    0x2, 0x2, 0x765, 0x766, 0x5, 0xd4, 0x6b, 0x2, 0x766, 0x767, 0x7, 0xc, 
    0x2, 0x2, 0x767, 0x769, 0x3, 0x2, 0x2, 0x2, 0x768, 0x764, 0x3, 0x2, 
    0x2, 0x2, 0x768, 0x769, 0x3, 0x2, 0x2, 0x2, 0x769, 0x76a, 0x3, 0x2, 
    0x2, 0x2, 0x76a, 0x774, 0x7, 0xb1, 0x2, 0x2, 0x76b, 0x775, 0x7, 0x5d, 
    0x2, 0x2, 0x76c, 0x775, 0x7, 0x79, 0x2, 0x2, 0x76d, 0x772, 0x7, 0x9b, 
    0x2, 0x2, 0x76e, 0x76f, 0x7, 0xb, 0x2, 0x2, 0x76f, 0x770, 0x5, 0xd6, 
    0x6c, 0x2, 0x770, 0x771, 0x7, 0xc, 0x2, 0x2, 0x771, 0x773, 0x3, 0x2, 
    0x2, 0x2, 0x772, 0x76e, 0x3, 0x2, 0x2, 0x2, 0x772, 0x773, 0x3, 0x2, 
    0x2, 0x2, 0x773, 0x775, 0x3, 0x2, 0x2, 0x2, 0x774, 0x76b, 0x3, 0x2, 
    0x2, 0x2, 0x774, 0x76c, 0x3, 0x2, 0x2, 0x2, 0x774, 0x76d, 0x3, 0x2, 
    0x2, 0x2, 0x775, 0x7b1, 0x3, 0x2, 0x2, 0x2, 0x776, 0x77b, 0x7, 0x5d, 
    0x2, 0x2, 0x777, 0x778, 0x7, 0xb, 0x2, 0x2, 0x778, 0x779, 0x5, 0xd4, 
    0x6b, 0x2, 0x779, 0x77a, 0x7, 0xc, 0x2, 0x2, 0x77a, 0x77c, 0x3, 0x2, 
    0x2, 0x2, 0x77b, 0x777, 0x3, 0x2, 0x2, 0x2, 0x77b, 0x77c, 0x3, 0x2, 
    0x2, 0x2, 0x77c, 0x77d, 0x3, 0x2, 0x2, 0x2, 0x77d, 0x786, 0x7, 0xb1, 
    0x2, 0x2, 0x77e, 0x787, 0x7, 0x79, 0x2, 0x2, 0x77f, 0x784, 0x7, 0x9b, 
    0x2, 0x2, 0x780, 0x781, 0x7, 0xb, 0x2, 0x2, 0x781, 0x782, 0x5, 0xd6, 
    0x6c, 0x2, 0x782, 0x783, 0x7, 0xc, 0x2, 0x2, 0x783, 0x785, 0x3, 0x2, 
    0x2, 0x2, 0x784, 0x780, 0x3, 0x2, 0x2, 0x2, 0x784, 0x785, 0x3, 0x2, 
    0x2, 0x2, 0x785, 0x787, 0x3, 0x2, 0x2, 0x2, 0x786, 0x77e, 0x3, 0x2, 
    0x2, 0x2, 0x786, 0x77f, 0x3, 0x2, 0x2, 0x2, 0x787, 0x7b1, 0x3, 0x2, 
    0x2, 0x2, 0x788, 0x78d, 0x7, 0x79, 0x2, 0x2, 0x789, 0x78a, 0x7, 0xb, 
    0x2, 0x2, 0x78a, 0x78b, 0x5, 0xd4, 0x6b, 0x2, 0x78b, 0x78c, 0x7, 0xc, 
    0x2, 0x2, 0x78c, 0x78e, 0x3, 0x2, 0x2, 0x2, 0x78d, 0x789, 0x3, 0x2, 
    0x2, 0x2, 0x78d, 0x78e, 0x3, 0x2, 0x2, 0x2, 0x78e, 0x78f, 0x3, 0x2, 
    0x2, 0x2, 0x78f, 0x790, 0x7, 0xb1, 0x2, 0x2, 0x790, 0x795, 0x7, 0x9b, 
    0x2, 0x2, 0x791, 0x792, 0x7, 0xb, 0x2, 0x2, 0x792, 0x793, 0x5, 0xd6, 
    0x6c, 0x2, 0x793, 0x794, 0x7, 0xc, 0x2, 0x2, 0x794, 0x796, 0x3, 0x2, 
    0x2, 0x2, 0x795, 0x791, 0x3, 0x2, 0x2, 0x2, 0x795, 0x796, 0x3, 0x2, 
    0x2, 0x2, 0x796, 0x7b1, 0x3, 0x2, 0x2, 0x2, 0x797, 0x79c, 0x7, 0x5d, 
    0x2, 0x2, 0x798, 0x799, 0x7, 0xb, 0x2, 0x2, 0x799, 0x79a, 0x5, 0xd4, 
    0x6b, 0x2, 0x79a, 0x79b, 0x7, 0xc, 0x2, 0x2, 0x79b, 0x79d, 0x3, 0x2, 
    0x2, 0x2, 0x79c, 0x798, 0x3, 0x2, 0x2, 0x2, 0x79c, 0x79d, 0x3, 0x2, 
    0x2, 0x2, 0x79d, 0x7b1, 0x3, 0x2, 0x2, 0x2, 0x79e, 0x7a3, 0x7, 0x79, 
    0x2, 0x2, 0x79f, 0x7a0, 0x7, 0xb, 0x2, 0x2, 0x7a0, 0x7a1, 0x5, 0xd4, 
    0x6b, 0x2, 0x7a1, 0x7a2, 0x7, 0xc, 0x2, 0x2, 0x7a2, 0x7a4, 0x3, 0x2, 
    0x2, 0x2, 0x7a3, 0x79f, 0x3, 0x2, 0x2, 0x2, 0x7a3, 0x7a4, 0x3, 0x2, 
    0x2, 0x2, 0x7a4, 0x7b1, 0x3, 0x2, 0x2, 0x2, 0x7a5, 0x7ae, 0x7, 0x9b, 
    0x2, 0x2, 0x7a6, 0x7a7, 0x7, 0xb, 0x2, 0x2, 0x7a7, 0x7aa, 0x5, 0xd4, 
    0x6b, 0x2, 0x7a8, 0x7a9, 0x7, 0x8, 0x2, 0x2, 0x7a9, 0x7ab, 0x5, 0xd6, 
    0x6c, 0x2, 0x7aa, 0x7a8, 0x3, 0x2, 0x2, 0x2, 0x7aa, 0x7ab, 0x3, 0x2, 
    0x2, 0x2, 0x7ab, 0x7ac, 0x3, 0x2, 0x2, 0x2, 0x7ac, 0x7ad, 0x7, 0xc, 
    0x2, 0x2, 0x7ad, 0x7af, 0x3, 0x2, 0x2, 0x2, 0x7ae, 0x7a6, 0x3, 0x2, 
    0x2, 0x2, 0x7ae, 0x7af, 0x3, 0x2, 0x2, 0x2, 0x7af, 0x7b1, 0x3, 0x2, 
    0x2, 0x2, 0x7b0, 0x75c, 0x3, 0x2, 0x2, 0x2, 0x7b0, 0x763, 0x3, 0x2, 
    0x2, 0x2, 0x7b0, 0x776, 0x3, 0x2, 0x2, 0x2, 0x7b0, 0x788, 0x3, 0x2, 
    0x2, 0x2, 0x7b0, 0x797, 0x3, 0x2, 0x2, 0x2, 0x7b0, 0x79e, 0x3, 0x2, 
    0x2, 0x2, 0x7b0, 0x7a5, 0x3, 0x2, 0x2, 0x2, 0x7b1, 0xf9, 0x3, 0x2, 0x2, 
    0x2, 0x7b2, 0x7b3, 0x7, 0x72, 0x2, 0x2, 0x7b3, 0x7b4, 0x5, 0xba, 0x5e, 
    0x2, 0x7b4, 0xfb, 0x3, 0x2, 0x2, 0x2, 0x7b5, 0x7b6, 0x7, 0x73, 0x2, 
    0x2, 0x7b6, 0x7b7, 0x5, 0xba, 0x5e, 0x2, 0x7b7, 0xfd, 0x3, 0x2, 0x2, 
    0x2, 0x7b8, 0x7b9, 0x7, 0x74, 0x2, 0x2, 0x7b9, 0x7ba, 0x5, 0xba, 0x5e, 
    0x2, 0x7ba, 0x7bb, 0x5, 0xc4, 0x63, 0x2, 0x7bb, 0xff, 0x3, 0x2, 0x2, 
    0x2, 0x7bc, 0x7bd, 0x7, 0x75, 0x2, 0x2, 0x7bd, 0x101, 0x3, 0x2, 0x2, 
    0x2, 0x7be, 0x7bf, 0x9, 0x20, 0x2, 0x2, 0x7bf, 0x103, 0x3, 0x2, 0x2, 
    0x2, 0xf7, 0x107, 0x11f, 0x123, 0x125, 0x12e, 0x135, 0x13a, 0x141, 0x14a, 
    0x152, 0x158, 0x15d, 0x165, 0x170, 0x175, 0x179, 0x17f, 0x18b, 0x193, 
    0x19e, 0x1a6, 0x1ab, 0x1b3, 0x1b6, 0x1bc, 0x1c2, 0x1c4, 0x1c9, 0x1ce, 
    0x1d2, 0x1d7, 0x1db, 0x1e1, 0x1e9, 0x1ec, 0x1f2, 0x1f8, 0x1fa, 0x202, 
    0x208, 0x20b, 0x210, 0x219, 0x21c, 0x220, 0x22a, 0x22d, 0x233, 0x237, 
    0x239, 0x23f, 0x243, 0x245, 0x247, 0x24d, 0x253, 0x25a, 0x25f, 0x267, 
    0x26b, 0x276, 0x27a, 0x283, 0x285, 0x28b, 0x290, 0x297, 0x299, 0x29e, 
    0x2ab, 0x2b4, 0x2b7, 0x2bb, 0x2c4, 0x2c9, 0x2ce, 0x2d6, 0x2d9, 0x2dd, 
    0x2e4, 0x2ec, 0x2ef, 0x2fe, 0x305, 0x309, 0x312, 0x31e, 0x325, 0x334, 
    0x338, 0x341, 0x346, 0x349, 0x351, 0x355, 0x35a, 0x35d, 0x364, 0x380, 
    0x385, 0x389, 0x393, 0x395, 0x3a0, 0x3ab, 0x3b4, 0x3ba, 0x3be, 0x3c1, 
    0x3ca, 0x3cf, 0x3d3, 0x3d7, 0x3e2, 0x3e6, 0x3ec, 0x3f1, 0x3f3, 0x3f7, 
    0x3fd, 0x401, 0x40d, 0x415, 0x419, 0x41c, 0x421, 0x428, 0x42f, 0x438, 
    0x43a, 0x441, 0x448, 0x452, 0x454, 0x45b, 0x464, 0x466, 0x46d, 0x475, 
    0x47c, 0x483, 0x48a, 0x48e, 0x490, 0x496, 0x49b, 0x49f, 0x4a2, 0x4a7, 
    0x4ab, 0x4ae, 0x4b2, 0x4ba, 0x4c3, 0x4c9, 0x4cb, 0x4d2, 0x4d7, 0x4da, 
    0x4dd, 0x4e4, 0x4ee, 0x4f3, 0x4fa, 0x504, 0x512, 0x51c, 0x522, 0x529, 
    0x552, 0x55b, 0x55d, 0x561, 0x583, 0x586, 0x590, 0x593, 0x59d, 0x5a0, 
    0x5ac, 0x5be, 0x5c5, 0x5c7, 0x5d0, 0x5d3, 0x5d6, 0x5da, 0x5e8, 0x5f0, 
    0x5f9, 0x602, 0x60b, 0x614, 0x620, 0x623, 0x630, 0x639, 0x646, 0x64c, 
    0x666, 0x669, 0x670, 0x675, 0x67e, 0x687, 0x68a, 0x68f, 0x696, 0x698, 
    0x6a0, 0x6a5, 0x6aa, 0x6b1, 0x6b6, 0x6bb, 0x6c4, 0x6e5, 0x6ea, 0x6f0, 
    0x704, 0x70c, 0x714, 0x71c, 0x724, 0x735, 0x73a, 0x742, 0x749, 0x752, 
    0x754, 0x759, 0x761, 0x768, 0x772, 0x774, 0x77b, 0x784, 0x786, 0x78d, 
    0x795, 0x79c, 0x7a3, 0x7aa, 0x7ae, 0x7b0, 
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
