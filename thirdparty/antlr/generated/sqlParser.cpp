
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
    setState(216);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ANTLR) {
      setState(214);
      match(sqlParser::ANTLR);
      setState(215);
      match(sqlParser::T__0);
    }
    setState(218);
    statementChoice();
    setState(219);
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
    setState(224);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(221);
      queryStatement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(222);
      explainStatement();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(223);
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
    setState(226);
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
    setState(231);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(228);
      queryHint();
      setState(233);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(234);
    match(sqlParser::EXPLAIN);
    setState(238);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::FORMAT || ((((_la - 100) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 100)) & ((1ULL << (sqlParser::STAGE - 100))
      | (1ULL << (sqlParser::WITH_NAMES - 100))
      | (1ULL << (sqlParser::WITH_TYPES - 100)))) != 0)) {
      setState(235);
      explainOptions();
      setState(240);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(241);
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
    setState(247);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::STAGE: {
        enterOuterAlt(_localctx, 1);
        setState(243);
        explainStageList();
        break;
      }

      case sqlParser::FORMAT: {
        enterOuterAlt(_localctx, 2);
        setState(244);
        explainFormat();
        break;
      }

      case sqlParser::WITH_NAMES: {
        enterOuterAlt(_localctx, 3);
        setState(245);
        match(sqlParser::WITH_NAMES);
        break;
      }

      case sqlParser::WITH_TYPES: {
        enterOuterAlt(_localctx, 4);
        setState(246);
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
    setState(249);
    match(sqlParser::STAGE);
    setState(250);
    explainRange();
    setState(255);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(251);
      match(sqlParser::T__2);
      setState(252);
      explainRange();
      setState(257);
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
    setState(258);
    dynamic_cast<ExplainRangeContext *>(_localctx)->begin = explainRangeChoice();
    setState(261);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__3 || _la == sqlParser::TO) {
      setState(259);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__3 || _la == sqlParser::TO)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(260);
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
    setState(266);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__4:
      case sqlParser::ALL: {
        enterOuterAlt(_localctx, 1);
        setState(263);
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
        setState(264);
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
        setState(265);
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
    setState(268);
    match(sqlParser::FORMAT);
    setState(269);
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
    setState(274);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(271);
      queryHint();
      setState(276);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(277);
    _la = _input->LA(1);
    if (!(_la == sqlParser::STATS

    || _la == sqlParser::STATISTICS)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(278);
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
    setState(283);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(280);
      queryHint();
      setState(285);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(294);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx)) {
    case 1: {
      setState(286);
      selectQuery();
      break;
    }

    case 2: {
      setState(287);
      setQuery();
      break;
    }

    case 3: {
      setState(288);
      insertQuery();
      break;
    }

    case 4: {
      setState(289);
      deleteQuery();
      break;
    }

    case 5: {
      setState(290);
      updateQuery();
      break;
    }

    case 6: {
      setState(291);
      createQuery();
      break;
    }

    case 7: {
      setState(292);
      dropQuery();
      break;
    }

    case 8: {
      setState(293);
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
    setState(296);
    match(sqlParser::HINT_START);
    setState(298);
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
      setState(297);
      hint();
    }
    setState(304);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(300);
      match(sqlParser::T__2);
      setState(301);
      hint();
      setState(306);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(307);
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
    setState(329);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(309);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(310);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(311);
      match(sqlParser::T__5);
      setState(312);
      hintValue();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(314);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(316);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__5) {
        setState(315);
        match(sqlParser::T__5);
      }
      setState(318);
      match(sqlParser::T__6);
      setState(319);
      hintValue();
      setState(324);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(320);
        match(sqlParser::T__2);
        setState(321);
        hintValue();
        setState(326);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(327);
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
    setState(340);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(331);
      match(sqlParser::WITH);
      setState(332);
      withQuery();
      setState(337);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(333);
        match(sqlParser::T__2);
        setState(334);
        withQuery();
        setState(339);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(342);
    selectExpr();
    setState(346);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(343);
      match(sqlParser::ORDER);
      setState(344);
      match(sqlParser::BY);
      setState(345);
      dynamic_cast<SelectQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(354);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(348);
      match(sqlParser::LIMIT);
      setState(349);
      dynamic_cast<SelectQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(352);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(350);
        match(sqlParser::OFFSET);
        setState(351);
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
    setState(383);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__8:
      case sqlParser::SELECT: {
        enterOuterAlt(_localctx, 1);
        setState(356);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__8 || _la == sqlParser::SELECT)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(359);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::ALL: {
            setState(357);
            match(sqlParser::ALL);
            break;
          }

          case sqlParser::DISTINCT: {
            setState(358);
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
        setState(361);
        selectList();
        setState(364);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
        case 1: {
          setState(362);
          match(sqlParser::FROM);
          setState(363);
          tableExpr(0);
          break;
        }

        }
        setState(368);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
        case 1: {
          setState(366);
          match(sqlParser::WHERE);
          setState(367);
          dynamic_cast<SelectExprContext *>(_localctx)->where = predicate();
          break;
        }

        }
        setState(373);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
        case 1: {
          setState(370);
          match(sqlParser::GROUP);
          setState(371);
          match(sqlParser::BY);
          setState(372);
          dynamic_cast<SelectExprContext *>(_localctx)->groupBy = groupByList();
          break;
        }

        }
        setState(377);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
        case 1: {
          setState(375);
          match(sqlParser::HAVING);
          setState(376);
          dynamic_cast<SelectExprContext *>(_localctx)->having = predicate();
          break;
        }

        }
        break;
      }

      case sqlParser::T__6: {
        enterOuterAlt(_localctx, 2);
        setState(379);
        match(sqlParser::T__6);
        setState(380);
        selectExpr();
        setState(381);
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
    setState(394);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(385);
      match(sqlParser::WITH);
      setState(386);
      withQuery();
      setState(391);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(387);
        match(sqlParser::T__2);
        setState(388);
        withQuery();
        setState(393);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(396);
    setExpr(0);
    setState(400);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(397);
      match(sqlParser::ORDER);
      setState(398);
      match(sqlParser::BY);
      setState(399);
      dynamic_cast<SetQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(408);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(402);
      match(sqlParser::LIMIT);
      setState(403);
      dynamic_cast<SetQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(406);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(404);
        match(sqlParser::OFFSET);
        setState(405);
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
    setState(416);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx)) {
    case 1: {
      setState(411);
      selectExpr();
      break;
    }

    case 2: {
      setState(412);
      match(sqlParser::T__6);
      setState(413);
      setExpr(0);
      setState(414);
      match(sqlParser::T__7);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(430);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<SetExprContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleSetExpr);
        setState(418);

        if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
        setState(419);
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
        setState(422);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::DISTINCT: {
            setState(420);
            match(sqlParser::DISTINCT);
            break;
          }

          case sqlParser::ALL: {
            setState(421);
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
        setState(425);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::CORRESPONDING) {
          setState(424);
          corrClause();
        }
        setState(427);
        setExpr(3); 
      }
      setState(432);
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
    setState(433);
    dynamic_cast<CorrClauseContext *>(_localctx)->corr = match(sqlParser::CORRESPONDING);
    setState(439);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::BY) {
      setState(434);
      match(sqlParser::BY);
      setState(435);
      match(sqlParser::T__6);
      setState(436);
      corrList();
      setState(437);
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
  enterRule(_localctx, 36, sqlParser::RuleWithQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(442);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::RECURSIVE) {
      setState(441);
      match(sqlParser::RECURSIVE);
    }
    setState(444);
    dynamic_cast<WithQueryContext *>(_localctx)->name = tableName();
    setState(446);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__6) {
      setState(445);
      colAliases();
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
  size_t startState = 38;
  enterRecursionRule(_localctx, 38, sqlParser::RuleTableExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(485);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 47, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<FetchContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(454);
      dynamic_cast<FetchContext *>(_localctx)->ref = tableReference();
      setState(459);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 40, _ctx)) {
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
      setState(471);
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
        setState(469);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 42, _ctx)) {
        case 1: {
          setState(468);
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
      setState(473);
      match(sqlParser::T__6);
      setState(474);
      valuesExpr();
      setState(475);
      match(sqlParser::T__7);
      setState(483);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx)) {
      case 1: {
        setState(477);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(476);
          match(sqlParser::AS);
        }
        setState(479);
        dynamic_cast<ValuesSubqueryContext *>(_localctx)->as = tableAlias();
        setState(481);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 45, _ctx)) {
        case 1: {
          setState(480);
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
    setState(547);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 57, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(545);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<CrossJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(487);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(491);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(488);
            queryHint();
            setState(493);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(497);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case sqlParser::T__2: {
              setState(494);
              match(sqlParser::T__2);
              break;
            }

            case sqlParser::CROSS: {
              setState(495);
              match(sqlParser::CROSS);
              setState(496);
              match(sqlParser::JOIN);
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(499);
          tableExpr(7);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<NaturalJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(500);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(504);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(501);
            queryHint();
            setState(506);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(507);
          match(sqlParser::NATURAL);

          setState(509);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 55) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 55)) & ((1ULL << (sqlParser::FULL - 55))
            | (1ULL << (sqlParser::INNER - 55))
            | (1ULL << (sqlParser::LEFT - 55))
            | (1ULL << (sqlParser::RIGHT - 55)))) != 0)) {
            setState(508);
            dynamic_cast<NaturalJoinContext *>(_localctx)->type = joinType();
          }
          setState(511);
          match(sqlParser::JOIN);
          setState(512);
          tableExpr(6);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<JoinOnContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(513);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(517);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(514);
            queryHint();
            setState(519);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(521);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::ANTI)
            | (1ULL << sqlParser::FULL)
            | (1ULL << sqlParser::INNER))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::LEFT - 69))
            | (1ULL << (sqlParser::RIGHT - 69))
            | (1ULL << (sqlParser::SEMI - 69)))) != 0)) {
            setState(520);
            dynamic_cast<JoinOnContext *>(_localctx)->type = extendedJoinType();
          }
          setState(523);
          match(sqlParser::JOIN);
          setState(524);
          tableExpr(0);
          setState(525);
          match(sqlParser::ON);
          setState(526);
          joinPredicate();
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<JoinUsingContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(528);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(532);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::HINT_START) {
            setState(529);
            queryHint();
            setState(534);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(536);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::ANTI)
            | (1ULL << sqlParser::FULL)
            | (1ULL << sqlParser::INNER))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::LEFT - 69))
            | (1ULL << (sqlParser::RIGHT - 69))
            | (1ULL << (sqlParser::SEMI - 69)))) != 0)) {
            setState(535);
            dynamic_cast<JoinUsingContext *>(_localctx)->type = extendedJoinType();
          }
          setState(538);
          match(sqlParser::JOIN);
          setState(539);
          tableExpr(0);
          setState(540);
          match(sqlParser::USING);
          setState(541);
          match(sqlParser::T__6);
          setState(542);
          joinUsingList();
          setState(543);
          match(sqlParser::T__7);
          break;
        }

        } 
      }
      setState(549);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 57, _ctx);
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
    setState(558);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 59, _ctx)) {
    case 1: {
      setState(553);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 58, _ctx)) {
      case 1: {
        setState(550);
        dynamic_cast<TableReferenceContext *>(_localctx)->catalog = catalogName();
        setState(551);
        match(sqlParser::T__9);
        break;
      }

      }
      setState(555);
      dynamic_cast<TableReferenceContext *>(_localctx)->schema = schemaName();
      setState(556);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(560);
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
    setState(567);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::INNER: {
        enterOuterAlt(_localctx, 1);
        setState(562);
        match(sqlParser::INNER);
        break;
      }

      case sqlParser::FULL:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 2);
        setState(563);
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
        setState(565);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::OUTER) {
          setState(564);
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
    setState(572);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::FULL:
      case sqlParser::INNER:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 1);
        setState(569);
        joinType();
        break;
      }

      case sqlParser::SEMI: {
        enterOuterAlt(_localctx, 2);
        setState(570);
        match(sqlParser::SEMI);
        break;
      }

      case sqlParser::ANTI: {
        enterOuterAlt(_localctx, 3);
        setState(571);
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
    setState(574);
    match(sqlParser::HINT_START);
    setState(575);
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
    setState(590);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__5

    || _la == sqlParser::T__6) {
      setState(577);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__5) {
        setState(576);
        match(sqlParser::T__5);
      }
      setState(579);
      match(sqlParser::T__6);
      setState(580);
      dynamic_cast<JoinHintContext *>(_localctx)->values = hintValue();
      setState(585);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(581);
        match(sqlParser::T__2);
        setState(582);
        hintValue();
        setState(587);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(588);
      match(sqlParser::T__7);
    }
    setState(592);
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
    setState(594);
    match(sqlParser::VALUES);
    setState(595);
    match(sqlParser::T__6);
    setState(596);
    valueList();
    setState(597);
    match(sqlParser::T__7);
    setState(605);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(598);
      match(sqlParser::T__2);
      setState(599);
      match(sqlParser::T__6);
      setState(600);
      valueList();
      setState(601);
      match(sqlParser::T__7);
      setState(607);
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
    setState(617);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(608);
      match(sqlParser::WITH);
      setState(609);
      withQuery();
      setState(614);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(610);
        match(sqlParser::T__2);
        setState(611);
        withQuery();
        setState(616);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(619);
    match(sqlParser::INSERT);
    setState(621);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 69, _ctx)) {
    case 1: {
      setState(620);
      match(sqlParser::INTO);
      break;
    }

    }
    setState(623);
    dynamic_cast<InsertQueryContext *>(_localctx)->ref = tableReference();
    setState(635);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 71, _ctx)) {
    case 1: {
      setState(624);
      match(sqlParser::T__6);
      setState(625);
      columnName();
      setState(630);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(626);
        match(sqlParser::T__2);
        setState(627);
        columnName();
        setState(632);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(633);
      match(sqlParser::T__7);
      break;
    }

    }
    setState(640);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 72, _ctx)) {
    case 1: {
      setState(637);
      valuesExpr();
      break;
    }

    case 2: {
      setState(638);
      selectQuery();
      break;
    }

    case 3: {
      setState(639);
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
    setState(651);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(642);
      match(sqlParser::WITH);
      setState(643);
      withQuery();
      setState(648);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(644);
        match(sqlParser::T__2);
        setState(645);
        withQuery();
        setState(650);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(653);
    match(sqlParser::DELETE);
    setState(655);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::FROM) {
      setState(654);
      match(sqlParser::FROM);
    }
    setState(657);
    dynamic_cast<DeleteQueryContext *>(_localctx)->ref = tableReference();
    setState(662);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(658);
        match(sqlParser::WHERE);
        setState(659);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(660);
        match(sqlParser::FROM);
        setState(661);
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
    setState(673);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(664);
      match(sqlParser::WITH);
      setState(665);
      withQuery();
      setState(670);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(666);
        match(sqlParser::T__2);
        setState(667);
        withQuery();
        setState(672);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(675);
    match(sqlParser::UPDATE);
    setState(676);
    dynamic_cast<UpdateQueryContext *>(_localctx)->ref = tableReference();
    setState(677);
    match(sqlParser::SET);
    setState(678);
    columnName();
    setState(679);
    match(sqlParser::T__5);
    setState(680);
    expr(0);
    setState(688);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(681);
      match(sqlParser::T__2);
      setState(682);
      columnName();
      setState(683);
      match(sqlParser::T__5);
      setState(684);
      expr(0);
      setState(690);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(695);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(691);
        match(sqlParser::WHERE);
        setState(692);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(693);
        match(sqlParser::FROM);
        setState(694);
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
    setState(699);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 81, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(697);
      createTableQuery();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(698);
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
    setState(701);
    match(sqlParser::CREATE);
    setState(703);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::REFERENCE) {
      setState(702);
      dynamic_cast<CreateTableQueryContext *>(_localctx)->reference = match(sqlParser::REFERENCE);
    }
    setState(705);
    match(sqlParser::TABLE);
    setState(709);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::IF) {
      setState(706);
      match(sqlParser::IF);
      setState(707);
      match(sqlParser::NOT);
      setState(708);
      match(sqlParser::EXISTS);
    }
    setState(711);
    dynamic_cast<CreateTableQueryContext *>(_localctx)->ref = tableReference();
    setState(739);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 91, _ctx)) {
    case 1: {
      setState(712);
      tableDefinition();
      break;
    }

    case 2: {
      setState(714);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 84, _ctx)) {
      case 1: {
        setState(713);
        tableDefinition();
        break;
      }

      }
      setState(717);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::AS) {
        setState(716);
        match(sqlParser::AS);
      }
      setState(719);
      asSubquery();
      setState(725);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::WITH) {
        setState(720);
        match(sqlParser::WITH);
        setState(722);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::NO) {
          setState(721);
          dynamic_cast<CreateTableQueryContext *>(_localctx)->noData = match(sqlParser::NO);
        }
        setState(724);
        match(sqlParser::DATA);
      }
      break;
    }

    case 3: {
      setState(727);
      columnNameList();
      setState(729);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::AS) {
        setState(728);
        match(sqlParser::AS);
      }
      setState(731);
      asSubquery();
      setState(737);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::WITH) {
        setState(732);
        match(sqlParser::WITH);
        setState(734);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::NO) {
          setState(733);
          dynamic_cast<CreateTableQueryContext *>(_localctx)->noData = match(sqlParser::NO);
        }
        setState(736);
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
    setState(741);
    match(sqlParser::CREATE);
    setState(743);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(742);
      match(sqlParser::UNIQUE);
    }
    setState(745);
    _la = _input->LA(1);
    if (!(_la == sqlParser::INDEX

    || _la == sqlParser::KEY)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(749);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::IF) {
      setState(746);
      match(sqlParser::IF);
      setState(747);
      match(sqlParser::NOT);
      setState(748);
      match(sqlParser::EXISTS);
    }
    setState(751);
    dynamic_cast<CreateIndexQueryContext *>(_localctx)->idxName = indexName();
    setState(752);
    match(sqlParser::ON);
    setState(753);
    dynamic_cast<CreateIndexQueryContext *>(_localctx)->tblName = tableReference();
    setState(756);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::USING) {
      setState(754);
      match(sqlParser::USING);
      setState(755);
      dynamic_cast<CreateIndexQueryContext *>(_localctx)->method = indexMethod();
    }
    setState(758);
    match(sqlParser::T__6);
    setState(759);
    expr(0);
    setState(764);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(760);
      match(sqlParser::T__2);
      setState(761);
      expr(0);
      setState(766);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(767);
    match(sqlParser::T__7);
    setState(770);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(768);
      match(sqlParser::WITH);
      setState(769);
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
    setState(789);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 99, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<DropQueryContext *>(_tracker.createInstance<sqlParser::DropTableContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(772);
      match(sqlParser::DROP);
      setState(773);
      match(sqlParser::TABLE);
      setState(776);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::IF) {
        setState(774);
        match(sqlParser::IF);
        setState(775);
        match(sqlParser::EXISTS);
      }
      setState(778);
      dynamic_cast<DropTableContext *>(_localctx)->name = tableReference();
      break;
    }

    case 2: {
      _localctx = dynamic_cast<DropQueryContext *>(_tracker.createInstance<sqlParser::DropIndexContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(779);
      match(sqlParser::DROP);
      setState(780);
      _la = _input->LA(1);
      if (!(_la == sqlParser::INDEX

      || _la == sqlParser::KEY)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(783);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::IF) {
        setState(781);
        match(sqlParser::IF);
        setState(782);
        match(sqlParser::EXISTS);
      }
      setState(785);
      dynamic_cast<DropIndexContext *>(_localctx)->idxName = indexName();
      setState(786);
      match(sqlParser::ON);
      setState(787);
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
    setState(791);
    match(sqlParser::TRUNCATE);
    setState(793);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 100, _ctx)) {
    case 1: {
      setState(792);
      match(sqlParser::TABLE);
      break;
    }

    }
    setState(795);
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
    setState(797);
    match(sqlParser::T__6);
    setState(801);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 101, _ctx)) {
    case 1: {
      setState(798);
      columnDefinition();
      break;
    }

    case 2: {
      setState(799);
      tableConstraint();
      break;
    }

    case 3: {
      setState(800);
      indexDefinition();
      break;
    }

    }
    setState(811);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(803);
      match(sqlParser::T__2);
      setState(807);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 102, _ctx)) {
      case 1: {
        setState(804);
        columnDefinition();
        break;
      }

      case 2: {
        setState(805);
        tableConstraint();
        break;
      }

      case 3: {
        setState(806);
        indexDefinition();
        break;
      }

      }
      setState(813);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(814);
    match(sqlParser::T__7);
    setState(816);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::REFERENCE) {
      setState(815);
      match(sqlParser::REFERENCE);
    }
    setState(819);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::PARTITION) {
      setState(818);
      tablePartitioning();
    }
    setState(823);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 106, _ctx)) {
    case 1: {
      setState(821);
      match(sqlParser::WITH);
      setState(822);
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
    setState(825);
    match(sqlParser::PARTITION);
    setState(827);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::BY) {
      setState(826);
      match(sqlParser::BY);
    }
    setState(830);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::HASH

    || _la == sqlParser::VALUE) {
      setState(829);
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
    setState(833);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ON) {
      setState(832);
      match(sqlParser::ON);
    }
    setState(835);
    match(sqlParser::T__6);
    setState(836);
    expr(0);
    setState(841);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(837);
      match(sqlParser::T__2);
      setState(838);
      expr(0);
      setState(843);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(844);
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
    setState(846);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->name = columnName();
    setState(847);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->type = dataType();
    setState(865);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 78) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 78)) & ((1ULL << (sqlParser::NOT - 78))
      | (1ULL << (sqlParser::SQL_NULL - 78))
      | (1ULL << (sqlParser::PRIMARY - 78))
      | (1ULL << (sqlParser::UNIQUE - 78)))) != 0)) {
      setState(863);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 114, _ctx)) {
      case 1: {
        setState(849);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::NOT) {
          setState(848);
          dynamic_cast<ColumnDefinitionContext *>(_localctx)->notNullable = match(sqlParser::NOT);
        }
        setState(851);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->nullable = match(sqlParser::SQL_NULL);
        break;
      }

      case 2: {
        setState(852);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->primary = match(sqlParser::PRIMARY);
        setState(853);
        match(sqlParser::KEY);
        break;
      }

      case 3: {
        setState(854);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
        setState(856);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::KEY) {
          setState(855);
          match(sqlParser::KEY);
        }
        break;
      }

      case 4: {
        setState(858);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
        setState(859);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->mono = match(sqlParser::MONOTONIC);
        setState(861);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::KEY) {
          setState(860);
          match(sqlParser::KEY);
        }
        break;
      }

      }
      setState(867);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(870);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(868);
      match(sqlParser::WITH);
      setState(869);
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
    setState(872);
    match(sqlParser::T__6);
    setState(873);
    columnName();
    setState(878);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(874);
      match(sqlParser::T__2);
      setState(875);
      columnName();
      setState(880);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(881);
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
    setState(918);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::PRIMARY: {
        enterOuterAlt(_localctx, 1);
        setState(883);
        dynamic_cast<TableConstraintContext *>(_localctx)->primary = match(sqlParser::PRIMARY);
        setState(884);
        match(sqlParser::KEY);
        setState(897);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 119, _ctx)) {
        case 1: {
          setState(885);
          expr(0);
          break;
        }

        case 2: {
          setState(886);
          match(sqlParser::T__6);
          setState(887);
          expr(0);
          setState(892);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::T__2) {
            setState(888);
            match(sqlParser::T__2);
            setState(889);
            expr(0);
            setState(894);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(895);
          match(sqlParser::T__7);
          break;
        }

        }
        break;
      }

      case sqlParser::MONOTONIC:
      case sqlParser::UNIQUE: {
        enterOuterAlt(_localctx, 2);
        setState(900);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::UNIQUE) {
          setState(899);
          dynamic_cast<TableConstraintContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
        }
        setState(902);
        dynamic_cast<TableConstraintContext *>(_localctx)->mono = match(sqlParser::MONOTONIC);
        setState(903);
        match(sqlParser::KEY);
        setState(916);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 122, _ctx)) {
        case 1: {
          setState(904);
          expr(0);
          break;
        }

        case 2: {
          setState(905);
          match(sqlParser::T__6);
          setState(906);
          expr(0);
          setState(911);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::T__2) {
            setState(907);
            match(sqlParser::T__2);
            setState(908);
            expr(0);
            setState(913);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(914);
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
    setState(921);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(920);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
    }
    setState(923);
    _la = _input->LA(1);
    if (!(_la == sqlParser::INDEX

    || _la == sqlParser::KEY)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(925);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 125, _ctx)) {
    case 1: {
      setState(924);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->name = indexName();
      break;
    }

    }
    setState(928);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ON) {
      setState(927);
      match(sqlParser::ON);
    }
    setState(942);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 128, _ctx)) {
    case 1: {
      setState(930);
      expr(0);
      break;
    }

    case 2: {
      setState(931);
      match(sqlParser::T__6);
      setState(932);
      expr(0);
      setState(937);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(933);
        match(sqlParser::T__2);
        setState(934);
        expr(0);
        setState(939);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(940);
      match(sqlParser::T__7);
      break;
    }

    }
    setState(946);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::USING) {
      setState(944);
      match(sqlParser::USING);
      setState(945);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->method = indexMethod();
    }
    setState(950);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(948);
      match(sqlParser::WITH);
      setState(949);
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
    setState(952);
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

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(954);
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
  enterRule(_localctx, 82, sqlParser::RuleSqlDataType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1018);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 142, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(956);
      match(sqlParser::BOOLEAN);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(957);
      match(sqlParser::INTEGER);
      setState(965);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(958);
        match(sqlParser::T__6);
        setState(959);
        precision();
        setState(961);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 137) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 137)) & ((1ULL << (sqlParser::BITS - 137))
          | (1ULL << (sqlParser::BYTES - 137))
          | (1ULL << (sqlParser::DIGITS - 137)))) != 0)) {
          setState(960);
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
        setState(963);
        match(sqlParser::T__7);
      }
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(967);
      match(sqlParser::DECIMAL);
      setState(982);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(968);
        match(sqlParser::T__6);
        setState(969);
        precision();
        setState(971);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::DIGITS) {
          setState(970);
          match(sqlParser::DIGITS);
        }
        setState(978);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__2) {
          setState(973);
          match(sqlParser::T__2);
          setState(974);
          scale();
          setState(976);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::DIGITS) {
            setState(975);
            match(sqlParser::DIGITS);
          }
        }
        setState(980);
        match(sqlParser::T__7);
      }
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(984);
      match(sqlParser::FLOAT);
      setState(992);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(985);
        match(sqlParser::T__6);
        setState(986);
        precision();
        setState(988);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 137) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 137)) & ((1ULL << (sqlParser::BITS - 137))
          | (1ULL << (sqlParser::BYTES - 137))
          | (1ULL << (sqlParser::DIGITS - 137)))) != 0)) {
          setState(987);
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
        setState(990);
        match(sqlParser::T__7);
      }
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(994);
      match(sqlParser::DATE);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(995);
      match(sqlParser::TIME);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(996);
      match(sqlParser::TIMESTAMP);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(997);
      fixedLenChar();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(998);
      varyingLenChar();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(999);
      match(sqlParser::BINARY);

      setState(1000);
      match(sqlParser::T__6);
      setState(1001);
      size();
      setState(1003);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::BYTE

      || _la == sqlParser::BYTES) {
        setState(1002);
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
      setState(1005);
      match(sqlParser::T__7);
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(1007);
      match(sqlParser::VARBINARY);
      setState(1015);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1008);
        match(sqlParser::T__6);
        setState(1009);
        size();
        setState(1011);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::BYTE

        || _la == sqlParser::BYTES) {
          setState(1010);
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
        setState(1013);
        match(sqlParser::T__7);
      }
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(1017);
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
  enterRule(_localctx, 84, sqlParser::RuleFixedLenChar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1020);
    _la = _input->LA(1);
    if (!(_la == sqlParser::CHAR

    || _la == sqlParser::CHARACTER)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(1021);
    match(sqlParser::T__6);
    setState(1022);
    size();
    setState(1024);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 128) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::CHAR - 128))
      | (1ULL << (sqlParser::BYTE - 128))
      | (1ULL << (sqlParser::BYTES - 128))
      | (1ULL << (sqlParser::CHARS - 128)))) != 0)) {
      setState(1023);
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
    setState(1026);
    match(sqlParser::T__7);
    setState(1029);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::COLLATE) {
      setState(1027);
      match(sqlParser::COLLATE);
      setState(1028);
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
  enterRule(_localctx, 86, sqlParser::RuleVaryingLenChar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1048);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::CHAR:
      case sqlParser::CHARACTER:
      case sqlParser::VARCHAR: {
        setState(1036);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::CHAR:
          case sqlParser::CHARACTER: {
            setState(1031);
            _la = _input->LA(1);
            if (!(_la == sqlParser::CHAR

            || _la == sqlParser::CHARACTER)) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
            setState(1033);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == sqlParser::VARYING) {
              setState(1032);
              match(sqlParser::VARYING);
            }
            break;
          }

          case sqlParser::VARCHAR: {
            setState(1035);
            match(sqlParser::VARCHAR);
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(1045);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__6) {
          setState(1038);
          match(sqlParser::T__6);
          setState(1039);
          size();
          setState(1041);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 128) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::CHAR - 128))
            | (1ULL << (sqlParser::BYTE - 128))
            | (1ULL << (sqlParser::BYTES - 128))
            | (1ULL << (sqlParser::CHARS - 128)))) != 0)) {
            setState(1040);
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
          setState(1043);
          match(sqlParser::T__7);
        }
        break;
      }

      case sqlParser::TEXT: {
        setState(1047);
        match(sqlParser::TEXT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(1052);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::COLLATE) {
      setState(1050);
      match(sqlParser::COLLATE);
      setState(1051);
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
  enterRule(_localctx, 88, sqlParser::RuleAsSubquery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1054);
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
  enterRule(_localctx, 90, sqlParser::RuleSelectOrValues);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    setState(1077);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 154, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1056);
      match(sqlParser::T__6);
      setState(1060);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 151, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(1057);
          queryHint(); 
        }
        setState(1062);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 151, _ctx);
      }
      setState(1063);
      selectOrValues();
      setState(1064);
      match(sqlParser::T__7);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1069);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::HINT_START) {
        setState(1066);
        queryHint();
        setState(1071);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(1075);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 153, _ctx)) {
      case 1: {
        setState(1072);
        selectQuery();
        break;
      }

      case 2: {
        setState(1073);
        valuesExpr();
        break;
      }

      case 3: {
        setState(1074);
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
  enterRule(_localctx, 92, sqlParser::RuleSelectList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1079);
    selectListItem();
    setState(1084);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 155, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1080);
        match(sqlParser::T__2);
        setState(1081);
        selectListItem(); 
      }
      setState(1086);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 155, _ctx);
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
  enterRule(_localctx, 94, sqlParser::RuleSelectListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1095);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 158, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1087);
      expr(0);
      setState(1092);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 157, _ctx)) {
      case 1: {
        setState(1089);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(1088);
          match(sqlParser::AS);
        }
        setState(1091);
        dynamic_cast<SelectListItemContext *>(_localctx)->as = identifier();
        break;
      }

      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1094);
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
  enterRule(_localctx, 96, sqlParser::RuleGroupByList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1097);
    groupByListItem();
    setState(1102);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 159, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1098);
        match(sqlParser::T__2);
        setState(1099);
        groupByListItem(); 
      }
      setState(1104);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 159, _ctx);
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
  enterRule(_localctx, 98, sqlParser::RuleGroupByListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1105);
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
  enterRule(_localctx, 100, sqlParser::RuleOrderByList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1107);
    orderByListItem();
    setState(1112);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1108);
      match(sqlParser::T__2);
      setState(1109);
      orderByListItem();
      setState(1114);
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
  enterRule(_localctx, 102, sqlParser::RuleOrderByListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1115);
    expr(0);
    setState(1117);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ASC

    || _la == sqlParser::DESC) {
      setState(1116);
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
  enterRule(_localctx, 104, sqlParser::RuleJoinUsingList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1119);
    joinUsingListItem();
    setState(1124);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1120);
      match(sqlParser::T__2);
      setState(1121);
      joinUsingListItem();
      setState(1126);
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
  enterRule(_localctx, 106, sqlParser::RuleJoinUsingListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1127);
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
  enterRule(_localctx, 108, sqlParser::RuleCorrList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1129);
    corrListItem();
    setState(1134);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1130);
      match(sqlParser::T__2);
      setState(1131);
      corrListItem();
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
  enterRule(_localctx, 110, sqlParser::RuleCorrListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1137);
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
  enterRule(_localctx, 112, sqlParser::RuleColAliases);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1139);
    match(sqlParser::T__6);
    setState(1140);
    colAliasList();
    setState(1141);
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
  enterRule(_localctx, 114, sqlParser::RuleColAliasList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1143);
    colAliasListItem();
    setState(1148);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1144);
      match(sqlParser::T__2);
      setState(1145);
      colAliasListItem();
      setState(1150);
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
  enterRule(_localctx, 116, sqlParser::RuleColAliasListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1151);
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
  enterRule(_localctx, 118, sqlParser::RulePropertyList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1153);
    property();
    setState(1158);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 165, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1154);
        match(sqlParser::T__2);
        setState(1155);
        property(); 
      }
      setState(1160);
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
  enterRule(_localctx, 120, sqlParser::RuleProperty);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1161);
    dynamic_cast<PropertyContext *>(_localctx)->name = propertyName();
    setState(1164);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__5) {
      setState(1162);
      match(sqlParser::T__5);
      setState(1163);
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
  enterRule(_localctx, 122, sqlParser::RuleValueList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1166);
    expr(0);
    setState(1171);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1167);
      match(sqlParser::T__2);
      setState(1168);
      expr(0);
      setState(1173);
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
  enterRule(_localctx, 124, sqlParser::RulePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1174);
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
  enterRule(_localctx, 126, sqlParser::RuleJoinPredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1176);
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
  enterRule(_localctx, 128, sqlParser::RuleCasePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1178);
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
  size_t startState = 130;
  enterRecursionRule(_localctx, 130, sqlParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1443);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 195, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<LiteralValContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(1181);
      literal();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<VarRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1182);
      variableName();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ColRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1183);
      columnRef();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<InlineCastContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1184);
      match(sqlParser::T__6);
      setState(1185);
      dataType();
      setState(1186);
      match(sqlParser::T__7);
      setState(1187);
      expr(40);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<UnaryOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1189);
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
      setState(1190);
      expr(39);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<CastOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1191);
      match(sqlParser::CAST);
      setState(1192);
      match(sqlParser::T__6);
      setState(1193);
      expr(0);
      setState(1194);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2

      || _la == sqlParser::AS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1195);
      dataType();
      setState(1196);
      match(sqlParser::T__7);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<TypeOfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1198);
      match(sqlParser::TYPEOF);
      setState(1199);
      match(sqlParser::T__6);
      setState(1200);
      expr(0);
      setState(1201);
      match(sqlParser::T__7);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<CaseOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1203);
      match(sqlParser::CASE);
      setState(1223);
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
          setState(1204);
          dynamic_cast<CaseOpContext *>(_localctx)->caseExpr = expr(0);
          setState(1210); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1205);
            match(sqlParser::WHEN);
            setState(1206);
            expr(0);
            setState(1207);
            match(sqlParser::THEN);
            setState(1208);
            expr(0);
            setState(1212); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

        case sqlParser::WHEN: {
          setState(1219); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1214);
            match(sqlParser::WHEN);
            setState(1215);
            casePredicate();
            setState(1216);
            match(sqlParser::THEN);
            setState(1217);
            expr(0);
            setState(1221); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(1227);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ELSE) {
        setState(1225);
        match(sqlParser::ELSE);
        setState(1226);
        dynamic_cast<CaseOpContext *>(_localctx)->elseExpr = expr(0);
      }
      setState(1229);
      match(sqlParser::END);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<IfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1231);
      match(sqlParser::IF);
      setState(1232);
      match(sqlParser::T__6);
      setState(1233);
      casePredicate();
      setState(1234);
      match(sqlParser::T__2);
      setState(1235);
      expr(0);
      setState(1236);
      match(sqlParser::T__2);
      setState(1237);
      expr(0);
      setState(1238);
      match(sqlParser::T__7);
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<IfNullOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1240);
      match(sqlParser::IFNULL);
      setState(1241);
      match(sqlParser::T__6);
      setState(1242);
      expr(0);
      setState(1243);
      match(sqlParser::T__2);
      setState(1244);
      expr(0);
      setState(1245);
      match(sqlParser::T__7);
      break;
    }

    case 11: {
      _localctx = _tracker.createInstance<NullIfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1247);
      match(sqlParser::NULLIF);
      setState(1248);
      match(sqlParser::T__6);
      setState(1249);
      expr(0);
      setState(1250);
      match(sqlParser::T__2);
      setState(1251);
      expr(0);
      setState(1252);
      match(sqlParser::T__7);
      break;
    }

    case 12: {
      _localctx = _tracker.createInstance<CoalesceOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1254);
      match(sqlParser::COALESCE);
      setState(1255);
      match(sqlParser::T__6);
      setState(1264);
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
        setState(1256);
        expr(0);
        setState(1261);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1257);
          match(sqlParser::T__2);
          setState(1258);
          expr(0);
          setState(1263);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1266);
      match(sqlParser::T__7);
      break;
    }

    case 13: {
      _localctx = _tracker.createInstance<DecodeOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1267);
      match(sqlParser::DECODE);
      setState(1268);
      match(sqlParser::T__6);
      setState(1277);
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
        setState(1269);
        expr(0);
        setState(1274);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1270);
          match(sqlParser::T__2);
          setState(1271);
          expr(0);
          setState(1276);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1279);
      match(sqlParser::T__7);
      break;
    }

    case 14: {
      _localctx = _tracker.createInstance<ConcatOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1280);
      match(sqlParser::CONCAT);
      setState(1281);
      match(sqlParser::T__6);
      setState(1290);
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
        setState(1282);
        expr(0);
        setState(1287);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1283);
          match(sqlParser::T__2);
          setState(1284);
          expr(0);
          setState(1289);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1292);
      match(sqlParser::T__7);
      break;
    }

    case 15: {
      _localctx = _tracker.createInstance<OverlayFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1293);
      match(sqlParser::OVERLAY);
      setState(1294);
      match(sqlParser::T__6);
      setState(1295);
      expr(0);
      setState(1296);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2 || _la == sqlParser::PLACING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1297);
      expr(0);
      setState(1298);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2

      || _la == sqlParser::FROM)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1299);
      expr(0);
      setState(1302);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__2

      || _la == sqlParser::FOR) {
        setState(1300);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__2

        || _la == sqlParser::FOR)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(1301);
        expr(0);
      }
      setState(1304);
      match(sqlParser::T__7);
      break;
    }

    case 16: {
      _localctx = _tracker.createInstance<PositionFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1306);
      match(sqlParser::POSITION);
      setState(1307);
      match(sqlParser::T__6);
      setState(1308);
      expr(0);
      setState(1309);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2

      || _la == sqlParser::IN)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1310);
      expr(0);
      setState(1311);
      match(sqlParser::T__7);
      break;
    }

    case 17: {
      _localctx = _tracker.createInstance<SubStringFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1313);
      _la = _input->LA(1);
      if (!(_la == sqlParser::SUBSTR

      || _la == sqlParser::SUBSTRING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1314);
      match(sqlParser::T__6);
      setState(1329);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 181, _ctx)) {
      case 1: {
        setState(1315);
        dynamic_cast<SubStringFuncContext *>(_localctx)->src = expr(0);
        setState(1316);
        match(sqlParser::T__2);
        setState(1317);
        dynamic_cast<SubStringFuncContext *>(_localctx)->start = expr(0);
        setState(1320);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__2) {
          setState(1318);
          match(sqlParser::T__2);
          setState(1319);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      case 2: {
        setState(1322);
        dynamic_cast<SubStringFuncContext *>(_localctx)->src = expr(0);
        setState(1323);
        match(sqlParser::FROM);
        setState(1324);
        dynamic_cast<SubStringFuncContext *>(_localctx)->start = expr(0);
        setState(1327);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::FOR) {
          setState(1325);
          match(sqlParser::FOR);
          setState(1326);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      }
      setState(1331);
      match(sqlParser::T__7);
      break;
    }

    case 18: {
      _localctx = _tracker.createInstance<TrimFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1333);
      match(sqlParser::TRIM);
      setState(1334);
      match(sqlParser::T__6);
      setState(1348);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 185, _ctx)) {
      case 1: {
        setState(1335);
        expr(0);
        setState(1338);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__2) {
          setState(1336);
          match(sqlParser::T__2);
          setState(1337);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        break;
      }

      case 2: {
        setState(1341);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 183, _ctx)) {
        case 1: {
          setState(1340);
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
        setState(1344);
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
          setState(1343);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        setState(1346);
        match(sqlParser::FROM);
        setState(1347);
        expr(0);
        break;
      }

      }
      setState(1350);
      match(sqlParser::T__7);
      break;
    }

    case 19: {
      _localctx = _tracker.createInstance<ExtractFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1352);
      match(sqlParser::EXTRACT);
      setState(1353);
      match(sqlParser::T__6);
      setState(1354);
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
      setState(1355);
      match(sqlParser::FROM);
      setState(1356);
      expr(0);
      setState(1357);
      match(sqlParser::T__7);
      break;
    }

    case 20: {
      _localctx = _tracker.createInstance<CountAllContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1359);
      match(sqlParser::COUNT);
      setState(1360);
      match(sqlParser::T__6);
      setState(1362);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ALL) {
        setState(1361);
        match(sqlParser::ALL);
      }
      setState(1364);
      match(sqlParser::T__4);
      setState(1365);
      match(sqlParser::T__7);
      break;
    }

    case 21: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1366);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::COUNT);
      setState(1367);
      match(sqlParser::T__6);
      setState(1370);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1368);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1369);
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
      setState(1372);
      expr(0);
      setState(1373);
      match(sqlParser::T__7);
      break;
    }

    case 22: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1375);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::SUM);
      setState(1376);
      match(sqlParser::T__6);
      setState(1379);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1377);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1378);
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
      setState(1381);
      expr(0);
      setState(1382);
      match(sqlParser::T__7);
      break;
    }

    case 23: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1384);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::AVG);
      setState(1385);
      match(sqlParser::T__6);
      setState(1388);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1386);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1387);
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
      setState(1390);
      expr(0);
      setState(1391);
      match(sqlParser::T__7);
      break;
    }

    case 24: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1393);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MIN);
      setState(1394);
      match(sqlParser::T__6);
      setState(1397);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1395);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1396);
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
      setState(1399);
      expr(0);
      setState(1400);
      match(sqlParser::T__7);
      break;
    }

    case 25: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1402);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MAX);
      setState(1403);
      match(sqlParser::T__6);
      setState(1406);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1404);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1405);
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
      setState(1408);
      expr(0);
      setState(1409);
      match(sqlParser::T__7);
      break;
    }

    case 26: {
      _localctx = _tracker.createInstance<ScalarFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1411);
      dynamic_cast<ScalarFuncContext *>(_localctx)->func = functionName();
      setState(1412);
      match(sqlParser::T__6);
      setState(1421);
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
        setState(1413);
        expr(0);
        setState(1418);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1414);
          match(sqlParser::T__2);
          setState(1415);
          expr(0);
          setState(1420);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1423);
      match(sqlParser::T__7);
      break;
    }

    case 27: {
      _localctx = _tracker.createInstance<ParentheticContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1425);
      match(sqlParser::T__6);
      setState(1426);
      expr(0);
      setState(1427);
      match(sqlParser::T__7);
      break;
    }

    case 28: {
      _localctx = _tracker.createInstance<SubqueryExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1429);
      match(sqlParser::T__6);
      setState(1430);
      query();
      setState(1431);
      match(sqlParser::T__7);
      break;
    }

    case 29: {
      _localctx = _tracker.createInstance<SubqueryExistsOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1434);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::NOT) {
        setState(1433);
        match(sqlParser::NOT);
      }
      setState(1436);
      match(sqlParser::EXISTS);

      setState(1437);
      match(sqlParser::T__6);
      setState(1438);
      query();
      setState(1439);
      match(sqlParser::T__7);
      break;
    }

    case 30: {
      _localctx = _tracker.createInstance<LogicOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1441);
      dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::NOT);
      setState(1442);
      expr(4);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(1538);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 207, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(1536);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 206, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1445);

          if (!(precpred(_ctx, 37))) throw FailedPredicateException(this, "precpred(_ctx, 37)");
          setState(1446);
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
          setState(1447);
          expr(38);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1448);

          if (!(precpred(_ctx, 36))) throw FailedPredicateException(this, "precpred(_ctx, 36)");
          setState(1449);
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
          setState(1450);
          expr(37);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1451);

          if (!(precpred(_ctx, 35))) throw FailedPredicateException(this, "precpred(_ctx, 35)");
          setState(1452);
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
          setState(1453);
          expr(36);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<LikeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1454);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(1456);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1455);
            match(sqlParser::NOT);
          }
          setState(1458);
          match(sqlParser::LIKE);
          setState(1459);
          expr(11);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<LikeEscapeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1460);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(1462);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1461);
            match(sqlParser::NOT);
          }
          setState(1464);
          match(sqlParser::LIKE);
          setState(1465);
          expr(0);
          setState(1466);
          match(sqlParser::ESCAPE);
          setState(1467);
          expr(10);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1469);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(1470);

          if (!(!(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent)))) throw FailedPredicateException(this, "!(dynamic_cast<sqlParser::BetweenOpContext*>(_ctx->parent))");
          setState(1471);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::AND);
          setState(1472);
          expr(4);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1473);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(1474);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::OR);
          setState(1475);
          expr(3);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1476);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(1477);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::XOR);
          setState(1478);
          expr(2);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<MethodCallContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1479);

          if (!(precpred(_ctx, 38))) throw FailedPredicateException(this, "precpred(_ctx, 38)");
          setState(1480);
          match(sqlParser::T__9);
          setState(1481);
          dynamic_cast<MethodCallContext *>(_localctx)->func = methodName();
          setState(1482);
          match(sqlParser::T__6);
          setState(1491);
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
            setState(1483);
            expr(0);
            setState(1488);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__2) {
              setState(1484);
              match(sqlParser::T__2);
              setState(1485);
              expr(0);
              setState(1490);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1493);
          match(sqlParser::T__7);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<IsNullOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1495);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(1496);
          match(sqlParser::IS);
          setState(1498);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1497);
            match(sqlParser::NOT);
          }
          setState(1500);
          match(sqlParser::SQL_NULL);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<SubqueryInOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1501);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(1503);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1502);
            match(sqlParser::NOT);
          }
          setState(1505);
          match(sqlParser::IN);

          setState(1506);
          match(sqlParser::T__6);
          setState(1507);
          query();
          setState(1508);
          match(sqlParser::T__7);
          break;
        }

        case 12: {
          auto newContext = _tracker.createInstance<InOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1510);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(1512);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1511);
            match(sqlParser::NOT);
          }
          setState(1514);
          match(sqlParser::IN);

          setState(1515);
          match(sqlParser::T__6);
          setState(1524);
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
            setState(1516);
            expr(0);
            setState(1521);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__2) {
              setState(1517);
              match(sqlParser::T__2);
              setState(1518);
              expr(0);
              setState(1523);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1526);
          match(sqlParser::T__7);
          break;
        }

        case 13: {
          auto newContext = _tracker.createInstance<BetweenOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1527);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(1529);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1528);
            match(sqlParser::NOT);
          }
          setState(1531);
          match(sqlParser::BETWEEN);

          setState(1532);
          expr(0);
          setState(1533);
          match(sqlParser::AND);
          setState(1534);
          expr(0);
          break;
        }

        } 
      }
      setState(1540);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 207, _ctx);
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
  enterRule(_localctx, 132, sqlParser::RuleVariableName);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1541);
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
  enterRule(_localctx, 134, sqlParser::RuleColumnRef);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1556);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 210, _ctx)) {
    case 1: {
      setState(1551);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 209, _ctx)) {
      case 1: {
        setState(1546);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 208, _ctx)) {
        case 1: {
          setState(1543);
          catalogName();
          setState(1544);
          match(sqlParser::T__9);
          break;
        }

        }

        setState(1548);
        schemaName();
        setState(1549);
        match(sqlParser::T__9);
        break;
      }

      }

      setState(1553);
      tableName();
      setState(1554);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(1558);
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
  enterRule(_localctx, 136, sqlParser::RuleColumnWildcard);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1573);
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
      setState(1568);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 212, _ctx)) {
      case 1: {
        setState(1563);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 211, _ctx)) {
        case 1: {
          setState(1560);
          catalogName();
          setState(1561);
          match(sqlParser::T__9);
          break;
        }

        }

        setState(1565);
        schemaName();
        setState(1566);
        match(sqlParser::T__9);
        break;
      }

      }

      setState(1570);
      tableName();
      setState(1571);
      match(sqlParser::T__9);
    }
    setState(1575);
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
  enterRule(_localctx, 138, sqlParser::RuleClassName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1582);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 214, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1577);
        qualifier();
        setState(1578);
        match(sqlParser::T__9); 
      }
      setState(1584);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 214, _ctx);
    }
    setState(1585);
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
  enterRule(_localctx, 140, sqlParser::RuleFunctionName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1587);
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
  enterRule(_localctx, 142, sqlParser::RuleMethodName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1589);
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
  enterRule(_localctx, 144, sqlParser::RuleCatalogName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1591);
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
  enterRule(_localctx, 146, sqlParser::RuleSchemaName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1593);
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
  enterRule(_localctx, 148, sqlParser::RuleTableName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1595);
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
  enterRule(_localctx, 150, sqlParser::RuleTableAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1597);
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
  enterRule(_localctx, 152, sqlParser::RuleColumnName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1599);
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
  enterRule(_localctx, 154, sqlParser::RuleColumnAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1601);
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
  enterRule(_localctx, 156, sqlParser::RuleCollationName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1603);
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
  enterRule(_localctx, 158, sqlParser::RuleIndexName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1605);
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
  enterRule(_localctx, 160, sqlParser::RulePropertyName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1607);
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
  enterRule(_localctx, 162, sqlParser::RuleHintName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1609);
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
  enterRule(_localctx, 164, sqlParser::RulePropertyValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1613);
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
        setState(1611);
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
        setState(1612);
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
  enterRule(_localctx, 166, sqlParser::RuleHintValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1617);
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
        setState(1615);
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
        setState(1616);
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
  enterRule(_localctx, 168, sqlParser::RuleIdentifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1621);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(1619);
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
        setState(1620);
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
  enterRule(_localctx, 170, sqlParser::RuleQualifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1623);
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
  enterRule(_localctx, 172, sqlParser::RulePrecision);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1625);
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
  enterRule(_localctx, 174, sqlParser::RuleScale);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1627);
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
  enterRule(_localctx, 176, sqlParser::RuleSize);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1629);
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
  enterRule(_localctx, 178, sqlParser::RuleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1641);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::SQL_NULL: {
        enterOuterAlt(_localctx, 1);
        setState(1631);
        match(sqlParser::SQL_NULL);
        break;
      }

      case sqlParser::SQL_FALSE:
      case sqlParser::SQL_TRUE: {
        enterOuterAlt(_localctx, 2);
        setState(1632);
        booleanLiteral();
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(1633);
        integerLiteral();
        break;
      }

      case sqlParser::DECIMAL_LITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(1634);
        decimalLiteral();
        break;
      }

      case sqlParser::FLOAT_LITERAL: {
        enterOuterAlt(_localctx, 5);
        setState(1635);
        floatLiteral();
        break;
      }

      case sqlParser::DATE: {
        enterOuterAlt(_localctx, 6);
        setState(1636);
        dateLiteral();
        break;
      }

      case sqlParser::TIME: {
        enterOuterAlt(_localctx, 7);
        setState(1637);
        timeLiteral();
        break;
      }

      case sqlParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 8);
        setState(1638);
        stringLiteral();
        break;
      }

      case sqlParser::TIMESTAMP: {
        enterOuterAlt(_localctx, 9);
        setState(1639);
        timestampLiteral();
        break;
      }

      case sqlParser::INTERVAL: {
        enterOuterAlt(_localctx, 10);
        setState(1640);
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
  enterRule(_localctx, 180, sqlParser::RuleSimpleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1649);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 219, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1643);
      match(sqlParser::SQL_NULL);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1644);
      booleanLiteral();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1645);
      signedIntegerLiteral();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(1646);
      signedDecimalLiteral();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(1647);
      signedFloatLiteral();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(1648);
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
  enterRule(_localctx, 182, sqlParser::RuleBooleanLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1651);
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
  enterRule(_localctx, 184, sqlParser::RuleIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1653);
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
  enterRule(_localctx, 186, sqlParser::RuleSignedIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1657);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__10: {
        setState(1655);
        match(sqlParser::T__10);
        break;
      }

      case sqlParser::T__3: {
        setState(1656);
        dynamic_cast<SignedIntegerLiteralContext *>(_localctx)->neg = match(sqlParser::T__3);
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1659);
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
  enterRule(_localctx, 188, sqlParser::RuleDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1661);
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
  enterRule(_localctx, 190, sqlParser::RuleSignedDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1665);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__10: {
        setState(1663);
        match(sqlParser::T__10);
        break;
      }

      case sqlParser::T__3: {
        setState(1664);
        dynamic_cast<SignedDecimalLiteralContext *>(_localctx)->neg = match(sqlParser::T__3);
        break;
      }

      case sqlParser::DECIMAL_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1667);
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
  enterRule(_localctx, 192, sqlParser::RuleFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1669);
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
  enterRule(_localctx, 194, sqlParser::RuleSignedFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1673);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__10: {
        setState(1671);
        match(sqlParser::T__10);
        break;
      }

      case sqlParser::T__3: {
        setState(1672);
        dynamic_cast<SignedFloatLiteralContext *>(_localctx)->neg = match(sqlParser::T__3);
        break;
      }

      case sqlParser::FLOAT_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(1675);
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
  enterRule(_localctx, 196, sqlParser::RuleDateLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1677);
    match(sqlParser::DATE);
    setState(1678);
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
  enterRule(_localctx, 198, sqlParser::RuleTimeLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1680);
    match(sqlParser::TIME);
    setState(1681);
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
  enterRule(_localctx, 200, sqlParser::RuleTimestampLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1683);
    match(sqlParser::TIMESTAMP);
    setState(1684);
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
  enterRule(_localctx, 202, sqlParser::RuleStringLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1686);
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
  enterRule(_localctx, 204, sqlParser::RuleIntervalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1691);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 223, _ctx)) {
    case 1: {
      setState(1688);
      sql92CalendarInterval();
      break;
    }

    case 2: {
      setState(1689);
      sql92TimeInterval();
      break;
    }

    case 3: {
      setState(1690);
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
  enterRule(_localctx, 206, sqlParser::RuleLooseInterval);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1693);
    match(sqlParser::INTERVAL);
    setState(1694);
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
  enterRule(_localctx, 208, sqlParser::RuleSql92CalendarInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1696);
    match(sqlParser::INTERVAL);
    setState(1697);
    stringLiteral();
    setState(1721);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 227, _ctx)) {
    case 1: {
      setState(1698);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1703);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 224, _ctx)) {
      case 1: {
        setState(1699);
        match(sqlParser::T__6);
        setState(1700);
        precision();
        setState(1701);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1705);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1710);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1706);
        match(sqlParser::T__6);
        setState(1707);
        precision();
        setState(1708);
        match(sqlParser::T__7);
      }
      setState(1712);
      match(sqlParser::TO);
      setState(1713);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      break;
    }

    case 3: {
      setState(1714);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      setState(1719);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 226, _ctx)) {
      case 1: {
        setState(1715);
        match(sqlParser::T__6);
        setState(1716);
        precision();
        setState(1717);
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
  enterRule(_localctx, 210, sqlParser::RuleSql92TimeInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1723);
    match(sqlParser::INTERVAL);
    setState(1724);
    stringLiteral();
    setState(1809);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 241, _ctx)) {
    case 1: {
      setState(1725);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1730);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 228, _ctx)) {
      case 1: {
        setState(1726);
        match(sqlParser::T__6);
        setState(1727);
        precision();
        setState(1728);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1732);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
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
      setState(1749);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::HOUR: {
          setState(1740);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
          break;
        }

        case sqlParser::MINUTE: {
          setState(1741);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1742);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1747);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 230, _ctx)) {
          case 1: {
            setState(1743);
            match(sqlParser::T__6);
            setState(1744);
            scale();
            setState(1745);
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
      setState(1751);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1756);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1752);
        match(sqlParser::T__6);
        setState(1753);
        precision();
        setState(1754);
        match(sqlParser::T__7);
      }
      setState(1758);
      match(sqlParser::TO);
      setState(1767);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::MINUTE: {
          setState(1759);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1760);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1765);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 233, _ctx)) {
          case 1: {
            setState(1761);
            match(sqlParser::T__6);
            setState(1762);
            scale();
            setState(1763);
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
      setState(1769);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1774);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1770);
        match(sqlParser::T__6);
        setState(1771);
        precision();
        setState(1772);
        match(sqlParser::T__7);
      }
      setState(1776);
      match(sqlParser::TO);

      setState(1777);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1782);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 236, _ctx)) {
      case 1: {
        setState(1778);
        match(sqlParser::T__6);
        setState(1779);
        scale();
        setState(1780);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 5: {
      setState(1784);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1789);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 237, _ctx)) {
      case 1: {
        setState(1785);
        match(sqlParser::T__6);
        setState(1786);
        precision();
        setState(1787);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 6: {
      setState(1791);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1796);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 238, _ctx)) {
      case 1: {
        setState(1792);
        match(sqlParser::T__6);
        setState(1793);
        precision();
        setState(1794);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 7: {
      setState(1798);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1807);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 240, _ctx)) {
      case 1: {
        setState(1799);
        match(sqlParser::T__6);
        setState(1800);
        precision();
        setState(1803);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__2) {
          setState(1801);
          match(sqlParser::T__2);
          setState(1802);
          scale();
        }
        setState(1805);
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
  enterRule(_localctx, 212, sqlParser::RuleNonReservedWord);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1811);
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
    case 65: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

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
  "fixedLenChar", "varyingLenChar", "asSubquery", "selectOrValues", "selectList", 
  "selectListItem", "groupByList", "groupByListItem", "orderByList", "orderByListItem", 
  "joinUsingList", "joinUsingListItem", "corrList", "corrListItem", "colAliases", 
  "colAliasList", "colAliasListItem", "propertyList", "property", "valueList", 
  "predicate", "joinPredicate", "casePredicate", "expr", "variableName", 
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
    0x3, 0xcb, 0x718, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
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
    0x4, 0x6a, 0x9, 0x6a, 0x4, 0x6b, 0x9, 0x6b, 0x4, 0x6c, 0x9, 0x6c, 0x3, 
    0x2, 0x3, 0x2, 0x5, 0x2, 0xdb, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 0xe3, 0xa, 0x3, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x5, 0x7, 0x5, 0xe8, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0xeb, 0xb, 
    0x5, 0x3, 0x5, 0x3, 0x5, 0x7, 0x5, 0xef, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 
    0xf2, 0xb, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x5, 0x6, 0xfa, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
    0x7, 0x7, 0x100, 0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 0x103, 0xb, 0x7, 0x3, 
    0x8, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 0x108, 0xa, 0x8, 0x3, 0x9, 0x3, 0x9, 
    0x3, 0x9, 0x5, 0x9, 0x10d, 0xa, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 
    0xb, 0x7, 0xb, 0x113, 0xa, 0xb, 0xc, 0xb, 0xe, 0xb, 0x116, 0xb, 0xb, 
    0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x7, 0xc, 0x11c, 0xa, 0xc, 0xc, 
    0xc, 0xe, 0xc, 0x11f, 0xb, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 
    0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0x129, 0xa, 0xc, 0x3, 
    0xd, 0x3, 0xd, 0x5, 0xd, 0x12d, 0xa, 0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 
    0x131, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 0x134, 0xb, 0xd, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 
    0xe, 0x5, 0xe, 0x13f, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x7, 0xe, 0x145, 0xa, 0xe, 0xc, 0xe, 0xe, 0xe, 0x148, 0xb, 0xe, 0x3, 
    0xe, 0x3, 0xe, 0x5, 0xe, 0x14c, 0xa, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
    0x3, 0xf, 0x7, 0xf, 0x152, 0xa, 0xf, 0xc, 0xf, 0xe, 0xf, 0x155, 0xb, 
    0xf, 0x5, 0xf, 0x157, 0xa, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
    0x5, 0xf, 0x15d, 0xa, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 
    0xf, 0x163, 0xa, 0xf, 0x5, 0xf, 0x165, 0xa, 0xf, 0x3, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x5, 0x10, 0x16a, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x5, 0x10, 0x16f, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x173, 
    0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x178, 0xa, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x17c, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x182, 0xa, 0x10, 0x3, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x3, 0x11, 0x7, 0x11, 0x188, 0xa, 0x11, 0xc, 0x11, 0xe, 0x11, 
    0x18b, 0xb, 0x11, 0x5, 0x11, 0x18d, 0xa, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0x193, 0xa, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0x199, 0xa, 0x11, 0x5, 0x11, 0x19b, 
    0xa, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 
    0x12, 0x5, 0x12, 0x1a3, 0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x5, 0x12, 0x1a9, 0xa, 0x12, 0x3, 0x12, 0x5, 0x12, 0x1ac, 
    0xa, 0x12, 0x3, 0x12, 0x7, 0x12, 0x1af, 0xa, 0x12, 0xc, 0x12, 0xe, 0x12, 
    0x1b2, 0xb, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x5, 0x13, 0x1ba, 0xa, 0x13, 0x3, 0x14, 0x5, 0x14, 0x1bd, 
    0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1c1, 0xa, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 
    0x15, 0x5, 0x15, 0x1cb, 0xa, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1ce, 0xa, 
    0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1d4, 
    0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1d8, 0xa, 0x15, 0x5, 0x15, 
    0x1da, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 
    0x1e0, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1e4, 0xa, 0x15, 
    0x5, 0x15, 0x1e6, 0xa, 0x15, 0x5, 0x15, 0x1e8, 0xa, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x7, 0x15, 0x1ec, 0xa, 0x15, 0xc, 0x15, 0xe, 0x15, 0x1ef, 
    0xb, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1f4, 0xa, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x7, 0x15, 0x1f9, 0xa, 0x15, 0xc, 0x15, 
    0xe, 0x15, 0x1fc, 0xb, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x200, 
    0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x7, 0x15, 0x206, 
    0xa, 0x15, 0xc, 0x15, 0xe, 0x15, 0x209, 0xb, 0x15, 0x3, 0x15, 0x5, 0x15, 
    0x20c, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x7, 0x15, 0x215, 0xa, 0x15, 0xc, 0x15, 0xe, 0x15, 
    0x218, 0xb, 0x15, 0x3, 0x15, 0x5, 0x15, 0x21b, 0xa, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x7, 
    0x15, 0x224, 0xa, 0x15, 0xc, 0x15, 0xe, 0x15, 0x227, 0xb, 0x15, 0x3, 
    0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x22c, 0xa, 0x16, 0x3, 0x16, 
    0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x231, 0xa, 0x16, 0x3, 0x16, 0x3, 0x16, 
    0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0x238, 0xa, 0x17, 0x5, 0x17, 
    0x23a, 0xa, 0x17, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0x23f, 
    0xa, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x244, 0xa, 0x19, 
    0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 0x19, 0x24a, 0xa, 0x19, 
    0xc, 0x19, 0xe, 0x19, 0x24d, 0xb, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 
    0x251, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 
    0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x7, 
    0x1a, 0x25e, 0xa, 0x1a, 0xc, 0x1a, 0xe, 0x1a, 0x261, 0xb, 0x1a, 0x3, 
    0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x7, 0x1b, 0x267, 0xa, 0x1b, 
    0xc, 0x1b, 0xe, 0x1b, 0x26a, 0xb, 0x1b, 0x5, 0x1b, 0x26c, 0xa, 0x1b, 
    0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x270, 0xa, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 
    0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x7, 0x1b, 0x277, 0xa, 0x1b, 0xc, 0x1b, 
    0xe, 0x1b, 0x27a, 0xb, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x27e, 
    0xa, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x283, 0xa, 0x1b, 
    0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x7, 0x1c, 0x289, 0xa, 0x1c, 
    0xc, 0x1c, 0xe, 0x1c, 0x28c, 0xb, 0x1c, 0x5, 0x1c, 0x28e, 0xa, 0x1c, 
    0x3, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x292, 0xa, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 
    0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x299, 0xa, 0x1c, 0x3, 0x1d, 
    0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x7, 0x1d, 0x29f, 0xa, 0x1d, 0xc, 0x1d, 
    0xe, 0x1d, 0x2a2, 0xb, 0x1d, 0x5, 0x1d, 0x2a4, 0xa, 0x1d, 0x3, 0x1d, 
    0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 
    0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x7, 0x1d, 0x2b1, 0xa, 0x1d, 
    0xc, 0x1d, 0xe, 0x1d, 0x2b4, 0xb, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 
    0x3, 0x1d, 0x5, 0x1d, 0x2ba, 0xa, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 
    0x2be, 0xa, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2c2, 0xa, 0x1f, 
    0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2c8, 0xa, 0x1f, 
    0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2cd, 0xa, 0x1f, 0x3, 0x1f, 
    0x5, 0x1f, 0x2d0, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 
    0x2d5, 0xa, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2d8, 0xa, 0x1f, 0x3, 0x1f, 
    0x3, 0x1f, 0x5, 0x1f, 0x2dc, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
    0x5, 0x1f, 0x2e1, 0xa, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2e4, 0xa, 0x1f, 
    0x5, 0x1f, 0x2e6, 0xa, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x5, 0x20, 0x2ea, 
    0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x5, 0x20, 0x2f0, 
    0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x5, 
    0x20, 0x2f7, 0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x7, 0x20, 0x2fd, 0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 0x300, 0xb, 0x20, 
    0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x5, 0x20, 0x305, 0xa, 0x20, 0x3, 0x21, 
    0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x5, 0x21, 0x30b, 0xa, 0x21, 0x3, 0x21, 
    0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x5, 0x21, 0x312, 0xa, 0x21, 
    0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x5, 0x21, 0x318, 0xa, 0x21, 
    0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x31c, 0xa, 0x22, 0x3, 0x22, 0x3, 0x22, 
    0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x5, 0x23, 0x324, 0xa, 0x23, 
    0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x5, 0x23, 0x32a, 0xa, 0x23, 
    0x7, 0x23, 0x32c, 0xa, 0x23, 0xc, 0x23, 0xe, 0x23, 0x32f, 0xb, 0x23, 
    0x3, 0x23, 0x3, 0x23, 0x5, 0x23, 0x333, 0xa, 0x23, 0x3, 0x23, 0x5, 0x23, 
    0x336, 0xa, 0x23, 0x3, 0x23, 0x3, 0x23, 0x5, 0x23, 0x33a, 0xa, 0x23, 
    0x3, 0x24, 0x3, 0x24, 0x5, 0x24, 0x33e, 0xa, 0x24, 0x3, 0x24, 0x5, 0x24, 
    0x341, 0xa, 0x24, 0x3, 0x24, 0x5, 0x24, 0x344, 0xa, 0x24, 0x3, 0x24, 
    0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x7, 0x24, 0x34a, 0xa, 0x24, 0xc, 0x24, 
    0xe, 0x24, 0x34d, 0xb, 0x24, 0x3, 0x24, 0x3, 0x24, 0x3, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x5, 0x25, 0x354, 0xa, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x35b, 0xa, 0x25, 0x3, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x5, 0x25, 0x360, 0xa, 0x25, 0x7, 0x25, 0x362, 0xa, 0x25, 
    0xc, 0x25, 0xe, 0x25, 0x365, 0xb, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 
    0x369, 0xa, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x7, 0x26, 
    0x36f, 0xa, 0x26, 0xc, 0x26, 0xe, 0x26, 0x372, 0xb, 0x26, 0x3, 0x26, 
    0x3, 0x26, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 
    0x27, 0x3, 0x27, 0x7, 0x27, 0x37d, 0xa, 0x27, 0xc, 0x27, 0xe, 0x27, 
    0x380, 0xb, 0x27, 0x3, 0x27, 0x3, 0x27, 0x5, 0x27, 0x384, 0xa, 0x27, 
    0x3, 0x27, 0x5, 0x27, 0x387, 0xa, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 
    0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x7, 0x27, 0x390, 0xa, 0x27, 
    0xc, 0x27, 0xe, 0x27, 0x393, 0xb, 0x27, 0x3, 0x27, 0x3, 0x27, 0x5, 0x27, 
    0x397, 0xa, 0x27, 0x5, 0x27, 0x399, 0xa, 0x27, 0x3, 0x28, 0x5, 0x28, 
    0x39c, 0xa, 0x28, 0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 0x3a0, 0xa, 0x28, 
    0x3, 0x28, 0x5, 0x28, 0x3a3, 0xa, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 
    0x3, 0x28, 0x3, 0x28, 0x7, 0x28, 0x3aa, 0xa, 0x28, 0xc, 0x28, 0xe, 0x28, 
    0x3ad, 0xb, 0x28, 0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 0x3b1, 0xa, 0x28, 
    0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 0x3b5, 0xa, 0x28, 0x3, 0x28, 0x3, 0x28, 
    0x5, 0x28, 0x3b9, 0xa, 0x28, 0x3, 0x29, 0x3, 0x29, 0x3, 0x2a, 0x3, 0x2a, 
    0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3c4, 
    0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3c8, 0xa, 0x2b, 0x3, 0x2b, 
    0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3ce, 0xa, 0x2b, 0x3, 0x2b, 
    0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3d3, 0xa, 0x2b, 0x5, 0x2b, 0x3d5, 
    0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3d9, 0xa, 0x2b, 0x3, 0x2b, 
    0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3df, 0xa, 0x2b, 0x3, 0x2b, 
    0x3, 0x2b, 0x5, 0x2b, 0x3e3, 0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 
    0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x5, 
    0x2b, 0x3ee, 0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 
    0x3, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3f6, 0xa, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 
    0x5, 0x2b, 0x3fa, 0xa, 0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x3fd, 0xa, 0x2b, 
    0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x5, 0x2c, 0x403, 0xa, 0x2c, 
    0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x5, 0x2c, 0x408, 0xa, 0x2c, 0x3, 0x2d, 
    0x3, 0x2d, 0x5, 0x2d, 0x40c, 0xa, 0x2d, 0x3, 0x2d, 0x5, 0x2d, 0x40f, 
    0xa, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x5, 0x2d, 0x414, 0xa, 0x2d, 
    0x3, 0x2d, 0x3, 0x2d, 0x5, 0x2d, 0x418, 0xa, 0x2d, 0x3, 0x2d, 0x5, 0x2d, 
    0x41b, 0xa, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x5, 0x2d, 0x41f, 0xa, 0x2d, 
    0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 0x7, 0x2f, 0x425, 0xa, 0x2f, 
    0xc, 0x2f, 0xe, 0x2f, 0x428, 0xb, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 
    0x3, 0x2f, 0x7, 0x2f, 0x42e, 0xa, 0x2f, 0xc, 0x2f, 0xe, 0x2f, 0x431, 
    0xb, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x5, 0x2f, 0x436, 0xa, 0x2f, 
    0x5, 0x2f, 0x438, 0xa, 0x2f, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x7, 0x30, 
    0x43d, 0xa, 0x30, 0xc, 0x30, 0xe, 0x30, 0x440, 0xb, 0x30, 0x3, 0x31, 
    0x3, 0x31, 0x5, 0x31, 0x444, 0xa, 0x31, 0x3, 0x31, 0x5, 0x31, 0x447, 
    0xa, 0x31, 0x3, 0x31, 0x5, 0x31, 0x44a, 0xa, 0x31, 0x3, 0x32, 0x3, 0x32, 
    0x3, 0x32, 0x7, 0x32, 0x44f, 0xa, 0x32, 0xc, 0x32, 0xe, 0x32, 0x452, 
    0xb, 0x32, 0x3, 0x33, 0x3, 0x33, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x7, 
    0x34, 0x459, 0xa, 0x34, 0xc, 0x34, 0xe, 0x34, 0x45c, 0xb, 0x34, 0x3, 
    0x35, 0x3, 0x35, 0x5, 0x35, 0x460, 0xa, 0x35, 0x3, 0x36, 0x3, 0x36, 
    0x3, 0x36, 0x7, 0x36, 0x465, 0xa, 0x36, 0xc, 0x36, 0xe, 0x36, 0x468, 
    0xb, 0x36, 0x3, 0x37, 0x3, 0x37, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x7, 
    0x38, 0x46f, 0xa, 0x38, 0xc, 0x38, 0xe, 0x38, 0x472, 0xb, 0x38, 0x3, 
    0x39, 0x3, 0x39, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3b, 
    0x3, 0x3b, 0x3, 0x3b, 0x7, 0x3b, 0x47d, 0xa, 0x3b, 0xc, 0x3b, 0xe, 0x3b, 
    0x480, 0xb, 0x3b, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x7, 0x3d, 0x487, 0xa, 0x3d, 0xc, 0x3d, 0xe, 0x3d, 0x48a, 0xb, 0x3d, 
    0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x48f, 0xa, 0x3e, 0x3, 0x3f, 
    0x3, 0x3f, 0x3, 0x3f, 0x7, 0x3f, 0x494, 0xa, 0x3f, 0xc, 0x3f, 0xe, 0x3f, 
    0x497, 0xb, 0x3f, 0x3, 0x40, 0x3, 0x40, 0x3, 0x41, 0x3, 0x41, 0x3, 0x42, 
    0x3, 0x42, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x6, 0x43, 0x4bd, 
    0xa, 0x43, 0xd, 0x43, 0xe, 0x43, 0x4be, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x6, 0x43, 0x4c6, 0xa, 0x43, 0xd, 0x43, 0xe, 0x43, 
    0x4c7, 0x5, 0x43, 0x4ca, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 
    0x4ce, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x7, 0x43, 
    0x4ee, 0xa, 0x43, 0xc, 0x43, 0xe, 0x43, 0x4f1, 0xb, 0x43, 0x5, 0x43, 
    0x4f3, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x7, 0x43, 0x4fb, 0xa, 0x43, 0xc, 0x43, 0xe, 0x43, 0x4fe, 
    0xb, 0x43, 0x5, 0x43, 0x500, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x7, 0x43, 0x508, 0xa, 0x43, 0xc, 0x43, 
    0xe, 0x43, 0x50b, 0xb, 0x43, 0x5, 0x43, 0x50d, 0xa, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x519, 0xa, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x52b, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x532, 0xa, 0x43, 0x5, 0x43, 
    0x534, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x53d, 0xa, 0x43, 0x3, 0x43, 0x5, 0x43, 
    0x540, 0xa, 0x43, 0x3, 0x43, 0x5, 0x43, 0x543, 0xa, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x5, 0x43, 0x547, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x555, 0xa, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x55d, 
    0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x5, 0x43, 0x566, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x56f, 
    0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x5, 0x43, 0x578, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x581, 
    0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x3, 0x43, 0x7, 0x43, 0x58b, 0xa, 0x43, 0xc, 0x43, 
    0xe, 0x43, 0x58e, 0xb, 0x43, 0x5, 0x43, 0x590, 0xa, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x59d, 0xa, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x5, 0x43, 0x5a6, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x5, 0x43, 0x5b3, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x5b9, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 
    0x43, 0x7, 0x43, 0x5d1, 0xa, 0x43, 0xc, 0x43, 0xe, 0x43, 0x5d4, 0xb, 
    0x43, 0x5, 0x43, 0x5d6, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x5dd, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x5, 0x43, 0x5e2, 0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x5eb, 0xa, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x7, 0x43, 0x5f2, 
    0xa, 0x43, 0xc, 0x43, 0xe, 0x43, 0x5f5, 0xb, 0x43, 0x5, 0x43, 0x5f7, 
    0xa, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x5, 0x43, 0x5fc, 0xa, 0x43, 
    0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x7, 0x43, 0x603, 
    0xa, 0x43, 0xc, 0x43, 0xe, 0x43, 0x606, 0xb, 0x43, 0x3, 0x44, 0x3, 0x44, 
    0x3, 0x45, 0x3, 0x45, 0x3, 0x45, 0x5, 0x45, 0x60d, 0xa, 0x45, 0x3, 0x45, 
    0x3, 0x45, 0x3, 0x45, 0x5, 0x45, 0x612, 0xa, 0x45, 0x3, 0x45, 0x3, 0x45, 
    0x3, 0x45, 0x5, 0x45, 0x617, 0xa, 0x45, 0x3, 0x45, 0x3, 0x45, 0x3, 0x46, 
    0x3, 0x46, 0x3, 0x46, 0x5, 0x46, 0x61e, 0xa, 0x46, 0x3, 0x46, 0x3, 0x46, 
    0x3, 0x46, 0x5, 0x46, 0x623, 0xa, 0x46, 0x3, 0x46, 0x3, 0x46, 0x3, 0x46, 
    0x5, 0x46, 0x628, 0xa, 0x46, 0x3, 0x46, 0x3, 0x46, 0x3, 0x47, 0x3, 0x47, 
    0x3, 0x47, 0x7, 0x47, 0x62f, 0xa, 0x47, 0xc, 0x47, 0xe, 0x47, 0x632, 
    0xb, 0x47, 0x3, 0x47, 0x3, 0x47, 0x3, 0x48, 0x3, 0x48, 0x3, 0x49, 0x3, 
    0x49, 0x3, 0x4a, 0x3, 0x4a, 0x3, 0x4b, 0x3, 0x4b, 0x3, 0x4c, 0x3, 0x4c, 
    0x3, 0x4d, 0x3, 0x4d, 0x3, 0x4e, 0x3, 0x4e, 0x3, 0x4f, 0x3, 0x4f, 0x3, 
    0x50, 0x3, 0x50, 0x3, 0x51, 0x3, 0x51, 0x3, 0x52, 0x3, 0x52, 0x3, 0x53, 
    0x3, 0x53, 0x3, 0x54, 0x3, 0x54, 0x5, 0x54, 0x650, 0xa, 0x54, 0x3, 0x55, 
    0x3, 0x55, 0x5, 0x55, 0x654, 0xa, 0x55, 0x3, 0x56, 0x3, 0x56, 0x5, 0x56, 
    0x658, 0xa, 0x56, 0x3, 0x57, 0x3, 0x57, 0x3, 0x58, 0x3, 0x58, 0x3, 0x59, 
    0x3, 0x59, 0x3, 0x5a, 0x3, 0x5a, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 
    0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x5, 0x5b, 0x66c, 0xa, 0x5b, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 
    0x3, 0x5c, 0x3, 0x5c, 0x5, 0x5c, 0x674, 0xa, 0x5c, 0x3, 0x5d, 0x3, 0x5d, 
    0x3, 0x5e, 0x3, 0x5e, 0x3, 0x5f, 0x3, 0x5f, 0x5, 0x5f, 0x67c, 0xa, 0x5f, 
    0x3, 0x5f, 0x3, 0x5f, 0x3, 0x60, 0x3, 0x60, 0x3, 0x61, 0x3, 0x61, 0x5, 
    0x61, 0x684, 0xa, 0x61, 0x3, 0x61, 0x3, 0x61, 0x3, 0x62, 0x3, 0x62, 
    0x3, 0x63, 0x3, 0x63, 0x5, 0x63, 0x68c, 0xa, 0x63, 0x3, 0x63, 0x3, 0x63, 
    0x3, 0x64, 0x3, 0x64, 0x3, 0x64, 0x3, 0x65, 0x3, 0x65, 0x3, 0x65, 0x3, 
    0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x67, 0x3, 0x67, 0x3, 0x68, 0x3, 0x68, 
    0x3, 0x68, 0x5, 0x68, 0x69e, 0xa, 0x68, 0x3, 0x69, 0x3, 0x69, 0x3, 0x69, 
    0x3, 0x6a, 0x3, 0x6a, 0x3, 0x6a, 0x3, 0x6a, 0x3, 0x6a, 0x3, 0x6a, 0x3, 
    0x6a, 0x5, 0x6a, 0x6aa, 0xa, 0x6a, 0x3, 0x6a, 0x3, 0x6a, 0x3, 0x6a, 
    0x3, 0x6a, 0x3, 0x6a, 0x5, 0x6a, 0x6b1, 0xa, 0x6a, 0x3, 0x6a, 0x3, 0x6a, 
    0x3, 0x6a, 0x3, 0x6a, 0x3, 0x6a, 0x3, 0x6a, 0x3, 0x6a, 0x5, 0x6a, 0x6ba, 
    0xa, 0x6a, 0x5, 0x6a, 0x6bc, 0xa, 0x6a, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 
    0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x6c5, 0xa, 0x6b, 
    0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x6cc, 
    0xa, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 
    0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x6d6, 0xa, 0x6b, 0x5, 0x6b, 
    0x6d8, 0xa, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 
    0x5, 0x6b, 0x6df, 0xa, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 
    0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x6e8, 0xa, 0x6b, 0x5, 0x6b, 
    0x6ea, 0xa, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 
    0x5, 0x6b, 0x6f1, 0xa, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 
    0x3, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x6f9, 0xa, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 
    0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x700, 0xa, 0x6b, 0x3, 0x6b, 
    0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x707, 0xa, 0x6b, 
    0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x70e, 
    0xa, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x712, 0xa, 0x6b, 0x5, 0x6b, 
    0x714, 0xa, 0x6b, 0x3, 0x6c, 0x3, 0x6c, 0x3, 0x6c, 0x2, 0x5, 0x22, 0x28, 
    0x84, 0x6d, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 
    0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 
    0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 
    0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 
    0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 
    0x78, 0x7a, 0x7c, 0x7e, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 
    0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, 0xa0, 0xa2, 0xa4, 0xa6, 
    0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe, 
    0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd6, 
    0x2, 0x21, 0x4, 0x2, 0x6, 0x6, 0x6b, 0x6b, 0x4, 0x2, 0x7, 0x7, 0x17, 
    0x17, 0x4, 0x2, 0x5c, 0x5c, 0x61, 0x61, 0x3, 0x2, 0x67, 0x68, 0x4, 0x2, 
    0xb, 0xb, 0x63, 0x63, 0x5, 0x2, 0x33, 0x33, 0x41, 0x41, 0x71, 0x71, 
    0x5, 0x2, 0x39, 0x39, 0x47, 0x47, 0x60, 0x60, 0x4, 0x2, 0xb4, 0xb5, 
    0xb7, 0xb8, 0x4, 0x2, 0x3e, 0x3e, 0x45, 0x45, 0x4, 0x2, 0xb7, 0xb7, 
    0xbb, 0xbb, 0x4, 0x2, 0xb6, 0xb7, 0xb9, 0xba, 0x5, 0x2, 0x8b, 0x8b, 
    0x8d, 0x8d, 0x8f, 0x8f, 0x3, 0x2, 0x8c, 0x8d, 0x3, 0x2, 0x82, 0x83, 
    0x4, 0x2, 0x82, 0x82, 0x8c, 0x8e, 0x4, 0x2, 0x1d, 0x1d, 0x2e, 0x2e, 
    0x4, 0x2, 0x6, 0x6, 0xd, 0xd, 0x4, 0x2, 0x5, 0x5, 0x1c, 0x1c, 0x4, 0x2, 
    0x5, 0x5, 0x5b, 0x5b, 0x4, 0x2, 0x5, 0x5, 0x3a, 0x3a, 0x4, 0x2, 0x5, 
    0x5, 0x37, 0x37, 0x4, 0x2, 0x5, 0x5, 0x3d, 0x3d, 0x3, 0x2, 0x9c, 0x9d, 
    0x5, 0x2, 0x20, 0x20, 0x46, 0x46, 0x6d, 0x6d, 0x8, 0x2, 0xa4, 0xa4, 
    0xa6, 0xa6, 0xa8, 0xa8, 0xaa, 0xaa, 0xae, 0xae, 0xb2, 0xb2, 0x4, 0x2, 
    0x7, 0x7, 0xe, 0xf, 0x5, 0x2, 0x6, 0x6, 0xd, 0xd, 0x10, 0x10, 0x4, 0x2, 
    0x8, 0x8, 0x11, 0x16, 0x3, 0x2, 0x27, 0x2a, 0x4, 0x2, 0x35, 0x35, 0x6f, 
    0x6f, 0x21, 0x2, 0x1a, 0x1a, 0x1d, 0x1d, 0x20, 0x22, 0x24, 0x25, 0x27, 
    0x2f, 0x31, 0x34, 0x36, 0x36, 0x38, 0x38, 0x3e, 0x3e, 0x40, 0x40, 0x42, 
    0x45, 0x47, 0x47, 0x4c, 0x4c, 0x4e, 0x4f, 0x53, 0x53, 0x57, 0x57, 0x59, 
    0x5a, 0x5c, 0x5c, 0x5f, 0x60, 0x62, 0x62, 0x65, 0x68, 0x6a, 0x6a, 0x70, 
    0x75, 0x77, 0x77, 0x7b, 0x80, 0x82, 0x83, 0x86, 0x87, 0x89, 0x8f, 0x92, 
    0x96, 0x99, 0x9d, 0xa4, 0xbb, 0x2, 0x805, 0x2, 0xda, 0x3, 0x2, 0x2, 
    0x2, 0x4, 0xe2, 0x3, 0x2, 0x2, 0x2, 0x6, 0xe4, 0x3, 0x2, 0x2, 0x2, 0x8, 
    0xe9, 0x3, 0x2, 0x2, 0x2, 0xa, 0xf9, 0x3, 0x2, 0x2, 0x2, 0xc, 0xfb, 
    0x3, 0x2, 0x2, 0x2, 0xe, 0x104, 0x3, 0x2, 0x2, 0x2, 0x10, 0x10c, 0x3, 
    0x2, 0x2, 0x2, 0x12, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x14, 0x114, 0x3, 0x2, 
    0x2, 0x2, 0x16, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x18, 0x12a, 0x3, 0x2, 0x2, 
    0x2, 0x1a, 0x14b, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x156, 0x3, 0x2, 0x2, 0x2, 
    0x1e, 0x181, 0x3, 0x2, 0x2, 0x2, 0x20, 0x18c, 0x3, 0x2, 0x2, 0x2, 0x22, 
    0x1a2, 0x3, 0x2, 0x2, 0x2, 0x24, 0x1b3, 0x3, 0x2, 0x2, 0x2, 0x26, 0x1bc, 
    0x3, 0x2, 0x2, 0x2, 0x28, 0x1e7, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x230, 0x3, 
    0x2, 0x2, 0x2, 0x2c, 0x239, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x23e, 0x3, 0x2, 
    0x2, 0x2, 0x30, 0x240, 0x3, 0x2, 0x2, 0x2, 0x32, 0x254, 0x3, 0x2, 0x2, 
    0x2, 0x34, 0x26b, 0x3, 0x2, 0x2, 0x2, 0x36, 0x28d, 0x3, 0x2, 0x2, 0x2, 
    0x38, 0x2a3, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x2bd, 0x3, 0x2, 0x2, 0x2, 0x3c, 
    0x2bf, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x2e7, 0x3, 0x2, 0x2, 0x2, 0x40, 0x317, 
    0x3, 0x2, 0x2, 0x2, 0x42, 0x319, 0x3, 0x2, 0x2, 0x2, 0x44, 0x31f, 0x3, 
    0x2, 0x2, 0x2, 0x46, 0x33b, 0x3, 0x2, 0x2, 0x2, 0x48, 0x350, 0x3, 0x2, 
    0x2, 0x2, 0x4a, 0x36a, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x398, 0x3, 0x2, 0x2, 
    0x2, 0x4e, 0x39b, 0x3, 0x2, 0x2, 0x2, 0x50, 0x3ba, 0x3, 0x2, 0x2, 0x2, 
    0x52, 0x3bc, 0x3, 0x2, 0x2, 0x2, 0x54, 0x3fc, 0x3, 0x2, 0x2, 0x2, 0x56, 
    0x3fe, 0x3, 0x2, 0x2, 0x2, 0x58, 0x41a, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x420, 
    0x3, 0x2, 0x2, 0x2, 0x5c, 0x437, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x439, 0x3, 
    0x2, 0x2, 0x2, 0x60, 0x449, 0x3, 0x2, 0x2, 0x2, 0x62, 0x44b, 0x3, 0x2, 
    0x2, 0x2, 0x64, 0x453, 0x3, 0x2, 0x2, 0x2, 0x66, 0x455, 0x3, 0x2, 0x2, 
    0x2, 0x68, 0x45d, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x461, 0x3, 0x2, 0x2, 0x2, 
    0x6c, 0x469, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x46b, 0x3, 0x2, 0x2, 0x2, 0x70, 
    0x473, 0x3, 0x2, 0x2, 0x2, 0x72, 0x475, 0x3, 0x2, 0x2, 0x2, 0x74, 0x479, 
    0x3, 0x2, 0x2, 0x2, 0x76, 0x481, 0x3, 0x2, 0x2, 0x2, 0x78, 0x483, 0x3, 
    0x2, 0x2, 0x2, 0x7a, 0x48b, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x490, 0x3, 0x2, 
    0x2, 0x2, 0x7e, 0x498, 0x3, 0x2, 0x2, 0x2, 0x80, 0x49a, 0x3, 0x2, 0x2, 
    0x2, 0x82, 0x49c, 0x3, 0x2, 0x2, 0x2, 0x84, 0x5a5, 0x3, 0x2, 0x2, 0x2, 
    0x86, 0x607, 0x3, 0x2, 0x2, 0x2, 0x88, 0x616, 0x3, 0x2, 0x2, 0x2, 0x8a, 
    0x627, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x630, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x635, 
    0x3, 0x2, 0x2, 0x2, 0x90, 0x637, 0x3, 0x2, 0x2, 0x2, 0x92, 0x639, 0x3, 
    0x2, 0x2, 0x2, 0x94, 0x63b, 0x3, 0x2, 0x2, 0x2, 0x96, 0x63d, 0x3, 0x2, 
    0x2, 0x2, 0x98, 0x63f, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x641, 0x3, 0x2, 0x2, 
    0x2, 0x9c, 0x643, 0x3, 0x2, 0x2, 0x2, 0x9e, 0x645, 0x3, 0x2, 0x2, 0x2, 
    0xa0, 0x647, 0x3, 0x2, 0x2, 0x2, 0xa2, 0x649, 0x3, 0x2, 0x2, 0x2, 0xa4, 
    0x64b, 0x3, 0x2, 0x2, 0x2, 0xa6, 0x64f, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x653, 
    0x3, 0x2, 0x2, 0x2, 0xaa, 0x657, 0x3, 0x2, 0x2, 0x2, 0xac, 0x659, 0x3, 
    0x2, 0x2, 0x2, 0xae, 0x65b, 0x3, 0x2, 0x2, 0x2, 0xb0, 0x65d, 0x3, 0x2, 
    0x2, 0x2, 0xb2, 0x65f, 0x3, 0x2, 0x2, 0x2, 0xb4, 0x66b, 0x3, 0x2, 0x2, 
    0x2, 0xb6, 0x673, 0x3, 0x2, 0x2, 0x2, 0xb8, 0x675, 0x3, 0x2, 0x2, 0x2, 
    0xba, 0x677, 0x3, 0x2, 0x2, 0x2, 0xbc, 0x67b, 0x3, 0x2, 0x2, 0x2, 0xbe, 
    0x67f, 0x3, 0x2, 0x2, 0x2, 0xc0, 0x683, 0x3, 0x2, 0x2, 0x2, 0xc2, 0x687, 
    0x3, 0x2, 0x2, 0x2, 0xc4, 0x68b, 0x3, 0x2, 0x2, 0x2, 0xc6, 0x68f, 0x3, 
    0x2, 0x2, 0x2, 0xc8, 0x692, 0x3, 0x2, 0x2, 0x2, 0xca, 0x695, 0x3, 0x2, 
    0x2, 0x2, 0xcc, 0x698, 0x3, 0x2, 0x2, 0x2, 0xce, 0x69d, 0x3, 0x2, 0x2, 
    0x2, 0xd0, 0x69f, 0x3, 0x2, 0x2, 0x2, 0xd2, 0x6a2, 0x3, 0x2, 0x2, 0x2, 
    0xd4, 0x6bd, 0x3, 0x2, 0x2, 0x2, 0xd6, 0x715, 0x3, 0x2, 0x2, 0x2, 0xd8, 
    0xd9, 0x7, 0x1a, 0x2, 0x2, 0xd9, 0xdb, 0x7, 0x3, 0x2, 0x2, 0xda, 0xd8, 
    0x3, 0x2, 0x2, 0x2, 0xda, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xdb, 0xdc, 0x3, 
    0x2, 0x2, 0x2, 0xdc, 0xdd, 0x5, 0x4, 0x3, 0x2, 0xdd, 0xde, 0x7, 0x4, 
    0x2, 0x2, 0xde, 0x3, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xe3, 0x5, 0x6, 0x4, 
    0x2, 0xe0, 0xe3, 0x5, 0x8, 0x5, 0x2, 0xe1, 0xe3, 0x5, 0x14, 0xb, 0x2, 
    0xe2, 0xdf, 0x3, 0x2, 0x2, 0x2, 0xe2, 0xe0, 0x3, 0x2, 0x2, 0x2, 0xe2, 
    0xe1, 0x3, 0x2, 0x2, 0x2, 0xe3, 0x5, 0x3, 0x2, 0x2, 0x2, 0xe4, 0xe5, 
    0x5, 0x16, 0xc, 0x2, 0xe5, 0x7, 0x3, 0x2, 0x2, 0x2, 0xe6, 0xe8, 0x5, 
    0x18, 0xd, 0x2, 0xe7, 0xe6, 0x3, 0x2, 0x2, 0x2, 0xe8, 0xeb, 0x3, 0x2, 
    0x2, 0x2, 0xe9, 0xe7, 0x3, 0x2, 0x2, 0x2, 0xe9, 0xea, 0x3, 0x2, 0x2, 
    0x2, 0xea, 0xec, 0x3, 0x2, 0x2, 0x2, 0xeb, 0xe9, 0x3, 0x2, 0x2, 0x2, 
    0xec, 0xf0, 0x7, 0x34, 0x2, 0x2, 0xed, 0xef, 0x5, 0xa, 0x6, 0x2, 0xee, 
    0xed, 0x3, 0x2, 0x2, 0x2, 0xef, 0xf2, 0x3, 0x2, 0x2, 0x2, 0xf0, 0xee, 
    0x3, 0x2, 0x2, 0x2, 0xf0, 0xf1, 0x3, 0x2, 0x2, 0x2, 0xf1, 0xf3, 0x3, 
    0x2, 0x2, 0x2, 0xf2, 0xf0, 0x3, 0x2, 0x2, 0x2, 0xf3, 0xf4, 0x5, 0x4, 
    0x3, 0x2, 0xf4, 0x9, 0x3, 0x2, 0x2, 0x2, 0xf5, 0xfa, 0x5, 0xc, 0x7, 
    0x2, 0xf6, 0xfa, 0x5, 0x12, 0xa, 0x2, 0xf7, 0xfa, 0x7, 0x77, 0x2, 0x2, 
    0xf8, 0xfa, 0x7, 0x78, 0x2, 0x2, 0xf9, 0xf5, 0x3, 0x2, 0x2, 0x2, 0xf9, 
    0xf6, 0x3, 0x2, 0x2, 0x2, 0xf9, 0xf7, 0x3, 0x2, 0x2, 0x2, 0xf9, 0xf8, 
    0x3, 0x2, 0x2, 0x2, 0xfa, 0xb, 0x3, 0x2, 0x2, 0x2, 0xfb, 0xfc, 0x7, 
    0x66, 0x2, 0x2, 0xfc, 0x101, 0x5, 0xe, 0x8, 0x2, 0xfd, 0xfe, 0x7, 0x5, 
    0x2, 0x2, 0xfe, 0x100, 0x5, 0xe, 0x8, 0x2, 0xff, 0xfd, 0x3, 0x2, 0x2, 
    0x2, 0x100, 0x103, 0x3, 0x2, 0x2, 0x2, 0x101, 0xff, 0x3, 0x2, 0x2, 0x2, 
    0x101, 0x102, 0x3, 0x2, 0x2, 0x2, 0x102, 0xd, 0x3, 0x2, 0x2, 0x2, 0x103, 
    0x101, 0x3, 0x2, 0x2, 0x2, 0x104, 0x107, 0x5, 0x10, 0x9, 0x2, 0x105, 
    0x106, 0x9, 0x2, 0x2, 0x2, 0x106, 0x108, 0x5, 0x10, 0x9, 0x2, 0x107, 
    0x105, 0x3, 0x2, 0x2, 0x2, 0x107, 0x108, 0x3, 0x2, 0x2, 0x2, 0x108, 
    0xf, 0x3, 0x2, 0x2, 0x2, 0x109, 0x10d, 0x9, 0x3, 0x2, 0x2, 0x10a, 0x10d, 
    0x7, 0xc5, 0x2, 0x2, 0x10b, 0x10d, 0x5, 0xaa, 0x56, 0x2, 0x10c, 0x109, 
    0x3, 0x2, 0x2, 0x2, 0x10c, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x10c, 0x10b, 
    0x3, 0x2, 0x2, 0x2, 0x10d, 0x11, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x10f, 0x7, 
    0x38, 0x2, 0x2, 0x10f, 0x110, 0x9, 0x4, 0x2, 0x2, 0x110, 0x13, 0x3, 
    0x2, 0x2, 0x2, 0x111, 0x113, 0x5, 0x18, 0xd, 0x2, 0x112, 0x111, 0x3, 
    0x2, 0x2, 0x2, 0x113, 0x116, 0x3, 0x2, 0x2, 0x2, 0x114, 0x112, 0x3, 
    0x2, 0x2, 0x2, 0x114, 0x115, 0x3, 0x2, 0x2, 0x2, 0x115, 0x117, 0x3, 
    0x2, 0x2, 0x2, 0x116, 0x114, 0x3, 0x2, 0x2, 0x2, 0x117, 0x118, 0x9, 
    0x5, 0x2, 0x2, 0x118, 0x119, 0x5, 0x4, 0x3, 0x2, 0x119, 0x15, 0x3, 0x2, 
    0x2, 0x2, 0x11a, 0x11c, 0x5, 0x18, 0xd, 0x2, 0x11b, 0x11a, 0x3, 0x2, 
    0x2, 0x2, 0x11c, 0x11f, 0x3, 0x2, 0x2, 0x2, 0x11d, 0x11b, 0x3, 0x2, 
    0x2, 0x2, 0x11d, 0x11e, 0x3, 0x2, 0x2, 0x2, 0x11e, 0x128, 0x3, 0x2, 
    0x2, 0x2, 0x11f, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x120, 0x129, 0x5, 0x1c, 
    0xf, 0x2, 0x121, 0x129, 0x5, 0x20, 0x11, 0x2, 0x122, 0x129, 0x5, 0x34, 
    0x1b, 0x2, 0x123, 0x129, 0x5, 0x36, 0x1c, 0x2, 0x124, 0x129, 0x5, 0x38, 
    0x1d, 0x2, 0x125, 0x129, 0x5, 0x3a, 0x1e, 0x2, 0x126, 0x129, 0x5, 0x40, 
    0x21, 0x2, 0x127, 0x129, 0x5, 0x42, 0x22, 0x2, 0x128, 0x120, 0x3, 0x2, 
    0x2, 0x2, 0x128, 0x121, 0x3, 0x2, 0x2, 0x2, 0x128, 0x122, 0x3, 0x2, 
    0x2, 0x2, 0x128, 0x123, 0x3, 0x2, 0x2, 0x2, 0x128, 0x124, 0x3, 0x2, 
    0x2, 0x2, 0x128, 0x125, 0x3, 0x2, 0x2, 0x2, 0x128, 0x126, 0x3, 0x2, 
    0x2, 0x2, 0x128, 0x127, 0x3, 0x2, 0x2, 0x2, 0x129, 0x17, 0x3, 0x2, 0x2, 
    0x2, 0x12a, 0x12c, 0x7, 0xc6, 0x2, 0x2, 0x12b, 0x12d, 0x5, 0x1a, 0xe, 
    0x2, 0x12c, 0x12b, 0x3, 0x2, 0x2, 0x2, 0x12c, 0x12d, 0x3, 0x2, 0x2, 
    0x2, 0x12d, 0x132, 0x3, 0x2, 0x2, 0x2, 0x12e, 0x12f, 0x7, 0x5, 0x2, 
    0x2, 0x12f, 0x131, 0x5, 0x1a, 0xe, 0x2, 0x130, 0x12e, 0x3, 0x2, 0x2, 
    0x2, 0x131, 0x134, 0x3, 0x2, 0x2, 0x2, 0x132, 0x130, 0x3, 0x2, 0x2, 
    0x2, 0x132, 0x133, 0x3, 0x2, 0x2, 0x2, 0x133, 0x135, 0x3, 0x2, 0x2, 
    0x2, 0x134, 0x132, 0x3, 0x2, 0x2, 0x2, 0x135, 0x136, 0x7, 0xc7, 0x2, 
    0x2, 0x136, 0x19, 0x3, 0x2, 0x2, 0x2, 0x137, 0x14c, 0x5, 0xa4, 0x53, 
    0x2, 0x138, 0x139, 0x5, 0xa4, 0x53, 0x2, 0x139, 0x13a, 0x7, 0x8, 0x2, 
    0x2, 0x13a, 0x13b, 0x5, 0xa8, 0x55, 0x2, 0x13b, 0x14c, 0x3, 0x2, 0x2, 
    0x2, 0x13c, 0x13e, 0x5, 0xa4, 0x53, 0x2, 0x13d, 0x13f, 0x7, 0x8, 0x2, 
    0x2, 0x13e, 0x13d, 0x3, 0x2, 0x2, 0x2, 0x13e, 0x13f, 0x3, 0x2, 0x2, 
    0x2, 0x13f, 0x140, 0x3, 0x2, 0x2, 0x2, 0x140, 0x141, 0x7, 0x9, 0x2, 
    0x2, 0x141, 0x146, 0x5, 0xa8, 0x55, 0x2, 0x142, 0x143, 0x7, 0x5, 0x2, 
    0x2, 0x143, 0x145, 0x5, 0xa8, 0x55, 0x2, 0x144, 0x142, 0x3, 0x2, 0x2, 
    0x2, 0x145, 0x148, 0x3, 0x2, 0x2, 0x2, 0x146, 0x144, 0x3, 0x2, 0x2, 
    0x2, 0x146, 0x147, 0x3, 0x2, 0x2, 0x2, 0x147, 0x149, 0x3, 0x2, 0x2, 
    0x2, 0x148, 0x146, 0x3, 0x2, 0x2, 0x2, 0x149, 0x14a, 0x7, 0xa, 0x2, 
    0x2, 0x14a, 0x14c, 0x3, 0x2, 0x2, 0x2, 0x14b, 0x137, 0x3, 0x2, 0x2, 
    0x2, 0x14b, 0x138, 0x3, 0x2, 0x2, 0x2, 0x14b, 0x13c, 0x3, 0x2, 0x2, 
    0x2, 0x14c, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x14d, 0x14e, 0x7, 0x79, 0x2, 
    0x2, 0x14e, 0x153, 0x5, 0x26, 0x14, 0x2, 0x14f, 0x150, 0x7, 0x5, 0x2, 
    0x2, 0x150, 0x152, 0x5, 0x26, 0x14, 0x2, 0x151, 0x14f, 0x3, 0x2, 0x2, 
    0x2, 0x152, 0x155, 0x3, 0x2, 0x2, 0x2, 0x153, 0x151, 0x3, 0x2, 0x2, 
    0x2, 0x153, 0x154, 0x3, 0x2, 0x2, 0x2, 0x154, 0x157, 0x3, 0x2, 0x2, 
    0x2, 0x155, 0x153, 0x3, 0x2, 0x2, 0x2, 0x156, 0x14d, 0x3, 0x2, 0x2, 
    0x2, 0x156, 0x157, 0x3, 0x2, 0x2, 0x2, 0x157, 0x158, 0x3, 0x2, 0x2, 
    0x2, 0x158, 0x15c, 0x5, 0x1e, 0x10, 0x2, 0x159, 0x15a, 0x7, 0x56, 0x2, 
    0x2, 0x15a, 0x15b, 0x7, 0x21, 0x2, 0x2, 0x15b, 0x15d, 0x5, 0x66, 0x34, 
    0x2, 0x15c, 0x159, 0x3, 0x2, 0x2, 0x2, 0x15c, 0x15d, 0x3, 0x2, 0x2, 
    0x2, 0x15d, 0x164, 0x3, 0x2, 0x2, 0x2, 0x15e, 0x15f, 0x7, 0x49, 0x2, 
    0x2, 0x15f, 0x162, 0x7, 0xc5, 0x2, 0x2, 0x160, 0x161, 0x7, 0x53, 0x2, 
    0x2, 0x161, 0x163, 0x7, 0xc5, 0x2, 0x2, 0x162, 0x160, 0x3, 0x2, 0x2, 
    0x2, 0x162, 0x163, 0x3, 0x2, 0x2, 0x2, 0x163, 0x165, 0x3, 0x2, 0x2, 
    0x2, 0x164, 0x15e, 0x3, 0x2, 0x2, 0x2, 0x164, 0x165, 0x3, 0x2, 0x2, 
    0x2, 0x165, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x166, 0x169, 0x9, 0x6, 0x2, 0x2, 
    0x167, 0x16a, 0x7, 0x17, 0x2, 0x2, 0x168, 0x16a, 0x7, 0x30, 0x2, 0x2, 
    0x169, 0x167, 0x3, 0x2, 0x2, 0x2, 0x169, 0x168, 0x3, 0x2, 0x2, 0x2, 
    0x169, 0x16a, 0x3, 0x2, 0x2, 0x2, 0x16a, 0x16b, 0x3, 0x2, 0x2, 0x2, 
    0x16b, 0x16e, 0x5, 0x5e, 0x30, 0x2, 0x16c, 0x16d, 0x7, 0x3a, 0x2, 0x2, 
    0x16d, 0x16f, 0x5, 0x28, 0x15, 0x2, 0x16e, 0x16c, 0x3, 0x2, 0x2, 0x2, 
    0x16e, 0x16f, 0x3, 0x2, 0x2, 0x2, 0x16f, 0x172, 0x3, 0x2, 0x2, 0x2, 
    0x170, 0x171, 0x7, 0x76, 0x2, 0x2, 0x171, 0x173, 0x5, 0x7e, 0x40, 0x2, 
    0x172, 0x170, 0x3, 0x2, 0x2, 0x2, 0x172, 0x173, 0x3, 0x2, 0x2, 0x2, 
    0x173, 0x177, 0x3, 0x2, 0x2, 0x2, 0x174, 0x175, 0x7, 0x3b, 0x2, 0x2, 
    0x175, 0x176, 0x7, 0x21, 0x2, 0x2, 0x176, 0x178, 0x5, 0x62, 0x32, 0x2, 
    0x177, 0x174, 0x3, 0x2, 0x2, 0x2, 0x177, 0x178, 0x3, 0x2, 0x2, 0x2, 
    0x178, 0x17b, 0x3, 0x2, 0x2, 0x2, 0x179, 0x17a, 0x7, 0x3c, 0x2, 0x2, 
    0x17a, 0x17c, 0x5, 0x7e, 0x40, 0x2, 0x17b, 0x179, 0x3, 0x2, 0x2, 0x2, 
    0x17b, 0x17c, 0x3, 0x2, 0x2, 0x2, 0x17c, 0x182, 0x3, 0x2, 0x2, 0x2, 
    0x17d, 0x17e, 0x7, 0x9, 0x2, 0x2, 0x17e, 0x17f, 0x5, 0x1e, 0x10, 0x2, 
    0x17f, 0x180, 0x7, 0xa, 0x2, 0x2, 0x180, 0x182, 0x3, 0x2, 0x2, 0x2, 
    0x181, 0x166, 0x3, 0x2, 0x2, 0x2, 0x181, 0x17d, 0x3, 0x2, 0x2, 0x2, 
    0x182, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x183, 0x184, 0x7, 0x79, 0x2, 0x2, 
    0x184, 0x189, 0x5, 0x26, 0x14, 0x2, 0x185, 0x186, 0x7, 0x5, 0x2, 0x2, 
    0x186, 0x188, 0x5, 0x26, 0x14, 0x2, 0x187, 0x185, 0x3, 0x2, 0x2, 0x2, 
    0x188, 0x18b, 0x3, 0x2, 0x2, 0x2, 0x189, 0x187, 0x3, 0x2, 0x2, 0x2, 
    0x189, 0x18a, 0x3, 0x2, 0x2, 0x2, 0x18a, 0x18d, 0x3, 0x2, 0x2, 0x2, 
    0x18b, 0x189, 0x3, 0x2, 0x2, 0x2, 0x18c, 0x183, 0x3, 0x2, 0x2, 0x2, 
    0x18c, 0x18d, 0x3, 0x2, 0x2, 0x2, 0x18d, 0x18e, 0x3, 0x2, 0x2, 0x2, 
    0x18e, 0x192, 0x5, 0x22, 0x12, 0x2, 0x18f, 0x190, 0x7, 0x56, 0x2, 0x2, 
    0x190, 0x191, 0x7, 0x21, 0x2, 0x2, 0x191, 0x193, 0x5, 0x66, 0x34, 0x2, 
    0x192, 0x18f, 0x3, 0x2, 0x2, 0x2, 0x192, 0x193, 0x3, 0x2, 0x2, 0x2, 
    0x193, 0x19a, 0x3, 0x2, 0x2, 0x2, 0x194, 0x195, 0x7, 0x49, 0x2, 0x2, 
    0x195, 0x198, 0x7, 0xc5, 0x2, 0x2, 0x196, 0x197, 0x7, 0x53, 0x2, 0x2, 
    0x197, 0x199, 0x7, 0xc5, 0x2, 0x2, 0x198, 0x196, 0x3, 0x2, 0x2, 0x2, 
    0x198, 0x199, 0x3, 0x2, 0x2, 0x2, 0x199, 0x19b, 0x3, 0x2, 0x2, 0x2, 
    0x19a, 0x194, 0x3, 0x2, 0x2, 0x2, 0x19a, 0x19b, 0x3, 0x2, 0x2, 0x2, 
    0x19b, 0x21, 0x3, 0x2, 0x2, 0x2, 0x19c, 0x19d, 0x8, 0x12, 0x1, 0x2, 
    0x19d, 0x1a3, 0x5, 0x1e, 0x10, 0x2, 0x19e, 0x19f, 0x7, 0x9, 0x2, 0x2, 
    0x19f, 0x1a0, 0x5, 0x22, 0x12, 0x2, 0x1a0, 0x1a1, 0x7, 0xa, 0x2, 0x2, 
    0x1a1, 0x1a3, 0x3, 0x2, 0x2, 0x2, 0x1a2, 0x19c, 0x3, 0x2, 0x2, 0x2, 
    0x1a2, 0x19e, 0x3, 0x2, 0x2, 0x2, 0x1a3, 0x1b0, 0x3, 0x2, 0x2, 0x2, 
    0x1a4, 0x1a5, 0xc, 0x4, 0x2, 0x2, 0x1a5, 0x1a8, 0x9, 0x7, 0x2, 0x2, 
    0x1a6, 0x1a9, 0x7, 0x30, 0x2, 0x2, 0x1a7, 0x1a9, 0x7, 0x17, 0x2, 0x2, 
    0x1a8, 0x1a6, 0x3, 0x2, 0x2, 0x2, 0x1a8, 0x1a7, 0x3, 0x2, 0x2, 0x2, 
    0x1a8, 0x1a9, 0x3, 0x2, 0x2, 0x2, 0x1a9, 0x1ab, 0x3, 0x2, 0x2, 0x2, 
    0x1aa, 0x1ac, 0x5, 0x24, 0x13, 0x2, 0x1ab, 0x1aa, 0x3, 0x2, 0x2, 0x2, 
    0x1ab, 0x1ac, 0x3, 0x2, 0x2, 0x2, 0x1ac, 0x1ad, 0x3, 0x2, 0x2, 0x2, 
    0x1ad, 0x1af, 0x5, 0x22, 0x12, 0x5, 0x1ae, 0x1a4, 0x3, 0x2, 0x2, 0x2, 
    0x1af, 0x1b2, 0x3, 0x2, 0x2, 0x2, 0x1b0, 0x1ae, 0x3, 0x2, 0x2, 0x2, 
    0x1b0, 0x1b1, 0x3, 0x2, 0x2, 0x2, 0x1b1, 0x23, 0x3, 0x2, 0x2, 0x2, 0x1b2, 
    0x1b0, 0x3, 0x2, 0x2, 0x2, 0x1b3, 0x1b9, 0x7, 0x24, 0x2, 0x2, 0x1b4, 
    0x1b5, 0x7, 0x21, 0x2, 0x2, 0x1b5, 0x1b6, 0x7, 0x9, 0x2, 0x2, 0x1b6, 
    0x1b7, 0x5, 0x6e, 0x38, 0x2, 0x1b7, 0x1b8, 0x7, 0xa, 0x2, 0x2, 0x1b8, 
    0x1ba, 0x3, 0x2, 0x2, 0x2, 0x1b9, 0x1b4, 0x3, 0x2, 0x2, 0x2, 0x1b9, 
    0x1ba, 0x3, 0x2, 0x2, 0x2, 0x1ba, 0x25, 0x3, 0x2, 0x2, 0x2, 0x1bb, 0x1bd, 
    0x7, 0x5e, 0x2, 0x2, 0x1bc, 0x1bb, 0x3, 0x2, 0x2, 0x2, 0x1bc, 0x1bd, 
    0x3, 0x2, 0x2, 0x2, 0x1bd, 0x1be, 0x3, 0x2, 0x2, 0x2, 0x1be, 0x1c0, 
    0x5, 0x96, 0x4c, 0x2, 0x1bf, 0x1c1, 0x5, 0x72, 0x3a, 0x2, 0x1c0, 0x1bf, 
    0x3, 0x2, 0x2, 0x2, 0x1c0, 0x1c1, 0x3, 0x2, 0x2, 0x2, 0x1c1, 0x1c2, 
    0x3, 0x2, 0x2, 0x2, 0x1c2, 0x1c3, 0x7, 0x1c, 0x2, 0x2, 0x1c3, 0x1c4, 
    0x7, 0x9, 0x2, 0x2, 0x1c4, 0x1c5, 0x5, 0x16, 0xc, 0x2, 0x1c5, 0x1c6, 
    0x7, 0xa, 0x2, 0x2, 0x1c6, 0x27, 0x3, 0x2, 0x2, 0x2, 0x1c7, 0x1c8, 0x8, 
    0x15, 0x1, 0x2, 0x1c8, 0x1cd, 0x5, 0x2a, 0x16, 0x2, 0x1c9, 0x1cb, 0x7, 
    0x1c, 0x2, 0x2, 0x1ca, 0x1c9, 0x3, 0x2, 0x2, 0x2, 0x1ca, 0x1cb, 0x3, 
    0x2, 0x2, 0x2, 0x1cb, 0x1cc, 0x3, 0x2, 0x2, 0x2, 0x1cc, 0x1ce, 0x5, 
    0x98, 0x4d, 0x2, 0x1cd, 0x1ca, 0x3, 0x2, 0x2, 0x2, 0x1cd, 0x1ce, 0x3, 
    0x2, 0x2, 0x2, 0x1ce, 0x1e8, 0x3, 0x2, 0x2, 0x2, 0x1cf, 0x1d0, 0x7, 
    0x9, 0x2, 0x2, 0x1d0, 0x1d1, 0x5, 0x16, 0xc, 0x2, 0x1d1, 0x1d9, 0x7, 
    0xa, 0x2, 0x2, 0x1d2, 0x1d4, 0x7, 0x1c, 0x2, 0x2, 0x1d3, 0x1d2, 0x3, 
    0x2, 0x2, 0x2, 0x1d3, 0x1d4, 0x3, 0x2, 0x2, 0x2, 0x1d4, 0x1d5, 0x3, 
    0x2, 0x2, 0x2, 0x1d5, 0x1d7, 0x5, 0x98, 0x4d, 0x2, 0x1d6, 0x1d8, 0x5, 
    0x72, 0x3a, 0x2, 0x1d7, 0x1d6, 0x3, 0x2, 0x2, 0x2, 0x1d7, 0x1d8, 0x3, 
    0x2, 0x2, 0x2, 0x1d8, 0x1da, 0x3, 0x2, 0x2, 0x2, 0x1d9, 0x1d3, 0x3, 
    0x2, 0x2, 0x2, 0x1d9, 0x1da, 0x3, 0x2, 0x2, 0x2, 0x1da, 0x1e8, 0x3, 
    0x2, 0x2, 0x2, 0x1db, 0x1dc, 0x7, 0x9, 0x2, 0x2, 0x1dc, 0x1dd, 0x5, 
    0x32, 0x1a, 0x2, 0x1dd, 0x1e5, 0x7, 0xa, 0x2, 0x2, 0x1de, 0x1e0, 0x7, 
    0x1c, 0x2, 0x2, 0x1df, 0x1de, 0x3, 0x2, 0x2, 0x2, 0x1df, 0x1e0, 0x3, 
    0x2, 0x2, 0x2, 0x1e0, 0x1e1, 0x3, 0x2, 0x2, 0x2, 0x1e1, 0x1e3, 0x5, 
    0x98, 0x4d, 0x2, 0x1e2, 0x1e4, 0x5, 0x72, 0x3a, 0x2, 0x1e3, 0x1e2, 0x3, 
    0x2, 0x2, 0x2, 0x1e3, 0x1e4, 0x3, 0x2, 0x2, 0x2, 0x1e4, 0x1e6, 0x3, 
    0x2, 0x2, 0x2, 0x1e5, 0x1df, 0x3, 0x2, 0x2, 0x2, 0x1e5, 0x1e6, 0x3, 
    0x2, 0x2, 0x2, 0x1e6, 0x1e8, 0x3, 0x2, 0x2, 0x2, 0x1e7, 0x1c7, 0x3, 
    0x2, 0x2, 0x2, 0x1e7, 0x1cf, 0x3, 0x2, 0x2, 0x2, 0x1e7, 0x1db, 0x3, 
    0x2, 0x2, 0x2, 0x1e8, 0x225, 0x3, 0x2, 0x2, 0x2, 0x1e9, 0x1ed, 0xc, 
    0x8, 0x2, 0x2, 0x1ea, 0x1ec, 0x5, 0x18, 0xd, 0x2, 0x1eb, 0x1ea, 0x3, 
    0x2, 0x2, 0x2, 0x1ec, 0x1ef, 0x3, 0x2, 0x2, 0x2, 0x1ed, 0x1eb, 0x3, 
    0x2, 0x2, 0x2, 0x1ed, 0x1ee, 0x3, 0x2, 0x2, 0x2, 0x1ee, 0x1f3, 0x3, 
    0x2, 0x2, 0x2, 0x1ef, 0x1ed, 0x3, 0x2, 0x2, 0x2, 0x1f0, 0x1f4, 0x7, 
    0x5, 0x2, 0x2, 0x1f1, 0x1f2, 0x7, 0x26, 0x2, 0x2, 0x1f2, 0x1f4, 0x7, 
    0x44, 0x2, 0x2, 0x1f3, 0x1f0, 0x3, 0x2, 0x2, 0x2, 0x1f3, 0x1f1, 0x3, 
    0x2, 0x2, 0x2, 0x1f4, 0x1f5, 0x3, 0x2, 0x2, 0x2, 0x1f5, 0x224, 0x5, 
    0x28, 0x15, 0x9, 0x1f6, 0x1fa, 0xc, 0x7, 0x2, 0x2, 0x1f7, 0x1f9, 0x5, 
    0x18, 0xd, 0x2, 0x1f8, 0x1f7, 0x3, 0x2, 0x2, 0x2, 0x1f9, 0x1fc, 0x3, 
    0x2, 0x2, 0x2, 0x1fa, 0x1f8, 0x3, 0x2, 0x2, 0x2, 0x1fa, 0x1fb, 0x3, 
    0x2, 0x2, 0x2, 0x1fb, 0x1fd, 0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1fa, 0x3, 
    0x2, 0x2, 0x2, 0x1fd, 0x1ff, 0x7, 0x4d, 0x2, 0x2, 0x1fe, 0x200, 0x5, 
    0x2c, 0x17, 0x2, 0x1ff, 0x1fe, 0x3, 0x2, 0x2, 0x2, 0x1ff, 0x200, 0x3, 
    0x2, 0x2, 0x2, 0x200, 0x201, 0x3, 0x2, 0x2, 0x2, 0x201, 0x202, 0x7, 
    0x44, 0x2, 0x2, 0x202, 0x224, 0x5, 0x28, 0x15, 0x8, 0x203, 0x207, 0xc, 
    0x6, 0x2, 0x2, 0x204, 0x206, 0x5, 0x18, 0xd, 0x2, 0x205, 0x204, 0x3, 
    0x2, 0x2, 0x2, 0x206, 0x209, 0x3, 0x2, 0x2, 0x2, 0x207, 0x205, 0x3, 
    0x2, 0x2, 0x2, 0x207, 0x208, 0x3, 0x2, 0x2, 0x2, 0x208, 0x20b, 0x3, 
    0x2, 0x2, 0x2, 0x209, 0x207, 0x3, 0x2, 0x2, 0x2, 0x20a, 0x20c, 0x5, 
    0x2e, 0x18, 0x2, 0x20b, 0x20a, 0x3, 0x2, 0x2, 0x2, 0x20b, 0x20c, 0x3, 
    0x2, 0x2, 0x2, 0x20c, 0x20d, 0x3, 0x2, 0x2, 0x2, 0x20d, 0x20e, 0x7, 
    0x44, 0x2, 0x2, 0x20e, 0x20f, 0x5, 0x28, 0x15, 0x2, 0x20f, 0x210, 0x7, 
    0x54, 0x2, 0x2, 0x210, 0x211, 0x5, 0x80, 0x41, 0x2, 0x211, 0x224, 0x3, 
    0x2, 0x2, 0x2, 0x212, 0x216, 0xc, 0x5, 0x2, 0x2, 0x213, 0x215, 0x5, 
    0x18, 0xd, 0x2, 0x214, 0x213, 0x3, 0x2, 0x2, 0x2, 0x215, 0x218, 0x3, 
    0x2, 0x2, 0x2, 0x216, 0x214, 0x3, 0x2, 0x2, 0x2, 0x216, 0x217, 0x3, 
    0x2, 0x2, 0x2, 0x217, 0x21a, 0x3, 0x2, 0x2, 0x2, 0x218, 0x216, 0x3, 
    0x2, 0x2, 0x2, 0x219, 0x21b, 0x5, 0x2e, 0x18, 0x2, 0x21a, 0x219, 0x3, 
    0x2, 0x2, 0x2, 0x21a, 0x21b, 0x3, 0x2, 0x2, 0x2, 0x21b, 0x21c, 0x3, 
    0x2, 0x2, 0x2, 0x21c, 0x21d, 0x7, 0x44, 0x2, 0x2, 0x21d, 0x21e, 0x5, 
    0x28, 0x15, 0x2, 0x21e, 0x21f, 0x7, 0x74, 0x2, 0x2, 0x21f, 0x220, 0x7, 
    0x9, 0x2, 0x2, 0x220, 0x221, 0x5, 0x6a, 0x36, 0x2, 0x221, 0x222, 0x7, 
    0xa, 0x2, 0x2, 0x222, 0x224, 0x3, 0x2, 0x2, 0x2, 0x223, 0x1e9, 0x3, 
    0x2, 0x2, 0x2, 0x223, 0x1f6, 0x3, 0x2, 0x2, 0x2, 0x223, 0x203, 0x3, 
    0x2, 0x2, 0x2, 0x223, 0x212, 0x3, 0x2, 0x2, 0x2, 0x224, 0x227, 0x3, 
    0x2, 0x2, 0x2, 0x225, 0x223, 0x3, 0x2, 0x2, 0x2, 0x225, 0x226, 0x3, 
    0x2, 0x2, 0x2, 0x226, 0x29, 0x3, 0x2, 0x2, 0x2, 0x227, 0x225, 0x3, 0x2, 
    0x2, 0x2, 0x228, 0x229, 0x5, 0x92, 0x4a, 0x2, 0x229, 0x22a, 0x7, 0xc, 
    0x2, 0x2, 0x22a, 0x22c, 0x3, 0x2, 0x2, 0x2, 0x22b, 0x228, 0x3, 0x2, 
    0x2, 0x2, 0x22b, 0x22c, 0x3, 0x2, 0x2, 0x2, 0x22c, 0x22d, 0x3, 0x2, 
    0x2, 0x2, 0x22d, 0x22e, 0x5, 0x94, 0x4b, 0x2, 0x22e, 0x22f, 0x7, 0xc, 
    0x2, 0x2, 0x22f, 0x231, 0x3, 0x2, 0x2, 0x2, 0x230, 0x22b, 0x3, 0x2, 
    0x2, 0x2, 0x230, 0x231, 0x3, 0x2, 0x2, 0x2, 0x231, 0x232, 0x3, 0x2, 
    0x2, 0x2, 0x232, 0x233, 0x5, 0x96, 0x4c, 0x2, 0x233, 0x2b, 0x3, 0x2, 
    0x2, 0x2, 0x234, 0x23a, 0x7, 0x3f, 0x2, 0x2, 0x235, 0x237, 0x9, 0x8, 
    0x2, 0x2, 0x236, 0x238, 0x7, 0x57, 0x2, 0x2, 0x237, 0x236, 0x3, 0x2, 
    0x2, 0x2, 0x237, 0x238, 0x3, 0x2, 0x2, 0x2, 0x238, 0x23a, 0x3, 0x2, 
    0x2, 0x2, 0x239, 0x234, 0x3, 0x2, 0x2, 0x2, 0x239, 0x235, 0x3, 0x2, 
    0x2, 0x2, 0x23a, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x23b, 0x23f, 0x5, 0x2c, 
    0x17, 0x2, 0x23c, 0x23f, 0x7, 0x64, 0x2, 0x2, 0x23d, 0x23f, 0x7, 0x19, 
    0x2, 0x2, 0x23e, 0x23b, 0x3, 0x2, 0x2, 0x2, 0x23e, 0x23c, 0x3, 0x2, 
    0x2, 0x2, 0x23e, 0x23d, 0x3, 0x2, 0x2, 0x2, 0x23f, 0x2f, 0x3, 0x2, 0x2, 
    0x2, 0x240, 0x241, 0x7, 0xc6, 0x2, 0x2, 0x241, 0x250, 0x9, 0x9, 0x2, 
    0x2, 0x242, 0x244, 0x7, 0x8, 0x2, 0x2, 0x243, 0x242, 0x3, 0x2, 0x2, 
    0x2, 0x243, 0x244, 0x3, 0x2, 0x2, 0x2, 0x244, 0x245, 0x3, 0x2, 0x2, 
    0x2, 0x245, 0x246, 0x7, 0x9, 0x2, 0x2, 0x246, 0x24b, 0x5, 0xa8, 0x55, 
    0x2, 0x247, 0x248, 0x7, 0x5, 0x2, 0x2, 0x248, 0x24a, 0x5, 0xa8, 0x55, 
    0x2, 0x249, 0x247, 0x3, 0x2, 0x2, 0x2, 0x24a, 0x24d, 0x3, 0x2, 0x2, 
    0x2, 0x24b, 0x249, 0x3, 0x2, 0x2, 0x2, 0x24b, 0x24c, 0x3, 0x2, 0x2, 
    0x2, 0x24c, 0x24e, 0x3, 0x2, 0x2, 0x2, 0x24d, 0x24b, 0x3, 0x2, 0x2, 
    0x2, 0x24e, 0x24f, 0x7, 0xa, 0x2, 0x2, 0x24f, 0x251, 0x3, 0x2, 0x2, 
    0x2, 0x250, 0x243, 0x3, 0x2, 0x2, 0x2, 0x250, 0x251, 0x3, 0x2, 0x2, 
    0x2, 0x251, 0x252, 0x3, 0x2, 0x2, 0x2, 0x252, 0x253, 0x7, 0xc7, 0x2, 
    0x2, 0x253, 0x31, 0x3, 0x2, 0x2, 0x2, 0x254, 0x255, 0x7, 0x75, 0x2, 
    0x2, 0x255, 0x256, 0x7, 0x9, 0x2, 0x2, 0x256, 0x257, 0x5, 0x7c, 0x3f, 
    0x2, 0x257, 0x25f, 0x7, 0xa, 0x2, 0x2, 0x258, 0x259, 0x7, 0x5, 0x2, 
    0x2, 0x259, 0x25a, 0x7, 0x9, 0x2, 0x2, 0x25a, 0x25b, 0x5, 0x7c, 0x3f, 
    0x2, 0x25b, 0x25c, 0x7, 0xa, 0x2, 0x2, 0x25c, 0x25e, 0x3, 0x2, 0x2, 
    0x2, 0x25d, 0x258, 0x3, 0x2, 0x2, 0x2, 0x25e, 0x261, 0x3, 0x2, 0x2, 
    0x2, 0x25f, 0x25d, 0x3, 0x2, 0x2, 0x2, 0x25f, 0x260, 0x3, 0x2, 0x2, 
    0x2, 0x260, 0x33, 0x3, 0x2, 0x2, 0x2, 0x261, 0x25f, 0x3, 0x2, 0x2, 0x2, 
    0x262, 0x263, 0x7, 0x79, 0x2, 0x2, 0x263, 0x268, 0x5, 0x26, 0x14, 0x2, 
    0x264, 0x265, 0x7, 0x5, 0x2, 0x2, 0x265, 0x267, 0x5, 0x26, 0x14, 0x2, 
    0x266, 0x264, 0x3, 0x2, 0x2, 0x2, 0x267, 0x26a, 0x3, 0x2, 0x2, 0x2, 
    0x268, 0x266, 0x3, 0x2, 0x2, 0x2, 0x268, 0x269, 0x3, 0x2, 0x2, 0x2, 
    0x269, 0x26c, 0x3, 0x2, 0x2, 0x2, 0x26a, 0x268, 0x3, 0x2, 0x2, 0x2, 
    0x26b, 0x262, 0x3, 0x2, 0x2, 0x2, 0x26b, 0x26c, 0x3, 0x2, 0x2, 0x2, 
    0x26c, 0x26d, 0x3, 0x2, 0x2, 0x2, 0x26d, 0x26f, 0x7, 0x40, 0x2, 0x2, 
    0x26e, 0x270, 0x7, 0x42, 0x2, 0x2, 0x26f, 0x26e, 0x3, 0x2, 0x2, 0x2, 
    0x26f, 0x270, 0x3, 0x2, 0x2, 0x2, 0x270, 0x271, 0x3, 0x2, 0x2, 0x2, 
    0x271, 0x27d, 0x5, 0x2a, 0x16, 0x2, 0x272, 0x273, 0x7, 0x9, 0x2, 0x2, 
    0x273, 0x278, 0x5, 0x9a, 0x4e, 0x2, 0x274, 0x275, 0x7, 0x5, 0x2, 0x2, 
    0x275, 0x277, 0x5, 0x9a, 0x4e, 0x2, 0x276, 0x274, 0x3, 0x2, 0x2, 0x2, 
    0x277, 0x27a, 0x3, 0x2, 0x2, 0x2, 0x278, 0x276, 0x3, 0x2, 0x2, 0x2, 
    0x278, 0x279, 0x3, 0x2, 0x2, 0x2, 0x279, 0x27b, 0x3, 0x2, 0x2, 0x2, 
    0x27a, 0x278, 0x3, 0x2, 0x2, 0x2, 0x27b, 0x27c, 0x7, 0xa, 0x2, 0x2, 
    0x27c, 0x27e, 0x3, 0x2, 0x2, 0x2, 0x27d, 0x272, 0x3, 0x2, 0x2, 0x2, 
    0x27d, 0x27e, 0x3, 0x2, 0x2, 0x2, 0x27e, 0x282, 0x3, 0x2, 0x2, 0x2, 
    0x27f, 0x283, 0x5, 0x32, 0x1a, 0x2, 0x280, 0x283, 0x5, 0x1c, 0xf, 0x2, 
    0x281, 0x283, 0x5, 0x20, 0x11, 0x2, 0x282, 0x27f, 0x3, 0x2, 0x2, 0x2, 
    0x282, 0x280, 0x3, 0x2, 0x2, 0x2, 0x282, 0x281, 0x3, 0x2, 0x2, 0x2, 
    0x283, 0x35, 0x3, 0x2, 0x2, 0x2, 0x284, 0x285, 0x7, 0x79, 0x2, 0x2, 
    0x285, 0x28a, 0x5, 0x26, 0x14, 0x2, 0x286, 0x287, 0x7, 0x5, 0x2, 0x2, 
    0x287, 0x289, 0x5, 0x26, 0x14, 0x2, 0x288, 0x286, 0x3, 0x2, 0x2, 0x2, 
    0x289, 0x28c, 0x3, 0x2, 0x2, 0x2, 0x28a, 0x288, 0x3, 0x2, 0x2, 0x2, 
    0x28a, 0x28b, 0x3, 0x2, 0x2, 0x2, 0x28b, 0x28e, 0x3, 0x2, 0x2, 0x2, 
    0x28c, 0x28a, 0x3, 0x2, 0x2, 0x2, 0x28d, 0x284, 0x3, 0x2, 0x2, 0x2, 
    0x28d, 0x28e, 0x3, 0x2, 0x2, 0x2, 0x28e, 0x28f, 0x3, 0x2, 0x2, 0x2, 
    0x28f, 0x291, 0x7, 0x2d, 0x2, 0x2, 0x290, 0x292, 0x7, 0x3a, 0x2, 0x2, 
    0x291, 0x290, 0x3, 0x2, 0x2, 0x2, 0x291, 0x292, 0x3, 0x2, 0x2, 0x2, 
    0x292, 0x293, 0x3, 0x2, 0x2, 0x2, 0x293, 0x298, 0x5, 0x2a, 0x16, 0x2, 
    0x294, 0x295, 0x7, 0x76, 0x2, 0x2, 0x295, 0x299, 0x5, 0x7e, 0x40, 0x2, 
    0x296, 0x297, 0x7, 0x3a, 0x2, 0x2, 0x297, 0x299, 0x5, 0x28, 0x15, 0x2, 
    0x298, 0x294, 0x3, 0x2, 0x2, 0x2, 0x298, 0x296, 0x3, 0x2, 0x2, 0x2, 
    0x298, 0x299, 0x3, 0x2, 0x2, 0x2, 0x299, 0x37, 0x3, 0x2, 0x2, 0x2, 0x29a, 
    0x29b, 0x7, 0x79, 0x2, 0x2, 0x29b, 0x2a0, 0x5, 0x26, 0x14, 0x2, 0x29c, 
    0x29d, 0x7, 0x5, 0x2, 0x2, 0x29d, 0x29f, 0x5, 0x26, 0x14, 0x2, 0x29e, 
    0x29c, 0x3, 0x2, 0x2, 0x2, 0x29f, 0x2a2, 0x3, 0x2, 0x2, 0x2, 0x2a0, 
    0x29e, 0x3, 0x2, 0x2, 0x2, 0x2a0, 0x2a1, 0x3, 0x2, 0x2, 0x2, 0x2a1, 
    0x2a4, 0x3, 0x2, 0x2, 0x2, 0x2a2, 0x2a0, 0x3, 0x2, 0x2, 0x2, 0x2a3, 
    0x29a, 0x3, 0x2, 0x2, 0x2, 0x2a3, 0x2a4, 0x3, 0x2, 0x2, 0x2, 0x2a4, 
    0x2a5, 0x3, 0x2, 0x2, 0x2, 0x2a5, 0x2a6, 0x7, 0x73, 0x2, 0x2, 0x2a6, 
    0x2a7, 0x5, 0x2a, 0x16, 0x2, 0x2a7, 0x2a8, 0x7, 0x65, 0x2, 0x2, 0x2a8, 
    0x2a9, 0x5, 0x9a, 0x4e, 0x2, 0x2a9, 0x2aa, 0x7, 0x8, 0x2, 0x2, 0x2aa, 
    0x2b2, 0x5, 0x84, 0x43, 0x2, 0x2ab, 0x2ac, 0x7, 0x5, 0x2, 0x2, 0x2ac, 
    0x2ad, 0x5, 0x9a, 0x4e, 0x2, 0x2ad, 0x2ae, 0x7, 0x8, 0x2, 0x2, 0x2ae, 
    0x2af, 0x5, 0x84, 0x43, 0x2, 0x2af, 0x2b1, 0x3, 0x2, 0x2, 0x2, 0x2b0, 
    0x2ab, 0x3, 0x2, 0x2, 0x2, 0x2b1, 0x2b4, 0x3, 0x2, 0x2, 0x2, 0x2b2, 
    0x2b0, 0x3, 0x2, 0x2, 0x2, 0x2b2, 0x2b3, 0x3, 0x2, 0x2, 0x2, 0x2b3, 
    0x2b9, 0x3, 0x2, 0x2, 0x2, 0x2b4, 0x2b2, 0x3, 0x2, 0x2, 0x2, 0x2b5, 
    0x2b6, 0x7, 0x76, 0x2, 0x2, 0x2b6, 0x2ba, 0x5, 0x7e, 0x40, 0x2, 0x2b7, 
    0x2b8, 0x7, 0x3a, 0x2, 0x2, 0x2b8, 0x2ba, 0x5, 0x28, 0x15, 0x2, 0x2b9, 
    0x2b5, 0x3, 0x2, 0x2, 0x2, 0x2b9, 0x2b7, 0x3, 0x2, 0x2, 0x2, 0x2b9, 
    0x2ba, 0x3, 0x2, 0x2, 0x2, 0x2ba, 0x39, 0x3, 0x2, 0x2, 0x2, 0x2bb, 0x2be, 
    0x5, 0x3c, 0x1f, 0x2, 0x2bc, 0x2be, 0x5, 0x3e, 0x20, 0x2, 0x2bd, 0x2bb, 
    0x3, 0x2, 0x2, 0x2, 0x2bd, 0x2bc, 0x3, 0x2, 0x2, 0x2, 0x2be, 0x3b, 0x3, 
    0x2, 0x2, 0x2, 0x2bf, 0x2c1, 0x7, 0x25, 0x2, 0x2, 0x2c0, 0x2c2, 0x7, 
    0x5f, 0x2, 0x2, 0x2c1, 0x2c0, 0x3, 0x2, 0x2, 0x2, 0x2c1, 0x2c2, 0x3, 
    0x2, 0x2, 0x2, 0x2c2, 0x2c3, 0x3, 0x2, 0x2, 0x2, 0x2c3, 0x2c7, 0x7, 
    0x6a, 0x2, 0x2, 0x2c4, 0x2c5, 0x7, 0x98, 0x2, 0x2, 0x2c5, 0x2c6, 0x7, 
    0x50, 0x2, 0x2, 0x2c6, 0x2c8, 0x7, 0x97, 0x2, 0x2, 0x2c7, 0x2c4, 0x3, 
    0x2, 0x2, 0x2, 0x2c7, 0x2c8, 0x3, 0x2, 0x2, 0x2, 0x2c8, 0x2c9, 0x3, 
    0x2, 0x2, 0x2, 0x2c9, 0x2e5, 0x5, 0x2a, 0x16, 0x2, 0x2ca, 0x2e6, 0x5, 
    0x44, 0x23, 0x2, 0x2cb, 0x2cd, 0x5, 0x44, 0x23, 0x2, 0x2cc, 0x2cb, 0x3, 
    0x2, 0x2, 0x2, 0x2cc, 0x2cd, 0x3, 0x2, 0x2, 0x2, 0x2cd, 0x2cf, 0x3, 
    0x2, 0x2, 0x2, 0x2ce, 0x2d0, 0x7, 0x1c, 0x2, 0x2, 0x2cf, 0x2ce, 0x3, 
    0x2, 0x2, 0x2, 0x2cf, 0x2d0, 0x3, 0x2, 0x2, 0x2, 0x2d0, 0x2d1, 0x3, 
    0x2, 0x2, 0x2, 0x2d1, 0x2d7, 0x5, 0x5a, 0x2e, 0x2, 0x2d2, 0x2d4, 0x7, 
    0x79, 0x2, 0x2, 0x2d3, 0x2d5, 0x7, 0x4e, 0x2, 0x2, 0x2d4, 0x2d3, 0x3, 
    0x2, 0x2, 0x2, 0x2d4, 0x2d5, 0x3, 0x2, 0x2, 0x2, 0x2d5, 0x2d6, 0x3, 
    0x2, 0x2, 0x2, 0x2d6, 0x2d8, 0x7, 0x2b, 0x2, 0x2, 0x2d7, 0x2d2, 0x3, 
    0x2, 0x2, 0x2, 0x2d7, 0x2d8, 0x3, 0x2, 0x2, 0x2, 0x2d8, 0x2e6, 0x3, 
    0x2, 0x2, 0x2, 0x2d9, 0x2db, 0x5, 0x4a, 0x26, 0x2, 0x2da, 0x2dc, 0x7, 
    0x1c, 0x2, 0x2, 0x2db, 0x2da, 0x3, 0x2, 0x2, 0x2, 0x2db, 0x2dc, 0x3, 
    0x2, 0x2, 0x2, 0x2dc, 0x2dd, 0x3, 0x2, 0x2, 0x2, 0x2dd, 0x2e3, 0x5, 
    0x5a, 0x2e, 0x2, 0x2de, 0x2e0, 0x7, 0x79, 0x2, 0x2, 0x2df, 0x2e1, 0x7, 
    0x4e, 0x2, 0x2, 0x2e0, 0x2df, 0x3, 0x2, 0x2, 0x2, 0x2e0, 0x2e1, 0x3, 
    0x2, 0x2, 0x2, 0x2e1, 0x2e2, 0x3, 0x2, 0x2, 0x2, 0x2e2, 0x2e4, 0x7, 
    0x2b, 0x2, 0x2, 0x2e3, 0x2de, 0x3, 0x2, 0x2, 0x2, 0x2e3, 0x2e4, 0x3, 
    0x2, 0x2, 0x2, 0x2e4, 0x2e6, 0x3, 0x2, 0x2, 0x2, 0x2e5, 0x2ca, 0x3, 
    0x2, 0x2, 0x2, 0x2e5, 0x2cc, 0x3, 0x2, 0x2, 0x2, 0x2e5, 0x2d9, 0x3, 
    0x2, 0x2, 0x2, 0x2e6, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x2e7, 0x2e9, 0x7, 0x25, 
    0x2, 0x2, 0x2e8, 0x2ea, 0x7, 0x72, 0x2, 0x2, 0x2e9, 0x2e8, 0x3, 0x2, 
    0x2, 0x2, 0x2e9, 0x2ea, 0x3, 0x2, 0x2, 0x2, 0x2ea, 0x2eb, 0x3, 0x2, 
    0x2, 0x2, 0x2eb, 0x2ef, 0x9, 0xa, 0x2, 0x2, 0x2ec, 0x2ed, 0x7, 0x98, 
    0x2, 0x2, 0x2ed, 0x2ee, 0x7, 0x50, 0x2, 0x2, 0x2ee, 0x2f0, 0x7, 0x97, 
    0x2, 0x2, 0x2ef, 0x2ec, 0x3, 0x2, 0x2, 0x2, 0x2ef, 0x2f0, 0x3, 0x2, 
    0x2, 0x2, 0x2f0, 0x2f1, 0x3, 0x2, 0x2, 0x2, 0x2f1, 0x2f2, 0x5, 0xa0, 
    0x51, 0x2, 0x2f2, 0x2f3, 0x7, 0x54, 0x2, 0x2, 0x2f3, 0x2f6, 0x5, 0x2a, 
    0x16, 0x2, 0x2f4, 0x2f5, 0x7, 0x74, 0x2, 0x2, 0x2f5, 0x2f7, 0x5, 0x50, 
    0x29, 0x2, 0x2f6, 0x2f4, 0x3, 0x2, 0x2, 0x2, 0x2f6, 0x2f7, 0x3, 0x2, 
    0x2, 0x2, 0x2f7, 0x2f8, 0x3, 0x2, 0x2, 0x2, 0x2f8, 0x2f9, 0x7, 0x9, 
    0x2, 0x2, 0x2f9, 0x2fe, 0x5, 0x84, 0x43, 0x2, 0x2fa, 0x2fb, 0x7, 0x5, 
    0x2, 0x2, 0x2fb, 0x2fd, 0x5, 0x84, 0x43, 0x2, 0x2fc, 0x2fa, 0x3, 0x2, 
    0x2, 0x2, 0x2fd, 0x300, 0x3, 0x2, 0x2, 0x2, 0x2fe, 0x2fc, 0x3, 0x2, 
    0x2, 0x2, 0x2fe, 0x2ff, 0x3, 0x2, 0x2, 0x2, 0x2ff, 0x301, 0x3, 0x2, 
    0x2, 0x2, 0x300, 0x2fe, 0x3, 0x2, 0x2, 0x2, 0x301, 0x304, 0x7, 0xa, 
    0x2, 0x2, 0x302, 0x303, 0x7, 0x79, 0x2, 0x2, 0x303, 0x305, 0x5, 0x78, 
    0x3d, 0x2, 0x304, 0x302, 0x3, 0x2, 0x2, 0x2, 0x304, 0x305, 0x3, 0x2, 
    0x2, 0x2, 0x305, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x306, 0x307, 0x7, 0x31, 
    0x2, 0x2, 0x307, 0x30a, 0x7, 0x6a, 0x2, 0x2, 0x308, 0x309, 0x7, 0x98, 
    0x2, 0x2, 0x309, 0x30b, 0x7, 0x97, 0x2, 0x2, 0x30a, 0x308, 0x3, 0x2, 
    0x2, 0x2, 0x30a, 0x30b, 0x3, 0x2, 0x2, 0x2, 0x30b, 0x30c, 0x3, 0x2, 
    0x2, 0x2, 0x30c, 0x318, 0x5, 0x2a, 0x16, 0x2, 0x30d, 0x30e, 0x7, 0x31, 
    0x2, 0x2, 0x30e, 0x311, 0x9, 0xa, 0x2, 0x2, 0x30f, 0x310, 0x7, 0x98, 
    0x2, 0x2, 0x310, 0x312, 0x7, 0x97, 0x2, 0x2, 0x311, 0x30f, 0x3, 0x2, 
    0x2, 0x2, 0x311, 0x312, 0x3, 0x2, 0x2, 0x2, 0x312, 0x313, 0x3, 0x2, 
    0x2, 0x2, 0x313, 0x314, 0x5, 0xa0, 0x51, 0x2, 0x314, 0x315, 0x7, 0x54, 
    0x2, 0x2, 0x315, 0x316, 0x5, 0x2a, 0x16, 0x2, 0x316, 0x318, 0x3, 0x2, 
    0x2, 0x2, 0x317, 0x306, 0x3, 0x2, 0x2, 0x2, 0x317, 0x30d, 0x3, 0x2, 
    0x2, 0x2, 0x318, 0x41, 0x3, 0x2, 0x2, 0x2, 0x319, 0x31b, 0x7, 0x6e, 
    0x2, 0x2, 0x31a, 0x31c, 0x7, 0x6a, 0x2, 0x2, 0x31b, 0x31a, 0x3, 0x2, 
    0x2, 0x2, 0x31b, 0x31c, 0x3, 0x2, 0x2, 0x2, 0x31c, 0x31d, 0x3, 0x2, 
    0x2, 0x2, 0x31d, 0x31e, 0x5, 0x2a, 0x16, 0x2, 0x31e, 0x43, 0x3, 0x2, 
    0x2, 0x2, 0x31f, 0x323, 0x7, 0x9, 0x2, 0x2, 0x320, 0x324, 0x5, 0x48, 
    0x25, 0x2, 0x321, 0x324, 0x5, 0x4c, 0x27, 0x2, 0x322, 0x324, 0x5, 0x4e, 
    0x28, 0x2, 0x323, 0x320, 0x3, 0x2, 0x2, 0x2, 0x323, 0x321, 0x3, 0x2, 
    0x2, 0x2, 0x323, 0x322, 0x3, 0x2, 0x2, 0x2, 0x324, 0x32d, 0x3, 0x2, 
    0x2, 0x2, 0x325, 0x329, 0x7, 0x5, 0x2, 0x2, 0x326, 0x32a, 0x5, 0x48, 
    0x25, 0x2, 0x327, 0x32a, 0x5, 0x4c, 0x27, 0x2, 0x328, 0x32a, 0x5, 0x4e, 
    0x28, 0x2, 0x329, 0x326, 0x3, 0x2, 0x2, 0x2, 0x329, 0x327, 0x3, 0x2, 
    0x2, 0x2, 0x329, 0x328, 0x3, 0x2, 0x2, 0x2, 0x32a, 0x32c, 0x3, 0x2, 
    0x2, 0x2, 0x32b, 0x325, 0x3, 0x2, 0x2, 0x2, 0x32c, 0x32f, 0x3, 0x2, 
    0x2, 0x2, 0x32d, 0x32b, 0x3, 0x2, 0x2, 0x2, 0x32d, 0x32e, 0x3, 0x2, 
    0x2, 0x2, 0x32e, 0x330, 0x3, 0x2, 0x2, 0x2, 0x32f, 0x32d, 0x3, 0x2, 
    0x2, 0x2, 0x330, 0x332, 0x7, 0xa, 0x2, 0x2, 0x331, 0x333, 0x7, 0x5f, 
    0x2, 0x2, 0x332, 0x331, 0x3, 0x2, 0x2, 0x2, 0x332, 0x333, 0x3, 0x2, 
    0x2, 0x2, 0x333, 0x335, 0x3, 0x2, 0x2, 0x2, 0x334, 0x336, 0x5, 0x46, 
    0x24, 0x2, 0x335, 0x334, 0x3, 0x2, 0x2, 0x2, 0x335, 0x336, 0x3, 0x2, 
    0x2, 0x2, 0x336, 0x339, 0x3, 0x2, 0x2, 0x2, 0x337, 0x338, 0x7, 0x79, 
    0x2, 0x2, 0x338, 0x33a, 0x5, 0x78, 0x3d, 0x2, 0x339, 0x337, 0x3, 0x2, 
    0x2, 0x2, 0x339, 0x33a, 0x3, 0x2, 0x2, 0x2, 0x33a, 0x45, 0x3, 0x2, 0x2, 
    0x2, 0x33b, 0x33d, 0x7, 0x5a, 0x2, 0x2, 0x33c, 0x33e, 0x7, 0x21, 0x2, 
    0x2, 0x33d, 0x33c, 0x3, 0x2, 0x2, 0x2, 0x33d, 0x33e, 0x3, 0x2, 0x2, 
    0x2, 0x33e, 0x340, 0x3, 0x2, 0x2, 0x2, 0x33f, 0x341, 0x9, 0xb, 0x2, 
    0x2, 0x340, 0x33f, 0x3, 0x2, 0x2, 0x2, 0x340, 0x341, 0x3, 0x2, 0x2, 
    0x2, 0x341, 0x343, 0x3, 0x2, 0x2, 0x2, 0x342, 0x344, 0x7, 0x54, 0x2, 
    0x2, 0x343, 0x342, 0x3, 0x2, 0x2, 0x2, 0x343, 0x344, 0x3, 0x2, 0x2, 
    0x2, 0x344, 0x345, 0x3, 0x2, 0x2, 0x2, 0x345, 0x346, 0x7, 0x9, 0x2, 
    0x2, 0x346, 0x34b, 0x5, 0x84, 0x43, 0x2, 0x347, 0x348, 0x7, 0x5, 0x2, 
    0x2, 0x348, 0x34a, 0x5, 0x84, 0x43, 0x2, 0x349, 0x347, 0x3, 0x2, 0x2, 
    0x2, 0x34a, 0x34d, 0x3, 0x2, 0x2, 0x2, 0x34b, 0x349, 0x3, 0x2, 0x2, 
    0x2, 0x34b, 0x34c, 0x3, 0x2, 0x2, 0x2, 0x34c, 0x34e, 0x3, 0x2, 0x2, 
    0x2, 0x34d, 0x34b, 0x3, 0x2, 0x2, 0x2, 0x34e, 0x34f, 0x7, 0xa, 0x2, 
    0x2, 0x34f, 0x47, 0x3, 0x2, 0x2, 0x2, 0x350, 0x351, 0x5, 0x9a, 0x4e, 
    0x2, 0x351, 0x363, 0x5, 0x52, 0x2a, 0x2, 0x352, 0x354, 0x7, 0x50, 0x2, 
    0x2, 0x353, 0x352, 0x3, 0x2, 0x2, 0x2, 0x353, 0x354, 0x3, 0x2, 0x2, 
    0x2, 0x354, 0x355, 0x3, 0x2, 0x2, 0x2, 0x355, 0x362, 0x7, 0x52, 0x2, 
    0x2, 0x356, 0x357, 0x7, 0x5d, 0x2, 0x2, 0x357, 0x362, 0x7, 0x45, 0x2, 
    0x2, 0x358, 0x35a, 0x7, 0x72, 0x2, 0x2, 0x359, 0x35b, 0x7, 0x45, 0x2, 
    0x2, 0x35a, 0x359, 0x3, 0x2, 0x2, 0x2, 0x35a, 0x35b, 0x3, 0x2, 0x2, 
    0x2, 0x35b, 0x362, 0x3, 0x2, 0x2, 0x2, 0x35c, 0x35d, 0x7, 0x72, 0x2, 
    0x2, 0x35d, 0x35f, 0x7, 0x4c, 0x2, 0x2, 0x35e, 0x360, 0x7, 0x45, 0x2, 
    0x2, 0x35f, 0x35e, 0x3, 0x2, 0x2, 0x2, 0x35f, 0x360, 0x3, 0x2, 0x2, 
    0x2, 0x360, 0x362, 0x3, 0x2, 0x2, 0x2, 0x361, 0x353, 0x3, 0x2, 0x2, 
    0x2, 0x361, 0x356, 0x3, 0x2, 0x2, 0x2, 0x361, 0x358, 0x3, 0x2, 0x2, 
    0x2, 0x361, 0x35c, 0x3, 0x2, 0x2, 0x2, 0x362, 0x365, 0x3, 0x2, 0x2, 
    0x2, 0x363, 0x361, 0x3, 0x2, 0x2, 0x2, 0x363, 0x364, 0x3, 0x2, 0x2, 
    0x2, 0x364, 0x368, 0x3, 0x2, 0x2, 0x2, 0x365, 0x363, 0x3, 0x2, 0x2, 
    0x2, 0x366, 0x367, 0x7, 0x79, 0x2, 0x2, 0x367, 0x369, 0x5, 0x78, 0x3d, 
    0x2, 0x368, 0x366, 0x3, 0x2, 0x2, 0x2, 0x368, 0x369, 0x3, 0x2, 0x2, 
    0x2, 0x369, 0x49, 0x3, 0x2, 0x2, 0x2, 0x36a, 0x36b, 0x7, 0x9, 0x2, 0x2, 
    0x36b, 0x370, 0x5, 0x9a, 0x4e, 0x2, 0x36c, 0x36d, 0x7, 0x5, 0x2, 0x2, 
    0x36d, 0x36f, 0x5, 0x9a, 0x4e, 0x2, 0x36e, 0x36c, 0x3, 0x2, 0x2, 0x2, 
    0x36f, 0x372, 0x3, 0x2, 0x2, 0x2, 0x370, 0x36e, 0x3, 0x2, 0x2, 0x2, 
    0x370, 0x371, 0x3, 0x2, 0x2, 0x2, 0x371, 0x373, 0x3, 0x2, 0x2, 0x2, 
    0x372, 0x370, 0x3, 0x2, 0x2, 0x2, 0x373, 0x374, 0x7, 0xa, 0x2, 0x2, 
    0x374, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x375, 0x376, 0x7, 0x5d, 0x2, 0x2, 
    0x376, 0x383, 0x7, 0x45, 0x2, 0x2, 0x377, 0x384, 0x5, 0x84, 0x43, 0x2, 
    0x378, 0x379, 0x7, 0x9, 0x2, 0x2, 0x379, 0x37e, 0x5, 0x84, 0x43, 0x2, 
    0x37a, 0x37b, 0x7, 0x5, 0x2, 0x2, 0x37b, 0x37d, 0x5, 0x84, 0x43, 0x2, 
    0x37c, 0x37a, 0x3, 0x2, 0x2, 0x2, 0x37d, 0x380, 0x3, 0x2, 0x2, 0x2, 
    0x37e, 0x37c, 0x3, 0x2, 0x2, 0x2, 0x37e, 0x37f, 0x3, 0x2, 0x2, 0x2, 
    0x37f, 0x381, 0x3, 0x2, 0x2, 0x2, 0x380, 0x37e, 0x3, 0x2, 0x2, 0x2, 
    0x381, 0x382, 0x7, 0xa, 0x2, 0x2, 0x382, 0x384, 0x3, 0x2, 0x2, 0x2, 
    0x383, 0x377, 0x3, 0x2, 0x2, 0x2, 0x383, 0x378, 0x3, 0x2, 0x2, 0x2, 
    0x384, 0x399, 0x3, 0x2, 0x2, 0x2, 0x385, 0x387, 0x7, 0x72, 0x2, 0x2, 
    0x386, 0x385, 0x3, 0x2, 0x2, 0x2, 0x386, 0x387, 0x3, 0x2, 0x2, 0x2, 
    0x387, 0x388, 0x3, 0x2, 0x2, 0x2, 0x388, 0x389, 0x7, 0x4c, 0x2, 0x2, 
    0x389, 0x396, 0x7, 0x45, 0x2, 0x2, 0x38a, 0x397, 0x5, 0x84, 0x43, 0x2, 
    0x38b, 0x38c, 0x7, 0x9, 0x2, 0x2, 0x38c, 0x391, 0x5, 0x84, 0x43, 0x2, 
    0x38d, 0x38e, 0x7, 0x5, 0x2, 0x2, 0x38e, 0x390, 0x5, 0x84, 0x43, 0x2, 
    0x38f, 0x38d, 0x3, 0x2, 0x2, 0x2, 0x390, 0x393, 0x3, 0x2, 0x2, 0x2, 
    0x391, 0x38f, 0x3, 0x2, 0x2, 0x2, 0x391, 0x392, 0x3, 0x2, 0x2, 0x2, 
    0x392, 0x394, 0x3, 0x2, 0x2, 0x2, 0x393, 0x391, 0x3, 0x2, 0x2, 0x2, 
    0x394, 0x395, 0x7, 0xa, 0x2, 0x2, 0x395, 0x397, 0x3, 0x2, 0x2, 0x2, 
    0x396, 0x38a, 0x3, 0x2, 0x2, 0x2, 0x396, 0x38b, 0x3, 0x2, 0x2, 0x2, 
    0x397, 0x399, 0x3, 0x2, 0x2, 0x2, 0x398, 0x375, 0x3, 0x2, 0x2, 0x2, 
    0x398, 0x386, 0x3, 0x2, 0x2, 0x2, 0x399, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x39a, 
    0x39c, 0x7, 0x72, 0x2, 0x2, 0x39b, 0x39a, 0x3, 0x2, 0x2, 0x2, 0x39b, 
    0x39c, 0x3, 0x2, 0x2, 0x2, 0x39c, 0x39d, 0x3, 0x2, 0x2, 0x2, 0x39d, 
    0x39f, 0x9, 0xa, 0x2, 0x2, 0x39e, 0x3a0, 0x5, 0xa0, 0x51, 0x2, 0x39f, 
    0x39e, 0x3, 0x2, 0x2, 0x2, 0x39f, 0x3a0, 0x3, 0x2, 0x2, 0x2, 0x3a0, 
    0x3a2, 0x3, 0x2, 0x2, 0x2, 0x3a1, 0x3a3, 0x7, 0x54, 0x2, 0x2, 0x3a2, 
    0x3a1, 0x3, 0x2, 0x2, 0x2, 0x3a2, 0x3a3, 0x3, 0x2, 0x2, 0x2, 0x3a3, 
    0x3b0, 0x3, 0x2, 0x2, 0x2, 0x3a4, 0x3b1, 0x5, 0x84, 0x43, 0x2, 0x3a5, 
    0x3a6, 0x7, 0x9, 0x2, 0x2, 0x3a6, 0x3ab, 0x5, 0x84, 0x43, 0x2, 0x3a7, 
    0x3a8, 0x7, 0x5, 0x2, 0x2, 0x3a8, 0x3aa, 0x5, 0x84, 0x43, 0x2, 0x3a9, 
    0x3a7, 0x3, 0x2, 0x2, 0x2, 0x3aa, 0x3ad, 0x3, 0x2, 0x2, 0x2, 0x3ab, 
    0x3a9, 0x3, 0x2, 0x2, 0x2, 0x3ab, 0x3ac, 0x3, 0x2, 0x2, 0x2, 0x3ac, 
    0x3ae, 0x3, 0x2, 0x2, 0x2, 0x3ad, 0x3ab, 0x3, 0x2, 0x2, 0x2, 0x3ae, 
    0x3af, 0x7, 0xa, 0x2, 0x2, 0x3af, 0x3b1, 0x3, 0x2, 0x2, 0x2, 0x3b0, 
    0x3a4, 0x3, 0x2, 0x2, 0x2, 0x3b0, 0x3a5, 0x3, 0x2, 0x2, 0x2, 0x3b1, 
    0x3b4, 0x3, 0x2, 0x2, 0x2, 0x3b2, 0x3b3, 0x7, 0x74, 0x2, 0x2, 0x3b3, 
    0x3b5, 0x5, 0x50, 0x29, 0x2, 0x3b4, 0x3b2, 0x3, 0x2, 0x2, 0x2, 0x3b4, 
    0x3b5, 0x3, 0x2, 0x2, 0x2, 0x3b5, 0x3b8, 0x3, 0x2, 0x2, 0x2, 0x3b6, 
    0x3b7, 0x7, 0x79, 0x2, 0x2, 0x3b7, 0x3b9, 0x5, 0x78, 0x3d, 0x2, 0x3b8, 
    0x3b6, 0x3, 0x2, 0x2, 0x2, 0x3b8, 0x3b9, 0x3, 0x2, 0x2, 0x2, 0x3b9, 
    0x4f, 0x3, 0x2, 0x2, 0x2, 0x3ba, 0x3bb, 0x9, 0xc, 0x2, 0x2, 0x3bb, 0x51, 
    0x3, 0x2, 0x2, 0x2, 0x3bc, 0x3bd, 0x5, 0x54, 0x2b, 0x2, 0x3bd, 0x53, 
    0x3, 0x2, 0x2, 0x2, 0x3be, 0x3fd, 0x7, 0x7b, 0x2, 0x2, 0x3bf, 0x3c7, 
    0x7, 0x7c, 0x2, 0x2, 0x3c0, 0x3c1, 0x7, 0x9, 0x2, 0x2, 0x3c1, 0x3c3, 
    0x5, 0xae, 0x58, 0x2, 0x3c2, 0x3c4, 0x9, 0xd, 0x2, 0x2, 0x3c3, 0x3c2, 
    0x3, 0x2, 0x2, 0x2, 0x3c3, 0x3c4, 0x3, 0x2, 0x2, 0x2, 0x3c4, 0x3c5, 
    0x3, 0x2, 0x2, 0x2, 0x3c5, 0x3c6, 0x7, 0xa, 0x2, 0x2, 0x3c6, 0x3c8, 
    0x3, 0x2, 0x2, 0x2, 0x3c7, 0x3c0, 0x3, 0x2, 0x2, 0x2, 0x3c7, 0x3c8, 
    0x3, 0x2, 0x2, 0x2, 0x3c8, 0x3fd, 0x3, 0x2, 0x2, 0x2, 0x3c9, 0x3d8, 
    0x7, 0x7d, 0x2, 0x2, 0x3ca, 0x3cb, 0x7, 0x9, 0x2, 0x2, 0x3cb, 0x3cd, 
    0x5, 0xae, 0x58, 0x2, 0x3cc, 0x3ce, 0x7, 0x8f, 0x2, 0x2, 0x3cd, 0x3cc, 
    0x3, 0x2, 0x2, 0x2, 0x3cd, 0x3ce, 0x3, 0x2, 0x2, 0x2, 0x3ce, 0x3d4, 
    0x3, 0x2, 0x2, 0x2, 0x3cf, 0x3d0, 0x7, 0x5, 0x2, 0x2, 0x3d0, 0x3d2, 
    0x5, 0xb0, 0x59, 0x2, 0x3d1, 0x3d3, 0x7, 0x8f, 0x2, 0x2, 0x3d2, 0x3d1, 
    0x3, 0x2, 0x2, 0x2, 0x3d2, 0x3d3, 0x3, 0x2, 0x2, 0x2, 0x3d3, 0x3d5, 
    0x3, 0x2, 0x2, 0x2, 0x3d4, 0x3cf, 0x3, 0x2, 0x2, 0x2, 0x3d4, 0x3d5, 
    0x3, 0x2, 0x2, 0x2, 0x3d5, 0x3d6, 0x3, 0x2, 0x2, 0x2, 0x3d6, 0x3d7, 
    0x7, 0xa, 0x2, 0x2, 0x3d7, 0x3d9, 0x3, 0x2, 0x2, 0x2, 0x3d8, 0x3ca, 
    0x3, 0x2, 0x2, 0x2, 0x3d8, 0x3d9, 0x3, 0x2, 0x2, 0x2, 0x3d9, 0x3fd, 
    0x3, 0x2, 0x2, 0x2, 0x3da, 0x3e2, 0x7, 0x7e, 0x2, 0x2, 0x3db, 0x3dc, 
    0x7, 0x9, 0x2, 0x2, 0x3dc, 0x3de, 0x5, 0xae, 0x58, 0x2, 0x3dd, 0x3df, 
    0x9, 0xd, 0x2, 0x2, 0x3de, 0x3dd, 0x3, 0x2, 0x2, 0x2, 0x3de, 0x3df, 
    0x3, 0x2, 0x2, 0x2, 0x3df, 0x3e0, 0x3, 0x2, 0x2, 0x2, 0x3e0, 0x3e1, 
    0x7, 0xa, 0x2, 0x2, 0x3e1, 0x3e3, 0x3, 0x2, 0x2, 0x2, 0x3e2, 0x3db, 
    0x3, 0x2, 0x2, 0x2, 0x3e2, 0x3e3, 0x3, 0x2, 0x2, 0x2, 0x3e3, 0x3fd, 
    0x3, 0x2, 0x2, 0x2, 0x3e4, 0x3fd, 0x7, 0x2c, 0x2, 0x2, 0x3e5, 0x3fd, 
    0x7, 0x7f, 0x2, 0x2, 0x3e6, 0x3fd, 0x7, 0x80, 0x2, 0x2, 0x3e7, 0x3fd, 
    0x5, 0x56, 0x2c, 0x2, 0x3e8, 0x3fd, 0x5, 0x58, 0x2d, 0x2, 0x3e9, 0x3ea, 
    0x7, 0x86, 0x2, 0x2, 0x3ea, 0x3eb, 0x7, 0x9, 0x2, 0x2, 0x3eb, 0x3ed, 
    0x5, 0xb2, 0x5a, 0x2, 0x3ec, 0x3ee, 0x9, 0xe, 0x2, 0x2, 0x3ed, 0x3ec, 
    0x3, 0x2, 0x2, 0x2, 0x3ed, 0x3ee, 0x3, 0x2, 0x2, 0x2, 0x3ee, 0x3ef, 
    0x3, 0x2, 0x2, 0x2, 0x3ef, 0x3f0, 0x7, 0xa, 0x2, 0x2, 0x3f0, 0x3fd, 
    0x3, 0x2, 0x2, 0x2, 0x3f1, 0x3f9, 0x7, 0x87, 0x2, 0x2, 0x3f2, 0x3f3, 
    0x7, 0x9, 0x2, 0x2, 0x3f3, 0x3f5, 0x5, 0xb2, 0x5a, 0x2, 0x3f4, 0x3f6, 
    0x9, 0xe, 0x2, 0x2, 0x3f5, 0x3f4, 0x3, 0x2, 0x2, 0x2, 0x3f5, 0x3f6, 
    0x3, 0x2, 0x2, 0x2, 0x3f6, 0x3f7, 0x3, 0x2, 0x2, 0x2, 0x3f7, 0x3f8, 
    0x7, 0xa, 0x2, 0x2, 0x3f8, 0x3fa, 0x3, 0x2, 0x2, 0x2, 0x3f9, 0x3f2, 
    0x3, 0x2, 0x2, 0x2, 0x3f9, 0x3fa, 0x3, 0x2, 0x2, 0x2, 0x3fa, 0x3fd, 
    0x3, 0x2, 0x2, 0x2, 0x3fb, 0x3fd, 0x7, 0x88, 0x2, 0x2, 0x3fc, 0x3be, 
    0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3bf, 0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3c9, 
    0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3da, 0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3e4, 
    0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3e5, 0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3e6, 
    0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3e7, 0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3e8, 
    0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3e9, 0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3f1, 
    0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3fb, 0x3, 0x2, 0x2, 0x2, 0x3fd, 0x55, 0x3, 
    0x2, 0x2, 0x2, 0x3fe, 0x3ff, 0x9, 0xf, 0x2, 0x2, 0x3ff, 0x400, 0x7, 
    0x9, 0x2, 0x2, 0x400, 0x402, 0x5, 0xb2, 0x5a, 0x2, 0x401, 0x403, 0x9, 
    0x10, 0x2, 0x2, 0x402, 0x401, 0x3, 0x2, 0x2, 0x2, 0x402, 0x403, 0x3, 
    0x2, 0x2, 0x2, 0x403, 0x404, 0x3, 0x2, 0x2, 0x2, 0x404, 0x407, 0x7, 
    0xa, 0x2, 0x2, 0x405, 0x406, 0x7, 0x23, 0x2, 0x2, 0x406, 0x408, 0x5, 
    0x9e, 0x50, 0x2, 0x407, 0x405, 0x3, 0x2, 0x2, 0x2, 0x407, 0x408, 0x3, 
    0x2, 0x2, 0x2, 0x408, 0x57, 0x3, 0x2, 0x2, 0x2, 0x409, 0x40b, 0x9, 0xf, 
    0x2, 0x2, 0x40a, 0x40c, 0x7, 0x89, 0x2, 0x2, 0x40b, 0x40a, 0x3, 0x2, 
    0x2, 0x2, 0x40b, 0x40c, 0x3, 0x2, 0x2, 0x2, 0x40c, 0x40f, 0x3, 0x2, 
    0x2, 0x2, 0x40d, 0x40f, 0x7, 0x84, 0x2, 0x2, 0x40e, 0x409, 0x3, 0x2, 
    0x2, 0x2, 0x40e, 0x40d, 0x3, 0x2, 0x2, 0x2, 0x40f, 0x417, 0x3, 0x2, 
    0x2, 0x2, 0x410, 0x411, 0x7, 0x9, 0x2, 0x2, 0x411, 0x413, 0x5, 0xb2, 
    0x5a, 0x2, 0x412, 0x414, 0x9, 0x10, 0x2, 0x2, 0x413, 0x412, 0x3, 0x2, 
    0x2, 0x2, 0x413, 0x414, 0x3, 0x2, 0x2, 0x2, 0x414, 0x415, 0x3, 0x2, 
    0x2, 0x2, 0x415, 0x416, 0x7, 0xa, 0x2, 0x2, 0x416, 0x418, 0x3, 0x2, 
    0x2, 0x2, 0x417, 0x410, 0x3, 0x2, 0x2, 0x2, 0x417, 0x418, 0x3, 0x2, 
    0x2, 0x2, 0x418, 0x41b, 0x3, 0x2, 0x2, 0x2, 0x419, 0x41b, 0x7, 0x85, 
    0x2, 0x2, 0x41a, 0x40e, 0x3, 0x2, 0x2, 0x2, 0x41a, 0x419, 0x3, 0x2, 
    0x2, 0x2, 0x41b, 0x41e, 0x3, 0x2, 0x2, 0x2, 0x41c, 0x41d, 0x7, 0x23, 
    0x2, 0x2, 0x41d, 0x41f, 0x5, 0x9e, 0x50, 0x2, 0x41e, 0x41c, 0x3, 0x2, 
    0x2, 0x2, 0x41e, 0x41f, 0x3, 0x2, 0x2, 0x2, 0x41f, 0x59, 0x3, 0x2, 0x2, 
    0x2, 0x420, 0x421, 0x5, 0x5c, 0x2f, 0x2, 0x421, 0x5b, 0x3, 0x2, 0x2, 
    0x2, 0x422, 0x426, 0x7, 0x9, 0x2, 0x2, 0x423, 0x425, 0x5, 0x18, 0xd, 
    0x2, 0x424, 0x423, 0x3, 0x2, 0x2, 0x2, 0x425, 0x428, 0x3, 0x2, 0x2, 
    0x2, 0x426, 0x424, 0x3, 0x2, 0x2, 0x2, 0x426, 0x427, 0x3, 0x2, 0x2, 
    0x2, 0x427, 0x429, 0x3, 0x2, 0x2, 0x2, 0x428, 0x426, 0x3, 0x2, 0x2, 
    0x2, 0x429, 0x42a, 0x5, 0x5c, 0x2f, 0x2, 0x42a, 0x42b, 0x7, 0xa, 0x2, 
    0x2, 0x42b, 0x438, 0x3, 0x2, 0x2, 0x2, 0x42c, 0x42e, 0x5, 0x18, 0xd, 
    0x2, 0x42d, 0x42c, 0x3, 0x2, 0x2, 0x2, 0x42e, 0x431, 0x3, 0x2, 0x2, 
    0x2, 0x42f, 0x42d, 0x3, 0x2, 0x2, 0x2, 0x42f, 0x430, 0x3, 0x2, 0x2, 
    0x2, 0x430, 0x435, 0x3, 0x2, 0x2, 0x2, 0x431, 0x42f, 0x3, 0x2, 0x2, 
    0x2, 0x432, 0x436, 0x5, 0x1c, 0xf, 0x2, 0x433, 0x436, 0x5, 0x32, 0x1a, 
    0x2, 0x434, 0x436, 0x5, 0x20, 0x11, 0x2, 0x435, 0x432, 0x3, 0x2, 0x2, 
    0x2, 0x435, 0x433, 0x3, 0x2, 0x2, 0x2, 0x435, 0x434, 0x3, 0x2, 0x2, 
    0x2, 0x436, 0x438, 0x3, 0x2, 0x2, 0x2, 0x437, 0x422, 0x3, 0x2, 0x2, 
    0x2, 0x437, 0x42f, 0x3, 0x2, 0x2, 0x2, 0x438, 0x5d, 0x3, 0x2, 0x2, 0x2, 
    0x439, 0x43e, 0x5, 0x60, 0x31, 0x2, 0x43a, 0x43b, 0x7, 0x5, 0x2, 0x2, 
    0x43b, 0x43d, 0x5, 0x60, 0x31, 0x2, 0x43c, 0x43a, 0x3, 0x2, 0x2, 0x2, 
    0x43d, 0x440, 0x3, 0x2, 0x2, 0x2, 0x43e, 0x43c, 0x3, 0x2, 0x2, 0x2, 
    0x43e, 0x43f, 0x3, 0x2, 0x2, 0x2, 0x43f, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x440, 
    0x43e, 0x3, 0x2, 0x2, 0x2, 0x441, 0x446, 0x5, 0x84, 0x43, 0x2, 0x442, 
    0x444, 0x7, 0x1c, 0x2, 0x2, 0x443, 0x442, 0x3, 0x2, 0x2, 0x2, 0x443, 
    0x444, 0x3, 0x2, 0x2, 0x2, 0x444, 0x445, 0x3, 0x2, 0x2, 0x2, 0x445, 
    0x447, 0x5, 0xaa, 0x56, 0x2, 0x446, 0x443, 0x3, 0x2, 0x2, 0x2, 0x446, 
    0x447, 0x3, 0x2, 0x2, 0x2, 0x447, 0x44a, 0x3, 0x2, 0x2, 0x2, 0x448, 
    0x44a, 0x5, 0x8a, 0x46, 0x2, 0x449, 0x441, 0x3, 0x2, 0x2, 0x2, 0x449, 
    0x448, 0x3, 0x2, 0x2, 0x2, 0x44a, 0x61, 0x3, 0x2, 0x2, 0x2, 0x44b, 0x450, 
    0x5, 0x64, 0x33, 0x2, 0x44c, 0x44d, 0x7, 0x5, 0x2, 0x2, 0x44d, 0x44f, 
    0x5, 0x64, 0x33, 0x2, 0x44e, 0x44c, 0x3, 0x2, 0x2, 0x2, 0x44f, 0x452, 
    0x3, 0x2, 0x2, 0x2, 0x450, 0x44e, 0x3, 0x2, 0x2, 0x2, 0x450, 0x451, 
    0x3, 0x2, 0x2, 0x2, 0x451, 0x63, 0x3, 0x2, 0x2, 0x2, 0x452, 0x450, 0x3, 
    0x2, 0x2, 0x2, 0x453, 0x454, 0x5, 0x84, 0x43, 0x2, 0x454, 0x65, 0x3, 
    0x2, 0x2, 0x2, 0x455, 0x45a, 0x5, 0x68, 0x35, 0x2, 0x456, 0x457, 0x7, 
    0x5, 0x2, 0x2, 0x457, 0x459, 0x5, 0x68, 0x35, 0x2, 0x458, 0x456, 0x3, 
    0x2, 0x2, 0x2, 0x459, 0x45c, 0x3, 0x2, 0x2, 0x2, 0x45a, 0x458, 0x3, 
    0x2, 0x2, 0x2, 0x45a, 0x45b, 0x3, 0x2, 0x2, 0x2, 0x45b, 0x67, 0x3, 0x2, 
    0x2, 0x2, 0x45c, 0x45a, 0x3, 0x2, 0x2, 0x2, 0x45d, 0x45f, 0x5, 0x84, 
    0x43, 0x2, 0x45e, 0x460, 0x9, 0x11, 0x2, 0x2, 0x45f, 0x45e, 0x3, 0x2, 
    0x2, 0x2, 0x45f, 0x460, 0x3, 0x2, 0x2, 0x2, 0x460, 0x69, 0x3, 0x2, 0x2, 
    0x2, 0x461, 0x466, 0x5, 0x6c, 0x37, 0x2, 0x462, 0x463, 0x7, 0x5, 0x2, 
    0x2, 0x463, 0x465, 0x5, 0x6c, 0x37, 0x2, 0x464, 0x462, 0x3, 0x2, 0x2, 
    0x2, 0x465, 0x468, 0x3, 0x2, 0x2, 0x2, 0x466, 0x464, 0x3, 0x2, 0x2, 
    0x2, 0x466, 0x467, 0x3, 0x2, 0x2, 0x2, 0x467, 0x6b, 0x3, 0x2, 0x2, 0x2, 
    0x468, 0x466, 0x3, 0x2, 0x2, 0x2, 0x469, 0x46a, 0x5, 0xaa, 0x56, 0x2, 
    0x46a, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x46b, 0x470, 0x5, 0x70, 0x39, 0x2, 
    0x46c, 0x46d, 0x7, 0x5, 0x2, 0x2, 0x46d, 0x46f, 0x5, 0x70, 0x39, 0x2, 
    0x46e, 0x46c, 0x3, 0x2, 0x2, 0x2, 0x46f, 0x472, 0x3, 0x2, 0x2, 0x2, 
    0x470, 0x46e, 0x3, 0x2, 0x2, 0x2, 0x470, 0x471, 0x3, 0x2, 0x2, 0x2, 
    0x471, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x472, 0x470, 0x3, 0x2, 0x2, 0x2, 0x473, 
    0x474, 0x5, 0xaa, 0x56, 0x2, 0x474, 0x71, 0x3, 0x2, 0x2, 0x2, 0x475, 
    0x476, 0x7, 0x9, 0x2, 0x2, 0x476, 0x477, 0x5, 0x74, 0x3b, 0x2, 0x477, 
    0x478, 0x7, 0xa, 0x2, 0x2, 0x478, 0x73, 0x3, 0x2, 0x2, 0x2, 0x479, 0x47e, 
    0x5, 0x76, 0x3c, 0x2, 0x47a, 0x47b, 0x7, 0x5, 0x2, 0x2, 0x47b, 0x47d, 
    0x5, 0x76, 0x3c, 0x2, 0x47c, 0x47a, 0x3, 0x2, 0x2, 0x2, 0x47d, 0x480, 
    0x3, 0x2, 0x2, 0x2, 0x47e, 0x47c, 0x3, 0x2, 0x2, 0x2, 0x47e, 0x47f, 
    0x3, 0x2, 0x2, 0x2, 0x47f, 0x75, 0x3, 0x2, 0x2, 0x2, 0x480, 0x47e, 0x3, 
    0x2, 0x2, 0x2, 0x481, 0x482, 0x5, 0xaa, 0x56, 0x2, 0x482, 0x77, 0x3, 
    0x2, 0x2, 0x2, 0x483, 0x488, 0x5, 0x7a, 0x3e, 0x2, 0x484, 0x485, 0x7, 
    0x5, 0x2, 0x2, 0x485, 0x487, 0x5, 0x7a, 0x3e, 0x2, 0x486, 0x484, 0x3, 
    0x2, 0x2, 0x2, 0x487, 0x48a, 0x3, 0x2, 0x2, 0x2, 0x488, 0x486, 0x3, 
    0x2, 0x2, 0x2, 0x488, 0x489, 0x3, 0x2, 0x2, 0x2, 0x489, 0x79, 0x3, 0x2, 
    0x2, 0x2, 0x48a, 0x488, 0x3, 0x2, 0x2, 0x2, 0x48b, 0x48e, 0x5, 0xa2, 
    0x52, 0x2, 0x48c, 0x48d, 0x7, 0x8, 0x2, 0x2, 0x48d, 0x48f, 0x5, 0xa6, 
    0x54, 0x2, 0x48e, 0x48c, 0x3, 0x2, 0x2, 0x2, 0x48e, 0x48f, 0x3, 0x2, 
    0x2, 0x2, 0x48f, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x490, 0x495, 0x5, 0x84, 
    0x43, 0x2, 0x491, 0x492, 0x7, 0x5, 0x2, 0x2, 0x492, 0x494, 0x5, 0x84, 
    0x43, 0x2, 0x493, 0x491, 0x3, 0x2, 0x2, 0x2, 0x494, 0x497, 0x3, 0x2, 
    0x2, 0x2, 0x495, 0x493, 0x3, 0x2, 0x2, 0x2, 0x495, 0x496, 0x3, 0x2, 
    0x2, 0x2, 0x496, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x497, 0x495, 0x3, 0x2, 0x2, 
    0x2, 0x498, 0x499, 0x5, 0x84, 0x43, 0x2, 0x499, 0x7f, 0x3, 0x2, 0x2, 
    0x2, 0x49a, 0x49b, 0x5, 0x84, 0x43, 0x2, 0x49b, 0x81, 0x3, 0x2, 0x2, 
    0x2, 0x49c, 0x49d, 0x5, 0x84, 0x43, 0x2, 0x49d, 0x83, 0x3, 0x2, 0x2, 
    0x2, 0x49e, 0x49f, 0x8, 0x43, 0x1, 0x2, 0x49f, 0x5a6, 0x5, 0xb4, 0x5b, 
    0x2, 0x4a0, 0x5a6, 0x5, 0x86, 0x44, 0x2, 0x4a1, 0x5a6, 0x5, 0x88, 0x45, 
    0x2, 0x4a2, 0x4a3, 0x7, 0x9, 0x2, 0x2, 0x4a3, 0x4a4, 0x5, 0x52, 0x2a, 
    0x2, 0x4a4, 0x4a5, 0x7, 0xa, 0x2, 0x2, 0x4a5, 0x4a6, 0x5, 0x84, 0x43, 
    0x2a, 0x4a6, 0x5a6, 0x3, 0x2, 0x2, 0x2, 0x4a7, 0x4a8, 0x9, 0x12, 0x2, 
    0x2, 0x4a8, 0x5a6, 0x5, 0x84, 0x43, 0x29, 0x4a9, 0x4aa, 0x7, 0x91, 0x2, 
    0x2, 0x4aa, 0x4ab, 0x7, 0x9, 0x2, 0x2, 0x4ab, 0x4ac, 0x5, 0x84, 0x43, 
    0x2, 0x4ac, 0x4ad, 0x9, 0x13, 0x2, 0x2, 0x4ad, 0x4ae, 0x5, 0x52, 0x2a, 
    0x2, 0x4ae, 0x4af, 0x7, 0xa, 0x2, 0x2, 0x4af, 0x5a6, 0x3, 0x2, 0x2, 
    0x2, 0x4b0, 0x4b1, 0x7, 0x9f, 0x2, 0x2, 0x4b1, 0x4b2, 0x7, 0x9, 0x2, 
    0x2, 0x4b2, 0x4b3, 0x5, 0x84, 0x43, 0x2, 0x4b3, 0x4b4, 0x7, 0xa, 0x2, 
    0x2, 0x4b4, 0x5a6, 0x3, 0x2, 0x2, 0x2, 0x4b5, 0x4c9, 0x7, 0x90, 0x2, 
    0x2, 0x4b6, 0x4bc, 0x5, 0x84, 0x43, 0x2, 0x4b7, 0x4b8, 0x7, 0xa0, 0x2, 
    0x2, 0x4b8, 0x4b9, 0x5, 0x84, 0x43, 0x2, 0x4b9, 0x4ba, 0x7, 0xa1, 0x2, 
    0x2, 0x4ba, 0x4bb, 0x5, 0x84, 0x43, 0x2, 0x4bb, 0x4bd, 0x3, 0x2, 0x2, 
    0x2, 0x4bc, 0x4b7, 0x3, 0x2, 0x2, 0x2, 0x4bd, 0x4be, 0x3, 0x2, 0x2, 
    0x2, 0x4be, 0x4bc, 0x3, 0x2, 0x2, 0x2, 0x4be, 0x4bf, 0x3, 0x2, 0x2, 
    0x2, 0x4bf, 0x4ca, 0x3, 0x2, 0x2, 0x2, 0x4c0, 0x4c1, 0x7, 0xa0, 0x2, 
    0x2, 0x4c1, 0x4c2, 0x5, 0x82, 0x42, 0x2, 0x4c2, 0x4c3, 0x7, 0xa1, 0x2, 
    0x2, 0x4c3, 0x4c4, 0x5, 0x84, 0x43, 0x2, 0x4c4, 0x4c6, 0x3, 0x2, 0x2, 
    0x2, 0x4c5, 0x4c0, 0x3, 0x2, 0x2, 0x2, 0x4c6, 0x4c7, 0x3, 0x2, 0x2, 
    0x2, 0x4c7, 0x4c5, 0x3, 0x2, 0x2, 0x2, 0x4c7, 0x4c8, 0x3, 0x2, 0x2, 
    0x2, 0x4c8, 0x4ca, 0x3, 0x2, 0x2, 0x2, 0x4c9, 0x4b6, 0x3, 0x2, 0x2, 
    0x2, 0x4c9, 0x4c5, 0x3, 0x2, 0x2, 0x2, 0x4ca, 0x4cd, 0x3, 0x2, 0x2, 
    0x2, 0x4cb, 0x4cc, 0x7, 0xa2, 0x2, 0x2, 0x4cc, 0x4ce, 0x5, 0x84, 0x43, 
    0x2, 0x4cd, 0x4cb, 0x3, 0x2, 0x2, 0x2, 0x4cd, 0x4ce, 0x3, 0x2, 0x2, 
    0x2, 0x4ce, 0x4cf, 0x3, 0x2, 0x2, 0x2, 0x4cf, 0x4d0, 0x7, 0xa3, 0x2, 
    0x2, 0x4d0, 0x5a6, 0x3, 0x2, 0x2, 0x2, 0x4d1, 0x4d2, 0x7, 0x98, 0x2, 
    0x2, 0x4d2, 0x4d3, 0x7, 0x9, 0x2, 0x2, 0x4d3, 0x4d4, 0x5, 0x82, 0x42, 
    0x2, 0x4d4, 0x4d5, 0x7, 0x5, 0x2, 0x2, 0x4d5, 0x4d6, 0x5, 0x84, 0x43, 
    0x2, 0x4d6, 0x4d7, 0x7, 0x5, 0x2, 0x2, 0x4d7, 0x4d8, 0x5, 0x84, 0x43, 
    0x2, 0x4d8, 0x4d9, 0x7, 0xa, 0x2, 0x2, 0x4d9, 0x5a6, 0x3, 0x2, 0x2, 
    0x2, 0x4da, 0x4db, 0x7, 0x99, 0x2, 0x2, 0x4db, 0x4dc, 0x7, 0x9, 0x2, 
    0x2, 0x4dc, 0x4dd, 0x5, 0x84, 0x43, 0x2, 0x4dd, 0x4de, 0x7, 0x5, 0x2, 
    0x2, 0x4de, 0x4df, 0x5, 0x84, 0x43, 0x2, 0x4df, 0x4e0, 0x7, 0xa, 0x2, 
    0x2, 0x4e0, 0x5a6, 0x3, 0x2, 0x2, 0x2, 0x4e1, 0x4e2, 0x7, 0x9a, 0x2, 
    0x2, 0x4e2, 0x4e3, 0x7, 0x9, 0x2, 0x2, 0x4e3, 0x4e4, 0x5, 0x84, 0x43, 
    0x2, 0x4e4, 0x4e5, 0x7, 0x5, 0x2, 0x2, 0x4e5, 0x4e6, 0x5, 0x84, 0x43, 
    0x2, 0x4e6, 0x4e7, 0x7, 0xa, 0x2, 0x2, 0x4e7, 0x5a6, 0x3, 0x2, 0x2, 
    0x2, 0x4e8, 0x4e9, 0x7, 0x92, 0x2, 0x2, 0x4e9, 0x4f2, 0x7, 0x9, 0x2, 
    0x2, 0x4ea, 0x4ef, 0x5, 0x84, 0x43, 0x2, 0x4eb, 0x4ec, 0x7, 0x5, 0x2, 
    0x2, 0x4ec, 0x4ee, 0x5, 0x84, 0x43, 0x2, 0x4ed, 0x4eb, 0x3, 0x2, 0x2, 
    0x2, 0x4ee, 0x4f1, 0x3, 0x2, 0x2, 0x2, 0x4ef, 0x4ed, 0x3, 0x2, 0x2, 
    0x2, 0x4ef, 0x4f0, 0x3, 0x2, 0x2, 0x2, 0x4f0, 0x4f3, 0x3, 0x2, 0x2, 
    0x2, 0x4f1, 0x4ef, 0x3, 0x2, 0x2, 0x2, 0x4f2, 0x4ea, 0x3, 0x2, 0x2, 
    0x2, 0x4f2, 0x4f3, 0x3, 0x2, 0x2, 0x2, 0x4f3, 0x4f4, 0x3, 0x2, 0x2, 
    0x2, 0x4f4, 0x5a6, 0x7, 0xa, 0x2, 0x2, 0x4f5, 0x4f6, 0x7, 0x95, 0x2, 
    0x2, 0x4f6, 0x4ff, 0x7, 0x9, 0x2, 0x2, 0x4f7, 0x4fc, 0x5, 0x84, 0x43, 
    0x2, 0x4f8, 0x4f9, 0x7, 0x5, 0x2, 0x2, 0x4f9, 0x4fb, 0x5, 0x84, 0x43, 
    0x2, 0x4fa, 0x4f8, 0x3, 0x2, 0x2, 0x2, 0x4fb, 0x4fe, 0x3, 0x2, 0x2, 
    0x2, 0x4fc, 0x4fa, 0x3, 0x2, 0x2, 0x2, 0x4fc, 0x4fd, 0x3, 0x2, 0x2, 
    0x2, 0x4fd, 0x500, 0x3, 0x2, 0x2, 0x2, 0x4fe, 0x4fc, 0x3, 0x2, 0x2, 
    0x2, 0x4ff, 0x4f7, 0x3, 0x2, 0x2, 0x2, 0x4ff, 0x500, 0x3, 0x2, 0x2, 
    0x2, 0x500, 0x501, 0x3, 0x2, 0x2, 0x2, 0x501, 0x5a6, 0x7, 0xa, 0x2, 
    0x2, 0x502, 0x503, 0x7, 0x93, 0x2, 0x2, 0x503, 0x50c, 0x7, 0x9, 0x2, 
    0x2, 0x504, 0x509, 0x5, 0x84, 0x43, 0x2, 0x505, 0x506, 0x7, 0x5, 0x2, 
    0x2, 0x506, 0x508, 0x5, 0x84, 0x43, 0x2, 0x507, 0x505, 0x3, 0x2, 0x2, 
    0x2, 0x508, 0x50b, 0x3, 0x2, 0x2, 0x2, 0x509, 0x507, 0x3, 0x2, 0x2, 
    0x2, 0x509, 0x50a, 0x3, 0x2, 0x2, 0x2, 0x50a, 0x50d, 0x3, 0x2, 0x2, 
    0x2, 0x50b, 0x509, 0x3, 0x2, 0x2, 0x2, 0x50c, 0x504, 0x3, 0x2, 0x2, 
    0x2, 0x50c, 0x50d, 0x3, 0x2, 0x2, 0x2, 0x50d, 0x50e, 0x3, 0x2, 0x2, 
    0x2, 0x50e, 0x5a6, 0x7, 0xa, 0x2, 0x2, 0x50f, 0x510, 0x7, 0x59, 0x2, 
    0x2, 0x510, 0x511, 0x7, 0x9, 0x2, 0x2, 0x511, 0x512, 0x5, 0x84, 0x43, 
    0x2, 0x512, 0x513, 0x9, 0x14, 0x2, 0x2, 0x513, 0x514, 0x5, 0x84, 0x43, 
    0x2, 0x514, 0x515, 0x9, 0x15, 0x2, 0x2, 0x515, 0x518, 0x5, 0x84, 0x43, 
    0x2, 0x516, 0x517, 0x9, 0x16, 0x2, 0x2, 0x517, 0x519, 0x5, 0x84, 0x43, 
    0x2, 0x518, 0x516, 0x3, 0x2, 0x2, 0x2, 0x518, 0x519, 0x3, 0x2, 0x2, 
    0x2, 0x519, 0x51a, 0x3, 0x2, 0x2, 0x2, 0x51a, 0x51b, 0x7, 0xa, 0x2, 
    0x2, 0x51b, 0x5a6, 0x3, 0x2, 0x2, 0x2, 0x51c, 0x51d, 0x7, 0x9b, 0x2, 
    0x2, 0x51d, 0x51e, 0x7, 0x9, 0x2, 0x2, 0x51e, 0x51f, 0x5, 0x84, 0x43, 
    0x2, 0x51f, 0x520, 0x9, 0x17, 0x2, 0x2, 0x520, 0x521, 0x5, 0x84, 0x43, 
    0x2, 0x521, 0x522, 0x7, 0xa, 0x2, 0x2, 0x522, 0x5a6, 0x3, 0x2, 0x2, 
    0x2, 0x523, 0x524, 0x9, 0x18, 0x2, 0x2, 0x524, 0x533, 0x7, 0x9, 0x2, 
    0x2, 0x525, 0x526, 0x5, 0x84, 0x43, 0x2, 0x526, 0x527, 0x7, 0x5, 0x2, 
    0x2, 0x527, 0x52a, 0x5, 0x84, 0x43, 0x2, 0x528, 0x529, 0x7, 0x5, 0x2, 
    0x2, 0x529, 0x52b, 0x5, 0x84, 0x43, 0x2, 0x52a, 0x528, 0x3, 0x2, 0x2, 
    0x2, 0x52a, 0x52b, 0x3, 0x2, 0x2, 0x2, 0x52b, 0x534, 0x3, 0x2, 0x2, 
    0x2, 0x52c, 0x52d, 0x5, 0x84, 0x43, 0x2, 0x52d, 0x52e, 0x7, 0x3a, 0x2, 
    0x2, 0x52e, 0x531, 0x5, 0x84, 0x43, 0x2, 0x52f, 0x530, 0x7, 0x37, 0x2, 
    0x2, 0x530, 0x532, 0x5, 0x84, 0x43, 0x2, 0x531, 0x52f, 0x3, 0x2, 0x2, 
    0x2, 0x531, 0x532, 0x3, 0x2, 0x2, 0x2, 0x532, 0x534, 0x3, 0x2, 0x2, 
    0x2, 0x533, 0x525, 0x3, 0x2, 0x2, 0x2, 0x533, 0x52c, 0x3, 0x2, 0x2, 
    0x2, 0x534, 0x535, 0x3, 0x2, 0x2, 0x2, 0x535, 0x536, 0x7, 0xa, 0x2, 
    0x2, 0x536, 0x5a6, 0x3, 0x2, 0x2, 0x2, 0x537, 0x538, 0x7, 0x9e, 0x2, 
    0x2, 0x538, 0x546, 0x7, 0x9, 0x2, 0x2, 0x539, 0x53c, 0x5, 0x84, 0x43, 
    0x2, 0x53a, 0x53b, 0x7, 0x5, 0x2, 0x2, 0x53b, 0x53d, 0x5, 0x84, 0x43, 
    0x2, 0x53c, 0x53a, 0x3, 0x2, 0x2, 0x2, 0x53c, 0x53d, 0x3, 0x2, 0x2, 
    0x2, 0x53d, 0x547, 0x3, 0x2, 0x2, 0x2, 0x53e, 0x540, 0x9, 0x19, 0x2, 
    0x2, 0x53f, 0x53e, 0x3, 0x2, 0x2, 0x2, 0x53f, 0x540, 0x3, 0x2, 0x2, 
    0x2, 0x540, 0x542, 0x3, 0x2, 0x2, 0x2, 0x541, 0x543, 0x5, 0x84, 0x43, 
    0x2, 0x542, 0x541, 0x3, 0x2, 0x2, 0x2, 0x542, 0x543, 0x3, 0x2, 0x2, 
    0x2, 0x543, 0x544, 0x3, 0x2, 0x2, 0x2, 0x544, 0x545, 0x7, 0x3a, 0x2, 
    0x2, 0x545, 0x547, 0x5, 0x84, 0x43, 0x2, 0x546, 0x539, 0x3, 0x2, 0x2, 
    0x2, 0x546, 0x53f, 0x3, 0x2, 0x2, 0x2, 0x547, 0x548, 0x3, 0x2, 0x2, 
    0x2, 0x548, 0x549, 0x7, 0xa, 0x2, 0x2, 0x549, 0x5a6, 0x3, 0x2, 0x2, 
    0x2, 0x54a, 0x54b, 0x7, 0x96, 0x2, 0x2, 0x54b, 0x54c, 0x7, 0x9, 0x2, 
    0x2, 0x54c, 0x54d, 0x9, 0x1a, 0x2, 0x2, 0x54d, 0x54e, 0x7, 0x3a, 0x2, 
    0x2, 0x54e, 0x54f, 0x5, 0x84, 0x43, 0x2, 0x54f, 0x550, 0x7, 0xa, 0x2, 
    0x2, 0x550, 0x5a6, 0x3, 0x2, 0x2, 0x2, 0x551, 0x552, 0x7, 0x94, 0x2, 
    0x2, 0x552, 0x554, 0x7, 0x9, 0x2, 0x2, 0x553, 0x555, 0x7, 0x17, 0x2, 
    0x2, 0x554, 0x553, 0x3, 0x2, 0x2, 0x2, 0x554, 0x555, 0x3, 0x2, 0x2, 
    0x2, 0x555, 0x556, 0x3, 0x2, 0x2, 0x2, 0x556, 0x557, 0x7, 0x7, 0x2, 
    0x2, 0x557, 0x5a6, 0x7, 0xa, 0x2, 0x2, 0x558, 0x559, 0x7, 0x94, 0x2, 
    0x2, 0x559, 0x55c, 0x7, 0x9, 0x2, 0x2, 0x55a, 0x55d, 0x7, 0x17, 0x2, 
    0x2, 0x55b, 0x55d, 0x7, 0x30, 0x2, 0x2, 0x55c, 0x55a, 0x3, 0x2, 0x2, 
    0x2, 0x55c, 0x55b, 0x3, 0x2, 0x2, 0x2, 0x55c, 0x55d, 0x3, 0x2, 0x2, 
    0x2, 0x55d, 0x55e, 0x3, 0x2, 0x2, 0x2, 0x55e, 0x55f, 0x5, 0x84, 0x43, 
    0x2, 0x55f, 0x560, 0x7, 0xa, 0x2, 0x2, 0x560, 0x5a6, 0x3, 0x2, 0x2, 
    0x2, 0x561, 0x562, 0x7, 0x69, 0x2, 0x2, 0x562, 0x565, 0x7, 0x9, 0x2, 
    0x2, 0x563, 0x566, 0x7, 0x17, 0x2, 0x2, 0x564, 0x566, 0x7, 0x30, 0x2, 
    0x2, 0x565, 0x563, 0x3, 0x2, 0x2, 0x2, 0x565, 0x564, 0x3, 0x2, 0x2, 
    0x2, 0x565, 0x566, 0x3, 0x2, 0x2, 0x2, 0x566, 0x567, 0x3, 0x2, 0x2, 
    0x2, 0x567, 0x568, 0x5, 0x84, 0x43, 0x2, 0x568, 0x569, 0x7, 0xa, 0x2, 
    0x2, 0x569, 0x5a6, 0x3, 0x2, 0x2, 0x2, 0x56a, 0x56b, 0x7, 0x1e, 0x2, 
    0x2, 0x56b, 0x56e, 0x7, 0x9, 0x2, 0x2, 0x56c, 0x56f, 0x7, 0x17, 0x2, 
    0x2, 0x56d, 0x56f, 0x7, 0x30, 0x2, 0x2, 0x56e, 0x56c, 0x3, 0x2, 0x2, 
    0x2, 0x56e, 0x56d, 0x3, 0x2, 0x2, 0x2, 0x56e, 0x56f, 0x3, 0x2, 0x2, 
    0x2, 0x56f, 0x570, 0x3, 0x2, 0x2, 0x2, 0x570, 0x571, 0x5, 0x84, 0x43, 
    0x2, 0x571, 0x572, 0x7, 0xa, 0x2, 0x2, 0x572, 0x5a6, 0x3, 0x2, 0x2, 
    0x2, 0x573, 0x574, 0x7, 0x4b, 0x2, 0x2, 0x574, 0x577, 0x7, 0x9, 0x2, 
    0x2, 0x575, 0x578, 0x7, 0x17, 0x2, 0x2, 0x576, 0x578, 0x7, 0x30, 0x2, 
    0x2, 0x577, 0x575, 0x3, 0x2, 0x2, 0x2, 0x577, 0x576, 0x3, 0x2, 0x2, 
    0x2, 0x577, 0x578, 0x3, 0x2, 0x2, 0x2, 0x578, 0x579, 0x3, 0x2, 0x2, 
    0x2, 0x579, 0x57a, 0x5, 0x84, 0x43, 0x2, 0x57a, 0x57b, 0x7, 0xa, 0x2, 
    0x2, 0x57b, 0x5a6, 0x3, 0x2, 0x2, 0x2, 0x57c, 0x57d, 0x7, 0x4a, 0x2, 
    0x2, 0x57d, 0x580, 0x7, 0x9, 0x2, 0x2, 0x57e, 0x581, 0x7, 0x17, 0x2, 
    0x2, 0x57f, 0x581, 0x7, 0x30, 0x2, 0x2, 0x580, 0x57e, 0x3, 0x2, 0x2, 
    0x2, 0x580, 0x57f, 0x3, 0x2, 0x2, 0x2, 0x580, 0x581, 0x3, 0x2, 0x2, 
    0x2, 0x581, 0x582, 0x3, 0x2, 0x2, 0x2, 0x582, 0x583, 0x5, 0x84, 0x43, 
    0x2, 0x583, 0x584, 0x7, 0xa, 0x2, 0x2, 0x584, 0x5a6, 0x3, 0x2, 0x2, 
    0x2, 0x585, 0x586, 0x5, 0x8e, 0x48, 0x2, 0x586, 0x58f, 0x7, 0x9, 0x2, 
    0x2, 0x587, 0x58c, 0x5, 0x84, 0x43, 0x2, 0x588, 0x589, 0x7, 0x5, 0x2, 
    0x2, 0x589, 0x58b, 0x5, 0x84, 0x43, 0x2, 0x58a, 0x588, 0x3, 0x2, 0x2, 
    0x2, 0x58b, 0x58e, 0x3, 0x2, 0x2, 0x2, 0x58c, 0x58a, 0x3, 0x2, 0x2, 
    0x2, 0x58c, 0x58d, 0x3, 0x2, 0x2, 0x2, 0x58d, 0x590, 0x3, 0x2, 0x2, 
    0x2, 0x58e, 0x58c, 0x3, 0x2, 0x2, 0x2, 0x58f, 0x587, 0x3, 0x2, 0x2, 
    0x2, 0x58f, 0x590, 0x3, 0x2, 0x2, 0x2, 0x590, 0x591, 0x3, 0x2, 0x2, 
    0x2, 0x591, 0x592, 0x7, 0xa, 0x2, 0x2, 0x592, 0x5a6, 0x3, 0x2, 0x2, 
    0x2, 0x593, 0x594, 0x7, 0x9, 0x2, 0x2, 0x594, 0x595, 0x5, 0x84, 0x43, 
    0x2, 0x595, 0x596, 0x7, 0xa, 0x2, 0x2, 0x596, 0x5a6, 0x3, 0x2, 0x2, 
    0x2, 0x597, 0x598, 0x7, 0x9, 0x2, 0x2, 0x598, 0x599, 0x5, 0x16, 0xc, 
    0x2, 0x599, 0x59a, 0x7, 0xa, 0x2, 0x2, 0x59a, 0x5a6, 0x3, 0x2, 0x2, 
    0x2, 0x59b, 0x59d, 0x7, 0x50, 0x2, 0x2, 0x59c, 0x59b, 0x3, 0x2, 0x2, 
    0x2, 0x59c, 0x59d, 0x3, 0x2, 0x2, 0x2, 0x59d, 0x59e, 0x3, 0x2, 0x2, 
    0x2, 0x59e, 0x59f, 0x7, 0x97, 0x2, 0x2, 0x59f, 0x5a0, 0x7, 0x9, 0x2, 
    0x2, 0x5a0, 0x5a1, 0x5, 0x16, 0xc, 0x2, 0x5a1, 0x5a2, 0x7, 0xa, 0x2, 
    0x2, 0x5a2, 0x5a6, 0x3, 0x2, 0x2, 0x2, 0x5a3, 0x5a4, 0x7, 0x50, 0x2, 
    0x2, 0x5a4, 0x5a6, 0x5, 0x84, 0x43, 0x6, 0x5a5, 0x49e, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x4a0, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x4a1, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x4a2, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x4a7, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x4a9, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x4b0, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x4b5, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x4d1, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x4da, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x4e1, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x4e8, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x4f5, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x502, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x50f, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x51c, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x523, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x537, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x54a, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x551, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x558, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x561, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x56a, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x573, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x57c, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x585, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x593, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x597, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x59c, 0x3, 0x2, 0x2, 
    0x2, 0x5a5, 0x5a3, 0x3, 0x2, 0x2, 0x2, 0x5a6, 0x604, 0x3, 0x2, 0x2, 
    0x2, 0x5a7, 0x5a8, 0xc, 0x27, 0x2, 0x2, 0x5a8, 0x5a9, 0x9, 0x1b, 0x2, 
    0x2, 0x5a9, 0x603, 0x5, 0x84, 0x43, 0x28, 0x5aa, 0x5ab, 0xc, 0x26, 0x2, 
    0x2, 0x5ab, 0x5ac, 0x9, 0x1c, 0x2, 0x2, 0x5ac, 0x603, 0x5, 0x84, 0x43, 
    0x27, 0x5ad, 0x5ae, 0xc, 0x25, 0x2, 0x2, 0x5ae, 0x5af, 0x9, 0x1d, 0x2, 
    0x2, 0x5af, 0x603, 0x5, 0x84, 0x43, 0x26, 0x5b0, 0x5b2, 0xc, 0xc, 0x2, 
    0x2, 0x5b1, 0x5b3, 0x7, 0x50, 0x2, 0x2, 0x5b2, 0x5b1, 0x3, 0x2, 0x2, 
    0x2, 0x5b2, 0x5b3, 0x3, 0x2, 0x2, 0x2, 0x5b3, 0x5b4, 0x3, 0x2, 0x2, 
    0x2, 0x5b4, 0x5b5, 0x7, 0x48, 0x2, 0x2, 0x5b5, 0x603, 0x5, 0x84, 0x43, 
    0xd, 0x5b6, 0x5b8, 0xc, 0xb, 0x2, 0x2, 0x5b7, 0x5b9, 0x7, 0x50, 0x2, 
    0x2, 0x5b8, 0x5b7, 0x3, 0x2, 0x2, 0x2, 0x5b8, 0x5b9, 0x3, 0x2, 0x2, 
    0x2, 0x5b9, 0x5ba, 0x3, 0x2, 0x2, 0x2, 0x5ba, 0x5bb, 0x7, 0x48, 0x2, 
    0x2, 0x5bb, 0x5bc, 0x5, 0x84, 0x43, 0x2, 0x5bc, 0x5bd, 0x7, 0x32, 0x2, 
    0x2, 0x5bd, 0x5be, 0x5, 0x84, 0x43, 0xc, 0x5be, 0x603, 0x3, 0x2, 0x2, 
    0x2, 0x5bf, 0x5c0, 0xc, 0x5, 0x2, 0x2, 0x5c0, 0x5c1, 0x6, 0x43, 0xd, 
    0x2, 0x5c1, 0x5c2, 0x7, 0x18, 0x2, 0x2, 0x5c2, 0x603, 0x5, 0x84, 0x43, 
    0x6, 0x5c3, 0x5c4, 0xc, 0x4, 0x2, 0x2, 0x5c4, 0x5c5, 0x7, 0x55, 0x2, 
    0x2, 0x5c5, 0x603, 0x5, 0x84, 0x43, 0x5, 0x5c6, 0x5c7, 0xc, 0x3, 0x2, 
    0x2, 0x5c7, 0x5c8, 0x7, 0x7a, 0x2, 0x2, 0x5c8, 0x603, 0x5, 0x84, 0x43, 
    0x4, 0x5c9, 0x5ca, 0xc, 0x28, 0x2, 0x2, 0x5ca, 0x5cb, 0x7, 0xc, 0x2, 
    0x2, 0x5cb, 0x5cc, 0x5, 0x90, 0x49, 0x2, 0x5cc, 0x5d5, 0x7, 0x9, 0x2, 
    0x2, 0x5cd, 0x5d2, 0x5, 0x84, 0x43, 0x2, 0x5ce, 0x5cf, 0x7, 0x5, 0x2, 
    0x2, 0x5cf, 0x5d1, 0x5, 0x84, 0x43, 0x2, 0x5d0, 0x5ce, 0x3, 0x2, 0x2, 
    0x2, 0x5d1, 0x5d4, 0x3, 0x2, 0x2, 0x2, 0x5d2, 0x5d0, 0x3, 0x2, 0x2, 
    0x2, 0x5d2, 0x5d3, 0x3, 0x2, 0x2, 0x2, 0x5d3, 0x5d6, 0x3, 0x2, 0x2, 
    0x2, 0x5d4, 0x5d2, 0x3, 0x2, 0x2, 0x2, 0x5d5, 0x5cd, 0x3, 0x2, 0x2, 
    0x2, 0x5d5, 0x5d6, 0x3, 0x2, 0x2, 0x2, 0x5d6, 0x5d7, 0x3, 0x2, 0x2, 
    0x2, 0x5d7, 0x5d8, 0x7, 0xa, 0x2, 0x2, 0x5d8, 0x603, 0x3, 0x2, 0x2, 
    0x2, 0x5d9, 0x5da, 0xc, 0xd, 0x2, 0x2, 0x5da, 0x5dc, 0x7, 0x43, 0x2, 
    0x2, 0x5db, 0x5dd, 0x7, 0x50, 0x2, 0x2, 0x5dc, 0x5db, 0x3, 0x2, 0x2, 
    0x2, 0x5dc, 0x5dd, 0x3, 0x2, 0x2, 0x2, 0x5dd, 0x5de, 0x3, 0x2, 0x2, 
    0x2, 0x5de, 0x603, 0x7, 0x52, 0x2, 0x2, 0x5df, 0x5e1, 0xc, 0x9, 0x2, 
    0x2, 0x5e0, 0x5e2, 0x7, 0x50, 0x2, 0x2, 0x5e1, 0x5e0, 0x3, 0x2, 0x2, 
    0x2, 0x5e1, 0x5e2, 0x3, 0x2, 0x2, 0x2, 0x5e2, 0x5e3, 0x3, 0x2, 0x2, 
    0x2, 0x5e3, 0x5e4, 0x7, 0x3d, 0x2, 0x2, 0x5e4, 0x5e5, 0x7, 0x9, 0x2, 
    0x2, 0x5e5, 0x5e6, 0x5, 0x16, 0xc, 0x2, 0x5e6, 0x5e7, 0x7, 0xa, 0x2, 
    0x2, 0x5e7, 0x603, 0x3, 0x2, 0x2, 0x2, 0x5e8, 0x5ea, 0xc, 0x8, 0x2, 
    0x2, 0x5e9, 0x5eb, 0x7, 0x50, 0x2, 0x2, 0x5ea, 0x5e9, 0x3, 0x2, 0x2, 
    0x2, 0x5ea, 0x5eb, 0x3, 0x2, 0x2, 0x2, 0x5eb, 0x5ec, 0x3, 0x2, 0x2, 
    0x2, 0x5ec, 0x5ed, 0x7, 0x3d, 0x2, 0x2, 0x5ed, 0x5f6, 0x7, 0x9, 0x2, 
    0x2, 0x5ee, 0x5f3, 0x5, 0x84, 0x43, 0x2, 0x5ef, 0x5f0, 0x7, 0x5, 0x2, 
    0x2, 0x5f0, 0x5f2, 0x5, 0x84, 0x43, 0x2, 0x5f1, 0x5ef, 0x3, 0x2, 0x2, 
    0x2, 0x5f2, 0x5f5, 0x3, 0x2, 0x2, 0x2, 0x5f3, 0x5f1, 0x3, 0x2, 0x2, 
    0x2, 0x5f3, 0x5f4, 0x3, 0x2, 0x2, 0x2, 0x5f4, 0x5f7, 0x3, 0x2, 0x2, 
    0x2, 0x5f5, 0x5f3, 0x3, 0x2, 0x2, 0x2, 0x5f6, 0x5ee, 0x3, 0x2, 0x2, 
    0x2, 0x5f6, 0x5f7, 0x3, 0x2, 0x2, 0x2, 0x5f7, 0x5f8, 0x3, 0x2, 0x2, 
    0x2, 0x5f8, 0x603, 0x7, 0xa, 0x2, 0x2, 0x5f9, 0x5fb, 0xc, 0x7, 0x2, 
    0x2, 0x5fa, 0x5fc, 0x7, 0x50, 0x2, 0x2, 0x5fb, 0x5fa, 0x3, 0x2, 0x2, 
    0x2, 0x5fb, 0x5fc, 0x3, 0x2, 0x2, 0x2, 0x5fc, 0x5fd, 0x3, 0x2, 0x2, 
    0x2, 0x5fd, 0x5fe, 0x7, 0x1f, 0x2, 0x2, 0x5fe, 0x5ff, 0x5, 0x84, 0x43, 
    0x2, 0x5ff, 0x600, 0x7, 0x18, 0x2, 0x2, 0x600, 0x601, 0x5, 0x84, 0x43, 
    0x2, 0x601, 0x603, 0x3, 0x2, 0x2, 0x2, 0x602, 0x5a7, 0x3, 0x2, 0x2, 
    0x2, 0x602, 0x5aa, 0x3, 0x2, 0x2, 0x2, 0x602, 0x5ad, 0x3, 0x2, 0x2, 
    0x2, 0x602, 0x5b0, 0x3, 0x2, 0x2, 0x2, 0x602, 0x5b6, 0x3, 0x2, 0x2, 
    0x2, 0x602, 0x5bf, 0x3, 0x2, 0x2, 0x2, 0x602, 0x5c3, 0x3, 0x2, 0x2, 
    0x2, 0x602, 0x5c6, 0x3, 0x2, 0x2, 0x2, 0x602, 0x5c9, 0x3, 0x2, 0x2, 
    0x2, 0x602, 0x5d9, 0x3, 0x2, 0x2, 0x2, 0x602, 0x5df, 0x3, 0x2, 0x2, 
    0x2, 0x602, 0x5e8, 0x3, 0x2, 0x2, 0x2, 0x602, 0x5f9, 0x3, 0x2, 0x2, 
    0x2, 0x603, 0x606, 0x3, 0x2, 0x2, 0x2, 0x604, 0x602, 0x3, 0x2, 0x2, 
    0x2, 0x604, 0x605, 0x3, 0x2, 0x2, 0x2, 0x605, 0x85, 0x3, 0x2, 0x2, 0x2, 
    0x606, 0x604, 0x3, 0x2, 0x2, 0x2, 0x607, 0x608, 0x9, 0x1e, 0x2, 0x2, 
    0x608, 0x87, 0x3, 0x2, 0x2, 0x2, 0x609, 0x60a, 0x5, 0x92, 0x4a, 0x2, 
    0x60a, 0x60b, 0x7, 0xc, 0x2, 0x2, 0x60b, 0x60d, 0x3, 0x2, 0x2, 0x2, 
    0x60c, 0x609, 0x3, 0x2, 0x2, 0x2, 0x60c, 0x60d, 0x3, 0x2, 0x2, 0x2, 
    0x60d, 0x60e, 0x3, 0x2, 0x2, 0x2, 0x60e, 0x60f, 0x5, 0x94, 0x4b, 0x2, 
    0x60f, 0x610, 0x7, 0xc, 0x2, 0x2, 0x610, 0x612, 0x3, 0x2, 0x2, 0x2, 
    0x611, 0x60c, 0x3, 0x2, 0x2, 0x2, 0x611, 0x612, 0x3, 0x2, 0x2, 0x2, 
    0x612, 0x613, 0x3, 0x2, 0x2, 0x2, 0x613, 0x614, 0x5, 0x96, 0x4c, 0x2, 
    0x614, 0x615, 0x7, 0xc, 0x2, 0x2, 0x615, 0x617, 0x3, 0x2, 0x2, 0x2, 
    0x616, 0x611, 0x3, 0x2, 0x2, 0x2, 0x616, 0x617, 0x3, 0x2, 0x2, 0x2, 
    0x617, 0x618, 0x3, 0x2, 0x2, 0x2, 0x618, 0x619, 0x5, 0x9a, 0x4e, 0x2, 
    0x619, 0x89, 0x3, 0x2, 0x2, 0x2, 0x61a, 0x61b, 0x5, 0x92, 0x4a, 0x2, 
    0x61b, 0x61c, 0x7, 0xc, 0x2, 0x2, 0x61c, 0x61e, 0x3, 0x2, 0x2, 0x2, 
    0x61d, 0x61a, 0x3, 0x2, 0x2, 0x2, 0x61d, 0x61e, 0x3, 0x2, 0x2, 0x2, 
    0x61e, 0x61f, 0x3, 0x2, 0x2, 0x2, 0x61f, 0x620, 0x5, 0x94, 0x4b, 0x2, 
    0x620, 0x621, 0x7, 0xc, 0x2, 0x2, 0x621, 0x623, 0x3, 0x2, 0x2, 0x2, 
    0x622, 0x61d, 0x3, 0x2, 0x2, 0x2, 0x622, 0x623, 0x3, 0x2, 0x2, 0x2, 
    0x623, 0x624, 0x3, 0x2, 0x2, 0x2, 0x624, 0x625, 0x5, 0x96, 0x4c, 0x2, 
    0x625, 0x626, 0x7, 0xc, 0x2, 0x2, 0x626, 0x628, 0x3, 0x2, 0x2, 0x2, 
    0x627, 0x622, 0x3, 0x2, 0x2, 0x2, 0x627, 0x628, 0x3, 0x2, 0x2, 0x2, 
    0x628, 0x629, 0x3, 0x2, 0x2, 0x2, 0x629, 0x62a, 0x7, 0x7, 0x2, 0x2, 
    0x62a, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x62b, 0x62c, 0x5, 0xac, 0x57, 0x2, 
    0x62c, 0x62d, 0x7, 0xc, 0x2, 0x2, 0x62d, 0x62f, 0x3, 0x2, 0x2, 0x2, 
    0x62e, 0x62b, 0x3, 0x2, 0x2, 0x2, 0x62f, 0x632, 0x3, 0x2, 0x2, 0x2, 
    0x630, 0x62e, 0x3, 0x2, 0x2, 0x2, 0x630, 0x631, 0x3, 0x2, 0x2, 0x2, 
    0x631, 0x633, 0x3, 0x2, 0x2, 0x2, 0x632, 0x630, 0x3, 0x2, 0x2, 0x2, 
    0x633, 0x634, 0x5, 0xaa, 0x56, 0x2, 0x634, 0x8d, 0x3, 0x2, 0x2, 0x2, 
    0x635, 0x636, 0x5, 0xaa, 0x56, 0x2, 0x636, 0x8f, 0x3, 0x2, 0x2, 0x2, 
    0x637, 0x638, 0x5, 0xaa, 0x56, 0x2, 0x638, 0x91, 0x3, 0x2, 0x2, 0x2, 
    0x639, 0x63a, 0x5, 0xaa, 0x56, 0x2, 0x63a, 0x93, 0x3, 0x2, 0x2, 0x2, 
    0x63b, 0x63c, 0x5, 0xaa, 0x56, 0x2, 0x63c, 0x95, 0x3, 0x2, 0x2, 0x2, 
    0x63d, 0x63e, 0x5, 0xaa, 0x56, 0x2, 0x63e, 0x97, 0x3, 0x2, 0x2, 0x2, 
    0x63f, 0x640, 0x5, 0xaa, 0x56, 0x2, 0x640, 0x99, 0x3, 0x2, 0x2, 0x2, 
    0x641, 0x642, 0x5, 0xaa, 0x56, 0x2, 0x642, 0x9b, 0x3, 0x2, 0x2, 0x2, 
    0x643, 0x644, 0x5, 0xaa, 0x56, 0x2, 0x644, 0x9d, 0x3, 0x2, 0x2, 0x2, 
    0x645, 0x646, 0x5, 0xaa, 0x56, 0x2, 0x646, 0x9f, 0x3, 0x2, 0x2, 0x2, 
    0x647, 0x648, 0x5, 0xaa, 0x56, 0x2, 0x648, 0xa1, 0x3, 0x2, 0x2, 0x2, 
    0x649, 0x64a, 0x5, 0xaa, 0x56, 0x2, 0x64a, 0xa3, 0x3, 0x2, 0x2, 0x2, 
    0x64b, 0x64c, 0x5, 0xaa, 0x56, 0x2, 0x64c, 0xa5, 0x3, 0x2, 0x2, 0x2, 
    0x64d, 0x650, 0x5, 0xb6, 0x5c, 0x2, 0x64e, 0x650, 0x5, 0xaa, 0x56, 0x2, 
    0x64f, 0x64d, 0x3, 0x2, 0x2, 0x2, 0x64f, 0x64e, 0x3, 0x2, 0x2, 0x2, 
    0x650, 0xa7, 0x3, 0x2, 0x2, 0x2, 0x651, 0x654, 0x5, 0xb6, 0x5c, 0x2, 
    0x652, 0x654, 0x5, 0xaa, 0x56, 0x2, 0x653, 0x651, 0x3, 0x2, 0x2, 0x2, 
    0x653, 0x652, 0x3, 0x2, 0x2, 0x2, 0x654, 0xa9, 0x3, 0x2, 0x2, 0x2, 0x655, 
    0x658, 0x7, 0xbc, 0x2, 0x2, 0x656, 0x658, 0x5, 0xd6, 0x6c, 0x2, 0x657, 
    0x655, 0x3, 0x2, 0x2, 0x2, 0x657, 0x656, 0x3, 0x2, 0x2, 0x2, 0x658, 
    0xab, 0x3, 0x2, 0x2, 0x2, 0x659, 0x65a, 0x5, 0xaa, 0x56, 0x2, 0x65a, 
    0xad, 0x3, 0x2, 0x2, 0x2, 0x65b, 0x65c, 0x5, 0xba, 0x5e, 0x2, 0x65c, 
    0xaf, 0x3, 0x2, 0x2, 0x2, 0x65d, 0x65e, 0x5, 0xba, 0x5e, 0x2, 0x65e, 
    0xb1, 0x3, 0x2, 0x2, 0x2, 0x65f, 0x660, 0x5, 0xba, 0x5e, 0x2, 0x660, 
    0xb3, 0x3, 0x2, 0x2, 0x2, 0x661, 0x66c, 0x7, 0x52, 0x2, 0x2, 0x662, 
    0x66c, 0x5, 0xb8, 0x5d, 0x2, 0x663, 0x66c, 0x5, 0xba, 0x5e, 0x2, 0x664, 
    0x66c, 0x5, 0xbe, 0x60, 0x2, 0x665, 0x66c, 0x5, 0xc2, 0x62, 0x2, 0x666, 
    0x66c, 0x5, 0xc6, 0x64, 0x2, 0x667, 0x66c, 0x5, 0xc8, 0x65, 0x2, 0x668, 
    0x66c, 0x5, 0xcc, 0x67, 0x2, 0x669, 0x66c, 0x5, 0xca, 0x66, 0x2, 0x66a, 
    0x66c, 0x5, 0xce, 0x68, 0x2, 0x66b, 0x661, 0x3, 0x2, 0x2, 0x2, 0x66b, 
    0x662, 0x3, 0x2, 0x2, 0x2, 0x66b, 0x663, 0x3, 0x2, 0x2, 0x2, 0x66b, 
    0x664, 0x3, 0x2, 0x2, 0x2, 0x66b, 0x665, 0x3, 0x2, 0x2, 0x2, 0x66b, 
    0x666, 0x3, 0x2, 0x2, 0x2, 0x66b, 0x667, 0x3, 0x2, 0x2, 0x2, 0x66b, 
    0x668, 0x3, 0x2, 0x2, 0x2, 0x66b, 0x669, 0x3, 0x2, 0x2, 0x2, 0x66b, 
    0x66a, 0x3, 0x2, 0x2, 0x2, 0x66c, 0xb5, 0x3, 0x2, 0x2, 0x2, 0x66d, 0x674, 
    0x7, 0x52, 0x2, 0x2, 0x66e, 0x674, 0x5, 0xb8, 0x5d, 0x2, 0x66f, 0x674, 
    0x5, 0xbc, 0x5f, 0x2, 0x670, 0x674, 0x5, 0xc0, 0x61, 0x2, 0x671, 0x674, 
    0x5, 0xc4, 0x63, 0x2, 0x672, 0x674, 0x5, 0xcc, 0x67, 0x2, 0x673, 0x66d, 
    0x3, 0x2, 0x2, 0x2, 0x673, 0x66e, 0x3, 0x2, 0x2, 0x2, 0x673, 0x66f, 
    0x3, 0x2, 0x2, 0x2, 0x673, 0x670, 0x3, 0x2, 0x2, 0x2, 0x673, 0x671, 
    0x3, 0x2, 0x2, 0x2, 0x673, 0x672, 0x3, 0x2, 0x2, 0x2, 0x674, 0xb7, 0x3, 
    0x2, 0x2, 0x2, 0x675, 0x676, 0x9, 0x1f, 0x2, 0x2, 0x676, 0xb9, 0x3, 
    0x2, 0x2, 0x2, 0x677, 0x678, 0x7, 0xc5, 0x2, 0x2, 0x678, 0xbb, 0x3, 
    0x2, 0x2, 0x2, 0x679, 0x67c, 0x7, 0xd, 0x2, 0x2, 0x67a, 0x67c, 0x7, 
    0x6, 0x2, 0x2, 0x67b, 0x679, 0x3, 0x2, 0x2, 0x2, 0x67b, 0x67a, 0x3, 
    0x2, 0x2, 0x2, 0x67b, 0x67c, 0x3, 0x2, 0x2, 0x2, 0x67c, 0x67d, 0x3, 
    0x2, 0x2, 0x2, 0x67d, 0x67e, 0x7, 0xc5, 0x2, 0x2, 0x67e, 0xbd, 0x3, 
    0x2, 0x2, 0x2, 0x67f, 0x680, 0x7, 0xc4, 0x2, 0x2, 0x680, 0xbf, 0x3, 
    0x2, 0x2, 0x2, 0x681, 0x684, 0x7, 0xd, 0x2, 0x2, 0x682, 0x684, 0x7, 
    0x6, 0x2, 0x2, 0x683, 0x681, 0x3, 0x2, 0x2, 0x2, 0x683, 0x682, 0x3, 
    0x2, 0x2, 0x2, 0x683, 0x684, 0x3, 0x2, 0x2, 0x2, 0x684, 0x685, 0x3, 
    0x2, 0x2, 0x2, 0x685, 0x686, 0x7, 0xc4, 0x2, 0x2, 0x686, 0xc1, 0x3, 
    0x2, 0x2, 0x2, 0x687, 0x688, 0x7, 0xc3, 0x2, 0x2, 0x688, 0xc3, 0x3, 
    0x2, 0x2, 0x2, 0x689, 0x68c, 0x7, 0xd, 0x2, 0x2, 0x68a, 0x68c, 0x7, 
    0x6, 0x2, 0x2, 0x68b, 0x689, 0x3, 0x2, 0x2, 0x2, 0x68b, 0x68a, 0x3, 
    0x2, 0x2, 0x2, 0x68b, 0x68c, 0x3, 0x2, 0x2, 0x2, 0x68c, 0x68d, 0x3, 
    0x2, 0x2, 0x2, 0x68d, 0x68e, 0x7, 0xc3, 0x2, 0x2, 0x68e, 0xc5, 0x3, 
    0x2, 0x2, 0x2, 0x68f, 0x690, 0x7, 0x2c, 0x2, 0x2, 0x690, 0x691, 0x5, 
    0xcc, 0x67, 0x2, 0x691, 0xc7, 0x3, 0x2, 0x2, 0x2, 0x692, 0x693, 0x7, 
    0x7f, 0x2, 0x2, 0x693, 0x694, 0x5, 0xcc, 0x67, 0x2, 0x694, 0xc9, 0x3, 
    0x2, 0x2, 0x2, 0x695, 0x696, 0x7, 0x80, 0x2, 0x2, 0x696, 0x697, 0x5, 
    0xcc, 0x67, 0x2, 0x697, 0xcb, 0x3, 0x2, 0x2, 0x2, 0x698, 0x699, 0x7, 
    0xc0, 0x2, 0x2, 0x699, 0xcd, 0x3, 0x2, 0x2, 0x2, 0x69a, 0x69e, 0x5, 
    0xd2, 0x6a, 0x2, 0x69b, 0x69e, 0x5, 0xd4, 0x6b, 0x2, 0x69c, 0x69e, 0x5, 
    0xd0, 0x69, 0x2, 0x69d, 0x69a, 0x3, 0x2, 0x2, 0x2, 0x69d, 0x69b, 0x3, 
    0x2, 0x2, 0x2, 0x69d, 0x69c, 0x3, 0x2, 0x2, 0x2, 0x69e, 0xcf, 0x3, 0x2, 
    0x2, 0x2, 0x69f, 0x6a0, 0x7, 0x81, 0x2, 0x2, 0x6a0, 0x6a1, 0x5, 0x84, 
    0x43, 0x2, 0x6a1, 0xd1, 0x3, 0x2, 0x2, 0x2, 0x6a2, 0x6a3, 0x7, 0x81, 
    0x2, 0x2, 0x6a3, 0x6bb, 0x5, 0xcc, 0x67, 0x2, 0x6a4, 0x6a9, 0x7, 0xb2, 
    0x2, 0x2, 0x6a5, 0x6a6, 0x7, 0x9, 0x2, 0x2, 0x6a6, 0x6a7, 0x5, 0xae, 
    0x58, 0x2, 0x6a7, 0x6a8, 0x7, 0xa, 0x2, 0x2, 0x6a8, 0x6aa, 0x3, 0x2, 
    0x2, 0x2, 0x6a9, 0x6a5, 0x3, 0x2, 0x2, 0x2, 0x6a9, 0x6aa, 0x3, 0x2, 
    0x2, 0x2, 0x6aa, 0x6bc, 0x3, 0x2, 0x2, 0x2, 0x6ab, 0x6b0, 0x7, 0xb2, 
    0x2, 0x2, 0x6ac, 0x6ad, 0x7, 0x9, 0x2, 0x2, 0x6ad, 0x6ae, 0x5, 0xae, 
    0x58, 0x2, 0x6ae, 0x6af, 0x7, 0xa, 0x2, 0x2, 0x6af, 0x6b1, 0x3, 0x2, 
    0x2, 0x2, 0x6b0, 0x6ac, 0x3, 0x2, 0x2, 0x2, 0x6b0, 0x6b1, 0x3, 0x2, 
    0x2, 0x2, 0x6b1, 0x6b2, 0x3, 0x2, 0x2, 0x2, 0x6b2, 0x6b3, 0x7, 0x6b, 
    0x2, 0x2, 0x6b3, 0x6bc, 0x7, 0xaa, 0x2, 0x2, 0x6b4, 0x6b9, 0x7, 0xaa, 
    0x2, 0x2, 0x6b5, 0x6b6, 0x7, 0x9, 0x2, 0x2, 0x6b6, 0x6b7, 0x5, 0xae, 
    0x58, 0x2, 0x6b7, 0x6b8, 0x7, 0xa, 0x2, 0x2, 0x6b8, 0x6ba, 0x3, 0x2, 
    0x2, 0x2, 0x6b9, 0x6b5, 0x3, 0x2, 0x2, 0x2, 0x6b9, 0x6ba, 0x3, 0x2, 
    0x2, 0x2, 0x6ba, 0x6bc, 0x3, 0x2, 0x2, 0x2, 0x6bb, 0x6a4, 0x3, 0x2, 
    0x2, 0x2, 0x6bb, 0x6ab, 0x3, 0x2, 0x2, 0x2, 0x6bb, 0x6b4, 0x3, 0x2, 
    0x2, 0x2, 0x6bc, 0xd3, 0x3, 0x2, 0x2, 0x2, 0x6bd, 0x6be, 0x7, 0x81, 
    0x2, 0x2, 0x6be, 0x713, 0x5, 0xcc, 0x67, 0x2, 0x6bf, 0x6c4, 0x7, 0xa4, 
    0x2, 0x2, 0x6c0, 0x6c1, 0x7, 0x9, 0x2, 0x2, 0x6c1, 0x6c2, 0x5, 0xae, 
    0x58, 0x2, 0x6c2, 0x6c3, 0x7, 0xa, 0x2, 0x2, 0x6c3, 0x6c5, 0x3, 0x2, 
    0x2, 0x2, 0x6c4, 0x6c0, 0x3, 0x2, 0x2, 0x2, 0x6c4, 0x6c5, 0x3, 0x2, 
    0x2, 0x2, 0x6c5, 0x714, 0x3, 0x2, 0x2, 0x2, 0x6c6, 0x6cb, 0x7, 0xa4, 
    0x2, 0x2, 0x6c7, 0x6c8, 0x7, 0x9, 0x2, 0x2, 0x6c8, 0x6c9, 0x5, 0xae, 
    0x58, 0x2, 0x6c9, 0x6ca, 0x7, 0xa, 0x2, 0x2, 0x6ca, 0x6cc, 0x3, 0x2, 
    0x2, 0x2, 0x6cb, 0x6c7, 0x3, 0x2, 0x2, 0x2, 0x6cb, 0x6cc, 0x3, 0x2, 
    0x2, 0x2, 0x6cc, 0x6cd, 0x3, 0x2, 0x2, 0x2, 0x6cd, 0x6d7, 0x7, 0x6b, 
    0x2, 0x2, 0x6ce, 0x6d8, 0x7, 0xa6, 0x2, 0x2, 0x6cf, 0x6d8, 0x7, 0xa8, 
    0x2, 0x2, 0x6d0, 0x6d5, 0x7, 0xae, 0x2, 0x2, 0x6d1, 0x6d2, 0x7, 0x9, 
    0x2, 0x2, 0x6d2, 0x6d3, 0x5, 0xb0, 0x59, 0x2, 0x6d3, 0x6d4, 0x7, 0xa, 
    0x2, 0x2, 0x6d4, 0x6d6, 0x3, 0x2, 0x2, 0x2, 0x6d5, 0x6d1, 0x3, 0x2, 
    0x2, 0x2, 0x6d5, 0x6d6, 0x3, 0x2, 0x2, 0x2, 0x6d6, 0x6d8, 0x3, 0x2, 
    0x2, 0x2, 0x6d7, 0x6ce, 0x3, 0x2, 0x2, 0x2, 0x6d7, 0x6cf, 0x3, 0x2, 
    0x2, 0x2, 0x6d7, 0x6d0, 0x3, 0x2, 0x2, 0x2, 0x6d8, 0x714, 0x3, 0x2, 
    0x2, 0x2, 0x6d9, 0x6de, 0x7, 0xa6, 0x2, 0x2, 0x6da, 0x6db, 0x7, 0x9, 
    0x2, 0x2, 0x6db, 0x6dc, 0x5, 0xae, 0x58, 0x2, 0x6dc, 0x6dd, 0x7, 0xa, 
    0x2, 0x2, 0x6dd, 0x6df, 0x3, 0x2, 0x2, 0x2, 0x6de, 0x6da, 0x3, 0x2, 
    0x2, 0x2, 0x6de, 0x6df, 0x3, 0x2, 0x2, 0x2, 0x6df, 0x6e0, 0x3, 0x2, 
    0x2, 0x2, 0x6e0, 0x6e9, 0x7, 0x6b, 0x2, 0x2, 0x6e1, 0x6ea, 0x7, 0xa8, 
    0x2, 0x2, 0x6e2, 0x6e7, 0x7, 0xae, 0x2, 0x2, 0x6e3, 0x6e4, 0x7, 0x9, 
    0x2, 0x2, 0x6e4, 0x6e5, 0x5, 0xb0, 0x59, 0x2, 0x6e5, 0x6e6, 0x7, 0xa, 
    0x2, 0x2, 0x6e6, 0x6e8, 0x3, 0x2, 0x2, 0x2, 0x6e7, 0x6e3, 0x3, 0x2, 
    0x2, 0x2, 0x6e7, 0x6e8, 0x3, 0x2, 0x2, 0x2, 0x6e8, 0x6ea, 0x3, 0x2, 
    0x2, 0x2, 0x6e9, 0x6e1, 0x3, 0x2, 0x2, 0x2, 0x6e9, 0x6e2, 0x3, 0x2, 
    0x2, 0x2, 0x6ea, 0x714, 0x3, 0x2, 0x2, 0x2, 0x6eb, 0x6f0, 0x7, 0xa8, 
    0x2, 0x2, 0x6ec, 0x6ed, 0x7, 0x9, 0x2, 0x2, 0x6ed, 0x6ee, 0x5, 0xae, 
    0x58, 0x2, 0x6ee, 0x6ef, 0x7, 0xa, 0x2, 0x2, 0x6ef, 0x6f1, 0x3, 0x2, 
    0x2, 0x2, 0x6f0, 0x6ec, 0x3, 0x2, 0x2, 0x2, 0x6f0, 0x6f1, 0x3, 0x2, 
    0x2, 0x2, 0x6f1, 0x6f2, 0x3, 0x2, 0x2, 0x2, 0x6f2, 0x6f3, 0x7, 0x6b, 
    0x2, 0x2, 0x6f3, 0x6f8, 0x7, 0xae, 0x2, 0x2, 0x6f4, 0x6f5, 0x7, 0x9, 
    0x2, 0x2, 0x6f5, 0x6f6, 0x5, 0xb0, 0x59, 0x2, 0x6f6, 0x6f7, 0x7, 0xa, 
    0x2, 0x2, 0x6f7, 0x6f9, 0x3, 0x2, 0x2, 0x2, 0x6f8, 0x6f4, 0x3, 0x2, 
    0x2, 0x2, 0x6f8, 0x6f9, 0x3, 0x2, 0x2, 0x2, 0x6f9, 0x714, 0x3, 0x2, 
    0x2, 0x2, 0x6fa, 0x6ff, 0x7, 0xa6, 0x2, 0x2, 0x6fb, 0x6fc, 0x7, 0x9, 
    0x2, 0x2, 0x6fc, 0x6fd, 0x5, 0xae, 0x58, 0x2, 0x6fd, 0x6fe, 0x7, 0xa, 
    0x2, 0x2, 0x6fe, 0x700, 0x3, 0x2, 0x2, 0x2, 0x6ff, 0x6fb, 0x3, 0x2, 
    0x2, 0x2, 0x6ff, 0x700, 0x3, 0x2, 0x2, 0x2, 0x700, 0x714, 0x3, 0x2, 
    0x2, 0x2, 0x701, 0x706, 0x7, 0xa8, 0x2, 0x2, 0x702, 0x703, 0x7, 0x9, 
    0x2, 0x2, 0x703, 0x704, 0x5, 0xae, 0x58, 0x2, 0x704, 0x705, 0x7, 0xa, 
    0x2, 0x2, 0x705, 0x707, 0x3, 0x2, 0x2, 0x2, 0x706, 0x702, 0x3, 0x2, 
    0x2, 0x2, 0x706, 0x707, 0x3, 0x2, 0x2, 0x2, 0x707, 0x714, 0x3, 0x2, 
    0x2, 0x2, 0x708, 0x711, 0x7, 0xae, 0x2, 0x2, 0x709, 0x70a, 0x7, 0x9, 
    0x2, 0x2, 0x70a, 0x70d, 0x5, 0xae, 0x58, 0x2, 0x70b, 0x70c, 0x7, 0x5, 
    0x2, 0x2, 0x70c, 0x70e, 0x5, 0xb0, 0x59, 0x2, 0x70d, 0x70b, 0x3, 0x2, 
    0x2, 0x2, 0x70d, 0x70e, 0x3, 0x2, 0x2, 0x2, 0x70e, 0x70f, 0x3, 0x2, 
    0x2, 0x2, 0x70f, 0x710, 0x7, 0xa, 0x2, 0x2, 0x710, 0x712, 0x3, 0x2, 
    0x2, 0x2, 0x711, 0x709, 0x3, 0x2, 0x2, 0x2, 0x711, 0x712, 0x3, 0x2, 
    0x2, 0x2, 0x712, 0x714, 0x3, 0x2, 0x2, 0x2, 0x713, 0x6bf, 0x3, 0x2, 
    0x2, 0x2, 0x713, 0x6c6, 0x3, 0x2, 0x2, 0x2, 0x713, 0x6d9, 0x3, 0x2, 
    0x2, 0x2, 0x713, 0x6eb, 0x3, 0x2, 0x2, 0x2, 0x713, 0x6fa, 0x3, 0x2, 
    0x2, 0x2, 0x713, 0x701, 0x3, 0x2, 0x2, 0x2, 0x713, 0x708, 0x3, 0x2, 
    0x2, 0x2, 0x714, 0xd5, 0x3, 0x2, 0x2, 0x2, 0x715, 0x716, 0x9, 0x20, 
    0x2, 0x2, 0x716, 0xd7, 0x3, 0x2, 0x2, 0x2, 0xf4, 0xda, 0xe2, 0xe9, 0xf0, 
    0xf9, 0x101, 0x107, 0x10c, 0x114, 0x11d, 0x128, 0x12c, 0x132, 0x13e, 
    0x146, 0x14b, 0x153, 0x156, 0x15c, 0x162, 0x164, 0x169, 0x16e, 0x172, 
    0x177, 0x17b, 0x181, 0x189, 0x18c, 0x192, 0x198, 0x19a, 0x1a2, 0x1a8, 
    0x1ab, 0x1b0, 0x1b9, 0x1bc, 0x1c0, 0x1ca, 0x1cd, 0x1d3, 0x1d7, 0x1d9, 
    0x1df, 0x1e3, 0x1e5, 0x1e7, 0x1ed, 0x1f3, 0x1fa, 0x1ff, 0x207, 0x20b, 
    0x216, 0x21a, 0x223, 0x225, 0x22b, 0x230, 0x237, 0x239, 0x23e, 0x243, 
    0x24b, 0x250, 0x25f, 0x268, 0x26b, 0x26f, 0x278, 0x27d, 0x282, 0x28a, 
    0x28d, 0x291, 0x298, 0x2a0, 0x2a3, 0x2b2, 0x2b9, 0x2bd, 0x2c1, 0x2c7, 
    0x2cc, 0x2cf, 0x2d4, 0x2d7, 0x2db, 0x2e0, 0x2e3, 0x2e5, 0x2e9, 0x2ef, 
    0x2f6, 0x2fe, 0x304, 0x30a, 0x311, 0x317, 0x31b, 0x323, 0x329, 0x32d, 
    0x332, 0x335, 0x339, 0x33d, 0x340, 0x343, 0x34b, 0x353, 0x35a, 0x35f, 
    0x361, 0x363, 0x368, 0x370, 0x37e, 0x383, 0x386, 0x391, 0x396, 0x398, 
    0x39b, 0x39f, 0x3a2, 0x3ab, 0x3b0, 0x3b4, 0x3b8, 0x3c3, 0x3c7, 0x3cd, 
    0x3d2, 0x3d4, 0x3d8, 0x3de, 0x3e2, 0x3ed, 0x3f5, 0x3f9, 0x3fc, 0x402, 
    0x407, 0x40b, 0x40e, 0x413, 0x417, 0x41a, 0x41e, 0x426, 0x42f, 0x435, 
    0x437, 0x43e, 0x443, 0x446, 0x449, 0x450, 0x45a, 0x45f, 0x466, 0x470, 
    0x47e, 0x488, 0x48e, 0x495, 0x4be, 0x4c7, 0x4c9, 0x4cd, 0x4ef, 0x4f2, 
    0x4fc, 0x4ff, 0x509, 0x50c, 0x518, 0x52a, 0x531, 0x533, 0x53c, 0x53f, 
    0x542, 0x546, 0x554, 0x55c, 0x565, 0x56e, 0x577, 0x580, 0x58c, 0x58f, 
    0x59c, 0x5a5, 0x5b2, 0x5b8, 0x5d2, 0x5d5, 0x5dc, 0x5e1, 0x5ea, 0x5f3, 
    0x5f6, 0x5fb, 0x602, 0x604, 0x60c, 0x611, 0x616, 0x61d, 0x622, 0x627, 
    0x630, 0x64f, 0x653, 0x657, 0x66b, 0x673, 0x67b, 0x683, 0x68b, 0x69d, 
    0x6a9, 0x6b0, 0x6b9, 0x6bb, 0x6c4, 0x6cb, 0x6d5, 0x6d7, 0x6de, 0x6e7, 
    0x6e9, 0x6f0, 0x6f8, 0x6ff, 0x706, 0x70d, 0x711, 0x713, 
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
