#include <QObject>
#include <QQmlApplicationEngine>

#include <piga/overlay/OverlayPlugin.hpp>

#include <piga/overlay/Applications.hpp>


#include <PigaOverlayConfig.hpp>

namespace piga
{
namespace overlay
{
OverlayPlugin::OverlayPlugin()
{
}
OverlayPlugin::~OverlayPlugin()
{

}
    
void OverlayPlugin::registerTypes()
{
    qmlRegisterType<Applications>("net.pigaco.overlay",
				  PIGA_OVERLAY_MAJOR_VERSION,
				  PIGA_OVERLAY_MINOR_VERSION,
				  "Applications");
}
}
}
