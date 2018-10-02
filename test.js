var keypress = require('keypress');
 
// make `process.stdin` begin emitting "keypress" events
keypress(process.stdin);
 
// listen for the "keypress" event
process.stdin.on('keypress', function (ch, key) {
  console.log('got "keypress"', key);
  if (key.name === "a") {
    process.stdin.pause();
  }
});
 
process.stdin.setRawMode(true);
process.stdin.resume();