
// Generated from /Users/craig/repos/rapidsse/thirdparty/antlr/json5.g4 by ANTLR 4.8


#include "json5Visitor.h"

#include "json5Parser.h"


using namespace antlrcpp;
using namespace Parser;
using namespace antlr4;

json5Parser::json5Parser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

json5Parser::~json5Parser() {
  delete _interpreter;
}

std::string json5Parser::getGrammarFileName() const {
  return "json5.g4";
}

const std::vector<std::string>& json5Parser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& json5Parser::getVocabulary() const {
  return _vocabulary;
}


//----------------- Json5Context ------------------------------------------------------------------

json5Parser::Json5Context::Json5Context(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* json5Parser::Json5Context::EOF() {
  return getToken(json5Parser::EOF, 0);
}

json5Parser::ValueContext* json5Parser::Json5Context::value() {
  return getRuleContext<json5Parser::ValueContext>(0);
}


size_t json5Parser::Json5Context::getRuleIndex() const {
  return json5Parser::RuleJson5;
}


antlrcpp::Any json5Parser::Json5Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<json5Visitor*>(visitor))
    return parserVisitor->visitJson5(this);
  else
    return visitor->visitChildren(this);
}

json5Parser::Json5Context* json5Parser::json5() {
  Json5Context *_localctx = _tracker.createInstance<Json5Context>(_ctx, getState());
  enterRule(_localctx, 0, json5Parser::RuleJson5);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(13);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << json5Parser::T__0)
      | (1ULL << json5Parser::T__4)
      | (1ULL << json5Parser::LITERAL)
      | (1ULL << json5Parser::STRING)
      | (1ULL << json5Parser::NUMBER))) != 0)) {
      setState(12);
      value();
    }
    setState(15);
    match(json5Parser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjContext ------------------------------------------------------------------

json5Parser::ObjContext::ObjContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<json5Parser::PairContext *> json5Parser::ObjContext::pair() {
  return getRuleContexts<json5Parser::PairContext>();
}

json5Parser::PairContext* json5Parser::ObjContext::pair(size_t i) {
  return getRuleContext<json5Parser::PairContext>(i);
}


size_t json5Parser::ObjContext::getRuleIndex() const {
  return json5Parser::RuleObj;
}


antlrcpp::Any json5Parser::ObjContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<json5Visitor*>(visitor))
    return parserVisitor->visitObj(this);
  else
    return visitor->visitChildren(this);
}

json5Parser::ObjContext* json5Parser::obj() {
  ObjContext *_localctx = _tracker.createInstance<ObjContext>(_ctx, getState());
  enterRule(_localctx, 2, json5Parser::RuleObj);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    setState(33);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(17);
      match(json5Parser::T__0);
      setState(18);
      pair();
      setState(23);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(19);
          match(json5Parser::T__1);
          setState(20);
          pair(); 
        }
        setState(25);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx);
      }
      setState(27);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == json5Parser::T__1) {
        setState(26);
        match(json5Parser::T__1);
      }
      setState(29);
      match(json5Parser::T__2);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(31);
      match(json5Parser::T__0);
      setState(32);
      match(json5Parser::T__2);
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

//----------------- PairContext ------------------------------------------------------------------

json5Parser::PairContext::PairContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

json5Parser::KeyContext* json5Parser::PairContext::key() {
  return getRuleContext<json5Parser::KeyContext>(0);
}

json5Parser::ValueContext* json5Parser::PairContext::value() {
  return getRuleContext<json5Parser::ValueContext>(0);
}


size_t json5Parser::PairContext::getRuleIndex() const {
  return json5Parser::RulePair;
}


antlrcpp::Any json5Parser::PairContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<json5Visitor*>(visitor))
    return parserVisitor->visitPair(this);
  else
    return visitor->visitChildren(this);
}

