#ifndef OVERLAY_HPP
#define OVERLAY_HPP

#include <QObject>
#include <QQuickWindow>

namespace piga
{
namespace overlay
{
class Overlay : public QObject
{
    Q_OBJECT
    
public:
    Q_INVOKABLE void setWindow(QQuickWindow *window);
private:
    QQuickWindow *m_window = nullptr;

};
}
}

#endif // OVERLAY_HPP
