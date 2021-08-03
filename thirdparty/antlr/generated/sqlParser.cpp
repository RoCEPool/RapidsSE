
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
    setState(198);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ANTLR) {
      setState(196);
      match(sqlParser::ANTLR);
      setState(197);
      match(sqlParser::T__0);
    }
    setState(200);
    statementChoice();
    setState(201);
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
    setState(206);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(203);
      queryStatement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(204);
      explainStatement();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(205);
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
    setState(208);
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
    setState(213);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(210);
      queryHint();
      setState(215);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(216);
    match(sqlParser::EXPLAIN);
    setState(220);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 54) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 54)) & ((1ULL << (sqlParser::FORMAT - 54))
      | (1ULL << (sqlParser::STAGE - 54))
      | (1ULL << (sqlParser::WITH_NAMES - 54)))) != 0)) {
      setState(217);
      explainOptions();
      setState(222);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(223);
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
    setState(228);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::STAGE: {
        enterOuterAlt(_localctx, 1);
        setState(225);
        explainStageList();
        break;
      }

      case sqlParser::FORMAT: {
        enterOuterAlt(_localctx, 2);
        setState(226);
        explainFormat();
        break;
      }

      case sqlParser::WITH_NAMES: {
        enterOuterAlt(_localctx, 3);
        setState(227);
        match(sqlParser::WITH_NAMES);
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
    setState(230);
    match(sqlParser::STAGE);
    setState(231);
    explainRange();
    setState(236);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(232);
      match(sqlParser::T__2);
      setState(233);
      explainRange();
      setState(238);
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
    setState(239);
    dynamic_cast<ExplainRangeContext *>(_localctx)->begin = explainRangeChoice();
    setState(242);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__3 || _la == sqlParser::TO) {
      setState(240);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__3 || _la == sqlParser::TO)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(241);
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
    setState(247);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__4:
      case sqlParser::ALL: {
        enterOuterAlt(_localctx, 1);
        setState(244);
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
        setState(245);
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
        setState(246);
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
    setState(249);
    match(sqlParser::FORMAT);
    setState(250);
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
    setState(255);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(252);
      queryHint();
      setState(257);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(258);
    _la = _input->LA(1);
    if (!(_la == sqlParser::STATS

    || _la == sqlParser::STATISTICS)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(259);
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
    setState(275);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx)) {
    case 1: {
      setState(267);
      selectQuery();
      break;
    }

    case 2: {
      setState(268);
      setQuery();
      break;
    }

    case 3: {
      setState(269);
      insertQuery();
      break;
    }

    case 4: {
      setState(270);
      deleteQuery();
      break;
    }

    case 5: {
      setState(271);
      updateQuery();
      break;
    }

    case 6: {
      setState(272);
      createQuery();
      break;
    }

    case 7: {
      setState(273);
      dropQuery();
      break;
    }

    case 8: {
      setState(274);
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
    setState(277);
    match(sqlParser::HINT_START);
    setState(279);
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
      | (1ULL << (sqlParser::TIMESTAMP - 64))
      | (1ULL << (sqlParser::CHAR - 64))
      | (1ULL << (sqlParser::CHARACTER - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::BINARY - 130))
      | (1ULL << (sqlParser::VARBINARY - 130))
      | (1ULL << (sqlParser::VARYING - 130))
      | (1ULL << (sqlParser::BIT - 130))
      | (1ULL << (sqlParser::BITS - 130))
      | (1ULL << (sqlParser::BYTE - 130))
      | (1ULL << (sqlParser::BYTES - 130))
      | (1ULL << (sqlParser::CHARS - 130))
      | (1ULL << (sqlParser::DIGITS - 130))
      | (1ULL << (sqlParser::COALESCE - 130))
      | (1ULL << (sqlParser::CONCAT - 130))
      | (1ULL << (sqlParser::COUNT - 130))
      | (1ULL << (sqlParser::DECODE - 130))
      | (1ULL << (sqlParser::EXTRACT - 130))
      | (1ULL << (sqlParser::IFNULL - 130))
      | (1ULL << (sqlParser::NULLIF - 130))
      | (1ULL << (sqlParser::POSITION - 130))
      | (1ULL << (sqlParser::SUBSTR - 130))
      | (1ULL << (sqlParser::SUBSTRING - 130))
      | (1ULL << (sqlParser::DAY - 130))
      | (1ULL << (sqlParser::DAYS - 130))
      | (1ULL << (sqlParser::HOUR - 130))
      | (1ULL << (sqlParser::HOURS - 130))
      | (1ULL << (sqlParser::MINUTE - 130))
      | (1ULL << (sqlParser::MINUTES - 130))
      | (1ULL << (sqlParser::MONTH - 130))
      | (1ULL << (sqlParser::MONTHS - 130))
      | (1ULL << (sqlParser::QUARTER - 130))
      | (1ULL << (sqlParser::QUARTERS - 130))
      | (1ULL << (sqlParser::SECOND - 130))
      | (1ULL << (sqlParser::SECONDS - 130))
      | (1ULL << (sqlParser::WEEK - 130))
      | (1ULL << (sqlParser::WEEKS - 130))
      | (1ULL << (sqlParser::YEAR - 130))
      | (1ULL << (sqlParser::YEARS - 130))
      | (1ULL << (sqlParser::BLOCK - 130))
      | (1ULL << (sqlParser::BLOOM - 130))
      | (1ULL << (sqlParser::BTREE - 130))
      | (1ULL << (sqlParser::HASH - 130))
      | (1ULL << (sqlParser::LOOP - 130))
      | (1ULL << (sqlParser::SKIPLIST - 130))
      | (1ULL << (sqlParser::TREE - 130))
      | (1ULL << (sqlParser::VALUE - 130))
      | (1ULL << (sqlParser::IDENTIFIER - 130)))) != 0)) {
      setState(278);
      hint();
    }
    setState(285);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(281);
      match(sqlParser::T__2);
      setState(282);
      hint();
      setState(287);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(288);
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
    setState(310);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(290);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(291);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(292);
      match(sqlParser::T__5);
      setState(293);
      hintValue();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(295);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(297);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__5) {
        setState(296);
        match(sqlParser::T__5);
      }
      setState(299);
      match(sqlParser::T__6);
      setState(300);
      hintValue();
      setState(305);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(301);
        match(sqlParser::T__2);
        setState(302);
        hintValue();
        setState(307);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(308);
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
    setState(321);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(312);
      match(sqlParser::WITH);
      setState(313);
      withQuery();
      setState(318);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(314);
        match(sqlParser::T__2);
        setState(315);
        withQuery();
        setState(320);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(323);
    selectExpr();
    setState(327);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(324);
      match(sqlParser::ORDER);
      setState(325);
      match(sqlParser::BY);
      setState(326);
      dynamic_cast<SelectQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(335);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(329);
      match(sqlParser::LIMIT);
      setState(330);
      dynamic_cast<SelectQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(333);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(331);
        match(sqlParser::OFFSET);
        setState(332);
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
    setState(364);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__8:
      case sqlParser::SELECT: {
        enterOuterAlt(_localctx, 1);
        setState(337);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__8 || _la == sqlParser::SELECT)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(340);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::ALL: {
            setState(338);
            match(sqlParser::ALL);
            break;
          }

          case sqlParser::DISTINCT: {
            setState(339);
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
        setState(342);
        selectList();
        setState(345);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
        case 1: {
          setState(343);
          match(sqlParser::FROM);
          setState(344);
          tableExpr(0);
          break;
        }

        }
        setState(349);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
        case 1: {
          setState(347);
          match(sqlParser::WHERE);
          setState(348);
          dynamic_cast<SelectExprContext *>(_localctx)->where = predicate();
          break;
        }

        }
        setState(354);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
        case 1: {
          setState(351);
          match(sqlParser::GROUP);
          setState(352);
          match(sqlParser::BY);
          setState(353);
          dynamic_cast<SelectExprContext *>(_localctx)->groupBy = groupByList();
          break;
        }

        }
        setState(358);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
        case 1: {
          setState(356);
          match(sqlParser::HAVING);
          setState(357);
          dynamic_cast<SelectExprContext *>(_localctx)->having = predicate();
          break;
        }

        }
        break;
      }

      case sqlParser::T__6: {
        enterOuterAlt(_localctx, 2);
        setState(360);
        match(sqlParser::T__6);
        setState(361);
        selectExpr();
        setState(362);
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
    setState(375);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(366);
      match(sqlParser::WITH);
      setState(367);
      withQuery();
      setState(372);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(368);
        match(sqlParser::T__2);
        setState(369);
        withQuery();
        setState(374);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(377);
    setExpr(0);
    setState(381);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(378);
      match(sqlParser::ORDER);
      setState(379);
      match(sqlParser::BY);
      setState(380);
      dynamic_cast<SetQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(389);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(383);
      match(sqlParser::LIMIT);
      setState(384);
      dynamic_cast<SetQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(387);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(385);
        match(sqlParser::OFFSET);
        setState(386);
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
    setState(397);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx)) {
    case 1: {
      setState(392);
      selectExpr();
      break;
    }

    case 2: {
      setState(393);
      match(sqlParser::T__6);
      setState(394);
      setExpr(0);
      setState(395);
      match(sqlParser::T__7);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(411);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<SetExprContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleSetExpr);
        setState(399);

        if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
        setState(400);
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
        setState(403);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::DISTINCT: {
            setState(401);
            match(sqlParser::DISTINCT);
            break;
          }

          case sqlParser::ALL: {
            setState(402);
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
        setState(406);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::CORRESPONDING) {
          setState(405);
          corrClause();
        }
        setState(408);
        setExpr(3); 
      }
      setState(413);
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
    setState(414);
    dynamic_cast<CorrClauseContext *>(_localctx)->corr = match(sqlParser::CORRESPONDING);
    setState(420);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::BY) {
      setState(415);
      match(sqlParser::BY);
      setState(416);
      match(sqlParser::T__6);
      setState(417);
      corrList();
      setState(418);
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
    setState(422);
    dynamic_cast<WithQueryContext *>(_localctx)->name = tableName();
    setState(434);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__6) {
      setState(423);
      match(sqlParser::T__6);
      setState(424);
      columnAlias();
      setState(429);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(425);
        match(sqlParser::T__2);
        setState(426);
        columnAlias();
        setState(431);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(432);
      match(sqlParser::T__7);
    }
    setState(436);
    match(sqlParser::AS);
    setState(437);
    match(sqlParser::T__6);
    setState(438);
    query();
    setState(439);
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

sqlParser::JoinTypeContext* sqlParser::JoinUsingContext::joinType() {
  return getRuleContext<sqlParser::JoinTypeContext>(0);
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

sqlParser::JoinTypeContext* sqlParser::JoinOnContext::joinType() {
  return getRuleContext<sqlParser::JoinTypeContext>(0);
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
    setState(467);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 45, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<FetchContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(442);
      dynamic_cast<FetchContext *>(_localctx)->ref = tableReference();
      setState(447);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 40, _ctx)) {
      case 1: {
        setState(444);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(443);
          match(sqlParser::AS);
        }
        setState(446);
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
      setState(449);
      match(sqlParser::T__6);
      setState(450);
      query();
      setState(451);
      match(sqlParser::T__7);
      setState(456);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 42, _ctx)) {
      case 1: {
        setState(453);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(452);
          match(sqlParser::AS);
        }
        setState(455);
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
      setState(458);
      match(sqlParser::T__6);
      setState(459);
      valuesExpr();
      setState(460);
      match(sqlParser::T__7);
      setState(465);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 44, _ctx)) {
      case 1: {
        setState(462);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(461);
          match(sqlParser::AS);
        }
        setState(464);
        dynamic_cast<ValuesSubqueryContext *>(_localctx)->as = tableAlias();
        break;
      }

      }
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(504);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 52, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(502);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<CrossJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(469);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(473);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case sqlParser::T__2: {
              setState(470);
              match(sqlParser::T__2);
              break;
            }

            case sqlParser::CROSS: {
              setState(471);
              match(sqlParser::CROSS);
              setState(472);
              match(sqlParser::JOIN);
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(475);
          tableExpr(4);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<JoinOnContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(476);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(478);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::ANTI)
            | (1ULL << sqlParser::FULL)
            | (1ULL << sqlParser::INNER))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::LEFT - 69))
            | (1ULL << (sqlParser::NATURAL - 69))
            | (1ULL << (sqlParser::RIGHT - 69))
            | (1ULL << (sqlParser::SEMI - 69)))) != 0)) {
            setState(477);
            dynamic_cast<JoinOnContext *>(_localctx)->type = joinType();
          }
          setState(481);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::HINT_START) {
            setState(480);
            dynamic_cast<JoinOnContext *>(_localctx)->joint = joinHint();
          }
          setState(483);
          match(sqlParser::JOIN);
          setState(484);
          tableExpr(0);
          setState(485);
          match(sqlParser::ON);
          setState(486);
          joinPredicate();
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<JoinUsingContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(488);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(490);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::ANTI)
            | (1ULL << sqlParser::FULL)
            | (1ULL << sqlParser::INNER))) != 0) || ((((_la - 69) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 69)) & ((1ULL << (sqlParser::LEFT - 69))
            | (1ULL << (sqlParser::NATURAL - 69))
            | (1ULL << (sqlParser::RIGHT - 69))
            | (1ULL << (sqlParser::SEMI - 69)))) != 0)) {
            setState(489);
            dynamic_cast<JoinUsingContext *>(_localctx)->type = joinType();
          }
          setState(493);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::HINT_START) {
            setState(492);
            dynamic_cast<JoinUsingContext *>(_localctx)->joint = joinHint();
          }
          setState(495);
          match(sqlParser::JOIN);
          setState(496);
          tableExpr(0);
          setState(497);
          match(sqlParser::USING);
          setState(498);
          match(sqlParser::T__6);
          setState(499);
          joinUsingList();
          setState(500);
          match(sqlParser::T__7);
          break;
        }

        } 
      }
      setState(506);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 52, _ctx);
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
    setState(515);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 54, _ctx)) {
    case 1: {
      setState(510);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx)) {
      case 1: {
        setState(507);
        dynamic_cast<TableReferenceContext *>(_localctx)->catalog = catalogName();
        setState(508);
        match(sqlParser::T__9);
        break;
      }

      }
      setState(512);
      dynamic_cast<TableReferenceContext *>(_localctx)->schema = schemaName();
      setState(513);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(517);
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

tree::TerminalNode* sqlParser::JoinTypeContext::NATURAL() {
  return getToken(sqlParser::NATURAL, 0);
}

tree::TerminalNode* sqlParser::JoinTypeContext::SEMI() {
  return getToken(sqlParser::SEMI, 0);
}

tree::TerminalNode* sqlParser::JoinTypeContext::ANTI() {
  return getToken(sqlParser::ANTI, 0);
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
    setState(527);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::INNER: {
        enterOuterAlt(_localctx, 1);
        setState(519);
        match(sqlParser::INNER);
        break;
      }

      case sqlParser::FULL:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 2);
        setState(520);
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
        setState(522);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::OUTER) {
          setState(521);
          match(sqlParser::OUTER);
        }
        break;
      }

      case sqlParser::NATURAL: {
        enterOuterAlt(_localctx, 3);
        setState(524);
        match(sqlParser::NATURAL);
        break;
      }

      case sqlParser::SEMI: {
        enterOuterAlt(_localctx, 4);
        setState(525);
        match(sqlParser::SEMI);
        break;
      }

      case sqlParser::ANTI: {
        enterOuterAlt(_localctx, 5);
        setState(526);
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
  enterRule(_localctx, 44, sqlParser::RuleJoinHint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(529);
    match(sqlParser::HINT_START);
    setState(530);
    dynamic_cast<JoinHintContext *>(_localctx)->method = _input->LT(1);
    _la = _input->LA(1);
    if (!(((((_la - 176) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 176)) & ((1ULL << (sqlParser::BLOCK - 176))
      | (1ULL << (sqlParser::BLOOM - 176))
      | (1ULL << (sqlParser::HASH - 176))
      | (1ULL << (sqlParser::LOOP - 176)))) != 0))) {
      dynamic_cast<JoinHintContext *>(_localctx)->method = _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(545);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__5

    || _la == sqlParser::T__6) {
      setState(532);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__5) {
        setState(531);
        match(sqlParser::T__5);
      }
      setState(534);
      match(sqlParser::T__6);
      setState(535);
      dynamic_cast<JoinHintContext *>(_localctx)->values = hintValue();
      setState(540);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(536);
        match(sqlParser::T__2);
        setState(537);
        hintValue();
        setState(542);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(543);
      match(sqlParser::T__7);
    }
    setState(547);
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
  enterRule(_localctx, 46, sqlParser::RuleValuesExpr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(549);
    match(sqlParser::VALUES);
    setState(550);
    match(sqlParser::T__6);
    setState(551);
    valueList();
    setState(552);
    match(sqlParser::T__7);
    setState(560);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(553);
      match(sqlParser::T__2);
      setState(554);
      match(sqlParser::T__6);
      setState(555);
      valueList();
      setState(556);
      match(sqlParser::T__7);
      setState(562);
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
  enterRule(_localctx, 48, sqlParser::RuleInsertQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(572);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(563);
      match(sqlParser::WITH);
      setState(564);
      withQuery();
      setState(569);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(565);
        match(sqlParser::T__2);
        setState(566);
        withQuery();
        setState(571);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(574);
    match(sqlParser::INSERT);
    setState(576);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 63, _ctx)) {
    case 1: {
      setState(575);
      match(sqlParser::INTO);
      break;
    }

    }
    setState(578);
    dynamic_cast<InsertQueryContext *>(_localctx)->ref = tableReference();
    setState(590);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 65, _ctx)) {
    case 1: {
      setState(579);
      match(sqlParser::T__6);
      setState(580);
      columnName();
      setState(585);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(581);
        match(sqlParser::T__2);
        setState(582);
        columnName();
        setState(587);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(588);
      match(sqlParser::T__7);
      break;
    }

    }
    setState(595);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 66, _ctx)) {
    case 1: {
      setState(592);
      valuesExpr();
      break;
    }

    case 2: {
      setState(593);
      selectQuery();
      break;
    }

    case 3: {
      setState(594);
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
  enterRule(_localctx, 50, sqlParser::RuleDeleteQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(606);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(597);
      match(sqlParser::WITH);
      setState(598);
      withQuery();
      setState(603);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(599);
        match(sqlParser::T__2);
        setState(600);
        withQuery();
        setState(605);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(608);
    match(sqlParser::DELETE);
    setState(610);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::FROM) {
      setState(609);
      match(sqlParser::FROM);
    }
    setState(612);
    dynamic_cast<DeleteQueryContext *>(_localctx)->ref = tableReference();
    setState(617);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(613);
        match(sqlParser::WHERE);
        setState(614);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(615);
        match(sqlParser::FROM);
        setState(616);
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
  enterRule(_localctx, 52, sqlParser::RuleUpdateQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(628);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(619);
      match(sqlParser::WITH);
      setState(620);
      withQuery();
      setState(625);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(621);
        match(sqlParser::T__2);
        setState(622);
        withQuery();
        setState(627);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(630);
    match(sqlParser::UPDATE);
    setState(631);
    dynamic_cast<UpdateQueryContext *>(_localctx)->ref = tableReference();
    setState(632);
    match(sqlParser::SET);
    setState(633);
    columnName();
    setState(634);
    match(sqlParser::T__5);
    setState(635);
    expr(0);
    setState(643);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(636);
      match(sqlParser::T__2);
      setState(637);
      columnName();
      setState(638);
      match(sqlParser::T__5);
      setState(639);
      expr(0);
      setState(645);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(650);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(646);
        match(sqlParser::WHERE);
        setState(647);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(648);
        match(sqlParser::FROM);
        setState(649);
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
  enterRule(_localctx, 54, sqlParser::RuleCreateQuery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(654);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(652);
      createTableQuery();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(653);
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
  enterRule(_localctx, 56, sqlParser::RuleCreateTableQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(656);
    match(sqlParser::CREATE);
    setState(658);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::REFERENCE) {
      setState(657);
      dynamic_cast<CreateTableQueryContext *>(_localctx)->reference = match(sqlParser::REFERENCE);
    }
    setState(660);
    match(sqlParser::TABLE);
    setState(664);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::IF) {
      setState(661);
      match(sqlParser::IF);
      setState(662);
      match(sqlParser::NOT);
      setState(663);
      match(sqlParser::EXISTS);
    }
    setState(666);
    dynamic_cast<CreateTableQueryContext *>(_localctx)->ref = tableReference();
    setState(694);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 85, _ctx)) {
    case 1: {
      setState(667);
      tableDefinition();
      break;
    }

    case 2: {
      setState(669);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 78, _ctx)) {
      case 1: {
        setState(668);
        tableDefinition();
        break;
      }

      }
      setState(672);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::AS) {
        setState(671);
        match(sqlParser::AS);
      }
      setState(674);
      asSubquery();
      setState(680);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::WITH) {
        setState(675);
        match(sqlParser::WITH);
        setState(677);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::NO) {
          setState(676);
          dynamic_cast<CreateTableQueryContext *>(_localctx)->noData = match(sqlParser::NO);
        }
        setState(679);
        match(sqlParser::DATA);
      }
      break;
    }

    case 3: {
      setState(682);
      columnNameList();
      setState(684);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::AS) {
        setState(683);
        match(sqlParser::AS);
      }
      setState(686);
      asSubquery();
      setState(692);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::WITH) {
        setState(687);
        match(sqlParser::WITH);
        setState(689);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::NO) {
          setState(688);
          dynamic_cast<CreateTableQueryContext *>(_localctx)->noData = match(sqlParser::NO);
        }
        setState(691);
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
  enterRule(_localctx, 58, sqlParser::RuleCreateIndexQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(696);
    match(sqlParser::CREATE);
    setState(698);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(697);
      match(sqlParser::UNIQUE);
    }
    setState(700);
    _la = _input->LA(1);
    if (!(_la == sqlParser::INDEX

    || _la == sqlParser::KEY)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(704);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::IF) {
      setState(701);
      match(sqlParser::IF);
      setState(702);
      match(sqlParser::NOT);
      setState(703);
      match(sqlParser::EXISTS);
    }
    setState(706);
    dynamic_cast<CreateIndexQueryContext *>(_localctx)->idxName = indexName();
    setState(707);
    match(sqlParser::ON);
    setState(708);
    dynamic_cast<CreateIndexQueryContext *>(_localctx)->tblName = tableReference();
    setState(711);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::USING) {
      setState(709);
      match(sqlParser::USING);
      setState(710);
      dynamic_cast<CreateIndexQueryContext *>(_localctx)->method = indexMethod();
    }
    setState(713);
    match(sqlParser::T__6);
    setState(714);
    expr(0);
    setState(719);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(715);
      match(sqlParser::T__2);
      setState(716);
      expr(0);
      setState(721);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(722);
    match(sqlParser::T__7);
    setState(725);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(723);
      match(sqlParser::WITH);
      setState(724);
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
  enterRule(_localctx, 60, sqlParser::RuleDropQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(744);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 93, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<DropQueryContext *>(_tracker.createInstance<sqlParser::DropTableContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(727);
      match(sqlParser::DROP);
      setState(728);
      match(sqlParser::TABLE);
      setState(731);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::IF) {
        setState(729);
        match(sqlParser::IF);
        setState(730);
        match(sqlParser::EXISTS);
      }
      setState(733);
      dynamic_cast<DropTableContext *>(_localctx)->name = tableReference();
      break;
    }

    case 2: {
      _localctx = dynamic_cast<DropQueryContext *>(_tracker.createInstance<sqlParser::DropIndexContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(734);
      match(sqlParser::DROP);
      setState(735);
      _la = _input->LA(1);
      if (!(_la == sqlParser::INDEX

      || _la == sqlParser::KEY)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(738);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::IF) {
        setState(736);
        match(sqlParser::IF);
        setState(737);
        match(sqlParser::EXISTS);
      }
      setState(740);
      dynamic_cast<DropIndexContext *>(_localctx)->idxName = indexName();
      setState(741);
      match(sqlParser::ON);
      setState(742);
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
  enterRule(_localctx, 62, sqlParser::RuleTruncateQuery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(746);
    match(sqlParser::TRUNCATE);
    setState(748);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 94, _ctx)) {
    case 1: {
      setState(747);
      match(sqlParser::TABLE);
      break;
    }

    }
    setState(750);
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
  enterRule(_localctx, 64, sqlParser::RuleTableDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(752);
    match(sqlParser::T__6);
    setState(756);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 95, _ctx)) {
    case 1: {
      setState(753);
      columnDefinition();
      break;
    }

    case 2: {
      setState(754);
      tableConstraint();
      break;
    }

    case 3: {
      setState(755);
      indexDefinition();
      break;
    }

    }
    setState(766);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(758);
      match(sqlParser::T__2);
      setState(762);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 96, _ctx)) {
      case 1: {
        setState(759);
        columnDefinition();
        break;
      }

      case 2: {
        setState(760);
        tableConstraint();
        break;
      }

      case 3: {
        setState(761);
        indexDefinition();
        break;
      }

      }
      setState(768);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(769);
    match(sqlParser::T__7);
    setState(771);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::REFERENCE) {
      setState(770);
      match(sqlParser::REFERENCE);
    }
    setState(774);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::PARTITION) {
      setState(773);
      tablePartitioning();
    }
    setState(778);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 100, _ctx)) {
    case 1: {
      setState(776);
      match(sqlParser::WITH);
      setState(777);
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
  enterRule(_localctx, 66, sqlParser::RuleTablePartitioning);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(780);
    match(sqlParser::PARTITION);
    setState(782);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::BY) {
      setState(781);
      match(sqlParser::BY);
    }
    setState(785);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::HASH

    || _la == sqlParser::VALUE) {
      setState(784);
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
    setState(788);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ON) {
      setState(787);
      match(sqlParser::ON);
    }
    setState(790);
    match(sqlParser::T__6);
    setState(791);
    expr(0);
    setState(796);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(792);
      match(sqlParser::T__2);
      setState(793);
      expr(0);
      setState(798);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(799);
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
  enterRule(_localctx, 68, sqlParser::RuleColumnDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(801);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->name = columnName();
    setState(802);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->type = dataType();
    setState(820);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 78) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 78)) & ((1ULL << (sqlParser::NOT - 78))
      | (1ULL << (sqlParser::SQL_NULL - 78))
      | (1ULL << (sqlParser::PRIMARY - 78))
      | (1ULL << (sqlParser::UNIQUE - 78)))) != 0)) {
      setState(818);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 108, _ctx)) {
      case 1: {
        setState(804);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::NOT) {
          setState(803);
          dynamic_cast<ColumnDefinitionContext *>(_localctx)->notNullable = match(sqlParser::NOT);
        }
        setState(806);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->nullable = match(sqlParser::SQL_NULL);
        break;
      }

      case 2: {
        setState(807);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->primary = match(sqlParser::PRIMARY);
        setState(808);
        match(sqlParser::KEY);
        break;
      }

      case 3: {
        setState(809);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
        setState(811);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::KEY) {
          setState(810);
          match(sqlParser::KEY);
        }
        break;
      }

      case 4: {
        setState(813);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
        setState(814);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->mono = match(sqlParser::MONOTONIC);
        setState(816);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::KEY) {
          setState(815);
          match(sqlParser::KEY);
        }
        break;
      }

      }
      setState(822);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(825);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(823);
      match(sqlParser::WITH);
      setState(824);
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
  enterRule(_localctx, 70, sqlParser::RuleColumnNameList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(827);
    match(sqlParser::T__6);
    setState(828);
    columnName();
    setState(833);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(829);
      match(sqlParser::T__2);
      setState(830);
      columnName();
      setState(835);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(836);
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
  enterRule(_localctx, 72, sqlParser::RuleTableConstraint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(873);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::PRIMARY: {
        enterOuterAlt(_localctx, 1);
        setState(838);
        dynamic_cast<TableConstraintContext *>(_localctx)->primary = match(sqlParser::PRIMARY);
        setState(839);
        match(sqlParser::KEY);
        setState(852);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 113, _ctx)) {
        case 1: {
          setState(840);
          expr(0);
          break;
        }

        case 2: {
          setState(841);
          match(sqlParser::T__6);
          setState(842);
          expr(0);
          setState(847);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::T__2) {
            setState(843);
            match(sqlParser::T__2);
            setState(844);
            expr(0);
            setState(849);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(850);
          match(sqlParser::T__7);
          break;
        }

        }
        break;
      }

      case sqlParser::MONOTONIC:
      case sqlParser::UNIQUE: {
        enterOuterAlt(_localctx, 2);
        setState(855);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::UNIQUE) {
          setState(854);
          dynamic_cast<TableConstraintContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
        }
        setState(857);
        dynamic_cast<TableConstraintContext *>(_localctx)->mono = match(sqlParser::MONOTONIC);
        setState(858);
        match(sqlParser::KEY);
        setState(871);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 116, _ctx)) {
        case 1: {
          setState(859);
          expr(0);
          break;
        }

        case 2: {
          setState(860);
          match(sqlParser::T__6);
          setState(861);
          expr(0);
          setState(866);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::T__2) {
            setState(862);
            match(sqlParser::T__2);
            setState(863);
            expr(0);
            setState(868);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(869);
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
  enterRule(_localctx, 74, sqlParser::RuleIndexDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(876);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(875);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
    }
    setState(878);
    _la = _input->LA(1);
    if (!(_la == sqlParser::INDEX

    || _la == sqlParser::KEY)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(880);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 119, _ctx)) {
    case 1: {
      setState(879);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->name = indexName();
      break;
    }

    }
    setState(883);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ON) {
      setState(882);
      match(sqlParser::ON);
    }
    setState(897);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 122, _ctx)) {
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
    setState(901);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::USING) {
      setState(899);
      match(sqlParser::USING);
      setState(900);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->method = indexMethod();
    }
    setState(905);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(903);
      match(sqlParser::WITH);
      setState(904);
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
  enterRule(_localctx, 76, sqlParser::RuleIndexMethod);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(907);
    _la = _input->LA(1);
    if (!(((((_la - 178) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 178)) & ((1ULL << (sqlParser::BTREE - 178))
      | (1ULL << (sqlParser::HASH - 178))
      | (1ULL << (sqlParser::SKIPLIST - 178))
      | (1ULL << (sqlParser::TREE - 178)))) != 0))) {
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
  enterRule(_localctx, 78, sqlParser::RuleDataType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(915);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 126, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(909);
      sqlDataType();
      setState(912);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::USING) {
        setState(910);
        match(sqlParser::USING);
        setState(911);
        className();
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(914);
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
  enterRule(_localctx, 80, sqlParser::RuleSqlDataType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1008);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::BOOLEAN: {
        enterOuterAlt(_localctx, 1);
        setState(917);
        match(sqlParser::BOOLEAN);
        break;
      }

      case sqlParser::INTEGER: {
        enterOuterAlt(_localctx, 2);
        setState(918);
        match(sqlParser::INTEGER);
        setState(926);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__6) {
          setState(919);
          match(sqlParser::T__6);
          setState(920);
          precision();
          setState(922);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 135) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 135)) & ((1ULL << (sqlParser::BITS - 135))
            | (1ULL << (sqlParser::BYTES - 135))
            | (1ULL << (sqlParser::DIGITS - 135)))) != 0)) {
            setState(921);
            _la = _input->LA(1);
            if (!(((((_la - 135) & ~ 0x3fULL) == 0) &&
              ((1ULL << (_la - 135)) & ((1ULL << (sqlParser::BITS - 135))
              | (1ULL << (sqlParser::BYTES - 135))
              | (1ULL << (sqlParser::DIGITS - 135)))) != 0))) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
          }
          setState(924);
          match(sqlParser::T__7);
        }
        break;
      }

      case sqlParser::DECIMAL: {
        enterOuterAlt(_localctx, 3);
        setState(928);
        match(sqlParser::DECIMAL);
        setState(943);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__6) {
          setState(929);
          match(sqlParser::T__6);
          setState(930);
          precision();
          setState(932);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::DIGITS) {
            setState(931);
            match(sqlParser::DIGITS);
          }
          setState(939);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__2) {
            setState(934);
            match(sqlParser::T__2);
            setState(935);
            scale();
            setState(937);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == sqlParser::DIGITS) {
              setState(936);
              match(sqlParser::DIGITS);
            }
          }
          setState(941);
          match(sqlParser::T__7);
        }
        break;
      }

      case sqlParser::FLOAT: {
        enterOuterAlt(_localctx, 4);
        setState(945);
        match(sqlParser::FLOAT);
        setState(953);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__6) {
          setState(946);
          match(sqlParser::T__6);
          setState(947);
          precision();
          setState(949);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 135) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 135)) & ((1ULL << (sqlParser::BITS - 135))
            | (1ULL << (sqlParser::BYTES - 135))
            | (1ULL << (sqlParser::DIGITS - 135)))) != 0)) {
            setState(948);
            _la = _input->LA(1);
            if (!(((((_la - 135) & ~ 0x3fULL) == 0) &&
              ((1ULL << (_la - 135)) & ((1ULL << (sqlParser::BITS - 135))
              | (1ULL << (sqlParser::BYTES - 135))
              | (1ULL << (sqlParser::DIGITS - 135)))) != 0))) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
          }
          setState(951);
          match(sqlParser::T__7);
        }
        break;
      }

      case sqlParser::DATE: {
        enterOuterAlt(_localctx, 5);
        setState(955);
        match(sqlParser::DATE);
        break;
      }

      case sqlParser::TIME: {
        enterOuterAlt(_localctx, 6);
        setState(956);
        match(sqlParser::TIME);
        break;
      }

      case sqlParser::TIMESTAMP: {
        enterOuterAlt(_localctx, 7);
        setState(957);
        match(sqlParser::TIMESTAMP);
        break;
      }

      case sqlParser::CHAR:
      case sqlParser::CHARACTER:
      case sqlParser::VARCHAR:
      case sqlParser::TEXT: {
        enterOuterAlt(_localctx, 8);
        setState(983);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 140, _ctx)) {
        case 1: {
          setState(958);
          _la = _input->LA(1);
          if (!(_la == sqlParser::CHAR

          || _la == sqlParser::CHARACTER)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(959);
          match(sqlParser::T__6);
          setState(960);
          size();
          setState(962);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 126) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 126)) & ((1ULL << (sqlParser::CHAR - 126))
            | (1ULL << (sqlParser::BYTE - 126))
            | (1ULL << (sqlParser::BYTES - 126))
            | (1ULL << (sqlParser::CHARS - 126)))) != 0)) {
            setState(961);
            _la = _input->LA(1);
            if (!(((((_la - 126) & ~ 0x3fULL) == 0) &&
              ((1ULL << (_la - 126)) & ((1ULL << (sqlParser::CHAR - 126))
              | (1ULL << (sqlParser::BYTE - 126))
              | (1ULL << (sqlParser::BYTES - 126))
              | (1ULL << (sqlParser::CHARS - 126)))) != 0))) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
          }
          setState(964);
          match(sqlParser::T__7);
          break;
        }

        case 2: {
          setState(971);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case sqlParser::CHAR:
            case sqlParser::CHARACTER: {
              setState(966);
              _la = _input->LA(1);
              if (!(_la == sqlParser::CHAR

              || _la == sqlParser::CHARACTER)) {
              _errHandler->recoverInline(this);
              }
              else {
                _errHandler->reportMatch(this);
                consume();
              }
              setState(968);
              _errHandler->sync(this);

              _la = _input->LA(1);
              if (_la == sqlParser::VARYING) {
                setState(967);
                match(sqlParser::VARYING);
              }
              break;
            }

            case sqlParser::VARCHAR: {
              setState(970);
              match(sqlParser::VARCHAR);
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(980);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__6) {
            setState(973);
            match(sqlParser::T__6);
            setState(974);
            size();
            setState(976);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (((((_la - 126) & ~ 0x3fULL) == 0) &&
              ((1ULL << (_la - 126)) & ((1ULL << (sqlParser::CHAR - 126))
              | (1ULL << (sqlParser::BYTE - 126))
              | (1ULL << (sqlParser::BYTES - 126))
              | (1ULL << (sqlParser::CHARS - 126)))) != 0)) {
              setState(975);
              _la = _input->LA(1);
              if (!(((((_la - 126) & ~ 0x3fULL) == 0) &&
                ((1ULL << (_la - 126)) & ((1ULL << (sqlParser::CHAR - 126))
                | (1ULL << (sqlParser::BYTE - 126))
                | (1ULL << (sqlParser::BYTES - 126))
                | (1ULL << (sqlParser::CHARS - 126)))) != 0))) {
              _errHandler->recoverInline(this);
              }
              else {
                _errHandler->reportMatch(this);
                consume();
              }
            }
            setState(978);
            match(sqlParser::T__7);
          }
          break;
        }

        case 3: {
          setState(982);
          match(sqlParser::TEXT);
          break;
        }

        }
        setState(987);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::COLLATE) {
          setState(985);
          match(sqlParser::COLLATE);
          setState(986);
          dynamic_cast<SqlDataTypeContext *>(_localctx)->collate = collationName();
        }
        break;
      }

      case sqlParser::BINARY: {
        enterOuterAlt(_localctx, 9);
        setState(989);
        match(sqlParser::BINARY);

        setState(990);
        match(sqlParser::T__6);
        setState(991);
        size();
        setState(993);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::BYTE

        || _la == sqlParser::BYTES) {
          setState(992);
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
        setState(995);
        match(sqlParser::T__7);
        break;
      }

      case sqlParser::VARBINARY: {
        enterOuterAlt(_localctx, 10);
        setState(997);
        match(sqlParser::VARBINARY);
        setState(1005);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__6) {
          setState(998);
          match(sqlParser::T__6);
          setState(999);
          size();
          setState(1001);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::BYTE

          || _la == sqlParser::BYTES) {
            setState(1000);
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
          setState(1003);
          match(sqlParser::T__7);
        }
        break;
      }

      case sqlParser::BLOB: {
        enterOuterAlt(_localctx, 11);
        setState(1007);
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
  enterRule(_localctx, 82, sqlParser::RuleAsSubquery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1010);
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
  enterRule(_localctx, 84, sqlParser::RuleSelectOrValues);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    setState(1033);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 149, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1012);
      match(sqlParser::T__6);
      setState(1016);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 146, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(1013);
          queryHint(); 
        }
        setState(1018);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 146, _ctx);
      }
      setState(1019);
      selectOrValues();
      setState(1020);
      match(sqlParser::T__7);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1025);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::HINT_START) {
        setState(1022);
        queryHint();
        setState(1027);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(1031);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 148, _ctx)) {
      case 1: {
        setState(1028);
        selectQuery();
        break;
      }

      case 2: {
        setState(1029);
        valuesExpr();
        break;
      }

      case 3: {
        setState(1030);
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
  enterRule(_localctx, 86, sqlParser::RuleSelectList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1035);
    selectListItem();
    setState(1040);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 150, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1036);
        match(sqlParser::T__2);
        setState(1037);
        selectListItem(); 
      }
      setState(1042);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 150, _ctx);
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
  enterRule(_localctx, 88, sqlParser::RuleSelectListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1051);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 153, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1043);
      expr(0);
      setState(1048);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 152, _ctx)) {
      case 1: {
        setState(1045);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(1044);
          match(sqlParser::AS);
        }
        setState(1047);
        dynamic_cast<SelectListItemContext *>(_localctx)->as = identifier();
        break;
      }

      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1050);
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
  enterRule(_localctx, 90, sqlParser::RuleGroupByList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1053);
    groupByListItem();
    setState(1058);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 154, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1054);
        match(sqlParser::T__2);
        setState(1055);
        groupByListItem(); 
      }
      setState(1060);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 154, _ctx);
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
  enterRule(_localctx, 92, sqlParser::RuleGroupByListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1061);
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
  enterRule(_localctx, 94, sqlParser::RuleOrderByList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1063);
    orderByListItem();
    setState(1068);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1064);
      match(sqlParser::T__2);
      setState(1065);
      orderByListItem();
      setState(1070);
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
  enterRule(_localctx, 96, sqlParser::RuleOrderByListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1071);
    expr(0);
    setState(1073);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ASC

    || _la == sqlParser::DESC) {
      setState(1072);
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
  enterRule(_localctx, 98, sqlParser::RuleJoinUsingList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1075);
    joinUsingListItem();
    setState(1080);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1076);
      match(sqlParser::T__2);
      setState(1077);
      joinUsingListItem();
      setState(1082);
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
  enterRule(_localctx, 100, sqlParser::RuleJoinUsingListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1083);
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
  enterRule(_localctx, 102, sqlParser::RuleCorrList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1085);
    corrListItem();
    setState(1090);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1086);
      match(sqlParser::T__2);
      setState(1087);
      corrListItem();
      setState(1092);
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
  enterRule(_localctx, 104, sqlParser::RuleCorrListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1093);
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
  enterRule(_localctx, 106, sqlParser::RulePropertyList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1095);
    property();
    setState(1100);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 159, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1096);
        match(sqlParser::T__2);
        setState(1097);
        property(); 
      }
      setState(1102);
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
  enterRule(_localctx, 108, sqlParser::RuleProperty);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1103);
    dynamic_cast<PropertyContext *>(_localctx)->name = propertyName();
    setState(1106);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__5) {
      setState(1104);
      match(sqlParser::T__5);
      setState(1105);
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
  enterRule(_localctx, 110, sqlParser::RuleValueList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1108);
    expr(0);
    setState(1113);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1109);
      match(sqlParser::T__2);
      setState(1110);
      expr(0);
      setState(1115);
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
  enterRule(_localctx, 112, sqlParser::RulePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1116);
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
  enterRule(_localctx, 114, sqlParser::RuleJoinPredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1118);
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
  enterRule(_localctx, 116, sqlParser::RuleCasePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1120);
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

