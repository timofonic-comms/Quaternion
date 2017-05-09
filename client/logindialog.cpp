/**************************************************************************
 *                                                                        *
 * Copyright (C) 2015 Felix Rohrbach <kde@fxrh.de>                        *
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

#include "logindialog.h"

#include <QtWidgets/QStackedLayout>

#include "login/connectscreen.h"
#include "login/loginscreen.h"
#include "quaternionconnection.h"
#include "settings.h"

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent)
{
    connectScreen = new ConnectScreen();
    loginScreen = new LoginScreen();

    QStackedLayout* layout = new QStackedLayout();
    layout->addWidget(connectScreen);
    layout->addWidget(loginScreen);

    setLayout(layout);

    connect( connectScreen, &ConnectScreen::connected, this, &LoginDialog::connected );
    connect( loginScreen, &LoginScreen::loggedIn, this, &QDialog::accept );
    connect( loginScreen, &LoginScreen::changeHomeserver, this, &LoginDialog::changeHomeserver );
}

void LoginDialog::setStatusMessage(const QString& msg)
{
    connectScreen->setStatusMessage(msg);
}

QuaternionConnection* LoginDialog::connection() const
{
    return loginScreen->connection();
}

bool LoginDialog::keepLoggedIn() const
{
    return loginScreen->keepLoggedIn();
}

void LoginDialog::connected()
{
    loginScreen->setServerUrl(connectScreen->serverUrl());
    loginScreen->setLoginTypes(connectScreen->loginTypes());
    qobject_cast<QStackedLayout*>(layout())->setCurrentWidget(loginScreen);
}

void LoginDialog::changeHomeserver()
{
    qobject_cast<QStackedLayout*>(layout())->setCurrentWidget(connectScreen);
}
