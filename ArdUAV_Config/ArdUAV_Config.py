"""

"""

import sys
import os
from PyQt5.QtWidgets import QApplication, QDialog, QDialogButtonBox
from configGUI import Ui_Dialog


class AppWindow(QDialog):
    def __init__(self):
        super().__init__()
        self.ui = Ui_Dialog()
        self.ui.setupUi(self)
        self.show()

        self.setupSignals()

        self.boolComboBoxIndices = {'1': 0,
                                    '0': 1}

        self.comboBoxIndicesBool = {'True': '1',
                                    'False': '0'}

        self.servoPinComboBoxIndices = {'12': 0,
                                        '39': 1,
                                        '16': 2,
                                        '24': 3,
                                        '25': 4,
                                        '26': 5,
                                        '27': 6,
                                        '28': 7,
                                        '29': 8,
                                        '30': 9,
                                        '23': 10,
                                        '22': 11,
                                        '17': 12,
                                        '16': 13,
                                        '14': 14,
                                        '36': 15}

        self.comboBoxIndicesServoPin = {'s0': '12',
                                        's1': '39',
                                        's2': '16',
                                        's3': '24',
                                        's4': '25',
                                        's5': '26',
                                        's6': '27',
                                        's7': '28',
                                        's8': '29',
                                        's9': '30',
                                        's10': '23',
                                        's11': '22',
                                        's12': '17',
                                        's13': '16',
                                        's14': '14',
                                        's15': '36'}

        self.analogPinComboBoxIndices = {'A20': 0,
                                         'A9': 1,
                                         'A8': 2,
                                         'A3': 3,
                                         'A2': 4,
                                         'A0': 5,
                                         'A17': 6}

        self.comboBoxIndicesAnalogPin = {'s1': 'A20',
                                         's10': 'A9',
                                         's11': 'A8',
                                         's12': 'A3',
                                         's13': 'A2',
                                         's14': 'A0',
                                         's15': 'A17'}

        self.baudComboBoxIndices = {'300': 0,
                                    '1200': 1,
                                    '2400': 2,
                                    '4800': 3,
                                    '9600': 4,
                                    '19200': 5,
                                    '38400': 6,
                                    '57600': 7,
                                    '115200': 8,
                                    '230400': 9,
                                    '250000': 10,
                                    '460800': 11,
                                    '500000': 12,
                                    '921600': 13,
                                    '1000000': 14,
                                    '2000000': 15}

        self.portComboBoxIndicesPortNum = {'USB': '0',
                                           'Serial1': '1',
                                           'Serial2': '2',
                                           'Serial3': '3',
                                           'Serial4': '4',
                                           'Serial5': '5'}

        self.BASE_LIBRARY_PATH = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))

        self.sharedToolsFilename = os.path.join(self.BASE_LIBRARY_PATH, "Shared_Tools", "Shared_Tools.h")
        self.GSToolsFilename = os.path.join(self.BASE_LIBRARY_PATH, "GS_Tools", "GS_Tools.h")
        self.GSSerialFilename = os.path.join(self.BASE_LIBRARY_PATH, "GS_Tools", "GS_Serial.h")
        self.IFCToolsFilename = os.path.join(self.BASE_LIBRARY_PATH, "IFC_Tools", "IFC_Tools.h")
        self.IFCSerialFilename = os.path.join(self.BASE_LIBRARY_PATH, "IFC_Tools", "IFC_Serial.h")

        self.currentParameters = {'SharedTools': {'SerialSettings': {'Debug_Port_Baud': "",
                                                                     'Command_Port_Baud': "",
                                                                     'GPS_Port_Baud': "",
                                                                     'Telemetry_Port_Baud': ""},
                                                  'Reporting_TimeoutSettings': {'CommandReportingRate': "",
                                                                                'TelemetryReportingRate': "",
                                                                                'LossLinkTimeout': ""},
                                                  'ThrottleSettings': {'MaxThrottleValue': "",
                                                                       'MinThrottleValue': ""},
                                                  'ElevatorSettings': {'MaxElevatorValue': "",
                                                                       'MinElevatorValue': ""},
                                                  'AileronSettings': {'MaxAileronValue': "",
                                                                      'MinAileronValue': ""},
                                                  'RudderSettings': {'MaxRudderValue': "",
                                                                     'MinRudderValue': ""}},
                                  'GSTools': {'SerialSettings': {'GS_DebugPortNumber': "",
                                                                 'GS_CommandPortNumber': "",
                                                                 'GS_TelemetryPortNumber': ""},
                                              'ThrottleSettings': {'ThrottleAnalogPin': "",
                                                                   'MinThrottleADCValue': "",
                                                                   'MaxThrottleADCValue': ""},
                                              'ElevatorSettings': {'ElevatorAnalogPin': "",
                                                                   'MinElevatorADCValue': "",
                                                                   'MaxElevatorADCValue': "",
                                                                   'MinElevatorServoValue': "",
                                                                   'MaxElevatorServoValue': ""},
                                              'AileronSettings': {'AileronAnalogPin': "",
                                                                  'MinAileronADCValue': "",
                                                                  'MaxAileronADCValue': "",
                                                                  'MinAileronServoValue': "",
                                                                  'MaxAileronServoValue': ""},
                                              'RudderSettings': {'RudderAnalogPin': "",
                                                                 'MinRudderADCValue': "",
                                                                 'MaxRudderADCValue': "",
                                                                 'MinRudderServoValue': "",
                                                                 'MaxRudderServoValue': ""}},
                                  'IFCTools': {'SerialSettings': {'IFC_DebugPortNumber': "",
                                                                  'IFC_CommandPortNumber': "",
                                                                  'IFC_GPSPortNumber': "",
                                                                  'IFC_TelemetryPortNumber': ""},
                                               'ControlSurfaceSettings': {'ThrottlePin': "",
                                                                          'ElevatorPin': "",
                                                                          'RightAileronPin': "",
                                                                          'LeftAileronPin': "",
                                                                          'RudderPin': ""},
                                               'AutopilotSettings': {'UnsafeRollRightAngle': "",
                                                                     'UnsafeRollLeftAngle': "",
                                                                     'MaxRollRightAngle': "",
                                                                     'MaxRollLeftAngle': "",
                                                                     'UnsafePitchUpAngle': "",
                                                                     'UnsafePitchDownAngle': "",
                                                                     'MaxPitchUpAngle': "",
                                                                     'MaxPitchDownAngle': ""},
                                               'OtherSettings': {'PitotTubeAnalogPin': "",
                                                                 'LiDARFixedMount': ""}}}
        self.getParameters()

    def setupSignals(self):
        self.ui.GetCurrentValues.clicked.connect(self.getParameters)
        self.ui.buttonBox.clicked.connect(self.handleButtonClick)

    def handleButtonClick(self, button):
        sb = self.ui.buttonBox.standardButton(button)
        if sb == QDialogButtonBox.Apply:
            self.writeOutParameters()

        elif sb == QDialogButtonBox.Cancel:
            self.close()

    def getParameters(self):
        try:
            self.readSharedTools()
            self.readGSTools()
            self.readGSSerial()
            self.readIFCTools()
            self.readIFCSerial()

            self.updateGUI()

        except:
            import traceback
            traceback.print_exc()

    def readSharedTools(self):
        with open(self.sharedToolsFilename, 'r') as inFile:
            contents = inFile.readlines()

        for line in contents:
            if len(line.split()) >= 3:
                if line.split()[0] == "#define" and line.split()[1] == "DEBUG_PORT_BAUD":
                    self.currentParameters['SharedTools']['SerialSettings']['Debug_Port_Baud'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "COMMAND_PORT_BAUD":
                    self.currentParameters['SharedTools']['SerialSettings']['Command_Port_Baud'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "GPS_PORT_BAUD":
                    self.currentParameters['SharedTools']['SerialSettings']['GPS_Port_Baud'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "TELEM_PORT_BAUD":
                    self.currentParameters['SharedTools']['SerialSettings']['Telemetry_Port_Baud'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "REPORT_COMMANDS_FREQ":
                    self.currentParameters['SharedTools']['Reporting_TimeoutSettings']['CommandReportingRate'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "REPORT_TELEM_FREQ":
                    self.currentParameters['SharedTools']['Reporting_TimeoutSettings']['TelemetryReportingRate'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "LOSS_LINK_TIMEOUT":
                    self.currentParameters['SharedTools']['Reporting_TimeoutSettings']['LossLinkTimeout'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "THROTTLE_MAX":
                    self.currentParameters['SharedTools']['ThrottleSettings']['MaxThrottleValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "AILERON_MAX":
                    self.currentParameters['SharedTools']['AileronSettings']['MaxAileronValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_MAX":
                    self.currentParameters['SharedTools']['ElevatorSettings']['MaxElevatorValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_MAX":
                    self.currentParameters['SharedTools']['RudderSettings']['MaxRudderValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "THROTTLE_MIN":
                    self.currentParameters['SharedTools']['ThrottleSettings']['MinThrottleValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "AILERON_MIN":
                    self.currentParameters['SharedTools']['AileronSettings']['MinAileronValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_MIN":
                    self.currentParameters['SharedTools']['ElevatorSettings']['MinElevatorValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_MIN":
                    self.currentParameters['SharedTools']['RudderSettings']['MinRudderValue'] = str(line.split()[2])

    def readGSTools(self):
        with open(self.GSToolsFilename, 'r') as inFile:
            contents = inFile.readlines()

        for line in contents:
            if len(line.split()) >= 3:
                if line.split()[0] == "#define" and line.split()[1] == "YAW_ANALOG_PIN":
                    self.currentParameters['GSTools']['RudderSettings']['RudderAnalogPin'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "THROTTLE_ANALOG_PIN":
                    self.currentParameters['GSTools']['ThrottleSettings']['ThrottleAnalogPin'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "ROLL_ANALOG_PIN":
                    self.currentParameters['GSTools']['AileronSettings']['AileronAnalogPin'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "PITCH_ANALOG_PIN":
                    self.currentParameters['GSTools']['ElevatorSettings']['ElevatorAnalogPin'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "AILERON_MAX_LOWRATES":
                    self.currentParameters['GSTools']['AileronSettings']['MaxAileronServoValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_MAX_LOWRATES":
                    self.currentParameters['GSTools']['ElevatorSettings']['MaxElevatorServoValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_MAX_LOWRATES":
                    self.currentParameters['GSTools']['RudderSettings']['MaxRudderServoValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "AILERON_MIN_LOWRATES":
                    self.currentParameters['GSTools']['AileronSettings']['MinAileronServoValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_MIN_LOWRATES":
                    self.currentParameters['GSTools']['ElevatorSettings']['MinElevatorServoValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_MIN_LOWRATES":
                    self.currentParameters['GSTools']['RudderSettings']['MinRudderServoValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "THROTTLE_MIN_ADC":
                    self.currentParameters['GSTools']['ThrottleSettings']['MinThrottleADCValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "AILERON_MIN_ADC":
                    self.currentParameters['GSTools']['AileronSettings']['MinAileronADCValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_MIN_ADC":
                    self.currentParameters['GSTools']['ElevatorSettings']['MinElevatorADCValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_MIN_ADC":
                    self.currentParameters['GSTools']['RudderSettings']['MinRudderADCValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "THROTTLE_MAX_ADC":
                    self.currentParameters['GSTools']['ThrottleSettings']['MaxThrottleADCValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "AILERON_MAX_ADC":
                    self.currentParameters['GSTools']['AileronSettings']['MaxAileronADCValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_MAX_ADC":
                    self.currentParameters['GSTools']['ElevatorSettings']['MaxElevatorADCValue'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_MAX_ADC":
                    self.currentParameters['GSTools']['RudderSettings']['MaxRudderADCValue'] = str(line.split()[2])

    def readGSSerial(self):
        with open(self.GSSerialFilename, 'r') as inFile:
            contents = inFile.readlines()

        for line in contents:
            if len(line.split()) >= 3:
                if line.split()[0] == "#define" and line.split()[1] == "GS_DEBUG_PORT_NUMBER":
                    self.currentParameters['GSTools']['SerialSettings']['GS_DebugPortNumber'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "GS_COMMAND_PORT_NUMBER":
                    self.currentParameters['GSTools']['SerialSettings']['GS_CommandPortNumber'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "GS_TELEM_PORT_NUMBER":
                    self.currentParameters['GSTools']['SerialSettings']['GS_TelemetryPortNumber'] = str(line.split()[2])

    def readIFCTools(self):
        with open(self.IFCToolsFilename, 'r') as inFile:
            contents = inFile.readlines()

        for line in contents:
            if len(line.split()) >= 3:
                if line.split()[0] == "#define" and line.split()[1] == "LIDAR_FIXED_MOUNT":
                    self.currentParameters['IFCTools']['OtherSettings']['LiDARFixedMount'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "PITOT_PIN":
                    self.currentParameters['IFCTools']['OtherSettings']['PitotTubeAnalogPin'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "THROTTLE_PIN":
                    self.currentParameters['IFCTools']['ControlSurfaceSettings']['ThrottlePin'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "R_AILERON_PIN":
                    self.currentParameters['IFCTools']['ControlSurfaceSettings']['RightAileronPin'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "L_AILERON_PIN":
                    self.currentParameters['IFCTools']['ControlSurfaceSettings']['LeftAileronPin'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_PIN":
                    self.currentParameters['IFCTools']['ControlSurfaceSettings']['ElevatorPin'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_PIN":
                    self.currentParameters['IFCTools']['ControlSurfaceSettings']['RudderPin'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "UNSAFE_ROLL_R":
                    self.currentParameters['IFCTools']['AutopilotSettings']['UnsafeRollRightAngle'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "UNSAFE_ROLL_L":
                    self.currentParameters['IFCTools']['AutopilotSettings']['UnsafeRollLeftAngle'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "MAX_ROLL_R":
                    self.currentParameters['IFCTools']['AutopilotSettings']['MaxRollRightAngle'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "MAX_ROLL_L":
                    self.currentParameters['IFCTools']['AutopilotSettings']['MaxRollLeftAngle'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "UNSAFE_PITCH_UP":
                    self.currentParameters['IFCTools']['AutopilotSettings']['UnsafePitchUpAngle'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "UNSAFE_PITCH_DOWN":
                    self.currentParameters['IFCTools']['AutopilotSettings']['UnsafePitchDownAngle'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "MAX_PITCH_UP":
                    self.currentParameters['IFCTools']['AutopilotSettings']['MaxPitchUpAngle'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "MAX_PITCH_DOWN":
                    self.currentParameters['IFCTools']['AutopilotSettings']['MaxPitchDownAngle'] = str(line.split()[2])

    def readIFCSerial(self):
        with open(self.IFCSerialFilename, 'r') as inFile:
            contents = inFile.readlines()

        for line in contents:
            if len(line.split()) >= 3:
                if line.split()[0] == "#define" and line.split()[1] == "IFC_DEBUG_PORT_NUMBER":
                    self.currentParameters['IFCTools']['SerialSettings']['IFC_DebugPortNumber'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "IFC_COMMAND_PORT_NUMBER":
                    self.currentParameters['IFCTools']['SerialSettings']['IFC_CommandPortNumber'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "IFC_GPS_PORT_NUMBER":
                    self.currentParameters['IFCTools']['SerialSettings']['IFC_GPSPortNumber'] = str(line.split()[2])

                elif line.split()[0] == "#define" and line.split()[1] == "IFC_TELEM_PORT_NUMBER":
                    self.currentParameters['IFCTools']['SerialSettings']['IFC_TelemetryPortNumber'] = str(line.split()[2])

    def updateGUI(self):
        self.updateGUISharedTools()
        self.updateGUIGSTools()
        self.updateGUIIFCTools()

    def updateGUISharedTools(self):
        self.value = self.currentParameters['SharedTools']['SerialSettings']['Debug_Port_Baud']
        self.ui.Debug_Port_Baud.setCurrentIndex(self.baudComboBoxIndices[self.value])

        self.value = self.currentParameters['SharedTools']['SerialSettings']['Command_Port_Baud']
        self.ui.Command_Port_Baud.setCurrentIndex(self.baudComboBoxIndices[self.value])

        self.value = self.currentParameters['SharedTools']['SerialSettings']['GPS_Port_Baud']
        self.ui.GPS_Port_Baud.setCurrentIndex(self.baudComboBoxIndices[self.value])

        self.value = self.currentParameters['SharedTools']['SerialSettings']['Telemetry_Port_Baud']
        self.ui.Telemetry_Port_Baud.setCurrentIndex(self.baudComboBoxIndices[self.value])

        self.value = float(self.currentParameters['SharedTools']['Reporting_TimeoutSettings']['CommandReportingRate'])
        self.ui.CommandReportingRate.setValue(self.value)

        self.value = float(self.currentParameters['SharedTools']['Reporting_TimeoutSettings']['TelemetryReportingRate'])
        self.ui.TelemetryReportingRate.setValue(self.value)

        self.value = int(self.currentParameters['SharedTools']['Reporting_TimeoutSettings']['LossLinkTimeout'])
        self.ui.LossLinkTimeout.setValue(self.value)

        self.value = int(self.currentParameters['SharedTools']['ThrottleSettings']['MaxThrottleValue'])
        self.ui.MaxThrottleValue.setValue(self.value)

        self.value = int(self.currentParameters['SharedTools']['ThrottleSettings']['MinThrottleValue'])
        self.ui.MinThrottleValue.setValue(self.value)

        self.value = int(self.currentParameters['SharedTools']['ElevatorSettings']['MaxElevatorValue'])
        self.ui.MaxElevatorValue.setValue(self.value)

        self.value = int(self.currentParameters['SharedTools']['ElevatorSettings']['MinElevatorValue'])
        self.ui.MinElevatorValue.setValue(self.value)

        self.value = int(self.currentParameters['SharedTools']['AileronSettings']['MaxAileronValue'])
        self.ui.MaxAileronValue.setValue(self.value)

        self.value = int(self.currentParameters['SharedTools']['AileronSettings']['MinAileronValue'])
        self.ui.MinAileronValue.setValue(self.value)

        self.value = int(self.currentParameters['SharedTools']['RudderSettings']['MaxRudderValue'])
        self.ui.MaxRudderValue.setValue(self.value)

        self.value = int(self.currentParameters['SharedTools']['RudderSettings']['MinRudderValue'])
        self.ui.MinRudderValue.setValue(self.value)

    def updateGUIGSTools(self):
        self.value = int(self.currentParameters['GSTools']['SerialSettings']['GS_DebugPortNumber'])
        self.ui.GSDebugPort.setCurrentIndex(self.value)

        self.value = int(self.currentParameters['GSTools']['SerialSettings']['GS_CommandPortNumber'])
        self.ui.GSCommandPort.setCurrentIndex(self.value)

        self.value = int(self.currentParameters['GSTools']['SerialSettings']['GS_TelemetryPortNumber'])
        self.ui.GSTelemetryPort.setCurrentIndex(self.value)

        self.value = self.currentParameters['GSTools']['ThrottleSettings']['ThrottleAnalogPin']
        self.ui.ThrottleAnalogPin.setCurrentIndex(self.analogPinComboBoxIndices[self.value])

        self.value = int(self.currentParameters['GSTools']['ThrottleSettings']['MinThrottleADCValue'])
        self.ui.MinThrottleADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['ThrottleSettings']['MaxThrottleADCValue'])
        self.ui.MaxThrottleADCValue.setValue(self.value)

        self.value = self.currentParameters['GSTools']['ElevatorSettings']['ElevatorAnalogPin']
        self.ui.ElevatorAnalogPin.setCurrentIndex(self.analogPinComboBoxIndices[self.value])

        self.value = int(self.currentParameters['GSTools']['ElevatorSettings']['MinElevatorADCValue'])
        self.ui.MinElevatorADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['ElevatorSettings']['MaxElevatorADCValue'])
        self.ui.MaxElevatorADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['ElevatorSettings']['MinElevatorServoValue'])
        self.ui.MinElevatorServoValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['ElevatorSettings']['MaxElevatorServoValue'])
        self.ui.MaxElevatorServoValue.setValue(self.value)

        self.value = self.currentParameters['GSTools']['AileronSettings']['AileronAnalogPin']
        self.ui.AileronAnalogPin.setCurrentIndex(self.analogPinComboBoxIndices[self.value])

        self.value = int(self.currentParameters['GSTools']['AileronSettings']['MinAileronADCValue'])
        self.ui.MinAileronADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['AileronSettings']['MaxAileronADCValue'])
        self.ui.MaxAileronADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['AileronSettings']['MinAileronServoValue'])
        self.ui.MinAileronServoValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['AileronSettings']['MaxAileronServoValue'])
        self.ui.MaxAileronServoValue.setValue(self.value)

        self.value = self.currentParameters['GSTools']['RudderSettings']['RudderAnalogPin']
        self.ui.RudderAnalogPin.setCurrentIndex(self.analogPinComboBoxIndices[self.value])

        self.value = int(self.currentParameters['GSTools']['RudderSettings']['MinRudderADCValue'])
        self.ui.MinRudderADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['RudderSettings']['MaxRudderADCValue'])
        self.ui.MaxRudderADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['RudderSettings']['MinRudderServoValue'])
        self.ui.MinRudderServoValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['RudderSettings']['MaxRudderServoValue'])
        self.ui.MaxRudderServoValue.setValue(self.value)

    def updateGUIIFCTools(self):
        self.value = int(self.currentParameters['IFCTools']['SerialSettings']['IFC_DebugPortNumber'])
        self.ui.IFCDebugPort.setCurrentIndex(self.value)

        self.value = int(self.currentParameters['IFCTools']['SerialSettings']['IFC_CommandPortNumber'])
        self.ui.IFCCommandPort.setCurrentIndex(self.value)
		
        self.value = int(self.currentParameters['IFCTools']['SerialSettings']['IFC_GPSPortNumber'])
        self.ui.IFCGPSPort.setCurrentIndex(self.value)

        self.value = int(self.currentParameters['IFCTools']['SerialSettings']['IFC_TelemetryPortNumber'])
        self.ui.IFCTelemetryPort.setCurrentIndex(self.value)

        self.value = self.currentParameters['IFCTools']['ControlSurfaceSettings']['ThrottlePin']
        self.ui.ThrottlePin.setCurrentIndex(self.servoPinComboBoxIndices[self.value])

        self.value = self.currentParameters['IFCTools']['ControlSurfaceSettings']['ElevatorPin']
        self.ui.ElevatorPin.setCurrentIndex(self.servoPinComboBoxIndices[self.value])

        self.value = self.currentParameters['IFCTools']['ControlSurfaceSettings']['RightAileronPin']
        self.ui.RightAileronPin.setCurrentIndex(self.servoPinComboBoxIndices[self.value])

        self.value = self.currentParameters['IFCTools']['ControlSurfaceSettings']['LeftAileronPin']
        self.ui.LeftAileronPin.setCurrentIndex(self.servoPinComboBoxIndices[self.value])

        self.value = self.currentParameters['IFCTools']['ControlSurfaceSettings']['RudderPin']
        self.ui.RudderPin.setCurrentIndex(self.servoPinComboBoxIndices[self.value])

        self.value = int(self.currentParameters['IFCTools']['AutopilotSettings']['UnsafeRollRightAngle'])
        self.ui.UnsafeRollRightAngle.setValue(self.value)

        self.value = int(self.currentParameters['IFCTools']['AutopilotSettings']['UnsafeRollLeftAngle'])
        self.ui.UnsafeRollLeftAngle.setValue(self.value)

        self.value = int(self.currentParameters['IFCTools']['AutopilotSettings']['MaxRollRightAngle'])
        self.ui.MaxRollRightAngle.setValue(self.value)

        self.value = int(self.currentParameters['IFCTools']['AutopilotSettings']['MaxRollLeftAngle'])
        self.ui.MaxRollLeftAngle.setValue(self.value)

        self.value = int(self.currentParameters['IFCTools']['AutopilotSettings']['UnsafePitchUpAngle'])
        self.ui.UnsafePitchUpAngle.setValue(self.value)

        self.value = int(self.currentParameters['IFCTools']['AutopilotSettings']['UnsafePitchDownAngle'])
        self.ui.UnsafePitchDownAngle.setValue(self.value)

        self.value = int(self.currentParameters['IFCTools']['AutopilotSettings']['MaxPitchUpAngle'])
        self.ui.MaxPitchUpAngle.setValue(self.value)

        self.value = int(self.currentParameters['IFCTools']['AutopilotSettings']['MaxPitchDownAngle'])
        self.ui.MaxPitchDownAngle.setValue(self.value)

        self.value = self.currentParameters['IFCTools']['OtherSettings']['PitotTubeAnalogPin']
        self.ui.PitotTubeAnalogPin.setCurrentIndex(self.analogPinComboBoxIndices[self.value])

        self.value = self.currentParameters['IFCTools']['OtherSettings']['LiDARFixedMount']
        self.ui.LiDARFixedMount.setCurrentIndex(self.boolComboBoxIndices[self.value])

    def writeOutParameters(self):
        try:
            self.writeOutSharedTools()
            self.writeOutGSTools()
            self.writeOutGSSerial()
            self.writeOutIFCTools()
            self.writeOutIFCSerial()

            print("--------------------------------")
            print("Library Updated")
        except:
            import traceback
            traceback.print_exc()

    def writeOutSharedTools(self):
        with open(self.sharedToolsFilename, 'r') as inFile:
            contents = inFile.readlines()

        newContents = []

        for line in contents:
            if len(line.split()) >= 3:
                if line.split()[0] == "#define" and line.split()[1] == "DEBUG_PORT_BAUD":
                    newValue = self.ui.Debug_Port_Baud.currentText()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "COMMAND_PORT_BAUD":
                    newValue = self.ui.Command_Port_Baud.currentText()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "GPS_PORT_BAUD":
                    newValue = self.ui.GPS_Port_Baud.currentText()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "TELEM_PORT_BAUD":
                    newValue = self.ui.Telemetry_Port_Baud.currentText()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "REPORT_COMMANDS_FREQ":
                    newValue = self.ui.CommandReportingRate.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "REPORT_TELEM_FREQ":
                    newValue = self.ui.TelemetryReportingRate.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "LOSS_LINK_TIMEOUT":
                    newValue = self.ui.LossLinkTimeout.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "THROTTLE_MAX":
                    newValue = self.ui.MaxThrottleValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "AILERON_MAX":
                    newValue = self.ui.MaxAileronValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_MAX":
                    newValue = self.ui.MaxElevatorValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_MAX":
                    newValue = self.ui.MaxRudderValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "THROTTLE_MIN":
                    newValue = self.ui.MinThrottleValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "AILERON_MIN":
                    newValue = self.ui.MinAileronValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_MIN":
                    newValue = self.ui.MinElevatorValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_MIN":
                    newValue = self.ui.MinRudderValue.text()
                    line = line.replace(line.split()[2], newValue)

            newContents.append(line)

        newContents = "".join(newContents)

        with open(self.sharedToolsFilename, 'w') as outFile:
            outFile.write(newContents)

    def writeOutGSTools(self):
        with open(self.GSToolsFilename, 'r') as inFile:
            contents = inFile.readlines()

        newContents = []

        for line in contents:
            if len(line.split()) >= 3:
                if line.split()[0] == "#define" and line.split()[1] == "YAW_ANALOG_PIN":
                    newValue = self.comboBoxIndicesAnalogPin[self.ui.RudderAnalogPin.currentText()]
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "THROTTLE_ANALOG_PIN":
                    newValue = self.comboBoxIndicesAnalogPin[self.ui.ThrottleAnalogPin.currentText()]
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "ROLL_ANALOG_PIN":
                    newValue = self.comboBoxIndicesAnalogPin[self.ui.AileronAnalogPin.currentText()]
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "PITCH_ANALOG_PIN":
                    newValue = self.comboBoxIndicesAnalogPin[self.ui.ElevatorAnalogPin.currentText()]
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "AILERON_MAX_LOWRATES":
                    newValue = self.ui.MaxAileronServoValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_MAX_LOWRATES":
                    newValue = self.ui.MaxElevatorServoValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_MAX_LOWRATES":
                    newValue = self.ui.MaxRudderServoValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "AILERON_MIN_LOWRATES":
                    newValue = self.ui.MinAileronServoValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_MIN_LOWRATES":
                    newValue = self.ui.MinElevatorServoValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_MIN_LOWRATES":
                    newValue = self.ui.MinRudderServoValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "THROTTLE_MIN_ADC":
                    newValue = self.ui.MinThrottleADCValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "AILERON_MIN_ADC":
                    newValue = self.ui.MinAileronADCValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_MIN_ADC":
                    newValue = self.ui.MinElevatorADCValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_MIN_ADC":
                    newValue = self.ui.MinRudderADCValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "THROTTLE_MAX_ADC":
                    newValue = self.ui.MaxThrottleADCValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "AILERON_MAX_ADC":
                    newValue = self.ui.MaxAileronADCValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_MAX_ADC":
                    newValue = self.ui.MaxElevatorADCValue.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_MAX_ADC":
                    newValue = self.ui.MaxRudderADCValue.text()
                    line = line.replace(line.split()[2], newValue)
            newContents.append(line)

        newContents = "".join(newContents)

        with open(self.GSToolsFilename, 'w') as outFile:
            outFile.write(newContents)

    def writeOutGSSerial(self):
        with open(self.GSSerialFilename, 'r') as inFile:
            contents = inFile.readlines()

        newContents = []

        for line in contents:
            if len(line.split()) >= 3:
                if line.split()[0] == "#define" and line.split()[1] == "GS_DEBUG_PORT_NUMBER":
                    newValue = self.portComboBoxIndicesPortNum[self.ui.GSDebugPort.currentText()]
                    line = line.replace(line.split()[2], newValue)

                    # attempt to change comment if possible
                    try:
                        line = line.replace(line.split()[3], "//" + self.ui.GSDebugPort.currentText())
                    except:
                        pass

                elif line.split()[0] == "#define" and line.split()[1] == "GS_COMMAND_PORT_NUMBER":
                    newValue = self.portComboBoxIndicesPortNum[self.ui.GSCommandPort.currentText()]
                    line = line.replace(line.split()[2], newValue)

                    # attempt to change comment if possible
                    try:
                        line = line.replace(line.split()[3], "//" + self.ui.GSCommandPort.currentText())
                    except:
                        pass

                elif line.split()[0] == "#define" and line.split()[1] == "GS_TELEM_PORT_NUMBER":
                    newValue = self.portComboBoxIndicesPortNum[self.ui.GSTelemetryPort.currentText()]
                    line = line.replace(line.split()[2], newValue)

                    # attempt to change comment if possible
                    try:
                        line = line.replace(line.split()[3], "//" + self.ui.GSTelemetryPort.currentText())
                    except:
                        pass

            newContents.append(line)

        newContents = "".join(newContents)

        with open(self.GSSerialFilename, 'w') as outFile:
            outFile.write(newContents)

    def writeOutIFCTools(self):
        with open(self.IFCToolsFilename, 'r') as inFile:
            contents = inFile.readlines()

        newContents = []

        for line in contents:
            if len(line.split()) >= 3:
                if line.split()[0] == "#define" and line.split()[1] == "LIDAR_FIXED_MOUNT":
                    newValue = self.comboBoxIndicesBool[self.ui.LiDARFixedMount.currentText()]
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "PITOT_PIN":
                    newValue = self.comboBoxIndicesAnalogPin[self.ui.PitotTubeAnalogPin.currentText()]
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "THROTTLE_PIN":
                    newValue = self.comboBoxIndicesServoPin[self.ui.ThrottleAnalogPin.currentText()]
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "R_AILERON_PIN":
                    newValue = self.comboBoxIndicesServoPin[self.ui.RightAileronPin.currentText()]
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "L_AILERON_PIN":
                    newValue = self.comboBoxIndicesServoPin[self.ui.LeftAileronPin.currentText()]
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "ELEVATOR_PIN":
                    newValue = self.comboBoxIndicesServoPin[self.ui.ElevatorPin.currentText()]
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "RUDDER_PIN":
                    newValue = self.comboBoxIndicesServoPin[self.ui.RudderPin.currentText()]
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "UNSAFE_ROLL_R":
                    newValue = self.ui.UnsafeRollRightAngle.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "UNSAFE_ROLL_L":
                    newValue = self.ui.UnsafeRollLeftAngle.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "MAX_ROLL_R":
                    newValue = self.ui.MaxRollRightAngle.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "MAX_ROLL_L":
                    newValue = self.ui.MaxRollLeftAngle.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "UNSAFE_PITCH_UP":
                    newValue = self.ui.UnsafePitchUpAngle.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "UNSAFE_PITCH_DOWN":
                    newValue = self.ui.UnsafePitchDownAngle.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "MAX_PITCH_UP":
                    newValue = self.ui.MaxPitchUpAngle.text()
                    line = line.replace(line.split()[2], newValue)

                elif line.split()[0] == "#define" and line.split()[1] == "MAX_PITCH_DOWN":
                    newValue = self.ui.MaxPitchDownAngle.text()
                    line = line.replace(line.split()[2], newValue)

            newContents.append(line)

        newContents = "".join(newContents)

        with open(self.IFCToolsFilename, 'w') as outFile:
            outFile.write(newContents)

    def writeOutIFCSerial(self):
        with open(self.IFCSerialFilename, 'r') as inFile:
            contents = inFile.readlines()

        newContents = []

        for line in contents:
            if len(line.split()) >= 3:
                if line.split()[0] == "#define" and line.split()[1] == "IFC_DEBUG_PORT_NUMBER":
                    newValue = self.portComboBoxIndicesPortNum[self.ui.IFCDebugPort.currentText()]
                    line = line.replace(line.split()[2], newValue)

                    # attempt to change comment if possible
                    try:
                        line = line.replace(line.split()[3], "//" + self.ui.IFCDebugPort.currentText())
                    except:
                        pass

                elif line.split()[0] == "#define" and line.split()[1] == "IFC_COMMAND_PORT_NUMBER":
                    newValue = self.portComboBoxIndicesPortNum[self.ui.IFCCommandPort.currentText()]
                    line = line.replace(line.split()[2], newValue)

                    # attempt to change comment if possible
                    try:
                        line = line.replace(line.split()[3], "//" + self.ui.IFCCommandPort.currentText())
                    except:
                        pass

                elif line.split()[0] == "#define" and line.split()[1] == "IFC_GPS_PORT_NUMBER":
                    newValue = self.portComboBoxIndicesPortNum[self.ui.IFCGPSPort.currentText()]
                    line = line.replace(line.split()[2], newValue)

                    # attempt to change comment if possible
                    try:
                        line = line.replace(line.split()[3], "//" + self.ui.IFCGPSPort.currentText())
                    except:
                        pass

                elif line.split()[0] == "#define" and line.split()[1] == "IFC_TELEM_PORT_NUMBER":
                    newValue = self.portComboBoxIndicesPortNum[self.ui.IFCTelemetryPort.currentText()]
                    line = line.replace(line.split()[2], newValue)

                    # attempt to change comment if possible
                    try:
                        line = line.replace(line.split()[3], "//" + self.ui.IFCTelemetryPort.currentText())
                    except:
                        pass

            newContents.append(line)

        newContents = "".join(newContents)

        with open(self.IFCSerialFilename, 'w') as outFile:
            outFile.write(newContents)

    def close(self):
        exit()


def main():
    app = QApplication(sys.argv)
    w = AppWindow()
    w.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