tree::TerminalNode* sqlParser::ExtractFuncContext::WEEK() {
  return getToken(sqlParser::WEEK, 0);
}

tree::TerminalNode* sqlParser::ExtractFuncContext::MONTH() {
  return getToken(sqlParser::MONTH, 0);
}

tree::TerminalNode* sqlParser::ExtractFuncContext::QUARTER() {
  return getToken(sqlParser::QUARTER, 0);
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
  size_t startState = 118;
  enterRecursionRule(_localctx, 118, sqlParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1385);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 189, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<LiteralValContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(1123);
      literal();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<VarRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1124);
      variableName();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ColRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1125);
      columnRef();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<InlineCastContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1126);
      match(sqlParser::T__6);
      setState(1127);
      dataType();
      setState(1128);
      match(sqlParser::T__7);
      setState(1129);
      expr(39);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<UnaryOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1131);
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
      setState(1132);
      expr(38);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<CastOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1133);
      match(sqlParser::CAST);
      setState(1134);
      match(sqlParser::T__6);
      setState(1135);
      expr(0);
      setState(1136);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2

      || _la == sqlParser::AS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1137);
      dataType();
      setState(1138);
      match(sqlParser::T__7);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<TypeOfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1140);
      match(sqlParser::TYPEOF);
      setState(1141);
      match(sqlParser::T__6);
      setState(1142);
      expr(0);
      setState(1143);
      match(sqlParser::T__7);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<CaseOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1145);
      match(sqlParser::CASE);
      setState(1165);
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
          setState(1146);
          dynamic_cast<CaseOpContext *>(_localctx)->caseExpr = expr(0);
          setState(1152); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1147);
            match(sqlParser::WHEN);
            setState(1148);
            expr(0);
            setState(1149);
            match(sqlParser::THEN);
            setState(1150);
            expr(0);
            setState(1154); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

        case sqlParser::WHEN: {
          setState(1161); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1156);
            match(sqlParser::WHEN);
            setState(1157);
            casePredicate();
            setState(1158);
            match(sqlParser::THEN);
            setState(1159);
            expr(0);
            setState(1163); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(1169);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ELSE) {
        setState(1167);
        match(sqlParser::ELSE);
        setState(1168);
        expr(0);
      }
      setState(1171);
      match(sqlParser::END);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<IfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1173);
      match(sqlParser::IF);
      setState(1174);
      match(sqlParser::T__6);
      setState(1175);
      casePredicate();
      setState(1176);
      match(sqlParser::T__2);
      setState(1177);
      expr(0);
      setState(1178);
      match(sqlParser::T__2);
      setState(1179);
      expr(0);
      setState(1180);
      match(sqlParser::T__7);
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<IfNullOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1182);
      match(sqlParser::IFNULL);
      setState(1183);
      match(sqlParser::T__6);
      setState(1184);
      expr(0);
      setState(1185);
      match(sqlParser::T__2);
      setState(1186);
      expr(0);
      setState(1187);
      match(sqlParser::T__7);
      break;
    }

    case 11: {
      _localctx = _tracker.createInstance<NullIfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1189);
      match(sqlParser::NULLIF);
      setState(1190);
      match(sqlParser::T__6);
      setState(1191);
      expr(0);
      setState(1192);
      match(sqlParser::T__2);
      setState(1193);
      expr(0);
      setState(1194);
      match(sqlParser::T__7);
      break;
    }

    case 12: {
      _localctx = _tracker.createInstance<CoalesceOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1196);
      match(sqlParser::COALESCE);
      setState(1197);
      match(sqlParser::T__6);
      setState(1206);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__3)
        | (1ULL << sqlParser::T__6)
        | (1ULL << sqlParser::T__10)
        | (1ULL << sqlParser::ANTLR)
        | (1ULL << sqlParser::ASC)
        | (1ULL << sqlParser::AVG)
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
        | (1ULL << sqlParser::SQL_FALSE)
        | (1ULL << sqlParser::FEDERATION)
        | (1ULL << sqlParser::FORMAT)
        | (1ULL << sqlParser::INDEX)
        | (1ULL << sqlParser::INSERT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::INTO - 64))
        | (1ULL << (sqlParser::IS - 64))
        | (1ULL << (sqlParser::JOIN - 64))
        | (1ULL << (sqlParser::KEY - 64))
        | (1ULL << (sqlParser::LEFT - 64))
        | (1ULL << (sqlParser::MAX - 64))
        | (1ULL << (sqlParser::MIN - 64))
        | (1ULL << (sqlParser::MONOTONIC - 64))
        | (1ULL << (sqlParser::NO - 64))
        | (1ULL << (sqlParser::NODE - 64))
        | (1ULL << (sqlParser::NOT - 64))
        | (1ULL << (sqlParser::SQL_NULL - 64))
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
        | (1ULL << (sqlParser::SUM - 64))
        | (1ULL << (sqlParser::TABLE - 64))
        | (1ULL << (sqlParser::SQL_TRUE - 64))
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
        | (1ULL << (sqlParser::TIMESTAMP - 64))
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::CHAR - 64))
        | (1ULL << (sqlParser::CHARACTER - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::BINARY - 130))
        | (1ULL << (sqlParser::VARBINARY - 130))
        | (1ULL << (sqlParser::VARYING - 130))
        | (1ULL << (sqlParser::BIT - 130))
        | (1ULL << (sqlParser::BITS - 130))
        | (1ULL << (sqlParser::BYTE - 130))
        | (1ULL << (sqlParser::BYTES - 130))
        | (1ULL << (sqlParser::CHARS - 130))
        | (1ULL << (sqlParser::DIGITS - 130))
        | (1ULL << (sqlParser::CASE - 130))
        | (1ULL << (sqlParser::CAST - 130))
        | (1ULL << (sqlParser::COALESCE - 130))
        | (1ULL << (sqlParser::CONCAT - 130))
        | (1ULL << (sqlParser::COUNT - 130))
        | (1ULL << (sqlParser::DECODE - 130))
        | (1ULL << (sqlParser::EXTRACT - 130))
        | (1ULL << (sqlParser::EXISTS - 130))
        | (1ULL << (sqlParser::IF - 130))
        | (1ULL << (sqlParser::IFNULL - 130))
        | (1ULL << (sqlParser::NULLIF - 130))
        | (1ULL << (sqlParser::POSITION - 130))
        | (1ULL << (sqlParser::SUBSTR - 130))
        | (1ULL << (sqlParser::SUBSTRING - 130))
        | (1ULL << (sqlParser::TRIM - 130))
        | (1ULL << (sqlParser::TYPEOF - 130))
        | (1ULL << (sqlParser::DAY - 130))
        | (1ULL << (sqlParser::DAYS - 130))
        | (1ULL << (sqlParser::HOUR - 130))
        | (1ULL << (sqlParser::HOURS - 130))
        | (1ULL << (sqlParser::MINUTE - 130))
        | (1ULL << (sqlParser::MINUTES - 130))
        | (1ULL << (sqlParser::MONTH - 130))
        | (1ULL << (sqlParser::MONTHS - 130))
        | (1ULL << (sqlParser::QUARTER - 130))
        | (1ULL << (sqlParser::QUARTERS - 130))
        | (1ULL << (sqlParser::SECOND - 130))
        | (1ULL << (sqlParser::SECONDS - 130))
        | (1ULL << (sqlParser::WEEK - 130))
        | (1ULL << (sqlParser::WEEKS - 130))
        | (1ULL << (sqlParser::YEAR - 130))
        | (1ULL << (sqlParser::YEARS - 130))
        | (1ULL << (sqlParser::BLOCK - 130))
        | (1ULL << (sqlParser::BLOOM - 130))
        | (1ULL << (sqlParser::BTREE - 130))
        | (1ULL << (sqlParser::HASH - 130))
        | (1ULL << (sqlParser::LOOP - 130))
        | (1ULL << (sqlParser::SKIPLIST - 130))
        | (1ULL << (sqlParser::TREE - 130))
        | (1ULL << (sqlParser::VALUE - 130))
        | (1ULL << (sqlParser::IDENTIFIER - 130))
        | (1ULL << (sqlParser::STRING_LITERAL - 130))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 130))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 130))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 130)))) != 0)) {
        setState(1198);
        expr(0);
        setState(1203);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1199);
          match(sqlParser::T__2);
          setState(1200);
          expr(0);
          setState(1205);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1208);
      match(sqlParser::T__7);
      break;
    }

    case 13: {
      _localctx = _tracker.createInstance<DecodeOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1209);
      match(sqlParser::DECODE);
      setState(1210);
      match(sqlParser::T__6);
      setState(1219);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__3)
        | (1ULL << sqlParser::T__6)
        | (1ULL << sqlParser::T__10)
        | (1ULL << sqlParser::ANTLR)
        | (1ULL << sqlParser::ASC)
        | (1ULL << sqlParser::AVG)
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
        | (1ULL << sqlParser::SQL_FALSE)
        | (1ULL << sqlParser::FEDERATION)
        | (1ULL << sqlParser::FORMAT)
        | (1ULL << sqlParser::INDEX)
        | (1ULL << sqlParser::INSERT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::INTO - 64))
        | (1ULL << (sqlParser::IS - 64))
        | (1ULL << (sqlParser::JOIN - 64))
        | (1ULL << (sqlParser::KEY - 64))
        | (1ULL << (sqlParser::LEFT - 64))
        | (1ULL << (sqlParser::MAX - 64))
        | (1ULL << (sqlParser::MIN - 64))
        | (1ULL << (sqlParser::MONOTONIC - 64))
        | (1ULL << (sqlParser::NO - 64))
        | (1ULL << (sqlParser::NODE - 64))
        | (1ULL << (sqlParser::NOT - 64))
        | (1ULL << (sqlParser::SQL_NULL - 64))
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
        | (1ULL << (sqlParser::SUM - 64))
        | (1ULL << (sqlParser::TABLE - 64))
        | (1ULL << (sqlParser::SQL_TRUE - 64))
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
        | (1ULL << (sqlParser::TIMESTAMP - 64))
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::CHAR - 64))
        | (1ULL << (sqlParser::CHARACTER - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::BINARY - 130))
        | (1ULL << (sqlParser::VARBINARY - 130))
        | (1ULL << (sqlParser::VARYING - 130))
        | (1ULL << (sqlParser::BIT - 130))
        | (1ULL << (sqlParser::BITS - 130))
        | (1ULL << (sqlParser::BYTE - 130))
        | (1ULL << (sqlParser::BYTES - 130))
        | (1ULL << (sqlParser::CHARS - 130))
        | (1ULL << (sqlParser::DIGITS - 130))
        | (1ULL << (sqlParser::CASE - 130))
        | (1ULL << (sqlParser::CAST - 130))
        | (1ULL << (sqlParser::COALESCE - 130))
        | (1ULL << (sqlParser::CONCAT - 130))
        | (1ULL << (sqlParser::COUNT - 130))
        | (1ULL << (sqlParser::DECODE - 130))
        | (1ULL << (sqlParser::EXTRACT - 130))
        | (1ULL << (sqlParser::EXISTS - 130))
        | (1ULL << (sqlParser::IF - 130))
        | (1ULL << (sqlParser::IFNULL - 130))
        | (1ULL << (sqlParser::NULLIF - 130))
        | (1ULL << (sqlParser::POSITION - 130))
        | (1ULL << (sqlParser::SUBSTR - 130))
        | (1ULL << (sqlParser::SUBSTRING - 130))
        | (1ULL << (sqlParser::TRIM - 130))
        | (1ULL << (sqlParser::TYPEOF - 130))
        | (1ULL << (sqlParser::DAY - 130))
        | (1ULL << (sqlParser::DAYS - 130))
        | (1ULL << (sqlParser::HOUR - 130))
        | (1ULL << (sqlParser::HOURS - 130))
        | (1ULL << (sqlParser::MINUTE - 130))
        | (1ULL << (sqlParser::MINUTES - 130))
        | (1ULL << (sqlParser::MONTH - 130))
        | (1ULL << (sqlParser::MONTHS - 130))
        | (1ULL << (sqlParser::QUARTER - 130))
        | (1ULL << (sqlParser::QUARTERS - 130))
        | (1ULL << (sqlParser::SECOND - 130))
        | (1ULL << (sqlParser::SECONDS - 130))
        | (1ULL << (sqlParser::WEEK - 130))
        | (1ULL << (sqlParser::WEEKS - 130))
        | (1ULL << (sqlParser::YEAR - 130))
        | (1ULL << (sqlParser::YEARS - 130))
        | (1ULL << (sqlParser::BLOCK - 130))
        | (1ULL << (sqlParser::BLOOM - 130))
        | (1ULL << (sqlParser::BTREE - 130))
        | (1ULL << (sqlParser::HASH - 130))
        | (1ULL << (sqlParser::LOOP - 130))
        | (1ULL << (sqlParser::SKIPLIST - 130))
        | (1ULL << (sqlParser::TREE - 130))
        | (1ULL << (sqlParser::VALUE - 130))
        | (1ULL << (sqlParser::IDENTIFIER - 130))
        | (1ULL << (sqlParser::STRING_LITERAL - 130))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 130))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 130))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 130)))) != 0)) {
        setState(1211);
        expr(0);
        setState(1216);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1212);
          match(sqlParser::T__2);
          setState(1213);
          expr(0);
          setState(1218);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1221);
      match(sqlParser::T__7);
      break;
    }

    case 14: {
      _localctx = _tracker.createInstance<ConcatOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1222);
      match(sqlParser::CONCAT);
      setState(1223);
      match(sqlParser::T__6);
      setState(1232);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__3)
        | (1ULL << sqlParser::T__6)
        | (1ULL << sqlParser::T__10)
        | (1ULL << sqlParser::ANTLR)
        | (1ULL << sqlParser::ASC)
        | (1ULL << sqlParser::AVG)
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
        | (1ULL << sqlParser::SQL_FALSE)
        | (1ULL << sqlParser::FEDERATION)
        | (1ULL << sqlParser::FORMAT)
        | (1ULL << sqlParser::INDEX)
        | (1ULL << sqlParser::INSERT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::INTO - 64))
        | (1ULL << (sqlParser::IS - 64))
        | (1ULL << (sqlParser::JOIN - 64))
        | (1ULL << (sqlParser::KEY - 64))
        | (1ULL << (sqlParser::LEFT - 64))
        | (1ULL << (sqlParser::MAX - 64))
        | (1ULL << (sqlParser::MIN - 64))
        | (1ULL << (sqlParser::MONOTONIC - 64))
        | (1ULL << (sqlParser::NO - 64))
        | (1ULL << (sqlParser::NODE - 64))
        | (1ULL << (sqlParser::NOT - 64))
        | (1ULL << (sqlParser::SQL_NULL - 64))
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
        | (1ULL << (sqlParser::SUM - 64))
        | (1ULL << (sqlParser::TABLE - 64))
        | (1ULL << (sqlParser::SQL_TRUE - 64))
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
        | (1ULL << (sqlParser::TIMESTAMP - 64))
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::CHAR - 64))
        | (1ULL << (sqlParser::CHARACTER - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::BINARY - 130))
        | (1ULL << (sqlParser::VARBINARY - 130))
        | (1ULL << (sqlParser::VARYING - 130))
        | (1ULL << (sqlParser::BIT - 130))
        | (1ULL << (sqlParser::BITS - 130))
        | (1ULL << (sqlParser::BYTE - 130))
        | (1ULL << (sqlParser::BYTES - 130))
        | (1ULL << (sqlParser::CHARS - 130))
        | (1ULL << (sqlParser::DIGITS - 130))
        | (1ULL << (sqlParser::CASE - 130))
        | (1ULL << (sqlParser::CAST - 130))
        | (1ULL << (sqlParser::COALESCE - 130))
        | (1ULL << (sqlParser::CONCAT - 130))
        | (1ULL << (sqlParser::COUNT - 130))
        | (1ULL << (sqlParser::DECODE - 130))
        | (1ULL << (sqlParser::EXTRACT - 130))
        | (1ULL << (sqlParser::EXISTS - 130))
        | (1ULL << (sqlParser::IF - 130))
        | (1ULL << (sqlParser::IFNULL - 130))
        | (1ULL << (sqlParser::NULLIF - 130))
        | (1ULL << (sqlParser::POSITION - 130))
        | (1ULL << (sqlParser::SUBSTR - 130))
        | (1ULL << (sqlParser::SUBSTRING - 130))
        | (1ULL << (sqlParser::TRIM - 130))
        | (1ULL << (sqlParser::TYPEOF - 130))
        | (1ULL << (sqlParser::DAY - 130))
        | (1ULL << (sqlParser::DAYS - 130))
        | (1ULL << (sqlParser::HOUR - 130))
        | (1ULL << (sqlParser::HOURS - 130))
        | (1ULL << (sqlParser::MINUTE - 130))
        | (1ULL << (sqlParser::MINUTES - 130))
        | (1ULL << (sqlParser::MONTH - 130))
        | (1ULL << (sqlParser::MONTHS - 130))
        | (1ULL << (sqlParser::QUARTER - 130))
        | (1ULL << (sqlParser::QUARTERS - 130))
        | (1ULL << (sqlParser::SECOND - 130))
        | (1ULL << (sqlParser::SECONDS - 130))
        | (1ULL << (sqlParser::WEEK - 130))
        | (1ULL << (sqlParser::WEEKS - 130))
        | (1ULL << (sqlParser::YEAR - 130))
        | (1ULL << (sqlParser::YEARS - 130))
        | (1ULL << (sqlParser::BLOCK - 130))
        | (1ULL << (sqlParser::BLOOM - 130))
        | (1ULL << (sqlParser::BTREE - 130))
        | (1ULL << (sqlParser::HASH - 130))
        | (1ULL << (sqlParser::LOOP - 130))
        | (1ULL << (sqlParser::SKIPLIST - 130))
        | (1ULL << (sqlParser::TREE - 130))
        | (1ULL << (sqlParser::VALUE - 130))
        | (1ULL << (sqlParser::IDENTIFIER - 130))
        | (1ULL << (sqlParser::STRING_LITERAL - 130))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 130))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 130))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 130)))) != 0)) {
        setState(1224);
        expr(0);
        setState(1229);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1225);
          match(sqlParser::T__2);
          setState(1226);
          expr(0);
          setState(1231);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1234);
      match(sqlParser::T__7);
      break;
    }

    case 15: {
      _localctx = _tracker.createInstance<OverlayFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1235);
      match(sqlParser::OVERLAY);
      setState(1236);
      match(sqlParser::T__6);
      setState(1237);
      expr(0);
      setState(1238);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2 || _la == sqlParser::PLACING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1239);
      expr(0);
      setState(1240);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2

      || _la == sqlParser::FROM)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1241);
      expr(0);
      setState(1244);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__2

      || _la == sqlParser::FOR) {
        setState(1242);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__2

        || _la == sqlParser::FOR)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(1243);
        expr(0);
      }
      setState(1246);
      match(sqlParser::T__7);
      break;
    }

    case 16: {
      _localctx = _tracker.createInstance<PositionFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1248);
      match(sqlParser::POSITION);
      setState(1249);
      match(sqlParser::T__6);
      setState(1250);
      expr(0);
      setState(1251);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2

      || _la == sqlParser::IN)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1252);
      expr(0);
      setState(1253);
      match(sqlParser::T__7);
      break;
    }

    case 17: {
      _localctx = _tracker.createInstance<SubStringFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1255);
      _la = _input->LA(1);
      if (!(_la == sqlParser::SUBSTR

      || _la == sqlParser::SUBSTRING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1256);
      match(sqlParser::T__6);
      setState(1271);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 175, _ctx)) {
      case 1: {
        setState(1257);
        expr(0);
        setState(1258);
        match(sqlParser::T__2);
        setState(1259);
        expr(0);
        setState(1262);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__2) {
          setState(1260);
          match(sqlParser::T__2);
          setState(1261);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      case 2: {
        setState(1264);
        expr(0);
        setState(1265);
        match(sqlParser::FROM);
        setState(1266);
        expr(0);
        setState(1269);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::FOR) {
          setState(1267);
          match(sqlParser::FOR);
          setState(1268);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      }
      setState(1273);
      match(sqlParser::T__7);
      break;
    }

    case 18: {
      _localctx = _tracker.createInstance<TrimFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1275);
      match(sqlParser::TRIM);
      setState(1276);
      match(sqlParser::T__6);
      setState(1290);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 179, _ctx)) {
      case 1: {
        setState(1277);
        expr(0);
        setState(1280);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__2) {
          setState(1278);
          match(sqlParser::T__2);
          setState(1279);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        break;
      }

      case 2: {
        setState(1283);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 177, _ctx)) {
        case 1: {
          setState(1282);
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
        setState(1286);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << sqlParser::T__3)
          | (1ULL << sqlParser::T__6)
          | (1ULL << sqlParser::T__10)
          | (1ULL << sqlParser::ANTLR)
          | (1ULL << sqlParser::ASC)
          | (1ULL << sqlParser::AVG)
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
          | (1ULL << sqlParser::SQL_FALSE)
          | (1ULL << sqlParser::FEDERATION)
          | (1ULL << sqlParser::FORMAT)
          | (1ULL << sqlParser::INDEX)
          | (1ULL << sqlParser::INSERT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::INTO - 64))
          | (1ULL << (sqlParser::IS - 64))
          | (1ULL << (sqlParser::JOIN - 64))
          | (1ULL << (sqlParser::KEY - 64))
          | (1ULL << (sqlParser::LEFT - 64))
          | (1ULL << (sqlParser::MAX - 64))
          | (1ULL << (sqlParser::MIN - 64))
          | (1ULL << (sqlParser::MONOTONIC - 64))
          | (1ULL << (sqlParser::NO - 64))
          | (1ULL << (sqlParser::NODE - 64))
          | (1ULL << (sqlParser::NOT - 64))
          | (1ULL << (sqlParser::SQL_NULL - 64))
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
          | (1ULL << (sqlParser::SUM - 64))
          | (1ULL << (sqlParser::TABLE - 64))
          | (1ULL << (sqlParser::SQL_TRUE - 64))
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
          | (1ULL << (sqlParser::TIMESTAMP - 64))
          | (1ULL << (sqlParser::INTERVAL - 64))
          | (1ULL << (sqlParser::CHAR - 64))
          | (1ULL << (sqlParser::CHARACTER - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::BINARY - 130))
          | (1ULL << (sqlParser::VARBINARY - 130))
          | (1ULL << (sqlParser::VARYING - 130))
          | (1ULL << (sqlParser::BIT - 130))
          | (1ULL << (sqlParser::BITS - 130))
          | (1ULL << (sqlParser::BYTE - 130))
          | (1ULL << (sqlParser::BYTES - 130))
          | (1ULL << (sqlParser::CHARS - 130))
          | (1ULL << (sqlParser::DIGITS - 130))
          | (1ULL << (sqlParser::CASE - 130))
          | (1ULL << (sqlParser::CAST - 130))
          | (1ULL << (sqlParser::COALESCE - 130))
          | (1ULL << (sqlParser::CONCAT - 130))
          | (1ULL << (sqlParser::COUNT - 130))
          | (1ULL << (sqlParser::DECODE - 130))
          | (1ULL << (sqlParser::EXTRACT - 130))
          | (1ULL << (sqlParser::EXISTS - 130))
          | (1ULL << (sqlParser::IF - 130))
          | (1ULL << (sqlParser::IFNULL - 130))
          | (1ULL << (sqlParser::NULLIF - 130))
          | (1ULL << (sqlParser::POSITION - 130))
          | (1ULL << (sqlParser::SUBSTR - 130))
          | (1ULL << (sqlParser::SUBSTRING - 130))
          | (1ULL << (sqlParser::TRIM - 130))
          | (1ULL << (sqlParser::TYPEOF - 130))
          | (1ULL << (sqlParser::DAY - 130))
          | (1ULL << (sqlParser::DAYS - 130))
          | (1ULL << (sqlParser::HOUR - 130))
          | (1ULL << (sqlParser::HOURS - 130))
          | (1ULL << (sqlParser::MINUTE - 130))
          | (1ULL << (sqlParser::MINUTES - 130))
          | (1ULL << (sqlParser::MONTH - 130))
          | (1ULL << (sqlParser::MONTHS - 130))
          | (1ULL << (sqlParser::QUARTER - 130))
          | (1ULL << (sqlParser::QUARTERS - 130))
          | (1ULL << (sqlParser::SECOND - 130))
          | (1ULL << (sqlParser::SECONDS - 130))
          | (1ULL << (sqlParser::WEEK - 130))
          | (1ULL << (sqlParser::WEEKS - 130))
          | (1ULL << (sqlParser::YEAR - 130))
          | (1ULL << (sqlParser::YEARS - 130))
          | (1ULL << (sqlParser::BLOCK - 130))
          | (1ULL << (sqlParser::BLOOM - 130))
          | (1ULL << (sqlParser::BTREE - 130))
          | (1ULL << (sqlParser::HASH - 130))
          | (1ULL << (sqlParser::LOOP - 130))
          | (1ULL << (sqlParser::SKIPLIST - 130))
          | (1ULL << (sqlParser::TREE - 130))
          | (1ULL << (sqlParser::VALUE - 130))
          | (1ULL << (sqlParser::IDENTIFIER - 130))
          | (1ULL << (sqlParser::STRING_LITERAL - 130))
          | (1ULL << (sqlParser::FLOAT_LITERAL - 130))
          | (1ULL << (sqlParser::DECIMAL_LITERAL - 130))
          | (1ULL << (sqlParser::INTEGER_LITERAL - 130)))) != 0)) {
          setState(1285);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        setState(1288);
        match(sqlParser::FROM);
        setState(1289);
        expr(0);
        break;
      }

      }
      setState(1292);
      match(sqlParser::T__7);
      break;
    }

    case 19: {
      _localctx = _tracker.createInstance<ExtractFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1294);
      match(sqlParser::EXTRACT);
      setState(1295);
      match(sqlParser::T__6);
      setState(1296);
      _la = _input->LA(1);
      if (!(((((_la - 160) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 160)) & ((1ULL << (sqlParser::DAY - 160))
        | (1ULL << (sqlParser::HOUR - 160))
        | (1ULL << (sqlParser::MINUTE - 160))
        | (1ULL << (sqlParser::MONTH - 160))
        | (1ULL << (sqlParser::QUARTER - 160))
        | (1ULL << (sqlParser::SECOND - 160))
        | (1ULL << (sqlParser::WEEK - 160))
        | (1ULL << (sqlParser::YEAR - 160)))) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1297);
      match(sqlParser::FROM);
      setState(1298);
      expr(0);
      setState(1299);
      match(sqlParser::T__7);
      break;
    }

    case 20: {
      _localctx = _tracker.createInstance<CountAllContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1301);
      match(sqlParser::COUNT);
      setState(1302);
      match(sqlParser::T__6);
      setState(1304);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ALL) {
        setState(1303);
        match(sqlParser::ALL);
      }
      setState(1306);
      match(sqlParser::T__4);
      setState(1307);
      match(sqlParser::T__7);
      break;
    }

    case 21: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1308);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::COUNT);
      setState(1309);
      match(sqlParser::T__6);
      setState(1312);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1310);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1311);
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
      setState(1314);
      expr(0);
      setState(1315);
      match(sqlParser::T__7);
      break;
    }

    case 22: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1317);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::SUM);
      setState(1318);
      match(sqlParser::T__6);
      setState(1321);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1319);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1320);
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
      setState(1323);
      expr(0);
      setState(1324);
      match(sqlParser::T__7);
      break;
    }

    case 23: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1326);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::AVG);
      setState(1327);
      match(sqlParser::T__6);
      setState(1330);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1328);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1329);
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
      setState(1332);
      expr(0);
      setState(1333);
      match(sqlParser::T__7);
      break;
    }

    case 24: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1335);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MIN);
      setState(1336);
      match(sqlParser::T__6);
      setState(1339);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1337);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1338);
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
      setState(1341);
      expr(0);
      setState(1342);
      match(sqlParser::T__7);
      break;
    }

    case 25: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1344);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MAX);
      setState(1345);
      match(sqlParser::T__6);
      setState(1348);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1346);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1347);
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
      setState(1350);
      expr(0);
      setState(1351);
      match(sqlParser::T__7);
      break;
    }

    case 26: {
      _localctx = _tracker.createInstance<ScalarFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1353);
      dynamic_cast<ScalarFuncContext *>(_localctx)->func = functionName();
      setState(1354);
      match(sqlParser::T__6);
      setState(1363);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__3)
        | (1ULL << sqlParser::T__6)
        | (1ULL << sqlParser::T__10)
        | (1ULL << sqlParser::ANTLR)
        | (1ULL << sqlParser::ASC)
        | (1ULL << sqlParser::AVG)
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
        | (1ULL << sqlParser::SQL_FALSE)
        | (1ULL << sqlParser::FEDERATION)
        | (1ULL << sqlParser::FORMAT)
        | (1ULL << sqlParser::INDEX)
        | (1ULL << sqlParser::INSERT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::INTO - 64))
        | (1ULL << (sqlParser::IS - 64))
        | (1ULL << (sqlParser::JOIN - 64))
        | (1ULL << (sqlParser::KEY - 64))
        | (1ULL << (sqlParser::LEFT - 64))
        | (1ULL << (sqlParser::MAX - 64))
        | (1ULL << (sqlParser::MIN - 64))
        | (1ULL << (sqlParser::MONOTONIC - 64))
        | (1ULL << (sqlParser::NO - 64))
        | (1ULL << (sqlParser::NODE - 64))
        | (1ULL << (sqlParser::NOT - 64))
        | (1ULL << (sqlParser::SQL_NULL - 64))
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
        | (1ULL << (sqlParser::SUM - 64))
        | (1ULL << (sqlParser::TABLE - 64))
        | (1ULL << (sqlParser::SQL_TRUE - 64))
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
        | (1ULL << (sqlParser::TIMESTAMP - 64))
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::CHAR - 64))
        | (1ULL << (sqlParser::CHARACTER - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::BINARY - 130))
        | (1ULL << (sqlParser::VARBINARY - 130))
        | (1ULL << (sqlParser::VARYING - 130))
        | (1ULL << (sqlParser::BIT - 130))
        | (1ULL << (sqlParser::BITS - 130))
        | (1ULL << (sqlParser::BYTE - 130))
        | (1ULL << (sqlParser::BYTES - 130))
        | (1ULL << (sqlParser::CHARS - 130))
        | (1ULL << (sqlParser::DIGITS - 130))
        | (1ULL << (sqlParser::CASE - 130))
        | (1ULL << (sqlParser::CAST - 130))
        | (1ULL << (sqlParser::COALESCE - 130))
        | (1ULL << (sqlParser::CONCAT - 130))
        | (1ULL << (sqlParser::COUNT - 130))
        | (1ULL << (sqlParser::DECODE - 130))
        | (1ULL << (sqlParser::EXTRACT - 130))
        | (1ULL << (sqlParser::EXISTS - 130))
        | (1ULL << (sqlParser::IF - 130))
        | (1ULL << (sqlParser::IFNULL - 130))
        | (1ULL << (sqlParser::NULLIF - 130))
        | (1ULL << (sqlParser::POSITION - 130))
        | (1ULL << (sqlParser::SUBSTR - 130))
        | (1ULL << (sqlParser::SUBSTRING - 130))
        | (1ULL << (sqlParser::TRIM - 130))
        | (1ULL << (sqlParser::TYPEOF - 130))
        | (1ULL << (sqlParser::DAY - 130))
        | (1ULL << (sqlParser::DAYS - 130))
        | (1ULL << (sqlParser::HOUR - 130))
        | (1ULL << (sqlParser::HOURS - 130))
        | (1ULL << (sqlParser::MINUTE - 130))
        | (1ULL << (sqlParser::MINUTES - 130))
        | (1ULL << (sqlParser::MONTH - 130))
        | (1ULL << (sqlParser::MONTHS - 130))
        | (1ULL << (sqlParser::QUARTER - 130))
        | (1ULL << (sqlParser::QUARTERS - 130))
        | (1ULL << (sqlParser::SECOND - 130))
        | (1ULL << (sqlParser::SECONDS - 130))
        | (1ULL << (sqlParser::WEEK - 130))
        | (1ULL << (sqlParser::WEEKS - 130))
        | (1ULL << (sqlParser::YEAR - 130))
        | (1ULL << (sqlParser::YEARS - 130))
        | (1ULL << (sqlParser::BLOCK - 130))
        | (1ULL << (sqlParser::BLOOM - 130))
        | (1ULL << (sqlParser::BTREE - 130))
        | (1ULL << (sqlParser::HASH - 130))
        | (1ULL << (sqlParser::LOOP - 130))
        | (1ULL << (sqlParser::SKIPLIST - 130))
        | (1ULL << (sqlParser::TREE - 130))
        | (1ULL << (sqlParser::VALUE - 130))
        | (1ULL << (sqlParser::IDENTIFIER - 130))
        | (1ULL << (sqlParser::STRING_LITERAL - 130))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 130))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 130))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 130)))) != 0)) {
        setState(1355);
        expr(0);
        setState(1360);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1356);
          match(sqlParser::T__2);
          setState(1357);
          expr(0);
          setState(1362);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1365);
      match(sqlParser::T__7);
      break;
    }

    case 27: {
      _localctx = _tracker.createInstance<ParentheticContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1367);
      match(sqlParser::T__6);
      setState(1368);
      expr(0);
      setState(1369);
      match(sqlParser::T__7);
      break;
    }

    case 28: {
      _localctx = _tracker.createInstance<SubqueryExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1371);
      match(sqlParser::T__6);
      setState(1372);
      query();
      setState(1373);
      match(sqlParser::T__7);
      break;
    }

    case 29: {
      _localctx = _tracker.createInstance<SubqueryExistsOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1376);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::NOT) {
        setState(1375);
        match(sqlParser::NOT);
      }
      setState(1378);
      match(sqlParser::EXISTS);

      setState(1379);
      match(sqlParser::T__6);
      setState(1380);
      query();
      setState(1381);
      match(sqlParser::T__7);
      break;
    }

    case 30: {
      _localctx = _tracker.createInstance<LogicOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1383);
      dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::NOT);
      setState(1384);
      expr(4);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(1470);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 200, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(1468);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 199, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1387);

          if (!(precpred(_ctx, 36))) throw FailedPredicateException(this, "precpred(_ctx, 36)");
          setState(1388);
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
          setState(1389);
          expr(37);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1390);

          if (!(precpred(_ctx, 35))) throw FailedPredicateException(this, "precpred(_ctx, 35)");
          setState(1391);
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
          setState(1392);
          expr(36);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1393);

          if (!(precpred(_ctx, 34))) throw FailedPredicateException(this, "precpred(_ctx, 34)");
          setState(1394);
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
          setState(1395);
          expr(35);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<LikeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1396);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(1398);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1397);
            match(sqlParser::NOT);
          }
          setState(1400);
          match(sqlParser::LIKE);
          setState(1401);
          expr(10);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<LikeEscapeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1402);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(1404);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1403);
            match(sqlParser::NOT);
          }
          setState(1406);
          match(sqlParser::LIKE);
          setState(1407);
          expr(0);
          setState(1408);
          match(sqlParser::ESCAPE);
          setState(1409);
          expr(9);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1411);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(1412);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::AND);
          setState(1413);
          expr(4);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1414);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(1415);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::OR);
          setState(1416);
          expr(3);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1417);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(1418);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::XOR);
          setState(1419);
          expr(2);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<MethodCallContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1420);

          if (!(precpred(_ctx, 37))) throw FailedPredicateException(this, "precpred(_ctx, 37)");
          setState(1421);
          match(sqlParser::T__9);
          setState(1422);
          dynamic_cast<MethodCallContext *>(_localctx)->func = methodName();
          setState(1423);
          match(sqlParser::T__6);
          setState(1432);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__3)
            | (1ULL << sqlParser::T__6)
            | (1ULL << sqlParser::T__10)
            | (1ULL << sqlParser::ANTLR)
            | (1ULL << sqlParser::ASC)
            | (1ULL << sqlParser::AVG)
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
            | (1ULL << sqlParser::SQL_FALSE)
            | (1ULL << sqlParser::FEDERATION)
            | (1ULL << sqlParser::FORMAT)
            | (1ULL << sqlParser::INDEX)
            | (1ULL << sqlParser::INSERT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::INTO - 64))
            | (1ULL << (sqlParser::IS - 64))
            | (1ULL << (sqlParser::JOIN - 64))
            | (1ULL << (sqlParser::KEY - 64))
            | (1ULL << (sqlParser::LEFT - 64))
            | (1ULL << (sqlParser::MAX - 64))
            | (1ULL << (sqlParser::MIN - 64))
            | (1ULL << (sqlParser::MONOTONIC - 64))
            | (1ULL << (sqlParser::NO - 64))
            | (1ULL << (sqlParser::NODE - 64))
            | (1ULL << (sqlParser::NOT - 64))
            | (1ULL << (sqlParser::SQL_NULL - 64))
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
            | (1ULL << (sqlParser::SUM - 64))
            | (1ULL << (sqlParser::TABLE - 64))
            | (1ULL << (sqlParser::SQL_TRUE - 64))
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
            | (1ULL << (sqlParser::TIMESTAMP - 64))
            | (1ULL << (sqlParser::INTERVAL - 64))
            | (1ULL << (sqlParser::CHAR - 64))
            | (1ULL << (sqlParser::CHARACTER - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::BINARY - 130))
            | (1ULL << (sqlParser::VARBINARY - 130))
            | (1ULL << (sqlParser::VARYING - 130))
            | (1ULL << (sqlParser::BIT - 130))
            | (1ULL << (sqlParser::BITS - 130))
            | (1ULL << (sqlParser::BYTE - 130))
            | (1ULL << (sqlParser::BYTES - 130))
            | (1ULL << (sqlParser::CHARS - 130))
            | (1ULL << (sqlParser::DIGITS - 130))
            | (1ULL << (sqlParser::CASE - 130))
            | (1ULL << (sqlParser::CAST - 130))
            | (1ULL << (sqlParser::COALESCE - 130))
            | (1ULL << (sqlParser::CONCAT - 130))
            | (1ULL << (sqlParser::COUNT - 130))
            | (1ULL << (sqlParser::DECODE - 130))
            | (1ULL << (sqlParser::EXTRACT - 130))
            | (1ULL << (sqlParser::EXISTS - 130))
            | (1ULL << (sqlParser::IF - 130))
            | (1ULL << (sqlParser::IFNULL - 130))
            | (1ULL << (sqlParser::NULLIF - 130))
            | (1ULL << (sqlParser::POSITION - 130))
            | (1ULL << (sqlParser::SUBSTR - 130))
            | (1ULL << (sqlParser::SUBSTRING - 130))
            | (1ULL << (sqlParser::TRIM - 130))
            | (1ULL << (sqlParser::TYPEOF - 130))
            | (1ULL << (sqlParser::DAY - 130))
            | (1ULL << (sqlParser::DAYS - 130))
            | (1ULL << (sqlParser::HOUR - 130))
            | (1ULL << (sqlParser::HOURS - 130))
            | (1ULL << (sqlParser::MINUTE - 130))
            | (1ULL << (sqlParser::MINUTES - 130))
            | (1ULL << (sqlParser::MONTH - 130))
            | (1ULL << (sqlParser::MONTHS - 130))
            | (1ULL << (sqlParser::QUARTER - 130))
            | (1ULL << (sqlParser::QUARTERS - 130))
            | (1ULL << (sqlParser::SECOND - 130))
            | (1ULL << (sqlParser::SECONDS - 130))
            | (1ULL << (sqlParser::WEEK - 130))
            | (1ULL << (sqlParser::WEEKS - 130))
            | (1ULL << (sqlParser::YEAR - 130))
            | (1ULL << (sqlParser::YEARS - 130))
            | (1ULL << (sqlParser::BLOCK - 130))
            | (1ULL << (sqlParser::BLOOM - 130))
            | (1ULL << (sqlParser::BTREE - 130))
            | (1ULL << (sqlParser::HASH - 130))
            | (1ULL << (sqlParser::LOOP - 130))
            | (1ULL << (sqlParser::SKIPLIST - 130))
            | (1ULL << (sqlParser::TREE - 130))
            | (1ULL << (sqlParser::VALUE - 130))
            | (1ULL << (sqlParser::IDENTIFIER - 130))
            | (1ULL << (sqlParser::STRING_LITERAL - 130))
            | (1ULL << (sqlParser::FLOAT_LITERAL - 130))
            | (1ULL << (sqlParser::DECIMAL_LITERAL - 130))
            | (1ULL << (sqlParser::INTEGER_LITERAL - 130)))) != 0)) {
            setState(1424);
            expr(0);
            setState(1429);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__2) {
              setState(1425);
              match(sqlParser::T__2);
              setState(1426);
              expr(0);
              setState(1431);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1434);
          match(sqlParser::T__7);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<IsNullOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1436);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(1437);
          match(sqlParser::IS);
          setState(1439);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1438);
            match(sqlParser::NOT);
          }
          setState(1441);
          match(sqlParser::SQL_NULL);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<SubqueryInOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1442);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(1444);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1443);
            match(sqlParser::NOT);
          }
          setState(1446);
          match(sqlParser::IN);

          setState(1447);
          match(sqlParser::T__6);
          setState(1448);
          query();
          setState(1449);
          match(sqlParser::T__7);
          break;
        }

        case 12: {
          auto newContext = _tracker.createInstance<InOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1451);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(1453);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1452);
            match(sqlParser::NOT);
          }
          setState(1455);
          match(sqlParser::IN);

          setState(1456);
          match(sqlParser::T__6);
          setState(1465);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__3)
            | (1ULL << sqlParser::T__6)
            | (1ULL << sqlParser::T__10)
            | (1ULL << sqlParser::ANTLR)
            | (1ULL << sqlParser::ASC)
            | (1ULL << sqlParser::AVG)
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
            | (1ULL << sqlParser::SQL_FALSE)
            | (1ULL << sqlParser::FEDERATION)
            | (1ULL << sqlParser::FORMAT)
            | (1ULL << sqlParser::INDEX)
            | (1ULL << sqlParser::INSERT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::INTO - 64))
            | (1ULL << (sqlParser::IS - 64))
            | (1ULL << (sqlParser::JOIN - 64))
            | (1ULL << (sqlParser::KEY - 64))
            | (1ULL << (sqlParser::LEFT - 64))
            | (1ULL << (sqlParser::MAX - 64))
            | (1ULL << (sqlParser::MIN - 64))
            | (1ULL << (sqlParser::MONOTONIC - 64))
            | (1ULL << (sqlParser::NO - 64))
            | (1ULL << (sqlParser::NODE - 64))
            | (1ULL << (sqlParser::NOT - 64))
            | (1ULL << (sqlParser::SQL_NULL - 64))
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
            | (1ULL << (sqlParser::SUM - 64))
            | (1ULL << (sqlParser::TABLE - 64))
            | (1ULL << (sqlParser::SQL_TRUE - 64))
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
            | (1ULL << (sqlParser::TIMESTAMP - 64))
            | (1ULL << (sqlParser::INTERVAL - 64))
            | (1ULL << (sqlParser::CHAR - 64))
            | (1ULL << (sqlParser::CHARACTER - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::BINARY - 130))
            | (1ULL << (sqlParser::VARBINARY - 130))
            | (1ULL << (sqlParser::VARYING - 130))
            | (1ULL << (sqlParser::BIT - 130))
            | (1ULL << (sqlParser::BITS - 130))
            | (1ULL << (sqlParser::BYTE - 130))
            | (1ULL << (sqlParser::BYTES - 130))
            | (1ULL << (sqlParser::CHARS - 130))
            | (1ULL << (sqlParser::DIGITS - 130))
            | (1ULL << (sqlParser::CASE - 130))
            | (1ULL << (sqlParser::CAST - 130))
            | (1ULL << (sqlParser::COALESCE - 130))
            | (1ULL << (sqlParser::CONCAT - 130))
            | (1ULL << (sqlParser::COUNT - 130))
            | (1ULL << (sqlParser::DECODE - 130))
            | (1ULL << (sqlParser::EXTRACT - 130))
            | (1ULL << (sqlParser::EXISTS - 130))
            | (1ULL << (sqlParser::IF - 130))
            | (1ULL << (sqlParser::IFNULL - 130))
            | (1ULL << (sqlParser::NULLIF - 130))
            | (1ULL << (sqlParser::POSITION - 130))
            | (1ULL << (sqlParser::SUBSTR - 130))
            | (1ULL << (sqlParser::SUBSTRING - 130))
            | (1ULL << (sqlParser::TRIM - 130))
            | (1ULL << (sqlParser::TYPEOF - 130))
            | (1ULL << (sqlParser::DAY - 130))
            | (1ULL << (sqlParser::DAYS - 130))
            | (1ULL << (sqlParser::HOUR - 130))
            | (1ULL << (sqlParser::HOURS - 130))
            | (1ULL << (sqlParser::MINUTE - 130))
            | (1ULL << (sqlParser::MINUTES - 130))
            | (1ULL << (sqlParser::MONTH - 130))
            | (1ULL << (sqlParser::MONTHS - 130))
            | (1ULL << (sqlParser::QUARTER - 130))
            | (1ULL << (sqlParser::QUARTERS - 130))
            | (1ULL << (sqlParser::SECOND - 130))
            | (1ULL << (sqlParser::SECONDS - 130))
            | (1ULL << (sqlParser::WEEK - 130))
            | (1ULL << (sqlParser::WEEKS - 130))
            | (1ULL << (sqlParser::YEAR - 130))
            | (1ULL << (sqlParser::YEARS - 130))
            | (1ULL << (sqlParser::BLOCK - 130))
            | (1ULL << (sqlParser::BLOOM - 130))
            | (1ULL << (sqlParser::BTREE - 130))
            | (1ULL << (sqlParser::HASH - 130))
            | (1ULL << (sqlParser::LOOP - 130))
            | (1ULL << (sqlParser::SKIPLIST - 130))
            | (1ULL << (sqlParser::TREE - 130))
            | (1ULL << (sqlParser::VALUE - 130))
            | (1ULL << (sqlParser::IDENTIFIER - 130))
            | (1ULL << (sqlParser::STRING_LITERAL - 130))
            | (1ULL << (sqlParser::FLOAT_LITERAL - 130))
            | (1ULL << (sqlParser::DECIMAL_LITERAL - 130))
            | (1ULL << (sqlParser::INTEGER_LITERAL - 130)))) != 0)) {
            setState(1457);
            expr(0);
            setState(1462);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__2) {
              setState(1458);
              match(sqlParser::T__2);
              setState(1459);
              expr(0);
              setState(1464);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1467);
          match(sqlParser::T__7);
          break;
        }

        } 
      }
      setState(1472);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 200, _ctx);
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
  enterRule(_localctx, 120, sqlParser::RuleVariableName);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1473);
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
  enterRule(_localctx, 122, sqlParser::RuleColumnRef);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1488);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 203, _ctx)) {
    case 1: {
      setState(1483);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 202, _ctx)) {
      case 1: {
        setState(1478);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 201, _ctx)) {
        case 1: {
          setState(1475);
          catalogName();
          setState(1476);
          match(sqlParser::T__9);
          break;
        }

        }

        setState(1480);
        schemaName();
        setState(1481);
        match(sqlParser::T__9);
        break;
      }

      }

      setState(1485);
      tableName();
      setState(1486);
      match(sqlParser::T__9);
      break;
    }

    }
    setState(1490);
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
  enterRule(_localctx, 124, sqlParser::RuleColumnWildcard);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1505);
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
      | (1ULL << (sqlParser::TIMESTAMP - 64))
      | (1ULL << (sqlParser::CHAR - 64))
      | (1ULL << (sqlParser::CHARACTER - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::BINARY - 130))
      | (1ULL << (sqlParser::VARBINARY - 130))
      | (1ULL << (sqlParser::VARYING - 130))
      | (1ULL << (sqlParser::BIT - 130))
      | (1ULL << (sqlParser::BITS - 130))
      | (1ULL << (sqlParser::BYTE - 130))
      | (1ULL << (sqlParser::BYTES - 130))
      | (1ULL << (sqlParser::CHARS - 130))
      | (1ULL << (sqlParser::DIGITS - 130))
      | (1ULL << (sqlParser::COALESCE - 130))
      | (1ULL << (sqlParser::CONCAT - 130))
      | (1ULL << (sqlParser::COUNT - 130))
      | (1ULL << (sqlParser::DECODE - 130))
      | (1ULL << (sqlParser::EXTRACT - 130))
      | (1ULL << (sqlParser::IFNULL - 130))
      | (1ULL << (sqlParser::NULLIF - 130))
      | (1ULL << (sqlParser::POSITION - 130))
      | (1ULL << (sqlParser::SUBSTR - 130))
      | (1ULL << (sqlParser::SUBSTRING - 130))
      | (1ULL << (sqlParser::DAY - 130))
      | (1ULL << (sqlParser::DAYS - 130))
      | (1ULL << (sqlParser::HOUR - 130))
      | (1ULL << (sqlParser::HOURS - 130))
      | (1ULL << (sqlParser::MINUTE - 130))
      | (1ULL << (sqlParser::MINUTES - 130))
      | (1ULL << (sqlParser::MONTH - 130))
      | (1ULL << (sqlParser::MONTHS - 130))
      | (1ULL << (sqlParser::QUARTER - 130))
      | (1ULL << (sqlParser::QUARTERS - 130))
      | (1ULL << (sqlParser::SECOND - 130))
      | (1ULL << (sqlParser::SECONDS - 130))
      | (1ULL << (sqlParser::WEEK - 130))
      | (1ULL << (sqlParser::WEEKS - 130))
      | (1ULL << (sqlParser::YEAR - 130))
      | (1ULL << (sqlParser::YEARS - 130))
      | (1ULL << (sqlParser::BLOCK - 130))
      | (1ULL << (sqlParser::BLOOM - 130))
      | (1ULL << (sqlParser::BTREE - 130))
      | (1ULL << (sqlParser::HASH - 130))
      | (1ULL << (sqlParser::LOOP - 130))
      | (1ULL << (sqlParser::SKIPLIST - 130))
      | (1ULL << (sqlParser::TREE - 130))
      | (1ULL << (sqlParser::VALUE - 130))
      | (1ULL << (sqlParser::IDENTIFIER - 130)))) != 0)) {
      setState(1500);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 205, _ctx)) {
      case 1: {
        setState(1495);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 204, _ctx)) {
        case 1: {
          setState(1492);
          catalogName();
          setState(1493);
          match(sqlParser::T__9);
          break;
        }

        }

        setState(1497);
        schemaName();
        setState(1498);
        match(sqlParser::T__9);
        break;
      }

      }

      setState(1502);
      tableName();
      setState(1503);
      match(sqlParser::T__9);
    }
    setState(1507);
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
  enterRule(_localctx, 126, sqlParser::RuleClassName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1514);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 207, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1509);
        qualifier();
        setState(1510);
        match(sqlParser::T__9); 
      }
      setState(1516);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 207, _ctx);
    }
    setState(1517);
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
  enterRule(_localctx, 128, sqlParser::RuleFunctionName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1519);
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
  enterRule(_localctx, 130, sqlParser::RuleMethodName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1521);
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
  enterRule(_localctx, 132, sqlParser::RuleCatalogName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1523);
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
  enterRule(_localctx, 134, sqlParser::RuleSchemaName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1525);
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
  enterRule(_localctx, 136, sqlParser::RuleTableName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1527);
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
  enterRule(_localctx, 138, sqlParser::RuleTableAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1529);
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
  enterRule(_localctx, 140, sqlParser::RuleColumnName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1531);
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
  enterRule(_localctx, 142, sqlParser::RuleColumnAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1533);
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
  enterRule(_localctx, 144, sqlParser::RuleCollationName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1535);
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
  enterRule(_localctx, 146, sqlParser::RuleIndexName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1537);
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
  enterRule(_localctx, 148, sqlParser::RulePropertyName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1539);
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
  enterRule(_localctx, 150, sqlParser::RuleHintName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1541);
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
  enterRule(_localctx, 152, sqlParser::RulePropertyValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1545);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::SQL_FALSE:
      case sqlParser::SQL_NULL:
      case sqlParser::SQL_TRUE:
      case sqlParser::STRING_LITERAL:
      case sqlParser::FLOAT_LITERAL:
      case sqlParser::DECIMAL_LITERAL:
      case sqlParser::INTEGER_LITERAL: {
        setState(1543);
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
        setState(1544);
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
  enterRule(_localctx, 154, sqlParser::RuleHintValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1549);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::SQL_FALSE:
      case sqlParser::SQL_NULL:
      case sqlParser::SQL_TRUE:
      case sqlParser::STRING_LITERAL:
      case sqlParser::FLOAT_LITERAL:
      case sqlParser::DECIMAL_LITERAL:
      case sqlParser::INTEGER_LITERAL: {
        setState(1547);
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
        setState(1548);
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
  enterRule(_localctx, 156, sqlParser::RuleIdentifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1553);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(1551);
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
        setState(1552);
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
  enterRule(_localctx, 158, sqlParser::RuleQualifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1555);
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
  enterRule(_localctx, 160, sqlParser::RulePrecision);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1557);
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
  enterRule(_localctx, 162, sqlParser::RuleScale);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1559);
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
  enterRule(_localctx, 164, sqlParser::RuleSize);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1561);
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
  enterRule(_localctx, 166, sqlParser::RuleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1573);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::SQL_NULL: {
        enterOuterAlt(_localctx, 1);
        setState(1563);
        match(sqlParser::SQL_NULL);
        break;
      }

      case sqlParser::SQL_FALSE:
      case sqlParser::SQL_TRUE: {
        enterOuterAlt(_localctx, 2);
        setState(1564);
        booleanLiteral();
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(1565);
        integerLiteral();
        break;
      }

      case sqlParser::DECIMAL_LITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(1566);
        decimalLiteral();
        break;
      }

      case sqlParser::FLOAT_LITERAL: {
        enterOuterAlt(_localctx, 5);
        setState(1567);
        floatLiteral();
        break;
      }

      case sqlParser::DATE: {
        enterOuterAlt(_localctx, 6);
        setState(1568);
        dateLiteral();
        break;
      }

      case sqlParser::TIME: {
        enterOuterAlt(_localctx, 7);
        setState(1569);
        timeLiteral();
        break;
      }

      case sqlParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 8);
        setState(1570);
        stringLiteral();
        break;
      }

      case sqlParser::TIMESTAMP: {
        enterOuterAlt(_localctx, 9);
        setState(1571);
        timestampLiteral();
        break;
      }

      case sqlParser::INTERVAL: {
        enterOuterAlt(_localctx, 10);
        setState(1572);
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

sqlParser::IntegerLiteralContext* sqlParser::SimpleLiteralContext::integerLiteral() {
  return getRuleContext<sqlParser::IntegerLiteralContext>(0);
}

sqlParser::DecimalLiteralContext* sqlParser::SimpleLiteralContext::decimalLiteral() {
  return getRuleContext<sqlParser::DecimalLiteralContext>(0);
}

sqlParser::FloatLiteralContext* sqlParser::SimpleLiteralContext::floatLiteral() {
  return getRuleContext<sqlParser::FloatLiteralContext>(0);
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
  enterRule(_localctx, 168, sqlParser::RuleSimpleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1581);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::SQL_NULL: {
        enterOuterAlt(_localctx, 1);
        setState(1575);
        match(sqlParser::SQL_NULL);
        break;
      }

      case sqlParser::SQL_FALSE:
      case sqlParser::SQL_TRUE: {
        enterOuterAlt(_localctx, 2);
        setState(1576);
        booleanLiteral();
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(1577);
        integerLiteral();
        break;
      }

      case sqlParser::DECIMAL_LITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(1578);
        decimalLiteral();
        break;
      }

      case sqlParser::FLOAT_LITERAL: {
        enterOuterAlt(_localctx, 5);
        setState(1579);
        floatLiteral();
        break;
      }

      case sqlParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 6);
        setState(1580);
        stringLiteral();
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
  enterRule(_localctx, 170, sqlParser::RuleBooleanLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1583);
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
  enterRule(_localctx, 172, sqlParser::RuleIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1585);
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
  enterRule(_localctx, 174, sqlParser::RuleDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1587);
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
  enterRule(_localctx, 176, sqlParser::RuleFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1589);
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
  enterRule(_localctx, 178, sqlParser::RuleDateLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1591);
    match(sqlParser::DATE);
    setState(1592);
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
  enterRule(_localctx, 180, sqlParser::RuleTimeLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1594);
    match(sqlParser::TIME);
    setState(1595);
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
  enterRule(_localctx, 182, sqlParser::RuleTimestampLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1597);
    match(sqlParser::TIMESTAMP);
    setState(1598);
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
  enterRule(_localctx, 184, sqlParser::RuleStringLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1600);
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
  enterRule(_localctx, 186, sqlParser::RuleIntervalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1605);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 213, _ctx)) {
    case 1: {
      setState(1602);
      sql92CalendarInterval();
      break;
    }

    case 2: {
      setState(1603);
      sql92TimeInterval();
      break;
    }

    case 3: {
      setState(1604);
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
  enterRule(_localctx, 188, sqlParser::RuleLooseInterval);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1607);
    match(sqlParser::INTERVAL);
    setState(1608);
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
  enterRule(_localctx, 190, sqlParser::RuleSql92CalendarInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1610);
    match(sqlParser::INTERVAL);
    setState(1611);
    stringLiteral();
    setState(1635);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 217, _ctx)) {
    case 1: {
      setState(1612);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1617);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 214, _ctx)) {
      case 1: {
        setState(1613);
        match(sqlParser::T__6);
        setState(1614);
        precision();
        setState(1615);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1619);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1624);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1620);
        match(sqlParser::T__6);
        setState(1621);
        precision();
        setState(1622);
        match(sqlParser::T__7);
      }
      setState(1626);
      match(sqlParser::TO);
      setState(1627);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      break;
    }

    case 3: {
      setState(1628);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      setState(1633);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 216, _ctx)) {
      case 1: {
        setState(1629);
        match(sqlParser::T__6);
        setState(1630);
        precision();
        setState(1631);
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
  enterRule(_localctx, 192, sqlParser::RuleSql92TimeInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1637);
    match(sqlParser::INTERVAL);
    setState(1638);
    stringLiteral();
    setState(1723);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 231, _ctx)) {
    case 1: {
      setState(1639);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1644);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 218, _ctx)) {
      case 1: {
        setState(1640);
        match(sqlParser::T__6);
        setState(1641);
        precision();
        setState(1642);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1646);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1651);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1647);
        match(sqlParser::T__6);
        setState(1648);
        precision();
        setState(1649);
        match(sqlParser::T__7);
      }
      setState(1653);
      match(sqlParser::TO);
      setState(1663);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::HOUR: {
          setState(1654);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
          break;
        }

        case sqlParser::MINUTE: {
          setState(1655);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1656);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1661);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 220, _ctx)) {
          case 1: {
            setState(1657);
            match(sqlParser::T__6);
            setState(1658);
            scale();
            setState(1659);
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
      setState(1665);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1670);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1666);
        match(sqlParser::T__6);
        setState(1667);
        precision();
        setState(1668);
        match(sqlParser::T__7);
      }
      setState(1672);
      match(sqlParser::TO);
      setState(1681);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::MINUTE: {
          setState(1673);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1674);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1679);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 223, _ctx)) {
          case 1: {
            setState(1675);
            match(sqlParser::T__6);
            setState(1676);
            scale();
            setState(1677);
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
      setState(1683);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1688);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__6) {
        setState(1684);
        match(sqlParser::T__6);
        setState(1685);
        precision();
        setState(1686);
        match(sqlParser::T__7);
      }
      setState(1690);
      match(sqlParser::TO);

      setState(1691);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1696);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 226, _ctx)) {
      case 1: {
        setState(1692);
        match(sqlParser::T__6);
        setState(1693);
        scale();
        setState(1694);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 5: {
      setState(1698);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1703);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 227, _ctx)) {
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

    case 6: {
      setState(1705);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1710);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 228, _ctx)) {
      case 1: {
        setState(1706);
        match(sqlParser::T__6);
        setState(1707);
        precision();
        setState(1708);
        match(sqlParser::T__7);
        break;
      }

      }
      break;
    }

    case 7: {
      setState(1712);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1721);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 230, _ctx)) {
      case 1: {
        setState(1713);
        match(sqlParser::T__6);
        setState(1714);
        precision();
        setState(1717);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__2) {
          setState(1715);
          match(sqlParser::T__2);
          setState(1716);
          scale();
        }
        setState(1719);
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
  enterRule(_localctx, 194, sqlParser::RuleNonReservedWord);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1725);
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
      | (1ULL << (sqlParser::TIMESTAMP - 64))
      | (1ULL << (sqlParser::CHAR - 64))
      | (1ULL << (sqlParser::CHARACTER - 64)))) != 0) || ((((_la - 130) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 130)) & ((1ULL << (sqlParser::BINARY - 130))
      | (1ULL << (sqlParser::VARBINARY - 130))
      | (1ULL << (sqlParser::VARYING - 130))
      | (1ULL << (sqlParser::BIT - 130))
      | (1ULL << (sqlParser::BITS - 130))
      | (1ULL << (sqlParser::BYTE - 130))
      | (1ULL << (sqlParser::BYTES - 130))
      | (1ULL << (sqlParser::CHARS - 130))
      | (1ULL << (sqlParser::DIGITS - 130))
      | (1ULL << (sqlParser::COALESCE - 130))
      | (1ULL << (sqlParser::CONCAT - 130))
      | (1ULL << (sqlParser::COUNT - 130))
      | (1ULL << (sqlParser::DECODE - 130))
      | (1ULL << (sqlParser::EXTRACT - 130))
      | (1ULL << (sqlParser::IFNULL - 130))
      | (1ULL << (sqlParser::NULLIF - 130))
      | (1ULL << (sqlParser::POSITION - 130))
      | (1ULL << (sqlParser::SUBSTR - 130))
      | (1ULL << (sqlParser::SUBSTRING - 130))
      | (1ULL << (sqlParser::DAY - 130))
      | (1ULL << (sqlParser::DAYS - 130))
      | (1ULL << (sqlParser::HOUR - 130))
      | (1ULL << (sqlParser::HOURS - 130))
      | (1ULL << (sqlParser::MINUTE - 130))
      | (1ULL << (sqlParser::MINUTES - 130))
      | (1ULL << (sqlParser::MONTH - 130))
      | (1ULL << (sqlParser::MONTHS - 130))
      | (1ULL << (sqlParser::QUARTER - 130))
      | (1ULL << (sqlParser::QUARTERS - 130))
      | (1ULL << (sqlParser::SECOND - 130))
      | (1ULL << (sqlParser::SECONDS - 130))
      | (1ULL << (sqlParser::WEEK - 130))
      | (1ULL << (sqlParser::WEEKS - 130))
      | (1ULL << (sqlParser::YEAR - 130))
      | (1ULL << (sqlParser::YEARS - 130))
      | (1ULL << (sqlParser::BLOCK - 130))
      | (1ULL << (sqlParser::BLOOM - 130))
      | (1ULL << (sqlParser::BTREE - 130))
      | (1ULL << (sqlParser::HASH - 130))
      | (1ULL << (sqlParser::LOOP - 130))
      | (1ULL << (sqlParser::SKIPLIST - 130))
      | (1ULL << (sqlParser::TREE - 130))
      | (1ULL << (sqlParser::VALUE - 130)))) != 0))) {
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
    case 59: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

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
    case 1: return precpred(_ctx, 3);
    case 2: return precpred(_ctx, 5);
    case 3: return precpred(_ctx, 4);

  default:
    break;
  }
  return true;
}

