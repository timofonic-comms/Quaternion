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

#include "passwordloginwidget.h"

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtGui/QShowEvent>

#include "../quaternionconnection.h"
#include "settings.h"

PasswordLoginWidget::PasswordLoginWidget(QUrl serverUrl, QWidget* parent)
    : LoginWidget(parent)
    , m_serverUrl(serverUrl)
{
    userEdit = new QLineEdit();
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode( QLineEdit::Password );
    saveTokenCheck = new QCheckBox("Stay logged in");
    statusLabel = new QLabel();
    loginButton = new QPushButton("Login");

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("User", userEdit);
    formLayout->addRow("Password", passwordEdit);
    formLayout->addRow(saveTokenCheck);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);

    {
        // Fill defaults
        using namespace QMatrixClient;
        QStringList accounts { SettingsGroup("Accounts").childGroups() };
        if ( !accounts.empty() )
        {
            AccountSettings account { accounts.front() };
            QUrl homeserver = account.homeserver();
            QString username = account.userId();
            if (username.startsWith('@'))
            {
                QString serverpart = ":" + homeserver.host();
                if (homeserver.port() != -1)
                    serverpart += ":" + QString::number(homeserver.port());
                if (username.endsWith(serverpart))
                {
                    // Keep only the local part of the user id
                    username.remove(0, 1).chop(serverpart.size());
                }
            }
            userEdit->setText(username);
            saveTokenCheck->setChecked(account.keepLoggedIn());
        }
        else
        {
            saveTokenCheck->setChecked(false);
        }
    }

    connect( loginButton, &QPushButton::clicked, this, &PasswordLoginWidget::login );
}

bool PasswordLoginWidget::keepLoggedIn() const
{
    return saveTokenCheck->isChecked();
}

void PasswordLoginWidget::login() {
    setStatusMessage("Connecting and logging in, please wait");
    setDisabled(true);
    QString user = userEdit->text();
    QString password = passwordEdit->text();

    setConnection(new QuaternionConnection(m_serverUrl));

    connect( connection(), &QMatrixClient::Connection::connected, this, &LoginWidget::loggedIn );
    connect( connection(), &QMatrixClient::Connection::loginError, this, &PasswordLoginWidget::error );
    connection()->connectToServer(user, password);
}

void PasswordLoginWidget::error(QString error)
{
    setStatusMessage( error );
    setConnection(nullptr);
    setDisabled(false);
}

void PasswordLoginWidget::showEvent(QShowEvent* event)
{
    if(!event->spontaneous()) {
        if (userEdit->text().isEmpty()) {
            userEdit->setFocus();
        } else {
            passwordEdit->setFocus();
        }
    }
}

void PasswordLoginWidget::setStatusMessage(const QString& msg)
{
    statusLabel->setText(msg);
}
