#pragma once

#include <QObject>
#include <QString>

namespace piga
{
namespace overlay 
{
class Notification : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString msg READ msg WRITE setMsg)
public:
    Notification();
    virtual ~Notification();
    
    void setTitle(const QString &title);
    void setMsg(const QString &msg);
    QString title() const;
    QString msg() const;
private:
    QString m_title;
    QString m_msg;
};
}
}