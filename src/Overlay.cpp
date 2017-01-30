#include <overlay/Overlay.hpp>
#include <QDebug>
#include <overlay/Notification.hpp>

namespace piga
{
namespace overlay
{

void Overlay::setWindow(QQuickWindow *window)
{
    m_window = window;
    qDebug() << "Test!";
    
    Notification msg;
    msg.setTitle("Hello World!");
    msg.setMsg("Test Content");
    
    QMetaObject::invokeMethod(window, "displayNotification",
        Q_ARG(QVariant, QVariant::fromValue(&msg))
    );
    
}

}
}
