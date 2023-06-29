
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
    setState(253); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(252);
      statement();
      setState(255); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::T__1)
      | (1ULL << sqlParser::T__4)
      | (1ULL << sqlParser::T__8)
      | (1ULL << sqlParser::T__10)
      | (1ULL << sqlParser::COUNT)
      | (1ULL << sqlParser::CREATE))) != 0) || ((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (sqlParser::DROP - 70))
      | (1ULL << (sqlParser::EXPLAIN - 70))
      | (1ULL << (sqlParser::LOAD - 70))
      | (1ULL << (sqlParser::LSCOL - 70))
      | (1ULL << (sqlParser::LSIDX - 70))
      | (1ULL << (sqlParser::LSIDXCOL - 70))
      | (1ULL << (sqlParser::LSTBL - 70)))) != 0) || ((((_la - 153) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 153)) & ((1ULL << (sqlParser::SELECT - 153))
      | (1ULL << (sqlParser::SPACE - 153))
      | (1ULL << (sqlParser::STATUS - 153))
      | (1ULL << (sqlParser::SZIDX - 153))
      | (1ULL << (sqlParser::SZTBL - 153))
      | (1ULL << (sqlParser::TRUNCATE - 153))
      | (1ULL << (sqlParser::WITH - 153))
      | (1ULL << (sqlParser::HINT_START - 153)))) != 0));
   
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
    setState(257);
    tags();
    setState(258);
    statementChoice();
    setState(259);
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

sqlParser::CreateQueryContext* sqlParser::StatementChoiceContext::createQuery() {
  return getRuleContext<sqlParser::CreateQueryContext>(0);
}

sqlParser::DropQueryContext* sqlParser::StatementChoiceContext::dropQuery() {
  return getRuleContext<sqlParser::DropQueryContext>(0);
}

sqlParser::TruncateQueryContext* sqlParser::StatementChoiceContext::truncateQuery() {
  return getRuleContext<sqlParser::TruncateQueryContext>(0);
}

