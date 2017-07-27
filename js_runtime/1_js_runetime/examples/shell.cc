
#include <include/v8.h>
#include <include/libplatform/libplatform.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <random>

using namespace v8;
using namespace std;

/**
 * This sample program shows how to implement a simple javascript shell
 * based on V8.  This includes initializing V8 with command line options,
 * creating global functions, compiling and executing strings.
 *
 * For a more sophisticated shell, consider using the debug shell D8.
 */

Local<Context> CreateShellContext(Isolate* isolate);
void RunREPL(Local<Context> context);
string EvaluateCode(Local<Context> context, string codeToBeEvaluated);
void Quit(const FunctionCallbackInfo<Value>& args);
void ReportException(Isolate* isolate, TryCatch* handler);
string readUserInput();
void print(string str);
Isolate* initV8(int argc, char* argv[]);

int main(int argc, char* argv[]) {
  fprintf(stderr, "My JavaScript Container, a simple repl shell (V8 version %s)\n", V8::GetVersion());
  /******************************************************/
  /* Init V8: Location, ExternalData, Platform, Isolate */
  /******************************************************/
  Isolate* isolate = initV8(argc, argv); // iframes, tab, web workers
  /*******************************************************/
  /* Start V8 Scope                                      */
  /*******************************************************/
  // Enter the created isolate scope
  Isolate::Scope isolate_scope(isolate);
  HandleScope handle_scope(isolate);
  // Create a local context and enter it
  Local<Context> context = CreateShellContext(isolate);
  Context::Scope context_scope(context);
  RunREPL(context);
  /*******************************************************/
  /* Clean V8: Location, ExternalData, Platform, Isolate */
  /*******************************************************/
  isolate->Dispose();
  V8::Dispose();
  V8::ShutdownPlatform();
  // delete platform;
  // delete create_params.array_buffer_allocator;
  return 0;
}

Isolate* initV8(int argc, char* argv[]) {
  V8::InitializeICUDefaultLocation(argv[0]);
  V8::InitializeExternalStartupData(argv[0]);
  V8::InitializePlatform(platform::CreateDefaultPlatform());
  V8::Initialize();
  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
      ArrayBuffer::Allocator::NewDefaultAllocator();
  return Isolate::New(create_params);
}

// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

// Creates a new execution environment containing the built-in
// functions.
Local<Context> CreateShellContext(Isolate* isolate) {
  // Create a template for the global object.
  Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
  // Bind the 'quit' function
  global->Set(String::NewFromUtf8(
                  isolate, "quit", NewStringType::kNormal).ToLocalChecked(),
              FunctionTemplate::New(isolate, Quit));
  return Context::New(isolate, NULL, global);
}

// The callback that is invoked by v8 whenever the JavaScript 'quit'
// function is called.  Quits.
void Quit(const FunctionCallbackInfo<Value>& args) {
  // If not arguments are given args[0] will yield undefined which
  // converts to the integer value 0.
  int exit_code =
      args[0]->Int32Value(args.GetIsolate()->GetCurrentContext()).FromMaybe(0);
  fflush(stdout);
  fflush(stderr);
  exit(exit_code);
}

// The read-eval-execute loop of the shell.
void RunREPL(Local<Context> context) {
  // Enter the execution environment before evaluating any code.
  while (true) {
    string codeToBeEvaluated = readUserInput();
    if(codeToBeEvaluated == "\n") continue;
    string codeEvaluated = EvaluateCode(context, codeToBeEvaluated);
    print(codeEvaluated);
  }
}

void print(string str){
  printf("%s\n", str.c_str());
}

string readUserInput() {
  static const int kBufferSize = 256;
  char buffer[kBufferSize];
  fprintf(stderr, "> ");
  const char* input = fgets(buffer, kBufferSize, stdin);
  string userInputString(input);
  return userInputString;
}

// Executes a string within the current v8 context.
string EvaluateCode(Local<Context> context, string codeToBeEvaluated) {
  Local<Script> script;
  Local<Value> result;
  Local<String> sourceCode(String::NewFromUtf8(context->GetIsolate(), codeToBeEvaluated.c_str(), NewStringType::kNormal).ToLocalChecked());
  Local<String> name(String::NewFromUtf8(context->GetIsolate(), "(shell)", NewStringType::kNormal).ToLocalChecked());
  ScriptOrigin origin(name);
  TryCatch try_catch(context->GetIsolate());
  string undefined("");

  if (!Script::Compile(context, sourceCode, &origin).ToLocal(&script)) {
    // Print errors that happened during compilation.
    ReportException(context->GetIsolate(), &try_catch);
    return undefined;
  }

  if (!script->Run(context).ToLocal(&result)) {
    assert(try_catch.HasCaught());
    // Print errors that happened during execution.
    ReportException(context->GetIsolate(), &try_catch);
    return undefined;
  }

  if (!result->IsUndefined()) {
    // If all went well and the result wasn't undefined then print
    // the returned value.
    String::Utf8Value strOf(result);
    string str(*strOf);
    return str;
  }

  return undefined;
}

void ReportException(Isolate* isolate, TryCatch* try_catch) {
  HandleScope handle_scope(isolate);
  String::Utf8Value exception(try_catch->Exception());
  const char* exception_string = ToCString(exception);
  Local<Message> message = try_catch->Message();
  if (message.IsEmpty()) {
    // V8 didn't provide any extra information about this error; just
    // print the exception.
    fprintf(stderr, "%s\n", exception_string);
  } else {
    // Print (filename):(line number): (message).
    String::Utf8Value filename(message->GetScriptOrigin().ResourceName());
    Local<Context> context(isolate->GetCurrentContext());
    const char* filename_string = ToCString(filename);
    int linenum = message->GetLineNumber(context).FromJust();
    fprintf(stderr, "%s:%i: %s\n", filename_string, linenum, exception_string);
    // Print line of source code.
    String::Utf8Value sourceline(
        message->GetSourceLine(context).ToLocalChecked());
    const char* sourceline_string = ToCString(sourceline);
    fprintf(stderr, "%s\n", sourceline_string);
    // Print wavy underline (GetUnderline is deprecated).
    int start = message->GetStartColumn(context).FromJust();
    for (int i = 0; i < start; i++) {
      fprintf(stderr, " ");
    }
    int end = message->GetEndColumn(context).FromJust();
    for (int i = start; i < end; i++) {
      fprintf(stderr, "^");
    }
    fprintf(stderr, "\n");
    Local<Value> stack_trace_string;
    if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
        stack_trace_string->IsString() &&
        Local<String>::Cast(stack_trace_string)->Length() > 0) {
      String::Utf8Value stack_trace(stack_trace_string);
      const char* stack_trace_string = ToCString(stack_trace);
      fprintf(stderr, "%s\n", stack_trace_string);
    }
  }
}
