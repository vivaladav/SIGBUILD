#pragma once

#include "qtcreatorsystray_global.h"

#include <extensionsystem/iplugin.h>

class QMenu;
class QSystemTrayIcon;

namespace QtCreatorSysTray {
namespace Internal {

class QtCreatorSysTrayPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "QtCreatorSysTray.json")

public:
    QtCreatorSysTrayPlugin();
    ~QtCreatorSysTrayPlugin();

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private slots:
    void onMessageClicked();

private:
    QMenu * mTrayMenu = nullptr;
    QSystemTrayIcon * mTrayIcon  = nullptr;
};

} // namespace Internal
} // namespace QtCreatorSysTray
