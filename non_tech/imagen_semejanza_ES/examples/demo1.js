//function main(){

  function hello(){
    debugger;
    return "Hello";
  }

  function world(){
    debugger;
    return "World";
  }

  function helloWorld(){
    debugger;
    return hello() + world();
  }

  const str = helloWorld();
  debugger;
  console.log(str);
  debugger;

//}
