if(window.matrixLogin) {
    new QWebChannel(qt.webChannelTransport, function(channel) {
        window.matrixLogin.onLogin = function(response) {
            channel.objects.callback.login(response.user_id, response.access_token);
        };
    });
}
