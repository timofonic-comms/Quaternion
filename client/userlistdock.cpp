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

#include "userlistdock.h"

#include <QtWidgets/QTableView>
#include <QtWidgets/QHeaderView>

#include "lib/connection.h"
#include "lib/room.h"
#include "models/userlistmodel.h"

UserListDock::UserListDock(QWidget* parent)
    : QDockWidget("Users", parent)
{
    setObjectName("UsersDock");
    m_view = new QTableView();
    m_view->setShowGrid(false);
    m_view->horizontalHeader()->setStretchLastSection(true);
    m_view->horizontalHeader()->setVisible(false);
    m_view->verticalHeader()->setVisible(false);
    setWidget(m_view);

    m_model = new UserListModel();
    m_view->setModel(m_model);
}

UserListDock::~UserListDock()
{
}

void UserListDock::setConnection(QMatrixClient::Connection* connection)
{
    m_model->setConnection(connection);
}

void UserListDock::setRoom(QMatrixClient::Room* room)
{
    m_model->setRoom(room);
}