json5Parser::PairContext* json5Parser::pair() {
  PairContext *_localctx = _tracker.createInstance<PairContext>(_ctx, getState());
  enterRule(_localctx, 4, json5Parser::RulePair);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(35);
    key();
    setState(36);
    match(json5Parser::T__3);
    setState(37);
    value();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- KeyContext ------------------------------------------------------------------

json5Parser::KeyContext::KeyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* json5Parser::KeyContext::STRING() {
  return getToken(json5Parser::STRING, 0);
}

tree::TerminalNode* json5Parser::KeyContext::IDENTIFIER() {
  return getToken(json5Parser::IDENTIFIER, 0);
}

tree::TerminalNode* json5Parser::KeyContext::LITERAL() {
  return getToken(json5Parser::LITERAL, 0);
}


size_t json5Parser::KeyContext::getRuleIndex() const {
  return json5Parser::RuleKey;
}


antlrcpp::Any json5Parser::KeyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<json5Visitor*>(visitor))
    return parserVisitor->visitKey(this);
  else
    return visitor->visitChildren(this);
}

json5Parser::KeyContext* json5Parser::key() {
  KeyContext *_localctx = _tracker.createInstance<KeyContext>(_ctx, getState());
  enterRule(_localctx, 6, json5Parser::RuleKey);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(39);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << json5Parser::LITERAL)
      | (1ULL << json5Parser::STRING)
      | (1ULL << json5Parser::IDENTIFIER))) != 0))) {
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

//----------------- ValueContext ------------------------------------------------------------------

json5Parser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* json5Parser::ValueContext::STRING() {
  return getToken(json5Parser::STRING, 0);
}

tree::TerminalNode* json5Parser::ValueContext::NUMBER() {
  return getToken(json5Parser::NUMBER, 0);
}

json5Parser::ObjContext* json5Parser::ValueContext::obj() {
  return getRuleContext<json5Parser::ObjContext>(0);
}

json5Parser::ArrContext* json5Parser::ValueContext::arr() {
  return getRuleContext<json5Parser::ArrContext>(0);
}

tree::TerminalNode* json5Parser::ValueContext::LITERAL() {
  return getToken(json5Parser::LITERAL, 0);
}


size_t json5Parser::ValueContext::getRuleIndex() const {
  return json5Parser::RuleValue;
}


antlrcpp::Any json5Parser::ValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<json5Visitor*>(visitor))
    return parserVisitor->visitValue(this);
  else
    return visitor->visitChildren(this);
}

json5Parser::ValueContext* json5Parser::value() {
  ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, getState());
  enterRule(_localctx, 8, json5Parser::RuleValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(46);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case json5Parser::STRING: {
        enterOuterAlt(_localctx, 1);
        setState(41);
        match(json5Parser::STRING);
        break;
      }

      case json5Parser::NUMBER: {
        enterOuterAlt(_localctx, 2);
        setState(42);
        match(json5Parser::NUMBER);
        break;
      }

      case json5Parser::T__0: {
        enterOuterAlt(_localctx, 3);
        setState(43);
        obj();
        break;
      }

      case json5Parser::T__4: {
        enterOuterAlt(_localctx, 4);
        setState(44);
        arr();
        break;
      }

      case json5Parser::LITERAL: {
        enterOuterAlt(_localctx, 5);
        setState(45);
        match(json5Parser::LITERAL);
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

//----------------- ArrContext ------------------------------------------------------------------

json5Parser::ArrContext::ArrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<json5Parser::ValueContext *> json5Parser::ArrContext::value() {
  return getRuleContexts<json5Parser::ValueContext>();
}

json5Parser::ValueContext* json5Parser::ArrContext::value(size_t i) {
  return getRuleContext<json5Parser::ValueContext>(i);
}


size_t json5Parser::ArrContext::getRuleIndex() const {
  return json5Parser::RuleArr;
}


antlrcpp::Any json5Parser::ArrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<json5Visitor*>(visitor))
    return parserVisitor->visitArr(this);
  else
    return visitor->visitChildren(this);
}