bool sqlParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 4: return precpred(_ctx, 36);
    case 5: return precpred(_ctx, 35);
    case 6: return precpred(_ctx, 34);
    case 7: return precpred(_ctx, 9);
    case 8: return precpred(_ctx, 8);
    case 9: return precpred(_ctx, 3);
    case 10: return precpred(_ctx, 2);
    case 11: return precpred(_ctx, 1);
    case 12: return precpred(_ctx, 37);
    case 13: return precpred(_ctx, 10);
    case 14: return precpred(_ctx, 6);
    case 15: return precpred(_ctx, 5);

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
  "tableExpr", "tableReference", "joinType", "joinHint", "valuesExpr", "insertQuery", 
  "deleteQuery", "updateQuery", "createQuery", "createTableQuery", "createIndexQuery", 
  "dropQuery", "truncateQuery", "tableDefinition", "tablePartitioning", 
  "columnDefinition", "columnNameList", "tableConstraint", "indexDefinition", 
  "indexMethod", "dataType", "sqlDataType", "asSubquery", "selectOrValues", 
  "selectList", "selectListItem", "groupByList", "groupByListItem", "orderByList", 
  "orderByListItem", "joinUsingList", "joinUsingListItem", "corrList", "corrListItem", 
  "propertyList", "property", "valueList", "predicate", "joinPredicate", 
  "casePredicate", "expr", "variableName", "columnRef", "columnWildcard", 
  "className", "functionName", "methodName", "catalogName", "schemaName", 
  "tableName", "tableAlias", "columnName", "columnAlias", "collationName", 
  "indexName", "propertyName", "hintName", "propertyValue", "hintValue", 
  "identifier", "qualifier", "precision", "scale", "size", "literal", "simpleLiteral", 
  "booleanLiteral", "integerLiteral", "decimalLiteral", "floatLiteral", 
  "dateLiteral", "timeLiteral", "timestampLiteral", "stringLiteral", "intervalLiteral", 
  "looseInterval", "sql92CalendarInterval", "sql92TimeInterval", "nonReservedWord"
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
  "", "", "", "", "", "", "", "", "", "", "", "'/*+'", "'*/'"
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
  "PLACING", "PRETTY", "PRIMARY", "REFERENCE", "RIGHT", "ROW", "SCHEMA", 
  "SELECT", "SEMI", "SET", "STAGE", "STATS", "STATISTICS", "SUM", "TABLE", 
  "TO", "TOP", "TRAILING", "TRUNCATE", "SQL_TRUE", "TYPE", "UNION", "UNIQUE", 
  "UPDATE", "USING", "VALUES", "WHERE", "WITH_NAMES", "WITH", "XOR", "BOOLEAN", 
  "INTEGER", "DECIMAL", "FLOAT", "TIME", "TIMESTAMP", "INTERVAL", "CHAR", 
  "CHARACTER", "VARCHAR", "TEXT", "BINARY", "VARBINARY", "BLOB", "VARYING", 
  "BIT", "BITS", "BYTE", "BYTES", "CHARS", "DIGITS", "CASE", "CAST", "COALESCE", 
  "CONCAT", "COUNT", "DECODE", "EXTRACT", "EXISTS", "IF", "IFNULL", "NULLIF", 
  "POSITION", "SUBSTR", "SUBSTRING", "TRIM", "TYPEOF", "WHEN", "THEN", "ELSE", 
  "END", "DAY", "DAYS", "HOUR", "HOURS", "MINUTE", "MINUTES", "MONTH", "MONTHS", 
  "QUARTER", "QUARTERS", "SECOND", "SECONDS", "WEEK", "WEEKS", "YEAR", "YEARS", 
  "BLOCK", "BLOOM", "BTREE", "HASH", "LOOP", "SKIPLIST", "TREE", "VALUE", 
  "IDENTIFIER", "PLAIN_IDENTIFIER", "BAKTIKD_IDENTIFIER", "DUBLQUOTED_IDENTIFIER", 
  "STRING_LITERAL", "SINGLQUOTED_STRING", "DUBLQUOTED_STRING", "FLOAT_LITERAL", 
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
    0x3, 0xc9, 0x6c2, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
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
    0x9, 0x63, 0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 0xc9, 0xa, 0x2, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 0xd1, 0xa, 0x3, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x7, 0x5, 0xd6, 0xa, 0x5, 0xc, 0x5, 0xe, 
    0x5, 0xd9, 0xb, 0x5, 0x3, 0x5, 0x3, 0x5, 0x7, 0x5, 0xdd, 0xa, 0x5, 0xc, 
    0x5, 0xe, 0x5, 0xe0, 0xb, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 
    0x3, 0x6, 0x5, 0x6, 0xe7, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x7, 0x7, 0x7, 0xed, 0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 0xf0, 0xb, 0x7, 0x3, 
    0x8, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 0xf5, 0xa, 0x8, 0x3, 0x9, 0x3, 0x9, 
    0x3, 0x9, 0x5, 0x9, 0xfa, 0xa, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 
    0xb, 0x7, 0xb, 0x100, 0xa, 0xb, 0xc, 0xb, 0xe, 0xb, 0x103, 0xb, 0xb, 
    0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x7, 0xc, 0x109, 0xa, 0xc, 0xc, 
    0xc, 0xe, 0xc, 0x10c, 0xb, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 
    0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0x116, 0xa, 0xc, 0x3, 
    0xd, 0x3, 0xd, 0x5, 0xd, 0x11a, 0xa, 0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 
    0x11e, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 0x121, 0xb, 0xd, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 
    0xe, 0x5, 0xe, 0x12c, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x7, 0xe, 0x132, 0xa, 0xe, 0xc, 0xe, 0xe, 0xe, 0x135, 0xb, 0xe, 0x3, 
    0xe, 0x3, 0xe, 0x5, 0xe, 0x139, 0xa, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
    0x3, 0xf, 0x7, 0xf, 0x13f, 0xa, 0xf, 0xc, 0xf, 0xe, 0xf, 0x142, 0xb, 
    0xf, 0x5, 0xf, 0x144, 0xa, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
    0x5, 0xf, 0x14a, 0xa, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 
    0xf, 0x150, 0xa, 0xf, 0x5, 0xf, 0x152, 0xa, 0xf, 0x3, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x5, 0x10, 0x157, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x5, 0x10, 0x15c, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x160, 
    0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x165, 0xa, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x169, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x16f, 0xa, 0x10, 0x3, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x3, 0x11, 0x7, 0x11, 0x175, 0xa, 0x11, 0xc, 0x11, 0xe, 0x11, 
    0x178, 0xb, 0x11, 0x5, 0x11, 0x17a, 0xa, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0x180, 0xa, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0x186, 0xa, 0x11, 0x5, 0x11, 0x188, 
    0xa, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 
    0x12, 0x5, 0x12, 0x190, 0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x5, 0x12, 0x196, 0xa, 0x12, 0x3, 0x12, 0x5, 0x12, 0x199, 
    0xa, 0x12, 0x3, 0x12, 0x7, 0x12, 0x19c, 0xa, 0x12, 0xc, 0x12, 0xe, 0x12, 
    0x19f, 0xb, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x5, 0x13, 0x1a7, 0xa, 0x13, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x7, 0x14, 0x1ae, 0xa, 0x14, 0xc, 0x14, 0xe, 0x14, 
    0x1b1, 0xb, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x1b5, 0xa, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x15, 0x3, 
    0x15, 0x3, 0x15, 0x5, 0x15, 0x1bf, 0xa, 0x15, 0x3, 0x15, 0x5, 0x15, 
    0x1c2, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 
    0x1c8, 0xa, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1cb, 0xa, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1d1, 0xa, 0x15, 0x3, 0x15, 
    0x5, 0x15, 0x1d4, 0xa, 0x15, 0x5, 0x15, 0x1d6, 0xa, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1dc, 0xa, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1e1, 0xa, 0x15, 0x3, 0x15, 0x5, 0x15, 
    0x1e4, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1ed, 0xa, 0x15, 0x3, 0x15, 0x5, 0x15, 
    0x1f0, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x7, 0x15, 0x1f9, 0xa, 0x15, 0xc, 0x15, 0xe, 0x15, 
    0x1fc, 0xb, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x201, 
    0xa, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x206, 0xa, 0x16, 
    0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0x20d, 
    0xa, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0x212, 0xa, 0x17, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0x217, 0xa, 0x18, 0x3, 0x18, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x7, 0x18, 0x21d, 0xa, 0x18, 0xc, 0x18, 
    0xe, 0x18, 0x220, 0xb, 0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0x224, 
    0xa, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 
    0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 0x19, 
    0x231, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 0x234, 0xb, 0x19, 0x3, 0x1a, 
    0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x7, 0x1a, 0x23a, 0xa, 0x1a, 0xc, 0x1a, 
    0xe, 0x1a, 0x23d, 0xb, 0x1a, 0x5, 0x1a, 0x23f, 0xa, 0x1a, 0x3, 0x1a, 
    0x3, 0x1a, 0x5, 0x1a, 0x243, 0xa, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 
    0x3, 0x1a, 0x3, 0x1a, 0x7, 0x1a, 0x24a, 0xa, 0x1a, 0xc, 0x1a, 0xe, 0x1a, 
    0x24d, 0xb, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 0x251, 0xa, 0x1a, 
    0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 0x256, 0xa, 0x1a, 0x3, 0x1b, 
    0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x7, 0x1b, 0x25c, 0xa, 0x1b, 0xc, 0x1b, 
    0xe, 0x1b, 0x25f, 0xb, 0x1b, 0x5, 0x1b, 0x261, 0xa, 0x1b, 0x3, 0x1b, 
    0x3, 0x1b, 0x5, 0x1b, 0x265, 0xa, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 
    0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x26c, 0xa, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 
    0x3, 0x1c, 0x3, 0x1c, 0x7, 0x1c, 0x272, 0xa, 0x1c, 0xc, 0x1c, 0xe, 0x1c, 
    0x275, 0xb, 0x1c, 0x5, 0x1c, 0x277, 0xa, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 
    0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 
    0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x7, 0x1c, 0x284, 0xa, 0x1c, 0xc, 0x1c, 
    0xe, 0x1c, 0x287, 0xb, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 
    0x5, 0x1c, 0x28d, 0xa, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x5, 0x1d, 0x291, 
    0xa, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x295, 0xa, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x29b, 0xa, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x2a0, 0xa, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 
    0x2a3, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x2a8, 
    0xa, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x2ab, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x5, 0x1e, 0x2af, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 
    0x2b4, 0xa, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x2b7, 0xa, 0x1e, 0x5, 0x1e, 
    0x2b9, 0xa, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2bd, 0xa, 0x1f, 
    0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2c3, 0xa, 0x1f, 
    0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x2ca, 
    0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x7, 0x1f, 0x2d0, 
    0xa, 0x1f, 0xc, 0x1f, 0xe, 0x1f, 0x2d3, 0xb, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
    0x3, 0x1f, 0x5, 0x1f, 0x2d8, 0xa, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x3, 0x20, 0x5, 0x20, 0x2de, 0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x3, 0x20, 0x3, 0x20, 0x5, 0x20, 0x2e5, 0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x3, 0x20, 0x3, 0x20, 0x5, 0x20, 0x2eb, 0xa, 0x20, 0x3, 0x21, 0x3, 0x21, 
    0x5, 0x21, 0x2ef, 0xa, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x22, 0x3, 0x22, 
    0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x2f7, 0xa, 0x22, 0x3, 0x22, 0x3, 0x22, 
    0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x2fd, 0xa, 0x22, 0x7, 0x22, 0x2ff, 
    0xa, 0x22, 0xc, 0x22, 0xe, 0x22, 0x302, 0xb, 0x22, 0x3, 0x22, 0x3, 0x22, 
    0x5, 0x22, 0x306, 0xa, 0x22, 0x3, 0x22, 0x5, 0x22, 0x309, 0xa, 0x22, 
    0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x30d, 0xa, 0x22, 0x3, 0x23, 0x3, 0x23, 
    0x5, 0x23, 0x311, 0xa, 0x23, 0x3, 0x23, 0x5, 0x23, 0x314, 0xa, 0x23, 
    0x3, 0x23, 0x5, 0x23, 0x317, 0xa, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 
    0x3, 0x23, 0x7, 0x23, 0x31d, 0xa, 0x23, 0xc, 0x23, 0xe, 0x23, 0x320, 
    0xb, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x5, 
    0x24, 0x327, 0xa, 0x24, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 
    0x3, 0x24, 0x5, 0x24, 0x32e, 0xa, 0x24, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 
    0x5, 0x24, 0x333, 0xa, 0x24, 0x7, 0x24, 0x335, 0xa, 0x24, 0xc, 0x24, 
    0xe, 0x24, 0x338, 0xb, 0x24, 0x3, 0x24, 0x3, 0x24, 0x5, 0x24, 0x33c, 
    0xa, 0x24, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x7, 0x25, 0x342, 
    0xa, 0x25, 0xc, 0x25, 0xe, 0x25, 0x345, 0xb, 0x25, 0x3, 0x25, 0x3, 0x25, 
    0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 
    0x26, 0x7, 0x26, 0x350, 0xa, 0x26, 0xc, 0x26, 0xe, 0x26, 0x353, 0xb, 
    0x26, 0x3, 0x26, 0x3, 0x26, 0x5, 0x26, 0x357, 0xa, 0x26, 0x3, 0x26, 
    0x5, 0x26, 0x35a, 0xa, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 
    0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x7, 0x26, 0x363, 0xa, 0x26, 0xc, 0x26, 
    0xe, 0x26, 0x366, 0xb, 0x26, 0x3, 0x26, 0x3, 0x26, 0x5, 0x26, 0x36a, 
    0xa, 0x26, 0x5, 0x26, 0x36c, 0xa, 0x26, 0x3, 0x27, 0x5, 0x27, 0x36f, 
    0xa, 0x27, 0x3, 0x27, 0x3, 0x27, 0x5, 0x27, 0x373, 0xa, 0x27, 0x3, 0x27, 
    0x5, 0x27, 0x376, 0xa, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 
    0x3, 0x27, 0x7, 0x27, 0x37d, 0xa, 0x27, 0xc, 0x27, 0xe, 0x27, 0x380, 
    0xb, 0x27, 0x3, 0x27, 0x3, 0x27, 0x5, 0x27, 0x384, 0xa, 0x27, 0x3, 0x27, 
    0x3, 0x27, 0x5, 0x27, 0x388, 0xa, 0x27, 0x3, 0x27, 0x3, 0x27, 0x5, 0x27, 
    0x38c, 0xa, 0x27, 0x3, 0x28, 0x3, 0x28, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 
    0x5, 0x29, 0x393, 0xa, 0x29, 0x3, 0x29, 0x5, 0x29, 0x396, 0xa, 0x29, 
    0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x39d, 
    0xa, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x3a1, 0xa, 0x2a, 0x3, 0x2a, 
    0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x3a7, 0xa, 0x2a, 0x3, 0x2a, 
    0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x3ac, 0xa, 0x2a, 0x5, 0x2a, 0x3ae, 
    0xa, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x3b2, 0xa, 0x2a, 0x3, 0x2a, 
    0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x3b8, 0xa, 0x2a, 0x3, 0x2a, 
    0x3, 0x2a, 0x5, 0x2a, 0x3bc, 0xa, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 
    0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x3c5, 0xa, 0x2a, 
    0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x3cb, 0xa, 0x2a, 
    0x3, 0x2a, 0x5, 0x2a, 0x3ce, 0xa, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 
    0x5, 0x2a, 0x3d3, 0xa, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x3d7, 
    0xa, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x3da, 0xa, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 
    0x5, 0x2a, 0x3de, 0xa, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 
    0x5, 0x2a, 0x3e4, 0xa, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 
    0x3, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x3ec, 0xa, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 
    0x5, 0x2a, 0x3f0, 0xa, 0x2a, 0x3, 0x2a, 0x5, 0x2a, 0x3f3, 0xa, 0x2a, 
    0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2c, 0x3, 0x2c, 0x7, 0x2c, 0x3f9, 0xa, 0x2c, 
    0xc, 0x2c, 0xe, 0x2c, 0x3fc, 0xb, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 
    0x3, 0x2c, 0x7, 0x2c, 0x402, 0xa, 0x2c, 0xc, 0x2c, 0xe, 0x2c, 0x405, 
    0xb, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x5, 0x2c, 0x40a, 0xa, 0x2c, 
    0x5, 0x2c, 0x40c, 0xa, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x7, 0x2d, 
    0x411, 0xa, 0x2d, 0xc, 0x2d, 0xe, 0x2d, 0x414, 0xb, 0x2d, 0x3, 0x2e, 
    0x3, 0x2e, 0x5, 0x2e, 0x418, 0xa, 0x2e, 0x3, 0x2e, 0x5, 0x2e, 0x41b, 
    0xa, 0x2e, 0x3, 0x2e, 0x5, 0x2e, 0x41e, 0xa, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 
    0x3, 0x2f, 0x7, 0x2f, 0x423, 0xa, 0x2f, 0xc, 0x2f, 0xe, 0x2f, 0x426, 
    0xb, 0x2f, 0x3, 0x30, 0x3, 0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x7, 
    0x31, 0x42d, 0xa, 0x31, 0xc, 0x31, 0xe, 0x31, 0x430, 0xb, 0x31, 0x3, 
    0x32, 0x3, 0x32, 0x5, 0x32, 0x434, 0xa, 0x32, 0x3, 0x33, 0x3, 0x33, 
    0x3, 0x33, 0x7, 0x33, 0x439, 0xa, 0x33, 0xc, 0x33, 0xe, 0x33, 0x43c, 
    0xb, 0x33, 0x3, 0x34, 0x3, 0x34, 0x3, 0x35, 0x3, 0x35, 0x3, 0x35, 0x7, 
    0x35, 0x443, 0xa, 0x35, 0xc, 0x35, 0xe, 0x35, 0x446, 0xb, 0x35, 0x3, 
    0x36, 0x3, 0x36, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x7, 0x37, 0x44d, 
    0xa, 0x37, 0xc, 0x37, 0xe, 0x37, 0x450, 0xb, 0x37, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x5, 0x38, 0x455, 0xa, 0x38, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 
    0x7, 0x39, 0x45a, 0xa, 0x39, 0xc, 0x39, 0xe, 0x39, 0x45d, 0xb, 0x39, 
    0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3c, 0x3, 0x3c, 0x3, 
    0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x6, 0x3d, 0x483, 0xa, 0x3d, 
    0xd, 0x3d, 0xe, 0x3d, 0x484, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x6, 0x3d, 0x48c, 0xa, 0x3d, 0xd, 0x3d, 0xe, 0x3d, 0x48d, 
    0x5, 0x3d, 0x490, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x494, 
    0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x7, 0x3d, 0x4b4, 
    0xa, 0x3d, 0xc, 0x3d, 0xe, 0x3d, 0x4b7, 0xb, 0x3d, 0x5, 0x3d, 0x4b9, 
    0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x7, 0x3d, 0x4c1, 0xa, 0x3d, 0xc, 0x3d, 0xe, 0x3d, 0x4c4, 0xb, 
    0x3d, 0x5, 0x3d, 0x4c6, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x7, 0x3d, 0x4ce, 0xa, 0x3d, 0xc, 0x3d, 
    0xe, 0x3d, 0x4d1, 0xb, 0x3d, 0x5, 0x3d, 0x4d3, 0xa, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x4df, 0xa, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x4f1, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x4f8, 0xa, 0x3d, 0x5, 0x3d, 
    0x4fa, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x503, 0xa, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 
    0x506, 0xa, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x509, 0xa, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x5, 0x3d, 0x50d, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x51b, 0xa, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x523, 
    0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x52c, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x535, 
    0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x53e, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x547, 
    0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x7, 0x3d, 0x551, 0xa, 0x3d, 0xc, 0x3d, 
    0xe, 0x3d, 0x554, 0xb, 0x3d, 0x5, 0x3d, 0x556, 0xa, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x563, 0xa, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x5, 0x3d, 0x56c, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x579, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x57f, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
    0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x7, 
    0x3d, 0x596, 0xa, 0x3d, 0xc, 0x3d, 0xe, 0x3d, 0x599, 0xb, 0x3d, 0x5, 
    0x3d, 0x59b, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x5, 0x3d, 0x5a2, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x5, 0x3d, 0x5a7, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 0x5b0, 0xa, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x7, 0x3d, 0x5b7, 0xa, 0x3d, 
    0xc, 0x3d, 0xe, 0x3d, 0x5ba, 0xb, 0x3d, 0x5, 0x3d, 0x5bc, 0xa, 0x3d, 
    0x3, 0x3d, 0x7, 0x3d, 0x5bf, 0xa, 0x3d, 0xc, 0x3d, 0xe, 0x3d, 0x5c2, 
    0xb, 0x3d, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x5, 
    0x3f, 0x5c9, 0xa, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x5, 0x3f, 
    0x5ce, 0xa, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x5, 0x3f, 0x5d3, 
    0xa, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 
    0x40, 0x5da, 0xa, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 
    0x5df, 0xa, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x5, 0x40, 0x5e4, 
    0xa, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x7, 
    0x41, 0x5eb, 0xa, 0x41, 0xc, 0x41, 0xe, 0x41, 0x5ee, 0xb, 0x41, 0x3, 
    0x41, 0x3, 0x41, 0x3, 0x42, 0x3, 0x42, 0x3, 0x43, 0x3, 0x43, 0x3, 0x44, 
    0x3, 0x44, 0x3, 0x45, 0x3, 0x45, 0x3, 0x46, 0x3, 0x46, 0x3, 0x47, 0x3, 
    0x47, 0x3, 0x48, 0x3, 0x48, 0x3, 0x49, 0x3, 0x49, 0x3, 0x4a, 0x3, 0x4a, 
    0x3, 0x4b, 0x3, 0x4b, 0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4d, 0x3, 0x4d, 0x3, 
    0x4e, 0x3, 0x4e, 0x5, 0x4e, 0x60c, 0xa, 0x4e, 0x3, 0x4f, 0x3, 0x4f, 
    0x5, 0x4f, 0x610, 0xa, 0x4f, 0x3, 0x50, 0x3, 0x50, 0x5, 0x50, 0x614, 
    0xa, 0x50, 0x3, 0x51, 0x3, 0x51, 0x3, 0x52, 0x3, 0x52, 0x3, 0x53, 0x3, 
    0x53, 0x3, 0x54, 0x3, 0x54, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 
    0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x3, 0x55, 0x5, 
    0x55, 0x628, 0xa, 0x55, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 
    0x3, 0x56, 0x3, 0x56, 0x5, 0x56, 0x630, 0xa, 0x56, 0x3, 0x57, 0x3, 0x57, 
    0x3, 0x58, 0x3, 0x58, 0x3, 0x59, 0x3, 0x59, 0x3, 0x5a, 0x3, 0x5a, 0x3, 
    0x5b, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5d, 
    0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5e, 0x3, 0x5e, 0x3, 0x5f, 0x3, 0x5f, 0x3, 
    0x5f, 0x5, 0x5f, 0x648, 0xa, 0x5f, 0x3, 0x60, 0x3, 0x60, 0x3, 0x60, 
    0x3, 0x61, 0x3, 0x61, 0x3, 0x61, 0x3, 0x61, 0x3, 0x61, 0x3, 0x61, 0x3, 
    0x61, 0x5, 0x61, 0x654, 0xa, 0x61, 0x3, 0x61, 0x3, 0x61, 0x3, 0x61, 
    0x3, 0x61, 0x3, 0x61, 0x5, 0x61, 0x65b, 0xa, 0x61, 0x3, 0x61, 0x3, 0x61, 
    0x3, 0x61, 0x3, 0x61, 0x3, 0x61, 0x3, 0x61, 0x3, 0x61, 0x5, 0x61, 0x664, 
    0xa, 0x61, 0x5, 0x61, 0x666, 0xa, 0x61, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 
    0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x5, 0x62, 0x66f, 0xa, 0x62, 
    0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x5, 0x62, 0x676, 
    0xa, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 
    0x62, 0x3, 0x62, 0x3, 0x62, 0x5, 0x62, 0x680, 0xa, 0x62, 0x5, 0x62, 
    0x682, 0xa, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 
    0x5, 0x62, 0x689, 0xa, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 
    0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x5, 0x62, 0x692, 0xa, 0x62, 0x5, 0x62, 
    0x694, 0xa, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 
    0x5, 0x62, 0x69b, 0xa, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 
    0x3, 0x62, 0x3, 0x62, 0x5, 0x62, 0x6a3, 0xa, 0x62, 0x3, 0x62, 0x3, 0x62, 
    0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x5, 0x62, 0x6aa, 0xa, 0x62, 0x3, 0x62, 
    0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x5, 0x62, 0x6b1, 0xa, 0x62, 
    0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x3, 0x62, 0x5, 0x62, 0x6b8, 
    0xa, 0x62, 0x3, 0x62, 0x3, 0x62, 0x5, 0x62, 0x6bc, 0xa, 0x62, 0x5, 0x62, 
    0x6be, 0xa, 0x62, 0x3, 0x63, 0x3, 0x63, 0x3, 0x63, 0x2, 0x5, 0x22, 0x28, 
    0x78, 0x64, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 
    0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 
    0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 
    0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 
    0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 
    0x78, 0x7a, 0x7c, 0x7e, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 
    0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, 0xa0, 0xa2, 0xa4, 0xa6, 
    0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe, 
    0xc0, 0xc2, 0xc4, 0x2, 0x21, 0x4, 0x2, 0x6, 0x6, 0x6a, 0x6a, 0x4, 0x2, 
    0x7, 0x7, 0x17, 0x17, 0x4, 0x2, 0x5c, 0x5c, 0x60, 0x60, 0x3, 0x2, 0x66, 
    0x67, 0x4, 0x2, 0xb, 0xb, 0x62, 0x62, 0x5, 0x2, 0x33, 0x33, 0x41, 0x41, 
    0x70, 0x70, 0x5, 0x2, 0x39, 0x39, 0x47, 0x47, 0x5f, 0x5f, 0x4, 0x2, 
    0xb2, 0xb3, 0xb5, 0xb6, 0x4, 0x2, 0x3e, 0x3e, 0x45, 0x45, 0x4, 0x2, 
    0xb5, 0xb5, 0xb9, 0xb9, 0x4, 0x2, 0xb4, 0xb5, 0xb7, 0xb8, 0x5, 0x2, 
    0x89, 0x89, 0x8b, 0x8b, 0x8d, 0x8d, 0x3, 0x2, 0x80, 0x81, 0x4, 0x2, 
    0x80, 0x80, 0x8a, 0x8c, 0x3, 0x2, 0x8a, 0x8b, 0x4, 0x2, 0x1d, 0x1d, 
    0x2e, 0x2e, 0x4, 0x2, 0x6, 0x6, 0xd, 0xd, 0x4, 0x2, 0x5, 0x5, 0x1c, 
    0x1c, 0x4, 0x2, 0x5, 0x5, 0x5b, 0x5b, 0x4, 0x2, 0x5, 0x5, 0x3a, 0x3a, 
    0x4, 0x2, 0x5, 0x5, 0x37, 0x37, 0x4, 0x2, 0x5, 0x5, 0x3d, 0x3d, 0x3, 
    0x2, 0x9a, 0x9b, 0x5, 0x2, 0x20, 0x20, 0x46, 0x46, 0x6c, 0x6c, 0xa, 
    0x2, 0xa2, 0xa2, 0xa4, 0xa4, 0xa6, 0xa6, 0xa8, 0xa8, 0xaa, 0xaa, 0xac, 
    0xac, 0xae, 0xae, 0xb0, 0xb0, 0x4, 0x2, 0x7, 0x7, 0xe, 0xf, 0x5, 0x2, 
    0x6, 0x6, 0xd, 0xd, 0x10, 0x10, 0x4, 0x2, 0x8, 0x8, 0x11, 0x16, 0x3, 
    0x2, 0x27, 0x2a, 0x4, 0x2, 0x35, 0x35, 0x6e, 0x6e, 0x21, 0x2, 0x1a, 
    0x1a, 0x1d, 0x1d, 0x20, 0x22, 0x24, 0x25, 0x27, 0x2f, 0x31, 0x34, 0x36, 
    0x36, 0x38, 0x38, 0x3e, 0x3e, 0x40, 0x40, 0x42, 0x45, 0x47, 0x47, 0x4c, 
    0x4c, 0x4e, 0x4f, 0x53, 0x53, 0x57, 0x57, 0x59, 0x5a, 0x5c, 0x5c, 0x5e, 
    0x5f, 0x61, 0x61, 0x64, 0x67, 0x69, 0x69, 0x6f, 0x74, 0x76, 0x76, 0x79, 
    0x7e, 0x80, 0x81, 0x84, 0x85, 0x87, 0x8d, 0x90, 0x94, 0x97, 0x9b, 0xa2, 
    0xb9, 0x2, 0x7aa, 0x2, 0xc8, 0x3, 0x2, 0x2, 0x2, 0x4, 0xd0, 0x3, 0x2, 
    0x2, 0x2, 0x6, 0xd2, 0x3, 0x2, 0x2, 0x2, 0x8, 0xd7, 0x3, 0x2, 0x2, 0x2, 
    0xa, 0xe6, 0x3, 0x2, 0x2, 0x2, 0xc, 0xe8, 0x3, 0x2, 0x2, 0x2, 0xe, 0xf1, 
    0x3, 0x2, 0x2, 0x2, 0x10, 0xf9, 0x3, 0x2, 0x2, 0x2, 0x12, 0xfb, 0x3, 
    0x2, 0x2, 0x2, 0x14, 0x101, 0x3, 0x2, 0x2, 0x2, 0x16, 0x10a, 0x3, 0x2, 
    0x2, 0x2, 0x18, 0x117, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x138, 0x3, 0x2, 0x2, 
    0x2, 0x1c, 0x143, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x16e, 0x3, 0x2, 0x2, 0x2, 
    0x20, 0x179, 0x3, 0x2, 0x2, 0x2, 0x22, 0x18f, 0x3, 0x2, 0x2, 0x2, 0x24, 
    0x1a0, 0x3, 0x2, 0x2, 0x2, 0x26, 0x1a8, 0x3, 0x2, 0x2, 0x2, 0x28, 0x1d5, 
    0x3, 0x2, 0x2, 0x2, 0x2a, 0x205, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x211, 0x3, 
    0x2, 0x2, 0x2, 0x2e, 0x213, 0x3, 0x2, 0x2, 0x2, 0x30, 0x227, 0x3, 0x2, 
    0x2, 0x2, 0x32, 0x23e, 0x3, 0x2, 0x2, 0x2, 0x34, 0x260, 0x3, 0x2, 0x2, 
    0x2, 0x36, 0x276, 0x3, 0x2, 0x2, 0x2, 0x38, 0x290, 0x3, 0x2, 0x2, 0x2, 
    0x3a, 0x292, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x2ba, 0x3, 0x2, 0x2, 0x2, 0x3e, 
    0x2ea, 0x3, 0x2, 0x2, 0x2, 0x40, 0x2ec, 0x3, 0x2, 0x2, 0x2, 0x42, 0x2f2, 
    0x3, 0x2, 0x2, 0x2, 0x44, 0x30e, 0x3, 0x2, 0x2, 0x2, 0x46, 0x323, 0x3, 
    0x2, 0x2, 0x2, 0x48, 0x33d, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x36b, 0x3, 0x2, 
    0x2, 0x2, 0x4c, 0x36e, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x38d, 0x3, 0x2, 0x2, 
    0x2, 0x50, 0x395, 0x3, 0x2, 0x2, 0x2, 0x52, 0x3f2, 0x3, 0x2, 0x2, 0x2, 
    0x54, 0x3f4, 0x3, 0x2, 0x2, 0x2, 0x56, 0x40b, 0x3, 0x2, 0x2, 0x2, 0x58, 
    0x40d, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x41d, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x41f, 
    0x3, 0x2, 0x2, 0x2, 0x5e, 0x427, 0x3, 0x2, 0x2, 0x2, 0x60, 0x429, 0x3, 
    0x2, 0x2, 0x2, 0x62, 0x431, 0x3, 0x2, 0x2, 0x2, 0x64, 0x435, 0x3, 0x2, 
    0x2, 0x2, 0x66, 0x43d, 0x3, 0x2, 0x2, 0x2, 0x68, 0x43f, 0x3, 0x2, 0x2, 
    0x2, 0x6a, 0x447, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x449, 0x3, 0x2, 0x2, 0x2, 
    0x6e, 0x451, 0x3, 0x2, 0x2, 0x2, 0x70, 0x456, 0x3, 0x2, 0x2, 0x2, 0x72, 
    0x45e, 0x3, 0x2, 0x2, 0x2, 0x74, 0x460, 0x3, 0x2, 0x2, 0x2, 0x76, 0x462, 
    0x3, 0x2, 0x2, 0x2, 0x78, 0x56b, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x5c3, 0x3, 
    0x2, 0x2, 0x2, 0x7c, 0x5d2, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x5e3, 0x3, 0x2, 
    0x2, 0x2, 0x80, 0x5ec, 0x3, 0x2, 0x2, 0x2, 0x82, 0x5f1, 0x3, 0x2, 0x2, 
    0x2, 0x84, 0x5f3, 0x3, 0x2, 0x2, 0x2, 0x86, 0x5f5, 0x3, 0x2, 0x2, 0x2, 
    0x88, 0x5f7, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x5f9, 0x3, 0x2, 0x2, 0x2, 0x8c, 
    0x5fb, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x5fd, 0x3, 0x2, 0x2, 0x2, 0x90, 0x5ff, 
    0x3, 0x2, 0x2, 0x2, 0x92, 0x601, 0x3, 0x2, 0x2, 0x2, 0x94, 0x603, 0x3, 
    0x2, 0x2, 0x2, 0x96, 0x605, 0x3, 0x2, 0x2, 0x2, 0x98, 0x607, 0x3, 0x2, 
    0x2, 0x2, 0x9a, 0x60b, 0x3, 0x2, 0x2, 0x2, 0x9c, 0x60f, 0x3, 0x2, 0x2, 
    0x2, 0x9e, 0x613, 0x3, 0x2, 0x2, 0x2, 0xa0, 0x615, 0x3, 0x2, 0x2, 0x2, 
    0xa2, 0x617, 0x3, 0x2, 0x2, 0x2, 0xa4, 0x619, 0x3, 0x2, 0x2, 0x2, 0xa6, 
    0x61b, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x627, 0x3, 0x2, 0x2, 0x2, 0xaa, 0x62f, 
    0x3, 0x2, 0x2, 0x2, 0xac, 0x631, 0x3, 0x2, 0x2, 0x2, 0xae, 0x633, 0x3, 
    0x2, 0x2, 0x2, 0xb0, 0x635, 0x3, 0x2, 0x2, 0x2, 0xb2, 0x637, 0x3, 0x2, 
    0x2, 0x2, 0xb4, 0x639, 0x3, 0x2, 0x2, 0x2, 0xb6, 0x63c, 0x3, 0x2, 0x2, 
    0x2, 0xb8, 0x63f, 0x3, 0x2, 0x2, 0x2, 0xba, 0x642, 0x3, 0x2, 0x2, 0x2, 
    0xbc, 0x647, 0x3, 0x2, 0x2, 0x2, 0xbe, 0x649, 0x3, 0x2, 0x2, 0x2, 0xc0, 
    0x64c, 0x3, 0x2, 0x2, 0x2, 0xc2, 0x667, 0x3, 0x2, 0x2, 0x2, 0xc4, 0x6bf, 
    0x3, 0x2, 0x2, 0x2, 0xc6, 0xc7, 0x7, 0x1a, 0x2, 0x2, 0xc7, 0xc9, 0x7, 
    0x3, 0x2, 0x2, 0xc8, 0xc6, 0x3, 0x2, 0x2, 0x2, 0xc8, 0xc9, 0x3, 0x2, 
    0x2, 0x2, 0xc9, 0xca, 0x3, 0x2, 0x2, 0x2, 0xca, 0xcb, 0x5, 0x4, 0x3, 
    0x2, 0xcb, 0xcc, 0x7, 0x4, 0x2, 0x2, 0xcc, 0x3, 0x3, 0x2, 0x2, 0x2, 
    0xcd, 0xd1, 0x5, 0x6, 0x4, 0x2, 0xce, 0xd1, 0x5, 0x8, 0x5, 0x2, 0xcf, 
    0xd1, 0x5, 0x14, 0xb, 0x2, 0xd0, 0xcd, 0x3, 0x2, 0x2, 0x2, 0xd0, 0xce, 
    0x3, 0x2, 0x2, 0x2, 0xd0, 0xcf, 0x3, 0x2, 0x2, 0x2, 0xd1, 0x5, 0x3, 
    0x2, 0x2, 0x2, 0xd2, 0xd3, 0x5, 0x16, 0xc, 0x2, 0xd3, 0x7, 0x3, 0x2, 
    0x2, 0x2, 0xd4, 0xd6, 0x5, 0x18, 0xd, 0x2, 0xd5, 0xd4, 0x3, 0x2, 0x2, 
    0x2, 0xd6, 0xd9, 0x3, 0x2, 0x2, 0x2, 0xd7, 0xd5, 0x3, 0x2, 0x2, 0x2, 
    0xd7, 0xd8, 0x3, 0x2, 0x2, 0x2, 0xd8, 0xda, 0x3, 0x2, 0x2, 0x2, 0xd9, 
    0xd7, 0x3, 0x2, 0x2, 0x2, 0xda, 0xde, 0x7, 0x34, 0x2, 0x2, 0xdb, 0xdd, 
    0x5, 0xa, 0x6, 0x2, 0xdc, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xe0, 0x3, 
    0x2, 0x2, 0x2, 0xde, 0xdc, 0x3, 0x2, 0x2, 0x2, 0xde, 0xdf, 0x3, 0x2, 
    0x2, 0x2, 0xdf, 0xe1, 0x3, 0x2, 0x2, 0x2, 0xe0, 0xde, 0x3, 0x2, 0x2, 
    0x2, 0xe1, 0xe2, 0x5, 0x4, 0x3, 0x2, 0xe2, 0x9, 0x3, 0x2, 0x2, 0x2, 
    0xe3, 0xe7, 0x5, 0xc, 0x7, 0x2, 0xe4, 0xe7, 0x5, 0x12, 0xa, 0x2, 0xe5, 
    0xe7, 0x7, 0x76, 0x2, 0x2, 0xe6, 0xe3, 0x3, 0x2, 0x2, 0x2, 0xe6, 0xe4, 
    0x3, 0x2, 0x2, 0x2, 0xe6, 0xe5, 0x3, 0x2, 0x2, 0x2, 0xe7, 0xb, 0x3, 
    0x2, 0x2, 0x2, 0xe8, 0xe9, 0x7, 0x65, 0x2, 0x2, 0xe9, 0xee, 0x5, 0xe, 
    0x8, 0x2, 0xea, 0xeb, 0x7, 0x5, 0x2, 0x2, 0xeb, 0xed, 0x5, 0xe, 0x8, 
    0x2, 0xec, 0xea, 0x3, 0x2, 0x2, 0x2, 0xed, 0xf0, 0x3, 0x2, 0x2, 0x2, 
    0xee, 0xec, 0x3, 0x2, 0x2, 0x2, 0xee, 0xef, 0x3, 0x2, 0x2, 0x2, 0xef, 
    0xd, 0x3, 0x2, 0x2, 0x2, 0xf0, 0xee, 0x3, 0x2, 0x2, 0x2, 0xf1, 0xf4, 
    0x5, 0x10, 0x9, 0x2, 0xf2, 0xf3, 0x9, 0x2, 0x2, 0x2, 0xf3, 0xf5, 0x5, 
    0x10, 0x9, 0x2, 0xf4, 0xf2, 0x3, 0x2, 0x2, 0x2, 0xf4, 0xf5, 0x3, 0x2, 
    0x2, 0x2, 0xf5, 0xf, 0x3, 0x2, 0x2, 0x2, 0xf6, 0xfa, 0x9, 0x3, 0x2, 
    0x2, 0xf7, 0xfa, 0x7, 0xc3, 0x2, 0x2, 0xf8, 0xfa, 0x5, 0x9e, 0x50, 0x2, 
    0xf9, 0xf6, 0x3, 0x2, 0x2, 0x2, 0xf9, 0xf7, 0x3, 0x2, 0x2, 0x2, 0xf9, 
    0xf8, 0x3, 0x2, 0x2, 0x2, 0xfa, 0x11, 0x3, 0x2, 0x2, 0x2, 0xfb, 0xfc, 
    0x7, 0x38, 0x2, 0x2, 0xfc, 0xfd, 0x9, 0x4, 0x2, 0x2, 0xfd, 0x13, 0x3, 
    0x2, 0x2, 0x2, 0xfe, 0x100, 0x5, 0x18, 0xd, 0x2, 0xff, 0xfe, 0x3, 0x2, 
    0x2, 0x2, 0x100, 0x103, 0x3, 0x2, 0x2, 0x2, 0x101, 0xff, 0x3, 0x2, 0x2, 
    0x2, 0x101, 0x102, 0x3, 0x2, 0x2, 0x2, 0x102, 0x104, 0x3, 0x2, 0x2, 
    0x2, 0x103, 0x101, 0x3, 0x2, 0x2, 0x2, 0x104, 0x105, 0x9, 0x5, 0x2, 
    0x2, 0x105, 0x106, 0x5, 0x4, 0x3, 0x2, 0x106, 0x15, 0x3, 0x2, 0x2, 0x2, 
    0x107, 0x109, 0x5, 0x18, 0xd, 0x2, 0x108, 0x107, 0x3, 0x2, 0x2, 0x2, 
    0x109, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x10a, 0x108, 0x3, 0x2, 0x2, 0x2, 
    0x10a, 0x10b, 0x3, 0x2, 0x2, 0x2, 0x10b, 0x115, 0x3, 0x2, 0x2, 0x2, 
    0x10c, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x116, 0x5, 0x1c, 0xf, 0x2, 
    0x10e, 0x116, 0x5, 0x20, 0x11, 0x2, 0x10f, 0x116, 0x5, 0x32, 0x1a, 0x2, 
    0x110, 0x116, 0x5, 0x34, 0x1b, 0x2, 0x111, 0x116, 0x5, 0x36, 0x1c, 0x2, 
    0x112, 0x116, 0x5, 0x38, 0x1d, 0x2, 0x113, 0x116, 0x5, 0x3e, 0x20, 0x2, 
    0x114, 0x116, 0x5, 0x40, 0x21, 0x2, 0x115, 0x10d, 0x3, 0x2, 0x2, 0x2, 
    0x115, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x115, 0x10f, 0x3, 0x2, 0x2, 0x2, 
    0x115, 0x110, 0x3, 0x2, 0x2, 0x2, 0x115, 0x111, 0x3, 0x2, 0x2, 0x2, 
    0x115, 0x112, 0x3, 0x2, 0x2, 0x2, 0x115, 0x113, 0x3, 0x2, 0x2, 0x2, 
    0x115, 0x114, 0x3, 0x2, 0x2, 0x2, 0x116, 0x17, 0x3, 0x2, 0x2, 0x2, 0x117, 
    0x119, 0x7, 0xc4, 0x2, 0x2, 0x118, 0x11a, 0x5, 0x1a, 0xe, 0x2, 0x119, 
    0x118, 0x3, 0x2, 0x2, 0x2, 0x119, 0x11a, 0x3, 0x2, 0x2, 0x2, 0x11a, 
    0x11f, 0x3, 0x2, 0x2, 0x2, 0x11b, 0x11c, 0x7, 0x5, 0x2, 0x2, 0x11c, 
    0x11e, 0x5, 0x1a, 0xe, 0x2, 0x11d, 0x11b, 0x3, 0x2, 0x2, 0x2, 0x11e, 
    0x121, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x11f, 
    0x120, 0x3, 0x2, 0x2, 0x2, 0x120, 0x122, 0x3, 0x2, 0x2, 0x2, 0x121, 
    0x11f, 0x3, 0x2, 0x2, 0x2, 0x122, 0x123, 0x7, 0xc5, 0x2, 0x2, 0x123, 
    0x19, 0x3, 0x2, 0x2, 0x2, 0x124, 0x139, 0x5, 0x98, 0x4d, 0x2, 0x125, 
    0x126, 0x5, 0x98, 0x4d, 0x2, 0x126, 0x127, 0x7, 0x8, 0x2, 0x2, 0x127, 
    0x128, 0x5, 0x9c, 0x4f, 0x2, 0x128, 0x139, 0x3, 0x2, 0x2, 0x2, 0x129, 
    0x12b, 0x5, 0x98, 0x4d, 0x2, 0x12a, 0x12c, 0x7, 0x8, 0x2, 0x2, 0x12b, 
    0x12a, 0x3, 0x2, 0x2, 0x2, 0x12b, 0x12c, 0x3, 0x2, 0x2, 0x2, 0x12c, 
    0x12d, 0x3, 0x2, 0x2, 0x2, 0x12d, 0x12e, 0x7, 0x9, 0x2, 0x2, 0x12e, 
    0x133, 0x5, 0x9c, 0x4f, 0x2, 0x12f, 0x130, 0x7, 0x5, 0x2, 0x2, 0x130, 
    0x132, 0x5, 0x9c, 0x4f, 0x2, 0x131, 0x12f, 0x3, 0x2, 0x2, 0x2, 0x132, 
    0x135, 0x3, 0x2, 0x2, 0x2, 0x133, 0x131, 0x3, 0x2, 0x2, 0x2, 0x133, 
    0x134, 0x3, 0x2, 0x2, 0x2, 0x134, 0x136, 0x3, 0x2, 0x2, 0x2, 0x135, 
    0x133, 0x3, 0x2, 0x2, 0x2, 0x136, 0x137, 0x7, 0xa, 0x2, 0x2, 0x137, 
    0x139, 0x3, 0x2, 0x2, 0x2, 0x138, 0x124, 0x3, 0x2, 0x2, 0x2, 0x138, 
    0x125, 0x3, 0x2, 0x2, 0x2, 0x138, 0x129, 0x3, 0x2, 0x2, 0x2, 0x139, 
    0x1b, 0x3, 0x2, 0x2, 0x2, 0x13a, 0x13b, 0x7, 0x77, 0x2, 0x2, 0x13b, 
    0x140, 0x5, 0x26, 0x14, 0x2, 0x13c, 0x13d, 0x7, 0x5, 0x2, 0x2, 0x13d, 
    0x13f, 0x5, 0x26, 0x14, 0x2, 0x13e, 0x13c, 0x3, 0x2, 0x2, 0x2, 0x13f, 
    0x142, 0x3, 0x2, 0x2, 0x2, 0x140, 0x13e, 0x3, 0x2, 0x2, 0x2, 0x140, 
    0x141, 0x3, 0x2, 0x2, 0x2, 0x141, 0x144, 0x3, 0x2, 0x2, 0x2, 0x142, 
    0x140, 0x3, 0x2, 0x2, 0x2, 0x143, 0x13a, 0x3, 0x2, 0x2, 0x2, 0x143, 
    0x144, 0x3, 0x2, 0x2, 0x2, 0x144, 0x145, 0x3, 0x2, 0x2, 0x2, 0x145, 
    0x149, 0x5, 0x1e, 0x10, 0x2, 0x146, 0x147, 0x7, 0x56, 0x2, 0x2, 0x147, 
    0x148, 0x7, 0x21, 0x2, 0x2, 0x148, 0x14a, 0x5, 0x60, 0x31, 0x2, 0x149, 
    0x146, 0x3, 0x2, 0x2, 0x2, 0x149, 0x14a, 0x3, 0x2, 0x2, 0x2, 0x14a, 
    0x151, 0x3, 0x2, 0x2, 0x2, 0x14b, 0x14c, 0x7, 0x49, 0x2, 0x2, 0x14c, 
    0x14f, 0x7, 0xc3, 0x2, 0x2, 0x14d, 0x14e, 0x7, 0x53, 0x2, 0x2, 0x14e, 
    0x150, 0x7, 0xc3, 0x2, 0x2, 0x14f, 0x14d, 0x3, 0x2, 0x2, 0x2, 0x14f, 
    0x150, 0x3, 0x2, 0x2, 0x2, 0x150, 0x152, 0x3, 0x2, 0x2, 0x2, 0x151, 
    0x14b, 0x3, 0x2, 0x2, 0x2, 0x151, 0x152, 0x3, 0x2, 0x2, 0x2, 0x152, 
    0x1d, 0x3, 0x2, 0x2, 0x2, 0x153, 0x156, 0x9, 0x6, 0x2, 0x2, 0x154, 0x157, 
    0x7, 0x17, 0x2, 0x2, 0x155, 0x157, 0x7, 0x30, 0x2, 0x2, 0x156, 0x154, 
    0x3, 0x2, 0x2, 0x2, 0x156, 0x155, 0x3, 0x2, 0x2, 0x2, 0x156, 0x157, 
    0x3, 0x2, 0x2, 0x2, 0x157, 0x158, 0x3, 0x2, 0x2, 0x2, 0x158, 0x15b, 
    0x5, 0x58, 0x2d, 0x2, 0x159, 0x15a, 0x7, 0x3a, 0x2, 0x2, 0x15a, 0x15c, 
    0x5, 0x28, 0x15, 0x2, 0x15b, 0x159, 0x3, 0x2, 0x2, 0x2, 0x15b, 0x15c, 
    0x3, 0x2, 0x2, 0x2, 0x15c, 0x15f, 0x3, 0x2, 0x2, 0x2, 0x15d, 0x15e, 
    0x7, 0x75, 0x2, 0x2, 0x15e, 0x160, 0x5, 0x72, 0x3a, 0x2, 0x15f, 0x15d, 
    0x3, 0x2, 0x2, 0x2, 0x15f, 0x160, 0x3, 0x2, 0x2, 0x2, 0x160, 0x164, 
    0x3, 0x2, 0x2, 0x2, 0x161, 0x162, 0x7, 0x3b, 0x2, 0x2, 0x162, 0x163, 
    0x7, 0x21, 0x2, 0x2, 0x163, 0x165, 0x5, 0x5c, 0x2f, 0x2, 0x164, 0x161, 
    0x3, 0x2, 0x2, 0x2, 0x164, 0x165, 0x3, 0x2, 0x2, 0x2, 0x165, 0x168, 
    0x3, 0x2, 0x2, 0x2, 0x166, 0x167, 0x7, 0x3c, 0x2, 0x2, 0x167, 0x169, 
    0x5, 0x72, 0x3a, 0x2, 0x168, 0x166, 0x3, 0x2, 0x2, 0x2, 0x168, 0x169, 
    0x3, 0x2, 0x2, 0x2, 0x169, 0x16f, 0x3, 0x2, 0x2, 0x2, 0x16a, 0x16b, 
    0x7, 0x9, 0x2, 0x2, 0x16b, 0x16c, 0x5, 0x1e, 0x10, 0x2, 0x16c, 0x16d, 
    0x7, 0xa, 0x2, 0x2, 0x16d, 0x16f, 0x3, 0x2, 0x2, 0x2, 0x16e, 0x153, 
    0x3, 0x2, 0x2, 0x2, 0x16e, 0x16a, 0x3, 0x2, 0x2, 0x2, 0x16f, 0x1f, 0x3, 
    0x2, 0x2, 0x2, 0x170, 0x171, 0x7, 0x77, 0x2, 0x2, 0x171, 0x176, 0x5, 
    0x26, 0x14, 0x2, 0x172, 0x173, 0x7, 0x5, 0x2, 0x2, 0x173, 0x175, 0x5, 
    0x26, 0x14, 0x2, 0x174, 0x172, 0x3, 0x2, 0x2, 0x2, 0x175, 0x178, 0x3, 
    0x2, 0x2, 0x2, 0x176, 0x174, 0x3, 0x2, 0x2, 0x2, 0x176, 0x177, 0x3, 
    0x2, 0x2, 0x2, 0x177, 0x17a, 0x3, 0x2, 0x2, 0x2, 0x178, 0x176, 0x3, 
    0x2, 0x2, 0x2, 0x179, 0x170, 0x3, 0x2, 0x2, 0x2, 0x179, 0x17a, 0x3, 
    0x2, 0x2, 0x2, 0x17a, 0x17b, 0x3, 0x2, 0x2, 0x2, 0x17b, 0x17f, 0x5, 
    0x22, 0x12, 0x2, 0x17c, 0x17d, 0x7, 0x56, 0x2, 0x2, 0x17d, 0x17e, 0x7, 
    0x21, 0x2, 0x2, 0x17e, 0x180, 0x5, 0x60, 0x31, 0x2, 0x17f, 0x17c, 0x3, 
    0x2, 0x2, 0x2, 0x17f, 0x180, 0x3, 0x2, 0x2, 0x2, 0x180, 0x187, 0x3, 
    0x2, 0x2, 0x2, 0x181, 0x182, 0x7, 0x49, 0x2, 0x2, 0x182, 0x185, 0x7, 
    0xc3, 0x2, 0x2, 0x183, 0x184, 0x7, 0x53, 0x2, 0x2, 0x184, 0x186, 0x7, 
    0xc3, 0x2, 0x2, 0x185, 0x183, 0x3, 0x2, 0x2, 0x2, 0x185, 0x186, 0x3, 
    0x2, 0x2, 0x2, 0x186, 0x188, 0x3, 0x2, 0x2, 0x2, 0x187, 0x181, 0x3, 
    0x2, 0x2, 0x2, 0x187, 0x188, 0x3, 0x2, 0x2, 0x2, 0x188, 0x21, 0x3, 0x2, 
    0x2, 0x2, 0x189, 0x18a, 0x8, 0x12, 0x1, 0x2, 0x18a, 0x190, 0x5, 0x1e, 
    0x10, 0x2, 0x18b, 0x18c, 0x7, 0x9, 0x2, 0x2, 0x18c, 0x18d, 0x5, 0x22, 
    0x12, 0x2, 0x18d, 0x18e, 0x7, 0xa, 0x2, 0x2, 0x18e, 0x190, 0x3, 0x2, 
    0x2, 0x2, 0x18f, 0x189, 0x3, 0x2, 0x2, 0x2, 0x18f, 0x18b, 0x3, 0x2, 
    0x2, 0x2, 0x190, 0x19d, 0x3, 0x2, 0x2, 0x2, 0x191, 0x192, 0xc, 0x4, 
    0x2, 0x2, 0x192, 0x195, 0x9, 0x7, 0x2, 0x2, 0x193, 0x196, 0x7, 0x30, 
    0x2, 0x2, 0x194, 0x196, 0x7, 0x17, 0x2, 0x2, 0x195, 0x193, 0x3, 0x2, 
    0x2, 0x2, 0x195, 0x194, 0x3, 0x2, 0x2, 0x2, 0x195, 0x196, 0x3, 0x2, 
    0x2, 0x2, 0x196, 0x198, 0x3, 0x2, 0x2, 0x2, 0x197, 0x199, 0x5, 0x24, 
    0x13, 0x2, 0x198, 0x197, 0x3, 0x2, 0x2, 0x2, 0x198, 0x199, 0x3, 0x2, 
    0x2, 0x2, 0x199, 0x19a, 0x3, 0x2, 0x2, 0x2, 0x19a, 0x19c, 0x5, 0x22, 
    0x12, 0x5, 0x19b, 0x191, 0x3, 0x2, 0x2, 0x2, 0x19c, 0x19f, 0x3, 0x2, 
    0x2, 0x2, 0x19d, 0x19b, 0x3, 0x2, 0x2, 0x2, 0x19d, 0x19e, 0x3, 0x2, 
    0x2, 0x2, 0x19e, 0x23, 0x3, 0x2, 0x2, 0x2, 0x19f, 0x19d, 0x3, 0x2, 0x2, 
    0x2, 0x1a0, 0x1a6, 0x7, 0x24, 0x2, 0x2, 0x1a1, 0x1a2, 0x7, 0x21, 0x2, 
    0x2, 0x1a2, 0x1a3, 0x7, 0x9, 0x2, 0x2, 0x1a3, 0x1a4, 0x5, 0x68, 0x35, 
    0x2, 0x1a4, 0x1a5, 0x7, 0xa, 0x2, 0x2, 0x1a5, 0x1a7, 0x3, 0x2, 0x2, 
    0x2, 0x1a6, 0x1a1, 0x3, 0x2, 0x2, 0x2, 0x1a6, 0x1a7, 0x3, 0x2, 0x2, 
    0x2, 0x1a7, 0x25, 0x3, 0x2, 0x2, 0x2, 0x1a8, 0x1b4, 0x5, 0x8a, 0x46, 
    0x2, 0x1a9, 0x1aa, 0x7, 0x9, 0x2, 0x2, 0x1aa, 0x1af, 0x5, 0x90, 0x49, 
    0x2, 0x1ab, 0x1ac, 0x7, 0x5, 0x2, 0x2, 0x1ac, 0x1ae, 0x5, 0x90, 0x49, 
    0x2, 0x1ad, 0x1ab, 0x3, 0x2, 0x2, 0x2, 0x1ae, 0x1b1, 0x3, 0x2, 0x2, 
    0x2, 0x1af, 0x1ad, 0x3, 0x2, 0x2, 0x2, 0x1af, 0x1b0, 0x3, 0x2, 0x2, 
    0x2, 0x1b0, 0x1b2, 0x3, 0x2, 0x2, 0x2, 0x1b1, 0x1af, 0x3, 0x2, 0x2, 
    0x2, 0x1b2, 0x1b3, 0x7, 0xa, 0x2, 0x2, 0x1b3, 0x1b5, 0x3, 0x2, 0x2, 
    0x2, 0x1b4, 0x1a9, 0x3, 0x2, 0x2, 0x2, 0x1b4, 0x1b5, 0x3, 0x2, 0x2, 
    0x2, 0x1b5, 0x1b6, 0x3, 0x2, 0x2, 0x2, 0x1b6, 0x1b7, 0x7, 0x1c, 0x2, 
    0x2, 0x1b7, 0x1b8, 0x7, 0x9, 0x2, 0x2, 0x1b8, 0x1b9, 0x5, 0x16, 0xc, 
    0x2, 0x1b9, 0x1ba, 0x7, 0xa, 0x2, 0x2, 0x1ba, 0x27, 0x3, 0x2, 0x2, 0x2, 
    0x1bb, 0x1bc, 0x8, 0x15, 0x1, 0x2, 0x1bc, 0x1c1, 0x5, 0x2a, 0x16, 0x2, 
    0x1bd, 0x1bf, 0x7, 0x1c, 0x2, 0x2, 0x1be, 0x1bd, 0x3, 0x2, 0x2, 0x2, 
    0x1be, 0x1bf, 0x3, 0x2, 0x2, 0x2, 0x1bf, 0x1c0, 0x3, 0x2, 0x2, 0x2, 
    0x1c0, 0x1c2, 0x5, 0x8c, 0x47, 0x2, 0x1c1, 0x1be, 0x3, 0x2, 0x2, 0x2, 
    0x1c1, 0x1c2, 0x3, 0x2, 0x2, 0x2, 0x1c2, 0x1d6, 0x3, 0x2, 0x2, 0x2, 
    0x1c3, 0x1c4, 0x7, 0x9, 0x2, 0x2, 0x1c4, 0x1c5, 0x5, 0x16, 0xc, 0x2, 
    0x1c5, 0x1ca, 0x7, 0xa, 0x2, 0x2, 0x1c6, 0x1c8, 0x7, 0x1c, 0x2, 0x2, 
    0x1c7, 0x1c6, 0x3, 0x2, 0x2, 0x2, 0x1c7, 0x1c8, 0x3, 0x2, 0x2, 0x2, 
    0x1c8, 0x1c9, 0x3, 0x2, 0x2, 0x2, 0x1c9, 0x1cb, 0x5, 0x8c, 0x47, 0x2, 
    0x1ca, 0x1c7, 0x3, 0x2, 0x2, 0x2, 0x1ca, 0x1cb, 0x3, 0x2, 0x2, 0x2, 
    0x1cb, 0x1d6, 0x3, 0x2, 0x2, 0x2, 0x1cc, 0x1cd, 0x7, 0x9, 0x2, 0x2, 
    0x1cd, 0x1ce, 0x5, 0x30, 0x19, 0x2, 0x1ce, 0x1d3, 0x7, 0xa, 0x2, 0x2, 
    0x1cf, 0x1d1, 0x7, 0x1c, 0x2, 0x2, 0x1d0, 0x1cf, 0x3, 0x2, 0x2, 0x2, 
    0x1d0, 0x1d1, 0x3, 0x2, 0x2, 0x2, 0x1d1, 0x1d2, 0x3, 0x2, 0x2, 0x2, 
    0x1d2, 0x1d4, 0x5, 0x8c, 0x47, 0x2, 0x1d3, 0x1d0, 0x3, 0x2, 0x2, 0x2, 
    0x1d3, 0x1d4, 0x3, 0x2, 0x2, 0x2, 0x1d4, 0x1d6, 0x3, 0x2, 0x2, 0x2, 
    0x1d5, 0x1bb, 0x3, 0x2, 0x2, 0x2, 0x1d5, 0x1c3, 0x3, 0x2, 0x2, 0x2, 
    0x1d5, 0x1cc, 0x3, 0x2, 0x2, 0x2, 0x1d6, 0x1fa, 0x3, 0x2, 0x2, 0x2, 
    0x1d7, 0x1db, 0xc, 0x5, 0x2, 0x2, 0x1d8, 0x1dc, 0x7, 0x5, 0x2, 0x2, 
    0x1d9, 0x1da, 0x7, 0x26, 0x2, 0x2, 0x1da, 0x1dc, 0x7, 0x44, 0x2, 0x2, 
    0x1db, 0x1d8, 0x3, 0x2, 0x2, 0x2, 0x1db, 0x1d9, 0x3, 0x2, 0x2, 0x2, 
    0x1dc, 0x1dd, 0x3, 0x2, 0x2, 0x2, 0x1dd, 0x1f9, 0x5, 0x28, 0x15, 0x6, 
    0x1de, 0x1e0, 0xc, 0x7, 0x2, 0x2, 0x1df, 0x1e1, 0x5, 0x2c, 0x17, 0x2, 
    0x1e0, 0x1df, 0x3, 0x2, 0x2, 0x2, 0x1e0, 0x1e1, 0x3, 0x2, 0x2, 0x2, 
    0x1e1, 0x1e3, 0x3, 0x2, 0x2, 0x2, 0x1e2, 0x1e4, 0x5, 0x2e, 0x18, 0x2, 
    0x1e3, 0x1e2, 0x3, 0x2, 0x2, 0x2, 0x1e3, 0x1e4, 0x3, 0x2, 0x2, 0x2, 
    0x1e4, 0x1e5, 0x3, 0x2, 0x2, 0x2, 0x1e5, 0x1e6, 0x7, 0x44, 0x2, 0x2, 
    0x1e6, 0x1e7, 0x5, 0x28, 0x15, 0x2, 0x1e7, 0x1e8, 0x7, 0x54, 0x2, 0x2, 
    0x1e8, 0x1e9, 0x5, 0x74, 0x3b, 0x2, 0x1e9, 0x1f9, 0x3, 0x2, 0x2, 0x2, 
    0x1ea, 0x1ec, 0xc, 0x6, 0x2, 0x2, 0x1eb, 0x1ed, 0x5, 0x2c, 0x17, 0x2, 
    0x1ec, 0x1eb, 0x3, 0x2, 0x2, 0x2, 0x1ec, 0x1ed, 0x3, 0x2, 0x2, 0x2, 
    0x1ed, 0x1ef, 0x3, 0x2, 0x2, 0x2, 0x1ee, 0x1f0, 0x5, 0x2e, 0x18, 0x2, 
    0x1ef, 0x1ee, 0x3, 0x2, 0x2, 0x2, 0x1ef, 0x1f0, 0x3, 0x2, 0x2, 0x2, 
    0x1f0, 0x1f1, 0x3, 0x2, 0x2, 0x2, 0x1f1, 0x1f2, 0x7, 0x44, 0x2, 0x2, 
    0x1f2, 0x1f3, 0x5, 0x28, 0x15, 0x2, 0x1f3, 0x1f4, 0x7, 0x73, 0x2, 0x2, 
    0x1f4, 0x1f5, 0x7, 0x9, 0x2, 0x2, 0x1f5, 0x1f6, 0x5, 0x64, 0x33, 0x2, 
    0x1f6, 0x1f7, 0x7, 0xa, 0x2, 0x2, 0x1f7, 0x1f9, 0x3, 0x2, 0x2, 0x2, 
    0x1f8, 0x1d7, 0x3, 0x2, 0x2, 0x2, 0x1f8, 0x1de, 0x3, 0x2, 0x2, 0x2, 
    0x1f8, 0x1ea, 0x3, 0x2, 0x2, 0x2, 0x1f9, 0x1fc, 0x3, 0x2, 0x2, 0x2, 
    0x1fa, 0x1f8, 0x3, 0x2, 0x2, 0x2, 0x1fa, 0x1fb, 0x3, 0x2, 0x2, 0x2, 
    0x1fb, 0x29, 0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1fa, 0x3, 0x2, 0x2, 0x2, 0x1fd, 
    0x1fe, 0x5, 0x86, 0x44, 0x2, 0x1fe, 0x1ff, 0x7, 0xc, 0x2, 0x2, 0x1ff, 
    0x201, 0x3, 0x2, 0x2, 0x2, 0x200, 0x1fd, 0x3, 0x2, 0x2, 0x2, 0x200, 
    0x201, 0x3, 0x2, 0x2, 0x2, 0x201, 0x202, 0x3, 0x2, 0x2, 0x2, 0x202, 
    0x203, 0x5, 0x88, 0x45, 0x2, 0x203, 0x204, 0x7, 0xc, 0x2, 0x2, 0x204, 
    0x206, 0x3, 0x2, 0x2, 0x2, 0x205, 0x200, 0x3, 0x2, 0x2, 0x2, 0x205, 
    0x206, 0x3, 0x2, 0x2, 0x2, 0x206, 0x207, 0x3, 0x2, 0x2, 0x2, 0x207, 
    0x208, 0x5, 0x8a, 0x46, 0x2, 0x208, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x209, 
    0x212, 0x7, 0x3f, 0x2, 0x2, 0x20a, 0x20c, 0x9, 0x8, 0x2, 0x2, 0x20b, 
    0x20d, 0x7, 0x57, 0x2, 0x2, 0x20c, 0x20b, 0x3, 0x2, 0x2, 0x2, 0x20c, 
    0x20d, 0x3, 0x2, 0x2, 0x2, 0x20d, 0x212, 0x3, 0x2, 0x2, 0x2, 0x20e, 
    0x212, 0x7, 0x4d, 0x2, 0x2, 0x20f, 0x212, 0x7, 0x63, 0x2, 0x2, 0x210, 
    0x212, 0x7, 0x19, 0x2, 0x2, 0x211, 0x209, 0x3, 0x2, 0x2, 0x2, 0x211, 
    0x20a, 0x3, 0x2, 0x2, 0x2, 0x211, 0x20e, 0x3, 0x2, 0x2, 0x2, 0x211, 
    0x20f, 0x3, 0x2, 0x2, 0x2, 0x211, 0x210, 0x3, 0x2, 0x2, 0x2, 0x212, 
    0x2d, 0x3, 0x2, 0x2, 0x2, 0x213, 0x214, 0x7, 0xc4, 0x2, 0x2, 0x214, 
    0x223, 0x9, 0x9, 0x2, 0x2, 0x215, 0x217, 0x7, 0x8, 0x2, 0x2, 0x216, 
    0x215, 0x3, 0x2, 0x2, 0x2, 0x216, 0x217, 0x3, 0x2, 0x2, 0x2, 0x217, 
    0x218, 0x3, 0x2, 0x2, 0x2, 0x218, 0x219, 0x7, 0x9, 0x2, 0x2, 0x219, 
    0x21e, 0x5, 0x9c, 0x4f, 0x2, 0x21a, 0x21b, 0x7, 0x5, 0x2, 0x2, 0x21b, 
    0x21d, 0x5, 0x9c, 0x4f, 0x2, 0x21c, 0x21a, 0x3, 0x2, 0x2, 0x2, 0x21d, 
    0x220, 0x3, 0x2, 0x2, 0x2, 0x21e, 0x21c, 0x3, 0x2, 0x2, 0x2, 0x21e, 
    0x21f, 0x3, 0x2, 0x2, 0x2, 0x21f, 0x221, 0x3, 0x2, 0x2, 0x2, 0x220, 
    0x21e, 0x3, 0x2, 0x2, 0x2, 0x221, 0x222, 0x7, 0xa, 0x2, 0x2, 0x222, 
    0x224, 0x3, 0x2, 0x2, 0x2, 0x223, 0x216, 0x3, 0x2, 0x2, 0x2, 0x223, 
    0x224, 0x3, 0x2, 0x2, 0x2, 0x224, 0x225, 0x3, 0x2, 0x2, 0x2, 0x225, 
    0x226, 0x7, 0xc5, 0x2, 0x2, 0x226, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x227, 
    0x228, 0x7, 0x74, 0x2, 0x2, 0x228, 0x229, 0x7, 0x9, 0x2, 0x2, 0x229, 
    0x22a, 0x5, 0x70, 0x39, 0x2, 0x22a, 0x232, 0x7, 0xa, 0x2, 0x2, 0x22b, 
    0x22c, 0x7, 0x5, 0x2, 0x2, 0x22c, 0x22d, 0x7, 0x9, 0x2, 0x2, 0x22d, 
    0x22e, 0x5, 0x70, 0x39, 0x2, 0x22e, 0x22f, 0x7, 0xa, 0x2, 0x2, 0x22f, 
    0x231, 0x3, 0x2, 0x2, 0x2, 0x230, 0x22b, 0x3, 0x2, 0x2, 0x2, 0x231, 
    0x234, 0x3, 0x2, 0x2, 0x2, 0x232, 0x230, 0x3, 0x2, 0x2, 0x2, 0x232, 
    0x233, 0x3, 0x2, 0x2, 0x2, 0x233, 0x31, 0x3, 0x2, 0x2, 0x2, 0x234, 0x232, 
    0x3, 0x2, 0x2, 0x2, 0x235, 0x236, 0x7, 0x77, 0x2, 0x2, 0x236, 0x23b, 
    0x5, 0x26, 0x14, 0x2, 0x237, 0x238, 0x7, 0x5, 0x2, 0x2, 0x238, 0x23a, 
    0x5, 0x26, 0x14, 0x2, 0x239, 0x237, 0x3, 0x2, 0x2, 0x2, 0x23a, 0x23d, 
    0x3, 0x2, 0x2, 0x2, 0x23b, 0x239, 0x3, 0x2, 0x2, 0x2, 0x23b, 0x23c, 
    0x3, 0x2, 0x2, 0x2, 0x23c, 0x23f, 0x3, 0x2, 0x2, 0x2, 0x23d, 0x23b, 
    0x3, 0x2, 0x2, 0x2, 0x23e, 0x235, 0x3, 0x2, 0x2, 0x2, 0x23e, 0x23f, 
    0x3, 0x2, 0x2, 0x2, 0x23f, 0x240, 0x3, 0x2, 0x2, 0x2, 0x240, 0x242, 
    0x7, 0x40, 0x2, 0x2, 0x241, 0x243, 0x7, 0x42, 0x2, 0x2, 0x242, 0x241, 
    0x3, 0x2, 0x2, 0x2, 0x242, 0x243, 0x3, 0x2, 0x2, 0x2, 0x243, 0x244, 
    0x3, 0x2, 0x2, 0x2, 0x244, 0x250, 0x5, 0x2a, 0x16, 0x2, 0x245, 0x246, 
    0x7, 0x9, 0x2, 0x2, 0x246, 0x24b, 0x5, 0x8e, 0x48, 0x2, 0x247, 0x248, 
    0x7, 0x5, 0x2, 0x2, 0x248, 0x24a, 0x5, 0x8e, 0x48, 0x2, 0x249, 0x247, 
    0x3, 0x2, 0x2, 0x2, 0x24a, 0x24d, 0x3, 0x2, 0x2, 0x2, 0x24b, 0x249, 
    0x3, 0x2, 0x2, 0x2, 0x24b, 0x24c, 0x3, 0x2, 0x2, 0x2, 0x24c, 0x24e, 
    0x3, 0x2, 0x2, 0x2, 0x24d, 0x24b, 0x3, 0x2, 0x2, 0x2, 0x24e, 0x24f, 
    0x7, 0xa, 0x2, 0x2, 0x24f, 0x251, 0x3, 0x2, 0x2, 0x2, 0x250, 0x245, 
    0x3, 0x2, 0x2, 0x2, 0x250, 0x251, 0x3, 0x2, 0x2, 0x2, 0x251, 0x255, 
    0x3, 0x2, 0x2, 0x2, 0x252, 0x256, 0x5, 0x30, 0x19, 0x2, 0x253, 0x256, 
    0x5, 0x1c, 0xf, 0x2, 0x254, 0x256, 0x5, 0x20, 0x11, 0x2, 0x255, 0x252, 
    0x3, 0x2, 0x2, 0x2, 0x255, 0x253, 0x3, 0x2, 0x2, 0x2, 0x255, 0x254, 
    0x3, 0x2, 0x2, 0x2, 0x256, 0x33, 0x3, 0x2, 0x2, 0x2, 0x257, 0x258, 0x7, 
    0x77, 0x2, 0x2, 0x258, 0x25d, 0x5, 0x26, 0x14, 0x2, 0x259, 0x25a, 0x7, 
    0x5, 0x2, 0x2, 0x25a, 0x25c, 0x5, 0x26, 0x14, 0x2, 0x25b, 0x259, 0x3, 
    0x2, 0x2, 0x2, 0x25c, 0x25f, 0x3, 0x2, 0x2, 0x2, 0x25d, 0x25b, 0x3, 
    0x2, 0x2, 0x2, 0x25d, 0x25e, 0x3, 0x2, 0x2, 0x2, 0x25e, 0x261, 0x3, 
    0x2, 0x2, 0x2, 0x25f, 0x25d, 0x3, 0x2, 0x2, 0x2, 0x260, 0x257, 0x3, 
    0x2, 0x2, 0x2, 0x260, 0x261, 0x3, 0x2, 0x2, 0x2, 0x261, 0x262, 0x3, 
    0x2, 0x2, 0x2, 0x262, 0x264, 0x7, 0x2d, 0x2, 0x2, 0x263, 0x265, 0x7, 
    0x3a, 0x2, 0x2, 0x264, 0x263, 0x3, 0x2, 0x2, 0x2, 0x264, 0x265, 0x3, 
    0x2, 0x2, 0x2, 0x265, 0x266, 0x3, 0x2, 0x2, 0x2, 0x266, 0x26b, 0x5, 
    0x2a, 0x16, 0x2, 0x267, 0x268, 0x7, 0x75, 0x2, 0x2, 0x268, 0x26c, 0x5, 
    0x72, 0x3a, 0x2, 0x269, 0x26a, 0x7, 0x3a, 0x2, 0x2, 0x26a, 0x26c, 0x5, 
    0x28, 0x15, 0x2, 0x26b, 0x267, 0x3, 0x2, 0x2, 0x2, 0x26b, 0x269, 0x3, 
    0x2, 0x2, 0x2, 0x26b, 0x26c, 0x3, 0x2, 0x2, 0x2, 0x26c, 0x35, 0x3, 0x2, 
    0x2, 0x2, 0x26d, 0x26e, 0x7, 0x77, 0x2, 0x2, 0x26e, 0x273, 0x5, 0x26, 
    0x14, 0x2, 0x26f, 0x270, 0x7, 0x5, 0x2, 0x2, 0x270, 0x272, 0x5, 0x26, 
    0x14, 0x2, 0x271, 0x26f, 0x3, 0x2, 0x2, 0x2, 0x272, 0x275, 0x3, 0x2, 
    0x2, 0x2, 0x273, 0x271, 0x3, 0x2, 0x2, 0x2, 0x273, 0x274, 0x3, 0x2, 
    0x2, 0x2, 0x274, 0x277, 0x3, 0x2, 0x2, 0x2, 0x275, 0x273, 0x3, 0x2, 
    0x2, 0x2, 0x276, 0x26d, 0x3, 0x2, 0x2, 0x2, 0x276, 0x277, 0x3, 0x2, 
    0x2, 0x2, 0x277, 0x278, 0x3, 0x2, 0x2, 0x2, 0x278, 0x279, 0x7, 0x72, 
    0x2, 0x2, 0x279, 0x27a, 0x5, 0x2a, 0x16, 0x2, 0x27a, 0x27b, 0x7, 0x64, 
    0x2, 0x2, 0x27b, 0x27c, 0x5, 0x8e, 0x48, 0x2, 0x27c, 0x27d, 0x7, 0x8, 
    0x2, 0x2, 0x27d, 0x285, 0x5, 0x78, 0x3d, 0x2, 0x27e, 0x27f, 0x7, 0x5, 
    0x2, 0x2, 0x27f, 0x280, 0x5, 0x8e, 0x48, 0x2, 0x280, 0x281, 0x7, 0x8, 
    0x2, 0x2, 0x281, 0x282, 0x5, 0x78, 0x3d, 0x2, 0x282, 0x284, 0x3, 0x2, 
    0x2, 0x2, 0x283, 0x27e, 0x3, 0x2, 0x2, 0x2, 0x284, 0x287, 0x3, 0x2, 
    0x2, 0x2, 0x285, 0x283, 0x3, 0x2, 0x2, 0x2, 0x285, 0x286, 0x3, 0x2, 
    0x2, 0x2, 0x286, 0x28c, 0x3, 0x2, 0x2, 0x2, 0x287, 0x285, 0x3, 0x2, 
    0x2, 0x2, 0x288, 0x289, 0x7, 0x75, 0x2, 0x2, 0x289, 0x28d, 0x5, 0x72, 
    0x3a, 0x2, 0x28a, 0x28b, 0x7, 0x3a, 0x2, 0x2, 0x28b, 0x28d, 0x5, 0x28, 
    0x15, 0x2, 0x28c, 0x288, 0x3, 0x2, 0x2, 0x2, 0x28c, 0x28a, 0x3, 0x2, 
    0x2, 0x2, 0x28c, 0x28d, 0x3, 0x2, 0x2, 0x2, 0x28d, 0x37, 0x3, 0x2, 0x2, 
    0x2, 0x28e, 0x291, 0x5, 0x3a, 0x1e, 0x2, 0x28f, 0x291, 0x5, 0x3c, 0x1f, 
    0x2, 0x290, 0x28e, 0x3, 0x2, 0x2, 0x2, 0x290, 0x28f, 0x3, 0x2, 0x2, 
    0x2, 0x291, 0x39, 0x3, 0x2, 0x2, 0x2, 0x292, 0x294, 0x7, 0x25, 0x2, 
    0x2, 0x293, 0x295, 0x7, 0x5e, 0x2, 0x2, 0x294, 0x293, 0x3, 0x2, 0x2, 
    0x2, 0x294, 0x295, 0x3, 0x2, 0x2, 0x2, 0x295, 0x296, 0x3, 0x2, 0x2, 
    0x2, 0x296, 0x29a, 0x7, 0x69, 0x2, 0x2, 0x297, 0x298, 0x7, 0x96, 0x2, 
    0x2, 0x298, 0x299, 0x7, 0x50, 0x2, 0x2, 0x299, 0x29b, 0x7, 0x95, 0x2, 
    0x2, 0x29a, 0x297, 0x3, 0x2, 0x2, 0x2, 0x29a, 0x29b, 0x3, 0x2, 0x2, 
    0x2, 0x29b, 0x29c, 0x3, 0x2, 0x2, 0x2, 0x29c, 0x2b8, 0x5, 0x2a, 0x16, 
    0x2, 0x29d, 0x2b9, 0x5, 0x42, 0x22, 0x2, 0x29e, 0x2a0, 0x5, 0x42, 0x22, 
    0x2, 0x29f, 0x29e, 0x3, 0x2, 0x2, 0x2, 0x29f, 0x2a0, 0x3, 0x2, 0x2, 
    0x2, 0x2a0, 0x2a2, 0x3, 0x2, 0x2, 0x2, 0x2a1, 0x2a3, 0x7, 0x1c, 0x2, 
    0x2, 0x2a2, 0x2a1, 0x3, 0x2, 0x2, 0x2, 0x2a2, 0x2a3, 0x3, 0x2, 0x2, 
    0x2, 0x2a3, 0x2a4, 0x3, 0x2, 0x2, 0x2, 0x2a4, 0x2aa, 0x5, 0x54, 0x2b, 
    0x2, 0x2a5, 0x2a7, 0x7, 0x77, 0x2, 0x2, 0x2a6, 0x2a8, 0x7, 0x4e, 0x2, 
    0x2, 0x2a7, 0x2a6, 0x3, 0x2, 0x2, 0x2, 0x2a7, 0x2a8, 0x3, 0x2, 0x2, 
    0x2, 0x2a8, 0x2a9, 0x3, 0x2, 0x2, 0x2, 0x2a9, 0x2ab, 0x7, 0x2b, 0x2, 
    0x2, 0x2aa, 0x2a5, 0x3, 0x2, 0x2, 0x2, 0x2aa, 0x2ab, 0x3, 0x2, 0x2, 
    0x2, 0x2ab, 0x2b9, 0x3, 0x2, 0x2, 0x2, 0x2ac, 0x2ae, 0x5, 0x48, 0x25, 
    0x2, 0x2ad, 0x2af, 0x7, 0x1c, 0x2, 0x2, 0x2ae, 0x2ad, 0x3, 0x2, 0x2, 
    0x2, 0x2ae, 0x2af, 0x3, 0x2, 0x2, 0x2, 0x2af, 0x2b0, 0x3, 0x2, 0x2, 
    0x2, 0x2b0, 0x2b6, 0x5, 0x54, 0x2b, 0x2, 0x2b1, 0x2b3, 0x7, 0x77, 0x2, 
    0x2, 0x2b2, 0x2b4, 0x7, 0x4e, 0x2, 0x2, 0x2b3, 0x2b2, 0x3, 0x2, 0x2, 
    0x2, 0x2b3, 0x2b4, 0x3, 0x2, 0x2, 0x2, 0x2b4, 0x2b5, 0x3, 0x2, 0x2, 
    0x2, 0x2b5, 0x2b7, 0x7, 0x2b, 0x2, 0x2, 0x2b6, 0x2b1, 0x3, 0x2, 0x2, 
    0x2, 0x2b6, 0x2b7, 0x3, 0x2, 0x2, 0x2, 0x2b7, 0x2b9, 0x3, 0x2, 0x2, 
    0x2, 0x2b8, 0x29d, 0x3, 0x2, 0x2, 0x2, 0x2b8, 0x29f, 0x3, 0x2, 0x2, 
    0x2, 0x2b8, 0x2ac, 0x3, 0x2, 0x2, 0x2, 0x2b9, 0x3b, 0x3, 0x2, 0x2, 0x2, 
    0x2ba, 0x2bc, 0x7, 0x25, 0x2, 0x2, 0x2bb, 0x2bd, 0x7, 0x71, 0x2, 0x2, 
    0x2bc, 0x2bb, 0x3, 0x2, 0x2, 0x2, 0x2bc, 0x2bd, 0x3, 0x2, 0x2, 0x2, 
    0x2bd, 0x2be, 0x3, 0x2, 0x2, 0x2, 0x2be, 0x2c2, 0x9, 0xa, 0x2, 0x2, 
    0x2bf, 0x2c0, 0x7, 0x96, 0x2, 0x2, 0x2c0, 0x2c1, 0x7, 0x50, 0x2, 0x2, 
    0x2c1, 0x2c3, 0x7, 0x95, 0x2, 0x2, 0x2c2, 0x2bf, 0x3, 0x2, 0x2, 0x2, 
    0x2c2, 0x2c3, 0x3, 0x2, 0x2, 0x2, 0x2c3, 0x2c4, 0x3, 0x2, 0x2, 0x2, 
    0x2c4, 0x2c5, 0x5, 0x94, 0x4b, 0x2, 0x2c5, 0x2c6, 0x7, 0x54, 0x2, 0x2, 
    0x2c6, 0x2c9, 0x5, 0x2a, 0x16, 0x2, 0x2c7, 0x2c8, 0x7, 0x73, 0x2, 0x2, 
    0x2c8, 0x2ca, 0x5, 0x4e, 0x28, 0x2, 0x2c9, 0x2c7, 0x3, 0x2, 0x2, 0x2, 
    0x2c9, 0x2ca, 0x3, 0x2, 0x2, 0x2, 0x2ca, 0x2cb, 0x3, 0x2, 0x2, 0x2, 
    0x2cb, 0x2cc, 0x7, 0x9, 0x2, 0x2, 0x2cc, 0x2d1, 0x5, 0x78, 0x3d, 0x2, 
    0x2cd, 0x2ce, 0x7, 0x5, 0x2, 0x2, 0x2ce, 0x2d0, 0x5, 0x78, 0x3d, 0x2, 
    0x2cf, 0x2cd, 0x3, 0x2, 0x2, 0x2, 0x2d0, 0x2d3, 0x3, 0x2, 0x2, 0x2, 
    0x2d1, 0x2cf, 0x3, 0x2, 0x2, 0x2, 0x2d1, 0x2d2, 0x3, 0x2, 0x2, 0x2, 
    0x2d2, 0x2d4, 0x3, 0x2, 0x2, 0x2, 0x2d3, 0x2d1, 0x3, 0x2, 0x2, 0x2, 
    0x2d4, 0x2d7, 0x7, 0xa, 0x2, 0x2, 0x2d5, 0x2d6, 0x7, 0x77, 0x2, 0x2, 
    0x2d6, 0x2d8, 0x5, 0x6c, 0x37, 0x2, 0x2d7, 0x2d5, 0x3, 0x2, 0x2, 0x2, 
    0x2d7, 0x2d8, 0x3, 0x2, 0x2, 0x2, 0x2d8, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x2d9, 
    0x2da, 0x7, 0x31, 0x2, 0x2, 0x2da, 0x2dd, 0x7, 0x69, 0x2, 0x2, 0x2db, 
    0x2dc, 0x7, 0x96, 0x2, 0x2, 0x2dc, 0x2de, 0x7, 0x95, 0x2, 0x2, 0x2dd, 
    0x2db, 0x3, 0x2, 0x2, 0x2, 0x2dd, 0x2de, 0x3, 0x2, 0x2, 0x2, 0x2de, 
    0x2df, 0x3, 0x2, 0x2, 0x2, 0x2df, 0x2eb, 0x5, 0x2a, 0x16, 0x2, 0x2e0, 
    0x2e1, 0x7, 0x31, 0x2, 0x2, 0x2e1, 0x2e4, 0x9, 0xa, 0x2, 0x2, 0x2e2, 
    0x2e3, 0x7, 0x96, 0x2, 0x2, 0x2e3, 0x2e5, 0x7, 0x95, 0x2, 0x2, 0x2e4, 
    0x2e2, 0x3, 0x2, 0x2, 0x2, 0x2e4, 0x2e5, 0x3, 0x2, 0x2, 0x2, 0x2e5, 
    0x2e6, 0x3, 0x2, 0x2, 0x2, 0x2e6, 0x2e7, 0x5, 0x94, 0x4b, 0x2, 0x2e7, 
    0x2e8, 0x7, 0x54, 0x2, 0x2, 0x2e8, 0x2e9, 0x5, 0x2a, 0x16, 0x2, 0x2e9, 
    0x2eb, 0x3, 0x2, 0x2, 0x2, 0x2ea, 0x2d9, 0x3, 0x2, 0x2, 0x2, 0x2ea, 
    0x2e0, 0x3, 0x2, 0x2, 0x2, 0x2eb, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x2ec, 0x2ee, 
    0x7, 0x6d, 0x2, 0x2, 0x2ed, 0x2ef, 0x7, 0x69, 0x2, 0x2, 0x2ee, 0x2ed, 
    0x3, 0x2, 0x2, 0x2, 0x2ee, 0x2ef, 0x3, 0x2, 0x2, 0x2, 0x2ef, 0x2f0, 
    0x3, 0x2, 0x2, 0x2, 0x2f0, 0x2f1, 0x5, 0x2a, 0x16, 0x2, 0x2f1, 0x41, 
    0x3, 0x2, 0x2, 0x2, 0x2f2, 0x2f6, 0x7, 0x9, 0x2, 0x2, 0x2f3, 0x2f7, 
    0x5, 0x46, 0x24, 0x2, 0x2f4, 0x2f7, 0x5, 0x4a, 0x26, 0x2, 0x2f5, 0x2f7, 
    0x5, 0x4c, 0x27, 0x2, 0x2f6, 0x2f3, 0x3, 0x2, 0x2, 0x2, 0x2f6, 0x2f4, 
    0x3, 0x2, 0x2, 0x2, 0x2f6, 0x2f5, 0x3, 0x2, 0x2, 0x2, 0x2f7, 0x300, 
    0x3, 0x2, 0x2, 0x2, 0x2f8, 0x2fc, 0x7, 0x5, 0x2, 0x2, 0x2f9, 0x2fd, 
    0x5, 0x46, 0x24, 0x2, 0x2fa, 0x2fd, 0x5, 0x4a, 0x26, 0x2, 0x2fb, 0x2fd, 
    0x5, 0x4c, 0x27, 0x2, 0x2fc, 0x2f9, 0x3, 0x2, 0x2, 0x2, 0x2fc, 0x2fa, 
    0x3, 0x2, 0x2, 0x2, 0x2fc, 0x2fb, 0x3, 0x2, 0x2, 0x2, 0x2fd, 0x2ff, 
    0x3, 0x2, 0x2, 0x2, 0x2fe, 0x2f8, 0x3, 0x2, 0x2, 0x2, 0x2ff, 0x302, 
    0x3, 0x2, 0x2, 0x2, 0x300, 0x2fe, 0x3, 0x2, 0x2, 0x2, 0x300, 0x301, 
    0x3, 0x2, 0x2, 0x2, 0x301, 0x303, 0x3, 0x2, 0x2, 0x2, 0x302, 0x300, 
    0x3, 0x2, 0x2, 0x2, 0x303, 0x305, 0x7, 0xa, 0x2, 0x2, 0x304, 0x306, 
    0x7, 0x5e, 0x2, 0x2, 0x305, 0x304, 0x3, 0x2, 0x2, 0x2, 0x305, 0x306, 
    0x3, 0x2, 0x2, 0x2, 0x306, 0x308, 0x3, 0x2, 0x2, 0x2, 0x307, 0x309, 
    0x5, 0x44, 0x23, 0x2, 0x308, 0x307, 0x3, 0x2, 0x2, 0x2, 0x308, 0x309, 
    0x3, 0x2, 0x2, 0x2, 0x309, 0x30c, 0x3, 0x2, 0x2, 0x2, 0x30a, 0x30b, 
    0x7, 0x77, 0x2, 0x2, 0x30b, 0x30d, 0x5, 0x6c, 0x37, 0x2, 0x30c, 0x30a, 
    0x3, 0x2, 0x2, 0x2, 0x30c, 0x30d, 0x3, 0x2, 0x2, 0x2, 0x30d, 0x43, 0x3, 
    0x2, 0x2, 0x2, 0x30e, 0x310, 0x7, 0x5a, 0x2, 0x2, 0x30f, 0x311, 0x7, 
    0x21, 0x2, 0x2, 0x310, 0x30f, 0x3, 0x2, 0x2, 0x2, 0x310, 0x311, 0x3, 
    0x2, 0x2, 0x2, 0x311, 0x313, 0x3, 0x2, 0x2, 0x2, 0x312, 0x314, 0x9, 
    0xb, 0x2, 0x2, 0x313, 0x312, 0x3, 0x2, 0x2, 0x2, 0x313, 0x314, 0x3, 
    0x2, 0x2, 0x2, 0x314, 0x316, 0x3, 0x2, 0x2, 0x2, 0x315, 0x317, 0x7, 
    0x54, 0x2, 0x2, 0x316, 0x315, 0x3, 0x2, 0x2, 0x2, 0x316, 0x317, 0x3, 
    0x2, 0x2, 0x2, 0x317, 0x318, 0x3, 0x2, 0x2, 0x2, 0x318, 0x319, 0x7, 
    0x9, 0x2, 0x2, 0x319, 0x31e, 0x5, 0x78, 0x3d, 0x2, 0x31a, 0x31b, 0x7, 
    0x5, 0x2, 0x2, 0x31b, 0x31d, 0x5, 0x78, 0x3d, 0x2, 0x31c, 0x31a, 0x3, 
    0x2, 0x2, 0x2, 0x31d, 0x320, 0x3, 0x2, 0x2, 0x2, 0x31e, 0x31c, 0x3, 
    0x2, 0x2, 0x2, 0x31e, 0x31f, 0x3, 0x2, 0x2, 0x2, 0x31f, 0x321, 0x3, 
    0x2, 0x2, 0x2, 0x320, 0x31e, 0x3, 0x2, 0x2, 0x2, 0x321, 0x322, 0x7, 
    0xa, 0x2, 0x2, 0x322, 0x45, 0x3, 0x2, 0x2, 0x2, 0x323, 0x324, 0x5, 0x8e, 
    0x48, 0x2, 0x324, 0x336, 0x5, 0x50, 0x29, 0x2, 0x325, 0x327, 0x7, 0x50, 
    0x2, 0x2, 0x326, 0x325, 0x3, 0x2, 0x2, 0x2, 0x326, 0x327, 0x3, 0x2, 
    0x2, 0x2, 0x327, 0x328, 0x3, 0x2, 0x2, 0x2, 0x328, 0x335, 0x7, 0x52, 
    0x2, 0x2, 0x329, 0x32a, 0x7, 0x5d, 0x2, 0x2, 0x32a, 0x335, 0x7, 0x45, 
    0x2, 0x2, 0x32b, 0x32d, 0x7, 0x71, 0x2, 0x2, 0x32c, 0x32e, 0x7, 0x45, 
    0x2, 0x2, 0x32d, 0x32c, 0x3, 0x2, 0x2, 0x2, 0x32d, 0x32e, 0x3, 0x2, 
    0x2, 0x2, 0x32e, 0x335, 0x3, 0x2, 0x2, 0x2, 0x32f, 0x330, 0x7, 0x71, 
    0x2, 0x2, 0x330, 0x332, 0x7, 0x4c, 0x2, 0x2, 0x331, 0x333, 0x7, 0x45, 
    0x2, 0x2, 0x332, 0x331, 0x3, 0x2, 0x2, 0x2, 0x332, 0x333, 0x3, 0x2, 
    0x2, 0x2, 0x333, 0x335, 0x3, 0x2, 0x2, 0x2, 0x334, 0x326, 0x3, 0x2, 
    0x2, 0x2, 0x334, 0x329, 0x3, 0x2, 0x2, 0x2, 0x334, 0x32b, 0x3, 0x2, 
    0x2, 0x2, 0x334, 0x32f, 0x3, 0x2, 0x2, 0x2, 0x335, 0x338, 0x3, 0x2, 
    0x2, 0x2, 0x336, 0x334, 0x3, 0x2, 0x2, 0x2, 0x336, 0x337, 0x3, 0x2, 
    0x2, 0x2, 0x337, 0x33b, 0x3, 0x2, 0x2, 0x2, 0x338, 0x336, 0x3, 0x2, 
    0x2, 0x2, 0x339, 0x33a, 0x7, 0x77, 0x2, 0x2, 0x33a, 0x33c, 0x5, 0x6c, 
    0x37, 0x2, 0x33b, 0x339, 0x3, 0x2, 0x2, 0x2, 0x33b, 0x33c, 0x3, 0x2, 
    0x2, 0x2, 0x33c, 0x47, 0x3, 0x2, 0x2, 0x2, 0x33d, 0x33e, 0x7, 0x9, 0x2, 
    0x2, 0x33e, 0x343, 0x5, 0x8e, 0x48, 0x2, 0x33f, 0x340, 0x7, 0x5, 0x2, 
    0x2, 0x340, 0x342, 0x5, 0x8e, 0x48, 0x2, 0x341, 0x33f, 0x3, 0x2, 0x2, 
    0x2, 0x342, 0x345, 0x3, 0x2, 0x2, 0x2, 0x343, 0x341, 0x3, 0x2, 0x2, 
    0x2, 0x343, 0x344, 0x3, 0x2, 0x2, 0x2, 0x344, 0x346, 0x3, 0x2, 0x2, 
    0x2, 0x345, 0x343, 0x3, 0x2, 0x2, 0x2, 0x346, 0x347, 0x7, 0xa, 0x2, 
    0x2, 0x347, 0x49, 0x3, 0x2, 0x2, 0x2, 0x348, 0x349, 0x7, 0x5d, 0x2, 
    0x2, 0x349, 0x356, 0x7, 0x45, 0x2, 0x2, 0x34a, 0x357, 0x5, 0x78, 0x3d, 
    0x2, 0x34b, 0x34c, 0x7, 0x9, 0x2, 0x2, 0x34c, 0x351, 0x5, 0x78, 0x3d, 
    0x2, 0x34d, 0x34e, 0x7, 0x5, 0x2, 0x2, 0x34e, 0x350, 0x5, 0x78, 0x3d, 
    0x2, 0x34f, 0x34d, 0x3, 0x2, 0x2, 0x2, 0x350, 0x353, 0x3, 0x2, 0x2, 
    0x2, 0x351, 0x34f, 0x3, 0x2, 0x2, 0x2, 0x351, 0x352, 0x3, 0x2, 0x2, 
    0x2, 0x352, 0x354, 0x3, 0x2, 0x2, 0x2, 0x353, 0x351, 0x3, 0x2, 0x2, 
    0x2, 0x354, 0x355, 0x7, 0xa, 0x2, 0x2, 0x355, 0x357, 0x3, 0x2, 0x2, 
    0x2, 0x356, 0x34a, 0x3, 0x2, 0x2, 0x2, 0x356, 0x34b, 0x3, 0x2, 0x2, 
    0x2, 0x357, 0x36c, 0x3, 0x2, 0x2, 0x2, 0x358, 0x35a, 0x7, 0x71, 0x2, 
    0x2, 0x359, 0x358, 0x3, 0x2, 0x2, 0x2, 0x359, 0x35a, 0x3, 0x2, 0x2, 
    0x2, 0x35a, 0x35b, 0x3, 0x2, 0x2, 0x2, 0x35b, 0x35c, 0x7, 0x4c, 0x2, 
    0x2, 0x35c, 0x369, 0x7, 0x45, 0x2, 0x2, 0x35d, 0x36a, 0x5, 0x78, 0x3d, 
    0x2, 0x35e, 0x35f, 0x7, 0x9, 0x2, 0x2, 0x35f, 0x364, 0x5, 0x78, 0x3d, 
    0x2, 0x360, 0x361, 0x7, 0x5, 0x2, 0x2, 0x361, 0x363, 0x5, 0x78, 0x3d, 
    0x2, 0x362, 0x360, 0x3, 0x2, 0x2, 0x2, 0x363, 0x366, 0x3, 0x2, 0x2, 
    0x2, 0x364, 0x362, 0x3, 0x2, 0x2, 0x2, 0x364, 0x365, 0x3, 0x2, 0x2, 
    0x2, 0x365, 0x367, 0x3, 0x2, 0x2, 0x2, 0x366, 0x364, 0x3, 0x2, 0x2, 
    0x2, 0x367, 0x368, 0x7, 0xa, 0x2, 0x2, 0x368, 0x36a, 0x3, 0x2, 0x2, 
    0x2, 0x369, 0x35d, 0x3, 0x2, 0x2, 0x2, 0x369, 0x35e, 0x3, 0x2, 0x2, 
    0x2, 0x36a, 0x36c, 0x3, 0x2, 0x2, 0x2, 0x36b, 0x348, 0x3, 0x2, 0x2, 
    0x2, 0x36b, 0x359, 0x3, 0x2, 0x2, 0x2, 0x36c, 0x4b, 0x3, 0x2, 0x2, 0x2, 
    0x36d, 0x36f, 0x7, 0x71, 0x2, 0x2, 0x36e, 0x36d, 0x3, 0x2, 0x2, 0x2, 
    0x36e, 0x36f, 0x3, 0x2, 0x2, 0x2, 0x36f, 0x370, 0x3, 0x2, 0x2, 0x2, 
    0x370, 0x372, 0x9, 0xa, 0x2, 0x2, 0x371, 0x373, 0x5, 0x94, 0x4b, 0x2, 
    0x372, 0x371, 0x3, 0x2, 0x2, 0x2, 0x372, 0x373, 0x3, 0x2, 0x2, 0x2, 
    0x373, 0x375, 0x3, 0x2, 0x2, 0x2, 0x374, 0x376, 0x7, 0x54, 0x2, 0x2, 
    0x375, 0x374, 0x3, 0x2, 0x2, 0x2, 0x375, 0x376, 0x3, 0x2, 0x2, 0x2, 
    0x376, 0x383, 0x3, 0x2, 0x2, 0x2, 0x377, 0x384, 0x5, 0x78, 0x3d, 0x2, 
    0x378, 0x379, 0x7, 0x9, 0x2, 0x2, 0x379, 0x37e, 0x5, 0x78, 0x3d, 0x2, 
    0x37a, 0x37b, 0x7, 0x5, 0x2, 0x2, 0x37b, 0x37d, 0x5, 0x78, 0x3d, 0x2, 
    0x37c, 0x37a, 0x3, 0x2, 0x2, 0x2, 0x37d, 0x380, 0x3, 0x2, 0x2, 0x2, 
    0x37e, 0x37c, 0x3, 0x2, 0x2, 0x2, 0x37e, 0x37f, 0x3, 0x2, 0x2, 0x2, 
    0x37f, 0x381, 0x3, 0x2, 0x2, 0x2, 0x380, 0x37e, 0x3, 0x2, 0x2, 0x2, 
    0x381, 0x382, 0x7, 0xa, 0x2, 0x2, 0x382, 0x384, 0x3, 0x2, 0x2, 0x2, 
    0x383, 0x377, 0x3, 0x2, 0x2, 0x2, 0x383, 0x378, 0x3, 0x2, 0x2, 0x2, 
    0x384, 0x387, 0x3, 0x2, 0x2, 0x2, 0x385, 0x386, 0x7, 0x73, 0x2, 0x2, 
    0x386, 0x388, 0x5, 0x4e, 0x28, 0x2, 0x387, 0x385, 0x3, 0x2, 0x2, 0x2, 
    0x387, 0x388, 0x3, 0x2, 0x2, 0x2, 0x388, 0x38b, 0x3, 0x2, 0x2, 0x2, 
    0x389, 0x38a, 0x7, 0x77, 0x2, 0x2, 0x38a, 0x38c, 0x5, 0x6c, 0x37, 0x2, 
    0x38b, 0x389, 0x3, 0x2, 0x2, 0x2, 0x38b, 0x38c, 0x3, 0x2, 0x2, 0x2, 
    0x38c, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x38d, 0x38e, 0x9, 0xc, 0x2, 0x2, 0x38e, 
    0x4f, 0x3, 0x2, 0x2, 0x2, 0x38f, 0x392, 0x5, 0x52, 0x2a, 0x2, 0x390, 
    0x391, 0x7, 0x73, 0x2, 0x2, 0x391, 0x393, 0x5, 0x80, 0x41, 0x2, 0x392, 
    0x390, 0x3, 0x2, 0x2, 0x2, 0x392, 0x393, 0x3, 0x2, 0x2, 0x2, 0x393, 
    0x396, 0x3, 0x2, 0x2, 0x2, 0x394, 0x396, 0x5, 0x80, 0x41, 0x2, 0x395, 
    0x38f, 0x3, 0x2, 0x2, 0x2, 0x395, 0x394, 0x3, 0x2, 0x2, 0x2, 0x396, 
    0x51, 0x3, 0x2, 0x2, 0x2, 0x397, 0x3f3, 0x7, 0x79, 0x2, 0x2, 0x398, 
    0x3a0, 0x7, 0x7a, 0x2, 0x2, 0x399, 0x39a, 0x7, 0x9, 0x2, 0x2, 0x39a, 
    0x39c, 0x5, 0xa2, 0x52, 0x2, 0x39b, 0x39d, 0x9, 0xd, 0x2, 0x2, 0x39c, 
    0x39b, 0x3, 0x2, 0x2, 0x2, 0x39c, 0x39d, 0x3, 0x2, 0x2, 0x2, 0x39d, 
    0x39e, 0x3, 0x2, 0x2, 0x2, 0x39e, 0x39f, 0x7, 0xa, 0x2, 0x2, 0x39f, 
    0x3a1, 0x3, 0x2, 0x2, 0x2, 0x3a0, 0x399, 0x3, 0x2, 0x2, 0x2, 0x3a0, 
    0x3a1, 0x3, 0x2, 0x2, 0x2, 0x3a1, 0x3f3, 0x3, 0x2, 0x2, 0x2, 0x3a2, 
    0x3b1, 0x7, 0x7b, 0x2, 0x2, 0x3a3, 0x3a4, 0x7, 0x9, 0x2, 0x2, 0x3a4, 
    0x3a6, 0x5, 0xa2, 0x52, 0x2, 0x3a5, 0x3a7, 0x7, 0x8d, 0x2, 0x2, 0x3a6, 
    0x3a5, 0x3, 0x2, 0x2, 0x2, 0x3a6, 0x3a7, 0x3, 0x2, 0x2, 0x2, 0x3a7, 
    0x3ad, 0x3, 0x2, 0x2, 0x2, 0x3a8, 0x3a9, 0x7, 0x5, 0x2, 0x2, 0x3a9, 
    0x3ab, 0x5, 0xa4, 0x53, 0x2, 0x3aa, 0x3ac, 0x7, 0x8d, 0x2, 0x2, 0x3ab, 
    0x3aa, 0x3, 0x2, 0x2, 0x2, 0x3ab, 0x3ac, 0x3, 0x2, 0x2, 0x2, 0x3ac, 
    0x3ae, 0x3, 0x2, 0x2, 0x2, 0x3ad, 0x3a8, 0x3, 0x2, 0x2, 0x2, 0x3ad, 
    0x3ae, 0x3, 0x2, 0x2, 0x2, 0x3ae, 0x3af, 0x3, 0x2, 0x2, 0x2, 0x3af, 
    0x3b0, 0x7, 0xa, 0x2, 0x2, 0x3b0, 0x3b2, 0x3, 0x2, 0x2, 0x2, 0x3b1, 
    0x3a3, 0x3, 0x2, 0x2, 0x2, 0x3b1, 0x3b2, 0x3, 0x2, 0x2, 0x2, 0x3b2, 
    0x3f3, 0x3, 0x2, 0x2, 0x2, 0x3b3, 0x3bb, 0x7, 0x7c, 0x2, 0x2, 0x3b4, 
    0x3b5, 0x7, 0x9, 0x2, 0x2, 0x3b5, 0x3b7, 0x5, 0xa2, 0x52, 0x2, 0x3b6, 
    0x3b8, 0x9, 0xd, 0x2, 0x2, 0x3b7, 0x3b6, 0x3, 0x2, 0x2, 0x2, 0x3b7, 
    0x3b8, 0x3, 0x2, 0x2, 0x2, 0x3b8, 0x3b9, 0x3, 0x2, 0x2, 0x2, 0x3b9, 
    0x3ba, 0x7, 0xa, 0x2, 0x2, 0x3ba, 0x3bc, 0x3, 0x2, 0x2, 0x2, 0x3bb, 
    0x3b4, 0x3, 0x2, 0x2, 0x2, 0x3bb, 0x3bc, 0x3, 0x2, 0x2, 0x2, 0x3bc, 
    0x3f3, 0x3, 0x2, 0x2, 0x2, 0x3bd, 0x3f3, 0x7, 0x2c, 0x2, 0x2, 0x3be, 
    0x3f3, 0x7, 0x7d, 0x2, 0x2, 0x3bf, 0x3f3, 0x7, 0x7e, 0x2, 0x2, 0x3c0, 
    0x3c1, 0x9, 0xe, 0x2, 0x2, 0x3c1, 0x3c2, 0x7, 0x9, 0x2, 0x2, 0x3c2, 
    0x3c4, 0x5, 0xa6, 0x54, 0x2, 0x3c3, 0x3c5, 0x9, 0xf, 0x2, 0x2, 0x3c4, 
    0x3c3, 0x3, 0x2, 0x2, 0x2, 0x3c4, 0x3c5, 0x3, 0x2, 0x2, 0x2, 0x3c5, 
    0x3c6, 0x3, 0x2, 0x2, 0x2, 0x3c6, 0x3c7, 0x7, 0xa, 0x2, 0x2, 0x3c7, 
    0x3da, 0x3, 0x2, 0x2, 0x2, 0x3c8, 0x3ca, 0x9, 0xe, 0x2, 0x2, 0x3c9, 
    0x3cb, 0x7, 0x87, 0x2, 0x2, 0x3ca, 0x3c9, 0x3, 0x2, 0x2, 0x2, 0x3ca, 
    0x3cb, 0x3, 0x2, 0x2, 0x2, 0x3cb, 0x3ce, 0x3, 0x2, 0x2, 0x2, 0x3cc, 
    0x3ce, 0x7, 0x82, 0x2, 0x2, 0x3cd, 0x3c8, 0x3, 0x2, 0x2, 0x2, 0x3cd, 
    0x3cc, 0x3, 0x2, 0x2, 0x2, 0x3ce, 0x3d6, 0x3, 0x2, 0x2, 0x2, 0x3cf, 
    0x3d0, 0x7, 0x9, 0x2, 0x2, 0x3d0, 0x3d2, 0x5, 0xa6, 0x54, 0x2, 0x3d1, 
    0x3d3, 0x9, 0xf, 0x2, 0x2, 0x3d2, 0x3d1, 0x3, 0x2, 0x2, 0x2, 0x3d2, 
    0x3d3, 0x3, 0x2, 0x2, 0x2, 0x3d3, 0x3d4, 0x3, 0x2, 0x2, 0x2, 0x3d4, 
    0x3d5, 0x7, 0xa, 0x2, 0x2, 0x3d5, 0x3d7, 0x3, 0x2, 0x2, 0x2, 0x3d6, 
    0x3cf, 0x3, 0x2, 0x2, 0x2, 0x3d6, 0x3d7, 0x3, 0x2, 0x2, 0x2, 0x3d7, 
    0x3da, 0x3, 0x2, 0x2, 0x2, 0x3d8, 0x3da, 0x7, 0x83, 0x2, 0x2, 0x3d9, 
    0x3c0, 0x3, 0x2, 0x2, 0x2, 0x3d9, 0x3cd, 0x3, 0x2, 0x2, 0x2, 0x3d9, 
    0x3d8, 0x3, 0x2, 0x2, 0x2, 0x3da, 0x3dd, 0x3, 0x2, 0x2, 0x2, 0x3db, 
    0x3dc, 0x7, 0x23, 0x2, 0x2, 0x3dc, 0x3de, 0x5, 0x92, 0x4a, 0x2, 0x3dd, 
    0x3db, 0x3, 0x2, 0x2, 0x2, 0x3dd, 0x3de, 0x3, 0x2, 0x2, 0x2, 0x3de, 
    0x3f3, 0x3, 0x2, 0x2, 0x2, 0x3df, 0x3e0, 0x7, 0x84, 0x2, 0x2, 0x3e0, 
    0x3e1, 0x7, 0x9, 0x2, 0x2, 0x3e1, 0x3e3, 0x5, 0xa6, 0x54, 0x2, 0x3e2, 
    0x3e4, 0x9, 0x10, 0x2, 0x2, 0x3e3, 0x3e2, 0x3, 0x2, 0x2, 0x2, 0x3e3, 
    0x3e4, 0x3, 0x2, 0x2, 0x2, 0x3e4, 0x3e5, 0x3, 0x2, 0x2, 0x2, 0x3e5, 
    0x3e6, 0x7, 0xa, 0x2, 0x2, 0x3e6, 0x3f3, 0x3, 0x2, 0x2, 0x2, 0x3e7, 
    0x3ef, 0x7, 0x85, 0x2, 0x2, 0x3e8, 0x3e9, 0x7, 0x9, 0x2, 0x2, 0x3e9, 
    0x3eb, 0x5, 0xa6, 0x54, 0x2, 0x3ea, 0x3ec, 0x9, 0x10, 0x2, 0x2, 0x3eb, 
    0x3ea, 0x3, 0x2, 0x2, 0x2, 0x3eb, 0x3ec, 0x3, 0x2, 0x2, 0x2, 0x3ec, 
    0x3ed, 0x3, 0x2, 0x2, 0x2, 0x3ed, 0x3ee, 0x7, 0xa, 0x2, 0x2, 0x3ee, 
    0x3f0, 0x3, 0x2, 0x2, 0x2, 0x3ef, 0x3e8, 0x3, 0x2, 0x2, 0x2, 0x3ef, 
    0x3f0, 0x3, 0x2, 0x2, 0x2, 0x3f0, 0x3f3, 0x3, 0x2, 0x2, 0x2, 0x3f1, 
    0x3f3, 0x7, 0x86, 0x2, 0x2, 0x3f2, 0x397, 0x3, 0x2, 0x2, 0x2, 0x3f2, 
    0x398, 0x3, 0x2, 0x2, 0x2, 0x3f2, 0x3a2, 0x3, 0x2, 0x2, 0x2, 0x3f2, 
    0x3b3, 0x3, 0x2, 0x2, 0x2, 0x3f2, 0x3bd, 0x3, 0x2, 0x2, 0x2, 0x3f2, 
    0x3be, 0x3, 0x2, 0x2, 0x2, 0x3f2, 0x3bf, 0x3, 0x2, 0x2, 0x2, 0x3f2, 
    0x3d9, 0x3, 0x2, 0x2, 0x2, 0x3f2, 0x3df, 0x3, 0x2, 0x2, 0x2, 0x3f2, 
    0x3e7, 0x3, 0x2, 0x2, 0x2, 0x3f2, 0x3f1, 0x3, 0x2, 0x2, 0x2, 0x3f3, 
    0x53, 0x3, 0x2, 0x2, 0x2, 0x3f4, 0x3f5, 0x5, 0x56, 0x2c, 0x2, 0x3f5, 
    0x55, 0x3, 0x2, 0x2, 0x2, 0x3f6, 0x3fa, 0x7, 0x9, 0x2, 0x2, 0x3f7, 0x3f9, 
    0x5, 0x18, 0xd, 0x2, 0x3f8, 0x3f7, 0x3, 0x2, 0x2, 0x2, 0x3f9, 0x3fc, 
    0x3, 0x2, 0x2, 0x2, 0x3fa, 0x3f8, 0x3, 0x2, 0x2, 0x2, 0x3fa, 0x3fb, 
    0x3, 0x2, 0x2, 0x2, 0x3fb, 0x3fd, 0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3fa, 
    0x3, 0x2, 0x2, 0x2, 0x3fd, 0x3fe, 0x5, 0x56, 0x2c, 0x2, 0x3fe, 0x3ff, 
    0x7, 0xa, 0x2, 0x2, 0x3ff, 0x40c, 0x3, 0x2, 0x2, 0x2, 0x400, 0x402, 
    0x5, 0x18, 0xd, 0x2, 0x401, 0x400, 0x3, 0x2, 0x2, 0x2, 0x402, 0x405, 
    0x3, 0x2, 0x2, 0x2, 0x403, 0x401, 0x3, 0x2, 0x2, 0x2, 0x403, 0x404, 
    0x3, 0x2, 0x2, 0x2, 0x404, 0x409, 0x3, 0x2, 0x2, 0x2, 0x405, 0x403, 
    0x3, 0x2, 0x2, 0x2, 0x406, 0x40a, 0x5, 0x1c, 0xf, 0x2, 0x407, 0x40a, 
    0x5, 0x30, 0x19, 0x2, 0x408, 0x40a, 0x5, 0x20, 0x11, 0x2, 0x409, 0x406, 
    0x3, 0x2, 0x2, 0x2, 0x409, 0x407, 0x3, 0x2, 0x2, 0x2, 0x409, 0x408, 
    0x3, 0x2, 0x2, 0x2, 0x40a, 0x40c, 0x3, 0x2, 0x2, 0x2, 0x40b, 0x3f6, 
    0x3, 0x2, 0x2, 0x2, 0x40b, 0x403, 0x3, 0x2, 0x2, 0x2, 0x40c, 0x57, 0x3, 
    0x2, 0x2, 0x2, 0x40d, 0x412, 0x5, 0x5a, 0x2e, 0x2, 0x40e, 0x40f, 0x7, 
    0x5, 0x2, 0x2, 0x40f, 0x411, 0x5, 0x5a, 0x2e, 0x2, 0x410, 0x40e, 0x3, 
    0x2, 0x2, 0x2, 0x411, 0x414, 0x3, 0x2, 0x2, 0x2, 0x412, 0x410, 0x3, 
    0x2, 0x2, 0x2, 0x412, 0x413, 0x3, 0x2, 0x2, 0x2, 0x413, 0x59, 0x3, 0x2, 
    0x2, 0x2, 0x414, 0x412, 0x3, 0x2, 0x2, 0x2, 0x415, 0x41a, 0x5, 0x78, 
    0x3d, 0x2, 0x416, 0x418, 0x7, 0x1c, 0x2, 0x2, 0x417, 0x416, 0x3, 0x2, 
    0x2, 0x2, 0x417, 0x418, 0x3, 0x2, 0x2, 0x2, 0x418, 0x419, 0x3, 0x2, 
    0x2, 0x2, 0x419, 0x41b, 0x5, 0x9e, 0x50, 0x2, 0x41a, 0x417, 0x3, 0x2, 
    0x2, 0x2, 0x41a, 0x41b, 0x3, 0x2, 0x2, 0x2, 0x41b, 0x41e, 0x3, 0x2, 
    0x2, 0x2, 0x41c, 0x41e, 0x5, 0x7e, 0x40, 0x2, 0x41d, 0x415, 0x3, 0x2, 
    0x2, 0x2, 0x41d, 0x41c, 0x3, 0x2, 0x2, 0x2, 0x41e, 0x5b, 0x3, 0x2, 0x2, 
    0x2, 0x41f, 0x424, 0x5, 0x5e, 0x30, 0x2, 0x420, 0x421, 0x7, 0x5, 0x2, 
    0x2, 0x421, 0x423, 0x5, 0x5e, 0x30, 0x2, 0x422, 0x420, 0x3, 0x2, 0x2, 
    0x2, 0x423, 0x426, 0x3, 0x2, 0x2, 0x2, 0x424, 0x422, 0x3, 0x2, 0x2, 
    0x2, 0x424, 0x425, 0x3, 0x2, 0x2, 0x2, 0x425, 0x5d, 0x3, 0x2, 0x2, 0x2, 
    0x426, 0x424, 0x3, 0x2, 0x2, 0x2, 0x427, 0x428, 0x5, 0x78, 0x3d, 0x2, 
    0x428, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x429, 0x42e, 0x5, 0x62, 0x32, 0x2, 
    0x42a, 0x42b, 0x7, 0x5, 0x2, 0x2, 0x42b, 0x42d, 0x5, 0x62, 0x32, 0x2, 
    0x42c, 0x42a, 0x3, 0x2, 0x2, 0x2, 0x42d, 0x430, 0x3, 0x2, 0x2, 0x2, 
    0x42e, 0x42c, 0x3, 0x2, 0x2, 0x2, 0x42e, 0x42f, 0x3, 0x2, 0x2, 0x2, 
    0x42f, 0x61, 0x3, 0x2, 0x2, 0x2, 0x430, 0x42e, 0x3, 0x2, 0x2, 0x2, 0x431, 
    0x433, 0x5, 0x78, 0x3d, 0x2, 0x432, 0x434, 0x9, 0x11, 0x2, 0x2, 0x433, 
    0x432, 0x3, 0x2, 0x2, 0x2, 0x433, 0x434, 0x3, 0x2, 0x2, 0x2, 0x434, 
    0x63, 0x3, 0x2, 0x2, 0x2, 0x435, 0x43a, 0x5, 0x66, 0x34, 0x2, 0x436, 
    0x437, 0x7, 0x5, 0x2, 0x2, 0x437, 0x439, 0x5, 0x66, 0x34, 0x2, 0x438, 
    0x436, 0x3, 0x2, 0x2, 0x2, 0x439, 0x43c, 0x3, 0x2, 0x2, 0x2, 0x43a, 
    0x438, 0x3, 0x2, 0x2, 0x2, 0x43a, 0x43b, 0x3, 0x2, 0x2, 0x2, 0x43b, 
    0x65, 0x3, 0x2, 0x2, 0x2, 0x43c, 0x43a, 0x3, 0x2, 0x2, 0x2, 0x43d, 0x43e, 
    0x5, 0x9e, 0x50, 0x2, 0x43e, 0x67, 0x3, 0x2, 0x2, 0x2, 0x43f, 0x444, 
    0x5, 0x6a, 0x36, 0x2, 0x440, 0x441, 0x7, 0x5, 0x2, 0x2, 0x441, 0x443, 
    0x5, 0x6a, 0x36, 0x2, 0x442, 0x440, 0x3, 0x2, 0x2, 0x2, 0x443, 0x446, 
    0x3, 0x2, 0x2, 0x2, 0x444, 0x442, 0x3, 0x2, 0x2, 0x2, 0x444, 0x445, 
    0x3, 0x2, 0x2, 0x2, 0x445, 0x69, 0x3, 0x2, 0x2, 0x2, 0x446, 0x444, 0x3, 
    0x2, 0x2, 0x2, 0x447, 0x448, 0x5, 0x9e, 0x50, 0x2, 0x448, 0x6b, 0x3, 
    0x2, 0x2, 0x2, 0x449, 0x44e, 0x5, 0x6e, 0x38, 0x2, 0x44a, 0x44b, 0x7, 
    0x5, 0x2, 0x2, 0x44b, 0x44d, 0x5, 0x6e, 0x38, 0x2, 0x44c, 0x44a, 0x3, 
    0x2, 0x2, 0x2, 0x44d, 0x450, 0x3, 0x2, 0x2, 0x2, 0x44e, 0x44c, 0x3, 
    0x2, 0x2, 0x2, 0x44e, 0x44f, 0x3, 0x2, 0x2, 0x2, 0x44f, 0x6d, 0x3, 0x2, 
    0x2, 0x2, 0x450, 0x44e, 0x3, 0x2, 0x2, 0x2, 0x451, 0x454, 0x5, 0x96, 
    0x4c, 0x2, 0x452, 0x453, 0x7, 0x8, 0x2, 0x2, 0x453, 0x455, 0x5, 0x9a, 
    0x4e, 0x2, 0x454, 0x452, 0x3, 0x2, 0x2, 0x2, 0x454, 0x455, 0x3, 0x2, 
    0x2, 0x2, 0x455, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x456, 0x45b, 0x5, 0x78, 
    0x3d, 0x2, 0x457, 0x458, 0x7, 0x5, 0x2, 0x2, 0x458, 0x45a, 0x5, 0x78, 
    0x3d, 0x2, 0x459, 0x457, 0x3, 0x2, 0x2, 0x2, 0x45a, 0x45d, 0x3, 0x2, 
    0x2, 0x2, 0x45b, 0x459, 0x3, 0x2, 0x2, 0x2, 0x45b, 0x45c, 0x3, 0x2, 
    0x2, 0x2, 0x45c, 0x71, 0x3, 0x2, 0x2, 0x2, 0x45d, 0x45b, 0x3, 0x2, 0x2, 
    0x2, 0x45e, 0x45f, 0x5, 0x78, 0x3d, 0x2, 0x45f, 0x73, 0x3, 0x2, 0x2, 
    0x2, 0x460, 0x461, 0x5, 0x78, 0x3d, 0x2, 0x461, 0x75, 0x3, 0x2, 0x2, 
    0x2, 0x462, 0x463, 0x5, 0x78, 0x3d, 0x2, 0x463, 0x77, 0x3, 0x2, 0x2, 
    0x2, 0x464, 0x465, 0x8, 0x3d, 0x1, 0x2, 0x465, 0x56c, 0x5, 0xa8, 0x55, 
    0x2, 0x466, 0x56c, 0x5, 0x7a, 0x3e, 0x2, 0x467, 0x56c, 0x5, 0x7c, 0x3f, 
    0x2, 0x468, 0x469, 0x7, 0x9, 0x2, 0x2, 0x469, 0x46a, 0x5, 0x50, 0x29, 
    0x2, 0x46a, 0x46b, 0x7, 0xa, 0x2, 0x2, 0x46b, 0x46c, 0x5, 0x78, 0x3d, 
    0x29, 0x46c, 0x56c, 0x3, 0x2, 0x2, 0x2, 0x46d, 0x46e, 0x9, 0x12, 0x2, 
    0x2, 0x46e, 0x56c, 0x5, 0x78, 0x3d, 0x28, 0x46f, 0x470, 0x7, 0x8f, 0x2, 
    0x2, 0x470, 0x471, 0x7, 0x9, 0x2, 0x2, 0x471, 0x472, 0x5, 0x78, 0x3d, 
    0x2, 0x472, 0x473, 0x9, 0x13, 0x2, 0x2, 0x473, 0x474, 0x5, 0x50, 0x29, 
    0x2, 0x474, 0x475, 0x7, 0xa, 0x2, 0x2, 0x475, 0x56c, 0x3, 0x2, 0x2, 
    0x2, 0x476, 0x477, 0x7, 0x9d, 0x2, 0x2, 0x477, 0x478, 0x7, 0x9, 0x2, 
    0x2, 0x478, 0x479, 0x5, 0x78, 0x3d, 0x2, 0x479, 0x47a, 0x7, 0xa, 0x2, 
    0x2, 0x47a, 0x56c, 0x3, 0x2, 0x2, 0x2, 0x47b, 0x48f, 0x7, 0x8e, 0x2, 
    0x2, 0x47c, 0x482, 0x5, 0x78, 0x3d, 0x2, 0x47d, 0x47e, 0x7, 0x9e, 0x2, 
    0x2, 0x47e, 0x47f, 0x5, 0x78, 0x3d, 0x2, 0x47f, 0x480, 0x7, 0x9f, 0x2, 
    0x2, 0x480, 0x481, 0x5, 0x78, 0x3d, 0x2, 0x481, 0x483, 0x3, 0x2, 0x2, 
    0x2, 0x482, 0x47d, 0x3, 0x2, 0x2, 0x2, 0x483, 0x484, 0x3, 0x2, 0x2, 
    0x2, 0x484, 0x482, 0x3, 0x2, 0x2, 0x2, 0x484, 0x485, 0x3, 0x2, 0x2, 
    0x2, 0x485, 0x490, 0x3, 0x2, 0x2, 0x2, 0x486, 0x487, 0x7, 0x9e, 0x2, 
    0x2, 0x487, 0x488, 0x5, 0x76, 0x3c, 0x2, 0x488, 0x489, 0x7, 0x9f, 0x2, 
    0x2, 0x489, 0x48a, 0x5, 0x78, 0x3d, 0x2, 0x48a, 0x48c, 0x3, 0x2, 0x2, 
    0x2, 0x48b, 0x486, 0x3, 0x2, 0x2, 0x2, 0x48c, 0x48d, 0x3, 0x2, 0x2, 
    0x2, 0x48d, 0x48b, 0x3, 0x2, 0x2, 0x2, 0x48d, 0x48e, 0x3, 0x2, 0x2, 
    0x2, 0x48e, 0x490, 0x3, 0x2, 0x2, 0x2, 0x48f, 0x47c, 0x3, 0x2, 0x2, 
    0x2, 0x48f, 0x48b, 0x3, 0x2, 0x2, 0x2, 0x490, 0x493, 0x3, 0x2, 0x2, 
    0x2, 0x491, 0x492, 0x7, 0xa0, 0x2, 0x2, 0x492, 0x494, 0x5, 0x78, 0x3d, 
    0x2, 0x493, 0x491, 0x3, 0x2, 0x2, 0x2, 0x493, 0x494, 0x3, 0x2, 0x2, 
    0x2, 0x494, 0x495, 0x3, 0x2, 0x2, 0x2, 0x495, 0x496, 0x7, 0xa1, 0x2, 
    0x2, 0x496, 0x56c, 0x3, 0x2, 0x2, 0x2, 0x497, 0x498, 0x7, 0x96, 0x2, 
    0x2, 0x498, 0x499, 0x7, 0x9, 0x2, 0x2, 0x499, 0x49a, 0x5, 0x76, 0x3c, 
    0x2, 0x49a, 0x49b, 0x7, 0x5, 0x2, 0x2, 0x49b, 0x49c, 0x5, 0x78, 0x3d, 
    0x2, 0x49c, 0x49d, 0x7, 0x5, 0x2, 0x2, 0x49d, 0x49e, 0x5, 0x78, 0x3d, 
    0x2, 0x49e, 0x49f, 0x7, 0xa, 0x2, 0x2, 0x49f, 0x56c, 0x3, 0x2, 0x2, 
    0x2, 0x4a0, 0x4a1, 0x7, 0x97, 0x2, 0x2, 0x4a1, 0x4a2, 0x7, 0x9, 0x2, 
    0x2, 0x4a2, 0x4a3, 0x5, 0x78, 0x3d, 0x2, 0x4a3, 0x4a4, 0x7, 0x5, 0x2, 
    0x2, 0x4a4, 0x4a5, 0x5, 0x78, 0x3d, 0x2, 0x4a5, 0x4a6, 0x7, 0xa, 0x2, 
    0x2, 0x4a6, 0x56c, 0x3, 0x2, 0x2, 0x2, 0x4a7, 0x4a8, 0x7, 0x98, 0x2, 
    0x2, 0x4a8, 0x4a9, 0x7, 0x9, 0x2, 0x2, 0x4a9, 0x4aa, 0x5, 0x78, 0x3d, 
    0x2, 0x4aa, 0x4ab, 0x7, 0x5, 0x2, 0x2, 0x4ab, 0x4ac, 0x5, 0x78, 0x3d, 
    0x2, 0x4ac, 0x4ad, 0x7, 0xa, 0x2, 0x2, 0x4ad, 0x56c, 0x3, 0x2, 0x2, 
    0x2, 0x4ae, 0x4af, 0x7, 0x90, 0x2, 0x2, 0x4af, 0x4b8, 0x7, 0x9, 0x2, 
    0x2, 0x4b0, 0x4b5, 0x5, 0x78, 0x3d, 0x2, 0x4b1, 0x4b2, 0x7, 0x5, 0x2, 
    0x2, 0x4b2, 0x4b4, 0x5, 0x78, 0x3d, 0x2, 0x4b3, 0x4b1, 0x3, 0x2, 0x2, 
    0x2, 0x4b4, 0x4b7, 0x3, 0x2, 0x2, 0x2, 0x4b5, 0x4b3, 0x3, 0x2, 0x2, 
    0x2, 0x4b5, 0x4b6, 0x3, 0x2, 0x2, 0x2, 0x4b6, 0x4b9, 0x3, 0x2, 0x2, 
    0x2, 0x4b7, 0x4b5, 0x3, 0x2, 0x2, 0x2, 0x4b8, 0x4b0, 0x3, 0x2, 0x2, 
    0x2, 0x4b8, 0x4b9, 0x3, 0x2, 0x2, 0x2, 0x4b9, 0x4ba, 0x3, 0x2, 0x2, 
    0x2, 0x4ba, 0x56c, 0x7, 0xa, 0x2, 0x2, 0x4bb, 0x4bc, 0x7, 0x93, 0x2, 
    0x2, 0x4bc, 0x4c5, 0x7, 0x9, 0x2, 0x2, 0x4bd, 0x4c2, 0x5, 0x78, 0x3d, 
    0x2, 0x4be, 0x4bf, 0x7, 0x5, 0x2, 0x2, 0x4bf, 0x4c1, 0x5, 0x78, 0x3d, 
    0x2, 0x4c0, 0x4be, 0x3, 0x2, 0x2, 0x2, 0x4c1, 0x4c4, 0x3, 0x2, 0x2, 
    0x2, 0x4c2, 0x4c0, 0x3, 0x2, 0x2, 0x2, 0x4c2, 0x4c3, 0x3, 0x2, 0x2, 
    0x2, 0x4c3, 0x4c6, 0x3, 0x2, 0x2, 0x2, 0x4c4, 0x4c2, 0x3, 0x2, 0x2, 
    0x2, 0x4c5, 0x4bd, 0x3, 0x2, 0x2, 0x2, 0x4c5, 0x4c6, 0x3, 0x2, 0x2, 
    0x2, 0x4c6, 0x4c7, 0x3, 0x2, 0x2, 0x2, 0x4c7, 0x56c, 0x7, 0xa, 0x2, 
    0x2, 0x4c8, 0x4c9, 0x7, 0x91, 0x2, 0x2, 0x4c9, 0x4d2, 0x7, 0x9, 0x2, 
    0x2, 0x4ca, 0x4cf, 0x5, 0x78, 0x3d, 0x2, 0x4cb, 0x4cc, 0x7, 0x5, 0x2, 
    0x2, 0x4cc, 0x4ce, 0x5, 0x78, 0x3d, 0x2, 0x4cd, 0x4cb, 0x3, 0x2, 0x2, 
    0x2, 0x4ce, 0x4d1, 0x3, 0x2, 0x2, 0x2, 0x4cf, 0x4cd, 0x3, 0x2, 0x2, 
    0x2, 0x4cf, 0x4d0, 0x3, 0x2, 0x2, 0x2, 0x4d0, 0x4d3, 0x3, 0x2, 0x2, 
    0x2, 0x4d1, 0x4cf, 0x3, 0x2, 0x2, 0x2, 0x4d2, 0x4ca, 0x3, 0x2, 0x2, 
    0x2, 0x4d2, 0x4d3, 0x3, 0x2, 0x2, 0x2, 0x4d3, 0x4d4, 0x3, 0x2, 0x2, 
    0x2, 0x4d4, 0x56c, 0x7, 0xa, 0x2, 0x2, 0x4d5, 0x4d6, 0x7, 0x59, 0x2, 
    0x2, 0x4d6, 0x4d7, 0x7, 0x9, 0x2, 0x2, 0x4d7, 0x4d8, 0x5, 0x78, 0x3d, 
    0x2, 0x4d8, 0x4d9, 0x9, 0x14, 0x2, 0x2, 0x4d9, 0x4da, 0x5, 0x78, 0x3d, 
    0x2, 0x4da, 0x4db, 0x9, 0x15, 0x2, 0x2, 0x4db, 0x4de, 0x5, 0x78, 0x3d, 
    0x2, 0x4dc, 0x4dd, 0x9, 0x16, 0x2, 0x2, 0x4dd, 0x4df, 0x5, 0x78, 0x3d, 
    0x2, 0x4de, 0x4dc, 0x3, 0x2, 0x2, 0x2, 0x4de, 0x4df, 0x3, 0x2, 0x2, 
    0x2, 0x4df, 0x4e0, 0x3, 0x2, 0x2, 0x2, 0x4e0, 0x4e1, 0x7, 0xa, 0x2, 
    0x2, 0x4e1, 0x56c, 0x3, 0x2, 0x2, 0x2, 0x4e2, 0x4e3, 0x7, 0x99, 0x2, 
    0x2, 0x4e3, 0x4e4, 0x7, 0x9, 0x2, 0x2, 0x4e4, 0x4e5, 0x5, 0x78, 0x3d, 
    0x2, 0x4e5, 0x4e6, 0x9, 0x17, 0x2, 0x2, 0x4e6, 0x4e7, 0x5, 0x78, 0x3d, 
    0x2, 0x4e7, 0x4e8, 0x7, 0xa, 0x2, 0x2, 0x4e8, 0x56c, 0x3, 0x2, 0x2, 
    0x2, 0x4e9, 0x4ea, 0x9, 0x18, 0x2, 0x2, 0x4ea, 0x4f9, 0x7, 0x9, 0x2, 
    0x2, 0x4eb, 0x4ec, 0x5, 0x78, 0x3d, 0x2, 0x4ec, 0x4ed, 0x7, 0x5, 0x2, 
    0x2, 0x4ed, 0x4f0, 0x5, 0x78, 0x3d, 0x2, 0x4ee, 0x4ef, 0x7, 0x5, 0x2, 
    0x2, 0x4ef, 0x4f1, 0x5, 0x78, 0x3d, 0x2, 0x4f0, 0x4ee, 0x3, 0x2, 0x2, 
    0x2, 0x4f0, 0x4f1, 0x3, 0x2, 0x2, 0x2, 0x4f1, 0x4fa, 0x3, 0x2, 0x2, 
    0x2, 0x4f2, 0x4f3, 0x5, 0x78, 0x3d, 0x2, 0x4f3, 0x4f4, 0x7, 0x3a, 0x2, 
    0x2, 0x4f4, 0x4f7, 0x5, 0x78, 0x3d, 0x2, 0x4f5, 0x4f6, 0x7, 0x37, 0x2, 
    0x2, 0x4f6, 0x4f8, 0x5, 0x78, 0x3d, 0x2, 0x4f7, 0x4f5, 0x3, 0x2, 0x2, 
    0x2, 0x4f7, 0x4f8, 0x3, 0x2, 0x2, 0x2, 0x4f8, 0x4fa, 0x3, 0x2, 0x2, 
    0x2, 0x4f9, 0x4eb, 0x3, 0x2, 0x2, 0x2, 0x4f9, 0x4f2, 0x3, 0x2, 0x2, 
    0x2, 0x4fa, 0x4fb, 0x3, 0x2, 0x2, 0x2, 0x4fb, 0x4fc, 0x7, 0xa, 0x2, 
    0x2, 0x4fc, 0x56c, 0x3, 0x2, 0x2, 0x2, 0x4fd, 0x4fe, 0x7, 0x9c, 0x2, 
    0x2, 0x4fe, 0x50c, 0x7, 0x9, 0x2, 0x2, 0x4ff, 0x502, 0x5, 0x78, 0x3d, 
    0x2, 0x500, 0x501, 0x7, 0x5, 0x2, 0x2, 0x501, 0x503, 0x5, 0x78, 0x3d, 
    0x2, 0x502, 0x500, 0x3, 0x2, 0x2, 0x2, 0x502, 0x503, 0x3, 0x2, 0x2, 
    0x2, 0x503, 0x50d, 0x3, 0x2, 0x2, 0x2, 0x504, 0x506, 0x9, 0x19, 0x2, 
    0x2, 0x505, 0x504, 0x3, 0x2, 0x2, 0x2, 0x505, 0x506, 0x3, 0x2, 0x2, 
    0x2, 0x506, 0x508, 0x3, 0x2, 0x2, 0x2, 0x507, 0x509, 0x5, 0x78, 0x3d, 
    0x2, 0x508, 0x507, 0x3, 0x2, 0x2, 0x2, 0x508, 0x509, 0x3, 0x2, 0x2, 
    0x2, 0x509, 0x50a, 0x3, 0x2, 0x2, 0x2, 0x50a, 0x50b, 0x7, 0x3a, 0x2, 
    0x2, 0x50b, 0x50d, 0x5, 0x78, 0x3d, 0x2, 0x50c, 0x4ff, 0x3, 0x2, 0x2, 
    0x2, 0x50c, 0x505, 0x3, 0x2, 0x2, 0x2, 0x50d, 0x50e, 0x3, 0x2, 0x2, 
    0x2, 0x50e, 0x50f, 0x7, 0xa, 0x2, 0x2, 0x50f, 0x56c, 0x3, 0x2, 0x2, 
    0x2, 0x510, 0x511, 0x7, 0x94, 0x2, 0x2, 0x511, 0x512, 0x7, 0x9, 0x2, 
    0x2, 0x512, 0x513, 0x9, 0x1a, 0x2, 0x2, 0x513, 0x514, 0x7, 0x3a, 0x2, 
    0x2, 0x514, 0x515, 0x5, 0x78, 0x3d, 0x2, 0x515, 0x516, 0x7, 0xa, 0x2, 
    0x2, 0x516, 0x56c, 0x3, 0x2, 0x2, 0x2, 0x517, 0x518, 0x7, 0x92, 0x2, 
    0x2, 0x518, 0x51a, 0x7, 0x9, 0x2, 0x2, 0x519, 0x51b, 0x7, 0x17, 0x2, 
    0x2, 0x51a, 0x519, 0x3, 0x2, 0x2, 0x2, 0x51a, 0x51b, 0x3, 0x2, 0x2, 
    0x2, 0x51b, 0x51c, 0x3, 0x2, 0x2, 0x2, 0x51c, 0x51d, 0x7, 0x7, 0x2, 
    0x2, 0x51d, 0x56c, 0x7, 0xa, 0x2, 0x2, 0x51e, 0x51f, 0x7, 0x92, 0x2, 
    0x2, 0x51f, 0x522, 0x7, 0x9, 0x2, 0x2, 0x520, 0x523, 0x7, 0x17, 0x2, 
    0x2, 0x521, 0x523, 0x7, 0x30, 0x2, 0x2, 0x522, 0x520, 0x3, 0x2, 0x2, 
    0x2, 0x522, 0x521, 0x3, 0x2, 0x2, 0x2, 0x522, 0x523, 0x3, 0x2, 0x2, 
    0x2, 0x523, 0x524, 0x3, 0x2, 0x2, 0x2, 0x524, 0x525, 0x5, 0x78, 0x3d, 
    0x2, 0x525, 0x526, 0x7, 0xa, 0x2, 0x2, 0x526, 0x56c, 0x3, 0x2, 0x2, 
    0x2, 0x527, 0x528, 0x7, 0x68, 0x2, 0x2, 0x528, 0x52b, 0x7, 0x9, 0x2, 
    0x2, 0x529, 0x52c, 0x7, 0x17, 0x2, 0x2, 0x52a, 0x52c, 0x7, 0x30, 0x2, 
    0x2, 0x52b, 0x529, 0x3, 0x2, 0x2, 0x2, 0x52b, 0x52a, 0x3, 0x2, 0x2, 
    0x2, 0x52b, 0x52c, 0x3, 0x2, 0x2, 0x2, 0x52c, 0x52d, 0x3, 0x2, 0x2, 
    0x2, 0x52d, 0x52e, 0x5, 0x78, 0x3d, 0x2, 0x52e, 0x52f, 0x7, 0xa, 0x2, 
    0x2, 0x52f, 0x56c, 0x3, 0x2, 0x2, 0x2, 0x530, 0x531, 0x7, 0x1e, 0x2, 
    0x2, 0x531, 0x534, 0x7, 0x9, 0x2, 0x2, 0x532, 0x535, 0x7, 0x17, 0x2, 
    0x2, 0x533, 0x535, 0x7, 0x30, 0x2, 0x2, 0x534, 0x532, 0x3, 0x2, 0x2, 
    0x2, 0x534, 0x533, 0x3, 0x2, 0x2, 0x2, 0x534, 0x535, 0x3, 0x2, 0x2, 
    0x2, 0x535, 0x536, 0x3, 0x2, 0x2, 0x2, 0x536, 0x537, 0x5, 0x78, 0x3d, 
    0x2, 0x537, 0x538, 0x7, 0xa, 0x2, 0x2, 0x538, 0x56c, 0x3, 0x2, 0x2, 
    0x2, 0x539, 0x53a, 0x7, 0x4b, 0x2, 0x2, 0x53a, 0x53d, 0x7, 0x9, 0x2, 
    0x2, 0x53b, 0x53e, 0x7, 0x17, 0x2, 0x2, 0x53c, 0x53e, 0x7, 0x30, 0x2, 
    0x2, 0x53d, 0x53b, 0x3, 0x2, 0x2, 0x2, 0x53d, 0x53c, 0x3, 0x2, 0x2, 
    0x2, 0x53d, 0x53e, 0x3, 0x2, 0x2, 0x2, 0x53e, 0x53f, 0x3, 0x2, 0x2, 
    0x2, 0x53f, 0x540, 0x5, 0x78, 0x3d, 0x2, 0x540, 0x541, 0x7, 0xa, 0x2, 
    0x2, 0x541, 0x56c, 0x3, 0x2, 0x2, 0x2, 0x542, 0x543, 0x7, 0x4a, 0x2, 
    0x2, 0x543, 0x546, 0x7, 0x9, 0x2, 0x2, 0x544, 0x547, 0x7, 0x17, 0x2, 
    0x2, 0x545, 0x547, 0x7, 0x30, 0x2, 0x2, 0x546, 0x544, 0x3, 0x2, 0x2, 
    0x2, 0x546, 0x545, 0x3, 0x2, 0x2, 0x2, 0x546, 0x547, 0x3, 0x2, 0x2, 
    0x2, 0x547, 0x548, 0x3, 0x2, 0x2, 0x2, 0x548, 0x549, 0x5, 0x78, 0x3d, 
    0x2, 0x549, 0x54a, 0x7, 0xa, 0x2, 0x2, 0x54a, 0x56c, 0x3, 0x2, 0x2, 
    0x2, 0x54b, 0x54c, 0x5, 0x82, 0x42, 0x2, 0x54c, 0x555, 0x7, 0x9, 0x2, 
    0x2, 0x54d, 0x552, 0x5, 0x78, 0x3d, 0x2, 0x54e, 0x54f, 0x7, 0x5, 0x2, 
    0x2, 0x54f, 0x551, 0x5, 0x78, 0x3d, 0x2, 0x550, 0x54e, 0x3, 0x2, 0x2, 
    0x2, 0x551, 0x554, 0x3, 0x2, 0x2, 0x2, 0x552, 0x550, 0x3, 0x2, 0x2, 
    0x2, 0x552, 0x553, 0x3, 0x2, 0x2, 0x2, 0x553, 0x556, 0x3, 0x2, 0x2, 
    0x2, 0x554, 0x552, 0x3, 0x2, 0x2, 0x2, 0x555, 0x54d, 0x3, 0x2, 0x2, 
    0x2, 0x555, 0x556, 0x3, 0x2, 0x2, 0x2, 0x556, 0x557, 0x3, 0x2, 0x2, 
    0x2, 0x557, 0x558, 0x7, 0xa, 0x2, 0x2, 0x558, 0x56c, 0x3, 0x2, 0x2, 
    0x2, 0x559, 0x55a, 0x7, 0x9, 0x2, 0x2, 0x55a, 0x55b, 0x5, 0x78, 0x3d, 
    0x2, 0x55b, 0x55c, 0x7, 0xa, 0x2, 0x2, 0x55c, 0x56c, 0x3, 0x2, 0x2, 
    0x2, 0x55d, 0x55e, 0x7, 0x9, 0x2, 0x2, 0x55e, 0x55f, 0x5, 0x16, 0xc, 
    0x2, 0x55f, 0x560, 0x7, 0xa, 0x2, 0x2, 0x560, 0x56c, 0x3, 0x2, 0x2, 
    0x2, 0x561, 0x563, 0x7, 0x50, 0x2, 0x2, 0x562, 0x561, 0x3, 0x2, 0x2, 
    0x2, 0x562, 0x563, 0x3, 0x2, 0x2, 0x2, 0x563, 0x564, 0x3, 0x2, 0x2, 
    0x2, 0x564, 0x565, 0x7, 0x95, 0x2, 0x2, 0x565, 0x566, 0x7, 0x9, 0x2, 
    0x2, 0x566, 0x567, 0x5, 0x16, 0xc, 0x2, 0x567, 0x568, 0x7, 0xa, 0x2, 
    0x2, 0x568, 0x56c, 0x3, 0x2, 0x2, 0x2, 0x569, 0x56a, 0x7, 0x50, 0x2, 
    0x2, 0x56a, 0x56c, 0x5, 0x78, 0x3d, 0x6, 0x56b, 0x464, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x466, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x467, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x468, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x46d, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x46f, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x476, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x47b, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x497, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x4a0, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x4a7, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x4ae, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x4bb, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x4c8, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x4d5, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x4e2, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x4e9, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x4fd, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x510, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x517, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x51e, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x527, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x530, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x539, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x542, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x54b, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x559, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x55d, 0x3, 0x2, 0x2, 0x2, 0x56b, 0x562, 0x3, 0x2, 0x2, 
    0x2, 0x56b, 0x569, 0x3, 0x2, 0x2, 0x2, 0x56c, 0x5c0, 0x3, 0x2, 0x2, 
    0x2, 0x56d, 0x56e, 0xc, 0x26, 0x2, 0x2, 0x56e, 0x56f, 0x9, 0x1b, 0x2, 
    0x2, 0x56f, 0x5bf, 0x5, 0x78, 0x3d, 0x27, 0x570, 0x571, 0xc, 0x25, 0x2, 
    0x2, 0x571, 0x572, 0x9, 0x1c, 0x2, 0x2, 0x572, 0x5bf, 0x5, 0x78, 0x3d, 
    0x26, 0x573, 0x574, 0xc, 0x24, 0x2, 0x2, 0x574, 0x575, 0x9, 0x1d, 0x2, 
    0x2, 0x575, 0x5bf, 0x5, 0x78, 0x3d, 0x25, 0x576, 0x578, 0xc, 0xb, 0x2, 
    0x2, 0x577, 0x579, 0x7, 0x50, 0x2, 0x2, 0x578, 0x577, 0x3, 0x2, 0x2, 
    0x2, 0x578, 0x579, 0x3, 0x2, 0x2, 0x2, 0x579, 0x57a, 0x3, 0x2, 0x2, 
    0x2, 0x57a, 0x57b, 0x7, 0x48, 0x2, 0x2, 0x57b, 0x5bf, 0x5, 0x78, 0x3d, 
    0xc, 0x57c, 0x57e, 0xc, 0xa, 0x2, 0x2, 0x57d, 0x57f, 0x7, 0x50, 0x2, 
    0x2, 0x57e, 0x57d, 0x3, 0x2, 0x2, 0x2, 0x57e, 0x57f, 0x3, 0x2, 0x2, 
    0x2, 0x57f, 0x580, 0x3, 0x2, 0x2, 0x2, 0x580, 0x581, 0x7, 0x48, 0x2, 
    0x2, 0x581, 0x582, 0x5, 0x78, 0x3d, 0x2, 0x582, 0x583, 0x7, 0x32, 0x2, 
    0x2, 0x583, 0x584, 0x5, 0x78, 0x3d, 0xb, 0x584, 0x5bf, 0x3, 0x2, 0x2, 
    0x2, 0x585, 0x586, 0xc, 0x5, 0x2, 0x2, 0x586, 0x587, 0x7, 0x18, 0x2, 
    0x2, 0x587, 0x5bf, 0x5, 0x78, 0x3d, 0x6, 0x588, 0x589, 0xc, 0x4, 0x2, 
    0x2, 0x589, 0x58a, 0x7, 0x55, 0x2, 0x2, 0x58a, 0x5bf, 0x5, 0x78, 0x3d, 
    0x5, 0x58b, 0x58c, 0xc, 0x3, 0x2, 0x2, 0x58c, 0x58d, 0x7, 0x78, 0x2, 
    0x2, 0x58d, 0x5bf, 0x5, 0x78, 0x3d, 0x4, 0x58e, 0x58f, 0xc, 0x27, 0x2, 
    0x2, 0x58f, 0x590, 0x7, 0xc, 0x2, 0x2, 0x590, 0x591, 0x5, 0x84, 0x43, 
    0x2, 0x591, 0x59a, 0x7, 0x9, 0x2, 0x2, 0x592, 0x597, 0x5, 0x78, 0x3d, 
    0x2, 0x593, 0x594, 0x7, 0x5, 0x2, 0x2, 0x594, 0x596, 0x5, 0x78, 0x3d, 
    0x2, 0x595, 0x593, 0x3, 0x2, 0x2, 0x2, 0x596, 0x599, 0x3, 0x2, 0x2, 
    0x2, 0x597, 0x595, 0x3, 0x2, 0x2, 0x2, 0x597, 0x598, 0x3, 0x2, 0x2, 
    0x2, 0x598, 0x59b, 0x3, 0x2, 0x2, 0x2, 0x599, 0x597, 0x3, 0x2, 0x2, 
    0x2, 0x59a, 0x592, 0x3, 0x2, 0x2, 0x2, 0x59a, 0x59b, 0x3, 0x2, 0x2, 
    0x2, 0x59b, 0x59c, 0x3, 0x2, 0x2, 0x2, 0x59c, 0x59d, 0x7, 0xa, 0x2, 
    0x2, 0x59d, 0x5bf, 0x3, 0x2, 0x2, 0x2, 0x59e, 0x59f, 0xc, 0xc, 0x2, 
    0x2, 0x59f, 0x5a1, 0x7, 0x43, 0x2, 0x2, 0x5a0, 0x5a2, 0x7, 0x50, 0x2, 
    0x2, 0x5a1, 0x5a0, 0x3, 0x2, 0x2, 0x2, 0x5a1, 0x5a2, 0x3, 0x2, 0x2, 
    0x2, 0x5a2, 0x5a3, 0x3, 0x2, 0x2, 0x2, 0x5a3, 0x5bf, 0x7, 0x52, 0x2, 
    0x2, 0x5a4, 0x5a6, 0xc, 0x8, 0x2, 0x2, 0x5a5, 0x5a7, 0x7, 0x50, 0x2, 
    0x2, 0x5a6, 0x5a5, 0x3, 0x2, 0x2, 0x2, 0x5a6, 0x5a7, 0x3, 0x2, 0x2, 
    0x2, 0x5a7, 0x5a8, 0x3, 0x2, 0x2, 0x2, 0x5a8, 0x5a9, 0x7, 0x3d, 0x2, 
    0x2, 0x5a9, 0x5aa, 0x7, 0x9, 0x2, 0x2, 0x5aa, 0x5ab, 0x5, 0x16, 0xc, 
    0x2, 0x5ab, 0x5ac, 0x7, 0xa, 0x2, 0x2, 0x5ac, 0x5bf, 0x3, 0x2, 0x2, 
    0x2, 0x5ad, 0x5af, 0xc, 0x7, 0x2, 0x2, 0x5ae, 0x5b0, 0x7, 0x50, 0x2, 
    0x2, 0x5af, 0x5ae, 0x3, 0x2, 0x2, 0x2, 0x5af, 0x5b0, 0x3, 0x2, 0x2, 
    0x2, 0x5b0, 0x5b1, 0x3, 0x2, 0x2, 0x2, 0x5b1, 0x5b2, 0x7, 0x3d, 0x2, 
    0x2, 0x5b2, 0x5bb, 0x7, 0x9, 0x2, 0x2, 0x5b3, 0x5b8, 0x5, 0x78, 0x3d, 
    0x2, 0x5b4, 0x5b5, 0x7, 0x5, 0x2, 0x2, 0x5b5, 0x5b7, 0x5, 0x78, 0x3d, 
    0x2, 0x5b6, 0x5b4, 0x3, 0x2, 0x2, 0x2, 0x5b7, 0x5ba, 0x3, 0x2, 0x2, 
    0x2, 0x5b8, 0x5b6, 0x3, 0x2, 0x2, 0x2, 0x5b8, 0x5b9, 0x3, 0x2, 0x2, 
    0x2, 0x5b9, 0x5bc, 0x3, 0x2, 0x2, 0x2, 0x5ba, 0x5b8, 0x3, 0x2, 0x2, 
    0x2, 0x5bb, 0x5b3, 0x3, 0x2, 0x2, 0x2, 0x5bb, 0x5bc, 0x3, 0x2, 0x2, 
    0x2, 0x5bc, 0x5bd, 0x3, 0x2, 0x2, 0x2, 0x5bd, 0x5bf, 0x7, 0xa, 0x2, 
    0x2, 0x5be, 0x56d, 0x3, 0x2, 0x2, 0x2, 0x5be, 0x570, 0x3, 0x2, 0x2, 
    0x2, 0x5be, 0x573, 0x3, 0x2, 0x2, 0x2, 0x5be, 0x576, 0x3, 0x2, 0x2, 
    0x2, 0x5be, 0x57c, 0x3, 0x2, 0x2, 0x2, 0x5be, 0x585, 0x3, 0x2, 0x2, 
    0x2, 0x5be, 0x588, 0x3, 0x2, 0x2, 0x2, 0x5be, 0x58b, 0x3, 0x2, 0x2, 
    0x2, 0x5be, 0x58e, 0x3, 0x2, 0x2, 0x2, 0x5be, 0x59e, 0x3, 0x2, 0x2, 
    0x2, 0x5be, 0x5a4, 0x3, 0x2, 0x2, 0x2, 0x5be, 0x5ad, 0x3, 0x2, 0x2, 
    0x2, 0x5bf, 0x5c2, 0x3, 0x2, 0x2, 0x2, 0x5c0, 0x5be, 0x3, 0x2, 0x2, 
    0x2, 0x5c0, 0x5c1, 0x3, 0x2, 0x2, 0x2, 0x5c1, 0x79, 0x3, 0x2, 0x2, 0x2, 
    0x5c2, 0x5c0, 0x3, 0x2, 0x2, 0x2, 0x5c3, 0x5c4, 0x9, 0x1e, 0x2, 0x2, 
    0x5c4, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x5c5, 0x5c6, 0x5, 0x86, 0x44, 0x2, 
    0x5c6, 0x5c7, 0x7, 0xc, 0x2, 0x2, 0x5c7, 0x5c9, 0x3, 0x2, 0x2, 0x2, 
    0x5c8, 0x5c5, 0x3, 0x2, 0x2, 0x2, 0x5c8, 0x5c9, 0x3, 0x2, 0x2, 0x2, 
    0x5c9, 0x5ca, 0x3, 0x2, 0x2, 0x2, 0x5ca, 0x5cb, 0x5, 0x88, 0x45, 0x2, 
    0x5cb, 0x5cc, 0x7, 0xc, 0x2, 0x2, 0x5cc, 0x5ce, 0x3, 0x2, 0x2, 0x2, 
    0x5cd, 0x5c8, 0x3, 0x2, 0x2, 0x2, 0x5cd, 0x5ce, 0x3, 0x2, 0x2, 0x2, 
    0x5ce, 0x5cf, 0x3, 0x2, 0x2, 0x2, 0x5cf, 0x5d0, 0x5, 0x8a, 0x46, 0x2, 
    0x5d0, 0x5d1, 0x7, 0xc, 0x2, 0x2, 0x5d1, 0x5d3, 0x3, 0x2, 0x2, 0x2, 
    0x5d2, 0x5cd, 0x3, 0x2, 0x2, 0x2, 0x5d2, 0x5d3, 0x3, 0x2, 0x2, 0x2, 
    0x5d3, 0x5d4, 0x3, 0x2, 0x2, 0x2, 0x5d4, 0x5d5, 0x5, 0x8e, 0x48, 0x2, 
    0x5d5, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x5d6, 0x5d7, 0x5, 0x86, 0x44, 0x2, 
    0x5d7, 0x5d8, 0x7, 0xc, 0x2, 0x2, 0x5d8, 0x5da, 0x3, 0x2, 0x2, 0x2, 
    0x5d9, 0x5d6, 0x3, 0x2, 0x2, 0x2, 0x5d9, 0x5da, 0x3, 0x2, 0x2, 0x2, 
    0x5da, 0x5db, 0x3, 0x2, 0x2, 0x2, 0x5db, 0x5dc, 0x5, 0x88, 0x45, 0x2, 
    0x5dc, 0x5dd, 0x7, 0xc, 0x2, 0x2, 0x5dd, 0x5df, 0x3, 0x2, 0x2, 0x2, 
    0x5de, 0x5d9, 0x3, 0x2, 0x2, 0x2, 0x5de, 0x5df, 0x3, 0x2, 0x2, 0x2, 
    0x5df, 0x5e0, 0x3, 0x2, 0x2, 0x2, 0x5e0, 0x5e1, 0x5, 0x8a, 0x46, 0x2, 
    0x5e1, 0x5e2, 0x7, 0xc, 0x2, 0x2, 0x5e2, 0x5e4, 0x3, 0x2, 0x2, 0x2, 
    0x5e3, 0x5de, 0x3, 0x2, 0x2, 0x2, 0x5e3, 0x5e4, 0x3, 0x2, 0x2, 0x2, 
    0x5e4, 0x5e5, 0x3, 0x2, 0x2, 0x2, 0x5e5, 0x5e6, 0x7, 0x7, 0x2, 0x2, 
    0x5e6, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x5e7, 0x5e8, 0x5, 0xa0, 0x51, 0x2, 
    0x5e8, 0x5e9, 0x7, 0xc, 0x2, 0x2, 0x5e9, 0x5eb, 0x3, 0x2, 0x2, 0x2, 
    0x5ea, 0x5e7, 0x3, 0x2, 0x2, 0x2, 0x5eb, 0x5ee, 0x3, 0x2, 0x2, 0x2, 
    0x5ec, 0x5ea, 0x3, 0x2, 0x2, 0x2, 0x5ec, 0x5ed, 0x3, 0x2, 0x2, 0x2, 
    0x5ed, 0x5ef, 0x3, 0x2, 0x2, 0x2, 0x5ee, 0x5ec, 0x3, 0x2, 0x2, 0x2, 
    0x5ef, 0x5f0, 0x5, 0x9e, 0x50, 0x2, 0x5f0, 0x81, 0x3, 0x2, 0x2, 0x2, 
    0x5f1, 0x5f2, 0x5, 0x9e, 0x50, 0x2, 0x5f2, 0x83, 0x3, 0x2, 0x2, 0x2, 
    0x5f3, 0x5f4, 0x5, 0x9e, 0x50, 0x2, 0x5f4, 0x85, 0x3, 0x2, 0x2, 0x2, 
    0x5f5, 0x5f6, 0x5, 0x9e, 0x50, 0x2, 0x5f6, 0x87, 0x3, 0x2, 0x2, 0x2, 
    0x5f7, 0x5f8, 0x5, 0x9e, 0x50, 0x2, 0x5f8, 0x89, 0x3, 0x2, 0x2, 0x2, 
    0x5f9, 0x5fa, 0x5, 0x9e, 0x50, 0x2, 0x5fa, 0x8b, 0x3, 0x2, 0x2, 0x2, 
    0x5fb, 0x5fc, 0x5, 0x9e, 0x50, 0x2, 0x5fc, 0x8d, 0x3, 0x2, 0x2, 0x2, 
    0x5fd, 0x5fe, 0x5, 0x9e, 0x50, 0x2, 0x5fe, 0x8f, 0x3, 0x2, 0x2, 0x2, 
    0x5ff, 0x600, 0x5, 0x9e, 0x50, 0x2, 0x600, 0x91, 0x3, 0x2, 0x2, 0x2, 
    0x601, 0x602, 0x5, 0x9e, 0x50, 0x2, 0x602, 0x93, 0x3, 0x2, 0x2, 0x2, 
    0x603, 0x604, 0x5, 0x9e, 0x50, 0x2, 0x604, 0x95, 0x3, 0x2, 0x2, 0x2, 
    0x605, 0x606, 0x5, 0x9e, 0x50, 0x2, 0x606, 0x97, 0x3, 0x2, 0x2, 0x2, 
    0x607, 0x608, 0x5, 0x9e, 0x50, 0x2, 0x608, 0x99, 0x3, 0x2, 0x2, 0x2, 
    0x609, 0x60c, 0x5, 0xaa, 0x56, 0x2, 0x60a, 0x60c, 0x5, 0x9e, 0x50, 0x2, 
    0x60b, 0x609, 0x3, 0x2, 0x2, 0x2, 0x60b, 0x60a, 0x3, 0x2, 0x2, 0x2, 
    0x60c, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x60d, 0x610, 0x5, 0xaa, 0x56, 0x2, 
    0x60e, 0x610, 0x5, 0x9e, 0x50, 0x2, 0x60f, 0x60d, 0x3, 0x2, 0x2, 0x2, 
    0x60f, 0x60e, 0x3, 0x2, 0x2, 0x2, 0x610, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x611, 
    0x614, 0x7, 0xba, 0x2, 0x2, 0x612, 0x614, 0x5, 0xc4, 0x63, 0x2, 0x613, 
    0x611, 0x3, 0x2, 0x2, 0x2, 0x613, 0x612, 0x3, 0x2, 0x2, 0x2, 0x614, 
    0x9f, 0x3, 0x2, 0x2, 0x2, 0x615, 0x616, 0x5, 0x9e, 0x50, 0x2, 0x616, 
    0xa1, 0x3, 0x2, 0x2, 0x2, 0x617, 0x618, 0x5, 0xae, 0x58, 0x2, 0x618, 
    0xa3, 0x3, 0x2, 0x2, 0x2, 0x619, 0x61a, 0x5, 0xae, 0x58, 0x2, 0x61a, 
    0xa5, 0x3, 0x2, 0x2, 0x2, 0x61b, 0x61c, 0x5, 0xae, 0x58, 0x2, 0x61c, 
    0xa7, 0x3, 0x2, 0x2, 0x2, 0x61d, 0x628, 0x7, 0x52, 0x2, 0x2, 0x61e, 
    0x628, 0x5, 0xac, 0x57, 0x2, 0x61f, 0x628, 0x5, 0xae, 0x58, 0x2, 0x620, 
    0x628, 0x5, 0xb0, 0x59, 0x2, 0x621, 0x628, 0x5, 0xb2, 0x5a, 0x2, 0x622, 
    0x628, 0x5, 0xb4, 0x5b, 0x2, 0x623, 0x628, 0x5, 0xb6, 0x5c, 0x2, 0x624, 
    0x628, 0x5, 0xba, 0x5e, 0x2, 0x625, 0x628, 0x5, 0xb8, 0x5d, 0x2, 0x626, 
    0x628, 0x5, 0xbc, 0x5f, 0x2, 0x627, 0x61d, 0x3, 0x2, 0x2, 0x2, 0x627, 
    0x61e, 0x3, 0x2, 0x2, 0x2, 0x627, 0x61f, 0x3, 0x2, 0x2, 0x2, 0x627, 
    0x620, 0x3, 0x2, 0x2, 0x2, 0x627, 0x621, 0x3, 0x2, 0x2, 0x2, 0x627, 
    0x622, 0x3, 0x2, 0x2, 0x2, 0x627, 0x623, 0x3, 0x2, 0x2, 0x2, 0x627, 
    0x624, 0x3, 0x2, 0x2, 0x2, 0x627, 0x625, 0x3, 0x2, 0x2, 0x2, 0x627, 
    0x626, 0x3, 0x2, 0x2, 0x2, 0x628, 0xa9, 0x3, 0x2, 0x2, 0x2, 0x629, 0x630, 
    0x7, 0x52, 0x2, 0x2, 0x62a, 0x630, 0x5, 0xac, 0x57, 0x2, 0x62b, 0x630, 
    0x5, 0xae, 0x58, 0x2, 0x62c, 0x630, 0x5, 0xb0, 0x59, 0x2, 0x62d, 0x630, 
    0x5, 0xb2, 0x5a, 0x2, 0x62e, 0x630, 0x5, 0xba, 0x5e, 0x2, 0x62f, 0x629, 
    0x3, 0x2, 0x2, 0x2, 0x62f, 0x62a, 0x3, 0x2, 0x2, 0x2, 0x62f, 0x62b, 
    0x3, 0x2, 0x2, 0x2, 0x62f, 0x62c, 0x3, 0x2, 0x2, 0x2, 0x62f, 0x62d, 
    0x3, 0x2, 0x2, 0x2, 0x62f, 0x62e, 0x3, 0x2, 0x2, 0x2, 0x630, 0xab, 0x3, 
    0x2, 0x2, 0x2, 0x631, 0x632, 0x9, 0x1f, 0x2, 0x2, 0x632, 0xad, 0x3, 
    0x2, 0x2, 0x2, 0x633, 0x634, 0x7, 0xc3, 0x2, 0x2, 0x634, 0xaf, 0x3, 
    0x2, 0x2, 0x2, 0x635, 0x636, 0x7, 0xc2, 0x2, 0x2, 0x636, 0xb1, 0x3, 
    0x2, 0x2, 0x2, 0x637, 0x638, 0x7, 0xc1, 0x2, 0x2, 0x638, 0xb3, 0x3, 
    0x2, 0x2, 0x2, 0x639, 0x63a, 0x7, 0x2c, 0x2, 0x2, 0x63a, 0x63b, 0x5, 
    0xba, 0x5e, 0x2, 0x63b, 0xb5, 0x3, 0x2, 0x2, 0x2, 0x63c, 0x63d, 0x7, 
    0x7d, 0x2, 0x2, 0x63d, 0x63e, 0x5, 0xba, 0x5e, 0x2, 0x63e, 0xb7, 0x3, 
    0x2, 0x2, 0x2, 0x63f, 0x640, 0x7, 0x7e, 0x2, 0x2, 0x640, 0x641, 0x5, 
    0xba, 0x5e, 0x2, 0x641, 0xb9, 0x3, 0x2, 0x2, 0x2, 0x642, 0x643, 0x7, 
    0xbe, 0x2, 0x2, 0x643, 0xbb, 0x3, 0x2, 0x2, 0x2, 0x644, 0x648, 0x5, 
    0xc0, 0x61, 0x2, 0x645, 0x648, 0x5, 0xc2, 0x62, 0x2, 0x646, 0x648, 0x5, 
    0xbe, 0x60, 0x2, 0x647, 0x644, 0x3, 0x2, 0x2, 0x2, 0x647, 0x645, 0x3, 
    0x2, 0x2, 0x2, 0x647, 0x646, 0x3, 0x2, 0x2, 0x2, 0x648, 0xbd, 0x3, 0x2, 
    0x2, 0x2, 0x649, 0x64a, 0x7, 0x7f, 0x2, 0x2, 0x64a, 0x64b, 0x5, 0x78, 
    0x3d, 0x2, 0x64b, 0xbf, 0x3, 0x2, 0x2, 0x2, 0x64c, 0x64d, 0x7, 0x7f, 
    0x2, 0x2, 0x64d, 0x665, 0x5, 0xba, 0x5e, 0x2, 0x64e, 0x653, 0x7, 0xb0, 
    0x2, 0x2, 0x64f, 0x650, 0x7, 0x9, 0x2, 0x2, 0x650, 0x651, 0x5, 0xa2, 
    0x52, 0x2, 0x651, 0x652, 0x7, 0xa, 0x2, 0x2, 0x652, 0x654, 0x3, 0x2, 
    0x2, 0x2, 0x653, 0x64f, 0x3, 0x2, 0x2, 0x2, 0x653, 0x654, 0x3, 0x2, 
    0x2, 0x2, 0x654, 0x666, 0x3, 0x2, 0x2, 0x2, 0x655, 0x65a, 0x7, 0xb0, 
    0x2, 0x2, 0x656, 0x657, 0x7, 0x9, 0x2, 0x2, 0x657, 0x658, 0x5, 0xa2, 
    0x52, 0x2, 0x658, 0x659, 0x7, 0xa, 0x2, 0x2, 0x659, 0x65b, 0x3, 0x2, 
    0x2, 0x2, 0x65a, 0x656, 0x3, 0x2, 0x2, 0x2, 0x65a, 0x65b, 0x3, 0x2, 
    0x2, 0x2, 0x65b, 0x65c, 0x3, 0x2, 0x2, 0x2, 0x65c, 0x65d, 0x7, 0x6a, 
    0x2, 0x2, 0x65d, 0x666, 0x7, 0xa8, 0x2, 0x2, 0x65e, 0x663, 0x7, 0xa8, 
    0x2, 0x2, 0x65f, 0x660, 0x7, 0x9, 0x2, 0x2, 0x660, 0x661, 0x5, 0xa2, 
    0x52, 0x2, 0x661, 0x662, 0x7, 0xa, 0x2, 0x2, 0x662, 0x664, 0x3, 0x2, 
    0x2, 0x2, 0x663, 0x65f, 0x3, 0x2, 0x2, 0x2, 0x663, 0x664, 0x3, 0x2, 
    0x2, 0x2, 0x664, 0x666, 0x3, 0x2, 0x2, 0x2, 0x665, 0x64e, 0x3, 0x2, 
    0x2, 0x2, 0x665, 0x655, 0x3, 0x2, 0x2, 0x2, 0x665, 0x65e, 0x3, 0x2, 
    0x2, 0x2, 0x666, 0xc1, 0x3, 0x2, 0x2, 0x2, 0x667, 0x668, 0x7, 0x7f, 
    0x2, 0x2, 0x668, 0x6bd, 0x5, 0xba, 0x5e, 0x2, 0x669, 0x66e, 0x7, 0xa2, 
    0x2, 0x2, 0x66a, 0x66b, 0x7, 0x9, 0x2, 0x2, 0x66b, 0x66c, 0x5, 0xa2, 
    0x52, 0x2, 0x66c, 0x66d, 0x7, 0xa, 0x2, 0x2, 0x66d, 0x66f, 0x3, 0x2, 
    0x2, 0x2, 0x66e, 0x66a, 0x3, 0x2, 0x2, 0x2, 0x66e, 0x66f, 0x3, 0x2, 
    0x2, 0x2, 0x66f, 0x6be, 0x3, 0x2, 0x2, 0x2, 0x670, 0x675, 0x7, 0xa2, 
    0x2, 0x2, 0x671, 0x672, 0x7, 0x9, 0x2, 0x2, 0x672, 0x673, 0x5, 0xa2, 
    0x52, 0x2, 0x673, 0x674, 0x7, 0xa, 0x2, 0x2, 0x674, 0x676, 0x3, 0x2, 
    0x2, 0x2, 0x675, 0x671, 0x3, 0x2, 0x2, 0x2, 0x675, 0x676, 0x3, 0x2, 
    0x2, 0x2, 0x676, 0x677, 0x3, 0x2, 0x2, 0x2, 0x677, 0x681, 0x7, 0x6a, 
    0x2, 0x2, 0x678, 0x682, 0x7, 0xa4, 0x2, 0x2, 0x679, 0x682, 0x7, 0xa6, 
    0x2, 0x2, 0x67a, 0x67f, 0x7, 0xac, 0x2, 0x2, 0x67b, 0x67c, 0x7, 0x9, 
    0x2, 0x2, 0x67c, 0x67d, 0x5, 0xa4, 0x53, 0x2, 0x67d, 0x67e, 0x7, 0xa, 
    0x2, 0x2, 0x67e, 0x680, 0x3, 0x2, 0x2, 0x2, 0x67f, 0x67b, 0x3, 0x2, 
    0x2, 0x2, 0x67f, 0x680, 0x3, 0x2, 0x2, 0x2, 0x680, 0x682, 0x3, 0x2, 
    0x2, 0x2, 0x681, 0x678, 0x3, 0x2, 0x2, 0x2, 0x681, 0x679, 0x3, 0x2, 
    0x2, 0x2, 0x681, 0x67a, 0x3, 0x2, 0x2, 0x2, 0x682, 0x6be, 0x3, 0x2, 
    0x2, 0x2, 0x683, 0x688, 0x7, 0xa4, 0x2, 0x2, 0x684, 0x685, 0x7, 0x9, 
    0x2, 0x2, 0x685, 0x686, 0x5, 0xa2, 0x52, 0x2, 0x686, 0x687, 0x7, 0xa, 
    0x2, 0x2, 0x687, 0x689, 0x3, 0x2, 0x2, 0x2, 0x688, 0x684, 0x3, 0x2, 
    0x2, 0x2, 0x688, 0x689, 0x3, 0x2, 0x2, 0x2, 0x689, 0x68a, 0x3, 0x2, 
    0x2, 0x2, 0x68a, 0x693, 0x7, 0x6a, 0x2, 0x2, 0x68b, 0x694, 0x7, 0xa6, 
    0x2, 0x2, 0x68c, 0x691, 0x7, 0xac, 0x2, 0x2, 0x68d, 0x68e, 0x7, 0x9, 
    0x2, 0x2, 0x68e, 0x68f, 0x5, 0xa4, 0x53, 0x2, 0x68f, 0x690, 0x7, 0xa, 
    0x2, 0x2, 0x690, 0x692, 0x3, 0x2, 0x2, 0x2, 0x691, 0x68d, 0x3, 0x2, 
    0x2, 0x2, 0x691, 0x692, 0x3, 0x2, 0x2, 0x2, 0x692, 0x694, 0x3, 0x2, 
    0x2, 0x2, 0x693, 0x68b, 0x3, 0x2, 0x2, 0x2, 0x693, 0x68c, 0x3, 0x2, 
    0x2, 0x2, 0x694, 0x6be, 0x3, 0x2, 0x2, 0x2, 0x695, 0x69a, 0x7, 0xa6, 
    0x2, 0x2, 0x696, 0x697, 0x7, 0x9, 0x2, 0x2, 0x697, 0x698, 0x5, 0xa2, 
    0x52, 0x2, 0x698, 0x699, 0x7, 0xa, 0x2, 0x2, 0x699, 0x69b, 0x3, 0x2, 
    0x2, 0x2, 0x69a, 0x696, 0x3, 0x2, 0x2, 0x2, 0x69a, 0x69b, 0x3, 0x2, 
    0x2, 0x2, 0x69b, 0x69c, 0x3, 0x2, 0x2, 0x2, 0x69c, 0x69d, 0x7, 0x6a, 
    0x2, 0x2, 0x69d, 0x6a2, 0x7, 0xac, 0x2, 0x2, 0x69e, 0x69f, 0x7, 0x9, 
    0x2, 0x2, 0x69f, 0x6a0, 0x5, 0xa4, 0x53, 0x2, 0x6a0, 0x6a1, 0x7, 0xa, 
    0x2, 0x2, 0x6a1, 0x6a3, 0x3, 0x2, 0x2, 0x2, 0x6a2, 0x69e, 0x3, 0x2, 
    0x2, 0x2, 0x6a2, 0x6a3, 0x3, 0x2, 0x2, 0x2, 0x6a3, 0x6be, 0x3, 0x2, 
    0x2, 0x2, 0x6a4, 0x6a9, 0x7, 0xa4, 0x2, 0x2, 0x6a5, 0x6a6, 0x7, 0x9, 
    0x2, 0x2, 0x6a6, 0x6a7, 0x5, 0xa2, 0x52, 0x2, 0x6a7, 0x6a8, 0x7, 0xa, 
    0x2, 0x2, 0x6a8, 0x6aa, 0x3, 0x2, 0x2, 0x2, 0x6a9, 0x6a5, 0x3, 0x2, 
    0x2, 0x2, 0x6a9, 0x6aa, 0x3, 0x2, 0x2, 0x2, 0x6aa, 0x6be, 0x3, 0x2, 
    0x2, 0x2, 0x6ab, 0x6b0, 0x7, 0xa6, 0x2, 0x2, 0x6ac, 0x6ad, 0x7, 0x9, 
    0x2, 0x2, 0x6ad, 0x6ae, 0x5, 0xa2, 0x52, 0x2, 0x6ae, 0x6af, 0x7, 0xa, 
    0x2, 0x2, 0x6af, 0x6b1, 0x3, 0x2, 0x2, 0x2, 0x6b0, 0x6ac, 0x3, 0x2, 
    0x2, 0x2, 0x6b0, 0x6b1, 0x3, 0x2, 0x2, 0x2, 0x6b1, 0x6be, 0x3, 0x2, 
    0x2, 0x2, 0x6b2, 0x6bb, 0x7, 0xac, 0x2, 0x2, 0x6b3, 0x6b4, 0x7, 0x9, 
    0x2, 0x2, 0x6b4, 0x6b7, 0x5, 0xa2, 0x52, 0x2, 0x6b5, 0x6b6, 0x7, 0x5, 
    0x2, 0x2, 0x6b6, 0x6b8, 0x5, 0xa4, 0x53, 0x2, 0x6b7, 0x6b5, 0x3, 0x2, 
    0x2, 0x2, 0x6b7, 0x6b8, 0x3, 0x2, 0x2, 0x2, 0x6b8, 0x6b9, 0x3, 0x2, 
    0x2, 0x2, 0x6b9, 0x6ba, 0x7, 0xa, 0x2, 0x2, 0x6ba, 0x6bc, 0x3, 0x2, 
    0x2, 0x2, 0x6bb, 0x6b3, 0x3, 0x2, 0x2, 0x2, 0x6bb, 0x6bc, 0x3, 0x2, 
    0x2, 0x2, 0x6bc, 0x6be, 0x3, 0x2, 0x2, 0x2, 0x6bd, 0x669, 0x3, 0x2, 
    0x2, 0x2, 0x6bd, 0x670, 0x3, 0x2, 0x2, 0x2, 0x6bd, 0x683, 0x3, 0x2, 
    0x2, 0x2, 0x6bd, 0x695, 0x3, 0x2, 0x2, 0x2, 0x6bd, 0x6a4, 0x3, 0x2, 
    0x2, 0x2, 0x6bd, 0x6ab, 0x3, 0x2, 0x2, 0x2, 0x6bd, 0x6b2, 0x3, 0x2, 
    0x2, 0x2, 0x6be, 0xc3, 0x3, 0x2, 0x2, 0x2, 0x6bf, 0x6c0, 0x9, 0x20, 
    0x2, 0x2, 0x6c0, 0xc5, 0x3, 0x2, 0x2, 0x2, 0xea, 0xc8, 0xd0, 0xd7, 0xde, 
    0xe6, 0xee, 0xf4, 0xf9, 0x101, 0x10a, 0x115, 0x119, 0x11f, 0x12b, 0x133, 
    0x138, 0x140, 0x143, 0x149, 0x14f, 0x151, 0x156, 0x15b, 0x15f, 0x164, 
    0x168, 0x16e, 0x176, 0x179, 0x17f, 0x185, 0x187, 0x18f, 0x195, 0x198, 
    0x19d, 0x1a6, 0x1af, 0x1b4, 0x1be, 0x1c1, 0x1c7, 0x1ca, 0x1d0, 0x1d3, 
    0x1d5, 0x1db, 0x1e0, 0x1e3, 0x1ec, 0x1ef, 0x1f8, 0x1fa, 0x200, 0x205, 
    0x20c, 0x211, 0x216, 0x21e, 0x223, 0x232, 0x23b, 0x23e, 0x242, 0x24b, 
    0x250, 0x255, 0x25d, 0x260, 0x264, 0x26b, 0x273, 0x276, 0x285, 0x28c, 
    0x290, 0x294, 0x29a, 0x29f, 0x2a2, 0x2a7, 0x2aa, 0x2ae, 0x2b3, 0x2b6, 
    0x2b8, 0x2bc, 0x2c2, 0x2c9, 0x2d1, 0x2d7, 0x2dd, 0x2e4, 0x2ea, 0x2ee, 
    0x2f6, 0x2fc, 0x300, 0x305, 0x308, 0x30c, 0x310, 0x313, 0x316, 0x31e, 
    0x326, 0x32d, 0x332, 0x334, 0x336, 0x33b, 0x343, 0x351, 0x356, 0x359, 
    0x364, 0x369, 0x36b, 0x36e, 0x372, 0x375, 0x37e, 0x383, 0x387, 0x38b, 
    0x392, 0x395, 0x39c, 0x3a0, 0x3a6, 0x3ab, 0x3ad, 0x3b1, 0x3b7, 0x3bb, 
    0x3c4, 0x3ca, 0x3cd, 0x3d2, 0x3d6, 0x3d9, 0x3dd, 0x3e3, 0x3eb, 0x3ef, 
    0x3f2, 0x3fa, 0x403, 0x409, 0x40b, 0x412, 0x417, 0x41a, 0x41d, 0x424, 
    0x42e, 0x433, 0x43a, 0x444, 0x44e, 0x454, 0x45b, 0x484, 0x48d, 0x48f, 
    0x493, 0x4b5, 0x4b8, 0x4c2, 0x4c5, 0x4cf, 0x4d2, 0x4de, 0x4f0, 0x4f7, 
    0x4f9, 0x502, 0x505, 0x508, 0x50c, 0x51a, 0x522, 0x52b, 0x534, 0x53d, 
    0x546, 0x552, 0x555, 0x562, 0x56b, 0x578, 0x57e, 0x597, 0x59a, 0x5a1, 
    0x5a6, 0x5af, 0x5b8, 0x5bb, 0x5be, 0x5c0, 0x5c8, 0x5cd, 0x5d2, 0x5d9, 
    0x5de, 0x5e3, 0x5ec, 0x60b, 0x60f, 0x613, 0x627, 0x62f, 0x647, 0x653, 
    0x65a, 0x663, 0x665, 0x66e, 0x675, 0x67f, 0x681, 0x688, 0x691, 0x693, 
    0x69a, 0x6a2, 0x6a9, 0x6b0, 0x6b7, 0x6bb, 0x6bd, 
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
