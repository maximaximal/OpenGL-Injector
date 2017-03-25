#include <QSqlDatabase>
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
    QSqlDatabase db = QSqlDatabase::addDatabase("SQLITE");

    QString databaseName = std::getenv("PIGA_DAEMON_APPDB_PATH");
    db.setDatabaseName(databaseName);

    qDebug() << "The appdb name is" << databaseName;

    bool ok = db.open();

    if(ok)
	qDebug() << "Successfully opened the database " << databaseName;
    else
	qDebug() << "Successfully opened the database " << databaseName;

    // Prepare queries.
    m_selectApplications.setForwardOnly(true);
    m_selectApplications.prepare("SELECT name,description,author,version FROM `applications`");	

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
    QSqlDatabase db = QSqlDatabase::database();
    m_selectApplications.exec();

    while(m_selectApplications.next()) {
	m_apps.append({
		m_selectApplications.value(0).toString(), // Name
		m_selectApplications.value(1).toString(), // Description
		m_selectApplications.value(2).toString(), // Author
		m_selectApplications.value(3).toString(), // Version
	    });
    }
}
}
}
