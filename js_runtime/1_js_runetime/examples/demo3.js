function hello(){
  return "Hello";
}

function world(){
  return "World";
}

function helloWorld(){
  return hello() + world();
}

console.log('Printing in 10 second');

setTimeout(function(){
  console.log(helloWorld());
}, 1e4);
