#include "IpMessage.h"

#include <array>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include <type_traits>
#include <QTcpSocket>

#include "LogMessage.h"


IpMessage::BodySize setBodySize(QDataStream& out)
{
    auto device = out.device();
    auto position = device->pos();
    IpMessage::BodySize bodySize = static_cast<IpMessage::BodySize>(position - sizeof(IpMessage::BodySize) - sizeof(IpMessage::MessageType));
    device->seek(0);
    return bodySize;
}

QByteArray IpMessage::prepare()
{
    QString messageContent = "Test base IpMessage";

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);

    out << BodySize() << m_messageType << messageContent;
    out << ::setBodySize(out);

    return block;
}


QString IpMessage::toString()
{
    static std::array names{ "kNone", "kLockItem", "kDeleteItem", "kUpdateItem" };
    assert(m_messageType < names.size() && "Message types names does not match m_messageType.");
    return names[m_messageType];
}

ItemTypeMessage::ItemTypeMessage(MessageType messageType, QDataStream& stream)
    : IpMessage(messageType)
{
    using ItemTypeUnderlyingType = typename std::underlying_type<ItemType>::type;

    ItemTypeUnderlyingType type;
    stream >> type >> m_itemId >> m_parentId;
    m_itemType = static_cast<ItemType>(type);
}

QString ItemTypeMessage::toString()
{
    return QString("Type: %1, Id: %2, ParentId: %3").arg(m_itemType).arg(m_itemId).arg(m_parentId);
}

QByteArray ItemTypeMessage::prepare()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);

    out << BodySize() << m_messageType << m_itemType << m_itemId << m_parentId;
	this->prepareMore(out);
    out << ::setBodySize(out);

    return block;
}

LockItemMessage::LockItemMessage(ItemTypeMessage::ItemType itemType, int itemId, LockActionType lockAction, int lockedUserId, QString lockedSince, QString unlockerUserName)
    : ItemTypeMessage(kLockItem, itemType, itemId),
    m_lockAction(lockAction),
    m_lockedUserId(lockedUserId),
    m_lockedSince(lockedSince),
    m_unlockerUserName(unlockerUserName)
{
}

LockItemMessage::LockItemMessage(QDataStream& stream)
: ItemTypeMessage(kLockItem, stream)
{
    using ItemTypeUnderlyingType = typename std::underlying_type<LockActionType>::type;

    ItemTypeUnderlyingType lockAction;
    stream >> lockAction >> m_lockedUserId >> m_lockedSince >> m_unlockerUserName;
    m_lockAction = static_cast<LockActionType>(lockAction);
}

QString LockItemMessage::toString()
{
    const char* lockActionName[] = { "Lock", "Unlock", "Force Unlock" };
    return ItemTypeMessage::toString() + QString(", Action: %1 by %4, User: %2, Since: %3").arg(lockActionName[m_lockAction]).arg(m_lockedUserId).arg(m_lockedSince, m_unlockerUserName);
}

void LockItemMessage::prepareMore(QDataStream& out)
{
    out << m_lockAction << m_lockedUserId << m_lockedSince << m_unlockerUserName;
}

InsertItemMessage::InsertItemMessage(QDataStream& stream)
    : ItemTypeMessage(kInsertItem, stream)
{
}

UpdateItemMessage::UpdateItemMessage(QDataStream& stream)
    : ItemTypeMessage(kUpdateItem, stream)
{
}

DeleteItemMessage::DeleteItemMessage(QDataStream& stream)
    : ItemTypeMessage(kDeleteItem, stream)
{
}

MoveItemMessage::MoveItemMessage(QDataStream& stream)
	: ItemTypeMessage(kMoveItem, stream)
{
	stream >> m_destinationId >> m_destinationTypeName >> m_sourceId >> m_sourceTypeName >> m_destinationRow;
}

void MoveItemMessage::prepareMore(QDataStream& out)
{
	out << m_destinationId << m_destinationTypeName << m_sourceId << m_sourceTypeName << m_destinationRow;
}

QString MoveItemMessage::toString()
{
	return QString("Type: %1, Id: %2, Destination Type: %3 Id: %5 Row: %7, Source Type: %4 Id: %6, ").
		arg(m_itemType).arg(m_itemId).arg(m_destinationTypeName, m_sourceTypeName).arg(m_destinationId).arg(m_sourceId).arg(m_destinationRow);
}


// static
IpMessage* IpMessage::messageFactory(QDataStream& stream)
{
    using MessageTypeUnderlyingType = typename std::underlying_type<MessageType>::type;

    MessageTypeUnderlyingType messageType;
    stream >> messageType;
    switch (messageType)
    {
    case IpMessage::kLockItem:
    {
        LogMessage::send("IpMessage::messageFactory: kLockItem");
        return new LockItemMessage(stream);
    }
    case IpMessage::kDeleteItem:
    {
        LogMessage::send("IpMessage::messageFactory: kDeleteItem");
        return new DeleteItemMessage(stream);
    }
    case IpMessage::kUpdateItem:
    {
        LogMessage::send("IpMessage::messageFactory: kUpdateItem");
        return new UpdateItemMessage(stream);
    }
    case IpMessage::kInsertItem:
    {
        LogMessage::send("IpMessage::messageFactory: kInsertItem");
        return new InsertItemMessage(stream);
    }
	case IpMessage::kMoveItem:
	{
		LogMessage::send("IpMessage::messageFactory: kMoveItem");
		return new MoveItemMessage(stream);
	}
    default:
        LogMessage::send(QString("IpMessage::messageFactory: unknown message type (%1)").arg(messageType));
        return nullptr;
    }
}

#if 0
bool IpMessage::isMessageComplete(QDataStream& stream, uint bytesAvailable, quint32& messageBodySize)
{
    static const uint kMessageBodySizeSize = static_cast<uint>(sizeof(quint32));
    static const uint kMessageTypeSize = static_cast<uint>(sizeof(quint16));

    uint headerSizeStillNeeded = (-1 == messageBodySize) ? kMessageBodySizeSize + kMessageTypeSize : kMessageTypeSize;
    if (bytesAvailable < headerSizeStillNeeded)
        return false;

    if (-1 == messageBodySize)
        stream >> messageBodySize;

    headerSizeStillNeeded = (-1 == messageBodySize) ? kMessageBodySizeSize + kMessageTypeSize : kMessageTypeSize;

    LogMessage::send(QString("NotificationConnectionHandler::isMessageComplete: bytesAvailable:%1, still need:%2: %3+%4+%5").
        arg(m_clientConnection->bytesAvailable()).
        arg(headerSizeStillNeeded + messageBodySize).
        arg((-1 == messageBodySize) ? kMessageBodySizeSize : 0).
        arg(kMessageTypeSize).
        arg(messageBodySize));

    if (bytesAvailable < headerSizeStillNeeded + messageBodySize)
        return false;

    return true;
}
#endif

