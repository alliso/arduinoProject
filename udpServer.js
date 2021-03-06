const fs = require('fs');
const dgram = require('dgram');
var keypress = require('keypress');
var path = require('path');

keypress(process.stdin);

var running = false
// listen for the "keypress" event
process.stdin.on('keypress', function (ch, key) {

  if (key.name === "s" && running == true) {
    console.log("Press R to run de server")
    running = false
      server.close()
      server.off
  }else if(key.name === "r" && running == false ){
    running = true
    console.log("Press S to stop de server")
    server = dgram.createSocket('udp4');
    server.on('error', (err) => {
      server.close();
    });
    
      var today = new Date()
      var month = today.getMonth() + 1
      var file = "./csv/" + today.getDate() + "." + month + "." + today.getFullYear() + "_" + today.getHours() +
                "." + today.getMinutes() + "." + today.getSeconds() + ".csv"
      var txtFile = "./txt/" + today.getDate() + "." + month + "." + today.getFullYear() + "_" + today.getHours() +
      "." + today.getMinutes() + "." + today.getSeconds() + ".txt"
      file = file.toString()
      txtFile = txtFile.toString()
    
      
    server.on('message', (msg,rinfo) => {
        let interval = new Date().getTime() - today.getTime()   
        var simpleId = 0
        if(rinfo.address.toString() === "192.168.43.34") simpleId = 1
        msg = msg.toString()
        let d = msg.indexOf("d")
        let inverseD = d - msg.length
        let status = msg.slice(0,d)
        let distance = msg.slice(inverseD + 1)
        msg = interval.toString() + "," + status + "," + distance + "," + simpleId;

        
    
        //console.log("newdata",msg)
     fs.appendFile(file, '\n' + msg, (err) => {  
        if (err) throw err;
    });
    fs.appendFile(txtFile, '\n' + msg, (err) => {  
      if (err) throw err;
    })
        
    });
    
    server.on('listening', () => {
      const address = server.address();
      intervalTime = today.getTime()
      let header = "time,status,distance,id"
      console.log(intervalTime)

      fs.stat(file, function(err, stat) {
        if(err == null) {
             console.log("Writing on the same file")    
        } else {
          fs.appendFile(file, header, (err) => {  
            if (err) throw err;
          });
          fs.appendFile(txtFile, header, (err) => {  
            if (err) throw err;
          })
        }
      })


      console.log(`server listening ${address.address}:${address.port}`);
    });
  
    server.bind(8080);
  }  else if(key.name === "c") {
        try {
          server.close();
          process.stdin.pause();
      } catch(err) {
          process.stdin.pause();
      }

  }

});
 
process.stdin.setRawMode(true);
process.stdin.resume();

console.log("Press R to run the server or C to close")