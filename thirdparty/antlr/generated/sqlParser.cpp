
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


//----------------- StatementContext ------------------------------------------------------------------

sqlParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

sqlParser::StatementChoiceContext* sqlParser::StatementContext::statementChoice() {
  return getRuleContext<sqlParser::StatementChoiceContext>(0);
}

tree::TerminalNode* sqlParser::StatementContext::ANTLR() {
  return getToken(sqlParser::ANTLR, 0);
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
  enterRule(_localctx, 0, sqlParser::RuleStatement);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(206);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ANTLR) {
      setState(204);
      match(sqlParser::ANTLR);
      setState(205);
      match(sqlParser::T__0);
    }
    setState(208);
    statementChoice();
    setState(209);
    match(sqlParser::T__1);
   
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

sqlParser::StatisticsStatementContext* sqlParser::StatementChoiceContext::statisticsStatement() {
  return getRuleContext<sqlParser::StatisticsStatementContext>(0);
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
  enterRule(_localctx, 2, sqlParser::RuleStatementChoice);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(214);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(211);
      queryStatement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(212);
      explainStatement();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(213);
      statisticsStatement();
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
  enterRule(_localctx, 4, sqlParser::RuleQueryStatement);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(216);
    query();
   
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
  enterRule(_localctx, 6, sqlParser::RuleExplainStatement);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(221);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(218);
      queryHint();
      setState(223);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(224);
    match(sqlParser::EXPLAIN);
    setState(228);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::FORMAT || ((((_la - 100) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 100)) & ((1ULL << (sqlParser::STAGE - 100))
      | (1ULL << (sqlParser::WITH_NAMES - 100))
      | (1ULL << (sqlParser::WITH_TYPES - 100)))) != 0)) {
      setState(225);
      explainOptions();
      setState(230);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(231);
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
  enterRule(_localctx, 8, sqlParser::RuleExplainOptions);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(237);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::STAGE: {
        enterOuterAlt(_localctx, 1);
        setState(233);
        explainStageList();
        break;
      }

      case sqlParser::FORMAT: {
        enterOuterAlt(_localctx, 2);
        setState(234);
        explainFormat();
        break;
      }

      case sqlParser::WITH_NAMES: {
        enterOuterAlt(_localctx, 3);
        setState(235);
        match(sqlParser::WITH_NAMES);
        break;
      }

      case sqlParser::WITH_TYPES: {
        enterOuterAlt(_localctx, 4);
        setState(236);
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
  enterRule(_localctx, 10, sqlParser::RuleExplainStageList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(239);
    match(sqlParser::STAGE);
    setState(240);
    explainRange();
    setState(245);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(241);
      match(sqlParser::T__2);
      setState(242);
      explainRange();
      setState(247);
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
  enterRule(_localctx, 12, sqlParser::RuleExplainRange);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(248);
    dynamic_cast<ExplainRangeContext *>(_localctx)->begin = explainRangeChoice();
    setState(251);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__3 || _la == sqlParser::TO) {
      setState(249);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__3 || _la == sqlParser::TO)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(250);
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
  enterRule(_localctx, 14, sqlParser::RuleExplainRangeChoice);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(256);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__4:
      case sqlParser::ALL: {
        enterOuterAlt(_localctx, 1);
        setState(253);
        dynamic_cast<ExplainRangeChoiceContext *>(_localctx)->all = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__4

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
        setState(254);
        dynamic_cast<ExplainRangeChoiceContext *>(_localctx)->num = match(sqlParser::INTEGER_LITERAL);
        break;
      }

      case sqlParser::ANTLR:
      case sqlParser::ASC:
      case sqlParser::BOTH:
      case sqlParser::BY:
      case sqlParser::CATALOG:
      case sqlParser::CORRESPONDING:
      case sqlParser::CREATE:
      case sqlParser::CURRENT_CATALOG:
      case sqlParser::CURRENT_SCHEMA:
      case sqlParser::CURRENT_TIMESTAMP:
      case sqlParser::CURRENT_USER:
      case sqlParser::DATA:
      case sqlParser::DATE:
      case sqlParser::DELETE:
      case sqlParser::DESC:
      case sqlParser::DETAIL:
      case sqlParser::DROP:
      case sqlParser::ESCAPE:
      case sqlParser::EXCEPT:
      case sqlParser::EXPLAIN:
      case sqlParser::FEDERATION:
      case sqlParser::FORMAT:
      case sqlParser::INDEX:
      case sqlParser::INSERT:
      case sqlParser::INTO:
      case sqlParser::IS:
      case sqlParser::JOIN:
      case sqlParser::KEY:
      case sqlParser::LEFT:
      case sqlParser::MONOTONIC:
      case sqlParser::NO:
      case sqlParser::NODE:
      case sqlParser::OFFSET:
      case sqlParser::OUTER:
      case sqlParser::OVERLAY:
      case sqlParser::PARTITION:
      case sqlParser::PRETTY:
      case sqlParser::REFERENCE:
      case sqlParser::RIGHT:
      case sqlParser::SCHEMA:
      case sqlParser::SET:
      case sqlParser::STAGE:
      case sqlParser::STATS:
      case sqlParser::STATISTICS:
      case sqlParser::TABLE:
      case sqlParser::TYPE:
      case sqlParser::UNION:
      case sqlParser::UNIQUE:
      case sqlParser::UPDATE:
      case sqlParser::USING:
      case sqlParser::VALUES:
      case sqlParser::WITH_NAMES:
      case sqlParser::BOOLEAN:
      case sqlParser::INTEGER:
      case sqlParser::DECIMAL:
      case sqlParser::FLOAT:
      case sqlParser::TIME:
      case sqlParser::TIMESTAMP:
      case sqlParser::CHAR:
      case sqlParser::CHARACTER:
      case sqlParser::BINARY:
      case sqlParser::VARBINARY:
      case sqlParser::VARYING:
      case sqlParser::BIT:
      case sqlParser::BITS:
      case sqlParser::BYTE:
      case sqlParser::BYTES:
      case sqlParser::CHARS:
      case sqlParser::DIGITS:
      case sqlParser::COALESCE:
      case sqlParser::CONCAT:
      case sqlParser::COUNT:
      case sqlParser::DECODE:
      case sqlParser::EXTRACT:
      case sqlParser::IFNULL:
      case sqlParser::NULLIF:
      case sqlParser::POSITION:
      case sqlParser::SUBSTR:
      case sqlParser::SUBSTRING:
      case sqlParser::DAY:
      case sqlParser::DAYS:
      case sqlParser::HOUR:
      case sqlParser::HOURS:
      case sqlParser::MINUTE:
      case sqlParser::MINUTES:
      case sqlParser::MONTH:
      case sqlParser::MONTHS:
      case sqlParser::QUARTER:
      case sqlParser::QUARTERS:
      case sqlParser::SECOND:
      case sqlParser::SECONDS:
      case sqlParser::WEEK:
      case sqlParser::WEEKS:
      case sqlParser::YEAR:
      case sqlParser::YEARS:
      case sqlParser::BLOCK:
      case sqlParser::BLOOM:
      case sqlParser::BTREE:
      case sqlParser::HASH:
      case sqlParser::LOOP:
      case sqlParser::SKIPLIST:
      case sqlParser::TREE:
      case sqlParser::VALUE:
      case sqlParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 3);
        setState(255);
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
  enterRule(_localctx, 16, sqlParser::RuleExplainFormat);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(258);
    match(sqlParser::FORMAT);
    setState(259);
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
  enterRule(_localctx, 18, sqlParser::RuleStatisticsStatement);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(264);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(261);
      queryHint();
      setState(266);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(267);
    _la = _input->LA(1);
    if (!(_la == sqlParser::STATS

    || _la == sqlParser::STATISTICS)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(268);
    statementChoice();
   
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

sqlParser::InsertQueryContext* sqlParser::QueryContext::insertQuery() {
  return getRuleContext<sqlParser::InsertQueryContext>(0);
}

sqlParser::DeleteQueryContext* sqlParser::QueryContext::deleteQuery() {
  return getRuleContext<sqlParser::DeleteQueryContext>(0);
}

sqlParser::UpdateQueryContext* sqlParser::QueryContext::updateQuery() {
  return getRuleContext<sqlParser::UpdateQueryContext>(0);
}

sqlParser::CreateQueryContext* sqlParser::QueryContext::createQuery() {
  return getRuleContext<sqlParser::CreateQueryContext>(0);
}

sqlParser::DropQueryContext* sqlParser::QueryContext::dropQuery() {
  return getRuleContext<sqlParser::DropQueryContext>(0);
}

sqlParser::TruncateQueryContext* sqlParser::QueryContext::truncateQuery() {
  return getRuleContext<sqlParser::TruncateQueryContext>(0);
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
  enterRule(_localctx, 20, sqlParser::RuleQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(273);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(270);
      queryHint();
      setState(275);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(284);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx)) {
    case 1: {
      setState(276);
      selectQuery();
      break;
    }

    case 2: {
      setState(277);
      setQuery();
      break;
    }

    case 3: {
      setState(278);
      insertQuery();
      break;
    }

    case 4: {
      setState(279);
      deleteQuery();
      break;
    }

    case 5: {
      setState(280);
      updateQuery();
      break;
    }

    case 6: {
      setState(281);
      createQuery();
      break;
    }

    case 7: {
      setState(282);
      dropQuery();
      break;
    }

    case 8: {
      setState(283);
      truncateQuery();
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
  enterRule(_localctx, 22, sqlParser::RuleQueryHint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(286);
    match(sqlParser::HINT_START);
    setState(288);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::ANTLR)
      | (1ULL << sqlParser::ASC)
      | (1ULL << sqlParser::BOTH)
      | (1ULL << sqlParser::BY)
      | (1ULL << sqlParser::CATALOG)
      | (1ULL << sqlParser::CORRESPONDING)
      | (1ULL << sqlParser::CREATE)
      | (1ULL << sqlParser::CURRENT_CATALOG)
      | (1ULL << sqlParser::CURRENT_SCHEMA)
      | (1ULL << sqlParser::CURRENT_TIMESTAMP)
      | (1ULL << sqlParser::CURRENT_USER)
      | (1ULL << sqlParser::DATA)
      | (1ULL << sqlParser::DATE)
      | (1ULL << sqlParser::DELETE)
      | (1ULL << sqlParser::DESC)
      | (1ULL << sqlParser::DETAIL)
      | (1ULL << sqlParser::DROP)
      | (1ULL << sqlParser::ESCAPE)
      | (1ULL << sqlParser::EXCEPT)
      | (1ULL << sqlParser::EXPLAIN)
      | (1ULL << sqlParser::FEDERATION)
      | (1ULL << sqlParser::FORMAT)
      | (1ULL << sqlParser::INDEX)
      | (1ULL << sqlParser::INSERT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::INTO - 64))
      | (1ULL << (sqlParser::IS - 64))
      | (1ULL << (sqlParser::JOIN - 64))
      | (1ULL << (sqlParser::KEY - 64))
      | (1ULL << (sqlParser::LEFT - 64))
      | (1ULL << (sqlParser::MONOTONIC - 64))
      | (1ULL << (sqlParser::NO - 64))
      | (1ULL << (sqlParser::NODE - 64))
      | (1ULL << (sqlParser::OFFSET - 64))
      | (1ULL << (sqlParser::OUTER - 64))
      | (1ULL << (sqlParser::OVERLAY - 64))
      | (1ULL << (sqlParser::PARTITION - 64))
      | (1ULL << (sqlParser::PRETTY - 64))
      | (1ULL << (sqlParser::REFERENCE - 64))
      | (1ULL << (sqlParser::RIGHT - 64))
      | (1ULL << (sqlParser::SCHEMA - 64))
      | (1ULL << (sqlParser::SET - 64))
      | (1ULL << (sqlParser::STAGE - 64))
      | (1ULL << (sqlParser::STATS - 64))
      | (1ULL << (sqlParser::STATISTICS - 64))
      | (1ULL << (sqlParser::TABLE - 64))
      | (1ULL << (sqlParser::TYPE - 64))
      | (1ULL << (sqlParser::UNION - 64))
      | (1ULL << (sqlParser::UNIQUE - 64))
      | (1ULL << (sqlParser::UPDATE - 64))
      | (1ULL << (sqlParser::USING - 64))
      | (1ULL << (sqlParser::VALUES - 64))
      | (1ULL << (sqlParser::WITH_NAMES - 64))
      | (1ULL << (sqlParser::BOOLEAN - 64))
      | (1ULL << (sqlParser::INTEGER - 64))
      | (1ULL << (sqlParser::DECIMAL - 64))
      | (1ULL << (sqlParser::FLOAT - 64))
      | (1ULL << (sqlParser::TIME - 64))
      | (1ULL << (sqlParser::TIMESTAMP - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::CHAR - 128))
      | (1ULL << (sqlParser::CHARACTER - 128))
      | (1ULL << (sqlParser::BINARY - 128))
      | (1ULL << (sqlParser::VARBINARY - 128))
      | (1ULL << (sqlParser::VARYING - 128))
      | (1ULL << (sqlParser::BIT - 128))
      | (1ULL << (sqlParser::BITS - 128))
      | (1ULL << (sqlParser::BYTE - 128))
      | (1ULL << (sqlParser::BYTES - 128))
      | (1ULL << (sqlParser::CHARS - 128))
      | (1ULL << (sqlParser::DIGITS - 128))
      | (1ULL << (sqlParser::COALESCE - 128))
      | (1ULL << (sqlParser::CONCAT - 128))
      | (1ULL << (sqlParser::COUNT - 128))
      | (1ULL << (sqlParser::DECODE - 128))
      | (1ULL << (sqlParser::EXTRACT - 128))
      | (1ULL << (sqlParser::IFNULL - 128))
      | (1ULL << (sqlParser::NULLIF - 128))
      | (1ULL << (sqlParser::POSITION - 128))
      | (1ULL << (sqlParser::SUBSTR - 128))
      | (1ULL << (sqlParser::SUBSTRING - 128))
      | (1ULL << (sqlParser::DAY - 128))
      | (1ULL << (sqlParser::DAYS - 128))
      | (1ULL << (sqlParser::HOUR - 128))
      | (1ULL << (sqlParser::HOURS - 128))
      | (1ULL << (sqlParser::MINUTE - 128))
      | (1ULL << (sqlParser::MINUTES - 128))
      | (1ULL << (sqlParser::MONTH - 128))
      | (1ULL << (sqlParser::MONTHS - 128))
      | (1ULL << (sqlParser::QUARTER - 128))
      | (1ULL << (sqlParser::QUARTERS - 128))
      | (1ULL << (sqlParser::SECOND - 128))
      | (1ULL << (sqlParser::SECONDS - 128))
      | (1ULL << (sqlParser::WEEK - 128))
      | (1ULL << (sqlParser::WEEKS - 128))
      | (1ULL << (sqlParser::YEAR - 128))
      | (1ULL << (sqlParser::YEARS - 128))
      | (1ULL << (sqlParser::BLOCK - 128))
      | (1ULL << (sqlParser::BLOOM - 128))
      | (1ULL << (sqlParser::BTREE - 128))
      | (1ULL << (sqlParser::HASH - 128))
      | (1ULL << (sqlParser::LOOP - 128))
      | (1ULL << (sqlParser::SKIPLIST - 128))
      | (1ULL << (sqlParser::TREE - 128))
      | (1ULL << (sqlParser::VALUE - 128))
      | (1ULL << (sqlParser::IDENTIFIER - 128)))) != 0)) {
      setState(287);
      hint();
    }
    setState(294);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(290);
      match(sqlParser::T__2);
      setState(291);
      hint();
      setState(296);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(297);
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
  enterRule(_localctx, 24, sqlParser::RuleHint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(319);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(299);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(300);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(301);
      match(sqlParser::T__5);
      setState(302);
      hintValue();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(304);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(306);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__5) {
        setState(305);
        match(sqlParser::T__5);
      }
      setState(308);
      match(sqlParser::T__6);
      setState(309);
      hintValue();
      setState(314);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(310);
        match(sqlParser::T__2);
        setState(311);
        hintValue();
        setState(316);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(317);
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
  enterRule(_localctx, 26, sqlParser::RuleSelectQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(330);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(321);
      match(sqlParser::WITH);
      setState(322);
      withQuery();
      setState(327);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(323);
        match(sqlParser::T__2);
        setState(324);
        withQuery();
        setState(329);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(332);
    selectExpr();
    setState(336);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(333);
      match(sqlParser::ORDER);
      setState(334);
      match(sqlParser::BY);
      setState(335);
      dynamic_cast<SelectQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(344);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(338);
      match(sqlParser::LIMIT);
      setState(339);
      dynamic_cast<SelectQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(342);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(340);
        match(sqlParser::OFFSET);
        setState(341);
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
  enterRule(_localctx, 28, sqlParser::RuleSelectExpr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(373);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__8:
      case sqlParser::SELECT: {
        enterOuterAlt(_localctx, 1);
        setState(346);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__8 || _la == sqlParser::SELECT)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(349);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::ALL: {
            setState(347);
            match(sqlParser::ALL);
            break;
          }

          case sqlParser::DISTINCT: {
            setState(348);
            dynamic_cast<SelectExprContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
            break;
          }

          case sqlParser::T__3:
          case sqlParser::T__4:
          case sqlParser::T__6:
          case sqlParser::T__10:
          case sqlParser::ANTLR:
          case sqlParser::ASC:
          case sqlParser::AVG:
          case sqlParser::BOTH:
          case sqlParser::BY:
          case sqlParser::CATALOG:
          case sqlParser::CORRESPONDING:
          case sqlParser::CREATE:
          case sqlParser::CURRENT_CATALOG:
          case sqlParser::CURRENT_SCHEMA:
          case sqlParser::CURRENT_TIMESTAMP:
          case sqlParser::CURRENT_USER:
          case sqlParser::DATA:
          case sqlParser::DATE:
          case sqlParser::DELETE:
          case sqlParser::DESC:
          case sqlParser::DETAIL:
          case sqlParser::DROP:
          case sqlParser::ESCAPE:
          case sqlParser::EXCEPT:
          case sqlParser::EXPLAIN:
          case sqlParser::SQL_FALSE:
          case sqlParser::FEDERATION:
          case sqlParser::FORMAT:
          case sqlParser::INDEX:
          case sqlParser::INSERT:
          case sqlParser::INTO:
          case sqlParser::IS:
          case sqlParser::JOIN:
          case sqlParser::KEY:
          case sqlParser::LEFT:
          case sqlParser::MAX:
          case sqlParser::MIN:
          case sqlParser::MONOTONIC:
          case sqlParser::NO:
          case sqlParser::NODE:
          case sqlParser::NOT:
          case sqlParser::SQL_NULL:
          case sqlParser::OFFSET:
          case sqlParser::OUTER:
          case sqlParser::OVERLAY:
          case sqlParser::PARTITION:
          case sqlParser::PRETTY:
          case sqlParser::REFERENCE:
          case sqlParser::RIGHT:
          case sqlParser::SCHEMA:
          case sqlParser::SET:
          case sqlParser::STAGE:
          case sqlParser::STATS:
          case sqlParser::STATISTICS:
          case sqlParser::SUM:
          case sqlParser::TABLE:
          case sqlParser::SQL_TRUE:
          case sqlParser::TYPE:
          case sqlParser::UNION:
          case sqlParser::UNIQUE:
          case sqlParser::UPDATE:
          case sqlParser::USING:
          case sqlParser::VALUES:
          case sqlParser::WITH_NAMES:
          case sqlParser::BOOLEAN:
          case sqlParser::INTEGER:
          case sqlParser::DECIMAL:
          case sqlParser::FLOAT:
          case sqlParser::TIME:
          case sqlParser::TIMESTAMP:
          case sqlParser::INTERVAL:
          case sqlParser::CHAR:
          case sqlParser::CHARACTER:
          case sqlParser::BINARY:
          case sqlParser::VARBINARY:
          case sqlParser::VARYING:
          case sqlParser::BIT:
          case sqlParser::BITS:
          case sqlParser::BYTE:
          case sqlParser::BYTES:
          case sqlParser::CHARS:
          case sqlParser::DIGITS:
          case sqlParser::CASE:
          case sqlParser::CAST:
          case sqlParser::COALESCE:
          case sqlParser::CONCAT:
          case sqlParser::COUNT:
          case sqlParser::DECODE:
          case sqlParser::EXTRACT:
          case sqlParser::EXISTS:
          case sqlParser::IF:
          case sqlParser::IFNULL:
          case sqlParser::NULLIF:
          case sqlParser::POSITION:
          case sqlParser::SUBSTR:
          case sqlParser::SUBSTRING:
          case sqlParser::TRIM:
          case sqlParser::TYPEOF:
          case sqlParser::DAY:
          case sqlParser::DAYS:
          case sqlParser::HOUR:
          case sqlParser::HOURS:
          case sqlParser::MINUTE:
          case sqlParser::MINUTES:
          case sqlParser::MONTH:
          case sqlParser::MONTHS:
          case sqlParser::QUARTER:
          case sqlParser::QUARTERS:
          case sqlParser::SECOND:
          case sqlParser::SECONDS:
          case sqlParser::WEEK:
          case sqlParser::WEEKS:
          case sqlParser::YEAR:
          case sqlParser::YEARS:
          case sqlParser::BLOCK:
          case sqlParser::BLOOM:
          case sqlParser::BTREE:
          case sqlParser::HASH:
          case sqlParser::LOOP:
          case sqlParser::SKIPLIST:
          case sqlParser::TREE:
          case sqlParser::VALUE:
          case sqlParser::IDENTIFIER:
          case sqlParser::STRING_LITERAL:
          case sqlParser::FLOAT_LITERAL:
          case sqlParser::DECIMAL_LITERAL:
          case sqlParser::INTEGER_LITERAL: {
            break;
          }

        default:
          break;
        }
        setState(351);
        selectList();
        setState(354);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
        case 1: {
          setState(352);
          match(sqlParser::FROM);
          setState(353);
          tableExpr(0);
          break;
        }

        }
        setState(358);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
        case 1: {
          setState(356);
          match(sqlParser::WHERE);
          setState(357);
          dynamic_cast<SelectExprContext *>(_localctx)->where = predicate();
          break;
        }

        }
        setState(363);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
        case 1: {
          setState(360);
          match(sqlParser::GROUP);
          setState(361);
          match(sqlParser::BY);
          setState(362);
          dynamic_cast<SelectExprContext *>(_localctx)->groupBy = groupByList();
          break;
        }

        }
        setState(367);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
        case 1: {
          setState(365);
          match(sqlParser::HAVING);
          setState(366);
          dynamic_cast<SelectExprContext *>(_localctx)->having = predicate();
          break;
        }

        }
        break;
      }

      case sqlParser::T__6: {
        enterOuterAlt(_localctx, 2);
        setState(369);
        match(sqlParser::T__6);
        setState(370);
        selectExpr();
        setState(371);
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
  enterRule(_localctx, 30, sqlParser::RuleSetQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(384);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(375);
      match(sqlParser::WITH);
      setState(376);
      withQuery();
      setState(381);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(377);
        match(sqlParser::T__2);
        setState(378);
        withQuery();
        setState(383);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(386);
    setExpr(0);
    setState(390);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(387);
      match(sqlParser::ORDER);
      setState(388);
      match(sqlParser::BY);
      setState(389);
      dynamic_cast<SetQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(398);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(392);
      match(sqlParser::LIMIT);
      setState(393);
      dynamic_cast<SetQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(396);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(394);
        match(sqlParser::OFFSET);
        setState(395);
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
  size_t startState = 32;
  enterRecursionRule(_localctx, 32, sqlParser::RuleSetExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(406);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx)) {
    case 1: {
      setState(401);
      selectExpr();
      break;
    }

    case 2: {
      setState(402);
      match(sqlParser::T__6);
      setState(403);
      setExpr(0);
      setState(404);
      match(sqlParser::T__7);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(420);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<SetExprContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleSetExpr);
        setState(408);

        if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
        setState(409);
        dynamic_cast<SetExprContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(((((_la - 49) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 49)) & ((1ULL << (sqlParser::EXCEPT - 49))
          | (1ULL << (sqlParser::INTERSECT - 49))
          | (1ULL << (sqlParser::UNION - 49)))) != 0))) {
          dynamic_cast<SetExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(412);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::DISTINCT: {
            setState(410);
            match(sqlParser::DISTINCT);
            break;
          }

          case sqlParser::ALL: {
            setState(411);
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
        setState(415);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::CORRESPONDING) {
          setState(414);
          corrClause();
        }
        setState(417);
        setExpr(3); 
      }
      setState(422);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
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
  enterRule(_localctx, 34, sqlParser::RuleCorrClause);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(423);
    dynamic_cast<CorrClauseContext *>(_localctx)->corr = match(sqlParser::CORRESPONDING);
    setState(429);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::BY) {
      setState(424);
      match(sqlParser::BY);
      setState(425);
      match(sqlParser::T__6);
      setState(426);
      corrList();
      setState(427);
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

std::vector<sqlParser::ColumnAliasContext *> sqlParser::WithQueryContext::columnAlias() {
  return getRuleContexts<sqlParser::ColumnAliasContext>();
}

sqlParser::ColumnAliasContext* sqlParser::WithQueryContext::columnAlias(size_t i) {
  return getRuleContext<sqlParser::ColumnAliasContext>(i);
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
  enterRule(_localctx, 36, sqlParser::RuleWithQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(432);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::RECURSIVE) {
      setState(431);
      match(sqlParser::RECURSIVE);
    }
    setState(434);
    dynamic_cast<WithQueryContext *>(_localctx)->name = tableName();
    setState(446);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__6) {
      setState(435);
      match(sqlParser::T__6);
      setState(436);
      columnAlias();
      setState(441);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(437);
        match(sqlParser::T__2);
        setState(438);
        columnAlias();
        setState(443);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(444);
      match(sqlParser::T__7);
    }
    setState(448);
    match(sqlParser::AS);
    setState(449);
    match(sqlParser::T__6);
    setState(450);
    query();
    setState(451);
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

sqlParser::ExtendedJoinTypeContext* sqlParser::JoinUsingContext::extendedJoinType() {
  return getRuleContext<sqlParser::ExtendedJoinTypeContext>(0);
}

sqlParser::JoinHintContext* sqlParser::JoinUsingContext::joinHint() {
  return getRuleContext<sqlParser::JoinHintContext>(0);
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

sqlParser::ExtendedJoinTypeContext* sqlParser::JoinOnContext::extendedJoinType() {
  return getRuleContext<sqlParser::ExtendedJoinTypeContext>(0);
}

sqlParser::JoinHintContext* sqlParser::JoinOnContext::joinHint() {
  return getRuleContext<sqlParser::JoinHintContext>(0);
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
  size_t startState = 38;
  enterRecursionRule(_localctx, 38, sqlParser::RuleTableExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(479);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<FetchContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(454);
      dynamic_cast<FetchContext *>(_localctx)->ref = tableReference();
      setState(459);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx)) {
      case 1: {
        setState(456);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(455);
          match(sqlParser::AS);
        }
        setState(458);
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
      setState(461);
      match(sqlParser::T__6);
      setState(462);
      query();
      setState(463);
      match(sqlParser::T__7);
      setState(468);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 43, _ctx)) {
      case 1: {
        setState(465);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(464);
          match(sqlParser::AS);
        }
        setState(467);
        dynamic_cast<SubqueryContext *>(_localctx)->as = tableAlias();
        break;
      }

      }
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ValuesSubqueryContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(470);
      match(sqlParser::T__6);
      setState(471);
      valuesExpr();
      setState(472);
      match(sqlParser::T__7);
      setState(477);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 45, _ctx)) {
      case 1: {
        setState(474);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(473);
          match(sqlParser::AS);
        }
        setState(476);
        dynamic_cast<ValuesSubqueryContext *>(_localctx)->as = tableAlias();
        break;
      }

      }
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(523);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 54, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(521);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<CrossJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(481);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(485);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case sqlParser::T__2: {
              setState(482);
              match(sqlParser::T__2);
              break;
            }

            case sqlParser::CROSS: {
              setState(483);
              match(sqlParser::CROSS);
              setState(484);
              match(sqlParser::JOIN);
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(487);
          tableExpr(7);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<NaturalJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(488);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(489);
          match(sqlParser::NATURAL);

          setState(491);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 55) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 55)) & ((1ULL << (sqlParser::FULL - 55))
            | (1ULL << (sqlParser::INNER - 55))
            | (1ULL << (sqlParser::LEFT - 55))
            | (1ULL << (sqlParser::RIGHT - 55)))) != 0)) {
            setState(490);
            dynamic_cast<NaturalJoinContext *>(_localctx)->type = joinType();
          }
          setState(493);
          match(sqlParser::JOIN);
          setState(494);
          tableExpr(6);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<JoinOnContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(495);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(497);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::ANTI)
            | (1ULL << sqlParser::FULL)
            | (1ULL << sqlParser::INNER))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::LEFT - 69))
            | (1ULL << (sqlParser::RIGHT - 69))
            | (1ULL << (sqlParser::SEMI - 69)))) != 0)) {
            setState(496);
            dynamic_cast<JoinOnContext *>(_localctx)->type = extendedJoinType();
          }
          setState(500);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::HINT_START) {
            setState(499);
            dynamic_cast<JoinOnContext *>(_localctx)->joint = joinHint();
          }
          setState(502);
          match(sqlParser::JOIN);
          setState(503);
          tableExpr(0);
          setState(504);
          match(sqlParser::ON);
          setState(505);
          joinPredicate();
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<JoinUsingContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(507);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(509);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::ANTI)
            | (1ULL << sqlParser::FULL)
            | (1ULL << sqlParser::INNER))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::LEFT - 69))
            | (1ULL << (sqlParser::RIGHT - 69))
            | (1ULL << (sqlParser::SEMI - 69)))) != 0)) {
            setState(508);
            dynamic_cast<JoinUsingContext *>(_localctx)->type = extendedJoinType();
          }
          setState(512);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::HINT_START) {
            setState(511);
            dynamic_cast<JoinUsingContext *>(_localctx)->joint = joinHint();
          }
          setState(514);
          match(sqlParser::JOIN);
          setState(515);
          tableExpr(0);
          setState(516);
          match(sqlParser::USING);
          setState(517);
          match(sqlParser::T__6);
          setState(518);
          joinUsingList();
          setState(519);
          match(sqlParser::T__7);
          break;
        }

        } 
      }
      setState(525);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 54, _ctx);
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
  enterRule(_localctx, 40, sqlParser::RuleTableReference);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(534);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx)) {
    case 1: {
      setState(529);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 55, _ctx)) {
      case 1: {
        setState(526);
        dynamic_cast<TableReferenceContext *>(_localctx)->catalog = catalogName();
        setState(527);
        match(sqlParser::T__9);
        break;
      }

      }
      setState(531);
      dynamic_cast<TableReferenceContext *>(_localctx)->schema = schemaName();
      setState(532);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(536);
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
  enterRule(_localctx, 42, sqlParser::RuleJoinType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(543);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::INNER: {
        enterOuterAlt(_localctx, 1);
        setState(538);
        match(sqlParser::INNER);
        break;
      }

      case sqlParser::FULL:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 2);
        setState(539);
        dynamic_cast<JoinTypeContext *>(_localctx)->outer = _input->LT(1);
        _la = _input->LA(1);
        if (!(((((_la - 55) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 55)) & ((1ULL << (sqlParser::FULL - 55))
          | (1ULL << (sqlParser::LEFT - 55))
          | (1ULL << (sqlParser::RIGHT - 55)))) != 0))) {
          dynamic_cast<JoinTypeContext *>(_localctx)->outer = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(541);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::OUTER) {
          setState(540);
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
  enterRule(_localctx, 44, sqlParser::RuleExtendedJoinType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(548);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::FULL:
      case sqlParser::INNER:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 1);
        setState(545);
        joinType();
        break;
      }

      case sqlParser::SEMI: {
        enterOuterAlt(_localctx, 2);
        setState(546);
        match(sqlParser::SEMI);
        break;
      }

      case sqlParser::ANTI: {
        enterOuterAlt(_localctx, 3);
        setState(547);
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

//----------------- JoinHintContext ------------------------------------------------------------------

sqlParser::JoinHintContext::JoinHintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::JoinHintContext::HINT_START() {
  return getToken(sqlParser::HINT_START, 0);
}

tree::TerminalNode* sqlParser::JoinHintContext::HINT_END() {
  return getToken(sqlParser::HINT_END, 0);
}

tree::TerminalNode* sqlParser::JoinHintContext::LOOP() {
  return getToken(sqlParser::LOOP, 0);
}

tree::TerminalNode* sqlParser::JoinHintContext::HASH() {
  return getToken(sqlParser::HASH, 0);
}

tree::TerminalNode* sqlParser::JoinHintContext::BLOCK() {
  return getToken(sqlParser::BLOCK, 0);
}

tree::TerminalNode* sqlParser::JoinHintContext::BLOOM() {
  return getToken(sqlParser::BLOOM, 0);
}

std::vector<sqlParser::HintValueContext *> sqlParser::JoinHintContext::hintValue() {
  return getRuleContexts<sqlParser::HintValueContext>();
}

sqlParser::HintValueContext* sqlParser::JoinHintContext::hintValue(size_t i) {
  return getRuleContext<sqlParser::HintValueContext>(i);
}


size_t sqlParser::JoinHintContext::getRuleIndex() const {
  return sqlParser::RuleJoinHint;
}


antlrcpp::Any sqlParser::JoinHintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitJoinHint(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::JoinHintContext* sqlParser::joinHint() {
  JoinHintContext *_localctx = _tracker.createInstance<JoinHintContext>(_ctx, getState());
  enterRule(_localctx, 46, sqlParser::RuleJoinHint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(550);
    match(sqlParser::HINT_START);
    setState(551);
    dynamic_cast<JoinHintContext *>(_localctx)->method = _input->LT(1);
    _la = _input->LA(1);
    if (!(((((_la - 178) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 178)) & ((1ULL << (sqlParser::BLOCK - 178))
      | (1ULL << (sqlParser::BLOOM - 178))
      | (1ULL << (sqlParser::HASH - 178))
      | (1ULL << (sqlParser::LOOP - 178)))) != 0))) {
      dynamic_cast<JoinHintContext *>(_localctx)->method = _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(566);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__5

    || _la == sqlParser::T__6) {
      setState(553);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__5) {
        setState(552);
        match(sqlParser::T__5);
      }
      setState(555);
      match(sqlParser::T__6);
      setState(556);
      dynamic_cast<JoinHintContext *>(_localctx)->values = hintValue();
      setState(561);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(557);
        match(sqlParser::T__2);
        setState(558);
        hintValue();
        setState(563);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(564);
      match(sqlParser::T__7);
    }
    setState(568);
    match(sqlParser::HINT_END);
   
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
  enterRule(_localctx, 48, sqlParser::RuleValuesExpr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(570);
    match(sqlParser::VALUES);
    setState(571);
    match(sqlParser::T__6);
    setState(572);
    valueList();
    setState(573);
    match(sqlParser::T__7);
    setState(581);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(574);
      match(sqlParser::T__2);
      setState(575);
      match(sqlParser::T__6);
      setState(576);
      valueList();
      setState(577);
      match(sqlParser::T__7);
      setState(583);
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
  enterRule(_localctx, 50, sqlParser::RuleInsertQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(593);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(584);
      match(sqlParser::WITH);
      setState(585);
      withQuery();
      setState(590);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(586);
        match(sqlParser::T__2);
        setState(587);
        withQuery();
        setState(592);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(595);
    match(sqlParser::INSERT);
    setState(597);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 66, _ctx)) {
    case 1: {
      setState(596);
      match(sqlParser::INTO);
      break;
    }

    }
    setState(599);
    dynamic_cast<InsertQueryContext *>(_localctx)->ref = tableReference();
    setState(611);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 68, _ctx)) {
    case 1: {
      setState(600);
      match(sqlParser::T__6);
      setState(601);
      columnName();
      setState(606);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(602);
        match(sqlParser::T__2);
        setState(603);
        columnName();
        setState(608);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(609);
      match(sqlParser::T__7);
      break;
    }

    }
    setState(616);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 69, _ctx)) {
    case 1: {
      setState(613);
      valuesExpr();
      break;
    }

    case 2: {
      setState(614);
      selectQuery();
      break;
    }

    case 3: {
      setState(615);
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
  enterRule(_localctx, 52, sqlParser::RuleDeleteQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(627);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(618);
      match(sqlParser::WITH);
      setState(619);
      withQuery();
      setState(624);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(620);
        match(sqlParser::T__2);
        setState(621);
        withQuery();
        setState(626);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(629);
    match(sqlParser::DELETE);
    setState(631);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::FROM) {
      setState(630);
      match(sqlParser::FROM);
    }
    setState(633);
    dynamic_cast<DeleteQueryContext *>(_localctx)->ref = tableReference();
    setState(638);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(634);
        match(sqlParser::WHERE);
        setState(635);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(636);
        match(sqlParser::FROM);
        setState(637);
        tableExpr(0);
        break;
      }

      case sqlParser::T__1:
      case sqlParser::T__7: {
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
  enterRule(_localctx, 54, sqlParser::RuleUpdateQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(649);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(640);
      match(sqlParser::WITH);
      setState(641);
      withQuery();
      setState(646);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(642);
        match(sqlParser::T__2);
        setState(643);
        withQuery();
        setState(648);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(651);
    match(sqlParser::UPDATE);
    setState(652);
    dynamic_cast<UpdateQueryContext *>(_localctx)->ref = tableReference();
    setState(653);
    match(sqlParser::SET);
    setState(654);
    columnName();
    setState(655);
    match(sqlParser::T__5);
    setState(656);
    expr(0);
    setState(664);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(657);
      match(sqlParser::T__2);
      setState(658);
      columnName();
      setState(659);
      match(sqlParser::T__5);
      setState(660);
      expr(0);
      setState(666);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(671);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(667);
        match(sqlParser::WHERE);
        setState(668);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(669);
        match(sqlParser::FROM);
        setState(670);
        tableExpr(0);
        break;
      }

      case sqlParser::T__1:
      case sqlParser::T__7: {
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
  enterRule(_localctx, 56, sqlParser::RuleCreateQuery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(675);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 78, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(673);
      createTableQuery();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(674);
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

sqlParser::TableReferenceContext* sqlParser::CreateTableQueryContext::tableReference() {
  return getRuleContext<sqlParser::TableReferenceContext>(0);
}

sqlParser::TableDefinitionContext* sqlParser::CreateTableQueryContext::tableDefinition() {
  return getRuleContext<sqlParser::TableDefinitionContext>(0);
}

tree::TerminalNode* sqlParser::CreateTableQueryContext::IF() {
  return getToken(sqlParser::IF, 0);
}

tree::TerminalNode* sqlParser::CreateTableQueryContext::NOT() {
  return getToken(sqlParser::NOT, 0);
}

tree::TerminalNode* sqlParser::CreateTableQueryContext::EXISTS() {
  return getToken(sqlParser::EXISTS, 0);
}

tree::TerminalNode* sqlParser::CreateTableQueryContext::REFERENCE() {
  return getToken(sqlParser::REFERENCE, 0);
}

sqlParser::AsSubqueryContext* sqlParser::CreateTableQueryContext::asSubquery() {
  return getRuleContext<sqlParser::AsSubqueryContext>(0);
}

sqlParser::ColumnNameListContext* sqlParser::CreateTableQueryContext::columnNameList() {
  return getRuleContext<sqlParser::ColumnNameListContext>(0);
}

tree::TerminalNode* sqlParser::CreateTableQueryContext::AS() {
  return getToken(sqlParser::AS, 0);
}

tree::TerminalNode* sqlParser::CreateTableQueryContext::WITH() {
  return getToken(sqlParser::WITH, 0);
}

tree::TerminalNode* sqlParser::CreateTableQueryContext::DATA() {
  return getToken(sqlParser::DATA, 0);
}

tree::TerminalNode* sqlParser::CreateTableQueryContext::NO() {
  return getToken(sqlParser::NO, 0);
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
  enterRule(_localctx, 58, sqlParser::RuleCreateTableQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(677);
    match(sqlParser::CREATE);
    setState(679);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::REFERENCE) {
      setState(678);
      dynamic_cast<CreateTableQueryContext *>(_localctx)->reference = match(sqlParser::REFERENCE);
    }
    setState(681);
    match(sqlParser::TABLE);
    setState(685);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::IF) {
      setState(682);
      match(sqlParser::IF);
      setState(683);
      match(sqlParser::NOT);
      setState(684);
      match(sqlParser::EXISTS);
    }
    setState(687);
    dynamic_cast<CreateTableQueryContext *>(_localctx)->ref = tableReference();
    setState(715);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 88, _ctx)) {
    case 1: {
      setState(688);
      tableDefinition();
      break;
    }

    case 2: {
      setState(690);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 81, _ctx)) {
      case 1: {
        setState(689);
        tableDefinition();
        break;
      }

      }
      setState(693);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::AS) {
        setState(692);
        match(sqlParser::AS);
      }
      setState(695);
      asSubquery();
      setState(701);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::WITH) {
        setState(696);
        match(sqlParser::WITH);
        setState(698);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::NO) {
          setState(697);
          dynamic_cast<CreateTableQueryContext *>(_localctx)->noData = match(sqlParser::NO);
        }
        setState(700);
        match(sqlParser::DATA);
      }
      break;
    }

    case 3: {
      setState(703);
      columnNameList();
      setState(705);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::AS) {
        setState(704);
        match(sqlParser::AS);
      }
      setState(707);
      asSubquery();
      setState(713);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::WITH) {
        setState(708);
        match(sqlParser::WITH);
        setState(710);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::NO) {
          setState(709);
          dynamic_cast<CreateTableQueryContext *>(_localctx)->noData = match(sqlParser::NO);
        }
        setState(712);
        match(sqlParser::DATA);
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

//----------------- CreateIndexQueryContext ------------------------------------------------------------------

sqlParser::CreateIndexQueryContext::CreateIndexQueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::CREATE() {
  return getToken(sqlParser::CREATE, 0);
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::ON() {
  return getToken(sqlParser::ON, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::CreateIndexQueryContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::CreateIndexQueryContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::INDEX() {
  return getToken(sqlParser::INDEX, 0);
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::KEY() {
  return getToken(sqlParser::KEY, 0);
}

sqlParser::IndexNameContext* sqlParser::CreateIndexQueryContext::indexName() {
  return getRuleContext<sqlParser::IndexNameContext>(0);
}

sqlParser::TableReferenceContext* sqlParser::CreateIndexQueryContext::tableReference() {
  return getRuleContext<sqlParser::TableReferenceContext>(0);
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::UNIQUE() {
  return getToken(sqlParser::UNIQUE, 0);
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::IF() {
  return getToken(sqlParser::IF, 0);
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::NOT() {
  return getToken(sqlParser::NOT, 0);
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::EXISTS() {
  return getToken(sqlParser::EXISTS, 0);
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::USING() {
  return getToken(sqlParser::USING, 0);
}

tree::TerminalNode* sqlParser::CreateIndexQueryContext::WITH() {
  return getToken(sqlParser::WITH, 0);
}

sqlParser::IndexMethodContext* sqlParser::CreateIndexQueryContext::indexMethod() {
  return getRuleContext<sqlParser::IndexMethodContext>(0);
}

sqlParser::PropertyListContext* sqlParser::CreateIndexQueryContext::propertyList() {
  return getRuleContext<sqlParser::PropertyListContext>(0);
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
  enterRule(_localctx, 60, sqlParser::RuleCreateIndexQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(717);
    match(sqlParser::CREATE);
    setState(719);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(718);
      match(sqlParser::UNIQUE);
    }
    setState(721);
    _la = _input->LA(1);
    if (!(_la == sqlParser::INDEX

    || _la == sqlParser::KEY)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(725);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::IF) {
      setState(722);
      match(sqlParser::IF);
      setState(723);
      match(sqlParser::NOT);
      setState(724);
      match(sqlParser::EXISTS);
    }
    setState(727);
    dynamic_cast<CreateIndexQueryContext *>(_localctx)->idxName = indexName();
    setState(728);
    match(sqlParser::ON);
    setState(729);
    dynamic_cast<CreateIndexQueryContext *>(_localctx)->tblName = tableReference();
    setState(732);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::USING) {
      setState(730);
      match(sqlParser::USING);
      setState(731);
      dynamic_cast<CreateIndexQueryContext *>(_localctx)->method = indexMethod();
    }
    setState(734);
    match(sqlParser::T__6);
    setState(735);
    expr(0);
    setState(740);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(736);
      match(sqlParser::T__2);
      setState(737);
      expr(0);
      setState(742);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(743);
    match(sqlParser::T__7);
    setState(746);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(744);
      match(sqlParser::WITH);
      setState(745);
      dynamic_cast<CreateIndexQueryContext *>(_localctx)->props = propertyList();
    }
   
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


size_t sqlParser::DropQueryContext::getRuleIndex() const {
  return sqlParser::RuleDropQuery;
}

void sqlParser::DropQueryContext::copyFrom(DropQueryContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- DropTableContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::DropTableContext::DROP() {
  return getToken(sqlParser::DROP, 0);
}

tree::TerminalNode* sqlParser::DropTableContext::TABLE() {
  return getToken(sqlParser::TABLE, 0);
}

sqlParser::TableReferenceContext* sqlParser::DropTableContext::tableReference() {
  return getRuleContext<sqlParser::TableReferenceContext>(0);
}

tree::TerminalNode* sqlParser::DropTableContext::IF() {
  return getToken(sqlParser::IF, 0);
}

tree::TerminalNode* sqlParser::DropTableContext::EXISTS() {
  return getToken(sqlParser::EXISTS, 0);
}

sqlParser::DropTableContext::DropTableContext(DropQueryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::DropTableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDropTable(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DropIndexContext ------------------------------------------------------------------

tree::TerminalNode* sqlParser::DropIndexContext::DROP() {
  return getToken(sqlParser::DROP, 0);
}

tree::TerminalNode* sqlParser::DropIndexContext::ON() {
  return getToken(sqlParser::ON, 0);
}

tree::TerminalNode* sqlParser::DropIndexContext::INDEX() {
  return getToken(sqlParser::INDEX, 0);
}

tree::TerminalNode* sqlParser::DropIndexContext::KEY() {
  return getToken(sqlParser::KEY, 0);
}

sqlParser::IndexNameContext* sqlParser::DropIndexContext::indexName() {
  return getRuleContext<sqlParser::IndexNameContext>(0);
}

sqlParser::TableReferenceContext* sqlParser::DropIndexContext::tableReference() {
  return getRuleContext<sqlParser::TableReferenceContext>(0);
}

tree::TerminalNode* sqlParser::DropIndexContext::IF() {
  return getToken(sqlParser::IF, 0);
}

tree::TerminalNode* sqlParser::DropIndexContext::EXISTS() {
  return getToken(sqlParser::EXISTS, 0);
}

sqlParser::DropIndexContext::DropIndexContext(DropQueryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any sqlParser::DropIndexContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitDropIndex(this);
  else
    return visitor->visitChildren(this);
}
sqlParser::DropQueryContext* sqlParser::dropQuery() {
  DropQueryContext *_localctx = _tracker.createInstance<DropQueryContext>(_ctx, getState());
  enterRule(_localctx, 62, sqlParser::RuleDropQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(765);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 96, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<DropQueryContext *>(_tracker.createInstance<sqlParser::DropTableContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(748);
      match(sqlParser::DROP);
      setState(749);
      match(sqlParser::TABLE);
      setState(752);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::IF) {
        setState(750);
        match(sqlParser::IF);
        setState(751);
        match(sqlParser::EXISTS);
      }
      setState(754);
      dynamic_cast<DropTableContext *>(_localctx)->name = tableReference();
      break;
    }

    case 2: {
      _localctx = dynamic_cast<DropQueryContext *>(_tracker.createInstance<sqlParser::DropIndexContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(755);
      match(sqlParser::DROP);
      setState(756);
      _la = _input->LA(1);
      if (!(_la == sqlParser::INDEX

      || _la == sqlParser::KEY)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(759);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::IF) {
        setState(757);
        match(sqlParser::IF);
        setState(758);
        match(sqlParser::EXISTS);
      }
      setState(761);
      dynamic_cast<DropIndexContext *>(_localctx)->idxName = indexName();
      setState(762);
      match(sqlParser::ON);
      setState(763);
      dynamic_cast<DropIndexContext *>(_localctx)->tblName = tableReference();
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

sqlParser::TableReferenceContext* sqlParser::TruncateQueryContext::tableReference() {
  return getRuleContext<sqlParser::TableReferenceContext>(0);
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
  enterRule(_localctx, 64, sqlParser::RuleTruncateQuery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(767);
    match(sqlParser::TRUNCATE);
    setState(769);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 97, _ctx)) {
    case 1: {
      setState(768);
      match(sqlParser::TABLE);
      break;
    }

    }
    setState(771);
    dynamic_cast<TruncateQueryContext *>(_localctx)->name = tableReference();
   
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

std::vector<sqlParser::TableConstraintContext *> sqlParser::TableDefinitionContext::tableConstraint() {
  return getRuleContexts<sqlParser::TableConstraintContext>();
}

sqlParser::TableConstraintContext* sqlParser::TableDefinitionContext::tableConstraint(size_t i) {
  return getRuleContext<sqlParser::TableConstraintContext>(i);
}

std::vector<sqlParser::IndexDefinitionContext *> sqlParser::TableDefinitionContext::indexDefinition() {
  return getRuleContexts<sqlParser::IndexDefinitionContext>();
}

sqlParser::IndexDefinitionContext* sqlParser::TableDefinitionContext::indexDefinition(size_t i) {
  return getRuleContext<sqlParser::IndexDefinitionContext>(i);
}

tree::TerminalNode* sqlParser::TableDefinitionContext::REFERENCE() {
  return getToken(sqlParser::REFERENCE, 0);
}

sqlParser::TablePartitioningContext* sqlParser::TableDefinitionContext::tablePartitioning() {
  return getRuleContext<sqlParser::TablePartitioningContext>(0);
}

tree::TerminalNode* sqlParser::TableDefinitionContext::WITH() {
  return getToken(sqlParser::WITH, 0);
}

sqlParser::PropertyListContext* sqlParser::TableDefinitionContext::propertyList() {
  return getRuleContext<sqlParser::PropertyListContext>(0);
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
  enterRule(_localctx, 66, sqlParser::RuleTableDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(773);
    match(sqlParser::T__6);
    setState(777);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 98, _ctx)) {
    case 1: {
      setState(774);
      columnDefinition();
      break;
    }

    case 2: {
      setState(775);
      tableConstraint();
      break;
    }

    case 3: {
      setState(776);
      indexDefinition();
      break;
    }

    }
    setState(787);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(779);
      match(sqlParser::T__2);
      setState(783);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 99, _ctx)) {
      case 1: {
        setState(780);
        columnDefinition();
        break;
      }

      case 2: {
        setState(781);
        tableConstraint();
        break;
      }

      case 3: {
        setState(782);
        indexDefinition();
        break;
      }

      }
      setState(789);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(790);
    match(sqlParser::T__7);
    setState(792);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::REFERENCE) {
      setState(791);
      match(sqlParser::REFERENCE);
    }
    setState(795);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::PARTITION) {
      setState(794);
      tablePartitioning();
    }
    setState(799);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 103, _ctx)) {
    case 1: {
      setState(797);
      match(sqlParser::WITH);
      setState(798);
      dynamic_cast<TableDefinitionContext *>(_localctx)->props = propertyList();
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

//----------------- TablePartitioningContext ------------------------------------------------------------------

sqlParser::TablePartitioningContext::TablePartitioningContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::TablePartitioningContext::PARTITION() {
  return getToken(sqlParser::PARTITION, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::TablePartitioningContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::TablePartitioningContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::TablePartitioningContext::BY() {
  return getToken(sqlParser::BY, 0);
}

tree::TerminalNode* sqlParser::TablePartitioningContext::ON() {
  return getToken(sqlParser::ON, 0);
}

tree::TerminalNode* sqlParser::TablePartitioningContext::HASH() {
  return getToken(sqlParser::HASH, 0);
}

tree::TerminalNode* sqlParser::TablePartitioningContext::VALUE() {
  return getToken(sqlParser::VALUE, 0);
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
  enterRule(_localctx, 68, sqlParser::RuleTablePartitioning);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(801);
    match(sqlParser::PARTITION);
    setState(803);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::BY) {
      setState(802);
      match(sqlParser::BY);
    }
    setState(806);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::HASH

    || _la == sqlParser::VALUE) {
      setState(805);
      dynamic_cast<TablePartitioningContext *>(_localctx)->method = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == sqlParser::HASH

      || _la == sqlParser::VALUE)) {
        dynamic_cast<TablePartitioningContext *>(_localctx)->method = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(809);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ON) {
      setState(808);
      match(sqlParser::ON);
    }
    setState(811);
    match(sqlParser::T__6);
    setState(812);
    expr(0);
    setState(817);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(813);
      match(sqlParser::T__2);
      setState(814);
      expr(0);
      setState(819);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(820);
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

tree::TerminalNode* sqlParser::ColumnDefinitionContext::WITH() {
  return getToken(sqlParser::WITH, 0);
}

sqlParser::PropertyListContext* sqlParser::ColumnDefinitionContext::propertyList() {
  return getRuleContext<sqlParser::PropertyListContext>(0);
}

std::vector<tree::TerminalNode *> sqlParser::ColumnDefinitionContext::KEY() {
  return getTokens(sqlParser::KEY);
}

tree::TerminalNode* sqlParser::ColumnDefinitionContext::KEY(size_t i) {
  return getToken(sqlParser::KEY, i);
}

std::vector<tree::TerminalNode *> sqlParser::ColumnDefinitionContext::SQL_NULL() {
  return getTokens(sqlParser::SQL_NULL);
}

tree::TerminalNode* sqlParser::ColumnDefinitionContext::SQL_NULL(size_t i) {
  return getToken(sqlParser::SQL_NULL, i);
}

std::vector<tree::TerminalNode *> sqlParser::ColumnDefinitionContext::PRIMARY() {
  return getTokens(sqlParser::PRIMARY);
}

tree::TerminalNode* sqlParser::ColumnDefinitionContext::PRIMARY(size_t i) {
  return getToken(sqlParser::PRIMARY, i);
}

std::vector<tree::TerminalNode *> sqlParser::ColumnDefinitionContext::UNIQUE() {
  return getTokens(sqlParser::UNIQUE);
}

tree::TerminalNode* sqlParser::ColumnDefinitionContext::UNIQUE(size_t i) {
  return getToken(sqlParser::UNIQUE, i);
}

std::vector<tree::TerminalNode *> sqlParser::ColumnDefinitionContext::MONOTONIC() {
  return getTokens(sqlParser::MONOTONIC);
}

tree::TerminalNode* sqlParser::ColumnDefinitionContext::MONOTONIC(size_t i) {
  return getToken(sqlParser::MONOTONIC, i);
}

std::vector<tree::TerminalNode *> sqlParser::ColumnDefinitionContext::NOT() {
  return getTokens(sqlParser::NOT);
}

tree::TerminalNode* sqlParser::ColumnDefinitionContext::NOT(size_t i) {
  return getToken(sqlParser::NOT, i);
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
  enterRule(_localctx, 70, sqlParser::RuleColumnDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(822);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->name = columnName();
    setState(823);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->type = dataType();
    setState(841);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 78) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 78)) & ((1ULL << (sqlParser::NOT - 78))
      | (1ULL << (sqlParser::SQL_NULL - 78))
      | (1ULL << (sqlParser::PRIMARY - 78))
      | (1ULL << (sqlParser::UNIQUE - 78)))) != 0)) {
      setState(839);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 111, _ctx)) {
      case 1: {
        setState(825);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::NOT) {
          setState(824);
          dynamic_cast<ColumnDefinitionContext *>(_localctx)->notNullable = match(sqlParser::NOT);
        }
        setState(827);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->nullable = match(sqlParser::SQL_NULL);
        break;
      }

      case 2: {
        setState(828);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->primary = match(sqlParser::PRIMARY);
        setState(829);
        match(sqlParser::KEY);
        break;
      }

      case 3: {
        setState(830);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
        setState(832);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::KEY) {
          setState(831);
          match(sqlParser::KEY);
        }
        break;
      }

      case 4: {
        setState(834);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
        setState(835);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->mono = match(sqlParser::MONOTONIC);
        setState(837);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::KEY) {
          setState(836);
          match(sqlParser::KEY);
        }
        break;
      }

      }
      setState(843);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(846);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(844);
      match(sqlParser::WITH);
      setState(845);
      dynamic_cast<ColumnDefinitionContext *>(_localctx)->props = propertyList();
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
  enterRule(_localctx, 72, sqlParser::RuleColumnNameList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(848);
    match(sqlParser::T__6);
    setState(849);
    columnName();
    setState(854);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(850);
      match(sqlParser::T__2);
      setState(851);
      columnName();
      setState(856);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(857);
    match(sqlParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TableConstraintContext ------------------------------------------------------------------

sqlParser::TableConstraintContext::TableConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::TableConstraintContext::KEY() {
  return getToken(sqlParser::KEY, 0);
}

tree::TerminalNode* sqlParser::TableConstraintContext::PRIMARY() {
  return getToken(sqlParser::PRIMARY, 0);
}

std::vector<sqlParser::ExprContext *> sqlParser::TableConstraintContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::TableConstraintContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::TableConstraintContext::MONOTONIC() {
  return getToken(sqlParser::MONOTONIC, 0);
}

tree::TerminalNode* sqlParser::TableConstraintContext::UNIQUE() {
  return getToken(sqlParser::UNIQUE, 0);
}


size_t sqlParser::TableConstraintContext::getRuleIndex() const {
  return sqlParser::RuleTableConstraint;
}


antlrcpp::Any sqlParser::TableConstraintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitTableConstraint(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::TableConstraintContext* sqlParser::tableConstraint() {
  TableConstraintContext *_localctx = _tracker.createInstance<TableConstraintContext>(_ctx, getState());
  enterRule(_localctx, 74, sqlParser::RuleTableConstraint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(894);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::PRIMARY: {
        enterOuterAlt(_localctx, 1);
        setState(859);
        dynamic_cast<TableConstraintContext *>(_localctx)->primary = match(sqlParser::PRIMARY);
        setState(860);
        match(sqlParser::KEY);
        setState(873);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 116, _ctx)) {
        case 1: {
          setState(861);
          expr(0);
          break;
        }

        case 2: {
          setState(862);
          match(sqlParser::T__6);
          setState(863);
          expr(0);
          setState(868);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::T__2) {
            setState(864);
            match(sqlParser::T__2);
            setState(865);
            expr(0);
            setState(870);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(871);
          match(sqlParser::T__7);
          break;
        }

        }
        break;
      }

      case sqlParser::MONOTONIC:
      case sqlParser::UNIQUE: {
        enterOuterAlt(_localctx, 2);
        setState(876);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::UNIQUE) {
          setState(875);
          dynamic_cast<TableConstraintContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
        }
        setState(878);
        dynamic_cast<TableConstraintContext *>(_localctx)->mono = match(sqlParser::MONOTONIC);
        setState(879);
        match(sqlParser::KEY);
        setState(892);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 119, _ctx)) {
        case 1: {
          setState(880);
          expr(0);
          break;
        }

        case 2: {
          setState(881);
          match(sqlParser::T__6);
          setState(882);
          expr(0);
          setState(887);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::T__2) {
            setState(883);
            match(sqlParser::T__2);
            setState(884);
            expr(0);
            setState(889);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(890);
          match(sqlParser::T__7);
          break;
        }

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
  enterRule(_localctx, 76, sqlParser::RuleIndexDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(897);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(896);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
    }
    setState(899);
    _la = _input->LA(1);
    if (!(_la == sqlParser::INDEX

    || _la == sqlParser::KEY)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(901);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 122, _ctx)) {
    case 1: {
      setState(900);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->name = indexName();
      break;
    }

    }
    setState(904);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ON) {
      setState(903);
      match(sqlParser::ON);
    }
    setState(918);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 125, _ctx)) {
    case 1: {
      setState(906);
      expr(0);
      break;
    }

    case 2: {
      setState(907);
      match(sqlParser::T__6);
      setState(908);
      expr(0);
      setState(913);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(909);
        match(sqlParser::T__2);
        setState(910);
        expr(0);
        setState(915);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(916);
      match(sqlParser::T__7);
      break;
    }

    }
    setState(922);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::USING) {
      setState(920);
      match(sqlParser::USING);
      setState(921);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->method = indexMethod();
    }
    setState(926);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(924);
      match(sqlParser::WITH);
      setState(925);
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
  enterRule(_localctx, 78, sqlParser::RuleIndexMethod);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(928);
    _la = _input->LA(1);
    if (!(((((_la - 180) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 180)) & ((1ULL << (sqlParser::BTREE - 180))
      | (1ULL << (sqlParser::HASH - 180))
      | (1ULL << (sqlParser::SKIPLIST - 180))
      | (1ULL << (sqlParser::TREE - 180)))) != 0))) {
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

tree::TerminalNode* sqlParser::DataTypeContext::USING() {
  return getToken(sqlParser::USING, 0);
}

sqlParser::ClassNameContext* sqlParser::DataTypeContext::className() {
  return getRuleContext<sqlParser::ClassNameContext>(0);
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
  enterRule(_localctx, 80, sqlParser::RuleDataType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(936);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 129, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(930);
      sqlDataType();
      setState(933);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::USING) {
        setState(931);
        match(sqlParser::USING);
        setState(932);
        className();
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(935);
      className();
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

tree::TerminalNode* sqlParser::SqlDataTypeContext::COLLATE() {
  return getToken(sqlParser::COLLATE, 0);
}

sqlParser::SizeContext* sqlParser::SqlDataTypeContext::size() {
  return getRuleContext<sqlParser::SizeContext>(0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::TEXT() {
  return getToken(sqlParser::TEXT, 0);
}

sqlParser::CollationNameContext* sqlParser::SqlDataTypeContext::collationName() {
  return getRuleContext<sqlParser::CollationNameContext>(0);
}

std::vector<tree::TerminalNode *> sqlParser::SqlDataTypeContext::CHAR() {
  return getTokens(sqlParser::CHAR);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::CHAR(size_t i) {
  return getToken(sqlParser::CHAR, i);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::CHARACTER() {
  return getToken(sqlParser::CHARACTER, 0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::VARCHAR() {
  return getToken(sqlParser::VARCHAR, 0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::BYTE() {
  return getToken(sqlParser::BYTE, 0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::CHARS() {
  return getToken(sqlParser::CHARS, 0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::VARYING() {
  return getToken(sqlParser::VARYING, 0);
}

tree::TerminalNode* sqlParser::SqlDataTypeContext::BINARY() {
  return getToken(sqlParser::BINARY, 0);
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
  enterRule(_localctx, 82, sqlParser::RuleSqlDataType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1029);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::BOOLEAN: {
        enterOuterAlt(_localctx, 1);
        setState(938);
        match(sqlParser::BOOLEAN);
        break;
      }

      case sqlParser::INTEGER: {
        enterOuterAlt(_localctx, 2);
        setState(939);
        match(sqlParser::INTEGER);
        setState(947);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__6) {
          setState(940);
          match(sqlParser::T__6);
          setState(941);
          precision();
          setState(943);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 137) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 137)) & ((1ULL << (sqlParser::BITS - 137))
            | (1ULL << (sqlParser::BYTES - 137))
            | (1ULL << (sqlParser::DIGITS - 137)))) != 0)) {
            setState(942);
            _la = _input->LA(1);
            if (!(((((_la - 137) & ~ 0x3fULL) == 0) &&
              ((1ULL << (_la - 137)) & ((1ULL << (sqlParser::BITS - 137))
              | (1ULL << (sqlParser::BYTES - 137))
              | (1ULL << (sqlParser::DIGITS - 137)))) != 0))) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
          }
          setState(945);
          match(sqlParser::T__7);
        }
        break;
      }

      case sqlParser::DECIMAL: {
        enterOuterAlt(_localctx, 3);
        setState(949);
        match(sqlParser::DECIMAL);
        setState(964);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__6) {
          setState(950);
          match(sqlParser::T__6);
          setState(951);
          precision();
          setState(953);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::DIGITS) {
            setState(952);
            match(sqlParser::DIGITS);
          }
          setState(960);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__2) {
            setState(955);
            match(sqlParser::T__2);
            setState(956);
            scale();
            setState(958);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == sqlParser::DIGITS) {
              setState(957);
              match(sqlParser::DIGITS);
            }
          }
          setState(962);
          match(sqlParser::T__7);
        }
        break;
      }

      case sqlParser::FLOAT: {
        enterOuterAlt(_localctx, 4);
        setState(966);
        match(sqlParser::FLOAT);
        setState(974);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__6) {
          setState(967);
          match(sqlParser::T__6);
          setState(968);
          precision();
          setState(970);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 137) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 137)) & ((1ULL << (sqlParser::BITS - 137))
            | (1ULL << (sqlParser::BYTES - 137))
            | (1ULL << (sqlParser::DIGITS - 137)))) != 0)) {
            setState(969);
            _la = _input->LA(1);
            if (!(((((_la - 137) & ~ 0x3fULL) == 0) &&
              ((1ULL << (_la - 137)) & ((1ULL << (sqlParser::BITS - 137))
              | (1ULL << (sqlParser::BYTES - 137))
              | (1ULL << (sqlParser::DIGITS - 137)))) != 0))) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
          }
          setState(972);
          match(sqlParser::T__7);
        }
        break;
      }

      case sqlParser::DATE: {
        enterOuterAlt(_localctx, 5);
        setState(976);
        match(sqlParser::DATE);
        break;
      }

      case sqlParser::TIME: {
        enterOuterAlt(_localctx, 6);
        setState(977);
        match(sqlParser::TIME);
        break;
      }

      case sqlParser::TIMESTAMP: {
        enterOuterAlt(_localctx, 7);
        setState(978);
        match(sqlParser::TIMESTAMP);
        break;
      }

      case sqlParser::CHAR:
      case sqlParser::CHARACTER:
      case sqlParser::VARCHAR:
      case sqlParser::TEXT: {
        enterOuterAlt(_localctx, 8);
        setState(1004);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 143, _ctx)) {
        case 1: {
          setState(979);
          _la = _input->LA(1);
          if (!(_la == sqlParser::CHAR

          || _la == sqlParser::CHARACTER)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(980);
          match(sqlParser::T__6);
          setState(981);
          size();
          setState(983);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 128) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::CHAR - 128))
            | (1ULL << (sqlParser::BYTE - 128))
            | (1ULL << (sqlParser::BYTES - 128))
            | (1ULL << (sqlParser::CHARS - 128)))) != 0)) {
            setState(982);
            _la = _input->LA(1);
            if (!(((((_la - 128) & ~ 0x3fULL) == 0) &&
              ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::CHAR - 128))
              | (1ULL << (sqlParser::BYTE - 128))
              | (1ULL << (sqlParser::BYTES - 128))
              | (1ULL << (sqlParser::CHARS - 128)))) != 0))) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
          }
          setState(985);
          match(sqlParser::T__7);
          break;
        }

        case 2: {
          setState(992);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case sqlParser::CHAR:
            case sqlParser::CHARACTER: {
              setState(987);
              _la = _input->LA(1);
              if (!(_la == sqlParser::CHAR

              || _la == sqlParser::CHARACTER)) {
              _errHandler->recoverInline(this);
              }
              else {
                _errHandler->reportMatch(this);
                consume();
              }
              setState(989);
              _errHandler->sync(this);

              _la = _input->LA(1);
              if (_la == sqlParser::VARYING) {
                setState(988);
                match(sqlParser::VARYING);
              }
              break;
            }

            case sqlParser::VARCHAR: {
              setState(991);
              match(sqlParser::VARCHAR);
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(1001);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__6) {
            setState(994);
            match(sqlParser::T__6);
            setState(995);
            size();
            setState(997);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (((((_la - 128) & ~ 0x3fULL) == 0) &&
              ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::CHAR - 128))
              | (1ULL << (sqlParser::BYTE - 128))
              | (1ULL << (sqlParser::BYTES - 128))
              | (1ULL << (sqlParser::CHARS - 128)))) != 0)) {
              setState(996);
              _la = _input->LA(1);
              if (!(((((_la - 128) & ~ 0x3fULL) == 0) &&
                ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::CHAR - 128))
                | (1ULL << (sqlParser::BYTE - 128))
                | (1ULL << (sqlParser::BYTES - 128))
                | (1ULL << (sqlParser::CHARS - 128)))) != 0))) {
              _errHandler->recoverInline(this);
              }
              else {
                _errHandler->reportMatch(this);
                consume();
              }
            }
            setState(999);
            match(sqlParser::T__7);
          }
          break;
        }

        case 3: {
          setState(1003);
          match(sqlParser::TEXT);
          break;
        }

        }
        setState(1008);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::COLLATE) {
          setState(1006);
          match(sqlParser::COLLATE);
          setState(1007);
          dynamic_cast<SqlDataTypeContext *>(_localctx)->collate = collationName();
        }
        break;
      }

      case sqlParser::BINARY: {
        enterOuterAlt(_localctx, 9);
        setState(1010);
        match(sqlParser::BINARY);

        setState(1011);
        match(sqlParser::T__6);
        setState(1012);
        size();
        setState(1014);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::BYTE

        || _la == sqlParser::BYTES) {
          setState(1013);
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
        setState(1016);
        match(sqlParser::T__7);
        break;
      }

      case sqlParser::VARBINARY: {
        enterOuterAlt(_localctx, 10);
        setState(1018);
        match(sqlParser::VARBINARY);
        setState(1026);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__6) {
          setState(1019);
          match(sqlParser::T__6);
          setState(1020);
          size();
          setState(1022);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::BYTE

          || _la == sqlParser::BYTES) {
            setState(1021);
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
          setState(1024);
          match(sqlParser::T__7);
        }
        break;
      }

      case sqlParser::BLOB: {
        enterOuterAlt(_localctx, 11);
        setState(1028);
        match(sqlParser::BLOB);
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
  enterRule(_localctx, 84, sqlParser::RuleAsSubquery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1031);
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
  enterRule(_localctx, 86, sqlParser::RuleSelectOrValues);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    setState(1054);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 152, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1033);
      match(sqlParser::T__6);
      setState(1037);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 149, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(1034);
          queryHint(); 
        }
        setState(1039);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 149, _ctx);
      }
      setState(1040);
      selectOrValues();
      setState(1041);
      match(sqlParser::T__7);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1046);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::HINT_START) {
        setState(1043);
        queryHint();
        setState(1048);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(1052);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 151, _ctx)) {
      case 1: {
        setState(1049);
        selectQuery();
        break;
      }

      case 2: {
        setState(1050);
        valuesExpr();
        break;
      }

      case 3: {
        setState(1051);
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
  enterRule(_localctx, 88, sqlParser::RuleSelectList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1056);
    selectListItem();
    setState(1061);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 153, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1057);
        match(sqlParser::T__2);
        setState(1058);
        selectListItem(); 
      }
      setState(1063);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 153, _ctx);
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
  enterRule(_localctx, 90, sqlParser::RuleSelectListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1072);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 156, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1064);
      expr(0);
      setState(1069);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 155, _ctx)) {
      case 1: {
        setState(1066);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(1065);
          match(sqlParser::AS);
        }
        setState(1068);
        dynamic_cast<SelectListItemContext *>(_localctx)->as = identifier();
        break;
      }

      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1071);
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
  enterRule(_localctx, 92, sqlParser::RuleGroupByList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1074);
    groupByListItem();
    setState(1079);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 157, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1075);
        match(sqlParser::T__2);
        setState(1076);
        groupByListItem(); 
      }
      setState(1081);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 157, _ctx);
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
  enterRule(_localctx, 94, sqlParser::RuleGroupByListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1082);
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
  enterRule(_localctx, 96, sqlParser::RuleOrderByList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1084);
    orderByListItem();
    setState(1089);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1085);
      match(sqlParser::T__2);
      setState(1086);
      orderByListItem();
      setState(1091);
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
  enterRule(_localctx, 98, sqlParser::RuleOrderByListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1092);
    expr(0);
    setState(1094);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ASC

    || _la == sqlParser::DESC) {
      setState(1093);
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
  enterRule(_localctx, 100, sqlParser::RuleJoinUsingList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1096);
    joinUsingListItem();
    setState(1101);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1097);
      match(sqlParser::T__2);
      setState(1098);
      joinUsingListItem();
      setState(1103);
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
  enterRule(_localctx, 102, sqlParser::RuleJoinUsingListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1104);
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
  enterRule(_localctx, 104, sqlParser::RuleCorrList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1106);
    corrListItem();
    setState(1111);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1107);
      match(sqlParser::T__2);
      setState(1108);
      corrListItem();
      setState(1113);
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
  enterRule(_localctx, 106, sqlParser::RuleCorrListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1114);
    dynamic_cast<CorrListItemContext *>(_localctx)->id = identifier();
   
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
  enterRule(_localctx, 108, sqlParser::RulePropertyList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1116);
    property();
    setState(1121);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 162, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1117);
        match(sqlParser::T__2);
        setState(1118);
        property(); 
      }
      setState(1123);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 162, _ctx);
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
  enterRule(_localctx, 110, sqlParser::RuleProperty);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1124);
    dynamic_cast<PropertyContext *>(_localctx)->name = propertyName();
    setState(1127);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__5) {
      setState(1125);
      match(sqlParser::T__5);
      setState(1126);
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
  enterRule(_localctx, 112, sqlParser::RuleValueList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1129);
    expr(0);
    setState(1134);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1130);
      match(sqlParser::T__2);
      setState(1131);
      expr(0);
      setState(1136);
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
  enterRule(_localctx, 114, sqlParser::RulePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1137);
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
  enterRule(_localctx, 116, sqlParser::RuleJoinPredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1139);
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
  enterRule(_localctx, 118, sqlParser::RuleCasePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1141);
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

std::vector<sqlParser::ExprContext *> sqlParser::SubStringFuncContext::expr() {
  return getRuleContexts<sqlParser::ExprContext>();
}

sqlParser::ExprContext* sqlParser::SubStringFuncContext::expr(size_t i) {
  return getRuleContext<sqlParser::ExprContext>(i);
}

tree::TerminalNode* sqlParser::SubStringFuncContext::FROM() {
  return getToken(sqlParser::FROM, 0);
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

tree::TerminalNode* sqlParser::LogicOpContext::OR() {
  return getToken(sqlParser::OR, 0);
}

tree::TerminalNode* sqlParser::LogicOpContext::XOR() {
  return getToken(sqlParser::XOR, 0);
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
  size_t startState = 120;
  enterRecursionRule(_localctx, 120, sqlParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1406);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 192, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<LiteralValContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(1144);
      literal();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<VarRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1145);
      variableName();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ColRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1146);
      columnRef();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<InlineCastContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1147);
      match(sqlParser::T__6);
      setState(1148);
      dataType();
      setState(1149);
      match(sqlParser::T__7);
      setState(1150);
      expr(40);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<UnaryOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1152);
      dynamic_cast<UnaryOpContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__3

      || _la == sqlParser::T__10)) {
        dynamic_cast<UnaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1153);
      expr(39);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<CastOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1154);
      match(sqlParser::CAST);
      setState(1155);
      match(sqlParser::T__6);
      setState(1156);
      expr(0);
      setState(1157);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2

      || _la == sqlParser::AS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1158);
      dataType();
      setState(1159);
      match(sqlParser::T__7);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<TypeOfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1161);
      match(sqlParser::TYPEOF);
      setState(1162);
      match(sqlParser::T__6);
      setState(1163);
      expr(0);
      setState(1164);
      match(sqlParser::T__7);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<CaseOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1166);
      match(sqlParser::CASE);
      setState(1186);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::T__3:
        case sqlParser::T__6:
        case sqlParser::T__10:
        case sqlParser::ANTLR:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BOTH:
        case sqlParser::BY:
        case sqlParser::CATALOG:
        case sqlParser::CORRESPONDING:
        case sqlParser::CREATE:
        case sqlParser::CURRENT_CATALOG:
        case sqlParser::CURRENT_SCHEMA:
        case sqlParser::CURRENT_TIMESTAMP:
        case sqlParser::CURRENT_USER:
        case sqlParser::DATA:
        case sqlParser::DATE:
        case sqlParser::DELETE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DROP:
        case sqlParser::ESCAPE:
        case sqlParser::EXCEPT:
        case sqlParser::EXPLAIN:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FORMAT:
        case sqlParser::INDEX:
        case sqlParser::INSERT:
        case sqlParser::INTO:
        case sqlParser::IS:
        case sqlParser::JOIN:
        case sqlParser::KEY:
        case sqlParser::LEFT:
        case sqlParser::MAX:
        case sqlParser::MIN:
        case sqlParser::MONOTONIC:
        case sqlParser::NO:
        case sqlParser::NODE:
        case sqlParser::NOT:
        case sqlParser::SQL_NULL:
        case sqlParser::OFFSET:
        case sqlParser::OUTER:
        case sqlParser::OVERLAY:
        case sqlParser::PARTITION:
        case sqlParser::PRETTY:
        case sqlParser::REFERENCE:
        case sqlParser::RIGHT:
        case sqlParser::SCHEMA:
        case sqlParser::SET:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::SUM:
        case sqlParser::TABLE:
        case sqlParser::SQL_TRUE:
        case sqlParser::TYPE:
        case sqlParser::UNION:
        case sqlParser::UNIQUE:
        case sqlParser::UPDATE:
        case sqlParser::USING:
        case sqlParser::VALUES:
        case sqlParser::WITH_NAMES:
        case sqlParser::BOOLEAN:
        case sqlParser::INTEGER:
        case sqlParser::DECIMAL:
        case sqlParser::FLOAT:
        case sqlParser::TIME:
        case sqlParser::TIMESTAMP:
        case sqlParser::INTERVAL:
        case sqlParser::CHAR:
        case sqlParser::CHARACTER:
        case sqlParser::BINARY:
        case sqlParser::VARBINARY:
        case sqlParser::VARYING:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CHARS:
        case sqlParser::DIGITS:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::COALESCE:
        case sqlParser::CONCAT:
        case sqlParser::COUNT:
        case sqlParser::DECODE:
        case sqlParser::EXTRACT:
        case sqlParser::EXISTS:
        case sqlParser::IF:
        case sqlParser::IFNULL:
        case sqlParser::NULLIF:
        case sqlParser::POSITION:
        case sqlParser::SUBSTR:
        case sqlParser::SUBSTRING:
        case sqlParser::TRIM:
        case sqlParser::TYPEOF:
        case sqlParser::DAY:
        case sqlParser::DAYS:
        case sqlParser::HOUR:
        case sqlParser::HOURS:
        case sqlParser::MINUTE:
        case sqlParser::MINUTES:
        case sqlParser::MONTH:
        case sqlParser::MONTHS:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::SECOND:
        case sqlParser::SECONDS:
        case sqlParser::WEEK:
        case sqlParser::WEEKS:
        case sqlParser::YEAR:
        case sqlParser::YEARS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BTREE:
        case sqlParser::HASH:
        case sqlParser::LOOP:
        case sqlParser::SKIPLIST:
        case sqlParser::TREE:
        case sqlParser::VALUE:
        case sqlParser::IDENTIFIER:
        case sqlParser::STRING_LITERAL:
        case sqlParser::FLOAT_LITERAL:
        case sqlParser::DECIMAL_LITERAL:
        case sqlParser::INTEGER_LITERAL: {
          setState(1167);
          dynamic_cast<CaseOpContext *>(_localctx)->caseExpr = expr(0);
          setState(1173); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1168);
            match(sqlParser::WHEN);
            setState(1169);
            expr(0);
            setState(1170);
            match(sqlParser::THEN);
            setState(1171);
            expr(0);
            setState(1175); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

        case sqlParser::WHEN: {
          setState(1182); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1177);
            match(sqlParser::WHEN);
            setState(1178);
            casePredicate();
            setState(1179);
            match(sqlParser::THEN);
            setState(1180);
            expr(0);
            setState(1184); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(1190);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ELSE) {
        setState(1188);
        match(sqlParser::ELSE);
        setState(1189);
        expr(0);
      }
      setState(1192);
      match(sqlParser::END);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<IfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1194);
      match(sqlParser::IF);
      setState(1195);
      match(sqlParser::T__6);
      setState(1196);
      casePredicate();
      setState(1197);
      match(sqlParser::T__2);
      setState(1198);
      expr(0);
      setState(1199);
      match(sqlParser::T__2);
      setState(1200);
      expr(0);
      setState(1201);
      match(sqlParser::T__7);
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<IfNullOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1203);
      match(sqlParser::IFNULL);
      setState(1204);
      match(sqlParser::T__6);
      setState(1205);
      expr(0);
      setState(1206);
      match(sqlParser::T__2);
      setState(1207);
      expr(0);
      setState(1208);
      match(sqlParser::T__7);
      break;
    }

    case 11: {
      _localctx = _tracker.createInstance<NullIfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1210);
      match(sqlParser::NULLIF);
      setState(1211);
      match(sqlParser::T__6);
      setState(1212);
      expr(0);
      setState(1213);
      match(sqlParser::T__2);
      setState(1214);
      expr(0);
      setState(1215);
      match(sqlParser::T__7);
      break;
    }

    case 12: {
      _localctx = _tracker.createInstance<CoalesceOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1217);
      match(sqlParser::COALESCE);
      setState(1218);
      match(sqlParser::T__6);
      setState(1227);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 4) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 4)) & ((1ULL << (sqlParser::T__3 - 4))
        | (1ULL << (sqlParser::T__6 - 4))
        | (1ULL << (sqlParser::T__10 - 4))
        | (1ULL << (sqlParser::ANTLR - 4))
        | (1ULL << (sqlParser::ASC - 4))
        | (1ULL << (sqlParser::AVG - 4))
        | (1ULL << (sqlParser::BOTH - 4))
        | (1ULL << (sqlParser::BY - 4))
        | (1ULL << (sqlParser::CATALOG - 4))
        | (1ULL << (sqlParser::CORRESPONDING - 4))
        | (1ULL << (sqlParser::CREATE - 4))
        | (1ULL << (sqlParser::CURRENT_CATALOG - 4))
        | (1ULL << (sqlParser::CURRENT_SCHEMA - 4))
        | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 4))
        | (1ULL << (sqlParser::CURRENT_USER - 4))
        | (1ULL << (sqlParser::DATA - 4))
        | (1ULL << (sqlParser::DATE - 4))
        | (1ULL << (sqlParser::DELETE - 4))
        | (1ULL << (sqlParser::DESC - 4))
        | (1ULL << (sqlParser::DETAIL - 4))
        | (1ULL << (sqlParser::DROP - 4))
        | (1ULL << (sqlParser::ESCAPE - 4))
        | (1ULL << (sqlParser::EXCEPT - 4))
        | (1ULL << (sqlParser::EXPLAIN - 4))
        | (1ULL << (sqlParser::SQL_FALSE - 4))
        | (1ULL << (sqlParser::FEDERATION - 4))
        | (1ULL << (sqlParser::FORMAT - 4))
        | (1ULL << (sqlParser::INDEX - 4))
        | (1ULL << (sqlParser::INSERT - 4))
        | (1ULL << (sqlParser::INTO - 4))
        | (1ULL << (sqlParser::IS - 4))
        | (1ULL << (sqlParser::JOIN - 4))
        | (1ULL << (sqlParser::KEY - 4)))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::LEFT - 69))
        | (1ULL << (sqlParser::MAX - 69))
        | (1ULL << (sqlParser::MIN - 69))
        | (1ULL << (sqlParser::MONOTONIC - 69))
        | (1ULL << (sqlParser::NO - 69))
        | (1ULL << (sqlParser::NODE - 69))
        | (1ULL << (sqlParser::NOT - 69))
        | (1ULL << (sqlParser::SQL_NULL - 69))
        | (1ULL << (sqlParser::OFFSET - 69))
        | (1ULL << (sqlParser::OUTER - 69))
        | (1ULL << (sqlParser::OVERLAY - 69))
        | (1ULL << (sqlParser::PARTITION - 69))
        | (1ULL << (sqlParser::PRETTY - 69))
        | (1ULL << (sqlParser::REFERENCE - 69))
        | (1ULL << (sqlParser::RIGHT - 69))
        | (1ULL << (sqlParser::SCHEMA - 69))
        | (1ULL << (sqlParser::SET - 69))
        | (1ULL << (sqlParser::STAGE - 69))
        | (1ULL << (sqlParser::STATS - 69))
        | (1ULL << (sqlParser::STATISTICS - 69))
        | (1ULL << (sqlParser::SUM - 69))
        | (1ULL << (sqlParser::TABLE - 69))
        | (1ULL << (sqlParser::SQL_TRUE - 69))
        | (1ULL << (sqlParser::TYPE - 69))
        | (1ULL << (sqlParser::UNION - 69))
        | (1ULL << (sqlParser::UNIQUE - 69))
        | (1ULL << (sqlParser::UPDATE - 69))
        | (1ULL << (sqlParser::USING - 69))
        | (1ULL << (sqlParser::VALUES - 69))
        | (1ULL << (sqlParser::WITH_NAMES - 69))
        | (1ULL << (sqlParser::BOOLEAN - 69))
        | (1ULL << (sqlParser::INTEGER - 69))
        | (1ULL << (sqlParser::DECIMAL - 69))
        | (1ULL << (sqlParser::FLOAT - 69))
        | (1ULL << (sqlParser::TIME - 69))
        | (1ULL << (sqlParser::TIMESTAMP - 69))
        | (1ULL << (sqlParser::INTERVAL - 69))
        | (1ULL << (sqlParser::CHAR - 69))
        | (1ULL << (sqlParser::CHARACTER - 69))
        | (1ULL << (sqlParser::BINARY - 69)))) != 0) || ((((_la - 133) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 133)) & ((1ULL << (sqlParser::VARBINARY - 133))
        | (1ULL << (sqlParser::VARYING - 133))
        | (1ULL << (sqlParser::BIT - 133))
        | (1ULL << (sqlParser::BITS - 133))
        | (1ULL << (sqlParser::BYTE - 133))
        | (1ULL << (sqlParser::BYTES - 133))
        | (1ULL << (sqlParser::CHARS - 133))
        | (1ULL << (sqlParser::DIGITS - 133))
        | (1ULL << (sqlParser::CASE - 133))
        | (1ULL << (sqlParser::CAST - 133))
        | (1ULL << (sqlParser::COALESCE - 133))
        | (1ULL << (sqlParser::CONCAT - 133))
        | (1ULL << (sqlParser::COUNT - 133))
        | (1ULL << (sqlParser::DECODE - 133))
        | (1ULL << (sqlParser::EXTRACT - 133))
        | (1ULL << (sqlParser::EXISTS - 133))
        | (1ULL << (sqlParser::IF - 133))
        | (1ULL << (sqlParser::IFNULL - 133))
        | (1ULL << (sqlParser::NULLIF - 133))
        | (1ULL << (sqlParser::POSITION - 133))
        | (1ULL << (sqlParser::SUBSTR - 133))
        | (1ULL << (sqlParser::SUBSTRING - 133))
        | (1ULL << (sqlParser::TRIM - 133))
        | (1ULL << (sqlParser::TYPEOF - 133))
        | (1ULL << (sqlParser::DAY - 133))
        | (1ULL << (sqlParser::DAYS - 133))
        | (1ULL << (sqlParser::HOUR - 133))
        | (1ULL << (sqlParser::HOURS - 133))
        | (1ULL << (sqlParser::MINUTE - 133))
        | (1ULL << (sqlParser::MINUTES - 133))
        | (1ULL << (sqlParser::MONTH - 133))
        | (1ULL << (sqlParser::MONTHS - 133))
        | (1ULL << (sqlParser::QUARTER - 133))
        | (1ULL << (sqlParser::QUARTERS - 133))
        | (1ULL << (sqlParser::SECOND - 133))
        | (1ULL << (sqlParser::SECONDS - 133))
        | (1ULL << (sqlParser::WEEK - 133))
        | (1ULL << (sqlParser::WEEKS - 133))
        | (1ULL << (sqlParser::YEAR - 133))
        | (1ULL << (sqlParser::YEARS - 133))
        | (1ULL << (sqlParser::BLOCK - 133))
        | (1ULL << (sqlParser::BLOOM - 133))
        | (1ULL << (sqlParser::BTREE - 133))
        | (1ULL << (sqlParser::HASH - 133))
        | (1ULL << (sqlParser::LOOP - 133))
        | (1ULL << (sqlParser::SKIPLIST - 133))
        | (1ULL << (sqlParser::TREE - 133))
        | (1ULL << (sqlParser::VALUE - 133))
        | (1ULL << (sqlParser::IDENTIFIER - 133))
        | (1ULL << (sqlParser::STRING_LITERAL - 133))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 133))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 133))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 133)))) != 0)) {
        setState(1219);
        expr(0);
        setState(1224);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1220);
          match(sqlParser::T__2);
          setState(1221);
          expr(0);
          setState(1226);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1229);
      match(sqlParser::T__7);
      break;
    }

    case 13: {
      _localctx = _tracker.createInstance<DecodeOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1230);
      match(sqlParser::DECODE);
      setState(1231);
      match(sqlParser::T__6);
      setState(1240);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 4) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 4)) & ((1ULL << (sqlParser::T__3 - 4))
        | (1ULL << (sqlParser::T__6 - 4))
        | (1ULL << (sqlParser::T__10 - 4))
        | (1ULL << (sqlParser::ANTLR - 4))
        | (1ULL << (sqlParser::ASC - 4))
        | (1ULL << (sqlParser::AVG - 4))
        | (1ULL << (sqlParser::BOTH - 4))
        | (1ULL << (sqlParser::BY - 4))
        | (1ULL << (sqlParser::CATALOG - 4))
        | (1ULL << (sqlParser::CORRESPONDING - 4))
        | (1ULL << (sqlParser::CREATE - 4))
        | (1ULL << (sqlParser::CURRENT_CATALOG - 4))
        | (1ULL << (sqlParser::CURRENT_SCHEMA - 4))
        | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 4))
        | (1ULL << (sqlParser::CURRENT_USER - 4))
        | (1ULL << (sqlParser::DATA - 4))
        | (1ULL << (sqlParser::DATE - 4))
        | (1ULL << (sqlParser::DELETE - 4))
        | (1ULL << (sqlParser::DESC - 4))
        | (1ULL << (sqlParser::DETAIL - 4))
        | (1ULL << (sqlParser::DROP - 4))
        | (1ULL << (sqlParser::ESCAPE - 4))
        | (1ULL << (sqlParser::EXCEPT - 4))
        | (1ULL << (sqlParser::EXPLAIN - 4))
        | (1ULL << (sqlParser::SQL_FALSE - 4))
        | (1ULL << (sqlParser::FEDERATION - 4))
        | (1ULL << (sqlParser::FORMAT - 4))
        | (1ULL << (sqlParser::INDEX - 4))
        | (1ULL << (sqlParser::INSERT - 4))
        | (1ULL << (sqlParser::INTO - 4))
        | (1ULL << (sqlParser::IS - 4))
        | (1ULL << (sqlParser::JOIN - 4))
        | (1ULL << (sqlParser::KEY - 4)))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::LEFT - 69))
        | (1ULL << (sqlParser::MAX - 69))
        | (1ULL << (sqlParser::MIN - 69))
        | (1ULL << (sqlParser::MONOTONIC - 69))
        | (1ULL << (sqlParser::NO - 69))
        | (1ULL << (sqlParser::NODE - 69))
        | (1ULL << (sqlParser::NOT - 69))
        | (1ULL << (sqlParser::SQL_NULL - 69))
        | (1ULL << (sqlParser::OFFSET - 69))
        | (1ULL << (sqlParser::OUTER - 69))
        | (1ULL << (sqlParser::OVERLAY - 69))
        | (1ULL << (sqlParser::PARTITION - 69))
        | (1ULL << (sqlParser::PRETTY - 69))
        | (1ULL << (sqlParser::REFERENCE - 69))
        | (1ULL << (sqlParser::RIGHT - 69))
        | (1ULL << (sqlParser::SCHEMA - 69))
        | (1ULL << (sqlParser::SET - 69))
        | (1ULL << (sqlParser::STAGE - 69))
        | (1ULL << (sqlParser::STATS - 69))
        | (1ULL << (sqlParser::STATISTICS - 69))
        | (1ULL << (sqlParser::SUM - 69))
        | (1ULL << (sqlParser::TABLE - 69))
        | (1ULL << (sqlParser::SQL_TRUE - 69))
        | (1ULL << (sqlParser::TYPE - 69))
        | (1ULL << (sqlParser::UNION - 69))
        | (1ULL << (sqlParser::UNIQUE - 69))
        | (1ULL << (sqlParser::UPDATE - 69))
        | (1ULL << (sqlParser::USING - 69))
        | (1ULL << (sqlParser::VALUES - 69))
        | (1ULL << (sqlParser::WITH_NAMES - 69))
        | (1ULL << (sqlParser::BOOLEAN - 69))
        | (1ULL << (sqlParser::INTEGER - 69))
        | (1ULL << (sqlParser::DECIMAL - 69))
        | (1ULL << (sqlParser::FLOAT - 69))
        | (1ULL << (sqlParser::TIME - 69))
        | (1ULL << (sqlParser::TIMESTAMP - 69))
        | (1ULL << (sqlParser::INTERVAL - 69))
        | (1ULL << (sqlParser::CHAR - 69))
        | (1ULL << (sqlParser::CHARACTER - 69))
        | (1ULL << (sqlParser::BINARY - 69)))) != 0) || ((((_la - 133) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 133)) & ((1ULL << (sqlParser::VARBINARY - 133))
        | (1ULL << (sqlParser::VARYING - 133))
        | (1ULL << (sqlParser::BIT - 133))
        | (1ULL << (sqlParser::BITS - 133))
        | (1ULL << (sqlParser::BYTE - 133))
        | (1ULL << (sqlParser::BYTES - 133))
        | (1ULL << (sqlParser::CHARS - 133))
        | (1ULL << (sqlParser::DIGITS - 133))
        | (1ULL << (sqlParser::CASE - 133))
        | (1ULL << (sqlParser::CAST - 133))
        | (1ULL << (sqlParser::COALESCE - 133))
        | (1ULL << (sqlParser::CONCAT - 133))
        | (1ULL << (sqlParser::COUNT - 133))
        | (1ULL << (sqlParser::DECODE - 133))
        | (1ULL << (sqlParser::EXTRACT - 133))
        | (1ULL << (sqlParser::EXISTS - 133))
        | (1ULL << (sqlParser::IF - 133))
        | (1ULL << (sqlParser::IFNULL - 133))
        | (1ULL << (sqlParser::NULLIF - 133))
        | (1ULL << (sqlParser::POSITION - 133))
        | (1ULL << (sqlParser::SUBSTR - 133))
        | (1ULL << (sqlParser::SUBSTRING - 133))
        | (1ULL << (sqlParser::TRIM - 133))
        | (1ULL << (sqlParser::TYPEOF - 133))
        | (1ULL << (sqlParser::DAY - 133))
        | (1ULL << (sqlParser::DAYS - 133))
        | (1ULL << (sqlParser::HOUR - 133))
        | (1ULL << (sqlParser::HOURS - 133))
        | (1ULL << (sqlParser::MINUTE - 133))
        | (1ULL << (sqlParser::MINUTES - 133))
        | (1ULL << (sqlParser::MONTH - 133))
        | (1ULL << (sqlParser::MONTHS - 133))
        | (1ULL << (sqlParser::QUARTER - 133))
        | (1ULL << (sqlParser::QUARTERS - 133))
        | (1ULL << (sqlParser::SECOND - 133))
        | (1ULL << (sqlParser::SECONDS - 133))
        | (1ULL << (sqlParser::WEEK - 133))
        | (1ULL << (sqlParser::WEEKS - 133))
        | (1ULL << (sqlParser::YEAR - 133))
        | (1ULL << (sqlParser::YEARS - 133))
        | (1ULL << (sqlParser::BLOCK - 133))
        | (1ULL << (sqlParser::BLOOM - 133))
        | (1ULL << (sqlParser::BTREE - 133))
        | (1ULL << (sqlParser::HASH - 133))
        | (1ULL << (sqlParser::LOOP - 133))
        | (1ULL << (sqlParser::SKIPLIST - 133))
        | (1ULL << (sqlParser::TREE - 133))
        | (1ULL << (sqlParser::VALUE - 133))
        | (1ULL << (sqlParser::IDENTIFIER - 133))
        | (1ULL << (sqlParser::STRING_LITERAL - 133))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 133))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 133))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 133)))) != 0)) {
        setState(1232);
        expr(0);
        setState(1237);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1233);
          match(sqlParser::T__2);
          setState(1234);
          expr(0);
          setState(1239);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1242);
      match(sqlParser::T__7);
      break;
    }

    case 14: {
      _localctx = _tracker.createInstance<ConcatOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1243);
      match(sqlParser::CONCAT);
      setState(1244);
      match(sqlParser::T__6);
      setState(1253);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 4) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 4)) & ((1ULL << (sqlParser::T__3 - 4))
        | (1ULL << (sqlParser::T__6 - 4))
        | (1ULL << (sqlParser::T__10 - 4))
        | (1ULL << (sqlParser::ANTLR - 4))
        | (1ULL << (sqlParser::ASC - 4))
        | (1ULL << (sqlParser::AVG - 4))
        | (1ULL << (sqlParser::BOTH - 4))
        | (1ULL << (sqlParser::BY - 4))
        | (1ULL << (sqlParser::CATALOG - 4))
        | (1ULL << (sqlParser::CORRESPONDING - 4))
        | (1ULL << (sqlParser::CREATE - 4))
        | (1ULL << (sqlParser::CURRENT_CATALOG - 4))
        | (1ULL << (sqlParser::CURRENT_SCHEMA - 4))
        | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 4))
        | (1ULL << (sqlParser::CURRENT_USER - 4))
        | (1ULL << (sqlParser::DATA - 4))
        | (1ULL << (sqlParser::DATE - 4))
        | (1ULL << (sqlParser::DELETE - 4))
        | (1ULL << (sqlParser::DESC - 4))
        | (1ULL << (sqlParser::DETAIL - 4))
        | (1ULL << (sqlParser::DROP - 4))
        | (1ULL << (sqlParser::ESCAPE - 4))
        | (1ULL << (sqlParser::EXCEPT - 4))
        | (1ULL << (sqlParser::EXPLAIN - 4))
        | (1ULL << (sqlParser::SQL_FALSE - 4))
        | (1ULL << (sqlParser::FEDERATION - 4))
        | (1ULL << (sqlParser::FORMAT - 4))
        | (1ULL << (sqlParser::INDEX - 4))
        | (1ULL << (sqlParser::INSERT - 4))
        | (1ULL << (sqlParser::INTO - 4))
        | (1ULL << (sqlParser::IS - 4))
        | (1ULL << (sqlParser::JOIN - 4))
        | (1ULL << (sqlParser::KEY - 4)))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::LEFT - 69))
        | (1ULL << (sqlParser::MAX - 69))
        | (1ULL << (sqlParser::MIN - 69))
        | (1ULL << (sqlParser::MONOTONIC - 69))
        | (1ULL << (sqlParser::NO - 69))
        | (1ULL << (sqlParser::NODE - 69))
        | (1ULL << (sqlParser::NOT - 69))
        | (1ULL << (sqlParser::SQL_NULL - 69))
        | (1ULL << (sqlParser::OFFSET - 69))
        | (1ULL << (sqlParser::OUTER - 69))
        | (1ULL << (sqlParser::OVERLAY - 69))
        | (1ULL << (sqlParser::PARTITION - 69))
        | (1ULL << (sqlParser::PRETTY - 69))
        | (1ULL << (sqlParser::REFERENCE - 69))
        | (1ULL << (sqlParser::RIGHT - 69))
        | (1ULL << (sqlParser::SCHEMA - 69))
        | (1ULL << (sqlParser::SET - 69))
        | (1ULL << (sqlParser::STAGE - 69))
        | (1ULL << (sqlParser::STATS - 69))
        | (1ULL << (sqlParser::STATISTICS - 69))
        | (1ULL << (sqlParser::SUM - 69))
        | (1ULL << (sqlParser::TABLE - 69))
        | (1ULL << (sqlParser::SQL_TRUE - 69))
        | (1ULL << (sqlParser::TYPE - 69))
        | (1ULL << (sqlParser::UNION - 69))
        | (1ULL << (sqlParser::UNIQUE - 69))
        | (1ULL << (sqlParser::UPDATE - 69))
        | (1ULL << (sqlParser::USING - 69))
        | (1ULL << (sqlParser::VALUES - 69))
        | (1ULL << (sqlParser::WITH_NAMES - 69))
        | (1ULL << (sqlParser::BOOLEAN - 69))
        | (1ULL << (sqlParser::INTEGER - 69))
        | (1ULL << (sqlParser::DECIMAL - 69))
        | (1ULL << (sqlParser::FLOAT - 69))
        | (1ULL << (sqlParser::TIME - 69))
        | (1ULL << (sqlParser::TIMESTAMP - 69))
        | (1ULL << (sqlParser::INTERVAL - 69))
        | (1ULL << (sqlParser::CHAR - 69))
        | (1ULL << (sqlParser::CHARACTER - 69))
        | (1ULL << (sqlParser::BINARY - 69)))) != 0) || ((((_la - 133) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 133)) & ((1ULL << (sqlParser::VARBINARY - 133))
        | (1ULL << (sqlParser::VARYING - 133))
        | (1ULL << (sqlParser::BIT - 133))
        | (1ULL << (sqlParser::BITS - 133))
        | (1ULL << (sqlParser::BYTE - 133))
        | (1ULL << (sqlParser::BYTES - 133))
        | (1ULL << (sqlParser::CHARS - 133))
        | (1ULL << (sqlParser::DIGITS - 133))
        | (1ULL << (sqlParser::CASE - 133))
        | (1ULL << (sqlParser::CAST - 133))
        | (1ULL << (sqlParser::COALESCE - 133))
        | (1ULL << (sqlParser::CONCAT - 133))
        | (1ULL << (sqlParser::COUNT - 133))
        | (1ULL << (sqlParser::DECODE - 133))
        | (1ULL << (sqlParser::EXTRACT - 133))
        | (1ULL << (sqlParser::EXISTS - 133))
        | (1ULL << (sqlParser::IF - 133))
        | (1ULL << (sqlParser::IFNULL - 133))
        | (1ULL << (sqlParser::NULLIF - 133))
        | (1ULL << (sqlParser::POSITION - 133))
        | (1ULL << (sqlParser::SUBSTR - 133))
        | (1ULL << (sqlParser::SUBSTRING - 133))
        | (1ULL << (sqlParser::TRIM - 133))
        | (1ULL << (sqlParser::TYPEOF - 133))
        | (1ULL << (sqlParser::DAY - 133))
        | (1ULL << (sqlParser::DAYS - 133))
        | (1ULL << (sqlParser::HOUR - 133))
        | (1ULL << (sqlParser::HOURS - 133))
        | (1ULL << (sqlParser::MINUTE - 133))
        | (1ULL << (sqlParser::MINUTES - 133))
        | (1ULL << (sqlParser::MONTH - 133))
        | (1ULL << (sqlParser::MONTHS - 133))
        | (1ULL << (sqlParser::QUARTER - 133))
        | (1ULL << (sqlParser::QUARTERS - 133))
        | (1ULL << (sqlParser::SECOND - 133))
        | (1ULL << (sqlParser::SECONDS - 133))
        | (1ULL << (sqlParser::WEEK - 133))
        | (1ULL << (sqlParser::WEEKS - 133))
        | (1ULL << (sqlParser::YEAR - 133))
        | (1ULL << (sqlParser::YEARS - 133))
        | (1ULL << (sqlParser::BLOCK - 133))
        | (1ULL << (sqlParser::BLOOM - 133))
        | (1ULL << (sqlParser::BTREE - 133))
        | (1ULL << (sqlParser::HASH - 133))
        | (1ULL << (sqlParser::LOOP - 133))
        | (1ULL << (sqlParser::SKIPLIST - 133))
        | (1ULL << (sqlParser::TREE - 133))
        | (1ULL << (sqlParser::VALUE - 133))
        | (1ULL << (sqlParser::IDENTIFIER - 133))
        | (1ULL << (sqlParser::STRING_LITERAL - 133))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 133))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 133))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 133)))) != 0)) {
        setState(1245);
        expr(0);
        setState(1250);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1246);
          match(sqlParser::T__2);
          setState(1247);
          expr(0);
          setState(1252);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1255);
      match(sqlParser::T__7);
      break;
    }

    case 15: {
      _localctx = _tracker.createInstance<OverlayFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1256);
      match(sqlParser::OVERLAY);
      setState(1257);
      match(sqlParser::T__6);
      setState(1258);
      expr(0);
      setState(1259);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2 || _la == sqlParser::PLACING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1260);
      expr(0);
      setState(1261);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2

      || _la == sqlParser::FROM)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1262);
      expr(0);
      setState(1265);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__2

      || _la == sqlParser::FOR) {
        setState(1263);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__2

        || _la == sqlParser::FOR)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(1264);
        expr(0);
      }
      setState(1267);
      match(sqlParser::T__7);
      break;
    }

    case 16: {
      _localctx = _tracker.createInstance<PositionFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1269);
      match(sqlParser::POSITION);
      setState(1270);
      match(sqlParser::T__6);
      setState(1271);
      expr(0);
      setState(1272);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2

      || _la == sqlParser::IN)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1273);
      expr(0);
      setState(1274);
      match(sqlParser::T__7);
      break;
    }

    case 17: {
      _localctx = _tracker.createInstance<SubStringFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1276);
      _la = _input->LA(1);
      if (!(_la == sqlParser::SUBSTR

      || _la == sqlParser::SUBSTRING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1277);
      match(sqlParser::T__6);
      setState(1292);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 178, _ctx)) {
      case 1: {
        setState(1278);
        expr(0);
        setState(1279);
        match(sqlParser::T__2);
        setState(1280);
        expr(0);
        setState(1283);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__2) {
          setState(1281);
          match(sqlParser::T__2);
          setState(1282);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      case 2: {
        setState(1285);
        expr(0);
        setState(1286);
        match(sqlParser::FROM);
        setState(1287);
        expr(0);
        setState(1290);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::FOR) {
          setState(1288);
          match(sqlParser::FOR);
          setState(1289);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      }
      setState(1294);
      match(sqlParser::T__7);
      break;
    }

    case 18: {
      _localctx = _tracker.createInstance<TrimFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1296);
      match(sqlParser::TRIM);
      setState(1297);
      match(sqlParser::T__6);
      setState(1311);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 182, _ctx)) {
      case 1: {
        setState(1298);
        expr(0);
        setState(1301);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__2) {
          setState(1299);
          match(sqlParser::T__2);
          setState(1300);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        break;
      }

      case 2: {
        setState(1304);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 180, _ctx)) {
        case 1: {
          setState(1303);
          _la = _input->LA(1);
          if (!(_la == sqlParser::BOTH || _la == sqlParser::LEADING

          || _la == sqlParser::TRAILING)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          break;
        }

        }
        setState(1307);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 4) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 4)) & ((1ULL << (sqlParser::T__3 - 4))
          | (1ULL << (sqlParser::T__6 - 4))
          | (1ULL << (sqlParser::T__10 - 4))
          | (1ULL << (sqlParser::ANTLR - 4))
          | (1ULL << (sqlParser::ASC - 4))
          | (1ULL << (sqlParser::AVG - 4))
          | (1ULL << (sqlParser::BOTH - 4))
          | (1ULL << (sqlParser::BY - 4))
          | (1ULL << (sqlParser::CATALOG - 4))
          | (1ULL << (sqlParser::CORRESPONDING - 4))
          | (1ULL << (sqlParser::CREATE - 4))
          | (1ULL << (sqlParser::CURRENT_CATALOG - 4))
          | (1ULL << (sqlParser::CURRENT_SCHEMA - 4))
          | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 4))
          | (1ULL << (sqlParser::CURRENT_USER - 4))
          | (1ULL << (sqlParser::DATA - 4))
          | (1ULL << (sqlParser::DATE - 4))
          | (1ULL << (sqlParser::DELETE - 4))
          | (1ULL << (sqlParser::DESC - 4))
          | (1ULL << (sqlParser::DETAIL - 4))
          | (1ULL << (sqlParser::DROP - 4))
          | (1ULL << (sqlParser::ESCAPE - 4))
          | (1ULL << (sqlParser::EXCEPT - 4))
          | (1ULL << (sqlParser::EXPLAIN - 4))
          | (1ULL << (sqlParser::SQL_FALSE - 4))
          | (1ULL << (sqlParser::FEDERATION - 4))
          | (1ULL << (sqlParser::FORMAT - 4))
          | (1ULL << (sqlParser::INDEX - 4))
          | (1ULL << (sqlParser::INSERT - 4))
          | (1ULL << (sqlParser::INTO - 4))
          | (1ULL << (sqlParser::IS - 4))
          | (1ULL << (sqlParser::JOIN - 4))
          | (1ULL << (sqlParser::KEY - 4)))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::LEFT - 69))
          | (1ULL << (sqlParser::MAX - 69))
          | (1ULL << (sqlParser::MIN - 69))
          | (1ULL << (sqlParser::MONOTONIC - 69))
          | (1ULL << (sqlParser::NO - 69))
          | (1ULL << (sqlParser::NODE - 69))
          | (1ULL << (sqlParser::NOT - 69))
          | (1ULL << (sqlParser::SQL_NULL - 69))
          | (1ULL << (sqlParser::OFFSET - 69))
          | (1ULL << (sqlParser::OUTER - 69))
          | (1ULL << (sqlParser::OVERLAY - 69))
          | (1ULL << (sqlParser::PARTITION - 69))
          | (1ULL << (sqlParser::PRETTY - 69))
          | (1ULL << (sqlParser::REFERENCE - 69))
          | (1ULL << (sqlParser::RIGHT - 69))
          | (1ULL << (sqlParser::SCHEMA - 69))
          | (1ULL << (sqlParser::SET - 69))
          | (1ULL << (sqlParser::STAGE - 69))
          | (1ULL << (sqlParser::STATS - 69))
          | (1ULL << (sqlParser::STATISTICS - 69))
          | (1ULL << (sqlParser::SUM - 69))
          | (1ULL << (sqlParser::TABLE - 69))
          | (1ULL << (sqlParser::SQL_TRUE - 69))
          | (1ULL << (sqlParser::TYPE - 69))
          | (1ULL << (sqlParser::UNION - 69))
          | (1ULL << (sqlParser::UNIQUE - 69))
          | (1ULL << (sqlParser::UPDATE - 69))
          | (1ULL << (sqlParser::USING - 69))
          | (1ULL << (sqlParser::VALUES - 69))
          | (1ULL << (sqlParser::WITH_NAMES - 69))
          | (1ULL << (sqlParser::BOOLEAN - 69))
          | (1ULL << (sqlParser::INTEGER - 69))
          | (1ULL << (sqlParser::DECIMAL - 69))
          | (1ULL << (sqlParser::FLOAT - 69))
          | (1ULL << (sqlParser::TIME - 69))
          | (1ULL << (sqlParser::TIMESTAMP - 69))
          | (1ULL << (sqlParser::INTERVAL - 69))
          | (1ULL << (sqlParser::CHAR - 69))
          | (1ULL << (sqlParser::CHARACTER - 69))
          | (1ULL << (sqlParser::BINARY - 69)))) != 0) || ((((_la - 133) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 133)) & ((1ULL << (sqlParser::VARBINARY - 133))
          | (1ULL << (sqlParser::VARYING - 133))
          | (1ULL << (sqlParser::BIT - 133))
          | (1ULL << (sqlParser::BITS - 133))
          | (1ULL << (sqlParser::BYTE - 133))
          | (1ULL << (sqlParser::BYTES - 133))
          | (1ULL << (sqlParser::CHARS - 133))
          | (1ULL << (sqlParser::DIGITS - 133))
          | (1ULL << (sqlParser::CASE - 133))
          | (1ULL << (sqlParser::CAST - 133))
          | (1ULL << (sqlParser::COALESCE - 133))
          | (1ULL << (sqlParser::CONCAT - 133))
          | (1ULL << (sqlParser::COUNT - 133))
          | (1ULL << (sqlParser::DECODE - 133))
          | (1ULL << (sqlParser::EXTRACT - 133))
          | (1ULL << (sqlParser::EXISTS - 133))
          | (1ULL << (sqlParser::IF - 133))
          | (1ULL << (sqlParser::IFNULL - 133))
          | (1ULL << (sqlParser::NULLIF - 133))
          | (1ULL << (sqlParser::POSITION - 133))
          | (1ULL << (sqlParser::SUBSTR - 133))
          | (1ULL << (sqlParser::SUBSTRING - 133))
          | (1ULL << (sqlParser::TRIM - 133))
          | (1ULL << (sqlParser::TYPEOF - 133))
          | (1ULL << (sqlParser::DAY - 133))
          | (1ULL << (sqlParser::DAYS - 133))
          | (1ULL << (sqlParser::HOUR - 133))
          | (1ULL << (sqlParser::HOURS - 133))
          | (1ULL << (sqlParser::MINUTE - 133))
          | (1ULL << (sqlParser::MINUTES - 133))
          | (1ULL << (sqlParser::MONTH - 133))
          | (1ULL << (sqlParser::MONTHS - 133))
          | (1ULL << (sqlParser::QUARTER - 133))
          | (1ULL << (sqlParser::QUARTERS - 133))
          | (1ULL << (sqlParser::SECOND - 133))
          | (1ULL << (sqlParser::SECONDS - 133))
          | (1ULL << (sqlParser::WEEK - 133))
          | (1ULL << (sqlParser::WEEKS - 133))
          | (1ULL << (sqlParser::YEAR - 133))
          | (1ULL << (sqlParser::YEARS - 133))
          | (1ULL << (sqlParser::BLOCK - 133))
          | (1ULL << (sqlParser::BLOOM - 133))
          | (1ULL << (sqlParser::BTREE - 133))
          | (1ULL << (sqlParser::HASH - 133))
          | (1ULL << (sqlParser::LOOP - 133))
          | (1ULL << (sqlParser::SKIPLIST - 133))
          | (1ULL << (sqlParser::TREE - 133))
          | (1ULL << (sqlParser::VALUE - 133))
          | (1ULL << (sqlParser::IDENTIFIER - 133))
          | (1ULL << (sqlParser::STRING_LITERAL - 133))
          | (1ULL << (sqlParser::FLOAT_LITERAL - 133))
          | (1ULL << (sqlParser::DECIMAL_LITERAL - 133))
          | (1ULL << (sqlParser::INTEGER_LITERAL - 133)))) != 0)) {
          setState(1306);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        setState(1309);
        match(sqlParser::FROM);
        setState(1310);
        expr(0);
        break;
      }

      }
      setState(1313);
      match(sqlParser::T__7);
      break;
    }

    case 19: {
      _localctx = _tracker.createInstance<ExtractFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1315);
      match(sqlParser::EXTRACT);
      setState(1316);
      match(sqlParser::T__6);
      setState(1317);
      _la = _input->LA(1);
      if (!(((((_la - 162) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 162)) & ((1ULL << (sqlParser::DAY - 162))
        | (1ULL << (sqlParser::HOUR - 162))
        | (1ULL << (sqlParser::MINUTE - 162))
        | (1ULL << (sqlParser::MONTH - 162))
        | (1ULL << (sqlParser::SECOND - 162))
        | (1ULL << (sqlParser::YEAR - 162)))) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1318);
      match(sqlParser::FROM);
      setState(1319);
      expr(0);
      setState(1320);
      match(sqlParser::T__7);
      break;
    }

    case 20: {
      _localctx = _tracker.createInstance<CountAllContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1322);
      match(sqlParser::COUNT);
      setState(1323);
      match(sqlParser::T__6);
      setState(1325);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ALL) {
        setState(1324);
        match(sqlParser::ALL);
      }
      setState(1327);
      match(sqlParser::T__4);
      setState(1328);
      match(sqlParser::T__7);
      break;
    }

    case 21: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1329);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::COUNT);
      setState(1330);
      match(sqlParser::T__6);
      setState(1333);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1331);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1332);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__3:
        case sqlParser::T__6:
        case sqlParser::T__10:
        case sqlParser::ANTLR:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BOTH:
        case sqlParser::BY:
        case sqlParser::CATALOG:
        case sqlParser::CORRESPONDING:
        case sqlParser::CREATE:
        case sqlParser::CURRENT_CATALOG:
        case sqlParser::CURRENT_SCHEMA:
        case sqlParser::CURRENT_TIMESTAMP:
        case sqlParser::CURRENT_USER:
        case sqlParser::DATA:
        case sqlParser::DATE:
        case sqlParser::DELETE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DROP:
        case sqlParser::ESCAPE:
        case sqlParser::EXCEPT:
        case sqlParser::EXPLAIN:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FORMAT:
        case sqlParser::INDEX:
        case sqlParser::INSERT:
        case sqlParser::INTO:
        case sqlParser::IS:
        case sqlParser::JOIN:
        case sqlParser::KEY:
        case sqlParser::LEFT:
        case sqlParser::MAX:
        case sqlParser::MIN:
        case sqlParser::MONOTONIC:
        case sqlParser::NO:
        case sqlParser::NODE:
        case sqlParser::NOT:
        case sqlParser::SQL_NULL:
        case sqlParser::OFFSET:
        case sqlParser::OUTER:
        case sqlParser::OVERLAY:
        case sqlParser::PARTITION:
        case sqlParser::PRETTY:
        case sqlParser::REFERENCE:
        case sqlParser::RIGHT:
        case sqlParser::SCHEMA:
        case sqlParser::SET:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::SUM:
        case sqlParser::TABLE:
        case sqlParser::SQL_TRUE:
        case sqlParser::TYPE:
        case sqlParser::UNION:
        case sqlParser::UNIQUE:
        case sqlParser::UPDATE:
        case sqlParser::USING:
        case sqlParser::VALUES:
        case sqlParser::WITH_NAMES:
        case sqlParser::BOOLEAN:
        case sqlParser::INTEGER:
        case sqlParser::DECIMAL:
        case sqlParser::FLOAT:
        case sqlParser::TIME:
        case sqlParser::TIMESTAMP:
        case sqlParser::INTERVAL:
        case sqlParser::CHAR:
        case sqlParser::CHARACTER:
        case sqlParser::BINARY:
        case sqlParser::VARBINARY:
        case sqlParser::VARYING:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CHARS:
        case sqlParser::DIGITS:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::COALESCE:
        case sqlParser::CONCAT:
        case sqlParser::COUNT:
        case sqlParser::DECODE:
        case sqlParser::EXTRACT:
        case sqlParser::EXISTS:
        case sqlParser::IF:
        case sqlParser::IFNULL:
        case sqlParser::NULLIF:
        case sqlParser::POSITION:
        case sqlParser::SUBSTR:
        case sqlParser::SUBSTRING:
        case sqlParser::TRIM:
        case sqlParser::TYPEOF:
        case sqlParser::DAY:
        case sqlParser::DAYS:
        case sqlParser::HOUR:
        case sqlParser::HOURS:
        case sqlParser::MINUTE:
        case sqlParser::MINUTES:
        case sqlParser::MONTH:
        case sqlParser::MONTHS:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::SECOND:
        case sqlParser::SECONDS:
        case sqlParser::WEEK:
        case sqlParser::WEEKS:
        case sqlParser::YEAR:
        case sqlParser::YEARS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BTREE:
        case sqlParser::HASH:
        case sqlParser::LOOP:
        case sqlParser::SKIPLIST:
        case sqlParser::TREE:
        case sqlParser::VALUE:
        case sqlParser::IDENTIFIER:
        case sqlParser::STRING_LITERAL:
        case sqlParser::FLOAT_LITERAL:
        case sqlParser::DECIMAL_LITERAL:
        case sqlParser::INTEGER_LITERAL: {
          break;
        }

      default:
        break;
      }
      setState(1335);
      expr(0);
      setState(1336);
      match(sqlParser::T__7);
      break;
    }

    case 22: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1338);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::SUM);
      setState(1339);
      match(sqlParser::T__6);
      setState(1342);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1340);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1341);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__3:
        case sqlParser::T__6:
        case sqlParser::T__10:
        case sqlParser::ANTLR:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BOTH:
        case sqlParser::BY:
        case sqlParser::CATALOG:
        case sqlParser::CORRESPONDING:
        case sqlParser::CREATE:
        case sqlParser::CURRENT_CATALOG:
        case sqlParser::CURRENT_SCHEMA:
        case sqlParser::CURRENT_TIMESTAMP:
        case sqlParser::CURRENT_USER:
        case sqlParser::DATA:
        case sqlParser::DATE:
        case sqlParser::DELETE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DROP:
        case sqlParser::ESCAPE:
        case sqlParser::EXCEPT:
        case sqlParser::EXPLAIN:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FORMAT:
        case sqlParser::INDEX:
        case sqlParser::INSERT:
        case sqlParser::INTO:
        case sqlParser::IS:
        case sqlParser::JOIN:
        case sqlParser::KEY:
        case sqlParser::LEFT:
        case sqlParser::MAX:
        case sqlParser::MIN:
        case sqlParser::MONOTONIC:
        case sqlParser::NO:
        case sqlParser::NODE:
        case sqlParser::NOT:
        case sqlParser::SQL_NULL:
        case sqlParser::OFFSET:
        case sqlParser::OUTER:
        case sqlParser::OVERLAY:
        case sqlParser::PARTITION:
        case sqlParser::PRETTY:
        case sqlParser::REFERENCE:
        case sqlParser::RIGHT:
        case sqlParser::SCHEMA:
        case sqlParser::SET:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::SUM:
        case sqlParser::TABLE:
        case sqlParser::SQL_TRUE:
        case sqlParser::TYPE:
        case sqlParser::UNION:
        case sqlParser::UNIQUE:
        case sqlParser::UPDATE:
        case sqlParser::USING:
        case sqlParser::VALUES:
        case sqlParser::WITH_NAMES:
        case sqlParser::BOOLEAN:
        case sqlParser::INTEGER:
        case sqlParser::DECIMAL:
        case sqlParser::FLOAT:
        case sqlParser::TIME:
        case sqlParser::TIMESTAMP:
        case sqlParser::INTERVAL:
        case sqlParser::CHAR:
        case sqlParser::CHARACTER:
        case sqlParser::BINARY:
        case sqlParser::VARBINARY:
        case sqlParser::VARYING:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CHARS:
        case sqlParser::DIGITS:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::COALESCE:
        case sqlParser::CONCAT:
        case sqlParser::COUNT:
        case sqlParser::DECODE:
        case sqlParser::EXTRACT:
        case sqlParser::EXISTS:
        case sqlParser::IF:
        case sqlParser::IFNULL:
        case sqlParser::NULLIF:
        case sqlParser::POSITION:
        case sqlParser::SUBSTR:
        case sqlParser::SUBSTRING:
        case sqlParser::TRIM:
        case sqlParser::TYPEOF:
        case sqlParser::DAY:
        case sqlParser::DAYS:
        case sqlParser::HOUR:
        case sqlParser::HOURS:
        case sqlParser::MINUTE:
        case sqlParser::MINUTES:
        case sqlParser::MONTH:
        case sqlParser::MONTHS:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::SECOND:
        case sqlParser::SECONDS:
        case sqlParser::WEEK:
        case sqlParser::WEEKS:
        case sqlParser::YEAR:
        case sqlParser::YEARS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BTREE:
        case sqlParser::HASH:
        case sqlParser::LOOP:
        case sqlParser::SKIPLIST:
        case sqlParser::TREE:
        case sqlParser::VALUE:
        case sqlParser::IDENTIFIER:
        case sqlParser::STRING_LITERAL:
        case sqlParser::FLOAT_LITERAL:
        case sqlParser::DECIMAL_LITERAL:
        case sqlParser::INTEGER_LITERAL: {
          break;
        }

      default:
        break;
      }
      setState(1344);
      expr(0);
      setState(1345);
      match(sqlParser::T__7);
      break;
    }

    case 23: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1347);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::AVG);
      setState(1348);
      match(sqlParser::T__6);
      setState(1351);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1349);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1350);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__3:
        case sqlParser::T__6:
        case sqlParser::T__10:
        case sqlParser::ANTLR:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BOTH:
        case sqlParser::BY:
        case sqlParser::CATALOG:
        case sqlParser::CORRESPONDING:
        case sqlParser::CREATE:
        case sqlParser::CURRENT_CATALOG:
        case sqlParser::CURRENT_SCHEMA:
        case sqlParser::CURRENT_TIMESTAMP:
        case sqlParser::CURRENT_USER:
        case sqlParser::DATA:
        case sqlParser::DATE:
        case sqlParser::DELETE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DROP:
        case sqlParser::ESCAPE:
        case sqlParser::EXCEPT:
        case sqlParser::EXPLAIN:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FORMAT:
        case sqlParser::INDEX:
        case sqlParser::INSERT:
        case sqlParser::INTO:
        case sqlParser::IS:
        case sqlParser::JOIN:
        case sqlParser::KEY:
        case sqlParser::LEFT:
        case sqlParser::MAX:
        case sqlParser::MIN:
        case sqlParser::MONOTONIC:
        case sqlParser::NO:
        case sqlParser::NODE:
        case sqlParser::NOT:
        case sqlParser::SQL_NULL:
        case sqlParser::OFFSET:
        case sqlParser::OUTER:
        case sqlParser::OVERLAY:
        case sqlParser::PARTITION:
        case sqlParser::PRETTY:
        case sqlParser::REFERENCE:
        case sqlParser::RIGHT:
        case sqlParser::SCHEMA:
        case sqlParser::SET:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::SUM:
        case sqlParser::TABLE:
        case sqlParser::SQL_TRUE:
        case sqlParser::TYPE:
        case sqlParser::UNION:
        case sqlParser::UNIQUE:
        case sqlParser::UPDATE:
        case sqlParser::USING:
        case sqlParser::VALUES:
        case sqlParser::WITH_NAMES:
        case sqlParser::BOOLEAN:
        case sqlParser::INTEGER:
        case sqlParser::DECIMAL:
        case sqlParser::FLOAT:
        case sqlParser::TIME:
        case sqlParser::TIMESTAMP:
        case sqlParser::INTERVAL:
        case sqlParser::CHAR:
        case sqlParser::CHARACTER:
        case sqlParser::BINARY:
        case sqlParser::VARBINARY:
        case sqlParser::VARYING:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CHARS:
        case sqlParser::DIGITS:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::COALESCE:
        case sqlParser::CONCAT:
        case sqlParser::COUNT:
        case sqlParser::DECODE:
        case sqlParser::EXTRACT:
        case sqlParser::EXISTS:
        case sqlParser::IF:
        case sqlParser::IFNULL:
        case sqlParser::NULLIF:
        case sqlParser::POSITION:
        case sqlParser::SUBSTR:
        case sqlParser::SUBSTRING:
        case sqlParser::TRIM:
        case sqlParser::TYPEOF:
        case sqlParser::DAY:
        case sqlParser::DAYS:
        case sqlParser::HOUR:
        case sqlParser::HOURS:
        case sqlParser::MINUTE:
        case sqlParser::MINUTES:
        case sqlParser::MONTH:
        case sqlParser::MONTHS:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::SECOND:
        case sqlParser::SECONDS:
        case sqlParser::WEEK:
        case sqlParser::WEEKS:
        case sqlParser::YEAR:
        case sqlParser::YEARS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BTREE:
        case sqlParser::HASH:
        case sqlParser::LOOP:
        case sqlParser::SKIPLIST:
        case sqlParser::TREE:
        case sqlParser::VALUE:
        case sqlParser::IDENTIFIER:
        case sqlParser::STRING_LITERAL:
        case sqlParser::FLOAT_LITERAL:
        case sqlParser::DECIMAL_LITERAL:
        case sqlParser::INTEGER_LITERAL: {
          break;
        }

      default:
        break;
      }
      setState(1353);
      expr(0);
      setState(1354);
      match(sqlParser::T__7);
      break;
    }

    case 24: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1356);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MIN);
      setState(1357);
      match(sqlParser::T__6);
      setState(1360);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1358);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1359);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__3:
        case sqlParser::T__6:
        case sqlParser::T__10:
        case sqlParser::ANTLR:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BOTH:
        case sqlParser::BY:
        case sqlParser::CATALOG:
        case sqlParser::CORRESPONDING:
        case sqlParser::CREATE:
        case sqlParser::CURRENT_CATALOG:
        case sqlParser::CURRENT_SCHEMA:
        case sqlParser::CURRENT_TIMESTAMP:
        case sqlParser::CURRENT_USER:
        case sqlParser::DATA:
        case sqlParser::DATE:
        case sqlParser::DELETE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DROP:
        case sqlParser::ESCAPE:
        case sqlParser::EXCEPT:
        case sqlParser::EXPLAIN:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FORMAT:
        case sqlParser::INDEX:
        case sqlParser::INSERT:
        case sqlParser::INTO:
        case sqlParser::IS:
        case sqlParser::JOIN:
        case sqlParser::KEY:
        case sqlParser::LEFT:
        case sqlParser::MAX:
        case sqlParser::MIN:
        case sqlParser::MONOTONIC:
        case sqlParser::NO:
        case sqlParser::NODE:
        case sqlParser::NOT:
        case sqlParser::SQL_NULL:
        case sqlParser::OFFSET:
        case sqlParser::OUTER:
        case sqlParser::OVERLAY:
        case sqlParser::PARTITION:
        case sqlParser::PRETTY:
        case sqlParser::REFERENCE:
        case sqlParser::RIGHT:
        case sqlParser::SCHEMA:
        case sqlParser::SET:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::SUM:
        case sqlParser::TABLE:
        case sqlParser::SQL_TRUE:
        case sqlParser::TYPE:
        case sqlParser::UNION:
        case sqlParser::UNIQUE:
        case sqlParser::UPDATE:
        case sqlParser::USING:
        case sqlParser::VALUES:
        case sqlParser::WITH_NAMES:
        case sqlParser::BOOLEAN:
        case sqlParser::INTEGER:
        case sqlParser::DECIMAL:
        case sqlParser::FLOAT:
        case sqlParser::TIME:
        case sqlParser::TIMESTAMP:
        case sqlParser::INTERVAL:
        case sqlParser::CHAR:
        case sqlParser::CHARACTER:
        case sqlParser::BINARY:
        case sqlParser::VARBINARY:
        case sqlParser::VARYING:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CHARS:
        case sqlParser::DIGITS:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::COALESCE:
        case sqlParser::CONCAT:
        case sqlParser::COUNT:
        case sqlParser::DECODE:
        case sqlParser::EXTRACT:
        case sqlParser::EXISTS:
        case sqlParser::IF:
        case sqlParser::IFNULL:
        case sqlParser::NULLIF:
        case sqlParser::POSITION:
        case sqlParser::SUBSTR:
        case sqlParser::SUBSTRING:
        case sqlParser::TRIM:
        case sqlParser::TYPEOF:
        case sqlParser::DAY:
        case sqlParser::DAYS:
        case sqlParser::HOUR:
        case sqlParser::HOURS:
        case sqlParser::MINUTE:
        case sqlParser::MINUTES:
        case sqlParser::MONTH:
        case sqlParser::MONTHS:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::SECOND:
        case sqlParser::SECONDS:
        case sqlParser::WEEK:
        case sqlParser::WEEKS:
        case sqlParser::YEAR:
        case sqlParser::YEARS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BTREE:
        case sqlParser::HASH:
        case sqlParser::LOOP:
        case sqlParser::SKIPLIST:
        case sqlParser::TREE:
        case sqlParser::VALUE:
        case sqlParser::IDENTIFIER:
        case sqlParser::STRING_LITERAL:
        case sqlParser::FLOAT_LITERAL:
        case sqlParser::DECIMAL_LITERAL:
        case sqlParser::INTEGER_LITERAL: {
          break;
        }

      default:
        break;
      }
      setState(1362);
      expr(0);
      setState(1363);
      match(sqlParser::T__7);
      break;
    }

    case 25: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1365);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MAX);
      setState(1366);
      match(sqlParser::T__6);
      setState(1369);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1367);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1368);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__3:
        case sqlParser::T__6:
        case sqlParser::T__10:
        case sqlParser::ANTLR:
        case sqlParser::ASC:
        case sqlParser::AVG:
        case sqlParser::BOTH:
        case sqlParser::BY:
        case sqlParser::CATALOG:
        case sqlParser::CORRESPONDING:
        case sqlParser::CREATE:
        case sqlParser::CURRENT_CATALOG:
        case sqlParser::CURRENT_SCHEMA:
        case sqlParser::CURRENT_TIMESTAMP:
        case sqlParser::CURRENT_USER:
        case sqlParser::DATA:
        case sqlParser::DATE:
        case sqlParser::DELETE:
        case sqlParser::DESC:
        case sqlParser::DETAIL:
        case sqlParser::DROP:
        case sqlParser::ESCAPE:
        case sqlParser::EXCEPT:
        case sqlParser::EXPLAIN:
        case sqlParser::SQL_FALSE:
        case sqlParser::FEDERATION:
        case sqlParser::FORMAT:
        case sqlParser::INDEX:
        case sqlParser::INSERT:
        case sqlParser::INTO:
        case sqlParser::IS:
        case sqlParser::JOIN:
        case sqlParser::KEY:
        case sqlParser::LEFT:
        case sqlParser::MAX:
        case sqlParser::MIN:
        case sqlParser::MONOTONIC:
        case sqlParser::NO:
        case sqlParser::NODE:
        case sqlParser::NOT:
        case sqlParser::SQL_NULL:
        case sqlParser::OFFSET:
        case sqlParser::OUTER:
        case sqlParser::OVERLAY:
        case sqlParser::PARTITION:
        case sqlParser::PRETTY:
        case sqlParser::REFERENCE:
        case sqlParser::RIGHT:
        case sqlParser::SCHEMA:
        case sqlParser::SET:
        case sqlParser::STAGE:
        case sqlParser::STATS:
        case sqlParser::STATISTICS:
        case sqlParser::SUM:
        case sqlParser::TABLE:
        case sqlParser::SQL_TRUE:
        case sqlParser::TYPE:
        case sqlParser::UNION:
        case sqlParser::UNIQUE:
        case sqlParser::UPDATE:
        case sqlParser::USING:
        case sqlParser::VALUES:
        case sqlParser::WITH_NAMES:
        case sqlParser::BOOLEAN:
        case sqlParser::INTEGER:
        case sqlParser::DECIMAL:
        case sqlParser::FLOAT:
        case sqlParser::TIME:
        case sqlParser::TIMESTAMP:
        case sqlParser::INTERVAL:
        case sqlParser::CHAR:
        case sqlParser::CHARACTER:
        case sqlParser::BINARY:
        case sqlParser::VARBINARY:
        case sqlParser::VARYING:
        case sqlParser::BIT:
        case sqlParser::BITS:
        case sqlParser::BYTE:
        case sqlParser::BYTES:
        case sqlParser::CHARS:
        case sqlParser::DIGITS:
        case sqlParser::CASE:
        case sqlParser::CAST:
        case sqlParser::COALESCE:
        case sqlParser::CONCAT:
        case sqlParser::COUNT:
        case sqlParser::DECODE:
        case sqlParser::EXTRACT:
        case sqlParser::EXISTS:
        case sqlParser::IF:
        case sqlParser::IFNULL:
        case sqlParser::NULLIF:
        case sqlParser::POSITION:
        case sqlParser::SUBSTR:
        case sqlParser::SUBSTRING:
        case sqlParser::TRIM:
        case sqlParser::TYPEOF:
        case sqlParser::DAY:
        case sqlParser::DAYS:
        case sqlParser::HOUR:
        case sqlParser::HOURS:
        case sqlParser::MINUTE:
        case sqlParser::MINUTES:
        case sqlParser::MONTH:
        case sqlParser::MONTHS:
        case sqlParser::QUARTER:
        case sqlParser::QUARTERS:
        case sqlParser::SECOND:
        case sqlParser::SECONDS:
        case sqlParser::WEEK:
        case sqlParser::WEEKS:
        case sqlParser::YEAR:
        case sqlParser::YEARS:
        case sqlParser::BLOCK:
        case sqlParser::BLOOM:
        case sqlParser::BTREE:
        case sqlParser::HASH:
        case sqlParser::LOOP:
        case sqlParser::SKIPLIST:
        case sqlParser::TREE:
        case sqlParser::VALUE:
        case sqlParser::IDENTIFIER:
        case sqlParser::STRING_LITERAL:
        case sqlParser::FLOAT_LITERAL:
        case sqlParser::DECIMAL_LITERAL:
        case sqlParser::INTEGER_LITERAL: {
          break;
        }

      default:
        break;
      }
      setState(1371);
      expr(0);
      setState(1372);
      match(sqlParser::T__7);
      break;
    }

    case 26: {
      _localctx = _tracker.createInstance<ScalarFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1374);
      dynamic_cast<ScalarFuncContext *>(_localctx)->func = functionName();
      setState(1375);
      match(sqlParser::T__6);
      setState(1384);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (((((_la - 4) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 4)) & ((1ULL << (sqlParser::T__3 - 4))
        | (1ULL << (sqlParser::T__6 - 4))
        | (1ULL << (sqlParser::T__10 - 4))
        | (1ULL << (sqlParser::ANTLR - 4))
        | (1ULL << (sqlParser::ASC - 4))
        | (1ULL << (sqlParser::AVG - 4))
        | (1ULL << (sqlParser::BOTH - 4))
        | (1ULL << (sqlParser::BY - 4))
        | (1ULL << (sqlParser::CATALOG - 4))
        | (1ULL << (sqlParser::CORRESPONDING - 4))
        | (1ULL << (sqlParser::CREATE - 4))
        | (1ULL << (sqlParser::CURRENT_CATALOG - 4))
        | (1ULL << (sqlParser::CURRENT_SCHEMA - 4))
        | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 4))
        | (1ULL << (sqlParser::CURRENT_USER - 4))
        | (1ULL << (sqlParser::DATA - 4))
        | (1ULL << (sqlParser::DATE - 4))
        | (1ULL << (sqlParser::DELETE - 4))
        | (1ULL << (sqlParser::DESC - 4))
        | (1ULL << (sqlParser::DETAIL - 4))
        | (1ULL << (sqlParser::DROP - 4))
        | (1ULL << (sqlParser::ESCAPE - 4))
        | (1ULL << (sqlParser::EXCEPT - 4))
        | (1ULL << (sqlParser::EXPLAIN - 4))
        | (1ULL << (sqlParser::SQL_FALSE - 4))
        | (1ULL << (sqlParser::FEDERATION - 4))
        | (1ULL << (sqlParser::FORMAT - 4))
        | (1ULL << (sqlParser::INDEX - 4))
        | (1ULL << (sqlParser::INSERT - 4))
        | (1ULL << (sqlParser::INTO - 4))
        | (1ULL << (sqlParser::IS - 4))
        | (1ULL << (sqlParser::JOIN - 4))
        | (1ULL << (sqlParser::KEY - 4)))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::LEFT - 69))
        | (1ULL << (sqlParser::MAX - 69))
        | (1ULL << (sqlParser::MIN - 69))
        | (1ULL << (sqlParser::MONOTONIC - 69))
        | (1ULL << (sqlParser::NO - 69))
        | (1ULL << (sqlParser::NODE - 69))
        | (1ULL << (sqlParser::NOT - 69))
        | (1ULL << (sqlParser::SQL_NULL - 69))
        | (1ULL << (sqlParser::OFFSET - 69))
        | (1ULL << (sqlParser::OUTER - 69))
        | (1ULL << (sqlParser::OVERLAY - 69))
        | (1ULL << (sqlParser::PARTITION - 69))
        | (1ULL << (sqlParser::PRETTY - 69))
        | (1ULL << (sqlParser::REFERENCE - 69))
        | (1ULL << (sqlParser::RIGHT - 69))
        | (1ULL << (sqlParser::SCHEMA - 69))
        | (1ULL << (sqlParser::SET - 69))
        | (1ULL << (sqlParser::STAGE - 69))
        | (1ULL << (sqlParser::STATS - 69))
        | (1ULL << (sqlParser::STATISTICS - 69))
        | (1ULL << (sqlParser::SUM - 69))
        | (1ULL << (sqlParser::TABLE - 69))
        | (1ULL << (sqlParser::SQL_TRUE - 69))
        | (1ULL << (sqlParser::TYPE - 69))
        | (1ULL << (sqlParser::UNION - 69))
        | (1ULL << (sqlParser::UNIQUE - 69))
        | (1ULL << (sqlParser::UPDATE - 69))
        | (1ULL << (sqlParser::USING - 69))
        | (1ULL << (sqlParser::VALUES - 69))
        | (1ULL << (sqlParser::WITH_NAMES - 69))
        | (1ULL << (sqlParser::BOOLEAN - 69))
        | (1ULL << (sqlParser::INTEGER - 69))
        | (1ULL << (sqlParser::DECIMAL - 69))
        | (1ULL << (sqlParser::FLOAT - 69))
        | (1ULL << (sqlParser::TIME - 69))
        | (1ULL << (sqlParser::TIMESTAMP - 69))
        | (1ULL << (sqlParser::INTERVAL - 69))
        | (1ULL << (sqlParser::CHAR - 69))
        | (1ULL << (sqlParser::CHARACTER - 69))
        | (1ULL << (sqlParser::BINARY - 69)))) != 0) || ((((_la - 133) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 133)) & ((1ULL << (sqlParser::VARBINARY - 133))
        | (1ULL << (sqlParser::VARYING - 133))
        | (1ULL << (sqlParser::BIT - 133))
        | (1ULL << (sqlParser::BITS - 133))
        | (1ULL << (sqlParser::BYTE - 133))
        | (1ULL << (sqlParser::BYTES - 133))
        | (1ULL << (sqlParser::CHARS - 133))
        | (1ULL << (sqlParser::DIGITS - 133))
        | (1ULL << (sqlParser::CASE - 133))
        | (1ULL << (sqlParser::CAST - 133))
        | (1ULL << (sqlParser::COALESCE - 133))
        | (1ULL << (sqlParser::CONCAT - 133))
        | (1ULL << (sqlParser::COUNT - 133))
        | (1ULL << (sqlParser::DECODE - 133))
        | (1ULL << (sqlParser::EXTRACT - 133))
        | (1ULL << (sqlParser::EXISTS - 133))
        | (1ULL << (sqlParser::IF - 133))
        | (1ULL << (sqlParser::IFNULL - 133))
        | (1ULL << (sqlParser::NULLIF - 133))
        | (1ULL << (sqlParser::POSITION - 133))
        | (1ULL << (sqlParser::SUBSTR - 133))
        | (1ULL << (sqlParser::SUBSTRING - 133))
        | (1ULL << (sqlParser::TRIM - 133))
        | (1ULL << (sqlParser::TYPEOF - 133))
        | (1ULL << (sqlParser::DAY - 133))
        | (1ULL << (sqlParser::DAYS - 133))
        | (1ULL << (sqlParser::HOUR - 133))
        | (1ULL << (sqlParser::HOURS - 133))
        | (1ULL << (sqlParser::MINUTE - 133))
        | (1ULL << (sqlParser::MINUTES - 133))
        | (1ULL << (sqlParser::MONTH - 133))
        | (1ULL << (sqlParser::MONTHS - 133))
        | (1ULL << (sqlParser::QUARTER - 133))
        | (1ULL << (sqlParser::QUARTERS - 133))
        | (1ULL << (sqlParser::SECOND - 133))
        | (1ULL << (sqlParser::SECONDS - 133))
        | (1ULL << (sqlParser::WEEK - 133))
        | (1ULL << (sqlParser::WEEKS - 133))
        | (1ULL << (sqlParser::YEAR - 133))
        | (1ULL << (sqlParser::YEARS - 133))
        | (1ULL << (sqlParser::BLOCK - 133))
        | (1ULL << (sqlParser::BLOOM - 133))
        | (1ULL << (sqlParser::BTREE - 133))
        | (1ULL << (sqlParser::HASH - 133))
        | (1ULL << (sqlParser::LOOP - 133))
        | (1ULL << (sqlParser::SKIPLIST - 133))
        | (1ULL << (sqlParser::TREE - 133))
        | (1ULL << (sqlParser::VALUE - 133))
        | (1ULL << (sqlParser::IDENTIFIER - 133))
        | (1ULL << (sqlParser::STRING_LITERAL - 133))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 133))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 133))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 133)))) != 0)) {
        setState(1376);
        expr(0);
        setState(1381);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1377);
          match(sqlParser::T__2);
          setState(1378);
          expr(0);
          setState(1383);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1386);
      match(sqlParser::T__7);
      break;
    }

    case 27: {
      _localctx = _tracker.createInstance<ParentheticContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1388);
      match(sqlParser::T__6);
      setState(1389);
      expr(0);
      setState(1390);
      match(sqlParser::T__7);
      break;
    }

    case 28: {
      _localctx = _tracker.createInstance<SubqueryExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1392);
      match(sqlParser::T__6);
      setState(1393);
      query();
      setState(1394);
      match(sqlParser::T__7);
      break;
    }

    case 29: {
      _localctx = _tracker.createInstance<SubqueryExistsOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1397);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::NOT) {
        setState(1396);
        match(sqlParser::NOT);
      }
      setState(1399);
      match(sqlParser::EXISTS);

      setState(1400);
      match(sqlParser::T__6);
      setState(1401);
      query();
      setState(1402);
      match(sqlParser::T__7);
      break;
    }

    case 30: {
      _localctx = _tracker.createInstance<LogicOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1404);
      dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::NOT);
      setState(1405);
      expr(4);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(1501);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 204, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(1499);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 203, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1408);

          if (!(precpred(_ctx, 37))) throw FailedPredicateException(this, "precpred(_ctx, 37)");
          setState(1409);
          dynamic_cast<BinaryOpContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__4)
            | (1ULL << sqlParser::T__11)
            | (1ULL << sqlParser::T__12))) != 0))) {
            dynamic_cast<BinaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1410);
          expr(38);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1411);

          if (!(precpred(_ctx, 36))) throw FailedPredicateException(this, "precpred(_ctx, 36)");
          setState(1412);
          dynamic_cast<BinaryOpContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__3)
            | (1ULL << sqlParser::T__10)
            | (1ULL << sqlParser::T__13))) != 0))) {
            dynamic_cast<BinaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1413);
          expr(37);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1414);

          if (!(precpred(_ctx, 35))) throw FailedPredicateException(this, "precpred(_ctx, 35)");
          setState(1415);
          dynamic_cast<BinaryOpContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__5)
            | (1ULL << sqlParser::T__14)
            | (1ULL << sqlParser::T__15)
            | (1ULL << sqlParser::T__16)
            | (1ULL << sqlParser::T__17)
            | (1ULL << sqlParser::T__18)
            | (1ULL << sqlParser::T__19))) != 0))) {
            dynamic_cast<BinaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1416);
          expr(36);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<LikeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1417);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(1419);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1418);
            match(sqlParser::NOT);
          }
          setState(1421);
          match(sqlParser::LIKE);
          setState(1422);
          expr(11);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<LikeEscapeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1423);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(1425);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1424);
            match(sqlParser::NOT);
          }
          setState(1427);
          match(sqlParser::LIKE);
          setState(1428);
          expr(0);
          setState(1429);
          match(sqlParser::ESCAPE);
          setState(1430);
          expr(10);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1432);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(1433);

          if (!(!(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent)))) throw FailedPredicateException(this, "!(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent))");
          setState(1434);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::AND);
          setState(1435);
          expr(4);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1436);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(1437);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::OR);
          setState(1438);
          expr(3);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1439);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(1440);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::XOR);
          setState(1441);
          expr(2);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<MethodCallContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1442);

          if (!(precpred(_ctx, 38))) throw FailedPredicateException(this, "precpred(_ctx, 38)");
          setState(1443);
          match(sqlParser::T__9);
          setState(1444);
          dynamic_cast<MethodCallContext *>(_localctx)->func = methodName();
          setState(1445);
          match(sqlParser::T__6);
          setState(1454);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 4) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 4)) & ((1ULL << (sqlParser::T__3 - 4))
            | (1ULL << (sqlParser::T__6 - 4))
            | (1ULL << (sqlParser::T__10 - 4))
            | (1ULL << (sqlParser::ANTLR - 4))
            | (1ULL << (sqlParser::ASC - 4))
            | (1ULL << (sqlParser::AVG - 4))
            | (1ULL << (sqlParser::BOTH - 4))
            | (1ULL << (sqlParser::BY - 4))
            | (1ULL << (sqlParser::CATALOG - 4))
            | (1ULL << (sqlParser::CORRESPONDING - 4))
            | (1ULL << (sqlParser::CREATE - 4))
            | (1ULL << (sqlParser::CURRENT_CATALOG - 4))
            | (1ULL << (sqlParser::CURRENT_SCHEMA - 4))
            | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 4))
            | (1ULL << (sqlParser::CURRENT_USER - 4))
            | (1ULL << (sqlParser::DATA - 4))
            | (1ULL << (sqlParser::DATE - 4))
            | (1ULL << (sqlParser::DELETE - 4))
            | (1ULL << (sqlParser::DESC - 4))
            | (1ULL << (sqlParser::DETAIL - 4))
            | (1ULL << (sqlParser::DROP - 4))
            | (1ULL << (sqlParser::ESCAPE - 4))
            | (1ULL << (sqlParser::EXCEPT - 4))
            | (1ULL << (sqlParser::EXPLAIN - 4))
            | (1ULL << (sqlParser::SQL_FALSE - 4))
            | (1ULL << (sqlParser::FEDERATION - 4))
            | (1ULL << (sqlParser::FORMAT - 4))
            | (1ULL << (sqlParser::INDEX - 4))
            | (1ULL << (sqlParser::INSERT - 4))
            | (1ULL << (sqlParser::INTO - 4))
            | (1ULL << (sqlParser::IS - 4))
            | (1ULL << (sqlParser::JOIN - 4))
            | (1ULL << (sqlParser::KEY - 4)))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::LEFT - 69))
            | (1ULL << (sqlParser::MAX - 69))
            | (1ULL << (sqlParser::MIN - 69))
            | (1ULL << (sqlParser::MONOTONIC - 69))
            | (1ULL << (sqlParser::NO - 69))
            | (1ULL << (sqlParser::NODE - 69))
            | (1ULL << (sqlParser::NOT - 69))
            | (1ULL << (sqlParser::SQL_NULL - 69))
            | (1ULL << (sqlParser::OFFSET - 69))
            | (1ULL << (sqlParser::OUTER - 69))
            | (1ULL << (sqlParser::OVERLAY - 69))
            | (1ULL << (sqlParser::PARTITION - 69))
            | (1ULL << (sqlParser::PRETTY - 69))
            | (1ULL << (sqlParser::REFERENCE - 69))
            | (1ULL << (sqlParser::RIGHT - 69))
            | (1ULL << (sqlParser::SCHEMA - 69))
            | (1ULL << (sqlParser::SET - 69))
            | (1ULL << (sqlParser::STAGE - 69))
            | (1ULL << (sqlParser::STATS - 69))
            | (1ULL << (sqlParser::STATISTICS - 69))
            | (1ULL << (sqlParser::SUM - 69))
            | (1ULL << (sqlParser::TABLE - 69))
            | (1ULL << (sqlParser::SQL_TRUE - 69))
            | (1ULL << (sqlParser::TYPE - 69))
            | (1ULL << (sqlParser::UNION - 69))
            | (1ULL << (sqlParser::UNIQUE - 69))
            | (1ULL << (sqlParser::UPDATE - 69))
            | (1ULL << (sqlParser::USING - 69))
            | (1ULL << (sqlParser::VALUES - 69))
            | (1ULL << (sqlParser::WITH_NAMES - 69))
            | (1ULL << (sqlParser::BOOLEAN - 69))
            | (1ULL << (sqlParser::INTEGER - 69))
            | (1ULL << (sqlParser::DECIMAL - 69))
            | (1ULL << (sqlParser::FLOAT - 69))
            | (1ULL << (sqlParser::TIME - 69))
            | (1ULL << (sqlParser::TIMESTAMP - 69))
            | (1ULL << (sqlParser::INTERVAL - 69))
            | (1ULL << (sqlParser::CHAR - 69))
            | (1ULL << (sqlParser::CHARACTER - 69))
            | (1ULL << (sqlParser::BINARY - 69)))) != 0) || ((((_la - 133) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 133)) & ((1ULL << (sqlParser::VARBINARY - 133))
            | (1ULL << (sqlParser::VARYING - 133))
            | (1ULL << (sqlParser::BIT - 133))
            | (1ULL << (sqlParser::BITS - 133))
            | (1ULL << (sqlParser::BYTE - 133))
            | (1ULL << (sqlParser::BYTES - 133))
            | (1ULL << (sqlParser::CHARS - 133))
            | (1ULL << (sqlParser::DIGITS - 133))
            | (1ULL << (sqlParser::CASE - 133))
            | (1ULL << (sqlParser::CAST - 133))
            | (1ULL << (sqlParser::COALESCE - 133))
            | (1ULL << (sqlParser::CONCAT - 133))
            | (1ULL << (sqlParser::COUNT - 133))
            | (1ULL << (sqlParser::DECODE - 133))
            | (1ULL << (sqlParser::EXTRACT - 133))
            | (1ULL << (sqlParser::EXISTS - 133))
            | (1ULL << (sqlParser::IF - 133))
            | (1ULL << (sqlParser::IFNULL - 133))
            | (1ULL << (sqlParser::NULLIF - 133))
            | (1ULL << (sqlParser::POSITION - 133))
            | (1ULL << (sqlParser::SUBSTR - 133))
            | (1ULL << (sqlParser::SUBSTRING - 133))
            | (1ULL << (sqlParser::TRIM - 133))
            | (1ULL << (sqlParser::TYPEOF - 133))
            | (1ULL << (sqlParser::DAY - 133))
            | (1ULL << (sqlParser::DAYS - 133))
            | (1ULL << (sqlParser::HOUR - 133))
            | (1ULL << (sqlParser::HOURS - 133))
            | (1ULL << (sqlParser::MINUTE - 133))
            | (1ULL << (sqlParser::MINUTES - 133))
            | (1ULL << (sqlParser::MONTH - 133))
            | (1ULL << (sqlParser::MONTHS - 133))
            | (1ULL << (sqlParser::QUARTER - 133))
            | (1ULL << (sqlParser::QUARTERS - 133))
            | (1ULL << (sqlParser::SECOND - 133))
            | (1ULL << (sqlParser::SECONDS - 133))
            | (1ULL << (sqlParser::WEEK - 133))
            | (1ULL << (sqlParser::WEEKS - 133))
            | (1ULL << (sqlParser::YEAR - 133))
            | (1ULL << (sqlParser::YEARS - 133))
            | (1ULL << (sqlParser::BLOCK - 133))
            | (1ULL << (sqlParser::BLOOM - 133))
            | (1ULL << (sqlParser::BTREE - 133))
            | (1ULL << (sqlParser::HASH - 133))
            | (1ULL << (sqlParser::LOOP - 133))
            | (1ULL << (sqlParser::SKIPLIST - 133))
            | (1ULL << (sqlParser::TREE - 133))
            | (1ULL << (sqlParser::VALUE - 133))
            | (1ULL << (sqlParser::IDENTIFIER - 133))
            | (1ULL << (sqlParser::STRING_LITERAL - 133))
            | (1ULL << (sqlParser::FLOAT_LITERAL - 133))
            | (1ULL << (sqlParser::DECIMAL_LITERAL - 133))
            | (1ULL << (sqlParser::INTEGER_LITERAL - 133)))) != 0)) {
            setState(1446);
            expr(0);
            setState(1451);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__2) {
              setState(1447);
              match(sqlParser::T__2);
              setState(1448);
              expr(0);
              setState(1453);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1456);
          match(sqlParser::T__7);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<IsNullOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1458);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(1459);
          match(sqlParser::IS);
          setState(1461);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1460);
            match(sqlParser::NOT);
          }
          setState(1463);
          match(sqlParser::SQL_NULL);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<SubqueryInOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1464);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(1466);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1465);
            match(sqlParser::NOT);
          }
          setState(1468);
          match(sqlParser::IN);

          setState(1469);
          match(sqlParser::T__6);
          setState(1470);
          query();
          setState(1471);
          match(sqlParser::T__7);
          break;
        }

        case 12: {
          auto newContext = _tracker.createInstance<InOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1473);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(1475);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1474);
            match(sqlParser::NOT);
          }
          setState(1477);
          match(sqlParser::IN);

          setState(1478);
          match(sqlParser::T__6);
          setState(1487);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 4) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 4)) & ((1ULL << (sqlParser::T__3 - 4))
            | (1ULL << (sqlParser::T__6 - 4))
            | (1ULL << (sqlParser::T__10 - 4))
            | (1ULL << (sqlParser::ANTLR - 4))
            | (1ULL << (sqlParser::ASC - 4))
            | (1ULL << (sqlParser::AVG - 4))
            | (1ULL << (sqlParser::BOTH - 4))
            | (1ULL << (sqlParser::BY - 4))
            | (1ULL << (sqlParser::CATALOG - 4))
            | (1ULL << (sqlParser::CORRESPONDING - 4))
            | (1ULL << (sqlParser::CREATE - 4))
            | (1ULL << (sqlParser::CURRENT_CATALOG - 4))
            | (1ULL << (sqlParser::CURRENT_SCHEMA - 4))
            | (1ULL << (sqlParser::CURRENT_TIMESTAMP - 4))
            | (1ULL << (sqlParser::CURRENT_USER - 4))
            | (1ULL << (sqlParser::DATA - 4))
            | (1ULL << (sqlParser::DATE - 4))
            | (1ULL << (sqlParser::DELETE - 4))
            | (1ULL << (sqlParser::DESC - 4))
            | (1ULL << (sqlParser::DETAIL - 4))
            | (1ULL << (sqlParser::DROP - 4))
            | (1ULL << (sqlParser::ESCAPE - 4))
            | (1ULL << (sqlParser::EXCEPT - 4))
            | (1ULL << (sqlParser::EXPLAIN - 4))
            | (1ULL << (sqlParser::SQL_FALSE - 4))
            | (1ULL << (sqlParser::FEDERATION - 4))
            | (1ULL << (sqlParser::FORMAT - 4))
            | (1ULL << (sqlParser::INDEX - 4))
            | (1ULL << (sqlParser::INSERT - 4))
            | (1ULL << (sqlParser::INTO - 4))
            | (1ULL << (sqlParser::IS - 4))
            | (1ULL << (sqlParser::JOIN - 4))
            | (1ULL << (sqlParser::KEY - 4)))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::LEFT - 69))
            | (1ULL << (sqlParser::MAX - 69))
            | (1ULL << (sqlParser::MIN - 69))
            | (1ULL << (sqlParser::MONOTONIC - 69))
            | (1ULL << (sqlParser::NO - 69))
            | (1ULL << (sqlParser::NODE - 69))
            | (1ULL << (sqlParser::NOT - 69))
            | (1ULL << (sqlParser::SQL_NULL - 69))
            | (1ULL << (sqlParser::OFFSET - 69))
            | (1ULL << (sqlParser::OUTER - 69))
            | (1ULL << (sqlParser::OVERLAY - 69))
            | (1ULL << (sqlParser::PARTITION - 69))
            | (1ULL << (sqlParser::PRETTY - 69))
            | (1ULL << (sqlParser::REFERENCE - 69))
            | (1ULL << (sqlParser::RIGHT - 69))
            | (1ULL << (sqlParser::SCHEMA - 69))
            | (1ULL << (sqlParser::SET - 69))
            | (1ULL << (sqlParser::STAGE - 69))
            | (1ULL << (sqlParser::STATS - 69))
            | (1ULL << (sqlParser::STATISTICS - 69))
            | (1ULL << (sqlParser::SUM - 69))
            | (1ULL << (sqlParser::TABLE - 69))
            | (1ULL << (sqlParser::SQL_TRUE - 69))
            | (1ULL << (sqlParser::TYPE - 69))
            | (1ULL << (sqlParser::UNION - 69))
            | (1ULL << (sqlParser::UNIQUE - 69))
            | (1ULL << (sqlParser::UPDATE - 69))
            | (1ULL << (sqlParser::USING - 69))
            | (1ULL << (sqlParser::VALUES - 69))
            | (1ULL << (sqlParser::WITH_NAMES - 69))
            | (1ULL << (sqlParser::BOOLEAN - 69))
            | (1ULL << (sqlParser::INTEGER - 69))
            | (1ULL << (sqlParser::DECIMAL - 69))
            | (1ULL << (sqlParser::FLOAT - 69))
            | (1ULL << (sqlParser::TIME - 69))
            | (1ULL << (sqlParser::TIMESTAMP - 69))
            | (1ULL << (sqlParser::INTERVAL - 69))
            | (1ULL << (sqlParser::CHAR - 69))
            | (1ULL << (sqlParser::CHARACTER - 69))
            | (1ULL << (sqlParser::BINARY - 69)))) != 0) || ((((_la - 133) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 133)) & ((1ULL << (sqlParser::VARBINARY - 133))
            | (1ULL << (sqlParser::VARYING - 133))
            | (1ULL << (sqlParser::BIT - 133))
            | (1ULL << (sqlParser::BITS - 133))
            | (1ULL << (sqlParser::BYTE - 133))
            | (1ULL << (sqlParser::BYTES - 133))
            | (1ULL << (sqlParser::CHARS - 133))
            | (1ULL << (sqlParser::DIGITS - 133))
            | (1ULL << (sqlParser::CASE - 133))
            | (1ULL << (sqlParser::CAST - 133))
            | (1ULL << (sqlParser::COALESCE - 133))
            | (1ULL << (sqlParser::CONCAT - 133))
            | (1ULL << (sqlParser::COUNT - 133))
            | (1ULL << (sqlParser::DECODE - 133))
            | (1ULL << (sqlParser::EXTRACT - 133))
            | (1ULL << (sqlParser::EXISTS - 133))
            | (1ULL << (sqlParser::IF - 133))
            | (1ULL << (sqlParser::IFNULL - 133))
            | (1ULL << (sqlParser::NULLIF - 133))
            | (1ULL << (sqlParser::POSITION - 133))
            | (1ULL << (sqlParser::SUBSTR - 133))
            | (1ULL << (sqlParser::SUBSTRING - 133))
            | (1ULL << (sqlParser::TRIM - 133))
            | (1ULL << (sqlParser::TYPEOF - 133))
            | (1ULL << (sqlParser::DAY - 133))
            | (1ULL << (sqlParser::DAYS - 133))
            | (1ULL << (sqlParser::HOUR - 133))
            | (1ULL << (sqlParser::HOURS - 133))
            | (1ULL << (sqlParser::MINUTE - 133))
            | (1ULL << (sqlParser::MINUTES - 133))
            | (1ULL << (sqlParser::MONTH - 133))
            | (1ULL << (sqlParser::MONTHS - 133))
            | (1ULL << (sqlParser::QUARTER - 133))
            | (1ULL << (sqlParser::QUARTERS - 133))
            | (1ULL << (sqlParser::SECOND - 133))
            | (1ULL << (sqlParser::SECONDS - 133))
            | (1ULL << (sqlParser::WEEK - 133))
            | (1ULL << (sqlParser::WEEKS - 133))
            | (1ULL << (sqlParser::YEAR - 133))
            | (1ULL << (sqlParser::YEARS - 133))
            | (1ULL << (sqlParser::BLOCK - 133))
            | (1ULL << (sqlParser::BLOOM - 133))
            | (1ULL << (sqlParser::BTREE - 133))
            | (1ULL << (sqlParser::HASH - 133))
            | (1ULL << (sqlParser::LOOP - 133))
            | (1ULL << (sqlParser::SKIPLIST - 133))
            | (1ULL << (sqlParser::TREE - 133))
            | (1ULL << (sqlParser::VALUE - 133))
            | (1ULL << (sqlParser::IDENTIFIER - 133))
            | (1ULL << (sqlParser::STRING_LITERAL - 133))
            | (1ULL << (sqlParser::FLOAT_LITERAL - 133))
            | (1ULL << (sqlParser::DECIMAL_LITERAL - 133))
            | (1ULL << (sqlParser::INTEGER_LITERAL - 133)))) != 0)) {
            setState(1479);
            expr(0);
            setState(1484);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__2) {
              setState(1480);
              match(sqlParser::T__2);
              setState(1481);
              expr(0);
              setState(1486);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1489);
          match(sqlParser::T__7);
          break;
        }

        case 13: {
          auto newContext = _tracker.createInstance<BetweenOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1490);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(1492);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1491);
            match(sqlParser::NOT);
          }
          setState(1494);
          match(sqlParser::BETWEEN);

          setState(1495);
          expr(0);
          setState(1496);
          match(sqlParser::AND);
          setState(1497);
          expr(0);
          break;
        }

        } 
      }
      setState(1503);
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
  enterRule(_localctx, 122, sqlParser::RuleVariableName);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1504);
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
  enterRule(_localctx, 124, sqlParser::RuleColumnRef);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1519);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 207, _ctx)) {
    case 1: {
      setState(1514);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 206, _ctx)) {
      case 1: {
        setState(1509);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 205, _ctx)) {
        case 1: {
          setState(1506);
          catalogName();
          setState(1507);
          match(sqlParser::T__9);
          break;
        }

        }

        setState(1511);
        schemaName();
        setState(1512);
        match(sqlParser::T__9);
        break;
      }

      }

      setState(1516);
      tableName();
      setState(1517);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(1521);
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
  enterRule(_localctx, 126, sqlParser::RuleColumnWildcard);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1536);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::ANTLR)
      | (1ULL << sqlParser::ASC)
      | (1ULL << sqlParser::BOTH)
      | (1ULL << sqlParser::BY)
      | (1ULL << sqlParser::CATALOG)
      | (1ULL << sqlParser::CORRESPONDING)
      | (1ULL << sqlParser::CREATE)
      | (1ULL << sqlParser::CURRENT_CATALOG)
      | (1ULL << sqlParser::CURRENT_SCHEMA)
      | (1ULL << sqlParser::CURRENT_TIMESTAMP)
      | (1ULL << sqlParser::CURRENT_USER)
      | (1ULL << sqlParser::DATA)
      | (1ULL << sqlParser::DATE)
      | (1ULL << sqlParser::DELETE)
      | (1ULL << sqlParser::DESC)
      | (1ULL << sqlParser::DETAIL)
      | (1ULL << sqlParser::DROP)
      | (1ULL << sqlParser::ESCAPE)
      | (1ULL << sqlParser::EXCEPT)
      | (1ULL << sqlParser::EXPLAIN)
      | (1ULL << sqlParser::FEDERATION)
      | (1ULL << sqlParser::FORMAT)
      | (1ULL << sqlParser::INDEX)
      | (1ULL << sqlParser::INSERT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::INTO - 64))
      | (1ULL << (sqlParser::IS - 64))
      | (1ULL << (sqlParser::JOIN - 64))
      | (1ULL << (sqlParser::KEY - 64))
      | (1ULL << (sqlParser::LEFT - 64))
      | (1ULL << (sqlParser::MONOTONIC - 64))
      | (1ULL << (sqlParser::NO - 64))
      | (1ULL << (sqlParser::NODE - 64))
      | (1ULL << (sqlParser::OFFSET - 64))
      | (1ULL << (sqlParser::OUTER - 64))
      | (1ULL << (sqlParser::OVERLAY - 64))
      | (1ULL << (sqlParser::PARTITION - 64))
      | (1ULL << (sqlParser::PRETTY - 64))
      | (1ULL << (sqlParser::REFERENCE - 64))
      | (1ULL << (sqlParser::RIGHT - 64))
      | (1ULL << (sqlParser::SCHEMA - 64))
      | (1ULL << (sqlParser::SET - 64))
      | (1ULL << (sqlParser::STAGE - 64))
      | (1ULL << (sqlParser::STATS - 64))
      | (1ULL << (sqlParser::STATISTICS - 64))
      | (1ULL << (sqlParser::TABLE - 64))
      | (1ULL << (sqlParser::TYPE - 64))
      | (1ULL << (sqlParser::UNION - 64))
      | (1ULL << (sqlParser::UNIQUE - 64))
      | (1ULL << (sqlParser::UPDATE - 64))
      | (1ULL << (sqlParser::USING - 64))
      | (1ULL << (sqlParser::VALUES - 64))
      | (1ULL << (sqlParser::WITH_NAMES - 64))
      | (1ULL << (sqlParser::BOOLEAN - 64))
      | (1ULL << (sqlParser::INTEGER - 64))
      | (1ULL << (sqlParser::DECIMAL - 64))
      | (1ULL << (sqlParser::FLOAT - 64))
      | (1ULL << (sqlParser::TIME - 64))
      | (1ULL << (sqlParser::TIMESTAMP - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::CHAR - 128))
      | (1ULL << (sqlParser::CHARACTER - 128))
      | (1ULL << (sqlParser::BINARY - 128))
      | (1ULL << (sqlParser::VARBINARY - 128))
      | (1ULL << (sqlParser::VARYING - 128))
      | (1ULL << (sqlParser::BIT - 128))
      | (1ULL << (sqlParser::BITS - 128))
      | (1ULL << (sqlParser::BYTE - 128))
      | (1ULL << (sqlParser::BYTES - 128))
      | (1ULL << (sqlParser::CHARS - 128))
      | (1ULL << (sqlParser::DIGITS - 128))
      | (1ULL << (sqlParser::COALESCE - 128))
      | (1ULL << (sqlParser::CONCAT - 128))
      | (1ULL << (sqlParser::COUNT - 128))
      | (1ULL << (sqlParser::DECODE - 128))
      | (1ULL << (sqlParser::EXTRACT - 128))
      | (1ULL << (sqlParser::IFNULL - 128))
      | (1ULL << (sqlParser::NULLIF - 128))
      | (1ULL << (sqlParser::POSITION - 128))
      | (1ULL << (sqlParser::SUBSTR - 128))
      | (1ULL << (sqlParser::SUBSTRING - 128))
      | (1ULL << (sqlParser::DAY - 128))
      | (1ULL << (sqlParser::DAYS - 128))
      | (1ULL << (sqlParser::HOUR - 128))
      | (1ULL << (sqlParser::HOURS - 128))
      | (1ULL << (sqlParser::MINUTE - 128))
      | (1ULL << (sqlParser::MINUTES - 128))
      | (1ULL << (sqlParser::MONTH - 128))
      | (1ULL << (sqlParser::MONTHS - 128))
      | (1ULL << (sqlParser::QUARTER - 128))
      | (1ULL << (sqlParser::QUARTERS - 128))
      | (1ULL << (sqlParser::SECOND - 128))
      | (1ULL << (sqlParser::SECONDS - 128))
      | (1ULL << (sqlParser::WEEK - 128))
      | (1ULL << (sqlParser::WEEKS - 128))
      | (1ULL << (sqlParser::YEAR - 128))
      | (1ULL << (sqlParser::YEARS - 128))
      | (1ULL << (sqlParser::BLOCK - 128))
      | (1ULL << (sqlParser::BLOOM - 128))
      | (1ULL << (sqlParser::BTREE - 128))
      | (1ULL << (sqlParser::HASH - 128))
      | (1ULL << (sqlParser::LOOP - 128))
      | (1ULL << (sqlParser::SKIPLIST - 128))
      | (1ULL << (sqlParser::TREE - 128))
      | (1ULL << (sqlParser::VALUE - 128))
      | (1ULL << (sqlParser::IDENTIFIER - 128)))) != 0)) {
      setState(1531);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 209, _ctx)) {
      case 1: {
        setState(1526);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 208, _ctx)) {
        case 1: {
          setState(1523);
          catalogName();
          setState(1524);
          match(sqlParser::T__9);
          break;
        }

        }

        setState(1528);
        schemaName();
        setState(1529);
        match(sqlParser::T__9);
        break;
      }

      }

      setState(1533);
      tableName();
      setState(1534);
      match(sqlParser::T__9);
    }
    setState(1538);
    match(sqlParser::T__4);
   
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
  enterRule(_localctx, 128, sqlParser::RuleClassName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1545);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 211, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1540);
        qualifier();
        setState(1541);
        match(sqlParser::T__9); 
      }
      setState(1547);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 211, _ctx);
    }
    setState(1548);
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
  enterRule(_localctx, 130, sqlParser::RuleFunctionName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1550);
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
  enterRule(_localctx, 132, sqlParser::RuleMethodName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1552);
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
  enterRule(_localctx, 134, sqlParser::RuleCatalogName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1554);
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
  enterRule(_localctx, 136, sqlParser::RuleSchemaName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1556);
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
  enterRule(_localctx, 138, sqlParser::RuleTableName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1558);
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
  enterRule(_localctx, 140, sqlParser::RuleTableAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1560);
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
  enterRule(_localctx, 142, sqlParser::RuleColumnName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1562);
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
  enterRule(_localctx, 144, sqlParser::RuleColumnAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1564);
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
  enterRule(_localctx, 146, sqlParser::RuleCollationName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1566);
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
  enterRule(_localctx, 148, sqlParser::RuleIndexName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1568);
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
  enterRule(_localctx, 150, sqlParser::RulePropertyName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1570);
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
  enterRule(_localctx, 152, sqlParser::RuleHintName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1572);
    dynamic_cast<HintNameContext *>(_localctx)->id = identifier();
   
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
  enterRule(_localctx, 154, sqlParser::RulePropertyValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1576);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__3:
      case sqlParser::T__10:
      case sqlParser::SQL_FALSE:
      case sqlParser::SQL_NULL:
      case sqlParser::SQL_TRUE:
      case sqlParser::STRING_LITERAL:
      case sqlParser::FLOAT_LITERAL:
      case sqlParser::DECIMAL_LITERAL:
      case sqlParser::INTEGER_LITERAL: {
        setState(1574);
        dynamic_cast<PropertyValueContext *>(_localctx)->lit = simpleLiteral();
        break;
      }

      case sqlParser::ANTLR:
      case sqlParser::ASC:
      case sqlParser::BOTH:
      case sqlParser::BY:
      case sqlParser::CATALOG:
      case sqlParser::CORRESPONDING:
      case sqlParser::CREATE:
      case sqlParser::CURRENT_CATALOG:
      case sqlParser::CURRENT_SCHEMA:
      case sqlParser::CURRENT_TIMESTAMP:
      case sqlParser::CURRENT_USER:
      case sqlParser::DATA:
      case sqlParser::DATE:
      case sqlParser::DELETE:
      case sqlParser::DESC:
      case sqlParser::DETAIL:
      case sqlParser::DROP:
      case sqlParser::ESCAPE:
      case sqlParser::EXCEPT:
      case sqlParser::EXPLAIN:
      case sqlParser::FEDERATION:
      case sqlParser::FORMAT:
      case sqlParser::INDEX:
      case sqlParser::INSERT:
      case sqlParser::INTO:
      case sqlParser::IS:
      case sqlParser::JOIN:
      case sqlParser::KEY:
      case sqlParser::LEFT:
      case sqlParser::MONOTONIC:
      case sqlParser::NO:
      case sqlParser::NODE:
      case sqlParser::OFFSET:
      case sqlParser::OUTER:
      case sqlParser::OVERLAY:
      case sqlParser::PARTITION:
      case sqlParser::PRETTY:
      case sqlParser::REFERENCE:
      case sqlParser::RIGHT:
      case sqlParser::SCHEMA:
      case sqlParser::SET:
      case sqlParser::STAGE:
      case sqlParser::STATS:
      case sqlParser::STATISTICS:
      case sqlParser::TABLE:
      case sqlParser::TYPE:
      case sqlParser::UNION:
      case sqlParser::UNIQUE:
      case sqlParser::UPDATE:
      case sqlParser::USING:
      case sqlParser::VALUES:
      case sqlParser::WITH_NAMES:
      case sqlParser::BOOLEAN:
      case sqlParser::INTEGER:
      case sqlParser::DECIMAL:
      case sqlParser::FLOAT:
      case sqlParser::TIME:
      case sqlParser::TIMESTAMP:
      case sqlParser::CHAR:
      case sqlParser::CHARACTER:
      case sqlParser::BINARY:
      case sqlParser::VARBINARY:
      case sqlParser::VARYING:
      case sqlParser::BIT:
      case sqlParser::BITS:
      case sqlParser::BYTE:
      case sqlParser::BYTES:
      case sqlParser::CHARS:
      case sqlParser::DIGITS:
      case sqlParser::COALESCE:
      case sqlParser::CONCAT:
      case sqlParser::COUNT:
      case sqlParser::DECODE:
      case sqlParser::EXTRACT:
      case sqlParser::IFNULL:
      case sqlParser::NULLIF:
      case sqlParser::POSITION:
      case sqlParser::SUBSTR:
      case sqlParser::SUBSTRING:
      case sqlParser::DAY:
      case sqlParser::DAYS:
      case sqlParser::HOUR:
      case sqlParser::HOURS:
      case sqlParser::MINUTE:
      case sqlParser::MINUTES:
      case sqlParser::MONTH:
      case sqlParser::MONTHS:
      case sqlParser::QUARTER:
      case sqlParser::QUARTERS:
      case sqlParser::SECOND:
      case sqlParser::SECONDS:
      case sqlParser::WEEK:
      case sqlParser::WEEKS:
      case sqlParser::YEAR:
      case sqlParser::YEARS:
      case sqlParser::BLOCK:
      case sqlParser::BLOOM:
      case sqlParser::BTREE:
      case sqlParser::HASH:
      case sqlParser::LOOP:
      case sqlParser::SKIPLIST:
      case sqlParser::TREE:
      case sqlParser::VALUE:
      case sqlParser::IDENTIFIER: {
        setState(1575);
        dynamic_cast<PropertyValueContext *>(_localctx)->id = identifier();
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
  enterRule(_localctx, 156, sqlParser::RuleHintValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1580);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__3:
      case sqlParser::T__10:
      case sqlParser::SQL_FALSE:
      case sqlParser::SQL_NULL:
      case sqlParser::SQL_TRUE:
      case sqlParser::STRING_LITERAL:
      case sqlParser::FLOAT_LITERAL:
      case sqlParser::DECIMAL_LITERAL:
      case sqlParser::INTEGER_LITERAL: {
        setState(1578);
        dynamic_cast<HintValueContext *>(_localctx)->lit = simpleLiteral();
        break;
      }

      case sqlParser::ANTLR:
      case sqlParser::ASC:
      case sqlParser::BOTH:
      case sqlParser::BY:
      case sqlParser::CATALOG:
      case sqlParser::CORRESPONDING:
      case sqlParser::CREATE:
      case sqlParser::CURRENT_CATALOG:
      case sqlParser::CURRENT_SCHEMA:
      case sqlParser::CURRENT_TIMESTAMP:
      case sqlParser::CURRENT_USER:
      case sqlParser::DATA:
      case sqlParser::DATE:
      case sqlParser::DELETE:
      case sqlParser::DESC:
      case sqlParser::DETAIL:
      case sqlParser::DROP:
      case sqlParser::ESCAPE:
      case sqlParser::EXCEPT:
      case sqlParser::EXPLAIN:
      case sqlParser::FEDERATION:
      case sqlParser::FORMAT:
      case sqlParser::INDEX:
      case sqlParser::INSERT:
      case sqlParser::INTO:
      case sqlParser::IS:
      case sqlParser::JOIN:
      case sqlParser::KEY:
      case sqlParser::LEFT:
      case sqlParser::MONOTONIC:
      case sqlParser::NO:
      case sqlParser::NODE:
      case sqlParser::OFFSET:
      case sqlParser::OUTER:
      case sqlParser::OVERLAY:
      case sqlParser::PARTITION:
      case sqlParser::PRETTY:
      case sqlParser::REFERENCE:
      case sqlParser::RIGHT:
      case sqlParser::SCHEMA:
      case sqlParser::SET:
      case sqlParser::STAGE:
      case sqlParser::STATS:
      case sqlParser::STATISTICS:
      case sqlParser::TABLE:
      case sqlParser::TYPE:
      case sqlParser::UNION:
      case sqlParser::UNIQUE:
      case sqlParser::UPDATE:
      case sqlParser::USING:
      case sqlParser::VALUES:
      case sqlParser::WITH_NAMES:
      case sqlParser::BOOLEAN:
      case sqlParser::INTEGER:
      case sqlParser::DECIMAL:
      case sqlParser::FLOAT:
      case sqlParser::TIME:
      case sqlParser::TIMESTAMP:
      case sqlParser::CHAR:
      case sqlParser::CHARACTER:
      case sqlParser::BINARY:
      case sqlParser::VARBINARY:
      case sqlParser::VARYING:
      case sqlParser::BIT:
      case sqlParser::BITS:
      case sqlParser::BYTE:
      case sqlParser::BYTES:
      case sqlParser::CHARS:
      case sqlParser::DIGITS:
      case sqlParser::COALESCE:
      case sqlParser::CONCAT:
      case sqlParser::COUNT:
      case sqlParser::DECODE:
      case sqlParser::EXTRACT:
      case sqlParser::IFNULL:
      case sqlParser::NULLIF:
      case sqlParser::POSITION:
      case sqlParser::SUBSTR:
      case sqlParser::SUBSTRING:
      case sqlParser::DAY:
      case sqlParser::DAYS:
      case sqlParser::HOUR:
      case sqlParser::HOURS:
      case sqlParser::MINUTE:
      case sqlParser::MINUTES:
      case sqlParser::MONTH:
      case sqlParser::MONTHS:
      case sqlParser::QUARTER:
      case sqlParser::QUARTERS:
      case sqlParser::SECOND:
      case sqlParser::SECONDS:
      case sqlParser::WEEK:
      case sqlParser::WEEKS:
      case sqlParser::YEAR:
      case sqlParser::YEARS:
      case sqlParser::BLOCK:
      case sqlParser::BLOOM:
      case sqlParser::BTREE:
      case sqlParser::HASH:
      case sqlParser::LOOP:
      case sqlParser::SKIPLIST:
      case sqlParser::TREE:
      case sqlParser::VALUE:
      case sqlParser::IDENTIFIER: {
        setState(1579);
        dynamic_cast<HintValueContext *>(_localctx)->id = identifier();
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

//----------------- IdentifierContext ------------------------------------------------------------------

sqlParser::IdentifierContext::IdentifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::IdentifierContext::IDENTIFIER() {
  return getToken(sqlParser::IDENTIFIER, 0);
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
  enterRule(_localctx, 158, sqlParser::RuleIdentifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1584);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(1582);
        match(sqlParser::IDENTIFIER);
        break;
      }

      case sqlParser::ANTLR:
      case sqlParser::ASC:
      case sqlParser::BOTH:
      case sqlParser::BY:
      case sqlParser::CATALOG:
      case sqlParser::CORRESPONDING:
      case sqlParser::CREATE:
      case sqlParser::CURRENT_CATALOG:
      case sqlParser::CURRENT_SCHEMA:
      case sqlParser::CURRENT_TIMESTAMP:
      case sqlParser::CURRENT_USER:
      case sqlParser::DATA:
      case sqlParser::DATE:
      case sqlParser::DELETE:
      case sqlParser::DESC:
      case sqlParser::DETAIL:
      case sqlParser::DROP:
      case sqlParser::ESCAPE:
      case sqlParser::EXCEPT:
      case sqlParser::EXPLAIN:
      case sqlParser::FEDERATION:
      case sqlParser::FORMAT:
      case sqlParser::INDEX:
      case sqlParser::INSERT:
      case sqlParser::INTO:
      case sqlParser::IS:
      case sqlParser::JOIN:
      case sqlParser::KEY:
      case sqlParser::LEFT:
      case sqlParser::MONOTONIC:
      case sqlParser::NO:
      case sqlParser::NODE:
      case sqlParser::OFFSET:
      case sqlParser::OUTER:
      case sqlParser::OVERLAY:
      case sqlParser::PARTITION:
      case sqlParser::PRETTY:
      case sqlParser::REFERENCE:
      case sqlParser::RIGHT:
      case sqlParser::SCHEMA:
      case sqlParser::SET:
      case sqlParser::STAGE:
      case sqlParser::STATS:
      case sqlParser::STATISTICS:
      case sqlParser::TABLE:
      case sqlParser::TYPE:
      case sqlParser::UNION:
      case sqlParser::UNIQUE:
      case sqlParser::UPDATE:
      case sqlParser::USING:
      case sqlParser::VALUES:
      case sqlParser::WITH_NAMES:
      case sqlParser::BOOLEAN:
      case sqlParser::INTEGER:
      case sqlParser::DECIMAL:
      case sqlParser::FLOAT:
      case sqlParser::TIME:
      case sqlParser::TIMESTAMP:
      case sqlParser::CHAR:
      case sqlParser::CHARACTER:
      case sqlParser::BINARY:
      case sqlParser::VARBINARY:
      case sqlParser::VARYING:
      case sqlParser::BIT:
      case sqlParser::BITS:
      case sqlParser::BYTE:
      case sqlParser::BYTES:
      case sqlParser::CHARS:
      case sqlParser::DIGITS:
      case sqlParser::COALESCE:
      case sqlParser::CONCAT:
      case sqlParser::COUNT:
      case sqlParser::DECODE:
      case sqlParser::EXTRACT:
      case sqlParser::IFNULL:
      case sqlParser::NULLIF:
      case sqlParser::POSITION:
      case sqlParser::SUBSTR:
      case sqlParser::SUBSTRING:
      case sqlParser::DAY:
      case sqlParser::DAYS:
      case sqlParser::HOUR:
      case sqlParser::HOURS:
      case sqlParser::MINUTE:
      case sqlParser::MINUTES:
      case sqlParser::MONTH:
      case sqlParser::MONTHS:
      case sqlParser::QUARTER:
      case sqlParser::QUARTERS:
      case sqlParser::SECOND:
      case sqlParser::SECONDS:
      case sqlParser::WEEK:
      case sqlParser::WEEKS:
      case sqlParser::YEAR:
      case sqlParser::YEARS:
      case sqlParser::BLOCK:
      case sqlParser::BLOOM:
      case sqlParser::BTREE:
      case sqlParser::HASH:
      case sqlParser::LOOP:
      case sqlParser::SKIPLIST:
      case sqlParser::TREE:
      case sqlParser::VALUE: {
        enterOuterAlt(_localctx, 2);
        setState(1583);
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
  enterRule(_localctx, 160, sqlParser::RuleQualifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1586);
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
  enterRule(_localctx, 162, sqlParser::RulePrecision);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1588);
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
  enterRule(_localctx, 164, sqlParser::RuleScale);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1590);
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
  enterRule(_localctx, 166, sqlParser::RuleSize);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1592);
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

sqlParser::IntegerLiteralContext* sqlParser::LiteralContext::integerLiteral() {
  return getRuleContext<sqlParser::IntegerLiteralContext>(0);
}

sqlParser::DecimalLiteralContext* sqlParser::LiteralContext::decimalLiteral() {
  return getRuleContext<sqlParser::DecimalLiteralContext>(0);
}

sqlParser::FloatLiteralContext* sqlParser::LiteralContext::floatLiteral() {
  return getRuleContext<sqlParser::FloatLiteralContext>(0);
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
  enterRule(_localctx, 168, sqlParser::RuleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1604);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::SQL_NULL: {
        enterOuterAlt(_localctx, 1);
        setState(1594);
        match(sqlParser::SQL_NULL);
        break;
      }

      case sqlParser::SQL_FALSE:
      case sqlParser::SQL_TRUE: {
        enterOuterAlt(_localctx, 2);
        setState(1595);
        booleanLiteral();
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(1596);
        integerLiteral();
        break;
      }

      case sqlParser::DECIMAL_LITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(1597);
        decimalLiteral();
        break;
      }

      case sqlParser::FLOAT_LITERAL: {
        enterOuterAlt(_localctx, 5);
        setState(1598);
        floatLiteral();
        break;
      }

      case sqlParser::DATE: {
        enterOuterAlt(_localctx, 6);
        setState(1599);
        dateLiteral();
        break;
      }

      case sqlParser::TIME: {
        enterOuterAlt(_localctx, 7);
        setState(1600);
        timeLiteral();
        break;
      }

      case sqlParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 8);
        setState(1601);
        stringLiteral();
        break;
      }

      case sqlParser::TIMESTAMP: {
        enterOuterAlt(_localctx, 9);
        setState(1602);
        timestampLiteral();
        break;
      }

      case sqlParser::INTERVAL: {
        enterOuterAlt(_localctx, 10);
        setState(1603);
        intervalLiteral();
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
  enterRule(_localctx, 170, sqlParser::RuleSimpleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1612);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 216, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1606);
      match(sqlParser::SQL_NULL);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1607);
      booleanLiteral();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1608);
      signedIntegerLiteral();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(1609);
      signedDecimalLiteral();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1610);
      signedFloatLiteral();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1611);
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
  enterRule(_localctx, 172, sqlParser::RuleBooleanLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1614);
    _la = _input->LA(1);
    if (!(_la == sqlParser::SQL_FALSE

    || _la == sqlParser::SQL_TRUE)) {
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
  enterRule(_localctx, 174, sqlParser::RuleIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1616);
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
  enterRule(_localctx, 176, sqlParser::RuleSignedIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1620);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__10: {
        setState(1618);
        match(sqlParser::T__10);
        break;
      }

      case sqlParser::T__3: {
        setState(1619);
        dynamic_cast<SignedIntegerLiteralContext *>(_localctx)->neg = match(sqlParser::T__3);
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1622);
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
  enterRule(_localctx, 178, sqlParser::RuleDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1624);
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
  enterRule(_localctx, 180, sqlParser::RuleSignedDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1628);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__10: {
        setState(1626);
        match(sqlParser::T__10);
        break;
      }

      case sqlParser::T__3: {
        setState(1627);
        dynamic_cast<SignedDecimalLiteralContext *>(_localctx)->neg = match(sqlParser::T__3);
        break;
      }

      case sqlParser::DECIMAL_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1630);
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
  enterRule(_localctx, 182, sqlParser::RuleFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1632);
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
  enterRule(_localctx, 184, sqlParser::RuleSignedFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1636);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__10: {
        setState(1634);
        match(sqlParser::T__10);
        break;
      }

      case sqlParser::T__3: {
        setState(1635);
        dynamic_cast<SignedFloatLiteralContext *>(_localctx)->neg = match(sqlParser::T__3);
        break;
      }

      case sqlParser::FLOAT_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1638);
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
  enterRule(_localctx, 186, sqlParser::RuleDateLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1640);
    match(sqlParser::DATE);
    setState(1641);
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
  enterRule(_localctx, 188, sqlParser::RuleTimeLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1643);
    match(sqlParser::TIME);
    setState(1644);
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
  enterRule(_localctx, 190, sqlParser::RuleTimestampLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1646);
    match(sqlParser::TIMESTAMP);
    setState(1647);
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

tree::TerminalNode* sqlParser::StringLiteralContext::STRING_LITERAL() {
  return getToken(sqlParser::STRING_LITERAL, 0);
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
  enterRule(_localctx, 192, sqlParser::RuleStringLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1649);
    match(sqlParser::STRING_LITERAL);
   
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

sqlParser::Sql92CalendarIntervalContext* sqlParser::IntervalLiteralContext::sql92CalendarInterval() {
  return getRuleContext<sqlParser::Sql92CalendarIntervalContext>(0);
}

sqlParser::Sql92TimeIntervalContext* sqlParser::IntervalLiteralContext::sql92TimeInterval() {
  return getRuleContext<sqlParser::Sql92TimeIntervalContext>(0);
}

sqlParser::LooseIntervalContext* sqlParser::IntervalLiteralContext::looseInterval() {
  return getRuleContext<sqlParser::LooseIntervalContext>(0);
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
  enterRule(_localctx, 194, sqlParser::RuleIntervalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1654);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 220, _ctx)) {
    case 1: {
      setState(1651);
      sql92CalendarInterval();
      break;
    }

    case 2: {
      setState(1652);
      sql92TimeInterval();
      break;
    }

    case 3: {
      setState(1653);
      looseInterval();
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

//----------------- LooseIntervalContext ------------------------------------------------------------------

sqlParser::LooseIntervalContext::LooseIntervalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::LooseIntervalContext::INTERVAL() {
  return getToken(sqlParser::INTERVAL, 0);
}

sqlParser::ExprContext* sqlParser::LooseIntervalContext::expr() {
  return getRuleContext<sqlParser::ExprContext>(0);
}


size_t sqlParser::LooseIntervalContext::getRuleIndex() const {
  return sqlParser::RuleLooseInterval;
}


antlrcpp::Any sqlParser::LooseIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitLooseInterval(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::LooseIntervalContext* sqlParser::looseInterval() {
  LooseIntervalContext *_localctx = _tracker.createInstance<LooseIntervalContext>(_ctx, getState());
  enterRule(_localctx, 196, sqlParser::RuleLooseInterval);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1656);
    match(sqlParser::INTERVAL);
    setState(1657);
    dynamic_cast<LooseIntervalContext *>(_localctx)->str = expr(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Sql92CalendarIntervalContext ------------------------------------------------------------------

sqlParser::Sql92CalendarIntervalContext::Sql92CalendarIntervalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::Sql92CalendarIntervalContext::INTERVAL() {
  return getToken(sqlParser::INTERVAL, 0);
}

sqlParser::StringLiteralContext* sqlParser::Sql92CalendarIntervalContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}

tree::TerminalNode* sqlParser::Sql92CalendarIntervalContext::TO() {
  return getToken(sqlParser::TO, 0);
}

tree::TerminalNode* sqlParser::Sql92CalendarIntervalContext::YEAR() {
  return getToken(sqlParser::YEAR, 0);
}

tree::TerminalNode* sqlParser::Sql92CalendarIntervalContext::MONTH() {
  return getToken(sqlParser::MONTH, 0);
}

sqlParser::PrecisionContext* sqlParser::Sql92CalendarIntervalContext::precision() {
  return getRuleContext<sqlParser::PrecisionContext>(0);
}


size_t sqlParser::Sql92CalendarIntervalContext::getRuleIndex() const {
  return sqlParser::RuleSql92CalendarInterval;
}


antlrcpp::Any sqlParser::Sql92CalendarIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSql92CalendarInterval(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::Sql92CalendarIntervalContext* sqlParser::sql92CalendarInterval() {
  Sql92CalendarIntervalContext *_localctx = _tracker.createInstance<Sql92CalendarIntervalContext>(_ctx, getState());
  enterRule(_localctx, 198, sqlParser::RuleSql92CalendarInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1659);
    match(sqlParser::INTERVAL);
    setState(1660);
    stringLiteral();
    setState(1684);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 224, _ctx)) {
    case 1: {
      setState(1661);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1666);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 221, _ctx)) {
      case 1: {
        setState(1662);
        match(sqlParser::T__6);
        setState(1663);
        precision();
        setState(1664);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1668);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1673);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1669);
        match(sqlParser::T__6);
        setState(1670);
        precision();
        setState(1671);
        match(sqlParser::T__7);
      }
      setState(1675);
      match(sqlParser::TO);
      setState(1676);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      break;
    }

    case 3: {
      setState(1677);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      setState(1682);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 223, _ctx)) {
      case 1: {
        setState(1678);
        match(sqlParser::T__6);
        setState(1679);
        precision();
        setState(1680);
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

//----------------- Sql92TimeIntervalContext ------------------------------------------------------------------

sqlParser::Sql92TimeIntervalContext::Sql92TimeIntervalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* sqlParser::Sql92TimeIntervalContext::INTERVAL() {
  return getToken(sqlParser::INTERVAL, 0);
}

sqlParser::StringLiteralContext* sqlParser::Sql92TimeIntervalContext::stringLiteral() {
  return getRuleContext<sqlParser::StringLiteralContext>(0);
}

tree::TerminalNode* sqlParser::Sql92TimeIntervalContext::TO() {
  return getToken(sqlParser::TO, 0);
}

tree::TerminalNode* sqlParser::Sql92TimeIntervalContext::DAY() {
  return getToken(sqlParser::DAY, 0);
}

tree::TerminalNode* sqlParser::Sql92TimeIntervalContext::HOUR() {
  return getToken(sqlParser::HOUR, 0);
}

tree::TerminalNode* sqlParser::Sql92TimeIntervalContext::MINUTE() {
  return getToken(sqlParser::MINUTE, 0);
}

tree::TerminalNode* sqlParser::Sql92TimeIntervalContext::SECOND() {
  return getToken(sqlParser::SECOND, 0);
}

sqlParser::PrecisionContext* sqlParser::Sql92TimeIntervalContext::precision() {
  return getRuleContext<sqlParser::PrecisionContext>(0);
}

sqlParser::ScaleContext* sqlParser::Sql92TimeIntervalContext::scale() {
  return getRuleContext<sqlParser::ScaleContext>(0);
}


size_t sqlParser::Sql92TimeIntervalContext::getRuleIndex() const {
  return sqlParser::RuleSql92TimeInterval;
}


antlrcpp::Any sqlParser::Sql92TimeIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<sqlVisitor*>(visitor))
    return parserVisitor->visitSql92TimeInterval(this);
  else
    return visitor->visitChildren(this);
}

sqlParser::Sql92TimeIntervalContext* sqlParser::sql92TimeInterval() {
  Sql92TimeIntervalContext *_localctx = _tracker.createInstance<Sql92TimeIntervalContext>(_ctx, getState());
  enterRule(_localctx, 200, sqlParser::RuleSql92TimeInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1686);
    match(sqlParser::INTERVAL);
    setState(1687);
    stringLiteral();
    setState(1772);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 238, _ctx)) {
    case 1: {
      setState(1688);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1693);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 225, _ctx)) {
      case 1: {
        setState(1689);
        match(sqlParser::T__6);
        setState(1690);
        precision();
        setState(1691);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1695);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1700);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1696);
        match(sqlParser::T__6);
        setState(1697);
        precision();
        setState(1698);
        match(sqlParser::T__7);
      }
      setState(1702);
      match(sqlParser::TO);
      setState(1712);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::HOUR: {
          setState(1703);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
          break;
        }

        case sqlParser::MINUTE: {
          setState(1704);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1705);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1710);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 227, _ctx)) {
          case 1: {
            setState(1706);
            match(sqlParser::T__6);
            setState(1707);
            scale();
            setState(1708);
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
      setState(1714);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1719);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1715);
        match(sqlParser::T__6);
        setState(1716);
        precision();
        setState(1717);
        match(sqlParser::T__7);
      }
      setState(1721);
      match(sqlParser::TO);
      setState(1730);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::MINUTE: {
          setState(1722);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1723);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1728);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 230, _ctx)) {
          case 1: {
            setState(1724);
            match(sqlParser::T__6);
            setState(1725);
            scale();
            setState(1726);
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
      setState(1732);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1737);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1733);
        match(sqlParser::T__6);
        setState(1734);
        precision();
        setState(1735);
        match(sqlParser::T__7);
      }
      setState(1739);
      match(sqlParser::TO);

      setState(1740);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1745);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 233, _ctx)) {
      case 1: {
        setState(1741);
        match(sqlParser::T__6);
        setState(1742);
        scale();
        setState(1743);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 5: {
      setState(1747);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1752);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 234, _ctx)) {
      case 1: {
        setState(1748);
        match(sqlParser::T__6);
        setState(1749);
        precision();
        setState(1750);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 6: {
      setState(1754);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1759);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 235, _ctx)) {
      case 1: {
        setState(1755);
        match(sqlParser::T__6);
        setState(1756);
        precision();
        setState(1757);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 7: {
      setState(1761);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1770);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 237, _ctx)) {
      case 1: {
        setState(1762);
        match(sqlParser::T__6);
        setState(1763);
        precision();
        setState(1766);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__2) {
          setState(1764);
          match(sqlParser::T__2);
          setState(1765);
          scale();
        }
        setState(1768);
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

tree::TerminalNode* sqlParser::NonReservedWordContext::ANTLR() {
  return getToken(sqlParser::ANTLR, 0);
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

tree::TerminalNode* sqlParser::NonReservedWordContext::BOTH() {
  return getToken(sqlParser::BOTH, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::BTREE() {
  return getToken(sqlParser::BTREE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::BY() {
  return getToken(sqlParser::BY, 0);
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

tree::TerminalNode* sqlParser::NonReservedWordContext::COALESCE() {
  return getToken(sqlParser::COALESCE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::CONCAT() {
  return getToken(sqlParser::CONCAT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::CORRESPONDING() {
  return getToken(sqlParser::CORRESPONDING, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::COUNT() {
  return getToken(sqlParser::COUNT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::CREATE() {
  return getToken(sqlParser::CREATE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::CURRENT_CATALOG() {
  return getToken(sqlParser::CURRENT_CATALOG, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::CURRENT_SCHEMA() {
  return getToken(sqlParser::CURRENT_SCHEMA, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::CURRENT_TIMESTAMP() {
  return getToken(sqlParser::CURRENT_TIMESTAMP, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::CURRENT_USER() {
  return getToken(sqlParser::CURRENT_USER, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DATA() {
  return getToken(sqlParser::DATA, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DATE() {
  return getToken(sqlParser::DATE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DAY() {
  return getToken(sqlParser::DAY, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DAYS() {
  return getToken(sqlParser::DAYS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DECODE() {
  return getToken(sqlParser::DECODE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DELETE() {
  return getToken(sqlParser::DELETE, 0);
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

tree::TerminalNode* sqlParser::NonReservedWordContext::DROP() {
  return getToken(sqlParser::DROP, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::ESCAPE() {
  return getToken(sqlParser::ESCAPE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::EXCEPT() {
  return getToken(sqlParser::EXCEPT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::EXPLAIN() {
  return getToken(sqlParser::EXPLAIN, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::EXTRACT() {
  return getToken(sqlParser::EXTRACT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::FEDERATION() {
  return getToken(sqlParser::FEDERATION, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::FORMAT() {
  return getToken(sqlParser::FORMAT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::HASH() {
  return getToken(sqlParser::HASH, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::HOUR() {
  return getToken(sqlParser::HOUR, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::HOURS() {
  return getToken(sqlParser::HOURS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::IFNULL() {
  return getToken(sqlParser::IFNULL, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::INDEX() {
  return getToken(sqlParser::INDEX, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::INSERT() {
  return getToken(sqlParser::INSERT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::INTO() {
  return getToken(sqlParser::INTO, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::IS() {
  return getToken(sqlParser::IS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::JOIN() {
  return getToken(sqlParser::JOIN, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::KEY() {
  return getToken(sqlParser::KEY, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::LEFT() {
  return getToken(sqlParser::LEFT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::LOOP() {
  return getToken(sqlParser::LOOP, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MINUTE() {
  return getToken(sqlParser::MINUTE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MINUTES() {
  return getToken(sqlParser::MINUTES, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MONOTONIC() {
  return getToken(sqlParser::MONOTONIC, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MONTH() {
  return getToken(sqlParser::MONTH, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::MONTHS() {
  return getToken(sqlParser::MONTHS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::NO() {
  return getToken(sqlParser::NO, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::NODE() {
  return getToken(sqlParser::NODE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::NULLIF() {
  return getToken(sqlParser::NULLIF, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::OFFSET() {
  return getToken(sqlParser::OFFSET, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::OUTER() {
  return getToken(sqlParser::OUTER, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::OVERLAY() {
  return getToken(sqlParser::OVERLAY, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::PARTITION() {
  return getToken(sqlParser::PARTITION, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::POSITION() {
  return getToken(sqlParser::POSITION, 0);
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

tree::TerminalNode* sqlParser::NonReservedWordContext::RIGHT() {
  return getToken(sqlParser::RIGHT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SCHEMA() {
  return getToken(sqlParser::SCHEMA, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SECOND() {
  return getToken(sqlParser::SECOND, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SECONDS() {
  return getToken(sqlParser::SECONDS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SET() {
  return getToken(sqlParser::SET, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SKIPLIST() {
  return getToken(sqlParser::SKIPLIST, 0);
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

tree::TerminalNode* sqlParser::NonReservedWordContext::SUBSTR() {
  return getToken(sqlParser::SUBSTR, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::SUBSTRING() {
  return getToken(sqlParser::SUBSTRING, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::TABLE() {
  return getToken(sqlParser::TABLE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::TIME() {
  return getToken(sqlParser::TIME, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::TREE() {
  return getToken(sqlParser::TREE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::TYPE() {
  return getToken(sqlParser::TYPE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::UNION() {
  return getToken(sqlParser::UNION, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::UNIQUE() {
  return getToken(sqlParser::UNIQUE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::UPDATE() {
  return getToken(sqlParser::UPDATE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::USING() {
  return getToken(sqlParser::USING, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::VALUE() {
  return getToken(sqlParser::VALUE, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::VALUES() {
  return getToken(sqlParser::VALUES, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::VARYING() {
  return getToken(sqlParser::VARYING, 0);
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

tree::TerminalNode* sqlParser::NonReservedWordContext::YEAR() {
  return getToken(sqlParser::YEAR, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::YEARS() {
  return getToken(sqlParser::YEARS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::INTEGER() {
  return getToken(sqlParser::INTEGER, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::DECIMAL() {
  return getToken(sqlParser::DECIMAL, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::FLOAT() {
  return getToken(sqlParser::FLOAT, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::TIMESTAMP() {
  return getToken(sqlParser::TIMESTAMP, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::CHARACTER() {
  return getToken(sqlParser::CHARACTER, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::CHAR() {
  return getToken(sqlParser::CHAR, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::BOOLEAN() {
  return getToken(sqlParser::BOOLEAN, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::BINARY() {
  return getToken(sqlParser::BINARY, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::VARBINARY() {
  return getToken(sqlParser::VARBINARY, 0);
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
  enterRule(_localctx, 202, sqlParser::RuleNonReservedWord);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1774);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << sqlParser::ANTLR)
      | (1ULL << sqlParser::ASC)
      | (1ULL << sqlParser::BOTH)
      | (1ULL << sqlParser::BY)
      | (1ULL << sqlParser::CATALOG)
      | (1ULL << sqlParser::CORRESPONDING)
      | (1ULL << sqlParser::CREATE)
      | (1ULL << sqlParser::CURRENT_CATALOG)
      | (1ULL << sqlParser::CURRENT_SCHEMA)
      | (1ULL << sqlParser::CURRENT_TIMESTAMP)
      | (1ULL << sqlParser::CURRENT_USER)
      | (1ULL << sqlParser::DATA)
      | (1ULL << sqlParser::DATE)
      | (1ULL << sqlParser::DELETE)
      | (1ULL << sqlParser::DESC)
      | (1ULL << sqlParser::DETAIL)
      | (1ULL << sqlParser::DROP)
      | (1ULL << sqlParser::ESCAPE)
      | (1ULL << sqlParser::EXCEPT)
      | (1ULL << sqlParser::EXPLAIN)
      | (1ULL << sqlParser::FEDERATION)
      | (1ULL << sqlParser::FORMAT)
      | (1ULL << sqlParser::INDEX)
      | (1ULL << sqlParser::INSERT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::INTO - 64))
      | (1ULL << (sqlParser::IS - 64))
      | (1ULL << (sqlParser::JOIN - 64))
      | (1ULL << (sqlParser::KEY - 64))
      | (1ULL << (sqlParser::LEFT - 64))
      | (1ULL << (sqlParser::MONOTONIC - 64))
      | (1ULL << (sqlParser::NO - 64))
      | (1ULL << (sqlParser::NODE - 64))
      | (1ULL << (sqlParser::OFFSET - 64))
      | (1ULL << (sqlParser::OUTER - 64))
      | (1ULL << (sqlParser::OVERLAY - 64))
      | (1ULL << (sqlParser::PARTITION - 64))
      | (1ULL << (sqlParser::PRETTY - 64))
      | (1ULL << (sqlParser::REFERENCE - 64))
      | (1ULL << (sqlParser::RIGHT - 64))
      | (1ULL << (sqlParser::SCHEMA - 64))
      | (1ULL << (sqlParser::SET - 64))
      | (1ULL << (sqlParser::STAGE - 64))
      | (1ULL << (sqlParser::STATS - 64))
      | (1ULL << (sqlParser::STATISTICS - 64))
      | (1ULL << (sqlParser::TABLE - 64))
      | (1ULL << (sqlParser::TYPE - 64))
      | (1ULL << (sqlParser::UNION - 64))
      | (1ULL << (sqlParser::UNIQUE - 64))
      | (1ULL << (sqlParser::UPDATE - 64))
      | (1ULL << (sqlParser::USING - 64))
      | (1ULL << (sqlParser::VALUES - 64))
      | (1ULL << (sqlParser::WITH_NAMES - 64))
      | (1ULL << (sqlParser::BOOLEAN - 64))
      | (1ULL << (sqlParser::INTEGER - 64))
      | (1ULL << (sqlParser::DECIMAL - 64))
      | (1ULL << (sqlParser::FLOAT - 64))
      | (1ULL << (sqlParser::TIME - 64))
      | (1ULL << (sqlParser::TIMESTAMP - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::CHAR - 128))
      | (1ULL << (sqlParser::CHARACTER - 128))
      | (1ULL << (sqlParser::BINARY - 128))
      | (1ULL << (sqlParser::VARBINARY - 128))
      | (1ULL << (sqlParser::VARYING - 128))
      | (1ULL << (sqlParser::BIT - 128))
      | (1ULL << (sqlParser::BITS - 128))
      | (1ULL << (sqlParser::BYTE - 128))
      | (1ULL << (sqlParser::BYTES - 128))
      | (1ULL << (sqlParser::CHARS - 128))
      | (1ULL << (sqlParser::DIGITS - 128))
      | (1ULL << (sqlParser::COALESCE - 128))
      | (1ULL << (sqlParser::CONCAT - 128))
      | (1ULL << (sqlParser::COUNT - 128))
      | (1ULL << (sqlParser::DECODE - 128))
      | (1ULL << (sqlParser::EXTRACT - 128))
      | (1ULL << (sqlParser::IFNULL - 128))
      | (1ULL << (sqlParser::NULLIF - 128))
      | (1ULL << (sqlParser::POSITION - 128))
      | (1ULL << (sqlParser::SUBSTR - 128))
      | (1ULL << (sqlParser::SUBSTRING - 128))
      | (1ULL << (sqlParser::DAY - 128))
      | (1ULL << (sqlParser::DAYS - 128))
      | (1ULL << (sqlParser::HOUR - 128))
      | (1ULL << (sqlParser::HOURS - 128))
      | (1ULL << (sqlParser::MINUTE - 128))
      | (1ULL << (sqlParser::MINUTES - 128))
      | (1ULL << (sqlParser::MONTH - 128))
      | (1ULL << (sqlParser::MONTHS - 128))
      | (1ULL << (sqlParser::QUARTER - 128))
      | (1ULL << (sqlParser::QUARTERS - 128))
      | (1ULL << (sqlParser::SECOND - 128))
      | (1ULL << (sqlParser::SECONDS - 128))
      | (1ULL << (sqlParser::WEEK - 128))
      | (1ULL << (sqlParser::WEEKS - 128))
      | (1ULL << (sqlParser::YEAR - 128))
      | (1ULL << (sqlParser::YEARS - 128))
      | (1ULL << (sqlParser::BLOCK - 128))
      | (1ULL << (sqlParser::BLOOM - 128))
      | (1ULL << (sqlParser::BTREE - 128))
      | (1ULL << (sqlParser::HASH - 128))
      | (1ULL << (sqlParser::LOOP - 128))
      | (1ULL << (sqlParser::SKIPLIST - 128))
      | (1ULL << (sqlParser::TREE - 128))
      | (1ULL << (sqlParser::VALUE - 128)))) != 0))) {
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
    case 16: return setExprSempred(dynamic_cast<SetExprContext *>(context), predicateIndex);
    case 19: return tableExprSempred(dynamic_cast<TableExprContext *>(context), predicateIndex);
    case 60: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

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
  "statement", "statementChoice", "queryStatement", "explainStatement", 
  "explainOptions", "explainStageList", "explainRange", "explainRangeChoice", 
  "explainFormat", "statisticsStatement", "query", "queryHint", "hint", 
  "selectQuery", "selectExpr", "setQuery", "setExpr", "corrClause", "withQuery", 
  "tableExpr", "tableReference", "joinType", "extendedJoinType", "joinHint", 
  "valuesExpr", "insertQuery", "deleteQuery", "updateQuery", "createQuery", 
  "createTableQuery", "createIndexQuery", "dropQuery", "truncateQuery", 
  "tableDefinition", "tablePartitioning", "columnDefinition", "columnNameList", 
  "tableConstraint", "indexDefinition", "indexMethod", "dataType", "sqlDataType", 
  "asSubquery", "selectOrValues", "selectList", "selectListItem", "groupByList", 
  "groupByListItem", "orderByList", "orderByListItem", "joinUsingList", 
  "joinUsingListItem", "corrList", "corrListItem", "propertyList", "property", 
  "valueList", "predicate", "joinPredicate", "casePredicate", "expr", "variableName", 
  "columnRef", "columnWildcard", "className", "functionName", "methodName", 
  "catalogName", "schemaName", "tableName", "tableAlias", "columnName", 
  "columnAlias", "collationName", "indexName", "propertyName", "hintName", 
  "propertyValue", "hintValue", "identifier", "qualifier", "precision", 
  "scale", "size", "literal", "simpleLiteral", "booleanLiteral", "integerLiteral", 
  "signedIntegerLiteral", "decimalLiteral", "signedDecimalLiteral", "floatLiteral", 
  "signedFloatLiteral", "dateLiteral", "timeLiteral", "timestampLiteral", 
  "stringLiteral", "intervalLiteral", "looseInterval", "sql92CalendarInterval", 
  "sql92TimeInterval", "nonReservedWord"
};

std::vector<std::string> sqlParser::_literalNames = {
  "", "':'", "';'", "','", "'-'", "'*'", "'='", "'('", "')'", "'?'", "'.'", 
  "'+'", "'/'", "'%'", "'||'", "'<'", "'<='", "'!='", "'<>'", "'>='", "'>'", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "'/*+'", "'*/'"
};

std::vector<std::string> sqlParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "ALL", "AND", "ANTI", "ANTLR", "ANY", "AS", "ASC", "AVG", 
  "BETWEEN", "BOTH", "BY", "CATALOG", "COLLATE", "CORRESPONDING", "CREATE", 
  "CROSS", "CURRENT_CATALOG", "CURRENT_SCHEMA", "CURRENT_TIMESTAMP", "CURRENT_USER", 
  "DATA", "DATE", "DELETE", "DESC", "DETAIL", "DISTINCT", "DROP", "ESCAPE", 
  "EXCEPT", "EXPLAIN", "SQL_FALSE", "FEDERATION", "FOR", "FORMAT", "FULL", 
  "FROM", "GROUP", "HAVING", "IN", "INDEX", "INNER", "INSERT", "INTERSECT", 
  "INTO", "IS", "JOIN", "KEY", "LEADING", "LEFT", "LIKE", "LIMIT", "MAX", 
  "MIN", "MONOTONIC", "NATURAL", "NO", "NODE", "NOT", "NOW", "SQL_NULL", 
  "OFFSET", "ON", "OR", "ORDER", "OUTER", "OVER", "OVERLAY", "PARTITION", 
  "PLACING", "PRETTY", "PRIMARY", "RECURSIVE", "REFERENCE", "RIGHT", "ROW", 
  "SCHEMA", "SELECT", "SEMI", "SET", "STAGE", "STATS", "STATISTICS", "SUM", 
  "TABLE", "TO", "TOP", "TRAILING", "TRUNCATE", "SQL_TRUE", "TYPE", "UNION", 
  "UNIQUE", "UPDATE", "USING", "VALUES", "WHERE", "WITH_NAMES", "WITH_TYPES", 
  "WITH", "XOR", "BOOLEAN", "INTEGER", "DECIMAL", "FLOAT", "TIME", "TIMESTAMP", 
  "INTERVAL", "CHAR", "CHARACTER", "VARCHAR", "TEXT", "BINARY", "VARBINARY", 
  "BLOB", "VARYING", "BIT", "BITS", "BYTE", "BYTES", "CHARS", "DIGITS", 
  "CASE", "CAST", "COALESCE", "CONCAT", "COUNT", "DECODE", "EXTRACT", "EXISTS", 
  "IF", "IFNULL", "NULLIF", "POSITION", "SUBSTR", "SUBSTRING", "TRIM", "TYPEOF", 
  "WHEN", "THEN", "ELSE", "END", "DAY", "DAYS", "HOUR", "HOURS", "MINUTE", 
  "MINUTES", "MONTH", "MONTHS", "QUARTER", "QUARTERS", "SECOND", "SECONDS", 
  "WEEK", "WEEKS", "YEAR", "YEARS", "BLOCK", "BLOOM", "BTREE", "HASH", "LOOP", 
  "SKIPLIST", "TREE", "VALUE", "IDENTIFIER", "PLAIN_IDENTIFIER", "BAKTIKD_IDENTIFIER", 
  "DUBLQUOTED_IDENTIFIER", "STRING_LITERAL", "SINGLQUOTED_STRING", "DUBLQUOTED_STRING", 
  "FLOAT_LITERAL", "DECIMAL_LITERAL", "INTEGER_LITERAL", "HINT_START", "HINT_END", 
  "SLASH_COMMENT", "SQL_COMMENT", "BLOCK_COMMENT", "WHITESPACE"
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
    0x3, 0xcb, 0x6f3, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
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
    0x66, 0x4, 0x67, 0x9, 0x67, 0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 0xd1, 0xa, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x5, 
    0x3, 0xd9, 0xa, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x7, 0x5, 0xde, 0xa, 
    0x5, 0xc, 0x5, 0xe, 0x5, 0xe1, 0xb, 0x5, 0x3, 0x5, 0x3, 0x5, 0x7, 0x5, 
    0xe5, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0xe8, 0xb, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0xf0, 0xa, 0x6, 0x3, 
    0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x7, 0x7, 0xf6, 0xa, 0x7, 0xc, 0x7, 
    0xe, 0x7, 0xf9, 0xb, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 0xfe, 
    0xa, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 0x103, 0xa, 0x9, 0x3, 
    0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 0x7, 0xb, 0x109, 0xa, 0xb, 0xc, 0xb, 
    0xe, 0xb, 0x10c, 0xb, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x7, 
    0xc, 0x112, 0xa, 0xc, 0xc, 0xc, 0xe, 0xc, 0x115, 0xb, 0xc, 0x3, 0xc, 
    0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 
    0x5, 0xc, 0x11f, 0xa, 0xc, 0x3, 0xd, 0x3, 0xd, 0x5, 0xd, 0x123, 0xa, 
    0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0x127, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 
    0x12a, 0xb, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 
    0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0x135, 0xa, 0xe, 0x3, 0xe, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x7, 0xe, 0x13b, 0xa, 0xe, 0xc, 0xe, 0xe, 
    0xe, 0x13e, 0xb, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0x142, 0xa, 0xe, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x7, 0xf, 0x148, 0xa, 0xf, 0xc, 
    0xf, 0xe, 0xf, 0x14b, 0xb, 0xf, 0x5, 0xf, 0x14d, 0xa, 0xf, 0x3, 0xf, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0x153, 0xa, 0xf, 0x3, 0xf, 0x3, 
    0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0x159, 0xa, 0xf, 0x5, 0xf, 0x15b, 
    0xa, 0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x160, 0xa, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x165, 0xa, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x5, 0x10, 0x169, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x5, 0x10, 0x16e, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x172, 
    0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x178, 
    0xa, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x7, 0x11, 0x17e, 
    0xa, 0x11, 0xc, 0x11, 0xe, 0x11, 0x181, 0xb, 0x11, 0x5, 0x11, 0x183, 
    0xa, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0x189, 
    0xa, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0x18f, 
    0xa, 0x11, 0x5, 0x11, 0x191, 0xa, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x199, 0xa, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x19f, 0xa, 0x12, 0x3, 0x12, 
    0x5, 0x12, 0x1a2, 0xa, 0x12, 0x3, 0x12, 0x7, 0x12, 0x1a5, 0xa, 0x12, 
    0xc, 0x12, 0xe, 0x12, 0x1a8, 0xb, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0x1b0, 0xa, 0x13, 0x3, 0x14, 
    0x5, 0x14, 0x1b3, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x7, 0x14, 0x1ba, 0xa, 0x14, 0xc, 0x14, 0xe, 0x14, 0x1bd, 
    0xb, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1c1, 0xa, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 
    0x15, 0x5, 0x15, 0x1cb, 0xa, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1ce, 0xa, 
    0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1d4, 
    0xa, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1d7, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1dd, 0xa, 0x15, 0x3, 0x15, 0x5, 0x15, 
    0x1e0, 0xa, 0x15, 0x5, 0x15, 0x1e2, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1e8, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1ee, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1f4, 0xa, 0x15, 0x3, 0x15, 0x5, 0x15, 
    0x1f7, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x200, 0xa, 0x15, 0x3, 0x15, 0x5, 0x15, 
    0x203, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x7, 0x15, 0x20c, 0xa, 0x15, 0xc, 0x15, 0xe, 0x15, 
    0x20f, 0xb, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x214, 
    0xa, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x219, 0xa, 0x16, 
    0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0x220, 
    0xa, 0x17, 0x5, 0x17, 0x222, 0xa, 0x17, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 
    0x5, 0x18, 0x227, 0xa, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 
    0x22c, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 0x19, 
    0x232, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 0x235, 0xb, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x5, 0x19, 0x239, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x1a, 
    0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 
    0x1a, 0x3, 0x1a, 0x7, 0x1a, 0x246, 0xa, 0x1a, 0xc, 0x1a, 0xe, 0x1a, 
    0x249, 0xb, 0x1a, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x7, 0x1b, 
    0x24f, 0xa, 0x1b, 0xc, 0x1b, 0xe, 0x1b, 0x252, 0xb, 0x1b, 0x5, 0x1b, 
    0x254, 0xa, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x258, 0xa, 0x1b, 
    0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x7, 0x1b, 0x25f, 
    0xa, 0x1b, 0xc, 0x1b, 0xe, 0x1b, 0x262, 0xb, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 
    0x5, 0x1b, 0x266, 0xa, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 
    0x26b, 0xa, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x7, 0x1c, 
    0x271, 0xa, 0x1c, 0xc, 0x1c, 0xe, 0x1c, 0x274, 0xb, 0x1c, 0x5, 0x1c, 
    0x276, 0xa, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x27a, 0xa, 0x1c, 
    0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x281, 
    0xa, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x7, 0x1d, 0x287, 
    0xa, 0x1d, 0xc, 0x1d, 0xe, 0x1d, 0x28a, 0xb, 0x1d, 0x5, 0x1d, 0x28c, 
    0xa, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 
    0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x7, 0x1d, 
    0x299, 0xa, 0x1d, 0xc, 0x1d, 0xe, 0x1d, 0x29c, 0xb, 0x1d, 0x3, 0x1d, 
    0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x5, 0x1d, 0x2a2, 0xa, 0x1d, 0x3, 0x1e, 
    0x3, 0x1e, 0x5, 0x1e, 0x2a6, 0xa, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 
    0x2aa, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 
    0x2b0, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2b5, 
    0xa, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2b8, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
    0x3, 0x1f, 0x5, 0x1f, 0x2bd, 0xa, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2c0, 
    0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2c4, 0xa, 0x1f, 0x3, 0x1f, 
    0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2c9, 0xa, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 
    0x2cc, 0xa, 0x1f, 0x5, 0x1f, 0x2ce, 0xa, 0x1f, 0x3, 0x20, 0x3, 0x20, 
    0x5, 0x20, 0x2d2, 0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x5, 0x20, 0x2d8, 0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x3, 0x20, 0x5, 0x20, 0x2df, 0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x3, 0x20, 0x7, 0x20, 0x2e5, 0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 0x2e8, 
    0xb, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x5, 0x20, 0x2ed, 0xa, 0x20, 
    0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x5, 0x21, 0x2f3, 0xa, 0x21, 
    0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x5, 0x21, 0x2fa, 
    0xa, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x5, 0x21, 0x300, 
    0xa, 0x21, 0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x304, 0xa, 0x22, 0x3, 0x22, 
    0x3, 0x22, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x5, 0x23, 0x30c, 
    0xa, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x5, 0x23, 0x312, 
    0xa, 0x23, 0x7, 0x23, 0x314, 0xa, 0x23, 0xc, 0x23, 0xe, 0x23, 0x317, 
    0xb, 0x23, 0x3, 0x23, 0x3, 0x23, 0x5, 0x23, 0x31b, 0xa, 0x23, 0x3, 0x23, 
    0x5, 0x23, 0x31e, 0xa, 0x23, 0x3, 0x23, 0x3, 0x23, 0x5, 0x23, 0x322, 
    0xa, 0x23, 0x3, 0x24, 0x3, 0x24, 0x5, 0x24, 0x326, 0xa, 0x24, 0x3, 0x24, 
    0x5, 0x24, 0x329, 0xa, 0x24, 0x3, 0x24, 0x5, 0x24, 0x32c, 0xa, 0x24, 
    0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x7, 0x24, 0x332, 0xa, 0x24, 
    0xc, 0x24, 0xe, 0x24, 0x335, 0xb, 0x24, 0x3, 0x24, 0x3, 0x24, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x33c, 0xa, 0x25, 0x3, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x343, 0xa, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x348, 0xa, 0x25, 0x7, 0x25, 0x34a, 
    0xa, 0x25, 0xc, 0x25, 0xe, 0x25, 0x34d, 0xb, 0x25, 0x3, 0x25, 0x3, 0x25, 
    0x5, 0x25, 0x351, 0xa, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 
    0x7, 0x26, 0x357, 0xa, 0x26, 0xc, 0x26, 0xe, 0x26, 0x35a, 0xb, 0x26, 
    0x3, 0x26, 0x3, 0x26, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 
    0x27, 0x3, 0x27, 0x3, 0x27, 0x7, 0x27, 0x365, 0xa, 0x27, 0xc, 0x27, 
    0xe, 0x27, 0x368, 0xb, 0x27, 0x3, 0x27, 0x3, 0x27, 0x5, 0x27, 0x36c, 
    0xa, 0x27, 0x3, 0x27, 0x5, 0x27, 0x36f, 0xa, 0x27, 0x3, 0x27, 0x3, 0x27, 
    0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x7, 0x27, 0x378, 
    0xa, 0x27, 0xc, 0x27, 0xe, 0x27, 0x37b, 0xb, 0x27, 0x3, 0x27, 0x3, 0x27, 
    0x5, 0x27, 0x37f, 0xa, 0x27, 0x5, 0x27, 0x381, 0xa, 0x27, 0x3, 0x28, 
    0x5, 0x28, 0x384, 0xa, 0x28, 0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 0x388, 
    0xa, 0x28, 0x3, 0x28, 0x5, 0x28, 0x38b, 0xa, 0x28, 0x3, 0x28, 0x3, 0x28, 
    0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x7, 0x28, 0x392, 0xa, 0x28, 0xc, 0x28, 
    0xe, 0x28, 0x395, 0xb, 0x28, 0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 0x399, 
    0xa, 0x28, 0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 0x39d, 0xa, 0x28, 0x3, 0x28, 
    0x3, 0x28, 0x5, 0x28, 0x3a1, 0xa, 0x28, 0x3, 0x29, 0x3, 0x29, 0x3, 0x2a, 
    0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x3a8, 0xa, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 
    0x3ab, 0xa, 0x2a, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 
    0x5, 0x2b, 0x3b2, 0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3b6, 
    0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3bc, 
    0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3c1, 0xa, 0x2b, 
    0x5, 0x2b, 0x3c3, 0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3c7, 
    0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3cd, 
    0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3d1, 0xa, 0x2b, 0x3, 0x2b, 
    0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 
    0x2b, 0x3da, 0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 
    0x5, 0x2b, 0x3e0, 0xa, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3e3, 0xa, 0x2b, 
    0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3e8, 0xa, 0x2b, 0x3, 0x2b, 
    0x3, 0x2b, 0x5, 0x2b, 0x3ec, 0xa, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3ef, 
    0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3f3, 0xa, 0x2b, 0x3, 0x2b, 
    0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3f9, 0xa, 0x2b, 0x3, 0x2b, 
    0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x401, 
    0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x405, 0xa, 0x2b, 0x3, 0x2b, 
    0x5, 0x2b, 0x408, 0xa, 0x2b, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 
    0x7, 0x2d, 0x40e, 0xa, 0x2d, 0xc, 0x2d, 0xe, 0x2d, 0x411, 0xb, 0x2d, 
    0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x7, 0x2d, 0x417, 0xa, 0x2d, 
    0xc, 0x2d, 0xe, 0x2d, 0x41a, 0xb, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 
    0x5, 0x2d, 0x41f, 0xa, 0x2d, 0x5, 0x2d, 0x421, 0xa, 0x2d, 0x3, 0x2e, 
    0x3, 0x2e, 0x3, 0x2e, 0x7, 0x2e, 0x426, 0xa, 0x2e, 0xc, 0x2e, 0xe, 0x2e, 
    0x429, 0xb, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 0x5, 0x2f, 0x42d, 0xa, 0x2f, 
    0x3, 0x2f, 0x5, 0x2f, 0x430, 0xa, 0x2f, 0x3, 0x2f, 0x5, 0x2f, 0x433, 
    0xa, 0x2f, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x7, 0x30, 0x438, 0xa, 0x30, 
    0xc, 0x30, 0xe, 0x30, 0x43b, 0xb, 0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x32, 
    0x3, 0x32, 0x3, 0x32, 0x7, 0x32, 0x442, 0xa, 0x32, 0xc, 0x32, 0xe, 0x32, 
    0x445, 0xb, 0x32, 0x3, 0x33, 0x3, 0x33, 0x5, 0x33, 0x449, 0xa, 0x33, 
    0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x7, 0x34, 0x44e, 0xa, 0x34, 0xc, 0x34, 
    0xe, 0x34, 0x451, 0xb, 0x34, 0x3, 0x35, 0x3, 0x35, 0x3, 0x36, 0x3, 0x36, 
    0x3, 0x36, 0x7, 0x36, 0x458, 0xa, 0x36, 0xc, 0x36, 0xe, 0x36, 0x45b, 
    0xb, 0x36, 0x3, 0x37, 0x3, 0x37, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x7, 
    0x38, 0x462, 0xa, 0x38, 0xc, 0x38, 0xe, 0x38, 0x465, 0xb, 0x38, 0x3, 
    0x39, 0x3, 0x39, 0x3, 0x39, 0x5, 0x39, 0x46a, 0xa, 0x39, 0x3, 0x3a, 
    0x3, 0x3a, 0x3, 0x3a, 0x7, 0x3a, 0x46f, 0xa, 0x3a, 0xc, 0x3a, 0xe, 0x3a, 
    0x472, 0xb, 0x3a, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x6, 0x3e, 0x498, 
    0xa, 0x3e, 0xd, 0x3e, 0xe, 0x3e, 0x499, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x6, 0x3e, 0x4a1, 0xa, 0x3e, 0xd, 0x3e, 0xe, 0x3e, 
    0x4a2, 0x5, 0x3e, 0x4a5, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 
    0x4a9, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x7, 0x3e, 
    0x4c9, 0xa, 0x3e, 0xc, 0x3e, 0xe, 0x3e, 0x4cc, 0xb, 0x3e, 0x5, 0x3e, 
    0x4ce, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x7, 0x3e, 0x4d6, 0xa, 0x3e, 0xc, 0x3e, 0xe, 0x3e, 0x4d9, 
    0xb, 0x3e, 0x5, 0x3e, 0x4db, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x7, 0x3e, 0x4e3, 0xa, 0x3e, 0xc, 0x3e, 
    0xe, 0x3e, 0x4e6, 0xb, 0x3e, 0x5, 0x3e, 0x4e8, 0xa, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x4f4, 0xa, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x506, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x50d, 0xa, 0x3e, 0x5, 0x3e, 
    0x50f, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x518, 0xa, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 
    0x51b, 0xa, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x51e, 0xa, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x5, 0x3e, 0x522, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x530, 0xa, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x538, 
    0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x541, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x54a, 
    0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x553, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x55c, 
    0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x7, 0x3e, 0x566, 0xa, 0x3e, 0xc, 0x3e, 
    0xe, 0x3e, 0x569, 0xb, 0x3e, 0x5, 0x3e, 0x56b, 0xa, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x578, 0xa, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x5, 0x3e, 0x581, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x58e, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x594, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x7, 0x3e, 0x5ac, 0xa, 0x3e, 0xc, 0x3e, 0xe, 0x3e, 0x5af, 0xb, 
    0x3e, 0x5, 0x3e, 0x5b1, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x5b8, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x5, 0x3e, 0x5bd, 0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x5c6, 0xa, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x7, 0x3e, 0x5cd, 
    0xa, 0x3e, 0xc, 0x3e, 0xe, 0x3e, 0x5d0, 0xb, 0x3e, 0x5, 0x3e, 0x5d2, 
    0xa, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x5d7, 0xa, 0x3e, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x7, 0x3e, 0x5de, 
    0xa, 0x3e, 0xc, 0x3e, 0xe, 0x3e, 0x5e1, 0xb, 0x3e, 0x3, 0x3f, 0x3, 0x3f, 
    0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 0x5e8, 0xa, 0x40, 0x3, 0x40, 
    0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 0x5ed, 0xa, 0x40, 0x3, 0x40, 0x3, 0x40, 
    0x3, 0x40, 0x5, 0x40, 0x5f2, 0xa, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x41, 
    0x3, 0x41, 0x3, 0x41, 0x5, 0x41, 0x5f9, 0xa, 0x41, 0x3, 0x41, 0x3, 0x41, 
    0x3, 0x41, 0x5, 0x41, 0x5fe, 0xa, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 
    0x5, 0x41, 0x603, 0xa, 0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x42, 0x3, 0x42, 
    0x3, 0x42, 0x7, 0x42, 0x60a, 0xa, 0x42, 0xc, 0x42, 0xe, 0x42, 0x60d, 
    0xb, 0x42, 0x3, 0x42, 0x3, 0x42, 0x3, 0x43, 0x3, 0x43, 0x3, 0x44, 0x3, 
    0x44, 0x3, 0x45, 0x3, 0x45, 0x3, 0x46, 0x3, 0x46, 0x3, 0x47, 0x3, 0x47, 
    0x3, 0x48, 0x3, 0x48, 0x3, 0x49, 0x3, 0x49, 0x3, 0x4a, 0x3, 0x4a, 0x3, 
    0x4b, 0x3, 0x4b, 0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4d, 0x3, 0x4d, 0x3, 0x4e, 
    0x3, 0x4e, 0x3, 0x4f, 0x3, 0x4f, 0x5, 0x4f, 0x62b, 0xa, 0x4f, 0x3, 0x50, 
    0x3, 0x50, 0x5, 0x50, 0x62f, 0xa, 0x50, 0x3, 0x51, 0x3, 0x51, 0x5, 0x51, 
    0x633, 0xa, 0x51, 0x3, 0x52, 0x3, 0x52, 0x3, 0x53, 0x3, 0x53, 0x3, 0x54, 
    0x3, 0x54, 0x3, 0x55, 0x3, 0x55, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 
    0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 
    0x5, 0x56, 0x647, 0xa, 0x56, 0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 
    0x3, 0x57, 0x3, 0x57, 0x5, 0x57, 0x64f, 0xa, 0x57, 0x3, 0x58, 0x3, 0x58, 
    0x3, 0x59, 0x3, 0x59, 0x3, 0x5a, 0x3, 0x5a, 0x5, 0x5a, 0x657, 0xa, 0x5a, 
    0x3, 0x5a, 0x3, 0x5a, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5c, 0x3, 0x5c, 0x5, 
    0x5c, 0x65f, 0xa, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5d, 0x3, 0x5d, 
    0x3, 0x5e, 0x3, 0x5e, 0x5, 0x5e, 0x667, 0xa, 0x5e, 0x3, 0x5e, 0x3, 0x5e, 
    0x3, 0x5f, 0x3, 0x5f, 0x3, 0x5f, 0x3, 0x60, 0x3, 0x60, 0x3, 0x60, 0x3, 
    0x61, 0x3, 0x61, 0x3, 0x61, 0x3, 0x62, 0x3, 0x62, 0x3, 0x63, 0x3, 0x63, 
    0x3, 0x63, 0x5, 0x63, 0x679, 0xa, 0x63, 0x3, 0x64, 0x3, 0x64, 0x3, 0x64, 
    0x3, 0x65, 0x3, 0x65, 0x3, 0x65, 0x3, 0x65, 0x3, 0x65, 0x3, 0x65, 0x3, 
    0x65, 0x5, 0x65, 0x685, 0xa, 0x65, 0x3, 0x65, 0x3, 0x65, 0x3, 0x65, 
    0x3, 0x65, 0x3, 0x65, 0x5, 0x65, 0x68c, 0xa, 0x65, 0x3, 0x65, 0x3, 0x65, 
    0x3, 0x65, 0x3, 0x65, 0x3, 0x65, 0x3, 0x65, 0x3, 0x65, 0x5, 0x65, 0x695, 
    0xa, 0x65, 0x5, 0x65, 0x697, 0xa, 0x65, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 
    0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x5, 0x66, 0x6a0, 0xa, 0x66, 
    0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x5, 0x66, 0x6a7, 
    0xa, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 
    0x66, 0x3, 0x66, 0x3, 0x66, 0x5, 0x66, 0x6b1, 0xa, 0x66, 0x5, 0x66, 
    0x6b3, 0xa, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 
    0x5, 0x66, 0x6ba, 0xa, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 
    0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x5, 0x66, 0x6c3, 0xa, 0x66, 0x5, 0x66, 
    0x6c5, 0xa, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 
    0x5, 0x66, 0x6cc, 0xa, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 
    0x3, 0x66, 0x3, 0x66, 0x5, 0x66, 0x6d4, 0xa, 0x66, 0x3, 0x66, 0x3, 0x66, 
    0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x5, 0x66, 0x6db, 0xa, 0x66, 0x3, 0x66, 
    0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x5, 0x66, 0x6e2, 0xa, 0x66, 
    0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x5, 0x66, 0x6e9, 
    0xa, 0x66, 0x3, 0x66, 0x3, 0x66, 0x5, 0x66, 0x6ed, 0xa, 0x66, 0x5, 0x66, 
    0x6ef, 0xa, 0x66, 0x3, 0x67, 0x3, 0x67, 0x3, 0x67, 0x2, 0x5, 0x22, 0x28, 
    0x7a, 0x68, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 
    0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 
    0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 
    0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 
    0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 
    0x78, 0x7a, 0x7c, 0x7e, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 
    0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, 0xa0, 0xa2, 0xa4, 0xa6, 
    0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe, 
    0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0x2, 0x21, 0x4, 0x2, 0x6, 
    0x6, 0x6b, 0x6b, 0x4, 0x2, 0x7, 0x7, 0x17, 0x17, 0x4, 0x2, 0x5c, 0x5c, 
    0x61, 0x61, 0x3, 0x2, 0x67, 0x68, 0x4, 0x2, 0xb, 0xb, 0x63, 0x63, 0x5, 
    0x2, 0x33, 0x33, 0x41, 0x41, 0x71, 0x71, 0x5, 0x2, 0x39, 0x39, 0x47, 
    0x47, 0x60, 0x60, 0x4, 0x2, 0xb4, 0xb5, 0xb7, 0xb8, 0x4, 0x2, 0x3e, 
    0x3e, 0x45, 0x45, 0x4, 0x2, 0xb7, 0xb7, 0xbb, 0xbb, 0x4, 0x2, 0xb6, 
    0xb7, 0xb9, 0xba, 0x5, 0x2, 0x8b, 0x8b, 0x8d, 0x8d, 0x8f, 0x8f, 0x3, 
    0x2, 0x82, 0x83, 0x4, 0x2, 0x82, 0x82, 0x8c, 0x8e, 0x3, 0x2, 0x8c, 0x8d, 
    0x4, 0x2, 0x1d, 0x1d, 0x2e, 0x2e, 0x4, 0x2, 0x6, 0x6, 0xd, 0xd, 0x4, 
    0x2, 0x5, 0x5, 0x1c, 0x1c, 0x4, 0x2, 0x5, 0x5, 0x5b, 0x5b, 0x4, 0x2, 
    0x5, 0x5, 0x3a, 0x3a, 0x4, 0x2, 0x5, 0x5, 0x37, 0x37, 0x4, 0x2, 0x5, 
    0x5, 0x3d, 0x3d, 0x3, 0x2, 0x9c, 0x9d, 0x5, 0x2, 0x20, 0x20, 0x46, 0x46, 
    0x6d, 0x6d, 0x8, 0x2, 0xa4, 0xa4, 0xa6, 0xa6, 0xa8, 0xa8, 0xaa, 0xaa, 
    0xae, 0xae, 0xb2, 0xb2, 0x4, 0x2, 0x7, 0x7, 0xe, 0xf, 0x5, 0x2, 0x6, 
    0x6, 0xd, 0xd, 0x10, 0x10, 0x4, 0x2, 0x8, 0x8, 0x11, 0x16, 0x3, 0x2, 
    0x27, 0x2a, 0x4, 0x2, 0x35, 0x35, 0x6f, 0x6f, 0x21, 0x2, 0x1a, 0x1a, 
    0x1d, 0x1d, 0x20, 0x22, 0x24, 0x25, 0x27, 0x2f, 0x31, 0x34, 0x36, 0x36, 
    0x38, 0x38, 0x3e, 0x3e, 0x40, 0x40, 0x42, 0x45, 0x47, 0x47, 0x4c, 0x4c, 
    0x4e, 0x4f, 0x53, 0x53, 0x57, 0x57, 0x59, 0x5a, 0x5c, 0x5c, 0x5f, 0x60, 
    0x62, 0x62, 0x65, 0x68, 0x6a, 0x6a, 0x70, 0x75, 0x77, 0x77, 0x7b, 0x80, 
    0x82, 0x83, 0x86, 0x87, 0x89, 0x8f, 0x92, 0x96, 0x99, 0x9d, 0xa4, 0xbb, 
    0x2, 0x7e2, 0x2, 0xd0, 0x3, 0x2, 0x2, 0x2, 0x4, 0xd8, 0x3, 0x2, 0x2, 
    0x2, 0x6, 0xda, 0x3, 0x2, 0x2, 0x2, 0x8, 0xdf, 0x3, 0x2, 0x2, 0x2, 0xa, 
    0xef, 0x3, 0x2, 0x2, 0x2, 0xc, 0xf1, 0x3, 0x2, 0x2, 0x2, 0xe, 0xfa, 
    0x3, 0x2, 0x2, 0x2, 0x10, 0x102, 0x3, 0x2, 0x2, 0x2, 0x12, 0x104, 0x3, 
    0x2, 0x2, 0x2, 0x14, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x16, 0x113, 0x3, 0x2, 
    0x2, 0x2, 0x18, 0x120, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x141, 0x3, 0x2, 0x2, 
    0x2, 0x1c, 0x14c, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x177, 0x3, 0x2, 0x2, 0x2, 
    0x20, 0x182, 0x3, 0x2, 0x2, 0x2, 0x22, 0x198, 0x3, 0x2, 0x2, 0x2, 0x24, 
    0x1a9, 0x3, 0x2, 0x2, 0x2, 0x26, 0x1b2, 0x3, 0x2, 0x2, 0x2, 0x28, 0x1e1, 
    0x3, 0x2, 0x2, 0x2, 0x2a, 0x218, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x221, 0x3, 
    0x2, 0x2, 0x2, 0x2e, 0x226, 0x3, 0x2, 0x2, 0x2, 0x30, 0x228, 0x3, 0x2, 
    0x2, 0x2, 0x32, 0x23c, 0x3, 0x2, 0x2, 0x2, 0x34, 0x253, 0x3, 0x2, 0x2, 
    0x2, 0x36, 0x275, 0x3, 0x2, 0x2, 0x2, 0x38, 0x28b, 0x3, 0x2, 0x2, 0x2, 
    0x3a, 0x2a5, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x2a7, 0x3, 0x2, 0x2, 0x2, 0x3e, 
    0x2cf, 0x3, 0x2, 0x2, 0x2, 0x40, 0x2ff, 0x3, 0x2, 0x2, 0x2, 0x42, 0x301, 
    0x3, 0x2, 0x2, 0x2, 0x44, 0x307, 0x3, 0x2, 0x2, 0x2, 0x46, 0x323, 0x3, 
    0x2, 0x2, 0x2, 0x48, 0x338, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x352, 0x3, 0x2, 
    0x2, 0x2, 0x4c, 0x380, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x383, 0x3, 0x2, 0x2, 
    0x2, 0x50, 0x3a2, 0x3, 0x2, 0x2, 0x2, 0x52, 0x3aa, 0x3, 0x2, 0x2, 0x2, 
    0x54, 0x407, 0x3, 0x2, 0x2, 0x2, 0x56, 0x409, 0x3, 0x2, 0x2, 0x2, 0x58, 
    0x420, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x422, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x432, 
    0x3, 0x2, 0x2, 0x2, 0x5e, 0x434, 0x3, 0x2, 0x2, 0x2, 0x60, 0x43c, 0x3, 
    0x2, 0x2, 0x2, 0x62, 0x43e, 0x3, 0x2, 0x2, 0x2, 0x64, 0x446, 0x3, 0x2, 
    0x2, 0x2, 0x66, 0x44a, 0x3, 0x2, 0x2, 0x2, 0x68, 0x452, 0x3, 0x2, 0x2, 
    0x2, 0x6a, 0x454, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x45c, 0x3, 0x2, 0x2, 0x2, 
    0x6e, 0x45e, 0x3, 0x2, 0x2, 0x2, 0x70, 0x466, 0x3, 0x2, 0x2, 0x2, 0x72, 
    0x46b, 0x3, 0x2, 0x2, 0x2, 0x74, 0x473, 0x3, 0x2, 0x2, 0x2, 0x76, 0x475, 
    0x3, 0x2, 0x2, 0x2, 0x78, 0x477, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x580, 0x3, 
    0x2, 0x2, 0x2, 0x7c, 0x5e2, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x5f1, 0x3, 0x2, 
    0x2, 0x2, 0x80, 0x602, 0x3, 0x2, 0x2, 0x2, 0x82, 0x60b, 0x3, 0x2, 0x2, 
    0x2, 0x84, 0x610, 0x3, 0x2, 0x2, 0x2, 0x86, 0x612, 0x3, 0x2, 0x2, 0x2, 
    0x88, 0x614, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x616, 0x3, 0x2, 0x2, 0x2, 0x8c, 
    0x618, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x61a, 0x3, 0x2, 0x2, 0x2, 0x90, 0x61c, 
    0x3, 0x2, 0x2, 0x2, 0x92, 0x61e, 0x3, 0x2, 0x2, 0x2, 0x94, 0x620, 0x3, 
    0x2, 0x2, 0x2, 0x96, 0x622, 0x3, 0x2, 0x2, 0x2, 0x98, 0x624, 0x3, 0x2, 
    0x2, 0x2, 0x9a, 0x626, 0x3, 0x2, 0x2, 0x2, 0x9c, 0x62a, 0x3, 0x2, 0x2, 
    0x2, 0x9e, 0x62e, 0x3, 0x2, 0x2, 0x2, 0xa0, 0x632, 0x3, 0x2, 0x2, 0x2, 
    0xa2, 0x634, 0x3, 0x2, 0x2, 0x2, 0xa4, 0x636, 0x3, 0x2, 0x2, 0x2, 0xa6, 
    0x638, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x63a, 0x3, 0x2, 0x2, 0x2, 0xaa, 0x646, 
    0x3, 0x2, 0x2, 0x2, 0xac, 0x64e, 0x3, 0x2, 0x2, 0x2, 0xae, 0x650, 0x3, 
    0x2, 0x2, 0x2, 0xb0, 0x652, 0x3, 0x2, 0x2, 0x2, 0xb2, 0x656, 0x3, 0x2, 
    0x2, 0x2, 0xb4, 0x65a, 0x3, 0x2, 0x2, 0x2, 0xb6, 0x65e, 0x3, 0x2, 0x2, 
    0x2, 0xb8, 0x662, 0x3, 0x2, 0x2, 0x2, 0xba, 0x666, 0x3, 0x2, 0x2, 0x2, 
    0xbc, 0x66a, 0x3, 0x2, 0x2, 0x2, 0xbe, 0x66d, 0x3, 0x2, 0x2, 0x2, 0xc0, 
    0x670, 0x3, 0x2, 0x2, 0x2, 0xc2, 0x673, 0x3, 0x2, 0x2, 0x2, 0xc4, 0x678, 
    0x3, 0x2, 0x2, 0x2, 0xc6, 0x67a, 0x3, 0x2, 0x2, 0x2, 0xc8, 0x67d, 0x3, 
    0x2, 0x2, 0x2, 0xca, 0x698, 0x3, 0x2, 0x2, 0x2, 0xcc, 0x6f0, 0x3, 0x2, 
    0x2, 0x2, 0xce, 0xcf, 0x7, 0x1a, 0x2, 0x2, 0xcf, 0xd1, 0x7, 0x3, 0x2, 
    0x2, 0xd0, 0xce, 0x3, 0x2, 0x2, 0x2, 0xd0, 0xd1, 0x3, 0x2, 0x2, 0x2, 
    0xd1, 0xd2, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xd3, 0x5, 0x4, 0x3, 0x2, 0xd3, 
    0xd4, 0x7, 0x4, 0x2, 0x2, 0xd4, 0x3, 0x3, 0x2, 0x2, 0x2, 0xd5, 0xd9, 
    0x5, 0x6, 0x4, 0x2, 0xd6, 0xd9, 0x5, 0x8, 0x5, 0x2, 0xd7, 0xd9, 0x5, 
    0x14, 0xb, 0x2, 0xd8, 0xd5, 0x3, 0x2, 0x2, 0x2, 0xd8, 0xd6, 0x3, 0x2, 
    0x2, 0x2, 0xd8, 0xd7, 0x3, 0x2, 0x2, 0x2, 0xd9, 0x5, 0x3, 0x2, 0x2, 
    0x2, 0xda, 0xdb, 0x5, 0x16, 0xc, 0x2, 0xdb, 0x7, 0x3, 0x2, 0x2, 0x2, 
    0xdc, 0xde, 0x5, 0x18, 0xd, 0x2, 0xdd, 0xdc, 0x3, 0x2, 0x2, 0x2, 0xde, 
    0xe1, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xdd, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xe0, 
    0x3, 0x2, 0x2, 0x2, 0xe0, 0xe2, 0x3, 0x2, 0x2, 0x2, 0xe1, 0xdf, 0x3, 
    0x2, 0x2, 0x2, 0xe2, 0xe6, 0x7, 0x34, 0x2, 0x2, 0xe3, 0xe5, 0x5, 0xa, 
    0x6, 0x2, 0xe4, 0xe3, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xe8, 0x3, 0x2, 0x2, 
    0x2, 0xe6, 0xe4, 0x3, 0x2, 0x2, 0x2, 0xe6, 0xe7, 0x3, 0x2, 0x2, 0x2, 
    0xe7, 0xe9, 0x3, 0x2, 0x2, 0x2, 0xe8, 0xe6, 0x3, 0x2, 0x2, 0x2, 0xe9, 
    0xea, 0x5, 0x4, 0x3, 0x2, 0xea, 0x9, 0x3, 0x2, 0x2, 0x2, 0xeb, 0xf0, 
    0x5, 0xc, 0x7, 0x2, 0xec, 0xf0, 0x5, 0x12, 0xa, 0x2, 0xed, 0xf0, 0x7, 
    0x77, 0x2, 0x2, 0xee, 0xf0, 0x7, 0x78, 0x2, 0x2, 0xef, 0xeb, 0x3, 0x2, 
    0x2, 0x2, 0xef, 0xec, 0x3, 0x2, 0x2, 0x2, 0xef, 0xed, 0x3, 0x2, 0x2, 
    0x2, 0xef, 0xee, 0x3, 0x2, 0x2, 0x2, 0xf0, 0xb, 0x3, 0x2, 0x2, 0x2, 
    0xf1, 0xf2, 0x7, 0x66, 0x2, 0x2, 0xf2, 0xf7, 0x5, 0xe, 0x8, 0x2, 0xf3, 
    0xf4, 0x7, 0x5, 0x2, 0x2, 0xf4, 0xf6, 0x5, 0xe, 0x8, 0x2, 0xf5, 0xf3, 
    0x3, 0x2, 0x2, 0x2, 0xf6, 0xf9, 0x3, 0x2, 0x2, 0x2, 0xf7, 0xf5, 0x3, 
    0x2, 0x2, 0x2, 0xf7, 0xf8, 0x3, 0x2, 0x2, 0x2, 0xf8, 0xd, 0x3, 0x2, 
    0x2, 0x2, 0xf9, 0xf7, 0x3, 0x2, 0x2, 0x2, 0xfa, 0xfd, 0x5, 0x10, 0x9, 
    0x2, 0xfb, 0xfc, 0x9, 0x2, 0x2, 0x2, 0xfc, 0xfe, 0x5, 0x10, 0x9, 0x2, 
    0xfd, 0xfb, 0x3, 0x2, 0x2, 0x2, 0xfd, 0xfe, 0x3, 0x2, 0x2, 0x2, 0xfe, 
    0xf, 0x3, 0x2, 0x2, 0x2, 0xff, 0x103, 0x9, 0x3, 0x2, 0x2, 0x100, 0x103, 
    0x7, 0xc5, 0x2, 0x2, 0x101, 0x103, 0x5, 0xa0, 0x51, 0x2, 0x102, 0xff, 
    0x3, 0x2, 0x2, 0x2, 0x102, 0x100, 0x3, 0x2, 0x2, 0x2, 0x102, 0x101, 
    0x3, 0x2, 0x2, 0x2, 0x103, 0x11, 0x3, 0x2, 0x2, 0x2, 0x104, 0x105, 0x7, 
    0x38, 0x2, 0x2, 0x105, 0x106, 0x9, 0x4, 0x2, 0x2, 0x106, 0x13, 0x3, 
    0x2, 0x2, 0x2, 0x107, 0x109, 0x5, 0x18, 0xd, 0x2, 0x108, 0x107, 0x3, 
    0x2, 0x2, 0x2, 0x109, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x10a, 0x108, 0x3, 
    0x2, 0x2, 0x2, 0x10a, 0x10b, 0x3, 0x2, 0x2, 0x2, 0x10b, 0x10d, 0x3, 
    0x2, 0x2, 0x2, 0x10c, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x10e, 0x9, 
    0x5, 0x2, 0x2, 0x10e, 0x10f, 0x5, 0x4, 0x3, 0x2, 0x10f, 0x15, 0x3, 0x2, 
    0x2, 0x2, 0x110, 0x112, 0x5, 0x18, 0xd, 0x2, 0x111, 0x110, 0x3, 0x2, 
    0x2, 0x2, 0x112, 0x115, 0x3, 0x2, 0x2, 0x2, 0x113, 0x111, 0x3, 0x2, 
    0x2, 0x2, 0x113, 0x114, 0x3, 0x2, 0x2, 0x2, 0x114, 0x11e, 0x3, 0x2, 
    0x2, 0x2, 0x115, 0x113, 0x3, 0x2, 0x2, 0x2, 0x116, 0x11f, 0x5, 0x1c, 
    0xf, 0x2, 0x117, 0x11f, 0x5, 0x20, 0x11, 0x2, 0x118, 0x11f, 0x5, 0x34, 
    0x1b, 0x2, 0x119, 0x11f, 0x5, 0x36, 0x1c, 0x2, 0x11a, 0x11f, 0x5, 0x38, 
    0x1d, 0x2, 0x11b, 0x11f, 0x5, 0x3a, 0x1e, 0x2, 0x11c, 0x11f, 0x5, 0x40, 
    0x21, 0x2, 0x11d, 0x11f, 0x5, 0x42, 0x22, 0x2, 0x11e, 0x116, 0x3, 0x2, 
    0x2, 0x2, 0x11e, 0x117, 0x3, 0x2, 0x2, 0x2, 0x11e, 0x118, 0x3, 0x2, 
    0x2, 0x2, 0x11e, 0x119, 0x3, 0x2, 0x2, 0x2, 0x11e, 0x11a, 0x3, 0x2, 
    0x2, 0x2, 0x11e, 0x11b, 0x3, 0x2, 0x2, 0x2, 0x11e, 0x11c, 0x3, 0x2, 
    0x2, 0x2, 0x11e, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x17, 0x3, 0x2, 0x2, 
    0x2, 0x120, 0x122, 0x7, 0xc6, 0x2, 0x2, 0x121, 0x123, 0x5, 0x1a, 0xe, 
    0x2, 0x122, 0x121, 0x3, 0x2, 0x2, 0x2, 0x122, 0x123, 0x3, 0x2, 0x2, 
    0x2, 0x123, 0x128, 0x3, 0x2, 0x2, 0x2, 0x124, 0x125, 0x7, 0x5, 0x2, 
    0x2, 0x125, 0x127, 0x5, 0x1a, 0xe, 0x2, 0x126, 0x124, 0x3, 0x2, 0x2, 
    0x2, 0x127, 0x12a, 0x3, 0x2, 0x2, 0x2, 0x128, 0x126, 0x3, 0x2, 0x2, 
    0x2, 0x128, 0x129, 0x3, 0x2, 0x2, 0x2, 0x129, 0x12b, 0x3, 0x2, 0x2, 
    0x2, 0x12a, 0x128, 0x3, 0x2, 0x2, 0x2, 0x12b, 0x12c, 0x7, 0xc7, 0x2, 
    0x2, 0x12c, 0x19, 0x3, 0x2, 0x2, 0x2, 0x12d, 0x142, 0x5, 0x9a, 0x4e, 
    0x2, 0x12e, 0x12f, 0x5, 0x9a, 0x4e, 0x2, 0x12f, 0x130, 0x7, 0x8, 0x2, 
    0x2, 0x130, 0x131, 0x5, 0x9e, 0x50, 0x2, 0x131, 0x142, 0x3, 0x2, 0x2, 
    0x2, 0x132, 0x134, 0x5, 0x9a, 0x4e, 0x2, 0x133, 0x135, 0x7, 0x8, 0x2, 
    0x2, 0x134, 0x133, 0x3, 0x2, 0x2, 0x2, 0x134, 0x135, 0x3, 0x2, 0x2, 
    0x2, 0x135, 0x136, 0x3, 0x2, 0x2, 0x2, 0x136, 0x137, 0x7, 0x9, 0x2, 
    0x2, 0x137, 0x13c, 0x5, 0x9e, 0x50, 0x2, 0x138, 0x139, 0x7, 0x5, 0x2, 
    0x2, 0x139, 0x13b, 0x5, 0x9e, 0x50, 0x2, 0x13a, 0x138, 0x3, 0x2, 0x2, 
    0x2, 0x13b, 0x13e, 0x3, 0x2, 0x2, 0x2, 0x13c, 0x13a, 0x3, 0x2, 0x2, 
    0x2, 0x13c, 0x13d, 0x3, 0x2, 0x2, 0x2, 0x13d, 0x13f, 0x3, 0x2, 0x2, 
    0x2, 0x13e, 0x13c, 0x3, 0x2, 0x2, 0x2, 0x13f, 0x140, 0x7, 0xa, 0x2, 
    0x2, 0x140, 0x142, 0x3, 0x2, 0x2, 0x2, 0x141, 0x12d, 0x3, 0x2, 0x2, 
    0x2, 0x141, 0x12e, 0x3, 0x2, 0x2, 0x2, 0x141, 0x132, 0x3, 0x2, 0x2, 
    0x2, 0x142, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x143, 0x144, 0x7, 0x79, 0x2, 
    0x2, 0x144, 0x149, 0x5, 0x26, 0x14, 0x2, 0x145, 0x146, 0x7, 0x5, 0x2, 
    0x2, 0x146, 0x148, 0x5, 0x26, 0x14, 0x2, 0x147, 0x145, 0x3, 0x2, 0x2, 
    0x2, 0x148, 0x14b, 0x3, 0x2, 0x2, 0x2, 0x149, 0x147, 0x3, 0x2, 0x2, 
    0x2, 0x149, 0x14a, 0x3, 0x2, 0x2, 0x2, 0x14a, 0x14d, 0x3, 0x2, 0x2, 
    0x2, 0x14b, 0x149, 0x3, 0x2, 0x2, 0x2, 0x14c, 0x143, 0x3, 0x2, 0x2, 
    0x2, 0x14c, 0x14d, 0x3, 0x2, 0x2, 0x2, 0x14d, 0x14e, 0x3, 0x2, 0x2, 
    0x2, 0x14e, 0x152, 0x5, 0x1e, 0x10, 0x2, 0x14f, 0x150, 0x7, 0x56, 0x2, 
    0x2, 0x150, 0x151, 0x7, 0x21, 0x2, 0x2, 0x151, 0x153, 0x5, 0x62, 0x32, 
    0x2, 0x152, 0x14f, 0x3, 0x2, 0x2, 0x2, 0x152, 0x153, 0x3, 0x2, 0x2, 
    0x2, 0x153, 0x15a, 0x3, 0x2, 0x2, 0x2, 0x154, 0x155, 0x7, 0x49, 0x2, 
    0x2, 0x155, 0x158, 0x7, 0xc5, 0x2, 0x2, 0x156, 0x157, 0x7, 0x53, 0x2, 
    0x2, 0x157, 0x159, 0x7, 0xc5, 0x2, 0x2, 0x158, 0x156, 0x3, 0x2, 0x2, 
    0x2, 0x158, 0x159, 0x3, 0x2, 0x2, 0x2, 0x159, 0x15b, 0x3, 0x2, 0x2, 
    0x2, 0x15a, 0x154, 0x3, 0x2, 0x2, 0x2, 0x15a, 0x15b, 0x3, 0x2, 0x2, 
    0x2, 0x15b, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x15c, 0x15f, 0x9, 0x6, 0x2, 0x2, 
    0x15d, 0x160, 0x7, 0x17, 0x2, 0x2, 0x15e, 0x160, 0x7, 0x30, 0x2, 0x2, 
    0x15f, 0x15d, 0x3, 0x2, 0x2, 0x2, 0x15f, 0x15e, 0x3, 0x2, 0x2, 0x2, 
    0x15f, 0x160, 0x3, 0x2, 0x2, 0x2, 0x160, 0x161, 0x3, 0x2, 0x2, 0x2, 
    0x161, 0x164, 0x5, 0x5a, 0x2e, 0x2, 0x162, 0x163, 0x7, 0x3a, 0x2, 0x2, 
    0x163, 0x165, 0x5, 0x28, 0x15, 0x2, 0x164, 0x162, 0x3, 0x2, 0x2, 0x2, 
    0x164, 0x165, 0x3, 0x2, 0x2, 0x2, 0x165, 0x168, 0x3, 0x2, 0x2, 0x2, 
    0x166, 0x167, 0x7, 0x76, 0x2, 0x2, 0x167, 0x169, 0x5, 0x74, 0x3b, 0x2, 
    0x168, 0x166, 0x3, 0x2, 0x2, 0x2, 0x168, 0x169, 0x3, 0x2, 0x2, 0x2, 
    0x169, 0x16d, 0x3, 0x2, 0x2, 0x2, 0x16a, 0x16b, 0x7, 0x3b, 0x2, 0x2, 
    0x16b, 0x16c, 0x7, 0x21, 0x2, 0x2, 0x16c, 0x16e, 0x5, 0x5e, 0x30, 0x2, 
    0x16d, 0x16a, 0x3, 0x2, 0x2, 0x2, 0x16d, 0x16e, 0x3, 0x2, 0x2, 0x2, 
    0x16e, 0x171, 0x3, 0x2, 0x2, 0x2, 0x16f, 0x170, 0x7, 0x3c, 0x2, 0x2, 
    0x170, 0x172, 0x5, 0x74, 0x3b, 0x2, 0x171, 0x16f, 0x3, 0x2, 0x2, 0x2, 
    0x171, 0x172, 0x3, 0x2, 0x2, 0x2, 0x172, 0x178, 0x3, 0x2, 0x2, 0x2, 
    0x173, 0x174, 0x7, 0x9, 0x2, 0x2, 0x174, 0x175, 0x5, 0x1e, 0x10, 0x2, 
    0x175, 0x176, 0x7, 0xa, 0x2, 0x2, 0x176, 0x178, 0x3, 0x2, 0x2, 0x2, 
    0x177, 0x15c, 0x3, 0x2, 0x2, 0x2, 0x177, 0x173, 0x3, 0x2, 0x2, 0x2, 
    0x178, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x179, 0x17a, 0x7, 0x79, 0x2, 0x2, 
    0x17a, 0x17f, 0x5, 0x26, 0x14, 0x2, 0x17b, 0x17c, 0x7, 0x5, 0x2, 0x2, 
    0x17c, 0x17e, 0x5, 0x26, 0x14, 0x2, 0x17d, 0x17b, 0x3, 0x2, 0x2, 0x2, 
    0x17e, 0x181, 0x3, 0x2, 0x2, 0x2, 0x17f, 0x17d, 0x3, 0x2, 0x2, 0x2, 
    0x17f, 0x180, 0x3, 0x2, 0x2, 0x2, 0x180, 0x183, 0x3, 0x2, 0x2, 0x2, 
    0x181, 0x17f, 0x3, 0x2, 0x2, 0x2, 0x182, 0x179, 0x3, 0x2, 0x2, 0x2, 
    0x182, 0x183, 0x3, 0x2, 0x2, 0x2, 0x183, 0x184, 0x3, 0x2, 0x2, 0x2, 
    0x184, 0x188, 0x5, 0x22, 0x12, 0x2, 0x185, 0x186, 0x7, 0x56, 0x2, 0x2, 
    0x186, 0x187, 0x7, 0x21, 0x2, 0x2, 0x187, 0x189, 0x5, 0x62, 0x32, 0x2, 
    0x188, 0x185, 0x3, 0x2, 0x2, 0x2, 0x188, 0x189, 0x3, 0x2, 0x2, 0x2, 
    0x189, 0x190, 0x3, 0x2, 0x2, 0x2, 0x18a, 0x18b, 0x7, 0x49, 0x2, 0x2, 
    0x18b, 0x18e, 0x7, 0xc5, 0x2, 0x2, 0x18c, 0x18d, 0x7, 0x53, 0x2, 0x2, 
    0x18d, 0x18f, 0x7, 0xc5, 0x2, 0x2, 0x18e, 0x18c, 0x3, 0x2, 0x2, 0x2, 
    0x18e, 0x18f, 0x3, 0x2, 0x2, 0x2, 0x18f, 0x191, 0x3, 0x2, 0x2, 0x2, 
    0x190, 0x18a, 0x3, 0x2, 0x2, 0x2, 0x190, 0x191, 0x3, 0x2, 0x2, 0x2, 
    0x191, 0x21, 0x3, 0x2, 0x2, 0x2, 0x192, 0x193, 0x8, 0x12, 0x1, 0x2, 
    0x193, 0x199, 0x5, 0x1e, 0x10, 0x2, 0x194, 0x195, 0x7, 0x9, 0x2, 0x2, 
    0x195, 0x196, 0x5, 0x22, 0x12, 0x2, 0x196, 0x197, 0x7, 0xa, 0x2, 0x2, 
    0x197, 0x199, 0x3, 0x2, 0x2, 0x2, 0x198, 0x192, 0x3, 0x2, 0x2, 0x2, 
    0x198, 0x194, 0x3, 0x2, 0x2, 0x2, 0x199, 0x1a6, 0x3, 0x2, 0x2, 0x2, 
    0x19a, 0x19b, 0xc, 0x4, 0x2, 0x2, 0x19b, 0x19e, 0x9, 0x7, 0x2, 0x2, 
    0x19c, 0x19f, 0x7, 0x30, 0x2, 0x2, 0x19d, 0x19f, 0x7, 0x17, 0x2, 0x2, 
    0x19e, 0x19c, 0x3, 0x2, 0x2, 0x2, 0x19e, 0x19d, 0x3, 0x2, 0x2, 0x2, 
    0x19e, 0x19f, 0x3, 0x2, 0x2, 0x2, 0x19f, 0x1a1, 0x3, 0x2, 0x2, 0x2, 
    0x1a0, 0x1a2, 0x5, 0x24, 0x13, 0x2, 0x1a1, 0x1a0, 0x3, 0x2, 0x2, 0x2, 
    0x1a1, 0x1a2, 0x3, 0x2, 0x2, 0x2, 0x1a2, 0x1a3, 0x3, 0x2, 0x2, 0x2, 
    0x1a3, 0x1a5, 0x5, 0x22, 0x12, 0x5, 0x1a4, 0x19a, 0x3, 0x2, 0x2, 0x2, 
    0x1a5, 0x1a8, 0x3, 0x2, 0x2, 0x2, 0x1a6, 0x1a4, 0x3, 0x2, 0x2, 0x2, 
    0x1a6, 0x1a7, 0x3, 0x2, 0x2, 0x2, 0x1a7, 0x23, 0x3, 0x2, 0x2, 0x2, 0x1a8, 
    0x1a6, 0x3, 0x2, 0x2, 0x2, 0x1a9, 0x1af, 0x7, 0x24, 0x2, 0x2, 0x1aa, 
    0x1ab, 0x7, 0x21, 0x2, 0x2, 0x1ab, 0x1ac, 0x7, 0x9, 0x2, 0x2, 0x1ac, 
    0x1ad, 0x5, 0x6a, 0x36, 0x2, 0x1ad, 0x1ae, 0x7, 0xa, 0x2, 0x2, 0x1ae, 
    0x1b0, 0x3, 0x2, 0x2, 0x2, 0x1af, 0x1aa, 0x3, 0x2, 0x2, 0x2, 0x1af, 
    0x1b0, 0x3, 0x2, 0x2, 0x2, 0x1b0, 0x25, 0x3, 0x2, 0x2, 0x2, 0x1b1, 0x1b3, 
    0x7, 0x5e, 0x2, 0x2, 0x1b2, 0x1b1, 0x3, 0x2, 0x2, 0x2, 0x1b2, 0x1b3, 
    0x3, 0x2, 0x2, 0x2, 0x1b3, 0x1b4, 0x3, 0x2, 0x2, 0x2, 0x1b4, 0x1c0, 
    0x5, 0x8c, 0x47, 0x2, 0x1b5, 0x1b6, 0x7, 0x9, 0x2, 0x2, 0x1b6, 0x1bb, 
    0x5, 0x92, 0x4a, 0x2, 0x1b7, 0x1b8, 0x7, 0x5, 0x2, 0x2, 0x1b8, 0x1ba, 
    0x5, 0x92, 0x4a, 0x2, 0x1b9, 0x1b7, 0x3, 0x2, 0x2, 0x2, 0x1ba, 0x1bd, 
    0x3, 0x2, 0x2, 0x2, 0x1bb, 0x1b9, 0x3, 0x2, 0x2, 0x2, 0x1bb, 0x1bc, 
    0x3, 0x2, 0x2, 0x2, 0x1bc, 0x1be, 0x3, 0x2, 0x2, 0x2, 0x1bd, 0x1bb, 
    0x3, 0x2, 0x2, 0x2, 0x1be, 0x1bf, 0x7, 0xa, 0x2, 0x2, 0x1bf, 0x1c1, 
    0x3, 0x2, 0x2, 0x2, 0x1c0, 0x1b5, 0x3, 0x2, 0x2, 0x2, 0x1c0, 0x1c1, 
    0x3, 0x2, 0x2, 0x2, 0x1c1, 0x1c2, 0x3, 0x2, 0x2, 0x2, 0x1c2, 0x1c3, 
    0x7, 0x1c, 0x2, 0x2, 0x1c3, 0x1c4, 0x7, 0x9, 0x2, 0x2, 0x1c4, 0x1c5, 
    0x5, 0x16, 0xc, 0x2, 0x1c5, 0x1c6, 0x7, 0xa, 0x2, 0x2, 0x1c6, 0x27, 
    0x3, 0x2, 0x2, 0x2, 0x1c7, 0x1c8, 0x8, 0x15, 0x1, 0x2, 0x1c8, 0x1cd, 
    0x5, 0x2a, 0x16, 0x2, 0x1c9, 0x1cb, 0x7, 0x1c, 0x2, 0x2, 0x1ca, 0x1c9, 
    0x3, 0x2, 0x2, 0x2, 0x1ca, 0x1cb, 0x3, 0x2, 0x2, 0x2, 0x1cb, 0x1cc, 
    0x3, 0x2, 0x2, 0x2, 0x1cc, 0x1ce, 0x5, 0x8e, 0x48, 0x2, 0x1cd, 0x1ca, 
    0x3, 0x2, 0x2, 0x2, 0x1cd, 0x1ce, 0x3, 0x2, 0x2, 0x2, 0x1ce, 0x1e2, 
    0x3, 0x2, 0x2, 0x2, 0x1cf, 0x1d0, 0x7, 0x9, 0x2, 0x2, 0x1d0, 0x1d1, 
    0x5, 0x16, 0xc, 0x2, 0x1d1, 0x1d6, 0x7, 0xa, 0x2, 0x2, 0x1d2, 0x1d4, 
    0x7, 0x1c, 0x2, 0x2, 0x1d3, 0x1d2, 0x3, 0x2, 0x2, 0x2, 0x1d3, 0x1d4, 
    0x3, 0x2, 0x2, 0x2, 0x1d4, 0x1d5, 0x3, 0x2, 0x2, 0x2, 0x1d5, 0x1d7, 
    0x5, 0x8e, 0x48, 0x2, 0x1d6, 0x1d3, 0x3, 0x2, 0x2, 0x2, 0x1d6, 0x1d7, 
    0x3, 0x2, 0x2, 0x2, 0x1d7, 0x1e2, 0x3, 0x2, 0x2, 0x2, 0x1d8, 0x1d9, 
    0x7, 0x9, 0x2, 0x2, 0x1d9, 0x1da, 0x5, 0x32, 0x1a, 0x2, 0x1da, 0x1df, 
    0x7, 0xa, 0x2, 0x2, 0x1db, 0x1dd, 0x7, 0x1c, 0x2, 0x2, 0x1dc, 0x1db, 
    0x3, 0x2, 0x2, 0x2, 0x1dc, 0x1dd, 0x3, 0x2, 0x2, 0x2, 0x1dd, 0x1de, 
    0x3, 0x2, 0x2, 0x2, 0x1de, 0x1e0, 0x5, 0x8e, 0x48, 0x2, 0x1df, 0x1dc, 
    0x3, 0x2, 0x2, 0x2, 0x1df, 0x1e0, 0x3, 0x2, 0x2, 0x2, 0x1e0, 0x1e2, 
    0x3, 0x2, 0x2, 0x2, 0x1e1, 0x1c7, 0x3, 0x2, 0x2, 0x2, 0x1e1, 0x1cf, 
    0x3, 0x2, 0x2, 0x2, 0x1e1, 0x1d8, 0x3, 0x2, 0x2, 0x2, 0x1e2, 0x20d, 
    0x3, 0x2, 0x2, 0x2, 0x1e3, 0x1e7, 0xc, 0x8, 0x2, 0x2, 0x1e4, 0x1e8, 
    0x7, 0x5, 0x2, 0x2, 0x1e5, 0x1e6, 0x7, 0x26, 0x2, 0x2, 0x1e6, 0x1e8, 
    0x7, 0x44, 0x2, 0x2, 0x1e7, 0x1e4, 0x3, 0x2, 0x2, 0x2, 0x1e7, 0x1e5, 
    0x3, 0x2, 0x2, 0x2, 0x1e8, 0x1e9, 0x3, 0x2, 0x2, 0x2, 0x1e9, 0x20c, 
    0x5, 0x28, 0x15, 0x9, 0x1ea, 0x1eb, 0xc, 0x7, 0x2, 0x2, 0x1eb, 0x1ed, 
    0x7, 0x4d, 0x2, 0x2, 0x1ec, 0x1ee, 0x5, 0x2c, 0x17, 0x2, 0x1ed, 0x1ec, 
    0x3, 0x2, 0x2, 0x2, 0x1ed, 0x1ee, 0x3, 0x2, 0x2, 0x2, 0x1ee, 0x1ef, 
    0x3, 0x2, 0x2, 0x2, 0x1ef, 0x1f0, 0x7, 0x44, 0x2, 0x2, 0x1f0, 0x20c, 
    0x5, 0x28, 0x15, 0x8, 0x1f1, 0x1f3, 0xc, 0x6, 0x2, 0x2, 0x1f2, 0x1f4, 
    0x5, 0x2e, 0x18, 0x2, 0x1f3, 0x1f2, 0x3, 0x2, 0x2, 0x2, 0x1f3, 0x1f4, 
    0x3, 0x2, 0x2, 0x2, 0x1f4, 0x1f6, 0x3, 0x2, 0x2, 0x2, 0x1f5, 0x1f7, 
    0x5, 0x30, 0x19, 0x2, 0x1f6, 0x1f5, 0x3, 0x2, 0x2, 0x2, 0x1f6, 0x1f7, 
    0x3, 0x2, 0x2, 0x2, 0x1f7, 0x1f8, 0x3, 0x2, 0x2, 0x2, 0x1f8, 0x1f9, 
    0x7, 0x44, 0x2, 0x2, 0x1f9, 0x1fa, 0x5, 0x28, 0x15, 0x2, 0x1fa, 0x1fb, 
    0x7, 0x54, 0x2, 0x2, 0x1fb, 0x1fc, 0x5, 0x76, 0x3c, 0x2, 0x1fc, 0x20c, 
    0x3, 0x2, 0x2, 0x2, 0x1fd, 0x1ff, 0xc, 0x5, 0x2, 0x2, 0x1fe, 0x200, 
    0x5, 0x2e, 0x18, 0x2, 0x1ff, 0x1fe, 0x3, 0x2, 0x2, 0x2, 0x1ff, 0x200, 
    0x3, 0x2, 0x2, 0x2, 0x200, 0x202, 0x3, 0x2, 0x2, 0x2, 0x201, 0x203, 
    0x5, 0x30, 0x19, 0x2, 0x202, 0x201, 0x3, 0x2, 0x2, 0x2, 0x202, 0x203, 
    0x3, 0x2, 0x2, 0x2, 0x203, 0x204, 0x3, 0x2, 0x2, 0x2, 0x204, 0x205, 
    0x7, 0x44, 0x2, 0x2, 0x205, 0x206, 0x5, 0x28, 0x15, 0x2, 0x206, 0x207, 
    0x7, 0x74, 0x2, 0x2, 0x207, 0x208, 0x7, 0x9, 0x2, 0x2, 0x208, 0x209, 
    0x5, 0x66, 0x34, 0x2, 0x209, 0x20a, 0x7, 0xa, 0x2, 0x2, 0x20a, 0x20c, 
    0x3, 0x2, 0x2, 0x2, 0x20b, 0x1e3, 0x3, 0x2, 0x2, 0x2, 0x20b, 0x1ea, 
    0x3, 0x2, 0x2, 0x2, 0x20b, 0x1f1, 0x3, 0x2, 0x2, 0x2, 0x20b, 0x1fd, 
    0x3, 0x2, 0x2, 0x2, 0x20c, 0x20f, 0x3, 0x2, 0x2, 0x2, 0x20d, 0x20b, 
    0x3, 0x2, 0x2, 0x2, 0x20d, 0x20e, 0x3, 0x2, 0x2, 0x2, 0x20e, 0x29, 0x3, 
    0x2, 0x2, 0x2, 0x20f, 0x20d, 0x3, 0x2, 0x2, 0x2, 0x210, 0x211, 0x5, 
    0x88, 0x45, 0x2, 0x211, 0x212, 0x7, 0xc, 0x2, 0x2, 0x212, 0x214, 0x3, 
    0x2, 0x2, 0x2, 0x213, 0x210, 0x3, 0x2, 0x2, 0x2, 0x213, 0x214, 0x3, 
    0x2, 0x2, 0x2, 0x214, 0x215, 0x3, 0x2, 0x2, 0x2, 0x215, 0x216, 0x5, 
    0x8a, 0x46, 0x2, 0x216, 0x217, 0x7, 0xc, 0x2, 0x2, 0x217, 0x219, 0x3, 
    0x2, 0x2, 0x2, 0x218, 0x213, 0x3, 0x2, 0x2, 0x2, 0x218, 0x219, 0x3, 
    0x2, 0x2, 0x2, 0x219, 0x21a, 0x3, 0x2, 0x2, 0x2, 0x21a, 0x21b, 0x5, 
    0x8c, 0x47, 0x2, 0x21b, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x21c, 0x222, 0x7, 
    0x3f, 0x2, 0x2, 0x21d, 0x21f, 0x9, 0x8, 0x2, 0x2, 0x21e, 0x220, 0x7, 
    0x57, 0x2, 0x2, 0x21f, 0x21e, 0x3, 0x2, 0x2, 0x2, 0x21f, 0x220, 0x3, 
    0x2, 0x2, 0x2, 0x220, 0x222, 0x3, 0x2, 0x2, 0x2, 0x221, 0x21c, 0x3, 
    0x2, 0x2, 0x2, 0x221, 0x21d, 0x3, 0x2, 0x2, 0x2, 0x222, 0x2d, 0x3, 0x2, 
    0x2, 0x2, 0x223, 0x227, 0x5, 0x2c, 0x17, 0x2, 0x224, 0x227, 0x7, 0x64, 
    0x2, 0x2, 0x225, 0x227, 0x7, 0x19, 0x2, 0x2, 0x226, 0x223, 0x3, 0x2, 
    0x2, 0x2, 0x226, 0x224, 0x3, 0x2, 0x2, 0x2, 0x226, 0x225, 0x3, 0x2, 
    0x2, 0x2, 0x227, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x228, 0x229, 0x7, 0xc6, 
    0x2, 0x2, 0x229, 0x238, 0x9, 0x9, 0x2, 0x2, 0x22a, 0x22c, 0x7, 0x8, 
    0x2, 0x2, 0x22b, 0x22a, 0x3, 0x2, 0x2, 0x2, 0x22b, 0x22c, 0x3, 0x2, 
    0x2, 0x2, 0x22c, 0x22d, 0x3, 0x2, 0x2, 0x2, 0x22d, 0x22e, 0x7, 0x9, 
    0x2, 0x2, 0x22e, 0x233, 0x5, 0x9e, 0x50, 0x2, 0x22f, 0x230, 0x7, 0x5, 
    0x2, 0x2, 0x230, 0x232, 0x5, 0x9e, 0x50, 0x2, 0x231, 0x22f, 0x3, 0x2, 
    0x2, 0x2, 0x232, 0x235, 0x3, 0x2, 0x2, 0x2, 0x233, 0x231, 0x3, 0x2, 
    0x2, 0x2, 0x233, 0x234, 0x3, 0x2, 0x2, 0x2, 0x234, 0x236, 0x3, 0x2, 
    0x2, 0x2, 0x235, 0x233, 0x3, 0x2, 0x2, 0x2, 0x236, 0x237, 0x7, 0xa, 
    0x2, 0x2, 0x237, 0x239, 0x3, 0x2, 0x2, 0x2, 0x238, 0x22b, 0x3, 0x2, 
    0x2, 0x2, 0x238, 0x239, 0x3, 0x2, 0x2, 0x2, 0x239, 0x23a, 0x3, 0x2, 
    0x2, 0x2, 0x23a, 0x23b, 0x7, 0xc7, 0x2, 0x2, 0x23b, 0x31, 0x3, 0x2, 
    0x2, 0x2, 0x23c, 0x23d, 0x7, 0x75, 0x2, 0x2, 0x23d, 0x23e, 0x7, 0x9, 
    0x2, 0x2, 0x23e, 0x23f, 0x5, 0x72, 0x3a, 0x2, 0x23f, 0x247, 0x7, 0xa, 
    0x2, 0x2, 0x240, 0x241, 0x7, 0x5, 0x2, 0x2, 0x241, 0x242, 0x7, 0x9, 
    0x2, 0x2, 0x242, 0x243, 0x5, 0x72, 0x3a, 0x2, 0x243, 0x244, 0x7, 0xa, 
    0x2, 0x2, 0x244, 0x246, 0x3, 0x2, 0x2, 0x2, 0x245, 0x240, 0x3, 0x2, 
    0x2, 0x2, 0x246, 0x249, 0x3, 0x2, 0x2, 0x2, 0x247, 0x245, 0x3, 0x2, 
    0x2, 0x2, 0x247, 0x248, 0x3, 0x2, 0x2, 0x2, 0x248, 0x33, 0x3, 0x2, 0x2, 
    0x2, 0x249, 0x247, 0x3, 0x2, 0x2, 0x2, 0x24a, 0x24b, 0x7, 0x79, 0x2, 
    0x2, 0x24b, 0x250, 0x5, 0x26, 0x14, 0x2, 0x24c, 0x24d, 0x7, 0x5, 0x2, 
    0x2, 0x24d, 0x24f, 0x5, 0x26, 0x14, 0x2, 0x24e, 0x24c, 0x3, 0x2, 0x2, 
    0x2, 0x24f, 0x252, 0x3, 0x2, 0x2, 0x2, 0x250, 0x24e, 0x3, 0x2, 0x2, 
    0x2, 0x250, 0x251, 0x3, 0x2, 0x2, 0x2, 0x251, 0x254, 0x3, 0x2, 0x2, 
    0x2, 0x252, 0x250, 0x3, 0x2, 0x2, 0x2, 0x253, 0x24a, 0x3, 0x2, 0x2, 
    0x2, 0x253, 0x254, 0x3, 0x2, 0x2, 0x2, 0x254, 0x255, 0x3, 0x2, 0x2, 
    0x2, 0x255, 0x257, 0x7, 0x40, 0x2, 0x2, 0x256, 0x258, 0x7, 0x42, 0x2, 
    0x2, 0x257, 0x256, 0x3, 0x2, 0x2, 0x2, 0x257, 0x258, 0x3, 0x2, 0x2, 
    0x2, 0x258, 0x259, 0x3, 0x2, 0x2, 0x2, 0x259, 0x265, 0x5, 0x2a, 0x16, 
    0x2, 0x25a, 0x25b, 0x7, 0x9, 0x2, 0x2, 0x25b, 0x260, 0x5, 0x90, 0x49, 
    0x2, 0x25c, 0x25d, 0x7, 0x5, 0x2, 0x2, 0x25d, 0x25f, 0x5, 0x90, 0x49, 
    0x2, 0x25e, 0x25c, 0x3, 0x2, 0x2, 0x2, 0x25f, 0x262, 0x3, 0x2, 0x2, 
    0x2, 0x260, 0x25e, 0x3, 0x2, 0x2, 0x2, 0x260, 0x261, 0x3, 0x2, 0x2, 
    0x2, 0x261, 0x263, 0x3, 0x2, 0x2, 0x2, 0x262, 0x260, 0x3, 0x2, 0x2, 
    0x2, 0x263, 0x264, 0x7, 0xa, 0x2, 0x2, 0x264, 0x266, 0x3, 0x2, 0x2, 
    0x2, 0x265, 0x25a, 0x3, 0x2, 0x2, 0x2, 0x265, 0x266, 0x3, 0x2, 0x2, 
    0x2, 0x266, 0x26a, 0x3, 0x2, 0x2, 0x2, 0x267, 0x26b, 0x5, 0x32, 0x1a, 
    0x2, 0x268, 0x26b, 0x5, 0x1c, 0xf, 0x2, 0x269, 0x26b, 0x5, 0x20, 0x11, 
    0x2, 0x26a, 0x267, 0x3, 0x2, 0x2, 0x2, 0x26a, 0x268, 0x3, 0x2, 0x2, 
    0x2, 0x26a, 0x269, 0x3, 0x2, 0x2, 0x2, 0x26b, 0x35, 0x3, 0x2, 0x2, 0x2, 
    0x26c, 0x26d, 0x7, 0x79, 0x2, 0x2, 0x26d, 0x272, 0x5, 0x26, 0x14, 0x2, 
    0x26e, 0x26f, 0x7, 0x5, 0x2, 0x2, 0x26f, 0x271, 0x5, 0x26, 0x14, 0x2, 
    0x270, 0x26e, 0x3, 0x2, 0x2, 0x2, 0x271, 0x274, 0x3, 0x2, 0x2, 0x2, 
    0x272, 0x270, 0x3, 0x2, 0x2, 0x2, 0x272, 0x273, 0x3, 0x2, 0x2, 0x2, 
    0x273, 0x276, 0x3, 0x2, 0x2, 0x2, 0x274, 0x272, 0x3, 0x2, 0x2, 0x2, 
    0x275, 0x26c, 0x3, 0x2, 0x2, 0x2, 0x275, 0x276, 0x3, 0x2, 0x2, 0x2, 
    0x276, 0x277, 0x3, 0x2, 0x2, 0x2, 0x277, 0x279, 0x7, 0x2d, 0x2, 0x2, 
    0x278, 0x27a, 0x7, 0x3a, 0x2, 0x2, 0x279, 0x278, 0x3, 0x2, 0x2, 0x2, 
    0x279, 0x27a, 0x3, 0x2, 0x2, 0x2, 0x27a, 0x27b, 0x3, 0x2, 0x2, 0x2, 
    0x27b, 0x280, 0x5, 0x2a, 0x16, 0x2, 0x27c, 0x27d, 0x7, 0x76, 0x2, 0x2, 
    0x27d, 0x281, 0x5, 0x74, 0x3b, 0x2, 0x27e, 0x27f, 0x7, 0x3a, 0x2, 0x2, 
    0x27f, 0x281, 0x5, 0x28, 0x15, 0x2, 0x280, 0x27c, 0x3, 0x2, 0x2, 0x2, 
    0x280, 0x27e, 0x3, 0x2, 0x2, 0x2, 0x280, 0x281, 0x3, 0x2, 0x2, 0x2, 
    0x281, 0x37, 0x3, 0x2, 0x2, 0x2, 0x282, 0x283, 0x7, 0x79, 0x2, 0x2, 
    0x283, 0x288, 0x5, 0x26, 0x14, 0x2, 0x284, 0x285, 0x7, 0x5, 0x2, 0x2, 
    0x285, 0x287, 0x5, 0x26, 0x14, 0x2, 0x286, 0x284, 0x3, 0x2, 0x2, 0x2, 
    0x287, 0x28a, 0x3, 0x2, 0x2, 0x2, 0x288, 0x286, 0x3, 0x2, 0x2, 0x2, 
    0x288, 0x289, 0x3, 0x2, 0x2, 0x2, 0x289, 0x28c, 0x3, 0x2, 0x2, 0x2, 
    0x28a, 0x288, 0x3, 0x2, 0x2, 0x2, 0x28b, 0x282, 0x3, 0x2, 0x2, 0x2, 
    0x28b, 0x28c, 0x3, 0x2, 0x2, 0x2, 0x28c, 0x28d, 0x3, 0x2, 0x2, 0x2, 
    0x28d, 0x28e, 0x7, 0x73, 0x2, 0x2, 0x28e, 0x28f, 0x5, 0x2a, 0x16, 0x2, 
    0x28f, 0x290, 0x7, 0x65, 0x2, 0x2, 0x290, 0x291, 0x5, 0x90, 0x49, 0x2, 
    0x291, 0x292, 0x7, 0x8, 0x2, 0x2, 0x292, 0x29a, 0x5, 0x7a, 0x3e, 0x2, 
    0x293, 0x294, 0x7, 0x5, 0x2, 0x2, 0x294, 0x295, 0x5, 0x90, 0x49, 0x2, 
    0x295, 0x296, 0x7, 0x8, 0x2, 0x2, 0x296, 0x297, 0x5, 0x7a, 0x3e, 0x2, 
    0x297, 0x299, 0x3, 0x2, 0x2, 0x2, 0x298, 0x293, 0x3, 0x2, 0x2, 0x2, 
    0x299, 0x29c, 0x3, 0x2, 0x2, 0x2, 0x29a, 0x298, 0x3, 0x2, 0x2, 0x2, 
    0x29a, 0x29b, 0x3, 0x2, 0x2, 0x2, 0x29b, 0x2a1, 0x3, 0x2, 0x2, 0x2, 
    0x29c, 0x29a, 0x3, 0x2, 0x2, 0x2, 0x29d, 0x29e, 0x7, 0x76, 0x2, 0x2, 
    0x29e, 0x2a2, 0x5, 0x74, 0x3b, 0x2, 0x29f, 0x2a0, 0x7, 0x3a, 0x2, 0x2, 
    0x2a0, 0x2a2, 0x5, 0x28, 0x15, 0x2, 0x2a1, 0x29d, 0x3, 0x2, 0x2, 0x2, 
    0x2a1, 0x29f, 0x3, 0x2, 0x2, 0x2, 0x2a1, 0x2a2, 0x3, 0x2, 0x2, 0x2, 
    0x2a2, 0x39, 0x3, 0x2, 0x2, 0x2, 0x2a3, 0x2a6, 0x5, 0x3c, 0x1f, 0x2, 
    0x2a4, 0x2a6, 0x5, 0x3e, 0x20, 0x2, 0x2a5, 0x2a3, 0x3, 0x2, 0x2, 0x2, 
    0x2a5, 0x2a4, 0x3, 0x2, 0x2, 0x2, 0x2a6, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x2a7, 
    0x2a9, 0x7, 0x25, 0x2, 0x2, 0x2a8, 0x2aa, 0x7, 0x5f, 0x2, 0x2, 0x2a9, 
    0x2a8, 0x3, 0x2, 0x2, 0x2, 0x2a9, 0x2aa, 0x3, 0x2, 0x2, 0x2, 0x2aa, 
    0x2ab, 0x3, 0x2, 0x2, 0x2, 0x2ab, 0x2af, 0x7, 0x6a, 0x2, 0x2, 0x2ac, 
    0x2ad, 0x7, 0x98, 0x2, 0x2, 0x2ad, 0x2ae, 0x7, 0x50, 0x2, 0x2, 0x2ae, 
    0x2b0, 0x7, 0x97, 0x2, 0x2, 0x2af, 0x2ac, 0x3, 0x2, 0x2, 0x2, 0x2af, 
    0x2b0, 0x3, 0x2, 0x2, 0x2, 0x2b0, 0x2b1, 0x3, 0x2, 0x2, 0x2, 0x2b1, 
    0x2cd, 0x5, 0x2a, 0x16, 0x2, 0x2b2, 0x2ce, 0x5, 0x44, 0x23, 0x2, 0x2b3, 
    0x2b5, 0x5, 0x44, 0x23, 0x2, 0x2b4, 0x2b3, 0x3, 0x2, 0x2, 0x2, 0x2b4, 
    0x2b5, 0x3, 0x2, 0x2, 0x2, 0x2b5, 0x2b7, 0x3, 0x2, 0x2, 0x2, 0x2b6, 
    0x2b8, 0x7, 0x1c, 0x2, 0x2, 0x2b7, 0x2b6, 0x3, 0x2, 0x2, 0x2, 0x2b7, 
    0x2b8, 0x3, 0x2, 0x2, 0x2, 0x2b8, 0x2b9, 0x3, 0x2, 0x2, 0x2, 0x2b9, 
    0x2bf, 0x5, 0x56, 0x2c, 0x2, 0x2ba, 0x2bc, 0x7, 0x79, 0x2, 0x2, 0x2bb, 
    0x2bd, 0x7, 0x4e, 0x2, 0x2, 0x2bc, 0x2bb, 0x3, 0x2, 0x2, 0x2, 0x2bc, 
    0x2bd, 0x3, 0x2, 0x2, 0x2, 0x2bd, 0x2be, 0x3, 0x2, 0x2, 0x2, 0x2be, 
    0x2c0, 0x7, 0x2b, 0x2, 0x2, 0x2bf, 0x2ba, 0x3, 0x2, 0x2, 0x2, 0x2bf, 
    0x2c0, 0x3, 0x2, 0x2, 0x2, 0x2c0, 0x2ce, 0x3, 0x2, 0x2, 0x2, 0x2c1, 
    0x2c3, 0x5, 0x4a, 0x26, 0x2, 0x2c2, 0x2c4, 0x7, 0x1c, 0x2, 0x2, 0x2c3, 
    0x2c2, 0x3, 0x2, 0x2, 0x2, 0x2c3, 0x2c4, 0x3, 0x2, 0x2, 0x2, 0x2c4, 
    0x2c5, 0x3, 0x2, 0x2, 0x2, 0x2c5, 0x2cb, 0x5, 0x56, 0x2c, 0x2, 0x2c6, 
    0x2c8, 0x7, 0x79, 0x2, 0x2, 0x2c7, 0x2c9, 0x7, 0x4e, 0x2, 0x2, 0x2c8, 
    0x2c7, 0x3, 0x2, 0x2, 0x2, 0x2c8, 0x2c9, 0x3, 0x2, 0x2, 0x2, 0x2c9, 
    0x2ca, 0x3, 0x2, 0x2, 0x2, 0x2ca, 0x2cc, 0x7, 0x2b, 0x2, 0x2, 0x2cb, 
    0x2c6, 0x3, 0x2, 0x2, 0x2, 0x2cb, 0x2cc, 0x3, 0x2, 0x2, 0x2, 0x2cc, 
    0x2ce, 0x3, 0x2, 0x2, 0x2, 0x2cd, 0x2b2, 0x3, 0x2, 0x2, 0x2, 0x2cd, 
    0x2b4, 0x3, 0x2, 0x2, 0x2, 0x2cd, 0x2c1, 0x3, 0x2, 0x2, 0x2, 0x2ce, 
    0x3d, 0x3, 0x2, 0x2, 0x2, 0x2cf, 0x2d1, 0x7, 0x25, 0x2, 0x2, 0x2d0, 
    0x2d2, 0x7, 0x72, 0x2, 0x2, 0x2d1, 0x2d0, 0x3, 0x2, 0x2, 0x2, 0x2d1, 
    0x2d2, 0x3, 0x2, 0x2, 0x2, 0x2d2, 0x2d3, 0x3, 0x2, 0x2, 0x2, 0x2d3, 
    0x2d7, 0x9, 0xa, 0x2, 0x2, 0x2d4, 0x2d5, 0x7, 0x98, 0x2, 0x2, 0x2d5, 
    0x2d6, 0x7, 0x50, 0x2, 0x2, 0x2d6, 0x2d8, 0x7, 0x97, 0x2, 0x2, 0x2d7, 
    0x2d4, 0x3, 0x2, 0x2, 0x2, 0x2d7, 0x2d8, 0x3, 0x2, 0x2, 0x2, 0x2d8, 
    0x2d9, 0x3, 0x2, 0x2, 0x2, 0x2d9, 0x2da, 0x5, 0x96, 0x4c, 0x2, 0x2da, 
    0x2db, 0x7, 0x54, 0x2, 0x2, 0x2db, 0x2de, 0x5, 0x2a, 0x16, 0x2, 0x2dc, 
    0x2dd, 0x7, 0x74, 0x2, 0x2, 0x2dd, 0x2df, 0x5, 0x50, 0x29, 0x2, 0x2de, 
    0x2dc, 0x3, 0x2, 0x2, 0x2, 0x2de, 0x2df, 0x3, 0x2, 0x2, 0x2, 0x2df, 
    0x2e0, 0x3, 0x2, 0x2, 0x2, 0x2e0, 0x2e1, 0x7, 0x9, 0x2, 0x2, 0x2e1, 
    0x2e6, 0x5, 0x7a, 0x3e, 0x2, 0x2e2, 0x2e3, 0x7, 0x5, 0x2, 0x2, 0x2e3, 
    0x2e5, 0x5, 0x7a, 0x3e, 0x2, 0x2e4, 0x2e2, 0x3, 0x2, 0x2, 0x2, 0x2e5, 
    0x2e8, 0x3, 0x2, 0x2, 0x2, 0x2e6, 0x2e4, 0x3, 0x2, 0x2, 0x2, 0x2e6, 
    0x2e7, 0x3, 0x2, 0x2, 0x2, 0x2e7, 0x2e9, 0x3, 0x2, 0x2, 0x2, 0x2e8, 
    0x2e6, 0x3, 0x2, 0x2, 0x2, 0x2e9, 0x2ec, 0x7, 0xa, 0x2, 0x2, 0x2ea, 
    0x2eb, 0x7, 0x79, 0x2, 0x2, 0x2eb, 0x2ed, 0x5, 0x6e, 0x38, 0x2, 0x2ec, 
    0x2ea, 0x3, 0x2, 0x2, 0x2, 0x2ec, 0x2ed, 0x3, 0x2, 0x2, 0x2, 0x2ed, 
    0x3f, 0x3, 0x2, 0x2, 0x2, 0x2ee, 0x2ef, 0x7, 0x31, 0x2, 0x2, 0x2ef, 
    0x2f2, 0x7, 0x6a, 0x2, 0x2, 0x2f0, 0x2f1, 0x7, 0x98, 0x2, 0x2, 0x2f1, 
    0x2f3, 0x7, 0x97, 0x2, 0x2, 0x2f2, 0x2f0, 0x3, 0x2, 0x2, 0x2, 0x2f2, 
    0x2f3, 0x3, 0x2, 0x2, 0x2, 0x2f3, 0x2f4, 0x3, 0x2, 0x2, 0x2, 0x2f4, 
    0x300, 0x5, 0x2a, 0x16, 0x2, 0x2f5, 0x2f6, 0x7, 0x31, 0x2, 0x2, 0x2f6, 
    0x2f9, 0x9, 0xa, 0x2, 0x2, 0x2f7, 0x2f8, 0x7, 0x98, 0x2, 0x2, 0x2f8, 
    0x2fa, 0x7, 0x97, 0x2, 0x2, 0x2f9, 0x2f7, 0x3, 0x2, 0x2, 0x2, 0x2f9, 
    0x2fa, 0x3, 0x2, 0x2, 0x2, 0x2fa, 0x2fb, 0x3, 0x2, 0x2, 0x2, 0x2fb, 
    0x2fc, 0x5, 0x96, 0x4c, 0x2, 0x2fc, 0x2fd, 0x7, 0x54, 0x2, 0x2, 0x2fd, 
    0x2fe, 0x5, 0x2a, 0x16, 0x2, 0x2fe, 0x300, 0x3, 0x2, 0x2, 0x2, 0x2ff, 
    0x2ee, 0x3, 0x2, 0x2, 0x2, 0x2ff, 0x2f5, 0x3, 0x2, 0x2, 0x2, 0x300, 
    0x41, 0x3, 0x2, 0x2, 0x2, 0x301, 0x303, 0x7, 0x6e, 0x2, 0x2, 0x302, 
    0x304, 0x7, 0x6a, 0x2, 0x2, 0x303, 0x302, 0x3, 0x2, 0x2, 0x2, 0x303, 
    0x304, 0x3, 0x2, 0x2, 0x2, 0x304, 0x305, 0x3, 0x2, 0x2, 0x2, 0x305, 
    0x306, 0x5, 0x2a, 0x16, 0x2, 0x306, 0x43, 0x3, 0x2, 0x2, 0x2, 0x307, 
    0x30b, 0x7, 0x9, 0x2, 0x2, 0x308, 0x30c, 0x5, 0x48, 0x25, 0x2, 0x309, 
    0x30c, 0x5, 0x4c, 0x27, 0x2, 0x30a, 0x30c, 0x5, 0x4e, 0x28, 0x2, 0x30b, 
    0x308, 0x3, 0x2, 0x2, 0x2, 0x30b, 0x309, 0x3, 0x2, 0x2, 0x2, 0x30b, 
    0x30a, 0x3, 0x2, 0x2, 0x2, 0x30c, 0x315, 0x3, 0x2, 0x2, 0x2, 0x30d, 
    0x311, 0x7, 0x5, 0x2, 0x2, 0x30e, 0x312, 0x5, 0x48, 0x25, 0x2, 0x30f, 
    0x312, 0x5, 0x4c, 0x27, 0x2, 0x310, 0x312, 0x5, 0x4e, 0x28, 0x2, 0x311, 
    0x30e, 0x3, 0x2, 0x2, 0x2, 0x311, 0x30f, 0x3, 0x2, 0x2, 0x2, 0x311, 
    0x310, 0x3, 0x2, 0x2, 0x2, 0x312, 0x314, 0x3, 0x2, 0x2, 0x2, 0x313, 
    0x30d, 0x3, 0x2, 0x2, 0x2, 0x314, 0x317, 0x3, 0x2, 0x2, 0x2, 0x315, 
    0x313, 0x3, 0x2, 0x2, 0x2, 0x315, 0x316, 0x3, 0x2, 0x2, 0x2, 0x316, 
    0x318, 0x3, 0x2, 0x2, 0x2, 0x317, 0x315, 0x3, 0x2, 0x2, 0x2, 0x318, 
    0x31a, 0x7, 0xa, 0x2, 0x2, 0x319, 0x31b, 0x7, 0x5f, 0x2, 0x2, 0x31a, 
    0x319, 0x3, 0x2, 0x2, 0x2, 0x31a, 0x31b, 0x3, 0x2, 0x2, 0x2, 0x31b, 
    0x31d, 0x3, 0x2, 0x2, 0x2, 0x31c, 0x31e, 0x5, 0x46, 0x24, 0x2, 0x31d, 
    0x31c, 0x3, 0x2, 0x2, 0x2, 0x31d, 0x31e, 0x3, 0x2, 0x2, 0x2, 0x31e, 
    0x321, 0x3, 0x2, 0x2, 0x2, 0x31f, 0x320, 0x7, 0x79, 0x2, 0x2, 0x320, 
    0x322, 0x5, 0x6e, 0x38, 0x2, 0x321, 0x31f, 0x3, 0x2, 0x2, 0x2, 0x321, 
    0x322, 0x3, 0x2, 0x2, 0x2, 0x322, 0x45, 0x3, 0x2, 0x2, 0x2, 0x323, 0x325, 
    0x7, 0x5a, 0x2, 0x2, 0x324, 0x326, 0x7, 0x21, 0x2, 0x2, 0x325, 0x324, 
    0x3, 0x2, 0x2, 0x2, 0x325, 0x326, 0x3, 0x2, 0x2, 0x2, 0x326, 0x328, 
    0x3, 0x2, 0x2, 0x2, 0x327, 0x329, 0x9, 0xb, 0x2, 0x2, 0x328, 0x327, 
    0x3, 0x2, 0x2, 0x2, 0x328, 0x329, 0x3, 0x2, 0x2, 0x2, 0x329, 0x32b, 
    0x3, 0x2, 0x2, 0x2, 0x32a, 0x32c, 0x7, 0x54, 0x2, 0x2, 0x32b, 0x32a, 
    0x3, 0x2, 0x2, 0x2, 0x32b, 0x32c, 0x3, 0x2, 0x2, 0x2, 0x32c, 0x32d, 
    0x3, 0x2, 0x2, 0x2, 0x32d, 0x32e, 0x7, 0x9, 0x2, 0x2, 0x32e, 0x333, 
    0x5, 0x7a, 0x3e, 0x2, 0x32f, 0x330, 0x7, 0x5, 0x2, 0x2, 0x330, 0x332, 
    0x5, 0x7a, 0x3e, 0x2, 0x331, 0x32f, 0x3, 0x2, 0x2, 0x2, 0x332, 0x335, 
    0x3, 0x2, 0x2, 0x2, 0x333, 0x331, 0x3, 0x2, 0x2, 0x2, 0x333, 0x334, 
    0x3, 0x2, 0x2, 0x2, 0x334, 0x336, 0x3, 0x2, 0x2, 0x2, 0x335, 0x333, 
    0x3, 0x2, 0x2, 0x2, 0x336, 0x337, 0x7, 0xa, 0x2, 0x2, 0x337, 0x47, 0x3, 
    0x2, 0x2, 0x2, 0x338, 0x339, 0x5, 0x90, 0x49, 0x2, 0x339, 0x34b, 0x5, 
    0x52, 0x2a, 0x2, 0x33a, 0x33c, 0x7, 0x50, 0x2, 0x2, 0x33b, 0x33a, 0x3, 
    0x2, 0x2, 0x2, 0x33b, 0x33c, 0x3, 0x2, 0x2, 0x2, 0x33c, 0x33d, 0x3, 
    0x2, 0x2, 0x2, 0x33d, 0x34a, 0x7, 0x52, 0x2, 0x2, 0x33e, 0x33f, 0x7, 
    0x5d, 0x2, 0x2, 0x33f, 0x34a, 0x7, 0x45, 0x2, 0x2, 0x340, 0x342, 0x7, 
    0x72, 0x2, 0x2, 0x341, 0x343, 0x7, 0x45, 0x2, 0x2, 0x342, 0x341, 0x3, 
    0x2, 0x2, 0x2, 0x342, 0x343, 0x3, 0x2, 0x2, 0x2, 0x343, 0x34a, 0x3, 
    0x2, 0x2, 0x2, 0x344, 0x345, 0x7, 0x72, 0x2, 0x2, 0x345, 0x347, 0x7, 
    0x4c, 0x2, 0x2, 0x346, 0x348, 0x7, 0x45, 0x2, 0x2, 0x347, 0x346, 0x3, 
    0x2, 0x2, 0x2, 0x347, 0x348, 0x3, 0x2, 0x2, 0x2, 0x348, 0x34a, 0x3, 
    0x2, 0x2, 0x2, 0x349, 0x33b, 0x3, 0x2, 0x2, 0x2, 0x349, 0x33e, 0x3, 
    0x2, 0x2, 0x2, 0x349, 0x340, 0x3, 0x2, 0x2, 0x2, 0x349, 0x344, 0x3, 
    0x2, 0x2, 0x2, 0x34a, 0x34d, 0x3, 0x2, 0x2, 0x2, 0x34b, 0x349, 0x3, 
    0x2, 0x2, 0x2, 0x34b, 0x34c, 0x3, 0x2, 0x2, 0x2, 0x34c, 0x350, 0x3, 
    0x2, 0x2, 0x2, 0x34d, 0x34b, 0x3, 0x2, 0x2, 0x2, 0x34e, 0x34f, 0x7, 
    0x79, 0x2, 0x2, 0x34f, 0x351, 0x5, 0x6e, 0x38, 0x2, 0x350, 0x34e, 0x3, 
    0x2, 0x2, 0x2, 0x350, 0x351, 0x3, 0x2, 0x2, 0x2, 0x351, 0x49, 0x3, 0x2, 
    0x2, 0x2, 0x352, 0x353, 0x7, 0x9, 0x2, 0x2, 0x353, 0x358, 0x5, 0x90, 
    0x49, 0x2, 0x354, 0x355, 0x7, 0x5, 0x2, 0x2, 0x355, 0x357, 0x5, 0x90, 
    0x49, 0x2, 0x356, 0x354, 0x3, 0x2, 0x2, 0x2, 0x357, 0x35a, 0x3, 0x2, 
    0x2, 0x2, 0x358, 0x356, 0x3, 0x2, 0x2, 0x2, 0x358, 0x359, 0x3, 0x2, 
    0x2, 0x2, 0x359, 0x35b, 0x3, 0x2, 0x2, 0x2, 0x35a, 0x358, 0x3, 0x2, 
    0x2, 0x2, 0x35b, 0x35c, 0x7, 0xa, 0x2, 0x2, 0x35c, 0x4b, 0x3, 0x2, 0x2, 
    0x2, 0x35d, 0x35e, 0x7, 0x5d, 0x2, 0x2, 0x35e, 0x36b, 0x7, 0x45, 0x2, 
    0x2, 0x35f, 0x36c, 0x5, 0x7a, 0x3e, 0x2, 0x360, 0x361, 0x7, 0x9, 0x2, 
    0x2, 0x361, 0x366, 0x5, 0x7a, 0x3e, 0x2, 0x362, 0x363, 0x7, 0x5, 0x2, 
    0x2, 0x363, 0x365, 0x5, 0x7a, 0x3e, 0x2, 0x364, 0x362, 0x3, 0x2, 0x2, 
    0x2, 0x365, 0x368, 0x3, 0x2, 0x2, 0x2, 0x366, 0x364, 0x3, 0x2, 0x2, 
    0x2, 0x366, 0x367, 0x3, 0x2, 0x2, 0x2, 0x367, 0x369, 0x3, 0x2, 0x2, 
    0x2, 0x368, 0x366, 0x3, 0x2, 0x2, 0x2, 0x369, 0x36a, 0x7, 0xa, 0x2, 
    0x2, 0x36a, 0x36c, 0x3, 0x2, 0x2, 0x2, 0x36b, 0x35f, 0x3, 0x2, 0x2, 
    0x2, 0x36b, 0x360, 0x3, 0x2, 0x2, 0x2, 0x36c, 0x381, 0x3, 0x2, 0x2, 
    0x2, 0x36d, 0x36f, 0x7, 0x72, 0x2, 0x2, 0x36e, 0x36d, 0x3, 0x2, 0x2, 
    0x2, 0x36e, 0x36f, 0x3, 0x2, 0x2, 0x2, 0x36f, 0x370, 0x3, 0x2, 0x2, 
    0x2, 0x370, 0x371, 0x7, 0x4c, 0x2, 0x2, 0x371, 0x37e, 0x7, 0x45, 0x2, 
    0x2, 0x372, 0x37f, 0x5, 0x7a, 0x3e, 0x2, 0x373, 0x374, 0x7, 0x9, 0x2, 
    0x2, 0x374, 0x379, 0x5, 0x7a, 0x3e, 0x2, 0x375, 0x376, 0x7, 0x5, 0x2, 
    0x2, 0x376, 0x378, 0x5, 0x7a, 0x3e, 0x2, 0x377, 0x375, 0x3, 0x2, 0x2, 
    0x2, 0x378, 0x37b, 0x3, 0x2, 0x2, 0x2, 0x379, 0x377, 0x3, 0x2, 0x2, 
    0x2, 0x379, 0x37a, 0x3, 0x2, 0x2, 0x2, 0x37a, 0x37c, 0x3, 0x2, 0x2, 
    0x2, 0x37b, 0x379, 0x3, 0x2, 0x2, 0x2, 0x37c, 0x37d, 0x7, 0xa, 0x2, 
    0x2, 0x37d, 0x37f, 0x3, 0x2, 0x2, 0x2, 0x37e, 0x372, 0x3, 0x2, 0x2, 
    0x2, 0x37e, 0x373, 0x3, 0x2, 0x2, 0x2, 0x37f, 0x381, 0x3, 0x2, 0x2, 
    0x2, 0x380, 0x35d, 0x3, 0x2, 0x2, 0x2, 0x380, 0x36e, 0x3, 0x2, 0x2, 
    0x2, 0x381, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x382, 0x384, 0x7, 0x72, 0x2, 
    0x2, 0x383, 0x382, 0x3, 0x2, 0x2, 0x2, 0x383, 0x384, 0x3, 0x2, 0x2, 
    0x2, 0x384, 0x385, 0x3, 0x2, 0x2, 0x2, 0x385, 0x387, 0x9, 0xa, 0x2, 
    0x2, 0x386, 0x388, 0x5, 0x96, 0x4c, 0x2, 0x387, 0x386, 0x3, 0x2, 0x2, 
    0x2, 0x387, 0x388, 0x3, 0x2, 0x2, 0x2, 0x388, 0x38a, 0x3, 0x2, 0x2, 
    0x2, 0x389, 0x38b, 0x7, 0x54, 0x2, 0x2, 0x38a, 0x389, 0x3, 0x2, 0x2, 
    0x2, 0x38a, 0x38b, 0x3, 0x2, 0x2, 0x2, 0x38b, 0x398, 0x3, 0x2, 0x2, 
    0x2, 0x38c, 0x399, 0x5, 0x7a, 0x3e, 0x2, 0x38d, 0x38e, 0x7, 0x9, 0x2, 
    0x2, 0x38e, 0x393, 0x5, 0x7a, 0x3e, 0x2, 0x38f, 0x390, 0x7, 0x5, 0x2, 
    0x2, 0x390, 0x392, 0x5, 0x7a, 0x3e, 0x2, 0x391, 0x38f, 0x3, 0x2, 0x2, 
    0x2, 0x392, 0x395, 0x3, 0x2, 0x2, 0x2, 0x393, 0x391, 0x3, 0x2, 0x2, 
    0x2, 0x393, 0x394, 0x3, 0x2, 0x2, 0x2, 0x394, 0x396, 0x3, 0x2, 0x2, 
    0x2, 0x395, 0x393, 0x3, 0x2, 0x2, 0x2, 0x396, 0x397, 0x7, 0xa, 0x2, 
    0x2, 0x397, 0x399, 0x3, 0x2, 0x2, 0x2, 0x398, 0x38c, 0x3, 0x2, 0x2, 
    0x2, 0x398, 0x38d, 0x3, 0x2, 0x2, 0x2, 0x399, 0x39c, 0x3, 0x2, 0x2, 
    0x2, 0x39a, 0x39b, 0x7, 0x74, 0x2, 0x2, 0x39b, 0x39d, 0x5, 0x50, 0x29, 
    0x2, 0x39c, 0x39a, 0x3, 0x2, 0x2, 0x2, 0x39c, 0x39d, 0x3, 0x2, 0x2, 
    0x2, 0x39d, 0x3a0, 0x3, 0x2, 0x2, 0x2, 0x39e, 0x39f, 0x7, 0x79, 0x2, 
    0x2, 0x39f, 0x3a1, 0x5, 0x6e, 0x38, 0x2, 0x3a0, 0x39e, 0x3, 0x2, 0x2, 
    0x2, 0x3a0, 0x3a1, 0x3, 0x2, 0x2, 0x2, 0x3a1, 0x4f, 0x3, 0x2, 0x2, 0x2, 
    0x3a2, 0x3a3, 0x9, 0xc, 0x2, 0x2, 0x3a3, 0x51, 0x3, 0x2, 0x2, 0x2, 0x3a4, 
    0x3a7, 0x5, 0x54, 0x2b, 0x2, 0x3a5, 0x3a6, 0x7, 0x74, 0x2, 0x2, 0x3a6, 
    0x3a8, 0x5, 0x82, 0x42, 0x2, 0x3a7, 0x3a5, 0x3, 0x2, 0x2, 0x2, 0x3a7, 
    0x3a8, 0x3, 0x2, 0x2, 0x2, 0x3a8, 0x3ab, 0x3, 0x2, 0x2, 0x2, 0x3a9, 
    0x3ab, 0x5, 0x82, 0x42, 0x2, 0x3aa, 0x3a4, 0x3, 0x2, 0x2, 0x2, 0x3aa, 
    0x3a9, 0x3, 0x2, 0x2, 0x2, 0x3ab, 0x53, 0x3, 0x2, 0x2, 0x2, 0x3ac, 0x408, 
    0x7, 0x7b, 0x2, 0x2, 0x3ad, 0x3b5, 0x7, 0x7c, 0x2, 0x2, 0x3ae, 0x3af, 
    0x7, 0x9, 0x2, 0x2, 0x3af, 0x3b1, 0x5, 0xa4, 0x53, 0x2, 0x3b0, 0x3b2, 
    0x9, 0xd, 0x2, 0x2, 0x3b1, 0x3b0, 0x3, 0x2, 0x2, 0x2, 0x3b1, 0x3b2, 
    0x3, 0x2, 0x2, 0x2, 0x3b2, 0x3b3, 0x3, 0x2, 0x2, 0x2, 0x3b3, 0x3b4, 
    0x7, 0xa, 0x2, 0x2, 0x3b4, 0x3b6, 0x3, 0x2, 0x2, 0x2, 0x3b5, 0x3ae, 
    0x3, 0x2, 0x2, 0x2, 0x3b5, 0x3b6, 0x3, 0x2, 0x2, 0x2, 0x3b6, 0x408, 
    0x3, 0x2, 0x2, 0x2, 0x3b7, 0x3c6, 0x7, 0x7d, 0x2, 0x2, 0x3b8, 0x3b9, 
    0x7, 0x9, 0x2, 0x2, 0x3b9, 0x3bb, 0x5, 0xa4, 0x53, 0x2, 0x3ba, 0x3bc, 
    0x7, 0x8f, 0x2, 0x2, 0x3bb, 0x3ba, 0x3, 0x2, 0x2, 0x2, 0x3bb, 0x3bc, 
    0x3, 0x2, 0x2, 0x2, 0x3bc, 0x3c2, 0x3, 0x2, 0x2, 0x2, 0x3bd, 0x3be, 
    0x7, 0x5, 0x2, 0x2, 0x3be, 0x3c0, 0x5, 0xa6, 0x54, 0x2, 0x3bf, 0x3c1, 
    0x7, 0x8f, 0x2, 0x2, 0x3c0, 0x3bf, 0x3, 0x2, 0x2, 0x2, 0x3c0, 0x3c1, 
    0x3, 0x2, 0x2, 0x2, 0x3c1, 0x3c3, 0x3, 0x2, 0x2, 0x2, 0x3c2, 0x3bd, 
    0x3, 0x2, 0x2, 0x2, 0x3c2, 0x3c3, 0x3, 0x2, 0x2, 0x2, 0x3c3, 0x3c4, 
    0x3, 0x2, 0x2, 0x2, 0x3c4, 0x3c5, 0x7, 0xa, 0x2, 0x2, 0x3c5, 0x3c7, 
    0x3, 0x2, 0x2, 0x2, 0x3c6, 0x3b8, 0x3, 0x2, 0x2, 0x2, 0x3c6, 0x3c7, 
    0x3, 0x2, 0x2, 0x2, 0x3c7, 0x408, 0x3, 0x2, 0x2, 0x2, 0x3c8, 0x3d0, 
    0x7, 0x7e, 0x2, 0x2, 0x3c9, 0x3ca, 0x7, 0x9, 0x2, 0x2, 0x3ca, 0x3cc, 
    0x5, 0xa4, 0x53, 0x2, 0x3cb, 0x3cd, 0x9, 0xd, 0x2, 0x2, 0x3cc, 0x3cb, 
    0x3, 0x2, 0x2, 0x2, 0x3cc, 0x3cd, 0x3, 0x2, 0x2, 0x2, 0x3cd, 0x3ce, 
    0x3, 0x2, 0x2, 0x2, 0x3ce, 0x3cf, 0x7, 0xa, 0x2, 0x2, 0x3cf, 0x3d1, 
    0x3, 0x2, 0x2, 0x2, 0x3d0, 0x3c9, 0x3, 0x2, 0x2, 0x2, 0x3d0, 0x3d1, 
    0x3, 0x2, 0x2, 0x2, 0x3d1, 0x408, 0x3, 0x2, 0x2, 0x2, 0x3d2, 0x408, 
    0x7, 0x2c, 0x2, 0x2, 0x3d3, 0x408, 0x7, 0x7f, 0x2, 0x2, 0x3d4, 0x408, 
    0x7, 0x80, 0x2, 0x2, 0x3d5, 0x3d6, 0x9, 0xe, 0x2, 0x2, 0x3d6, 0x3d7, 
    0x7, 0x9, 0x2, 0x2, 0x3d7, 0x3d9, 0x5, 0xa8, 0x55, 0x2, 0x3d8, 0x3da, 
    0x9, 0xf, 0x2, 0x2, 0x3d9, 0x3d8, 0x3, 0x2, 0x2, 0x2, 0x3d9, 0x3da, 
    0x3, 0x2, 0x2, 0x2, 0x3da, 0x3db, 0x3, 0x2, 0x2, 0x2, 0x3db, 0x3dc, 
    0x7, 0xa, 0x2, 0x2, 0x3dc, 0x3ef, 0x3, 0x2, 0x2, 0x2, 0x3dd, 0x3df, 
    0x9, 0xe, 0x2, 0x2, 0x3de, 0x3e0, 0x7, 0x89, 0x2, 0x2, 0x3df, 0x3de, 
    0x3, 0x2, 0x2, 0x2, 0x3df, 0x3e0, 0x3, 0x2, 0x2, 0x2, 0x3e0, 0x3e3, 
    0x3, 0x2, 0x2, 0x2, 0x3e1, 0x3e3, 0x7, 0x84, 0x2, 0x2, 0x3e2, 0x3dd, 
    0x3, 0x2, 0x2, 0x2, 0x3e2, 0x3e1, 0x3, 0x2, 0x2, 0x2, 0x3e3, 0x3eb, 
    0x3, 0x2, 0x2, 0x2, 0x3e4, 0x3e5, 0x7, 0x9, 0x2, 0x2, 0x3e5, 0x3e7, 
    0x5, 0xa8, 0x55, 0x2, 0x3e6, 0x3e8, 0x9, 0xf, 0x2, 0x2, 0x3e7, 0x3e6, 
    0x3, 0x2, 0x2, 0x2, 0x3e7, 0x3e8, 0x3, 0x2, 0x2, 0x2, 0x3e8, 0x3e9, 
    0x3, 0x2, 0x2, 0x2, 0x3e9, 0x3ea, 0x7, 0xa, 0x2, 0x2, 0x3ea, 0x3ec, 
    0x3, 0x2, 0x2, 0x2, 0x3eb, 0x3e4, 0x3, 0x2, 0x2, 0x2, 0x3eb, 0x3ec, 
    0x3, 0x2, 0x2, 0x2, 0x3ec, 0x3ef, 0x3, 0x2, 0x2, 0x2, 0x3ed, 0x3ef, 
    0x7, 0x85, 0x2, 0x2, 0x3ee, 0x3d5, 0x3, 0x2, 0x2, 0x2, 0x3ee, 0x3e2, 
    0x3, 0x2, 0x2, 0x2, 0x3ee, 0x3ed, 0x3, 0x2, 0x2, 0x2, 0x3ef, 0x3f2, 
    0x3, 0x2, 0x2, 0x2, 0x3f0, 0x3f1, 0x7, 0x23, 0x2, 0x2, 0x3f1, 0x3f3, 
    0x5, 0x94, 0x4b, 0x2, 0x3f2, 0x3f0, 0x3, 0x2, 0x2, 0x2, 0x3f2, 0x3f3, 
    0x3, 0x2, 0x2, 0x2, 0x3f3, 0x408, 0x3, 0x2, 0x2, 0x2, 0x3f4, 0x3f5, 
    0x7, 0x86, 0x2, 0x2, 0x3f5, 0x3f6, 0x7, 0x9, 0x2, 0x2, 0x3f6, 0x3f8, 
    0x5, 0xa8, 0x55, 0x2, 0x3f7, 0x3f9, 0x9, 0x10, 0x2, 0x2, 0x3f8, 0x3f7, 
    0x3, 0x2, 0x2, 0x2, 0x3f8, 0x3f9, 0x3, 0x2, 0x2, 0x2, 0x3f9, 0x3fa, 
    0x3, 0x2, 0x2, 0x2, 0x3fa, 0x3fb, 0x7, 0xa, 0x2, 0x2, 0x3fb, 0x408, 
    0x3, 0x2, 0x2, 0x2, 0x3fc, 0x404, 0x7, 0x87, 0x2, 0x2, 0x3fd, 0x3fe, 
    0x7, 0x9, 0x2, 0x2, 0x3fe, 0x400, 0x5, 0xa8, 0x55, 0x2, 0x3ff, 0x401, 
    0x9, 0x10, 0x2, 0x2, 0x400, 0x3ff, 0x3, 0x2, 0x2, 0x2, 0x400, 0x401, 
    0x3, 0x2, 0x2, 0x2, 0x401, 0x402, 0x3, 0x2, 0x2, 0x2, 0x402, 0x403, 
    0x7, 0xa, 0x2, 0x2, 0x403, 0x405, 0x3, 0x2, 0x2, 0x2, 0x404, 0x3fd, 
    0x3, 0x2, 0x2, 0x2, 0x404, 0x405, 0x3, 0x2, 0x2, 0x2, 0x405, 0x408, 
    0x3, 0x2, 0x2, 0x2, 0x406, 0x408, 0x7, 0x88, 0x2, 0x2, 0x407, 0x3ac, 
    0x3, 0x2, 0x2, 0x2, 0x407, 0x3ad, 0x3, 0x2, 0x2, 0x2, 0x407, 0x3b7, 
    0x3, 0x2, 0x2, 0x2, 0x407, 0x3c8, 0x3, 0x2, 0x2, 0x2, 0x407, 0x3d2, 
    0x3, 0x2, 0x2, 0x2, 0x407, 0x3d3, 0x3, 0x2, 0x2, 0x2, 0x407, 0x3d4, 
    0x3, 0x2, 0x2, 0x2, 0x407, 0x3ee, 0x3, 0x2, 0x2, 0x2, 0x407, 0x3f4, 
    0x3, 0x2, 0x2, 0x2, 0x407, 0x3fc, 0x3, 0x2, 0x2, 0x2, 0x407, 0x406, 
    0x3, 0x2, 0x2, 0x2, 0x408, 0x55, 0x3, 0x2, 0x2, 0x2, 0x409, 0x40a, 0x5, 
    0x58, 0x2d, 0x2, 0x40a, 0x57, 0x3, 0x2, 0x2, 0x2, 0x40b, 0x40f, 0x7, 
    0x9, 0x2, 0x2, 0x40c, 0x40e, 0x5, 0x18, 0xd, 0x2, 0x40d, 0x40c, 0x3, 
    0x2, 0x2, 0x2, 0x40e, 0x411, 0x3, 0x2, 0x2, 0x2, 0x40f, 0x40d, 0x3, 
    0x2, 0x2, 0x2, 0x40f, 0x410, 0x3, 0x2, 0x2, 0x2, 0x410, 0x412, 0x3, 
    0x2, 0x2, 0x2, 0x411, 0x40f, 0x3, 0x2, 0x2, 0x2, 0x412, 0x413, 0x5, 
    0x58, 0x2d, 0x2, 0x413, 0x414, 0x7, 0xa, 0x2, 0x2, 0x414, 0x421, 0x3, 
    0x2, 0x2, 0x2, 0x415, 0x417, 0x5, 0x18, 0xd, 0x2, 0x416, 0x415, 0x3, 
    0x2, 0x2, 0x2, 0x417, 0x41a, 0x3, 0x2, 0x2, 0x2, 0x418, 0x416, 0x3, 
    0x2, 0x2, 0x2, 0x418, 0x419, 0x3, 0x2, 0x2, 0x2, 0x419, 0x41e, 0x3, 
    0x2, 0x2, 0x2, 0x41a, 0x418, 0x3, 0x2, 0x2, 0x2, 0x41b, 0x41f, 0x5, 
    0x1c, 0xf, 0x2, 0x41c, 0x41f, 0x5, 0x32, 0x1a, 0x2, 0x41d, 0x41f, 0x5, 
    0x20, 0x11, 0x2, 0x41e, 0x41b, 0x3, 0x2, 0x2, 0x2, 0x41e, 0x41c, 0x3, 
    0x2, 0x2, 0x2, 0x41e, 0x41d, 0x3, 0x2, 0x2, 0x2, 0x41f, 0x421, 0x3, 
    0x2, 0x2, 0x2, 0x420, 0x40b, 0x3, 0x2, 0x2, 0x2, 0x420, 0x418, 0x3, 
    0x2, 0x2, 0x2, 0x421, 0x59, 0x3, 0x2, 0x2, 0x2, 0x422, 0x427, 0x5, 0x5c, 
    0x2f, 0x2, 0x423, 0x424, 0x7, 0x5, 0x2, 0x2, 0x424, 0x426, 0x5, 0x5c, 
    0x2f, 0x2, 0x425, 0x423, 0x3, 0x2, 0x2, 0x2, 0x426, 0x429, 0x3, 0x2, 
    0x2, 0x2, 0x427, 0x425, 0x3, 0x2, 0x2, 0x2, 0x427, 0x428, 0x3, 0x2, 
    0x2, 0x2, 0x428, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x429, 0x427, 0x3, 0x2, 0x2, 
    0x2, 0x42a, 0x42f, 0x5, 0x7a, 0x3e, 0x2, 0x42b, 0x42d, 0x7, 0x1c, 0x2, 
    0x2, 0x42c, 0x42b, 0x3, 0x2, 0x2, 0x2, 0x42c, 0x42d, 0x3, 0x2, 0x2, 
    0x2, 0x42d, 0x42e, 0x3, 0x2, 0x2, 0x2, 0x42e, 0x430, 0x5, 0xa0, 0x51, 
    0x2, 0x42f, 0x42c, 0x3, 0x2, 0x2, 0x2, 0x42f, 0x430, 0x3, 0x2, 0x2, 
    0x2, 0x430, 0x433, 0x3, 0x2, 0x2, 0x2, 0x431, 0x433, 0x5, 0x80, 0x41, 
    0x2, 0x432, 0x42a, 0x3, 0x2, 0x2, 0x2, 0x432, 0x431, 0x3, 0x2, 0x2, 
    0x2, 0x433, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x434, 0x439, 0x5, 0x60, 0x31, 
    0x2, 0x435, 0x436, 0x7, 0x5, 0x2, 0x2, 0x436, 0x438, 0x5, 0x60, 0x31, 
    0x2, 0x437, 0x435, 0x3, 0x2, 0x2, 0x2, 0x438, 0x43b, 0x3, 0x2, 0x2, 
    0x2, 0x439, 0x437, 0x3, 0x2, 0x2, 0x2, 0x439, 0x43a, 0x3, 0x2, 0x2, 
    0x2, 0x43a, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x43b, 0x439, 0x3, 0x2, 0x2, 0x2, 
    0x43c, 0x43d, 0x5, 0x7a, 0x3e, 0x2, 0x43d, 0x61, 0x3, 0x2, 0x2, 0x2, 
    0x43e, 0x443, 0x5, 0x64, 0x33, 0x2, 0x43f, 0x440, 0x7, 0x5, 0x2, 0x2, 
    0x440, 0x442, 0x5, 0x64, 0x33, 0x2, 0x441, 0x43f, 0x3, 0x2, 0x2, 0x2, 
    0x442, 0x445, 0x3, 0x2, 0x2, 0x2, 0x443, 0x441, 0x3, 0x2, 0x2, 0x2, 
    0x443, 0x444, 0x3, 0x2, 0x2, 0x2, 0x444, 0x63, 0x3, 0x2, 0x2, 0x2, 0x445, 
    0x443, 0x3, 0x2, 0x2, 0x2, 0x446, 0x448, 0x5, 0x7a, 0x3e, 0x2, 0x447, 
    0x449, 0x9, 0x11, 0x2, 0x2, 0x448, 0x447, 0x3, 0x2, 0x2, 0x2, 0x448, 
    0x449, 0x3, 0x2, 0x2, 0x2, 0x449, 0x65, 0x3, 0x2, 0x2, 0x2, 0x44a, 0x44f, 
    0x5, 0x68, 0x35, 0x2, 0x44b, 0x44c, 0x7, 0x5, 0x2, 0x2, 0x44c, 0x44e, 
    0x5, 0x68, 0x35, 0x2, 0x44d, 0x44b, 0x3, 0x2, 0x2, 0x2, 0x44e, 0x451, 
    0x3, 0x2, 0x2, 0x2, 0x44f, 0x44d, 0x3, 0x2, 0x2, 0x2, 0x44f, 0x450, 
    0x3, 0x2, 0x2, 0x2, 0x450, 0x67, 0x3, 0x2, 0x2, 0x2, 0x451, 0x44f, 0x3, 
    0x2, 0x2, 0x2, 0x452, 0x453, 0x5, 0xa0, 0x51, 0x2, 0x453, 0x69, 0x3, 
    0x2, 0x2, 0x2, 0x454, 0x459, 0x5, 0x6c, 0x37, 0x2, 0x455, 0x456, 0x7, 
    0x5, 0x2, 0x2, 0x456, 0x458, 0x5, 0x6c, 0x37, 0x2, 0x457, 0x455, 0x3, 
    0x2, 0x2, 0x2, 0x458, 0x45b, 0x3, 0x2, 0x2, 0x2, 0x459, 0x457, 0x3, 
    0x2, 0x2, 0x2, 0x459, 0x45a, 0x3, 0x2, 0x2, 0x2, 0x45a, 0x6b, 0x3, 0x2, 
    0x2, 0x2, 0x45b, 0x459, 0x3, 0x2, 0x2, 0x2, 0x45c, 0x45d, 0x5, 0xa0, 
    0x51, 0x2, 0x45d, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x45e, 0x463, 0x5, 0x70, 
    0x39, 0x2, 0x45f, 0x460, 0x7, 0x5, 0x2, 0x2, 0x460, 0x462, 0x5, 0x70, 
    0x39, 0x2, 0x461, 0x45f, 0x3, 0x2, 0x2, 0x2, 0x462, 0x465, 0x3, 0x2, 
    0x2, 0x2, 0x463, 0x461, 0x3, 0x2, 0x2, 0x2, 0x463, 0x464, 0x3, 0x2, 
    0x2, 0x2, 0x464, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x465, 0x463, 0x3, 0x2, 0x2, 
    0x2, 0x466, 0x469, 0x5, 0x98, 0x4d, 0x2, 0x467, 0x468, 0x7, 0x8, 0x2, 
    0x2, 0x468, 0x46a, 0x5, 0x9c, 0x4f, 0x2, 0x469, 0x467, 0x3, 0x2, 0x2, 
    0x2, 0x469, 0x46a, 0x3, 0x2, 0x2, 0x2, 0x46a, 0x71, 0x3, 0x2, 0x2, 0x2, 
    0x46b, 0x470, 0x5, 0x7a, 0x3e, 0x2, 0x46c, 0x46d, 0x7, 0x5, 0x2, 0x2, 
    0x46d, 0x46f, 0x5, 0x7a, 0x3e, 0x2, 0x46e, 0x46c, 0x3, 0x2, 0x2, 0x2, 
    0x46f, 0x472, 0x3, 0x2, 0x2, 0x2, 0x470, 0x46e, 0x3, 0x2, 0x2, 0x2, 
    0x470, 0x471, 0x3, 0x2, 0x2, 0x2, 0x471, 0x73, 0x3, 0x2, 0x2, 0x2, 0x472, 
    0x470, 0x3, 0x2, 0x2, 0x2, 0x473, 0x474, 0x5, 0x7a, 0x3e, 0x2, 0x474, 
    0x75, 0x3, 0x2, 0x2, 0x2, 0x475, 0x476, 0x5, 0x7a, 0x3e, 0x2, 0x476, 
    0x77, 0x3, 0x2, 0x2, 0x2, 0x477, 0x478, 0x5, 0x7a, 0x3e, 0x2, 0x478, 
    0x79, 0x3, 0x2, 0x2, 0x2, 0x479, 0x47a, 0x8, 0x3e, 0x1, 0x2, 0x47a, 
    0x581, 0x5, 0xaa, 0x56, 0x2, 0x47b, 0x581, 0x5, 0x7c, 0x3f, 0x2, 0x47c, 
    0x581, 0x5, 0x7e, 0x40, 0x2, 0x47d, 0x47e, 0x7, 0x9, 0x2, 0x2, 0x47e, 
    0x47f, 0x5, 0x52, 0x2a, 0x2, 0x47f, 0x480, 0x7, 0xa, 0x2, 0x2, 0x480, 
    0x481, 0x5, 0x7a, 0x3e, 0x2a, 0x481, 0x581, 0x3, 0x2, 0x2, 0x2, 0x482, 
    0x483, 0x9, 0x12, 0x2, 0x2, 0x483, 0x581, 0x5, 0x7a, 0x3e, 0x29, 0x484, 
    0x485, 0x7, 0x91, 0x2, 0x2, 0x485, 0x486, 0x7, 0x9, 0x2, 0x2, 0x486, 
    0x487, 0x5, 0x7a, 0x3e, 0x2, 0x487, 0x488, 0x9, 0x13, 0x2, 0x2, 0x488, 
    0x489, 0x5, 0x52, 0x2a, 0x2, 0x489, 0x48a, 0x7, 0xa, 0x2, 0x2, 0x48a, 
    0x581, 0x3, 0x2, 0x2, 0x2, 0x48b, 0x48c, 0x7, 0x9f, 0x2, 0x2, 0x48c, 
    0x48d, 0x7, 0x9, 0x2, 0x2, 0x48d, 0x48e, 0x5, 0x7a, 0x3e, 0x2, 0x48e, 
    0x48f, 0x7, 0xa, 0x2, 0x2, 0x48f, 0x581, 0x3, 0x2, 0x2, 0x2, 0x490, 
    0x4a4, 0x7, 0x90, 0x2, 0x2, 0x491, 0x497, 0x5, 0x7a, 0x3e, 0x2, 0x492, 
    0x493, 0x7, 0xa0, 0x2, 0x2, 0x493, 0x494, 0x5, 0x7a, 0x3e, 0x2, 0x494, 
    0x495, 0x7, 0xa1, 0x2, 0x2, 0x495, 0x496, 0x5, 0x7a, 0x3e, 0x2, 0x496, 
    0x498, 0x3, 0x2, 0x2, 0x2, 0x497, 0x492, 0x3, 0x2, 0x2, 0x2, 0x498, 
    0x499, 0x3, 0x2, 0x2, 0x2, 0x499, 0x497, 0x3, 0x2, 0x2, 0x2, 0x499, 
    0x49a, 0x3, 0x2, 0x2, 0x2, 0x49a, 0x4a5, 0x3, 0x2, 0x2, 0x2, 0x49b, 
    0x49c, 0x7, 0xa0, 0x2, 0x2, 0x49c, 0x49d, 0x5, 0x78, 0x3d, 0x2, 0x49d, 
    0x49e, 0x7, 0xa1, 0x2, 0x2, 0x49e, 0x49f, 0x5, 0x7a, 0x3e, 0x2, 0x49f, 
    0x4a1, 0x3, 0x2, 0x2, 0x2, 0x4a0, 0x49b, 0x3, 0x2, 0x2, 0x2, 0x4a1, 
    0x4a2, 0x3, 0x2, 0x2, 0x2, 0x4a2, 0x4a0, 0x3, 0x2, 0x2, 0x2, 0x4a2, 
    0x4a3, 0x3, 0x2, 0x2, 0x2, 0x4a3, 0x4a5, 0x3, 0x2, 0x2, 0x2, 0x4a4, 
    0x491, 0x3, 0x2, 0x2, 0x2, 0x4a4, 0x4a0, 0x3, 0x2, 0x2, 0x2, 0x4a5, 
    0x4a8, 0x3, 0x2, 0x2, 0x2, 0x4a6, 0x4a7, 0x7, 0xa2, 0x2, 0x2, 0x4a7, 
    0x4a9, 0x5, 0x7a, 0x3e, 0x2, 0x4a8, 0x4a6, 0x3, 0x2, 0x2, 0x2, 0x4a8, 
    0x4a9, 0x3, 0x2, 0x2, 0x2, 0x4a9, 0x4aa, 0x3, 0x2, 0x2, 0x2, 0x4aa, 
    0x4ab, 0x7, 0xa3, 0x2, 0x2, 0x4ab, 0x581, 0x3, 0x2, 0x2, 0x2, 0x4ac, 
    0x4ad, 0x7, 0x98, 0x2, 0x2, 0x4ad, 0x4ae, 0x7, 0x9, 0x2, 0x2, 0x4ae, 
    0x4af, 0x5, 0x78, 0x3d, 0x2, 0x4af, 0x4b0, 0x7, 0x5, 0x2, 0x2, 0x4b0, 
    0x4b1, 0x5, 0x7a, 0x3e, 0x2, 0x4b1, 0x4b2, 0x7, 0x5, 0x2, 0x2, 0x4b2, 
    0x4b3, 0x5, 0x7a, 0x3e, 0x2, 0x4b3, 0x4b4, 0x7, 0xa, 0x2, 0x2, 0x4b4, 
    0x581, 0x3, 0x2, 0x2, 0x2, 0x4b5, 0x4b6, 0x7, 0x99, 0x2, 0x2, 0x4b6, 
    0x4b7, 0x7, 0x9, 0x2, 0x2, 0x4b7, 0x4b8, 0x5, 0x7a, 0x3e, 0x2, 0x4b8, 
    0x4b9, 0x7, 0x5, 0x2, 0x2, 0x4b9, 0x4ba, 0x5, 0x7a, 0x3e, 0x2, 0x4ba, 
    0x4bb, 0x7, 0xa, 0x2, 0x2, 0x4bb, 0x581, 0x3, 0x2, 0x2, 0x2, 0x4bc, 
    0x4bd, 0x7, 0x9a, 0x2, 0x2, 0x4bd, 0x4be, 0x7, 0x9, 0x2, 0x2, 0x4be, 
    0x4bf, 0x5, 0x7a, 0x3e, 0x2, 0x4bf, 0x4c0, 0x7, 0x5, 0x2, 0x2, 0x4c0, 
    0x4c1, 0x5, 0x7a, 0x3e, 0x2, 0x4c1, 0x4c2, 0x7, 0xa, 0x2, 0x2, 0x4c2, 
    0x581, 0x3, 0x2, 0x2, 0x2, 0x4c3, 0x4c4, 0x7, 0x92, 0x2, 0x2, 0x4c4, 
    0x4cd, 0x7, 0x9, 0x2, 0x2, 0x4c5, 0x4ca, 0x5, 0x7a, 0x3e, 0x2, 0x4c6, 
    0x4c7, 0x7, 0x5, 0x2, 0x2, 0x4c7, 0x4c9, 0x5, 0x7a, 0x3e, 0x2, 0x4c8, 
    0x4c6, 0x3, 0x2, 0x2, 0x2, 0x4c9, 0x4cc, 0x3, 0x2, 0x2, 0x2, 0x4ca, 
    0x4c8, 0x3, 0x2, 0x2, 0x2, 0x4ca, 0x4cb, 0x3, 0x2, 0x2, 0x2, 0x4cb, 
    0x4ce, 0x3, 0x2, 0x2, 0x2, 0x4cc, 0x4ca, 0x3, 0x2, 0x2, 0x2, 0x4cd, 
    0x4c5, 0x3, 0x2, 0x2, 0x2, 0x4cd, 0x4ce, 0x3, 0x2, 0x2, 0x2, 0x4ce, 
    0x4cf, 0x3, 0x2, 0x2, 0x2, 0x4cf, 0x581, 0x7, 0xa, 0x2, 0x2, 0x4d0, 
    0x4d1, 0x7, 0x95, 0x2, 0x2, 0x4d1, 0x4da, 0x7, 0x9, 0x2, 0x2, 0x4d2, 
    0x4d7, 0x5, 0x7a, 0x3e, 0x2, 0x4d3, 0x4d4, 0x7, 0x5, 0x2, 0x2, 0x4d4, 
    0x4d6, 0x5, 0x7a, 0x3e, 0x2, 0x4d5, 0x4d3, 0x3, 0x2, 0x2, 0x2, 0x4d6, 
    0x4d9, 0x3, 0x2, 0x2, 0x2, 0x4d7, 0x4d5, 0x3, 0x2, 0x2, 0x2, 0x4d7, 
    0x4d8, 0x3, 0x2, 0x2, 0x2, 0x4d8, 0x4db, 0x3, 0x2, 0x2, 0x2, 0x4d9, 
    0x4d7, 0x3, 0x2, 0x2, 0x2, 0x4da, 0x4d2, 0x3, 0x2, 0x2, 0x2, 0x4da, 
    0x4db, 0x3, 0x2, 0x2, 0x2, 0x4db, 0x4dc, 0x3, 0x2, 0x2, 0x2, 0x4dc, 
    0x581, 0x7, 0xa, 0x2, 0x2, 0x4dd, 0x4de, 0x7, 0x93, 0x2, 0x2, 0x4de, 
    0x4e7, 0x7, 0x9, 0x2, 0x2, 0x4df, 0x4e4, 0x5, 0x7a, 0x3e, 0x2, 0x4e0, 
    0x4e1, 0x7, 0x5, 0x2, 0x2, 0x4e1, 0x4e3, 0x5, 0x7a, 0x3e, 0x2, 0x4e2, 
    0x4e0, 0x3, 0x2, 0x2, 0x2, 0x4e3, 0x4e6, 0x3, 0x2, 0x2, 0x2, 0x4e4, 
    0x4e2, 0x3, 0x2, 0x2, 0x2, 0x4e4, 0x4e5, 0x3, 0x2, 0x2, 0x2, 0x4e5, 
    0x4e8, 0x3, 0x2, 0x2, 0x2, 0x4e6, 0x4e4, 0x3, 0x2, 0x2, 0x2, 0x4e7, 
    0x4df, 0x3, 0x2, 0x2, 0x2, 0x4e7, 0x4e8, 0x3, 0x2, 0x2, 0x2, 0x4e8, 
    0x4e9, 0x3, 0x2, 0x2, 0x2, 0x4e9, 0x581, 0x7, 0xa, 0x2, 0x2, 0x4ea, 
    0x4eb, 0x7, 0x59, 0x2, 0x2, 0x4eb, 0x4ec, 0x7, 0x9, 0x2, 0x2, 0x4ec, 
    0x4ed, 0x5, 0x7a, 0x3e, 0x2, 0x4ed, 0x4ee, 0x9, 0x14, 0x2, 0x2, 0x4ee, 
    0x4ef, 0x5, 0x7a, 0x3e, 0x2, 0x4ef, 0x4f0, 0x9, 0x15, 0x2, 0x2, 0x4f0, 
    0x4f3, 0x5, 0x7a, 0x3e, 0x2, 0x4f1, 0x4f2, 0x9, 0x16, 0x2, 0x2, 0x4f2, 
    0x4f4, 0x5, 0x7a, 0x3e, 0x2, 0x4f3, 0x4f1, 0x3, 0x2, 0x2, 0x2, 0x4f3, 
    0x4f4, 0x3, 0x2, 0x2, 0x2, 0x4f4, 0x4f5, 0x3, 0x2, 0x2, 0x2, 0x4f5, 
    0x4f6, 0x7, 0xa, 0x2, 0x2, 0x4f6, 0x581, 0x3, 0x2, 0x2, 0x2, 0x4f7, 
    0x4f8, 0x7, 0x9b, 0x2, 0x2, 0x4f8, 0x4f9, 0x7, 0x9, 0x2, 0x2, 0x4f9, 
    0x4fa, 0x5, 0x7a, 0x3e, 0x2, 0x4fa, 0x4fb, 0x9, 0x17, 0x2, 0x2, 0x4fb, 
    0x4fc, 0x5, 0x7a, 0x3e, 0x2, 0x4fc, 0x4fd, 0x7, 0xa, 0x2, 0x2, 0x4fd, 
    0x581, 0x3, 0x2, 0x2, 0x2, 0x4fe, 0x4ff, 0x9, 0x18, 0x2, 0x2, 0x4ff, 
    0x50e, 0x7, 0x9, 0x2, 0x2, 0x500, 0x501, 0x5, 0x7a, 0x3e, 0x2, 0x501, 
    0x502, 0x7, 0x5, 0x2, 0x2, 0x502, 0x505, 0x5, 0x7a, 0x3e, 0x2, 0x503, 
    0x504, 0x7, 0x5, 0x2, 0x2, 0x504, 0x506, 0x5, 0x7a, 0x3e, 0x2, 0x505, 
    0x503, 0x3, 0x2, 0x2, 0x2, 0x505, 0x506, 0x3, 0x2, 0x2, 0x2, 0x506, 
    0x50f, 0x3, 0x2, 0x2, 0x2, 0x507, 0x508, 0x5, 0x7a, 0x3e, 0x2, 0x508, 
    0x509, 0x7, 0x3a, 0x2, 0x2, 0x509, 0x50c, 0x5, 0x7a, 0x3e, 0x2, 0x50a, 
    0x50b, 0x7, 0x37, 0x2, 0x2, 0x50b, 0x50d, 0x5, 0x7a, 0x3e, 0x2, 0x50c, 
    0x50a, 0x3, 0x2, 0x2, 0x2, 0x50c, 0x50d, 0x3, 0x2, 0x2, 0x2, 0x50d, 
    0x50f, 0x3, 0x2, 0x2, 0x2, 0x50e, 0x500, 0x3, 0x2, 0x2, 0x2, 0x50e, 
    0x507, 0x3, 0x2, 0x2, 0x2, 0x50f, 0x510, 0x3, 0x2, 0x2, 0x2, 0x510, 
    0x511, 0x7, 0xa, 0x2, 0x2, 0x511, 0x581, 0x3, 0x2, 0x2, 0x2, 0x512, 
    0x513, 0x7, 0x9e, 0x2, 0x2, 0x513, 0x521, 0x7, 0x9, 0x2, 0x2, 0x514, 
    0x517, 0x5, 0x7a, 0x3e, 0x2, 0x515, 0x516, 0x7, 0x5, 0x2, 0x2, 0x516, 
    0x518, 0x5, 0x7a, 0x3e, 0x2, 0x517, 0x515, 0x3, 0x2, 0x2, 0x2, 0x517, 
    0x518, 0x3, 0x2, 0x2, 0x2, 0x518, 0x522, 0x3, 0x2, 0x2, 0x2, 0x519, 
    0x51b, 0x9, 0x19, 0x2, 0x2, 0x51a, 0x519, 0x3, 0x2, 0x2, 0x2, 0x51a, 
    0x51b, 0x3, 0x2, 0x2, 0x2, 0x51b, 0x51d, 0x3, 0x2, 0x2, 0x2, 0x51c, 
    0x51e, 0x5, 0x7a, 0x3e, 0x2, 0x51d, 0x51c, 0x3, 0x2, 0x2, 0x2, 0x51d, 
    0x51e, 0x3, 0x2, 0x2, 0x2, 0x51e, 0x51f, 0x3, 0x2, 0x2, 0x2, 0x51f, 
    0x520, 0x7, 0x3a, 0x2, 0x2, 0x520, 0x522, 0x5, 0x7a, 0x3e, 0x2, 0x521, 
    0x514, 0x3, 0x2, 0x2, 0x2, 0x521, 0x51a, 0x3, 0x2, 0x2, 0x2, 0x522, 
    0x523, 0x3, 0x2, 0x2, 0x2, 0x523, 0x524, 0x7, 0xa, 0x2, 0x2, 0x524, 
    0x581, 0x3, 0x2, 0x2, 0x2, 0x525, 0x526, 0x7, 0x96, 0x2, 0x2, 0x526, 
    0x527, 0x7, 0x9, 0x2, 0x2, 0x527, 0x528, 0x9, 0x1a, 0x2, 0x2, 0x528, 
    0x529, 0x7, 0x3a, 0x2, 0x2, 0x529, 0x52a, 0x5, 0x7a, 0x3e, 0x2, 0x52a, 
    0x52b, 0x7, 0xa, 0x2, 0x2, 0x52b, 0x581, 0x3, 0x2, 0x2, 0x2, 0x52c, 
    0x52d, 0x7, 0x94, 0x2, 0x2, 0x52d, 0x52f, 0x7, 0x9, 0x2, 0x2, 0x52e, 
    0x530, 0x7, 0x17, 0x2, 0x2, 0x52f, 0x52e, 0x3, 0x2, 0x2, 0x2, 0x52f, 
    0x530, 0x3, 0x2, 0x2, 0x2, 0x530, 0x531, 0x3, 0x2, 0x2, 0x2, 0x531, 
    0x532, 0x7, 0x7, 0x2, 0x2, 0x532, 0x581, 0x7, 0xa, 0x2, 0x2, 0x533, 
    0x534, 0x7, 0x94, 0x2, 0x2, 0x534, 0x537, 0x7, 0x9, 0x2, 0x2, 0x535, 
    0x538, 0x7, 0x17, 0x2, 0x2, 0x536, 0x538, 0x7, 0x30, 0x2, 0x2, 0x537, 
    0x535, 0x3, 0x2, 0x2, 0x2, 0x537, 0x536, 0x3, 0x2, 0x2, 0x2, 0x537, 
    0x538, 0x3, 0x2, 0x2, 0x2, 0x538, 0x539, 0x3, 0x2, 0x2, 0x2, 0x539, 
    0x53a, 0x5, 0x7a, 0x3e, 0x2, 0x53a, 0x53b, 0x7, 0xa, 0x2, 0x2, 0x53b, 
    0x581, 0x3, 0x2, 0x2, 0x2, 0x53c, 0x53d, 0x7, 0x69, 0x2, 0x2, 0x53d, 
    0x540, 0x7, 0x9, 0x2, 0x2, 0x53e, 0x541, 0x7, 0x17, 0x2, 0x2, 0x53f, 
    0x541, 0x7, 0x30, 0x2, 0x2, 0x540, 0x53e, 0x3, 0x2, 0x2, 0x2, 0x540, 
    0x53f, 0x3, 0x2, 0x2, 0x2, 0x540, 0x541, 0x3, 0x2, 0x2, 0x2, 0x541, 
    0x542, 0x3, 0x2, 0x2, 0x2, 0x542, 0x543, 0x5, 0x7a, 0x3e, 0x2, 0x543, 
    0x544, 0x7, 0xa, 0x2, 0x2, 0x544, 0x581, 0x3, 0x2, 0x2, 0x2, 0x545, 
    0x546, 0x7, 0x1e, 0x2, 0x2, 0x546, 0x549, 0x7, 0x9, 0x2, 0x2, 0x547, 
    0x54a, 0x7, 0x17, 0x2, 0x2, 0x548, 0x54a, 0x7, 0x30, 0x2, 0x2, 0x549, 
    0x547, 0x3, 0x2, 0x2, 0x2, 0x549, 0x548, 0x3, 0x2, 0x2, 0x2, 0x549, 
    0x54a, 0x3, 0x2, 0x2, 0x2, 0x54a, 0x54b, 0x3, 0x2, 0x2, 0x2, 0x54b, 
    0x54c, 0x5, 0x7a, 0x3e, 0x2, 0x54c, 0x54d, 0x7, 0xa, 0x2, 0x2, 0x54d, 
    0x581, 0x3, 0x2, 0x2, 0x2, 0x54e, 0x54f, 0x7, 0x4b, 0x2, 0x2, 0x54f, 
    0x552, 0x7, 0x9, 0x2, 0x2, 0x550, 0x553, 0x7, 0x17, 0x2, 0x2, 0x551, 
    0x553, 0x7, 0x30, 0x2, 0x2, 0x552, 0x550, 0x3, 0x2, 0x2, 0x2, 0x552, 
    0x551, 0x3, 0x2, 0x2, 0x2, 0x552, 0x553, 0x3, 0x2, 0x2, 0x2, 0x553, 
    0x554, 0x3, 0x2, 0x2, 0x2, 0x554, 0x555, 0x5, 0x7a, 0x3e, 0x2, 0x555, 
    0x556, 0x7, 0xa, 0x2, 0x2, 0x556, 0x581, 0x3, 0x2, 0x2, 0x2, 0x557, 
    0x558, 0x7, 0x4a, 0x2, 0x2, 0x558, 0x55b, 0x7, 0x9, 0x2, 0x2, 0x559, 
    0x55c, 0x7, 0x17, 0x2, 0x2, 0x55a, 0x55c, 0x7, 0x30, 0x2, 0x2, 0x55b, 
    0x559, 0x3, 0x2, 0x2, 0x2, 0x55b, 0x55a, 0x3, 0x2, 0x2, 0x2, 0x55b, 
    0x55c, 0x3, 0x2, 0x2, 0x2, 0x55c, 0x55d, 0x3, 0x2, 0x2, 0x2, 0x55d, 
    0x55e, 0x5, 0x7a, 0x3e, 0x2, 0x55e, 0x55f, 0x7, 0xa, 0x2, 0x2, 0x55f, 
    0x581, 0x3, 0x2, 0x2, 0x2, 0x560, 0x561, 0x5, 0x84, 0x43, 0x2, 0x561, 
    0x56a, 0x7, 0x9, 0x2, 0x2, 0x562, 0x567, 0x5, 0x7a, 0x3e, 0x2, 0x563, 
    0x564, 0x7, 0x5, 0x2, 0x2, 0x564, 0x566, 0x5, 0x7a, 0x3e, 0x2, 0x565, 
    0x563, 0x3, 0x2, 0x2, 0x2, 0x566, 0x569, 0x3, 0x2, 0x2, 0x2, 0x567, 
    0x565, 0x3, 0x2, 0x2, 0x2, 0x567, 0x568, 0x3, 0x2, 0x2, 0x2, 0x568, 
    0x56b, 0x3, 0x2, 0x2, 0x2, 0x569, 0x567, 0x3, 0x2, 0x2, 0x2, 0x56a, 
    0x562, 0x3, 0x2, 0x2, 0x2, 0x56a, 0x56b, 0x3, 0x2, 0x2, 0x2, 0x56b, 
    0x56c, 0x3, 0x2, 0x2, 0x2, 0x56c, 0x56d, 0x7, 0xa, 0x2, 0x2, 0x56d, 
    0x581, 0x3, 0x2, 0x2, 0x2, 0x56e, 0x56f, 0x7, 0x9, 0x2, 0x2, 0x56f, 
    0x570, 0x5, 0x7a, 0x3e, 0x2, 0x570, 0x571, 0x7, 0xa, 0x2, 0x2, 0x571, 
    0x581, 0x3, 0x2, 0x2, 0x2, 0x572, 0x573, 0x7, 0x9, 0x2, 0x2, 0x573, 
    0x574, 0x5, 0x16, 0xc, 0x2, 0x574, 0x575, 0x7, 0xa, 0x2, 0x2, 0x575, 
    0x581, 0x3, 0x2, 0x2, 0x2, 0x576, 0x578, 0x7, 0x50, 0x2, 0x2, 0x577, 
    0x576, 0x3, 0x2, 0x2, 0x2, 0x577, 0x578, 0x3, 0x2, 0x2, 0x2, 0x578, 
    0x579, 0x3, 0x2, 0x2, 0x2, 0x579, 0x57a, 0x7, 0x97, 0x2, 0x2, 0x57a, 
    0x57b, 0x7, 0x9, 0x2, 0x2, 0x57b, 0x57c, 0x5, 0x16, 0xc, 0x2, 0x57c, 
    0x57d, 0x7, 0xa, 0x2, 0x2, 0x57d, 0x581, 0x3, 0x2, 0x2, 0x2, 0x57e, 
    0x57f, 0x7, 0x50, 0x2, 0x2, 0x57f, 0x581, 0x5, 0x7a, 0x3e, 0x6, 0x580, 
    0x479, 0x3, 0x2, 0x2, 0x2, 0x580, 0x47b, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x47c, 0x3, 0x2, 0x2, 0x2, 0x580, 0x47d, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x482, 0x3, 0x2, 0x2, 0x2, 0x580, 0x484, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x48b, 0x3, 0x2, 0x2, 0x2, 0x580, 0x490, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x4ac, 0x3, 0x2, 0x2, 0x2, 0x580, 0x4b5, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x4bc, 0x3, 0x2, 0x2, 0x2, 0x580, 0x4c3, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x4d0, 0x3, 0x2, 0x2, 0x2, 0x580, 0x4dd, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x4ea, 0x3, 0x2, 0x2, 0x2, 0x580, 0x4f7, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x4fe, 0x3, 0x2, 0x2, 0x2, 0x580, 0x512, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x525, 0x3, 0x2, 0x2, 0x2, 0x580, 0x52c, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x533, 0x3, 0x2, 0x2, 0x2, 0x580, 0x53c, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x545, 0x3, 0x2, 0x2, 0x2, 0x580, 0x54e, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x557, 0x3, 0x2, 0x2, 0x2, 0x580, 0x560, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x56e, 0x3, 0x2, 0x2, 0x2, 0x580, 0x572, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x577, 0x3, 0x2, 0x2, 0x2, 0x580, 0x57e, 0x3, 0x2, 0x2, 0x2, 0x581, 
    0x5df, 0x3, 0x2, 0x2, 0x2, 0x582, 0x583, 0xc, 0x27, 0x2, 0x2, 0x583, 
    0x584, 0x9, 0x1b, 0x2, 0x2, 0x584, 0x5de, 0x5, 0x7a, 0x3e, 0x28, 0x585, 
    0x586, 0xc, 0x26, 0x2, 0x2, 0x586, 0x587, 0x9, 0x1c, 0x2, 0x2, 0x587, 
    0x5de, 0x5, 0x7a, 0x3e, 0x27, 0x588, 0x589, 0xc, 0x25, 0x2, 0x2, 0x589, 
    0x58a, 0x9, 0x1d, 0x2, 0x2, 0x58a, 0x5de, 0x5, 0x7a, 0x3e, 0x26, 0x58b, 
    0x58d, 0xc, 0xc, 0x2, 0x2, 0x58c, 0x58e, 0x7, 0x50, 0x2, 0x2, 0x58d, 
    0x58c, 0x3, 0x2, 0x2, 0x2, 0x58d, 0x58e, 0x3, 0x2, 0x2, 0x2, 0x58e, 
    0x58f, 0x3, 0x2, 0x2, 0x2, 0x58f, 0x590, 0x7, 0x48, 0x2, 0x2, 0x590, 
    0x5de, 0x5, 0x7a, 0x3e, 0xd, 0x591, 0x593, 0xc, 0xb, 0x2, 0x2, 0x592, 
    0x594, 0x7, 0x50, 0x2, 0x2, 0x593, 0x592, 0x3, 0x2, 0x2, 0x2, 0x593, 
    0x594, 0x3, 0x2, 0x2, 0x2, 0x594, 0x595, 0x3, 0x2, 0x2, 0x2, 0x595, 
    0x596, 0x7, 0x48, 0x2, 0x2, 0x596, 0x597, 0x5, 0x7a, 0x3e, 0x2, 0x597, 
    0x598, 0x7, 0x32, 0x2, 0x2, 0x598, 0x599, 0x5, 0x7a, 0x3e, 0xc, 0x599, 
    0x5de, 0x3, 0x2, 0x2, 0x2, 0x59a, 0x59b, 0xc, 0x5, 0x2, 0x2, 0x59b, 
    0x59c, 0x6, 0x3e, 0xd, 0x2, 0x59c, 0x59d, 0x7, 0x18, 0x2, 0x2, 0x59d, 
    0x5de, 0x5, 0x7a, 0x3e, 0x6, 0x59e, 0x59f, 0xc, 0x4, 0x2, 0x2, 0x59f, 
    0x5a0, 0x7, 0x55, 0x2, 0x2, 0x5a0, 0x5de, 0x5, 0x7a, 0x3e, 0x5, 0x5a1, 
    0x5a2, 0xc, 0x3, 0x2, 0x2, 0x5a2, 0x5a3, 0x7, 0x7a, 0x2, 0x2, 0x5a3, 
    0x5de, 0x5, 0x7a, 0x3e, 0x4, 0x5a4, 0x5a5, 0xc, 0x28, 0x2, 0x2, 0x5a5, 
    0x5a6, 0x7, 0xc, 0x2, 0x2, 0x5a6, 0x5a7, 0x5, 0x86, 0x44, 0x2, 0x5a7, 
    0x5b0, 0x7, 0x9, 0x2, 0x2, 0x5a8, 0x5ad, 0x5, 0x7a, 0x3e, 0x2, 0x5a9, 
    0x5aa, 0x7, 0x5, 0x2, 0x2, 0x5aa, 0x5ac, 0x5, 0x7a, 0x3e, 0x2, 0x5ab, 
    0x5a9, 0x3, 0x2, 0x2, 0x2, 0x5ac, 0x5af, 0x3, 0x2, 0x2, 0x2, 0x5ad, 
    0x5ab, 0x3, 0x2, 0x2, 0x2, 0x5ad, 0x5ae, 0x3, 0x2, 0x2, 0x2, 0x5ae, 
    0x5b1, 0x3, 0x2, 0x2, 0x2, 0x5af, 0x5ad, 0x3, 0x2, 0x2, 0x2, 0x5b0, 
    0x5a8, 0x3, 0x2, 0x2, 0x2, 0x5b0, 0x5b1, 0x3, 0x2, 0x2, 0x2, 0x5b1, 
    0x5b2, 0x3, 0x2, 0x2, 0x2, 0x5b2, 0x5b3, 0x7, 0xa, 0x2, 0x2, 0x5b3, 
    0x5de, 0x3, 0x2, 0x2, 0x2, 0x5b4, 0x5b5, 0xc, 0xd, 0x2, 0x2, 0x5b5, 
    0x5b7, 0x7, 0x43, 0x2, 0x2, 0x5b6, 0x5b8, 0x7, 0x50, 0x2, 0x2, 0x5b7, 
    0x5b6, 0x3, 0x2, 0x2, 0x2, 0x5b7, 0x5b8, 0x3, 0x2, 0x2, 0x2, 0x5b8, 
    0x5b9, 0x3, 0x2, 0x2, 0x2, 0x5b9, 0x5de, 0x7, 0x52, 0x2, 0x2, 0x5ba, 
    0x5bc, 0xc, 0x9, 0x2, 0x2, 0x5bb, 0x5bd, 0x7, 0x50, 0x2, 0x2, 0x5bc, 
    0x5bb, 0x3, 0x2, 0x2, 0x2, 0x5bc, 0x5bd, 0x3, 0x2, 0x2, 0x2, 0x5bd, 
    0x5be, 0x3, 0x2, 0x2, 0x2, 0x5be, 0x5bf, 0x7, 0x3d, 0x2, 0x2, 0x5bf, 
    0x5c0, 0x7, 0x9, 0x2, 0x2, 0x5c0, 0x5c1, 0x5, 0x16, 0xc, 0x2, 0x5c1, 
    0x5c2, 0x7, 0xa, 0x2, 0x2, 0x5c2, 0x5de, 0x3, 0x2, 0x2, 0x2, 0x5c3, 
    0x5c5, 0xc, 0x8, 0x2, 0x2, 0x5c4, 0x5c6, 0x7, 0x50, 0x2, 0x2, 0x5c5, 
    0x5c4, 0x3, 0x2, 0x2, 0x2, 0x5c5, 0x5c6, 0x3, 0x2, 0x2, 0x2, 0x5c6, 
    0x5c7, 0x3, 0x2, 0x2, 0x2, 0x5c7, 0x5c8, 0x7, 0x3d, 0x2, 0x2, 0x5c8, 
    0x5d1, 0x7, 0x9, 0x2, 0x2, 0x5c9, 0x5ce, 0x5, 0x7a, 0x3e, 0x2, 0x5ca, 
    0x5cb, 0x7, 0x5, 0x2, 0x2, 0x5cb, 0x5cd, 0x5, 0x7a, 0x3e, 0x2, 0x5cc, 
    0x5ca, 0x3, 0x2, 0x2, 0x2, 0x5cd, 0x5d0, 0x3, 0x2, 0x2, 0x2, 0x5ce, 
    0x5cc, 0x3, 0x2, 0x2, 0x2, 0x5ce, 0x5cf, 0x3, 0x2, 0x2, 0x2, 0x5cf, 
    0x5d2, 0x3, 0x2, 0x2, 0x2, 0x5d0, 0x5ce, 0x3, 0x2, 0x2, 0x2, 0x5d1, 
    0x5c9, 0x3, 0x2, 0x2, 0x2, 0x5d1, 0x5d2, 0x3, 0x2, 0x2, 0x2, 0x5d2, 
    0x5d3, 0x3, 0x2, 0x2, 0x2, 0x5d3, 0x5de, 0x7, 0xa, 0x2, 0x2, 0x5d4, 
    0x5d6, 0xc, 0x7, 0x2, 0x2, 0x5d5, 0x5d7, 0x7, 0x50, 0x2, 0x2, 0x5d6, 
    0x5d5, 0x3, 0x2, 0x2, 0x2, 0x5d6, 0x5d7, 0x3, 0x2, 0x2, 0x2, 0x5d7, 
    0x5d8, 0x3, 0x2, 0x2, 0x2, 0x5d8, 0x5d9, 0x7, 0x1f, 0x2, 0x2, 0x5d9, 
    0x5da, 0x5, 0x7a, 0x3e, 0x2, 0x5da, 0x5db, 0x7, 0x18, 0x2, 0x2, 0x5db, 
    0x5dc, 0x5, 0x7a, 0x3e, 0x2, 0x5dc, 0x5de, 0x3, 0x2, 0x2, 0x2, 0x5dd, 
    0x582, 0x3, 0x2, 0x2, 0x2, 0x5dd, 0x585, 0x3, 0x2, 0x2, 0x2, 0x5dd, 
    0x588, 0x3, 0x2, 0x2, 0x2, 0x5dd, 0x58b, 0x3, 0x2, 0x2, 0x2, 0x5dd, 
    0x591, 0x3, 0x2, 0x2, 0x2, 0x5dd, 0x59a, 0x3, 0x2, 0x2, 0x2, 0x5dd, 
    0x59e, 0x3, 0x2, 0x2, 0x2, 0x5dd, 0x5a1, 0x3, 0x2, 0x2, 0x2, 0x5dd, 
    0x5a4, 0x3, 0x2, 0x2, 0x2, 0x5dd, 0x5b4, 0x3, 0x2, 0x2, 0x2, 0x5dd, 
    0x5ba, 0x3, 0x2, 0x2, 0x2, 0x5dd, 0x5c3, 0x3, 0x2, 0x2, 0x2, 0x5dd, 
    0x5d4, 0x3, 0x2, 0x2, 0x2, 0x5de, 0x5e1, 0x3, 0x2, 0x2, 0x2, 0x5df, 
    0x5dd, 0x3, 0x2, 0x2, 0x2, 0x5df, 0x5e0, 0x3, 0x2, 0x2, 0x2, 0x5e0, 
    0x7b, 0x3, 0x2, 0x2, 0x2, 0x5e1, 0x5df, 0x3, 0x2, 0x2, 0x2, 0x5e2, 0x5e3, 
    0x9, 0x1e, 0x2, 0x2, 0x5e3, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x5e4, 0x5e5, 
    0x5, 0x88, 0x45, 0x2, 0x5e5, 0x5e6, 0x7, 0xc, 0x2, 0x2, 0x5e6, 0x5e8, 
    0x3, 0x2, 0x2, 0x2, 0x5e7, 0x5e4, 0x3, 0x2, 0x2, 0x2, 0x5e7, 0x5e8, 
    0x3, 0x2, 0x2, 0x2, 0x5e8, 0x5e9, 0x3, 0x2, 0x2, 0x2, 0x5e9, 0x5ea, 
    0x5, 0x8a, 0x46, 0x2, 0x5ea, 0x5eb, 0x7, 0xc, 0x2, 0x2, 0x5eb, 0x5ed, 
    0x3, 0x2, 0x2, 0x2, 0x5ec, 0x5e7, 0x3, 0x2, 0x2, 0x2, 0x5ec, 0x5ed, 
    0x3, 0x2, 0x2, 0x2, 0x5ed, 0x5ee, 0x3, 0x2, 0x2, 0x2, 0x5ee, 0x5ef, 
    0x5, 0x8c, 0x47, 0x2, 0x5ef, 0x5f0, 0x7, 0xc, 0x2, 0x2, 0x5f0, 0x5f2, 
    0x3, 0x2, 0x2, 0x2, 0x5f1, 0x5ec, 0x3, 0x2, 0x2, 0x2, 0x5f1, 0x5f2, 
    0x3, 0x2, 0x2, 0x2, 0x5f2, 0x5f3, 0x3, 0x2, 0x2, 0x2, 0x5f3, 0x5f4, 
    0x5, 0x90, 0x49, 0x2, 0x5f4, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x5f5, 0x5f6, 
    0x5, 0x88, 0x45, 0x2, 0x5f6, 0x5f7, 0x7, 0xc, 0x2, 0x2, 0x5f7, 0x5f9, 
    0x3, 0x2, 0x2, 0x2, 0x5f8, 0x5f5, 0x3, 0x2, 0x2, 0x2, 0x5f8, 0x5f9, 
    0x3, 0x2, 0x2, 0x2, 0x5f9, 0x5fa, 0x3, 0x2, 0x2, 0x2, 0x5fa, 0x5fb, 
    0x5, 0x8a, 0x46, 0x2, 0x5fb, 0x5fc, 0x7, 0xc, 0x2, 0x2, 0x5fc, 0x5fe, 
    0x3, 0x2, 0x2, 0x2, 0x5fd, 0x5f8, 0x3, 0x2, 0x2, 0x2, 0x5fd, 0x5fe, 
    0x3, 0x2, 0x2, 0x2, 0x5fe, 0x5ff, 0x3, 0x2, 0x2, 0x2, 0x5ff, 0x600, 
    0x5, 0x8c, 0x47, 0x2, 0x600, 0x601, 0x7, 0xc, 0x2, 0x2, 0x601, 0x603, 
    0x3, 0x2, 0x2, 0x2, 0x602, 0x5fd, 0x3, 0x2, 0x2, 0x2, 0x602, 0x603, 
    0x3, 0x2, 0x2, 0x2, 0x603, 0x604, 0x3, 0x2, 0x2, 0x2, 0x604, 0x605, 
    0x7, 0x7, 0x2, 0x2, 0x605, 0x81, 0x3, 0x2, 0x2, 0x2, 0x606, 0x607, 0x5, 
    0xa2, 0x52, 0x2, 0x607, 0x608, 0x7, 0xc, 0x2, 0x2, 0x608, 0x60a, 0x3, 
    0x2, 0x2, 0x2, 0x609, 0x606, 0x3, 0x2, 0x2, 0x2, 0x60a, 0x60d, 0x3, 
    0x2, 0x2, 0x2, 0x60b, 0x609, 0x3, 0x2, 0x2, 0x2, 0x60b, 0x60c, 0x3, 
    0x2, 0x2, 0x2, 0x60c, 0x60e, 0x3, 0x2, 0x2, 0x2, 0x60d, 0x60b, 0x3, 
    0x2, 0x2, 0x2, 0x60e, 0x60f, 0x5, 0xa0, 0x51, 0x2, 0x60f, 0x83, 0x3, 
    0x2, 0x2, 0x2, 0x610, 0x611, 0x5, 0xa0, 0x51, 0x2, 0x611, 0x85, 0x3, 
    0x2, 0x2, 0x2, 0x612, 0x613, 0x5, 0xa0, 0x51, 0x2, 0x613, 0x87, 0x3, 
    0x2, 0x2, 0x2, 0x614, 0x615, 0x5, 0xa0, 0x51, 0x2, 0x615, 0x89, 0x3, 
    0x2, 0x2, 0x2, 0x616, 0x617, 0x5, 0xa0, 0x51, 0x2, 0x617, 0x8b, 0x3, 
    0x2, 0x2, 0x2, 0x618, 0x619, 0x5, 0xa0, 0x51, 0x2, 0x619, 0x8d, 0x3, 
    0x2, 0x2, 0x2, 0x61a, 0x61b, 0x5, 0xa0, 0x51, 0x2, 0x61b, 0x8f, 0x3, 
    0x2, 0x2, 0x2, 0x61c, 0x61d, 0x5, 0xa0, 0x51, 0x2, 0x61d, 0x91, 0x3, 
    0x2, 0x2, 0x2, 0x61e, 0x61f, 0x5, 0xa0, 0x51, 0x2, 0x61f, 0x93, 0x3, 
    0x2, 0x2, 0x2, 0x620, 0x621, 0x5, 0xa0, 0x51, 0x2, 0x621, 0x95, 0x3, 
    0x2, 0x2, 0x2, 0x622, 0x623, 0x5, 0xa0, 0x51, 0x2, 0x623, 0x97, 0x3, 
    0x2, 0x2, 0x2, 0x624, 0x625, 0x5, 0xa0, 0x51, 0x2, 0x625, 0x99, 0x3, 
    0x2, 0x2, 0x2, 0x626, 0x627, 0x5, 0xa0, 0x51, 0x2, 0x627, 0x9b, 0x3, 
    0x2, 0x2, 0x2, 0x628, 0x62b, 0x5, 0xac, 0x57, 0x2, 0x629, 0x62b, 0x5, 
    0xa0, 0x51, 0x2, 0x62a, 0x628, 0x3, 0x2, 0x2, 0x2, 0x62a, 0x629, 0x3, 
    0x2, 0x2, 0x2, 0x62b, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x62c, 0x62f, 0x5, 0xac, 
    0x57, 0x2, 0x62d, 0x62f, 0x5, 0xa0, 0x51, 0x2, 0x62e, 0x62c, 0x3, 0x2, 
    0x2, 0x2, 0x62e, 0x62d, 0x3, 0x2, 0x2, 0x2, 0x62f, 0x9f, 0x3, 0x2, 0x2, 
    0x2, 0x630, 0x633, 0x7, 0xbc, 0x2, 0x2, 0x631, 0x633, 0x5, 0xcc, 0x67, 
    0x2, 0x632, 0x630, 0x3, 0x2, 0x2, 0x2, 0x632, 0x631, 0x3, 0x2, 0x2, 
    0x2, 0x633, 0xa1, 0x3, 0x2, 0x2, 0x2, 0x634, 0x635, 0x5, 0xa0, 0x51, 
    0x2, 0x635, 0xa3, 0x3, 0x2, 0x2, 0x2, 0x636, 0x637, 0x5, 0xb0, 0x59, 
    0x2, 0x637, 0xa5, 0x3, 0x2, 0x2, 0x2, 0x638, 0x639, 0x5, 0xb0, 0x59, 
    0x2, 0x639, 0xa7, 0x3, 0x2, 0x2, 0x2, 0x63a, 0x63b, 0x5, 0xb0, 0x59, 
    0x2, 0x63b, 0xa9, 0x3, 0x2, 0x2, 0x2, 0x63c, 0x647, 0x7, 0x52, 0x2, 
    0x2, 0x63d, 0x647, 0x5, 0xae, 0x58, 0x2, 0x63e, 0x647, 0x5, 0xb0, 0x59, 
    0x2, 0x63f, 0x647, 0x5, 0xb4, 0x5b, 0x2, 0x640, 0x647, 0x5, 0xb8, 0x5d, 
    0x2, 0x641, 0x647, 0x5, 0xbc, 0x5f, 0x2, 0x642, 0x647, 0x5, 0xbe, 0x60, 
    0x2, 0x643, 0x647, 0x5, 0xc2, 0x62, 0x2, 0x644, 0x647, 0x5, 0xc0, 0x61, 
    0x2, 0x645, 0x647, 0x5, 0xc4, 0x63, 0x2, 0x646, 0x63c, 0x3, 0x2, 0x2, 
    0x2, 0x646, 0x63d, 0x3, 0x2, 0x2, 0x2, 0x646, 0x63e, 0x3, 0x2, 0x2, 
    0x2, 0x646, 0x63f, 0x3, 0x2, 0x2, 0x2, 0x646, 0x640, 0x3, 0x2, 0x2, 
    0x2, 0x646, 0x641, 0x3, 0x2, 0x2, 0x2, 0x646, 0x642, 0x3, 0x2, 0x2, 
    0x2, 0x646, 0x643, 0x3, 0x2, 0x2, 0x2, 0x646, 0x644, 0x3, 0x2, 0x2, 
    0x2, 0x646, 0x645, 0x3, 0x2, 0x2, 0x2, 0x647, 0xab, 0x3, 0x2, 0x2, 0x2, 
    0x648, 0x64f, 0x7, 0x52, 0x2, 0x2, 0x649, 0x64f, 0x5, 0xae, 0x58, 0x2, 
    0x64a, 0x64f, 0x5, 0xb2, 0x5a, 0x2, 0x64b, 0x64f, 0x5, 0xb6, 0x5c, 0x2, 
    0x64c, 0x64f, 0x5, 0xba, 0x5e, 0x2, 0x64d, 0x64f, 0x5, 0xc2, 0x62, 0x2, 
    0x64e, 0x648, 0x3, 0x2, 0x2, 0x2, 0x64e, 0x649, 0x3, 0x2, 0x2, 0x2, 
    0x64e, 0x64a, 0x3, 0x2, 0x2, 0x2, 0x64e, 0x64b, 0x3, 0x2, 0x2, 0x2, 
    0x64e, 0x64c, 0x3, 0x2, 0x2, 0x2, 0x64e, 0x64d, 0x3, 0x2, 0x2, 0x2, 
    0x64f, 0xad, 0x3, 0x2, 0x2, 0x2, 0x650, 0x651, 0x9, 0x1f, 0x2, 0x2, 
    0x651, 0xaf, 0x3, 0x2, 0x2, 0x2, 0x652, 0x653, 0x7, 0xc5, 0x2, 0x2, 
    0x653, 0xb1, 0x3, 0x2, 0x2, 0x2, 0x654, 0x657, 0x7, 0xd, 0x2, 0x2, 0x655, 
    0x657, 0x7, 0x6, 0x2, 0x2, 0x656, 0x654, 0x3, 0x2, 0x2, 0x2, 0x656, 
    0x655, 0x3, 0x2, 0x2, 0x2, 0x656, 0x657, 0x3, 0x2, 0x2, 0x2, 0x657, 
    0x658, 0x3, 0x2, 0x2, 0x2, 0x658, 0x659, 0x7, 0xc5, 0x2, 0x2, 0x659, 
    0xb3, 0x3, 0x2, 0x2, 0x2, 0x65a, 0x65b, 0x7, 0xc4, 0x2, 0x2, 0x65b, 
    0xb5, 0x3, 0x2, 0x2, 0x2, 0x65c, 0x65f, 0x7, 0xd, 0x2, 0x2, 0x65d, 0x65f, 
    0x7, 0x6, 0x2, 0x2, 0x65e, 0x65c, 0x3, 0x2, 0x2, 0x2, 0x65e, 0x65d, 
    0x3, 0x2, 0x2, 0x2, 0x65e, 0x65f, 0x3, 0x2, 0x2, 0x2, 0x65f, 0x660, 
    0x3, 0x2, 0x2, 0x2, 0x660, 0x661, 0x7, 0xc4, 0x2, 0x2, 0x661, 0xb7, 
    0x3, 0x2, 0x2, 0x2, 0x662, 0x663, 0x7, 0xc3, 0x2, 0x2, 0x663, 0xb9, 
    0x3, 0x2, 0x2, 0x2, 0x664, 0x667, 0x7, 0xd, 0x2, 0x2, 0x665, 0x667, 
    0x7, 0x6, 0x2, 0x2, 0x666, 0x664, 0x3, 0x2, 0x2, 0x2, 0x666, 0x665, 
    0x3, 0x2, 0x2, 0x2, 0x666, 0x667, 0x3, 0x2, 0x2, 0x2, 0x667, 0x668, 
    0x3, 0x2, 0x2, 0x2, 0x668, 0x669, 0x7, 0xc3, 0x2, 0x2, 0x669, 0xbb, 
    0x3, 0x2, 0x2, 0x2, 0x66a, 0x66b, 0x7, 0x2c, 0x2, 0x2, 0x66b, 0x66c, 
    0x5, 0xc2, 0x62, 0x2, 0x66c, 0xbd, 0x3, 0x2, 0x2, 0x2, 0x66d, 0x66e, 
    0x7, 0x7f, 0x2, 0x2, 0x66e, 0x66f, 0x5, 0xc2, 0x62, 0x2, 0x66f, 0xbf, 
    0x3, 0x2, 0x2, 0x2, 0x670, 0x671, 0x7, 0x80, 0x2, 0x2, 0x671, 0x672, 
    0x5, 0xc2, 0x62, 0x2, 0x672, 0xc1, 0x3, 0x2, 0x2, 0x2, 0x673, 0x674, 
    0x7, 0xc0, 0x2, 0x2, 0x674, 0xc3, 0x3, 0x2, 0x2, 0x2, 0x675, 0x679, 
    0x5, 0xc8, 0x65, 0x2, 0x676, 0x679, 0x5, 0xca, 0x66, 0x2, 0x677, 0x679, 
    0x5, 0xc6, 0x64, 0x2, 0x678, 0x675, 0x3, 0x2, 0x2, 0x2, 0x678, 0x676, 
    0x3, 0x2, 0x2, 0x2, 0x678, 0x677, 0x3, 0x2, 0x2, 0x2, 0x679, 0xc5, 0x3, 
    0x2, 0x2, 0x2, 0x67a, 0x67b, 0x7, 0x81, 0x2, 0x2, 0x67b, 0x67c, 0x5, 
    0x7a, 0x3e, 0x2, 0x67c, 0xc7, 0x3, 0x2, 0x2, 0x2, 0x67d, 0x67e, 0x7, 
    0x81, 0x2, 0x2, 0x67e, 0x696, 0x5, 0xc2, 0x62, 0x2, 0x67f, 0x684, 0x7, 
    0xb2, 0x2, 0x2, 0x680, 0x681, 0x7, 0x9, 0x2, 0x2, 0x681, 0x682, 0x5, 
    0xa4, 0x53, 0x2, 0x682, 0x683, 0x7, 0xa, 0x2, 0x2, 0x683, 0x685, 0x3, 
    0x2, 0x2, 0x2, 0x684, 0x680, 0x3, 0x2, 0x2, 0x2, 0x684, 0x685, 0x3, 
    0x2, 0x2, 0x2, 0x685, 0x697, 0x3, 0x2, 0x2, 0x2, 0x686, 0x68b, 0x7, 
    0xb2, 0x2, 0x2, 0x687, 0x688, 0x7, 0x9, 0x2, 0x2, 0x688, 0x689, 0x5, 
    0xa4, 0x53, 0x2, 0x689, 0x68a, 0x7, 0xa, 0x2, 0x2, 0x68a, 0x68c, 0x3, 
    0x2, 0x2, 0x2, 0x68b, 0x687, 0x3, 0x2, 0x2, 0x2, 0x68b, 0x68c, 0x3, 
    0x2, 0x2, 0x2, 0x68c, 0x68d, 0x3, 0x2, 0x2, 0x2, 0x68d, 0x68e, 0x7, 
    0x6b, 0x2, 0x2, 0x68e, 0x697, 0x7, 0xaa, 0x2, 0x2, 0x68f, 0x694, 0x7, 
    0xaa, 0x2, 0x2, 0x690, 0x691, 0x7, 0x9, 0x2, 0x2, 0x691, 0x692, 0x5, 
    0xa4, 0x53, 0x2, 0x692, 0x693, 0x7, 0xa, 0x2, 0x2, 0x693, 0x695, 0x3, 
    0x2, 0x2, 0x2, 0x694, 0x690, 0x3, 0x2, 0x2, 0x2, 0x694, 0x695, 0x3, 
    0x2, 0x2, 0x2, 0x695, 0x697, 0x3, 0x2, 0x2, 0x2, 0x696, 0x67f, 0x3, 
    0x2, 0x2, 0x2, 0x696, 0x686, 0x3, 0x2, 0x2, 0x2, 0x696, 0x68f, 0x3, 
    0x2, 0x2, 0x2, 0x697, 0xc9, 0x3, 0x2, 0x2, 0x2, 0x698, 0x699, 0x7, 0x81, 
    0x2, 0x2, 0x699, 0x6ee, 0x5, 0xc2, 0x62, 0x2, 0x69a, 0x69f, 0x7, 0xa4, 
    0x2, 0x2, 0x69b, 0x69c, 0x7, 0x9, 0x2, 0x2, 0x69c, 0x69d, 0x5, 0xa4, 
    0x53, 0x2, 0x69d, 0x69e, 0x7, 0xa, 0x2, 0x2, 0x69e, 0x6a0, 0x3, 0x2, 
    0x2, 0x2, 0x69f, 0x69b, 0x3, 0x2, 0x2, 0x2, 0x69f, 0x6a0, 0x3, 0x2, 
    0x2, 0x2, 0x6a0, 0x6ef, 0x3, 0x2, 0x2, 0x2, 0x6a1, 0x6a6, 0x7, 0xa4, 
    0x2, 0x2, 0x6a2, 0x6a3, 0x7, 0x9, 0x2, 0x2, 0x6a3, 0x6a4, 0x5, 0xa4, 
    0x53, 0x2, 0x6a4, 0x6a5, 0x7, 0xa, 0x2, 0x2, 0x6a5, 0x6a7, 0x3, 0x2, 
    0x2, 0x2, 0x6a6, 0x6a2, 0x3, 0x2, 0x2, 0x2, 0x6a6, 0x6a7, 0x3, 0x2, 
    0x2, 0x2, 0x6a7, 0x6a8, 0x3, 0x2, 0x2, 0x2, 0x6a8, 0x6b2, 0x7, 0x6b, 
    0x2, 0x2, 0x6a9, 0x6b3, 0x7, 0xa6, 0x2, 0x2, 0x6aa, 0x6b3, 0x7, 0xa8, 
    0x2, 0x2, 0x6ab, 0x6b0, 0x7, 0xae, 0x2, 0x2, 0x6ac, 0x6ad, 0x7, 0x9, 
    0x2, 0x2, 0x6ad, 0x6ae, 0x5, 0xa6, 0x54, 0x2, 0x6ae, 0x6af, 0x7, 0xa, 
    0x2, 0x2, 0x6af, 0x6b1, 0x3, 0x2, 0x2, 0x2, 0x6b0, 0x6ac, 0x3, 0x2, 
    0x2, 0x2, 0x6b0, 0x6b1, 0x3, 0x2, 0x2, 0x2, 0x6b1, 0x6b3, 0x3, 0x2, 
    0x2, 0x2, 0x6b2, 0x6a9, 0x3, 0x2, 0x2, 0x2, 0x6b2, 0x6aa, 0x3, 0x2, 
    0x2, 0x2, 0x6b2, 0x6ab, 0x3, 0x2, 0x2, 0x2, 0x6b3, 0x6ef, 0x3, 0x2, 
    0x2, 0x2, 0x6b4, 0x6b9, 0x7, 0xa6, 0x2, 0x2, 0x6b5, 0x6b6, 0x7, 0x9, 
    0x2, 0x2, 0x6b6, 0x6b7, 0x5, 0xa4, 0x53, 0x2, 0x6b7, 0x6b8, 0x7, 0xa, 
    0x2, 0x2, 0x6b8, 0x6ba, 0x3, 0x2, 0x2, 0x2, 0x6b9, 0x6b5, 0x3, 0x2, 
    0x2, 0x2, 0x6b9, 0x6ba, 0x3, 0x2, 0x2, 0x2, 0x6ba, 0x6bb, 0x3, 0x2, 
    0x2, 0x2, 0x6bb, 0x6c4, 0x7, 0x6b, 0x2, 0x2, 0x6bc, 0x6c5, 0x7, 0xa8, 
    0x2, 0x2, 0x6bd, 0x6c2, 0x7, 0xae, 0x2, 0x2, 0x6be, 0x6bf, 0x7, 0x9, 
    0x2, 0x2, 0x6bf, 0x6c0, 0x5, 0xa6, 0x54, 0x2, 0x6c0, 0x6c1, 0x7, 0xa, 
    0x2, 0x2, 0x6c1, 0x6c3, 0x3, 0x2, 0x2, 0x2, 0x6c2, 0x6be, 0x3, 0x2, 
    0x2, 0x2, 0x6c2, 0x6c3, 0x3, 0x2, 0x2, 0x2, 0x6c3, 0x6c5, 0x3, 0x2, 
    0x2, 0x2, 0x6c4, 0x6bc, 0x3, 0x2, 0x2, 0x2, 0x6c4, 0x6bd, 0x3, 0x2, 
    0x2, 0x2, 0x6c5, 0x6ef, 0x3, 0x2, 0x2, 0x2, 0x6c6, 0x6cb, 0x7, 0xa8, 
    0x2, 0x2, 0x6c7, 0x6c8, 0x7, 0x9, 0x2, 0x2, 0x6c8, 0x6c9, 0x5, 0xa4, 
    0x53, 0x2, 0x6c9, 0x6ca, 0x7, 0xa, 0x2, 0x2, 0x6ca, 0x6cc, 0x3, 0x2, 
    0x2, 0x2, 0x6cb, 0x6c7, 0x3, 0x2, 0x2, 0x2, 0x6cb, 0x6cc, 0x3, 0x2, 
    0x2, 0x2, 0x6cc, 0x6cd, 0x3, 0x2, 0x2, 0x2, 0x6cd, 0x6ce, 0x7, 0x6b, 
    0x2, 0x2, 0x6ce, 0x6d3, 0x7, 0xae, 0x2, 0x2, 0x6cf, 0x6d0, 0x7, 0x9, 
    0x2, 0x2, 0x6d0, 0x6d1, 0x5, 0xa6, 0x54, 0x2, 0x6d1, 0x6d2, 0x7, 0xa, 
    0x2, 0x2, 0x6d2, 0x6d4, 0x3, 0x2, 0x2, 0x2, 0x6d3, 0x6cf, 0x3, 0x2, 
    0x2, 0x2, 0x6d3, 0x6d4, 0x3, 0x2, 0x2, 0x2, 0x6d4, 0x6ef, 0x3, 0x2, 
    0x2, 0x2, 0x6d5, 0x6da, 0x7, 0xa6, 0x2, 0x2, 0x6d6, 0x6d7, 0x7, 0x9, 
    0x2, 0x2, 0x6d7, 0x6d8, 0x5, 0xa4, 0x53, 0x2, 0x6d8, 0x6d9, 0x7, 0xa, 
    0x2, 0x2, 0x6d9, 0x6db, 0x3, 0x2, 0x2, 0x2, 0x6da, 0x6d6, 0x3, 0x2, 
    0x2, 0x2, 0x6da, 0x6db, 0x3, 0x2, 0x2, 0x2, 0x6db, 0x6ef, 0x3, 0x2, 
    0x2, 0x2, 0x6dc, 0x6e1, 0x7, 0xa8, 0x2, 0x2, 0x6dd, 0x6de, 0x7, 0x9, 
    0x2, 0x2, 0x6de, 0x6df, 0x5, 0xa4, 0x53, 0x2, 0x6df, 0x6e0, 0x7, 0xa, 
    0x2, 0x2, 0x6e0, 0x6e2, 0x3, 0x2, 0x2, 0x2, 0x6e1, 0x6dd, 0x3, 0x2, 
    0x2, 0x2, 0x6e1, 0x6e2, 0x3, 0x2, 0x2, 0x2, 0x6e2, 0x6ef, 0x3, 0x2, 
    0x2, 0x2, 0x6e3, 0x6ec, 0x7, 0xae, 0x2, 0x2, 0x6e4, 0x6e5, 0x7, 0x9, 
    0x2, 0x2, 0x6e5, 0x6e8, 0x5, 0xa4, 0x53, 0x2, 0x6e6, 0x6e7, 0x7, 0x5, 
    0x2, 0x2, 0x6e7, 0x6e9, 0x5, 0xa6, 0x54, 0x2, 0x6e8, 0x6e6, 0x3, 0x2, 
    0x2, 0x2, 0x6e8, 0x6e9, 0x3, 0x2, 0x2, 0x2, 0x6e9, 0x6ea, 0x3, 0x2, 
    0x2, 0x2, 0x6ea, 0x6eb, 0x7, 0xa, 0x2, 0x2, 0x6eb, 0x6ed, 0x3, 0x2, 
    0x2, 0x2, 0x6ec, 0x6e4, 0x3, 0x2, 0x2, 0x2, 0x6ec, 0x6ed, 0x3, 0x2, 
    0x2, 0x2, 0x6ed, 0x6ef, 0x3, 0x2, 0x2, 0x2, 0x6ee, 0x69a, 0x3, 0x2, 
    0x2, 0x2, 0x6ee, 0x6a1, 0x3, 0x2, 0x2, 0x2, 0x6ee, 0x6b4, 0x3, 0x2, 
    0x2, 0x2, 0x6ee, 0x6c6, 0x3, 0x2, 0x2, 0x2, 0x6ee, 0x6d5, 0x3, 0x2, 
    0x2, 0x2, 0x6ee, 0x6dc, 0x3, 0x2, 0x2, 0x2, 0x6ee, 0x6e3, 0x3, 0x2, 
    0x2, 0x2, 0x6ef, 0xcb, 0x3, 0x2, 0x2, 0x2, 0x6f0, 0x6f1, 0x9, 0x20, 
    0x2, 0x2, 0x6f1, 0xcd, 0x3, 0x2, 0x2, 0x2, 0xf1, 0xd0, 0xd8, 0xdf, 0xe6, 
    0xef, 0xf7, 0xfd, 0x102, 0x10a, 0x113, 0x11e, 0x122, 0x128, 0x134, 0x13c, 
    0x141, 0x149, 0x14c, 0x152, 0x158, 0x15a, 0x15f, 0x164, 0x168, 0x16d, 
    0x171, 0x177, 0x17f, 0x182, 0x188, 0x18e, 0x190, 0x198, 0x19e, 0x1a1, 
    0x1a6, 0x1af, 0x1b2, 0x1bb, 0x1c0, 0x1ca, 0x1cd, 0x1d3, 0x1d6, 0x1dc, 
    0x1df, 0x1e1, 0x1e7, 0x1ed, 0x1f3, 0x1f6, 0x1ff, 0x202, 0x20b, 0x20d, 
    0x213, 0x218, 0x21f, 0x221, 0x226, 0x22b, 0x233, 0x238, 0x247, 0x250, 
    0x253, 0x257, 0x260, 0x265, 0x26a, 0x272, 0x275, 0x279, 0x280, 0x288, 
    0x28b, 0x29a, 0x2a1, 0x2a5, 0x2a9, 0x2af, 0x2b4, 0x2b7, 0x2bc, 0x2bf, 
    0x2c3, 0x2c8, 0x2cb, 0x2cd, 0x2d1, 0x2d7, 0x2de, 0x2e6, 0x2ec, 0x2f2, 
    0x2f9, 0x2ff, 0x303, 0x30b, 0x311, 0x315, 0x31a, 0x31d, 0x321, 0x325, 
    0x328, 0x32b, 0x333, 0x33b, 0x342, 0x347, 0x349, 0x34b, 0x350, 0x358, 
    0x366, 0x36b, 0x36e, 0x379, 0x37e, 0x380, 0x383, 0x387, 0x38a, 0x393, 
    0x398, 0x39c, 0x3a0, 0x3a7, 0x3aa, 0x3b1, 0x3b5, 0x3bb, 0x3c0, 0x3c2, 
    0x3c6, 0x3cc, 0x3d0, 0x3d9, 0x3df, 0x3e2, 0x3e7, 0x3eb, 0x3ee, 0x3f2, 
    0x3f8, 0x400, 0x404, 0x407, 0x40f, 0x418, 0x41e, 0x420, 0x427, 0x42c, 
    0x42f, 0x432, 0x439, 0x443, 0x448, 0x44f, 0x459, 0x463, 0x469, 0x470, 
    0x499, 0x4a2, 0x4a4, 0x4a8, 0x4ca, 0x4cd, 0x4d7, 0x4da, 0x4e4, 0x4e7, 
    0x4f3, 0x505, 0x50c, 0x50e, 0x517, 0x51a, 0x51d, 0x521, 0x52f, 0x537, 
    0x540, 0x549, 0x552, 0x55b, 0x567, 0x56a, 0x577, 0x580, 0x58d, 0x593, 
    0x5ad, 0x5b0, 0x5b7, 0x5bc, 0x5c5, 0x5ce, 0x5d1, 0x5d6, 0x5dd, 0x5df, 
    0x5e7, 0x5ec, 0x5f1, 0x5f8, 0x5fd, 0x602, 0x60b, 0x62a, 0x62e, 0x632, 
    0x646, 0x64e, 0x656, 0x65e, 0x666, 0x678, 0x684, 0x68b, 0x694, 0x696, 
    0x69f, 0x6a6, 0x6b0, 0x6b2, 0x6b9, 0x6c2, 0x6c4, 0x6cb, 0x6d3, 0x6da, 
    0x6e1, 0x6e8, 0x6ec, 0x6ee, 
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
