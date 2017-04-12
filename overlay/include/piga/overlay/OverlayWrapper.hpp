#pragma once

#include <QObject>
#include <QQuickView>
#include <QGuiApplication>

typedef struct _XDisplay Display;
typedef unsigned long Window;

#include <memory>

namespace piga
{
namespace overlay
{
    class OverlayWrapper : public QObject
{
    Q_OBJECT
public:
    OverlayWrapper(int argc, char* argv[]);
    ~OverlayWrapper();

    int startOverlay();

    bool hasStarted();
private:
    std::unique_ptr<QQuickView> m_view;
    std::unique_ptr<QGuiApplication> m_qtApp;

    bool m_started = false;
};
}
}
