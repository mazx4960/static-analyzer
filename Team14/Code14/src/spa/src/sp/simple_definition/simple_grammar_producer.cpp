#include "simple_grammar_producer.h"

BracedGrammarProducer::BracedGrammarProducer(GrammarRuleProducer* producer) : braced_rule_(producer) {}

BracedGrammarRule* BracedGrammarProducer::produce() { return new BracedGrammarRule(braced_rule_); }

ProgramGrammarRule* ProgramGrammarProducer::produce() { return new ProgramGrammarRule(); }

ProcedureGrammarRule* ProcedureGrammarProducer::produce() { return new ProcedureGrammarRule(); }

StatementListGrammarRule* StatementListGrammarProducer::produce() { return new StatementListGrammarRule(); }

StatementGrammarRule* StatementGrammarProducer::produce() { return new StatementGrammarRule(); }

ReadGrammarRule* ReadGrammarProducer::produce() { return new ReadGrammarRule(); }

PrintGrammarRule* PrintGrammarProducer::produce() { return new PrintGrammarRule(); }

CallGrammarRule* CallGrammarProducer::produce() { return new CallGrammarRule(); }

WhileGrammarRule* WhileGrammarProducer::produce() { return new WhileGrammarRule(); }

IfGrammarRule* IfGrammarProducer::produce() { return new IfGrammarRule(); }

AssignGrammarRule* AssignGrammarProducer::produce() { return new AssignGrammarRule(); }

CondExprGrammarRule* CondExprGrammarProducer::produce() { return new CondExprGrammarRule(); }

NotExprGrammarRule* NotExprGrammarProducer::produce() { return new NotExprGrammarRule(); }

BinaryCondGrammarRule* BinaryCondGrammarProducer::produce() { return new BinaryCondGrammarRule(); }

RelExprGrammarRule* RelExprGrammarProducer::produce() { return new RelExprGrammarRule(); }
