var fs = require('fs');
const express = require('express')
const app = express()
//const https = require('https').createServer(app)

// Load html
app.use('/', express.static(__dirname + '/public/'))

var options = {
  key: fs.readFileSync(__dirname + '/ssl/server-key.pem'),
  ca: [fs.readFileSync(__dirname + '/ssl/cert.pem')],
  cert: fs.readFileSync(__dirname + '/ssl/server-cert.pem')
};

const https = require('https').createServer(options, app)
/*
https.createServer(options, app).listen(1025, function() {
  console.log('https server listening on port 1025');
});
*/

const io = require('socket.io')(https, {
  cors: true
})


// Using socket.io to handle connection
io.on('connection', (socket) => {
  // Join room
  socket.on('join', (room) => {
    socket.join(room)
    socket.to(room).emit('ready', 'ready to communicate')
  })

  // Forward Offer
  socket.on('offer', (room, desc) => {
    socket.to(room).emit('offer', desc)
  })

  // Forward Answer
  socket.on('answer', (room, desc) => {
    socket.to(room).emit('answer', desc)
  })

  // Exchange ice candidate
  socket.on('ice_candidate', (room, data) => {
    socket.to(room).emit('ice_candidate', data)
  })

})

https.listen(1025, () => {
  console.log(`Server running in 1025`)
})
