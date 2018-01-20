First Training Session on the Javascript Runtime

Title: The Javascript Runetime
Description: This will be an introduction to the Javascript Runtime.
Resources: https://github.com/Tsur/talks/tree/master/js_runtime/1_js_runetime
Program: Javascript Introduction - Javascript Runtime Concepts - Live Demos

Coming next ...

* JS History
* Call Stack
* Heap
* Event Loop
* Render Loop
* Web APIs
* Compiler Optimizations
* Building a Javascript Container

--------------------------------------------------------------------------------
Abstract
--------------------------------------------------------------------------------

Before we start talking about the JavaScript Runtime Environment, we first need to understand what Javascript itself is, so let's cover the basics.

Unlike other programming languages such as C, Go, or C#, Javascript is a programming language that runs within a container, say a program that reads and run the javascript code for you, rather than letting the CPU, GPU, the hardware, to do it for you like in those languages I did mention before. In other words, Javascript is an interpreted programming language, so the container it runs in is able to interpret, to understand, to read the code and therefore to run it.

That container provides two different components:

One of them has the responsability to parse and translate the code into machine-executable operations (JIT compilation).This is known as the Javascript Engine or Interpret.

The other provides the built-in packages, libraries and APIS that are available by default to your code. This is known as the Javascript Runtime.

The runtime is to the engine what the linker is to the compiler in a traditional compiled language.

So for example we have the Chrome Container and de NodeJs Container. Both of them use the same JavaScript Engine named V8 but they both have different Javascript runtime environments. In Chrome your program have access to built-in libraries such as the Window and the DOM APIs, while in Nodejs you have the FileSystem, the Net or the Crypto APIs. Both runtimes include the built-in data types and common libraries such as the Console API.

DEMO, go to browser, go to nodejs
DEMO, build our own container from scratch, compile V8 shell and use it.

from ubuntu bash download depot-tools and follow steps
1) http://dev.chromium.org/developers/how-tos/install-depot-tools
2) https://github.com/v8/v8/wiki/Building-from-Source

Note: if using ubuntu bash and going to step 2 without doing step 1, remember to run
export PATH=`pwd`/depot_tools:"$PATH" && cd v8/v8 && ninja -C out.gn/x64.release
Then run it as  ./out.gn/x64.release/v8_shell

Move to v8/out.gn/x64.release and run ./v8_shell.
The full featured shell is called d8

As we can see, the engine is not related with any of the most common javascript built-in apis developers use every day like window, document, setTimeout or xmlHTTPRequest for making ajax calls. The engine is focused just on the language itself, data types, control structures, loops, prototype, functions, ... and so on

This is a very simple container we did, but well known containers like nodejs, chrome, firefox, internet explore are much more complex. The real value are on all those APIs the container provides us as the DOM, xmlHTTPRequest, fetch, setTimeout. Now implementing those APIs are not so easy, so the picture gets more complicated.

For example, the DOM APIs requires a rendering engine in charge of updating your screen whenever an DOM node is updated, animated, removed or added. In chrome we have blink, in firefox we have gecko, in safari webkit, and so on.

By another and, other APIs as the xmlHTTPRequest, fetch, setTimeout and browser events requires a non-blocking I/O model, and this is quite interesting. At the end, your javascript code will be translate into a list of machine-code instructions that your CPU will run sequentally, one after one another, in order, the problem comes with some of those instructions have to wait for some input, so let me show an example of this.

This behaviour is acceptable in most cases, but when we move to a browser context, it's not!, how then javascript solve this ?, The standard fix for most programming languages are threads, but threading have many caveats and problems like race-conditions, dead-locks, live-locks, resource-starvation, and so on. They way javascript containers solve this is by something you have maybe heard about: The event loop.


So our original container turns into something more complex like this.
So, please, welcome to the Call Stack, the Event Loop, the Rendering Loop and the (Async) Workers Threads (libuv) (https://docs.google.com/spreadsheets/d/1ErClfUEnwiVRUg9DZC5BqUTI7T_T4agpLqUmYR1yfdc/edit#gid=0)

Let's now see how this work by using different examples.

DEMO 1
DEMO 2
DEMO 3

whatever computation, whatever interaction like clicking a button, hovering an element, scrolling, nothing will work until the sleep time is over, such a bad thing ... doesn't it? this is call a blocking operation. We need to do something with this!!, lets convert every possible blocking operation into an asynchronous operation

Now the sleep is send to a worker thread and it returns immediately so the call stack keeps working with next function, but how do I know what to run after the async operation is done, this is where the concept of a callback come into scene, we are going to connect every async operation to a function that will be run once the async operation is done.

It is now transferred the sleep and its callback to the worker thread once it is done, the callback is send to the event loop queue and the event loop moves it to the call stack

This happens for every async operation, you can replace that with event listeners, with file input/ output operations, with network ajax calls, whatever is an async operation is linked to a callback and is manage by a worker thread and the event loop

Finally, I would like to talk about the rendering loop. This has the responsability of repainting, updating, refreshing the screen. It normally repaints the screen 60 times per second, but you need to know something very important. The repainting process lives on the same thread as the call stack so it needs to wait for the call stack to get empty before repainting and this is where things can start getting slow. If for some reason your call stack is huge, massive, the rendering process will have to wait and instead of repainting your screen 60 times per second, it will do it maybe 20 times per second or less and as a consequence you will notice as the UI start getting laggy, like everything going in slow motion. That's the reason.