sqlParser::LoadContext* sqlParser::StatementChoiceContext::load() {
  return getRuleContext<sqlParser::LoadContext>(0);
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
    setState(276);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(261);
      queryStatement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(262);
      explainStatement();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(263);
      lscol();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(264);
      lsidx();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(265);
      lsidxcol();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(266);
      lstbl();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(267);
      createQuery();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(268);
      dropQuery();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(269);
      truncateQuery();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(270);
      load();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(271);
      count();
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(272);
      space();
      break;
    }

    case 13: {
      enterOuterAlt(_localctx, 13);
      setState(273);
      status();
      break;
    }

    case 14: {
      enterOuterAlt(_localctx, 14);
      setState(274);
      szidx();
      break;
    }

    case 15: {
      enterOuterAlt(_localctx, 15);
      setState(275);
      sztbl();
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
    setState(282);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__1

    || _la == sqlParser::T__4) {
      setState(280);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
      case 1: {
        setState(278);
        keyOnlyTag();
        break;
      }

      case 2: {
        setState(279);
        keyValueTag();
        break;
      }

      }
      setState(284);
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
    setState(285);
    match(sqlParser::T__1);
    setState(286);
    dynamic_cast<KeyValueTagContext *>(_localctx)->keyName = match(sqlParser::PLAIN_IDENTIFIER);
    setState(287);
    match(sqlParser::T__2);
    setState(288);
    match(sqlParser::T__3);
    setState(291);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::DUBLQUOTED_IDENTIFIER:
      case sqlParser::SINGLQUOTED_STRING:
      case sqlParser::DUBLQUOTED_STRING: {
        setState(289);
        stringLiteral();
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        setState(290);
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
    setState(298);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__4: {
        enterOuterAlt(_localctx, 1);
        setState(293);
        match(sqlParser::T__4);
        setState(294);
        match(sqlParser::PLAIN_IDENTIFIER);
        break;
      }

      case sqlParser::T__1: {
        enterOuterAlt(_localctx, 2);
        setState(295);
        match(sqlParser::T__1);
        setState(296);
        match(sqlParser::PLAIN_IDENTIFIER);
        setState(297);
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
    setState(303);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(300);
      queryHint();
      setState(305);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(306);
    match(sqlParser::EXPLAIN);
    setState(310);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::FORMAT || ((((_la - 158) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 158)) & ((1ULL << (sqlParser::STAGE - 158))
      | (1ULL << (sqlParser::WITH_NAMES - 158))
      | (1ULL << (sqlParser::WITH_TYPES - 158)))) != 0)) {
      setState(307);
      explainOptions();
      setState(312);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(313);
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
    setState(319);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::STAGE: {
        enterOuterAlt(_localctx, 1);
        setState(315);
        explainStageList();
        break;
      }

      case sqlParser::FORMAT: {
        enterOuterAlt(_localctx, 2);
        setState(316);
        explainFormat();
        break;
      }

      case sqlParser::WITH_NAMES: {
        enterOuterAlt(_localctx, 3);
        setState(317);
        match(sqlParser::WITH_NAMES);
        break;
      }

      case sqlParser::WITH_TYPES: {
        enterOuterAlt(_localctx, 4);
        setState(318);
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
    setState(321);
    match(sqlParser::STAGE);
    setState(322);
    explainRange();
    setState(327);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(323);
      match(sqlParser::T__5);
      setState(324);
      explainRange();
      setState(329);
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
    setState(330);
    dynamic_cast<ExplainRangeContext *>(_localctx)->begin = explainRangeChoice();
    setState(333);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__6 || _la == sqlParser::TO) {
      setState(331);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__6 || _la == sqlParser::TO)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(332);
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
    setState(338);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__7:
      case sqlParser::ALL: {
        enterOuterAlt(_localctx, 1);
        setState(335);
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
        setState(336);
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
      case sqlParser::MINUTES:
      case sqlParser::MONOTONIC:
      case sqlParser::MONTHS:
      case sqlParser::NODE:
      case sqlParser::NOW:
      case sqlParser::OFFSET:
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
        setState(337);
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
    setState(340);
    match(sqlParser::FORMAT);
    setState(341);
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
    setState(346);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(343);
      queryHint();
      setState(348);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(349);
    _la = _input->LA(1);
    if (!(_la == sqlParser::STATS

    || _la == sqlParser::STATISTICS)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(350);
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
    setState(352);
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
    setState(357);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(354);
      queryHint();
      setState(359);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(362);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      setState(360);
      selectQuery();
      break;
    }

    case 2: {
      setState(361);
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
    setState(364);
    match(sqlParser::HINT_START);
    setState(366);
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
      | (1ULL << (sqlParser::MINUTES - 89))
      | (1ULL << (sqlParser::MONOTONIC - 89))
      | (1ULL << (sqlParser::MONTHS - 89))
      | (1ULL << (sqlParser::NODE - 89))
      | (1ULL << (sqlParser::NOW - 89))
      | (1ULL << (sqlParser::OFFSET - 89))
      | (1ULL << (sqlParser::PLACING - 89))
      | (1ULL << (sqlParser::PRETTY - 89))
      | (1ULL << (sqlParser::QUARTER - 89))
      | (1ULL << (sqlParser::QUARTERS - 89))
      | (1ULL << (sqlParser::REFERENCE - 89))
      | (1ULL << (sqlParser::REPLICATED - 89))
      | (1ULL << (sqlParser::ROWSIZE - 89))
      | (1ULL << (sqlParser::SCHEMA - 89))
      | (1ULL << (sqlParser::SECONDS - 89)))) != 0) || ((((_la - 156) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 156)) & ((1ULL << (sqlParser::SKIPLIST - 156))
      | (1ULL << (sqlParser::SPACE - 156))
      | (1ULL << (sqlParser::STAGE - 156))
      | (1ULL << (sqlParser::STATS - 156))
      | (1ULL << (sqlParser::STATISTICS - 156))
      | (1ULL << (sqlParser::STATUS - 156))
      | (1ULL << (sqlParser::SUBSTR - 156))
      | (1ULL << (sqlParser::SZIDX - 156))
      | (1ULL << (sqlParser::SZTBL - 156))
      | (1ULL << (sqlParser::TABLESIZE - 156))
      | (1ULL << (sqlParser::TEXT - 156))
      | (1ULL << (sqlParser::TOP - 156))
      | (1ULL << (sqlParser::TREE - 156))
      | (1ULL << (sqlParser::TRUNCATE - 156))
      | (1ULL << (sqlParser::TYPE - 156))
      | (1ULL << (sqlParser::TYPEOF - 156))
      | (1ULL << (sqlParser::VALUE - 156))
      | (1ULL << (sqlParser::WEEK - 156))
      | (1ULL << (sqlParser::WEEKS - 156))
      | (1ULL << (sqlParser::WITH_NAMES - 156))
      | (1ULL << (sqlParser::WITH_TYPES - 156))
      | (1ULL << (sqlParser::XOR - 156))
      | (1ULL << (sqlParser::YEARS - 156))
      | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 156))
      | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 156))
      | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 156)))) != 0)) {
      setState(365);
      hint();
    }
    setState(372);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(368);
      match(sqlParser::T__5);
      setState(369);
      hint();
      setState(374);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(375);
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
    setState(416);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(377);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(378);
      dynamic_cast<HintContext *>(_localctx)->exprName = exprHint();
      setState(379);
      match(sqlParser::T__3);
      setState(380);
      expr(0);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(382);
      dynamic_cast<HintContext *>(_localctx)->exprName = exprHint();
      setState(384);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__3) {
        setState(383);
        match(sqlParser::T__3);
      }
      setState(386);
      match(sqlParser::T__8);
      setState(387);
      expr(0);
      setState(392);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(388);
        match(sqlParser::T__5);
        setState(389);
        expr(0);
        setState(394);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(395);
      match(sqlParser::T__9);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(397);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(398);
      match(sqlParser::T__3);
      setState(399);
      hintValue();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(401);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(403);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__3) {
        setState(402);
        match(sqlParser::T__3);
      }
      setState(405);
      match(sqlParser::T__8);
      setState(406);
      hintValue();
      setState(411);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(407);
        match(sqlParser::T__5);
        setState(408);
        hintValue();
        setState(413);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(414);
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
    setState(427);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(418);
      match(sqlParser::WITH);
      setState(419);
      withQuery();
      setState(424);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(420);
        match(sqlParser::T__5);
        setState(421);
        withQuery();
        setState(426);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(429);
    selectExpr();
    setState(433);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(430);
      match(sqlParser::ORDER);
      setState(431);
      match(sqlParser::BY);
      setState(432);
      dynamic_cast<SelectQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(441);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(435);
      match(sqlParser::LIMIT);
      setState(436);
      dynamic_cast<SelectQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(439);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(437);
        match(sqlParser::OFFSET);
        setState(438);
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
    setState(470);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__10:
      case sqlParser::SELECT: {
        enterOuterAlt(_localctx, 1);
        setState(443);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__10 || _la == sqlParser::SELECT)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(446);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::ALL: {
            setState(444);
            match(sqlParser::ALL);
            break;
          }

          case sqlParser::DISTINCT: {
            setState(445);
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
        setState(448);
        selectList();
        setState(451);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
        case 1: {
          setState(449);
          match(sqlParser::FROM);
          setState(450);
          tableExpr(0);
          break;
        }

        }
        setState(455);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
        case 1: {
          setState(453);
          match(sqlParser::WHERE);
          setState(454);
          dynamic_cast<SelectExprContext *>(_localctx)->where = predicate();
          break;
        }

        }
        setState(460);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
        case 1: {
          setState(457);
          match(sqlParser::GROUP);
          setState(458);
          match(sqlParser::BY);
          setState(459);
          dynamic_cast<SelectExprContext *>(_localctx)->groupBy = groupByList();
          break;
        }

        }
        setState(464);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
        case 1: {
          setState(462);
          match(sqlParser::HAVING);
          setState(463);
          dynamic_cast<SelectExprContext *>(_localctx)->having = predicate();
          break;
        }

        }
        break;
      }

      case sqlParser::T__8: {
        enterOuterAlt(_localctx, 2);
        setState(466);
        match(sqlParser::T__8);
        setState(467);
        selectExpr();
        setState(468);
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
    setState(481);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(472);
      match(sqlParser::WITH);
      setState(473);
      withQuery();
      setState(478);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(474);
        match(sqlParser::T__5);
        setState(475);
        withQuery();
        setState(480);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(483);
    setExpr(0);
    setState(487);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(484);
      match(sqlParser::ORDER);
      setState(485);
      match(sqlParser::BY);
      setState(486);
      dynamic_cast<SetQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(495);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(489);
      match(sqlParser::LIMIT);
      setState(490);
      dynamic_cast<SetQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(493);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(491);
        match(sqlParser::OFFSET);
        setState(492);
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
    setState(503);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      setState(498);
      selectExpr();
      break;
    }

    case 2: {
      setState(499);
      match(sqlParser::T__8);
      setState(500);
      setExpr(0);
      setState(501);
      match(sqlParser::T__9);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(517);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<SetExprContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleSetExpr);
        setState(505);

        if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
        setState(506);
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
        setState(509);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::DISTINCT: {
            setState(507);
            match(sqlParser::DISTINCT);
            break;
          }

          case sqlParser::ALL: {
            setState(508);
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
        setState(512);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::CORRESPONDING) {
          setState(511);
          corrClause();
        }
        setState(514);
        setExpr(3); 
      }
      setState(519);
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
    setState(520);
    dynamic_cast<CorrClauseContext *>(_localctx)->corr = match(sqlParser::CORRESPONDING);
    setState(526);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::BY) {
      setState(521);
      match(sqlParser::BY);
      setState(522);
      match(sqlParser::T__8);
      setState(523);
      corrList();
      setState(524);
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
    setState(529);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::RECURSIVE) {
      setState(528);
      match(sqlParser::RECURSIVE);
    }
    setState(531);
    dynamic_cast<WithQueryContext *>(_localctx)->name = tableName();
    setState(533);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__8) {
      setState(532);
      colAliases();
    }
    setState(535);
    match(sqlParser::AS);
    setState(536);
    match(sqlParser::T__8);
    setState(537);
    query();
    setState(538);
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
    setState(572);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<FetchContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(541);
      dynamic_cast<FetchContext *>(_localctx)->ref = tableReference();
      setState(546);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx)) {
      case 1: {
        setState(543);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(542);
          match(sqlParser::AS);
        }
        setState(545);
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
      setState(548);
      match(sqlParser::T__8);
      setState(549);
      query();
      setState(550);
      match(sqlParser::T__9);
      setState(558);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 49, _ctx)) {
      case 1: {
        setState(552);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(551);
          match(sqlParser::AS);
        }
        setState(554);
        dynamic_cast<SubqueryContext *>(_localctx)->as = tableAlias();
        setState(556);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 48, _ctx)) {
        case 1: {
          setState(555);
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
      setState(560);
      match(sqlParser::T__8);
      setState(561);
      valuesExpr();
      setState(562);
      match(sqlParser::T__9);
      setState(570);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 52, _ctx)) {
      case 1: {
        setState(564);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(563);
          match(sqlParser::AS);
        }
        setState(566);
        dynamic_cast<ValuesSubqueryContext *>(_localctx)->as = tableAlias();
        setState(568);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx)) {
        case 1: {
          setState(567);
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
    setState(634);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 63, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(632);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 62, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<CrossJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(574);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(578);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(575);
            queryHint();
            setState(580);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(584);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case sqlParser::T__5: {
              setState(581);
              match(sqlParser::T__5);
              break;
            }

            case sqlParser::CROSS: {
              setState(582);
              match(sqlParser::CROSS);
              setState(583);
              match(sqlParser::JOIN);
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(586);
          tableExpr(7);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<NaturalJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(587);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(591);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(588);
            queryHint();
            setState(593);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(594);
          match(sqlParser::NATURAL);

          setState(596);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 85) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 85)) & ((1ULL << (sqlParser::FULL - 85))
            | (1ULL << (sqlParser::INNER - 85))
            | (1ULL << (sqlParser::LEFT - 85))
            | (1ULL << (sqlParser::RIGHT - 85)))) != 0)) {
            setState(595);
            dynamic_cast<NaturalJoinContext *>(_localctx)->type = joinType();
          }
          setState(598);
          match(sqlParser::JOIN);
          setState(599);
          tableExpr(6);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<JoinOnContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(600);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
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
          setState(608);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::ANTI

          || _la == sqlParser::FULL || ((((_la - 97) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 97)) & ((1ULL << (sqlParser::INNER - 97))
            | (1ULL << (sqlParser::LEFT - 97))
            | (1ULL << (sqlParser::RIGHT - 97))
            | (1ULL << (sqlParser::SEMI - 97)))) != 0)) {
            setState(607);
            dynamic_cast<JoinOnContext *>(_localctx)->type = extendedJoinType();
          }
          setState(610);
          match(sqlParser::JOIN);
          setState(611);
          tableExpr(0);
          setState(612);
          match(sqlParser::ON);
          setState(613);
          joinPredicate();
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<JoinUsingContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(615);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(619);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(616);
            queryHint();
            setState(621);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(623);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::ANTI

          || _la == sqlParser::FULL || ((((_la - 97) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 97)) & ((1ULL << (sqlParser::INNER - 97))
            | (1ULL << (sqlParser::LEFT - 97))
            | (1ULL << (sqlParser::RIGHT - 97))
            | (1ULL << (sqlParser::SEMI - 97)))) != 0)) {
            setState(622);
            dynamic_cast<JoinUsingContext *>(_localctx)->type = extendedJoinType();
          }
          setState(625);
          match(sqlParser::JOIN);
          setState(626);
          tableExpr(0);
          setState(627);
          match(sqlParser::USING);
          setState(628);
          match(sqlParser::T__8);
          setState(629);
          joinUsingList();
          setState(630);
          match(sqlParser::T__9);
          break;
        }

        } 
      }
      setState(636);
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
    setState(645);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 65, _ctx)) {
    case 1: {
      setState(640);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 64, _ctx)) {
      case 1: {
        setState(637);
        dynamic_cast<TableReferenceContext *>(_localctx)->catalog = catalogName();
        setState(638);
        match(sqlParser::T__11);
        break;
      }

      }
      setState(642);
      dynamic_cast<TableReferenceContext *>(_localctx)->schema = schemaName();
      setState(643);
      match(sqlParser::T__11);
      break;
    }

    }
    setState(647);
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
    setState(654);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::INNER: {
        enterOuterAlt(_localctx, 1);
        setState(649);
        match(sqlParser::INNER);
        break;
      }

      case sqlParser::FULL:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 2);
        setState(650);
        dynamic_cast<JoinTypeContext *>(_localctx)->outer = _input->LT(1);
        _la = _input->LA(1);
        if (!(((((_la - 85) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 85)) & ((1ULL << (sqlParser::FULL - 85))
          | (1ULL << (sqlParser::LEFT - 85))
          | (1ULL << (sqlParser::RIGHT - 85)))) != 0))) {
          dynamic_cast<JoinTypeContext *>(_localctx)->outer = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(652);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::OUTER) {
          setState(651);
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
    setState(659);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::FULL:
      case sqlParser::INNER:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 1);
        setState(656);
        joinType();
        break;
      }

      case sqlParser::SEMI: {
        enterOuterAlt(_localctx, 2);
        setState(657);
        match(sqlParser::SEMI);
        break;
      }

      case sqlParser::ANTI: {
        enterOuterAlt(_localctx, 3);
        setState(658);
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
    setState(661);
    match(sqlParser::VALUES);
    setState(662);
    match(sqlParser::T__8);
    setState(663);
    valueList();
    setState(664);
    match(sqlParser::T__9);
    setState(672);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(665);
      match(sqlParser::T__5);
      setState(666);
      match(sqlParser::T__8);
      setState(667);
      valueList();
      setState(668);
      match(sqlParser::T__9);
      setState(674);
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
    setState(684);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(675);
      match(sqlParser::WITH);
      setState(676);
      withQuery();
      setState(681);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(677);
        match(sqlParser::T__5);
        setState(678);
        withQuery();
        setState(683);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(686);
    match(sqlParser::INSERT);
    setState(688);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::INTO) {
      setState(687);
      match(sqlParser::INTO);
    }
    setState(690);
    dynamic_cast<InsertQueryContext *>(_localctx)->ref = tableReference();
    setState(702);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 74, _ctx)) {
    case 1: {
      setState(691);
      match(sqlParser::T__8);
      setState(692);
      columnName();
      setState(697);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(693);
        match(sqlParser::T__5);
        setState(694);
        columnName();
        setState(699);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(700);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(707);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx)) {
    case 1: {
      setState(704);
      valuesExpr();
      break;
    }

    case 2: {
      setState(705);
      selectQuery();
      break;
    }

    case 3: {
      setState(706);
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
    setState(718);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(709);
      match(sqlParser::WITH);
      setState(710);
      withQuery();
      setState(715);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(711);
        match(sqlParser::T__5);
        setState(712);
        withQuery();
        setState(717);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(720);
    match(sqlParser::DELETE);
    setState(722);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::FROM) {
      setState(721);
      match(sqlParser::FROM);
    }
    setState(724);
    dynamic_cast<DeleteQueryContext *>(_localctx)->ref = tableReference();
    setState(729);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(725);
        match(sqlParser::WHERE);
        setState(726);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(727);
        match(sqlParser::FROM);
        setState(728);
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
    setState(740);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(731);
      match(sqlParser::WITH);
      setState(732);
      withQuery();
      setState(737);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(733);
        match(sqlParser::T__5);
        setState(734);
        withQuery();
        setState(739);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(742);
    match(sqlParser::UPDATE);
    setState(743);
    dynamic_cast<UpdateQueryContext *>(_localctx)->ref = tableReference();
    setState(744);
    match(sqlParser::SET);
    setState(745);
    columnName();
    setState(746);
    match(sqlParser::T__3);
    setState(747);
    expr(0);
    setState(755);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(748);
      match(sqlParser::T__5);
      setState(749);
      columnName();
      setState(750);
      match(sqlParser::T__3);
      setState(751);
      expr(0);
      setState(757);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(762);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(758);
        match(sqlParser::WHERE);
        setState(759);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(760);
        match(sqlParser::FROM);
        setState(761);
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
    setState(766);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 84, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(764);
      createTableQuery();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(765);
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
    setState(768);
    match(sqlParser::CREATE);
    setState(769);
    match(sqlParser::TABLE);
    setState(770);
    tableName();

    setState(771);
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
    setState(773);
    match(sqlParser::CREATE);
    setState(775);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(774);
      match(sqlParser::UNIQUE);
    }
    setState(777);
    match(sqlParser::INDEX);
    setState(778);
    dynamic_cast<CreateIndexQueryContext *>(_localctx)->idxName = indexName();
    setState(779);
    match(sqlParser::ON);
    setState(780);
    tableName();
    setState(781);
    match(sqlParser::T__8);
    setState(782);
    expr(0);
    setState(787);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(783);
      match(sqlParser::T__5);
      setState(784);
      expr(0);
      setState(789);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(790);
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
    setState(794);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 87, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(792);
      dropTableQuery();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(793);
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
    setState(796);
    match(sqlParser::DROP);
    setState(797);
    match(sqlParser::TABLE);
    setState(798);
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
    setState(800);
    match(sqlParser::DROP);
    setState(801);
    match(sqlParser::INDEX);
    setState(809);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 88, _ctx)) {
    case 1: {
      setState(802);
      dynamic_cast<DropIndexQueryContext *>(_localctx)->idxName = indexName();
      setState(803);
      match(sqlParser::ON);
      setState(804);
      dynamic_cast<DropIndexQueryContext *>(_localctx)->tblName = tableName();
      break;
    }

    case 2: {
      setState(806);
      dynamic_cast<DropIndexQueryContext *>(_localctx)->tblName = tableName();
      setState(807);
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
    setState(811);
    match(sqlParser::TRUNCATE);
    setState(813);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::TABLE) {
      setState(812);
      match(sqlParser::TABLE);
    }
    setState(815);
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
    setState(817);
    match(sqlParser::LOAD);
    setState(818);
    tableName();
    setState(819);
    dynamic_cast<LoadContext *>(_localctx)->filePath = stringLiteral();
    setState(830);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__1

    || _la == sqlParser::DATE_FORMAT || _la == sqlParser::ESCAPE_CHAR

    || _la == sqlParser::FIELD_DELIMITER) {
      setState(820);
      loadOption();
      setState(827);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__1)
        | (1ULL << sqlParser::T__5)
        | (1ULL << sqlParser::DATE_FORMAT))) != 0) || _la == sqlParser::ESCAPE_CHAR

      || _la == sqlParser::FIELD_DELIMITER) {
        setState(822);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(821);
          match(sqlParser::T__5);
        }
        setState(824);
        loadOption();
        setState(829);
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
  enterRule(_localctx, 78, sqlParser::RuleLoadOption);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(837);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__1: {
        setState(832);
        match(sqlParser::T__1);
        setState(833);
        loadOptionKeys();
        setState(834);
        match(sqlParser::T__2);
        break;
      }

      case sqlParser::DATE_FORMAT:
      case sqlParser::ESCAPE_CHAR:
      case sqlParser::FIELD_DELIMITER: {
        setState(836);
        loadOptionKeys();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(839);
    match(sqlParser::T__3);
    setState(840);
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
  enterRule(_localctx, 80, sqlParser::RuleLoadOptionKeys);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(842);
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
  enterRule(_localctx, 82, sqlParser::RuleCount);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(844);
    match(sqlParser::COUNT);
    setState(845);
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
  enterRule(_localctx, 84, sqlParser::RuleSpace);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(847);
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
  enterRule(_localctx, 86, sqlParser::RuleStatus);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(849);
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
  enterRule(_localctx, 88, sqlParser::RuleSzidx);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(851);
    match(sqlParser::SZIDX);
    setState(852);
    tableName();
    setState(853);
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
  enterRule(_localctx, 90, sqlParser::RuleSztbl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(855);
    match(sqlParser::SZTBL);
    setState(856);
    tableName();
   
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
  enterRule(_localctx, 92, sqlParser::RuleTableDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(858);
    match(sqlParser::T__8);
    setState(861);
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
      case sqlParser::MINUTES:
      case sqlParser::MONOTONIC:
      case sqlParser::MONTHS:
      case sqlParser::NODE:
      case sqlParser::NOW:
      case sqlParser::OFFSET:
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
        setState(859);
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
    setState(870);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(863);
      match(sqlParser::T__5);
      setState(866);
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
        case sqlParser::MINUTES:
        case sqlParser::MONOTONIC:
        case sqlParser::MONTHS:
        case sqlParser::NODE:
        case sqlParser::NOW:
        case sqlParser::OFFSET:
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
          setState(864);
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
      setState(872);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(873);
    match(sqlParser::T__9);
    setState(882);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 137) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 137)) & ((1ULL << (sqlParser::PARTITION - 137))
      | (1ULL << (sqlParser::REPLICATED - 137))
      | (1ULL << (sqlParser::ROWSIZE - 137))
      | (1ULL << (sqlParser::TABLESIZE - 137)))) != 0)) {
      setState(880);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::PARTITION: {
          setState(874);
          tablePartitioning();
          break;
        }

        case sqlParser::REPLICATED: {
          setState(875);
          match(sqlParser::REPLICATED);
          break;
        }

        case sqlParser::ROWSIZE: {
          setState(876);
          match(sqlParser::ROWSIZE);
          setState(877);
          integerLiteral();
          break;
        }

        case sqlParser::TABLESIZE: {
          setState(878);
          match(sqlParser::TABLESIZE);
          setState(879);
          integerLiteral();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(884);
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
  enterRule(_localctx, 94, sqlParser::RuleTablePartitioning);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(885);
    match(sqlParser::PARTITION);
    setState(886);
    match(sqlParser::BY);
    setState(887);
    match(sqlParser::T__8);
    setState(888);
    expr(0);
    setState(893);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(889);
      match(sqlParser::T__5);
      setState(890);
      expr(0);
      setState(895);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(896);
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
  enterRule(_localctx, 96, sqlParser::RuleColumnDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(898);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->name = columnName();
    setState(899);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->type = dataType();
    setState(904);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::NOT) {
      setState(900);
      dynamic_cast<ColumnDefinitionContext *>(_localctx)->notNullable = match(sqlParser::NOT);
      setState(901);
      dynamic_cast<ColumnDefinitionContext *>(_localctx)->nullable = match(sqlParser::SQL_NULL);
      setState(906);
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
  enterRule(_localctx, 98, sqlParser::RuleColumnNameList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(907);
    match(sqlParser::T__8);
    setState(908);
    columnName();
    setState(913);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(909);
      match(sqlParser::T__5);
      setState(910);
      columnName();
      setState(915);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(916);
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
  enterRule(_localctx, 100, sqlParser::RuleIndexDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(919);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(918);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
    }
    setState(921);
    _la = _input->LA(1);
    if (!(_la == sqlParser::INDEX

    || _la == sqlParser::KEY)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(923);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 103, _ctx)) {
    case 1: {
      setState(922);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->name = indexName();
      break;
    }

    }
    setState(926);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ON) {
      setState(925);
      match(sqlParser::ON);
    }
    setState(940);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 106, _ctx)) {
    case 1: {
      setState(928);
      expr(0);
      break;
    }

    case 2: {
      setState(929);
      match(sqlParser::T__8);
      setState(930);
      expr(0);
      setState(935);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__5) {
        setState(931);
        match(sqlParser::T__5);
        setState(932);
        expr(0);
        setState(937);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(938);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(944);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::USING) {
      setState(942);
      match(sqlParser::USING);
      setState(943);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->method = indexMethod();
    }
    setState(948);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(946);
      match(sqlParser::WITH);
      setState(947);
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
  enterRule(_localctx, 102, sqlParser::RuleIndexMethod);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(950);
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
  enterRule(_localctx, 104, sqlParser::RuleDataType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(952);
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
  enterRule(_localctx, 106, sqlParser::RuleSqlDataType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1017);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 120, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(954);
      match(sqlParser::BOOLEAN);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(955);
      match(sqlParser::INTEGER);
      setState(963);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(956);
        match(sqlParser::T__8);
        setState(957);
        precision();
        setState(959);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 30) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 30)) & ((1ULL << (sqlParser::BITS - 30))
          | (1ULL << (sqlParser::BYTES - 30))
          | (1ULL << (sqlParser::DIGITS - 30)))) != 0)) {
          setState(958);
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
        setState(961);
        match(sqlParser::T__9);
      }
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(965);
      match(sqlParser::DECIMAL);
      setState(980);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(966);
        match(sqlParser::T__8);
        setState(967);
        precision();
        setState(969);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::DIGITS) {
          setState(968);
          match(sqlParser::DIGITS);
        }
        setState(976);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(971);
          match(sqlParser::T__5);
          setState(972);
          scale();
          setState(974);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::DIGITS) {
            setState(973);
            match(sqlParser::DIGITS);
          }
        }
        setState(978);
        match(sqlParser::T__9);
      }
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(982);
      match(sqlParser::FLOAT);
      setState(990);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(983);
        match(sqlParser::T__8);
        setState(984);
        precision();
        setState(986);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 30) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 30)) & ((1ULL << (sqlParser::BITS - 30))
          | (1ULL << (sqlParser::BYTES - 30))
          | (1ULL << (sqlParser::DIGITS - 30)))) != 0)) {
          setState(985);
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
        setState(988);
        match(sqlParser::T__9);
      }
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(992);
      match(sqlParser::DATE);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(993);
      match(sqlParser::TIME);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(994);
      match(sqlParser::TIMESTAMP);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(995);
      intervalType();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(996);
      fixedLenChar();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(997);
      varyingLenChar();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(998);
      match(sqlParser::BINARY);

      setState(999);
      match(sqlParser::T__8);
      setState(1000);
      size();
      setState(1002);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::BYTE

      || _la == sqlParser::BYTES) {
        setState(1001);
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
      setState(1004);
      match(sqlParser::T__9);
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(1006);
      match(sqlParser::VARBINARY);
      setState(1014);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1007);
        match(sqlParser::T__8);
        setState(1008);
        size();
        setState(1010);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::BYTE

        || _la == sqlParser::BYTES) {
          setState(1009);
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
        setState(1012);
        match(sqlParser::T__9);
      }
      break;
    }

    case 13: {
      enterOuterAlt(_localctx, 13);
      setState(1016);
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
  enterRule(_localctx, 108, sqlParser::RuleIntervalType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1019);
    match(sqlParser::INTERVAL);
    setState(1022);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::MONTH:
      case sqlParser::YEAR: {
        setState(1020);
        yearMonthIntervalType();
        break;
      }

      case sqlParser::DAY:
      case sqlParser::HOUR:
      case sqlParser::MINUTE:
      case sqlParser::SECOND: {
        setState(1021);
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
  enterRule(_localctx, 110, sqlParser::RuleYearMonthIntervalType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1047);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 125, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1024);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1029);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1025);
        match(sqlParser::T__8);
        setState(1026);
        precision();
        setState(1027);
        match(sqlParser::T__9);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1031);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1036);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1032);
        match(sqlParser::T__8);
        setState(1033);
        precision();
        setState(1034);
        match(sqlParser::T__9);
      }
      setState(1038);
      match(sqlParser::TO);
      setState(1039);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->month = match(sqlParser::MONTH);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1040);
      dynamic_cast<YearMonthIntervalTypeContext *>(_localctx)->month = match(sqlParser::MONTH);
      setState(1045);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1041);
        match(sqlParser::T__8);
        setState(1042);
        precision();
        setState(1043);
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
  enterRule(_localctx, 112, sqlParser::RuleDayTimeIntervalType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1133);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 139, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1049);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1054);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1050);
        match(sqlParser::T__8);
        setState(1051);
        precision();
        setState(1052);
        match(sqlParser::T__9);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1056);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1061);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1057);
        match(sqlParser::T__8);
        setState(1058);
        precision();
        setState(1059);
        match(sqlParser::T__9);
      }
      setState(1063);
      match(sqlParser::TO);
      setState(1073);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::HOUR: {
          setState(1064);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->hour = match(sqlParser::HOUR);
          break;
        }

        case sqlParser::MINUTE: {
          setState(1065);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1066);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1071);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__8) {
            setState(1067);
            match(sqlParser::T__8);
            setState(1068);
            scale();
            setState(1069);
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
      setState(1075);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1080);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1076);
        match(sqlParser::T__8);
        setState(1077);
        precision();
        setState(1078);
        match(sqlParser::T__9);
      }
      setState(1082);
      match(sqlParser::TO);
      setState(1091);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::MINUTE: {
          setState(1083);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1084);
          dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1089);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__8) {
            setState(1085);
            match(sqlParser::T__8);
            setState(1086);
            scale();
            setState(1087);
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
      setState(1093);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1098);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1094);
        match(sqlParser::T__8);
        setState(1095);
        precision();
        setState(1096);
        match(sqlParser::T__9);
      }
      setState(1100);
      match(sqlParser::TO);

      setState(1101);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1106);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1102);
        match(sqlParser::T__8);
        setState(1103);
        scale();
        setState(1104);
        match(sqlParser::T__9);
      }
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
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
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1115);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1120);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1116);
        match(sqlParser::T__8);
        setState(1117);
        precision();
        setState(1118);
        match(sqlParser::T__9);
      }
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(1122);
      dynamic_cast<DayTimeIntervalTypeContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1131);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1123);
        match(sqlParser::T__8);
        setState(1124);
        precision();
        setState(1127);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1125);
          match(sqlParser::T__5);
          setState(1126);
          scale();
        }
        setState(1129);
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
  enterRule(_localctx, 114, sqlParser::RuleFixedLenChar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1135);
    _la = _input->LA(1);
    if (!(_la == sqlParser::CHAR

    || _la == sqlParser::CHARACTER)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(1136);
    match(sqlParser::T__8);
    setState(1137);
    size();
    setState(1139);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::BYTE)
      | (1ULL << sqlParser::BYTES)
      | (1ULL << sqlParser::CHAR)
      | (1ULL << sqlParser::CHARS))) != 0)) {
      setState(1138);
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
    setState(1141);
    match(sqlParser::T__9);
    setState(1144);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::COLLATE) {
      setState(1142);
      match(sqlParser::COLLATE);
      setState(1143);
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
  enterRule(_localctx, 116, sqlParser::RuleVaryingLenChar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1163);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::CHAR:
      case sqlParser::CHARACTER:
      case sqlParser::VARCHAR: {
        setState(1151);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::CHAR:
          case sqlParser::CHARACTER: {
            setState(1146);
            _la = _input->LA(1);
            if (!(_la == sqlParser::CHAR

            || _la == sqlParser::CHARACTER)) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
            setState(1148);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == sqlParser::VARYING) {
              setState(1147);
              match(sqlParser::VARYING);
            }
            break;
          }

          case sqlParser::VARCHAR: {
            setState(1150);
            match(sqlParser::VARCHAR);
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(1160);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__8) {
          setState(1153);
          match(sqlParser::T__8);
          setState(1154);
          size();
          setState(1156);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::BYTE)
            | (1ULL << sqlParser::BYTES)
            | (1ULL << sqlParser::CHAR)
            | (1ULL << sqlParser::CHARS))) != 0)) {
            setState(1155);
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
          setState(1158);
          match(sqlParser::T__9);
        }
        break;
      }

      case sqlParser::TEXT: {
        setState(1162);
        match(sqlParser::TEXT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(1167);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::COLLATE) {
      setState(1165);
      match(sqlParser::COLLATE);
      setState(1166);
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
  enterRule(_localctx, 118, sqlParser::RuleAsSubquery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1169);
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
  enterRule(_localctx, 120, sqlParser::RuleSelectOrValues);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    setState(1192);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 151, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1171);
      match(sqlParser::T__8);
      setState(1175);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 148, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(1172);
          queryHint(); 
        }
        setState(1177);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 148, _ctx);
      }
      setState(1178);
      selectOrValues();
      setState(1179);
      match(sqlParser::T__9);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1184);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::HINT_START) {
        setState(1181);
        queryHint();
        setState(1186);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(1190);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 150, _ctx)) {
      case 1: {
        setState(1187);
        selectQuery();
        break;
      }

      case 2: {
        setState(1188);
        valuesExpr();
        break;
      }

      case 3: {
        setState(1189);
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
  enterRule(_localctx, 122, sqlParser::RuleSelectList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1194);
    selectListItem();
    setState(1199);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 152, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1195);
        match(sqlParser::T__5);
        setState(1196);
        selectListItem(); 
      }
      setState(1201);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 152, _ctx);
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
  enterRule(_localctx, 124, sqlParser::RuleSelectListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1210);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 155, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1202);
      expr(0);
      setState(1207);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 154, _ctx)) {
      case 1: {
        setState(1204);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(1203);
          match(sqlParser::AS);
        }
        setState(1206);
        dynamic_cast<SelectListItemContext *>(_localctx)->as = identifier();
        break;
      }

      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1209);
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
  enterRule(_localctx, 126, sqlParser::RuleGroupByList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1212);
    groupByListItem();
    setState(1217);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 156, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1213);
        match(sqlParser::T__5);
        setState(1214);
        groupByListItem(); 
      }
      setState(1219);
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
  enterRule(_localctx, 128, sqlParser::RuleGroupByListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1220);
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
  enterRule(_localctx, 130, sqlParser::RuleOrderByList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1222);
    orderByListItem();
    setState(1227);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1223);
      match(sqlParser::T__5);
      setState(1224);
      orderByListItem();
      setState(1229);
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
  enterRule(_localctx, 132, sqlParser::RuleOrderByListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1230);
    expr(0);
    setState(1232);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ASC

    || _la == sqlParser::DESC) {
      setState(1231);
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
  enterRule(_localctx, 134, sqlParser::RuleJoinUsingList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1234);
    joinUsingListItem();
    setState(1239);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1235);
      match(sqlParser::T__5);
      setState(1236);
      joinUsingListItem();
      setState(1241);
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
  enterRule(_localctx, 136, sqlParser::RuleJoinUsingListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1242);
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
  enterRule(_localctx, 138, sqlParser::RuleCorrList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1244);
    corrListItem();
    setState(1249);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1245);
      match(sqlParser::T__5);
      setState(1246);
      corrListItem();
      setState(1251);
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
  enterRule(_localctx, 140, sqlParser::RuleCorrListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1252);
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
  enterRule(_localctx, 142, sqlParser::RuleColAliases);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1254);
    match(sqlParser::T__8);
    setState(1255);
    colAliasList();
    setState(1256);
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
  enterRule(_localctx, 144, sqlParser::RuleColAliasList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1258);
    colAliasListItem();
    setState(1263);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1259);
      match(sqlParser::T__5);
      setState(1260);
      colAliasListItem();
      setState(1265);
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
  enterRule(_localctx, 146, sqlParser::RuleColAliasListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1266);
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
  enterRule(_localctx, 148, sqlParser::RulePropertyList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1268);
    property();
    setState(1273);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1269);
      match(sqlParser::T__5);
      setState(1270);
      property();
      setState(1275);
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
  enterRule(_localctx, 150, sqlParser::RuleProperty);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1276);
    dynamic_cast<PropertyContext *>(_localctx)->name = propertyName();
    setState(1279);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__3) {
      setState(1277);
      match(sqlParser::T__3);
      setState(1278);
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
  enterRule(_localctx, 152, sqlParser::RuleValueList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1281);
    expr(0);
    setState(1286);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__5) {
      setState(1282);
      match(sqlParser::T__5);
      setState(1283);
      expr(0);
      setState(1288);
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
  enterRule(_localctx, 154, sqlParser::RulePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1289);
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
  enterRule(_localctx, 156, sqlParser::RuleJoinPredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1291);
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
  enterRule(_localctx, 158, sqlParser::RuleCasePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1293);
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
  size_t startState = 160;
  enterRecursionRule(_localctx, 160, sqlParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1558);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 192, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<LiteralValContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(1296);
      literal();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<VarRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1297);
      variableName();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ColRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1298);
      columnRef();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<InlineCastContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1299);
      match(sqlParser::T__8);
      setState(1300);
      dataType();
      setState(1301);
      match(sqlParser::T__9);
      setState(1302);
      expr(40);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<UnaryOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1304);
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
      setState(1305);
      expr(39);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<CastOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1306);
      match(sqlParser::CAST);
      setState(1307);
      match(sqlParser::T__8);
      setState(1308);
      expr(0);
      setState(1309);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5

      || _la == sqlParser::AS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1310);
      dataType();
      setState(1311);
      match(sqlParser::T__9);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<TypeOfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1313);
      match(sqlParser::TYPEOF);
      setState(1314);
      match(sqlParser::T__8);
      setState(1315);
      expr(0);
      setState(1316);
      match(sqlParser::T__9);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<CaseOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1318);
      match(sqlParser::CASE);
      setState(1338);
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
          setState(1319);
          dynamic_cast<CaseOpContext *>(_localctx)->caseExpr = expr(0);
          setState(1325); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1320);
            match(sqlParser::WHEN);
            setState(1321);
            expr(0);
            setState(1322);
            match(sqlParser::THEN);
            setState(1323);
            expr(0);
            setState(1327); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

        case sqlParser::WHEN: {
          setState(1334); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1329);
            match(sqlParser::WHEN);
            setState(1330);
            casePredicate();
            setState(1331);
            match(sqlParser::THEN);
            setState(1332);
            expr(0);
            setState(1336); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(1342);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ELSE) {
        setState(1340);
        match(sqlParser::ELSE);
        setState(1341);
        dynamic_cast<CaseOpContext *>(_localctx)->elseExpr = expr(0);
      }
      setState(1344);
      match(sqlParser::END);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<IfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1346);
      match(sqlParser::IF);
      setState(1347);
      match(sqlParser::T__8);
      setState(1348);
      casePredicate();
      setState(1349);
      match(sqlParser::T__5);
      setState(1350);
      expr(0);
      setState(1351);
      match(sqlParser::T__5);
      setState(1352);
      expr(0);
      setState(1353);
      match(sqlParser::T__9);
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<IfNullOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1355);
      match(sqlParser::IFNULL);
      setState(1356);
      match(sqlParser::T__8);
      setState(1357);
      expr(0);
      setState(1358);
      match(sqlParser::T__5);
      setState(1359);
      expr(0);
      setState(1360);
      match(sqlParser::T__9);
      break;
    }

    case 11: {
      _localctx = _tracker.createInstance<NullIfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1362);
      match(sqlParser::NULLIF);
      setState(1363);
      match(sqlParser::T__8);
      setState(1364);
      expr(0);
      setState(1365);
      match(sqlParser::T__5);
      setState(1366);
      expr(0);
      setState(1367);
      match(sqlParser::T__9);
      break;
    }

    case 12: {
      _localctx = _tracker.createInstance<CoalesceOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1369);
      match(sqlParser::COALESCE);
      setState(1370);
      match(sqlParser::T__8);
      setState(1379);
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
        | (1ULL << (sqlParser::MIN - 65))
        | (1ULL << (sqlParser::MINUTES - 65))
        | (1ULL << (sqlParser::MONOTONIC - 65))
        | (1ULL << (sqlParser::MONTHS - 65))
        | (1ULL << (sqlParser::NODE - 65))
        | (1ULL << (sqlParser::NOT - 65))
        | (1ULL << (sqlParser::NOW - 65))
        | (1ULL << (sqlParser::SQL_NULL - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::NULLIF - 129))
        | (1ULL << (sqlParser::OFFSET - 129))
        | (1ULL << (sqlParser::OVERLAY - 129))
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
        | (1ULL << (sqlParser::TREE - 129))
        | (1ULL << (sqlParser::TRIM - 129))
        | (1ULL << (sqlParser::TRUNCATE - 129))
        | (1ULL << (sqlParser::SQL_TRUE - 129))
        | (1ULL << (sqlParser::TYPE - 129))
        | (1ULL << (sqlParser::TYPEOF - 129))
        | (1ULL << (sqlParser::VALUE - 129))
        | (1ULL << (sqlParser::WEEK - 129))
        | (1ULL << (sqlParser::WEEKS - 129)))) != 0) || ((((_la - 195) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 195)) & ((1ULL << (sqlParser::WITH_NAMES - 195))
        | (1ULL << (sqlParser::WITH_TYPES - 195))
        | (1ULL << (sqlParser::XOR - 195))
        | (1ULL << (sqlParser::YEARS - 195))
        | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 195))
        | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 195))
        | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 195))
        | (1ULL << (sqlParser::SINGLQUOTED_STRING - 195))
        | (1ULL << (sqlParser::DUBLQUOTED_STRING - 195))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 195))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 195))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 195)))) != 0)) {
        setState(1371);
        expr(0);
        setState(1376);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1372);
          match(sqlParser::T__5);
          setState(1373);
          expr(0);
          setState(1378);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1381);
      match(sqlParser::T__9);
      break;
    }

    case 13: {
      _localctx = _tracker.createInstance<DecodeOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1382);
      match(sqlParser::DECODE);
      setState(1383);
      match(sqlParser::T__8);
      setState(1392);
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
        | (1ULL << (sqlParser::MIN - 65))
        | (1ULL << (sqlParser::MINUTES - 65))
        | (1ULL << (sqlParser::MONOTONIC - 65))
        | (1ULL << (sqlParser::MONTHS - 65))
        | (1ULL << (sqlParser::NODE - 65))
        | (1ULL << (sqlParser::NOT - 65))
        | (1ULL << (sqlParser::NOW - 65))
        | (1ULL << (sqlParser::SQL_NULL - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::NULLIF - 129))
        | (1ULL << (sqlParser::OFFSET - 129))
        | (1ULL << (sqlParser::OVERLAY - 129))
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
        | (1ULL << (sqlParser::TREE - 129))
        | (1ULL << (sqlParser::TRIM - 129))
        | (1ULL << (sqlParser::TRUNCATE - 129))
        | (1ULL << (sqlParser::SQL_TRUE - 129))
        | (1ULL << (sqlParser::TYPE - 129))
        | (1ULL << (sqlParser::TYPEOF - 129))
        | (1ULL << (sqlParser::VALUE - 129))
        | (1ULL << (sqlParser::WEEK - 129))
        | (1ULL << (sqlParser::WEEKS - 129)))) != 0) || ((((_la - 195) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 195)) & ((1ULL << (sqlParser::WITH_NAMES - 195))
        | (1ULL << (sqlParser::WITH_TYPES - 195))
        | (1ULL << (sqlParser::XOR - 195))
        | (1ULL << (sqlParser::YEARS - 195))
        | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 195))
        | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 195))
        | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 195))
        | (1ULL << (sqlParser::SINGLQUOTED_STRING - 195))
        | (1ULL << (sqlParser::DUBLQUOTED_STRING - 195))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 195))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 195))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 195)))) != 0)) {
        setState(1384);
        expr(0);
        setState(1389);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1385);
          match(sqlParser::T__5);
          setState(1386);
          expr(0);
          setState(1391);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1394);
      match(sqlParser::T__9);
      break;
    }

    case 14: {
      _localctx = _tracker.createInstance<ConcatOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1395);
      match(sqlParser::CONCAT);
      setState(1396);
      match(sqlParser::T__8);
      setState(1405);
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
        | (1ULL << (sqlParser::MIN - 65))
        | (1ULL << (sqlParser::MINUTES - 65))
        | (1ULL << (sqlParser::MONOTONIC - 65))
        | (1ULL << (sqlParser::MONTHS - 65))
        | (1ULL << (sqlParser::NODE - 65))
        | (1ULL << (sqlParser::NOT - 65))
        | (1ULL << (sqlParser::NOW - 65))
        | (1ULL << (sqlParser::SQL_NULL - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::NULLIF - 129))
        | (1ULL << (sqlParser::OFFSET - 129))
        | (1ULL << (sqlParser::OVERLAY - 129))
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
        | (1ULL << (sqlParser::TREE - 129))
        | (1ULL << (sqlParser::TRIM - 129))
        | (1ULL << (sqlParser::TRUNCATE - 129))
        | (1ULL << (sqlParser::SQL_TRUE - 129))
        | (1ULL << (sqlParser::TYPE - 129))
        | (1ULL << (sqlParser::TYPEOF - 129))
        | (1ULL << (sqlParser::VALUE - 129))
        | (1ULL << (sqlParser::WEEK - 129))
        | (1ULL << (sqlParser::WEEKS - 129)))) != 0) || ((((_la - 195) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 195)) & ((1ULL << (sqlParser::WITH_NAMES - 195))
        | (1ULL << (sqlParser::WITH_TYPES - 195))
        | (1ULL << (sqlParser::XOR - 195))
        | (1ULL << (sqlParser::YEARS - 195))
        | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 195))
        | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 195))
        | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 195))
        | (1ULL << (sqlParser::SINGLQUOTED_STRING - 195))
        | (1ULL << (sqlParser::DUBLQUOTED_STRING - 195))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 195))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 195))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 195)))) != 0)) {
        setState(1397);
        expr(0);
        setState(1402);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1398);
          match(sqlParser::T__5);
          setState(1399);
          expr(0);
          setState(1404);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1407);
      match(sqlParser::T__9);
      break;
    }

    case 15: {
      _localctx = _tracker.createInstance<OverlayFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1408);
      match(sqlParser::OVERLAY);
      setState(1409);
      match(sqlParser::T__8);
      setState(1410);
      expr(0);
      setState(1411);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5 || _la == sqlParser::PLACING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1412);
      expr(0);
      setState(1413);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5 || _la == sqlParser::FROM)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1414);
      expr(0);
      setState(1417);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__5 || _la == sqlParser::FOR) {
        setState(1415);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__5 || _la == sqlParser::FOR)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(1416);
        expr(0);
      }
      setState(1419);
      match(sqlParser::T__9);
      break;
    }

    case 16: {
      _localctx = _tracker.createInstance<PositionFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1421);
      match(sqlParser::POSITION);
      setState(1422);
      match(sqlParser::T__8);
      setState(1423);
      expr(0);
      setState(1424);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__5 || _la == sqlParser::IN)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1425);
      expr(0);
      setState(1426);
      match(sqlParser::T__9);
      break;
    }

    case 17: {
      _localctx = _tracker.createInstance<SubStringFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1428);
      _la = _input->LA(1);
      if (!(_la == sqlParser::SUBSTR

      || _la == sqlParser::SUBSTRING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1429);
      match(sqlParser::T__8);
      setState(1444);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 178, _ctx)) {
      case 1: {
        setState(1430);
        dynamic_cast<SubStringFuncContext *>(_localctx)->src = expr(0);
        setState(1431);
        match(sqlParser::T__5);
        setState(1432);
        dynamic_cast<SubStringFuncContext *>(_localctx)->start = expr(0);
        setState(1435);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1433);
          match(sqlParser::T__5);
          setState(1434);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      case 2: {
        setState(1437);
        dynamic_cast<SubStringFuncContext *>(_localctx)->src = expr(0);
        setState(1438);
        match(sqlParser::FROM);
        setState(1439);
        dynamic_cast<SubStringFuncContext *>(_localctx)->start = expr(0);
        setState(1442);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::FOR) {
          setState(1440);
          match(sqlParser::FOR);
          setState(1441);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      }
      setState(1446);
      match(sqlParser::T__9);
      break;
    }

    case 18: {
      _localctx = _tracker.createInstance<TrimFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1448);
      match(sqlParser::TRIM);
      setState(1449);
      match(sqlParser::T__8);
      setState(1463);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 182, _ctx)) {
      case 1: {
        setState(1450);
        expr(0);
        setState(1453);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1451);
          match(sqlParser::T__5);
          setState(1452);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        break;
      }

      case 2: {
        setState(1456);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::BOTH || _la == sqlParser::LEADING || _la == sqlParser::TRAILING) {
          setState(1455);
          _la = _input->LA(1);
          if (!(_la == sqlParser::BOTH || _la == sqlParser::LEADING || _la == sqlParser::TRAILING)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        setState(1459);
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
          | (1ULL << (sqlParser::MIN - 65))
          | (1ULL << (sqlParser::MINUTES - 65))
          | (1ULL << (sqlParser::MONOTONIC - 65))
          | (1ULL << (sqlParser::MONTHS - 65))
          | (1ULL << (sqlParser::NODE - 65))
          | (1ULL << (sqlParser::NOT - 65))
          | (1ULL << (sqlParser::NOW - 65))
          | (1ULL << (sqlParser::SQL_NULL - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::NULLIF - 129))
          | (1ULL << (sqlParser::OFFSET - 129))
          | (1ULL << (sqlParser::OVERLAY - 129))
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
          | (1ULL << (sqlParser::TREE - 129))
          | (1ULL << (sqlParser::TRIM - 129))
          | (1ULL << (sqlParser::TRUNCATE - 129))
          | (1ULL << (sqlParser::SQL_TRUE - 129))
          | (1ULL << (sqlParser::TYPE - 129))
          | (1ULL << (sqlParser::TYPEOF - 129))
          | (1ULL << (sqlParser::VALUE - 129))
          | (1ULL << (sqlParser::WEEK - 129))
          | (1ULL << (sqlParser::WEEKS - 129)))) != 0) || ((((_la - 195) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 195)) & ((1ULL << (sqlParser::WITH_NAMES - 195))
          | (1ULL << (sqlParser::WITH_TYPES - 195))
          | (1ULL << (sqlParser::XOR - 195))
          | (1ULL << (sqlParser::YEARS - 195))
          | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 195))
          | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 195))
          | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 195))
          | (1ULL << (sqlParser::SINGLQUOTED_STRING - 195))
          | (1ULL << (sqlParser::DUBLQUOTED_STRING - 195))
          | (1ULL << (sqlParser::FLOAT_LITERAL - 195))
          | (1ULL << (sqlParser::DECIMAL_LITERAL - 195))
          | (1ULL << (sqlParser::INTEGER_LITERAL - 195)))) != 0)) {
          setState(1458);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        setState(1461);
        match(sqlParser::FROM);
        setState(1462);
        expr(0);
        break;
      }

      }
      setState(1465);
      match(sqlParser::T__9);
      break;
    }

    case 19: {
      _localctx = _tracker.createInstance<ExtractFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1467);
      match(sqlParser::EXTRACT);
      setState(1468);
      match(sqlParser::T__8);
      setState(1469);
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
      setState(1470);
      match(sqlParser::FROM);
      setState(1471);
      expr(0);
      setState(1472);
      match(sqlParser::T__9);
      break;
    }

    case 20: {
      _localctx = _tracker.createInstance<CountAllContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1474);
      match(sqlParser::COUNT);
      setState(1475);
      match(sqlParser::T__8);
      setState(1477);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ALL) {
        setState(1476);
        match(sqlParser::ALL);
      }
      setState(1479);
      match(sqlParser::T__7);
      setState(1480);
      match(sqlParser::T__9);
      break;
    }

    case 21: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1481);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::COUNT);
      setState(1482);
      match(sqlParser::T__8);
      setState(1485);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1483);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1484);
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
      setState(1487);
      expr(0);
      setState(1488);
      match(sqlParser::T__9);
      break;
    }

    case 22: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1490);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::SUM);
      setState(1491);
      match(sqlParser::T__8);
      setState(1494);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1492);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1493);
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
      setState(1496);
      expr(0);
      setState(1497);
      match(sqlParser::T__9);
      break;
    }

    case 23: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1499);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::AVG);
      setState(1500);
      match(sqlParser::T__8);
      setState(1503);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1501);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1502);
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
      setState(1505);
      expr(0);
      setState(1506);
      match(sqlParser::T__9);
      break;
    }

    case 24: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1508);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MIN);
      setState(1509);
      match(sqlParser::T__8);
      setState(1512);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1510);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1511);
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
      setState(1514);
      expr(0);
      setState(1515);
      match(sqlParser::T__9);
      break;
    }

    case 25: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1517);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MAX);
      setState(1518);
      match(sqlParser::T__8);
      setState(1521);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1519);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1520);
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
      setState(1523);
      expr(0);
      setState(1524);
      match(sqlParser::T__9);
      break;
    }

    case 26: {
      _localctx = _tracker.createInstance<ScalarFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1526);
      dynamic_cast<ScalarFuncContext *>(_localctx)->func = functionName();
      setState(1527);
      match(sqlParser::T__8);
      setState(1536);
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
        | (1ULL << (sqlParser::MIN - 65))
        | (1ULL << (sqlParser::MINUTES - 65))
        | (1ULL << (sqlParser::MONOTONIC - 65))
        | (1ULL << (sqlParser::MONTHS - 65))
        | (1ULL << (sqlParser::NODE - 65))
        | (1ULL << (sqlParser::NOT - 65))
        | (1ULL << (sqlParser::NOW - 65))
        | (1ULL << (sqlParser::SQL_NULL - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::NULLIF - 129))
        | (1ULL << (sqlParser::OFFSET - 129))
        | (1ULL << (sqlParser::OVERLAY - 129))
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
        | (1ULL << (sqlParser::TREE - 129))
        | (1ULL << (sqlParser::TRIM - 129))
        | (1ULL << (sqlParser::TRUNCATE - 129))
        | (1ULL << (sqlParser::SQL_TRUE - 129))
        | (1ULL << (sqlParser::TYPE - 129))
        | (1ULL << (sqlParser::TYPEOF - 129))
        | (1ULL << (sqlParser::VALUE - 129))
        | (1ULL << (sqlParser::WEEK - 129))
        | (1ULL << (sqlParser::WEEKS - 129)))) != 0) || ((((_la - 195) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 195)) & ((1ULL << (sqlParser::WITH_NAMES - 195))
        | (1ULL << (sqlParser::WITH_TYPES - 195))
        | (1ULL << (sqlParser::XOR - 195))
        | (1ULL << (sqlParser::YEARS - 195))
        | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 195))
        | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 195))
        | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 195))
        | (1ULL << (sqlParser::SINGLQUOTED_STRING - 195))
        | (1ULL << (sqlParser::DUBLQUOTED_STRING - 195))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 195))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 195))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 195)))) != 0)) {
        setState(1528);
        expr(0);
        setState(1533);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__5) {
          setState(1529);
          match(sqlParser::T__5);
          setState(1530);
          expr(0);
          setState(1535);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1538);
      match(sqlParser::T__9);
      break;
    }

    case 27: {
      _localctx = _tracker.createInstance<ParentheticContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1540);
      match(sqlParser::T__8);
      setState(1541);
      expr(0);
      setState(1542);
      match(sqlParser::T__9);
      break;
    }

    case 28: {
      _localctx = _tracker.createInstance<SubqueryExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1544);
      match(sqlParser::T__8);
      setState(1545);
      query();
      setState(1546);
      match(sqlParser::T__9);
      break;
    }

    case 29: {
      _localctx = _tracker.createInstance<SubqueryExistsOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1549);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::NOT) {
        setState(1548);
        match(sqlParser::NOT);
      }
      setState(1551);
      match(sqlParser::EXISTS);

      setState(1552);
      match(sqlParser::T__8);
      setState(1553);
      query();
      setState(1554);
      match(sqlParser::T__9);
      break;
    }

    case 30: {
      _localctx = _tracker.createInstance<LogicOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1556);
      dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::NOT);
      setState(1557);
      expr(4);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(1653);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 204, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(1651);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 203, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1560);

          if (!(precpred(_ctx, 37))) throw FailedPredicateException(this, "precpred(_ctx, 37)");
          setState(1561);
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
          setState(1562);
          expr(38);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1563);

          if (!(precpred(_ctx, 36))) throw FailedPredicateException(this, "precpred(_ctx, 36)");
          setState(1564);
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
          setState(1565);
          expr(37);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1566);

          if (!(precpred(_ctx, 35))) throw FailedPredicateException(this, "precpred(_ctx, 35)");
          setState(1567);
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
          setState(1568);
          expr(36);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<LikeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1569);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(1571);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1570);
            match(sqlParser::NOT);
          }
          setState(1573);
          match(sqlParser::LIKE);
          setState(1574);
          expr(11);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<LikeEscapeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1575);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(1577);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1576);
            match(sqlParser::NOT);
          }
          setState(1579);
          match(sqlParser::LIKE);
          setState(1580);
          expr(0);
          setState(1581);
          match(sqlParser::ESCAPE);
          setState(1582);
          expr(10);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1584);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(1585);

          if (!(!(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent)))) throw FailedPredicateException(this, "!(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent))");
          setState(1586);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::AND);
          setState(1587);
          expr(4);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1588);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(1589);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::XOR);
          setState(1590);
          expr(3);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1591);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(1592);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::OR);
          setState(1593);
          expr(2);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<MethodCallContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1594);

          if (!(precpred(_ctx, 38))) throw FailedPredicateException(this, "precpred(_ctx, 38)");
          setState(1595);
          match(sqlParser::T__11);
          setState(1596);
          dynamic_cast<MethodCallContext *>(_localctx)->func = methodName();
          setState(1597);
          match(sqlParser::T__8);
          setState(1606);
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
            | (1ULL << (sqlParser::MIN - 65))
            | (1ULL << (sqlParser::MINUTES - 65))
            | (1ULL << (sqlParser::MONOTONIC - 65))
            | (1ULL << (sqlParser::MONTHS - 65))
            | (1ULL << (sqlParser::NODE - 65))
            | (1ULL << (sqlParser::NOT - 65))
            | (1ULL << (sqlParser::NOW - 65))
            | (1ULL << (sqlParser::SQL_NULL - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::NULLIF - 129))
            | (1ULL << (sqlParser::OFFSET - 129))
            | (1ULL << (sqlParser::OVERLAY - 129))
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
            | (1ULL << (sqlParser::TREE - 129))
            | (1ULL << (sqlParser::TRIM - 129))
            | (1ULL << (sqlParser::TRUNCATE - 129))
            | (1ULL << (sqlParser::SQL_TRUE - 129))
            | (1ULL << (sqlParser::TYPE - 129))
            | (1ULL << (sqlParser::TYPEOF - 129))
            | (1ULL << (sqlParser::VALUE - 129))
            | (1ULL << (sqlParser::WEEK - 129))
            | (1ULL << (sqlParser::WEEKS - 129)))) != 0) || ((((_la - 195) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 195)) & ((1ULL << (sqlParser::WITH_NAMES - 195))
            | (1ULL << (sqlParser::WITH_TYPES - 195))
            | (1ULL << (sqlParser::XOR - 195))
            | (1ULL << (sqlParser::YEARS - 195))
            | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 195))
            | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 195))
            | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 195))
            | (1ULL << (sqlParser::SINGLQUOTED_STRING - 195))
            | (1ULL << (sqlParser::DUBLQUOTED_STRING - 195))
            | (1ULL << (sqlParser::FLOAT_LITERAL - 195))
            | (1ULL << (sqlParser::DECIMAL_LITERAL - 195))
            | (1ULL << (sqlParser::INTEGER_LITERAL - 195)))) != 0)) {
            setState(1598);
            expr(0);
            setState(1603);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__5) {
              setState(1599);
              match(sqlParser::T__5);
              setState(1600);
              expr(0);
              setState(1605);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1608);
          match(sqlParser::T__9);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<IsNullOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1610);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(1611);
          match(sqlParser::IS);
          setState(1613);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1612);
            match(sqlParser::NOT);
          }
          setState(1615);
          match(sqlParser::SQL_NULL);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<SubqueryInOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1616);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(1618);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1617);
            match(sqlParser::NOT);
          }
          setState(1620);
          match(sqlParser::IN);

          setState(1621);
          match(sqlParser::T__8);
          setState(1622);
          query();
          setState(1623);
          match(sqlParser::T__9);
          break;
        }

        case 12: {
          auto newContext = _tracker.createInstance<InOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1625);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(1627);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1626);
            match(sqlParser::NOT);
          }
          setState(1629);
          match(sqlParser::IN);

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
            | (1ULL << (sqlParser::MIN - 65))
            | (1ULL << (sqlParser::MINUTES - 65))
            | (1ULL << (sqlParser::MONOTONIC - 65))
            | (1ULL << (sqlParser::MONTHS - 65))
            | (1ULL << (sqlParser::NODE - 65))
            | (1ULL << (sqlParser::NOT - 65))
            | (1ULL << (sqlParser::NOW - 65))
            | (1ULL << (sqlParser::SQL_NULL - 65)))) != 0) || ((((_la - 129) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 129)) & ((1ULL << (sqlParser::NULLIF - 129))
            | (1ULL << (sqlParser::OFFSET - 129))
            | (1ULL << (sqlParser::OVERLAY - 129))
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
            | (1ULL << (sqlParser::TREE - 129))
            | (1ULL << (sqlParser::TRIM - 129))
            | (1ULL << (sqlParser::TRUNCATE - 129))
            | (1ULL << (sqlParser::SQL_TRUE - 129))
            | (1ULL << (sqlParser::TYPE - 129))
            | (1ULL << (sqlParser::TYPEOF - 129))
            | (1ULL << (sqlParser::VALUE - 129))
            | (1ULL << (sqlParser::WEEK - 129))
            | (1ULL << (sqlParser::WEEKS - 129)))) != 0) || ((((_la - 195) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 195)) & ((1ULL << (sqlParser::WITH_NAMES - 195))
            | (1ULL << (sqlParser::WITH_TYPES - 195))
            | (1ULL << (sqlParser::XOR - 195))
            | (1ULL << (sqlParser::YEARS - 195))
            | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 195))
            | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 195))
            | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 195))
            | (1ULL << (sqlParser::SINGLQUOTED_STRING - 195))
            | (1ULL << (sqlParser::DUBLQUOTED_STRING - 195))
            | (1ULL << (sqlParser::FLOAT_LITERAL - 195))
            | (1ULL << (sqlParser::DECIMAL_LITERAL - 195))
            | (1ULL << (sqlParser::INTEGER_LITERAL - 195)))) != 0)) {
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

        case 13: {
          auto newContext = _tracker.createInstance<BetweenOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1642);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(1644);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1643);
            match(sqlParser::NOT);
          }
          setState(1646);
          match(sqlParser::BETWEEN);

          setState(1647);
          expr(0);
          setState(1648);
          match(sqlParser::AND);
          setState(1649);
          expr(0);
          break;
        }

        } 
      }
      setState(1655);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 204, _ctx);
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
  enterRule(_localctx, 162, sqlParser::RuleVariableName);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1656);
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
  enterRule(_localctx, 164, sqlParser::RuleColumnRef);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1671);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 207, _ctx)) {
    case 1: {
      setState(1666);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 206, _ctx)) {
      case 1: {
        setState(1661);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 205, _ctx)) {
        case 1: {
          setState(1658);
          catalogName();
          setState(1659);
          match(sqlParser::T__11);
          break;
        }

        }

        setState(1663);
        schemaName();
        setState(1664);
        match(sqlParser::T__11);
        break;
      }

      }

      setState(1668);
      tableName();
      setState(1669);
      match(sqlParser::T__11);
      break;
    }

    }
    setState(1673);
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
  enterRule(_localctx, 166, sqlParser::RuleColumnWildcard);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1688);
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
      | (1ULL << (sqlParser::MINUTES - 89))
      | (1ULL << (sqlParser::MONOTONIC - 89))
      | (1ULL << (sqlParser::MONTHS - 89))
      | (1ULL << (sqlParser::NODE - 89))
      | (1ULL << (sqlParser::NOW - 89))
      | (1ULL << (sqlParser::OFFSET - 89))
      | (1ULL << (sqlParser::PLACING - 89))
      | (1ULL << (sqlParser::PRETTY - 89))
      | (1ULL << (sqlParser::QUARTER - 89))
      | (1ULL << (sqlParser::QUARTERS - 89))
      | (1ULL << (sqlParser::REFERENCE - 89))
      | (1ULL << (sqlParser::REPLICATED - 89))
      | (1ULL << (sqlParser::ROWSIZE - 89))
      | (1ULL << (sqlParser::SCHEMA - 89))
      | (1ULL << (sqlParser::SECONDS - 89)))) != 0) || ((((_la - 156) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 156)) & ((1ULL << (sqlParser::SKIPLIST - 156))
      | (1ULL << (sqlParser::SPACE - 156))
      | (1ULL << (sqlParser::STAGE - 156))
      | (1ULL << (sqlParser::STATS - 156))
      | (1ULL << (sqlParser::STATISTICS - 156))
      | (1ULL << (sqlParser::STATUS - 156))
      | (1ULL << (sqlParser::SUBSTR - 156))
      | (1ULL << (sqlParser::SZIDX - 156))
      | (1ULL << (sqlParser::SZTBL - 156))
      | (1ULL << (sqlParser::TABLESIZE - 156))
      | (1ULL << (sqlParser::TEXT - 156))
      | (1ULL << (sqlParser::TOP - 156))
      | (1ULL << (sqlParser::TREE - 156))
      | (1ULL << (sqlParser::TRUNCATE - 156))
      | (1ULL << (sqlParser::TYPE - 156))
      | (1ULL << (sqlParser::TYPEOF - 156))
      | (1ULL << (sqlParser::VALUE - 156))
      | (1ULL << (sqlParser::WEEK - 156))
      | (1ULL << (sqlParser::WEEKS - 156))
      | (1ULL << (sqlParser::WITH_NAMES - 156))
      | (1ULL << (sqlParser::WITH_TYPES - 156))
      | (1ULL << (sqlParser::XOR - 156))
      | (1ULL << (sqlParser::YEARS - 156))
      | (1ULL << (sqlParser::PLAIN_IDENTIFIER - 156))
      | (1ULL << (sqlParser::BAKTIKD_IDENTIFIER - 156))
      | (1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 156)))) != 0)) {
      setState(1683);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 209, _ctx)) {
      case 1: {
        setState(1678);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 208, _ctx)) {
        case 1: {
          setState(1675);
          catalogName();
          setState(1676);
          match(sqlParser::T__11);
          break;
        }

        }

        setState(1680);
        schemaName();
        setState(1681);
        match(sqlParser::T__11);
        break;
      }

      }

      setState(1685);
      tableName();
      setState(1686);
      match(sqlParser::T__11);
    }
    setState(1690);
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
  enterRule(_localctx, 168, sqlParser::RuleClassName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1697);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 211, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1692);
        qualifier();
        setState(1693);
        match(sqlParser::T__11); 
      }
      setState(1699);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 211, _ctx);
    }
    setState(1700);
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
  enterRule(_localctx, 170, sqlParser::RuleFunctionName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1702);
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
  enterRule(_localctx, 172, sqlParser::RuleMethodName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1704);
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
  enterRule(_localctx, 174, sqlParser::RuleCatalogName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1706);
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
  enterRule(_localctx, 176, sqlParser::RuleSchemaName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1708);
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
  enterRule(_localctx, 178, sqlParser::RuleTableName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1710);
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
  enterRule(_localctx, 180, sqlParser::RuleTableAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1712);
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
  enterRule(_localctx, 182, sqlParser::RuleColumnName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1714);
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
  enterRule(_localctx, 184, sqlParser::RuleColumnAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1716);
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
  enterRule(_localctx, 186, sqlParser::RuleCollationName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1718);
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
  enterRule(_localctx, 188, sqlParser::RuleIndexName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1720);
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
  enterRule(_localctx, 190, sqlParser::RulePropertyName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1722);
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
  enterRule(_localctx, 192, sqlParser::RuleHintName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1724);
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
  enterRule(_localctx, 194, sqlParser::RuleExprHint);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1726);
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
  enterRule(_localctx, 196, sqlParser::RulePropertyValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1730);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 212, _ctx)) {
    case 1: {
      setState(1728);
      dynamic_cast<PropertyValueContext *>(_localctx)->lit = simpleLiteral();
      break;
    }

    case 2: {
      setState(1729);
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
  enterRule(_localctx, 198, sqlParser::RuleHintValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1735);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 213, _ctx)) {
    case 1: {
      setState(1732);
      dynamic_cast<HintValueContext *>(_localctx)->lit = simpleLiteral();
      break;
    }

    case 2: {
      setState(1733);
      dynamic_cast<HintValueContext *>(_localctx)->id = identifier();
      break;
    }

    case 3: {
      setState(1734);
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
  enterRule(_localctx, 200, sqlParser::RuleIdentifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1741);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::PLAIN_IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(1737);
        match(sqlParser::PLAIN_IDENTIFIER);
        break;
      }

      case sqlParser::BAKTIKD_IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(1738);
        match(sqlParser::BAKTIKD_IDENTIFIER);
        break;
      }

      case sqlParser::DUBLQUOTED_IDENTIFIER: {
        enterOuterAlt(_localctx, 3);
        setState(1739);
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
      case sqlParser::MINUTES:
      case sqlParser::MONOTONIC:
      case sqlParser::MONTHS:
      case sqlParser::NODE:
      case sqlParser::NOW:
      case sqlParser::OFFSET:
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
        setState(1740);
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
  enterRule(_localctx, 202, sqlParser::RuleQualifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1743);
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
  enterRule(_localctx, 204, sqlParser::RulePrecision);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1745);
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
  enterRule(_localctx, 206, sqlParser::RuleScale);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1747);
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
  enterRule(_localctx, 208, sqlParser::RuleSize);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1749);
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
  enterRule(_localctx, 210, sqlParser::RuleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1761);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 215, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1751);
      match(sqlParser::SQL_NULL);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1752);
      booleanLiteral();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1753);
      signedIntegerLiteral();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(1754);
      signedDecimalLiteral();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1755);
      signedFloatLiteral();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1756);
      dateLiteral();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(1757);
      timeLiteral();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(1758);
      stringLiteral();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(1759);
      timestampLiteral();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(1760);
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
  enterRule(_localctx, 212, sqlParser::RuleSimpleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1769);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 216, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1763);
      match(sqlParser::SQL_NULL);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1764);
      booleanLiteral();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1765);
      signedIntegerLiteral();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(1766);
      signedDecimalLiteral();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1767);
      signedFloatLiteral();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1768);
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
  enterRule(_localctx, 214, sqlParser::RuleBooleanLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1771);
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
  enterRule(_localctx, 216, sqlParser::RuleIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1773);
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
  enterRule(_localctx, 218, sqlParser::RuleSignedIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1777);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1775);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1776);
        dynamic_cast<SignedIntegerLiteralContext *>(_localctx)->neg = match(sqlParser::T__6);
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1779);
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
  enterRule(_localctx, 220, sqlParser::RuleDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1781);
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
  enterRule(_localctx, 222, sqlParser::RuleSignedDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1785);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1783);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1784);
        dynamic_cast<SignedDecimalLiteralContext *>(_localctx)->neg = match(sqlParser::T__6);
        break;
      }

      case sqlParser::DECIMAL_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1787);
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
  enterRule(_localctx, 224, sqlParser::RuleFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1789);
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
  enterRule(_localctx, 226, sqlParser::RuleSignedFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1793);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1791);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1792);
        dynamic_cast<SignedFloatLiteralContext *>(_localctx)->neg = match(sqlParser::T__6);
        break;
      }

      case sqlParser::FLOAT_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1795);
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
  enterRule(_localctx, 228, sqlParser::RuleDateLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1797);
    match(sqlParser::DATE);
    setState(1798);
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
  enterRule(_localctx, 230, sqlParser::RuleTimeLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1800);
    match(sqlParser::TIME);
    setState(1801);
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
  enterRule(_localctx, 232, sqlParser::RuleTimestampLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1803);
    match(sqlParser::TIMESTAMP);
    setState(1804);
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
  enterRule(_localctx, 234, sqlParser::RuleStringLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1806);
    _la = _input->LA(1);
    if (!(((((_la - 203) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 203)) & ((1ULL << (sqlParser::DUBLQUOTED_IDENTIFIER - 203))
      | (1ULL << (sqlParser::SINGLQUOTED_STRING - 203))
      | (1ULL << (sqlParser::DUBLQUOTED_STRING - 203)))) != 0))) {
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
  enterRule(_localctx, 236, sqlParser::RuleIntervalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1810);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 220, _ctx)) {
    case 1: {
      setState(1808);
      yearMonthInterval();
      break;
    }

    case 2: {
      setState(1809);
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
  enterRule(_localctx, 238, sqlParser::RuleYearMonthInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1812);
    match(sqlParser::INTERVAL);
    setState(1815);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1813);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1814);
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
    setState(1817);
    stringLiteral();
    setState(1841);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 225, _ctx)) {
    case 1: {
      setState(1818);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1823);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 222, _ctx)) {
      case 1: {
        setState(1819);
        match(sqlParser::T__8);
        setState(1820);
        precision();
        setState(1821);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1825);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1830);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1826);
        match(sqlParser::T__8);
        setState(1827);
        precision();
        setState(1828);
        match(sqlParser::T__9);
      }
      setState(1832);
      match(sqlParser::TO);
      setState(1833);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      break;
    }

    case 3: {
      setState(1834);
      dynamic_cast<YearMonthIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      setState(1839);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 224, _ctx)) {
      case 1: {
        setState(1835);
        match(sqlParser::T__8);
        setState(1836);
        precision();
        setState(1837);
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
  enterRule(_localctx, 240, sqlParser::RuleDayTimeInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1843);
    match(sqlParser::INTERVAL);
    setState(1846);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__12: {
        setState(1844);
        match(sqlParser::T__12);
        break;
      }

      case sqlParser::T__6: {
        setState(1845);
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
    setState(1848);
    stringLiteral();
    setState(1933);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 240, _ctx)) {
    case 1: {
      setState(1849);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1854);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 227, _ctx)) {
      case 1: {
        setState(1850);
        match(sqlParser::T__8);
        setState(1851);
        precision();
        setState(1852);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1856);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1861);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1857);
        match(sqlParser::T__8);
        setState(1858);
        precision();
        setState(1859);
        match(sqlParser::T__9);
      }
      setState(1863);
      match(sqlParser::TO);
      setState(1873);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::HOUR: {
          setState(1864);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
          break;
        }

        case sqlParser::MINUTE: {
          setState(1865);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1866);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1871);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 229, _ctx)) {
          case 1: {
            setState(1867);
            match(sqlParser::T__8);
            setState(1868);
            scale();
            setState(1869);
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
      setState(1875);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1880);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1876);
        match(sqlParser::T__8);
        setState(1877);
        precision();
        setState(1878);
        match(sqlParser::T__9);
      }
      setState(1882);
      match(sqlParser::TO);
      setState(1891);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::MINUTE: {
          setState(1883);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1884);
          dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1889);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 232, _ctx)) {
          case 1: {
            setState(1885);
            match(sqlParser::T__8);
            setState(1886);
            scale();
            setState(1887);
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
      setState(1893);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1898);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__8) {
        setState(1894);
        match(sqlParser::T__8);
        setState(1895);
        precision();
        setState(1896);
        match(sqlParser::T__9);
      }
      setState(1900);
      match(sqlParser::TO);

      setState(1901);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1906);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 235, _ctx)) {
      case 1: {
        setState(1902);
        match(sqlParser::T__8);
        setState(1903);
        scale();
        setState(1904);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 5: {
      setState(1908);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1913);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 236, _ctx)) {
      case 1: {
        setState(1909);
        match(sqlParser::T__8);
        setState(1910);
        precision();
        setState(1911);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 6: {
      setState(1915);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1920);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 237, _ctx)) {
      case 1: {
        setState(1916);
        match(sqlParser::T__8);
        setState(1917);
        precision();
        setState(1918);
        match(sqlParser::T__9);
        break;
      }

      }
      break;
    }

    case 7: {
      setState(1922);
      dynamic_cast<DayTimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1931);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 239, _ctx)) {
      case 1: {
        setState(1923);
        match(sqlParser::T__8);
        setState(1924);
        precision();
        setState(1927);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__5) {
          setState(1925);
          match(sqlParser::T__5);
          setState(1926);
          scale();
        }
        setState(1929);
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
  enterRule(_localctx, 242, sqlParser::RuleLscol);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1935);
    match(sqlParser::LSCOL);
    setState(1936);
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
  enterRule(_localctx, 244, sqlParser::RuleLsidx);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1938);
    match(sqlParser::LSIDX);
    setState(1939);
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
  enterRule(_localctx, 246, sqlParser::RuleLsidxcol);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1941);
    match(sqlParser::LSIDXCOL);
    setState(1942);
    tableName();
    setState(1943);
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
  enterRule(_localctx, 248, sqlParser::RuleLstbl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1945);
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
  enterRule(_localctx, 250, sqlParser::RuleNonReservedWord);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1947);
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
      | (1ULL << (sqlParser::MINUTES - 89))
      | (1ULL << (sqlParser::MONOTONIC - 89))
      | (1ULL << (sqlParser::MONTHS - 89))
      | (1ULL << (sqlParser::NODE - 89))
      | (1ULL << (sqlParser::NOW - 89))
      | (1ULL << (sqlParser::OFFSET - 89))
      | (1ULL << (sqlParser::PLACING - 89))
      | (1ULL << (sqlParser::PRETTY - 89))
      | (1ULL << (sqlParser::QUARTER - 89))
      | (1ULL << (sqlParser::QUARTERS - 89))
      | (1ULL << (sqlParser::REFERENCE - 89))
      | (1ULL << (sqlParser::REPLICATED - 89))
      | (1ULL << (sqlParser::ROWSIZE - 89))
      | (1ULL << (sqlParser::SCHEMA - 89))
      | (1ULL << (sqlParser::SECONDS - 89)))) != 0) || ((((_la - 156) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 156)) & ((1ULL << (sqlParser::SKIPLIST - 156))
      | (1ULL << (sqlParser::SPACE - 156))
      | (1ULL << (sqlParser::STAGE - 156))
      | (1ULL << (sqlParser::STATS - 156))
      | (1ULL << (sqlParser::STATISTICS - 156))
      | (1ULL << (sqlParser::STATUS - 156))
      | (1ULL << (sqlParser::SUBSTR - 156))
      | (1ULL << (sqlParser::SZIDX - 156))
      | (1ULL << (sqlParser::SZTBL - 156))
      | (1ULL << (sqlParser::TABLESIZE - 156))
      | (1ULL << (sqlParser::TEXT - 156))
      | (1ULL << (sqlParser::TOP - 156))
      | (1ULL << (sqlParser::TREE - 156))
      | (1ULL << (sqlParser::TRUNCATE - 156))
      | (1ULL << (sqlParser::TYPE - 156))
      | (1ULL << (sqlParser::TYPEOF - 156))
      | (1ULL << (sqlParser::VALUE - 156))
      | (1ULL << (sqlParser::WEEK - 156))
      | (1ULL << (sqlParser::WEEKS - 156))
      | (1ULL << (sqlParser::WITH_NAMES - 156))
      | (1ULL << (sqlParser::WITH_TYPES - 156))
      | (1ULL << (sqlParser::XOR - 156))
      | (1ULL << (sqlParser::YEARS - 156)))) != 0))) {
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
    case 80: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

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
  "dropIndexQuery", "truncateQuery", "load", "loadOption", "loadOptionKeys", 
  "count", "space", "status", "szidx", "sztbl", "tableDefinition", "tablePartitioning", 
  "columnDefinition", "columnNameList", "indexDefinition", "indexMethod", 
  "dataType", "sqlDataType", "intervalType", "yearMonthIntervalType", "dayTimeIntervalType", 
  "fixedLenChar", "varyingLenChar", "asSubquery", "selectOrValues", "selectList", 
  "selectListItem", "groupByList", "groupByListItem", "orderByList", "orderByListItem", 
  "joinUsingList", "joinUsingListItem", "corrList", "corrListItem", "colAliases", 
  "colAliasList", "colAliasListItem", "propertyList", "property", "valueList", 
  "predicate", "joinPredicate", "casePredicate", "expr", "variableName", 
  "columnRef", "columnWildcard", "className", "functionName", "methodName", 
  "catalogName", "schemaName", "tableName", "tableAlias", "columnName", 
  "columnAlias", "collationName", "indexName", "propertyName", "hintName", 
  "exprHint", "propertyValue", "hintValue", "identifier", "qualifier", "precision", 
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
  "", "", "", "", "", "", "", "", "'/*+'", "'*/'"
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
  "LOOP", "LSCOL", "LSIDX", "LSIDXCOL", "LSTBL", "MAX", "MIN", "MINUTE", 
  "MINUTES", "MONOTONIC", "MONTH", "MONTHS", "NATURAL", "NO", "NODE", "NOT", 
  "NOW", "SQL_NULL", "NULLIF", "OFFSET", "ON", "OR", "ORDER", "OUTER", "OVER", 
  "OVERLAY", "PARTITION", "PLACING", "POSITION", "PRETTY", "PRIMARY", "QUARTER", 
  "QUARTERS", "RECURSIVE", "REFERENCE", "REPLICATED", "RIGHT", "ROW", "ROWSIZE", 
  "SCHEMA", "SECOND", "SECONDS", "SELECT", "SEMI", "SET", "SKIPLIST", "SPACE", 
  "STAGE", "STATS", "STATISTICS", "STATUS", "SUBSTR", "SUBSTRING", "SUM", 
  "SZIDX", "SZTBL", "TABLE", "TABLESIZE", "TEXT", "THEN", "TIME", "TIMESTAMP", 
  "TO", "TOP", "TREE", "TRIM", "TRAILING", "TRUNCATE", "SQL_TRUE", "TYPE", 
  "TYPEOF", "UNION", "UNIQUE", "UPDATE", "USING", "VALUE", "VALUES", "VARBINARY", 
  "VARCHAR", "VARYING", "WEEK", "WEEKS", "WHEN", "WHERE", "WITH_NAMES", 
  "WITH_TYPES", "WITH", "XOR", "YEAR", "YEARS", "PLAIN_IDENTIFIER", "BAKTIKD_IDENTIFIER", 
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
    0x3, 0xd8, 0x7a0, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
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
    0x9, 0x7d, 0x4, 0x7e, 0x9, 0x7e, 0x4, 0x7f, 0x9, 0x7f, 0x3, 0x2, 0x6, 
    0x2, 0x100, 0xa, 0x2, 0xd, 0x2, 0xe, 0x2, 0x101, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x117, 0xa, 0x4, 0x3, 0x5, 0x3, 
    0x5, 0x7, 0x5, 0x11b, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0x11e, 0xb, 0x5, 
    0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 
    0x126, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 
    0x7, 0x12d, 0xa, 0x7, 0x3, 0x8, 0x7, 0x8, 0x130, 0xa, 0x8, 0xc, 0x8, 
    0xe, 0x8, 0x133, 0xb, 0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 0x137, 0xa, 
    0x8, 0xc, 0x8, 0xe, 0x8, 0x13a, 0xb, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 
    0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 0x142, 0xa, 0x9, 0x3, 0xa, 0x3, 
    0xa, 0x3, 0xa, 0x3, 0xa, 0x7, 0xa, 0x148, 0xa, 0xa, 0xc, 0xa, 0xe, 0xa, 
    0x14b, 0xb, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x150, 0xa, 
    0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0x155, 0xa, 0xc, 0x3, 0xd, 
    0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x7, 0xe, 0x15b, 0xa, 0xe, 0xc, 0xe, 0xe, 
    0xe, 0x15e, 0xb, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 
    0x3, 0x10, 0x7, 0x10, 0x166, 0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 0x169, 
    0xb, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x16d, 0xa, 0x10, 0x3, 0x11, 
    0x3, 0x11, 0x5, 0x11, 0x171, 0xa, 0x11, 0x3, 0x11, 0x3, 0x11, 0x7, 0x11, 
    0x175, 0xa, 0x11, 0xc, 0x11, 0xe, 0x11, 0x178, 0xb, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 
    0x12, 0x3, 0x12, 0x5, 0x12, 0x183, 0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x7, 0x12, 0x189, 0xa, 0x12, 0xc, 0x12, 0xe, 0x12, 
    0x18c, 0xb, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x196, 0xa, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x7, 0x12, 0x19c, 0xa, 0x12, 0xc, 0x12, 
    0xe, 0x12, 0x19f, 0xb, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x1a3, 
    0xa, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x7, 0x13, 0x1a9, 
    0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0x1ac, 0xb, 0x13, 0x5, 0x13, 0x1ae, 
    0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0x1b4, 
    0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0x1ba, 
    0xa, 0x13, 0x5, 0x13, 0x1bc, 0xa, 0x13, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x5, 0x14, 0x1c1, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 
    0x1c6, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1ca, 0xa, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1cf, 0xa, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x5, 0x14, 0x1d3, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x5, 0x14, 0x1d9, 0xa, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x7, 0x15, 0x1df, 0xa, 0x15, 0xc, 0x15, 0xe, 0x15, 0x1e2, 
    0xb, 0x15, 0x5, 0x15, 0x1e4, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x5, 0x15, 0x1ea, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x5, 0x15, 0x1f0, 0xa, 0x15, 0x5, 0x15, 0x1f2, 0xa, 0x15, 
    0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 
    0x16, 0x1fa, 0xa, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 
    0x5, 0x16, 0x200, 0xa, 0x16, 0x3, 0x16, 0x5, 0x16, 0x203, 0xa, 0x16, 
    0x3, 0x16, 0x7, 0x16, 0x206, 0xa, 0x16, 0xc, 0x16, 0xe, 0x16, 0x209, 
    0xb, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 
    0x17, 0x5, 0x17, 0x211, 0xa, 0x17, 0x3, 0x18, 0x5, 0x18, 0x214, 0xa, 
    0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0x218, 0xa, 0x18, 0x3, 0x18, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 
    0x19, 0x5, 0x19, 0x222, 0xa, 0x19, 0x3, 0x19, 0x5, 0x19, 0x225, 0xa, 
    0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x22b, 
    0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x22f, 0xa, 0x19, 0x5, 0x19, 
    0x231, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 
    0x237, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x23b, 0xa, 0x19, 
    0x5, 0x19, 0x23d, 0xa, 0x19, 0x5, 0x19, 0x23f, 0xa, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x7, 0x19, 0x243, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 0x246, 
    0xb, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x24b, 0xa, 0x19, 
    0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 0x19, 0x250, 0xa, 0x19, 0xc, 0x19, 
    0xe, 0x19, 0x253, 0xb, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x257, 
    0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 0x19, 0x25d, 
    0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 0x260, 0xb, 0x19, 0x3, 0x19, 0x5, 0x19, 
    0x263, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x3, 0x19, 0x7, 0x19, 0x26c, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 
    0x26f, 0xb, 0x19, 0x3, 0x19, 0x5, 0x19, 0x272, 0xa, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 
    0x19, 0x27b, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 0x27e, 0xb, 0x19, 0x3, 
    0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 0x283, 0xa, 0x1a, 0x3, 0x1a, 
    0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 0x288, 0xa, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 
    0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x28f, 0xa, 0x1b, 0x5, 0x1b, 
    0x291, 0xa, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x296, 
    0xa, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 
    0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x7, 0x1d, 0x2a1, 0xa, 0x1d, 
    0xc, 0x1d, 0xe, 0x1d, 0x2a4, 0xb, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x7, 0x1e, 0x2aa, 0xa, 0x1e, 0xc, 0x1e, 0xe, 0x1e, 0x2ad, 
    0xb, 0x1e, 0x5, 0x1e, 0x2af, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 
    0x2b3, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x7, 0x1e, 0x2ba, 0xa, 0x1e, 0xc, 0x1e, 0xe, 0x1e, 0x2bd, 0xb, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x2c1, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x5, 0x1e, 0x2c6, 0xa, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
    0x3, 0x1f, 0x7, 0x1f, 0x2cc, 0xa, 0x1f, 0xc, 0x1f, 0xe, 0x1f, 0x2cf, 
    0xb, 0x1f, 0x5, 0x1f, 0x2d1, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 
    0x2d5, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
    0x5, 0x1f, 0x2dc, 0xa, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x7, 0x20, 0x2e2, 0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 0x2e5, 0xb, 0x20, 
    0x5, 0x20, 0x2e7, 0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 
    0x20, 0x7, 0x20, 0x2f4, 0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 0x2f7, 0xb, 
    0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x5, 0x20, 0x2fd, 
    0xa, 0x20, 0x3, 0x21, 0x3, 0x21, 0x5, 0x21, 0x301, 0xa, 0x21, 0x3, 0x22, 
    0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x23, 0x3, 0x23, 0x5, 
    0x23, 0x30a, 0xa, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 
    0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x7, 0x23, 0x314, 0xa, 0x23, 
    0xc, 0x23, 0xe, 0x23, 0x317, 0xb, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x24, 
    0x3, 0x24, 0x5, 0x24, 0x31d, 0xa, 0x24, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 
    0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x5, 0x26, 0x32c, 0xa, 0x26, 
    0x3, 0x27, 0x3, 0x27, 0x5, 0x27, 0x330, 0xa, 0x27, 0x3, 0x27, 0x3, 0x27, 
    0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 0x339, 
    0xa, 0x28, 0x3, 0x28, 0x7, 0x28, 0x33c, 0xa, 0x28, 0xc, 0x28, 0xe, 0x28, 
    0x33f, 0xb, 0x28, 0x5, 0x28, 0x341, 0xa, 0x28, 0x3, 0x29, 0x3, 0x29, 
    0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x5, 0x29, 0x348, 0xa, 0x29, 0x3, 0x29, 
    0x3, 0x29, 0x3, 0x29, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2b, 0x3, 0x2b, 0x3, 
    0x2b, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2e, 0x3, 0x2e, 
    0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x30, 0x3, 
    0x30, 0x3, 0x30, 0x5, 0x30, 0x360, 0xa, 0x30, 0x3, 0x30, 0x3, 0x30, 
    0x3, 0x30, 0x5, 0x30, 0x365, 0xa, 0x30, 0x7, 0x30, 0x367, 0xa, 0x30, 
    0xc, 0x30, 0xe, 0x30, 0x36a, 0xb, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 
    0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x7, 0x30, 0x373, 0xa, 0x30, 
    0xc, 0x30, 0xe, 0x30, 0x376, 0xb, 0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 
    0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x7, 0x31, 0x37e, 0xa, 0x31, 0xc, 0x31, 
    0xe, 0x31, 0x381, 0xb, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x32, 0x3, 0x32, 
    0x3, 0x32, 0x3, 0x32, 0x7, 0x32, 0x389, 0xa, 0x32, 0xc, 0x32, 0xe, 0x32, 
    0x38c, 0xb, 0x32, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x7, 0x33, 
    0x392, 0xa, 0x33, 0xc, 0x33, 0xe, 0x33, 0x395, 0xb, 0x33, 0x3, 0x33, 
    0x3, 0x33, 0x3, 0x34, 0x5, 0x34, 0x39a, 0xa, 0x34, 0x3, 0x34, 0x3, 0x34, 
    0x5, 0x34, 0x39e, 0xa, 0x34, 0x3, 0x34, 0x5, 0x34, 0x3a1, 0xa, 0x34, 
    0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x7, 0x34, 0x3a8, 
    0xa, 0x34, 0xc, 0x34, 0xe, 0x34, 0x3ab, 0xb, 0x34, 0x3, 0x34, 0x3, 0x34, 
    0x5, 0x34, 0x3af, 0xa, 0x34, 0x3, 0x34, 0x3, 0x34, 0x5, 0x34, 0x3b3, 
    0xa, 0x34, 0x3, 0x34, 0x3, 0x34, 0x5, 0x34, 0x3b7, 0xa, 0x34, 0x3, 0x35, 
    0x3, 0x35, 0x3, 0x36, 0x3, 0x36, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 
    0x37, 0x3, 0x37, 0x5, 0x37, 0x3c2, 0xa, 0x37, 0x3, 0x37, 0x3, 0x37, 
    0x5, 0x37, 0x3c6, 0xa, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 
    0x5, 0x37, 0x3cc, 0xa, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x5, 0x37, 
    0x3d1, 0xa, 0x37, 0x5, 0x37, 0x3d3, 0xa, 0x37, 0x3, 0x37, 0x3, 0x37, 
    0x5, 0x37, 0x3d7, 0xa, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 
    0x5, 0x37, 0x3dd, 0xa, 0x37, 0x3, 0x37, 0x3, 0x37, 0x5, 0x37, 0x3e1, 
    0xa, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 
    0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x5, 0x37, 0x3ed, 
    0xa, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 
    0x37, 0x5, 0x37, 0x3f5, 0xa, 0x37, 0x3, 0x37, 0x3, 0x37, 0x5, 0x37, 
    0x3f9, 0xa, 0x37, 0x3, 0x37, 0x5, 0x37, 0x3fc, 0xa, 0x37, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x401, 0xa, 0x38, 0x3, 0x39, 0x3, 0x39, 
    0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x5, 0x39, 0x408, 0xa, 0x39, 0x3, 0x39, 
    0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x5, 0x39, 0x40f, 0xa, 0x39, 
    0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 
    0x39, 0x5, 0x39, 0x418, 0xa, 0x39, 0x5, 0x39, 0x41a, 0xa, 0x39, 0x3, 
    0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x421, 
    0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 
    0x3a, 0x428, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x432, 0xa, 0x3a, 
    0x5, 0x3a, 0x434, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x5, 0x3a, 0x43b, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x444, 0xa, 0x3a, 
    0x5, 0x3a, 0x446, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x5, 0x3a, 0x44d, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x455, 0xa, 0x3a, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x45c, 0xa, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x463, 
    0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 
    0x3a, 0x46a, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x46e, 0xa, 
    0x3a, 0x5, 0x3a, 0x470, 0xa, 0x3a, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 
    0x3, 0x3b, 0x5, 0x3b, 0x476, 0xa, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 
    0x5, 0x3b, 0x47b, 0xa, 0x3b, 0x3, 0x3c, 0x3, 0x3c, 0x5, 0x3c, 0x47f, 
    0xa, 0x3c, 0x3, 0x3c, 0x5, 0x3c, 0x482, 0xa, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 
    0x3, 0x3c, 0x5, 0x3c, 0x487, 0xa, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x5, 0x3c, 
    0x48b, 0xa, 0x3c, 0x3, 0x3c, 0x5, 0x3c, 0x48e, 0xa, 0x3c, 0x3, 0x3c, 
    0x3, 0x3c, 0x5, 0x3c, 0x492, 0xa, 0x3c, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3e, 
    0x3, 0x3e, 0x7, 0x3e, 0x498, 0xa, 0x3e, 0xc, 0x3e, 0xe, 0x3e, 0x49b, 
    0xb, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x7, 0x3e, 0x4a1, 
    0xa, 0x3e, 0xc, 0x3e, 0xe, 0x3e, 0x4a4, 0xb, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x5, 0x3e, 0x4a9, 0xa, 0x3e, 0x5, 0x3e, 0x4ab, 0xa, 0x3e, 
    0x3, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x7, 0x3f, 0x4b0, 0xa, 0x3f, 0xc, 0x3f, 
    0xe, 0x3f, 0x4b3, 0xb, 0x3f, 0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 0x4b7, 
    0xa, 0x40, 0x3, 0x40, 0x5, 0x40, 0x4ba, 0xa, 0x40, 0x3, 0x40, 0x5, 0x40, 
    0x4bd, 0xa, 0x40, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x7, 0x41, 0x4c2, 
    0xa, 0x41, 0xc, 0x41, 0xe, 0x41, 0x4c5, 0xb, 0x41, 0x3, 0x42, 0x3, 0x42, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x7, 0x43, 0x4cc, 0xa, 0x43, 0xc, 0x43, 
    0xe, 0x43, 0x4cf, 0xb, 0x43, 0x3, 0x44, 0x3, 0x44, 0x5, 0x44, 0x4d3, 
    0xa, 0x44, 0x3, 0x45, 0x3, 0x45, 0x3, 0x45, 0x7, 0x45, 0x4d8, 0xa, 0x45, 
    0xc, 0x45, 0xe, 0x45, 0x4db, 0xb, 0x45, 0x3, 0x46, 0x3, 0x46, 0x3, 0x47, 
    0x3, 0x47, 0x3, 0x47, 0x7, 0x47, 0x4e2, 0xa, 0x47, 0xc, 0x47, 0xe, 0x47, 
    0x4e5, 0xb, 0x47, 0x3, 0x48, 0x3, 0x48, 0x3, 0x49, 0x3, 0x49, 0x3, 0x49, 
    0x3, 0x49, 0x3, 0x4a, 0x3, 0x4a, 0x3, 0x4a, 0x7, 0x4a, 0x4f0, 0xa, 0x4a, 
    0xc, 0x4a, 0xe, 0x4a, 0x4f3, 0xb, 0x4a, 0x3, 0x4b, 0x3, 0x4b, 0x3, 0x4c, 
    0x3, 0x4c, 0x3, 0x4c, 0x7, 0x4c, 0x4fa, 0xa, 0x4c, 0xc, 0x4c, 0xe, 0x4c, 
    0x4fd, 0xb, 0x4c, 0x3, 0x4d, 0x3, 0x4d, 0x3, 0x4d, 0x5, 0x4d, 0x502, 
    0xa, 0x4d, 0x3, 0x4e, 0x3, 0x4e, 0x3, 0x4e, 0x7, 0x4e, 0x507, 0xa, 0x4e, 
    0xc, 0x4e, 0xe, 0x4e, 0x50a, 0xb, 0x4e, 0x3, 0x4f, 0x3, 0x4f, 0x3, 0x50, 
    0x3, 0x50, 0x3, 0x51, 0x3, 0x51, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 
    0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 
    0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 
    0x52, 0x6, 0x52, 0x530, 0xa, 0x52, 0xd, 0x52, 0xe, 0x52, 0x531, 0x3, 
    0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x6, 0x52, 0x539, 
    0xa, 0x52, 0xd, 0x52, 0xe, 0x52, 0x53a, 0x5, 0x52, 0x53d, 0xa, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 0x541, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 
    0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 
    0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x7, 0x52, 0x561, 0xa, 0x52, 0xc, 0x52, 0xe, 0x52, 
    0x564, 0xb, 0x52, 0x5, 0x52, 0x566, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x7, 0x52, 0x56e, 0xa, 0x52, 
    0xc, 0x52, 0xe, 0x52, 0x571, 0xb, 0x52, 0x5, 0x52, 0x573, 0xa, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x7, 
    0x52, 0x57b, 0xa, 0x52, 0xc, 0x52, 0xe, 0x52, 0x57e, 0xb, 0x52, 0x5, 
    0x52, 0x580, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 
    0x52, 0x58c, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 
    0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 
    0x59e, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x5, 0x52, 0x5a5, 0xa, 0x52, 0x5, 0x52, 0x5a7, 0xa, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 
    0x52, 0x5b0, 0xa, 0x52, 0x3, 0x52, 0x5, 0x52, 0x5b3, 0xa, 0x52, 0x3, 
    0x52, 0x5, 0x52, 0x5b6, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 
    0x5ba, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 
    0x52, 0x5, 0x52, 0x5c8, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 0x5d0, 0xa, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 
    0x52, 0x5d9, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 0x5e2, 0xa, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 
    0x52, 0x5eb, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 0x5f4, 0xa, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 
    0x52, 0x7, 0x52, 0x5fe, 0xa, 0x52, 0xc, 0x52, 0xe, 0x52, 0x601, 0xb, 
    0x52, 0x5, 0x52, 0x603, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 
    0x52, 0x3, 0x52, 0x5, 0x52, 0x610, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 0x619, 
    0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 
    0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 
    0x626, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 
    0x62c, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 
    0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x7, 0x52, 0x644, 0xa, 0x52, 
    0xc, 0x52, 0xe, 0x52, 0x647, 0xb, 0x52, 0x5, 0x52, 0x649, 0xa, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 0x650, 
    0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 0x655, 0xa, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 
    0x52, 0x5, 0x52, 0x65e, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x7, 0x52, 0x665, 0xa, 0x52, 0xc, 0x52, 0xe, 0x52, 
    0x668, 0xb, 0x52, 0x5, 0x52, 0x66a, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x5, 0x52, 0x66f, 0xa, 0x52, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x52, 0x3, 0x52, 0x7, 0x52, 0x676, 0xa, 0x52, 0xc, 0x52, 0xe, 0x52, 
    0x679, 0xb, 0x52, 0x3, 0x53, 0x3, 0x53, 0x3, 0x54, 0x3, 0x54, 0x3, 0x54, 
    0x5, 0x54, 0x680, 0xa, 0x54, 0x3, 0x54, 0x3, 0x54, 0x3, 0x54, 0x5, 0x54, 
    0x685, 0xa, 0x54, 0x3, 0x54, 0x3, 0x54, 0x3, 0x54, 0x5, 0x54, 0x68a, 
    0xa, 0x54, 0x3, 0x54, 0x3, 0x54, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 
    0x55, 0x691, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 
    0x696, 0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 0x69b, 
    0xa, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 0x7, 
    0x56, 0x6a2, 0xa, 0x56, 0xc, 0x56, 0xe, 0x56, 0x6a5, 0xb, 0x56, 0x3, 
    0x56, 0x3, 0x56, 0x3, 0x57, 0x3, 0x57, 0x3, 0x58, 0x3, 0x58, 0x3, 0x59, 
    0x3, 0x59, 0x3, 0x5a, 0x3, 0x5a, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5c, 0x3, 
    0x5c, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5e, 0x3, 0x5e, 0x3, 0x5f, 0x3, 0x5f, 
    0x3, 0x60, 0x3, 0x60, 0x3, 0x61, 0x3, 0x61, 0x3, 0x62, 0x3, 0x62, 0x3, 
    0x63, 0x3, 0x63, 0x3, 0x64, 0x3, 0x64, 0x5, 0x64, 0x6c5, 0xa, 0x64, 
    0x3, 0x65, 0x3, 0x65, 0x3, 0x65, 0x5, 0x65, 0x6ca, 0xa, 0x65, 0x3, 0x66, 
    0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x5, 0x66, 0x6d0, 0xa, 0x66, 0x3, 0x67, 
    0x3, 0x67, 0x3, 0x68, 0x3, 0x68, 0x3, 0x69, 0x3, 0x69, 0x3, 0x6a, 0x3, 
    0x6a, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 
    0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x6e4, 0xa, 0x6b, 
    0x3, 0x6c, 0x3, 0x6c, 0x3, 0x6c, 0x3, 0x6c, 0x3, 0x6c, 0x3, 0x6c, 0x5, 
    0x6c, 0x6ec, 0xa, 0x6c, 0x3, 0x6d, 0x3, 0x6d, 0x3, 0x6e, 0x3, 0x6e, 
    0x3, 0x6f, 0x3, 0x6f, 0x5, 0x6f, 0x6f4, 0xa, 0x6f, 0x3, 0x6f, 0x3, 0x6f, 
    0x3, 0x70, 0x3, 0x70, 0x3, 0x71, 0x3, 0x71, 0x5, 0x71, 0x6fc, 0xa, 0x71, 
    0x3, 0x71, 0x3, 0x71, 0x3, 0x72, 0x3, 0x72, 0x3, 0x73, 0x3, 0x73, 0x5, 
    0x73, 0x704, 0xa, 0x73, 0x3, 0x73, 0x3, 0x73, 0x3, 0x74, 0x3, 0x74, 
    0x3, 0x74, 0x3, 0x75, 0x3, 0x75, 0x3, 0x75, 0x3, 0x76, 0x3, 0x76, 0x3, 
    0x76, 0x3, 0x77, 0x3, 0x77, 0x3, 0x78, 0x3, 0x78, 0x5, 0x78, 0x715, 
    0xa, 0x78, 0x3, 0x79, 0x3, 0x79, 0x3, 0x79, 0x5, 0x79, 0x71a, 0xa, 0x79, 
    0x3, 0x79, 0x3, 0x79, 0x3, 0x79, 0x3, 0x79, 0x3, 0x79, 0x3, 0x79, 0x5, 
    0x79, 0x722, 0xa, 0x79, 0x3, 0x79, 0x3, 0x79, 0x3, 0x79, 0x3, 0x79, 
    0x3, 0x79, 0x5, 0x79, 0x729, 0xa, 0x79, 0x3, 0x79, 0x3, 0x79, 0x3, 0x79, 
    0x3, 0x79, 0x3, 0x79, 0x3, 0x79, 0x3, 0x79, 0x5, 0x79, 0x732, 0xa, 0x79, 
    0x5, 0x79, 0x734, 0xa, 0x79, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x5, 0x7a, 
    0x739, 0xa, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 
    0x3, 0x7a, 0x5, 0x7a, 0x741, 0xa, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 
    0x3, 0x7a, 0x3, 0x7a, 0x5, 0x7a, 0x748, 0xa, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 
    0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x5, 
    0x7a, 0x752, 0xa, 0x7a, 0x5, 0x7a, 0x754, 0xa, 0x7a, 0x3, 0x7a, 0x3, 
    0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x5, 0x7a, 0x75b, 0xa, 0x7a, 
    0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 
    0x7a, 0x5, 0x7a, 0x764, 0xa, 0x7a, 0x5, 0x7a, 0x766, 0xa, 0x7a, 0x3, 
    0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x5, 0x7a, 0x76d, 
    0xa, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 
    0x7a, 0x5, 0x7a, 0x775, 0xa, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 
    0x3, 0x7a, 0x3, 0x7a, 0x5, 0x7a, 0x77c, 0xa, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 
    0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x5, 0x7a, 0x783, 0xa, 0x7a, 0x3, 0x7a, 
    0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7a, 0x5, 0x7a, 0x78a, 0xa, 0x7a, 
    0x3, 0x7a, 0x3, 0x7a, 0x5, 0x7a, 0x78e, 0xa, 0x7a, 0x5, 0x7a, 0x790, 
    0xa, 0x7a, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7c, 0x3, 0x7c, 0x3, 
    0x7c, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7e, 0x3, 0x7e, 
    0x3, 0x7f, 0x3, 0x7f, 0x3, 0x7f, 0x2, 0x5, 0x2a, 0x30, 0xa2, 0x80, 0x2, 
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
    0xf6, 0xf8, 0xfa, 0xfc, 0x2, 0x21, 0x4, 0x2, 0x9, 0x9, 0xaf, 0xaf, 0x4, 
    0x2, 0xa, 0xa, 0x16, 0x16, 0x4, 0x2, 0x8e, 0x8e, 0x96, 0x96, 0x3, 0x2, 
    0xa1, 0xa2, 0x4, 0x2, 0xd, 0xd, 0x9b, 0x9b, 0x5, 0x2, 0x4d, 0x4d, 0x66, 
    0x66, 0xb8, 0xb8, 0x5, 0x2, 0x57, 0x57, 0x6d, 0x6d, 0x95, 0x95, 0x5, 
    0x2, 0x3d, 0x3d, 0x4c, 0x4c, 0x53, 0x53, 0x4, 0x2, 0x62, 0x62, 0x6b, 
    0x6b, 0x6, 0x2, 0x26, 0x26, 0x5a, 0x5a, 0x9e, 0x9e, 0xb1, 0xb1, 0x5, 
    0x2, 0x20, 0x20, 0x29, 0x29, 0x45, 0x45, 0x3, 0x2, 0x28, 0x29, 0x4, 
    0x2, 0x2d, 0x2d, 0x2f, 0x2f, 0x4, 0x2, 0x28, 0x29, 0x2d, 0x2e, 0x4, 
    0x2, 0x1b, 0x1b, 0x43, 0x43, 0x4, 0x2, 0x9, 0x9, 0xf, 0xf, 0x4, 0x2, 
    0x8, 0x8, 0x1a, 0x1a, 0x4, 0x2, 0x8, 0x8, 0x8c, 0x8c, 0x4, 0x2, 0x8, 
    0x8, 0x58, 0x58, 0x4, 0x2, 0x8, 0x8, 0x55, 0x55, 0x4, 0x2, 0x8, 0x8, 
    0x61, 0x61, 0x3, 0x2, 0xa4, 0xa5, 0x5, 0x2, 0x24, 0x24, 0x6c, 0x6c, 
    0xb3, 0xb3, 0x8, 0x2, 0x3e, 0x3e, 0x5d, 0x5d, 0x78, 0x78, 0x7b, 0x7b, 
    0x99, 0x99, 0xc9, 0xc9, 0x5, 0x2, 0x7, 0x7, 0xa, 0xa, 0x10, 0x10, 0x5, 
    0x2, 0x9, 0x9, 0xf, 0xf, 0x11, 0x11, 0x4, 0x2, 0x4, 0x6, 0x12, 0x15, 
    0x3, 0x2, 0x37, 0x3a, 0x4, 0x2, 0x51, 0x51, 0xb5, 0xb5, 0x3, 0x2, 0xcd, 
    0xcf, 0x31, 0x2, 0x1b, 0x1b, 0x1f, 0x20, 0x22, 0x23, 0x26, 0x26, 0x28, 
    0x29, 0x2c, 0x2c, 0x2e, 0x2e, 0x32, 0x32, 0x3b, 0x3b, 0x3d, 0x3d, 0x3f, 
    0x3f, 0x41, 0x41, 0x43, 0x45, 0x47, 0x47, 0x4c, 0x4c, 0x4f, 0x4f, 0x51, 
    0x53, 0x56, 0x56, 0x5a, 0x5b, 0x5e, 0x60, 0x62, 0x62, 0x6b, 0x6b, 0x6f, 
    0x71, 0x73, 0x73, 0x75, 0x75, 0x79, 0x7a, 0x7c, 0x7c, 0x7f, 0x7f, 0x81, 
    0x81, 0x84, 0x84, 0x8c, 0x8c, 0x8e, 0x8e, 0x90, 0x91, 0x93, 0x94, 0x97, 
    0x98, 0x9a, 0x9a, 0x9e, 0xa4, 0xa7, 0xa8, 0xaa, 0xab, 0xb0, 0xb1, 0xb4, 
    0xb4, 0xb6, 0xb7, 0xbc, 0xbc, 0xc1, 0xc2, 0xc5, 0xc6, 0xc8, 0xc8, 0xca, 
    0xca, 0x2, 0x88a, 0x2, 0xff, 0x3, 0x2, 0x2, 0x2, 0x4, 0x103, 0x3, 0x2, 
    0x2, 0x2, 0x6, 0x116, 0x3, 0x2, 0x2, 0x2, 0x8, 0x11c, 0x3, 0x2, 0x2, 
    0x2, 0xa, 0x11f, 0x3, 0x2, 0x2, 0x2, 0xc, 0x12c, 0x3, 0x2, 0x2, 0x2, 
    0xe, 0x131, 0x3, 0x2, 0x2, 0x2, 0x10, 0x141, 0x3, 0x2, 0x2, 0x2, 0x12, 
    0x143, 0x3, 0x2, 0x2, 0x2, 0x14, 0x14c, 0x3, 0x2, 0x2, 0x2, 0x16, 0x154, 
    0x3, 0x2, 0x2, 0x2, 0x18, 0x156, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x15c, 0x3, 
    0x2, 0x2, 0x2, 0x1c, 0x162, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x167, 0x3, 0x2, 
    0x2, 0x2, 0x20, 0x16e, 0x3, 0x2, 0x2, 0x2, 0x22, 0x1a2, 0x3, 0x2, 0x2, 
    0x2, 0x24, 0x1ad, 0x3, 0x2, 0x2, 0x2, 0x26, 0x1d8, 0x3, 0x2, 0x2, 0x2, 
    0x28, 0x1e3, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x1f9, 0x3, 0x2, 0x2, 0x2, 0x2c, 
    0x20a, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x213, 0x3, 0x2, 0x2, 0x2, 0x30, 0x23e, 
    0x3, 0x2, 0x2, 0x2, 0x32, 0x287, 0x3, 0x2, 0x2, 0x2, 0x34, 0x290, 0x3, 
    0x2, 0x2, 0x2, 0x36, 0x295, 0x3, 0x2, 0x2, 0x2, 0x38, 0x297, 0x3, 0x2, 
    0x2, 0x2, 0x3a, 0x2ae, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x2d0, 0x3, 0x2, 0x2, 
    0x2, 0x3e, 0x2e6, 0x3, 0x2, 0x2, 0x2, 0x40, 0x300, 0x3, 0x2, 0x2, 0x2, 
    0x42, 0x302, 0x3, 0x2, 0x2, 0x2, 0x44, 0x307, 0x3, 0x2, 0x2, 0x2, 0x46, 
    0x31c, 0x3, 0x2, 0x2, 0x2, 0x48, 0x31e, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x322, 
    0x3, 0x2, 0x2, 0x2, 0x4c, 0x32d, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x333, 0x3, 
    0x2, 0x2, 0x2, 0x50, 0x347, 0x3, 0x2, 0x2, 0x2, 0x52, 0x34c, 0x3, 0x2, 
    0x2, 0x2, 0x54, 0x34e, 0x3, 0x2, 0x2, 0x2, 0x56, 0x351, 0x3, 0x2, 0x2, 
    0x2, 0x58, 0x353, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x355, 0x3, 0x2, 0x2, 0x2, 
    0x5c, 0x359, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x35c, 0x3, 0x2, 0x2, 0x2, 0x60, 
    0x377, 0x3, 0x2, 0x2, 0x2, 0x62, 0x384, 0x3, 0x2, 0x2, 0x2, 0x64, 0x38d, 
    0x3, 0x2, 0x2, 0x2, 0x66, 0x399, 0x3, 0x2, 0x2, 0x2, 0x68, 0x3b8, 0x3, 
    0x2, 0x2, 0x2, 0x6a, 0x3ba, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x3fb, 0x3, 0x2, 
    0x2, 0x2, 0x6e, 0x3fd, 0x3, 0x2, 0x2, 0x2, 0x70, 0x419, 0x3, 0x2, 0x2, 
    0x2, 0x72, 0x46f, 0x3, 0x2, 0x2, 0x2, 0x74, 0x471, 0x3, 0x2, 0x2, 0x2, 
    0x76, 0x48d, 0x3, 0x2, 0x2, 0x2, 0x78, 0x493, 0x3, 0x2, 0x2, 0x2, 0x7a, 
    0x4aa, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x4ac, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x4bc, 
    0x3, 0x2, 0x2, 0x2, 0x80, 0x4be, 0x3, 0x2, 0x2, 0x2, 0x82, 0x4c6, 0x3, 
    0x2, 0x2, 0x2, 0x84, 0x4c8, 0x3, 0x2, 0x2, 0x2, 0x86, 0x4d0, 0x3, 0x2, 
    0x2, 0x2, 0x88, 0x4d4, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x4dc, 0x3, 0x2, 0x2, 
    0x2, 0x8c, 0x4de, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x4e6, 0x3, 0x2, 0x2, 0x2, 
    0x90, 0x4e8, 0x3, 0x2, 0x2, 0x2, 0x92, 0x4ec, 0x3, 0x2, 0x2, 0x2, 0x94, 
    0x4f4, 0x3, 0x2, 0x2, 0x2, 0x96, 0x4f6, 0x3, 0x2, 0x2, 0x2, 0x98, 0x4fe, 
    0x3, 0x2, 0x2, 0x2, 0x9a, 0x503, 0x3, 0x2, 0x2, 0x2, 0x9c, 0x50b, 0x3, 
    0x2, 0x2, 0x2, 0x9e, 0x50d, 0x3, 0x2, 0x2, 0x2, 0xa0, 0x50f, 0x3, 0x2, 
    0x2, 0x2, 0xa2, 0x618, 0x3, 0x2, 0x2, 0x2, 0xa4, 0x67a, 0x3, 0x2, 0x2, 
    0x2, 0xa6, 0x689, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x69a, 0x3, 0x2, 0x2, 0x2, 
    0xaa, 0x6a3, 0x3, 0x2, 0x2, 0x2, 0xac, 0x6a8, 0x3, 0x2, 0x2, 0x2, 0xae, 
    0x6aa, 0x3, 0x2, 0x2, 0x2, 0xb0, 0x6ac, 0x3, 0x2, 0x2, 0x2, 0xb2, 0x6ae, 
    0x3, 0x2, 0x2, 0x2, 0xb4, 0x6b0, 0x3, 0x2, 0x2, 0x2, 0xb6, 0x6b2, 0x3, 
    0x2, 0x2, 0x2, 0xb8, 0x6b4, 0x3, 0x2, 0x2, 0x2, 0xba, 0x6b6, 0x3, 0x2, 
    0x2, 0x2, 0xbc, 0x6b8, 0x3, 0x2, 0x2, 0x2, 0xbe, 0x6ba, 0x3, 0x2, 0x2, 
    0x2, 0xc0, 0x6bc, 0x3, 0x2, 0x2, 0x2, 0xc2, 0x6be, 0x3, 0x2, 0x2, 0x2, 
    0xc4, 0x6c0, 0x3, 0x2, 0x2, 0x2, 0xc6, 0x6c4, 0x3, 0x2, 0x2, 0x2, 0xc8, 
    0x6c9, 0x3, 0x2, 0x2, 0x2, 0xca, 0x6cf, 0x3, 0x2, 0x2, 0x2, 0xcc, 0x6d1, 
    0x3, 0x2, 0x2, 0x2, 0xce, 0x6d3, 0x3, 0x2, 0x2, 0x2, 0xd0, 0x6d5, 0x3, 
    0x2, 0x2, 0x2, 0xd2, 0x6d7, 0x3, 0x2, 0x2, 0x2, 0xd4, 0x6e3, 0x3, 0x2, 
    0x2, 0x2, 0xd6, 0x6eb, 0x3, 0x2, 0x2, 0x2, 0xd8, 0x6ed, 0x3, 0x2, 0x2, 
    0x2, 0xda, 0x6ef, 0x3, 0x2, 0x2, 0x2, 0xdc, 0x6f3, 0x3, 0x2, 0x2, 0x2, 
    0xde, 0x6f7, 0x3, 0x2, 0x2, 0x2, 0xe0, 0x6fb, 0x3, 0x2, 0x2, 0x2, 0xe2, 
    0x6ff, 0x3, 0x2, 0x2, 0x2, 0xe4, 0x703, 0x3, 0x2, 0x2, 0x2, 0xe6, 0x707, 
    0x3, 0x2, 0x2, 0x2, 0xe8, 0x70a, 0x3, 0x2, 0x2, 0x2, 0xea, 0x70d, 0x3, 
    0x2, 0x2, 0x2, 0xec, 0x710, 0x3, 0x2, 0x2, 0x2, 0xee, 0x714, 0x3, 0x2, 
    0x2, 0x2, 0xf0, 0x716, 0x3, 0x2, 0x2, 0x2, 0xf2, 0x735, 0x3, 0x2, 0x2, 
    0x2, 0xf4, 0x791, 0x3, 0x2, 0x2, 0x2, 0xf6, 0x794, 0x3, 0x2, 0x2, 0x2, 
    0xf8, 0x797, 0x3, 0x2, 0x2, 0x2, 0xfa, 0x79b, 0x3, 0x2, 0x2, 0x2, 0xfc, 
    0x79d, 0x3, 0x2, 0x2, 0x2, 0xfe, 0x100, 0x5, 0x4, 0x3, 0x2, 0xff, 0xfe, 
    0x3, 0x2, 0x2, 0x2, 0x100, 0x101, 0x3, 0x2, 0x2, 0x2, 0x101, 0xff, 0x3, 
    0x2, 0x2, 0x2, 0x101, 0x102, 0x3, 0x2, 0x2, 0x2, 0x102, 0x3, 0x3, 0x2, 
    0x2, 0x2, 0x103, 0x104, 0x5, 0x8, 0x5, 0x2, 0x104, 0x105, 0x5, 0x6, 
    0x4, 0x2, 0x105, 0x106, 0x7, 0x3, 0x2, 0x2, 0x106, 0x5, 0x3, 0x2, 0x2, 
    0x2, 0x107, 0x117, 0x5, 0x1c, 0xf, 0x2, 0x108, 0x117, 0x5, 0xe, 0x8, 
    0x2, 0x109, 0x117, 0x5, 0xf4, 0x7b, 0x2, 0x10a, 0x117, 0x5, 0xf6, 0x7c, 
    0x2, 0x10b, 0x117, 0x5, 0xf8, 0x7d, 0x2, 0x10c, 0x117, 0x5, 0xfa, 0x7e, 
    0x2, 0x10d, 0x117, 0x5, 0x40, 0x21, 0x2, 0x10e, 0x117, 0x5, 0x46, 0x24, 
    0x2, 0x10f, 0x117, 0x5, 0x4c, 0x27, 0x2, 0x110, 0x117, 0x5, 0x4e, 0x28, 
    0x2, 0x111, 0x117, 0x5, 0x54, 0x2b, 0x2, 0x112, 0x117, 0x5, 0x56, 0x2c, 
    0x2, 0x113, 0x117, 0x5, 0x58, 0x2d, 0x2, 0x114, 0x117, 0x5, 0x5a, 0x2e, 
    0x2, 0x115, 0x117, 0x5, 0x5c, 0x2f, 0x2, 0x116, 0x107, 0x3, 0x2, 0x2, 
    0x2, 0x116, 0x108, 0x3, 0x2, 0x2, 0x2, 0x116, 0x109, 0x3, 0x2, 0x2, 
    0x2, 0x116, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x116, 0x10b, 0x3, 0x2, 0x2, 
    0x2, 0x116, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x116, 0x10d, 0x3, 0x2, 0x2, 
    0x2, 0x116, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x116, 0x10f, 0x3, 0x2, 0x2, 
    0x2, 0x116, 0x110, 0x3, 0x2, 0x2, 0x2, 0x116, 0x111, 0x3, 0x2, 0x2, 
    0x2, 0x116, 0x112, 0x3, 0x2, 0x2, 0x2, 0x116, 0x113, 0x3, 0x2, 0x2, 
    0x2, 0x116, 0x114, 0x3, 0x2, 0x2, 0x2, 0x116, 0x115, 0x3, 0x2, 0x2, 
    0x2, 0x117, 0x7, 0x3, 0x2, 0x2, 0x2, 0x118, 0x11b, 0x5, 0xc, 0x7, 0x2, 
    0x119, 0x11b, 0x5, 0xa, 0x6, 0x2, 0x11a, 0x118, 0x3, 0x2, 0x2, 0x2, 
    0x11a, 0x119, 0x3, 0x2, 0x2, 0x2, 0x11b, 0x11e, 0x3, 0x2, 0x2, 0x2, 
    0x11c, 0x11a, 0x3, 0x2, 0x2, 0x2, 0x11c, 0x11d, 0x3, 0x2, 0x2, 0x2, 
    0x11d, 0x9, 0x3, 0x2, 0x2, 0x2, 0x11e, 0x11c, 0x3, 0x2, 0x2, 0x2, 0x11f, 
    0x120, 0x7, 0x4, 0x2, 0x2, 0x120, 0x121, 0x7, 0xcb, 0x2, 0x2, 0x121, 
    0x122, 0x7, 0x5, 0x2, 0x2, 0x122, 0x125, 0x7, 0x6, 0x2, 0x2, 0x123, 
    0x126, 0x5, 0xec, 0x77, 0x2, 0x124, 0x126, 0x7, 0xd2, 0x2, 0x2, 0x125, 
    0x123, 0x3, 0x2, 0x2, 0x2, 0x125, 0x124, 0x3, 0x2, 0x2, 0x2, 0x126, 
    0xb, 0x3, 0x2, 0x2, 0x2, 0x127, 0x128, 0x7, 0x7, 0x2, 0x2, 0x128, 0x12d, 
    0x7, 0xcb, 0x2, 0x2, 0x129, 0x12a, 0x7, 0x4, 0x2, 0x2, 0x12a, 0x12b, 
    0x7, 0xcb, 0x2, 0x2, 0x12b, 0x12d, 0x7, 0x5, 0x2, 0x2, 0x12c, 0x127, 
    0x3, 0x2, 0x2, 0x2, 0x12c, 0x129, 0x3, 0x2, 0x2, 0x2, 0x12d, 0xd, 0x3, 
    0x2, 0x2, 0x2, 0x12e, 0x130, 0x5, 0x20, 0x11, 0x2, 0x12f, 0x12e, 0x3, 
    0x2, 0x2, 0x2, 0x130, 0x133, 0x3, 0x2, 0x2, 0x2, 0x131, 0x12f, 0x3, 
    0x2, 0x2, 0x2, 0x131, 0x132, 0x3, 0x2, 0x2, 0x2, 0x132, 0x134, 0x3, 
    0x2, 0x2, 0x2, 0x133, 0x131, 0x3, 0x2, 0x2, 0x2, 0x134, 0x138, 0x7, 
    0x4f, 0x2, 0x2, 0x135, 0x137, 0x5, 0x10, 0x9, 0x2, 0x136, 0x135, 0x3, 
    0x2, 0x2, 0x2, 0x137, 0x13a, 0x3, 0x2, 0x2, 0x2, 0x138, 0x136, 0x3, 
    0x2, 0x2, 0x2, 0x138, 0x139, 0x3, 0x2, 0x2, 0x2, 0x139, 0x13b, 0x3, 
    0x2, 0x2, 0x2, 0x13a, 0x138, 0x3, 0x2, 0x2, 0x2, 0x13b, 0x13c, 0x5, 
    0x6, 0x4, 0x2, 0x13c, 0xf, 0x3, 0x2, 0x2, 0x2, 0x13d, 0x142, 0x5, 0x12, 
    0xa, 0x2, 0x13e, 0x142, 0x5, 0x18, 0xd, 0x2, 0x13f, 0x142, 0x7, 0xc5, 
    0x2, 0x2, 0x140, 0x142, 0x7, 0xc6, 0x2, 0x2, 0x141, 0x13d, 0x3, 0x2, 
    0x2, 0x2, 0x141, 0x13e, 0x3, 0x2, 0x2, 0x2, 0x141, 0x13f, 0x3, 0x2, 
    0x2, 0x2, 0x141, 0x140, 0x3, 0x2, 0x2, 0x2, 0x142, 0x11, 0x3, 0x2, 0x2, 
    0x2, 0x143, 0x144, 0x7, 0xa0, 0x2, 0x2, 0x144, 0x149, 0x5, 0x14, 0xb, 
    0x2, 0x145, 0x146, 0x7, 0x8, 0x2, 0x2, 0x146, 0x148, 0x5, 0x14, 0xb, 
    0x2, 0x147, 0x145, 0x3, 0x2, 0x2, 0x2, 0x148, 0x14b, 0x3, 0x2, 0x2, 
    0x2, 0x149, 0x147, 0x3, 0x2, 0x2, 0x2, 0x149, 0x14a, 0x3, 0x2, 0x2, 
    0x2, 0x14a, 0x13, 0x3, 0x2, 0x2, 0x2, 0x14b, 0x149, 0x3, 0x2, 0x2, 0x2, 
    0x14c, 0x14f, 0x5, 0x16, 0xc, 0x2, 0x14d, 0x14e, 0x9, 0x2, 0x2, 0x2, 
    0x14e, 0x150, 0x5, 0x16, 0xc, 0x2, 0x14f, 0x14d, 0x3, 0x2, 0x2, 0x2, 
    0x14f, 0x150, 0x3, 0x2, 0x2, 0x2, 0x150, 0x15, 0x3, 0x2, 0x2, 0x2, 0x151, 
    0x155, 0x9, 0x3, 0x2, 0x2, 0x152, 0x155, 0x7, 0xd2, 0x2, 0x2, 0x153, 
    0x155, 0x5, 0xca, 0x66, 0x2, 0x154, 0x151, 0x3, 0x2, 0x2, 0x2, 0x154, 
    0x152, 0x3, 0x2, 0x2, 0x2, 0x154, 0x153, 0x3, 0x2, 0x2, 0x2, 0x155, 
    0x17, 0x3, 0x2, 0x2, 0x2, 0x156, 0x157, 0x7, 0x56, 0x2, 0x2, 0x157, 
    0x158, 0x9, 0x4, 0x2, 0x2, 0x158, 0x19, 0x3, 0x2, 0x2, 0x2, 0x159, 0x15b, 
    0x5, 0x20, 0x11, 0x2, 0x15a, 0x159, 0x3, 0x2, 0x2, 0x2, 0x15b, 0x15e, 
    0x3, 0x2, 0x2, 0x2, 0x15c, 0x15a, 0x3, 0x2, 0x2, 0x2, 0x15c, 0x15d, 
    0x3, 0x2, 0x2, 0x2, 0x15d, 0x15f, 0x3, 0x2, 0x2, 0x2, 0x15e, 0x15c, 
    0x3, 0x2, 0x2, 0x2, 0x15f, 0x160, 0x9, 0x5, 0x2, 0x2, 0x160, 0x161, 
    0x5, 0x6, 0x4, 0x2, 0x161, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x162, 0x163, 0x5, 
    0x1e, 0x10, 0x2, 0x163, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x164, 0x166, 0x5, 
    0x20, 0x11, 0x2, 0x165, 0x164, 0x3, 0x2, 0x2, 0x2, 0x166, 0x169, 0x3, 
    0x2, 0x2, 0x2, 0x167, 0x165, 0x3, 0x2, 0x2, 0x2, 0x167, 0x168, 0x3, 
    0x2, 0x2, 0x2, 0x168, 0x16c, 0x3, 0x2, 0x2, 0x2, 0x169, 0x167, 0x3, 
    0x2, 0x2, 0x2, 0x16a, 0x16d, 0x5, 0x24, 0x13, 0x2, 0x16b, 0x16d, 0x5, 
    0x28, 0x15, 0x2, 0x16c, 0x16a, 0x3, 0x2, 0x2, 0x2, 0x16c, 0x16b, 0x3, 
    0x2, 0x2, 0x2, 0x16d, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x16e, 0x170, 0x7, 0xd3, 
    0x2, 0x2, 0x16f, 0x171, 0x5, 0x22, 0x12, 0x2, 0x170, 0x16f, 0x3, 0x2, 
    0x2, 0x2, 0x170, 0x171, 0x3, 0x2, 0x2, 0x2, 0x171, 0x176, 0x3, 0x2, 
    0x2, 0x2, 0x172, 0x173, 0x7, 0x8, 0x2, 0x2, 0x173, 0x175, 0x5, 0x22, 
    0x12, 0x2, 0x174, 0x172, 0x3, 0x2, 0x2, 0x2, 0x175, 0x178, 0x3, 0x2, 
    0x2, 0x2, 0x176, 0x174, 0x3, 0x2, 0x2, 0x2, 0x176, 0x177, 0x3, 0x2, 
    0x2, 0x2, 0x177, 0x179, 0x3, 0x2, 0x2, 0x2, 0x178, 0x176, 0x3, 0x2, 
    0x2, 0x2, 0x179, 0x17a, 0x7, 0xd4, 0x2, 0x2, 0x17a, 0x21, 0x3, 0x2, 
    0x2, 0x2, 0x17b, 0x1a3, 0x5, 0xc2, 0x62, 0x2, 0x17c, 0x17d, 0x5, 0xc4, 
    0x63, 0x2, 0x17d, 0x17e, 0x7, 0x6, 0x2, 0x2, 0x17e, 0x17f, 0x5, 0xa2, 
    0x52, 0x2, 0x17f, 0x1a3, 0x3, 0x2, 0x2, 0x2, 0x180, 0x182, 0x5, 0xc4, 
    0x63, 0x2, 0x181, 0x183, 0x7, 0x6, 0x2, 0x2, 0x182, 0x181, 0x3, 0x2, 
    0x2, 0x2, 0x182, 0x183, 0x3, 0x2, 0x2, 0x2, 0x183, 0x184, 0x3, 0x2, 
    0x2, 0x2, 0x184, 0x185, 0x7, 0xb, 0x2, 0x2, 0x185, 0x18a, 0x5, 0xa2, 
    0x52, 0x2, 0x186, 0x187, 0x7, 0x8, 0x2, 0x2, 0x187, 0x189, 0x5, 0xa2, 
    0x52, 0x2, 0x188, 0x186, 0x3, 0x2, 0x2, 0x2, 0x189, 0x18c, 0x3, 0x2, 
    0x2, 0x2, 0x18a, 0x188, 0x3, 0x2, 0x2, 0x2, 0x18a, 0x18b, 0x3, 0x2, 
    0x2, 0x2, 0x18b, 0x18d, 0x3, 0x2, 0x2, 0x2, 0x18c, 0x18a, 0x3, 0x2, 
    0x2, 0x2, 0x18d, 0x18e, 0x7, 0xc, 0x2, 0x2, 0x18e, 0x1a3, 0x3, 0x2, 
    0x2, 0x2, 0x18f, 0x190, 0x5, 0xc2, 0x62, 0x2, 0x190, 0x191, 0x7, 0x6, 
    0x2, 0x2, 0x191, 0x192, 0x5, 0xc8, 0x65, 0x2, 0x192, 0x1a3, 0x3, 0x2, 
    0x2, 0x2, 0x193, 0x195, 0x5, 0xc2, 0x62, 0x2, 0x194, 0x196, 0x7, 0x6, 
    0x2, 0x2, 0x195, 0x194, 0x3, 0x2, 0x2, 0x2, 0x195, 0x196, 0x3, 0x2, 
    0x2, 0x2, 0x196, 0x197, 0x3, 0x2, 0x2, 0x2, 0x197, 0x198, 0x7, 0xb, 
    0x2, 0x2, 0x198, 0x19d, 0x5, 0xc8, 0x65, 0x2, 0x199, 0x19a, 0x7, 0x8, 
    0x2, 0x2, 0x19a, 0x19c, 0x5, 0xc8, 0x65, 0x2, 0x19b, 0x199, 0x3, 0x2, 
    0x2, 0x2, 0x19c, 0x19f, 0x3, 0x2, 0x2, 0x2, 0x19d, 0x19b, 0x3, 0x2, 
    0x2, 0x2, 0x19d, 0x19e, 0x3, 0x2, 0x2, 0x2, 0x19e, 0x1a0, 0x3, 0x2, 
    0x2, 0x2, 0x19f, 0x19d, 0x3, 0x2, 0x2, 0x2, 0x1a0, 0x1a1, 0x7, 0xc, 
    0x2, 0x2, 0x1a1, 0x1a3, 0x3, 0x2, 0x2, 0x2, 0x1a2, 0x17b, 0x3, 0x2, 
    0x2, 0x2, 0x1a2, 0x17c, 0x3, 0x2, 0x2, 0x2, 0x1a2, 0x180, 0x3, 0x2, 
    0x2, 0x2, 0x1a2, 0x18f, 0x3, 0x2, 0x2, 0x2, 0x1a2, 0x193, 0x3, 0x2, 
    0x2, 0x2, 0x1a3, 0x23, 0x3, 0x2, 0x2, 0x2, 0x1a4, 0x1a5, 0x7, 0xc7, 
    0x2, 0x2, 0x1a5, 0x1aa, 0x5, 0x2e, 0x18, 0x2, 0x1a6, 0x1a7, 0x7, 0x8, 
    0x2, 0x2, 0x1a7, 0x1a9, 0x5, 0x2e, 0x18, 0x2, 0x1a8, 0x1a6, 0x3, 0x2, 
    0x2, 0x2, 0x1a9, 0x1ac, 0x3, 0x2, 0x2, 0x2, 0x1aa, 0x1a8, 0x3, 0x2, 
    0x2, 0x2, 0x1aa, 0x1ab, 0x3, 0x2, 0x2, 0x2, 0x1ab, 0x1ae, 0x3, 0x2, 
    0x2, 0x2, 0x1ac, 0x1aa, 0x3, 0x2, 0x2, 0x2, 0x1ad, 0x1a4, 0x3, 0x2, 
    0x2, 0x2, 0x1ad, 0x1ae, 0x3, 0x2, 0x2, 0x2, 0x1ae, 0x1af, 0x3, 0x2, 
    0x2, 0x2, 0x1af, 0x1b3, 0x5, 0x26, 0x14, 0x2, 0x1b0, 0x1b1, 0x7, 0x87, 
    0x2, 0x2, 0x1b1, 0x1b2, 0x7, 0x27, 0x2, 0x2, 0x1b2, 0x1b4, 0x5, 0x84, 
    0x43, 0x2, 0x1b3, 0x1b0, 0x3, 0x2, 0x2, 0x2, 0x1b3, 0x1b4, 0x3, 0x2, 
    0x2, 0x2, 0x1b4, 0x1bb, 0x3, 0x2, 0x2, 0x2, 0x1b5, 0x1b6, 0x7, 0x6f, 
    0x2, 0x2, 0x1b6, 0x1b9, 0x7, 0xd2, 0x2, 0x2, 0x1b7, 0x1b8, 0x7, 0x84, 
    0x2, 0x2, 0x1b8, 0x1ba, 0x7, 0xd2, 0x2, 0x2, 0x1b9, 0x1b7, 0x3, 0x2, 
    0x2, 0x2, 0x1b9, 0x1ba, 0x3, 0x2, 0x2, 0x2, 0x1ba, 0x1bc, 0x3, 0x2, 
    0x2, 0x2, 0x1bb, 0x1b5, 0x3, 0x2, 0x2, 0x2, 0x1bb, 0x1bc, 0x3, 0x2, 
    0x2, 0x2, 0x1bc, 0x25, 0x3, 0x2, 0x2, 0x2, 0x1bd, 0x1c0, 0x9, 0x6, 0x2, 
    0x2, 0x1be, 0x1c1, 0x7, 0x16, 0x2, 0x2, 0x1bf, 0x1c1, 0x7, 0x46, 0x2, 
    0x2, 0x1c0, 0x1be, 0x3, 0x2, 0x2, 0x2, 0x1c0, 0x1bf, 0x3, 0x2, 0x2, 
    0x2, 0x1c0, 0x1c1, 0x3, 0x2, 0x2, 0x2, 0x1c1, 0x1c2, 0x3, 0x2, 0x2, 
    0x2, 0x1c2, 0x1c5, 0x5, 0x7c, 0x3f, 0x2, 0x1c3, 0x1c4, 0x7, 0x58, 0x2, 
    0x2, 0x1c4, 0x1c6, 0x5, 0x30, 0x19, 0x2, 0x1c5, 0x1c3, 0x3, 0x2, 0x2, 
    0x2, 0x1c5, 0x1c6, 0x3, 0x2, 0x2, 0x2, 0x1c6, 0x1c9, 0x3, 0x2, 0x2, 
    0x2, 0x1c7, 0x1c8, 0x7, 0xc4, 0x2, 0x2, 0x1c8, 0x1ca, 0x5, 0x9c, 0x4f, 
    0x2, 0x1c9, 0x1c7, 0x3, 0x2, 0x2, 0x2, 0x1c9, 0x1ca, 0x3, 0x2, 0x2, 
    0x2, 0x1ca, 0x1ce, 0x3, 0x2, 0x2, 0x2, 0x1cb, 0x1cc, 0x7, 0x59, 0x2, 
    0x2, 0x1cc, 0x1cd, 0x7, 0x27, 0x2, 0x2, 0x1cd, 0x1cf, 0x5, 0x80, 0x41, 
    0x2, 0x1ce, 0x1cb, 0x3, 0x2, 0x2, 0x2, 0x1ce, 0x1cf, 0x3, 0x2, 0x2, 
    0x2, 0x1cf, 0x1d2, 0x3, 0x2, 0x2, 0x2, 0x1d0, 0x1d1, 0x7, 0x5c, 0x2, 
    0x2, 0x1d1, 0x1d3, 0x5, 0x9c, 0x4f, 0x2, 0x1d2, 0x1d0, 0x3, 0x2, 0x2, 
    0x2, 0x1d2, 0x1d3, 0x3, 0x2, 0x2, 0x2, 0x1d3, 0x1d9, 0x3, 0x2, 0x2, 
    0x2, 0x1d4, 0x1d5, 0x7, 0xb, 0x2, 0x2, 0x1d5, 0x1d6, 0x5, 0x26, 0x14, 
    0x2, 0x1d6, 0x1d7, 0x7, 0xc, 0x2, 0x2, 0x1d7, 0x1d9, 0x3, 0x2, 0x2, 
    0x2, 0x1d8, 0x1bd, 0x3, 0x2, 0x2, 0x2, 0x1d8, 0x1d4, 0x3, 0x2, 0x2, 
    0x2, 0x1d9, 0x27, 0x3, 0x2, 0x2, 0x2, 0x1da, 0x1db, 0x7, 0xc7, 0x2, 
    0x2, 0x1db, 0x1e0, 0x5, 0x2e, 0x18, 0x2, 0x1dc, 0x1dd, 0x7, 0x8, 0x2, 
    0x2, 0x1dd, 0x1df, 0x5, 0x2e, 0x18, 0x2, 0x1de, 0x1dc, 0x3, 0x2, 0x2, 
    0x2, 0x1df, 0x1e2, 0x3, 0x2, 0x2, 0x2, 0x1e0, 0x1de, 0x3, 0x2, 0x2, 
    0x2, 0x1e0, 0x1e1, 0x3, 0x2, 0x2, 0x2, 0x1e1, 0x1e4, 0x3, 0x2, 0x2, 
    0x2, 0x1e2, 0x1e0, 0x3, 0x2, 0x2, 0x2, 0x1e3, 0x1da, 0x3, 0x2, 0x2, 
    0x2, 0x1e3, 0x1e4, 0x3, 0x2, 0x2, 0x2, 0x1e4, 0x1e5, 0x3, 0x2, 0x2, 
    0x2, 0x1e5, 0x1e9, 0x5, 0x2a, 0x16, 0x2, 0x1e6, 0x1e7, 0x7, 0x87, 0x2, 
    0x2, 0x1e7, 0x1e8, 0x7, 0x27, 0x2, 0x2, 0x1e8, 0x1ea, 0x5, 0x84, 0x43, 
    0x2, 0x1e9, 0x1e6, 0x3, 0x2, 0x2, 0x2, 0x1e9, 0x1ea, 0x3, 0x2, 0x2, 
    0x2, 0x1ea, 0x1f1, 0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1ec, 0x7, 0x6f, 0x2, 
    0x2, 0x1ec, 0x1ef, 0x7, 0xd2, 0x2, 0x2, 0x1ed, 0x1ee, 0x7, 0x84, 0x2, 
    0x2, 0x1ee, 0x1f0, 0x7, 0xd2, 0x2, 0x2, 0x1ef, 0x1ed, 0x3, 0x2, 0x2, 
    0x2, 0x1ef, 0x1f0, 0x3, 0x2, 0x2, 0x2, 0x1f0, 0x1f2, 0x3, 0x2, 0x2, 
    0x2, 0x1f1, 0x1eb, 0x3, 0x2, 0x2, 0x2, 0x1f1, 0x1f2, 0x3, 0x2, 0x2, 
    0x2, 0x1f2, 0x29, 0x3, 0x2, 0x2, 0x2, 0x1f3, 0x1f4, 0x8, 0x16, 0x1, 
    0x2, 0x1f4, 0x1fa, 0x5, 0x26, 0x14, 0x2, 0x1f5, 0x1f6, 0x7, 0xb, 0x2, 
    0x2, 0x1f6, 0x1f7, 0x5, 0x2a, 0x16, 0x2, 0x1f7, 0x1f8, 0x7, 0xc, 0x2, 
    0x2, 0x1f8, 0x1fa, 0x3, 0x2, 0x2, 0x2, 0x1f9, 0x1f3, 0x3, 0x2, 0x2, 
    0x2, 0x1f9, 0x1f5, 0x3, 0x2, 0x2, 0x2, 0x1fa, 0x207, 0x3, 0x2, 0x2, 
    0x2, 0x1fb, 0x1fc, 0xc, 0x4, 0x2, 0x2, 0x1fc, 0x1ff, 0x9, 0x7, 0x2, 
    0x2, 0x1fd, 0x200, 0x7, 0x46, 0x2, 0x2, 0x1fe, 0x200, 0x7, 0x16, 0x2, 
    0x2, 0x1ff, 0x1fd, 0x3, 0x2, 0x2, 0x2, 0x1ff, 0x1fe, 0x3, 0x2, 0x2, 
    0x2, 0x1ff, 0x200, 0x3, 0x2, 0x2, 0x2, 0x200, 0x202, 0x3, 0x2, 0x2, 
    0x2, 0x201, 0x203, 0x5, 0x2c, 0x17, 0x2, 0x202, 0x201, 0x3, 0x2, 0x2, 
    0x2, 0x202, 0x203, 0x3, 0x2, 0x2, 0x2, 0x203, 0x204, 0x3, 0x2, 0x2, 
    0x2, 0x204, 0x206, 0x5, 0x2a, 0x16, 0x5, 0x205, 0x1fb, 0x3, 0x2, 0x2, 
    0x2, 0x206, 0x209, 0x3, 0x2, 0x2, 0x2, 0x207, 0x205, 0x3, 0x2, 0x2, 
    0x2, 0x207, 0x208, 0x3, 0x2, 0x2, 0x2, 0x208, 0x2b, 0x3, 0x2, 0x2, 0x2, 
    0x209, 0x207, 0x3, 0x2, 0x2, 0x2, 0x20a, 0x210, 0x7, 0x33, 0x2, 0x2, 
    0x20b, 0x20c, 0x7, 0x27, 0x2, 0x2, 0x20c, 0x20d, 0x7, 0xb, 0x2, 0x2, 
    0x20d, 0x20e, 0x5, 0x8c, 0x47, 0x2, 0x20e, 0x20f, 0x7, 0xc, 0x2, 0x2, 
    0x20f, 0x211, 0x3, 0x2, 0x2, 0x2, 0x210, 0x20b, 0x3, 0x2, 0x2, 0x2, 
    0x210, 0x211, 0x3, 0x2, 0x2, 0x2, 0x211, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x212, 
    0x214, 0x7, 0x92, 0x2, 0x2, 0x213, 0x212, 0x3, 0x2, 0x2, 0x2, 0x213, 
    0x214, 0x3, 0x2, 0x2, 0x2, 0x214, 0x215, 0x3, 0x2, 0x2, 0x2, 0x215, 
    0x217, 0x5, 0xb4, 0x5b, 0x2, 0x216, 0x218, 0x5, 0x90, 0x49, 0x2, 0x217, 
    0x216, 0x3, 0x2, 0x2, 0x2, 0x217, 0x218, 0x3, 0x2, 0x2, 0x2, 0x218, 
    0x219, 0x3, 0x2, 0x2, 0x2, 0x219, 0x21a, 0x7, 0x1a, 0x2, 0x2, 0x21a, 
    0x21b, 0x7, 0xb, 0x2, 0x2, 0x21b, 0x21c, 0x5, 0x1e, 0x10, 0x2, 0x21c, 
    0x21d, 0x7, 0xc, 0x2, 0x2, 0x21d, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x21e, 0x21f, 
    0x8, 0x19, 0x1, 0x2, 0x21f, 0x224, 0x5, 0x32, 0x1a, 0x2, 0x220, 0x222, 
    0x7, 0x1a, 0x2, 0x2, 0x221, 0x220, 0x3, 0x2, 0x2, 0x2, 0x221, 0x222, 
    0x3, 0x2, 0x2, 0x2, 0x222, 0x223, 0x3, 0x2, 0x2, 0x2, 0x223, 0x225, 
    0x5, 0xb6, 0x5c, 0x2, 0x224, 0x221, 0x3, 0x2, 0x2, 0x2, 0x224, 0x225, 
    0x3, 0x2, 0x2, 0x2, 0x225, 0x23f, 0x3, 0x2, 0x2, 0x2, 0x226, 0x227, 
    0x7, 0xb, 0x2, 0x2, 0x227, 0x228, 0x5, 0x1e, 0x10, 0x2, 0x228, 0x230, 
    0x7, 0xc, 0x2, 0x2, 0x229, 0x22b, 0x7, 0x1a, 0x2, 0x2, 0x22a, 0x229, 
    0x3, 0x2, 0x2, 0x2, 0x22a, 0x22b, 0x3, 0x2, 0x2, 0x2, 0x22b, 0x22c, 
    0x3, 0x2, 0x2, 0x2, 0x22c, 0x22e, 0x5, 0xb6, 0x5c, 0x2, 0x22d, 0x22f, 
    0x5, 0x90, 0x49, 0x2, 0x22e, 0x22d, 0x3, 0x2, 0x2, 0x2, 0x22e, 0x22f, 
    0x3, 0x2, 0x2, 0x2, 0x22f, 0x231, 0x3, 0x2, 0x2, 0x2, 0x230, 0x22a, 
    0x3, 0x2, 0x2, 0x2, 0x230, 0x231, 0x3, 0x2, 0x2, 0x2, 0x231, 0x23f, 
    0x3, 0x2, 0x2, 0x2, 0x232, 0x233, 0x7, 0xb, 0x2, 0x2, 0x233, 0x234, 
    0x5, 0x38, 0x1d, 0x2, 0x234, 0x23c, 0x7, 0xc, 0x2, 0x2, 0x235, 0x237, 
    0x7, 0x1a, 0x2, 0x2, 0x236, 0x235, 0x3, 0x2, 0x2, 0x2, 0x236, 0x237, 
    0x3, 0x2, 0x2, 0x2, 0x237, 0x238, 0x3, 0x2, 0x2, 0x2, 0x238, 0x23a, 
    0x5, 0xb6, 0x5c, 0x2, 0x239, 0x23b, 0x5, 0x90, 0x49, 0x2, 0x23a, 0x239, 
    0x3, 0x2, 0x2, 0x2, 0x23a, 0x23b, 0x3, 0x2, 0x2, 0x2, 0x23b, 0x23d, 
    0x3, 0x2, 0x2, 0x2, 0x23c, 0x236, 0x3, 0x2, 0x2, 0x2, 0x23c, 0x23d, 
    0x3, 0x2, 0x2, 0x2, 0x23d, 0x23f, 0x3, 0x2, 0x2, 0x2, 0x23e, 0x21e, 
    0x3, 0x2, 0x2, 0x2, 0x23e, 0x226, 0x3, 0x2, 0x2, 0x2, 0x23e, 0x232, 
    0x3, 0x2, 0x2, 0x2, 0x23f, 0x27c, 0x3, 0x2, 0x2, 0x2, 0x240, 0x244, 
    0xc, 0x8, 0x2, 0x2, 0x241, 0x243, 0x5, 0x20, 0x11, 0x2, 0x242, 0x241, 
    0x3, 0x2, 0x2, 0x2, 0x243, 0x246, 0x3, 0x2, 0x2, 0x2, 0x244, 0x242, 
    0x3, 0x2, 0x2, 0x2, 0x244, 0x245, 0x3, 0x2, 0x2, 0x2, 0x245, 0x24a, 
    0x3, 0x2, 0x2, 0x2, 0x246, 0x244, 0x3, 0x2, 0x2, 0x2, 0x247, 0x24b, 
    0x7, 0x8, 0x2, 0x2, 0x248, 0x249, 0x7, 0x36, 0x2, 0x2, 0x249, 0x24b, 
    0x7, 0x6a, 0x2, 0x2, 0x24a, 0x247, 0x3, 0x2, 0x2, 0x2, 0x24a, 0x248, 
    0x3, 0x2, 0x2, 0x2, 0x24b, 0x24c, 0x3, 0x2, 0x2, 0x2, 0x24c, 0x27b, 
    0x5, 0x30, 0x19, 0x9, 0x24d, 0x251, 0xc, 0x7, 0x2, 0x2, 0x24e, 0x250, 
    0x5, 0x20, 0x11, 0x2, 0x24f, 0x24e, 0x3, 0x2, 0x2, 0x2, 0x250, 0x253, 
    0x3, 0x2, 0x2, 0x2, 0x251, 0x24f, 0x3, 0x2, 0x2, 0x2, 0x251, 0x252, 
    0x3, 0x2, 0x2, 0x2, 0x252, 0x254, 0x3, 0x2, 0x2, 0x2, 0x253, 0x251, 
    0x3, 0x2, 0x2, 0x2, 0x254, 0x256, 0x7, 0x7d, 0x2, 0x2, 0x255, 0x257, 
    0x5, 0x34, 0x1b, 0x2, 0x256, 0x255, 0x3, 0x2, 0x2, 0x2, 0x256, 0x257, 
    0x3, 0x2, 0x2, 0x2, 0x257, 0x258, 0x3, 0x2, 0x2, 0x2, 0x258, 0x259, 
    0x7, 0x6a, 0x2, 0x2, 0x259, 0x27b, 0x5, 0x30, 0x19, 0x8, 0x25a, 0x25e, 
    0xc, 0x6, 0x2, 0x2, 0x25b, 0x25d, 0x5, 0x20, 0x11, 0x2, 0x25c, 0x25b, 
    0x3, 0x2, 0x2, 0x2, 0x25d, 0x260, 0x3, 0x2, 0x2, 0x2, 0x25e, 0x25c, 
    0x3, 0x2, 0x2, 0x2, 0x25e, 0x25f, 0x3, 0x2, 0x2, 0x2, 0x25f, 0x262, 
    0x3, 0x2, 0x2, 0x2, 0x260, 0x25e, 0x3, 0x2, 0x2, 0x2, 0x261, 0x263, 
    0x5, 0x36, 0x1c, 0x2, 0x262, 0x261, 0x3, 0x2, 0x2, 0x2, 0x262, 0x263, 
    0x3, 0x2, 0x2, 0x2, 0x263, 0x264, 0x3, 0x2, 0x2, 0x2, 0x264, 0x265, 
    0x7, 0x6a, 0x2, 0x2, 0x265, 0x266, 0x5, 0x30, 0x19, 0x2, 0x266, 0x267, 
    0x7, 0x85, 0x2, 0x2, 0x267, 0x268, 0x5, 0x9e, 0x50, 0x2, 0x268, 0x27b, 
    0x3, 0x2, 0x2, 0x2, 0x269, 0x26d, 0xc, 0x5, 0x2, 0x2, 0x26a, 0x26c, 
    0x5, 0x20, 0x11, 0x2, 0x26b, 0x26a, 0x3, 0x2, 0x2, 0x2, 0x26c, 0x26f, 
    0x3, 0x2, 0x2, 0x2, 0x26d, 0x26b, 0x3, 0x2, 0x2, 0x2, 0x26d, 0x26e, 
    0x3, 0x2, 0x2, 0x2, 0x26e, 0x271, 0x3, 0x2, 0x2, 0x2, 0x26f, 0x26d, 
    0x3, 0x2, 0x2, 0x2, 0x270, 0x272, 0x5, 0x36, 0x1c, 0x2, 0x271, 0x270, 
    0x3, 0x2, 0x2, 0x2, 0x271, 0x272, 0x3, 0x2, 0x2, 0x2, 0x272, 0x273, 
    0x3, 0x2, 0x2, 0x2, 0x273, 0x274, 0x7, 0x6a, 0x2, 0x2, 0x274, 0x275, 
    0x5, 0x30, 0x19, 0x2, 0x275, 0x276, 0x7, 0xbb, 0x2, 0x2, 0x276, 0x277, 
    0x7, 0xb, 0x2, 0x2, 0x277, 0x278, 0x5, 0x88, 0x45, 0x2, 0x278, 0x279, 
    0x7, 0xc, 0x2, 0x2, 0x279, 0x27b, 0x3, 0x2, 0x2, 0x2, 0x27a, 0x240, 
    0x3, 0x2, 0x2, 0x2, 0x27a, 0x24d, 0x3, 0x2, 0x2, 0x2, 0x27a, 0x25a, 
    0x3, 0x2, 0x2, 0x2, 0x27a, 0x269, 0x3, 0x2, 0x2, 0x2, 0x27b, 0x27e, 
    0x3, 0x2, 0x2, 0x2, 0x27c, 0x27a, 0x3, 0x2, 0x2, 0x2, 0x27c, 0x27d, 
    0x3, 0x2, 0x2, 0x2, 0x27d, 0x31, 0x3, 0x2, 0x2, 0x2, 0x27e, 0x27c, 0x3, 
    0x2, 0x2, 0x2, 0x27f, 0x280, 0x5, 0xb0, 0x59, 0x2, 0x280, 0x281, 0x7, 
    0xe, 0x2, 0x2, 0x281, 0x283, 0x3, 0x2, 0x2, 0x2, 0x282, 0x27f, 0x3, 
    0x2, 0x2, 0x2, 0x282, 0x283, 0x3, 0x2, 0x2, 0x2, 0x283, 0x284, 0x3, 
    0x2, 0x2, 0x2, 0x284, 0x285, 0x5, 0xb2, 0x5a, 0x2, 0x285, 0x286, 0x7, 
    0xe, 0x2, 0x2, 0x286, 0x288, 0x3, 0x2, 0x2, 0x2, 0x287, 0x282, 0x3, 
    0x2, 0x2, 0x2, 0x287, 0x288, 0x3, 0x2, 0x2, 0x2, 0x288, 0x289, 0x3, 
    0x2, 0x2, 0x2, 0x289, 0x28a, 0x5, 0xb4, 0x5b, 0x2, 0x28a, 0x33, 0x3, 
    0x2, 0x2, 0x2, 0x28b, 0x291, 0x7, 0x63, 0x2, 0x2, 0x28c, 0x28e, 0x9, 
    0x8, 0x2, 0x2, 0x28d, 0x28f, 0x7, 0x88, 0x2, 0x2, 0x28e, 0x28d, 0x3, 
    0x2, 0x2, 0x2, 0x28e, 0x28f, 0x3, 0x2, 0x2, 0x2, 0x28f, 0x291, 0x3, 
    0x2, 0x2, 0x2, 0x290, 0x28b, 0x3, 0x2, 0x2, 0x2, 0x290, 0x28c, 0x3, 
    0x2, 0x2, 0x2, 0x291, 0x35, 0x3, 0x2, 0x2, 0x2, 0x292, 0x296, 0x5, 0x34, 
    0x1b, 0x2, 0x293, 0x296, 0x7, 0x9c, 0x2, 0x2, 0x294, 0x296, 0x7, 0x18, 
    0x2, 0x2, 0x295, 0x292, 0x3, 0x2, 0x2, 0x2, 0x295, 0x293, 0x3, 0x2, 
    0x2, 0x2, 0x295, 0x294, 0x3, 0x2, 0x2, 0x2, 0x296, 0x37, 0x3, 0x2, 0x2, 
    0x2, 0x297, 0x298, 0x7, 0xbd, 0x2, 0x2, 0x298, 0x299, 0x7, 0xb, 0x2, 
    0x2, 0x299, 0x29a, 0x5, 0x9a, 0x4e, 0x2, 0x29a, 0x2a2, 0x7, 0xc, 0x2, 
    0x2, 0x29b, 0x29c, 0x7, 0x8, 0x2, 0x2, 0x29c, 0x29d, 0x7, 0xb, 0x2, 
    0x2, 0x29d, 0x29e, 0x5, 0x9a, 0x4e, 0x2, 0x29e, 0x29f, 0x7, 0xc, 0x2, 
    0x2, 0x29f, 0x2a1, 0x3, 0x2, 0x2, 0x2, 0x2a0, 0x29b, 0x3, 0x2, 0x2, 
    0x2, 0x2a1, 0x2a4, 0x3, 0x2, 0x2, 0x2, 0x2a2, 0x2a0, 0x3, 0x2, 0x2, 
    0x2, 0x2a2, 0x2a3, 0x3, 0x2, 0x2, 0x2, 0x2a3, 0x39, 0x3, 0x2, 0x2, 0x2, 
    0x2a4, 0x2a2, 0x3, 0x2, 0x2, 0x2, 0x2a5, 0x2a6, 0x7, 0xc7, 0x2, 0x2, 
    0x2a6, 0x2ab, 0x5, 0x2e, 0x18, 0x2, 0x2a7, 0x2a8, 0x7, 0x8, 0x2, 0x2, 
    0x2a8, 0x2aa, 0x5, 0x2e, 0x18, 0x2, 0x2a9, 0x2a7, 0x3, 0x2, 0x2, 0x2, 
    0x2aa, 0x2ad, 0x3, 0x2, 0x2, 0x2, 0x2ab, 0x2a9, 0x3, 0x2, 0x2, 0x2, 
    0x2ab, 0x2ac, 0x3, 0x2, 0x2, 0x2, 0x2ac, 0x2af, 0x3, 0x2, 0x2, 0x2, 
    0x2ad, 0x2ab, 0x3, 0x2, 0x2, 0x2, 0x2ae, 0x2a5, 0x3, 0x2, 0x2, 0x2, 
    0x2ae, 0x2af, 0x3, 0x2, 0x2, 0x2, 0x2af, 0x2b0, 0x3, 0x2, 0x2, 0x2, 
    0x2b0, 0x2b2, 0x7, 0x64, 0x2, 0x2, 0x2b1, 0x2b3, 0x7, 0x68, 0x2, 0x2, 
    0x2b2, 0x2b1, 0x3, 0x2, 0x2, 0x2, 0x2b2, 0x2b3, 0x3, 0x2, 0x2, 0x2, 
    0x2b3, 0x2b4, 0x3, 0x2, 0x2, 0x2, 0x2b4, 0x2c0, 0x5, 0x32, 0x1a, 0x2, 
    0x2b5, 0x2b6, 0x7, 0xb, 0x2, 0x2, 0x2b6, 0x2bb, 0x5, 0xb8, 0x5d, 0x2, 
    0x2b7, 0x2b8, 0x7, 0x8, 0x2, 0x2, 0x2b8, 0x2ba, 0x5, 0xb8, 0x5d, 0x2, 
    0x2b9, 0x2b7, 0x3, 0x2, 0x2, 0x2, 0x2ba, 0x2bd, 0x3, 0x2, 0x2, 0x2, 
    0x2bb, 0x2b9, 0x3, 0x2, 0x2, 0x2, 0x2bb, 0x2bc, 0x3, 0x2, 0x2, 0x2, 
    0x2bc, 0x2be, 0x3, 0x2, 0x2, 0x2, 0x2bd, 0x2bb, 0x3, 0x2, 0x2, 0x2, 
    0x2be, 0x2bf, 0x7, 0xc, 0x2, 0x2, 0x2bf, 0x2c1, 0x3, 0x2, 0x2, 0x2, 
    0x2c0, 0x2b5, 0x3, 0x2, 0x2, 0x2, 0x2c0, 0x2c1, 0x3, 0x2, 0x2, 0x2, 
    0x2c1, 0x2c5, 0x3, 0x2, 0x2, 0x2, 0x2c2, 0x2c6, 0x5, 0x38, 0x1d, 0x2, 
    0x2c3, 0x2c6, 0x5, 0x24, 0x13, 0x2, 0x2c4, 0x2c6, 0x5, 0x28, 0x15, 0x2, 
    0x2c5, 0x2c2, 0x3, 0x2, 0x2, 0x2, 0x2c5, 0x2c3, 0x3, 0x2, 0x2, 0x2, 
    0x2c5, 0x2c4, 0x3, 0x2, 0x2, 0x2, 0x2c6, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x2c7, 
    0x2c8, 0x7, 0xc7, 0x2, 0x2, 0x2c8, 0x2cd, 0x5, 0x2e, 0x18, 0x2, 0x2c9, 
    0x2ca, 0x7, 0x8, 0x2, 0x2, 0x2ca, 0x2cc, 0x5, 0x2e, 0x18, 0x2, 0x2cb, 
    0x2c9, 0x3, 0x2, 0x2, 0x2, 0x2cc, 0x2cf, 0x3, 0x2, 0x2, 0x2, 0x2cd, 
    0x2cb, 0x3, 0x2, 0x2, 0x2, 0x2cd, 0x2ce, 0x3, 0x2, 0x2, 0x2, 0x2ce, 
    0x2d1, 0x3, 0x2, 0x2, 0x2, 0x2cf, 0x2cd, 0x3, 0x2, 0x2, 0x2, 0x2d0, 
    0x2c7, 0x3, 0x2, 0x2, 0x2, 0x2d0, 0x2d1, 0x3, 0x2, 0x2, 0x2, 0x2d1, 
    0x2d2, 0x3, 0x2, 0x2, 0x2, 0x2d2, 0x2d4, 0x7, 0x42, 0x2, 0x2, 0x2d3, 
    0x2d5, 0x7, 0x58, 0x2, 0x2, 0x2d4, 0x2d3, 0x3, 0x2, 0x2, 0x2, 0x2d4, 
    0x2d5, 0x3, 0x2, 0x2, 0x2, 0x2d5, 0x2d6, 0x3, 0x2, 0x2, 0x2, 0x2d6, 
    0x2db, 0x5, 0x32, 0x1a, 0x2, 0x2d7, 0x2d8, 0x7, 0xc4, 0x2, 0x2, 0x2d8, 
    0x2dc, 0x5, 0x9c, 0x4f, 0x2, 0x2d9, 0x2da, 0x7, 0x58, 0x2, 0x2, 0x2da, 
    0x2dc, 0x5, 0x30, 0x19, 0x2, 0x2db, 0x2d7, 0x3, 0x2, 0x2, 0x2, 0x2db, 
    0x2d9, 0x3, 0x2, 0x2, 0x2, 0x2db, 0x2dc, 0x3, 0x2, 0x2, 0x2, 0x2dc, 
    0x3d, 0x3, 0x2, 0x2, 0x2, 0x2dd, 0x2de, 0x7, 0xc7, 0x2, 0x2, 0x2de, 
    0x2e3, 0x5, 0x2e, 0x18, 0x2, 0x2df, 0x2e0, 0x7, 0x8, 0x2, 0x2, 0x2e0, 
    0x2e2, 0x5, 0x2e, 0x18, 0x2, 0x2e1, 0x2df, 0x3, 0x2, 0x2, 0x2, 0x2e2, 
    0x2e5, 0x3, 0x2, 0x2, 0x2, 0x2e3, 0x2e1, 0x3, 0x2, 0x2, 0x2, 0x2e3, 
    0x2e4, 0x3, 0x2, 0x2, 0x2, 0x2e4, 0x2e7, 0x3, 0x2, 0x2, 0x2, 0x2e5, 
    0x2e3, 0x3, 0x2, 0x2, 0x2, 0x2e6, 0x2dd, 0x3, 0x2, 0x2, 0x2, 0x2e6, 
    0x2e7, 0x3, 0x2, 0x2, 0x2, 0x2e7, 0x2e8, 0x3, 0x2, 0x2, 0x2, 0x2e8, 
    0x2e9, 0x7, 0xba, 0x2, 0x2, 0x2e9, 0x2ea, 0x5, 0x32, 0x1a, 0x2, 0x2ea, 
    0x2eb, 0x7, 0x9d, 0x2, 0x2, 0x2eb, 0x2ec, 0x5, 0xb8, 0x5d, 0x2, 0x2ec, 
    0x2ed, 0x7, 0x6, 0x2, 0x2, 0x2ed, 0x2f5, 0x5, 0xa2, 0x52, 0x2, 0x2ee, 
    0x2ef, 0x7, 0x8, 0x2, 0x2, 0x2ef, 0x2f0, 0x5, 0xb8, 0x5d, 0x2, 0x2f0, 
    0x2f1, 0x7, 0x6, 0x2, 0x2, 0x2f1, 0x2f2, 0x5, 0xa2, 0x52, 0x2, 0x2f2, 
    0x2f4, 0x3, 0x2, 0x2, 0x2, 0x2f3, 0x2ee, 0x3, 0x2, 0x2, 0x2, 0x2f4, 
    0x2f7, 0x3, 0x2, 0x2, 0x2, 0x2f5, 0x2f3, 0x3, 0x2, 0x2, 0x2, 0x2f5, 
    0x2f6, 0x3, 0x2, 0x2, 0x2, 0x2f6, 0x2fc, 0x3, 0x2, 0x2, 0x2, 0x2f7, 
    0x2f5, 0x3, 0x2, 0x2, 0x2, 0x2f8, 0x2f9, 0x7, 0xc4, 0x2, 0x2, 0x2f9, 
    0x2fd, 0x5, 0x9c, 0x4f, 0x2, 0x2fa, 0x2fb, 0x7, 0x58, 0x2, 0x2, 0x2fb, 
    0x2fd, 0x5, 0x30, 0x19, 0x2, 0x2fc, 0x2f8, 0x3, 0x2, 0x2, 0x2, 0x2fc, 
    0x2fa, 0x3, 0x2, 0x2, 0x2, 0x2fc, 0x2fd, 0x3, 0x2, 0x2, 0x2, 0x2fd, 
    0x3f, 0x3, 0x2, 0x2, 0x2, 0x2fe, 0x301, 0x5, 0x42, 0x22, 0x2, 0x2ff, 
    0x301, 0x5, 0x44, 0x23, 0x2, 0x300, 0x2fe, 0x3, 0x2, 0x2, 0x2, 0x300, 
    0x2ff, 0x3, 0x2, 0x2, 0x2, 0x301, 0x41, 0x3, 0x2, 0x2, 0x2, 0x302, 0x303, 
    0x7, 0x35, 0x2, 0x2, 0x303, 0x304, 0x7, 0xa9, 0x2, 0x2, 0x304, 0x305, 
    0x5, 0xb4, 0x5b, 0x2, 0x305, 0x306, 0x5, 0x5e, 0x30, 0x2, 0x306, 0x43, 
    0x3, 0x2, 0x2, 0x2, 0x307, 0x309, 0x7, 0x35, 0x2, 0x2, 0x308, 0x30a, 
    0x7, 0xb9, 0x2, 0x2, 0x309, 0x308, 0x3, 0x2, 0x2, 0x2, 0x309, 0x30a, 
    0x3, 0x2, 0x2, 0x2, 0x30a, 0x30b, 0x3, 0x2, 0x2, 0x2, 0x30b, 0x30c, 
    0x7, 0x62, 0x2, 0x2, 0x30c, 0x30d, 0x5, 0xbe, 0x60, 0x2, 0x30d, 0x30e, 
    0x7, 0x85, 0x2, 0x2, 0x30e, 0x30f, 0x5, 0xb4, 0x5b, 0x2, 0x30f, 0x310, 
    0x7, 0xb, 0x2, 0x2, 0x310, 0x315, 0x5, 0xa2, 0x52, 0x2, 0x311, 0x312, 
    0x7, 0x8, 0x2, 0x2, 0x312, 0x314, 0x5, 0xa2, 0x52, 0x2, 0x313, 0x311, 
    0x3, 0x2, 0x2, 0x2, 0x314, 0x317, 0x3, 0x2, 0x2, 0x2, 0x315, 0x313, 
    0x3, 0x2, 0x2, 0x2, 0x315, 0x316, 0x3, 0x2, 0x2, 0x2, 0x316, 0x318, 
    0x3, 0x2, 0x2, 0x2, 0x317, 0x315, 0x3, 0x2, 0x2, 0x2, 0x318, 0x319, 
    0x7, 0xc, 0x2, 0x2, 0x319, 0x45, 0x3, 0x2, 0x2, 0x2, 0x31a, 0x31d, 0x5, 
    0x48, 0x25, 0x2, 0x31b, 0x31d, 0x5, 0x4a, 0x26, 0x2, 0x31c, 0x31a, 0x3, 
    0x2, 0x2, 0x2, 0x31c, 0x31b, 0x3, 0x2, 0x2, 0x2, 0x31d, 0x47, 0x3, 0x2, 
    0x2, 0x2, 0x31e, 0x31f, 0x7, 0x48, 0x2, 0x2, 0x31f, 0x320, 0x7, 0xa9, 
    0x2, 0x2, 0x320, 0x321, 0x5, 0xb4, 0x5b, 0x2, 0x321, 0x49, 0x3, 0x2, 
    0x2, 0x2, 0x322, 0x323, 0x7, 0x48, 0x2, 0x2, 0x323, 0x32b, 0x7, 0x62, 
    0x2, 0x2, 0x324, 0x325, 0x5, 0xbe, 0x60, 0x2, 0x325, 0x326, 0x7, 0x85, 
    0x2, 0x2, 0x326, 0x327, 0x5, 0xb4, 0x5b, 0x2, 0x327, 0x32c, 0x3, 0x2, 
    0x2, 0x2, 0x328, 0x329, 0x5, 0xb4, 0x5b, 0x2, 0x329, 0x32a, 0x5, 0xbe, 
    0x60, 0x2, 0x32a, 0x32c, 0x3, 0x2, 0x2, 0x2, 0x32b, 0x324, 0x3, 0x2, 
    0x2, 0x2, 0x32b, 0x328, 0x3, 0x2, 0x2, 0x2, 0x32c, 0x4b, 0x3, 0x2, 0x2, 
    0x2, 0x32d, 0x32f, 0x7, 0xb4, 0x2, 0x2, 0x32e, 0x330, 0x7, 0xa9, 0x2, 
    0x2, 0x32f, 0x32e, 0x3, 0x2, 0x2, 0x2, 0x32f, 0x330, 0x3, 0x2, 0x2, 
    0x2, 0x330, 0x331, 0x3, 0x2, 0x2, 0x2, 0x331, 0x332, 0x5, 0xb4, 0x5b, 
    0x2, 0x332, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x333, 0x334, 0x7, 0x70, 0x2, 
    0x2, 0x334, 0x335, 0x5, 0xb4, 0x5b, 0x2, 0x335, 0x340, 0x5, 0xec, 0x77, 
    0x2, 0x336, 0x33d, 0x5, 0x50, 0x29, 0x2, 0x337, 0x339, 0x7, 0x8, 0x2, 
    0x2, 0x338, 0x337, 0x3, 0x2, 0x2, 0x2, 0x338, 0x339, 0x3, 0x2, 0x2, 
    0x2, 0x339, 0x33a, 0x3, 0x2, 0x2, 0x2, 0x33a, 0x33c, 0x5, 0x50, 0x29, 
    0x2, 0x33b, 0x338, 0x3, 0x2, 0x2, 0x2, 0x33c, 0x33f, 0x3, 0x2, 0x2, 
    0x2, 0x33d, 0x33b, 0x3, 0x2, 0x2, 0x2, 0x33d, 0x33e, 0x3, 0x2, 0x2, 
    0x2, 0x33e, 0x341, 0x3, 0x2, 0x2, 0x2, 0x33f, 0x33d, 0x3, 0x2, 0x2, 
    0x2, 0x340, 0x336, 0x3, 0x2, 0x2, 0x2, 0x340, 0x341, 0x3, 0x2, 0x2, 
    0x2, 0x341, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x342, 0x343, 0x7, 0x4, 0x2, 0x2, 
    0x343, 0x344, 0x5, 0x52, 0x2a, 0x2, 0x344, 0x345, 0x7, 0x5, 0x2, 0x2, 
    0x345, 0x348, 0x3, 0x2, 0x2, 0x2, 0x346, 0x348, 0x5, 0x52, 0x2a, 0x2, 
    0x347, 0x342, 0x3, 0x2, 0x2, 0x2, 0x347, 0x346, 0x3, 0x2, 0x2, 0x2, 
    0x348, 0x349, 0x3, 0x2, 0x2, 0x2, 0x349, 0x34a, 0x7, 0x6, 0x2, 0x2, 
    0x34a, 0x34b, 0x5, 0xec, 0x77, 0x2, 0x34b, 0x51, 0x3, 0x2, 0x2, 0x2, 
    0x34c, 0x34d, 0x9, 0x9, 0x2, 0x2, 0x34d, 0x53, 0x3, 0x2, 0x2, 0x2, 0x34e, 
    0x34f, 0x7, 0x34, 0x2, 0x2, 0x34f, 0x350, 0x5, 0xb4, 0x5b, 0x2, 0x350, 
    0x55, 0x3, 0x2, 0x2, 0x2, 0x351, 0x352, 0x7, 0x9f, 0x2, 0x2, 0x352, 
    0x57, 0x3, 0x2, 0x2, 0x2, 0x353, 0x354, 0x7, 0xa3, 0x2, 0x2, 0x354, 
    0x59, 0x3, 0x2, 0x2, 0x2, 0x355, 0x356, 0x7, 0xa7, 0x2, 0x2, 0x356, 
    0x357, 0x5, 0xb4, 0x5b, 0x2, 0x357, 0x358, 0x5, 0xbe, 0x60, 0x2, 0x358, 
    0x5b, 0x3, 0x2, 0x2, 0x2, 0x359, 0x35a, 0x7, 0xa8, 0x2, 0x2, 0x35a, 
    0x35b, 0x5, 0xb4, 0x5b, 0x2, 0x35b, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x35c, 
    0x35f, 0x7, 0xb, 0x2, 0x2, 0x35d, 0x360, 0x5, 0x62, 0x32, 0x2, 0x35e, 
    0x360, 0x3, 0x2, 0x2, 0x2, 0x35f, 0x35d, 0x3, 0x2, 0x2, 0x2, 0x35f, 
    0x35e, 0x3, 0x2, 0x2, 0x2, 0x360, 0x368, 0x3, 0x2, 0x2, 0x2, 0x361, 
    0x364, 0x7, 0x8, 0x2, 0x2, 0x362, 0x365, 0x5, 0x62, 0x32, 0x2, 0x363, 
    0x365, 0x3, 0x2, 0x2, 0x2, 0x364, 0x362, 0x3, 0x2, 0x2, 0x2, 0x364, 
    0x363, 0x3, 0x2, 0x2, 0x2, 0x365, 0x367, 0x3, 0x2, 0x2, 0x2, 0x366, 
    0x361, 0x3, 0x2, 0x2, 0x2, 0x367, 0x36a, 0x3, 0x2, 0x2, 0x2, 0x368, 
    0x366, 0x3, 0x2, 0x2, 0x2, 0x368, 0x369, 0x3, 0x2, 0x2, 0x2, 0x369, 
    0x36b, 0x3, 0x2, 0x2, 0x2, 0x36a, 0x368, 0x3, 0x2, 0x2, 0x2, 0x36b, 
    0x374, 0x7, 0xc, 0x2, 0x2, 0x36c, 0x373, 0x5, 0x60, 0x31, 0x2, 0x36d, 
    0x373, 0x7, 0x94, 0x2, 0x2, 0x36e, 0x36f, 0x7, 0x97, 0x2, 0x2, 0x36f, 
    0x373, 0x5, 0xda, 0x6e, 0x2, 0x370, 0x371, 0x7, 0xaa, 0x2, 0x2, 0x371, 
    0x373, 0x5, 0xda, 0x6e, 0x2, 0x372, 0x36c, 0x3, 0x2, 0x2, 0x2, 0x372, 
    0x36d, 0x3, 0x2, 0x2, 0x2, 0x372, 0x36e, 0x3, 0x2, 0x2, 0x2, 0x372, 
    0x370, 0x3, 0x2, 0x2, 0x2, 0x373, 0x376, 0x3, 0x2, 0x2, 0x2, 0x374, 
    0x372, 0x3, 0x2, 0x2, 0x2, 0x374, 0x375, 0x3, 0x2, 0x2, 0x2, 0x375, 
    0x5f, 0x3, 0x2, 0x2, 0x2, 0x376, 0x374, 0x3, 0x2, 0x2, 0x2, 0x377, 0x378, 
    0x7, 0x8b, 0x2, 0x2, 0x378, 0x379, 0x7, 0x27, 0x2, 0x2, 0x379, 0x37a, 
    0x7, 0xb, 0x2, 0x2, 0x37a, 0x37f, 0x5, 0xa2, 0x52, 0x2, 0x37b, 0x37c, 
    0x7, 0x8, 0x2, 0x2, 0x37c, 0x37e, 0x5, 0xa2, 0x52, 0x2, 0x37d, 0x37b, 
    0x3, 0x2, 0x2, 0x2, 0x37e, 0x381, 0x3, 0x2, 0x2, 0x2, 0x37f, 0x37d, 
    0x3, 0x2, 0x2, 0x2, 0x37f, 0x380, 0x3, 0x2, 0x2, 0x2, 0x380, 0x382, 
    0x3, 0x2, 0x2, 0x2, 0x381, 0x37f, 0x3, 0x2, 0x2, 0x2, 0x382, 0x383, 
    0x7, 0xc, 0x2, 0x2, 0x383, 0x61, 0x3, 0x2, 0x2, 0x2, 0x384, 0x385, 0x5, 
    0xb8, 0x5d, 0x2, 0x385, 0x38a, 0x5, 0x6a, 0x36, 0x2, 0x386, 0x387, 0x7, 
    0x80, 0x2, 0x2, 0x387, 0x389, 0x7, 0x82, 0x2, 0x2, 0x388, 0x386, 0x3, 
    0x2, 0x2, 0x2, 0x389, 0x38c, 0x3, 0x2, 0x2, 0x2, 0x38a, 0x388, 0x3, 
    0x2, 0x2, 0x2, 0x38a, 0x38b, 0x3, 0x2, 0x2, 0x2, 0x38b, 0x63, 0x3, 0x2, 
    0x2, 0x2, 0x38c, 0x38a, 0x3, 0x2, 0x2, 0x2, 0x38d, 0x38e, 0x7, 0xb, 
    0x2, 0x2, 0x38e, 0x393, 0x5, 0xb8, 0x5d, 0x2, 0x38f, 0x390, 0x7, 0x8, 
    0x2, 0x2, 0x390, 0x392, 0x5, 0xb8, 0x5d, 0x2, 0x391, 0x38f, 0x3, 0x2, 
    0x2, 0x2, 0x392, 0x395, 0x3, 0x2, 0x2, 0x2, 0x393, 0x391, 0x3, 0x2, 
    0x2, 0x2, 0x393, 0x394, 0x3, 0x2, 0x2, 0x2, 0x394, 0x396, 0x3, 0x2, 
    0x2, 0x2, 0x395, 0x393, 0x3, 0x2, 0x2, 0x2, 0x396, 0x397, 0x7, 0xc, 
    0x2, 0x2, 0x397, 0x65, 0x3, 0x2, 0x2, 0x2, 0x398, 0x39a, 0x7, 0xb9, 
    0x2, 0x2, 0x399, 0x398, 0x3, 0x2, 0x2, 0x2, 0x399, 0x39a, 0x3, 0x2, 
    0x2, 0x2, 0x39a, 0x39b, 0x3, 0x2, 0x2, 0x2, 0x39b, 0x39d, 0x9, 0xa, 
    0x2, 0x2, 0x39c, 0x39e, 0x5, 0xbe, 0x60, 0x2, 0x39d, 0x39c, 0x3, 0x2, 
    0x2, 0x2, 0x39d, 0x39e, 0x3, 0x2, 0x2, 0x2, 0x39e, 0x3a0, 0x3, 0x2, 
    0x2, 0x2, 0x39f, 0x3a1, 0x7, 0x85, 0x2, 0x2, 0x3a0, 0x39f, 0x3, 0x2, 
    0x2, 0x2, 0x3a0, 0x3a1, 0x3, 0x2, 0x2, 0x2, 0x3a1, 0x3ae, 0x3, 0x2, 
    0x2, 0x2, 0x3a2, 0x3af, 0x5, 0xa2, 0x52, 0x2, 0x3a3, 0x3a4, 0x7, 0xb, 
    0x2, 0x2, 0x3a4, 0x3a9, 0x5, 0xa2, 0x52, 0x2, 0x3a5, 0x3a6, 0x7, 0x8, 
    0x2, 0x2, 0x3a6, 0x3a8, 0x5, 0xa2, 0x52, 0x2, 0x3a7, 0x3a5, 0x3, 0x2, 
    0x2, 0x2, 0x3a8, 0x3ab, 0x3, 0x2, 0x2, 0x2, 0x3a9, 0x3a7, 0x3, 0x2, 
    0x2, 0x2, 0x3a9, 0x3aa, 0x3, 0x2, 0x2, 0x2, 0x3aa, 0x3ac, 0x3, 0x2, 
    0x2, 0x2, 0x3ab, 0x3a9, 0x3, 0x2, 0x2, 0x2, 0x3ac, 0x3ad, 0x7, 0xc, 
    0x2, 0x2, 0x3ad, 0x3af, 0x3, 0x2, 0x2, 0x2, 0x3ae, 0x3a2, 0x3, 0x2, 
    0x2, 0x2, 0x3ae, 0x3a3, 0x3, 0x2, 0x2, 0x2, 0x3af, 0x3b2, 0x3, 0x2, 
    0x2, 0x2, 0x3b0, 0x3b1, 0x7, 0xbb, 0x2, 0x2, 0x3b1, 0x3b3, 0x5, 0x68, 
    0x35, 0x2, 0x3b2, 0x3b0, 0x3, 0x2, 0x2, 0x2, 0x3b2, 0x3b3, 0x3, 0x2, 
    0x2, 0x2, 0x3b3, 0x3b6, 0x3, 0x2, 0x2, 0x2, 0x3b4, 0x3b5, 0x7, 0xc7, 
    0x2, 0x2, 0x3b5, 0x3b7, 0x5, 0x96, 0x4c, 0x2, 0x3b6, 0x3b4, 0x3, 0x2, 
    0x2, 0x2, 0x3b6, 0x3b7, 0x3, 0x2, 0x2, 0x2, 0x3b7, 0x67, 0x3, 0x2, 0x2, 
    0x2, 0x3b8, 0x3b9, 0x9, 0xb, 0x2, 0x2, 0x3b9, 0x69, 0x3, 0x2, 0x2, 0x2, 
    0x3ba, 0x3bb, 0x5, 0x6c, 0x37, 0x2, 0x3bb, 0x6b, 0x3, 0x2, 0x2, 0x2, 
    0x3bc, 0x3fc, 0x7, 0x25, 0x2, 0x2, 0x3bd, 0x3c5, 0x7, 0x65, 0x2, 0x2, 
    0x3be, 0x3bf, 0x7, 0xb, 0x2, 0x2, 0x3bf, 0x3c1, 0x5, 0xce, 0x68, 0x2, 
    0x3c0, 0x3c2, 0x9, 0xc, 0x2, 0x2, 0x3c1, 0x3c0, 0x3, 0x2, 0x2, 0x2, 
    0x3c1, 0x3c2, 0x3, 0x2, 0x2, 0x2, 0x3c2, 0x3c3, 0x3, 0x2, 0x2, 0x2, 
    0x3c3, 0x3c4, 0x7, 0xc, 0x2, 0x2, 0x3c4, 0x3c6, 0x3, 0x2, 0x2, 0x2, 
    0x3c5, 0x3be, 0x3, 0x2, 0x2, 0x2, 0x3c5, 0x3c6, 0x3, 0x2, 0x2, 0x2, 
    0x3c6, 0x3fc, 0x3, 0x2, 0x2, 0x2, 0x3c7, 0x3d6, 0x7, 0x40, 0x2, 0x2, 
    0x3c8, 0x3c9, 0x7, 0xb, 0x2, 0x2, 0x3c9, 0x3cb, 0x5, 0xce, 0x68, 0x2, 
    0x3ca, 0x3cc, 0x7, 0x45, 0x2, 0x2, 0x3cb, 0x3ca, 0x3, 0x2, 0x2, 0x2, 
    0x3cb, 0x3cc, 0x3, 0x2, 0x2, 0x2, 0x3cc, 0x3d2, 0x3, 0x2, 0x2, 0x2, 
    0x3cd, 0x3ce, 0x7, 0x8, 0x2, 0x2, 0x3ce, 0x3d0, 0x5, 0xd0, 0x69, 0x2, 
    0x3cf, 0x3d1, 0x7, 0x45, 0x2, 0x2, 0x3d0, 0x3cf, 0x3, 0x2, 0x2, 0x2, 
    0x3d0, 0x3d1, 0x3, 0x2, 0x2, 0x2, 0x3d1, 0x3d3, 0x3, 0x2, 0x2, 0x2, 
    0x3d2, 0x3cd, 0x3, 0x2, 0x2, 0x2, 0x3d2, 0x3d3, 0x3, 0x2, 0x2, 0x2, 
    0x3d3, 0x3d4, 0x3, 0x2, 0x2, 0x2, 0x3d4, 0x3d5, 0x7, 0xc, 0x2, 0x2, 
    0x3d5, 0x3d7, 0x3, 0x2, 0x2, 0x2, 0x3d6, 0x3c8, 0x3, 0x2, 0x2, 0x2, 
    0x3d6, 0x3d7, 0x3, 0x2, 0x2, 0x2, 0x3d7, 0x3fc, 0x3, 0x2, 0x2, 0x2, 
    0x3d8, 0x3e0, 0x7, 0x54, 0x2, 0x2, 0x3d9, 0x3da, 0x7, 0xb, 0x2, 0x2, 
    0x3da, 0x3dc, 0x5, 0xce, 0x68, 0x2, 0x3db, 0x3dd, 0x9, 0xc, 0x2, 0x2, 
    0x3dc, 0x3db, 0x3, 0x2, 0x2, 0x2, 0x3dc, 0x3dd, 0x3, 0x2, 0x2, 0x2, 
    0x3dd, 0x3de, 0x3, 0x2, 0x2, 0x2, 0x3de, 0x3df, 0x7, 0xc, 0x2, 0x2, 
    0x3df, 0x3e1, 0x3, 0x2, 0x2, 0x2, 0x3e0, 0x3d9, 0x3, 0x2, 0x2, 0x2, 
    0x3e0, 0x3e1, 0x3, 0x2, 0x2, 0x2, 0x3e1, 0x3fc, 0x3, 0x2, 0x2, 0x2, 
    0x3e2, 0x3fc, 0x7, 0x3c, 0x2, 0x2, 0x3e3, 0x3fc, 0x7, 0xad, 0x2, 0x2, 
    0x3e4, 0x3fc, 0x7, 0xae, 0x2, 0x2, 0x3e5, 0x3fc, 0x5, 0x6e, 0x38, 0x2, 
    0x3e6, 0x3fc, 0x5, 0x74, 0x3b, 0x2, 0x3e7, 0x3fc, 0x5, 0x76, 0x3c, 0x2, 
    0x3e8, 0x3e9, 0x7, 0x1e, 0x2, 0x2, 0x3e9, 0x3ea, 0x7, 0xb, 0x2, 0x2, 
    0x3ea, 0x3ec, 0x5, 0xd2, 0x6a, 0x2, 0x3eb, 0x3ed, 0x9, 0xd, 0x2, 0x2, 
    0x3ec, 0x3eb, 0x3, 0x2, 0x2, 0x2, 0x3ec, 0x3ed, 0x3, 0x2, 0x2, 0x2, 
    0x3ed, 0x3ee, 0x3, 0x2, 0x2, 0x2, 0x3ee, 0x3ef, 0x7, 0xc, 0x2, 0x2, 
    0x3ef, 0x3fc, 0x3, 0x2, 0x2, 0x2, 0x3f0, 0x3f8, 0x7, 0xbe, 0x2, 0x2, 
    0x3f1, 0x3f2, 0x7, 0xb, 0x2, 0x2, 0x3f2, 0x3f4, 0x5, 0xd2, 0x6a, 0x2, 
    0x3f3, 0x3f5, 0x9, 0xd, 0x2, 0x2, 0x3f4, 0x3f3, 0x3, 0x2, 0x2, 0x2, 
    0x3f4, 0x3f5, 0x3, 0x2, 0x2, 0x2, 0x3f5, 0x3f6, 0x3, 0x2, 0x2, 0x2, 
    0x3f6, 0x3f7, 0x7, 0xc, 0x2, 0x2, 0x3f7, 0x3f9, 0x3, 0x2, 0x2, 0x2, 
    0x3f8, 0x3f1, 0x3, 0x2, 0x2, 0x2, 0x3f8, 0x3f9, 0x3, 0x2, 0x2, 0x2, 
    0x3f9, 0x3fc, 0x3, 0x2, 0x2, 0x2, 0x3fa, 0x3fc, 0x7, 0x21, 0x2, 0x2, 
    0x3fb, 0x3bc, 0x3, 0x2, 0x2, 0x2, 0x3fb, 0x3bd, 0x3, 0x2, 0x2, 0x2, 
    0x3fb, 0x3c7, 0x3, 0x2, 0x2, 0x2, 0x3fb, 0x3d8, 0x3, 0x2, 0x2, 0x2, 
    0x3fb, 0x3e2, 0x3, 0x2, 0x2, 0x2, 0x3fb, 0x3e3, 0x3, 0x2, 0x2, 0x2, 
    0x3fb, 0x3e4, 0x3, 0x2, 0x2, 0x2, 0x3fb, 0x3e5, 0x3, 0x2, 0x2, 0x2, 
    0x3fb, 0x3e6, 0x3, 0x2, 0x2, 0x2, 0x3fb, 0x3e7, 0x3, 0x2, 0x2, 0x2, 
    0x3fb, 0x3e8, 0x3, 0x2, 0x2, 0x2, 0x3fb, 0x3f0, 0x3, 0x2, 0x2, 0x2, 
    0x3fb, 0x3fa, 0x3, 0x2, 0x2, 0x2, 0x3fc, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x3fd, 
    0x400, 0x7, 0x67, 0x2, 0x2, 0x3fe, 0x401, 0x5, 0x70, 0x39, 0x2, 0x3ff, 
    0x401, 0x5, 0x72, 0x3a, 0x2, 0x400, 0x3fe, 0x3, 0x2, 0x2, 0x2, 0x400, 
    0x3ff, 0x3, 0x2, 0x2, 0x2, 0x401, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x402, 0x407, 
    0x7, 0xc9, 0x2, 0x2, 0x403, 0x404, 0x7, 0xb, 0x2, 0x2, 0x404, 0x405, 
    0x5, 0xce, 0x68, 0x2, 0x405, 0x406, 0x7, 0xc, 0x2, 0x2, 0x406, 0x408, 
    0x3, 0x2, 0x2, 0x2, 0x407, 0x403, 0x3, 0x2, 0x2, 0x2, 0x407, 0x408, 
    0x3, 0x2, 0x2, 0x2, 0x408, 0x41a, 0x3, 0x2, 0x2, 0x2, 0x409, 0x40e, 
    0x7, 0xc9, 0x2, 0x2, 0x40a, 0x40b, 0x7, 0xb, 0x2, 0x2, 0x40b, 0x40c, 
    0x5, 0xce, 0x68, 0x2, 0x40c, 0x40d, 0x7, 0xc, 0x2, 0x2, 0x40d, 0x40f, 
    0x3, 0x2, 0x2, 0x2, 0x40e, 0x40a, 0x3, 0x2, 0x2, 0x2, 0x40e, 0x40f, 
    0x3, 0x2, 0x2, 0x2, 0x40f, 0x410, 0x3, 0x2, 0x2, 0x2, 0x410, 0x411, 
    0x7, 0xaf, 0x2, 0x2, 0x411, 0x41a, 0x7, 0x7b, 0x2, 0x2, 0x412, 0x417, 
    0x7, 0x7b, 0x2, 0x2, 0x413, 0x414, 0x7, 0xb, 0x2, 0x2, 0x414, 0x415, 
    0x5, 0xce, 0x68, 0x2, 0x415, 0x416, 0x7, 0xc, 0x2, 0x2, 0x416, 0x418, 
    0x3, 0x2, 0x2, 0x2, 0x417, 0x413, 0x3, 0x2, 0x2, 0x2, 0x417, 0x418, 
    0x3, 0x2, 0x2, 0x2, 0x418, 0x41a, 0x3, 0x2, 0x2, 0x2, 0x419, 0x402, 
    0x3, 0x2, 0x2, 0x2, 0x419, 0x409, 0x3, 0x2, 0x2, 0x2, 0x419, 0x412, 
    0x3, 0x2, 0x2, 0x2, 0x41a, 0x71, 0x3, 0x2, 0x2, 0x2, 0x41b, 0x420, 0x7, 
    0x3e, 0x2, 0x2, 0x41c, 0x41d, 0x7, 0xb, 0x2, 0x2, 0x41d, 0x41e, 0x5, 
    0xce, 0x68, 0x2, 0x41e, 0x41f, 0x7, 0xc, 0x2, 0x2, 0x41f, 0x421, 0x3, 
    0x2, 0x2, 0x2, 0x420, 0x41c, 0x3, 0x2, 0x2, 0x2, 0x420, 0x421, 0x3, 
    0x2, 0x2, 0x2, 0x421, 0x470, 0x3, 0x2, 0x2, 0x2, 0x422, 0x427, 0x7, 
    0x3e, 0x2, 0x2, 0x423, 0x424, 0x7, 0xb, 0x2, 0x2, 0x424, 0x425, 0x5, 
    0xce, 0x68, 0x2, 0x425, 0x426, 0x7, 0xc, 0x2, 0x2, 0x426, 0x428, 0x3, 
    0x2, 0x2, 0x2, 0x427, 0x423, 0x3, 0x2, 0x2, 0x2, 0x427, 0x428, 0x3, 
    0x2, 0x2, 0x2, 0x428, 0x429, 0x3, 0x2, 0x2, 0x2, 0x429, 0x433, 0x7, 
    0xaf, 0x2, 0x2, 0x42a, 0x434, 0x7, 0x5d, 0x2, 0x2, 0x42b, 0x434, 0x7, 
    0x78, 0x2, 0x2, 0x42c, 0x431, 0x7, 0x99, 0x2, 0x2, 0x42d, 0x42e, 0x7, 
    0xb, 0x2, 0x2, 0x42e, 0x42f, 0x5, 0xd0, 0x69, 0x2, 0x42f, 0x430, 0x7, 
    0xc, 0x2, 0x2, 0x430, 0x432, 0x3, 0x2, 0x2, 0x2, 0x431, 0x42d, 0x3, 
    0x2, 0x2, 0x2, 0x431, 0x432, 0x3, 0x2, 0x2, 0x2, 0x432, 0x434, 0x3, 
    0x2, 0x2, 0x2, 0x433, 0x42a, 0x3, 0x2, 0x2, 0x2, 0x433, 0x42b, 0x3, 
    0x2, 0x2, 0x2, 0x433, 0x42c, 0x3, 0x2, 0x2, 0x2, 0x434, 0x470, 0x3, 
    0x2, 0x2, 0x2, 0x435, 0x43a, 0x7, 0x5d, 0x2, 0x2, 0x436, 0x437, 0x7, 
    0xb, 0x2, 0x2, 0x437, 0x438, 0x5, 0xce, 0x68, 0x2, 0x438, 0x439, 0x7, 
    0xc, 0x2, 0x2, 0x439, 0x43b, 0x3, 0x2, 0x2, 0x2, 0x43a, 0x436, 0x3, 
    0x2, 0x2, 0x2, 0x43a, 0x43b, 0x3, 0x2, 0x2, 0x2, 0x43b, 0x43c, 0x3, 
    0x2, 0x2, 0x2, 0x43c, 0x445, 0x7, 0xaf, 0x2, 0x2, 0x43d, 0x446, 0x7, 
    0x78, 0x2, 0x2, 0x43e, 0x443, 0x7, 0x99, 0x2, 0x2, 0x43f, 0x440, 0x7, 
    0xb, 0x2, 0x2, 0x440, 0x441, 0x5, 0xd0, 0x69, 0x2, 0x441, 0x442, 0x7, 
    0xc, 0x2, 0x2, 0x442, 0x444, 0x3, 0x2, 0x2, 0x2, 0x443, 0x43f, 0x3, 
    0x2, 0x2, 0x2, 0x443, 0x444, 0x3, 0x2, 0x2, 0x2, 0x444, 0x446, 0x3, 
    0x2, 0x2, 0x2, 0x445, 0x43d, 0x3, 0x2, 0x2, 0x2, 0x445, 0x43e, 0x3, 
    0x2, 0x2, 0x2, 0x446, 0x470, 0x3, 0x2, 0x2, 0x2, 0x447, 0x44c, 0x7, 
    0x78, 0x2, 0x2, 0x448, 0x449, 0x7, 0xb, 0x2, 0x2, 0x449, 0x44a, 0x5, 
    0xce, 0x68, 0x2, 0x44a, 0x44b, 0x7, 0xc, 0x2, 0x2, 0x44b, 0x44d, 0x3, 
    0x2, 0x2, 0x2, 0x44c, 0x448, 0x3, 0x2, 0x2, 0x2, 0x44c, 0x44d, 0x3, 
    0x2, 0x2, 0x2, 0x44d, 0x44e, 0x3, 0x2, 0x2, 0x2, 0x44e, 0x44f, 0x7, 
    0xaf, 0x2, 0x2, 0x44f, 0x454, 0x7, 0x99, 0x2, 0x2, 0x450, 0x451, 0x7, 
    0xb, 0x2, 0x2, 0x451, 0x452, 0x5, 0xd0, 0x69, 0x2, 0x452, 0x453, 0x7, 
    0xc, 0x2, 0x2, 0x453, 0x455, 0x3, 0x2, 0x2, 0x2, 0x454, 0x450, 0x3, 
    0x2, 0x2, 0x2, 0x454, 0x455, 0x3, 0x2, 0x2, 0x2, 0x455, 0x470, 0x3, 
    0x2, 0x2, 0x2, 0x456, 0x45b, 0x7, 0x5d, 0x2, 0x2, 0x457, 0x458, 0x7, 
    0xb, 0x2, 0x2, 0x458, 0x459, 0x5, 0xce, 0x68, 0x2, 0x459, 0x45a, 0x7, 
    0xc, 0x2, 0x2, 0x45a, 0x45c, 0x3, 0x2, 0x2, 0x2, 0x45b, 0x457, 0x3, 
    0x2, 0x2, 0x2, 0x45b, 0x45c, 0x3, 0x2, 0x2, 0x2, 0x45c, 0x470, 0x3, 
    0x2, 0x2, 0x2, 0x45d, 0x462, 0x7, 0x78, 0x2, 0x2, 0x45e, 0x45f, 0x7, 
    0xb, 0x2, 0x2, 0x45f, 0x460, 0x5, 0xce, 0x68, 0x2, 0x460, 0x461, 0x7, 
    0xc, 0x2, 0x2, 0x461, 0x463, 0x3, 0x2, 0x2, 0x2, 0x462, 0x45e, 0x3, 
    0x2, 0x2, 0x2, 0x462, 0x463, 0x3, 0x2, 0x2, 0x2, 0x463, 0x470, 0x3, 
    0x2, 0x2, 0x2, 0x464, 0x46d, 0x7, 0x99, 0x2, 0x2, 0x465, 0x466, 0x7, 
    0xb, 0x2, 0x2, 0x466, 0x469, 0x5, 0xce, 0x68, 0x2, 0x467, 0x468, 0x7, 
    0x8, 0x2, 0x2, 0x468, 0x46a, 0x5, 0xd0, 0x69, 0x2, 0x469, 0x467, 0x3, 
    0x2, 0x2, 0x2, 0x469, 0x46a, 0x3, 0x2, 0x2, 0x2, 0x46a, 0x46b, 0x3, 
    0x2, 0x2, 0x2, 0x46b, 0x46c, 0x7, 0xc, 0x2, 0x2, 0x46c, 0x46e, 0x3, 
    0x2, 0x2, 0x2, 0x46d, 0x465, 0x3, 0x2, 0x2, 0x2, 0x46d, 0x46e, 0x3, 
    0x2, 0x2, 0x2, 0x46e, 0x470, 0x3, 0x2, 0x2, 0x2, 0x46f, 0x41b, 0x3, 
    0x2, 0x2, 0x2, 0x46f, 0x422, 0x3, 0x2, 0x2, 0x2, 0x46f, 0x435, 0x3, 
    0x2, 0x2, 0x2, 0x46f, 0x447, 0x3, 0x2, 0x2, 0x2, 0x46f, 0x456, 0x3, 
    0x2, 0x2, 0x2, 0x46f, 0x45d, 0x3, 0x2, 0x2, 0x2, 0x46f, 0x464, 0x3, 
    0x2, 0x2, 0x2, 0x470, 0x73, 0x3, 0x2, 0x2, 0x2, 0x471, 0x472, 0x9, 0xe, 
    0x2, 0x2, 0x472, 0x473, 0x7, 0xb, 0x2, 0x2, 0x473, 0x475, 0x5, 0xd2, 
    0x6a, 0x2, 0x474, 0x476, 0x9, 0xf, 0x2, 0x2, 0x475, 0x474, 0x3, 0x2, 
    0x2, 0x2, 0x475, 0x476, 0x3, 0x2, 0x2, 0x2, 0x476, 0x477, 0x3, 0x2, 
    0x2, 0x2, 0x477, 0x47a, 0x7, 0xc, 0x2, 0x2, 0x478, 0x479, 0x7, 0x31, 
    0x2, 0x2, 0x479, 0x47b, 0x5, 0xbc, 0x5f, 0x2, 0x47a, 0x478, 0x3, 0x2, 
    0x2, 0x2, 0x47a, 0x47b, 0x3, 0x2, 0x2, 0x2, 0x47b, 0x75, 0x3, 0x2, 0x2, 
    0x2, 0x47c, 0x47e, 0x9, 0xe, 0x2, 0x2, 0x47d, 0x47f, 0x7, 0xc0, 0x2, 
    0x2, 0x47e, 0x47d, 0x3, 0x2, 0x2, 0x2, 0x47e, 0x47f, 0x3, 0x2, 0x2, 
    0x2, 0x47f, 0x482, 0x3, 0x2, 0x2, 0x2, 0x480, 0x482, 0x7, 0xbf, 0x2, 
    0x2, 0x481, 0x47c, 0x3, 0x2, 0x2, 0x2, 0x481, 0x480, 0x3, 0x2, 0x2, 
    0x2, 0x482, 0x48a, 0x3, 0x2, 0x2, 0x2, 0x483, 0x484, 0x7, 0xb, 0x2, 
    0x2, 0x484, 0x486, 0x5, 0xd2, 0x6a, 0x2, 0x485, 0x487, 0x9, 0xf, 0x2, 
    0x2, 0x486, 0x485, 0x3, 0x2, 0x2, 0x2, 0x486, 0x487, 0x3, 0x2, 0x2, 
    0x2, 0x487, 0x488, 0x3, 0x2, 0x2, 0x2, 0x488, 0x489, 0x7, 0xc, 0x2, 
    0x2, 0x489, 0x48b, 0x3, 0x2, 0x2, 0x2, 0x48a, 0x483, 0x3, 0x2, 0x2, 
    0x2, 0x48a, 0x48b, 0x3, 0x2, 0x2, 0x2, 0x48b, 0x48e, 0x3, 0x2, 0x2, 
    0x2, 0x48c, 0x48e, 0x7, 0xab, 0x2, 0x2, 0x48d, 0x481, 0x3, 0x2, 0x2, 
    0x2, 0x48d, 0x48c, 0x3, 0x2, 0x2, 0x2, 0x48e, 0x491, 0x3, 0x2, 0x2, 
    0x2, 0x48f, 0x490, 0x7, 0x31, 0x2, 0x2, 0x490, 0x492, 0x5, 0xbc, 0x5f, 
    0x2, 0x491, 0x48f, 0x3, 0x2, 0x2, 0x2, 0x491, 0x492, 0x3, 0x2, 0x2, 
    0x2, 0x492, 0x77, 0x3, 0x2, 0x2, 0x2, 0x493, 0x494, 0x5, 0x7a, 0x3e, 
    0x2, 0x494, 0x79, 0x3, 0x2, 0x2, 0x2, 0x495, 0x499, 0x7, 0xb, 0x2, 0x2, 
    0x496, 0x498, 0x5, 0x20, 0x11, 0x2, 0x497, 0x496, 0x3, 0x2, 0x2, 0x2, 
    0x498, 0x49b, 0x3, 0x2, 0x2, 0x2, 0x499, 0x497, 0x3, 0x2, 0x2, 0x2, 
    0x499, 0x49a, 0x3, 0x2, 0x2, 0x2, 0x49a, 0x49c, 0x3, 0x2, 0x2, 0x2, 
    0x49b, 0x499, 0x3, 0x2, 0x2, 0x2, 0x49c, 0x49d, 0x5, 0x7a, 0x3e, 0x2, 
    0x49d, 0x49e, 0x7, 0xc, 0x2, 0x2, 0x49e, 0x4ab, 0x3, 0x2, 0x2, 0x2, 
    0x49f, 0x4a1, 0x5, 0x20, 0x11, 0x2, 0x4a0, 0x49f, 0x3, 0x2, 0x2, 0x2, 
    0x4a1, 0x4a4, 0x3, 0x2, 0x2, 0x2, 0x4a2, 0x4a0, 0x3, 0x2, 0x2, 0x2, 
    0x4a2, 0x4a3, 0x3, 0x2, 0x2, 0x2, 0x4a3, 0x4a8, 0x3, 0x2, 0x2, 0x2, 
    0x4a4, 0x4a2, 0x3, 0x2, 0x2, 0x2, 0x4a5, 0x4a9, 0x5, 0x24, 0x13, 0x2, 
    0x4a6, 0x4a9, 0x5, 0x38, 0x1d, 0x2, 0x4a7, 0x4a9, 0x5, 0x28, 0x15, 0x2, 
    0x4a8, 0x4a5, 0x3, 0x2, 0x2, 0x2, 0x4a8, 0x4a6, 0x3, 0x2, 0x2, 0x2, 
    0x4a8, 0x4a7, 0x3, 0x2, 0x2, 0x2, 0x4a9, 0x4ab, 0x3, 0x2, 0x2, 0x2, 
    0x4aa, 0x495, 0x3, 0x2, 0x2, 0x2, 0x4aa, 0x4a2, 0x3, 0x2, 0x2, 0x2, 
    0x4ab, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x4ac, 0x4b1, 0x5, 0x7e, 0x40, 0x2, 
    0x4ad, 0x4ae, 0x7, 0x8, 0x2, 0x2, 0x4ae, 0x4b0, 0x5, 0x7e, 0x40, 0x2, 
    0x4af, 0x4ad, 0x3, 0x2, 0x2, 0x2, 0x4b0, 0x4b3, 0x3, 0x2, 0x2, 0x2, 
    0x4b1, 0x4af, 0x3, 0x2, 0x2, 0x2, 0x4b1, 0x4b2, 0x3, 0x2, 0x2, 0x2, 
    0x4b2, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x4b3, 0x4b1, 0x3, 0x2, 0x2, 0x2, 0x4b4, 
    0x4b9, 0x5, 0xa2, 0x52, 0x2, 0x4b5, 0x4b7, 0x7, 0x1a, 0x2, 0x2, 0x4b6, 
    0x4b5, 0x3, 0x2, 0x2, 0x2, 0x4b6, 0x4b7, 0x3, 0x2, 0x2, 0x2, 0x4b7, 
    0x4b8, 0x3, 0x2, 0x2, 0x2, 0x4b8, 0x4ba, 0x5, 0xca, 0x66, 0x2, 0x4b9, 
    0x4b6, 0x3, 0x2, 0x2, 0x2, 0x4b9, 0x4ba, 0x3, 0x2, 0x2, 0x2, 0x4ba, 
    0x4bd, 0x3, 0x2, 0x2, 0x2, 0x4bb, 0x4bd, 0x5, 0xa8, 0x55, 0x2, 0x4bc, 
    0x4b4, 0x3, 0x2, 0x2, 0x2, 0x4bc, 0x4bb, 0x3, 0x2, 0x2, 0x2, 0x4bd, 
    0x7f, 0x3, 0x2, 0x2, 0x2, 0x4be, 0x4c3, 0x5, 0x82, 0x42, 0x2, 0x4bf, 
    0x4c0, 0x7, 0x8, 0x2, 0x2, 0x4c0, 0x4c2, 0x5, 0x82, 0x42, 0x2, 0x4c1, 
    0x4bf, 0x3, 0x2, 0x2, 0x2, 0x4c2, 0x4c5, 0x3, 0x2, 0x2, 0x2, 0x4c3, 
    0x4c1, 0x3, 0x2, 0x2, 0x2, 0x4c3, 0x4c4, 0x3, 0x2, 0x2, 0x2, 0x4c4, 
    0x81, 0x3, 0x2, 0x2, 0x2, 0x4c5, 0x4c3, 0x3, 0x2, 0x2, 0x2, 0x4c6, 0x4c7, 
    0x5, 0xa2, 0x52, 0x2, 0x4c7, 0x83, 0x3, 0x2, 0x2, 0x2, 0x4c8, 0x4cd, 
    0x5, 0x86, 0x44, 0x2, 0x4c9, 0x4ca, 0x7, 0x8, 0x2, 0x2, 0x4ca, 0x4cc, 
    0x5, 0x86, 0x44, 0x2, 0x4cb, 0x4c9, 0x3, 0x2, 0x2, 0x2, 0x4cc, 0x4cf, 
    0x3, 0x2, 0x2, 0x2, 0x4cd, 0x4cb, 0x3, 0x2, 0x2, 0x2, 0x4cd, 0x4ce, 
    0x3, 0x2, 0x2, 0x2, 0x4ce, 0x85, 0x3, 0x2, 0x2, 0x2, 0x4cf, 0x4cd, 0x3, 
    0x2, 0x2, 0x2, 0x4d0, 0x4d2, 0x5, 0xa2, 0x52, 0x2, 0x4d1, 0x4d3, 0x9, 
    0x10, 0x2, 0x2, 0x4d2, 0x4d1, 0x3, 0x2, 0x2, 0x2, 0x4d2, 0x4d3, 0x3, 
    0x2, 0x2, 0x2, 0x4d3, 0x87, 0x3, 0x2, 0x2, 0x2, 0x4d4, 0x4d9, 0x5, 0x8a, 
    0x46, 0x2, 0x4d5, 0x4d6, 0x7, 0x8, 0x2, 0x2, 0x4d6, 0x4d8, 0x5, 0x8a, 
    0x46, 0x2, 0x4d7, 0x4d5, 0x3, 0x2, 0x2, 0x2, 0x4d8, 0x4db, 0x3, 0x2, 
    0x2, 0x2, 0x4d9, 0x4d7, 0x3, 0x2, 0x2, 0x2, 0x4d9, 0x4da, 0x3, 0x2, 
    0x2, 0x2, 0x4da, 0x89, 0x3, 0x2, 0x2, 0x2, 0x4db, 0x4d9, 0x3, 0x2, 0x2, 
    0x2, 0x4dc, 0x4dd, 0x5, 0xca, 0x66, 0x2, 0x4dd, 0x8b, 0x3, 0x2, 0x2, 
    0x2, 0x4de, 0x4e3, 0x5, 0x8e, 0x48, 0x2, 0x4df, 0x4e0, 0x7, 0x8, 0x2, 
    0x2, 0x4e0, 0x4e2, 0x5, 0x8e, 0x48, 0x2, 0x4e1, 0x4df, 0x3, 0x2, 0x2, 
    0x2, 0x4e2, 0x4e5, 0x3, 0x2, 0x2, 0x2, 0x4e3, 0x4e1, 0x3, 0x2, 0x2, 
    0x2, 0x4e3, 0x4e4, 0x3, 0x2, 0x2, 0x2, 0x4e4, 0x8d, 0x3, 0x2, 0x2, 0x2, 
    0x4e5, 0x4e3, 0x3, 0x2, 0x2, 0x2, 0x4e6, 0x4e7, 0x5, 0xca, 0x66, 0x2, 
    0x4e7, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x4e8, 0x4e9, 0x7, 0xb, 0x2, 0x2, 0x4e9, 
    0x4ea, 0x5, 0x92, 0x4a, 0x2, 0x4ea, 0x4eb, 0x7, 0xc, 0x2, 0x2, 0x4eb, 
    0x91, 0x3, 0x2, 0x2, 0x2, 0x4ec, 0x4f1, 0x5, 0x94, 0x4b, 0x2, 0x4ed, 
    0x4ee, 0x7, 0x8, 0x2, 0x2, 0x4ee, 0x4f0, 0x5, 0x94, 0x4b, 0x2, 0x4ef, 
    0x4ed, 0x3, 0x2, 0x2, 0x2, 0x4f0, 0x4f3, 0x3, 0x2, 0x2, 0x2, 0x4f1, 
    0x4ef, 0x3, 0x2, 0x2, 0x2, 0x4f1, 0x4f2, 0x3, 0x2, 0x2, 0x2, 0x4f2, 
    0x93, 0x3, 0x2, 0x2, 0x2, 0x4f3, 0x4f1, 0x3, 0x2, 0x2, 0x2, 0x4f4, 0x4f5, 
    0x5, 0xca, 0x66, 0x2, 0x4f5, 0x95, 0x3, 0x2, 0x2, 0x2, 0x4f6, 0x4fb, 
    0x5, 0x98, 0x4d, 0x2, 0x4f7, 0x4f8, 0x7, 0x8, 0x2, 0x2, 0x4f8, 0x4fa, 
    0x5, 0x98, 0x4d, 0x2, 0x4f9, 0x4f7, 0x3, 0x2, 0x2, 0x2, 0x4fa, 0x4fd, 
    0x3, 0x2, 0x2, 0x2, 0x4fb, 0x4f9, 0x3, 0x2, 0x2, 0x2, 0x4fb, 0x4fc, 
    0x3, 0x2, 0x2, 0x2, 0x4fc, 0x97, 0x3, 0x2, 0x2, 0x2, 0x4fd, 0x4fb, 0x3, 
    0x2, 0x2, 0x2, 0x4fe, 0x501, 0x5, 0xc0, 0x61, 0x2, 0x4ff, 0x500, 0x7, 
    0x6, 0x2, 0x2, 0x500, 0x502, 0x5, 0xc6, 0x64, 0x2, 0x501, 0x4ff, 0x3, 
    0x2, 0x2, 0x2, 0x501, 0x502, 0x3, 0x2, 0x2, 0x2, 0x502, 0x99, 0x3, 0x2, 
    0x2, 0x2, 0x503, 0x508, 0x5, 0xa2, 0x52, 0x2, 0x504, 0x505, 0x7, 0x8, 
    0x2, 0x2, 0x505, 0x507, 0x5, 0xa2, 0x52, 0x2, 0x506, 0x504, 0x3, 0x2, 
    0x2, 0x2, 0x507, 0x50a, 0x3, 0x2, 0x2, 0x2, 0x508, 0x506, 0x3, 0x2, 
    0x2, 0x2, 0x508, 0x509, 0x3, 0x2, 0x2, 0x2, 0x509, 0x9b, 0x3, 0x2, 0x2, 
    0x2, 0x50a, 0x508, 0x3, 0x2, 0x2, 0x2, 0x50b, 0x50c, 0x5, 0xa2, 0x52, 
    0x2, 0x50c, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x50d, 0x50e, 0x5, 0xa2, 0x52, 
    0x2, 0x50e, 0x9f, 0x3, 0x2, 0x2, 0x2, 0x50f, 0x510, 0x5, 0xa2, 0x52, 
    0x2, 0x510, 0xa1, 0x3, 0x2, 0x2, 0x2, 0x511, 0x512, 0x8, 0x52, 0x1, 
    0x2, 0x512, 0x619, 0x5, 0xd4, 0x6b, 0x2, 0x513, 0x619, 0x5, 0xa4, 0x53, 
    0x2, 0x514, 0x619, 0x5, 0xa6, 0x54, 0x2, 0x515, 0x516, 0x7, 0xb, 0x2, 
    0x2, 0x516, 0x517, 0x5, 0x6a, 0x36, 0x2, 0x517, 0x518, 0x7, 0xc, 0x2, 
    0x2, 0x518, 0x519, 0x5, 0xa2, 0x52, 0x2a, 0x519, 0x619, 0x3, 0x2, 0x2, 
    0x2, 0x51a, 0x51b, 0x9, 0x11, 0x2, 0x2, 0x51b, 0x619, 0x5, 0xa2, 0x52, 
    0x29, 0x51c, 0x51d, 0x7, 0x2b, 0x2, 0x2, 0x51d, 0x51e, 0x7, 0xb, 0x2, 
    0x2, 0x51e, 0x51f, 0x5, 0xa2, 0x52, 0x2, 0x51f, 0x520, 0x9, 0x12, 0x2, 
    0x2, 0x520, 0x521, 0x5, 0x6a, 0x36, 0x2, 0x521, 0x522, 0x7, 0xc, 0x2, 
    0x2, 0x522, 0x619, 0x3, 0x2, 0x2, 0x2, 0x523, 0x524, 0x7, 0xb7, 0x2, 
    0x2, 0x524, 0x525, 0x7, 0xb, 0x2, 0x2, 0x525, 0x526, 0x5, 0xa2, 0x52, 
    0x2, 0x526, 0x527, 0x7, 0xc, 0x2, 0x2, 0x527, 0x619, 0x3, 0x2, 0x2, 
    0x2, 0x528, 0x53c, 0x7, 0x2a, 0x2, 0x2, 0x529, 0x52f, 0x5, 0xa2, 0x52, 
    0x2, 0x52a, 0x52b, 0x7, 0xc3, 0x2, 0x2, 0x52b, 0x52c, 0x5, 0xa2, 0x52, 
    0x2, 0x52c, 0x52d, 0x7, 0xac, 0x2, 0x2, 0x52d, 0x52e, 0x5, 0xa2, 0x52, 
    0x2, 0x52e, 0x530, 0x3, 0x2, 0x2, 0x2, 0x52f, 0x52a, 0x3, 0x2, 0x2, 
    0x2, 0x530, 0x531, 0x3, 0x2, 0x2, 0x2, 0x531, 0x52f, 0x3, 0x2, 0x2, 
    0x2, 0x531, 0x532, 0x3, 0x2, 0x2, 0x2, 0x532, 0x53d, 0x3, 0x2, 0x2, 
    0x2, 0x533, 0x534, 0x7, 0xc3, 0x2, 0x2, 0x534, 0x535, 0x5, 0xa0, 0x51, 
    0x2, 0x535, 0x536, 0x7, 0xac, 0x2, 0x2, 0x536, 0x537, 0x5, 0xa2, 0x52, 
    0x2, 0x537, 0x539, 0x3, 0x2, 0x2, 0x2, 0x538, 0x533, 0x3, 0x2, 0x2, 
    0x2, 0x539, 0x53a, 0x3, 0x2, 0x2, 0x2, 0x53a, 0x538, 0x3, 0x2, 0x2, 
    0x2, 0x53a, 0x53b, 0x3, 0x2, 0x2, 0x2, 0x53b, 0x53d, 0x3, 0x2, 0x2, 
    0x2, 0x53c, 0x529, 0x3, 0x2, 0x2, 0x2, 0x53c, 0x538, 0x3, 0x2, 0x2, 
    0x2, 0x53d, 0x540, 0x3, 0x2, 0x2, 0x2, 0x53e, 0x53f, 0x7, 0x49, 0x2, 
    0x2, 0x53f, 0x541, 0x5, 0xa2, 0x52, 0x2, 0x540, 0x53e, 0x3, 0x2, 0x2, 
    0x2, 0x540, 0x541, 0x3, 0x2, 0x2, 0x2, 0x541, 0x542, 0x3, 0x2, 0x2, 
    0x2, 0x542, 0x543, 0x7, 0x4a, 0x2, 0x2, 0x543, 0x619, 0x3, 0x2, 0x2, 
    0x2, 0x544, 0x545, 0x7, 0x5f, 0x2, 0x2, 0x545, 0x546, 0x7, 0xb, 0x2, 
    0x2, 0x546, 0x547, 0x5, 0xa0, 0x51, 0x2, 0x547, 0x548, 0x7, 0x8, 0x2, 
    0x2, 0x548, 0x549, 0x5, 0xa2, 0x52, 0x2, 0x549, 0x54a, 0x7, 0x8, 0x2, 
    0x2, 0x54a, 0x54b, 0x5, 0xa2, 0x52, 0x2, 0x54b, 0x54c, 0x7, 0xc, 0x2, 
    0x2, 0x54c, 0x619, 0x3, 0x2, 0x2, 0x2, 0x54d, 0x54e, 0x7, 0x60, 0x2, 
    0x2, 0x54e, 0x54f, 0x7, 0xb, 0x2, 0x2, 0x54f, 0x550, 0x5, 0xa2, 0x52, 
    0x2, 0x550, 0x551, 0x7, 0x8, 0x2, 0x2, 0x551, 0x552, 0x5, 0xa2, 0x52, 
    0x2, 0x552, 0x553, 0x7, 0xc, 0x2, 0x2, 0x553, 0x619, 0x3, 0x2, 0x2, 
    0x2, 0x554, 0x555, 0x7, 0x83, 0x2, 0x2, 0x555, 0x556, 0x7, 0xb, 0x2, 
    0x2, 0x556, 0x557, 0x5, 0xa2, 0x52, 0x2, 0x557, 0x558, 0x7, 0x8, 0x2, 
    0x2, 0x558, 0x559, 0x5, 0xa2, 0x52, 0x2, 0x559, 0x55a, 0x7, 0xc, 0x2, 
    0x2, 0x55a, 0x619, 0x3, 0x2, 0x2, 0x2, 0x55b, 0x55c, 0x7, 0x30, 0x2, 
    0x2, 0x55c, 0x565, 0x7, 0xb, 0x2, 0x2, 0x55d, 0x562, 0x5, 0xa2, 0x52, 
    0x2, 0x55e, 0x55f, 0x7, 0x8, 0x2, 0x2, 0x55f, 0x561, 0x5, 0xa2, 0x52, 
    0x2, 0x560, 0x55e, 0x3, 0x2, 0x2, 0x2, 0x561, 0x564, 0x3, 0x2, 0x2, 
    0x2, 0x562, 0x560, 0x3, 0x2, 0x2, 0x2, 0x562, 0x563, 0x3, 0x2, 0x2, 
    0x2, 0x563, 0x566, 0x3, 0x2, 0x2, 0x2, 0x564, 0x562, 0x3, 0x2, 0x2, 
    0x2, 0x565, 0x55d, 0x3, 0x2, 0x2, 0x2, 0x565, 0x566, 0x3, 0x2, 0x2, 
    0x2, 0x566, 0x567, 0x3, 0x2, 0x2, 0x2, 0x567, 0x619, 0x7, 0xc, 0x2, 
    0x2, 0x568, 0x569, 0x7, 0x41, 0x2, 0x2, 0x569, 0x572, 0x7, 0xb, 0x2, 
    0x2, 0x56a, 0x56f, 0x5, 0xa2, 0x52, 0x2, 0x56b, 0x56c, 0x7, 0x8, 0x2, 
    0x2, 0x56c, 0x56e, 0x5, 0xa2, 0x52, 0x2, 0x56d, 0x56b, 0x3, 0x2, 0x2, 
    0x2, 0x56e, 0x571, 0x3, 0x2, 0x2, 0x2, 0x56f, 0x56d, 0x3, 0x2, 0x2, 
    0x2, 0x56f, 0x570, 0x3, 0x2, 0x2, 0x2, 0x570, 0x573, 0x3, 0x2, 0x2, 
    0x2, 0x571, 0x56f, 0x3, 0x2, 0x2, 0x2, 0x572, 0x56a, 0x3, 0x2, 0x2, 
    0x2, 0x572, 0x573, 0x3, 0x2, 0x2, 0x2, 0x573, 0x574, 0x3, 0x2, 0x2, 
    0x2, 0x574, 0x619, 0x7, 0xc, 0x2, 0x2, 0x575, 0x576, 0x7, 0x32, 0x2, 
    0x2, 0x576, 0x57f, 0x7, 0xb, 0x2, 0x2, 0x577, 0x57c, 0x5, 0xa2, 0x52, 
    0x2, 0x578, 0x579, 0x7, 0x8, 0x2, 0x2, 0x579, 0x57b, 0x5, 0xa2, 0x52, 
    0x2, 0x57a, 0x578, 0x3, 0x2, 0x2, 0x2, 0x57b, 0x57e, 0x3, 0x2, 0x2, 
    0x2, 0x57c, 0x57a, 0x3, 0x2, 0x2, 0x2, 0x57c, 0x57d, 0x3, 0x2, 0x2, 
    0x2, 0x57d, 0x580, 0x3, 0x2, 0x2, 0x2, 0x57e, 0x57c, 0x3, 0x2, 0x2, 
    0x2, 0x57f, 0x577, 0x3, 0x2, 0x2, 0x2, 0x57f, 0x580, 0x3, 0x2, 0x2, 
    0x2, 0x580, 0x581, 0x3, 0x2, 0x2, 0x2, 0x581, 0x619, 0x7, 0xc, 0x2, 
    0x2, 0x582, 0x583, 0x7, 0x8a, 0x2, 0x2, 0x583, 0x584, 0x7, 0xb, 0x2, 
    0x2, 0x584, 0x585, 0x5, 0xa2, 0x52, 0x2, 0x585, 0x586, 0x9, 0x13, 0x2, 
    0x2, 0x586, 0x587, 0x5, 0xa2, 0x52, 0x2, 0x587, 0x588, 0x9, 0x14, 0x2, 
    0x2, 0x588, 0x58b, 0x5, 0xa2, 0x52, 0x2, 0x589, 0x58a, 0x9, 0x15, 0x2, 
    0x2, 0x58a, 0x58c, 0x5, 0xa2, 0x52, 0x2, 0x58b, 0x589, 0x3, 0x2, 0x2, 
    0x2, 0x58b, 0x58c, 0x3, 0x2, 0x2, 0x2, 0x58c, 0x58d, 0x3, 0x2, 0x2, 
    0x2, 0x58d, 0x58e, 0x7, 0xc, 0x2, 0x2, 0x58e, 0x619, 0x3, 0x2, 0x2, 
    0x2, 0x58f, 0x590, 0x7, 0x8d, 0x2, 0x2, 0x590, 0x591, 0x7, 0xb, 0x2, 
    0x2, 0x591, 0x592, 0x5, 0xa2, 0x52, 0x2, 0x592, 0x593, 0x9, 0x16, 0x2, 
    0x2, 0x593, 0x594, 0x5, 0xa2, 0x52, 0x2, 0x594, 0x595, 0x7, 0xc, 0x2, 
    0x2, 0x595, 0x619, 0x3, 0x2, 0x2, 0x2, 0x596, 0x597, 0x9, 0x17, 0x2, 
    0x2, 0x597, 0x5a6, 0x7, 0xb, 0x2, 0x2, 0x598, 0x599, 0x5, 0xa2, 0x52, 
    0x2, 0x599, 0x59a, 0x7, 0x8, 0x2, 0x2, 0x59a, 0x59d, 0x5, 0xa2, 0x52, 
    0x2, 0x59b, 0x59c, 0x7, 0x8, 0x2, 0x2, 0x59c, 0x59e, 0x5, 0xa2, 0x52, 
    0x2, 0x59d, 0x59b, 0x3, 0x2, 0x2, 0x2, 0x59d, 0x59e, 0x3, 0x2, 0x2, 
    0x2, 0x59e, 0x5a7, 0x3, 0x2, 0x2, 0x2, 0x59f, 0x5a0, 0x5, 0xa2, 0x52, 
    0x2, 0x5a0, 0x5a1, 0x7, 0x58, 0x2, 0x2, 0x5a1, 0x5a4, 0x5, 0xa2, 0x52, 
    0x2, 0x5a2, 0x5a3, 0x7, 0x55, 0x2, 0x2, 0x5a3, 0x5a5, 0x5, 0xa2, 0x52, 
    0x2, 0x5a4, 0x5a2, 0x3, 0x2, 0x2, 0x2, 0x5a4, 0x5a5, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x5a7, 0x3, 0x2, 0x2, 0x2, 0x5a6, 0x598, 0x3, 0x2, 0x2, 
    0x2, 0x5a6, 0x59f, 0x3, 0x2, 0x2, 0x2, 0x5a7, 0x5a8, 0x3, 0x2, 0x2, 
    0x2, 0x5a8, 0x5a9, 0x7, 0xc, 0x2, 0x2, 0x5a9, 0x619, 0x3, 0x2, 0x2, 
    0x2, 0x5aa, 0x5ab, 0x7, 0xb2, 0x2, 0x2, 0x5ab, 0x5b9, 0x7, 0xb, 0x2, 
    0x2, 0x5ac, 0x5af, 0x5, 0xa2, 0x52, 0x2, 0x5ad, 0x5ae, 0x7, 0x8, 0x2, 
    0x2, 0x5ae, 0x5b0, 0x5, 0xa2, 0x52, 0x2, 0x5af, 0x5ad, 0x3, 0x2, 0x2, 
    0x2, 0x5af, 0x5b0, 0x3, 0x2, 0x2, 0x2, 0x5b0, 0x5ba, 0x3, 0x2, 0x2, 
    0x2, 0x5b1, 0x5b3, 0x9, 0x18, 0x2, 0x2, 0x5b2, 0x5b1, 0x3, 0x2, 0x2, 
    0x2, 0x5b2, 0x5b3, 0x3, 0x2, 0x2, 0x2, 0x5b3, 0x5b5, 0x3, 0x2, 0x2, 
    0x2, 0x5b4, 0x5b6, 0x5, 0xa2, 0x52, 0x2, 0x5b5, 0x5b4, 0x3, 0x2, 0x2, 
    0x2, 0x5b5, 0x5b6, 0x3, 0x2, 0x2, 0x2, 0x5b6, 0x5b7, 0x3, 0x2, 0x2, 
    0x2, 0x5b7, 0x5b8, 0x7, 0x58, 0x2, 0x2, 0x5b8, 0x5ba, 0x5, 0xa2, 0x52, 
    0x2, 0x5b9, 0x5ac, 0x3, 0x2, 0x2, 0x2, 0x5b9, 0x5b2, 0x3, 0x2, 0x2, 
    0x2, 0x5ba, 0x5bb, 0x3, 0x2, 0x2, 0x2, 0x5bb, 0x5bc, 0x7, 0xc, 0x2, 
    0x2, 0x5bc, 0x619, 0x3, 0x2, 0x2, 0x2, 0x5bd, 0x5be, 0x7, 0x50, 0x2, 
    0x2, 0x5be, 0x5bf, 0x7, 0xb, 0x2, 0x2, 0x5bf, 0x5c0, 0x9, 0x19, 0x2, 
    0x2, 0x5c0, 0x5c1, 0x7, 0x58, 0x2, 0x2, 0x5c1, 0x5c2, 0x5, 0xa2, 0x52, 
    0x2, 0x5c2, 0x5c3, 0x7, 0xc, 0x2, 0x2, 0x5c3, 0x619, 0x3, 0x2, 0x2, 
    0x2, 0x5c4, 0x5c5, 0x7, 0x34, 0x2, 0x2, 0x5c5, 0x5c7, 0x7, 0xb, 0x2, 
    0x2, 0x5c6, 0x5c8, 0x7, 0x16, 0x2, 0x2, 0x5c7, 0x5c6, 0x3, 0x2, 0x2, 
    0x2, 0x5c7, 0x5c8, 0x3, 0x2, 0x2, 0x2, 0x5c8, 0x5c9, 0x3, 0x2, 0x2, 
    0x2, 0x5c9, 0x5ca, 0x7, 0xa, 0x2, 0x2, 0x5ca, 0x619, 0x7, 0xc, 0x2, 
    0x2, 0x5cb, 0x5cc, 0x7, 0x34, 0x2, 0x2, 0x5cc, 0x5cf, 0x7, 0xb, 0x2, 
    0x2, 0x5cd, 0x5d0, 0x7, 0x16, 0x2, 0x2, 0x5ce, 0x5d0, 0x7, 0x46, 0x2, 
    0x2, 0x5cf, 0x5cd, 0x3, 0x2, 0x2, 0x2, 0x5cf, 0x5ce, 0x3, 0x2, 0x2, 
    0x2, 0x5cf, 0x5d0, 0x3, 0x2, 0x2, 0x2, 0x5d0, 0x5d1, 0x3, 0x2, 0x2, 
    0x2, 0x5d1, 0x5d2, 0x5, 0xa2, 0x52, 0x2, 0x5d2, 0x5d3, 0x7, 0xc, 0x2, 
    0x2, 0x5d3, 0x619, 0x3, 0x2, 0x2, 0x2, 0x5d4, 0x5d5, 0x7, 0xa6, 0x2, 
    0x2, 0x5d5, 0x5d8, 0x7, 0xb, 0x2, 0x2, 0x5d6, 0x5d9, 0x7, 0x16, 0x2, 
    0x2, 0x5d7, 0x5d9, 0x7, 0x46, 0x2, 0x2, 0x5d8, 0x5d6, 0x3, 0x2, 0x2, 
    0x2, 0x5d8, 0x5d7, 0x3, 0x2, 0x2, 0x2, 0x5d8, 0x5d9, 0x3, 0x2, 0x2, 
    0x2, 0x5d9, 0x5da, 0x3, 0x2, 0x2, 0x2, 0x5da, 0x5db, 0x5, 0xa2, 0x52, 
    0x2, 0x5db, 0x5dc, 0x7, 0xc, 0x2, 0x2, 0x5dc, 0x619, 0x3, 0x2, 0x2, 
    0x2, 0x5dd, 0x5de, 0x7, 0x1c, 0x2, 0x2, 0x5de, 0x5e1, 0x7, 0xb, 0x2, 
    0x2, 0x5df, 0x5e2, 0x7, 0x16, 0x2, 0x2, 0x5e0, 0x5e2, 0x7, 0x46, 0x2, 
    0x2, 0x5e1, 0x5df, 0x3, 0x2, 0x2, 0x2, 0x5e1, 0x5e0, 0x3, 0x2, 0x2, 
    0x2, 0x5e1, 0x5e2, 0x3, 0x2, 0x2, 0x2, 0x5e2, 0x5e3, 0x3, 0x2, 0x2, 
    0x2, 0x5e3, 0x5e4, 0x5, 0xa2, 0x52, 0x2, 0x5e4, 0x5e5, 0x7, 0xc, 0x2, 
    0x2, 0x5e5, 0x619, 0x3, 0x2, 0x2, 0x2, 0x5e6, 0x5e7, 0x7, 0x77, 0x2, 
    0x2, 0x5e7, 0x5ea, 0x7, 0xb, 0x2, 0x2, 0x5e8, 0x5eb, 0x7, 0x16, 0x2, 
    0x2, 0x5e9, 0x5eb, 0x7, 0x46, 0x2, 0x2, 0x5ea, 0x5e8, 0x3, 0x2, 0x2, 
    0x2, 0x5ea, 0x5e9, 0x3, 0x2, 0x2, 0x2, 0x5ea, 0x5eb, 0x3, 0x2, 0x2, 
    0x2, 0x5eb, 0x5ec, 0x3, 0x2, 0x2, 0x2, 0x5ec, 0x5ed, 0x5, 0xa2, 0x52, 
    0x2, 0x5ed, 0x5ee, 0x7, 0xc, 0x2, 0x2, 0x5ee, 0x619, 0x3, 0x2, 0x2, 
    0x2, 0x5ef, 0x5f0, 0x7, 0x76, 0x2, 0x2, 0x5f0, 0x5f3, 0x7, 0xb, 0x2, 
    0x2, 0x5f1, 0x5f4, 0x7, 0x16, 0x2, 0x2, 0x5f2, 0x5f4, 0x7, 0x46, 0x2, 
    0x2, 0x5f3, 0x5f1, 0x3, 0x2, 0x2, 0x2, 0x5f3, 0x5f2, 0x3, 0x2, 0x2, 
    0x2, 0x5f3, 0x5f4, 0x3, 0x2, 0x2, 0x2, 0x5f4, 0x5f5, 0x3, 0x2, 0x2, 
    0x2, 0x5f5, 0x5f6, 0x5, 0xa2, 0x52, 0x2, 0x5f6, 0x5f7, 0x7, 0xc, 0x2, 
    0x2, 0x5f7, 0x619, 0x3, 0x2, 0x2, 0x2, 0x5f8, 0x5f9, 0x5, 0xac, 0x57, 
    0x2, 0x5f9, 0x602, 0x7, 0xb, 0x2, 0x2, 0x5fa, 0x5ff, 0x5, 0xa2, 0x52, 
    0x2, 0x5fb, 0x5fc, 0x7, 0x8, 0x2, 0x2, 0x5fc, 0x5fe, 0x5, 0xa2, 0x52, 
    0x2, 0x5fd, 0x5fb, 0x3, 0x2, 0x2, 0x2, 0x5fe, 0x601, 0x3, 0x2, 0x2, 
    0x2, 0x5ff, 0x5fd, 0x3, 0x2, 0x2, 0x2, 0x5ff, 0x600, 0x3, 0x2, 0x2, 
    0x2, 0x600, 0x603, 0x3, 0x2, 0x2, 0x2, 0x601, 0x5ff, 0x3, 0x2, 0x2, 
    0x2, 0x602, 0x5fa, 0x3, 0x2, 0x2, 0x2, 0x602, 0x603, 0x3, 0x2, 0x2, 
    0x2, 0x603, 0x604, 0x3, 0x2, 0x2, 0x2, 0x604, 0x605, 0x7, 0xc, 0x2, 
    0x2, 0x605, 0x619, 0x3, 0x2, 0x2, 0x2, 0x606, 0x607, 0x7, 0xb, 0x2, 
    0x2, 0x607, 0x608, 0x5, 0xa2, 0x52, 0x2, 0x608, 0x609, 0x7, 0xc, 0x2, 
    0x2, 0x609, 0x619, 0x3, 0x2, 0x2, 0x2, 0x60a, 0x60b, 0x7, 0xb, 0x2, 
    0x2, 0x60b, 0x60c, 0x5, 0x1e, 0x10, 0x2, 0x60c, 0x60d, 0x7, 0xc, 0x2, 
    0x2, 0x60d, 0x619, 0x3, 0x2, 0x2, 0x2, 0x60e, 0x610, 0x7, 0x80, 0x2, 
    0x2, 0x60f, 0x60e, 0x3, 0x2, 0x2, 0x2, 0x60f, 0x610, 0x3, 0x2, 0x2, 
    0x2, 0x610, 0x611, 0x3, 0x2, 0x2, 0x2, 0x611, 0x612, 0x7, 0x4e, 0x2, 
    0x2, 0x612, 0x613, 0x7, 0xb, 0x2, 0x2, 0x613, 0x614, 0x5, 0x1e, 0x10, 
    0x2, 0x614, 0x615, 0x7, 0xc, 0x2, 0x2, 0x615, 0x619, 0x3, 0x2, 0x2, 
    0x2, 0x616, 0x617, 0x7, 0x80, 0x2, 0x2, 0x617, 0x619, 0x5, 0xa2, 0x52, 
    0x6, 0x618, 0x511, 0x3, 0x2, 0x2, 0x2, 0x618, 0x513, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x514, 0x3, 0x2, 0x2, 0x2, 0x618, 0x515, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x51a, 0x3, 0x2, 0x2, 0x2, 0x618, 0x51c, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x523, 0x3, 0x2, 0x2, 0x2, 0x618, 0x528, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x544, 0x3, 0x2, 0x2, 0x2, 0x618, 0x54d, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x554, 0x3, 0x2, 0x2, 0x2, 0x618, 0x55b, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x568, 0x3, 0x2, 0x2, 0x2, 0x618, 0x575, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x582, 0x3, 0x2, 0x2, 0x2, 0x618, 0x58f, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x596, 0x3, 0x2, 0x2, 0x2, 0x618, 0x5aa, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x5bd, 0x3, 0x2, 0x2, 0x2, 0x618, 0x5c4, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x5cb, 0x3, 0x2, 0x2, 0x2, 0x618, 0x5d4, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x5dd, 0x3, 0x2, 0x2, 0x2, 0x618, 0x5e6, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x5ef, 0x3, 0x2, 0x2, 0x2, 0x618, 0x5f8, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x606, 0x3, 0x2, 0x2, 0x2, 0x618, 0x60a, 0x3, 0x2, 0x2, 
    0x2, 0x618, 0x60f, 0x3, 0x2, 0x2, 0x2, 0x618, 0x616, 0x3, 0x2, 0x2, 
    0x2, 0x619, 0x677, 0x3, 0x2, 0x2, 0x2, 0x61a, 0x61b, 0xc, 0x27, 0x2, 
    0x2, 0x61b, 0x61c, 0x9, 0x1a, 0x2, 0x2, 0x61c, 0x676, 0x5, 0xa2, 0x52, 
    0x28, 0x61d, 0x61e, 0xc, 0x26, 0x2, 0x2, 0x61e, 0x61f, 0x9, 0x1b, 0x2, 
    0x2, 0x61f, 0x676, 0x5, 0xa2, 0x52, 0x27, 0x620, 0x621, 0xc, 0x25, 0x2, 
    0x2, 0x621, 0x622, 0x9, 0x1c, 0x2, 0x2, 0x622, 0x676, 0x5, 0xa2, 0x52, 
    0x26, 0x623, 0x625, 0xc, 0xc, 0x2, 0x2, 0x624, 0x626, 0x7, 0x80, 0x2, 
    0x2, 0x625, 0x624, 0x3, 0x2, 0x2, 0x2, 0x625, 0x626, 0x3, 0x2, 0x2, 
    0x2, 0x626, 0x627, 0x3, 0x2, 0x2, 0x2, 0x627, 0x628, 0x7, 0x6e, 0x2, 
    0x2, 0x628, 0x676, 0x5, 0xa2, 0x52, 0xd, 0x629, 0x62b, 0xc, 0xb, 0x2, 
    0x2, 0x62a, 0x62c, 0x7, 0x80, 0x2, 0x2, 0x62b, 0x62a, 0x3, 0x2, 0x2, 
    0x2, 0x62b, 0x62c, 0x3, 0x2, 0x2, 0x2, 0x62c, 0x62d, 0x3, 0x2, 0x2, 
    0x2, 0x62d, 0x62e, 0x7, 0x6e, 0x2, 0x2, 0x62e, 0x62f, 0x5, 0xa2, 0x52, 
    0x2, 0x62f, 0x630, 0x7, 0x4b, 0x2, 0x2, 0x630, 0x631, 0x5, 0xa2, 0x52, 
    0xc, 0x631, 0x676, 0x3, 0x2, 0x2, 0x2, 0x632, 0x633, 0xc, 0x5, 0x2, 
    0x2, 0x633, 0x634, 0x6, 0x52, 0xd, 0x2, 0x634, 0x635, 0x7, 0x17, 0x2, 
    0x2, 0x635, 0x676, 0x5, 0xa2, 0x52, 0x6, 0x636, 0x637, 0xc, 0x4, 0x2, 
    0x2, 0x637, 0x638, 0x7, 0xc8, 0x2, 0x2, 0x638, 0x676, 0x5, 0xa2, 0x52, 
    0x5, 0x639, 0x63a, 0xc, 0x3, 0x2, 0x2, 0x63a, 0x63b, 0x7, 0x86, 0x2, 
    0x2, 0x63b, 0x676, 0x5, 0xa2, 0x52, 0x4, 0x63c, 0x63d, 0xc, 0x28, 0x2, 
    0x2, 0x63d, 0x63e, 0x7, 0xe, 0x2, 0x2, 0x63e, 0x63f, 0x5, 0xae, 0x58, 
    0x2, 0x63f, 0x648, 0x7, 0xb, 0x2, 0x2, 0x640, 0x645, 0x5, 0xa2, 0x52, 
    0x2, 0x641, 0x642, 0x7, 0x8, 0x2, 0x2, 0x642, 0x644, 0x5, 0xa2, 0x52, 
    0x2, 0x643, 0x641, 0x3, 0x2, 0x2, 0x2, 0x644, 0x647, 0x3, 0x2, 0x2, 
    0x2, 0x645, 0x643, 0x3, 0x2, 0x2, 0x2, 0x645, 0x646, 0x3, 0x2, 0x2, 
    0x2, 0x646, 0x649, 0x3, 0x2, 0x2, 0x2, 0x647, 0x645, 0x3, 0x2, 0x2, 
    0x2, 0x648, 0x640, 0x3, 0x2, 0x2, 0x2, 0x648, 0x649, 0x3, 0x2, 0x2, 
    0x2, 0x649, 0x64a, 0x3, 0x2, 0x2, 0x2, 0x64a, 0x64b, 0x7, 0xc, 0x2, 
    0x2, 0x64b, 0x676, 0x3, 0x2, 0x2, 0x2, 0x64c, 0x64d, 0xc, 0xd, 0x2, 
    0x2, 0x64d, 0x64f, 0x7, 0x69, 0x2, 0x2, 0x64e, 0x650, 0x7, 0x80, 0x2, 
    0x2, 0x64f, 0x64e, 0x3, 0x2, 0x2, 0x2, 0x64f, 0x650, 0x3, 0x2, 0x2, 
    0x2, 0x650, 0x651, 0x3, 0x2, 0x2, 0x2, 0x651, 0x676, 0x7, 0x82, 0x2, 
    0x2, 0x652, 0x654, 0xc, 0x9, 0x2, 0x2, 0x653, 0x655, 0x7, 0x80, 0x2, 
    0x2, 0x654, 0x653, 0x3, 0x2, 0x2, 0x2, 0x654, 0x655, 0x3, 0x2, 0x2, 
    0x2, 0x655, 0x656, 0x3, 0x2, 0x2, 0x2, 0x656, 0x657, 0x7, 0x61, 0x2, 
    0x2, 0x657, 0x658, 0x7, 0xb, 0x2, 0x2, 0x658, 0x659, 0x5, 0x1e, 0x10, 
    0x2, 0x659, 0x65a, 0x7, 0xc, 0x2, 0x2, 0x65a, 0x676, 0x3, 0x2, 0x2, 
    0x2, 0x65b, 0x65d, 0xc, 0x8, 0x2, 0x2, 0x65c, 0x65e, 0x7, 0x80, 0x2, 
    0x2, 0x65d, 0x65c, 0x3, 0x2, 0x2, 0x2, 0x65d, 0x65e, 0x3, 0x2, 0x2, 
    0x2, 0x65e, 0x65f, 0x3, 0x2, 0x2, 0x2, 0x65f, 0x660, 0x7, 0x61, 0x2, 
    0x2, 0x660, 0x669, 0x7, 0xb, 0x2, 0x2, 0x661, 0x666, 0x5, 0xa2, 0x52, 
    0x2, 0x662, 0x663, 0x7, 0x8, 0x2, 0x2, 0x663, 0x665, 0x5, 0xa2, 0x52, 
    0x2, 0x664, 0x662, 0x3, 0x2, 0x2, 0x2, 0x665, 0x668, 0x3, 0x2, 0x2, 
    0x2, 0x666, 0x664, 0x3, 0x2, 0x2, 0x2, 0x666, 0x667, 0x3, 0x2, 0x2, 
    0x2, 0x667, 0x66a, 0x3, 0x2, 0x2, 0x2, 0x668, 0x666, 0x3, 0x2, 0x2, 
    0x2, 0x669, 0x661, 0x3, 0x2, 0x2, 0x2, 0x669, 0x66a, 0x3, 0x2, 0x2, 
    0x2, 0x66a, 0x66b, 0x3, 0x2, 0x2, 0x2, 0x66b, 0x676, 0x7, 0xc, 0x2, 
    0x2, 0x66c, 0x66e, 0xc, 0x7, 0x2, 0x2, 0x66d, 0x66f, 0x7, 0x80, 0x2, 
    0x2, 0x66e, 0x66d, 0x3, 0x2, 0x2, 0x2, 0x66e, 0x66f, 0x3, 0x2, 0x2, 
    0x2, 0x66f, 0x670, 0x3, 0x2, 0x2, 0x2, 0x670, 0x671, 0x7, 0x1d, 0x2, 
    0x2, 0x671, 0x672, 0x5, 0xa2, 0x52, 0x2, 0x672, 0x673, 0x7, 0x17, 0x2, 
    0x2, 0x673, 0x674, 0x5, 0xa2, 0x52, 0x2, 0x674, 0x676, 0x3, 0x2, 0x2, 
    0x2, 0x675, 0x61a, 0x3, 0x2, 0x2, 0x2, 0x675, 0x61d, 0x3, 0x2, 0x2, 
    0x2, 0x675, 0x620, 0x3, 0x2, 0x2, 0x2, 0x675, 0x623, 0x3, 0x2, 0x2, 
    0x2, 0x675, 0x629, 0x3, 0x2, 0x2, 0x2, 0x675, 0x632, 0x3, 0x2, 0x2, 
    0x2, 0x675, 0x636, 0x3, 0x2, 0x2, 0x2, 0x675, 0x639, 0x3, 0x2, 0x2, 
    0x2, 0x675, 0x63c, 0x3, 0x2, 0x2, 0x2, 0x675, 0x64c, 0x3, 0x2, 0x2, 
    0x2, 0x675, 0x652, 0x3, 0x2, 0x2, 0x2, 0x675, 0x65b, 0x3, 0x2, 0x2, 
    0x2, 0x675, 0x66c, 0x3, 0x2, 0x2, 0x2, 0x676, 0x679, 0x3, 0x2, 0x2, 
    0x2, 0x677, 0x675, 0x3, 0x2, 0x2, 0x2, 0x677, 0x678, 0x3, 0x2, 0x2, 
    0x2, 0x678, 0xa3, 0x3, 0x2, 0x2, 0x2, 0x679, 0x677, 0x3, 0x2, 0x2, 0x2, 
    0x67a, 0x67b, 0x9, 0x1d, 0x2, 0x2, 0x67b, 0xa5, 0x3, 0x2, 0x2, 0x2, 
    0x67c, 0x67d, 0x5, 0xb0, 0x59, 0x2, 0x67d, 0x67e, 0x7, 0xe, 0x2, 0x2, 
    0x67e, 0x680, 0x3, 0x2, 0x2, 0x2, 0x67f, 0x67c, 0x3, 0x2, 0x2, 0x2, 
    0x67f, 0x680, 0x3, 0x2, 0x2, 0x2, 0x680, 0x681, 0x3, 0x2, 0x2, 0x2, 
    0x681, 0x682, 0x5, 0xb2, 0x5a, 0x2, 0x682, 0x683, 0x7, 0xe, 0x2, 0x2, 
    0x683, 0x685, 0x3, 0x2, 0x2, 0x2, 0x684, 0x67f, 0x3, 0x2, 0x2, 0x2, 
    0x684, 0x685, 0x3, 0x2, 0x2, 0x2, 0x685, 0x686, 0x3, 0x2, 0x2, 0x2, 
    0x686, 0x687, 0x5, 0xb4, 0x5b, 0x2, 0x687, 0x688, 0x7, 0xe, 0x2, 0x2, 
    0x688, 0x68a, 0x3, 0x2, 0x2, 0x2, 0x689, 0x684, 0x3, 0x2, 0x2, 0x2, 
    0x689, 0x68a, 0x3, 0x2, 0x2, 0x2, 0x68a, 0x68b, 0x3, 0x2, 0x2, 0x2, 
    0x68b, 0x68c, 0x5, 0xb8, 0x5d, 0x2, 0x68c, 0xa7, 0x3, 0x2, 0x2, 0x2, 
    0x68d, 0x68e, 0x5, 0xb0, 0x59, 0x2, 0x68e, 0x68f, 0x7, 0xe, 0x2, 0x2, 
    0x68f, 0x691, 0x3, 0x2, 0x2, 0x2, 0x690, 0x68d, 0x3, 0x2, 0x2, 0x2, 
    0x690, 0x691, 0x3, 0x2, 0x2, 0x2, 0x691, 0x692, 0x3, 0x2, 0x2, 0x2, 
    0x692, 0x693, 0x5, 0xb2, 0x5a, 0x2, 0x693, 0x694, 0x7, 0xe, 0x2, 0x2, 
    0x694, 0x696, 0x3, 0x2, 0x2, 0x2, 0x695, 0x690, 0x3, 0x2, 0x2, 0x2, 
    0x695, 0x696, 0x3, 0x2, 0x2, 0x2, 0x696, 0x697, 0x3, 0x2, 0x2, 0x2, 
    0x697, 0x698, 0x5, 0xb4, 0x5b, 0x2, 0x698, 0x699, 0x7, 0xe, 0x2, 0x2, 
    0x699, 0x69b, 0x3, 0x2, 0x2, 0x2, 0x69a, 0x695, 0x3, 0x2, 0x2, 0x2, 
    0x69a, 0x69b, 0x3, 0x2, 0x2, 0x2, 0x69b, 0x69c, 0x3, 0x2, 0x2, 0x2, 
    0x69c, 0x69d, 0x7, 0xa, 0x2, 0x2, 0x69d, 0xa9, 0x3, 0x2, 0x2, 0x2, 0x69e, 
    0x69f, 0x5, 0xcc, 0x67, 0x2, 0x69f, 0x6a0, 0x7, 0xe, 0x2, 0x2, 0x6a0, 
    0x6a2, 0x3, 0x2, 0x2, 0x2, 0x6a1, 0x69e, 0x3, 0x2, 0x2, 0x2, 0x6a2, 
    0x6a5, 0x3, 0x2, 0x2, 0x2, 0x6a3, 0x6a1, 0x3, 0x2, 0x2, 0x2, 0x6a3, 
    0x6a4, 0x3, 0x2, 0x2, 0x2, 0x6a4, 0x6a6, 0x3, 0x2, 0x2, 0x2, 0x6a5, 
    0x6a3, 0x3, 0x2, 0x2, 0x2, 0x6a6, 0x6a7, 0x5, 0xca, 0x66, 0x2, 0x6a7, 
    0xab, 0x3, 0x2, 0x2, 0x2, 0x6a8, 0x6a9, 0x5, 0xca, 0x66, 0x2, 0x6a9, 
    0xad, 0x3, 0x2, 0x2, 0x2, 0x6aa, 0x6ab, 0x5, 0xca, 0x66, 0x2, 0x6ab, 
    0xaf, 0x3, 0x2, 0x2, 0x2, 0x6ac, 0x6ad, 0x5, 0xca, 0x66, 0x2, 0x6ad, 
    0xb1, 0x3, 0x2, 0x2, 0x2, 0x6ae, 0x6af, 0x5, 0xca, 0x66, 0x2, 0x6af, 
    0xb3, 0x3, 0x2, 0x2, 0x2, 0x6b0, 0x6b1, 0x5, 0xca, 0x66, 0x2, 0x6b1, 
    0xb5, 0x3, 0x2, 0x2, 0x2, 0x6b2, 0x6b3, 0x5, 0xca, 0x66, 0x2, 0x6b3, 
    0xb7, 0x3, 0x2, 0x2, 0x2, 0x6b4, 0x6b5, 0x5, 0xca, 0x66, 0x2, 0x6b5, 
    0xb9, 0x3, 0x2, 0x2, 0x2, 0x6b6, 0x6b7, 0x5, 0xca, 0x66, 0x2, 0x6b7, 
    0xbb, 0x3, 0x2, 0x2, 0x2, 0x6b8, 0x6b9, 0x5, 0xca, 0x66, 0x2, 0x6b9, 
    0xbd, 0x3, 0x2, 0x2, 0x2, 0x6ba, 0x6bb, 0x5, 0xca, 0x66, 0x2, 0x6bb, 
    0xbf, 0x3, 0x2, 0x2, 0x2, 0x6bc, 0x6bd, 0x5, 0xca, 0x66, 0x2, 0x6bd, 
    0xc1, 0x3, 0x2, 0x2, 0x2, 0x6be, 0x6bf, 0x5, 0xca, 0x66, 0x2, 0x6bf, 
    0xc3, 0x3, 0x2, 0x2, 0x2, 0x6c0, 0x6c1, 0x7, 0x5b, 0x2, 0x2, 0x6c1, 
    0xc5, 0x3, 0x2, 0x2, 0x2, 0x6c2, 0x6c5, 0x5, 0xd6, 0x6c, 0x2, 0x6c3, 
    0x6c5, 0x5, 0xca, 0x66, 0x2, 0x6c4, 0x6c2, 0x3, 0x2, 0x2, 0x2, 0x6c4, 
    0x6c3, 0x3, 0x2, 0x2, 0x2, 0x6c5, 0xc7, 0x3, 0x2, 0x2, 0x2, 0x6c6, 0x6ca, 
    0x5, 0xd6, 0x6c, 0x2, 0x6c7, 0x6ca, 0x5, 0xca, 0x66, 0x2, 0x6c8, 0x6ca, 
    0x5, 0xa2, 0x52, 0x2, 0x6c9, 0x6c6, 0x3, 0x2, 0x2, 0x2, 0x6c9, 0x6c7, 
    0x3, 0x2, 0x2, 0x2, 0x6c9, 0x6c8, 0x3, 0x2, 0x2, 0x2, 0x6ca, 0xc9, 0x3, 
    0x2, 0x2, 0x2, 0x6cb, 0x6d0, 0x7, 0xcb, 0x2, 0x2, 0x6cc, 0x6d0, 0x7, 
    0xcc, 0x2, 0x2, 0x6cd, 0x6d0, 0x7, 0xcd, 0x2, 0x2, 0x6ce, 0x6d0, 0x5, 
    0xfc, 0x7f, 0x2, 0x6cf, 0x6cb, 0x3, 0x2, 0x2, 0x2, 0x6cf, 0x6cc, 0x3, 
    0x2, 0x2, 0x2, 0x6cf, 0x6cd, 0x3, 0x2, 0x2, 0x2, 0x6cf, 0x6ce, 0x3, 
    0x2, 0x2, 0x2, 0x6d0, 0xcb, 0x3, 0x2, 0x2, 0x2, 0x6d1, 0x6d2, 0x5, 0xca, 
    0x66, 0x2, 0x6d2, 0xcd, 0x3, 0x2, 0x2, 0x2, 0x6d3, 0x6d4, 0x5, 0xda, 
    0x6e, 0x2, 0x6d4, 0xcf, 0x3, 0x2, 0x2, 0x2, 0x6d5, 0x6d6, 0x5, 0xda, 
    0x6e, 0x2, 0x6d6, 0xd1, 0x3, 0x2, 0x2, 0x2, 0x6d7, 0x6d8, 0x5, 0xda, 
    0x6e, 0x2, 0x6d8, 0xd3, 0x3, 0x2, 0x2, 0x2, 0x6d9, 0x6e4, 0x7, 0x82, 
    0x2, 0x2, 0x6da, 0x6e4, 0x5, 0xd8, 0x6d, 0x2, 0x6db, 0x6e4, 0x5, 0xdc, 
    0x6f, 0x2, 0x6dc, 0x6e4, 0x5, 0xe0, 0x71, 0x2, 0x6dd, 0x6e4, 0x5, 0xe4, 
    0x73, 0x2, 0x6de, 0x6e4, 0x5, 0xe6, 0x74, 0x2, 0x6df, 0x6e4, 0x5, 0xe8, 
    0x75, 0x2, 0x6e0, 0x6e4, 0x5, 0xec, 0x77, 0x2, 0x6e1, 0x6e4, 0x5, 0xea, 
    0x76, 0x2, 0x6e2, 0x6e4, 0x5, 0xee, 0x78, 0x2, 0x6e3, 0x6d9, 0x3, 0x2, 
    0x2, 0x2, 0x6e3, 0x6da, 0x3, 0x2, 0x2, 0x2, 0x6e3, 0x6db, 0x3, 0x2, 
    0x2, 0x2, 0x6e3, 0x6dc, 0x3, 0x2, 0x2, 0x2, 0x6e3, 0x6dd, 0x3, 0x2, 
    0x2, 0x2, 0x6e3, 0x6de, 0x3, 0x2, 0x2, 0x2, 0x6e3, 0x6df, 0x3, 0x2, 
    0x2, 0x2, 0x6e3, 0x6e0, 0x3, 0x2, 0x2, 0x2, 0x6e3, 0x6e1, 0x3, 0x2, 
    0x2, 0x2, 0x6e3, 0x6e2, 0x3, 0x2, 0x2, 0x2, 0x6e4, 0xd5, 0x3, 0x2, 0x2, 
    0x2, 0x6e5, 0x6ec, 0x7, 0x82, 0x2, 0x2, 0x6e6, 0x6ec, 0x5, 0xd8, 0x6d, 
    0x2, 0x6e7, 0x6ec, 0x5, 0xdc, 0x6f, 0x2, 0x6e8, 0x6ec, 0x5, 0xe0, 0x71, 
    0x2, 0x6e9, 0x6ec, 0x5, 0xe4, 0x73, 0x2, 0x6ea, 0x6ec, 0x5, 0xec, 0x77, 
    0x2, 0x6eb, 0x6e5, 0x3, 0x2, 0x2, 0x2, 0x6eb, 0x6e6, 0x3, 0x2, 0x2, 
    0x2, 0x6eb, 0x6e7, 0x3, 0x2, 0x2, 0x2, 0x6eb, 0x6e8, 0x3, 0x2, 0x2, 
    0x2, 0x6eb, 0x6e9, 0x3, 0x2, 0x2, 0x2, 0x6eb, 0x6ea, 0x3, 0x2, 0x2, 
    0x2, 0x6ec, 0xd7, 0x3, 0x2, 0x2, 0x2, 0x6ed, 0x6ee, 0x9, 0x1e, 0x2, 
    0x2, 0x6ee, 0xd9, 0x3, 0x2, 0x2, 0x2, 0x6ef, 0x6f0, 0x7, 0xd2, 0x2, 
    0x2, 0x6f0, 0xdb, 0x3, 0x2, 0x2, 0x2, 0x6f1, 0x6f4, 0x7, 0xf, 0x2, 0x2, 
    0x6f2, 0x6f4, 0x7, 0x9, 0x2, 0x2, 0x6f3, 0x6f1, 0x3, 0x2, 0x2, 0x2, 
    0x6f3, 0x6f2, 0x3, 0x2, 0x2, 0x2, 0x6f3, 0x6f4, 0x3, 0x2, 0x2, 0x2, 
    0x6f4, 0x6f5, 0x3, 0x2, 0x2, 0x2, 0x6f5, 0x6f6, 0x7, 0xd2, 0x2, 0x2, 
    0x6f6, 0xdd, 0x3, 0x2, 0x2, 0x2, 0x6f7, 0x6f8, 0x7, 0xd1, 0x2, 0x2, 
    0x6f8, 0xdf, 0x3, 0x2, 0x2, 0x2, 0x6f9, 0x6fc, 0x7, 0xf, 0x2, 0x2, 0x6fa, 
    0x6fc, 0x7, 0x9, 0x2, 0x2, 0x6fb, 0x6f9, 0x3, 0x2, 0x2, 0x2, 0x6fb, 
    0x6fa, 0x3, 0x2, 0x2, 0x2, 0x6fb, 0x6fc, 0x3, 0x2, 0x2, 0x2, 0x6fc, 
    0x6fd, 0x3, 0x2, 0x2, 0x2, 0x6fd, 0x6fe, 0x7, 0xd1, 0x2, 0x2, 0x6fe, 
    0xe1, 0x3, 0x2, 0x2, 0x2, 0x6ff, 0x700, 0x7, 0xd0, 0x2, 0x2, 0x700, 
    0xe3, 0x3, 0x2, 0x2, 0x2, 0x701, 0x704, 0x7, 0xf, 0x2, 0x2, 0x702, 0x704, 
    0x7, 0x9, 0x2, 0x2, 0x703, 0x701, 0x3, 0x2, 0x2, 0x2, 0x703, 0x702, 
    0x3, 0x2, 0x2, 0x2, 0x703, 0x704, 0x3, 0x2, 0x2, 0x2, 0x704, 0x705, 
    0x3, 0x2, 0x2, 0x2, 0x705, 0x706, 0x7, 0xd0, 0x2, 0x2, 0x706, 0xe5, 
    0x3, 0x2, 0x2, 0x2, 0x707, 0x708, 0x7, 0x3c, 0x2, 0x2, 0x708, 0x709, 
    0x5, 0xec, 0x77, 0x2, 0x709, 0xe7, 0x3, 0x2, 0x2, 0x2, 0x70a, 0x70b, 
    0x7, 0xad, 0x2, 0x2, 0x70b, 0x70c, 0x5, 0xec, 0x77, 0x2, 0x70c, 0xe9, 
    0x3, 0x2, 0x2, 0x2, 0x70d, 0x70e, 0x7, 0xae, 0x2, 0x2, 0x70e, 0x70f, 
    0x5, 0xec, 0x77, 0x2, 0x70f, 0xeb, 0x3, 0x2, 0x2, 0x2, 0x710, 0x711, 
    0x9, 0x1f, 0x2, 0x2, 0x711, 0xed, 0x3, 0x2, 0x2, 0x2, 0x712, 0x715, 
    0x5, 0xf0, 0x79, 0x2, 0x713, 0x715, 0x5, 0xf2, 0x7a, 0x2, 0x714, 0x712, 
    0x3, 0x2, 0x2, 0x2, 0x714, 0x713, 0x3, 0x2, 0x2, 0x2, 0x715, 0xef, 0x3, 
    0x2, 0x2, 0x2, 0x716, 0x719, 0x7, 0x67, 0x2, 0x2, 0x717, 0x71a, 0x7, 
    0xf, 0x2, 0x2, 0x718, 0x71a, 0x7, 0x9, 0x2, 0x2, 0x719, 0x717, 0x3, 
    0x2, 0x2, 0x2, 0x719, 0x718, 0x3, 0x2, 0x2, 0x2, 0x719, 0x71a, 0x3, 
    0x2, 0x2, 0x2, 0x71a, 0x71b, 0x3, 0x2, 0x2, 0x2, 0x71b, 0x733, 0x5, 
    0xec, 0x77, 0x2, 0x71c, 0x721, 0x7, 0xc9, 0x2, 0x2, 0x71d, 0x71e, 0x7, 
    0xb, 0x2, 0x2, 0x71e, 0x71f, 0x5, 0xce, 0x68, 0x2, 0x71f, 0x720, 0x7, 
    0xc, 0x2, 0x2, 0x720, 0x722, 0x3, 0x2, 0x2, 0x2, 0x721, 0x71d, 0x3, 
    0x2, 0x2, 0x2, 0x721, 0x722, 0x3, 0x2, 0x2, 0x2, 0x722, 0x734, 0x3, 
    0x2, 0x2, 0x2, 0x723, 0x728, 0x7, 0xc9, 0x2, 0x2, 0x724, 0x725, 0x7, 
    0xb, 0x2, 0x2, 0x725, 0x726, 0x5, 0xce, 0x68, 0x2, 0x726, 0x727, 0x7, 
    0xc, 0x2, 0x2, 0x727, 0x729, 0x3, 0x2, 0x2, 0x2, 0x728, 0x724, 0x3, 
    0x2, 0x2, 0x2, 0x728, 0x729, 0x3, 0x2, 0x2, 0x2, 0x729, 0x72a, 0x3, 
    0x2, 0x2, 0x2, 0x72a, 0x72b, 0x7, 0xaf, 0x2, 0x2, 0x72b, 0x734, 0x7, 
    0x7b, 0x2, 0x2, 0x72c, 0x731, 0x7, 0x7b, 0x2, 0x2, 0x72d, 0x72e, 0x7, 
    0xb, 0x2, 0x2, 0x72e, 0x72f, 0x5, 0xce, 0x68, 0x2, 0x72f, 0x730, 0x7, 
    0xc, 0x2, 0x2, 0x730, 0x732, 0x3, 0x2, 0x2, 0x2, 0x731, 0x72d, 0x3, 
    0x2, 0x2, 0x2, 0x731, 0x732, 0x3, 0x2, 0x2, 0x2, 0x732, 0x734, 0x3, 
    0x2, 0x2, 0x2, 0x733, 0x71c, 0x3, 0x2, 0x2, 0x2, 0x733, 0x723, 0x3, 
    0x2, 0x2, 0x2, 0x733, 0x72c, 0x3, 0x2, 0x2, 0x2, 0x734, 0xf1, 0x3, 0x2, 
    0x2, 0x2, 0x735, 0x738, 0x7, 0x67, 0x2, 0x2, 0x736, 0x739, 0x7, 0xf, 
    0x2, 0x2, 0x737, 0x739, 0x7, 0x9, 0x2, 0x2, 0x738, 0x736, 0x3, 0x2, 
    0x2, 0x2, 0x738, 0x737, 0x3, 0x2, 0x2, 0x2, 0x738, 0x739, 0x3, 0x2, 
    0x2, 0x2, 0x739, 0x73a, 0x3, 0x2, 0x2, 0x2, 0x73a, 0x78f, 0x5, 0xec, 
    0x77, 0x2, 0x73b, 0x740, 0x7, 0x3e, 0x2, 0x2, 0x73c, 0x73d, 0x7, 0xb, 
    0x2, 0x2, 0x73d, 0x73e, 0x5, 0xce, 0x68, 0x2, 0x73e, 0x73f, 0x7, 0xc, 
    0x2, 0x2, 0x73f, 0x741, 0x3, 0x2, 0x2, 0x2, 0x740, 0x73c, 0x3, 0x2, 
    0x2, 0x2, 0x740, 0x741, 0x3, 0x2, 0x2, 0x2, 0x741, 0x790, 0x3, 0x2, 
    0x2, 0x2, 0x742, 0x747, 0x7, 0x3e, 0x2, 0x2, 0x743, 0x744, 0x7, 0xb, 
    0x2, 0x2, 0x744, 0x745, 0x5, 0xce, 0x68, 0x2, 0x745, 0x746, 0x7, 0xc, 
    0x2, 0x2, 0x746, 0x748, 0x3, 0x2, 0x2, 0x2, 0x747, 0x743, 0x3, 0x2, 
    0x2, 0x2, 0x747, 0x748, 0x3, 0x2, 0x2, 0x2, 0x748, 0x749, 0x3, 0x2, 
    0x2, 0x2, 0x749, 0x753, 0x7, 0xaf, 0x2, 0x2, 0x74a, 0x754, 0x7, 0x5d, 
    0x2, 0x2, 0x74b, 0x754, 0x7, 0x78, 0x2, 0x2, 0x74c, 0x751, 0x7, 0x99, 
    0x2, 0x2, 0x74d, 0x74e, 0x7, 0xb, 0x2, 0x2, 0x74e, 0x74f, 0x5, 0xd0, 
    0x69, 0x2, 0x74f, 0x750, 0x7, 0xc, 0x2, 0x2, 0x750, 0x752, 0x3, 0x2, 
    0x2, 0x2, 0x751, 0x74d, 0x3, 0x2, 0x2, 0x2, 0x751, 0x752, 0x3, 0x2, 
    0x2, 0x2, 0x752, 0x754, 0x3, 0x2, 0x2, 0x2, 0x753, 0x74a, 0x3, 0x2, 
    0x2, 0x2, 0x753, 0x74b, 0x3, 0x2, 0x2, 0x2, 0x753, 0x74c, 0x3, 0x2, 
    0x2, 0x2, 0x754, 0x790, 0x3, 0x2, 0x2, 0x2, 0x755, 0x75a, 0x7, 0x5d, 
    0x2, 0x2, 0x756, 0x757, 0x7, 0xb, 0x2, 0x2, 0x757, 0x758, 0x5, 0xce, 
    0x68, 0x2, 0x758, 0x759, 0x7, 0xc, 0x2, 0x2, 0x759, 0x75b, 0x3, 0x2, 
    0x2, 0x2, 0x75a, 0x756, 0x3, 0x2, 0x2, 0x2, 0x75a, 0x75b, 0x3, 0x2, 
    0x2, 0x2, 0x75b, 0x75c, 0x3, 0x2, 0x2, 0x2, 0x75c, 0x765, 0x7, 0xaf, 
    0x2, 0x2, 0x75d, 0x766, 0x7, 0x78, 0x2, 0x2, 0x75e, 0x763, 0x7, 0x99, 
    0x2, 0x2, 0x75f, 0x760, 0x7, 0xb, 0x2, 0x2, 0x760, 0x761, 0x5, 0xd0, 
    0x69, 0x2, 0x761, 0x762, 0x7, 0xc, 0x2, 0x2, 0x762, 0x764, 0x3, 0x2, 
    0x2, 0x2, 0x763, 0x75f, 0x3, 0x2, 0x2, 0x2, 0x763, 0x764, 0x3, 0x2, 
    0x2, 0x2, 0x764, 0x766, 0x3, 0x2, 0x2, 0x2, 0x765, 0x75d, 0x3, 0x2, 
    0x2, 0x2, 0x765, 0x75e, 0x3, 0x2, 0x2, 0x2, 0x766, 0x790, 0x3, 0x2, 
    0x2, 0x2, 0x767, 0x76c, 0x7, 0x78, 0x2, 0x2, 0x768, 0x769, 0x7, 0xb, 
    0x2, 0x2, 0x769, 0x76a, 0x5, 0xce, 0x68, 0x2, 0x76a, 0x76b, 0x7, 0xc, 
    0x2, 0x2, 0x76b, 0x76d, 0x3, 0x2, 0x2, 0x2, 0x76c, 0x768, 0x3, 0x2, 
    0x2, 0x2, 0x76c, 0x76d, 0x3, 0x2, 0x2, 0x2, 0x76d, 0x76e, 0x3, 0x2, 
    0x2, 0x2, 0x76e, 0x76f, 0x7, 0xaf, 0x2, 0x2, 0x76f, 0x774, 0x7, 0x99, 
    0x2, 0x2, 0x770, 0x771, 0x7, 0xb, 0x2, 0x2, 0x771, 0x772, 0x5, 0xd0, 
    0x69, 0x2, 0x772, 0x773, 0x7, 0xc, 0x2, 0x2, 0x773, 0x775, 0x3, 0x2, 
    0x2, 0x2, 0x774, 0x770, 0x3, 0x2, 0x2, 0x2, 0x774, 0x775, 0x3, 0x2, 
    0x2, 0x2, 0x775, 0x790, 0x3, 0x2, 0x2, 0x2, 0x776, 0x77b, 0x7, 0x5d, 
    0x2, 0x2, 0x777, 0x778, 0x7, 0xb, 0x2, 0x2, 0x778, 0x779, 0x5, 0xce, 
    0x68, 0x2, 0x779, 0x77a, 0x7, 0xc, 0x2, 0x2, 0x77a, 0x77c, 0x3, 0x2, 
    0x2, 0x2, 0x77b, 0x777, 0x3, 0x2, 0x2, 0x2, 0x77b, 0x77c, 0x3, 0x2, 
    0x2, 0x2, 0x77c, 0x790, 0x3, 0x2, 0x2, 0x2, 0x77d, 0x782, 0x7, 0x78, 
    0x2, 0x2, 0x77e, 0x77f, 0x7, 0xb, 0x2, 0x2, 0x77f, 0x780, 0x5, 0xce, 
    0x68, 0x2, 0x780, 0x781, 0x7, 0xc, 0x2, 0x2, 0x781, 0x783, 0x3, 0x2, 
    0x2, 0x2, 0x782, 0x77e, 0x3, 0x2, 0x2, 0x2, 0x782, 0x783, 0x3, 0x2, 
    0x2, 0x2, 0x783, 0x790, 0x3, 0x2, 0x2, 0x2, 0x784, 0x78d, 0x7, 0x99, 
    0x2, 0x2, 0x785, 0x786, 0x7, 0xb, 0x2, 0x2, 0x786, 0x789, 0x5, 0xce, 
    0x68, 0x2, 0x787, 0x788, 0x7, 0x8, 0x2, 0x2, 0x788, 0x78a, 0x5, 0xd0, 
    0x69, 0x2, 0x789, 0x787, 0x3, 0x2, 0x2, 0x2, 0x789, 0x78a, 0x3, 0x2, 
    0x2, 0x2, 0x78a, 0x78b, 0x3, 0x2, 0x2, 0x2, 0x78b, 0x78c, 0x7, 0xc, 
    0x2, 0x2, 0x78c, 0x78e, 0x3, 0x2, 0x2, 0x2, 0x78d, 0x785, 0x3, 0x2, 
    0x2, 0x2, 0x78d, 0x78e, 0x3, 0x2, 0x2, 0x2, 0x78e, 0x790, 0x3, 0x2, 
    0x2, 0x2, 0x78f, 0x73b, 0x3, 0x2, 0x2, 0x2, 0x78f, 0x742, 0x3, 0x2, 
    0x2, 0x2, 0x78f, 0x755, 0x3, 0x2, 0x2, 0x2, 0x78f, 0x767, 0x3, 0x2, 
    0x2, 0x2, 0x78f, 0x776, 0x3, 0x2, 0x2, 0x2, 0x78f, 0x77d, 0x3, 0x2, 
    0x2, 0x2, 0x78f, 0x784, 0x3, 0x2, 0x2, 0x2, 0x790, 0xf3, 0x3, 0x2, 0x2, 
    0x2, 0x791, 0x792, 0x7, 0x72, 0x2, 0x2, 0x792, 0x793, 0x5, 0xb4, 0x5b, 
    0x2, 0x793, 0xf5, 0x3, 0x2, 0x2, 0x2, 0x794, 0x795, 0x7, 0x73, 0x2, 
    0x2, 0x795, 0x796, 0x5, 0xb4, 0x5b, 0x2, 0x796, 0xf7, 0x3, 0x2, 0x2, 
    0x2, 0x797, 0x798, 0x7, 0x74, 0x2, 0x2, 0x798, 0x799, 0x5, 0xb4, 0x5b, 
    0x2, 0x799, 0x79a, 0x5, 0xbe, 0x60, 0x2, 0x79a, 0xf9, 0x3, 0x2, 0x2, 
    0x2, 0x79b, 0x79c, 0x7, 0x75, 0x2, 0x2, 0x79c, 0xfb, 0x3, 0x2, 0x2, 
    0x2, 0x79d, 0x79e, 0x9, 0x20, 0x2, 0x2, 0x79e, 0xfd, 0x3, 0x2, 0x2, 
    0x2, 0xf3, 0x101, 0x116, 0x11a, 0x11c, 0x125, 0x12c, 0x131, 0x138, 0x141, 
    0x149, 0x14f, 0x154, 0x15c, 0x167, 0x16c, 0x170, 0x176, 0x182, 0x18a, 
    0x195, 0x19d, 0x1a2, 0x1aa, 0x1ad, 0x1b3, 0x1b9, 0x1bb, 0x1c0, 0x1c5, 
    0x1c9, 0x1ce, 0x1d2, 0x1d8, 0x1e0, 0x1e3, 0x1e9, 0x1ef, 0x1f1, 0x1f9, 
    0x1ff, 0x202, 0x207, 0x210, 0x213, 0x217, 0x221, 0x224, 0x22a, 0x22e, 
    0x230, 0x236, 0x23a, 0x23c, 0x23e, 0x244, 0x24a, 0x251, 0x256, 0x25e, 
    0x262, 0x26d, 0x271, 0x27a, 0x27c, 0x282, 0x287, 0x28e, 0x290, 0x295, 
    0x2a2, 0x2ab, 0x2ae, 0x2b2, 0x2bb, 0x2c0, 0x2c5, 0x2cd, 0x2d0, 0x2d4, 
    0x2db, 0x2e3, 0x2e6, 0x2f5, 0x2fc, 0x300, 0x309, 0x315, 0x31c, 0x32b, 
    0x32f, 0x338, 0x33d, 0x340, 0x347, 0x35f, 0x364, 0x368, 0x372, 0x374, 
    0x37f, 0x38a, 0x393, 0x399, 0x39d, 0x3a0, 0x3a9, 0x3ae, 0x3b2, 0x3b6, 
    0x3c1, 0x3c5, 0x3cb, 0x3d0, 0x3d2, 0x3d6, 0x3dc, 0x3e0, 0x3ec, 0x3f4, 
    0x3f8, 0x3fb, 0x400, 0x407, 0x40e, 0x417, 0x419, 0x420, 0x427, 0x431, 
    0x433, 0x43a, 0x443, 0x445, 0x44c, 0x454, 0x45b, 0x462, 0x469, 0x46d, 
    0x46f, 0x475, 0x47a, 0x47e, 0x481, 0x486, 0x48a, 0x48d, 0x491, 0x499, 
    0x4a2, 0x4a8, 0x4aa, 0x4b1, 0x4b6, 0x4b9, 0x4bc, 0x4c3, 0x4cd, 0x4d2, 
    0x4d9, 0x4e3, 0x4f1, 0x4fb, 0x501, 0x508, 0x531, 0x53a, 0x53c, 0x540, 
    0x562, 0x565, 0x56f, 0x572, 0x57c, 0x57f, 0x58b, 0x59d, 0x5a4, 0x5a6, 
    0x5af, 0x5b2, 0x5b5, 0x5b9, 0x5c7, 0x5cf, 0x5d8, 0x5e1, 0x5ea, 0x5f3, 
    0x5ff, 0x602, 0x60f, 0x618, 0x625, 0x62b, 0x645, 0x648, 0x64f, 0x654, 
    0x65d, 0x666, 0x669, 0x66e, 0x675, 0x677, 0x67f, 0x684, 0x689, 0x690, 
    0x695, 0x69a, 0x6a3, 0x6c4, 0x6c9, 0x6cf, 0x6e3, 0x6eb, 0x6f3, 0x6fb, 
    0x703, 0x714, 0x719, 0x721, 0x728, 0x731, 0x733, 0x738, 0x740, 0x747, 
    0x751, 0x753, 0x75a, 0x763, 0x765, 0x76c, 0x774, 0x77b, 0x782, 0x789, 
    0x78d, 0x78f, 
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
