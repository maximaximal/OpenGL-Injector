#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QDebug>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QOpenGLContext>
#include <Qt3DQuickExtras/qt3dquickwindow.h>
#include <iostream>

#include <QCommandLineParser>

#include <overlay/Overlay.hpp>
#include <overlay/Notification.hpp>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QCoreApplication::setApplicationName("Pigaco Overlay");
    QCoreApplication::setApplicationVersion("0.1");
    
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption ressourcesDirectory(QStringList() << "r" << "ressources-dir",
					   QCoreApplication::translate("main", "Set the ressource directory to use when loading files."),
					   QCoreApplication::translate("main", "ressource-dir"));
    parser.addOption(ressourcesDirectory);

    parser.process(app);

    QString ressourceDir = "./";
    if(parser.isSet(ressourcesDirectory))
	ressourceDir = parser.value(ressourcesDirectory);

    piga::overlay::Overlay overlay;

    engine.addImportPath(ressourceDir + "/ressources/shared/");
    
    // Set the overlay property for communication with the piga stack.
    engine.rootContext()->setContextProperty("Overlay", &overlay);
    
    qmlRegisterType<piga::overlay::Notification>("net.piga.overlay", 1, 0, "Notification");
    
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
    
    // Load the overlay QML code.
    QQuickView view(&engine, nullptr);
    view.setSource(QUrl(ressourceDir + "/ressources/overlay.qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setColor("transparent");
    view.setDefaultAlphaBuffer(true);
    view.setFormat(format);
    view.setFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    view.setClearBeforeRendering(true);
    
    view.showFullScreen();
    
    
    return app.exec();
}
