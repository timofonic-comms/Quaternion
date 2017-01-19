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

#pragma once

#include <QtWidgets/QWidget>

namespace QMatrixClient
{
    class Room;
    class Connection;
    class User;
    class Event;
}
class MessageEventModel;
class QuaternionRoom;
class ImageProvider;
class QQuickView;
class QListView;
class QLineEdit;
class QLabel;

class ChatRoomWidget: public QWidget
{
        Q_OBJECT
    public:
        ChatRoomWidget(QWidget* parent = nullptr);
        virtual ~ChatRoomWidget();

        void enableDebug();
        void triggerCompletion();
        void cancelCompletion();
        void lookAtRoom();

    signals:
        void joinRoomNeedsInteraction();
        void showStatusMessage(const QString& message, int timeout);

    public slots:
        void setRoom(QuaternionRoom* room);
        void setConnection(QMatrixClient::Connection* connection);
        void topicChanged();
        void typingChanged();
        void getPreviousContent();

    private slots:
        void sendLine();

    private:
        MessageEventModel* m_messageModel;
        QuaternionRoom* m_currentRoom;
        QMatrixClient::Connection* m_currentConnection;
        bool m_completing;
        QStringList m_completionList;
        int m_completionListPosition;
        int m_completionInsertStart;
        int m_completionLength;
        int m_completionCursorOffset;

        void findCompletionMatches(const QString& pattern);
        void startNewCompletion();

        //QListView* m_messageView;
        QQuickView* m_quickView;
        ImageProvider* m_imageProvider;
        QLineEdit* m_chatEdit;
        QLabel* m_currentlyTyping;
        QLabel* m_topicLabel;
};
