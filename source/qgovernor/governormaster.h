/*
 * qgovernor - QT based Open-BLDC PC interface tool
 * Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GOVERNORMASTER_H
#define GOVERNORMASTER_H

#include <QObject>
#include "log.h"

class GovernorMaster : public QObject
{
    Q_OBJECT
public:
    ~GovernorMaster();
    GovernorMaster();
    void outputTriggerCB();
    void registerChangedCB(unsigned char addr);
    signed short pickupByte();
    int sendSet(unsigned char addr, unsigned short data);
    int sendGet(unsigned char addr);
    int sendGetCont(unsigned char addr);
    int sendGetVersion(void);
    unsigned short getRegisterMapValue(unsigned char addr);
    int handleByte(unsigned char byte);
    void newLog(const QString &name);
    void stringReceivedCB(char *string, int size);
    QGLogger * reglog;

  signals:
    void outputTriggered();
    void registerChanged(unsigned char addr);
    void stringReceived(const QString &string);

};

#endif // GOVERNORMASTER_H
