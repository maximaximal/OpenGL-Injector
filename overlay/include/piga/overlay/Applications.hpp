#pragma once

#include <memory>
#include <QAbstractListModel>

namespace piga
{
namespace overlay
{
class Applications : public QAbstractListModel
{
    Q_OBJECT

public:
    struct Application {
	QString name;
	QString description;
	QString author;
	QString version;
    };
    
    enum ApplicationRoles {
	NameRole = Qt::UserRole + 1,
	DescriptionRole = Qt::UserRole + 2,
	AuthorRole = Qt::UserRole + 3,
	VersionRole = Qt::UserRole + 4
    };

    Applications(QObject *parent = 0);
    virtual ~Applications();

    virtual QHash<int, QByteArray> roleNames() const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void updateData();

private:
    QList<Application> m_apps;
};
}
}
