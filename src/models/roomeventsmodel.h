/*
 * Copyright 2020 Richard Liebscher <richard.liebscher@gmail.com>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ROOMMODEL_H
#define ROOMMODEL_H

#include <QAbstractListModel>

#include <events/roomevent.h>
#include <room.h>

namespace Quotient {
class RoomMessageEvent;
class RoomMemberEvent;
class RoomCreateEvent;
} // namespace Quotient

namespace Det {

class RoomEventsModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(Quotient::Room* room READ room WRITE setRoom NOTIFY roomChanged)
    Q_PROPERTY(bool fetching READ fetching NOTIFY fetchingChanged)
    Q_PROPERTY(QString fetchingError READ fetchingError NOTIFY fetchingErrorChanged)

    enum {
        DisplayRole = Qt::UserRole,
        AuthorRole,
        AuthorDisplayNameRole,
        EventIdRole,
        MatrixTypeRole,
        EventTypeRole,
        EventStatusRole,
        AnnotationRole,
        DateTimeRole,
        DateRole,
        TimeRole,
        EditedRole,
        HiddenRole,
        ContentTypeRole,
        ContentJsonRole,
        ShowAuthorRole,
        ReadMarkerRole,
        FileTransferInfoRole,
        PlainTextRole
    };

public:
    explicit RoomEventsModel(QObject* parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setRoom(Quotient::Room* room);
    Quotient::Room* room() const;
    inline int pendingEventCount() const {
        return int(m_room->pendingEvents().size());
    }

    bool fetching() const;
    QString fetchingError() const { return m_fetchingError; }

signals:
    void roomChanged();
    void fetchingChanged();
    void fetchingErrorChanged();

private:
    Quotient::Room* m_room = nullptr;

    QString m_fetchingError;
    bool m_movingEvents = false;

    void onBeginInsertMessages(Quotient::RoomEventsRange events);
    void onBeginInsertOldMessages(Quotient::RoomEventsRange events);
    void onBeginSyncMessage(int i);
    void onEndSyncMessage();
    void onReplacedMessage(const Quotient::RoomEvent* newEvent,
        const Quotient::RoomEvent* oldEvent);

    void updateRow(int row, const QVector<int>& roles);
    void updateRow(int row);
    void updateEvent(const QString& eventId, const QVector<int>& roles);
    void updateEvent(const QString& eventId);
    int findEvent(const QString& eventId);
};

} // namespace Det

#endif // ROOMMODEL_H
