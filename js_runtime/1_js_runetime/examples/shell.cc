#include <include/v8.h>
#include <include/libplatform/libplatform.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * This sample program shows how to implement a simple javascript container
 * based on v8 that runs a simple REPL.
 */
int main(int argc, char* argv[]) {
  fprintf(stderr, "V8 version %s [sample shell]\n", v8::V8::GetVersion());
  v8::Local<v8::Context> context = create_runtime_context();
  run_repl(context);
  return 0;
}

/**
 * read -> evaluate -> print -> loop (REPL)
 */
void run_repl(v8::Local<v8::Context> runtime_context){
  while(true){
    char* code_to_be_evaluated = read_user_input();
    if (code_to_be_evaluated == NULL) break;
    v8::Local<v8::Value> code_result = evaluate_code(runtime_context, code_to_be_evaluated);
    if (code_result->IsUndefined()) break;
    print(code_result);
  }
}

char* read_user_input(){
  static const int kBufferSize = 256;
  char buffer[kBufferSize];
  fprintf(stderr, "> ");
  char* str = fgets(buffer, kBufferSize, stdin);
  return str;
}

v8::Local<v8::Value> result evaluate_code(v8::Local<v8::Context> runtime_context, char* code_to_be_evaluated) {
  v8::Local<v8::Value> result;
  v8::Local<v8::Script> script;
  v8::Script::Compile(runtime_context, code_to_be_evaluated).ToLocal(&script);
  script->Run(runtime_context).ToLocal(&result);
  return result;
}

void print(v8::Local<v8::Value> result){
  v8::String::Utf8Value str(result);
  printf("%s\n", *str);
}

// Creates a new execution environment containing the built-in functions.
v8::Local<v8::Context> create_runtime_context() {
  // Createa global object as the context
  v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
  global->Set(
      v8::String::NewFromUtf8(isolate, "escupir", v8::NewStringType::kNormal)
          .ToLocalChecked(),
      v8::FunctionTemplate::New(isolate, Escupir));
  global->Set(
      v8::String::NewFromUtf8(isolate, "quit", v8::NewStringType::kNormal)
          .ToLocalChecked(),
      v8::FunctionTemplate::New(isolate, Quit));
  return v8::Context::New(global);
}


// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called.  Prints its arguments on stdout separated by
// spaces and ending with a newline.
void Escupir(const v8::FunctionCallbackInfo<v8::Value>& args) {
  for (int i = 0; i < args.Length(); i++) {
    printf("%s", *args[i]);
  }
  printf("\n");
  fflush(stdout);
}


// The callback that is invoked by v8 whenever the JavaScript 'quit'
// function is called.  Quits.
void Quit(const v8::FunctionCallbackInfo<v8::Value>& args) {
  exit(0);
}
