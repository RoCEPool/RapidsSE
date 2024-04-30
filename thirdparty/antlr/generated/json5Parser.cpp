
// Generated from /Users/alka/rapidsse/thirdparty/antlr/json5.g4 by ANTLR 4.13.1


#include "json5Visitor.h"

#include "json5Parser.h"


using namespace antlrcpp;
using namespace Parser;

using namespace antlr4;

namespace {

struct Json5ParserStaticData final {
  Json5ParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  Json5ParserStaticData(const Json5ParserStaticData&) = delete;
  Json5ParserStaticData(Json5ParserStaticData&&) = delete;
  Json5ParserStaticData& operator=(const Json5ParserStaticData&) = delete;
  Json5ParserStaticData& operator=(Json5ParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag json5ParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
Json5ParserStaticData *json5ParserStaticData = nullptr;

void json5ParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (json5ParserStaticData != nullptr) {
    return;
  }
#else
  assert(json5ParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<Json5ParserStaticData>(
    std::vector<std::string>{
      "json5", "obj", "pair", "key", "value", "arr"
    },
    std::vector<std::string>{
      "", "'{'", "','", "'}'", "':'", "'['", "']'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "SINGLE_LINE_COMMENT", "MULTI_LINE_COMMENT", 
      "LITERAL", "STRING", "IDENTIFIER", "NUMBER", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,13,67,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,1,0,3,0,14,
  	8,0,1,0,1,0,1,1,1,1,1,1,1,1,5,1,22,8,1,10,1,12,1,25,9,1,1,1,3,1,28,8,
  	1,1,1,1,1,1,1,1,1,3,1,34,8,1,1,2,1,2,1,2,1,2,1,3,1,3,1,4,1,4,1,4,1,4,
  	1,4,3,4,47,8,4,1,5,1,5,1,5,1,5,5,5,53,8,5,10,5,12,5,56,9,5,1,5,3,5,59,
  	8,5,1,5,1,5,1,5,1,5,3,5,65,8,5,1,5,0,0,6,0,2,4,6,8,10,0,1,1,0,9,11,71,
  	0,13,1,0,0,0,2,33,1,0,0,0,4,35,1,0,0,0,6,39,1,0,0,0,8,46,1,0,0,0,10,64,
  	1,0,0,0,12,14,3,8,4,0,13,12,1,0,0,0,13,14,1,0,0,0,14,15,1,0,0,0,15,16,
  	5,0,0,1,16,1,1,0,0,0,17,18,5,1,0,0,18,23,3,4,2,0,19,20,5,2,0,0,20,22,
  	3,4,2,0,21,19,1,0,0,0,22,25,1,0,0,0,23,21,1,0,0,0,23,24,1,0,0,0,24,27,
  	1,0,0,0,25,23,1,0,0,0,26,28,5,2,0,0,27,26,1,0,0,0,27,28,1,0,0,0,28,29,
  	1,0,0,0,29,30,5,3,0,0,30,34,1,0,0,0,31,32,5,1,0,0,32,34,5,3,0,0,33,17,
  	1,0,0,0,33,31,1,0,0,0,34,3,1,0,0,0,35,36,3,6,3,0,36,37,5,4,0,0,37,38,
  	3,8,4,0,38,5,1,0,0,0,39,40,7,0,0,0,40,7,1,0,0,0,41,47,5,10,0,0,42,47,
  	5,12,0,0,43,47,3,2,1,0,44,47,3,10,5,0,45,47,5,9,0,0,46,41,1,0,0,0,46,
  	42,1,0,0,0,46,43,1,0,0,0,46,44,1,0,0,0,46,45,1,0,0,0,47,9,1,0,0,0,48,
  	49,5,5,0,0,49,54,3,8,4,0,50,51,5,2,0,0,51,53,3,8,4,0,52,50,1,0,0,0,53,
  	56,1,0,0,0,54,52,1,0,0,0,54,55,1,0,0,0,55,58,1,0,0,0,56,54,1,0,0,0,57,
  	59,5,2,0,0,58,57,1,0,0,0,58,59,1,0,0,0,59,60,1,0,0,0,60,61,5,6,0,0,61,
  	65,1,0,0,0,62,63,5,5,0,0,63,65,5,6,0,0,64,48,1,0,0,0,64,62,1,0,0,0,65,
  	11,1,0,0,0,8,13,23,27,33,46,54,58,64
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  json5ParserStaticData = staticData.release();
}

}

json5Parser::json5Parser(TokenStream *input) : json5Parser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

json5Parser::json5Parser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  json5Parser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *json5ParserStaticData->atn, json5ParserStaticData->decisionToDFA, json5ParserStaticData->sharedContextCache, options);
}

json5Parser::~json5Parser() {
  delete _interpreter;
}

const atn::ATN& json5Parser::getATN() const {
  return *json5ParserStaticData->atn;
}

std::string json5Parser::getGrammarFileName() const {
  return "json5.g4";
}

const std::vector<std::string>& json5Parser::getRuleNames() const {
  return json5ParserStaticData->ruleNames;
}

const dfa::Vocabulary& json5Parser::getVocabulary() const {
  return json5ParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView json5Parser::getSerializedATN() const {
  return json5ParserStaticData->serializedATN;
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


std::any json5Parser::Json5Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<json5Visitor*>(visitor))
    return parserVisitor->visitJson5(this);
  else
    return visitor->visitChildren(this);
}

json5Parser::Json5Context* json5Parser::json5() {
  Json5Context *_localctx = _tracker.createInstance<Json5Context>(_ctx, getState());
  enterRule(_localctx, 0, json5Parser::RuleJson5);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(13);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 5666) != 0)) {
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


std::any json5Parser::ObjContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<json5Visitor*>(visitor))
    return parserVisitor->visitObj(this);
  else
    return visitor->visitChildren(this);
}