json5Parser::ArrContext* json5Parser::arr() {
  ArrContext *_localctx = _tracker.createInstance<ArrContext>(_ctx, getState());
  enterRule(_localctx, 10, json5Parser::RuleArr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    setState(64);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(48);
      match(json5Parser::T__4);
      setState(49);
      value();
      setState(54);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(50);
          match(json5Parser::T__1);
          setState(51);
          value(); 
        }
        setState(56);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
      }
      setState(58);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == json5Parser::T__1) {
        setState(57);
        match(json5Parser::T__1);
      }
      setState(60);
      match(json5Parser::T__5);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(62);
      match(json5Parser::T__4);
      setState(63);
      match(json5Parser::T__5);
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

// Static vars and initialization.
std::vector<dfa::DFA> json5Parser::_decisionToDFA;
atn::PredictionContextCache json5Parser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN json5Parser::_atn;
std::vector<uint16_t> json5Parser::_serializedATN;

std::vector<std::string> json5Parser::_ruleNames = {
  "json5", "obj", "pair", "key", "value", "arr"
};

std::vector<std::string> json5Parser::_literalNames = {
  "", "'{'", "','", "'}'", "':'", "'['", "']'"
};

std::vector<std::string> json5Parser::_symbolicNames = {
  "", "", "", "", "", "", "", "SINGLE_LINE_COMMENT", "MULTI_LINE_COMMENT", 
  "LITERAL", "STRING", "IDENTIFIER", "NUMBER", "WS"
};

dfa::Vocabulary json5Parser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> json5Parser::_tokenNames;

