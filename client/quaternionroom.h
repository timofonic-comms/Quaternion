/**************************************************************************
 *                                                                        *
 * Copyright (C) 2016 Felix Rohrbach <kde@fxrh.de>                        *
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

#ifndef QUATERNIONROOM_H
#define QUATERNIONROOM_H

#include "lib/room.h"

class Message;

class QuaternionRoom: public QMatrixClient::Room
{
        Q_OBJECT
    public:
        using Timeline = QList<Message*>;
        using size_type = Timeline::size_type;

        QuaternionRoom(QMatrixClient::Connection* connection, QString roomId);

        /**
         * set/get whether this room is currently show to the user.
         * This is used to mark messages as read.
         */
        void setShown(bool shown);
        bool isShown();

        const Timeline& messages() const;

        bool hasUnreadMessages();

    signals:
        void aboutToInsertMessages(size_type from, size_type to);
        void insertedMessages();
        void unreadMessagesChanged(QuaternionRoom* room);

    protected:
        virtual void doAddNewMessageEvents(const QMatrixClient::Events& events) override;
        virtual void doAddHistoricalMessageEvents(const QMatrixClient::Events& events) override;
        virtual void processEphemeralEvent(QMatrixClient::Event* event) override;

    private slots:
        void countChanged();

    private:
        Timeline m_messages;
        bool m_shown;
        bool m_unreadMessages;

        Message* makeMessage(QMatrixClient::Event* e);
};

#endif // QUATERNIONROOM_H
