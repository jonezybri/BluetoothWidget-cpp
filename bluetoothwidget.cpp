#include "bluetoothwidget.h"
#include <QDebug> // For logging
#include <QMessageBox>

BluetoothWidget::BluetoothWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Bluetooth Device Widget (C++)");
    setFixedSize(400, 300); // Fixed window size

    // Initialize UI components
    deviceListWidget = new QListWidget(this);
    refreshButton = new QPushButton("Refresh Devices", this);
    statusLabel = new QLabel("Ready to discover...", this);

    // Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(statusLabel);
    layout->addWidget(deviceListWidget);
    layout->addWidget(refreshButton);
    setLayout(layout);

    // Initialize Bluetooth device discovery agent
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    // Connect signals and slots
    connect(refreshButton, &QPushButton::clicked, this, &BluetoothWidget::startDiscovery);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BluetoothWidget::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BluetoothWidget::discoveryFinished);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &BluetoothWidget::discoveryError);

    // Set up a timer for periodic refresh
    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &BluetoothWidget::startDiscovery);
    refreshTimer->start(10000); // Refresh every 10 seconds (10000 ms)

    // Start initial discovery
    startDiscovery();
}

BluetoothWidget::~BluetoothWidget()
{
    // Stop discovery if it's still running
    if (discoveryAgent->isActive()) {
        discoveryAgent->stop();
    }
    // The discoveryAgent and other QObjects will be deleted by their parent QWidget
    // when the widget is destroyed, so no explicit delete is needed for them.
}

void BluetoothWidget::startDiscovery()
{
    deviceListWidget->clear(); // Clear previous list
    statusLabel->setText("Searching for Bluetooth devices...");
    qDebug() << "Starting Bluetooth device discovery...";
    if (discoveryAgent->isActive()) {
        discoveryAgent->stop(); // Stop any ongoing discovery before starting a new one
    }
    discoveryAgent->start();
    refreshButton->setEnabled(false); // Disable button during discovery
}

void BluetoothWidget::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    // Filter out devices without names or that are not paired/connected (heuristic)
    // Note: QBluetoothDeviceInfo::isPaired() and isConnected() can give more accurate status.
    if (device.isValid() && !device.name().isEmpty()) {
        QString deviceInfo = QString("Name: %1, Address: %2")
                                 .arg(device.name())
                                 .arg(device.address().toString());

        // Add more pertinent info if available
        if (device.isPaired()) {
            deviceInfo += ", Paired";
        }
        if (device.isConnected()) {
            deviceInfo += ", Connected";
        }
        deviceListWidget->addItem(deviceInfo);
        qDebug() << "Discovered device:" << deviceInfo;
    }
}

void BluetoothWidget::discoveryFinished()
{
    statusLabel->setText("Discovery finished. Ready.");
    refreshButton->setEnabled(true); // Re-enable button
    qDebug() << "Bluetooth device discovery finished.";
    if (deviceListWidget->count() == 0) {
        deviceListWidget->addItem("No Bluetooth devices found.");
    }
}

void BluetoothWidget::discoveryError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    QString errorString;
    switch (error) {
        case QBluetoothDeviceDiscoveryAgent::PoweredOffError:
            errorString = "Bluetooth is powered off.";
            break;
        case QBluetoothDeviceDiscoveryAgent::InputOutputError:
            errorString = "Input/Output error.";
            break;
        case QBluetoothDeviceDiscoveryAgent::UnsupportedMethodError:
            errorString = "Unsupported discovery method.";
            break;
        case QBluetoothDeviceDiscoveryAgent::InvalidBluetoothAdapterError:
            errorString = "Invalid Bluetooth adapter.";
            break;
        case QBluetoothDeviceDiscoveryAgent::UnknownError:
        default:
            errorString = "An unknown error occurred during discovery.";
            break;
    }
    statusLabel->setText("Error: " + errorString);
    refreshButton->setEnabled(true); // Re-enable button
    qWarning() << "Bluetooth discovery error:" << errorString;
    QMessageBox::warning(this, "Bluetooth Error", "Could not complete Bluetooth device discovery: " + errorString);
}
