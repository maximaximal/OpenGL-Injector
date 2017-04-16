#include <piga/overlay/OverlayWrapper.hpp>

using piga::overlay::OverlayWrapper;

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(overlay);
    OverlayWrapper wrapper(argc, argv);
    return wrapper.startOverlay();
}
