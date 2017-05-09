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

#include "loginscreen.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QFormLayout>

#include "passwordloginwidget.h"
#include "fallbackloginwidget.h"
#include "../quaternionconnection.h"

LoginScreen::LoginScreen(QWidget* parent)
    : QWidget(parent)
    , m_connection(nullptr)
{
    serverLinkLabel = new QLabel("<a href='#connect'>Homeserver</a>");
    serverLabel = new QLabel();
    serverLabel->setTextFormat(Qt::PlainText);
    loginTabs = new QTabWidget();
    loginTabs->setTabBarAutoHide(true);

    separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow(serverLinkLabel, serverLabel);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(separator);
    mainLayout->addWidget(loginTabs);

    setLayout(mainLayout);

    connect( serverLinkLabel, &QLabel::linkActivated, [=](const QString& link) {
        emit changeHomeserver();
    } );
}

void LoginScreen::setServerUrl(QUrl serverUrl)
{
    m_serverUrl = serverUrl;
    this->serverLabel->setText(serverUrl.toDisplayString());
}

void LoginScreen::setLoginTypes(QVector<QString> loginTypes)
{
    while(loginTabs->count())
    {
        delete loginTabs->widget(0);
    }

    foreach( QString item, loginTypes ) {
        if(item == "m.login.password") {
            PasswordLoginWidget* widget = new PasswordLoginWidget(m_serverUrl);
            loginTabs->addTab(widget, "&Password");
            connect(widget, &LoginWidget::loggedIn, this, &LoginScreen::onLoggedIn);
        } else {
            FallbackLoginWidget* widget = new FallbackLoginWidget(m_serverUrl);
            loginTabs->addTab(widget, "&Other");
            connect(widget, &LoginWidget::loggedIn, this, &LoginScreen::onLoggedIn);
        }
    }
}

QuaternionConnection* LoginScreen::connection() const
{
    return m_connection;
}

bool LoginScreen::keepLoggedIn() const
{
    return m_keepLogin;
}

void LoginScreen::onLoggedIn()
{
    LoginWidget* widget = qobject_cast<LoginWidget*>(sender());
    setConnection(widget->connection());
    m_keepLogin = widget->keepLoggedIn();
    emit loggedIn();
}

void LoginScreen::setConnection(QuaternionConnection* connection)
{
    if (m_connection != nullptr) {
        m_connection->deleteLater();
    }

    m_connection = connection;
}
