#ifndef IpMessage_h__
#define IpMessage_h__

#include <QFlags>
#include <qglobal.h>
#include <QObject>

class QByteArray;
class QDataStream;
class QTcpSocket;

class LockItemMessage;
class InsertItemMessage;
class UpdateItemMessage;
class DeleteItemMessage;
class MoveItemMessage;

enum class NotificationType { None = 0, Insert = 1, Update = 2, Delete = 4, Move = 8 };
Q_DECLARE_FLAGS(NotificationTypes, NotificationType)
Q_DECLARE_OPERATORS_FOR_FLAGS(NotificationTypes)

class IpMessage : public QObject
{
    Q_OBJECT

public:
    enum MessageType { kNone, kLockItem, kDeleteItem, kUpdateItem, kInsertItem, kMoveItem }; // No enum class because it needs to be converted to int. // #WORK DUPLICATE with NotificationType! Remove this one!!
    using BodySize = qint32;

    IpMessage(MessageType messageType) 
        : m_messageType(messageType) 
    {}
	virtual ~IpMessage() = default;
    virtual QByteArray prepare();
    virtual QString toString();

    static IpMessage* messageFactory(QDataStream& stream);

    virtual void emitIpMessageReady() {}

signals:
    void ipMessageReady(LockItemMessage& message);
    void ipMessageReady(InsertItemMessage& message);
    void ipMessageReady(UpdateItemMessage& message);
    void ipMessageReady(DeleteItemMessage& message);
    void ipMessageReady(MoveItemMessage& message);

protected:
    MessageType m_messageType;
};

class ItemTypeMessage : public IpMessage // #TODO move to CxModel project
{
public:
    enum ItemType { kNone, kMacro, kWorkCollection, kDrilling, kTapping, kCountersink, kCirclePocket, kRectanglePocket }; // No enum class because it needs to be converted to int. // #WORK should not be needed

    ItemTypeMessage(MessageType messageType, ItemType itemType, int itemId, int parentId = -1) 
        : IpMessage(messageType)
        , m_itemType(itemType)
        , m_itemId(itemId)
        , m_parentId(parentId) 
    {}
    ItemTypeMessage(MessageType messageType, QDataStream& stream);

    virtual QString toString() override;
    ItemType itemType() { return m_itemType; }
    int itemId() { return m_itemId; }
    int parentId() { return m_parentId; }

protected:
    virtual QByteArray prepare() override;
	virtual void prepareMore(QDataStream& /*out*/) {}

    ItemType m_itemType;
    int m_itemId;
    int m_parentId;
};

class LockItemMessage : public ItemTypeMessage
{
public:
    enum LockActionType { Lock, NormalUnlock, ForceUnlock };
    LockItemMessage(ItemTypeMessage::ItemType itemType, int itemId, LockActionType lockAction, int lockedUserId, QString lockedSince, QString unlockerUserName);
    LockItemMessage(QDataStream& stream);

    virtual QString toString() override;
	virtual void emitIpMessageReady() override { emit ipMessageReady(*this); }
    LockActionType lockAction() { return m_lockAction; }
    bool isUnlock() { return m_lockAction == NormalUnlock || m_lockAction == ForceUnlock; }
    bool forceUnlock() { return m_lockAction == ForceUnlock; }
    int lockedUserId() { return m_lockedUserId; }
    QString lockedSince() { return m_lockedSince; }
    QString unlockerUserName() { return m_unlockerUserName; }

private:
	virtual void prepareMore(QDataStream& out) override;

    LockActionType m_lockAction;
    int m_lockedUserId;
    QString m_lockedSince;
    QString m_unlockerUserName;
};

class InsertItemMessage : public ItemTypeMessage
{
public:
    InsertItemMessage(ItemTypeMessage::ItemType itemType, int itemId, int parentId = -1)
        : ItemTypeMessage(kInsertItem, itemType, itemId, parentId) 
    {}
    InsertItemMessage(QDataStream& stream);
    virtual void emitIpMessageReady() override { emit ipMessageReady(*this); }
};

class UpdateItemMessage : public ItemTypeMessage
{
public:
    UpdateItemMessage(ItemTypeMessage::ItemType itemType, int itemId, int parentId = -1) 
        : ItemTypeMessage(kUpdateItem, itemType, itemId, parentId) 
    {}
    UpdateItemMessage(QDataStream& stream);
    virtual void emitIpMessageReady() override { emit ipMessageReady(*this); }
};

class DeleteItemMessage : public ItemTypeMessage
{
public:
    DeleteItemMessage(ItemTypeMessage::ItemType itemType, int itemId, int parentId = -1) 
        : ItemTypeMessage(kDeleteItem, itemType, itemId, parentId) 
    {}
    DeleteItemMessage(QDataStream& stream);
    virtual void emitIpMessageReady() override { emit ipMessageReady(*this); }
};

class MoveItemMessage : public ItemTypeMessage
{
public:
	// Base class m_parentId is unused, but set to destinationItemId
	MoveItemMessage(ItemTypeMessage::ItemType itemType, int itemId, int destinationItemId, QString destinationTypeName,
		int sourceParentId, QString sourceTypeName, int destinationRow)
		: ItemTypeMessage(kMoveItem, itemType, itemId, destinationItemId),
		m_destinationId(destinationItemId),
		m_destinationTypeName(destinationTypeName),
		m_sourceId(sourceParentId),
		m_sourceTypeName(sourceTypeName),
		m_destinationRow(destinationRow)
	{}
	MoveItemMessage(QDataStream& stream);

	virtual void emitIpMessageReady() override { emit ipMessageReady(*this); }
	virtual QString toString() override;
	QString destinationTypeName() { return m_destinationTypeName; }
	int destinationId() { return m_destinationId; }
	QString sourceTypeName() { return m_sourceTypeName; }
	int sourceId() { return m_sourceId; }
	int destinatoinRow() { return m_destinationRow; }

private:
	virtual void prepareMore(QDataStream& out) override;

	int m_destinationId;
	QString m_destinationTypeName;
	int m_sourceId;
	QString m_sourceTypeName;
	int m_destinationRow;
};

#endif // IpMessage_h__
