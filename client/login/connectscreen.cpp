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

#include "connectscreen.h"

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFormLayout>
#include <QtGui/QShowEvent>

#include "../quaternionconnection.h"
#include "settings.h"

ConnectScreen::ConnectScreen(QWidget* parent)
    : QWidget(parent)
    , m_connection(nullptr)
{
    serverEdit = new QLineEdit("https://matrix.org");
    statusLabel = new QLabel();
    connectButton = new QPushButton("Connect");

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Server", serverEdit);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(connectButton);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);

    connect( connectButton, &QPushButton::clicked, this, &ConnectScreen::fetchLoginTypes );
}

void ConnectScreen::setStatusMessage(const QString& msg)
{
    statusLabel->setText(msg);
}

QUrl ConnectScreen::serverUrl() const
{
    return m_serverUrl;
}

QVector<QString> ConnectScreen::loginTypes() const
{
    return m_loginTypes;
}

void ConnectScreen::fetchLoginTypes()
{
    setStatusMessage("Fetching login types, please wait");
    setDisabled(true);
    QUrl url = QUrl::fromUserInput(serverEdit->text());

    //setConnection(new QuaternionConnection(url));

    // TODO: fetch login types
    m_serverUrl = url;
    m_loginTypes = { "m.login.password", "m.login.cas" };
    emit connected();
    setStatusMessage("");
    setDisabled(false);
}

void ConnectScreen::showEvent(QShowEvent* event)
{
    if(!event->spontaneous()) {
        serverEdit->setFocus();
    }
}

void ConnectScreen::setConnection(QuaternionConnection* connection)
{
    if (m_connection != nullptr) {
        m_connection->deleteLater();
    }

    m_connection = connection;
}
