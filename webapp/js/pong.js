function Paddle(pos, bounds) {
    this.geometry = new THREE.BoxGeometry(2,1,0.08);
    this.material = new THREE.MeshBasicMaterial({color: 0x0000ff, wireframe: true});
    this.cube = new THREE.Mesh(this.geometry, this.material);
    this.cube.position = pos;
    this.bounds = bounds || {
        minX: -3,
        maxX: 3,
        minY: -2,
        maxY: 2
    };
    this.velocity = new THREE.Vector3(0, 0, 0);
}

Paddle.prototype.position = function() {
    return this.cube.position;
};

Paddle.prototype.setPosition = function(x, y) {
    if (x < this.bounds.minX)
        this.cube.position.x = this.bounds.minX;
    else if (x > this.bounds.maxX)
        this.cube.position.x = this.bounds.maxX;
    else {
        this.velocity.x = x - this.cube.position.x * 1.5;
        this.cube.position.x = x;
    }
    if (y < this.bounds.minY)
        this.cube.position.y = this.bounds.minY;
    else if (y > this.bounds.maxY)
        this.cube.position.y = this.bounds.maxY;
    else {
        this.velocity.y = y - this.cube.position.y * 1.5;
        this.cube.position.y = y;
    }
};

Paddle.prototype.mesh = function() {
    return this.cube;
};

Paddle.prototype.contains = function(vec) {
    var pos = this.cube.position;
    return vec.x > pos.x - 1.2 && vec.x < pos.x + 1.2
        && vec.y > pos.y - 0.7 && vec.y < pos.y + 0.7;
};

function Puck(pos, vel) {
    this.geometry = new THREE.SphereGeometry(0.5);
    this.material = new THREE.MeshBasicMaterial({color: 0x0000ff, wireframe: true});
    this.sphere = new THREE.Mesh(this.geometry, this.material);
    this.sphere.position = pos;
    this.velocity = vel;
}

Puck.prototype.position = function() {
    return this.sphere.position;
};

Puck.prototype.setPosition = function(pos) {
    this.sphere.position.x = pos.x;
    this.sphere.position.y = pos.y;
    this.sphere.position.z = pos.z;
};

Puck.prototype.update = function() {
    this.setPosition(this.position().add(this.velocity));
}

Puck.prototype.mesh = function() {
    return this.sphere;
};

function Pong(width, height, depth) {
    this.width = width;
    this.height = height;
    this.depth = depth;

    this.paddleA = new Paddle(new THREE.Vector3(0, 0, 0));
    this.paddleB = new Paddle(new THREE.Vector3(0, 0, -depth));

    this.puck = new Puck(new THREE.Vector3(0, 0, -depth / 2),
        new THREE.Vector3(0, 0, 0.1));

    this.boundsGeometry = new THREE.BoxGeometry(width, height, depth);
    this.boundsMaterial = new THREE.MeshBasicMaterial({color: 0x0000ff, wireframe: true});
    this.boundsRect = new THREE.Mesh(this.boundsGeometry, this.boundsMaterial);
    this.boundsRect.position = new THREE.Vector3(0, 0, -depth / 2);

    this.sceneObjs = [this.paddleA.mesh(),
        this.paddleB.mesh(),
        this.puck.mesh(),
        this.boundsRect];
}

Pong.prototype.addObjsToScene = function(scene) {
    this.sceneObjs.forEach(function(obj) {
        scene.add(obj);
    });
};

Pong.prototype.setInputHandlerA = function(handler) {
    handler.move = this.paddleA.setPosition.bind(this.paddleA);
};

Pong.prototype.setInputHandlerB = function(handler) {
    handler.move = function(x, y) {
        this.paddleB.setPosition(-x, y);
    }.bind(this);
};

Pong.prototype.update = function() {
    this.puck.setPosition(this.puck.position().add(this.puck.velocity));

    // Check and handle collisions between the puck and walls
    if (Math.abs(this.puck.position().x) > this.width / 2)
        this.puck.velocity.x = -this.puck.velocity.x;
    if (Math.abs(this.puck.position().y) > this.height / 2)
        this.puck.velocity.y = -this.puck.velocity.y;

    // Check and handle collisions between the puck and paddles
    if (this.puck.position().z > 0) {
        this.handleCollision(this.paddleA);
    }
    if (this.puck.position().z < -10) {
        this.handleCollision(this.paddleB);
    }

    this.paddleA.velocity = new THREE.Vector3(0, 0, 0);
    this.paddleB.velocity = new THREE.Vector3(0, 0, 0);
};

Pong.prototype.handleCollision = function(paddle) {
    if (paddle.contains(this.puck.position())) {
            if (paddle.velocity.x > 0.001)
                this.puck.velocity.x = paddle.velocity.x;
            if (paddle.velocity.y > 0.001)
            this.puck.velocity.y = paddle.velocity.y;
            this.puck.velocity.z = -this.puck.velocity.z;
            playBlip();
        } else {
            this.puck.velocity.x = 0;
            this.puck.velocity.y = 0;
            this.puck.setPosition(new THREE.Vector3(0, 0, -this.depth / 2));
        }
}

function playBlip() {
    document.getElementById('blip').play();
}