/**************************************************************************
 *                                                                        *
 * Copyright (C) 2016 Shell Turner <cam.turn@gmail.com>                   *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/

#include "fallbackloginwidget.h"

#include <QtWebChannel/QWebChannel>
#include <QtWebEngineWidgets/QWebEngineProfile>
#include <QtWebEngineWidgets/QWebEngineScript>
#include <QtWebEngineWidgets/QWebEngineScriptCollection>
#include <QtWebEngineWidgets/QWebEnginePage>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWidgets/QVBoxLayout>
#include <QtGui/QShowEvent>

#include <QtWidgets/QLabel>
#include <QtCore/QDebug>

#include "../quaternionconnection.h"
#include "settings.h"

class LoginCallback : public QObject
{
    Q_OBJECT
    public:
        LoginCallback(QObject* parent = nullptr) : QObject(parent) {};

    public slots:
        void login(QString userId, QString accessToken)
        {
            emit onLogin(userId, accessToken);
        }

    signals:
        void onLogin(QString& userId, QString& accessToken);
};

#include "fallbackloginwidget.moc"

FallbackLoginWidget::FallbackLoginWidget(QUrl serverUrl, QWidget* parent)
    : LoginWidget(parent),
      serverUrl(serverUrl)
{
    LoginCallback* loginCallback = new LoginCallback();
    connect( loginCallback, &LoginCallback::onLogin, this, &FallbackLoginWidget::login );

    QWebChannel* channel = new QWebChannel();
    channel->registerObject("callback", loginCallback);

    QWebEngineProfile* profile = new QWebEngineProfile();
    QFile qwebchannelScriptFile(":/qtwebchannel/qwebchannel.js");
    QFile fallbackloginScriptFile(":/fallbacklogin.js");
    if(qwebchannelScriptFile.open(QIODevice::ReadOnly) &&
       fallbackloginScriptFile.open(QIODevice::ReadOnly)) {
        QWebEngineScript script;
        script.setSourceCode(qwebchannelScriptFile.readAll()+fallbackloginScriptFile.readAll());
        script.setName("qwebchannel.js");
        script.setWorldId(QWebEngineScript::MainWorld);
        script.setInjectionPoint(QWebEngineScript::DocumentReady);
        script.setRunsOnSubFrames(false);
        profile->scripts()->insert(script);
    }

    QWebEnginePage* page = new QWebEnginePage(profile);
    serverUrl.setPath(serverUrl.path()+"/_matrix/static/client/login/");
    page->setUrl(serverUrl);
    page->setWebChannel(channel);

    webview = new QWebEngineView();
    webview->setPage(page);

    QVBoxLayout* layout = new QVBoxLayout();
    //layout->addWidget(new QLabel("hi"));
    layout->addWidget(webview);
    setLayout(layout);

    connect(webview, &QWebEngineView::renderProcessTerminated, [=](QWebEnginePage::RenderProcessTerminationStatus terminationStatus, int exitCode) {
        qDebug() << terminationStatus;
        qDebug() << exitCode;
    });

    webview->resize(1024, 750);
    //webview->setUrl(QUrl("http://www.example.com"));
    webview->show();
}

bool FallbackLoginWidget::keepLoggedIn() const
{
    return false;
}

void FallbackLoginWidget::login(QString& userId, QString& accessToken)
{
    setConnection(new QuaternionConnection(QUrl(this->serverUrl)));
    connect( connection(), &QMatrixClient::Connection::connected, this, &LoginWidget::loggedIn );
    connection()->connectWithToken(userId, accessToken);
}
