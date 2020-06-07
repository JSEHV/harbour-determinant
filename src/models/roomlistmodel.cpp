#include "roomlistmodel.h"

#include <QDebug>
#include <algorithm>

#include "messagerenderer.h"
#include <util.h>

namespace Det {

using namespace QMatrixClient;
using namespace Det;

RoomListModel::RoomListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int RoomListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return rows();
}

QVariant RoomListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= rows())
        return QVariant();

    Room* room = m_rooms[size_t(index.row())];
    MessageRenderer renderer(room);

    switch (role) {
    case DisplayNameRole:
        return room->displayName();
    case AvatarRole:
        return room->avatarMediaId();
    case UnreadCountRole:
        return room->unreadCount();
    case NotificationsCountRole:
        return room->notificationCount();
    case HighlightsCountRole:
        return room->highlightCount();
    case LastEventRole:
        return renderer.getLastEvent();
    case LastActivityRole:
        return renderer.getLastActivity();
    case RoomRole:
        return QVariant::fromValue(room);
    case RoomIdRole:
        return room->id();
    }

    return QVariant();
}

Connection* RoomListModel::connection() const
{
    return m_connection;
}

void RoomListModel::setConnection(Connection* connection)
{
    if (m_connection == connection)
        return;

    beginResetModel();

    if (m_connection) {
        disconnect(m_connection);

        for (Room* room : m_rooms)
            room->disconnect(this);
        m_rooms.clear();
    }

    m_connection = connection;

    if (m_connection) {
        connect(m_connection, &Connection::loggedOut,
            this, [=] { setConnection(nullptr); });
        connect(m_connection, &Connection::newRoom,
            this, &RoomListModel::onNewRoom);
        connect(m_connection, &Connection::aboutToDeleteRoom,
            this, &RoomListModel::onDeleteRoom);

        qDebug() << connection->roomMap().size() << connection->directChats().size();
        for (Room* room : connection->roomMap())
            addRoom(room);
    }

    endResetModel();

    emit connectionChanged();
}

void RoomListModel::onNewRoom(Room* room)
{
    Q_ASSERT(room != nullptr);
    qDebug() << room->displayName();

    beginInsertRows(QModelIndex(), int(m_rooms.size()), int(m_rooms.size()));
    addRoom(room);
    endInsertRows();
}

void RoomListModel::onDeleteRoom(Room* room)
{
    qDebug() << room->displayName();

    int i = indexOfRoom(room);
    Q_ASSERT(i != rows());
    if (i != rows()) {
        beginRemoveRows(QModelIndex(), i, i);
        m_rooms.erase(m_rooms.cbegin() + i);
        endRemoveRows();
    }
}

void RoomListModel::onRoomChanged(Room* room, const QVector<int>& roles)
{
    qDebug() << room->displayName();

    int i = indexOfRoom(room);
    Q_ASSERT(i != rows());
    if (i != rows()) {
        emit dataChanged(index(i), index(i), roles);
    }
}

void RoomListModel::onRoomMessage(Room* room)
{
    onRoomChanged(room, { LastEventRole, LastActivityRole });
}

int RoomListModel::indexOfRoom(Room* room) const
{
    Q_ASSERT(room != nullptr);

    return std::distance(
        m_rooms.begin(), std::find(m_rooms.begin(), m_rooms.end(), room));
}

void RoomListModel::addRoom(Room* room)
{
    qDebug() << room->displayName();
    Q_ASSERT(room != nullptr);

    if (room) {
        m_rooms.push_back(room);
        connectToRoom(room);
    }
}

void RoomListModel::connectToRoom(Room* room)
{
    qDebug() << room->displayName();
    Q_ASSERT(room != nullptr);

    connect(room, &Room::displaynameChanged,
        this, [=] { onRoomChanged(room, { DisplayNameRole }); });
    //connect(room, &Room::joinStateChanged,
    //        this, [=]{ onRoomChanged(room, {}); });
    connect(room, &Room::avatarChanged,
        this, [=] { onRoomChanged(room, { AvatarRole }); });
    connect(room, &Room::unreadMessagesChanged,
        this, [=] { onRoomChanged(room, { UnreadCountRole }); });
    connect(room, &Room::notificationCountChanged,
        this, [=] { onRoomChanged(room, { NotificationsCountRole }); });
    connect(room, &Room::highlightCountChanged,
        this, [=] { onRoomChanged(room, { HighlightsCountRole }); });
    connect(room, &Room::addedMessages,
        this, [=] { onRoomMessage(room); });
    connect(room, &Room::pendingEventMerged,
        this, [=] { onRoomMessage(room); });
    connect(room, &Room::pendingEventChanged,
        this, [=] { onRoomMessage(room); });
    connect(room, &Room::pendingEventAdded,
        this, [=] { onRoomMessage(room); });
}

QHash<int, QByteArray> RoomListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(DisplayNameRole, "displayName");
    roles.insert(AvatarRole, "avatar");
    roles.insert(UnreadCountRole, "unreadCount");
    roles.insert(NotificationsCountRole, "notificationCount");
    roles.insert(HighlightsCountRole, "highlightCount");
    roles.insert(LastEventRole, "lastEvent");
    roles.insert(LastActivityRole, "lastActivity");
    roles.insert(RoomRole, "room");
    roles.insert(RoomIdRole, "roomId");
    return roles;
}

} // namespace Det