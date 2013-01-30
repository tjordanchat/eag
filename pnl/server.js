   var 
        port        = process.env.PORT || 8005,
        io              = require('socket.io'),
        express         = require('express'),
        UUID            = require('node-uuid'),
        verbose         = false,
        app             = express.createServer();
 
    app.listen( port );
    console.log('\t :: Express :: Listening on port ' + port );
    app.get( '/', function( req, res ){ 
        res.sendfile( __dirname + '/client.html' );
    });
 
    app.get( '/*' , function( req, res, next ) {
        var file = req.params[0]; 
        if(verbose) console.log('\t :: Express :: file requested : ' + file);
        res.sendfile( __dirname + '/' + file );
 
    }); 

    var sio = io.listen(app);
    sio.configure(function (){
        sio.set('log level', 0);
        sio.set('authorization', function (handshakeData, callback) {
          callback(null, true);
        });
    });
 
    sio.sockets.on('connection', function (client) {
        client.userid = UUID();
        client.emit('onconnected', { id: client.userid } );
        console.log('\t socket.io:: player ' + client.userid + ' connected');
        client.on('disconnect', function () {
            console.log('\t socket.io:: client disconnected ' + client.userid );
        });
    }); 
