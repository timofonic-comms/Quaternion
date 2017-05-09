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

class QuaternionConnection;

class LoginWidget : public QWidget {
        Q_OBJECT
    public:
        QuaternionConnection* connection() const;
        virtual bool keepLoggedIn() const = 0;

    signals:
        void loggedIn();

    protected:
        LoginWidget(QWidget* parent = nullptr);
        void setConnection(QuaternionConnection* connection);

    private:
        QuaternionConnection* m_connection;
};
