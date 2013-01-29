#include "context.h"
#include "compilation_unit.h"
#include <stdio.h>

// Runtime Context

extern "C" {

Runtime_Context::Runtime_Context() {
	st = new SymbolInfoTable();
}
;

Runtime_Context::Runtime_Context(Tmodule *mod) {
	prog = mod;
	st = new SymbolInfoTable();
}
Tmodule *Runtime_Context::get_program() {
	return prog;
}

SymbolInfoTable *Runtime_Context::get_symboltable() {
	return st;
}

void Runtime_Context::set_symboltable(SymbolInfoTable *_st) {
	st = _st;
}
void Runtime_Context::assign_symbol(Variable *var, SymbolInfo *value) {
	st->assign(var, value);
}

void Runtime_Context::add_symbol(SymbolInfo *info) {
	st->add(info);
}

SymbolInfo * Runtime_Context::get_symbol(string name) {
	return st->get(name);
}
void Runtime_Context::setup_js_codegen(FILE *fd) {
    js_fd = fd;
}

void Runtime_Context::update_stream(string str) {
    fwrite(str.c_str(),1,str.length(),js_fd);    
}

Runtime_Context::~Runtime_Context() {
  
}
// Compilation Context

Compilation_Context::Compilation_Context() {
	st = new SymbolInfoTable();
}
SymbolInfoTable * Compilation_Context::get_symboltable() {
	return st;
}

void Compilation_Context::assign_symbol(Variable *var, SymbolInfo *value) {
	st->assign(var, value);
}

void Compilation_Context::set_symboltable(SymbolInfoTable *_st) {
	st = _st;
}

void Compilation_Context::add_symbol(SymbolInfo *info) {
	st->add(info);
}

SymbolInfo * Compilation_Context::get_symbol(string name) {
	return st->get(name);
}


Compilation_Context::~Compilation_Context() {
}

}
