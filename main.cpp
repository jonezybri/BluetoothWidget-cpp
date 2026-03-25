#include <QApplication>
#include "bluetoothwidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    BluetoothWidget w;
    w.show();
    return a.exec();
}
