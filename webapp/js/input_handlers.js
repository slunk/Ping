function MouseInputHandler(move) {
    this.mousePos = null;
    window.onmousemove = this.handleMouseMove.bind(this);
    this.move = move || function(x, y) { };
}

MouseInputHandler.prototype.handleMouseMove = function(event) {
    event = event || window.event; // IE-ism
    this.mousePos = mouseToThree({
        x: event.clientX,
        y: event.clientY
    });
    this.move(this.mousePos.x, this.mousePos.y);
};

function LeapInputHandler(move) {
    this.handPos = null;
    Leap.loop({}, this.handleInput.bind(this));
    this.move = move || function(x, y) { };
}

LeapInputHandler.prototype.handleInput = function(frame) {
    if (frame.hands.length > 0) {
        var hand = frame.hands[0];
        var pos = hand.palmPosition;
        this.handPos = {
            x: pos[0],
            y: pos[1],
            z: pos[2]
        };
        this.move(this.handPos.x / 20, this.handPos.y / 20 - 10);
    }
};

/* three.js wizardry courtesy of West Langley on Stack Overflow
 * http://stackoverflow.com/questions/13055214/mouse-canvas-x-y-to-three-js-world-x-y-z
 */
function mouseToThree(mousePos) {
    var projector = new THREE.Projector();
    var vector = new THREE.Vector3(
        (mousePos.x / window.innerWidth) * 2 - 1,
        - (mousePos.y / window.innerHeight) * 2 + 1,
        0.5);

    projector.unprojectVector(vector, camera);

    var dir = vector.sub(camera.position).normalize();

    var distance = -camera.position.z / dir.z;

    var pos = camera.position.clone().add(dir.multiplyScalar(distance));
    return pos;
}