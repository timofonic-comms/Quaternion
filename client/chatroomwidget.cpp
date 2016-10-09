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

#include "chatroomwidget.h"

#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtWidgets/QListView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickItem>

#include "lib/room.h"
#include "lib/user.h"
#include "lib/connection.h"
#include "lib/logmessage.h"
#include "lib/jobs/postmessagejob.h"
#include "lib/events/event.h"
#include "lib/events/typingevent.h"
#include "models/messageeventmodel.h"
#include "quaternionroom.h"
#include "imageprovider.h"

ChatRoomWidget::ChatRoomWidget(QWidget* parent)
    : QWidget(parent)
{
    m_messageModel = new MessageEventModel(this);
    m_currentRoom = nullptr;
    m_currentConnection = nullptr;

    //m_messageView = new QListView();
    //m_messageView->setModel(m_messageModel);

    m_quickView = new QQuickView();

    m_imageProvider = new ImageProvider(m_currentConnection);
    m_quickView->engine()->addImageProvider("mtx", m_imageProvider);

    QWidget* container = QWidget::createWindowContainer(m_quickView, this);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QQmlContext* ctxt = m_quickView->rootContext();
    ctxt->setContextProperty("messageModel", m_messageModel);
    ctxt->setContextProperty("debug", QVariant(false));
    m_quickView->setSource(QUrl("qrc:///qml/chat.qml"));
    m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);

    QObject* rootItem = m_quickView->rootObject();
    connect( rootItem, SIGNAL(getPreviousContent()), this, SLOT(getPreviousContent()) );


    m_chatEdit = new QLineEdit();
    connect( m_chatEdit, &QLineEdit::returnPressed, this, &ChatRoomWidget::sendLine );

    m_currentlyTyping = new QLabel();
    m_topicLabel = new QLabel();
    m_topicLabel->setWordWrap(true);
    m_topicLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);


    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_topicLabel);
    layout->addWidget(container);
    layout->addWidget(m_currentlyTyping);
    layout->addWidget(m_chatEdit);
    setLayout(layout);
}

ChatRoomWidget::~ChatRoomWidget()
{
}

void ChatRoomWidget::enableDebug()
{
    QQmlContext* ctxt = m_quickView->rootContext();
    ctxt->setContextProperty("debug", true);
}

void ChatRoomWidget::setRoom(QuaternionRoom* room)
{
    if( m_currentRoom )
    {
        m_currentRoom->disconnect( this );
        m_currentRoom->setShown(false);
    }
    m_currentRoom = room;
    if( m_currentRoom )
    {
        connect( m_currentRoom, &QMatrixClient::Room::typingChanged, this, &ChatRoomWidget::typingChanged );
        connect( m_currentRoom, &QMatrixClient::Room::topicChanged, this, &ChatRoomWidget::topicChanged );
        m_currentRoom->setShown(true);
        topicChanged();
        typingChanged();
    } else {
        m_topicLabel->clear();
        m_currentlyTyping->clear();
    }
    m_messageModel->changeRoom( m_currentRoom );
    //m_messageView->scrollToBottom();
    QObject* rootItem = m_quickView->rootObject();
    QMetaObject::invokeMethod(rootItem, "scrollToBottom");
}

void ChatRoomWidget::setConnection(QMatrixClient::Connection* connection)
{
    setRoom(nullptr);
    m_currentConnection = connection;
    m_imageProvider->setConnection(connection);
    m_messageModel->setConnection(connection);
}

void ChatRoomWidget::typingChanged()
{
    QList<QMatrixClient::User*> typing = m_currentRoom->usersTyping();
    if( typing.isEmpty() )
    {
        m_currentlyTyping->clear();
        return;
    }
    QStringList typingNames;
    for( QMatrixClient::User* user: typing )
    {
        typingNames << m_currentRoom->roomMembername(user);
    }
    m_currentlyTyping->setText( QString("<i>Currently typing: %1</i>").arg( typingNames.join(", ") ) );
}

void ChatRoomWidget::topicChanged()
{
    m_topicLabel->setText( m_currentRoom->topic() );
}

void ChatRoomWidget::getPreviousContent()
{
    if (m_currentRoom)
        m_currentRoom->getPreviousContent();
}

void ChatRoomWidget::sendLine()
{
    qDebug() << "sendLine";
    if( !m_currentConnection )
        return;
    QString text = m_chatEdit->displayText();

    // Commands available without current room
    if( text.startsWith("/join") )
    {
        QString roomName = text.section(' ', 1, 1, QString::SectionSkipEmpty);
        if( !roomName.isEmpty() )
            m_currentConnection->joinRoom( roomName );
        else
        {
            qDebug() << "No arguments for /join, going interactive";
            emit joinRoomNeedsInteraction();
        }
    }
    else // Commands available only in the room context
        if (m_currentRoom)
        {
            if( text.startsWith("/leave") )
            {
                m_currentConnection->leaveRoom( m_currentRoom );
            }
            else if( text.startsWith("/me") )
            {
                text.remove(0, 3);
                m_currentConnection->postMessage(m_currentRoom, "m.emote", text);
            } else
                m_currentConnection->postMessage(m_currentRoom, "m.text", text);
        }
    m_chatEdit->setText("");
}
