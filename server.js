var connect = require('connect'),
    http = require('http'),
    socketio = require('socket.io');
    serialport = require('serialport'),
    argv = process.argv;

if (argv.length < 4) {
    console.log("Usage: node server.js /serial/device baudrate");
    process.exit(1);
}

/* Initialize serial connection */
var ser = new serialport.SerialPort(argv[2], {baudrate: parseInt(argv[3])});

/* Initialize server */
var app = connect().use(connect.static(__dirname + '/webapp')),
    server = http.createServer(app);
    IO = socketio.listen(server);

server.listen(9001);

function coordsToString(coords) {
    return coords.x + "," + coords.y + "," + coords.z;
}

/* Socket.io routes */
IO.sockets.on('connection', function (socket) {
    console.log('socket connected');

    socket.on('coords', function(data) {
        ser.write(coordsToString(data.paddleA) + ":"
            + coordsToString(data.paddleB) + ":"
            + coordsToString(data.puck));
    });
});