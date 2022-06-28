from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice
from csv import writer

"""Налаштування програми з графічним інтерфейсом"""
app = QtWidgets.QApplication([])
ui = uic.loadUi("design.ui")
ui.setWindowTitle("SerialGUI")

"""Встановлення налаштовунь для роботи з послідовним портом"""
serial = QSerialPort()
serial.setBaudRate(115200)
portList = []
ports = QSerialPortInfo().availablePorts()
for port in ports:
    portList.append(port.portName())
ui.comL.addItems(portList)


class Values:
    """клас для збереження масивів з значеннями акселерометра"""

    def __init__(self, x_list=None, y_list=None, z_list=None):
        if z_list is None:
            z_list = []
        if x_list is None:
            x_list = []
        if y_list is None:
            y_list = []
        self.x_list = x_list
        self.y_list = y_list
        self.z_list = z_list

    def get_X(self):
        return self.x_list

    def get_Y(self):
        return self.y_list

    def get_Z(self):
        return self.z_list

    def set_X(self, list_x:list):
        self.x_list = list_x

    def set_Y(self, list_y:list):
        self.y_list = list_y

    def set_Z(self, list_z:list):
        self.z_list = list_z

    def append_X(self, value):
        self.x_list.append(value)

    def append_Y(self, value):
        self.y_list.append(value)

    def append_Z(self, value):
        self.z_list.append(value)


class Interface:
    def __init__(self, uint, sensivity):
        self.uint = uint
        self.sensitivity = sensivity

    """Додаємо вибір фізичних величин"""
    ui.units_list.addItems(["mg", "g", "m/s2"])

    """Створюємо екземлпяри класу для збереження даних"""
    listValue = Values()  # список значень з акселерометра
    physicalListValue = Values()  # список значень з акселерометра, приведенних до фізичних величин

    def onRead(self):
        """Постійно опитує порт, працює безперервно"""

        """отримуємо дані та переводимо до зручного вигляду"""
        rx = serial.readLine()
        rxs = str(rx, 'utf-8').strip()
        data = [int(i) for i in rxs.split(";")]
        print(data)

        """фіксуємо поточні отриманні значення"""
        self.listValue.append_X(data[0])
        self.listValue.append_Y(data[1])
        self.listValue.append_Z(data[2])

        """фіксуємо обрані фізичні величини"""
        if ui.units_list.currentText() != self.uint:
            """якщо було змінено значення фізичних одиниць, то перерарахувати та встановити нові значення."""
            self.clearGraph()
            uint = ui.units_list.currentText()
            if uint == "g":
                ui.label_uintsX.setText("g")
                ui.label_uintsY.setText("g")
                ui.label_uintsZ.setText("g")
                self.uint = "g"
                self.sensitivity = 0.00006
                ui.sensitive_Value.setText(str(self.sensitivity))
            if uint == "mg":
                ui.label_uintsX.setText("mg")
                ui.label_uintsY.setText("mg")
                ui.label_uintsZ.setText("mg")
                self.uint = "mg"
                self.sensitivity = 0.06
                ui.sensitive_Value.setText(str(self.sensitivity))
            if uint == "m/s2":
                ui.label_uintsX.setText("m/s2")
                ui.label_uintsY.setText("m/s2")
                ui.label_uintsZ.setText("m/s2")
                self.uint = "m/s2"
                self.sensitivity = 0.000588399
                ui.sensitive_Value.setText(str(self.sensitivity))

        """Перетворюємо отримані данні до фізачних величин"""

        phisicData = []
        for i in range(len(data)):
            phisicData.append(data[i] * self.sensitivity)

        self.physicalListValue.append_X(phisicData[0])
        self.physicalListValue.append_Y(phisicData[1])
        self.physicalListValue.append_Z(phisicData[2])

        """Виводимо данні у додатку"""
        ui.Xlabel.setText("X: {: .4f}".format(phisicData[0]))
        ui.Ylabel.setText("Y: {: .4f}".format(phisicData[1]))
        ui.Zlabel.setText("Z: {: .4f}".format(phisicData[2]))

        """Встановлюємо данні для гістограми"""
        ui.XBar.setValue(round(data[0] / 150))
        ui.YBar.setValue(round(data[1] / 150))
        ui.ZBar.setValue(round(data[2] / 150))

        """Генеруємо данні осі Х  для побудови графіка"""
        axis_X = [j for j in range(len(self.physicalListValue.get_X()))]

        """Очищаємо старий графік"""
        ui.graph.clear()

        """Будуємо нові графіки згідно встановленних чекбоксів"""
        if ui.XcheckBox.isChecked():
            ui.graph.plot(axis_X, self.physicalListValue.get_X(), pen=(0, 3))
        if ui.YcheckBox.isChecked():
            ui.graph.plot(axis_X, self.physicalListValue.get_Y(), pen=(1, 3))
        if ui.ZcheckBox.isChecked():
            ui.graph.plot(axis_X, self.physicalListValue.get_Z(), pen=(2, 3))

    def onOpen(self):
        """Відкриття обраного порту"""
        serial.setPortName(ui.comL.currentText())
        serial.open(QIODevice.ReadWrite)

    def onClose(self):
        """Закриття порту"""
        serial.close()

    def clearGraph(self):
        """Очищення графіку"""
        ui.graph.clear()
        self.physicalListValue.set_X([])
        self.physicalListValue.set_Y([])
        self.physicalListValue.set_Z([])

    def saveData(self):
        """Збереження даних у csv файл"""
        myData = [["X", "Y", "Z"]]
        a = self.listValue.get_X()
        for i in range(len(self.listValue.get_X())):
            myData.append([self.listValue.get_X()[i], self.listValue.get_Y()[i], self.listValue.get_Z()[i]])
        myFile = open('data.csv', 'w')
        with myFile:
            writ = writer(myFile)
            writ.writerows(myData)
        print("Writing complete")


interface = Interface("mg", 0.06)

"""постійне зчитування данних з порту"""
serial.readyRead.connect(interface.onRead)

"""опрацювання натиску на кнопки графічного інтерфейсу"""
ui.openB.clicked.connect(interface.onOpen)
ui.clearB.clicked.connect(interface.clearGraph)
ui.closeB.clicked.connect(interface.onClose)
ui.outputButton.clicked.connect(interface.saveData)

"""запуск програми"""
ui.show()
app.exec()
