/* This file is part of the KDE libraries

    Copyright (c) 2001  Martin R. Jones <mjones@kde.org>
    Copyright 2006  David Faure <faure@kde.org>

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

#ifndef KSCREENSAVER_H
#define KSCREENSAVER_H

#include <QWidget>
#include <QString>
#include <QKeyEvent>

#include <kaboutdata.h> // needed by all users of this header, so no point in a forward declaration

class KScreenSaverPrivate;
class KBlankEffectPrivate;

/**
 * Provides a QWidget for a screensaver to draw into.
 *
 * You should derive from this widget and implement your screensaver's
 * functionality.
 *
 * @see KScreenSaverInterface
 *
 * @short Provides a QWidget for a screensaver to draw into.
 * @author Martin R. Jones <mjones@kde.org>
 */
class Q_DECL_EXPORT KScreenSaver : public QWidget
{
    Q_OBJECT
public:
    KScreenSaver(QWidget *id);
    KScreenSaver(WId id);
    ~KScreenSaver();

protected:
    QWidget *self;
    bool event( QEvent* event );
    virtual bool eventFilter(QObject *, QEvent *e);
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void closeEvent(QCloseEvent *);
    bool isXCB;
};

/**
 *
 * To use libkscreensaver, derive from KScreenSaverInterface and reimplement
 * its virtual methods. Then write
 * <code>
 *   int main( int argc, char *argv[] )
 *   {
 *       MyKScreenSaverInterface kssi;
 *       return kScreenSaverMain( argc, argv, kssi );
 *   }
 * </code>
 *
 * In order to convert a KDE-3 screensaver (which used to export kss_* symbols and had no main function)
 * to KDE-4, you can use the following python script
 * kdebase/workspace/kscreensaver/libkscreensaver/kscreensaver-kde3to4-porting.py
 */
class Q_DECL_EXPORT KScreenSaverInterface
{
public:
    /**
     * Destructor
     */
    virtual ~KScreenSaverInterface();
    /**
     * Reimplement this method to return the KAboutData instance describing your screensaver
     */
    virtual KAboutData *aboutData();
    /**
     * Reimplement this method to return your KScreenSaver-derived screensaver
     */
    virtual KScreenSaver* create(QWidget *id) = 0;
    virtual KScreenSaver* create(WId id);
    /**
     * Reimplement this method to return your modal setup dialog
     */
    virtual QDialog* setup();
};

/**
 * The entry point for the program's main()
 * @see KScreenSaverInterface
 */
Q_DECL_EXPORT int kScreenSaverMain( int argc, char** argv, KScreenSaverInterface& screenSaverInterface );

#endif