json5Parser::ObjContext* json5Parser::obj() {
  ObjContext *_localctx = _tracker.createInstance<ObjContext>(_ctx, getState());
  enterRule(_localctx, 2, json5Parser::RuleObj);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
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


std::any json5Parser::PairContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<json5Visitor*>(visitor))
    return parserVisitor->visitPair(this);
  else
    return visitor->visitChildren(this);
}

json5Parser::PairContext* json5Parser::pair() {
  PairContext *_localctx = _tracker.createInstance<PairContext>(_ctx, getState());
  enterRule(_localctx, 4, json5Parser::RulePair);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
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


std::any json5Parser::KeyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<json5Visitor*>(visitor))
    return parserVisitor->visitKey(this);
  else
    return visitor->visitChildren(this);
}

json5Parser::KeyContext* json5Parser::key() {
  KeyContext *_localctx = _tracker.createInstance<KeyContext>(_ctx, getState());
  enterRule(_localctx, 6, json5Parser::RuleKey);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(39);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3584) != 0))) {
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


std::any json5Parser::ValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<json5Visitor*>(visitor))
    return parserVisitor->visitValue(this);
  else
    return visitor->visitChildren(this);
}

json5Parser::ValueContext* json5Parser::value() {
  ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, getState());
  enterRule(_localctx, 8, json5Parser::RuleValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
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


std::any json5Parser::ArrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<json5Visitor*>(visitor))
    return parserVisitor->visitArr(this);
  else
    return visitor->visitChildren(this);
}

json5Parser::ArrContext* json5Parser::arr() {
  ArrContext *_localctx = _tracker.createInstance<ArrContext>(_ctx, getState());
  enterRule(_localctx, 10, json5Parser::RuleArr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
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

void json5Parser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  json5ParserInitialize();
#else
  ::antlr4::internal::call_once(json5ParserOnceFlag, json5ParserInitialize);
#endif
}
