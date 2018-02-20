
Hi everyone, and thank you very much for attending this talk

Slide 1
=======

Have you ever heard about Runes ? Do you know what it is ? ... hmm, it's like a ancient language and knowledge, pretty hard to understand, to acquire, that let you make wonders, miracles. It's like magic !

Have you ever heard about The Javascript Runtime ? Do you know what it is ? ... hmm, it's something like, hmm, like an ancient language (The event loop, the worker threads, the rendering engine, the call stack) like an ancient knowledge, pretty hard to understand and to acquire, that let you make wonderful applications. It's like magic !, white or black magic, depending if having a good or a bad day coding Javascript.

But we all are serious engineers who do not believe in magic and things like that, right? so the purpose of this talk is to take you by my side, step by step, little by little in this magical journey where we will go from the JavaScript RuneTime to the JavaScript Runtime!

Slide 2
=======

Ok, so I'm going to split this talk in two different parts, in the first one I will cover the basics about the Javascript runtime, and for the second one I'll be playing with some interesting life demos.

This going to be a very technical talk, but at the same time we'll take it easy so please do not worry if you do not understand something and just try to enjoy it. In case you are already familiar with the javascript runtime, do not watch it.

Slide 3
=======

Ok great, before we start talking about the Javascript runtime, we need to talk a little bit about Javascript itself because Javascript is somehow an special programming language.

Unlike other programming languages such as C, Go, or C#, Javascrpit is a programming language that just can run within a container, and what does it mean? well, it means that javascript is a hosted programming language and it means that it requires and needs an special environment where it can be run. It's like a submarine that can only operate and work properly under the water and if we take it out of the water it just doesn't work. Putting this in more techical words, Javascript is an interpreted programming language, so the container it runs is in charge of reading, understanding, interpreting the Javascirpt code you write and running this for you.

In order to do so, the container is made of two different parts:

One of them has the responsability to parse and translate the Javascript code into machine-code operations using a  just-in-time compilation. This is known as the Javascript Engine or Interpret.

The other part provides the built-in packages, libraries and APIS that are available by default to your code. This is often known as the Javascript Runtime.

You can think that the Javascript runtime is to the Javascript engine what the linker is to the compiler in a traditional compiled language.

Take into account that sometimes when we talk about the Javascript runtime, we do acually refer to the whole container, so when developers talk about the runtime they normally talk about both the engine and the runtime.

So for example we have the Chrome Runtime and de NodeJs Runtime. Both of them are built upon the same JavaScript Engine named V8 but they both are different Javascript runtimes. In Chrome your Javascript Application have access to built-in libraries such as the Window and the DOM APIs, while in Nodejs you have the FileSystem, the Net or the Crypto APIs. They have also libraries in common such as the Console API or the built-in data types.

As you may imagine, having a good understanding of how the container, both the engine and the runtime, works is very important if you want to become a good, competent Javascript software developer

Slide 4
=======

Ok fantastic

do not worry too much if this is not completely clear because this is the only theory we need to know up to this point. and what we are going to do from now till time is over is to work out some life, interactive demos to enforce the theory we have just learn, and the first demo we are going to do is to build our own container from scratch.

Slide 5
=======

The container we are going to build will be a repl. A repl is a program that waits for the user to type something, once the user types something and hits enter key, it will evaluate the user input and will print the results back to us. Then, it will keep iterating this over and over again until we exit the program.

Ok, so let's start by creating a basic repl.

As we said, the container has two parts, the engine and the runtime. We are going to use v8 as the engine because it's a very well know engine and it's open source.
