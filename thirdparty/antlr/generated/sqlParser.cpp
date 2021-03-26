
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
    setState(188);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ANTLR) {
      setState(186);
      match(sqlParser::ANTLR);
      setState(187);
      match(sqlParser::T__0);
    }
    setState(190);
    statementChoice();
    setState(191);
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
    setState(196);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(193);
      queryStatement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(194);
      explainStatement();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(195);
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
    setState(198);
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

tree::TerminalNode* sqlParser::ExplainStatementContext::ALL() {
  return getToken(sqlParser::ALL, 0);
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
    setState(203);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(200);
      queryHint();
      setState(205);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(206);
    match(sqlParser::EXPLAIN);
    setState(208);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ALL) {
      setState(207);
      dynamic_cast<ExplainStatementContext *>(_localctx)->all = match(sqlParser::ALL);
    }
    setState(210);
    statementChoice();
   
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

tree::TerminalNode* sqlParser::StatisticsStatementContext::ALL() {
  return getToken(sqlParser::ALL, 0);
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
  enterRule(_localctx, 8, sqlParser::RuleStatisticsStatement);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(215);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(212);
      queryHint();
      setState(217);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(218);
    _la = _input->LA(1);
    if (!(_la == sqlParser::STATS

    || _la == sqlParser::STATISTICS)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(220);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ALL) {
      setState(219);
      dynamic_cast<StatisticsStatementContext *>(_localctx)->all = match(sqlParser::ALL);
    }
    setState(222);
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
  enterRule(_localctx, 10, sqlParser::RuleQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(227);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::HINT_START) {
      setState(224);
      queryHint();
      setState(229);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(238);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      setState(230);
      selectQuery();
      break;
    }

    case 2: {
      setState(231);
      setQuery();
      break;
    }

    case 3: {
      setState(232);
      insertQuery();
      break;
    }

    case 4: {
      setState(233);
      deleteQuery();
      break;
    }

    case 5: {
      setState(234);
      updateQuery();
      break;
    }

    case 6: {
      setState(235);
      createQuery();
      break;
    }

    case 7: {
      setState(236);
      dropQuery();
      break;
    }

    case 8: {
      setState(237);
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
  enterRule(_localctx, 12, sqlParser::RuleQueryHint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(240);
    match(sqlParser::HINT_START);
    setState(242);
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
      | (1ULL << sqlParser::INDEX)
      | (1ULL << sqlParser::INSERT)
      | (1ULL << sqlParser::INTO)
      | (1ULL << sqlParser::IS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::JOIN - 64))
      | (1ULL << (sqlParser::KEY - 64))
      | (1ULL << (sqlParser::LEFT - 64))
      | (1ULL << (sqlParser::MONOTONIC - 64))
      | (1ULL << (sqlParser::NO - 64))
      | (1ULL << (sqlParser::NODE - 64))
      | (1ULL << (sqlParser::OFFSET - 64))
      | (1ULL << (sqlParser::OUTER - 64))
      | (1ULL << (sqlParser::OVERLAY - 64))
      | (1ULL << (sqlParser::PARTITION - 64))
      | (1ULL << (sqlParser::REFERENCE - 64))
      | (1ULL << (sqlParser::RIGHT - 64))
      | (1ULL << (sqlParser::SCHEMA - 64))
      | (1ULL << (sqlParser::SET - 64))
      | (1ULL << (sqlParser::STATS - 64))
      | (1ULL << (sqlParser::STATISTICS - 64))
      | (1ULL << (sqlParser::TABLE - 64))
      | (1ULL << (sqlParser::TYPE - 64))
      | (1ULL << (sqlParser::UNION - 64))
      | (1ULL << (sqlParser::UNIQUE - 64))
      | (1ULL << (sqlParser::UPDATE - 64))
      | (1ULL << (sqlParser::USING - 64))
      | (1ULL << (sqlParser::VALUES - 64))
      | (1ULL << (sqlParser::BOOLEAN - 64))
      | (1ULL << (sqlParser::INTEGER - 64))
      | (1ULL << (sqlParser::DECIMAL - 64))
      | (1ULL << (sqlParser::FLOAT - 64))
      | (1ULL << (sqlParser::TIME - 64))
      | (1ULL << (sqlParser::TIMESTAMP - 64))
      | (1ULL << (sqlParser::CHAR - 64))
      | (1ULL << (sqlParser::CHARACTER - 64))
      | (1ULL << (sqlParser::BINARY - 64))
      | (1ULL << (sqlParser::VARBINARY - 64))
      | (1ULL << (sqlParser::VARYING - 64))
      | (1ULL << (sqlParser::BIT - 64))
      | (1ULL << (sqlParser::BITS - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::BYTE - 128))
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
      setState(241);
      hint();
    }
    setState(248);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(244);
      match(sqlParser::T__2);
      setState(245);
      hint();
      setState(250);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(251);
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
  enterRule(_localctx, 14, sqlParser::RuleHint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(273);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(253);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(254);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(255);
      match(sqlParser::T__3);
      setState(256);
      hintValue();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(258);
      dynamic_cast<HintContext *>(_localctx)->name = hintName();
      setState(260);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__3) {
        setState(259);
        match(sqlParser::T__3);
      }
      setState(262);
      match(sqlParser::T__4);
      setState(263);
      hintValue();
      setState(268);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(264);
        match(sqlParser::T__2);
        setState(265);
        hintValue();
        setState(270);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(271);
      match(sqlParser::T__5);
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
  enterRule(_localctx, 16, sqlParser::RuleSelectQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(284);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(275);
      match(sqlParser::WITH);
      setState(276);
      withQuery();
      setState(281);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(277);
        match(sqlParser::T__2);
        setState(278);
        withQuery();
        setState(283);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(286);
    selectExpr();
    setState(290);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(287);
      match(sqlParser::ORDER);
      setState(288);
      match(sqlParser::BY);
      setState(289);
      dynamic_cast<SelectQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(298);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(292);
      match(sqlParser::LIMIT);
      setState(293);
      dynamic_cast<SelectQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(296);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(294);
        match(sqlParser::OFFSET);
        setState(295);
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
  enterRule(_localctx, 18, sqlParser::RuleSelectExpr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(327);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::T__6:
      case sqlParser::SELECT: {
        enterOuterAlt(_localctx, 1);
        setState(300);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__6 || _la == sqlParser::SELECT)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(303);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::ALL: {
            setState(301);
            match(sqlParser::ALL);
            break;
          }

          case sqlParser::DISTINCT: {
            setState(302);
            dynamic_cast<SelectExprContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
            break;
          }

          case sqlParser::T__4:
          case sqlParser::T__8:
          case sqlParser::T__9:
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
          case sqlParser::REFERENCE:
          case sqlParser::RIGHT:
          case sqlParser::SCHEMA:
          case sqlParser::SET:
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
        setState(305);
        selectList();
        setState(308);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
        case 1: {
          setState(306);
          match(sqlParser::FROM);
          setState(307);
          tableExpr(0);
          break;
        }

        }
        setState(312);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx)) {
        case 1: {
          setState(310);
          match(sqlParser::WHERE);
          setState(311);
          dynamic_cast<SelectExprContext *>(_localctx)->where = predicate();
          break;
        }

        }
        setState(317);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
        case 1: {
          setState(314);
          match(sqlParser::GROUP);
          setState(315);
          match(sqlParser::BY);
          setState(316);
          dynamic_cast<SelectExprContext *>(_localctx)->groupBy = groupByList();
          break;
        }

        }
        setState(321);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
        case 1: {
          setState(319);
          match(sqlParser::HAVING);
          setState(320);
          dynamic_cast<SelectExprContext *>(_localctx)->having = predicate();
          break;
        }

        }
        break;
      }

      case sqlParser::T__4: {
        enterOuterAlt(_localctx, 2);
        setState(323);
        match(sqlParser::T__4);
        setState(324);
        selectExpr();
        setState(325);
        match(sqlParser::T__5);
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
  enterRule(_localctx, 20, sqlParser::RuleSetQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(338);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(329);
      match(sqlParser::WITH);
      setState(330);
      withQuery();
      setState(335);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(331);
        match(sqlParser::T__2);
        setState(332);
        withQuery();
        setState(337);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(340);
    setExpr(0);
    setState(344);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ORDER) {
      setState(341);
      match(sqlParser::ORDER);
      setState(342);
      match(sqlParser::BY);
      setState(343);
      dynamic_cast<SetQueryContext *>(_localctx)->orderBy = orderByList();
    }
    setState(352);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::LIMIT) {
      setState(346);
      match(sqlParser::LIMIT);
      setState(347);
      dynamic_cast<SetQueryContext *>(_localctx)->limit = match(sqlParser::INTEGER_LITERAL);
      setState(350);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::OFFSET) {
        setState(348);
        match(sqlParser::OFFSET);
        setState(349);
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
  size_t startState = 22;
  enterRecursionRule(_localctx, 22, sqlParser::RuleSetExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(360);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
    case 1: {
      setState(355);
      selectExpr();
      break;
    }

    case 2: {
      setState(356);
      match(sqlParser::T__4);
      setState(357);
      setExpr(0);
      setState(358);
      match(sqlParser::T__5);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(374);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<SetExprContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleSetExpr);
        setState(362);

        if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
        setState(363);
        dynamic_cast<SetExprContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(((((_la - 48) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 48)) & ((1ULL << (sqlParser::EXCEPT - 48))
          | (1ULL << (sqlParser::INTERSECT - 48))
          | (1ULL << (sqlParser::UNION - 48)))) != 0))) {
          dynamic_cast<SetExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(366);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case sqlParser::DISTINCT: {
            setState(364);
            match(sqlParser::DISTINCT);
            break;
          }

          case sqlParser::ALL: {
            setState(365);
            dynamic_cast<SetExprContext *>(_localctx)->all = match(sqlParser::ALL);
            break;
          }

          case sqlParser::T__4:
          case sqlParser::T__6:
          case sqlParser::CORRESPONDING:
          case sqlParser::SELECT: {
            break;
          }

        default:
          break;
        }
        setState(369);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::CORRESPONDING) {
          setState(368);
          corrClause();
        }
        setState(371);
        setExpr(3); 
      }
      setState(376);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx);
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
  enterRule(_localctx, 24, sqlParser::RuleCorrClause);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(377);
    dynamic_cast<CorrClauseContext *>(_localctx)->corr = match(sqlParser::CORRESPONDING);
    setState(383);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::BY) {
      setState(378);
      match(sqlParser::BY);
      setState(379);
      match(sqlParser::T__4);
      setState(380);
      corrList();
      setState(381);
      match(sqlParser::T__5);
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
  enterRule(_localctx, 26, sqlParser::RuleWithQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(385);
    dynamic_cast<WithQueryContext *>(_localctx)->name = tableName();
    setState(397);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__4) {
      setState(386);
      match(sqlParser::T__4);
      setState(387);
      columnAlias();
      setState(392);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(388);
        match(sqlParser::T__2);
        setState(389);
        columnAlias();
        setState(394);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(395);
      match(sqlParser::T__5);
    }
    setState(399);
    match(sqlParser::AS);
    setState(400);
    match(sqlParser::T__4);
    setState(401);
    query();
    setState(402);
    match(sqlParser::T__5);
   
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
  size_t startState = 28;
  enterRecursionRule(_localctx, 28, sqlParser::RuleTableExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(430);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 42, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<FetchContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(405);
      dynamic_cast<FetchContext *>(_localctx)->ref = tableReference();
      setState(410);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
      case 1: {
        setState(407);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(406);
          match(sqlParser::AS);
        }
        setState(409);
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
      setState(412);
      match(sqlParser::T__4);
      setState(413);
      query();
      setState(414);
      match(sqlParser::T__5);
      setState(419);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
      case 1: {
        setState(416);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(415);
          match(sqlParser::AS);
        }
        setState(418);
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
      setState(421);
      match(sqlParser::T__4);
      setState(422);
      valuesExpr();
      setState(423);
      match(sqlParser::T__5);
      setState(428);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx)) {
      case 1: {
        setState(425);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(424);
          match(sqlParser::AS);
        }
        setState(427);
        dynamic_cast<ValuesSubqueryContext *>(_localctx)->as = tableAlias();
        break;
      }

      }
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(467);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 49, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(465);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 48, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<CrossJoinContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(432);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(436);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case sqlParser::T__2: {
              setState(433);
              match(sqlParser::T__2);
              break;
            }

            case sqlParser::CROSS: {
              setState(434);
              match(sqlParser::CROSS);
              setState(435);
              match(sqlParser::JOIN);
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(438);
          tableExpr(4);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<JoinOnContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(439);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(441);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 53) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 53)) & ((1ULL << (sqlParser::FULL - 53))
            | (1ULL << (sqlParser::INNER - 53))
            | (1ULL << (sqlParser::LEFT - 53))
            | (1ULL << (sqlParser::RIGHT - 53)))) != 0)) {
            setState(440);
            dynamic_cast<JoinOnContext *>(_localctx)->type = joinType();
          }
          setState(444);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::HINT_START) {
            setState(443);
            dynamic_cast<JoinOnContext *>(_localctx)->joint = joinHint();
          }
          setState(446);
          match(sqlParser::JOIN);
          setState(447);
          tableExpr(0);
          setState(448);
          match(sqlParser::ON);
          setState(449);
          joinPredicate();
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<JoinUsingContext>(_tracker.createInstance<TableExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleTableExpr);
          setState(451);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(453);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 53) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 53)) & ((1ULL << (sqlParser::FULL - 53))
            | (1ULL << (sqlParser::INNER - 53))
            | (1ULL << (sqlParser::LEFT - 53))
            | (1ULL << (sqlParser::RIGHT - 53)))) != 0)) {
            setState(452);
            dynamic_cast<JoinUsingContext *>(_localctx)->type = joinType();
          }
          setState(456);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::HINT_START) {
            setState(455);
            dynamic_cast<JoinUsingContext *>(_localctx)->joint = joinHint();
          }
          setState(458);
          match(sqlParser::JOIN);
          setState(459);
          tableExpr(0);
          setState(460);
          match(sqlParser::USING);
          setState(461);
          match(sqlParser::T__4);
          setState(462);
          joinUsingList();
          setState(463);
          match(sqlParser::T__5);
          break;
        }

        } 
      }
      setState(469);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 49, _ctx);
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
  enterRule(_localctx, 30, sqlParser::RuleTableReference);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(478);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx)) {
    case 1: {
      setState(473);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 50, _ctx)) {
      case 1: {
        setState(470);
        dynamic_cast<TableReferenceContext *>(_localctx)->catalog = catalogName();
        setState(471);
        match(sqlParser::T__7);
        break;
      }

      }
      setState(475);
      dynamic_cast<TableReferenceContext *>(_localctx)->schema = schemaName();
      setState(476);
      match(sqlParser::T__7);
      break;
    }

    }
    setState(480);
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
  enterRule(_localctx, 32, sqlParser::RuleJoinType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(487);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::INNER: {
        enterOuterAlt(_localctx, 1);
        setState(482);
        match(sqlParser::INNER);
        break;
      }

      case sqlParser::FULL:
      case sqlParser::LEFT:
      case sqlParser::RIGHT: {
        enterOuterAlt(_localctx, 2);
        setState(483);
        dynamic_cast<JoinTypeContext *>(_localctx)->outer = _input->LT(1);
        _la = _input->LA(1);
        if (!(((((_la - 53) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 53)) & ((1ULL << (sqlParser::FULL - 53))
          | (1ULL << (sqlParser::LEFT - 53))
          | (1ULL << (sqlParser::RIGHT - 53)))) != 0))) {
          dynamic_cast<JoinTypeContext *>(_localctx)->outer = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(485);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::OUTER) {
          setState(484);
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
  enterRule(_localctx, 34, sqlParser::RuleJoinHint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(489);
    match(sqlParser::HINT_START);
    setState(490);
    dynamic_cast<JoinHintContext *>(_localctx)->method = _input->LT(1);
    _la = _input->LA(1);
    if (!(((((_la - 168) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 168)) & ((1ULL << (sqlParser::BLOCK - 168))
      | (1ULL << (sqlParser::BLOOM - 168))
      | (1ULL << (sqlParser::HASH - 168))
      | (1ULL << (sqlParser::LOOP - 168)))) != 0))) {
      dynamic_cast<JoinHintContext *>(_localctx)->method = _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(505);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__3

    || _la == sqlParser::T__4) {
      setState(492);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__3) {
        setState(491);
        match(sqlParser::T__3);
      }
      setState(494);
      match(sqlParser::T__4);
      setState(495);
      dynamic_cast<JoinHintContext *>(_localctx)->values = hintValue();
      setState(500);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(496);
        match(sqlParser::T__2);
        setState(497);
        hintValue();
        setState(502);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(503);
      match(sqlParser::T__5);
    }
    setState(507);
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
  enterRule(_localctx, 36, sqlParser::RuleValuesExpr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(509);
    match(sqlParser::VALUES);
    setState(510);
    match(sqlParser::T__4);
    setState(511);
    valueList();
    setState(512);
    match(sqlParser::T__5);
    setState(520);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(513);
      match(sqlParser::T__2);
      setState(514);
      match(sqlParser::T__4);
      setState(515);
      valueList();
      setState(516);
      match(sqlParser::T__5);
      setState(522);
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
  enterRule(_localctx, 38, sqlParser::RuleInsertQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(532);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(523);
      match(sqlParser::WITH);
      setState(524);
      withQuery();
      setState(529);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(525);
        match(sqlParser::T__2);
        setState(526);
        withQuery();
        setState(531);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(534);
    match(sqlParser::INSERT);
    setState(536);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 60, _ctx)) {
    case 1: {
      setState(535);
      match(sqlParser::INTO);
      break;
    }

    }
    setState(538);
    dynamic_cast<InsertQueryContext *>(_localctx)->ref = tableReference();
    setState(550);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 62, _ctx)) {
    case 1: {
      setState(539);
      match(sqlParser::T__4);
      setState(540);
      columnName();
      setState(545);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(541);
        match(sqlParser::T__2);
        setState(542);
        columnName();
        setState(547);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(548);
      match(sqlParser::T__5);
      break;
    }

    }
    setState(555);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 63, _ctx)) {
    case 1: {
      setState(552);
      valuesExpr();
      break;
    }

    case 2: {
      setState(553);
      selectQuery();
      break;
    }

    case 3: {
      setState(554);
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
  enterRule(_localctx, 40, sqlParser::RuleDeleteQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(566);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(557);
      match(sqlParser::WITH);
      setState(558);
      withQuery();
      setState(563);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(559);
        match(sqlParser::T__2);
        setState(560);
        withQuery();
        setState(565);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(568);
    match(sqlParser::DELETE);
    setState(570);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::FROM) {
      setState(569);
      match(sqlParser::FROM);
    }
    setState(572);
    dynamic_cast<DeleteQueryContext *>(_localctx)->ref = tableReference();
    setState(577);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(573);
        match(sqlParser::WHERE);
        setState(574);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(575);
        match(sqlParser::FROM);
        setState(576);
        tableExpr(0);
        break;
      }

      case sqlParser::T__1:
      case sqlParser::T__5: {
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
  enterRule(_localctx, 42, sqlParser::RuleUpdateQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(588);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(579);
      match(sqlParser::WITH);
      setState(580);
      withQuery();
      setState(585);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(581);
        match(sqlParser::T__2);
        setState(582);
        withQuery();
        setState(587);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(590);
    match(sqlParser::UPDATE);
    setState(591);
    dynamic_cast<UpdateQueryContext *>(_localctx)->ref = tableReference();
    setState(592);
    match(sqlParser::SET);
    setState(593);
    columnName();
    setState(594);
    match(sqlParser::T__3);
    setState(595);
    expr(0);
    setState(603);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(596);
      match(sqlParser::T__2);
      setState(597);
      columnName();
      setState(598);
      match(sqlParser::T__3);
      setState(599);
      expr(0);
      setState(605);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(610);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::WHERE: {
        setState(606);
        match(sqlParser::WHERE);
        setState(607);
        predicate();
        break;
      }

      case sqlParser::FROM: {
        setState(608);
        match(sqlParser::FROM);
        setState(609);
        tableExpr(0);
        break;
      }

      case sqlParser::T__1:
      case sqlParser::T__5: {
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
  enterRule(_localctx, 44, sqlParser::RuleCreateQuery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(614);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 72, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(612);
      createTableQuery();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(613);
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
  enterRule(_localctx, 46, sqlParser::RuleCreateTableQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(616);
    match(sqlParser::CREATE);
    setState(618);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::REFERENCE) {
      setState(617);
      dynamic_cast<CreateTableQueryContext *>(_localctx)->reference = match(sqlParser::REFERENCE);
    }
    setState(620);
    match(sqlParser::TABLE);
    setState(624);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::IF) {
      setState(621);
      match(sqlParser::IF);
      setState(622);
      match(sqlParser::NOT);
      setState(623);
      match(sqlParser::EXISTS);
    }
    setState(626);
    dynamic_cast<CreateTableQueryContext *>(_localctx)->ref = tableReference();
    setState(654);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 82, _ctx)) {
    case 1: {
      setState(627);
      tableDefinition();
      break;
    }

    case 2: {
      setState(629);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx)) {
      case 1: {
        setState(628);
        tableDefinition();
        break;
      }

      }
      setState(632);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::AS) {
        setState(631);
        match(sqlParser::AS);
      }
      setState(634);
      asSubquery();
      setState(640);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::WITH) {
        setState(635);
        match(sqlParser::WITH);
        setState(637);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::NO) {
          setState(636);
          dynamic_cast<CreateTableQueryContext *>(_localctx)->noData = match(sqlParser::NO);
        }
        setState(639);
        match(sqlParser::DATA);
      }
      break;
    }

    case 3: {
      setState(642);
      columnNameList();
      setState(644);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::AS) {
        setState(643);
        match(sqlParser::AS);
      }
      setState(646);
      asSubquery();
      setState(652);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::WITH) {
        setState(647);
        match(sqlParser::WITH);
        setState(649);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::NO) {
          setState(648);
          dynamic_cast<CreateTableQueryContext *>(_localctx)->noData = match(sqlParser::NO);
        }
        setState(651);
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
  enterRule(_localctx, 48, sqlParser::RuleCreateIndexQuery);
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
    if (_la == sqlParser::UNIQUE) {
      setState(657);
      match(sqlParser::UNIQUE);
    }
    setState(660);
    _la = _input->LA(1);
    if (!(_la == sqlParser::INDEX

    || _la == sqlParser::KEY)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
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
    dynamic_cast<CreateIndexQueryContext *>(_localctx)->idxName = indexName();
    setState(667);
    match(sqlParser::ON);
    setState(668);
    dynamic_cast<CreateIndexQueryContext *>(_localctx)->tblName = tableReference();
    setState(671);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::USING) {
      setState(669);
      match(sqlParser::USING);
      setState(670);
      dynamic_cast<CreateIndexQueryContext *>(_localctx)->method = indexMethod();
    }
    setState(673);
    match(sqlParser::T__4);
    setState(674);
    expr(0);
    setState(679);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(675);
      match(sqlParser::T__2);
      setState(676);
      expr(0);
      setState(681);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(682);
    match(sqlParser::T__5);
    setState(685);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(683);
      match(sqlParser::WITH);
      setState(684);
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
  enterRule(_localctx, 50, sqlParser::RuleDropQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(704);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 90, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<DropQueryContext *>(_tracker.createInstance<sqlParser::DropTableContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(687);
      match(sqlParser::DROP);
      setState(688);
      match(sqlParser::TABLE);
      setState(691);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::IF) {
        setState(689);
        match(sqlParser::IF);
        setState(690);
        match(sqlParser::EXISTS);
      }
      setState(693);
      dynamic_cast<DropTableContext *>(_localctx)->name = tableReference();
      break;
    }

    case 2: {
      _localctx = dynamic_cast<DropQueryContext *>(_tracker.createInstance<sqlParser::DropIndexContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(694);
      match(sqlParser::DROP);
      setState(695);
      _la = _input->LA(1);
      if (!(_la == sqlParser::INDEX

      || _la == sqlParser::KEY)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(698);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::IF) {
        setState(696);
        match(sqlParser::IF);
        setState(697);
        match(sqlParser::EXISTS);
      }
      setState(700);
      dynamic_cast<DropIndexContext *>(_localctx)->idxName = indexName();
      setState(701);
      match(sqlParser::ON);
      setState(702);
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
  enterRule(_localctx, 52, sqlParser::RuleTruncateQuery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(706);
    match(sqlParser::TRUNCATE);
    setState(708);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 91, _ctx)) {
    case 1: {
      setState(707);
      match(sqlParser::TABLE);
      break;
    }

    }
    setState(710);
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
  enterRule(_localctx, 54, sqlParser::RuleTableDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(712);
    match(sqlParser::T__4);
    setState(716);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 92, _ctx)) {
    case 1: {
      setState(713);
      columnDefinition();
      break;
    }

    case 2: {
      setState(714);
      tableConstraint();
      break;
    }

    case 3: {
      setState(715);
      indexDefinition();
      break;
    }

    }
    setState(726);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(718);
      match(sqlParser::T__2);
      setState(722);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 93, _ctx)) {
      case 1: {
        setState(719);
        columnDefinition();
        break;
      }

      case 2: {
        setState(720);
        tableConstraint();
        break;
      }

      case 3: {
        setState(721);
        indexDefinition();
        break;
      }

      }
      setState(728);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(729);
    match(sqlParser::T__5);
    setState(731);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::REFERENCE) {
      setState(730);
      match(sqlParser::REFERENCE);
    }
    setState(734);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::PARTITION) {
      setState(733);
      tablePartitioning();
    }
    setState(738);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 97, _ctx)) {
    case 1: {
      setState(736);
      match(sqlParser::WITH);
      setState(737);
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
  enterRule(_localctx, 56, sqlParser::RuleTablePartitioning);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(740);
    match(sqlParser::PARTITION);
    setState(742);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::BY) {
      setState(741);
      match(sqlParser::BY);
    }
    setState(745);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::HASH

    || _la == sqlParser::VALUE) {
      setState(744);
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
    setState(748);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ON) {
      setState(747);
      match(sqlParser::ON);
    }
    setState(750);
    match(sqlParser::T__4);
    setState(751);
    expr(0);
    setState(756);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(752);
      match(sqlParser::T__2);
      setState(753);
      expr(0);
      setState(758);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(759);
    match(sqlParser::T__5);
   
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
  enterRule(_localctx, 58, sqlParser::RuleColumnDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(761);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->name = columnName();
    setState(762);
    dynamic_cast<ColumnDefinitionContext *>(_localctx)->type = dataType();
    setState(780);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 75) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 75)) & ((1ULL << (sqlParser::NOT - 75))
      | (1ULL << (sqlParser::SQL_NULL - 75))
      | (1ULL << (sqlParser::PRIMARY - 75))
      | (1ULL << (sqlParser::UNIQUE - 75)))) != 0)) {
      setState(778);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 105, _ctx)) {
      case 1: {
        setState(764);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::NOT) {
          setState(763);
          dynamic_cast<ColumnDefinitionContext *>(_localctx)->notNullable = match(sqlParser::NOT);
        }
        setState(766);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->nullable = match(sqlParser::SQL_NULL);
        break;
      }

      case 2: {
        setState(767);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->primary = match(sqlParser::PRIMARY);
        setState(768);
        match(sqlParser::KEY);
        break;
      }

      case 3: {
        setState(769);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
        setState(771);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::KEY) {
          setState(770);
          match(sqlParser::KEY);
        }
        break;
      }

      case 4: {
        setState(773);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
        setState(774);
        dynamic_cast<ColumnDefinitionContext *>(_localctx)->mono = match(sqlParser::MONOTONIC);
        setState(776);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::KEY) {
          setState(775);
          match(sqlParser::KEY);
        }
        break;
      }

      }
      setState(782);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(785);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(783);
      match(sqlParser::WITH);
      setState(784);
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
  enterRule(_localctx, 60, sqlParser::RuleColumnNameList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(787);
    match(sqlParser::T__4);
    setState(788);
    columnName();
    setState(793);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(789);
      match(sqlParser::T__2);
      setState(790);
      columnName();
      setState(795);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(796);
    match(sqlParser::T__5);
   
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
  enterRule(_localctx, 62, sqlParser::RuleTableConstraint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(833);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::PRIMARY: {
        enterOuterAlt(_localctx, 1);
        setState(798);
        dynamic_cast<TableConstraintContext *>(_localctx)->primary = match(sqlParser::PRIMARY);
        setState(799);
        match(sqlParser::KEY);
        setState(812);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 110, _ctx)) {
        case 1: {
          setState(800);
          expr(0);
          break;
        }

        case 2: {
          setState(801);
          match(sqlParser::T__4);
          setState(802);
          expr(0);
          setState(807);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::T__2) {
            setState(803);
            match(sqlParser::T__2);
            setState(804);
            expr(0);
            setState(809);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(810);
          match(sqlParser::T__5);
          break;
        }

        }
        break;
      }

      case sqlParser::MONOTONIC:
      case sqlParser::UNIQUE: {
        enterOuterAlt(_localctx, 2);
        setState(815);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::UNIQUE) {
          setState(814);
          dynamic_cast<TableConstraintContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
        }
        setState(817);
        dynamic_cast<TableConstraintContext *>(_localctx)->mono = match(sqlParser::MONOTONIC);
        setState(818);
        match(sqlParser::KEY);
        setState(831);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 113, _ctx)) {
        case 1: {
          setState(819);
          expr(0);
          break;
        }

        case 2: {
          setState(820);
          match(sqlParser::T__4);
          setState(821);
          expr(0);
          setState(826);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == sqlParser::T__2) {
            setState(822);
            match(sqlParser::T__2);
            setState(823);
            expr(0);
            setState(828);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
          setState(829);
          match(sqlParser::T__5);
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
  enterRule(_localctx, 64, sqlParser::RuleIndexDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(836);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::UNIQUE) {
      setState(835);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->unique = match(sqlParser::UNIQUE);
    }
    setState(838);
    _la = _input->LA(1);
    if (!(_la == sqlParser::INDEX

    || _la == sqlParser::KEY)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(840);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 116, _ctx)) {
    case 1: {
      setState(839);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->name = indexName();
      break;
    }

    }
    setState(843);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ON) {
      setState(842);
      match(sqlParser::ON);
    }
    setState(857);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 119, _ctx)) {
    case 1: {
      setState(845);
      expr(0);
      break;
    }

    case 2: {
      setState(846);
      match(sqlParser::T__4);
      setState(847);
      expr(0);
      setState(852);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::T__2) {
        setState(848);
        match(sqlParser::T__2);
        setState(849);
        expr(0);
        setState(854);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(855);
      match(sqlParser::T__5);
      break;
    }

    }
    setState(861);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::USING) {
      setState(859);
      match(sqlParser::USING);
      setState(860);
      dynamic_cast<IndexDefinitionContext *>(_localctx)->method = indexMethod();
    }
    setState(865);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::WITH) {
      setState(863);
      match(sqlParser::WITH);
      setState(864);
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
  enterRule(_localctx, 66, sqlParser::RuleIndexMethod);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(867);
    _la = _input->LA(1);
    if (!(((((_la - 170) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 170)) & ((1ULL << (sqlParser::BTREE - 170))
      | (1ULL << (sqlParser::HASH - 170))
      | (1ULL << (sqlParser::SKIPLIST - 170))
      | (1ULL << (sqlParser::TREE - 170)))) != 0))) {
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
  enterRule(_localctx, 68, sqlParser::RuleDataType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(875);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 123, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(869);
      sqlDataType();
      setState(872);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::USING) {
        setState(870);
        match(sqlParser::USING);
        setState(871);
        className();
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(874);
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
  enterRule(_localctx, 70, sqlParser::RuleSqlDataType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(968);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::BOOLEAN: {
        enterOuterAlt(_localctx, 1);
        setState(877);
        match(sqlParser::BOOLEAN);
        break;
      }

      case sqlParser::INTEGER: {
        enterOuterAlt(_localctx, 2);
        setState(878);
        match(sqlParser::INTEGER);
        setState(886);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__4) {
          setState(879);
          match(sqlParser::T__4);
          setState(880);
          precision();
          setState(882);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 127) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 127)) & ((1ULL << (sqlParser::BITS - 127))
            | (1ULL << (sqlParser::BYTES - 127))
            | (1ULL << (sqlParser::DIGITS - 127)))) != 0)) {
            setState(881);
            _la = _input->LA(1);
            if (!(((((_la - 127) & ~ 0x3fULL) == 0) &&
              ((1ULL << (_la - 127)) & ((1ULL << (sqlParser::BITS - 127))
              | (1ULL << (sqlParser::BYTES - 127))
              | (1ULL << (sqlParser::DIGITS - 127)))) != 0))) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
          }
          setState(884);
          match(sqlParser::T__5);
        }
        break;
      }

      case sqlParser::DECIMAL: {
        enterOuterAlt(_localctx, 3);
        setState(888);
        match(sqlParser::DECIMAL);
        setState(903);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__4) {
          setState(889);
          match(sqlParser::T__4);
          setState(890);
          precision();
          setState(892);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::DIGITS) {
            setState(891);
            match(sqlParser::DIGITS);
          }
          setState(899);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__2) {
            setState(894);
            match(sqlParser::T__2);
            setState(895);
            scale();
            setState(897);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == sqlParser::DIGITS) {
              setState(896);
              match(sqlParser::DIGITS);
            }
          }
          setState(901);
          match(sqlParser::T__5);
        }
        break;
      }

      case sqlParser::FLOAT: {
        enterOuterAlt(_localctx, 4);
        setState(905);
        match(sqlParser::FLOAT);
        setState(913);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__4) {
          setState(906);
          match(sqlParser::T__4);
          setState(907);
          precision();
          setState(909);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 127) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 127)) & ((1ULL << (sqlParser::BITS - 127))
            | (1ULL << (sqlParser::BYTES - 127))
            | (1ULL << (sqlParser::DIGITS - 127)))) != 0)) {
            setState(908);
            _la = _input->LA(1);
            if (!(((((_la - 127) & ~ 0x3fULL) == 0) &&
              ((1ULL << (_la - 127)) & ((1ULL << (sqlParser::BITS - 127))
              | (1ULL << (sqlParser::BYTES - 127))
              | (1ULL << (sqlParser::DIGITS - 127)))) != 0))) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
          }
          setState(911);
          match(sqlParser::T__5);
        }
        break;
      }

      case sqlParser::DATE: {
        enterOuterAlt(_localctx, 5);
        setState(915);
        match(sqlParser::DATE);
        break;
      }

      case sqlParser::TIME: {
        enterOuterAlt(_localctx, 6);
        setState(916);
        match(sqlParser::TIME);
        break;
      }

      case sqlParser::TIMESTAMP: {
        enterOuterAlt(_localctx, 7);
        setState(917);
        match(sqlParser::TIMESTAMP);
        break;
      }

      case sqlParser::CHAR:
      case sqlParser::CHARACTER:
      case sqlParser::VARCHAR:
      case sqlParser::TEXT: {
        enterOuterAlt(_localctx, 8);
        setState(943);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 137, _ctx)) {
        case 1: {
          setState(918);
          _la = _input->LA(1);
          if (!(_la == sqlParser::CHAR

          || _la == sqlParser::CHARACTER)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(919);
          match(sqlParser::T__4);
          setState(920);
          size();
          setState(922);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 118) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 118)) & ((1ULL << (sqlParser::CHAR - 118))
            | (1ULL << (sqlParser::BYTE - 118))
            | (1ULL << (sqlParser::BYTES - 118))
            | (1ULL << (sqlParser::CHARS - 118)))) != 0)) {
            setState(921);
            _la = _input->LA(1);
            if (!(((((_la - 118) & ~ 0x3fULL) == 0) &&
              ((1ULL << (_la - 118)) & ((1ULL << (sqlParser::CHAR - 118))
              | (1ULL << (sqlParser::BYTE - 118))
              | (1ULL << (sqlParser::BYTES - 118))
              | (1ULL << (sqlParser::CHARS - 118)))) != 0))) {
            _errHandler->recoverInline(this);
            }
            else {
              _errHandler->reportMatch(this);
              consume();
            }
          }
          setState(924);
          match(sqlParser::T__5);
          break;
        }

        case 2: {
          setState(931);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case sqlParser::CHAR:
            case sqlParser::CHARACTER: {
              setState(926);
              _la = _input->LA(1);
              if (!(_la == sqlParser::CHAR

              || _la == sqlParser::CHARACTER)) {
              _errHandler->recoverInline(this);
              }
              else {
                _errHandler->reportMatch(this);
                consume();
              }
              setState(928);
              _errHandler->sync(this);

              _la = _input->LA(1);
              if (_la == sqlParser::VARYING) {
                setState(927);
                match(sqlParser::VARYING);
              }
              break;
            }

            case sqlParser::VARCHAR: {
              setState(930);
              match(sqlParser::VARCHAR);
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(940);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::T__4) {
            setState(933);
            match(sqlParser::T__4);
            setState(934);
            size();
            setState(936);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (((((_la - 118) & ~ 0x3fULL) == 0) &&
              ((1ULL << (_la - 118)) & ((1ULL << (sqlParser::CHAR - 118))
              | (1ULL << (sqlParser::BYTE - 118))
              | (1ULL << (sqlParser::BYTES - 118))
              | (1ULL << (sqlParser::CHARS - 118)))) != 0)) {
              setState(935);
              _la = _input->LA(1);
              if (!(((((_la - 118) & ~ 0x3fULL) == 0) &&
                ((1ULL << (_la - 118)) & ((1ULL << (sqlParser::CHAR - 118))
                | (1ULL << (sqlParser::BYTE - 118))
                | (1ULL << (sqlParser::BYTES - 118))
                | (1ULL << (sqlParser::CHARS - 118)))) != 0))) {
              _errHandler->recoverInline(this);
              }
              else {
                _errHandler->reportMatch(this);
                consume();
              }
            }
            setState(938);
            match(sqlParser::T__5);
          }
          break;
        }

        case 3: {
          setState(942);
          match(sqlParser::TEXT);
          break;
        }

        }
        setState(947);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::COLLATE) {
          setState(945);
          match(sqlParser::COLLATE);
          setState(946);
          dynamic_cast<SqlDataTypeContext *>(_localctx)->collate = collationName();
        }
        break;
      }

      case sqlParser::BINARY: {
        enterOuterAlt(_localctx, 9);
        setState(949);
        match(sqlParser::BINARY);

        setState(950);
        match(sqlParser::T__4);
        setState(951);
        size();
        setState(953);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::BYTE

        || _la == sqlParser::BYTES) {
          setState(952);
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
        setState(955);
        match(sqlParser::T__5);
        break;
      }

      case sqlParser::VARBINARY: {
        enterOuterAlt(_localctx, 10);
        setState(957);
        match(sqlParser::VARBINARY);
        setState(965);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__4) {
          setState(958);
          match(sqlParser::T__4);
          setState(959);
          size();
          setState(961);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::BYTE

          || _la == sqlParser::BYTES) {
            setState(960);
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
          setState(963);
          match(sqlParser::T__5);
        }
        break;
      }

      case sqlParser::BLOB: {
        enterOuterAlt(_localctx, 11);
        setState(967);
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
  enterRule(_localctx, 72, sqlParser::RuleAsSubquery);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(970);
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
  enterRule(_localctx, 74, sqlParser::RuleSelectOrValues);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    setState(993);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 146, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(972);
      match(sqlParser::T__4);
      setState(976);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 143, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(973);
          queryHint(); 
        }
        setState(978);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 143, _ctx);
      }
      setState(979);
      selectOrValues();
      setState(980);
      match(sqlParser::T__5);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(985);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == sqlParser::HINT_START) {
        setState(982);
        queryHint();
        setState(987);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(991);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 145, _ctx)) {
      case 1: {
        setState(988);
        selectQuery();
        break;
      }

      case 2: {
        setState(989);
        valuesExpr();
        break;
      }

      case 3: {
        setState(990);
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
  enterRule(_localctx, 76, sqlParser::RuleSelectList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(995);
    selectListItem();
    setState(1000);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 147, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(996);
        match(sqlParser::T__2);
        setState(997);
        selectListItem(); 
      }
      setState(1002);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 147, _ctx);
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
  enterRule(_localctx, 78, sqlParser::RuleSelectListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1011);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 150, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1003);
      expr(0);
      setState(1008);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 149, _ctx)) {
      case 1: {
        setState(1005);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::AS) {
          setState(1004);
          match(sqlParser::AS);
        }
        setState(1007);
        dynamic_cast<SelectListItemContext *>(_localctx)->as = identifier();
        break;
      }

      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1010);
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
  enterRule(_localctx, 80, sqlParser::RuleGroupByList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1013);
    groupByListItem();
    setState(1018);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 151, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1014);
        match(sqlParser::T__2);
        setState(1015);
        groupByListItem(); 
      }
      setState(1020);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 151, _ctx);
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
  enterRule(_localctx, 82, sqlParser::RuleGroupByListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1021);
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
  enterRule(_localctx, 84, sqlParser::RuleOrderByList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1023);
    orderByListItem();
    setState(1028);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1024);
      match(sqlParser::T__2);
      setState(1025);
      orderByListItem();
      setState(1030);
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
  enterRule(_localctx, 86, sqlParser::RuleOrderByListItem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1031);
    expr(0);
    setState(1033);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::ASC

    || _la == sqlParser::DESC) {
      setState(1032);
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
  enterRule(_localctx, 88, sqlParser::RuleJoinUsingList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1035);
    joinUsingListItem();
    setState(1040);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1036);
      match(sqlParser::T__2);
      setState(1037);
      joinUsingListItem();
      setState(1042);
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
  enterRule(_localctx, 90, sqlParser::RuleJoinUsingListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1043);
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
  enterRule(_localctx, 92, sqlParser::RuleCorrList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1045);
    corrListItem();
    setState(1050);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1046);
      match(sqlParser::T__2);
      setState(1047);
      corrListItem();
      setState(1052);
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
  enterRule(_localctx, 94, sqlParser::RuleCorrListItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1053);
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
  enterRule(_localctx, 96, sqlParser::RulePropertyList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1055);
    property();
    setState(1060);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 156, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1056);
        match(sqlParser::T__2);
        setState(1057);
        property(); 
      }
      setState(1062);
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
  enterRule(_localctx, 98, sqlParser::RuleProperty);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1063);
    dynamic_cast<PropertyContext *>(_localctx)->name = propertyName();
    setState(1066);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == sqlParser::T__3) {
      setState(1064);
      match(sqlParser::T__3);
      setState(1065);
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
  enterRule(_localctx, 100, sqlParser::RuleValueList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1068);
    expr(0);
    setState(1073);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == sqlParser::T__2) {
      setState(1069);
      match(sqlParser::T__2);
      setState(1070);
      expr(0);
      setState(1075);
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
  enterRule(_localctx, 102, sqlParser::RulePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1076);
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
  enterRule(_localctx, 104, sqlParser::RuleJoinPredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1078);
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
  enterRule(_localctx, 106, sqlParser::RuleCasePredicate);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1080);
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
  size_t startState = 108;
  enterRecursionRule(_localctx, 108, sqlParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1345);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 186, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<LiteralValContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(1083);
      literal();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<VarRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1084);
      variableName();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ColRefContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1085);
      columnRef();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<InlineCastContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1086);
      match(sqlParser::T__4);
      setState(1087);
      dataType();
      setState(1088);
      match(sqlParser::T__5);
      setState(1089);
      expr(39);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<UnaryOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1091);
      dynamic_cast<UnaryOpContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__8

      || _la == sqlParser::T__9)) {
        dynamic_cast<UnaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1092);
      expr(38);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<CastOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1093);
      match(sqlParser::CAST);
      setState(1094);
      match(sqlParser::T__4);
      setState(1095);
      expr(0);
      setState(1096);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2

      || _la == sqlParser::AS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1097);
      dataType();
      setState(1098);
      match(sqlParser::T__5);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<TypeOfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1100);
      match(sqlParser::TYPEOF);
      setState(1101);
      match(sqlParser::T__4);
      setState(1102);
      expr(0);
      setState(1103);
      match(sqlParser::T__5);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<CaseOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1105);
      match(sqlParser::CASE);
      setState(1125);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::T__4:
        case sqlParser::T__8:
        case sqlParser::T__9:
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
        case sqlParser::REFERENCE:
        case sqlParser::RIGHT:
        case sqlParser::SCHEMA:
        case sqlParser::SET:
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
          setState(1106);
          dynamic_cast<CaseOpContext *>(_localctx)->caseExpr = expr(0);
          setState(1112); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1107);
            match(sqlParser::WHEN);
            setState(1108);
            expr(0);
            setState(1109);
            match(sqlParser::THEN);
            setState(1110);
            expr(0);
            setState(1114); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

        case sqlParser::WHEN: {
          setState(1121); 
          _errHandler->sync(this);
          _la = _input->LA(1);
          do {
            setState(1116);
            match(sqlParser::WHEN);
            setState(1117);
            casePredicate();
            setState(1118);
            match(sqlParser::THEN);
            setState(1119);
            expr(0);
            setState(1123); 
            _errHandler->sync(this);
            _la = _input->LA(1);
          } while (_la == sqlParser::WHEN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(1129);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ELSE) {
        setState(1127);
        match(sqlParser::ELSE);
        setState(1128);
        expr(0);
      }
      setState(1131);
      match(sqlParser::END);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<IfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1133);
      match(sqlParser::IF);
      setState(1134);
      match(sqlParser::T__4);
      setState(1135);
      casePredicate();
      setState(1136);
      match(sqlParser::T__2);
      setState(1137);
      expr(0);
      setState(1138);
      match(sqlParser::T__2);
      setState(1139);
      expr(0);
      setState(1140);
      match(sqlParser::T__5);
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<IfNullOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1142);
      match(sqlParser::IFNULL);
      setState(1143);
      match(sqlParser::T__4);
      setState(1144);
      expr(0);
      setState(1145);
      match(sqlParser::T__2);
      setState(1146);
      expr(0);
      setState(1147);
      match(sqlParser::T__5);
      break;
    }

    case 11: {
      _localctx = _tracker.createInstance<NullIfOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1149);
      match(sqlParser::NULLIF);
      setState(1150);
      match(sqlParser::T__4);
      setState(1151);
      expr(0);
      setState(1152);
      match(sqlParser::T__2);
      setState(1153);
      expr(0);
      setState(1154);
      match(sqlParser::T__5);
      break;
    }

    case 12: {
      _localctx = _tracker.createInstance<CoalesceOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1156);
      match(sqlParser::COALESCE);
      setState(1157);
      match(sqlParser::T__4);
      setState(1166);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__4)
        | (1ULL << sqlParser::T__8)
        | (1ULL << sqlParser::T__9)
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
        | (1ULL << sqlParser::INDEX)
        | (1ULL << sqlParser::INSERT)
        | (1ULL << sqlParser::INTO)
        | (1ULL << sqlParser::IS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::JOIN - 64))
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
        | (1ULL << (sqlParser::REFERENCE - 64))
        | (1ULL << (sqlParser::RIGHT - 64))
        | (1ULL << (sqlParser::SCHEMA - 64))
        | (1ULL << (sqlParser::SET - 64))
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
        | (1ULL << (sqlParser::BOOLEAN - 64))
        | (1ULL << (sqlParser::INTEGER - 64))
        | (1ULL << (sqlParser::DECIMAL - 64))
        | (1ULL << (sqlParser::FLOAT - 64))
        | (1ULL << (sqlParser::TIME - 64))
        | (1ULL << (sqlParser::TIMESTAMP - 64))
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::CHAR - 64))
        | (1ULL << (sqlParser::CHARACTER - 64))
        | (1ULL << (sqlParser::BINARY - 64))
        | (1ULL << (sqlParser::VARBINARY - 64))
        | (1ULL << (sqlParser::VARYING - 64))
        | (1ULL << (sqlParser::BIT - 64))
        | (1ULL << (sqlParser::BITS - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::BYTE - 128))
        | (1ULL << (sqlParser::BYTES - 128))
        | (1ULL << (sqlParser::CHARS - 128))
        | (1ULL << (sqlParser::DIGITS - 128))
        | (1ULL << (sqlParser::CASE - 128))
        | (1ULL << (sqlParser::CAST - 128))
        | (1ULL << (sqlParser::COALESCE - 128))
        | (1ULL << (sqlParser::CONCAT - 128))
        | (1ULL << (sqlParser::COUNT - 128))
        | (1ULL << (sqlParser::DECODE - 128))
        | (1ULL << (sqlParser::EXTRACT - 128))
        | (1ULL << (sqlParser::EXISTS - 128))
        | (1ULL << (sqlParser::IF - 128))
        | (1ULL << (sqlParser::IFNULL - 128))
        | (1ULL << (sqlParser::NULLIF - 128))
        | (1ULL << (sqlParser::POSITION - 128))
        | (1ULL << (sqlParser::SUBSTR - 128))
        | (1ULL << (sqlParser::SUBSTRING - 128))
        | (1ULL << (sqlParser::TRIM - 128))
        | (1ULL << (sqlParser::TYPEOF - 128))
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
        | (1ULL << (sqlParser::IDENTIFIER - 128))
        | (1ULL << (sqlParser::STRING_LITERAL - 128))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 128))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 128))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 128)))) != 0)) {
        setState(1158);
        expr(0);
        setState(1163);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1159);
          match(sqlParser::T__2);
          setState(1160);
          expr(0);
          setState(1165);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1168);
      match(sqlParser::T__5);
      break;
    }

    case 13: {
      _localctx = _tracker.createInstance<DecodeOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1169);
      match(sqlParser::DECODE);
      setState(1170);
      match(sqlParser::T__4);
      setState(1179);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__4)
        | (1ULL << sqlParser::T__8)
        | (1ULL << sqlParser::T__9)
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
        | (1ULL << sqlParser::INDEX)
        | (1ULL << sqlParser::INSERT)
        | (1ULL << sqlParser::INTO)
        | (1ULL << sqlParser::IS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::JOIN - 64))
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
        | (1ULL << (sqlParser::REFERENCE - 64))
        | (1ULL << (sqlParser::RIGHT - 64))
        | (1ULL << (sqlParser::SCHEMA - 64))
        | (1ULL << (sqlParser::SET - 64))
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
        | (1ULL << (sqlParser::BOOLEAN - 64))
        | (1ULL << (sqlParser::INTEGER - 64))
        | (1ULL << (sqlParser::DECIMAL - 64))
        | (1ULL << (sqlParser::FLOAT - 64))
        | (1ULL << (sqlParser::TIME - 64))
        | (1ULL << (sqlParser::TIMESTAMP - 64))
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::CHAR - 64))
        | (1ULL << (sqlParser::CHARACTER - 64))
        | (1ULL << (sqlParser::BINARY - 64))
        | (1ULL << (sqlParser::VARBINARY - 64))
        | (1ULL << (sqlParser::VARYING - 64))
        | (1ULL << (sqlParser::BIT - 64))
        | (1ULL << (sqlParser::BITS - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::BYTE - 128))
        | (1ULL << (sqlParser::BYTES - 128))
        | (1ULL << (sqlParser::CHARS - 128))
        | (1ULL << (sqlParser::DIGITS - 128))
        | (1ULL << (sqlParser::CASE - 128))
        | (1ULL << (sqlParser::CAST - 128))
        | (1ULL << (sqlParser::COALESCE - 128))
        | (1ULL << (sqlParser::CONCAT - 128))
        | (1ULL << (sqlParser::COUNT - 128))
        | (1ULL << (sqlParser::DECODE - 128))
        | (1ULL << (sqlParser::EXTRACT - 128))
        | (1ULL << (sqlParser::EXISTS - 128))
        | (1ULL << (sqlParser::IF - 128))
        | (1ULL << (sqlParser::IFNULL - 128))
        | (1ULL << (sqlParser::NULLIF - 128))
        | (1ULL << (sqlParser::POSITION - 128))
        | (1ULL << (sqlParser::SUBSTR - 128))
        | (1ULL << (sqlParser::SUBSTRING - 128))
        | (1ULL << (sqlParser::TRIM - 128))
        | (1ULL << (sqlParser::TYPEOF - 128))
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
        | (1ULL << (sqlParser::IDENTIFIER - 128))
        | (1ULL << (sqlParser::STRING_LITERAL - 128))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 128))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 128))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 128)))) != 0)) {
        setState(1171);
        expr(0);
        setState(1176);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1172);
          match(sqlParser::T__2);
          setState(1173);
          expr(0);
          setState(1178);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1181);
      match(sqlParser::T__5);
      break;
    }

    case 14: {
      _localctx = _tracker.createInstance<ConcatOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1182);
      match(sqlParser::CONCAT);
      setState(1183);
      match(sqlParser::T__4);
      setState(1192);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__4)
        | (1ULL << sqlParser::T__8)
        | (1ULL << sqlParser::T__9)
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
        | (1ULL << sqlParser::INDEX)
        | (1ULL << sqlParser::INSERT)
        | (1ULL << sqlParser::INTO)
        | (1ULL << sqlParser::IS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::JOIN - 64))
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
        | (1ULL << (sqlParser::REFERENCE - 64))
        | (1ULL << (sqlParser::RIGHT - 64))
        | (1ULL << (sqlParser::SCHEMA - 64))
        | (1ULL << (sqlParser::SET - 64))
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
        | (1ULL << (sqlParser::BOOLEAN - 64))
        | (1ULL << (sqlParser::INTEGER - 64))
        | (1ULL << (sqlParser::DECIMAL - 64))
        | (1ULL << (sqlParser::FLOAT - 64))
        | (1ULL << (sqlParser::TIME - 64))
        | (1ULL << (sqlParser::TIMESTAMP - 64))
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::CHAR - 64))
        | (1ULL << (sqlParser::CHARACTER - 64))
        | (1ULL << (sqlParser::BINARY - 64))
        | (1ULL << (sqlParser::VARBINARY - 64))
        | (1ULL << (sqlParser::VARYING - 64))
        | (1ULL << (sqlParser::BIT - 64))
        | (1ULL << (sqlParser::BITS - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::BYTE - 128))
        | (1ULL << (sqlParser::BYTES - 128))
        | (1ULL << (sqlParser::CHARS - 128))
        | (1ULL << (sqlParser::DIGITS - 128))
        | (1ULL << (sqlParser::CASE - 128))
        | (1ULL << (sqlParser::CAST - 128))
        | (1ULL << (sqlParser::COALESCE - 128))
        | (1ULL << (sqlParser::CONCAT - 128))
        | (1ULL << (sqlParser::COUNT - 128))
        | (1ULL << (sqlParser::DECODE - 128))
        | (1ULL << (sqlParser::EXTRACT - 128))
        | (1ULL << (sqlParser::EXISTS - 128))
        | (1ULL << (sqlParser::IF - 128))
        | (1ULL << (sqlParser::IFNULL - 128))
        | (1ULL << (sqlParser::NULLIF - 128))
        | (1ULL << (sqlParser::POSITION - 128))
        | (1ULL << (sqlParser::SUBSTR - 128))
        | (1ULL << (sqlParser::SUBSTRING - 128))
        | (1ULL << (sqlParser::TRIM - 128))
        | (1ULL << (sqlParser::TYPEOF - 128))
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
        | (1ULL << (sqlParser::IDENTIFIER - 128))
        | (1ULL << (sqlParser::STRING_LITERAL - 128))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 128))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 128))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 128)))) != 0)) {
        setState(1184);
        expr(0);
        setState(1189);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1185);
          match(sqlParser::T__2);
          setState(1186);
          expr(0);
          setState(1191);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1194);
      match(sqlParser::T__5);
      break;
    }

    case 15: {
      _localctx = _tracker.createInstance<OverlayFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1195);
      match(sqlParser::OVERLAY);
      setState(1196);
      match(sqlParser::T__4);
      setState(1197);
      expr(0);
      setState(1198);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2 || _la == sqlParser::PLACING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1199);
      expr(0);
      setState(1200);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2

      || _la == sqlParser::FROM)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1201);
      expr(0);
      setState(1204);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__2

      || _la == sqlParser::FOR) {
        setState(1202);
        _la = _input->LA(1);
        if (!(_la == sqlParser::T__2

        || _la == sqlParser::FOR)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(1203);
        expr(0);
      }
      setState(1206);
      match(sqlParser::T__5);
      break;
    }

    case 16: {
      _localctx = _tracker.createInstance<PositionFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1208);
      match(sqlParser::POSITION);
      setState(1209);
      match(sqlParser::T__4);
      setState(1210);
      expr(0);
      setState(1211);
      _la = _input->LA(1);
      if (!(_la == sqlParser::T__2

      || _la == sqlParser::IN)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1212);
      expr(0);
      setState(1213);
      match(sqlParser::T__5);
      break;
    }

    case 17: {
      _localctx = _tracker.createInstance<SubStringFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1215);
      _la = _input->LA(1);
      if (!(_la == sqlParser::SUBSTR

      || _la == sqlParser::SUBSTRING)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1216);
      match(sqlParser::T__4);
      setState(1231);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 172, _ctx)) {
      case 1: {
        setState(1217);
        expr(0);
        setState(1218);
        match(sqlParser::T__2);
        setState(1219);
        expr(0);
        setState(1222);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__2) {
          setState(1220);
          match(sqlParser::T__2);
          setState(1221);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      case 2: {
        setState(1224);
        expr(0);
        setState(1225);
        match(sqlParser::FROM);
        setState(1226);
        expr(0);
        setState(1229);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::FOR) {
          setState(1227);
          match(sqlParser::FOR);
          setState(1228);
          dynamic_cast<SubStringFuncContext *>(_localctx)->length = expr(0);
        }
        break;
      }

      }
      setState(1233);
      match(sqlParser::T__5);
      break;
    }

    case 18: {
      _localctx = _tracker.createInstance<TrimFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1235);
      match(sqlParser::TRIM);
      setState(1236);
      match(sqlParser::T__4);
      setState(1250);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 176, _ctx)) {
      case 1: {
        setState(1237);
        expr(0);
        setState(1240);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__2) {
          setState(1238);
          match(sqlParser::T__2);
          setState(1239);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        break;
      }

      case 2: {
        setState(1243);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 174, _ctx)) {
        case 1: {
          setState(1242);
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
        setState(1246);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << sqlParser::T__4)
          | (1ULL << sqlParser::T__8)
          | (1ULL << sqlParser::T__9)
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
          | (1ULL << sqlParser::INDEX)
          | (1ULL << sqlParser::INSERT)
          | (1ULL << sqlParser::INTO)
          | (1ULL << sqlParser::IS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::JOIN - 64))
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
          | (1ULL << (sqlParser::REFERENCE - 64))
          | (1ULL << (sqlParser::RIGHT - 64))
          | (1ULL << (sqlParser::SCHEMA - 64))
          | (1ULL << (sqlParser::SET - 64))
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
          | (1ULL << (sqlParser::BOOLEAN - 64))
          | (1ULL << (sqlParser::INTEGER - 64))
          | (1ULL << (sqlParser::DECIMAL - 64))
          | (1ULL << (sqlParser::FLOAT - 64))
          | (1ULL << (sqlParser::TIME - 64))
          | (1ULL << (sqlParser::TIMESTAMP - 64))
          | (1ULL << (sqlParser::INTERVAL - 64))
          | (1ULL << (sqlParser::CHAR - 64))
          | (1ULL << (sqlParser::CHARACTER - 64))
          | (1ULL << (sqlParser::BINARY - 64))
          | (1ULL << (sqlParser::VARBINARY - 64))
          | (1ULL << (sqlParser::VARYING - 64))
          | (1ULL << (sqlParser::BIT - 64))
          | (1ULL << (sqlParser::BITS - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::BYTE - 128))
          | (1ULL << (sqlParser::BYTES - 128))
          | (1ULL << (sqlParser::CHARS - 128))
          | (1ULL << (sqlParser::DIGITS - 128))
          | (1ULL << (sqlParser::CASE - 128))
          | (1ULL << (sqlParser::CAST - 128))
          | (1ULL << (sqlParser::COALESCE - 128))
          | (1ULL << (sqlParser::CONCAT - 128))
          | (1ULL << (sqlParser::COUNT - 128))
          | (1ULL << (sqlParser::DECODE - 128))
          | (1ULL << (sqlParser::EXTRACT - 128))
          | (1ULL << (sqlParser::EXISTS - 128))
          | (1ULL << (sqlParser::IF - 128))
          | (1ULL << (sqlParser::IFNULL - 128))
          | (1ULL << (sqlParser::NULLIF - 128))
          | (1ULL << (sqlParser::POSITION - 128))
          | (1ULL << (sqlParser::SUBSTR - 128))
          | (1ULL << (sqlParser::SUBSTRING - 128))
          | (1ULL << (sqlParser::TRIM - 128))
          | (1ULL << (sqlParser::TYPEOF - 128))
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
          | (1ULL << (sqlParser::IDENTIFIER - 128))
          | (1ULL << (sqlParser::STRING_LITERAL - 128))
          | (1ULL << (sqlParser::FLOAT_LITERAL - 128))
          | (1ULL << (sqlParser::DECIMAL_LITERAL - 128))
          | (1ULL << (sqlParser::INTEGER_LITERAL - 128)))) != 0)) {
          setState(1245);
          dynamic_cast<TrimFuncContext *>(_localctx)->trimExpr = expr(0);
        }
        setState(1248);
        match(sqlParser::FROM);
        setState(1249);
        expr(0);
        break;
      }

      }
      setState(1252);
      match(sqlParser::T__5);
      break;
    }

    case 19: {
      _localctx = _tracker.createInstance<ExtractFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1254);
      match(sqlParser::EXTRACT);
      setState(1255);
      match(sqlParser::T__4);
      setState(1256);
      _la = _input->LA(1);
      if (!(((((_la - 152) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 152)) & ((1ULL << (sqlParser::DAY - 152))
        | (1ULL << (sqlParser::HOUR - 152))
        | (1ULL << (sqlParser::MINUTE - 152))
        | (1ULL << (sqlParser::MONTH - 152))
        | (1ULL << (sqlParser::QUARTER - 152))
        | (1ULL << (sqlParser::SECOND - 152))
        | (1ULL << (sqlParser::WEEK - 152))
        | (1ULL << (sqlParser::YEAR - 152)))) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(1257);
      match(sqlParser::FROM);
      setState(1258);
      expr(0);
      setState(1259);
      match(sqlParser::T__5);
      break;
    }

    case 20: {
      _localctx = _tracker.createInstance<CountAllContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1261);
      match(sqlParser::COUNT);
      setState(1262);
      match(sqlParser::T__4);
      setState(1264);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::ALL) {
        setState(1263);
        match(sqlParser::ALL);
      }
      setState(1266);
      match(sqlParser::T__10);
      setState(1267);
      match(sqlParser::T__5);
      break;
    }

    case 21: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1268);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::COUNT);
      setState(1269);
      match(sqlParser::T__4);
      setState(1272);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1270);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1271);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__4:
        case sqlParser::T__8:
        case sqlParser::T__9:
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
        case sqlParser::REFERENCE:
        case sqlParser::RIGHT:
        case sqlParser::SCHEMA:
        case sqlParser::SET:
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
      setState(1274);
      expr(0);
      setState(1275);
      match(sqlParser::T__5);
      break;
    }

    case 22: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1277);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::SUM);
      setState(1278);
      match(sqlParser::T__4);
      setState(1281);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1279);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1280);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__4:
        case sqlParser::T__8:
        case sqlParser::T__9:
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
        case sqlParser::REFERENCE:
        case sqlParser::RIGHT:
        case sqlParser::SCHEMA:
        case sqlParser::SET:
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
      setState(1283);
      expr(0);
      setState(1284);
      match(sqlParser::T__5);
      break;
    }

    case 23: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1286);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::AVG);
      setState(1287);
      match(sqlParser::T__4);
      setState(1290);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1288);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1289);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__4:
        case sqlParser::T__8:
        case sqlParser::T__9:
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
        case sqlParser::REFERENCE:
        case sqlParser::RIGHT:
        case sqlParser::SCHEMA:
        case sqlParser::SET:
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
      setState(1292);
      expr(0);
      setState(1293);
      match(sqlParser::T__5);
      break;
    }

    case 24: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1295);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MIN);
      setState(1296);
      match(sqlParser::T__4);
      setState(1299);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1297);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1298);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__4:
        case sqlParser::T__8:
        case sqlParser::T__9:
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
        case sqlParser::REFERENCE:
        case sqlParser::RIGHT:
        case sqlParser::SCHEMA:
        case sqlParser::SET:
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
      setState(1301);
      expr(0);
      setState(1302);
      match(sqlParser::T__5);
      break;
    }

    case 25: {
      _localctx = _tracker.createInstance<AggFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1304);
      dynamic_cast<AggFuncContext *>(_localctx)->func = match(sqlParser::MAX);
      setState(1305);
      match(sqlParser::T__4);
      setState(1308);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::ALL: {
          setState(1306);
          match(sqlParser::ALL);
          break;
        }

        case sqlParser::DISTINCT: {
          setState(1307);
          dynamic_cast<AggFuncContext *>(_localctx)->distinct = match(sqlParser::DISTINCT);
          break;
        }

        case sqlParser::T__4:
        case sqlParser::T__8:
        case sqlParser::T__9:
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
        case sqlParser::REFERENCE:
        case sqlParser::RIGHT:
        case sqlParser::SCHEMA:
        case sqlParser::SET:
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
      setState(1310);
      expr(0);
      setState(1311);
      match(sqlParser::T__5);
      break;
    }

    case 26: {
      _localctx = _tracker.createInstance<ScalarFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1313);
      dynamic_cast<ScalarFuncContext *>(_localctx)->func = functionName();
      setState(1314);
      match(sqlParser::T__4);
      setState(1323);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << sqlParser::T__4)
        | (1ULL << sqlParser::T__8)
        | (1ULL << sqlParser::T__9)
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
        | (1ULL << sqlParser::INDEX)
        | (1ULL << sqlParser::INSERT)
        | (1ULL << sqlParser::INTO)
        | (1ULL << sqlParser::IS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::JOIN - 64))
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
        | (1ULL << (sqlParser::REFERENCE - 64))
        | (1ULL << (sqlParser::RIGHT - 64))
        | (1ULL << (sqlParser::SCHEMA - 64))
        | (1ULL << (sqlParser::SET - 64))
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
        | (1ULL << (sqlParser::BOOLEAN - 64))
        | (1ULL << (sqlParser::INTEGER - 64))
        | (1ULL << (sqlParser::DECIMAL - 64))
        | (1ULL << (sqlParser::FLOAT - 64))
        | (1ULL << (sqlParser::TIME - 64))
        | (1ULL << (sqlParser::TIMESTAMP - 64))
        | (1ULL << (sqlParser::INTERVAL - 64))
        | (1ULL << (sqlParser::CHAR - 64))
        | (1ULL << (sqlParser::CHARACTER - 64))
        | (1ULL << (sqlParser::BINARY - 64))
        | (1ULL << (sqlParser::VARBINARY - 64))
        | (1ULL << (sqlParser::VARYING - 64))
        | (1ULL << (sqlParser::BIT - 64))
        | (1ULL << (sqlParser::BITS - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::BYTE - 128))
        | (1ULL << (sqlParser::BYTES - 128))
        | (1ULL << (sqlParser::CHARS - 128))
        | (1ULL << (sqlParser::DIGITS - 128))
        | (1ULL << (sqlParser::CASE - 128))
        | (1ULL << (sqlParser::CAST - 128))
        | (1ULL << (sqlParser::COALESCE - 128))
        | (1ULL << (sqlParser::CONCAT - 128))
        | (1ULL << (sqlParser::COUNT - 128))
        | (1ULL << (sqlParser::DECODE - 128))
        | (1ULL << (sqlParser::EXTRACT - 128))
        | (1ULL << (sqlParser::EXISTS - 128))
        | (1ULL << (sqlParser::IF - 128))
        | (1ULL << (sqlParser::IFNULL - 128))
        | (1ULL << (sqlParser::NULLIF - 128))
        | (1ULL << (sqlParser::POSITION - 128))
        | (1ULL << (sqlParser::SUBSTR - 128))
        | (1ULL << (sqlParser::SUBSTRING - 128))
        | (1ULL << (sqlParser::TRIM - 128))
        | (1ULL << (sqlParser::TYPEOF - 128))
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
        | (1ULL << (sqlParser::IDENTIFIER - 128))
        | (1ULL << (sqlParser::STRING_LITERAL - 128))
        | (1ULL << (sqlParser::FLOAT_LITERAL - 128))
        | (1ULL << (sqlParser::DECIMAL_LITERAL - 128))
        | (1ULL << (sqlParser::INTEGER_LITERAL - 128)))) != 0)) {
        setState(1315);
        expr(0);
        setState(1320);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == sqlParser::T__2) {
          setState(1316);
          match(sqlParser::T__2);
          setState(1317);
          expr(0);
          setState(1322);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(1325);
      match(sqlParser::T__5);
      break;
    }

    case 27: {
      _localctx = _tracker.createInstance<ParentheticContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1327);
      match(sqlParser::T__4);
      setState(1328);
      expr(0);
      setState(1329);
      match(sqlParser::T__5);
      break;
    }

    case 28: {
      _localctx = _tracker.createInstance<SubqueryExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1331);
      match(sqlParser::T__4);
      setState(1332);
      query();
      setState(1333);
      match(sqlParser::T__5);
      break;
    }

    case 29: {
      _localctx = _tracker.createInstance<SubqueryExistsOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1336);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::NOT) {
        setState(1335);
        match(sqlParser::NOT);
      }
      setState(1338);
      match(sqlParser::EXISTS);

      setState(1339);
      match(sqlParser::T__4);
      setState(1340);
      query();
      setState(1341);
      match(sqlParser::T__5);
      break;
    }

    case 30: {
      _localctx = _tracker.createInstance<LogicOpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(1343);
      dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::NOT);
      setState(1344);
      expr(4);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(1430);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 197, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(1428);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 196, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1347);

          if (!(precpred(_ctx, 36))) throw FailedPredicateException(this, "precpred(_ctx, 36)");
          setState(1348);
          dynamic_cast<BinaryOpContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__10)
            | (1ULL << sqlParser::T__11)
            | (1ULL << sqlParser::T__12))) != 0))) {
            dynamic_cast<BinaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1349);
          expr(37);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1350);

          if (!(precpred(_ctx, 35))) throw FailedPredicateException(this, "precpred(_ctx, 35)");
          setState(1351);
          dynamic_cast<BinaryOpContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__8)
            | (1ULL << sqlParser::T__9)
            | (1ULL << sqlParser::T__13))) != 0))) {
            dynamic_cast<BinaryOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(1352);
          expr(36);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<BinaryOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1353);

          if (!(precpred(_ctx, 34))) throw FailedPredicateException(this, "precpred(_ctx, 34)");
          setState(1354);
          dynamic_cast<BinaryOpContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__3)
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
          setState(1355);
          expr(35);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<LikeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1356);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(1358);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1357);
            match(sqlParser::NOT);
          }
          setState(1360);
          match(sqlParser::LIKE);
          setState(1361);
          expr(10);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<LikeEscapeOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1362);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(1364);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1363);
            match(sqlParser::NOT);
          }
          setState(1366);
          match(sqlParser::LIKE);
          setState(1367);
          expr(0);
          setState(1368);
          match(sqlParser::ESCAPE);
          setState(1369);
          expr(9);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1371);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(1372);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::AND);
          setState(1373);
          expr(4);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1374);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(1375);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::OR);
          setState(1376);
          expr(3);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<LogicOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1377);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(1378);
          dynamic_cast<LogicOpContext *>(_localctx)->op = match(sqlParser::XOR);
          setState(1379);
          expr(2);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<MethodCallContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1380);

          if (!(precpred(_ctx, 37))) throw FailedPredicateException(this, "precpred(_ctx, 37)");
          setState(1381);
          match(sqlParser::T__7);
          setState(1382);
          dynamic_cast<MethodCallContext *>(_localctx)->func = methodName();
          setState(1383);
          match(sqlParser::T__4);
          setState(1392);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__4)
            | (1ULL << sqlParser::T__8)
            | (1ULL << sqlParser::T__9)
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
            | (1ULL << sqlParser::INDEX)
            | (1ULL << sqlParser::INSERT)
            | (1ULL << sqlParser::INTO)
            | (1ULL << sqlParser::IS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::JOIN - 64))
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
            | (1ULL << (sqlParser::REFERENCE - 64))
            | (1ULL << (sqlParser::RIGHT - 64))
            | (1ULL << (sqlParser::SCHEMA - 64))
            | (1ULL << (sqlParser::SET - 64))
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
            | (1ULL << (sqlParser::BOOLEAN - 64))
            | (1ULL << (sqlParser::INTEGER - 64))
            | (1ULL << (sqlParser::DECIMAL - 64))
            | (1ULL << (sqlParser::FLOAT - 64))
            | (1ULL << (sqlParser::TIME - 64))
            | (1ULL << (sqlParser::TIMESTAMP - 64))
            | (1ULL << (sqlParser::INTERVAL - 64))
            | (1ULL << (sqlParser::CHAR - 64))
            | (1ULL << (sqlParser::CHARACTER - 64))
            | (1ULL << (sqlParser::BINARY - 64))
            | (1ULL << (sqlParser::VARBINARY - 64))
            | (1ULL << (sqlParser::VARYING - 64))
            | (1ULL << (sqlParser::BIT - 64))
            | (1ULL << (sqlParser::BITS - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::BYTE - 128))
            | (1ULL << (sqlParser::BYTES - 128))
            | (1ULL << (sqlParser::CHARS - 128))
            | (1ULL << (sqlParser::DIGITS - 128))
            | (1ULL << (sqlParser::CASE - 128))
            | (1ULL << (sqlParser::CAST - 128))
            | (1ULL << (sqlParser::COALESCE - 128))
            | (1ULL << (sqlParser::CONCAT - 128))
            | (1ULL << (sqlParser::COUNT - 128))
            | (1ULL << (sqlParser::DECODE - 128))
            | (1ULL << (sqlParser::EXTRACT - 128))
            | (1ULL << (sqlParser::EXISTS - 128))
            | (1ULL << (sqlParser::IF - 128))
            | (1ULL << (sqlParser::IFNULL - 128))
            | (1ULL << (sqlParser::NULLIF - 128))
            | (1ULL << (sqlParser::POSITION - 128))
            | (1ULL << (sqlParser::SUBSTR - 128))
            | (1ULL << (sqlParser::SUBSTRING - 128))
            | (1ULL << (sqlParser::TRIM - 128))
            | (1ULL << (sqlParser::TYPEOF - 128))
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
            | (1ULL << (sqlParser::IDENTIFIER - 128))
            | (1ULL << (sqlParser::STRING_LITERAL - 128))
            | (1ULL << (sqlParser::FLOAT_LITERAL - 128))
            | (1ULL << (sqlParser::DECIMAL_LITERAL - 128))
            | (1ULL << (sqlParser::INTEGER_LITERAL - 128)))) != 0)) {
            setState(1384);
            expr(0);
            setState(1389);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__2) {
              setState(1385);
              match(sqlParser::T__2);
              setState(1386);
              expr(0);
              setState(1391);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1394);
          match(sqlParser::T__5);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<IsNullOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1396);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(1397);
          match(sqlParser::IS);
          setState(1399);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1398);
            match(sqlParser::NOT);
          }
          setState(1401);
          match(sqlParser::SQL_NULL);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<SubqueryInOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1402);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(1404);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1403);
            match(sqlParser::NOT);
          }
          setState(1406);
          match(sqlParser::IN);

          setState(1407);
          match(sqlParser::T__4);
          setState(1408);
          query();
          setState(1409);
          match(sqlParser::T__5);
          break;
        }

        case 12: {
          auto newContext = _tracker.createInstance<InOpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(1411);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(1413);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == sqlParser::NOT) {
            setState(1412);
            match(sqlParser::NOT);
          }
          setState(1415);
          match(sqlParser::IN);

          setState(1416);
          match(sqlParser::T__4);
          setState(1425);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << sqlParser::T__4)
            | (1ULL << sqlParser::T__8)
            | (1ULL << sqlParser::T__9)
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
            | (1ULL << sqlParser::INDEX)
            | (1ULL << sqlParser::INSERT)
            | (1ULL << sqlParser::INTO)
            | (1ULL << sqlParser::IS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::JOIN - 64))
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
            | (1ULL << (sqlParser::REFERENCE - 64))
            | (1ULL << (sqlParser::RIGHT - 64))
            | (1ULL << (sqlParser::SCHEMA - 64))
            | (1ULL << (sqlParser::SET - 64))
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
            | (1ULL << (sqlParser::BOOLEAN - 64))
            | (1ULL << (sqlParser::INTEGER - 64))
            | (1ULL << (sqlParser::DECIMAL - 64))
            | (1ULL << (sqlParser::FLOAT - 64))
            | (1ULL << (sqlParser::TIME - 64))
            | (1ULL << (sqlParser::TIMESTAMP - 64))
            | (1ULL << (sqlParser::INTERVAL - 64))
            | (1ULL << (sqlParser::CHAR - 64))
            | (1ULL << (sqlParser::CHARACTER - 64))
            | (1ULL << (sqlParser::BINARY - 64))
            | (1ULL << (sqlParser::VARBINARY - 64))
            | (1ULL << (sqlParser::VARYING - 64))
            | (1ULL << (sqlParser::BIT - 64))
            | (1ULL << (sqlParser::BITS - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::BYTE - 128))
            | (1ULL << (sqlParser::BYTES - 128))
            | (1ULL << (sqlParser::CHARS - 128))
            | (1ULL << (sqlParser::DIGITS - 128))
            | (1ULL << (sqlParser::CASE - 128))
            | (1ULL << (sqlParser::CAST - 128))
            | (1ULL << (sqlParser::COALESCE - 128))
            | (1ULL << (sqlParser::CONCAT - 128))
            | (1ULL << (sqlParser::COUNT - 128))
            | (1ULL << (sqlParser::DECODE - 128))
            | (1ULL << (sqlParser::EXTRACT - 128))
            | (1ULL << (sqlParser::EXISTS - 128))
            | (1ULL << (sqlParser::IF - 128))
            | (1ULL << (sqlParser::IFNULL - 128))
            | (1ULL << (sqlParser::NULLIF - 128))
            | (1ULL << (sqlParser::POSITION - 128))
            | (1ULL << (sqlParser::SUBSTR - 128))
            | (1ULL << (sqlParser::SUBSTRING - 128))
            | (1ULL << (sqlParser::TRIM - 128))
            | (1ULL << (sqlParser::TYPEOF - 128))
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
            | (1ULL << (sqlParser::IDENTIFIER - 128))
            | (1ULL << (sqlParser::STRING_LITERAL - 128))
            | (1ULL << (sqlParser::FLOAT_LITERAL - 128))
            | (1ULL << (sqlParser::DECIMAL_LITERAL - 128))
            | (1ULL << (sqlParser::INTEGER_LITERAL - 128)))) != 0)) {
            setState(1417);
            expr(0);
            setState(1422);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == sqlParser::T__2) {
              setState(1418);
              match(sqlParser::T__2);
              setState(1419);
              expr(0);
              setState(1424);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(1427);
          match(sqlParser::T__5);
          break;
        }

        } 
      }
      setState(1432);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 197, _ctx);
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
  enterRule(_localctx, 110, sqlParser::RuleVariableName);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1433);
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
  enterRule(_localctx, 112, sqlParser::RuleColumnRef);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1448);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 200, _ctx)) {
    case 1: {
      setState(1443);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 199, _ctx)) {
      case 1: {
        setState(1438);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 198, _ctx)) {
        case 1: {
          setState(1435);
          catalogName();
          setState(1436);
          match(sqlParser::T__7);
          break;
        }

        }

        setState(1440);
        schemaName();
        setState(1441);
        match(sqlParser::T__7);
        break;
      }

      }

      setState(1445);
      tableName();
      setState(1446);
      match(sqlParser::T__7);
      break;
    }

    }
    setState(1450);
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
  enterRule(_localctx, 114, sqlParser::RuleColumnWildcard);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1465);
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
      | (1ULL << sqlParser::INDEX)
      | (1ULL << sqlParser::INSERT)
      | (1ULL << sqlParser::INTO)
      | (1ULL << sqlParser::IS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::JOIN - 64))
      | (1ULL << (sqlParser::KEY - 64))
      | (1ULL << (sqlParser::LEFT - 64))
      | (1ULL << (sqlParser::MONOTONIC - 64))
      | (1ULL << (sqlParser::NO - 64))
      | (1ULL << (sqlParser::NODE - 64))
      | (1ULL << (sqlParser::OFFSET - 64))
      | (1ULL << (sqlParser::OUTER - 64))
      | (1ULL << (sqlParser::OVERLAY - 64))
      | (1ULL << (sqlParser::PARTITION - 64))
      | (1ULL << (sqlParser::REFERENCE - 64))
      | (1ULL << (sqlParser::RIGHT - 64))
      | (1ULL << (sqlParser::SCHEMA - 64))
      | (1ULL << (sqlParser::SET - 64))
      | (1ULL << (sqlParser::STATS - 64))
      | (1ULL << (sqlParser::STATISTICS - 64))
      | (1ULL << (sqlParser::TABLE - 64))
      | (1ULL << (sqlParser::TYPE - 64))
      | (1ULL << (sqlParser::UNION - 64))
      | (1ULL << (sqlParser::UNIQUE - 64))
      | (1ULL << (sqlParser::UPDATE - 64))
      | (1ULL << (sqlParser::USING - 64))
      | (1ULL << (sqlParser::VALUES - 64))
      | (1ULL << (sqlParser::BOOLEAN - 64))
      | (1ULL << (sqlParser::INTEGER - 64))
      | (1ULL << (sqlParser::DECIMAL - 64))
      | (1ULL << (sqlParser::FLOAT - 64))
      | (1ULL << (sqlParser::TIME - 64))
      | (1ULL << (sqlParser::TIMESTAMP - 64))
      | (1ULL << (sqlParser::CHAR - 64))
      | (1ULL << (sqlParser::CHARACTER - 64))
      | (1ULL << (sqlParser::BINARY - 64))
      | (1ULL << (sqlParser::VARBINARY - 64))
      | (1ULL << (sqlParser::VARYING - 64))
      | (1ULL << (sqlParser::BIT - 64))
      | (1ULL << (sqlParser::BITS - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::BYTE - 128))
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
      setState(1460);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 202, _ctx)) {
      case 1: {
        setState(1455);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 201, _ctx)) {
        case 1: {
          setState(1452);
          catalogName();
          setState(1453);
          match(sqlParser::T__7);
          break;
        }

        }

        setState(1457);
        schemaName();
        setState(1458);
        match(sqlParser::T__7);
        break;
      }

      }

      setState(1462);
      tableName();
      setState(1463);
      match(sqlParser::T__7);
    }
    setState(1467);
    match(sqlParser::T__10);
   
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
  enterRule(_localctx, 116, sqlParser::RuleClassName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1474);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 204, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1469);
        qualifier();
        setState(1470);
        match(sqlParser::T__7); 
      }
      setState(1476);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 204, _ctx);
    }
    setState(1477);
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
  enterRule(_localctx, 118, sqlParser::RuleFunctionName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1479);
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
  enterRule(_localctx, 120, sqlParser::RuleMethodName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1481);
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
  enterRule(_localctx, 122, sqlParser::RuleCatalogName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1483);
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
  enterRule(_localctx, 124, sqlParser::RuleSchemaName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1485);
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
  enterRule(_localctx, 126, sqlParser::RuleTableName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1487);
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
  enterRule(_localctx, 128, sqlParser::RuleTableAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1489);
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
  enterRule(_localctx, 130, sqlParser::RuleColumnName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1491);
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
  enterRule(_localctx, 132, sqlParser::RuleColumnAlias);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1493);
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
  enterRule(_localctx, 134, sqlParser::RuleCollationName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1495);
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
  enterRule(_localctx, 136, sqlParser::RuleIndexName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1497);
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
  enterRule(_localctx, 138, sqlParser::RulePropertyName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1499);
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
  enterRule(_localctx, 140, sqlParser::RuleHintName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1501);
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
  enterRule(_localctx, 142, sqlParser::RulePropertyValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1505);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::SQL_FALSE:
      case sqlParser::SQL_NULL:
      case sqlParser::SQL_TRUE:
      case sqlParser::STRING_LITERAL:
      case sqlParser::FLOAT_LITERAL:
      case sqlParser::DECIMAL_LITERAL:
      case sqlParser::INTEGER_LITERAL: {
        setState(1503);
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
      case sqlParser::REFERENCE:
      case sqlParser::RIGHT:
      case sqlParser::SCHEMA:
      case sqlParser::SET:
      case sqlParser::STATS:
      case sqlParser::STATISTICS:
      case sqlParser::TABLE:
      case sqlParser::TYPE:
      case sqlParser::UNION:
      case sqlParser::UNIQUE:
      case sqlParser::UPDATE:
      case sqlParser::USING:
      case sqlParser::VALUES:
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
        setState(1504);
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
  enterRule(_localctx, 144, sqlParser::RuleHintValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1509);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::SQL_FALSE:
      case sqlParser::SQL_NULL:
      case sqlParser::SQL_TRUE:
      case sqlParser::STRING_LITERAL:
      case sqlParser::FLOAT_LITERAL:
      case sqlParser::DECIMAL_LITERAL:
      case sqlParser::INTEGER_LITERAL: {
        setState(1507);
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
      case sqlParser::REFERENCE:
      case sqlParser::RIGHT:
      case sqlParser::SCHEMA:
      case sqlParser::SET:
      case sqlParser::STATS:
      case sqlParser::STATISTICS:
      case sqlParser::TABLE:
      case sqlParser::TYPE:
      case sqlParser::UNION:
      case sqlParser::UNIQUE:
      case sqlParser::UPDATE:
      case sqlParser::USING:
      case sqlParser::VALUES:
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
        setState(1508);
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
  enterRule(_localctx, 146, sqlParser::RuleIdentifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1513);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(1511);
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
      case sqlParser::REFERENCE:
      case sqlParser::RIGHT:
      case sqlParser::SCHEMA:
      case sqlParser::SET:
      case sqlParser::STATS:
      case sqlParser::STATISTICS:
      case sqlParser::TABLE:
      case sqlParser::TYPE:
      case sqlParser::UNION:
      case sqlParser::UNIQUE:
      case sqlParser::UPDATE:
      case sqlParser::USING:
      case sqlParser::VALUES:
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
        setState(1512);
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
  enterRule(_localctx, 148, sqlParser::RuleQualifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1515);
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
  enterRule(_localctx, 150, sqlParser::RulePrecision);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1517);
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
  enterRule(_localctx, 152, sqlParser::RuleScale);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1519);
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
  enterRule(_localctx, 154, sqlParser::RuleSize);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1521);
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
  enterRule(_localctx, 156, sqlParser::RuleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1533);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::SQL_NULL: {
        enterOuterAlt(_localctx, 1);
        setState(1523);
        match(sqlParser::SQL_NULL);
        break;
      }

      case sqlParser::SQL_FALSE:
      case sqlParser::SQL_TRUE: {
        enterOuterAlt(_localctx, 2);
        setState(1524);
        booleanLiteral();
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(1525);
        integerLiteral();
        break;
      }

      case sqlParser::DECIMAL_LITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(1526);
        decimalLiteral();
        break;
      }

      case sqlParser::FLOAT_LITERAL: {
        enterOuterAlt(_localctx, 5);
        setState(1527);
        floatLiteral();
        break;
      }

      case sqlParser::DATE: {
        enterOuterAlt(_localctx, 6);
        setState(1528);
        dateLiteral();
        break;
      }

      case sqlParser::TIME: {
        enterOuterAlt(_localctx, 7);
        setState(1529);
        timeLiteral();
        break;
      }

      case sqlParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 8);
        setState(1530);
        stringLiteral();
        break;
      }

      case sqlParser::TIMESTAMP: {
        enterOuterAlt(_localctx, 9);
        setState(1531);
        timestampLiteral();
        break;
      }

      case sqlParser::INTERVAL: {
        enterOuterAlt(_localctx, 10);
        setState(1532);
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
  enterRule(_localctx, 158, sqlParser::RuleSimpleLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1541);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case sqlParser::SQL_NULL: {
        enterOuterAlt(_localctx, 1);
        setState(1535);
        match(sqlParser::SQL_NULL);
        break;
      }

      case sqlParser::SQL_FALSE:
      case sqlParser::SQL_TRUE: {
        enterOuterAlt(_localctx, 2);
        setState(1536);
        booleanLiteral();
        break;
      }

      case sqlParser::INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(1537);
        integerLiteral();
        break;
      }

      case sqlParser::DECIMAL_LITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(1538);
        decimalLiteral();
        break;
      }

      case sqlParser::FLOAT_LITERAL: {
        enterOuterAlt(_localctx, 5);
        setState(1539);
        floatLiteral();
        break;
      }

      case sqlParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 6);
        setState(1540);
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
  enterRule(_localctx, 160, sqlParser::RuleBooleanLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1543);
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
  enterRule(_localctx, 162, sqlParser::RuleIntegerLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1545);
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
  enterRule(_localctx, 164, sqlParser::RuleDecimalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1547);
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
  enterRule(_localctx, 166, sqlParser::RuleFloatLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1549);
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
  enterRule(_localctx, 168, sqlParser::RuleDateLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1551);
    match(sqlParser::DATE);
    setState(1552);
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
  enterRule(_localctx, 170, sqlParser::RuleTimeLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1554);
    match(sqlParser::TIME);
    setState(1555);
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
  enterRule(_localctx, 172, sqlParser::RuleTimestampLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1557);
    match(sqlParser::TIMESTAMP);
    setState(1558);
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
  enterRule(_localctx, 174, sqlParser::RuleStringLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1560);
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
  enterRule(_localctx, 176, sqlParser::RuleIntervalLiteral);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1565);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 210, _ctx)) {
    case 1: {
      setState(1562);
      sql92CalendarInterval();
      break;
    }

    case 2: {
      setState(1563);
      sql92TimeInterval();
      break;
    }

    case 3: {
      setState(1564);
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
  enterRule(_localctx, 178, sqlParser::RuleLooseInterval);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1567);
    match(sqlParser::INTERVAL);
    setState(1568);
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
  enterRule(_localctx, 180, sqlParser::RuleSql92CalendarInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1570);
    match(sqlParser::INTERVAL);
    setState(1571);
    stringLiteral();
    setState(1595);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 214, _ctx)) {
    case 1: {
      setState(1572);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1577);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 211, _ctx)) {
      case 1: {
        setState(1573);
        match(sqlParser::T__4);
        setState(1574);
        precision();
        setState(1575);
        match(sqlParser::T__5);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1579);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->year = match(sqlParser::YEAR);
      setState(1584);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__4) {
        setState(1580);
        match(sqlParser::T__4);
        setState(1581);
        precision();
        setState(1582);
        match(sqlParser::T__5);
      }
      setState(1586);
      match(sqlParser::TO);
      setState(1587);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      break;
    }

    case 3: {
      setState(1588);
      dynamic_cast<Sql92CalendarIntervalContext *>(_localctx)->month = match(sqlParser::MONTH);
      setState(1593);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 213, _ctx)) {
      case 1: {
        setState(1589);
        match(sqlParser::T__4);
        setState(1590);
        precision();
        setState(1591);
        match(sqlParser::T__5);
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
  enterRule(_localctx, 182, sqlParser::RuleSql92TimeInterval);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1597);
    match(sqlParser::INTERVAL);
    setState(1598);
    stringLiteral();
    setState(1683);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 228, _ctx)) {
    case 1: {
      setState(1599);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1604);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 215, _ctx)) {
      case 1: {
        setState(1600);
        match(sqlParser::T__4);
        setState(1601);
        precision();
        setState(1602);
        match(sqlParser::T__5);
        break;
      }

      }
      break;
    }

    case 2: {
      setState(1606);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->day = match(sqlParser::DAY);
      setState(1611);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__4) {
        setState(1607);
        match(sqlParser::T__4);
        setState(1608);
        precision();
        setState(1609);
        match(sqlParser::T__5);
      }
      setState(1613);
      match(sqlParser::TO);
      setState(1623);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::HOUR: {
          setState(1614);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
          break;
        }

        case sqlParser::MINUTE: {
          setState(1615);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1616);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1621);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 217, _ctx)) {
          case 1: {
            setState(1617);
            match(sqlParser::T__4);
            setState(1618);
            scale();
            setState(1619);
            match(sqlParser::T__5);
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
      setState(1625);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1630);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__4) {
        setState(1626);
        match(sqlParser::T__4);
        setState(1627);
        precision();
        setState(1628);
        match(sqlParser::T__5);
      }
      setState(1632);
      match(sqlParser::TO);
      setState(1641);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case sqlParser::MINUTE: {
          setState(1633);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
          break;
        }

        case sqlParser::SECOND: {
          setState(1634);
          dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
          setState(1639);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 220, _ctx)) {
          case 1: {
            setState(1635);
            match(sqlParser::T__4);
            setState(1636);
            scale();
            setState(1637);
            match(sqlParser::T__5);
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
      setState(1643);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1648);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == sqlParser::T__4) {
        setState(1644);
        match(sqlParser::T__4);
        setState(1645);
        precision();
        setState(1646);
        match(sqlParser::T__5);
      }
      setState(1650);
      match(sqlParser::TO);

      setState(1651);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1656);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 223, _ctx)) {
      case 1: {
        setState(1652);
        match(sqlParser::T__4);
        setState(1653);
        scale();
        setState(1654);
        match(sqlParser::T__5);
        break;
      }

      }
      break;
    }

    case 5: {
      setState(1658);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->hour = match(sqlParser::HOUR);
      setState(1663);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 224, _ctx)) {
      case 1: {
        setState(1659);
        match(sqlParser::T__4);
        setState(1660);
        precision();
        setState(1661);
        match(sqlParser::T__5);
        break;
      }

      }
      break;
    }

    case 6: {
      setState(1665);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->min = match(sqlParser::MINUTE);
      setState(1670);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 225, _ctx)) {
      case 1: {
        setState(1666);
        match(sqlParser::T__4);
        setState(1667);
        precision();
        setState(1668);
        match(sqlParser::T__5);
        break;
      }

      }
      break;
    }

    case 7: {
      setState(1672);
      dynamic_cast<Sql92TimeIntervalContext *>(_localctx)->sec = match(sqlParser::SECOND);
      setState(1681);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 227, _ctx)) {
      case 1: {
        setState(1673);
        match(sqlParser::T__4);
        setState(1674);
        precision();
        setState(1677);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == sqlParser::T__2) {
          setState(1675);
          match(sqlParser::T__2);
          setState(1676);
          scale();
        }
        setState(1679);
        match(sqlParser::T__5);
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

tree::TerminalNode* sqlParser::NonReservedWordContext::YEAR() {
  return getToken(sqlParser::YEAR, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::YEARS() {
  return getToken(sqlParser::YEARS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::STATS() {
  return getToken(sqlParser::STATS, 0);
}

tree::TerminalNode* sqlParser::NonReservedWordContext::STATISTICS() {
  return getToken(sqlParser::STATISTICS, 0);
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
  enterRule(_localctx, 184, sqlParser::RuleNonReservedWord);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1685);
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
      | (1ULL << sqlParser::INDEX)
      | (1ULL << sqlParser::INSERT)
      | (1ULL << sqlParser::INTO)
      | (1ULL << sqlParser::IS))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (sqlParser::JOIN - 64))
      | (1ULL << (sqlParser::KEY - 64))
      | (1ULL << (sqlParser::LEFT - 64))
      | (1ULL << (sqlParser::MONOTONIC - 64))
      | (1ULL << (sqlParser::NO - 64))
      | (1ULL << (sqlParser::NODE - 64))
      | (1ULL << (sqlParser::OFFSET - 64))
      | (1ULL << (sqlParser::OUTER - 64))
      | (1ULL << (sqlParser::OVERLAY - 64))
      | (1ULL << (sqlParser::PARTITION - 64))
      | (1ULL << (sqlParser::REFERENCE - 64))
      | (1ULL << (sqlParser::RIGHT - 64))
      | (1ULL << (sqlParser::SCHEMA - 64))
      | (1ULL << (sqlParser::SET - 64))
      | (1ULL << (sqlParser::STATS - 64))
      | (1ULL << (sqlParser::STATISTICS - 64))
      | (1ULL << (sqlParser::TABLE - 64))
      | (1ULL << (sqlParser::TYPE - 64))
      | (1ULL << (sqlParser::UNION - 64))
      | (1ULL << (sqlParser::UNIQUE - 64))
      | (1ULL << (sqlParser::UPDATE - 64))
      | (1ULL << (sqlParser::USING - 64))
      | (1ULL << (sqlParser::VALUES - 64))
      | (1ULL << (sqlParser::BOOLEAN - 64))
      | (1ULL << (sqlParser::INTEGER - 64))
      | (1ULL << (sqlParser::DECIMAL - 64))
      | (1ULL << (sqlParser::FLOAT - 64))
      | (1ULL << (sqlParser::TIME - 64))
      | (1ULL << (sqlParser::TIMESTAMP - 64))
      | (1ULL << (sqlParser::CHAR - 64))
      | (1ULL << (sqlParser::CHARACTER - 64))
      | (1ULL << (sqlParser::BINARY - 64))
      | (1ULL << (sqlParser::VARBINARY - 64))
      | (1ULL << (sqlParser::VARYING - 64))
      | (1ULL << (sqlParser::BIT - 64))
      | (1ULL << (sqlParser::BITS - 64)))) != 0) || ((((_la - 128) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 128)) & ((1ULL << (sqlParser::BYTE - 128))
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
    case 11: return setExprSempred(dynamic_cast<SetExprContext *>(context), predicateIndex);
    case 14: return tableExprSempred(dynamic_cast<TableExprContext *>(context), predicateIndex);
    case 54: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

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
  "statisticsStatement", "query", "queryHint", "hint", "selectQuery", "selectExpr", 
  "setQuery", "setExpr", "corrClause", "withQuery", "tableExpr", "tableReference", 
  "joinType", "joinHint", "valuesExpr", "insertQuery", "deleteQuery", "updateQuery", 
  "createQuery", "createTableQuery", "createIndexQuery", "dropQuery", "truncateQuery", 
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
  "decimalLiteral", "floatLiteral", "dateLiteral", "timeLiteral", "timestampLiteral", 
  "stringLiteral", "intervalLiteral", "looseInterval", "sql92CalendarInterval", 
  "sql92TimeInterval", "nonReservedWord"
};

std::vector<std::string> sqlParser::_literalNames = {
  "", "':'", "';'", "','", "'='", "'('", "')'", "'?'", "'.'", "'-'", "'+'", 
  "'*'", "'/'", "'%'", "'||'", "'<'", "'<='", "'!='", "'<>'", "'>='", "'>'", 
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
  "", "", "", "ALL", "AND", "ANTLR", "ANY", "AS", "ASC", "AVG", "BETWEEN", 
  "BOTH", "BY", "CATALOG", "COLLATE", "CORRESPONDING", "CREATE", "CROSS", 
  "CURRENT_CATALOG", "CURRENT_SCHEMA", "CURRENT_TIMESTAMP", "CURRENT_USER", 
  "DATA", "DATE", "DELETE", "DESC", "DETAIL", "DISTINCT", "DROP", "ESCAPE", 
  "EXCEPT", "EXPLAIN", "SQL_FALSE", "FEDERATION", "FOR", "FULL", "FROM", 
  "GROUP", "HAVING", "IN", "INDEX", "INNER", "INSERT", "INTERSECT", "INTO", 
  "IS", "JOIN", "KEY", "LEADING", "LEFT", "LIKE", "LIMIT", "MAX", "MIN", 
  "MONOTONIC", "NO", "NODE", "NOT", "NOW", "SQL_NULL", "OFFSET", "ON", "OR", 
  "ORDER", "OUTER", "OVER", "OVERLAY", "PARTITION", "PLACING", "PRIMARY", 
  "REFERENCE", "RIGHT", "SCHEMA", "SELECT", "SET", "STATS", "STATISTICS", 
  "SUM", "TABLE", "TO", "TOP", "TRAILING", "TRUNCATE", "SQL_TRUE", "TYPE", 
  "UNION", "UNIQUE", "UPDATE", "USING", "VALUES", "WHERE", "WITH", "XOR", 
  "BOOLEAN", "INTEGER", "DECIMAL", "FLOAT", "TIME", "TIMESTAMP", "INTERVAL", 
  "CHAR", "CHARACTER", "VARCHAR", "TEXT", "BINARY", "VARBINARY", "BLOB", 
  "VARYING", "BIT", "BITS", "BYTE", "BYTES", "CHARS", "DIGITS", "CASE", 
  "CAST", "COALESCE", "CONCAT", "COUNT", "DECODE", "EXTRACT", "EXISTS", 
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
    0x3, 0xc1, 0x69a, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
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
    0x4, 0x5d, 0x9, 0x5d, 0x4, 0x5e, 0x9, 0x5e, 0x3, 0x2, 0x3, 0x2, 0x5, 
    0x2, 0xbf, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x5, 0x3, 0xc7, 0xa, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x7, 
    0x5, 0xcc, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0xcf, 0xb, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x5, 0x5, 0xd3, 0xa, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x7, 0x6, 
    0xd8, 0xa, 0x6, 0xc, 0x6, 0xe, 0x6, 0xdb, 0xb, 0x6, 0x3, 0x6, 0x3, 0x6, 
    0x5, 0x6, 0xdf, 0xa, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x7, 0x7, 0xe4, 
    0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 0xe7, 0xb, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 0xf1, 
    0xa, 0x7, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 0xf5, 0xa, 0x8, 0x3, 0x8, 0x3, 
    0x8, 0x7, 0x8, 0xf9, 0xa, 0x8, 0xc, 0x8, 0xe, 0x8, 0xfc, 0xb, 0x8, 0x3, 
    0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
    0x9, 0x3, 0x9, 0x5, 0x9, 0x107, 0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
    0x3, 0x9, 0x7, 0x9, 0x10d, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x110, 0xb, 
    0x9, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 0x114, 0xa, 0x9, 0x3, 0xa, 0x3, 0xa, 
    0x3, 0xa, 0x3, 0xa, 0x7, 0xa, 0x11a, 0xa, 0xa, 0xc, 0xa, 0xe, 0xa, 0x11d, 
    0xb, 0xa, 0x5, 0xa, 0x11f, 0xa, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 
    0xa, 0x5, 0xa, 0x125, 0xa, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 
    0x5, 0xa, 0x12b, 0xa, 0xa, 0x5, 0xa, 0x12d, 0xa, 0xa, 0x3, 0xb, 0x3, 
    0xb, 0x3, 0xb, 0x5, 0xb, 0x132, 0xa, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 
    0x5, 0xb, 0x137, 0xa, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x13b, 0xa, 
    0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x140, 0xa, 0xb, 0x3, 0xb, 
    0x3, 0xb, 0x5, 0xb, 0x144, 0xa, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 
    0xb, 0x5, 0xb, 0x14a, 0xa, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 
    0x7, 0xc, 0x150, 0xa, 0xc, 0xc, 0xc, 0xe, 0xc, 0x153, 0xb, 0xc, 0x5, 
    0xc, 0x155, 0xa, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 
    0x15b, 0xa, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0x161, 
    0xa, 0xc, 0x5, 0xc, 0x163, 0xa, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xd, 0x3, 0xd, 0x5, 0xd, 0x16b, 0xa, 0xd, 0x3, 0xd, 0x3, 0xd, 
    0x3, 0xd, 0x3, 0xd, 0x5, 0xd, 0x171, 0xa, 0xd, 0x3, 0xd, 0x5, 0xd, 0x174, 
    0xa, 0xd, 0x3, 0xd, 0x7, 0xd, 0x177, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 0x17a, 
    0xb, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x5, 0xe, 0x182, 0xa, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 
    0xf, 0x7, 0xf, 0x189, 0xa, 0xf, 0xc, 0xf, 0xe, 0xf, 0x18c, 0xb, 0xf, 
    0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0x190, 0xa, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 
    0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 
    0x19a, 0xa, 0x10, 0x3, 0x10, 0x5, 0x10, 0x19d, 0xa, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0x1a3, 0xa, 0x10, 0x3, 0x10, 
    0x5, 0x10, 0x1a6, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x5, 0x10, 0x1ac, 0xa, 0x10, 0x3, 0x10, 0x5, 0x10, 0x1af, 0xa, 0x10, 
    0x5, 0x10, 0x1b1, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x5, 0x10, 0x1b7, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 
    0x1bc, 0xa, 0x10, 0x3, 0x10, 0x5, 0x10, 0x1bf, 0xa, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 
    0x10, 0x1c8, 0xa, 0x10, 0x3, 0x10, 0x5, 0x10, 0x1cb, 0xa, 0x10, 0x3, 
    0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x7, 0x10, 0x1d4, 0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 0x1d7, 0xb, 0x10, 
    0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0x1dc, 0xa, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0x1e1, 0xa, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x1e8, 0xa, 0x12, 0x5, 0x12, 
    0x1ea, 0xa, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0x1ef, 
    0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x7, 0x13, 0x1f5, 
    0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0x1f8, 0xb, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x5, 0x13, 0x1fc, 0xa, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 
    0x14, 0x7, 0x14, 0x209, 0xa, 0x14, 0xc, 0x14, 0xe, 0x14, 0x20c, 0xb, 
    0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x7, 0x15, 0x212, 
    0xa, 0x15, 0xc, 0x15, 0xe, 0x15, 0x215, 0xb, 0x15, 0x5, 0x15, 0x217, 
    0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x21b, 0xa, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x7, 0x15, 0x222, 0xa, 0x15, 
    0xc, 0x15, 0xe, 0x15, 0x225, 0xb, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 
    0x229, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x22e, 
    0xa, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x7, 0x16, 0x234, 
    0xa, 0x16, 0xc, 0x16, 0xe, 0x16, 0x237, 0xb, 0x16, 0x5, 0x16, 0x239, 
    0xa, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x23d, 0xa, 0x16, 0x3, 0x16, 
    0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x244, 0xa, 0x16, 
    0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x7, 0x17, 0x24a, 0xa, 0x17, 
    0xc, 0x17, 0xe, 0x17, 0x24d, 0xb, 0x17, 0x5, 0x17, 0x24f, 0xa, 0x17, 
    0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 
    0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x7, 0x17, 0x25c, 
    0xa, 0x17, 0xc, 0x17, 0xe, 0x17, 0x25f, 0xb, 0x17, 0x3, 0x17, 0x3, 0x17, 
    0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0x265, 0xa, 0x17, 0x3, 0x18, 0x3, 0x18, 
    0x5, 0x18, 0x269, 0xa, 0x18, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x26d, 
    0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x273, 
    0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x278, 0xa, 0x19, 
    0x3, 0x19, 0x5, 0x19, 0x27b, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 
    0x5, 0x19, 0x280, 0xa, 0x19, 0x3, 0x19, 0x5, 0x19, 0x283, 0xa, 0x19, 
    0x3, 0x19, 0x3, 0x19, 0x5, 0x19, 0x287, 0xa, 0x19, 0x3, 0x19, 0x3, 0x19, 
    0x3, 0x19, 0x5, 0x19, 0x28c, 0xa, 0x19, 0x3, 0x19, 0x5, 0x19, 0x28f, 
    0xa, 0x19, 0x5, 0x19, 0x291, 0xa, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 
    0x295, 0xa, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 
    0x29b, 0xa, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 
    0x5, 0x1a, 0x2a2, 0xa, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 
    0x7, 0x1a, 0x2a8, 0xa, 0x1a, 0xc, 0x1a, 0xe, 0x1a, 0x2ab, 0xb, 0x1a, 
    0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 0x2b0, 0xa, 0x1a, 0x3, 0x1b, 
    0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x2b6, 0xa, 0x1b, 0x3, 0x1b, 
    0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x2bd, 0xa, 0x1b, 
    0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x2c3, 0xa, 0x1b, 
    0x3, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x2c7, 0xa, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 
    0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x5, 0x1d, 0x2cf, 0xa, 0x1d, 
    0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x5, 0x1d, 0x2d5, 0xa, 0x1d, 
    0x7, 0x1d, 0x2d7, 0xa, 0x1d, 0xc, 0x1d, 0xe, 0x1d, 0x2da, 0xb, 0x1d, 
    0x3, 0x1d, 0x3, 0x1d, 0x5, 0x1d, 0x2de, 0xa, 0x1d, 0x3, 0x1d, 0x5, 0x1d, 
    0x2e1, 0xa, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x5, 0x1d, 0x2e5, 0xa, 0x1d, 
    0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x2e9, 0xa, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 
    0x2ec, 0xa, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x2ef, 0xa, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x7, 0x1e, 0x2f5, 0xa, 0x1e, 0xc, 0x1e, 
    0xe, 0x1e, 0x2f8, 0xb, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 
    0x3, 0x1f, 0x5, 0x1f, 0x2ff, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
    0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0x306, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
    0x3, 0x1f, 0x5, 0x1f, 0x30b, 0xa, 0x1f, 0x7, 0x1f, 0x30d, 0xa, 0x1f, 
    0xc, 0x1f, 0xe, 0x1f, 0x310, 0xb, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 
    0x314, 0xa, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x7, 0x20, 
    0x31a, 0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 0x31d, 0xb, 0x20, 0x3, 0x20, 
    0x3, 0x20, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 
    0x21, 0x3, 0x21, 0x7, 0x21, 0x328, 0xa, 0x21, 0xc, 0x21, 0xe, 0x21, 
    0x32b, 0xb, 0x21, 0x3, 0x21, 0x3, 0x21, 0x5, 0x21, 0x32f, 0xa, 0x21, 
    0x3, 0x21, 0x5, 0x21, 0x332, 0xa, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 
    0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x7, 0x21, 0x33b, 0xa, 0x21, 
    0xc, 0x21, 0xe, 0x21, 0x33e, 0xb, 0x21, 0x3, 0x21, 0x3, 0x21, 0x5, 0x21, 
    0x342, 0xa, 0x21, 0x5, 0x21, 0x344, 0xa, 0x21, 0x3, 0x22, 0x5, 0x22, 
    0x347, 0xa, 0x22, 0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x34b, 0xa, 0x22, 
    0x3, 0x22, 0x5, 0x22, 0x34e, 0xa, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 
    0x3, 0x22, 0x3, 0x22, 0x7, 0x22, 0x355, 0xa, 0x22, 0xc, 0x22, 0xe, 0x22, 
    0x358, 0xb, 0x22, 0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x35c, 0xa, 0x22, 
    0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x360, 0xa, 0x22, 0x3, 0x22, 0x3, 0x22, 
    0x5, 0x22, 0x364, 0xa, 0x22, 0x3, 0x23, 0x3, 0x23, 0x3, 0x24, 0x3, 0x24, 
    0x3, 0x24, 0x5, 0x24, 0x36b, 0xa, 0x24, 0x3, 0x24, 0x5, 0x24, 0x36e, 
    0xa, 0x24, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 
    0x25, 0x375, 0xa, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x379, 0xa, 
    0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x37f, 
    0xa, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x384, 0xa, 0x25, 
    0x5, 0x25, 0x386, 0xa, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x38a, 
    0xa, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x390, 
    0xa, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x394, 0xa, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 
    0x25, 0x39d, 0xa, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 
    0x5, 0x25, 0x3a3, 0xa, 0x25, 0x3, 0x25, 0x5, 0x25, 0x3a6, 0xa, 0x25, 
    0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x3ab, 0xa, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x5, 0x25, 0x3af, 0xa, 0x25, 0x3, 0x25, 0x5, 0x25, 0x3b2, 
    0xa, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x3b6, 0xa, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x3bc, 0xa, 0x25, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x3c4, 
    0xa, 0x25, 0x3, 0x25, 0x3, 0x25, 0x5, 0x25, 0x3c8, 0xa, 0x25, 0x3, 0x25, 
    0x5, 0x25, 0x3cb, 0xa, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x27, 0x3, 0x27, 
    0x7, 0x27, 0x3d1, 0xa, 0x27, 0xc, 0x27, 0xe, 0x27, 0x3d4, 0xb, 0x27, 
    0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x7, 0x27, 0x3da, 0xa, 0x27, 
    0xc, 0x27, 0xe, 0x27, 0x3dd, 0xb, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 
    0x5, 0x27, 0x3e2, 0xa, 0x27, 0x5, 0x27, 0x3e4, 0xa, 0x27, 0x3, 0x28, 
    0x3, 0x28, 0x3, 0x28, 0x7, 0x28, 0x3e9, 0xa, 0x28, 0xc, 0x28, 0xe, 0x28, 
    0x3ec, 0xb, 0x28, 0x3, 0x29, 0x3, 0x29, 0x5, 0x29, 0x3f0, 0xa, 0x29, 
    0x3, 0x29, 0x5, 0x29, 0x3f3, 0xa, 0x29, 0x3, 0x29, 0x5, 0x29, 0x3f6, 
    0xa, 0x29, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x7, 0x2a, 0x3fb, 0xa, 0x2a, 
    0xc, 0x2a, 0xe, 0x2a, 0x3fe, 0xb, 0x2a, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2c, 
    0x3, 0x2c, 0x3, 0x2c, 0x7, 0x2c, 0x405, 0xa, 0x2c, 0xc, 0x2c, 0xe, 0x2c, 
    0x408, 0xb, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 0x5, 0x2d, 0x40c, 0xa, 0x2d, 
    0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x7, 0x2e, 0x411, 0xa, 0x2e, 0xc, 0x2e, 
    0xe, 0x2e, 0x414, 0xb, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x30, 0x3, 0x30, 
    0x3, 0x30, 0x7, 0x30, 0x41b, 0xa, 0x30, 0xc, 0x30, 0xe, 0x30, 0x41e, 
    0xb, 0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x7, 
    0x32, 0x425, 0xa, 0x32, 0xc, 0x32, 0xe, 0x32, 0x428, 0xb, 0x32, 0x3, 
    0x33, 0x3, 0x33, 0x3, 0x33, 0x5, 0x33, 0x42d, 0xa, 0x33, 0x3, 0x34, 
    0x3, 0x34, 0x3, 0x34, 0x7, 0x34, 0x432, 0xa, 0x34, 0xc, 0x34, 0xe, 0x34, 
    0x435, 0xb, 0x34, 0x3, 0x35, 0x3, 0x35, 0x3, 0x36, 0x3, 0x36, 0x3, 0x37, 
    0x3, 0x37, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x6, 0x38, 0x45b, 
    0xa, 0x38, 0xd, 0x38, 0xe, 0x38, 0x45c, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x6, 0x38, 0x464, 0xa, 0x38, 0xd, 0x38, 0xe, 0x38, 
    0x465, 0x5, 0x38, 0x468, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 
    0x46c, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x7, 0x38, 
    0x48c, 0xa, 0x38, 0xc, 0x38, 0xe, 0x38, 0x48f, 0xb, 0x38, 0x5, 0x38, 
    0x491, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x7, 0x38, 0x499, 0xa, 0x38, 0xc, 0x38, 0xe, 0x38, 0x49c, 
    0xb, 0x38, 0x5, 0x38, 0x49e, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x7, 0x38, 0x4a6, 0xa, 0x38, 0xc, 0x38, 
    0xe, 0x38, 0x4a9, 0xb, 0x38, 0x5, 0x38, 0x4ab, 0xa, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x4b7, 0xa, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x4c9, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x4d0, 0xa, 0x38, 0x5, 0x38, 
    0x4d2, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x4db, 0xa, 0x38, 0x3, 0x38, 0x5, 0x38, 
    0x4de, 0xa, 0x38, 0x3, 0x38, 0x5, 0x38, 0x4e1, 0xa, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x5, 0x38, 0x4e5, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x4f3, 0xa, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x4fb, 
    0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x3, 0x38, 0x5, 0x38, 0x504, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x50d, 
    0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x3, 0x38, 0x5, 0x38, 0x516, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x51f, 
    0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x3, 0x38, 0x3, 0x38, 0x7, 0x38, 0x529, 0xa, 0x38, 0xc, 0x38, 
    0xe, 0x38, 0x52c, 0xb, 0x38, 0x5, 0x38, 0x52e, 0xa, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x53b, 0xa, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x5, 0x38, 0x544, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x3, 0x38, 0x5, 0x38, 0x551, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x557, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
    0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x7, 
    0x38, 0x56e, 0xa, 0x38, 0xc, 0x38, 0xe, 0x38, 0x571, 0xb, 0x38, 0x5, 
    0x38, 0x573, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x5, 0x38, 0x57a, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x5, 0x38, 0x57f, 0xa, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x588, 0xa, 0x38, 0x3, 0x38, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x7, 0x38, 0x58f, 0xa, 0x38, 
    0xc, 0x38, 0xe, 0x38, 0x592, 0xb, 0x38, 0x5, 0x38, 0x594, 0xa, 0x38, 
    0x3, 0x38, 0x7, 0x38, 0x597, 0xa, 0x38, 0xc, 0x38, 0xe, 0x38, 0x59a, 
    0xb, 0x38, 0x3, 0x39, 0x3, 0x39, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 
    0x3a, 0x5a1, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 
    0x5a6, 0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x5, 0x3a, 0x5ab, 
    0xa, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x5, 
    0x3b, 0x5b2, 0xa, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x5, 0x3b, 
    0x5b7, 0xa, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x5, 0x3b, 0x5bc, 
    0xa, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 0x7, 
    0x3c, 0x5c3, 0xa, 0x3c, 0xc, 0x3c, 0xe, 0x3c, 0x5c6, 0xb, 0x3c, 0x3, 
    0x3c, 0x3, 0x3c, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3f, 
    0x3, 0x3f, 0x3, 0x40, 0x3, 0x40, 0x3, 0x41, 0x3, 0x41, 0x3, 0x42, 0x3, 
    0x42, 0x3, 0x43, 0x3, 0x43, 0x3, 0x44, 0x3, 0x44, 0x3, 0x45, 0x3, 0x45, 
    0x3, 0x46, 0x3, 0x46, 0x3, 0x47, 0x3, 0x47, 0x3, 0x48, 0x3, 0x48, 0x3, 
    0x49, 0x3, 0x49, 0x5, 0x49, 0x5e4, 0xa, 0x49, 0x3, 0x4a, 0x3, 0x4a, 
    0x5, 0x4a, 0x5e8, 0xa, 0x4a, 0x3, 0x4b, 0x3, 0x4b, 0x5, 0x4b, 0x5ec, 
    0xa, 0x4b, 0x3, 0x4c, 0x3, 0x4c, 0x3, 0x4d, 0x3, 0x4d, 0x3, 0x4e, 0x3, 
    0x4e, 0x3, 0x4f, 0x3, 0x4f, 0x3, 0x50, 0x3, 0x50, 0x3, 0x50, 0x3, 0x50, 
    0x3, 0x50, 0x3, 0x50, 0x3, 0x50, 0x3, 0x50, 0x3, 0x50, 0x3, 0x50, 0x5, 
    0x50, 0x600, 0xa, 0x50, 0x3, 0x51, 0x3, 0x51, 0x3, 0x51, 0x3, 0x51, 
    0x3, 0x51, 0x3, 0x51, 0x5, 0x51, 0x608, 0xa, 0x51, 0x3, 0x52, 0x3, 0x52, 
    0x3, 0x53, 0x3, 0x53, 0x3, 0x54, 0x3, 0x54, 0x3, 0x55, 0x3, 0x55, 0x3, 
    0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 0x3, 0x58, 
    0x3, 0x58, 0x3, 0x58, 0x3, 0x59, 0x3, 0x59, 0x3, 0x5a, 0x3, 0x5a, 0x3, 
    0x5a, 0x5, 0x5a, 0x620, 0xa, 0x5a, 0x3, 0x5b, 0x3, 0x5b, 0x3, 0x5b, 
    0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x3, 
    0x5c, 0x5, 0x5c, 0x62c, 0xa, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 
    0x3, 0x5c, 0x3, 0x5c, 0x5, 0x5c, 0x633, 0xa, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 
    0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x5, 0x5c, 0x63c, 
    0xa, 0x5c, 0x5, 0x5c, 0x63e, 0xa, 0x5c, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 
    0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x647, 0xa, 0x5d, 
    0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x64e, 
    0xa, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 
    0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x658, 0xa, 0x5d, 0x5, 0x5d, 
    0x65a, 0xa, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 
    0x5, 0x5d, 0x661, 0xa, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 
    0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x66a, 0xa, 0x5d, 0x5, 0x5d, 
    0x66c, 0xa, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 
    0x5, 0x5d, 0x673, 0xa, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 
    0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x67b, 0xa, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 
    0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x682, 0xa, 0x5d, 0x3, 0x5d, 
    0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x689, 0xa, 0x5d, 
    0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x690, 
    0xa, 0x5d, 0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x694, 0xa, 0x5d, 0x5, 0x5d, 
    0x696, 0xa, 0x5d, 0x3, 0x5e, 0x3, 0x5e, 0x3, 0x5e, 0x2, 0x5, 0x18, 0x1e, 
    0x6e, 0x5f, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 
    0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 
    0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 
    0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 
    0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 
    0x78, 0x7a, 0x7c, 0x7e, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 
    0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, 0xa0, 0xa2, 0xa4, 0xa6, 
    0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0x2, 0x1e, 
    0x3, 0x2, 0x5f, 0x60, 0x4, 0x2, 0x9, 0x9, 0x5d, 0x5d, 0x5, 0x2, 0x32, 
    0x32, 0x3f, 0x3f, 0x69, 0x69, 0x5, 0x2, 0x37, 0x37, 0x45, 0x45, 0x5b, 
    0x5b, 0x4, 0x2, 0xaa, 0xab, 0xad, 0xae, 0x4, 0x2, 0x3c, 0x3c, 0x43, 
    0x43, 0x4, 0x2, 0xad, 0xad, 0xb1, 0xb1, 0x4, 0x2, 0xac, 0xad, 0xaf, 
    0xb0, 0x5, 0x2, 0x81, 0x81, 0x83, 0x83, 0x85, 0x85, 0x3, 0x2, 0x78, 
    0x79, 0x4, 0x2, 0x78, 0x78, 0x82, 0x84, 0x3, 0x2, 0x82, 0x83, 0x4, 0x2, 
    0x1c, 0x1c, 0x2d, 0x2d, 0x3, 0x2, 0xb, 0xc, 0x4, 0x2, 0x5, 0x5, 0x1b, 
    0x1b, 0x4, 0x2, 0x5, 0x5, 0x58, 0x58, 0x4, 0x2, 0x5, 0x5, 0x38, 0x38, 
    0x4, 0x2, 0x5, 0x5, 0x36, 0x36, 0x4, 0x2, 0x5, 0x5, 0x3b, 0x3b, 0x3, 
    0x2, 0x92, 0x93, 0x5, 0x2, 0x1f, 0x1f, 0x44, 0x44, 0x65, 0x65, 0xa, 
    0x2, 0x9a, 0x9a, 0x9c, 0x9c, 0x9e, 0x9e, 0xa0, 0xa0, 0xa2, 0xa2, 0xa4, 
    0xa4, 0xa6, 0xa6, 0xa8, 0xa8, 0x3, 0x2, 0xd, 0xf, 0x4, 0x2, 0xb, 0xc, 
    0x10, 0x10, 0x4, 0x2, 0x6, 0x6, 0x11, 0x16, 0x3, 0x2, 0x26, 0x29, 0x4, 
    0x2, 0x34, 0x34, 0x67, 0x67, 0x1c, 0x2, 0x19, 0x19, 0x1c, 0x1c, 0x1f, 
    0x21, 0x23, 0x24, 0x26, 0x2e, 0x30, 0x33, 0x35, 0x35, 0x3c, 0x3c, 0x3e, 
    0x3e, 0x40, 0x43, 0x45, 0x45, 0x4a, 0x4c, 0x50, 0x50, 0x54, 0x54, 0x56, 
    0x57, 0x5a, 0x5c, 0x5e, 0x60, 0x62, 0x62, 0x68, 0x6d, 0x71, 0x76, 0x78, 
    0x79, 0x7c, 0x7d, 0x7f, 0x85, 0x88, 0x8c, 0x8f, 0x93, 0x9a, 0xb1, 0x2, 
    0x77f, 0x2, 0xbe, 0x3, 0x2, 0x2, 0x2, 0x4, 0xc6, 0x3, 0x2, 0x2, 0x2, 
    0x6, 0xc8, 0x3, 0x2, 0x2, 0x2, 0x8, 0xcd, 0x3, 0x2, 0x2, 0x2, 0xa, 0xd9, 
    0x3, 0x2, 0x2, 0x2, 0xc, 0xe5, 0x3, 0x2, 0x2, 0x2, 0xe, 0xf2, 0x3, 0x2, 
    0x2, 0x2, 0x10, 0x113, 0x3, 0x2, 0x2, 0x2, 0x12, 0x11e, 0x3, 0x2, 0x2, 
    0x2, 0x14, 0x149, 0x3, 0x2, 0x2, 0x2, 0x16, 0x154, 0x3, 0x2, 0x2, 0x2, 
    0x18, 0x16a, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x17b, 0x3, 0x2, 0x2, 0x2, 0x1c, 
    0x183, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x1b0, 0x3, 0x2, 0x2, 0x2, 0x20, 0x1e0, 
    0x3, 0x2, 0x2, 0x2, 0x22, 0x1e9, 0x3, 0x2, 0x2, 0x2, 0x24, 0x1eb, 0x3, 
    0x2, 0x2, 0x2, 0x26, 0x1ff, 0x3, 0x2, 0x2, 0x2, 0x28, 0x216, 0x3, 0x2, 
    0x2, 0x2, 0x2a, 0x238, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x24e, 0x3, 0x2, 0x2, 
    0x2, 0x2e, 0x268, 0x3, 0x2, 0x2, 0x2, 0x30, 0x26a, 0x3, 0x2, 0x2, 0x2, 
    0x32, 0x292, 0x3, 0x2, 0x2, 0x2, 0x34, 0x2c2, 0x3, 0x2, 0x2, 0x2, 0x36, 
    0x2c4, 0x3, 0x2, 0x2, 0x2, 0x38, 0x2ca, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x2e6, 
    0x3, 0x2, 0x2, 0x2, 0x3c, 0x2fb, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x315, 0x3, 
    0x2, 0x2, 0x2, 0x40, 0x343, 0x3, 0x2, 0x2, 0x2, 0x42, 0x346, 0x3, 0x2, 
    0x2, 0x2, 0x44, 0x365, 0x3, 0x2, 0x2, 0x2, 0x46, 0x36d, 0x3, 0x2, 0x2, 
    0x2, 0x48, 0x3ca, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x3cc, 0x3, 0x2, 0x2, 0x2, 
    0x4c, 0x3e3, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x3e5, 0x3, 0x2, 0x2, 0x2, 0x50, 
    0x3f5, 0x3, 0x2, 0x2, 0x2, 0x52, 0x3f7, 0x3, 0x2, 0x2, 0x2, 0x54, 0x3ff, 
    0x3, 0x2, 0x2, 0x2, 0x56, 0x401, 0x3, 0x2, 0x2, 0x2, 0x58, 0x409, 0x3, 
    0x2, 0x2, 0x2, 0x5a, 0x40d, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x415, 0x3, 0x2, 
    0x2, 0x2, 0x5e, 0x417, 0x3, 0x2, 0x2, 0x2, 0x60, 0x41f, 0x3, 0x2, 0x2, 
    0x2, 0x62, 0x421, 0x3, 0x2, 0x2, 0x2, 0x64, 0x429, 0x3, 0x2, 0x2, 0x2, 
    0x66, 0x42e, 0x3, 0x2, 0x2, 0x2, 0x68, 0x436, 0x3, 0x2, 0x2, 0x2, 0x6a, 
    0x438, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x43a, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x543, 
    0x3, 0x2, 0x2, 0x2, 0x70, 0x59b, 0x3, 0x2, 0x2, 0x2, 0x72, 0x5aa, 0x3, 
    0x2, 0x2, 0x2, 0x74, 0x5bb, 0x3, 0x2, 0x2, 0x2, 0x76, 0x5c4, 0x3, 0x2, 
    0x2, 0x2, 0x78, 0x5c9, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x5cb, 0x3, 0x2, 0x2, 
    0x2, 0x7c, 0x5cd, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x5cf, 0x3, 0x2, 0x2, 0x2, 
    0x80, 0x5d1, 0x3, 0x2, 0x2, 0x2, 0x82, 0x5d3, 0x3, 0x2, 0x2, 0x2, 0x84, 
    0x5d5, 0x3, 0x2, 0x2, 0x2, 0x86, 0x5d7, 0x3, 0x2, 0x2, 0x2, 0x88, 0x5d9, 
    0x3, 0x2, 0x2, 0x2, 0x8a, 0x5db, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x5dd, 0x3, 
    0x2, 0x2, 0x2, 0x8e, 0x5df, 0x3, 0x2, 0x2, 0x2, 0x90, 0x5e3, 0x3, 0x2, 
    0x2, 0x2, 0x92, 0x5e7, 0x3, 0x2, 0x2, 0x2, 0x94, 0x5eb, 0x3, 0x2, 0x2, 
    0x2, 0x96, 0x5ed, 0x3, 0x2, 0x2, 0x2, 0x98, 0x5ef, 0x3, 0x2, 0x2, 0x2, 
    0x9a, 0x5f1, 0x3, 0x2, 0x2, 0x2, 0x9c, 0x5f3, 0x3, 0x2, 0x2, 0x2, 0x9e, 
    0x5ff, 0x3, 0x2, 0x2, 0x2, 0xa0, 0x607, 0x3, 0x2, 0x2, 0x2, 0xa2, 0x609, 
    0x3, 0x2, 0x2, 0x2, 0xa4, 0x60b, 0x3, 0x2, 0x2, 0x2, 0xa6, 0x60d, 0x3, 
    0x2, 0x2, 0x2, 0xa8, 0x60f, 0x3, 0x2, 0x2, 0x2, 0xaa, 0x611, 0x3, 0x2, 
    0x2, 0x2, 0xac, 0x614, 0x3, 0x2, 0x2, 0x2, 0xae, 0x617, 0x3, 0x2, 0x2, 
    0x2, 0xb0, 0x61a, 0x3, 0x2, 0x2, 0x2, 0xb2, 0x61f, 0x3, 0x2, 0x2, 0x2, 
    0xb4, 0x621, 0x3, 0x2, 0x2, 0x2, 0xb6, 0x624, 0x3, 0x2, 0x2, 0x2, 0xb8, 
    0x63f, 0x3, 0x2, 0x2, 0x2, 0xba, 0x697, 0x3, 0x2, 0x2, 0x2, 0xbc, 0xbd, 
    0x7, 0x19, 0x2, 0x2, 0xbd, 0xbf, 0x7, 0x3, 0x2, 0x2, 0xbe, 0xbc, 0x3, 
    0x2, 0x2, 0x2, 0xbe, 0xbf, 0x3, 0x2, 0x2, 0x2, 0xbf, 0xc0, 0x3, 0x2, 
    0x2, 0x2, 0xc0, 0xc1, 0x5, 0x4, 0x3, 0x2, 0xc1, 0xc2, 0x7, 0x4, 0x2, 
    0x2, 0xc2, 0x3, 0x3, 0x2, 0x2, 0x2, 0xc3, 0xc7, 0x5, 0x6, 0x4, 0x2, 
    0xc4, 0xc7, 0x5, 0x8, 0x5, 0x2, 0xc5, 0xc7, 0x5, 0xa, 0x6, 0x2, 0xc6, 
    0xc3, 0x3, 0x2, 0x2, 0x2, 0xc6, 0xc4, 0x3, 0x2, 0x2, 0x2, 0xc6, 0xc5, 
    0x3, 0x2, 0x2, 0x2, 0xc7, 0x5, 0x3, 0x2, 0x2, 0x2, 0xc8, 0xc9, 0x5, 
    0xc, 0x7, 0x2, 0xc9, 0x7, 0x3, 0x2, 0x2, 0x2, 0xca, 0xcc, 0x5, 0xe, 
    0x8, 0x2, 0xcb, 0xca, 0x3, 0x2, 0x2, 0x2, 0xcc, 0xcf, 0x3, 0x2, 0x2, 
    0x2, 0xcd, 0xcb, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xce, 0x3, 0x2, 0x2, 0x2, 
    0xce, 0xd0, 0x3, 0x2, 0x2, 0x2, 0xcf, 0xcd, 0x3, 0x2, 0x2, 0x2, 0xd0, 
    0xd2, 0x7, 0x33, 0x2, 0x2, 0xd1, 0xd3, 0x7, 0x17, 0x2, 0x2, 0xd2, 0xd1, 
    0x3, 0x2, 0x2, 0x2, 0xd2, 0xd3, 0x3, 0x2, 0x2, 0x2, 0xd3, 0xd4, 0x3, 
    0x2, 0x2, 0x2, 0xd4, 0xd5, 0x5, 0x4, 0x3, 0x2, 0xd5, 0x9, 0x3, 0x2, 
    0x2, 0x2, 0xd6, 0xd8, 0x5, 0xe, 0x8, 0x2, 0xd7, 0xd6, 0x3, 0x2, 0x2, 
    0x2, 0xd8, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xd9, 0xd7, 0x3, 0x2, 0x2, 0x2, 
    0xd9, 0xda, 0x3, 0x2, 0x2, 0x2, 0xda, 0xdc, 0x3, 0x2, 0x2, 0x2, 0xdb, 
    0xd9, 0x3, 0x2, 0x2, 0x2, 0xdc, 0xde, 0x9, 0x2, 0x2, 0x2, 0xdd, 0xdf, 
    0x7, 0x17, 0x2, 0x2, 0xde, 0xdd, 0x3, 0x2, 0x2, 0x2, 0xde, 0xdf, 0x3, 
    0x2, 0x2, 0x2, 0xdf, 0xe0, 0x3, 0x2, 0x2, 0x2, 0xe0, 0xe1, 0x5, 0x4, 
    0x3, 0x2, 0xe1, 0xb, 0x3, 0x2, 0x2, 0x2, 0xe2, 0xe4, 0x5, 0xe, 0x8, 
    0x2, 0xe3, 0xe2, 0x3, 0x2, 0x2, 0x2, 0xe4, 0xe7, 0x3, 0x2, 0x2, 0x2, 
    0xe5, 0xe3, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xe6, 0x3, 0x2, 0x2, 0x2, 0xe6, 
    0xf0, 0x3, 0x2, 0x2, 0x2, 0xe7, 0xe5, 0x3, 0x2, 0x2, 0x2, 0xe8, 0xf1, 
    0x5, 0x12, 0xa, 0x2, 0xe9, 0xf1, 0x5, 0x16, 0xc, 0x2, 0xea, 0xf1, 0x5, 
    0x28, 0x15, 0x2, 0xeb, 0xf1, 0x5, 0x2a, 0x16, 0x2, 0xec, 0xf1, 0x5, 
    0x2c, 0x17, 0x2, 0xed, 0xf1, 0x5, 0x2e, 0x18, 0x2, 0xee, 0xf1, 0x5, 
    0x34, 0x1b, 0x2, 0xef, 0xf1, 0x5, 0x36, 0x1c, 0x2, 0xf0, 0xe8, 0x3, 
    0x2, 0x2, 0x2, 0xf0, 0xe9, 0x3, 0x2, 0x2, 0x2, 0xf0, 0xea, 0x3, 0x2, 
    0x2, 0x2, 0xf0, 0xeb, 0x3, 0x2, 0x2, 0x2, 0xf0, 0xec, 0x3, 0x2, 0x2, 
    0x2, 0xf0, 0xed, 0x3, 0x2, 0x2, 0x2, 0xf0, 0xee, 0x3, 0x2, 0x2, 0x2, 
    0xf0, 0xef, 0x3, 0x2, 0x2, 0x2, 0xf1, 0xd, 0x3, 0x2, 0x2, 0x2, 0xf2, 
    0xf4, 0x7, 0xbc, 0x2, 0x2, 0xf3, 0xf5, 0x5, 0x10, 0x9, 0x2, 0xf4, 0xf3, 
    0x3, 0x2, 0x2, 0x2, 0xf4, 0xf5, 0x3, 0x2, 0x2, 0x2, 0xf5, 0xfa, 0x3, 
    0x2, 0x2, 0x2, 0xf6, 0xf7, 0x7, 0x5, 0x2, 0x2, 0xf7, 0xf9, 0x5, 0x10, 
    0x9, 0x2, 0xf8, 0xf6, 0x3, 0x2, 0x2, 0x2, 0xf9, 0xfc, 0x3, 0x2, 0x2, 
    0x2, 0xfa, 0xf8, 0x3, 0x2, 0x2, 0x2, 0xfa, 0xfb, 0x3, 0x2, 0x2, 0x2, 
    0xfb, 0xfd, 0x3, 0x2, 0x2, 0x2, 0xfc, 0xfa, 0x3, 0x2, 0x2, 0x2, 0xfd, 
    0xfe, 0x7, 0xbd, 0x2, 0x2, 0xfe, 0xf, 0x3, 0x2, 0x2, 0x2, 0xff, 0x114, 
    0x5, 0x8e, 0x48, 0x2, 0x100, 0x101, 0x5, 0x8e, 0x48, 0x2, 0x101, 0x102, 
    0x7, 0x6, 0x2, 0x2, 0x102, 0x103, 0x5, 0x92, 0x4a, 0x2, 0x103, 0x114, 
    0x3, 0x2, 0x2, 0x2, 0x104, 0x106, 0x5, 0x8e, 0x48, 0x2, 0x105, 0x107, 
    0x7, 0x6, 0x2, 0x2, 0x106, 0x105, 0x3, 0x2, 0x2, 0x2, 0x106, 0x107, 
    0x3, 0x2, 0x2, 0x2, 0x107, 0x108, 0x3, 0x2, 0x2, 0x2, 0x108, 0x109, 
    0x7, 0x7, 0x2, 0x2, 0x109, 0x10e, 0x5, 0x92, 0x4a, 0x2, 0x10a, 0x10b, 
    0x7, 0x5, 0x2, 0x2, 0x10b, 0x10d, 0x5, 0x92, 0x4a, 0x2, 0x10c, 0x10a, 
    0x3, 0x2, 0x2, 0x2, 0x10d, 0x110, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x10c, 
    0x3, 0x2, 0x2, 0x2, 0x10e, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x10f, 0x111, 
    0x3, 0x2, 0x2, 0x2, 0x110, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x111, 0x112, 
    0x7, 0x8, 0x2, 0x2, 0x112, 0x114, 0x3, 0x2, 0x2, 0x2, 0x113, 0xff, 0x3, 
    0x2, 0x2, 0x2, 0x113, 0x100, 0x3, 0x2, 0x2, 0x2, 0x113, 0x104, 0x3, 
    0x2, 0x2, 0x2, 0x114, 0x11, 0x3, 0x2, 0x2, 0x2, 0x115, 0x116, 0x7, 0x6f, 
    0x2, 0x2, 0x116, 0x11b, 0x5, 0x1c, 0xf, 0x2, 0x117, 0x118, 0x7, 0x5, 
    0x2, 0x2, 0x118, 0x11a, 0x5, 0x1c, 0xf, 0x2, 0x119, 0x117, 0x3, 0x2, 
    0x2, 0x2, 0x11a, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x11b, 0x119, 0x3, 0x2, 
    0x2, 0x2, 0x11b, 0x11c, 0x3, 0x2, 0x2, 0x2, 0x11c, 0x11f, 0x3, 0x2, 
    0x2, 0x2, 0x11d, 0x11b, 0x3, 0x2, 0x2, 0x2, 0x11e, 0x115, 0x3, 0x2, 
    0x2, 0x2, 0x11e, 0x11f, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x120, 0x3, 0x2, 
    0x2, 0x2, 0x120, 0x124, 0x5, 0x14, 0xb, 0x2, 0x121, 0x122, 0x7, 0x53, 
    0x2, 0x2, 0x122, 0x123, 0x7, 0x20, 0x2, 0x2, 0x123, 0x125, 0x5, 0x56, 
    0x2c, 0x2, 0x124, 0x121, 0x3, 0x2, 0x2, 0x2, 0x124, 0x125, 0x3, 0x2, 
    0x2, 0x2, 0x125, 0x12c, 0x3, 0x2, 0x2, 0x2, 0x126, 0x127, 0x7, 0x47, 
    0x2, 0x2, 0x127, 0x12a, 0x7, 0xbb, 0x2, 0x2, 0x128, 0x129, 0x7, 0x50, 
    0x2, 0x2, 0x129, 0x12b, 0x7, 0xbb, 0x2, 0x2, 0x12a, 0x128, 0x3, 0x2, 
    0x2, 0x2, 0x12a, 0x12b, 0x3, 0x2, 0x2, 0x2, 0x12b, 0x12d, 0x3, 0x2, 
    0x2, 0x2, 0x12c, 0x126, 0x3, 0x2, 0x2, 0x2, 0x12c, 0x12d, 0x3, 0x2, 
    0x2, 0x2, 0x12d, 0x13, 0x3, 0x2, 0x2, 0x2, 0x12e, 0x131, 0x9, 0x3, 0x2, 
    0x2, 0x12f, 0x132, 0x7, 0x17, 0x2, 0x2, 0x130, 0x132, 0x7, 0x2f, 0x2, 
    0x2, 0x131, 0x12f, 0x3, 0x2, 0x2, 0x2, 0x131, 0x130, 0x3, 0x2, 0x2, 
    0x2, 0x131, 0x132, 0x3, 0x2, 0x2, 0x2, 0x132, 0x133, 0x3, 0x2, 0x2, 
    0x2, 0x133, 0x136, 0x5, 0x4e, 0x28, 0x2, 0x134, 0x135, 0x7, 0x38, 0x2, 
    0x2, 0x135, 0x137, 0x5, 0x1e, 0x10, 0x2, 0x136, 0x134, 0x3, 0x2, 0x2, 
    0x2, 0x136, 0x137, 0x3, 0x2, 0x2, 0x2, 0x137, 0x13a, 0x3, 0x2, 0x2, 
    0x2, 0x138, 0x139, 0x7, 0x6e, 0x2, 0x2, 0x139, 0x13b, 0x5, 0x68, 0x35, 
    0x2, 0x13a, 0x138, 0x3, 0x2, 0x2, 0x2, 0x13a, 0x13b, 0x3, 0x2, 0x2, 
    0x2, 0x13b, 0x13f, 0x3, 0x2, 0x2, 0x2, 0x13c, 0x13d, 0x7, 0x39, 0x2, 
    0x2, 0x13d, 0x13e, 0x7, 0x20, 0x2, 0x2, 0x13e, 0x140, 0x5, 0x52, 0x2a, 
    0x2, 0x13f, 0x13c, 0x3, 0x2, 0x2, 0x2, 0x13f, 0x140, 0x3, 0x2, 0x2, 
    0x2, 0x140, 0x143, 0x3, 0x2, 0x2, 0x2, 0x141, 0x142, 0x7, 0x3a, 0x2, 
    0x2, 0x142, 0x144, 0x5, 0x68, 0x35, 0x2, 0x143, 0x141, 0x3, 0x2, 0x2, 
    0x2, 0x143, 0x144, 0x3, 0x2, 0x2, 0x2, 0x144, 0x14a, 0x3, 0x2, 0x2, 
    0x2, 0x145, 0x146, 0x7, 0x7, 0x2, 0x2, 0x146, 0x147, 0x5, 0x14, 0xb, 
    0x2, 0x147, 0x148, 0x7, 0x8, 0x2, 0x2, 0x148, 0x14a, 0x3, 0x2, 0x2, 
    0x2, 0x149, 0x12e, 0x3, 0x2, 0x2, 0x2, 0x149, 0x145, 0x3, 0x2, 0x2, 
    0x2, 0x14a, 0x15, 0x3, 0x2, 0x2, 0x2, 0x14b, 0x14c, 0x7, 0x6f, 0x2, 
    0x2, 0x14c, 0x151, 0x5, 0x1c, 0xf, 0x2, 0x14d, 0x14e, 0x7, 0x5, 0x2, 
    0x2, 0x14e, 0x150, 0x5, 0x1c, 0xf, 0x2, 0x14f, 0x14d, 0x3, 0x2, 0x2, 
    0x2, 0x150, 0x153, 0x3, 0x2, 0x2, 0x2, 0x151, 0x14f, 0x3, 0x2, 0x2, 
    0x2, 0x151, 0x152, 0x3, 0x2, 0x2, 0x2, 0x152, 0x155, 0x3, 0x2, 0x2, 
    0x2, 0x153, 0x151, 0x3, 0x2, 0x2, 0x2, 0x154, 0x14b, 0x3, 0x2, 0x2, 
    0x2, 0x154, 0x155, 0x3, 0x2, 0x2, 0x2, 0x155, 0x156, 0x3, 0x2, 0x2, 
    0x2, 0x156, 0x15a, 0x5, 0x18, 0xd, 0x2, 0x157, 0x158, 0x7, 0x53, 0x2, 
    0x2, 0x158, 0x159, 0x7, 0x20, 0x2, 0x2, 0x159, 0x15b, 0x5, 0x56, 0x2c, 
    0x2, 0x15a, 0x157, 0x3, 0x2, 0x2, 0x2, 0x15a, 0x15b, 0x3, 0x2, 0x2, 
    0x2, 0x15b, 0x162, 0x3, 0x2, 0x2, 0x2, 0x15c, 0x15d, 0x7, 0x47, 0x2, 
    0x2, 0x15d, 0x160, 0x7, 0xbb, 0x2, 0x2, 0x15e, 0x15f, 0x7, 0x50, 0x2, 
    0x2, 0x15f, 0x161, 0x7, 0xbb, 0x2, 0x2, 0x160, 0x15e, 0x3, 0x2, 0x2, 
    0x2, 0x160, 0x161, 0x3, 0x2, 0x2, 0x2, 0x161, 0x163, 0x3, 0x2, 0x2, 
    0x2, 0x162, 0x15c, 0x3, 0x2, 0x2, 0x2, 0x162, 0x163, 0x3, 0x2, 0x2, 
    0x2, 0x163, 0x17, 0x3, 0x2, 0x2, 0x2, 0x164, 0x165, 0x8, 0xd, 0x1, 0x2, 
    0x165, 0x16b, 0x5, 0x14, 0xb, 0x2, 0x166, 0x167, 0x7, 0x7, 0x2, 0x2, 
    0x167, 0x168, 0x5, 0x18, 0xd, 0x2, 0x168, 0x169, 0x7, 0x8, 0x2, 0x2, 
    0x169, 0x16b, 0x3, 0x2, 0x2, 0x2, 0x16a, 0x164, 0x3, 0x2, 0x2, 0x2, 
    0x16a, 0x166, 0x3, 0x2, 0x2, 0x2, 0x16b, 0x178, 0x3, 0x2, 0x2, 0x2, 
    0x16c, 0x16d, 0xc, 0x4, 0x2, 0x2, 0x16d, 0x170, 0x9, 0x4, 0x2, 0x2, 
    0x16e, 0x171, 0x7, 0x2f, 0x2, 0x2, 0x16f, 0x171, 0x7, 0x17, 0x2, 0x2, 
    0x170, 0x16e, 0x3, 0x2, 0x2, 0x2, 0x170, 0x16f, 0x3, 0x2, 0x2, 0x2, 
    0x170, 0x171, 0x3, 0x2, 0x2, 0x2, 0x171, 0x173, 0x3, 0x2, 0x2, 0x2, 
    0x172, 0x174, 0x5, 0x1a, 0xe, 0x2, 0x173, 0x172, 0x3, 0x2, 0x2, 0x2, 
    0x173, 0x174, 0x3, 0x2, 0x2, 0x2, 0x174, 0x175, 0x3, 0x2, 0x2, 0x2, 
    0x175, 0x177, 0x5, 0x18, 0xd, 0x5, 0x176, 0x16c, 0x3, 0x2, 0x2, 0x2, 
    0x177, 0x17a, 0x3, 0x2, 0x2, 0x2, 0x178, 0x176, 0x3, 0x2, 0x2, 0x2, 
    0x178, 0x179, 0x3, 0x2, 0x2, 0x2, 0x179, 0x19, 0x3, 0x2, 0x2, 0x2, 0x17a, 
    0x178, 0x3, 0x2, 0x2, 0x2, 0x17b, 0x181, 0x7, 0x23, 0x2, 0x2, 0x17c, 
    0x17d, 0x7, 0x20, 0x2, 0x2, 0x17d, 0x17e, 0x7, 0x7, 0x2, 0x2, 0x17e, 
    0x17f, 0x5, 0x5e, 0x30, 0x2, 0x17f, 0x180, 0x7, 0x8, 0x2, 0x2, 0x180, 
    0x182, 0x3, 0x2, 0x2, 0x2, 0x181, 0x17c, 0x3, 0x2, 0x2, 0x2, 0x181, 
    0x182, 0x3, 0x2, 0x2, 0x2, 0x182, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x183, 0x18f, 
    0x5, 0x80, 0x41, 0x2, 0x184, 0x185, 0x7, 0x7, 0x2, 0x2, 0x185, 0x18a, 
    0x5, 0x86, 0x44, 0x2, 0x186, 0x187, 0x7, 0x5, 0x2, 0x2, 0x187, 0x189, 
    0x5, 0x86, 0x44, 0x2, 0x188, 0x186, 0x3, 0x2, 0x2, 0x2, 0x189, 0x18c, 
    0x3, 0x2, 0x2, 0x2, 0x18a, 0x188, 0x3, 0x2, 0x2, 0x2, 0x18a, 0x18b, 
    0x3, 0x2, 0x2, 0x2, 0x18b, 0x18d, 0x3, 0x2, 0x2, 0x2, 0x18c, 0x18a, 
    0x3, 0x2, 0x2, 0x2, 0x18d, 0x18e, 0x7, 0x8, 0x2, 0x2, 0x18e, 0x190, 
    0x3, 0x2, 0x2, 0x2, 0x18f, 0x184, 0x3, 0x2, 0x2, 0x2, 0x18f, 0x190, 
    0x3, 0x2, 0x2, 0x2, 0x190, 0x191, 0x3, 0x2, 0x2, 0x2, 0x191, 0x192, 
    0x7, 0x1b, 0x2, 0x2, 0x192, 0x193, 0x7, 0x7, 0x2, 0x2, 0x193, 0x194, 
    0x5, 0xc, 0x7, 0x2, 0x194, 0x195, 0x7, 0x8, 0x2, 0x2, 0x195, 0x1d, 0x3, 
    0x2, 0x2, 0x2, 0x196, 0x197, 0x8, 0x10, 0x1, 0x2, 0x197, 0x19c, 0x5, 
    0x20, 0x11, 0x2, 0x198, 0x19a, 0x7, 0x1b, 0x2, 0x2, 0x199, 0x198, 0x3, 
    0x2, 0x2, 0x2, 0x199, 0x19a, 0x3, 0x2, 0x2, 0x2, 0x19a, 0x19b, 0x3, 
    0x2, 0x2, 0x2, 0x19b, 0x19d, 0x5, 0x82, 0x42, 0x2, 0x19c, 0x199, 0x3, 
    0x2, 0x2, 0x2, 0x19c, 0x19d, 0x3, 0x2, 0x2, 0x2, 0x19d, 0x1b1, 0x3, 
    0x2, 0x2, 0x2, 0x19e, 0x19f, 0x7, 0x7, 0x2, 0x2, 0x19f, 0x1a0, 0x5, 
    0xc, 0x7, 0x2, 0x1a0, 0x1a5, 0x7, 0x8, 0x2, 0x2, 0x1a1, 0x1a3, 0x7, 
    0x1b, 0x2, 0x2, 0x1a2, 0x1a1, 0x3, 0x2, 0x2, 0x2, 0x1a2, 0x1a3, 0x3, 
    0x2, 0x2, 0x2, 0x1a3, 0x1a4, 0x3, 0x2, 0x2, 0x2, 0x1a4, 0x1a6, 0x5, 
    0x82, 0x42, 0x2, 0x1a5, 0x1a2, 0x3, 0x2, 0x2, 0x2, 0x1a5, 0x1a6, 0x3, 
    0x2, 0x2, 0x2, 0x1a6, 0x1b1, 0x3, 0x2, 0x2, 0x2, 0x1a7, 0x1a8, 0x7, 
    0x7, 0x2, 0x2, 0x1a8, 0x1a9, 0x5, 0x26, 0x14, 0x2, 0x1a9, 0x1ae, 0x7, 
    0x8, 0x2, 0x2, 0x1aa, 0x1ac, 0x7, 0x1b, 0x2, 0x2, 0x1ab, 0x1aa, 0x3, 
    0x2, 0x2, 0x2, 0x1ab, 0x1ac, 0x3, 0x2, 0x2, 0x2, 0x1ac, 0x1ad, 0x3, 
    0x2, 0x2, 0x2, 0x1ad, 0x1af, 0x5, 0x82, 0x42, 0x2, 0x1ae, 0x1ab, 0x3, 
    0x2, 0x2, 0x2, 0x1ae, 0x1af, 0x3, 0x2, 0x2, 0x2, 0x1af, 0x1b1, 0x3, 
    0x2, 0x2, 0x2, 0x1b0, 0x196, 0x3, 0x2, 0x2, 0x2, 0x1b0, 0x19e, 0x3, 
    0x2, 0x2, 0x2, 0x1b0, 0x1a7, 0x3, 0x2, 0x2, 0x2, 0x1b1, 0x1d5, 0x3, 
    0x2, 0x2, 0x2, 0x1b2, 0x1b6, 0xc, 0x5, 0x2, 0x2, 0x1b3, 0x1b7, 0x7, 
    0x5, 0x2, 0x2, 0x1b4, 0x1b5, 0x7, 0x25, 0x2, 0x2, 0x1b5, 0x1b7, 0x7, 
    0x42, 0x2, 0x2, 0x1b6, 0x1b3, 0x3, 0x2, 0x2, 0x2, 0x1b6, 0x1b4, 0x3, 
    0x2, 0x2, 0x2, 0x1b7, 0x1b8, 0x3, 0x2, 0x2, 0x2, 0x1b8, 0x1d4, 0x5, 
    0x1e, 0x10, 0x6, 0x1b9, 0x1bb, 0xc, 0x7, 0x2, 0x2, 0x1ba, 0x1bc, 0x5, 
    0x22, 0x12, 0x2, 0x1bb, 0x1ba, 0x3, 0x2, 0x2, 0x2, 0x1bb, 0x1bc, 0x3, 
    0x2, 0x2, 0x2, 0x1bc, 0x1be, 0x3, 0x2, 0x2, 0x2, 0x1bd, 0x1bf, 0x5, 
    0x24, 0x13, 0x2, 0x1be, 0x1bd, 0x3, 0x2, 0x2, 0x2, 0x1be, 0x1bf, 0x3, 
    0x2, 0x2, 0x2, 0x1bf, 0x1c0, 0x3, 0x2, 0x2, 0x2, 0x1c0, 0x1c1, 0x7, 
    0x42, 0x2, 0x2, 0x1c1, 0x1c2, 0x5, 0x1e, 0x10, 0x2, 0x1c2, 0x1c3, 0x7, 
    0x51, 0x2, 0x2, 0x1c3, 0x1c4, 0x5, 0x6a, 0x36, 0x2, 0x1c4, 0x1d4, 0x3, 
    0x2, 0x2, 0x2, 0x1c5, 0x1c7, 0xc, 0x6, 0x2, 0x2, 0x1c6, 0x1c8, 0x5, 
    0x22, 0x12, 0x2, 0x1c7, 0x1c6, 0x3, 0x2, 0x2, 0x2, 0x1c7, 0x1c8, 0x3, 
    0x2, 0x2, 0x2, 0x1c8, 0x1ca, 0x3, 0x2, 0x2, 0x2, 0x1c9, 0x1cb, 0x5, 
    0x24, 0x13, 0x2, 0x1ca, 0x1c9, 0x3, 0x2, 0x2, 0x2, 0x1ca, 0x1cb, 0x3, 
    0x2, 0x2, 0x2, 0x1cb, 0x1cc, 0x3, 0x2, 0x2, 0x2, 0x1cc, 0x1cd, 0x7, 
    0x42, 0x2, 0x2, 0x1cd, 0x1ce, 0x5, 0x1e, 0x10, 0x2, 0x1ce, 0x1cf, 0x7, 
    0x6c, 0x2, 0x2, 0x1cf, 0x1d0, 0x7, 0x7, 0x2, 0x2, 0x1d0, 0x1d1, 0x5, 
    0x5a, 0x2e, 0x2, 0x1d1, 0x1d2, 0x7, 0x8, 0x2, 0x2, 0x1d2, 0x1d4, 0x3, 
    0x2, 0x2, 0x2, 0x1d3, 0x1b2, 0x3, 0x2, 0x2, 0x2, 0x1d3, 0x1b9, 0x3, 
    0x2, 0x2, 0x2, 0x1d3, 0x1c5, 0x3, 0x2, 0x2, 0x2, 0x1d4, 0x1d7, 0x3, 
    0x2, 0x2, 0x2, 0x1d5, 0x1d3, 0x3, 0x2, 0x2, 0x2, 0x1d5, 0x1d6, 0x3, 
    0x2, 0x2, 0x2, 0x1d6, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x1d7, 0x1d5, 0x3, 0x2, 
    0x2, 0x2, 0x1d8, 0x1d9, 0x5, 0x7c, 0x3f, 0x2, 0x1d9, 0x1da, 0x7, 0xa, 
    0x2, 0x2, 0x1da, 0x1dc, 0x3, 0x2, 0x2, 0x2, 0x1db, 0x1d8, 0x3, 0x2, 
    0x2, 0x2, 0x1db, 0x1dc, 0x3, 0x2, 0x2, 0x2, 0x1dc, 0x1dd, 0x3, 0x2, 
    0x2, 0x2, 0x1dd, 0x1de, 0x5, 0x7e, 0x40, 0x2, 0x1de, 0x1df, 0x7, 0xa, 
    0x2, 0x2, 0x1df, 0x1e1, 0x3, 0x2, 0x2, 0x2, 0x1e0, 0x1db, 0x3, 0x2, 
    0x2, 0x2, 0x1e0, 0x1e1, 0x3, 0x2, 0x2, 0x2, 0x1e1, 0x1e2, 0x3, 0x2, 
    0x2, 0x2, 0x1e2, 0x1e3, 0x5, 0x80, 0x41, 0x2, 0x1e3, 0x21, 0x3, 0x2, 
    0x2, 0x2, 0x1e4, 0x1ea, 0x7, 0x3d, 0x2, 0x2, 0x1e5, 0x1e7, 0x9, 0x5, 
    0x2, 0x2, 0x1e6, 0x1e8, 0x7, 0x54, 0x2, 0x2, 0x1e7, 0x1e6, 0x3, 0x2, 
    0x2, 0x2, 0x1e7, 0x1e8, 0x3, 0x2, 0x2, 0x2, 0x1e8, 0x1ea, 0x3, 0x2, 
    0x2, 0x2, 0x1e9, 0x1e4, 0x3, 0x2, 0x2, 0x2, 0x1e9, 0x1e5, 0x3, 0x2, 
    0x2, 0x2, 0x1ea, 0x23, 0x3, 0x2, 0x2, 0x2, 0x1eb, 0x1ec, 0x7, 0xbc, 
    0x2, 0x2, 0x1ec, 0x1fb, 0x9, 0x6, 0x2, 0x2, 0x1ed, 0x1ef, 0x7, 0x6, 
    0x2, 0x2, 0x1ee, 0x1ed, 0x3, 0x2, 0x2, 0x2, 0x1ee, 0x1ef, 0x3, 0x2, 
    0x2, 0x2, 0x1ef, 0x1f0, 0x3, 0x2, 0x2, 0x2, 0x1f0, 0x1f1, 0x7, 0x7, 
    0x2, 0x2, 0x1f1, 0x1f6, 0x5, 0x92, 0x4a, 0x2, 0x1f2, 0x1f3, 0x7, 0x5, 
    0x2, 0x2, 0x1f3, 0x1f5, 0x5, 0x92, 0x4a, 0x2, 0x1f4, 0x1f2, 0x3, 0x2, 
    0x2, 0x2, 0x1f5, 0x1f8, 0x3, 0x2, 0x2, 0x2, 0x1f6, 0x1f4, 0x3, 0x2, 
    0x2, 0x2, 0x1f6, 0x1f7, 0x3, 0x2, 0x2, 0x2, 0x1f7, 0x1f9, 0x3, 0x2, 
    0x2, 0x2, 0x1f8, 0x1f6, 0x3, 0x2, 0x2, 0x2, 0x1f9, 0x1fa, 0x7, 0x8, 
    0x2, 0x2, 0x1fa, 0x1fc, 0x3, 0x2, 0x2, 0x2, 0x1fb, 0x1ee, 0x3, 0x2, 
    0x2, 0x2, 0x1fb, 0x1fc, 0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1fd, 0x3, 0x2, 
    0x2, 0x2, 0x1fd, 0x1fe, 0x7, 0xbd, 0x2, 0x2, 0x1fe, 0x25, 0x3, 0x2, 
    0x2, 0x2, 0x1ff, 0x200, 0x7, 0x6d, 0x2, 0x2, 0x200, 0x201, 0x7, 0x7, 
    0x2, 0x2, 0x201, 0x202, 0x5, 0x66, 0x34, 0x2, 0x202, 0x20a, 0x7, 0x8, 
    0x2, 0x2, 0x203, 0x204, 0x7, 0x5, 0x2, 0x2, 0x204, 0x205, 0x7, 0x7, 
    0x2, 0x2, 0x205, 0x206, 0x5, 0x66, 0x34, 0x2, 0x206, 0x207, 0x7, 0x8, 
    0x2, 0x2, 0x207, 0x209, 0x3, 0x2, 0x2, 0x2, 0x208, 0x203, 0x3, 0x2, 
    0x2, 0x2, 0x209, 0x20c, 0x3, 0x2, 0x2, 0x2, 0x20a, 0x208, 0x3, 0x2, 
    0x2, 0x2, 0x20a, 0x20b, 0x3, 0x2, 0x2, 0x2, 0x20b, 0x27, 0x3, 0x2, 0x2, 
    0x2, 0x20c, 0x20a, 0x3, 0x2, 0x2, 0x2, 0x20d, 0x20e, 0x7, 0x6f, 0x2, 
    0x2, 0x20e, 0x213, 0x5, 0x1c, 0xf, 0x2, 0x20f, 0x210, 0x7, 0x5, 0x2, 
    0x2, 0x210, 0x212, 0x5, 0x1c, 0xf, 0x2, 0x211, 0x20f, 0x3, 0x2, 0x2, 
    0x2, 0x212, 0x215, 0x3, 0x2, 0x2, 0x2, 0x213, 0x211, 0x3, 0x2, 0x2, 
    0x2, 0x213, 0x214, 0x3, 0x2, 0x2, 0x2, 0x214, 0x217, 0x3, 0x2, 0x2, 
    0x2, 0x215, 0x213, 0x3, 0x2, 0x2, 0x2, 0x216, 0x20d, 0x3, 0x2, 0x2, 
    0x2, 0x216, 0x217, 0x3, 0x2, 0x2, 0x2, 0x217, 0x218, 0x3, 0x2, 0x2, 
    0x2, 0x218, 0x21a, 0x7, 0x3e, 0x2, 0x2, 0x219, 0x21b, 0x7, 0x40, 0x2, 
    0x2, 0x21a, 0x219, 0x3, 0x2, 0x2, 0x2, 0x21a, 0x21b, 0x3, 0x2, 0x2, 
    0x2, 0x21b, 0x21c, 0x3, 0x2, 0x2, 0x2, 0x21c, 0x228, 0x5, 0x20, 0x11, 
    0x2, 0x21d, 0x21e, 0x7, 0x7, 0x2, 0x2, 0x21e, 0x223, 0x5, 0x84, 0x43, 
    0x2, 0x21f, 0x220, 0x7, 0x5, 0x2, 0x2, 0x220, 0x222, 0x5, 0x84, 0x43, 
    0x2, 0x221, 0x21f, 0x3, 0x2, 0x2, 0x2, 0x222, 0x225, 0x3, 0x2, 0x2, 
    0x2, 0x223, 0x221, 0x3, 0x2, 0x2, 0x2, 0x223, 0x224, 0x3, 0x2, 0x2, 
    0x2, 0x224, 0x226, 0x3, 0x2, 0x2, 0x2, 0x225, 0x223, 0x3, 0x2, 0x2, 
    0x2, 0x226, 0x227, 0x7, 0x8, 0x2, 0x2, 0x227, 0x229, 0x3, 0x2, 0x2, 
    0x2, 0x228, 0x21d, 0x3, 0x2, 0x2, 0x2, 0x228, 0x229, 0x3, 0x2, 0x2, 
    0x2, 0x229, 0x22d, 0x3, 0x2, 0x2, 0x2, 0x22a, 0x22e, 0x5, 0x26, 0x14, 
    0x2, 0x22b, 0x22e, 0x5, 0x12, 0xa, 0x2, 0x22c, 0x22e, 0x5, 0x16, 0xc, 
    0x2, 0x22d, 0x22a, 0x3, 0x2, 0x2, 0x2, 0x22d, 0x22b, 0x3, 0x2, 0x2, 
    0x2, 0x22d, 0x22c, 0x3, 0x2, 0x2, 0x2, 0x22e, 0x29, 0x3, 0x2, 0x2, 0x2, 
    0x22f, 0x230, 0x7, 0x6f, 0x2, 0x2, 0x230, 0x235, 0x5, 0x1c, 0xf, 0x2, 
    0x231, 0x232, 0x7, 0x5, 0x2, 0x2, 0x232, 0x234, 0x5, 0x1c, 0xf, 0x2, 
    0x233, 0x231, 0x3, 0x2, 0x2, 0x2, 0x234, 0x237, 0x3, 0x2, 0x2, 0x2, 
    0x235, 0x233, 0x3, 0x2, 0x2, 0x2, 0x235, 0x236, 0x3, 0x2, 0x2, 0x2, 
    0x236, 0x239, 0x3, 0x2, 0x2, 0x2, 0x237, 0x235, 0x3, 0x2, 0x2, 0x2, 
    0x238, 0x22f, 0x3, 0x2, 0x2, 0x2, 0x238, 0x239, 0x3, 0x2, 0x2, 0x2, 
    0x239, 0x23a, 0x3, 0x2, 0x2, 0x2, 0x23a, 0x23c, 0x7, 0x2c, 0x2, 0x2, 
    0x23b, 0x23d, 0x7, 0x38, 0x2, 0x2, 0x23c, 0x23b, 0x3, 0x2, 0x2, 0x2, 
    0x23c, 0x23d, 0x3, 0x2, 0x2, 0x2, 0x23d, 0x23e, 0x3, 0x2, 0x2, 0x2, 
    0x23e, 0x243, 0x5, 0x20, 0x11, 0x2, 0x23f, 0x240, 0x7, 0x6e, 0x2, 0x2, 
    0x240, 0x244, 0x5, 0x68, 0x35, 0x2, 0x241, 0x242, 0x7, 0x38, 0x2, 0x2, 
    0x242, 0x244, 0x5, 0x1e, 0x10, 0x2, 0x243, 0x23f, 0x3, 0x2, 0x2, 0x2, 
    0x243, 0x241, 0x3, 0x2, 0x2, 0x2, 0x243, 0x244, 0x3, 0x2, 0x2, 0x2, 
    0x244, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x245, 0x246, 0x7, 0x6f, 0x2, 0x2, 
    0x246, 0x24b, 0x5, 0x1c, 0xf, 0x2, 0x247, 0x248, 0x7, 0x5, 0x2, 0x2, 
    0x248, 0x24a, 0x5, 0x1c, 0xf, 0x2, 0x249, 0x247, 0x3, 0x2, 0x2, 0x2, 
    0x24a, 0x24d, 0x3, 0x2, 0x2, 0x2, 0x24b, 0x249, 0x3, 0x2, 0x2, 0x2, 
    0x24b, 0x24c, 0x3, 0x2, 0x2, 0x2, 0x24c, 0x24f, 0x3, 0x2, 0x2, 0x2, 
    0x24d, 0x24b, 0x3, 0x2, 0x2, 0x2, 0x24e, 0x245, 0x3, 0x2, 0x2, 0x2, 
    0x24e, 0x24f, 0x3, 0x2, 0x2, 0x2, 0x24f, 0x250, 0x3, 0x2, 0x2, 0x2, 
    0x250, 0x251, 0x7, 0x6b, 0x2, 0x2, 0x251, 0x252, 0x5, 0x20, 0x11, 0x2, 
    0x252, 0x253, 0x7, 0x5e, 0x2, 0x2, 0x253, 0x254, 0x5, 0x84, 0x43, 0x2, 
    0x254, 0x255, 0x7, 0x6, 0x2, 0x2, 0x255, 0x25d, 0x5, 0x6e, 0x38, 0x2, 
    0x256, 0x257, 0x7, 0x5, 0x2, 0x2, 0x257, 0x258, 0x5, 0x84, 0x43, 0x2, 
    0x258, 0x259, 0x7, 0x6, 0x2, 0x2, 0x259, 0x25a, 0x5, 0x6e, 0x38, 0x2, 
    0x25a, 0x25c, 0x3, 0x2, 0x2, 0x2, 0x25b, 0x256, 0x3, 0x2, 0x2, 0x2, 
    0x25c, 0x25f, 0x3, 0x2, 0x2, 0x2, 0x25d, 0x25b, 0x3, 0x2, 0x2, 0x2, 
    0x25d, 0x25e, 0x3, 0x2, 0x2, 0x2, 0x25e, 0x264, 0x3, 0x2, 0x2, 0x2, 
    0x25f, 0x25d, 0x3, 0x2, 0x2, 0x2, 0x260, 0x261, 0x7, 0x6e, 0x2, 0x2, 
    0x261, 0x265, 0x5, 0x68, 0x35, 0x2, 0x262, 0x263, 0x7, 0x38, 0x2, 0x2, 
    0x263, 0x265, 0x5, 0x1e, 0x10, 0x2, 0x264, 0x260, 0x3, 0x2, 0x2, 0x2, 
    0x264, 0x262, 0x3, 0x2, 0x2, 0x2, 0x264, 0x265, 0x3, 0x2, 0x2, 0x2, 
    0x265, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x266, 0x269, 0x5, 0x30, 0x19, 0x2, 
    0x267, 0x269, 0x5, 0x32, 0x1a, 0x2, 0x268, 0x266, 0x3, 0x2, 0x2, 0x2, 
    0x268, 0x267, 0x3, 0x2, 0x2, 0x2, 0x269, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x26a, 
    0x26c, 0x7, 0x24, 0x2, 0x2, 0x26b, 0x26d, 0x7, 0x5a, 0x2, 0x2, 0x26c, 
    0x26b, 0x3, 0x2, 0x2, 0x2, 0x26c, 0x26d, 0x3, 0x2, 0x2, 0x2, 0x26d, 
    0x26e, 0x3, 0x2, 0x2, 0x2, 0x26e, 0x272, 0x7, 0x62, 0x2, 0x2, 0x26f, 
    0x270, 0x7, 0x8e, 0x2, 0x2, 0x270, 0x271, 0x7, 0x4d, 0x2, 0x2, 0x271, 
    0x273, 0x7, 0x8d, 0x2, 0x2, 0x272, 0x26f, 0x3, 0x2, 0x2, 0x2, 0x272, 
    0x273, 0x3, 0x2, 0x2, 0x2, 0x273, 0x274, 0x3, 0x2, 0x2, 0x2, 0x274, 
    0x290, 0x5, 0x20, 0x11, 0x2, 0x275, 0x291, 0x5, 0x38, 0x1d, 0x2, 0x276, 
    0x278, 0x5, 0x38, 0x1d, 0x2, 0x277, 0x276, 0x3, 0x2, 0x2, 0x2, 0x277, 
    0x278, 0x3, 0x2, 0x2, 0x2, 0x278, 0x27a, 0x3, 0x2, 0x2, 0x2, 0x279, 
    0x27b, 0x7, 0x1b, 0x2, 0x2, 0x27a, 0x279, 0x3, 0x2, 0x2, 0x2, 0x27a, 
    0x27b, 0x3, 0x2, 0x2, 0x2, 0x27b, 0x27c, 0x3, 0x2, 0x2, 0x2, 0x27c, 
    0x282, 0x5, 0x4a, 0x26, 0x2, 0x27d, 0x27f, 0x7, 0x6f, 0x2, 0x2, 0x27e, 
    0x280, 0x7, 0x4b, 0x2, 0x2, 0x27f, 0x27e, 0x3, 0x2, 0x2, 0x2, 0x27f, 
    0x280, 0x3, 0x2, 0x2, 0x2, 0x280, 0x281, 0x3, 0x2, 0x2, 0x2, 0x281, 
    0x283, 0x7, 0x2a, 0x2, 0x2, 0x282, 0x27d, 0x3, 0x2, 0x2, 0x2, 0x282, 
    0x283, 0x3, 0x2, 0x2, 0x2, 0x283, 0x291, 0x3, 0x2, 0x2, 0x2, 0x284, 
    0x286, 0x5, 0x3e, 0x20, 0x2, 0x285, 0x287, 0x7, 0x1b, 0x2, 0x2, 0x286, 
    0x285, 0x3, 0x2, 0x2, 0x2, 0x286, 0x287, 0x3, 0x2, 0x2, 0x2, 0x287, 
    0x288, 0x3, 0x2, 0x2, 0x2, 0x288, 0x28e, 0x5, 0x4a, 0x26, 0x2, 0x289, 
    0x28b, 0x7, 0x6f, 0x2, 0x2, 0x28a, 0x28c, 0x7, 0x4b, 0x2, 0x2, 0x28b, 
    0x28a, 0x3, 0x2, 0x2, 0x2, 0x28b, 0x28c, 0x3, 0x2, 0x2, 0x2, 0x28c, 
    0x28d, 0x3, 0x2, 0x2, 0x2, 0x28d, 0x28f, 0x7, 0x2a, 0x2, 0x2, 0x28e, 
    0x289, 0x3, 0x2, 0x2, 0x2, 0x28e, 0x28f, 0x3, 0x2, 0x2, 0x2, 0x28f, 
    0x291, 0x3, 0x2, 0x2, 0x2, 0x290, 0x275, 0x3, 0x2, 0x2, 0x2, 0x290, 
    0x277, 0x3, 0x2, 0x2, 0x2, 0x290, 0x284, 0x3, 0x2, 0x2, 0x2, 0x291, 
    0x31, 0x3, 0x2, 0x2, 0x2, 0x292, 0x294, 0x7, 0x24, 0x2, 0x2, 0x293, 
    0x295, 0x7, 0x6a, 0x2, 0x2, 0x294, 0x293, 0x3, 0x2, 0x2, 0x2, 0x294, 
    0x295, 0x3, 0x2, 0x2, 0x2, 0x295, 0x296, 0x3, 0x2, 0x2, 0x2, 0x296, 
    0x29a, 0x9, 0x7, 0x2, 0x2, 0x297, 0x298, 0x7, 0x8e, 0x2, 0x2, 0x298, 
    0x299, 0x7, 0x4d, 0x2, 0x2, 0x299, 0x29b, 0x7, 0x8d, 0x2, 0x2, 0x29a, 
    0x297, 0x3, 0x2, 0x2, 0x2, 0x29a, 0x29b, 0x3, 0x2, 0x2, 0x2, 0x29b, 
    0x29c, 0x3, 0x2, 0x2, 0x2, 0x29c, 0x29d, 0x5, 0x8a, 0x46, 0x2, 0x29d, 
    0x29e, 0x7, 0x51, 0x2, 0x2, 0x29e, 0x2a1, 0x5, 0x20, 0x11, 0x2, 0x29f, 
    0x2a0, 0x7, 0x6c, 0x2, 0x2, 0x2a0, 0x2a2, 0x5, 0x44, 0x23, 0x2, 0x2a1, 
    0x29f, 0x3, 0x2, 0x2, 0x2, 0x2a1, 0x2a2, 0x3, 0x2, 0x2, 0x2, 0x2a2, 
    0x2a3, 0x3, 0x2, 0x2, 0x2, 0x2a3, 0x2a4, 0x7, 0x7, 0x2, 0x2, 0x2a4, 
    0x2a9, 0x5, 0x6e, 0x38, 0x2, 0x2a5, 0x2a6, 0x7, 0x5, 0x2, 0x2, 0x2a6, 
    0x2a8, 0x5, 0x6e, 0x38, 0x2, 0x2a7, 0x2a5, 0x3, 0x2, 0x2, 0x2, 0x2a8, 
    0x2ab, 0x3, 0x2, 0x2, 0x2, 0x2a9, 0x2a7, 0x3, 0x2, 0x2, 0x2, 0x2a9, 
    0x2aa, 0x3, 0x2, 0x2, 0x2, 0x2aa, 0x2ac, 0x3, 0x2, 0x2, 0x2, 0x2ab, 
    0x2a9, 0x3, 0x2, 0x2, 0x2, 0x2ac, 0x2af, 0x7, 0x8, 0x2, 0x2, 0x2ad, 
    0x2ae, 0x7, 0x6f, 0x2, 0x2, 0x2ae, 0x2b0, 0x5, 0x62, 0x32, 0x2, 0x2af, 
    0x2ad, 0x3, 0x2, 0x2, 0x2, 0x2af, 0x2b0, 0x3, 0x2, 0x2, 0x2, 0x2b0, 
    0x33, 0x3, 0x2, 0x2, 0x2, 0x2b1, 0x2b2, 0x7, 0x30, 0x2, 0x2, 0x2b2, 
    0x2b5, 0x7, 0x62, 0x2, 0x2, 0x2b3, 0x2b4, 0x7, 0x8e, 0x2, 0x2, 0x2b4, 
    0x2b6, 0x7, 0x8d, 0x2, 0x2, 0x2b5, 0x2b3, 0x3, 0x2, 0x2, 0x2, 0x2b5, 
    0x2b6, 0x3, 0x2, 0x2, 0x2, 0x2b6, 0x2b7, 0x3, 0x2, 0x2, 0x2, 0x2b7, 
    0x2c3, 0x5, 0x20, 0x11, 0x2, 0x2b8, 0x2b9, 0x7, 0x30, 0x2, 0x2, 0x2b9, 
    0x2bc, 0x9, 0x7, 0x2, 0x2, 0x2ba, 0x2bb, 0x7, 0x8e, 0x2, 0x2, 0x2bb, 
    0x2bd, 0x7, 0x8d, 0x2, 0x2, 0x2bc, 0x2ba, 0x3, 0x2, 0x2, 0x2, 0x2bc, 
    0x2bd, 0x3, 0x2, 0x2, 0x2, 0x2bd, 0x2be, 0x3, 0x2, 0x2, 0x2, 0x2be, 
    0x2bf, 0x5, 0x8a, 0x46, 0x2, 0x2bf, 0x2c0, 0x7, 0x51, 0x2, 0x2, 0x2c0, 
    0x2c1, 0x5, 0x20, 0x11, 0x2, 0x2c1, 0x2c3, 0x3, 0x2, 0x2, 0x2, 0x2c2, 
    0x2b1, 0x3, 0x2, 0x2, 0x2, 0x2c2, 0x2b8, 0x3, 0x2, 0x2, 0x2, 0x2c3, 
    0x35, 0x3, 0x2, 0x2, 0x2, 0x2c4, 0x2c6, 0x7, 0x66, 0x2, 0x2, 0x2c5, 
    0x2c7, 0x7, 0x62, 0x2, 0x2, 0x2c6, 0x2c5, 0x3, 0x2, 0x2, 0x2, 0x2c6, 
    0x2c7, 0x3, 0x2, 0x2, 0x2, 0x2c7, 0x2c8, 0x3, 0x2, 0x2, 0x2, 0x2c8, 
    0x2c9, 0x5, 0x20, 0x11, 0x2, 0x2c9, 0x37, 0x3, 0x2, 0x2, 0x2, 0x2ca, 
    0x2ce, 0x7, 0x7, 0x2, 0x2, 0x2cb, 0x2cf, 0x5, 0x3c, 0x1f, 0x2, 0x2cc, 
    0x2cf, 0x5, 0x40, 0x21, 0x2, 0x2cd, 0x2cf, 0x5, 0x42, 0x22, 0x2, 0x2ce, 
    0x2cb, 0x3, 0x2, 0x2, 0x2, 0x2ce, 0x2cc, 0x3, 0x2, 0x2, 0x2, 0x2ce, 
    0x2cd, 0x3, 0x2, 0x2, 0x2, 0x2cf, 0x2d8, 0x3, 0x2, 0x2, 0x2, 0x2d0, 
    0x2d4, 0x7, 0x5, 0x2, 0x2, 0x2d1, 0x2d5, 0x5, 0x3c, 0x1f, 0x2, 0x2d2, 
    0x2d5, 0x5, 0x40, 0x21, 0x2, 0x2d3, 0x2d5, 0x5, 0x42, 0x22, 0x2, 0x2d4, 
    0x2d1, 0x3, 0x2, 0x2, 0x2, 0x2d4, 0x2d2, 0x3, 0x2, 0x2, 0x2, 0x2d4, 
    0x2d3, 0x3, 0x2, 0x2, 0x2, 0x2d5, 0x2d7, 0x3, 0x2, 0x2, 0x2, 0x2d6, 
    0x2d0, 0x3, 0x2, 0x2, 0x2, 0x2d7, 0x2da, 0x3, 0x2, 0x2, 0x2, 0x2d8, 
    0x2d6, 0x3, 0x2, 0x2, 0x2, 0x2d8, 0x2d9, 0x3, 0x2, 0x2, 0x2, 0x2d9, 
    0x2db, 0x3, 0x2, 0x2, 0x2, 0x2da, 0x2d8, 0x3, 0x2, 0x2, 0x2, 0x2db, 
    0x2dd, 0x7, 0x8, 0x2, 0x2, 0x2dc, 0x2de, 0x7, 0x5a, 0x2, 0x2, 0x2dd, 
    0x2dc, 0x3, 0x2, 0x2, 0x2, 0x2dd, 0x2de, 0x3, 0x2, 0x2, 0x2, 0x2de, 
    0x2e0, 0x3, 0x2, 0x2, 0x2, 0x2df, 0x2e1, 0x5, 0x3a, 0x1e, 0x2, 0x2e0, 
    0x2df, 0x3, 0x2, 0x2, 0x2, 0x2e0, 0x2e1, 0x3, 0x2, 0x2, 0x2, 0x2e1, 
    0x2e4, 0x3, 0x2, 0x2, 0x2, 0x2e2, 0x2e3, 0x7, 0x6f, 0x2, 0x2, 0x2e3, 
    0x2e5, 0x5, 0x62, 0x32, 0x2, 0x2e4, 0x2e2, 0x3, 0x2, 0x2, 0x2, 0x2e4, 
    0x2e5, 0x3, 0x2, 0x2, 0x2, 0x2e5, 0x39, 0x3, 0x2, 0x2, 0x2, 0x2e6, 0x2e8, 
    0x7, 0x57, 0x2, 0x2, 0x2e7, 0x2e9, 0x7, 0x20, 0x2, 0x2, 0x2e8, 0x2e7, 
    0x3, 0x2, 0x2, 0x2, 0x2e8, 0x2e9, 0x3, 0x2, 0x2, 0x2, 0x2e9, 0x2eb, 
    0x3, 0x2, 0x2, 0x2, 0x2ea, 0x2ec, 0x9, 0x8, 0x2, 0x2, 0x2eb, 0x2ea, 
    0x3, 0x2, 0x2, 0x2, 0x2eb, 0x2ec, 0x3, 0x2, 0x2, 0x2, 0x2ec, 0x2ee, 
    0x3, 0x2, 0x2, 0x2, 0x2ed, 0x2ef, 0x7, 0x51, 0x2, 0x2, 0x2ee, 0x2ed, 
    0x3, 0x2, 0x2, 0x2, 0x2ee, 0x2ef, 0x3, 0x2, 0x2, 0x2, 0x2ef, 0x2f0, 
    0x3, 0x2, 0x2, 0x2, 0x2f0, 0x2f1, 0x7, 0x7, 0x2, 0x2, 0x2f1, 0x2f6, 
    0x5, 0x6e, 0x38, 0x2, 0x2f2, 0x2f3, 0x7, 0x5, 0x2, 0x2, 0x2f3, 0x2f5, 
    0x5, 0x6e, 0x38, 0x2, 0x2f4, 0x2f2, 0x3, 0x2, 0x2, 0x2, 0x2f5, 0x2f8, 
    0x3, 0x2, 0x2, 0x2, 0x2f6, 0x2f4, 0x3, 0x2, 0x2, 0x2, 0x2f6, 0x2f7, 
    0x3, 0x2, 0x2, 0x2, 0x2f7, 0x2f9, 0x3, 0x2, 0x2, 0x2, 0x2f8, 0x2f6, 
    0x3, 0x2, 0x2, 0x2, 0x2f9, 0x2fa, 0x7, 0x8, 0x2, 0x2, 0x2fa, 0x3b, 0x3, 
    0x2, 0x2, 0x2, 0x2fb, 0x2fc, 0x5, 0x84, 0x43, 0x2, 0x2fc, 0x30e, 0x5, 
    0x46, 0x24, 0x2, 0x2fd, 0x2ff, 0x7, 0x4d, 0x2, 0x2, 0x2fe, 0x2fd, 0x3, 
    0x2, 0x2, 0x2, 0x2fe, 0x2ff, 0x3, 0x2, 0x2, 0x2, 0x2ff, 0x300, 0x3, 
    0x2, 0x2, 0x2, 0x300, 0x30d, 0x7, 0x4f, 0x2, 0x2, 0x301, 0x302, 0x7, 
    0x59, 0x2, 0x2, 0x302, 0x30d, 0x7, 0x43, 0x2, 0x2, 0x303, 0x305, 0x7, 
    0x6a, 0x2, 0x2, 0x304, 0x306, 0x7, 0x43, 0x2, 0x2, 0x305, 0x304, 0x3, 
    0x2, 0x2, 0x2, 0x305, 0x306, 0x3, 0x2, 0x2, 0x2, 0x306, 0x30d, 0x3, 
    0x2, 0x2, 0x2, 0x307, 0x308, 0x7, 0x6a, 0x2, 0x2, 0x308, 0x30a, 0x7, 
    0x4a, 0x2, 0x2, 0x309, 0x30b, 0x7, 0x43, 0x2, 0x2, 0x30a, 0x309, 0x3, 
    0x2, 0x2, 0x2, 0x30a, 0x30b, 0x3, 0x2, 0x2, 0x2, 0x30b, 0x30d, 0x3, 
    0x2, 0x2, 0x2, 0x30c, 0x2fe, 0x3, 0x2, 0x2, 0x2, 0x30c, 0x301, 0x3, 
    0x2, 0x2, 0x2, 0x30c, 0x303, 0x3, 0x2, 0x2, 0x2, 0x30c, 0x307, 0x3, 
    0x2, 0x2, 0x2, 0x30d, 0x310, 0x3, 0x2, 0x2, 0x2, 0x30e, 0x30c, 0x3, 
    0x2, 0x2, 0x2, 0x30e, 0x30f, 0x3, 0x2, 0x2, 0x2, 0x30f, 0x313, 0x3, 
    0x2, 0x2, 0x2, 0x310, 0x30e, 0x3, 0x2, 0x2, 0x2, 0x311, 0x312, 0x7, 
    0x6f, 0x2, 0x2, 0x312, 0x314, 0x5, 0x62, 0x32, 0x2, 0x313, 0x311, 0x3, 
    0x2, 0x2, 0x2, 0x313, 0x314, 0x3, 0x2, 0x2, 0x2, 0x314, 0x3d, 0x3, 0x2, 
    0x2, 0x2, 0x315, 0x316, 0x7, 0x7, 0x2, 0x2, 0x316, 0x31b, 0x5, 0x84, 
    0x43, 0x2, 0x317, 0x318, 0x7, 0x5, 0x2, 0x2, 0x318, 0x31a, 0x5, 0x84, 
    0x43, 0x2, 0x319, 0x317, 0x3, 0x2, 0x2, 0x2, 0x31a, 0x31d, 0x3, 0x2, 
    0x2, 0x2, 0x31b, 0x319, 0x3, 0x2, 0x2, 0x2, 0x31b, 0x31c, 0x3, 0x2, 
    0x2, 0x2, 0x31c, 0x31e, 0x3, 0x2, 0x2, 0x2, 0x31d, 0x31b, 0x3, 0x2, 
    0x2, 0x2, 0x31e, 0x31f, 0x7, 0x8, 0x2, 0x2, 0x31f, 0x3f, 0x3, 0x2, 0x2, 
    0x2, 0x320, 0x321, 0x7, 0x59, 0x2, 0x2, 0x321, 0x32e, 0x7, 0x43, 0x2, 
    0x2, 0x322, 0x32f, 0x5, 0x6e, 0x38, 0x2, 0x323, 0x324, 0x7, 0x7, 0x2, 
    0x2, 0x324, 0x329, 0x5, 0x6e, 0x38, 0x2, 0x325, 0x326, 0x7, 0x5, 0x2, 
    0x2, 0x326, 0x328, 0x5, 0x6e, 0x38, 0x2, 0x327, 0x325, 0x3, 0x2, 0x2, 
    0x2, 0x328, 0x32b, 0x3, 0x2, 0x2, 0x2, 0x329, 0x327, 0x3, 0x2, 0x2, 
    0x2, 0x329, 0x32a, 0x3, 0x2, 0x2, 0x2, 0x32a, 0x32c, 0x3, 0x2, 0x2, 
    0x2, 0x32b, 0x329, 0x3, 0x2, 0x2, 0x2, 0x32c, 0x32d, 0x7, 0x8, 0x2, 
    0x2, 0x32d, 0x32f, 0x3, 0x2, 0x2, 0x2, 0x32e, 0x322, 0x3, 0x2, 0x2, 
    0x2, 0x32e, 0x323, 0x3, 0x2, 0x2, 0x2, 0x32f, 0x344, 0x3, 0x2, 0x2, 
    0x2, 0x330, 0x332, 0x7, 0x6a, 0x2, 0x2, 0x331, 0x330, 0x3, 0x2, 0x2, 
    0x2, 0x331, 0x332, 0x3, 0x2, 0x2, 0x2, 0x332, 0x333, 0x3, 0x2, 0x2, 
    0x2, 0x333, 0x334, 0x7, 0x4a, 0x2, 0x2, 0x334, 0x341, 0x7, 0x43, 0x2, 
    0x2, 0x335, 0x342, 0x5, 0x6e, 0x38, 0x2, 0x336, 0x337, 0x7, 0x7, 0x2, 
    0x2, 0x337, 0x33c, 0x5, 0x6e, 0x38, 0x2, 0x338, 0x339, 0x7, 0x5, 0x2, 
    0x2, 0x339, 0x33b, 0x5, 0x6e, 0x38, 0x2, 0x33a, 0x338, 0x3, 0x2, 0x2, 
    0x2, 0x33b, 0x33e, 0x3, 0x2, 0x2, 0x2, 0x33c, 0x33a, 0x3, 0x2, 0x2, 
    0x2, 0x33c, 0x33d, 0x3, 0x2, 0x2, 0x2, 0x33d, 0x33f, 0x3, 0x2, 0x2, 
    0x2, 0x33e, 0x33c, 0x3, 0x2, 0x2, 0x2, 0x33f, 0x340, 0x7, 0x8, 0x2, 
    0x2, 0x340, 0x342, 0x3, 0x2, 0x2, 0x2, 0x341, 0x335, 0x3, 0x2, 0x2, 
    0x2, 0x341, 0x336, 0x3, 0x2, 0x2, 0x2, 0x342, 0x344, 0x3, 0x2, 0x2, 
    0x2, 0x343, 0x320, 0x3, 0x2, 0x2, 0x2, 0x343, 0x331, 0x3, 0x2, 0x2, 
    0x2, 0x344, 0x41, 0x3, 0x2, 0x2, 0x2, 0x345, 0x347, 0x7, 0x6a, 0x2, 
    0x2, 0x346, 0x345, 0x3, 0x2, 0x2, 0x2, 0x346, 0x347, 0x3, 0x2, 0x2, 
    0x2, 0x347, 0x348, 0x3, 0x2, 0x2, 0x2, 0x348, 0x34a, 0x9, 0x7, 0x2, 
    0x2, 0x349, 0x34b, 0x5, 0x8a, 0x46, 0x2, 0x34a, 0x349, 0x3, 0x2, 0x2, 
    0x2, 0x34a, 0x34b, 0x3, 0x2, 0x2, 0x2, 0x34b, 0x34d, 0x3, 0x2, 0x2, 
    0x2, 0x34c, 0x34e, 0x7, 0x51, 0x2, 0x2, 0x34d, 0x34c, 0x3, 0x2, 0x2, 
    0x2, 0x34d, 0x34e, 0x3, 0x2, 0x2, 0x2, 0x34e, 0x35b, 0x3, 0x2, 0x2, 
    0x2, 0x34f, 0x35c, 0x5, 0x6e, 0x38, 0x2, 0x350, 0x351, 0x7, 0x7, 0x2, 
    0x2, 0x351, 0x356, 0x5, 0x6e, 0x38, 0x2, 0x352, 0x353, 0x7, 0x5, 0x2, 
    0x2, 0x353, 0x355, 0x5, 0x6e, 0x38, 0x2, 0x354, 0x352, 0x3, 0x2, 0x2, 
    0x2, 0x355, 0x358, 0x3, 0x2, 0x2, 0x2, 0x356, 0x354, 0x3, 0x2, 0x2, 
    0x2, 0x356, 0x357, 0x3, 0x2, 0x2, 0x2, 0x357, 0x359, 0x3, 0x2, 0x2, 
    0x2, 0x358, 0x356, 0x3, 0x2, 0x2, 0x2, 0x359, 0x35a, 0x7, 0x8, 0x2, 
    0x2, 0x35a, 0x35c, 0x3, 0x2, 0x2, 0x2, 0x35b, 0x34f, 0x3, 0x2, 0x2, 
    0x2, 0x35b, 0x350, 0x3, 0x2, 0x2, 0x2, 0x35c, 0x35f, 0x3, 0x2, 0x2, 
    0x2, 0x35d, 0x35e, 0x7, 0x6c, 0x2, 0x2, 0x35e, 0x360, 0x5, 0x44, 0x23, 
    0x2, 0x35f, 0x35d, 0x3, 0x2, 0x2, 0x2, 0x35f, 0x360, 0x3, 0x2, 0x2, 
    0x2, 0x360, 0x363, 0x3, 0x2, 0x2, 0x2, 0x361, 0x362, 0x7, 0x6f, 0x2, 
    0x2, 0x362, 0x364, 0x5, 0x62, 0x32, 0x2, 0x363, 0x361, 0x3, 0x2, 0x2, 
    0x2, 0x363, 0x364, 0x3, 0x2, 0x2, 0x2, 0x364, 0x43, 0x3, 0x2, 0x2, 0x2, 
    0x365, 0x366, 0x9, 0x9, 0x2, 0x2, 0x366, 0x45, 0x3, 0x2, 0x2, 0x2, 0x367, 
    0x36a, 0x5, 0x48, 0x25, 0x2, 0x368, 0x369, 0x7, 0x6c, 0x2, 0x2, 0x369, 
    0x36b, 0x5, 0x76, 0x3c, 0x2, 0x36a, 0x368, 0x3, 0x2, 0x2, 0x2, 0x36a, 
    0x36b, 0x3, 0x2, 0x2, 0x2, 0x36b, 0x36e, 0x3, 0x2, 0x2, 0x2, 0x36c, 
    0x36e, 0x5, 0x76, 0x3c, 0x2, 0x36d, 0x367, 0x3, 0x2, 0x2, 0x2, 0x36d, 
    0x36c, 0x3, 0x2, 0x2, 0x2, 0x36e, 0x47, 0x3, 0x2, 0x2, 0x2, 0x36f, 0x3cb, 
    0x7, 0x71, 0x2, 0x2, 0x370, 0x378, 0x7, 0x72, 0x2, 0x2, 0x371, 0x372, 
    0x7, 0x7, 0x2, 0x2, 0x372, 0x374, 0x5, 0x98, 0x4d, 0x2, 0x373, 0x375, 
    0x9, 0xa, 0x2, 0x2, 0x374, 0x373, 0x3, 0x2, 0x2, 0x2, 0x374, 0x375, 
    0x3, 0x2, 0x2, 0x2, 0x375, 0x376, 0x3, 0x2, 0x2, 0x2, 0x376, 0x377, 
    0x7, 0x8, 0x2, 0x2, 0x377, 0x379, 0x3, 0x2, 0x2, 0x2, 0x378, 0x371, 
    0x3, 0x2, 0x2, 0x2, 0x378, 0x379, 0x3, 0x2, 0x2, 0x2, 0x379, 0x3cb, 
    0x3, 0x2, 0x2, 0x2, 0x37a, 0x389, 0x7, 0x73, 0x2, 0x2, 0x37b, 0x37c, 
    0x7, 0x7, 0x2, 0x2, 0x37c, 0x37e, 0x5, 0x98, 0x4d, 0x2, 0x37d, 0x37f, 
    0x7, 0x85, 0x2, 0x2, 0x37e, 0x37d, 0x3, 0x2, 0x2, 0x2, 0x37e, 0x37f, 
    0x3, 0x2, 0x2, 0x2, 0x37f, 0x385, 0x3, 0x2, 0x2, 0x2, 0x380, 0x381, 
    0x7, 0x5, 0x2, 0x2, 0x381, 0x383, 0x5, 0x9a, 0x4e, 0x2, 0x382, 0x384, 
    0x7, 0x85, 0x2, 0x2, 0x383, 0x382, 0x3, 0x2, 0x2, 0x2, 0x383, 0x384, 
    0x3, 0x2, 0x2, 0x2, 0x384, 0x386, 0x3, 0x2, 0x2, 0x2, 0x385, 0x380, 
    0x3, 0x2, 0x2, 0x2, 0x385, 0x386, 0x3, 0x2, 0x2, 0x2, 0x386, 0x387, 
    0x3, 0x2, 0x2, 0x2, 0x387, 0x388, 0x7, 0x8, 0x2, 0x2, 0x388, 0x38a, 
    0x3, 0x2, 0x2, 0x2, 0x389, 0x37b, 0x3, 0x2, 0x2, 0x2, 0x389, 0x38a, 
    0x3, 0x2, 0x2, 0x2, 0x38a, 0x3cb, 0x3, 0x2, 0x2, 0x2, 0x38b, 0x393, 
    0x7, 0x74, 0x2, 0x2, 0x38c, 0x38d, 0x7, 0x7, 0x2, 0x2, 0x38d, 0x38f, 
    0x5, 0x98, 0x4d, 0x2, 0x38e, 0x390, 0x9, 0xa, 0x2, 0x2, 0x38f, 0x38e, 
    0x3, 0x2, 0x2, 0x2, 0x38f, 0x390, 0x3, 0x2, 0x2, 0x2, 0x390, 0x391, 
    0x3, 0x2, 0x2, 0x2, 0x391, 0x392, 0x7, 0x8, 0x2, 0x2, 0x392, 0x394, 
    0x3, 0x2, 0x2, 0x2, 0x393, 0x38c, 0x3, 0x2, 0x2, 0x2, 0x393, 0x394, 
    0x3, 0x2, 0x2, 0x2, 0x394, 0x3cb, 0x3, 0x2, 0x2, 0x2, 0x395, 0x3cb, 
    0x7, 0x2b, 0x2, 0x2, 0x396, 0x3cb, 0x7, 0x75, 0x2, 0x2, 0x397, 0x3cb, 
    0x7, 0x76, 0x2, 0x2, 0x398, 0x399, 0x9, 0xb, 0x2, 0x2, 0x399, 0x39a, 
    0x7, 0x7, 0x2, 0x2, 0x39a, 0x39c, 0x5, 0x9c, 0x4f, 0x2, 0x39b, 0x39d, 
    0x9, 0xc, 0x2, 0x2, 0x39c, 0x39b, 0x3, 0x2, 0x2, 0x2, 0x39c, 0x39d, 
    0x3, 0x2, 0x2, 0x2, 0x39d, 0x39e, 0x3, 0x2, 0x2, 0x2, 0x39e, 0x39f, 
    0x7, 0x8, 0x2, 0x2, 0x39f, 0x3b2, 0x3, 0x2, 0x2, 0x2, 0x3a0, 0x3a2, 
    0x9, 0xb, 0x2, 0x2, 0x3a1, 0x3a3, 0x7, 0x7f, 0x2, 0x2, 0x3a2, 0x3a1, 
    0x3, 0x2, 0x2, 0x2, 0x3a2, 0x3a3, 0x3, 0x2, 0x2, 0x2, 0x3a3, 0x3a6, 
    0x3, 0x2, 0x2, 0x2, 0x3a4, 0x3a6, 0x7, 0x7a, 0x2, 0x2, 0x3a5, 0x3a0, 
    0x3, 0x2, 0x2, 0x2, 0x3a5, 0x3a4, 0x3, 0x2, 0x2, 0x2, 0x3a6, 0x3ae, 
    0x3, 0x2, 0x2, 0x2, 0x3a7, 0x3a8, 0x7, 0x7, 0x2, 0x2, 0x3a8, 0x3aa, 
    0x5, 0x9c, 0x4f, 0x2, 0x3a9, 0x3ab, 0x9, 0xc, 0x2, 0x2, 0x3aa, 0x3a9, 
    0x3, 0x2, 0x2, 0x2, 0x3aa, 0x3ab, 0x3, 0x2, 0x2, 0x2, 0x3ab, 0x3ac, 
    0x3, 0x2, 0x2, 0x2, 0x3ac, 0x3ad, 0x7, 0x8, 0x2, 0x2, 0x3ad, 0x3af, 
    0x3, 0x2, 0x2, 0x2, 0x3ae, 0x3a7, 0x3, 0x2, 0x2, 0x2, 0x3ae, 0x3af, 
    0x3, 0x2, 0x2, 0x2, 0x3af, 0x3b2, 0x3, 0x2, 0x2, 0x2, 0x3b0, 0x3b2, 
    0x7, 0x7b, 0x2, 0x2, 0x3b1, 0x398, 0x3, 0x2, 0x2, 0x2, 0x3b1, 0x3a5, 
    0x3, 0x2, 0x2, 0x2, 0x3b1, 0x3b0, 0x3, 0x2, 0x2, 0x2, 0x3b2, 0x3b5, 
    0x3, 0x2, 0x2, 0x2, 0x3b3, 0x3b4, 0x7, 0x22, 0x2, 0x2, 0x3b4, 0x3b6, 
    0x5, 0x88, 0x45, 0x2, 0x3b5, 0x3b3, 0x3, 0x2, 0x2, 0x2, 0x3b5, 0x3b6, 
    0x3, 0x2, 0x2, 0x2, 0x3b6, 0x3cb, 0x3, 0x2, 0x2, 0x2, 0x3b7, 0x3b8, 
    0x7, 0x7c, 0x2, 0x2, 0x3b8, 0x3b9, 0x7, 0x7, 0x2, 0x2, 0x3b9, 0x3bb, 
    0x5, 0x9c, 0x4f, 0x2, 0x3ba, 0x3bc, 0x9, 0xd, 0x2, 0x2, 0x3bb, 0x3ba, 
    0x3, 0x2, 0x2, 0x2, 0x3bb, 0x3bc, 0x3, 0x2, 0x2, 0x2, 0x3bc, 0x3bd, 
    0x3, 0x2, 0x2, 0x2, 0x3bd, 0x3be, 0x7, 0x8, 0x2, 0x2, 0x3be, 0x3cb, 
    0x3, 0x2, 0x2, 0x2, 0x3bf, 0x3c7, 0x7, 0x7d, 0x2, 0x2, 0x3c0, 0x3c1, 
    0x7, 0x7, 0x2, 0x2, 0x3c1, 0x3c3, 0x5, 0x9c, 0x4f, 0x2, 0x3c2, 0x3c4, 
    0x9, 0xd, 0x2, 0x2, 0x3c3, 0x3c2, 0x3, 0x2, 0x2, 0x2, 0x3c3, 0x3c4, 
    0x3, 0x2, 0x2, 0x2, 0x3c4, 0x3c5, 0x3, 0x2, 0x2, 0x2, 0x3c5, 0x3c6, 
    0x7, 0x8, 0x2, 0x2, 0x3c6, 0x3c8, 0x3, 0x2, 0x2, 0x2, 0x3c7, 0x3c0, 
    0x3, 0x2, 0x2, 0x2, 0x3c7, 0x3c8, 0x3, 0x2, 0x2, 0x2, 0x3c8, 0x3cb, 
    0x3, 0x2, 0x2, 0x2, 0x3c9, 0x3cb, 0x7, 0x7e, 0x2, 0x2, 0x3ca, 0x36f, 
    0x3, 0x2, 0x2, 0x2, 0x3ca, 0x370, 0x3, 0x2, 0x2, 0x2, 0x3ca, 0x37a, 
    0x3, 0x2, 0x2, 0x2, 0x3ca, 0x38b, 0x3, 0x2, 0x2, 0x2, 0x3ca, 0x395, 
    0x3, 0x2, 0x2, 0x2, 0x3ca, 0x396, 0x3, 0x2, 0x2, 0x2, 0x3ca, 0x397, 
    0x3, 0x2, 0x2, 0x2, 0x3ca, 0x3b1, 0x3, 0x2, 0x2, 0x2, 0x3ca, 0x3b7, 
    0x3, 0x2, 0x2, 0x2, 0x3ca, 0x3bf, 0x3, 0x2, 0x2, 0x2, 0x3ca, 0x3c9, 
    0x3, 0x2, 0x2, 0x2, 0x3cb, 0x49, 0x3, 0x2, 0x2, 0x2, 0x3cc, 0x3cd, 0x5, 
    0x4c, 0x27, 0x2, 0x3cd, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x3ce, 0x3d2, 0x7, 
    0x7, 0x2, 0x2, 0x3cf, 0x3d1, 0x5, 0xe, 0x8, 0x2, 0x3d0, 0x3cf, 0x3, 
    0x2, 0x2, 0x2, 0x3d1, 0x3d4, 0x3, 0x2, 0x2, 0x2, 0x3d2, 0x3d0, 0x3, 
    0x2, 0x2, 0x2, 0x3d2, 0x3d3, 0x3, 0x2, 0x2, 0x2, 0x3d3, 0x3d5, 0x3, 
    0x2, 0x2, 0x2, 0x3d4, 0x3d2, 0x3, 0x2, 0x2, 0x2, 0x3d5, 0x3d6, 0x5, 
    0x4c, 0x27, 0x2, 0x3d6, 0x3d7, 0x7, 0x8, 0x2, 0x2, 0x3d7, 0x3e4, 0x3, 
    0x2, 0x2, 0x2, 0x3d8, 0x3da, 0x5, 0xe, 0x8, 0x2, 0x3d9, 0x3d8, 0x3, 
    0x2, 0x2, 0x2, 0x3da, 0x3dd, 0x3, 0x2, 0x2, 0x2, 0x3db, 0x3d9, 0x3, 
    0x2, 0x2, 0x2, 0x3db, 0x3dc, 0x3, 0x2, 0x2, 0x2, 0x3dc, 0x3e1, 0x3, 
    0x2, 0x2, 0x2, 0x3dd, 0x3db, 0x3, 0x2, 0x2, 0x2, 0x3de, 0x3e2, 0x5, 
    0x12, 0xa, 0x2, 0x3df, 0x3e2, 0x5, 0x26, 0x14, 0x2, 0x3e0, 0x3e2, 0x5, 
    0x16, 0xc, 0x2, 0x3e1, 0x3de, 0x3, 0x2, 0x2, 0x2, 0x3e1, 0x3df, 0x3, 
    0x2, 0x2, 0x2, 0x3e1, 0x3e0, 0x3, 0x2, 0x2, 0x2, 0x3e2, 0x3e4, 0x3, 
    0x2, 0x2, 0x2, 0x3e3, 0x3ce, 0x3, 0x2, 0x2, 0x2, 0x3e3, 0x3db, 0x3, 
    0x2, 0x2, 0x2, 0x3e4, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x3e5, 0x3ea, 0x5, 0x50, 
    0x29, 0x2, 0x3e6, 0x3e7, 0x7, 0x5, 0x2, 0x2, 0x3e7, 0x3e9, 0x5, 0x50, 
    0x29, 0x2, 0x3e8, 0x3e6, 0x3, 0x2, 0x2, 0x2, 0x3e9, 0x3ec, 0x3, 0x2, 
    0x2, 0x2, 0x3ea, 0x3e8, 0x3, 0x2, 0x2, 0x2, 0x3ea, 0x3eb, 0x3, 0x2, 
    0x2, 0x2, 0x3eb, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x3ec, 0x3ea, 0x3, 0x2, 0x2, 
    0x2, 0x3ed, 0x3f2, 0x5, 0x6e, 0x38, 0x2, 0x3ee, 0x3f0, 0x7, 0x1b, 0x2, 
    0x2, 0x3ef, 0x3ee, 0x3, 0x2, 0x2, 0x2, 0x3ef, 0x3f0, 0x3, 0x2, 0x2, 
    0x2, 0x3f0, 0x3f1, 0x3, 0x2, 0x2, 0x2, 0x3f1, 0x3f3, 0x5, 0x94, 0x4b, 
    0x2, 0x3f2, 0x3ef, 0x3, 0x2, 0x2, 0x2, 0x3f2, 0x3f3, 0x3, 0x2, 0x2, 
    0x2, 0x3f3, 0x3f6, 0x3, 0x2, 0x2, 0x2, 0x3f4, 0x3f6, 0x5, 0x74, 0x3b, 
    0x2, 0x3f5, 0x3ed, 0x3, 0x2, 0x2, 0x2, 0x3f5, 0x3f4, 0x3, 0x2, 0x2, 
    0x2, 0x3f6, 0x51, 0x3, 0x2, 0x2, 0x2, 0x3f7, 0x3fc, 0x5, 0x54, 0x2b, 
    0x2, 0x3f8, 0x3f9, 0x7, 0x5, 0x2, 0x2, 0x3f9, 0x3fb, 0x5, 0x54, 0x2b, 
    0x2, 0x3fa, 0x3f8, 0x3, 0x2, 0x2, 0x2, 0x3fb, 0x3fe, 0x3, 0x2, 0x2, 
    0x2, 0x3fc, 0x3fa, 0x3, 0x2, 0x2, 0x2, 0x3fc, 0x3fd, 0x3, 0x2, 0x2, 
    0x2, 0x3fd, 0x53, 0x3, 0x2, 0x2, 0x2, 0x3fe, 0x3fc, 0x3, 0x2, 0x2, 0x2, 
    0x3ff, 0x400, 0x5, 0x6e, 0x38, 0x2, 0x400, 0x55, 0x3, 0x2, 0x2, 0x2, 
    0x401, 0x406, 0x5, 0x58, 0x2d, 0x2, 0x402, 0x403, 0x7, 0x5, 0x2, 0x2, 
    0x403, 0x405, 0x5, 0x58, 0x2d, 0x2, 0x404, 0x402, 0x3, 0x2, 0x2, 0x2, 
    0x405, 0x408, 0x3, 0x2, 0x2, 0x2, 0x406, 0x404, 0x3, 0x2, 0x2, 0x2, 
    0x406, 0x407, 0x3, 0x2, 0x2, 0x2, 0x407, 0x57, 0x3, 0x2, 0x2, 0x2, 0x408, 
    0x406, 0x3, 0x2, 0x2, 0x2, 0x409, 0x40b, 0x5, 0x6e, 0x38, 0x2, 0x40a, 
    0x40c, 0x9, 0xe, 0x2, 0x2, 0x40b, 0x40a, 0x3, 0x2, 0x2, 0x2, 0x40b, 
    0x40c, 0x3, 0x2, 0x2, 0x2, 0x40c, 0x59, 0x3, 0x2, 0x2, 0x2, 0x40d, 0x412, 
    0x5, 0x5c, 0x2f, 0x2, 0x40e, 0x40f, 0x7, 0x5, 0x2, 0x2, 0x40f, 0x411, 
    0x5, 0x5c, 0x2f, 0x2, 0x410, 0x40e, 0x3, 0x2, 0x2, 0x2, 0x411, 0x414, 
    0x3, 0x2, 0x2, 0x2, 0x412, 0x410, 0x3, 0x2, 0x2, 0x2, 0x412, 0x413, 
    0x3, 0x2, 0x2, 0x2, 0x413, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x414, 0x412, 0x3, 
    0x2, 0x2, 0x2, 0x415, 0x416, 0x5, 0x94, 0x4b, 0x2, 0x416, 0x5d, 0x3, 
    0x2, 0x2, 0x2, 0x417, 0x41c, 0x5, 0x60, 0x31, 0x2, 0x418, 0x419, 0x7, 
    0x5, 0x2, 0x2, 0x419, 0x41b, 0x5, 0x60, 0x31, 0x2, 0x41a, 0x418, 0x3, 
    0x2, 0x2, 0x2, 0x41b, 0x41e, 0x3, 0x2, 0x2, 0x2, 0x41c, 0x41a, 0x3, 
    0x2, 0x2, 0x2, 0x41c, 0x41d, 0x3, 0x2, 0x2, 0x2, 0x41d, 0x5f, 0x3, 0x2, 
    0x2, 0x2, 0x41e, 0x41c, 0x3, 0x2, 0x2, 0x2, 0x41f, 0x420, 0x5, 0x94, 
    0x4b, 0x2, 0x420, 0x61, 0x3, 0x2, 0x2, 0x2, 0x421, 0x426, 0x5, 0x64, 
    0x33, 0x2, 0x422, 0x423, 0x7, 0x5, 0x2, 0x2, 0x423, 0x425, 0x5, 0x64, 
    0x33, 0x2, 0x424, 0x422, 0x3, 0x2, 0x2, 0x2, 0x425, 0x428, 0x3, 0x2, 
    0x2, 0x2, 0x426, 0x424, 0x3, 0x2, 0x2, 0x2, 0x426, 0x427, 0x3, 0x2, 
    0x2, 0x2, 0x427, 0x63, 0x3, 0x2, 0x2, 0x2, 0x428, 0x426, 0x3, 0x2, 0x2, 
    0x2, 0x429, 0x42c, 0x5, 0x8c, 0x47, 0x2, 0x42a, 0x42b, 0x7, 0x6, 0x2, 
    0x2, 0x42b, 0x42d, 0x5, 0x90, 0x49, 0x2, 0x42c, 0x42a, 0x3, 0x2, 0x2, 
    0x2, 0x42c, 0x42d, 0x3, 0x2, 0x2, 0x2, 0x42d, 0x65, 0x3, 0x2, 0x2, 0x2, 
    0x42e, 0x433, 0x5, 0x6e, 0x38, 0x2, 0x42f, 0x430, 0x7, 0x5, 0x2, 0x2, 
    0x430, 0x432, 0x5, 0x6e, 0x38, 0x2, 0x431, 0x42f, 0x3, 0x2, 0x2, 0x2, 
    0x432, 0x435, 0x3, 0x2, 0x2, 0x2, 0x433, 0x431, 0x3, 0x2, 0x2, 0x2, 
    0x433, 0x434, 0x3, 0x2, 0x2, 0x2, 0x434, 0x67, 0x3, 0x2, 0x2, 0x2, 0x435, 
    0x433, 0x3, 0x2, 0x2, 0x2, 0x436, 0x437, 0x5, 0x6e, 0x38, 0x2, 0x437, 
    0x69, 0x3, 0x2, 0x2, 0x2, 0x438, 0x439, 0x5, 0x6e, 0x38, 0x2, 0x439, 
    0x6b, 0x3, 0x2, 0x2, 0x2, 0x43a, 0x43b, 0x5, 0x6e, 0x38, 0x2, 0x43b, 
    0x6d, 0x3, 0x2, 0x2, 0x2, 0x43c, 0x43d, 0x8, 0x38, 0x1, 0x2, 0x43d, 
    0x544, 0x5, 0x9e, 0x50, 0x2, 0x43e, 0x544, 0x5, 0x70, 0x39, 0x2, 0x43f, 
    0x544, 0x5, 0x72, 0x3a, 0x2, 0x440, 0x441, 0x7, 0x7, 0x2, 0x2, 0x441, 
    0x442, 0x5, 0x46, 0x24, 0x2, 0x442, 0x443, 0x7, 0x8, 0x2, 0x2, 0x443, 
    0x444, 0x5, 0x6e, 0x38, 0x29, 0x444, 0x544, 0x3, 0x2, 0x2, 0x2, 0x445, 
    0x446, 0x9, 0xf, 0x2, 0x2, 0x446, 0x544, 0x5, 0x6e, 0x38, 0x28, 0x447, 
    0x448, 0x7, 0x87, 0x2, 0x2, 0x448, 0x449, 0x7, 0x7, 0x2, 0x2, 0x449, 
    0x44a, 0x5, 0x6e, 0x38, 0x2, 0x44a, 0x44b, 0x9, 0x10, 0x2, 0x2, 0x44b, 
    0x44c, 0x5, 0x46, 0x24, 0x2, 0x44c, 0x44d, 0x7, 0x8, 0x2, 0x2, 0x44d, 
    0x544, 0x3, 0x2, 0x2, 0x2, 0x44e, 0x44f, 0x7, 0x95, 0x2, 0x2, 0x44f, 
    0x450, 0x7, 0x7, 0x2, 0x2, 0x450, 0x451, 0x5, 0x6e, 0x38, 0x2, 0x451, 
    0x452, 0x7, 0x8, 0x2, 0x2, 0x452, 0x544, 0x3, 0x2, 0x2, 0x2, 0x453, 
    0x467, 0x7, 0x86, 0x2, 0x2, 0x454, 0x45a, 0x5, 0x6e, 0x38, 0x2, 0x455, 
    0x456, 0x7, 0x96, 0x2, 0x2, 0x456, 0x457, 0x5, 0x6e, 0x38, 0x2, 0x457, 
    0x458, 0x7, 0x97, 0x2, 0x2, 0x458, 0x459, 0x5, 0x6e, 0x38, 0x2, 0x459, 
    0x45b, 0x3, 0x2, 0x2, 0x2, 0x45a, 0x455, 0x3, 0x2, 0x2, 0x2, 0x45b, 
    0x45c, 0x3, 0x2, 0x2, 0x2, 0x45c, 0x45a, 0x3, 0x2, 0x2, 0x2, 0x45c, 
    0x45d, 0x3, 0x2, 0x2, 0x2, 0x45d, 0x468, 0x3, 0x2, 0x2, 0x2, 0x45e, 
    0x45f, 0x7, 0x96, 0x2, 0x2, 0x45f, 0x460, 0x5, 0x6c, 0x37, 0x2, 0x460, 
    0x461, 0x7, 0x97, 0x2, 0x2, 0x461, 0x462, 0x5, 0x6e, 0x38, 0x2, 0x462, 
    0x464, 0x3, 0x2, 0x2, 0x2, 0x463, 0x45e, 0x3, 0x2, 0x2, 0x2, 0x464, 
    0x465, 0x3, 0x2, 0x2, 0x2, 0x465, 0x463, 0x3, 0x2, 0x2, 0x2, 0x465, 
    0x466, 0x3, 0x2, 0x2, 0x2, 0x466, 0x468, 0x3, 0x2, 0x2, 0x2, 0x467, 
    0x454, 0x3, 0x2, 0x2, 0x2, 0x467, 0x463, 0x3, 0x2, 0x2, 0x2, 0x468, 
    0x46b, 0x3, 0x2, 0x2, 0x2, 0x469, 0x46a, 0x7, 0x98, 0x2, 0x2, 0x46a, 
    0x46c, 0x5, 0x6e, 0x38, 0x2, 0x46b, 0x469, 0x3, 0x2, 0x2, 0x2, 0x46b, 
    0x46c, 0x3, 0x2, 0x2, 0x2, 0x46c, 0x46d, 0x3, 0x2, 0x2, 0x2, 0x46d, 
    0x46e, 0x7, 0x99, 0x2, 0x2, 0x46e, 0x544, 0x3, 0x2, 0x2, 0x2, 0x46f, 
    0x470, 0x7, 0x8e, 0x2, 0x2, 0x470, 0x471, 0x7, 0x7, 0x2, 0x2, 0x471, 
    0x472, 0x5, 0x6c, 0x37, 0x2, 0x472, 0x473, 0x7, 0x5, 0x2, 0x2, 0x473, 
    0x474, 0x5, 0x6e, 0x38, 0x2, 0x474, 0x475, 0x7, 0x5, 0x2, 0x2, 0x475, 
    0x476, 0x5, 0x6e, 0x38, 0x2, 0x476, 0x477, 0x7, 0x8, 0x2, 0x2, 0x477, 
    0x544, 0x3, 0x2, 0x2, 0x2, 0x478, 0x479, 0x7, 0x8f, 0x2, 0x2, 0x479, 
    0x47a, 0x7, 0x7, 0x2, 0x2, 0x47a, 0x47b, 0x5, 0x6e, 0x38, 0x2, 0x47b, 
    0x47c, 0x7, 0x5, 0x2, 0x2, 0x47c, 0x47d, 0x5, 0x6e, 0x38, 0x2, 0x47d, 
    0x47e, 0x7, 0x8, 0x2, 0x2, 0x47e, 0x544, 0x3, 0x2, 0x2, 0x2, 0x47f, 
    0x480, 0x7, 0x90, 0x2, 0x2, 0x480, 0x481, 0x7, 0x7, 0x2, 0x2, 0x481, 
    0x482, 0x5, 0x6e, 0x38, 0x2, 0x482, 0x483, 0x7, 0x5, 0x2, 0x2, 0x483, 
    0x484, 0x5, 0x6e, 0x38, 0x2, 0x484, 0x485, 0x7, 0x8, 0x2, 0x2, 0x485, 
    0x544, 0x3, 0x2, 0x2, 0x2, 0x486, 0x487, 0x7, 0x88, 0x2, 0x2, 0x487, 
    0x490, 0x7, 0x7, 0x2, 0x2, 0x488, 0x48d, 0x5, 0x6e, 0x38, 0x2, 0x489, 
    0x48a, 0x7, 0x5, 0x2, 0x2, 0x48a, 0x48c, 0x5, 0x6e, 0x38, 0x2, 0x48b, 
    0x489, 0x3, 0x2, 0x2, 0x2, 0x48c, 0x48f, 0x3, 0x2, 0x2, 0x2, 0x48d, 
    0x48b, 0x3, 0x2, 0x2, 0x2, 0x48d, 0x48e, 0x3, 0x2, 0x2, 0x2, 0x48e, 
    0x491, 0x3, 0x2, 0x2, 0x2, 0x48f, 0x48d, 0x3, 0x2, 0x2, 0x2, 0x490, 
    0x488, 0x3, 0x2, 0x2, 0x2, 0x490, 0x491, 0x3, 0x2, 0x2, 0x2, 0x491, 
    0x492, 0x3, 0x2, 0x2, 0x2, 0x492, 0x544, 0x7, 0x8, 0x2, 0x2, 0x493, 
    0x494, 0x7, 0x8b, 0x2, 0x2, 0x494, 0x49d, 0x7, 0x7, 0x2, 0x2, 0x495, 
    0x49a, 0x5, 0x6e, 0x38, 0x2, 0x496, 0x497, 0x7, 0x5, 0x2, 0x2, 0x497, 
    0x499, 0x5, 0x6e, 0x38, 0x2, 0x498, 0x496, 0x3, 0x2, 0x2, 0x2, 0x499, 
    0x49c, 0x3, 0x2, 0x2, 0x2, 0x49a, 0x498, 0x3, 0x2, 0x2, 0x2, 0x49a, 
    0x49b, 0x3, 0x2, 0x2, 0x2, 0x49b, 0x49e, 0x3, 0x2, 0x2, 0x2, 0x49c, 
    0x49a, 0x3, 0x2, 0x2, 0x2, 0x49d, 0x495, 0x3, 0x2, 0x2, 0x2, 0x49d, 
    0x49e, 0x3, 0x2, 0x2, 0x2, 0x49e, 0x49f, 0x3, 0x2, 0x2, 0x2, 0x49f, 
    0x544, 0x7, 0x8, 0x2, 0x2, 0x4a0, 0x4a1, 0x7, 0x89, 0x2, 0x2, 0x4a1, 
    0x4aa, 0x7, 0x7, 0x2, 0x2, 0x4a2, 0x4a7, 0x5, 0x6e, 0x38, 0x2, 0x4a3, 
    0x4a4, 0x7, 0x5, 0x2, 0x2, 0x4a4, 0x4a6, 0x5, 0x6e, 0x38, 0x2, 0x4a5, 
    0x4a3, 0x3, 0x2, 0x2, 0x2, 0x4a6, 0x4a9, 0x3, 0x2, 0x2, 0x2, 0x4a7, 
    0x4a5, 0x3, 0x2, 0x2, 0x2, 0x4a7, 0x4a8, 0x3, 0x2, 0x2, 0x2, 0x4a8, 
    0x4ab, 0x3, 0x2, 0x2, 0x2, 0x4a9, 0x4a7, 0x3, 0x2, 0x2, 0x2, 0x4aa, 
    0x4a2, 0x3, 0x2, 0x2, 0x2, 0x4aa, 0x4ab, 0x3, 0x2, 0x2, 0x2, 0x4ab, 
    0x4ac, 0x3, 0x2, 0x2, 0x2, 0x4ac, 0x544, 0x7, 0x8, 0x2, 0x2, 0x4ad, 
    0x4ae, 0x7, 0x56, 0x2, 0x2, 0x4ae, 0x4af, 0x7, 0x7, 0x2, 0x2, 0x4af, 
    0x4b0, 0x5, 0x6e, 0x38, 0x2, 0x4b0, 0x4b1, 0x9, 0x11, 0x2, 0x2, 0x4b1, 
    0x4b2, 0x5, 0x6e, 0x38, 0x2, 0x4b2, 0x4b3, 0x9, 0x12, 0x2, 0x2, 0x4b3, 
    0x4b6, 0x5, 0x6e, 0x38, 0x2, 0x4b4, 0x4b5, 0x9, 0x13, 0x2, 0x2, 0x4b5, 
    0x4b7, 0x5, 0x6e, 0x38, 0x2, 0x4b6, 0x4b4, 0x3, 0x2, 0x2, 0x2, 0x4b6, 
    0x4b7, 0x3, 0x2, 0x2, 0x2, 0x4b7, 0x4b8, 0x3, 0x2, 0x2, 0x2, 0x4b8, 
    0x4b9, 0x7, 0x8, 0x2, 0x2, 0x4b9, 0x544, 0x3, 0x2, 0x2, 0x2, 0x4ba, 
    0x4bb, 0x7, 0x91, 0x2, 0x2, 0x4bb, 0x4bc, 0x7, 0x7, 0x2, 0x2, 0x4bc, 
    0x4bd, 0x5, 0x6e, 0x38, 0x2, 0x4bd, 0x4be, 0x9, 0x14, 0x2, 0x2, 0x4be, 
    0x4bf, 0x5, 0x6e, 0x38, 0x2, 0x4bf, 0x4c0, 0x7, 0x8, 0x2, 0x2, 0x4c0, 
    0x544, 0x3, 0x2, 0x2, 0x2, 0x4c1, 0x4c2, 0x9, 0x15, 0x2, 0x2, 0x4c2, 
    0x4d1, 0x7, 0x7, 0x2, 0x2, 0x4c3, 0x4c4, 0x5, 0x6e, 0x38, 0x2, 0x4c4, 
    0x4c5, 0x7, 0x5, 0x2, 0x2, 0x4c5, 0x4c8, 0x5, 0x6e, 0x38, 0x2, 0x4c6, 
    0x4c7, 0x7, 0x5, 0x2, 0x2, 0x4c7, 0x4c9, 0x5, 0x6e, 0x38, 0x2, 0x4c8, 
    0x4c6, 0x3, 0x2, 0x2, 0x2, 0x4c8, 0x4c9, 0x3, 0x2, 0x2, 0x2, 0x4c9, 
    0x4d2, 0x3, 0x2, 0x2, 0x2, 0x4ca, 0x4cb, 0x5, 0x6e, 0x38, 0x2, 0x4cb, 
    0x4cc, 0x7, 0x38, 0x2, 0x2, 0x4cc, 0x4cf, 0x5, 0x6e, 0x38, 0x2, 0x4cd, 
    0x4ce, 0x7, 0x36, 0x2, 0x2, 0x4ce, 0x4d0, 0x5, 0x6e, 0x38, 0x2, 0x4cf, 
    0x4cd, 0x3, 0x2, 0x2, 0x2, 0x4cf, 0x4d0, 0x3, 0x2, 0x2, 0x2, 0x4d0, 
    0x4d2, 0x3, 0x2, 0x2, 0x2, 0x4d1, 0x4c3, 0x3, 0x2, 0x2, 0x2, 0x4d1, 
    0x4ca, 0x3, 0x2, 0x2, 0x2, 0x4d2, 0x4d3, 0x3, 0x2, 0x2, 0x2, 0x4d3, 
    0x4d4, 0x7, 0x8, 0x2, 0x2, 0x4d4, 0x544, 0x3, 0x2, 0x2, 0x2, 0x4d5, 
    0x4d6, 0x7, 0x94, 0x2, 0x2, 0x4d6, 0x4e4, 0x7, 0x7, 0x2, 0x2, 0x4d7, 
    0x4da, 0x5, 0x6e, 0x38, 0x2, 0x4d8, 0x4d9, 0x7, 0x5, 0x2, 0x2, 0x4d9, 
    0x4db, 0x5, 0x6e, 0x38, 0x2, 0x4da, 0x4d8, 0x3, 0x2, 0x2, 0x2, 0x4da, 
    0x4db, 0x3, 0x2, 0x2, 0x2, 0x4db, 0x4e5, 0x3, 0x2, 0x2, 0x2, 0x4dc, 
    0x4de, 0x9, 0x16, 0x2, 0x2, 0x4dd, 0x4dc, 0x3, 0x2, 0x2, 0x2, 0x4dd, 
    0x4de, 0x3, 0x2, 0x2, 0x2, 0x4de, 0x4e0, 0x3, 0x2, 0x2, 0x2, 0x4df, 
    0x4e1, 0x5, 0x6e, 0x38, 0x2, 0x4e0, 0x4df, 0x3, 0x2, 0x2, 0x2, 0x4e0, 
    0x4e1, 0x3, 0x2, 0x2, 0x2, 0x4e1, 0x4e2, 0x3, 0x2, 0x2, 0x2, 0x4e2, 
    0x4e3, 0x7, 0x38, 0x2, 0x2, 0x4e3, 0x4e5, 0x5, 0x6e, 0x38, 0x2, 0x4e4, 
    0x4d7, 0x3, 0x2, 0x2, 0x2, 0x4e4, 0x4dd, 0x3, 0x2, 0x2, 0x2, 0x4e5, 
    0x4e6, 0x3, 0x2, 0x2, 0x2, 0x4e6, 0x4e7, 0x7, 0x8, 0x2, 0x2, 0x4e7, 
    0x544, 0x3, 0x2, 0x2, 0x2, 0x4e8, 0x4e9, 0x7, 0x8c, 0x2, 0x2, 0x4e9, 
    0x4ea, 0x7, 0x7, 0x2, 0x2, 0x4ea, 0x4eb, 0x9, 0x17, 0x2, 0x2, 0x4eb, 
    0x4ec, 0x7, 0x38, 0x2, 0x2, 0x4ec, 0x4ed, 0x5, 0x6e, 0x38, 0x2, 0x4ed, 
    0x4ee, 0x7, 0x8, 0x2, 0x2, 0x4ee, 0x544, 0x3, 0x2, 0x2, 0x2, 0x4ef, 
    0x4f0, 0x7, 0x8a, 0x2, 0x2, 0x4f0, 0x4f2, 0x7, 0x7, 0x2, 0x2, 0x4f1, 
    0x4f3, 0x7, 0x17, 0x2, 0x2, 0x4f2, 0x4f1, 0x3, 0x2, 0x2, 0x2, 0x4f2, 
    0x4f3, 0x3, 0x2, 0x2, 0x2, 0x4f3, 0x4f4, 0x3, 0x2, 0x2, 0x2, 0x4f4, 
    0x4f5, 0x7, 0xd, 0x2, 0x2, 0x4f5, 0x544, 0x7, 0x8, 0x2, 0x2, 0x4f6, 
    0x4f7, 0x7, 0x8a, 0x2, 0x2, 0x4f7, 0x4fa, 0x7, 0x7, 0x2, 0x2, 0x4f8, 
    0x4fb, 0x7, 0x17, 0x2, 0x2, 0x4f9, 0x4fb, 0x7, 0x2f, 0x2, 0x2, 0x4fa, 
    0x4f8, 0x3, 0x2, 0x2, 0x2, 0x4fa, 0x4f9, 0x3, 0x2, 0x2, 0x2, 0x4fa, 
    0x4fb, 0x3, 0x2, 0x2, 0x2, 0x4fb, 0x4fc, 0x3, 0x2, 0x2, 0x2, 0x4fc, 
    0x4fd, 0x5, 0x6e, 0x38, 0x2, 0x4fd, 0x4fe, 0x7, 0x8, 0x2, 0x2, 0x4fe, 
    0x544, 0x3, 0x2, 0x2, 0x2, 0x4ff, 0x500, 0x7, 0x61, 0x2, 0x2, 0x500, 
    0x503, 0x7, 0x7, 0x2, 0x2, 0x501, 0x504, 0x7, 0x17, 0x2, 0x2, 0x502, 
    0x504, 0x7, 0x2f, 0x2, 0x2, 0x503, 0x501, 0x3, 0x2, 0x2, 0x2, 0x503, 
    0x502, 0x3, 0x2, 0x2, 0x2, 0x503, 0x504, 0x3, 0x2, 0x2, 0x2, 0x504, 
    0x505, 0x3, 0x2, 0x2, 0x2, 0x505, 0x506, 0x5, 0x6e, 0x38, 0x2, 0x506, 
    0x507, 0x7, 0x8, 0x2, 0x2, 0x507, 0x544, 0x3, 0x2, 0x2, 0x2, 0x508, 
    0x509, 0x7, 0x1d, 0x2, 0x2, 0x509, 0x50c, 0x7, 0x7, 0x2, 0x2, 0x50a, 
    0x50d, 0x7, 0x17, 0x2, 0x2, 0x50b, 0x50d, 0x7, 0x2f, 0x2, 0x2, 0x50c, 
    0x50a, 0x3, 0x2, 0x2, 0x2, 0x50c, 0x50b, 0x3, 0x2, 0x2, 0x2, 0x50c, 
    0x50d, 0x3, 0x2, 0x2, 0x2, 0x50d, 0x50e, 0x3, 0x2, 0x2, 0x2, 0x50e, 
    0x50f, 0x5, 0x6e, 0x38, 0x2, 0x50f, 0x510, 0x7, 0x8, 0x2, 0x2, 0x510, 
    0x544, 0x3, 0x2, 0x2, 0x2, 0x511, 0x512, 0x7, 0x49, 0x2, 0x2, 0x512, 
    0x515, 0x7, 0x7, 0x2, 0x2, 0x513, 0x516, 0x7, 0x17, 0x2, 0x2, 0x514, 
    0x516, 0x7, 0x2f, 0x2, 0x2, 0x515, 0x513, 0x3, 0x2, 0x2, 0x2, 0x515, 
    0x514, 0x3, 0x2, 0x2, 0x2, 0x515, 0x516, 0x3, 0x2, 0x2, 0x2, 0x516, 
    0x517, 0x3, 0x2, 0x2, 0x2, 0x517, 0x518, 0x5, 0x6e, 0x38, 0x2, 0x518, 
    0x519, 0x7, 0x8, 0x2, 0x2, 0x519, 0x544, 0x3, 0x2, 0x2, 0x2, 0x51a, 
    0x51b, 0x7, 0x48, 0x2, 0x2, 0x51b, 0x51e, 0x7, 0x7, 0x2, 0x2, 0x51c, 
    0x51f, 0x7, 0x17, 0x2, 0x2, 0x51d, 0x51f, 0x7, 0x2f, 0x2, 0x2, 0x51e, 
    0x51c, 0x3, 0x2, 0x2, 0x2, 0x51e, 0x51d, 0x3, 0x2, 0x2, 0x2, 0x51e, 
    0x51f, 0x3, 0x2, 0x2, 0x2, 0x51f, 0x520, 0x3, 0x2, 0x2, 0x2, 0x520, 
    0x521, 0x5, 0x6e, 0x38, 0x2, 0x521, 0x522, 0x7, 0x8, 0x2, 0x2, 0x522, 
    0x544, 0x3, 0x2, 0x2, 0x2, 0x523, 0x524, 0x5, 0x78, 0x3d, 0x2, 0x524, 
    0x52d, 0x7, 0x7, 0x2, 0x2, 0x525, 0x52a, 0x5, 0x6e, 0x38, 0x2, 0x526, 
    0x527, 0x7, 0x5, 0x2, 0x2, 0x527, 0x529, 0x5, 0x6e, 0x38, 0x2, 0x528, 
    0x526, 0x3, 0x2, 0x2, 0x2, 0x529, 0x52c, 0x3, 0x2, 0x2, 0x2, 0x52a, 
    0x528, 0x3, 0x2, 0x2, 0x2, 0x52a, 0x52b, 0x3, 0x2, 0x2, 0x2, 0x52b, 
    0x52e, 0x3, 0x2, 0x2, 0x2, 0x52c, 0x52a, 0x3, 0x2, 0x2, 0x2, 0x52d, 
    0x525, 0x3, 0x2, 0x2, 0x2, 0x52d, 0x52e, 0x3, 0x2, 0x2, 0x2, 0x52e, 
    0x52f, 0x3, 0x2, 0x2, 0x2, 0x52f, 0x530, 0x7, 0x8, 0x2, 0x2, 0x530, 
    0x544, 0x3, 0x2, 0x2, 0x2, 0x531, 0x532, 0x7, 0x7, 0x2, 0x2, 0x532, 
    0x533, 0x5, 0x6e, 0x38, 0x2, 0x533, 0x534, 0x7, 0x8, 0x2, 0x2, 0x534, 
    0x544, 0x3, 0x2, 0x2, 0x2, 0x535, 0x536, 0x7, 0x7, 0x2, 0x2, 0x536, 
    0x537, 0x5, 0xc, 0x7, 0x2, 0x537, 0x538, 0x7, 0x8, 0x2, 0x2, 0x538, 
    0x544, 0x3, 0x2, 0x2, 0x2, 0x539, 0x53b, 0x7, 0x4d, 0x2, 0x2, 0x53a, 
    0x539, 0x3, 0x2, 0x2, 0x2, 0x53a, 0x53b, 0x3, 0x2, 0x2, 0x2, 0x53b, 
    0x53c, 0x3, 0x2, 0x2, 0x2, 0x53c, 0x53d, 0x7, 0x8d, 0x2, 0x2, 0x53d, 
    0x53e, 0x7, 0x7, 0x2, 0x2, 0x53e, 0x53f, 0x5, 0xc, 0x7, 0x2, 0x53f, 
    0x540, 0x7, 0x8, 0x2, 0x2, 0x540, 0x544, 0x3, 0x2, 0x2, 0x2, 0x541, 
    0x542, 0x7, 0x4d, 0x2, 0x2, 0x542, 0x544, 0x5, 0x6e, 0x38, 0x6, 0x543, 
    0x43c, 0x3, 0x2, 0x2, 0x2, 0x543, 0x43e, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x43f, 0x3, 0x2, 0x2, 0x2, 0x543, 0x440, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x445, 0x3, 0x2, 0x2, 0x2, 0x543, 0x447, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x44e, 0x3, 0x2, 0x2, 0x2, 0x543, 0x453, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x46f, 0x3, 0x2, 0x2, 0x2, 0x543, 0x478, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x47f, 0x3, 0x2, 0x2, 0x2, 0x543, 0x486, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x493, 0x3, 0x2, 0x2, 0x2, 0x543, 0x4a0, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x4ad, 0x3, 0x2, 0x2, 0x2, 0x543, 0x4ba, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x4c1, 0x3, 0x2, 0x2, 0x2, 0x543, 0x4d5, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x4e8, 0x3, 0x2, 0x2, 0x2, 0x543, 0x4ef, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x4f6, 0x3, 0x2, 0x2, 0x2, 0x543, 0x4ff, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x508, 0x3, 0x2, 0x2, 0x2, 0x543, 0x511, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x51a, 0x3, 0x2, 0x2, 0x2, 0x543, 0x523, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x531, 0x3, 0x2, 0x2, 0x2, 0x543, 0x535, 0x3, 0x2, 0x2, 0x2, 0x543, 
    0x53a, 0x3, 0x2, 0x2, 0x2, 0x543, 0x541, 0x3, 0x2, 0x2, 0x2, 0x544, 
    0x598, 0x3, 0x2, 0x2, 0x2, 0x545, 0x546, 0xc, 0x26, 0x2, 0x2, 0x546, 
    0x547, 0x9, 0x18, 0x2, 0x2, 0x547, 0x597, 0x5, 0x6e, 0x38, 0x27, 0x548, 
    0x549, 0xc, 0x25, 0x2, 0x2, 0x549, 0x54a, 0x9, 0x19, 0x2, 0x2, 0x54a, 
    0x597, 0x5, 0x6e, 0x38, 0x26, 0x54b, 0x54c, 0xc, 0x24, 0x2, 0x2, 0x54c, 
    0x54d, 0x9, 0x1a, 0x2, 0x2, 0x54d, 0x597, 0x5, 0x6e, 0x38, 0x25, 0x54e, 
    0x550, 0xc, 0xb, 0x2, 0x2, 0x54f, 0x551, 0x7, 0x4d, 0x2, 0x2, 0x550, 
    0x54f, 0x3, 0x2, 0x2, 0x2, 0x550, 0x551, 0x3, 0x2, 0x2, 0x2, 0x551, 
    0x552, 0x3, 0x2, 0x2, 0x2, 0x552, 0x553, 0x7, 0x46, 0x2, 0x2, 0x553, 
    0x597, 0x5, 0x6e, 0x38, 0xc, 0x554, 0x556, 0xc, 0xa, 0x2, 0x2, 0x555, 
    0x557, 0x7, 0x4d, 0x2, 0x2, 0x556, 0x555, 0x3, 0x2, 0x2, 0x2, 0x556, 
    0x557, 0x3, 0x2, 0x2, 0x2, 0x557, 0x558, 0x3, 0x2, 0x2, 0x2, 0x558, 
    0x559, 0x7, 0x46, 0x2, 0x2, 0x559, 0x55a, 0x5, 0x6e, 0x38, 0x2, 0x55a, 
    0x55b, 0x7, 0x31, 0x2, 0x2, 0x55b, 0x55c, 0x5, 0x6e, 0x38, 0xb, 0x55c, 
    0x597, 0x3, 0x2, 0x2, 0x2, 0x55d, 0x55e, 0xc, 0x5, 0x2, 0x2, 0x55e, 
    0x55f, 0x7, 0x18, 0x2, 0x2, 0x55f, 0x597, 0x5, 0x6e, 0x38, 0x6, 0x560, 
    0x561, 0xc, 0x4, 0x2, 0x2, 0x561, 0x562, 0x7, 0x52, 0x2, 0x2, 0x562, 
    0x597, 0x5, 0x6e, 0x38, 0x5, 0x563, 0x564, 0xc, 0x3, 0x2, 0x2, 0x564, 
    0x565, 0x7, 0x70, 0x2, 0x2, 0x565, 0x597, 0x5, 0x6e, 0x38, 0x4, 0x566, 
    0x567, 0xc, 0x27, 0x2, 0x2, 0x567, 0x568, 0x7, 0xa, 0x2, 0x2, 0x568, 
    0x569, 0x5, 0x7a, 0x3e, 0x2, 0x569, 0x572, 0x7, 0x7, 0x2, 0x2, 0x56a, 
    0x56f, 0x5, 0x6e, 0x38, 0x2, 0x56b, 0x56c, 0x7, 0x5, 0x2, 0x2, 0x56c, 
    0x56e, 0x5, 0x6e, 0x38, 0x2, 0x56d, 0x56b, 0x3, 0x2, 0x2, 0x2, 0x56e, 
    0x571, 0x3, 0x2, 0x2, 0x2, 0x56f, 0x56d, 0x3, 0x2, 0x2, 0x2, 0x56f, 
    0x570, 0x3, 0x2, 0x2, 0x2, 0x570, 0x573, 0x3, 0x2, 0x2, 0x2, 0x571, 
    0x56f, 0x3, 0x2, 0x2, 0x2, 0x572, 0x56a, 0x3, 0x2, 0x2, 0x2, 0x572, 
    0x573, 0x3, 0x2, 0x2, 0x2, 0x573, 0x574, 0x3, 0x2, 0x2, 0x2, 0x574, 
    0x575, 0x7, 0x8, 0x2, 0x2, 0x575, 0x597, 0x3, 0x2, 0x2, 0x2, 0x576, 
    0x577, 0xc, 0xc, 0x2, 0x2, 0x577, 0x579, 0x7, 0x41, 0x2, 0x2, 0x578, 
    0x57a, 0x7, 0x4d, 0x2, 0x2, 0x579, 0x578, 0x3, 0x2, 0x2, 0x2, 0x579, 
    0x57a, 0x3, 0x2, 0x2, 0x2, 0x57a, 0x57b, 0x3, 0x2, 0x2, 0x2, 0x57b, 
    0x597, 0x7, 0x4f, 0x2, 0x2, 0x57c, 0x57e, 0xc, 0x8, 0x2, 0x2, 0x57d, 
    0x57f, 0x7, 0x4d, 0x2, 0x2, 0x57e, 0x57d, 0x3, 0x2, 0x2, 0x2, 0x57e, 
    0x57f, 0x3, 0x2, 0x2, 0x2, 0x57f, 0x580, 0x3, 0x2, 0x2, 0x2, 0x580, 
    0x581, 0x7, 0x3b, 0x2, 0x2, 0x581, 0x582, 0x7, 0x7, 0x2, 0x2, 0x582, 
    0x583, 0x5, 0xc, 0x7, 0x2, 0x583, 0x584, 0x7, 0x8, 0x2, 0x2, 0x584, 
    0x597, 0x3, 0x2, 0x2, 0x2, 0x585, 0x587, 0xc, 0x7, 0x2, 0x2, 0x586, 
    0x588, 0x7, 0x4d, 0x2, 0x2, 0x587, 0x586, 0x3, 0x2, 0x2, 0x2, 0x587, 
    0x588, 0x3, 0x2, 0x2, 0x2, 0x588, 0x589, 0x3, 0x2, 0x2, 0x2, 0x589, 
    0x58a, 0x7, 0x3b, 0x2, 0x2, 0x58a, 0x593, 0x7, 0x7, 0x2, 0x2, 0x58b, 
    0x590, 0x5, 0x6e, 0x38, 0x2, 0x58c, 0x58d, 0x7, 0x5, 0x2, 0x2, 0x58d, 
    0x58f, 0x5, 0x6e, 0x38, 0x2, 0x58e, 0x58c, 0x3, 0x2, 0x2, 0x2, 0x58f, 
    0x592, 0x3, 0x2, 0x2, 0x2, 0x590, 0x58e, 0x3, 0x2, 0x2, 0x2, 0x590, 
    0x591, 0x3, 0x2, 0x2, 0x2, 0x591, 0x594, 0x3, 0x2, 0x2, 0x2, 0x592, 
    0x590, 0x3, 0x2, 0x2, 0x2, 0x593, 0x58b, 0x3, 0x2, 0x2, 0x2, 0x593, 
    0x594, 0x3, 0x2, 0x2, 0x2, 0x594, 0x595, 0x3, 0x2, 0x2, 0x2, 0x595, 
    0x597, 0x7, 0x8, 0x2, 0x2, 0x596, 0x545, 0x3, 0x2, 0x2, 0x2, 0x596, 
    0x548, 0x3, 0x2, 0x2, 0x2, 0x596, 0x54b, 0x3, 0x2, 0x2, 0x2, 0x596, 
    0x54e, 0x3, 0x2, 0x2, 0x2, 0x596, 0x554, 0x3, 0x2, 0x2, 0x2, 0x596, 
    0x55d, 0x3, 0x2, 0x2, 0x2, 0x596, 0x560, 0x3, 0x2, 0x2, 0x2, 0x596, 
    0x563, 0x3, 0x2, 0x2, 0x2, 0x596, 0x566, 0x3, 0x2, 0x2, 0x2, 0x596, 
    0x576, 0x3, 0x2, 0x2, 0x2, 0x596, 0x57c, 0x3, 0x2, 0x2, 0x2, 0x596, 
    0x585, 0x3, 0x2, 0x2, 0x2, 0x597, 0x59a, 0x3, 0x2, 0x2, 0x2, 0x598, 
    0x596, 0x3, 0x2, 0x2, 0x2, 0x598, 0x599, 0x3, 0x2, 0x2, 0x2, 0x599, 
    0x6f, 0x3, 0x2, 0x2, 0x2, 0x59a, 0x598, 0x3, 0x2, 0x2, 0x2, 0x59b, 0x59c, 
    0x9, 0x1b, 0x2, 0x2, 0x59c, 0x71, 0x3, 0x2, 0x2, 0x2, 0x59d, 0x59e, 
    0x5, 0x7c, 0x3f, 0x2, 0x59e, 0x59f, 0x7, 0xa, 0x2, 0x2, 0x59f, 0x5a1, 
    0x3, 0x2, 0x2, 0x2, 0x5a0, 0x59d, 0x3, 0x2, 0x2, 0x2, 0x5a0, 0x5a1, 
    0x3, 0x2, 0x2, 0x2, 0x5a1, 0x5a2, 0x3, 0x2, 0x2, 0x2, 0x5a2, 0x5a3, 
    0x5, 0x7e, 0x40, 0x2, 0x5a3, 0x5a4, 0x7, 0xa, 0x2, 0x2, 0x5a4, 0x5a6, 
    0x3, 0x2, 0x2, 0x2, 0x5a5, 0x5a0, 0x3, 0x2, 0x2, 0x2, 0x5a5, 0x5a6, 
    0x3, 0x2, 0x2, 0x2, 0x5a6, 0x5a7, 0x3, 0x2, 0x2, 0x2, 0x5a7, 0x5a8, 
    0x5, 0x80, 0x41, 0x2, 0x5a8, 0x5a9, 0x7, 0xa, 0x2, 0x2, 0x5a9, 0x5ab, 
    0x3, 0x2, 0x2, 0x2, 0x5aa, 0x5a5, 0x3, 0x2, 0x2, 0x2, 0x5aa, 0x5ab, 
    0x3, 0x2, 0x2, 0x2, 0x5ab, 0x5ac, 0x3, 0x2, 0x2, 0x2, 0x5ac, 0x5ad, 
    0x5, 0x84, 0x43, 0x2, 0x5ad, 0x73, 0x3, 0x2, 0x2, 0x2, 0x5ae, 0x5af, 
    0x5, 0x7c, 0x3f, 0x2, 0x5af, 0x5b0, 0x7, 0xa, 0x2, 0x2, 0x5b0, 0x5b2, 
    0x3, 0x2, 0x2, 0x2, 0x5b1, 0x5ae, 0x3, 0x2, 0x2, 0x2, 0x5b1, 0x5b2, 
    0x3, 0x2, 0x2, 0x2, 0x5b2, 0x5b3, 0x3, 0x2, 0x2, 0x2, 0x5b3, 0x5b4, 
    0x5, 0x7e, 0x40, 0x2, 0x5b4, 0x5b5, 0x7, 0xa, 0x2, 0x2, 0x5b5, 0x5b7, 
    0x3, 0x2, 0x2, 0x2, 0x5b6, 0x5b1, 0x3, 0x2, 0x2, 0x2, 0x5b6, 0x5b7, 
    0x3, 0x2, 0x2, 0x2, 0x5b7, 0x5b8, 0x3, 0x2, 0x2, 0x2, 0x5b8, 0x5b9, 
    0x5, 0x80, 0x41, 0x2, 0x5b9, 0x5ba, 0x7, 0xa, 0x2, 0x2, 0x5ba, 0x5bc, 
    0x3, 0x2, 0x2, 0x2, 0x5bb, 0x5b6, 0x3, 0x2, 0x2, 0x2, 0x5bb, 0x5bc, 
    0x3, 0x2, 0x2, 0x2, 0x5bc, 0x5bd, 0x3, 0x2, 0x2, 0x2, 0x5bd, 0x5be, 
    0x7, 0xd, 0x2, 0x2, 0x5be, 0x75, 0x3, 0x2, 0x2, 0x2, 0x5bf, 0x5c0, 0x5, 
    0x96, 0x4c, 0x2, 0x5c0, 0x5c1, 0x7, 0xa, 0x2, 0x2, 0x5c1, 0x5c3, 0x3, 
    0x2, 0x2, 0x2, 0x5c2, 0x5bf, 0x3, 0x2, 0x2, 0x2, 0x5c3, 0x5c6, 0x3, 
    0x2, 0x2, 0x2, 0x5c4, 0x5c2, 0x3, 0x2, 0x2, 0x2, 0x5c4, 0x5c5, 0x3, 
    0x2, 0x2, 0x2, 0x5c5, 0x5c7, 0x3, 0x2, 0x2, 0x2, 0x5c6, 0x5c4, 0x3, 
    0x2, 0x2, 0x2, 0x5c7, 0x5c8, 0x5, 0x94, 0x4b, 0x2, 0x5c8, 0x77, 0x3, 
    0x2, 0x2, 0x2, 0x5c9, 0x5ca, 0x5, 0x94, 0x4b, 0x2, 0x5ca, 0x79, 0x3, 
    0x2, 0x2, 0x2, 0x5cb, 0x5cc, 0x5, 0x94, 0x4b, 0x2, 0x5cc, 0x7b, 0x3, 
    0x2, 0x2, 0x2, 0x5cd, 0x5ce, 0x5, 0x94, 0x4b, 0x2, 0x5ce, 0x7d, 0x3, 
    0x2, 0x2, 0x2, 0x5cf, 0x5d0, 0x5, 0x94, 0x4b, 0x2, 0x5d0, 0x7f, 0x3, 
    0x2, 0x2, 0x2, 0x5d1, 0x5d2, 0x5, 0x94, 0x4b, 0x2, 0x5d2, 0x81, 0x3, 
    0x2, 0x2, 0x2, 0x5d3, 0x5d4, 0x5, 0x94, 0x4b, 0x2, 0x5d4, 0x83, 0x3, 
    0x2, 0x2, 0x2, 0x5d5, 0x5d6, 0x5, 0x94, 0x4b, 0x2, 0x5d6, 0x85, 0x3, 
    0x2, 0x2, 0x2, 0x5d7, 0x5d8, 0x5, 0x94, 0x4b, 0x2, 0x5d8, 0x87, 0x3, 
    0x2, 0x2, 0x2, 0x5d9, 0x5da, 0x5, 0x94, 0x4b, 0x2, 0x5da, 0x89, 0x3, 
    0x2, 0x2, 0x2, 0x5db, 0x5dc, 0x5, 0x94, 0x4b, 0x2, 0x5dc, 0x8b, 0x3, 
    0x2, 0x2, 0x2, 0x5dd, 0x5de, 0x5, 0x94, 0x4b, 0x2, 0x5de, 0x8d, 0x3, 
    0x2, 0x2, 0x2, 0x5df, 0x5e0, 0x5, 0x94, 0x4b, 0x2, 0x5e0, 0x8f, 0x3, 
    0x2, 0x2, 0x2, 0x5e1, 0x5e4, 0x5, 0xa0, 0x51, 0x2, 0x5e2, 0x5e4, 0x5, 
    0x94, 0x4b, 0x2, 0x5e3, 0x5e1, 0x3, 0x2, 0x2, 0x2, 0x5e3, 0x5e2, 0x3, 
    0x2, 0x2, 0x2, 0x5e4, 0x91, 0x3, 0x2, 0x2, 0x2, 0x5e5, 0x5e8, 0x5, 0xa0, 
    0x51, 0x2, 0x5e6, 0x5e8, 0x5, 0x94, 0x4b, 0x2, 0x5e7, 0x5e5, 0x3, 0x2, 
    0x2, 0x2, 0x5e7, 0x5e6, 0x3, 0x2, 0x2, 0x2, 0x5e8, 0x93, 0x3, 0x2, 0x2, 
    0x2, 0x5e9, 0x5ec, 0x7, 0xb2, 0x2, 0x2, 0x5ea, 0x5ec, 0x5, 0xba, 0x5e, 
    0x2, 0x5eb, 0x5e9, 0x3, 0x2, 0x2, 0x2, 0x5eb, 0x5ea, 0x3, 0x2, 0x2, 
    0x2, 0x5ec, 0x95, 0x3, 0x2, 0x2, 0x2, 0x5ed, 0x5ee, 0x5, 0x94, 0x4b, 
    0x2, 0x5ee, 0x97, 0x3, 0x2, 0x2, 0x2, 0x5ef, 0x5f0, 0x5, 0xa4, 0x53, 
    0x2, 0x5f0, 0x99, 0x3, 0x2, 0x2, 0x2, 0x5f1, 0x5f2, 0x5, 0xa4, 0x53, 
    0x2, 0x5f2, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x5f3, 0x5f4, 0x5, 0xa4, 0x53, 
    0x2, 0x5f4, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x5f5, 0x600, 0x7, 0x4f, 0x2, 
    0x2, 0x5f6, 0x600, 0x5, 0xa2, 0x52, 0x2, 0x5f7, 0x600, 0x5, 0xa4, 0x53, 
    0x2, 0x5f8, 0x600, 0x5, 0xa6, 0x54, 0x2, 0x5f9, 0x600, 0x5, 0xa8, 0x55, 
    0x2, 0x5fa, 0x600, 0x5, 0xaa, 0x56, 0x2, 0x5fb, 0x600, 0x5, 0xac, 0x57, 
    0x2, 0x5fc, 0x600, 0x5, 0xb0, 0x59, 0x2, 0x5fd, 0x600, 0x5, 0xae, 0x58, 
    0x2, 0x5fe, 0x600, 0x5, 0xb2, 0x5a, 0x2, 0x5ff, 0x5f5, 0x3, 0x2, 0x2, 
    0x2, 0x5ff, 0x5f6, 0x3, 0x2, 0x2, 0x2, 0x5ff, 0x5f7, 0x3, 0x2, 0x2, 
    0x2, 0x5ff, 0x5f8, 0x3, 0x2, 0x2, 0x2, 0x5ff, 0x5f9, 0x3, 0x2, 0x2, 
    0x2, 0x5ff, 0x5fa, 0x3, 0x2, 0x2, 0x2, 0x5ff, 0x5fb, 0x3, 0x2, 0x2, 
    0x2, 0x5ff, 0x5fc, 0x3, 0x2, 0x2, 0x2, 0x5ff, 0x5fd, 0x3, 0x2, 0x2, 
    0x2, 0x5ff, 0x5fe, 0x3, 0x2, 0x2, 0x2, 0x600, 0x9f, 0x3, 0x2, 0x2, 0x2, 
    0x601, 0x608, 0x7, 0x4f, 0x2, 0x2, 0x602, 0x608, 0x5, 0xa2, 0x52, 0x2, 
    0x603, 0x608, 0x5, 0xa4, 0x53, 0x2, 0x604, 0x608, 0x5, 0xa6, 0x54, 0x2, 
    0x605, 0x608, 0x5, 0xa8, 0x55, 0x2, 0x606, 0x608, 0x5, 0xb0, 0x59, 0x2, 
    0x607, 0x601, 0x3, 0x2, 0x2, 0x2, 0x607, 0x602, 0x3, 0x2, 0x2, 0x2, 
    0x607, 0x603, 0x3, 0x2, 0x2, 0x2, 0x607, 0x604, 0x3, 0x2, 0x2, 0x2, 
    0x607, 0x605, 0x3, 0x2, 0x2, 0x2, 0x607, 0x606, 0x3, 0x2, 0x2, 0x2, 
    0x608, 0xa1, 0x3, 0x2, 0x2, 0x2, 0x609, 0x60a, 0x9, 0x1c, 0x2, 0x2, 
    0x60a, 0xa3, 0x3, 0x2, 0x2, 0x2, 0x60b, 0x60c, 0x7, 0xbb, 0x2, 0x2, 
    0x60c, 0xa5, 0x3, 0x2, 0x2, 0x2, 0x60d, 0x60e, 0x7, 0xba, 0x2, 0x2, 
    0x60e, 0xa7, 0x3, 0x2, 0x2, 0x2, 0x60f, 0x610, 0x7, 0xb9, 0x2, 0x2, 
    0x610, 0xa9, 0x3, 0x2, 0x2, 0x2, 0x611, 0x612, 0x7, 0x2b, 0x2, 0x2, 
    0x612, 0x613, 0x5, 0xb0, 0x59, 0x2, 0x613, 0xab, 0x3, 0x2, 0x2, 0x2, 
    0x614, 0x615, 0x7, 0x75, 0x2, 0x2, 0x615, 0x616, 0x5, 0xb0, 0x59, 0x2, 
    0x616, 0xad, 0x3, 0x2, 0x2, 0x2, 0x617, 0x618, 0x7, 0x76, 0x2, 0x2, 
    0x618, 0x619, 0x5, 0xb0, 0x59, 0x2, 0x619, 0xaf, 0x3, 0x2, 0x2, 0x2, 
    0x61a, 0x61b, 0x7, 0xb6, 0x2, 0x2, 0x61b, 0xb1, 0x3, 0x2, 0x2, 0x2, 
    0x61c, 0x620, 0x5, 0xb6, 0x5c, 0x2, 0x61d, 0x620, 0x5, 0xb8, 0x5d, 0x2, 
    0x61e, 0x620, 0x5, 0xb4, 0x5b, 0x2, 0x61f, 0x61c, 0x3, 0x2, 0x2, 0x2, 
    0x61f, 0x61d, 0x3, 0x2, 0x2, 0x2, 0x61f, 0x61e, 0x3, 0x2, 0x2, 0x2, 
    0x620, 0xb3, 0x3, 0x2, 0x2, 0x2, 0x621, 0x622, 0x7, 0x77, 0x2, 0x2, 
    0x622, 0x623, 0x5, 0x6e, 0x38, 0x2, 0x623, 0xb5, 0x3, 0x2, 0x2, 0x2, 
    0x624, 0x625, 0x7, 0x77, 0x2, 0x2, 0x625, 0x63d, 0x5, 0xb0, 0x59, 0x2, 
    0x626, 0x62b, 0x7, 0xa8, 0x2, 0x2, 0x627, 0x628, 0x7, 0x7, 0x2, 0x2, 
    0x628, 0x629, 0x5, 0x98, 0x4d, 0x2, 0x629, 0x62a, 0x7, 0x8, 0x2, 0x2, 
    0x62a, 0x62c, 0x3, 0x2, 0x2, 0x2, 0x62b, 0x627, 0x3, 0x2, 0x2, 0x2, 
    0x62b, 0x62c, 0x3, 0x2, 0x2, 0x2, 0x62c, 0x63e, 0x3, 0x2, 0x2, 0x2, 
    0x62d, 0x632, 0x7, 0xa8, 0x2, 0x2, 0x62e, 0x62f, 0x7, 0x7, 0x2, 0x2, 
    0x62f, 0x630, 0x5, 0x98, 0x4d, 0x2, 0x630, 0x631, 0x7, 0x8, 0x2, 0x2, 
    0x631, 0x633, 0x3, 0x2, 0x2, 0x2, 0x632, 0x62e, 0x3, 0x2, 0x2, 0x2, 
    0x632, 0x633, 0x3, 0x2, 0x2, 0x2, 0x633, 0x634, 0x3, 0x2, 0x2, 0x2, 
    0x634, 0x635, 0x7, 0x63, 0x2, 0x2, 0x635, 0x63e, 0x7, 0xa0, 0x2, 0x2, 
    0x636, 0x63b, 0x7, 0xa0, 0x2, 0x2, 0x637, 0x638, 0x7, 0x7, 0x2, 0x2, 
    0x638, 0x639, 0x5, 0x98, 0x4d, 0x2, 0x639, 0x63a, 0x7, 0x8, 0x2, 0x2, 
    0x63a, 0x63c, 0x3, 0x2, 0x2, 0x2, 0x63b, 0x637, 0x3, 0x2, 0x2, 0x2, 
    0x63b, 0x63c, 0x3, 0x2, 0x2, 0x2, 0x63c, 0x63e, 0x3, 0x2, 0x2, 0x2, 
    0x63d, 0x626, 0x3, 0x2, 0x2, 0x2, 0x63d, 0x62d, 0x3, 0x2, 0x2, 0x2, 
    0x63d, 0x636, 0x3, 0x2, 0x2, 0x2, 0x63e, 0xb7, 0x3, 0x2, 0x2, 0x2, 0x63f, 
    0x640, 0x7, 0x77, 0x2, 0x2, 0x640, 0x695, 0x5, 0xb0, 0x59, 0x2, 0x641, 
    0x646, 0x7, 0x9a, 0x2, 0x2, 0x642, 0x643, 0x7, 0x7, 0x2, 0x2, 0x643, 
    0x644, 0x5, 0x98, 0x4d, 0x2, 0x644, 0x645, 0x7, 0x8, 0x2, 0x2, 0x645, 
    0x647, 0x3, 0x2, 0x2, 0x2, 0x646, 0x642, 0x3, 0x2, 0x2, 0x2, 0x646, 
    0x647, 0x3, 0x2, 0x2, 0x2, 0x647, 0x696, 0x3, 0x2, 0x2, 0x2, 0x648, 
    0x64d, 0x7, 0x9a, 0x2, 0x2, 0x649, 0x64a, 0x7, 0x7, 0x2, 0x2, 0x64a, 
    0x64b, 0x5, 0x98, 0x4d, 0x2, 0x64b, 0x64c, 0x7, 0x8, 0x2, 0x2, 0x64c, 
    0x64e, 0x3, 0x2, 0x2, 0x2, 0x64d, 0x649, 0x3, 0x2, 0x2, 0x2, 0x64d, 
    0x64e, 0x3, 0x2, 0x2, 0x2, 0x64e, 0x64f, 0x3, 0x2, 0x2, 0x2, 0x64f, 
    0x659, 0x7, 0x63, 0x2, 0x2, 0x650, 0x65a, 0x7, 0x9c, 0x2, 0x2, 0x651, 
    0x65a, 0x7, 0x9e, 0x2, 0x2, 0x652, 0x657, 0x7, 0xa4, 0x2, 0x2, 0x653, 
    0x654, 0x7, 0x7, 0x2, 0x2, 0x654, 0x655, 0x5, 0x9a, 0x4e, 0x2, 0x655, 
    0x656, 0x7, 0x8, 0x2, 0x2, 0x656, 0x658, 0x3, 0x2, 0x2, 0x2, 0x657, 
    0x653, 0x3, 0x2, 0x2, 0x2, 0x657, 0x658, 0x3, 0x2, 0x2, 0x2, 0x658, 
    0x65a, 0x3, 0x2, 0x2, 0x2, 0x659, 0x650, 0x3, 0x2, 0x2, 0x2, 0x659, 
    0x651, 0x3, 0x2, 0x2, 0x2, 0x659, 0x652, 0x3, 0x2, 0x2, 0x2, 0x65a, 
    0x696, 0x3, 0x2, 0x2, 0x2, 0x65b, 0x660, 0x7, 0x9c, 0x2, 0x2, 0x65c, 
    0x65d, 0x7, 0x7, 0x2, 0x2, 0x65d, 0x65e, 0x5, 0x98, 0x4d, 0x2, 0x65e, 
    0x65f, 0x7, 0x8, 0x2, 0x2, 0x65f, 0x661, 0x3, 0x2, 0x2, 0x2, 0x660, 
    0x65c, 0x3, 0x2, 0x2, 0x2, 0x660, 0x661, 0x3, 0x2, 0x2, 0x2, 0x661, 
    0x662, 0x3, 0x2, 0x2, 0x2, 0x662, 0x66b, 0x7, 0x63, 0x2, 0x2, 0x663, 
    0x66c, 0x7, 0x9e, 0x2, 0x2, 0x664, 0x669, 0x7, 0xa4, 0x2, 0x2, 0x665, 
    0x666, 0x7, 0x7, 0x2, 0x2, 0x666, 0x667, 0x5, 0x9a, 0x4e, 0x2, 0x667, 
    0x668, 0x7, 0x8, 0x2, 0x2, 0x668, 0x66a, 0x3, 0x2, 0x2, 0x2, 0x669, 
    0x665, 0x3, 0x2, 0x2, 0x2, 0x669, 0x66a, 0x3, 0x2, 0x2, 0x2, 0x66a, 
    0x66c, 0x3, 0x2, 0x2, 0x2, 0x66b, 0x663, 0x3, 0x2, 0x2, 0x2, 0x66b, 
    0x664, 0x3, 0x2, 0x2, 0x2, 0x66c, 0x696, 0x3, 0x2, 0x2, 0x2, 0x66d, 
    0x672, 0x7, 0x9e, 0x2, 0x2, 0x66e, 0x66f, 0x7, 0x7, 0x2, 0x2, 0x66f, 
    0x670, 0x5, 0x98, 0x4d, 0x2, 0x670, 0x671, 0x7, 0x8, 0x2, 0x2, 0x671, 
    0x673, 0x3, 0x2, 0x2, 0x2, 0x672, 0x66e, 0x3, 0x2, 0x2, 0x2, 0x672, 
    0x673, 0x3, 0x2, 0x2, 0x2, 0x673, 0x674, 0x3, 0x2, 0x2, 0x2, 0x674, 
    0x675, 0x7, 0x63, 0x2, 0x2, 0x675, 0x67a, 0x7, 0xa4, 0x2, 0x2, 0x676, 
    0x677, 0x7, 0x7, 0x2, 0x2, 0x677, 0x678, 0x5, 0x9a, 0x4e, 0x2, 0x678, 
    0x679, 0x7, 0x8, 0x2, 0x2, 0x679, 0x67b, 0x3, 0x2, 0x2, 0x2, 0x67a, 
    0x676, 0x3, 0x2, 0x2, 0x2, 0x67a, 0x67b, 0x3, 0x2, 0x2, 0x2, 0x67b, 
    0x696, 0x3, 0x2, 0x2, 0x2, 0x67c, 0x681, 0x7, 0x9c, 0x2, 0x2, 0x67d, 
    0x67e, 0x7, 0x7, 0x2, 0x2, 0x67e, 0x67f, 0x5, 0x98, 0x4d, 0x2, 0x67f, 
    0x680, 0x7, 0x8, 0x2, 0x2, 0x680, 0x682, 0x3, 0x2, 0x2, 0x2, 0x681, 
    0x67d, 0x3, 0x2, 0x2, 0x2, 0x681, 0x682, 0x3, 0x2, 0x2, 0x2, 0x682, 
    0x696, 0x3, 0x2, 0x2, 0x2, 0x683, 0x688, 0x7, 0x9e, 0x2, 0x2, 0x684, 
    0x685, 0x7, 0x7, 0x2, 0x2, 0x685, 0x686, 0x5, 0x98, 0x4d, 0x2, 0x686, 
    0x687, 0x7, 0x8, 0x2, 0x2, 0x687, 0x689, 0x3, 0x2, 0x2, 0x2, 0x688, 
    0x684, 0x3, 0x2, 0x2, 0x2, 0x688, 0x689, 0x3, 0x2, 0x2, 0x2, 0x689, 
    0x696, 0x3, 0x2, 0x2, 0x2, 0x68a, 0x693, 0x7, 0xa4, 0x2, 0x2, 0x68b, 
    0x68c, 0x7, 0x7, 0x2, 0x2, 0x68c, 0x68f, 0x5, 0x98, 0x4d, 0x2, 0x68d, 
    0x68e, 0x7, 0x5, 0x2, 0x2, 0x68e, 0x690, 0x5, 0x9a, 0x4e, 0x2, 0x68f, 
    0x68d, 0x3, 0x2, 0x2, 0x2, 0x68f, 0x690, 0x3, 0x2, 0x2, 0x2, 0x690, 
    0x691, 0x3, 0x2, 0x2, 0x2, 0x691, 0x692, 0x7, 0x8, 0x2, 0x2, 0x692, 
    0x694, 0x3, 0x2, 0x2, 0x2, 0x693, 0x68b, 0x3, 0x2, 0x2, 0x2, 0x693, 
    0x694, 0x3, 0x2, 0x2, 0x2, 0x694, 0x696, 0x3, 0x2, 0x2, 0x2, 0x695, 
    0x641, 0x3, 0x2, 0x2, 0x2, 0x695, 0x648, 0x3, 0x2, 0x2, 0x2, 0x695, 
    0x65b, 0x3, 0x2, 0x2, 0x2, 0x695, 0x66d, 0x3, 0x2, 0x2, 0x2, 0x695, 
    0x67c, 0x3, 0x2, 0x2, 0x2, 0x695, 0x683, 0x3, 0x2, 0x2, 0x2, 0x695, 
    0x68a, 0x3, 0x2, 0x2, 0x2, 0x696, 0xb9, 0x3, 0x2, 0x2, 0x2, 0x697, 0x698, 
    0x9, 0x1d, 0x2, 0x2, 0x698, 0xbb, 0x3, 0x2, 0x2, 0x2, 0xe7, 0xbe, 0xc6, 
    0xcd, 0xd2, 0xd9, 0xde, 0xe5, 0xf0, 0xf4, 0xfa, 0x106, 0x10e, 0x113, 
    0x11b, 0x11e, 0x124, 0x12a, 0x12c, 0x131, 0x136, 0x13a, 0x13f, 0x143, 
    0x149, 0x151, 0x154, 0x15a, 0x160, 0x162, 0x16a, 0x170, 0x173, 0x178, 
    0x181, 0x18a, 0x18f, 0x199, 0x19c, 0x1a2, 0x1a5, 0x1ab, 0x1ae, 0x1b0, 
    0x1b6, 0x1bb, 0x1be, 0x1c7, 0x1ca, 0x1d3, 0x1d5, 0x1db, 0x1e0, 0x1e7, 
    0x1e9, 0x1ee, 0x1f6, 0x1fb, 0x20a, 0x213, 0x216, 0x21a, 0x223, 0x228, 
    0x22d, 0x235, 0x238, 0x23c, 0x243, 0x24b, 0x24e, 0x25d, 0x264, 0x268, 
    0x26c, 0x272, 0x277, 0x27a, 0x27f, 0x282, 0x286, 0x28b, 0x28e, 0x290, 
    0x294, 0x29a, 0x2a1, 0x2a9, 0x2af, 0x2b5, 0x2bc, 0x2c2, 0x2c6, 0x2ce, 
    0x2d4, 0x2d8, 0x2dd, 0x2e0, 0x2e4, 0x2e8, 0x2eb, 0x2ee, 0x2f6, 0x2fe, 
    0x305, 0x30a, 0x30c, 0x30e, 0x313, 0x31b, 0x329, 0x32e, 0x331, 0x33c, 
    0x341, 0x343, 0x346, 0x34a, 0x34d, 0x356, 0x35b, 0x35f, 0x363, 0x36a, 
    0x36d, 0x374, 0x378, 0x37e, 0x383, 0x385, 0x389, 0x38f, 0x393, 0x39c, 
    0x3a2, 0x3a5, 0x3aa, 0x3ae, 0x3b1, 0x3b5, 0x3bb, 0x3c3, 0x3c7, 0x3ca, 
    0x3d2, 0x3db, 0x3e1, 0x3e3, 0x3ea, 0x3ef, 0x3f2, 0x3f5, 0x3fc, 0x406, 
    0x40b, 0x412, 0x41c, 0x426, 0x42c, 0x433, 0x45c, 0x465, 0x467, 0x46b, 
    0x48d, 0x490, 0x49a, 0x49d, 0x4a7, 0x4aa, 0x4b6, 0x4c8, 0x4cf, 0x4d1, 
    0x4da, 0x4dd, 0x4e0, 0x4e4, 0x4f2, 0x4fa, 0x503, 0x50c, 0x515, 0x51e, 
    0x52a, 0x52d, 0x53a, 0x543, 0x550, 0x556, 0x56f, 0x572, 0x579, 0x57e, 
    0x587, 0x590, 0x593, 0x596, 0x598, 0x5a0, 0x5a5, 0x5aa, 0x5b1, 0x5b6, 
    0x5bb, 0x5c4, 0x5e3, 0x5e7, 0x5eb, 0x5ff, 0x607, 0x61f, 0x62b, 0x632, 
    0x63b, 0x63d, 0x646, 0x64d, 0x657, 0x659, 0x660, 0x669, 0x66b, 0x672, 
    0x67a, 0x681, 0x688, 0x68f, 0x693, 0x695, 
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
