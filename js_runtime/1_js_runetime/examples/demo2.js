function hello(){
  return "Hello";
}

function world(){
  return "World";
}

function helloWorld(){
  return hello() + world();
}

console.log('Printing in X seconds');

setTimeout(5);

console.log(helloWorld());
