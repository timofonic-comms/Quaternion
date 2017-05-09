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
#include <QtCore/QVector>
#include <QtCore/QUrl>

class QLineEdit;
class QPushButton;
class QLabel;
class QuaternionConnection;

class ConnectScreen : public QWidget
{
        Q_OBJECT
    public:
        ConnectScreen(QWidget* parent = nullptr);

        void setStatusMessage(const QString& msg);
        QUrl serverUrl() const;
        QVector<QString> loginTypes() const;

    signals:
        void connected();

    private slots:
        void fetchLoginTypes();

    protected:
        virtual void showEvent(QShowEvent* event) override;

    private:
        QLineEdit* serverEdit;
        QPushButton* connectButton;
        QLabel* statusLabel;

        QuaternionConnection* m_connection;
        QUrl m_serverUrl;
        QVector<QString> m_loginTypes;

        void setConnection(QuaternionConnection* connection);
};
