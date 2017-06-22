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

sleep(5);

console.log(helloWorld());
