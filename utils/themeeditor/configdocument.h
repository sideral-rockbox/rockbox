/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2010 Robert Bieber
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#ifndef CONFIGDOCUMENT_H
#define CONFIGDOCUMENT_H

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QMap>

#include "tabcontent.h"

namespace Ui {
    class ConfigDocument;
}

class ConfigDocument : public TabContent {
    Q_OBJECT
public:
    ConfigDocument(QMap<QString, QString>& settings, QString file,
                   QWidget *parent = 0);
    virtual ~ConfigDocument();

    TabType type() const{ return TabContent::Config; }
    QString file() const{ return filePath; }
    QString title() const;

    QString toPlainText() const;

    void save();
    void saveAs();

    bool requestClose();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ConfigDocument *ui;
    QList<QHBoxLayout*> containers;
    QList<QLineEdit*> keys;
    QList<QLineEdit*> values;
    QList<QPushButton*> deleteButtons;

    QString filePath;
    QString saved;

    void addRow(QString key, QString value);

signals:
    void configFileChanged(QString);

private slots:
    void deleteClicked();
    void addClicked();
    void textChanged();
};

#endif // CONFIGDOCUMENT_H