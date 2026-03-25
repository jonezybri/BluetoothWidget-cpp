#ifndef BLUETOOTHWIDGET_H
#define BLUETOOTHWIDGET_H

#include <QWidget>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

class BluetoothWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BluetoothWidget(QWidget *parent = nullptr);
    ~BluetoothWidget();

private slots:
    void startDiscovery();
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void discoveryFinished();
    void discoveryError(QBluetoothDeviceDiscoveryAgent::Error error);

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QListWidget *deviceListWidget;
    QPushButton *refreshButton;
    QLabel *statusLabel;
    QTimer *refreshTimer; // Timer for periodic refresh
};

#endif // BLUETOOTHWIDGET_H
