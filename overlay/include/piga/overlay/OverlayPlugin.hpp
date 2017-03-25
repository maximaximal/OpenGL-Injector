#pragma once

#include <QObject>

namespace piga
{
namespace overlay
{
    class OverlayPlugin : public QObject
{
    Q_OBJECT
    
public:
    OverlayPlugin();
    virtual ~OverlayPlugin();

    /**
     * @brief This method registers all the overlay QML C++ extensions.
     */
    virtual void registerTypes();
private:
    
};
}
}
