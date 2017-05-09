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

#pragma once

#include <QtWidgets/QWidget>
#include <QtCore/QUrl>
#include <QtCore/QVector>

class QLabel;
class QTabWidget;
class QFrame;
class QuaternionConnection;

class LoginScreen : public QWidget
{
        Q_OBJECT
    public:
        LoginScreen(QWidget* parent = nullptr);

        QuaternionConnection* connection() const;
        bool keepLoggedIn() const;

    signals:
        void loggedIn();
        void changeHomeserver();

    public slots:
        void setServerUrl(QUrl serverUrl);
        void setLoginTypes(QVector<QString> loginTypes);

    private slots:
        void onLoggedIn();

    private:
        QLabel* serverLinkLabel;
        QLabel* serverLabel;
        QTabWidget* loginTabs;
        QFrame* separator;

        QUrl m_serverUrl;
        QuaternionConnection* m_connection;
        bool m_keepLogin;

        void setConnection(QuaternionConnection* connection);
};
