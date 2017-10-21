#include "qtcreatorsystrayplugin.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/mainwindow.h>
#include <projectexplorer/buildmanager.h>

#include <QAction>
#include <QDebug>
#include <QCoreApplication>
#include <QIcon>
#include <QMainWindow>
#include <QMenu>
#include <QSystemTrayIcon>

namespace QtCreatorSysTray {
namespace Internal {

QtCreatorSysTrayPlugin::QtCreatorSysTrayPlugin()
{
    // -- CREATE ICON --
    QIcon icon(":/img/qt_logo.png");
    mTrayIcon = new QSystemTrayIcon(icon, Core::ICore::mainWindow());

    // -- CREATE CONTEXT MENU --
    mTrayMenu = new QMenu();

    mTrayMenu->addSeparator();

    QAction * actionQuit = new QAction(tr("&Quit"));
    connect(actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
    mTrayMenu->addAction(actionQuit);

    mTrayIcon->setContextMenu(mTrayMenu);

    // SHOW
    mTrayIcon->show();
}

QtCreatorSysTrayPlugin::~QtCreatorSysTrayPlugin()
{
    delete mTrayMenu;
}

bool QtCreatorSysTrayPlugin::initialize(const QStringList & arguments, QString * errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    // get notified of builds
    connect(ProjectExplorer::BuildManager::instance(), &ProjectExplorer::BuildManager::buildQueueFinished, [=](bool res)
    {
        const int TIME = 2500;

        if(res)
            mTrayIcon->showMessage("Qt Creator", "build succesful! :)", QSystemTrayIcon::Information, TIME);
        else
            mTrayIcon->showMessage("Qt Creator", "build failed! :(", QSystemTrayIcon::Critical, TIME);
    });

    return true;
}

void QtCreatorSysTrayPlugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag QtCreatorSysTrayPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}


} // namespace Internal
} // namespace QtCreatorSysTray
