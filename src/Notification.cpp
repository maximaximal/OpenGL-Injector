#include <overlay/Notification.hpp>

namespace piga
{
namespace overlay 
{
Notification::Notification()
{

}
Notification::~Notification()
{

}
void Notification::setMsg(const QString &msg)
{
    m_msg = msg;
}
void Notification::setTitle(const QString &title)
{
    m_title = title;
}
QString Notification::msg() const
{
    return m_msg;
}
QString Notification::title() const
{
    return m_title;
}
}
}