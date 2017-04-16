#include <QDebug>
#include <piga/overlay/Applications.hpp>

#include <cstdlib>

namespace piga
{
namespace overlay
{
Applications::Applications(QObject *parent)
    : QAbstractListModel(parent)
{

    // Update data.
    updateData();
}
Applications::~Applications()
{

}
    
QHash<int, QByteArray> Applications::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[NameRole] = "name";
    roles[DescriptionRole] = "description";
    roles[AuthorRole] = "author";
    roles[VersionRole] = "version";

    return roles;
}
int Applications::rowCount(const QModelIndex &parent) const
{
    return 1;
}
QVariant Applications::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case NameRole:
	return "Name";
	break;
    default:
	return "No data";
    }
}
void Applications::updateData()
{
}
}
}
