var connect = require('connect'),
    http = require('http'),
    socketio = require('socket.io');
    serialport = require('serialport'),
    argv = process.argv;

if (argv.length < 3) {
    console.log("Usage: node server.js /serial/device (baudrate)");
    process.exit(1);
}

/* Initialize serial connection */
//var ser = new serialport.SerialPort(argv[2],
//        {baudrate: 115200});
        //{baudrate: 1843200});

/* Initialize server */
var app = connect().use(connect.static(__dirname + '/webapp')),
    server = http.createServer(app);
    IO = socketio.listen(server);

server.listen(9001);

function coordsToString(coords) {
    return coords.x + "," + coords.y + "," + coords.z;
}

var last = {
    paddleA: {
        x: 0,
        y: 0,
        z: 0,
    },
    paddleB: {
        x: 0,
        y: 0,
        z: 0
    },
    puck: {
        x: 0,
        y: 0,
        z: 0
    },
};

function stateChanged(curr) {
    var paddleASame = curr.paddleA.x == last.paddleA.x
        && curr.paddleA.y == last.paddleA.y
        && curr.paddleA.z == last.paddleA.z;
    var paddleBSame = curr.paddleB.x == last.paddleB.x
        && curr.paddleB.y == last.paddleB.y
        && curr.paddleB.z == last.paddleB.z;
    var puckSame = curr.puck.x == last.puck.x
        && curr.puck.y == last.puck.y
        && curr.puck.z == last.puck.z;
    return !paddleASame || !paddleBSame || !puckSame;
}

var counter = 0;

/* Socket.io routes */
IO.sockets.on('connection', function (socket) {
    console.log('socket connected');

    socket.on('coords', function(data) {
        if (counter % 12 == 0 && stateChanged(data)) {
            var msg = coordsToString(data.paddleA) + ":"
                + coordsToString(data.paddleB) + ":"
                + coordsToString(data.puck);
            console.log(msg);
            last = data;
            //ser.write(msg);
        }
        counter++;
    });
});
