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

sleep(5, function(){
  console.log(helloWorld());
});
