#include <piga/overlay/OverlayWrapper.hpp>

#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QOpenGLContext>

#include <piga/overlay/OverlayPlugin.hpp>

extern "C" {
#include <X11/Xlib.h>
}

namespace piga
{
namespace overlay
{
OverlayWrapper::OverlayWrapper(int argc, char* argv[])
    : QObject(nullptr)
{ 
    m_qtApp.reset(new QGuiApplication(argc, argv));

    QQmlEngine *engine = new QQmlEngine();
    QQmlComponent *component = new QQmlComponent(engine);


    m_view.reset(new QQuickView(engine, nullptr));
}

OverlayWrapper::~OverlayWrapper()
{

}

int OverlayWrapper::startOverlay()
{
    if(m_started) {
	return 0;
    }
    m_started = true;

    // Set the format
    QSurfaceFormat format;


    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        format.setVersion(3, 2);
        format.setProfile(QSurfaceFormat::CoreProfile);
    }
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setAlphaBufferSize(8);
    format.setSamples(4);
    format.setSwapInterval(1);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

    m_view->setSource(QUrl("qrc:/overlay/qml/Overlay.qml"));
    m_view->setResizeMode(QQuickView::SizeRootObjectToView);
    m_view->setColor("transparent");
    m_view->setDefaultAlphaBuffer(true);
    m_view->setFormat(format);
    m_view->setFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    m_view->setClearBeforeRendering(true);

    m_view->showFullScreen();

    return m_qtApp->exec();
}
bool OverlayWrapper::hasStarted()
{
    return m_started;
}
}
}
