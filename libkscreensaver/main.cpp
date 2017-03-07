/* This file is part of the KDE libraries

    Copyright (c) 2001  Martin R. Jones <mjones@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#include "kscreensaver.h"
#include "kscreensaver_vroot.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#include <QDialog>
#include <QEvent>
#include <QKeyEvent>
#include <QSocketNotifier>

#include <QWidget>
#include <QWindow>
#include <QDesktopWidget>
#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>

#include <QDebug>

#include <KCrash>
#include <KAboutData>
#include <KLocalizedString>

#ifdef HAVE_X11
#include <QX11Info>
#endif

static void crashHandler( int  )
{
#ifdef SIGABRT
    signal (SIGABRT, SIG_DFL);
#endif
    abort();
}

extern "C" {

static int termPipe[2];

static void termHandler( int )
{
    write( termPipe[1], "", 1 );
}

}

//----------------------------------------------------------------------------


#ifdef HAVE_X11
extern "C" {

static int (*oldXErrorHandler)(Display *, XErrorEvent *);

static int xErrorHandler(Display *dpy, XErrorEvent *err)
{
    if (getppid() == 1) {
        qFatal("Got X error after loss of parent process. Terminating.");
    }
    return oldXErrorHandler(dpy, err);
}

}
#endif

int kScreenSaverMain( int argc, char** argv, KScreenSaverInterface& screenSaverInterface )
{
    // Disable session management so screensaver windows don't get restored on login (bug#314859)
    qunsetenv("SESSION_MANAGER");
    QApplication app(argc, argv);
    KAboutData *aboutData = screenSaverInterface.aboutData();
    if (aboutData) {
        KAboutData::setApplicationData(*aboutData);
    }

#ifdef HAVE_X11
    bool isXCB = QApplication::platformName().contains(QLatin1String("xcb"));
    qWarning() << Q_FUNC_INFO << "running on" << QGuiApplication::platformName() << isXCB;
#endif

    QCommandLineParser parser;
    parser.setApplicationDescription(app.translate("main", "The KClock analog clock screensaver from KDE4's kdeartwork"));
    const QCommandLineOption setupOption(QStringLiteral("setup"), i18n("Set up the screensaver"));
    const QCommandLineOption rootOption(QStringLiteral("root"), i18n("Run in the root XWindow or fullscreen"));
    const QCommandLineOption demoOption(QStringLiteral("demo"), i18n("Run in demo mode (default)"));
    parser.addOptions({setupOption, rootOption, demoOption});
    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(app);

    // Set a useful default icon.
    app.setWindowIcon(QIcon::fromTheme("preferences-desktop-screensaver", app.windowIcon()));

    if (!pipe(termPipe)) {
#ifndef Q_WS_WIN
        struct sigaction sa;
        sa.sa_handler = termHandler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGTERM, &sa, 0);
#endif
        QSocketNotifier *sn = new QSocketNotifier(termPipe[0], QSocketNotifier::Read, &app);
        QObject::connect(sn, SIGNAL(activated(int)), &app, SLOT(quit()));
    }

#ifdef HAVE_X11
    if (isXCB) {
        oldXErrorHandler = XSetErrorHandler(xErrorHandler);
    }
#endif
    KCrash::setCrashHandler(crashHandler);

    WId saveWin = 0;
    KScreenSaver *target;
    QWidget *rootWidget = 0;

    if (parser.isSet("setup")) {
       QDialog *dlg = screenSaverInterface.setup();
       dlg->exec();
       delete dlg;
       return 0;
    }

    if (parser.isSet("root")) {
#ifdef HAVE_X11
        if (isXCB) {
            saveWin = RootWindow(QX11Info::display(), QX11Info::appScreen());
            rootWidget = QApplication::desktop()->screen(QX11Info::appScreen());
            qWarning() << Q_FUNC_INFO << "RootWindow" << saveWin << "rootWidget=" << rootWidget
                << "for screen" << QX11Info::appScreen() << "of display" << QX11Info::display();
        }
        else
#endif
            rootWidget = QApplication::desktop()->screen();
    }

    if (parser.isSet("demo")) {
        saveWin = 0;
    }

    if (saveWin == 0) {
        target = screenSaverInterface.create(rootWidget);
    } else {
        target = screenSaverInterface.create(saveWin);
    }
    target->show();

    app.exec();

    delete target;

    return 0;
}

