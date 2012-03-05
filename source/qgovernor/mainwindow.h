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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QtGui>
#include <QTcpSocket>

#include "governormaster.h"

#include "registermodel.h"
#include "protocolmodel.h"
#include "connectdialog.h"
#include "governorsimulator.h"
#include "governorftdi.h"
#include "governorserial.h"
#include "serialselect.h"

#include "govconfig.h"



namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

    ConnectDialog *connectDialog;
    serialSelect *SerialSelect;
    QIODevice *governorInterface;
    RegisterModel registerModel;
    ProtocolModel outputModel;
    ProtocolModel inputModel;
    QTcpSocket *tcpSocket;

    GovernorMaster *governorMaster;
    bool connected;

    QAction *updateRegister;
    QAction *updateAllRegisters;

private slots:
    void on_consoleClearPushButton_pressed();
    void on_PWMDutyCycleHorizontalSlider_valueChanged(int value);
    void on_commIIRPoleSpinBox_valueChanged(int );
    void on_commDirectCutoffSpinBox_valueChanged(int );
    void on_commSparkAdvanceSpinBox_valueChanged(int );
    void on_ADCLevelMonCheckBox_clicked(bool checked);
    void on_forcedCommMonCheckBox_clicked(bool checked);
    void on_forcedCommTimIncSpinBox_valueChanged(int );
    void on_triggerCommPushButton_clicked();
    void on_ADCZeroValueSpinBox_valueChanged(int );
    void on_forcedCommTimValSpinBox_valueChanged(int );
    void on_PWMOffsetSpinBox_valueChanged(int );
    void on_PWMDutyCycleSpinBox_valueChanged(int );
    void on_ADCCommCheckBox_clicked(bool checked);
    void on_forcedCommCheckBox_clicked(bool checked);
    void on_actionConnect_triggered(bool checked);
    void on_governorInterface_aboutToClose();
    void on_actionLoadTarget_triggered();
    void on_actionOpenLog_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();
    void on_registerTableView_customContextMenuRequested(QPoint pos);
    void on_outputTriggered();
    void on_registerChanged(unsigned char addr);
    void on_guiRegisterChanged(QStandardItem *item);
    void on_governorInterface_readyRead();
    void on_stringReceived(QString string);

    void addTargetTab(GovConfig const & config);
};

#endif // MAINWINDOW_H