json5Parser::Initializer::Initializer() {
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
    0x3, 0xf, 0x45, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x3, 
    0x2, 0x5, 0x2, 0x10, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x7, 0x3, 0x18, 0xa, 0x3, 0xc, 0x3, 0xe, 0x3, 0x1b, 
    0xb, 0x3, 0x3, 0x3, 0x5, 0x3, 0x1e, 0xa, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x5, 0x3, 0x24, 0xa, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 
    0x3, 0x6, 0x5, 0x6, 0x31, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x7, 0x7, 0x7, 0x37, 0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 0x3a, 0xb, 0x7, 0x3, 
    0x7, 0x5, 0x7, 0x3d, 0xa, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
    0x5, 0x7, 0x43, 0xa, 0x7, 0x3, 0x7, 0x2, 0x2, 0x8, 0x2, 0x4, 0x6, 0x8, 
    0xa, 0xc, 0x2, 0x3, 0x3, 0x2, 0xb, 0xd, 0x2, 0x49, 0x2, 0xf, 0x3, 0x2, 
    0x2, 0x2, 0x4, 0x23, 0x3, 0x2, 0x2, 0x2, 0x6, 0x25, 0x3, 0x2, 0x2, 0x2, 
    0x8, 0x29, 0x3, 0x2, 0x2, 0x2, 0xa, 0x30, 0x3, 0x2, 0x2, 0x2, 0xc, 0x42, 
    0x3, 0x2, 0x2, 0x2, 0xe, 0x10, 0x5, 0xa, 0x6, 0x2, 0xf, 0xe, 0x3, 0x2, 
    0x2, 0x2, 0xf, 0x10, 0x3, 0x2, 0x2, 0x2, 0x10, 0x11, 0x3, 0x2, 0x2, 
    0x2, 0x11, 0x12, 0x7, 0x2, 0x2, 0x3, 0x12, 0x3, 0x3, 0x2, 0x2, 0x2, 
    0x13, 0x14, 0x7, 0x3, 0x2, 0x2, 0x14, 0x19, 0x5, 0x6, 0x4, 0x2, 0x15, 
    0x16, 0x7, 0x4, 0x2, 0x2, 0x16, 0x18, 0x5, 0x6, 0x4, 0x2, 0x17, 0x15, 
    0x3, 0x2, 0x2, 0x2, 0x18, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x19, 0x17, 0x3, 
    0x2, 0x2, 0x2, 0x19, 0x1a, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x1d, 0x3, 0x2, 
    0x2, 0x2, 0x1b, 0x19, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x1e, 0x7, 0x4, 0x2, 
    0x2, 0x1d, 0x1c, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x1e, 0x3, 0x2, 0x2, 0x2, 
    0x1e, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x1f, 0x20, 0x7, 0x5, 0x2, 0x2, 0x20, 
    0x24, 0x3, 0x2, 0x2, 0x2, 0x21, 0x22, 0x7, 0x3, 0x2, 0x2, 0x22, 0x24, 
    0x7, 0x5, 0x2, 0x2, 0x23, 0x13, 0x3, 0x2, 0x2, 0x2, 0x23, 0x21, 0x3, 
    0x2, 0x2, 0x2, 0x24, 0x5, 0x3, 0x2, 0x2, 0x2, 0x25, 0x26, 0x5, 0x8, 
    0x5, 0x2, 0x26, 0x27, 0x7, 0x6, 0x2, 0x2, 0x27, 0x28, 0x5, 0xa, 0x6, 
    0x2, 0x28, 0x7, 0x3, 0x2, 0x2, 0x2, 0x29, 0x2a, 0x9, 0x2, 0x2, 0x2, 
    0x2a, 0x9, 0x3, 0x2, 0x2, 0x2, 0x2b, 0x31, 0x7, 0xc, 0x2, 0x2, 0x2c, 
    0x31, 0x7, 0xe, 0x2, 0x2, 0x2d, 0x31, 0x5, 0x4, 0x3, 0x2, 0x2e, 0x31, 
    0x5, 0xc, 0x7, 0x2, 0x2f, 0x31, 0x7, 0xb, 0x2, 0x2, 0x30, 0x2b, 0x3, 
    0x2, 0x2, 0x2, 0x30, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x30, 0x2d, 0x3, 0x2, 
    0x2, 0x2, 0x30, 0x2e, 0x3, 0x2, 0x2, 0x2, 0x30, 0x2f, 0x3, 0x2, 0x2, 
    0x2, 0x31, 0xb, 0x3, 0x2, 0x2, 0x2, 0x32, 0x33, 0x7, 0x7, 0x2, 0x2, 
    0x33, 0x38, 0x5, 0xa, 0x6, 0x2, 0x34, 0x35, 0x7, 0x4, 0x2, 0x2, 0x35, 
    0x37, 0x5, 0xa, 0x6, 0x2, 0x36, 0x34, 0x3, 0x2, 0x2, 0x2, 0x37, 0x3a, 
    0x3, 0x2, 0x2, 0x2, 0x38, 0x36, 0x3, 0x2, 0x2, 0x2, 0x38, 0x39, 0x3, 
    0x2, 0x2, 0x2, 0x39, 0x3c, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x38, 0x3, 0x2, 
    0x2, 0x2, 0x3b, 0x3d, 0x7, 0x4, 0x2, 0x2, 0x3c, 0x3b, 0x3, 0x2, 0x2, 
    0x2, 0x3c, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3e, 0x3, 0x2, 0x2, 0x2, 
    0x3e, 0x3f, 0x7, 0x8, 0x2, 0x2, 0x3f, 0x43, 0x3, 0x2, 0x2, 0x2, 0x40, 
    0x41, 0x7, 0x7, 0x2, 0x2, 0x41, 0x43, 0x7, 0x8, 0x2, 0x2, 0x42, 0x32, 
    0x3, 0x2, 0x2, 0x2, 0x42, 0x40, 0x3, 0x2, 0x2, 0x2, 0x43, 0xd, 0x3, 
    0x2, 0x2, 0x2, 0xa, 0xf, 0x19, 0x1d, 0x23, 0x30, 0x38, 0x3c, 0x42, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

json5Parser::Initializer json5Parser::_init;
