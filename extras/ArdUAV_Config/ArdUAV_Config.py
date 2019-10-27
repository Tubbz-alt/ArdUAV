"""

"""

import os
import sys
import pprint
import json
from PyQt5.QtWidgets import QApplication, QDialog, QDialogButtonBox, QFileDialog
from configGUI import Ui_Dialog


def reverse_dict(ref_dict):
    return_dict = {}
    
    for key in ref_dict.keys():
        return_dict[str(ref_dict[key])] = key
    
    return return_dict


boolRep_to_index = {'1':     0,
                    '0':     1,
                    'true':  0,
                    'false': 1}
index_to_boolRep = reverse_dict(boolRep_to_index)

boolStr_to_digStr = {'True':  '1',
                     'False': '0'}
digStr_to_boolStr = reverse_dict(boolStr_to_digStr)

dNum_to_index = {'2':  0,
                 '39': 1,
                 '6':  2,
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
index_to_dNum = reverse_dict(dNum_to_index)

sNum_to_index = {'s0':  0,
                 's1':  1,
                 's2':  2,
                 's3':  3,
                 's4':  4,
                 's5':  5,
                 's6':  6,
                 's7':  7,
                 's8':  8,
                 's9':  9,
                 's10': 10,
                 's11': 11,
                 's12': 12,
                 's13': 13,
                 's14': 14,
                 's15': 15}
index_to_sNum = reverse_dict(sNum_to_index)

sNum_to_dNum = {'s0':  '2',
                's1':  '39',
                's2':  '16',
                's3':  '24',
                's4':  '25',
                's5':  '26',
                's6':  '27',
                's7':  '28',
                's8':  '29',
                's9':  '30',
                's10': '23',
                's11': '22',
                's12': '17',
                's13': '16',
                's14': '14',
                's15': '36'}
dNum_to_sNum = reverse_dict(sNum_to_dNum)

aNum_to_index = {'A20': 0,
                 'A9':  1,
                 'A8':  2,
                 'A3':  3,
                 'A2':  4,
                 'A0':  5,
                 'A17': 6}
index_to_aNum = reverse_dict(aNum_to_index)

sNum_to_aNum = {'s1':  'A20',
                's10': 'A9',
                's11': 'A8',
                's12': 'A3',
                's13': 'A2',
                's14': 'A0',
                's15': 'A17'}
aNum_to_sNum = reverse_dict(sNum_to_aNum)

baudNum_to_baudIndex = {'300':     0,
                        '1200':    1,
                        '2400':    2,
                        '4800':    3,
                        '9600':    4,
                        '19200':   5,
                        '38400':   6,
                        '57600':   7,
                        '115200':  8,
                        '230400':  9,
                        '250000':  10,
                        '460800':  11,
                        '500000':  12,
                        '921600':  13,
                        '1000000': 14,
                        '2000000': 15}
baudIndex_to_baudNum = reverse_dict(baudNum_to_baudIndex)

portName_to_portNum = {'USB':     '0',
                        'Serial1': '1',
                        'Serial2': '2',
                        'Serial3': '3',
                        'Serial4': '4',
                        'Serial5': '5'}
portName_to_portIndex = reverse_dict(portName_to_portNum)


class AppWindow(QDialog):
    def __init__(self):
        super().__init__()
        self.ui = Ui_Dialog()
        self.ui.setupUi(self)
        self.show()

        self.setup_signals()

        self.BASE_LIBRARY_PATH = os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))

        if not os.path.exists("configs"):
            os.makedirs("configs")

        self.sharedToolsFilename = os.path.join(self.BASE_LIBRARY_PATH, "Shared_Tools.h")
        self.GSToolsFilename     = os.path.join(self.BASE_LIBRARY_PATH, "GS_Tools.h")
        self.GSSerialFilename    = os.path.join(self.BASE_LIBRARY_PATH, "GS_Serial.h")
        self.IFCToolsFilename    = os.path.join(self.BASE_LIBRARY_PATH, "IFC_Tools.h")
        self.IFCSerialFilename   = os.path.join(self.BASE_LIBRARY_PATH, "IFC_Serial.h")

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
                                                                   'MaxThrottleADCValue': "",
                                                                   'ThrottleReverse': ""},
                                              'ElevatorSettings': {'ElevatorAnalogPin': "",
                                                                   'MinElevatorADCValue': "",
                                                                   'MaxElevatorADCValue': "",
                                                                   'MinElevatorServoValue': "",
                                                                   'MaxElevatorServoValue': "",
                                                                   'ElevatorReverse': ""},
                                              'AileronSettings': {'AileronAnalogPin': "",
                                                                  'MinAileronADCValue': "",
                                                                  'MaxAileronADCValue': "",
                                                                  'MinAileronServoValue': "",
                                                                  'MaxAileronServoValue': "",
                                                                  'AileronReverse': ""},
                                              'RudderSettings': {'RudderAnalogPin': "",
                                                                 'MinRudderADCValue': "",
                                                                 'MaxRudderADCValue': "",
                                                                 'MinRudderServoValue': "",
                                                                 'MaxRudderServoValue': "",
                                                                 'RudderReverse': ""}},
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
        self.get_parameters()

    def setup_signals(self):
        self.ui.GetCurrentValues.clicked.connect(self.get_parameters)
        self.ui.buttonBox.clicked.connect(self.handle_button_click)
        self.ui.load_config.clicked.connect(self.ingest_config)
        self.ui.save_config.clicked.connect(self.write_out_config)
        
        # Shared Settings
        self.ui.Debug_Port_Baud.currentIndexChanged.connect(self.update_param_dict)
        self.ui.Command_Port_Baud.currentIndexChanged.connect(self.update_param_dict)
        self.ui.GPS_Port_Baud.currentIndexChanged.connect(self.update_param_dict)
        self.ui.Telemetry_Port_Baud.currentIndexChanged.connect(self.update_param_dict)
        self.ui.CommandReportingRate.valueChanged.connect(self.update_param_dict)
        self.ui.TelemetryReportingRate.valueChanged.connect(self.update_param_dict)
        self.ui.LossLinkTimeout.valueChanged.connect(self.update_param_dict)
        self.ui.MaxThrottleValue.valueChanged.connect(self.update_param_dict)
        self.ui.MinThrottleValue.valueChanged.connect(self.update_param_dict)
        self.ui.MaxElevatorValue.valueChanged.connect(self.update_param_dict)
        self.ui.MinElevatorValue.valueChanged.connect(self.update_param_dict)
        self.ui.MaxAileronValue.valueChanged.connect(self.update_param_dict)
        self.ui.MinAileronValue.valueChanged.connect(self.update_param_dict)
        self.ui.MaxRudderValue.valueChanged.connect(self.update_param_dict)
        self.ui.MinRudderValue.valueChanged.connect(self.update_param_dict)
        
        # GS Settings
        self.ui.GSDebugPort.currentIndexChanged.connect(self.update_param_dict)
        self.ui.GSCommandPort.currentIndexChanged.connect(self.update_param_dict)
        self.ui.GSTelemetryPort.currentIndexChanged.connect(self.update_param_dict)
        self.ui.ThrottleAnalogPin.currentIndexChanged.connect(self.update_param_dict)
        self.ui.MinThrottleADCValue.valueChanged.connect(self.update_param_dict)
        self.ui.MaxThrottleADCValue.valueChanged.connect(self.update_param_dict)
        self.ui.ElevatorAnalogPin.currentIndexChanged.connect(self.update_param_dict)
        self.ui.MinElevatorADCValue.valueChanged.connect(self.update_param_dict)
        self.ui.MaxElevatorADCValue.valueChanged.connect(self.update_param_dict)
        self.ui.MinElevatorServoValue.valueChanged.connect(self.update_param_dict)
        self.ui.MaxElevatorServoValue.valueChanged.connect(self.update_param_dict)
        self.ui.AileronAnalogPin.currentIndexChanged.connect(self.update_param_dict)
        self.ui.MinAileronADCValue.valueChanged.connect(self.update_param_dict)
        self.ui.MinAileronServoValue.valueChanged.connect(self.update_param_dict)
        self.ui.MaxAileronServoValue.valueChanged.connect(self.update_param_dict)
        self.ui.RudderAnalogPin.currentIndexChanged.connect(self.update_param_dict)
        self.ui.MinRudderADCValue.valueChanged.connect(self.update_param_dict)
        self.ui.MaxRudderADCValue.valueChanged.connect(self.update_param_dict)
        self.ui.MinRudderServoValue.valueChanged.connect(self.update_param_dict)
        self.ui.MaxRudderServoValue.valueChanged.connect(self.update_param_dict)
        self.ui.ThrottleReverse.currentIndexChanged.connect(self.update_param_dict)
        self.ui.ElevatorReverse.currentIndexChanged.connect(self.update_param_dict)
        self.ui.AileronReverse.currentIndexChanged.connect(self.update_param_dict)
        self.ui.RudderReverse.currentIndexChanged.connect(self.update_param_dict)
        
        # IFC Settings
        self.ui.IFCDebugPort.currentIndexChanged.connect(self.update_param_dict)
        self.ui.IFCCommandPort.currentIndexChanged.connect(self.update_param_dict)
        self.ui.IFCGPSPort.currentIndexChanged.connect(self.update_param_dict)
        self.ui.IFCTelemetryPort.currentIndexChanged.connect(self.update_param_dict)
        self.ui.ThrottlePin.currentIndexChanged.connect(self.update_param_dict)
        self.ui.ElevatorPin.currentIndexChanged.connect(self.update_param_dict)
        self.ui.RightAileronPin.currentIndexChanged.connect(self.update_param_dict)
        self.ui.LeftAileronPin.currentIndexChanged.connect(self.update_param_dict)
        self.ui.RudderPin.currentIndexChanged.connect(self.update_param_dict)
        self.ui.UnsafeRollRightAngle.valueChanged.connect(self.update_param_dict)
        self.ui.UnsafeRollLeftAngle.valueChanged.connect(self.update_param_dict)
        self.ui.MaxRollRightAngle.valueChanged.connect(self.update_param_dict)
        self.ui.MaxRollLeftAngle.valueChanged.connect(self.update_param_dict)
        self.ui.UnsafePitchUpAngle.valueChanged.connect(self.update_param_dict)
        self.ui.UnsafePitchDownAngle.valueChanged.connect(self.update_param_dict)
        self.ui.MaxPitchUpAngle.valueChanged.connect(self.update_param_dict)
        self.ui.MaxPitchDownAngle.valueChanged.connect(self.update_param_dict)
        self.ui.PitotTubeAnalogPin.currentIndexChanged.connect(self.update_param_dict)
        self.ui.LiDARFixedMount.currentIndexChanged.connect(self.update_param_dict)

    def handle_button_click(self, button):
        sb = self.ui.buttonBox.standardButton(button)
        if sb == QDialogButtonBox.Apply:
            self.write_out_parameters()

        elif sb == QDialogButtonBox.Cancel:
            self.close()

    def ingest_config(self):
        openfile = QFileDialog.getOpenFileName(self, directory="configs", filter="*.json")[0]

        try:
            if openfile:
                with open(openfile, "r") as _file:
                    self.currentParameters = json.loads(_file.read().replace("'", '"'))

                self.update_gui()

                print("--------------------------------")
                print("Loaded config file: {}".format(os.path.basename(openfile)))
        except:
            import traceback
            traceback.print_exc()
    
    def update_param_dict(self):
        # Shared Settings
        self.currentParameters['SharedTools']['SerialSettings']['Debug_Port_Baud']     = self.ui.Debug_Port_Baud.currentText()
        self.currentParameters['SharedTools']['SerialSettings']['Command_Port_Baud']   = self.ui.Command_Port_Baud.currentText()
        self.currentParameters['SharedTools']['SerialSettings']['GPS_Port_Baud']       = self.ui.GPS_Port_Baud.currentText()
        self.currentParameters['SharedTools']['SerialSettings']['Telemetry_Port_Baud'] = self.ui.Telemetry_Port_Baud.currentText()
        self.currentParameters['SharedTools']['Reporting_TimeoutSettings']['CommandReportingRate']   = self.ui.CommandReportingRate.text()
        self.currentParameters['SharedTools']['Reporting_TimeoutSettings']['TelemetryReportingRate'] = self.ui.TelemetryReportingRate.text()
        self.currentParameters['SharedTools']['Reporting_TimeoutSettings']['LossLinkTimeout']        = self.ui.LossLinkTimeout.text()
        self.currentParameters['SharedTools']['ThrottleSettings']['MaxThrottleValue']  = self.ui.MaxThrottleValue.text()
        self.currentParameters['SharedTools']['AileronSettings']['MaxAileronValue']    = self.ui.MaxAileronValue.text()
        self.currentParameters['SharedTools']['ElevatorSettings']['MaxElevatorValue']  = self.ui.MaxElevatorValue.text()
        self.currentParameters['SharedTools']['RudderSettings']['MaxRudderValue']      = self.ui.MaxRudderValue.text()
        self.currentParameters['SharedTools']['ThrottleSettings']['MinThrottleValue']  = self.ui.MinThrottleValue.text()
        self.currentParameters['SharedTools']['AileronSettings']['MinAileronValue']    = self.ui.MinAileronValue.text()
        self.currentParameters['SharedTools']['ElevatorSettings']['MinElevatorValue']  = self.ui.MinElevatorValue.text()
        self.currentParameters['SharedTools']['RudderSettings']['MinRudderValue']      = self.ui.MinRudderValue.text()

        #GS Settings
        self.currentParameters['GSTools']['SerialSettings']['GS_DebugPortNumber']      = portName_to_portNum[self.ui.GSDebugPort.currentText()]
        self.currentParameters['GSTools']['SerialSettings']['GS_CommandPortNumber']    = portName_to_portNum[self.ui.GSCommandPort.currentText()]
        self.currentParameters['GSTools']['SerialSettings']['GS_TelemetryPortNumber']  = portName_to_portNum[self.ui.GSTelemetryPort.currentText()]
        self.currentParameters['GSTools']['RudderSettings']['RudderAnalogPin']         = sNum_to_aNum[self.ui.RudderAnalogPin.currentText()]
        self.currentParameters['GSTools']['ThrottleSettings']['ThrottleAnalogPin']     = sNum_to_aNum[self.ui.ThrottleAnalogPin.currentText()]
        self.currentParameters['GSTools']['AileronSettings']['AileronAnalogPin']       = sNum_to_aNum[self.ui.AileronAnalogPin.currentText()]
        self.currentParameters['GSTools']['ElevatorSettings']['ElevatorAnalogPin']     = sNum_to_aNum[self.ui.ElevatorAnalogPin.currentText()]
        self.currentParameters['GSTools']['AileronSettings']['MaxAileronServoValue']   = self.ui.MaxAileronServoValue.text()
        self.currentParameters['GSTools']['ElevatorSettings']['MaxElevatorServoValue'] = self.ui.MaxElevatorServoValue.text()
        self.currentParameters['GSTools']['RudderSettings']['MaxRudderServoValue']     = self.ui.MaxRudderServoValue.text()
        self.currentParameters['GSTools']['AileronSettings']['MinAileronServoValue']   = self.ui.MinAileronServoValue.text()
        self.currentParameters['GSTools']['ElevatorSettings']['MinElevatorServoValue'] = self.ui.MinElevatorServoValue.text()
        self.currentParameters['GSTools']['RudderSettings']['MinRudderServoValue']     = self.ui.MinRudderServoValue.text()
        self.currentParameters['GSTools']['ThrottleSettings']['MinThrottleADCValue']   = self.ui.MinThrottleADCValue.text()
        self.currentParameters['GSTools']['AileronSettings']['MinAileronADCValue']     = self.ui.MinAileronADCValue.text()
        self.currentParameters['GSTools']['ElevatorSettings']['MinElevatorADCValue']   = self.ui.MinElevatorADCValue.text()
        self.currentParameters['GSTools']['RudderSettings']['MinRudderADCValue']       = self.ui.MinRudderADCValue.text()
        self.currentParameters['GSTools']['ThrottleSettings']['MaxThrottleADCValue']   = self.ui.MaxThrottleADCValue.text()
        self.currentParameters['GSTools']['AileronSettings']['MaxAileronADCValue']     = self.ui.MaxAileronADCValue.text()
        self.currentParameters['GSTools']['ElevatorSettings']['MaxElevatorADCValue']   = self.ui.MaxElevatorADCValue.text()
        self.currentParameters['GSTools']['RudderSettings']['MaxRudderADCValue']       = self.ui.MaxRudderADCValue.text()
        self.currentParameters['GSTools']['AileronSettings']['AileronReverse']         = boolStr_to_digStr[self.ui.AileronReverse.currentText()]
        self.currentParameters['GSTools']['ElevatorSettings']['ElevatorReverse']       = boolStr_to_digStr[self.ui.ElevatorReverse.currentText()]
        self.currentParameters['GSTools']['RudderSettings']['RudderReverse']           = boolStr_to_digStr[self.ui.RudderReverse.currentText()]
        self.currentParameters['GSTools']['ThrottleSettings']['ThrottleReverse']       = boolStr_to_digStr[self.ui.ThrottleReverse.currentText()]

        #IFC Settings
        self.currentParameters['IFCTools']['SerialSettings']['IFC_DebugPortNumber']     = portName_to_portNum[self.ui.IFCDebugPort.currentText()]
        self.currentParameters['IFCTools']['SerialSettings']['IFC_CommandPortNumber']   = portName_to_portNum[self.ui.IFCCommandPort.currentText()]
        self.currentParameters['IFCTools']['SerialSettings']['IFC_GPSPortNumber']       = portName_to_portNum[self.ui.IFCGPSPort.currentText()]
        self.currentParameters['IFCTools']['SerialSettings']['IFC_TelemetryPortNumber'] = portName_to_portNum[self.ui.IFCTelemetryPort.currentText()]
        self.currentParameters['IFCTools']['ControlSurfaceSettings']['ThrottlePin']     = sNum_to_dNum[self.ui.ThrottlePin.currentText()]
        self.currentParameters['IFCTools']['ControlSurfaceSettings']['ElevatorPin']     = sNum_to_dNum[self.ui.ElevatorPin.currentText()]
        self.currentParameters['IFCTools']['ControlSurfaceSettings']['RightAileronPin'] = sNum_to_dNum[self.ui.RightAileronPin.currentText()]
        self.currentParameters['IFCTools']['ControlSurfaceSettings']['LeftAileronPin']  = sNum_to_dNum[self.ui.LeftAileronPin.currentText()]
        self.currentParameters['IFCTools']['ControlSurfaceSettings']['RudderPin']       = sNum_to_dNum[self.ui.RudderPin.currentText()]
        self.currentParameters['IFCTools']['AutopilotSettings']['UnsafeRollRightAngle'] = self.ui.UnsafeRollRightAngle.text()
        self.currentParameters['IFCTools']['AutopilotSettings']['UnsafeRollLeftAngle']  = self.ui.UnsafeRollLeftAngle.text()
        self.currentParameters['IFCTools']['AutopilotSettings']['MaxRollRightAngle']    = self.ui.MaxRollRightAngle.text()
        self.currentParameters['IFCTools']['AutopilotSettings']['MaxRollLeftAngle']     = self.ui.MaxRollLeftAngle.text()
        self.currentParameters['IFCTools']['AutopilotSettings']['UnsafePitchUpAngle']   = self.ui.UnsafePitchUpAngle.text()
        self.currentParameters['IFCTools']['AutopilotSettings']['UnsafePitchDownAngle'] = self.ui.UnsafePitchDownAngle.text()
        self.currentParameters['IFCTools']['AutopilotSettings']['MaxPitchUpAngle']      = self.ui.MaxPitchUpAngle.text()
        self.currentParameters['IFCTools']['AutopilotSettings']['MaxPitchDownAngle']    = self.ui.MaxPitchDownAngle.text()
        self.currentParameters['IFCTools']['OtherSettings']['PitotTubeAnalogPin']       = sNum_to_aNum[self.ui.PitotTubeAnalogPin.currentText()]
        self.currentParameters['IFCTools']['OtherSettings']['LiDARFixedMount']          = boolStr_to_digStr[self.ui.LiDARFixedMount.currentText()]

    def write_out_config(self):
        contents = pprint.pformat(self.currentParameters)

        config_name = os.path.join("configs", "ArdUAV_config_v{}.json")
        version = 1

        while os.path.exists(config_name.format(version)):
            version += 1

        final_name = config_name.format(version)

        with open(final_name, "w") as config:
            config.write(contents)

        print("--------------------------------")
        print("Config saved as {}".format(final_name))

    def get_parameters(self):
        try:
            self.read_shared_tools()
            self.read_gs_tools()
            self.read_gs_serial()
            self.read_ifc_tools()
            self.read_ifc_serial()

        except:
            import traceback
            traceback.print_exc()

    def read_shared_tools(self):
        with open(self.sharedToolsFilename, 'r') as inFile:
            contents = inFile.readlines()

        for line in contents:
            if len(line.split()) >= 3:
                define       = line.split()[0]
                setting_name = line.split()[1]
                setting_val  = line.split()[2]
                
                if define == "#define":
                    if setting_name == "DEBUG_PORT_BAUD":
                        self.ui.Debug_Port_Baud.setCurrentIndex(baudNum_to_baudIndex[setting_val])
    
                    elif setting_name == "COMMAND_PORT_BAUD":
                        self.ui.Command_Port_Baud.setCurrentIndex(baudNum_to_baudIndex[setting_val])
    
                    elif setting_name == "GPS_PORT_BAUD":
                        self.ui.GPS_Port_Baud.setCurrentIndex(baudNum_to_baudIndex[setting_val])
    
                    elif setting_name == "TELEM_PORT_BAUD":
                        self.ui.Telemetry_Port_Baud.setCurrentIndex(baudNum_to_baudIndex[setting_val])
    
                    elif setting_name == "REPORT_COMMANDS_FREQ":
                        self.ui.CommandReportingRate.setValue(float(setting_val))
    
                    elif setting_name == "REPORT_TELEM_FREQ":
                        self.ui.TelemetryReportingRate.setValue(float(setting_val))
    
                    elif setting_name == "LOSS_LINK_TIMEOUT":
                        self.ui.LossLinkTimeout.setValue(int(setting_val))
    
                    elif setting_name == "THROTTLE_MAX":
                        self.ui.MaxThrottleValue.setValue(int(setting_val))
    
                    elif setting_name == "AILERON_MAX":
                        self.ui.MinThrottleValue.setValue(int(setting_val))
    
                    elif setting_name == "ELEVATOR_MAX":
                        self.ui.MaxElevatorValue.setValue(int(setting_val))
    
                    elif setting_name == "RUDDER_MAX":
                        self.ui.MinElevatorValue.setValue(int(setting_val))
    
                    elif setting_name == "THROTTLE_MIN":
                        self.ui.MaxAileronValue.setValue(int(setting_val))
    
                    elif setting_name == "AILERON_MIN":
                        self.ui.MinAileronValue.setValue(int(setting_val))
    
                    elif setting_name == "ELEVATOR_MIN":
                        self.ui.MaxRudderValue.setValue(int(setting_val))
    
                    elif setting_name == "RUDDER_MIN":
                        self.ui.MinRudderValue.setValue(int(setting_val))

    def read_gs_tools(self):
        with open(self.GSToolsFilename, 'r') as inFile:
            contents = inFile.readlines()

        for line in contents:
            if len(line.split()) >= 3:
                define       = line.split()[0]
                setting_name = line.split()[1]
                setting_val  = line.split()[2]
                
                if define == "#define":
                    if setting_name == "YAW_ANALOG_PIN":
                        self.ui.RudderAnalogPin.setCurrentIndex(aNum_to_index[setting_val])
    
                    elif setting_name == "THROTTLE_ANALOG_PIN":
                        self.ui.ThrottleAnalogPin.setCurrentIndex(aNum_to_index[setting_val])
    
                    elif setting_name == "ROLL_ANALOG_PIN":
                        self.ui.AileronAnalogPin.setCurrentIndex(aNum_to_index[setting_val])
    
                    elif setting_name == "PITCH_ANALOG_PIN":
                        self.ui.ElevatorAnalogPin.setCurrentIndex(aNum_to_index[setting_val])
    
                    elif setting_name == "AILERON_MAX_LOWRATES":
                        self.ui.MaxAileronServoValue.setValue(int(setting_val))
    
                    elif setting_name == "ELEVATOR_MAX_LOWRATES":
                        self.ui.MaxElevatorServoValue.setValue(int(setting_val))
    
                    elif setting_name == "RUDDER_MAX_LOWRATES":
                        self.ui.MaxRudderServoValue.setValue(int(setting_val))
    
                    elif setting_name == "AILERON_MIN_LOWRATES":
                        self.ui.MinAileronServoValue.setValue(int(setting_val))
    
                    elif setting_name == "ELEVATOR_MIN_LOWRATES":
                        self.ui.MinElevatorServoValue.setValue(int(setting_val))
    
                    elif setting_name == "RUDDER_MIN_LOWRATES":
                        self.ui.MinRudderServoValue.setValue(int(setting_val))
    
                    elif setting_name == "THROTTLE_MIN_ADC":
                        self.ui.MinThrottleADCValue.setValue(int(setting_val))
    
                    elif setting_name == "AILERON_MIN_ADC":
                        self.ui.MinAileronADCValue.setValue(int(setting_val))
    
                    elif setting_name == "ELEVATOR_MIN_ADC":
                        self.ui.MinElevatorADCValue.setValue(int(setting_val))
    
                    elif setting_name == "RUDDER_MIN_ADC":
                        self.ui.MinRudderADCValue.setValue(int(setting_val))
    
                    elif setting_name == "THROTTLE_MAX_ADC":
                        self.ui.MaxThrottleADCValue.setValue(int(setting_val))
    
                    elif setting_name == "AILERON_MAX_ADC":
                        self.ui.MaxAileronADCValue.setValue(int(setting_val))
    
                    elif setting_name == "ELEVATOR_MAX_ADC":
                        self.ui.MaxElevatorADCValue.setValue(int(setting_val))
    
                    elif setting_name == "RUDDER_MAX_ADC":
                        self.ui.MaxRudderADCValue.setValue(int(setting_val))
    
                    elif setting_name == "AILERON_REVERSE":
                        self.ui.AileronReverse.setCurrentIndex(boolRep_to_index[setting_val])
    
                    elif setting_name == "ELEVATOR_REVERSE":
                        self.ui.ElevatorReverse.setCurrentIndex(boolRep_to_index[setting_val])
    
                    elif setting_name == "RUDDER_REVERSE":
                        self.ui.RudderReverse.setCurrentIndex(boolRep_to_index[setting_val])
    
                    elif setting_name == "THROTTLE_REVERSE":
                        self.ui.ThrottleReverse.setCurrentIndex(boolRep_to_index[setting_val])

    def read_gs_serial(self):
        with open(self.GSSerialFilename, 'r') as inFile:
            contents = inFile.readlines()

        for line in contents:
            if len(line.split()) >= 3:
                define       = line.split()[0]
                setting_name = line.split()[1]
                setting_val  = line.split()[2]
                
                if define == "#define":
                    if setting_name == "GS_DEBUG_PORT_NUMBER":
                        self.ui.GSDebugPort.setCurrentIndex(int(setting_val))
    
                    elif setting_name == "GS_COMMAND_PORT_NUMBER":
                        self.ui.GSCommandPort.setCurrentIndex(int(setting_val))
    
                    elif setting_name == "GS_TELEM_PORT_NUMBER":
                        self.ui.GSTelemetryPort.setCurrentIndex(int(setting_val))

    def read_ifc_tools(self):
        with open(self.IFCToolsFilename, 'r') as inFile:
            contents = inFile.readlines()

        for line in contents:
            if len(line.split()) >= 3:
                define       = line.split()[0]
                setting_name = line.split()[1]
                setting_val  = line.split()[2]
                
                if define == "#define":
                    if setting_name == "LIDAR_FIXED_MOUNT":
                        self.ui.LiDARFixedMount.setCurrentIndex(boolRep_to_index[setting_val])
    
                    elif setting_name == "PITOT_PIN":
                        self.ui.PitotTubeAnalogPin.setCurrentIndex(aNum_to_index[setting_val])
    
                    elif setting_name == "THROTTLE_PIN":
                        self.ui.ThrottlePin.setCurrentIndex(dNum_to_index[setting_val])
    
                    elif setting_name == "R_AILERON_PIN":
                        self.ui.RightAileronPin.setCurrentIndex(dNum_to_index[setting_val])
    
                    elif setting_name == "L_AILERON_PIN":
                        self.ui.LeftAileronPin.setCurrentIndex(dNum_to_index[setting_val])
    
                    elif setting_name == "ELEVATOR_PIN":
                        self.ui.ElevatorPin.setCurrentIndex(dNum_to_index[setting_val])
    
                    elif setting_name == "RUDDER_PIN":
                        self.ui.RudderPin.setCurrentIndex(dNum_to_index[setting_val])
    
                    elif setting_name == "UNSAFE_ROLL_R":
                        self.ui.UnsafeRollRightAngle.setValue(int(setting_val))
    
                    elif setting_name == "UNSAFE_ROLL_L":
                        self.ui.UnsafeRollLeftAngle.setValue(int(setting_val))
    
                    elif setting_name == "MAX_ROLL_R":
                        self.ui.MaxRollRightAngle.setValue(int(setting_val))
    
                    elif setting_name == "MAX_ROLL_L":
                        self.ui.MaxRollLeftAngle.setValue(int(setting_val))
    
                    elif setting_name == "UNSAFE_PITCH_UP":
                        self.ui.UnsafePitchUpAngle.setValue(int(setting_val))
    
                    elif setting_name == "UNSAFE_PITCH_DOWN":
                        self.ui.UnsafePitchDownAngle.setValue(int(setting_val))
    
                    elif setting_name == "MAX_PITCH_UP":
                        self.ui.MaxPitchUpAngle.setValue(int(setting_val))
    
                    elif setting_name == "MAX_PITCH_DOWN":
                        self.ui.MaxPitchDownAngle.setValue(int(setting_val))

    def read_ifc_serial(self):
        with open(self.IFCSerialFilename, 'r') as inFile:
            contents = inFile.readlines()

        for line in contents:
            if len(line.split()) >= 3:
                define       = line.split()[0]
                setting_name = line.split()[1]
                setting_val  = line.split()[2]
                
                if define == "#define":
                    if setting_name == "IFC_DEBUG_PORT_NUMBER":
                        self.ui.IFCDebugPort.setCurrentIndex(int(setting_val))
    
                    elif setting_name == "IFC_COMMAND_PORT_NUMBER":
                        self.ui.IFCCommandPort.setCurrentIndex(int(setting_val))
    
                    elif setting_name == "IFC_GPS_PORT_NUMBER":
                        self.ui.IFCGPSPort.setCurrentIndex(int(setting_val))
    
                    elif setting_name == "IFC_TELEM_PORT_NUMBER":
                        self.ui.IFCTelemetryPort.setCurrentIndex(int(setting_val))

    def update_gui(self):
        self.update_gui_shared_tools()
        self.update_gui_gs_tools()
        self.update_gui_ifc_tools()

    def update_gui_shared_tools(self):
        self.value = self.currentParameters['SharedTools']['SerialSettings']['Debug_Port_Baud']
        self.ui.Debug_Port_Baud.setCurrentIndex(baudNum_to_baudIndex[self.value])

        self.value = self.currentParameters['SharedTools']['SerialSettings']['Command_Port_Baud']
        self.ui.Command_Port_Baud.setCurrentIndex(baudNum_to_baudIndex[self.value])

        self.value = self.currentParameters['SharedTools']['SerialSettings']['GPS_Port_Baud']
        self.ui.GPS_Port_Baud.setCurrentIndex(baudNum_to_baudIndex[self.value])

        self.value = self.currentParameters['SharedTools']['SerialSettings']['Telemetry_Port_Baud']
        self.ui.Telemetry_Port_Baud.setCurrentIndex(baudNum_to_baudIndex[self.value])

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

    def update_gui_gs_tools(self):
        self.value = int(self.currentParameters['GSTools']['SerialSettings']['GS_DebugPortNumber'])
        self.ui.GSDebugPort.setCurrentIndex(self.value)

        self.value = int(self.currentParameters['GSTools']['SerialSettings']['GS_CommandPortNumber'])
        self.ui.GSCommandPort.setCurrentIndex(self.value)

        self.value = int(self.currentParameters['GSTools']['SerialSettings']['GS_TelemetryPortNumber'])
        self.ui.GSTelemetryPort.setCurrentIndex(self.value)

        self.value = self.currentParameters['GSTools']['ThrottleSettings']['ThrottleAnalogPin']
        self.ui.ThrottleAnalogPin.setCurrentIndex(aNum_to_index[self.value])

        self.value = int(self.currentParameters['GSTools']['ThrottleSettings']['MinThrottleADCValue'])
        self.ui.MinThrottleADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['ThrottleSettings']['MaxThrottleADCValue'])
        self.ui.MaxThrottleADCValue.setValue(self.value)

        self.value = self.currentParameters['GSTools']['ElevatorSettings']['ElevatorAnalogPin']
        self.ui.ElevatorAnalogPin.setCurrentIndex(aNum_to_index[self.value])

        self.value = int(self.currentParameters['GSTools']['ElevatorSettings']['MinElevatorADCValue'])
        self.ui.MinElevatorADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['ElevatorSettings']['MaxElevatorADCValue'])
        self.ui.MaxElevatorADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['ElevatorSettings']['MinElevatorServoValue'])
        self.ui.MinElevatorServoValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['ElevatorSettings']['MaxElevatorServoValue'])
        self.ui.MaxElevatorServoValue.setValue(self.value)

        self.value = self.currentParameters['GSTools']['AileronSettings']['AileronAnalogPin']
        self.ui.AileronAnalogPin.setCurrentIndex(aNum_to_index[self.value])

        self.value = int(self.currentParameters['GSTools']['AileronSettings']['MinAileronADCValue'])
        self.ui.MinAileronADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['AileronSettings']['MaxAileronADCValue'])
        self.ui.MaxAileronADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['AileronSettings']['MinAileronServoValue'])
        self.ui.MinAileronServoValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['AileronSettings']['MaxAileronServoValue'])
        self.ui.MaxAileronServoValue.setValue(self.value)

        self.value = self.currentParameters['GSTools']['RudderSettings']['RudderAnalogPin']
        self.ui.RudderAnalogPin.setCurrentIndex(aNum_to_index[self.value])

        self.value = int(self.currentParameters['GSTools']['RudderSettings']['MinRudderADCValue'])
        self.ui.MinRudderADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['RudderSettings']['MaxRudderADCValue'])
        self.ui.MaxRudderADCValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['RudderSettings']['MinRudderServoValue'])
        self.ui.MinRudderServoValue.setValue(self.value)

        self.value = int(self.currentParameters['GSTools']['RudderSettings']['MaxRudderServoValue'])
        self.ui.MaxRudderServoValue.setValue(self.value)

        self.value = self.currentParameters['GSTools']['ThrottleSettings']['ThrottleReverse']
        self.ui.ThrottleReverse.setCurrentIndex(boolRep_to_index[self.value])

        self.value = self.currentParameters['GSTools']['ElevatorSettings']['ElevatorReverse']
        self.ui.ElevatorReverse.setCurrentIndex(boolRep_to_index[self.value])

        self.value = self.currentParameters['GSTools']['AileronSettings']['AileronReverse']
        self.ui.AileronReverse.setCurrentIndex(boolRep_to_index[self.value])

        self.value = self.currentParameters['GSTools']['RudderSettings']['RudderReverse']
        self.ui.RudderReverse.setCurrentIndex(boolRep_to_index[self.value])

    def update_gui_ifc_tools(self):
        self.value = int(self.currentParameters['IFCTools']['SerialSettings']['IFC_DebugPortNumber'])
        self.ui.IFCDebugPort.setCurrentIndex(self.value)

        self.value = int(self.currentParameters['IFCTools']['SerialSettings']['IFC_CommandPortNumber'])
        self.ui.IFCCommandPort.setCurrentIndex(self.value)

        self.value = int(self.currentParameters['IFCTools']['SerialSettings']['IFC_GPSPortNumber'])
        self.ui.IFCGPSPort.setCurrentIndex(self.value)

        self.value = int(self.currentParameters['IFCTools']['SerialSettings']['IFC_TelemetryPortNumber'])
        self.ui.IFCTelemetryPort.setCurrentIndex(self.value)

        self.value = self.currentParameters['IFCTools']['ControlSurfaceSettings']['ThrottlePin']
        self.ui.ThrottlePin.setCurrentIndex(dNum_to_index[self.value])

        self.value = self.currentParameters['IFCTools']['ControlSurfaceSettings']['ElevatorPin']
        self.ui.ElevatorPin.setCurrentIndex(dNum_to_index[self.value])

        self.value = self.currentParameters['IFCTools']['ControlSurfaceSettings']['RightAileronPin']
        self.ui.RightAileronPin.setCurrentIndex(dNum_to_index[self.value])

        self.value = self.currentParameters['IFCTools']['ControlSurfaceSettings']['LeftAileronPin']
        self.ui.LeftAileronPin.setCurrentIndex(dNum_to_index[self.value])

        self.value = self.currentParameters['IFCTools']['ControlSurfaceSettings']['RudderPin']
        self.ui.RudderPin.setCurrentIndex(dNum_to_index[self.value])

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
        self.ui.PitotTubeAnalogPin.setCurrentIndex(aNum_to_index[self.value])

        self.value = self.currentParameters['IFCTools']['OtherSettings']['LiDARFixedMount']
        self.ui.LiDARFixedMount.setCurrentIndex(boolRep_to_index[self.value])

    def write_out_parameters(self):
        try:
            self.write_out_shared_tools()
            self.write_out_gs_tools()
            self.write_out_gs_serial()
            self.write_out_ifc_tools()
            self.write_out_ifc_serial()

            print("--------------------------------")
            print("Library Updated")
        except:
            import traceback
            traceback.print_exc()

    def write_out_shared_tools(self):
        with open(self.sharedToolsFilename, 'r') as inFile:
            contents = inFile.readlines()

        new_contents = []

        for line in contents:
            if len(line.split()) >= 3:
                define       = line.split()[0]
                setting_name = line.split()[1]
                setting_val  = line.split()[2]
                
                if define == "#define":
                    if setting_name == "DEBUG_PORT_BAUD":
                        new_value = self.ui.Debug_Port_Baud.currentText()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "COMMAND_PORT_BAUD":
                        new_value = self.ui.Command_Port_Baud.currentText()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "GPS_PORT_BAUD":
                        new_value = self.ui.GPS_Port_Baud.currentText()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "TELEM_PORT_BAUD":
                        new_value = self.ui.Telemetry_Port_Baud.currentText()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "REPORT_COMMANDS_FREQ":
                        new_value = self.ui.CommandReportingRate.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "REPORT_TELEM_FREQ":
                        new_value = self.ui.TelemetryReportingRate.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "LOSS_LINK_TIMEOUT":
                        new_value = self.ui.LossLinkTimeout.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "THROTTLE_MAX":
                        new_value = self.ui.MaxThrottleValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "AILERON_MAX":
                        new_value = self.ui.MaxAileronValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "ELEVATOR_MAX":
                        new_value = self.ui.MaxElevatorValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "RUDDER_MAX":
                        new_value = self.ui.MaxRudderValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "THROTTLE_MIN":
                        new_value = self.ui.MinThrottleValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "AILERON_MIN":
                        new_value = self.ui.MinAileronValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "ELEVATOR_MIN":
                        new_value = self.ui.MinElevatorValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "RUDDER_MIN":
                        new_value = self.ui.MinRudderValue.text()
                        line = line.replace(setting_val, new_value)

            new_contents.append(line)

        new_contents = "".join(new_contents)

        with open(self.sharedToolsFilename, 'w') as outFile:
            outFile.write(new_contents)

    def write_out_gs_tools(self):
        with open(self.GSToolsFilename, 'r') as inFile:
            contents = inFile.readlines()

        new_contents = []

        for line in contents:
            if len(line.split()) >= 3:
                define       = line.split()[0]
                setting_name = line.split()[1]
                setting_val  = line.split()[2]
                
                if define == "#define":
                    if setting_name == "YAW_ANALOG_PIN":
                        new_value = self.comboBoxIndicesAnalogPin[self.ui.RudderAnalogPin.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "THROTTLE_ANALOG_PIN":
                        new_value = self.comboBoxIndicesAnalogPin[self.ui.ThrottleAnalogPin.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "ROLL_ANALOG_PIN":
                        new_value = self.comboBoxIndicesAnalogPin[self.ui.AileronAnalogPin.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "PITCH_ANALOG_PIN":
                        new_value = self.comboBoxIndicesAnalogPin[self.ui.ElevatorAnalogPin.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "AILERON_MAX_LOWRATES":
                        new_value = self.ui.MaxAileronServoValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "ELEVATOR_MAX_LOWRATES":
                        new_value = self.ui.MaxElevatorServoValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "RUDDER_MAX_LOWRATES":
                        new_value = self.ui.MaxRudderServoValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "AILERON_MIN_LOWRATES":
                        new_value = self.ui.MinAileronServoValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "ELEVATOR_MIN_LOWRATES":
                        new_value = self.ui.MinElevatorServoValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "RUDDER_MIN_LOWRATES":
                        new_value = self.ui.MinRudderServoValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "THROTTLE_MIN_ADC":
                        new_value = self.ui.MinThrottleADCValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "AILERON_MIN_ADC":
                        new_value = self.ui.MinAileronADCValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "ELEVATOR_MIN_ADC":
                        new_value = self.ui.MinElevatorADCValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "RUDDER_MIN_ADC":
                        new_value = self.ui.MinRudderADCValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "THROTTLE_MAX_ADC":
                        new_value = self.ui.MaxThrottleADCValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "AILERON_MAX_ADC":
                        new_value = self.ui.MaxAileronADCValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "ELEVATOR_MAX_ADC":
                        new_value = self.ui.MaxElevatorADCValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "RUDDER_MAX_ADC":
                        new_value = self.ui.MaxRudderADCValue.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "AILERON_REVERSE":
                        new_value = self.comboBoxIndicesBool[self.ui.AileronReverse.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "ELEVATOR_REVERSE":
                        new_value = self.comboBoxIndicesBool[self.ui.ElevatorReverse.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "RUDDER_REVERSE":
                        new_value = self.comboBoxIndicesBool[self.ui.RudderReverse.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "THROTTLE_REVERSE":
                        new_value = self.comboBoxIndicesBool[self.ui.ThrottleReverse.currentText()]
                        line = line.replace(setting_val, new_value)
            
            new_contents.append(line)

        new_contents = "".join(new_contents)

        with open(self.GSToolsFilename, 'w') as outFile:
            outFile.write(new_contents)

    def write_out_gs_serial(self):
        with open(self.GSSerialFilename, 'r') as inFile:
            contents = inFile.readlines()

        new_contents = []

        for line in contents:
            if len(line.split()) >= 3:
                define       = line.split()[0]
                setting_name = line.split()[1]
                setting_val  = line.split()[2]
                
                try:
                    comment  = line.split()[3]
                except:
                    pass
                
                if define == "#define":
                    if setting_name == "GS_DEBUG_PORT_NUMBER":
                        new_value = self.portComboBoxIndicesPortNum[self.ui.GSDebugPort.currentText()]
                        line = line.replace(setting_val, new_value)
    
                        try:
                            line = line.replace(comment, "//" + self.ui.GSDebugPort.currentText())
                        except:
                            pass
    
                    elif setting_name == "GS_COMMAND_PORT_NUMBER":
                        new_value = self.portComboBoxIndicesPortNum[self.ui.GSCommandPort.currentText()]
                        line = line.replace(setting_val, new_value)
    
                        try:
                            line = line.replace(comment, "//" + self.ui.GSCommandPort.currentText())
                        except:
                            pass
    
                    elif setting_name == "GS_TELEM_PORT_NUMBER":
                        new_value = self.portComboBoxIndicesPortNum[self.ui.GSTelemetryPort.currentText()]
                        line = line.replace(setting_val, new_value)
    
                        try:
                            line = line.replace(comment, "//" + self.ui.GSTelemetryPort.currentText())
                        except:
                            pass

            new_contents.append(line)

        new_contents = "".join(new_contents)

        with open(self.GSSerialFilename, 'w') as outFile:
            outFile.write(new_contents)

    def write_out_ifc_tools(self):
        with open(self.IFCToolsFilename, 'r') as inFile:
            contents = inFile.readlines()

        new_contents = []

        for line in contents:
            if len(line.split()) >= 3:
                define       = line.split()[0]
                setting_name = line.split()[1]
                setting_val  = line.split()[2]
                
                if define == "#define":
                    if setting_name == "LIDAR_FIXED_MOUNT":
                        new_value = self.comboBoxIndicesBool[self.ui.LiDARFixedMount.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "PITOT_PIN":
                        new_value = self.comboBoxIndicesAnalogPin[self.ui.PitotTubeAnalogPin.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "THROTTLE_PIN":
                        new_value = self.comboBoxIndicesServoPin[self.ui.ThrottlePin.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "R_AILERON_PIN":
                        new_value = self.comboBoxIndicesServoPin[self.ui.RightAileronPin.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "L_AILERON_PIN":
                        new_value = self.comboBoxIndicesServoPin[self.ui.LeftAileronPin.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "ELEVATOR_PIN":
                        new_value = self.comboBoxIndicesServoPin[self.ui.ElevatorPin.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "RUDDER_PIN":
                        new_value = self.comboBoxIndicesServoPin[self.ui.RudderPin.currentText()]
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "UNSAFE_ROLL_R":
                        new_value = self.ui.UnsafeRollRightAngle.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "UNSAFE_ROLL_L":
                        new_value = self.ui.UnsafeRollLeftAngle.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "MAX_ROLL_R":
                        new_value = self.ui.MaxRollRightAngle.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "MAX_ROLL_L":
                        new_value = self.ui.MaxRollLeftAngle.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "UNSAFE_PITCH_UP":
                        new_value = self.ui.UnsafePitchUpAngle.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "UNSAFE_PITCH_DOWN":
                        new_value = self.ui.UnsafePitchDownAngle.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "MAX_PITCH_UP":
                        new_value = self.ui.MaxPitchUpAngle.text()
                        line = line.replace(setting_val, new_value)
    
                    elif setting_name == "MAX_PITCH_DOWN":
                        new_value = self.ui.MaxPitchDownAngle.text()
                        line = line.replace(setting_val, new_value)

            new_contents.append(line)

        new_contents = "".join(new_contents)

        with open(self.IFCToolsFilename, 'w') as outFile:
            outFile.write(new_contents)

    def write_out_ifc_serial(self):
        with open(self.IFCSerialFilename, 'r') as inFile:
            contents = inFile.readlines()

        new_contents = []

        for line in contents:
            if len(line.split()) >= 3:
                define       = line.split()[0]
                setting_name = line.split()[1]
                setting_val  = line.split()[2]
                
                try:
                    comment  = line.split()[3]
                except:
                    pass
                
                if define == "#define":
                    if setting_name == "IFC_DEBUG_PORT_NUMBER":
                        new_value = self.portComboBoxIndicesPortNum[self.ui.IFCDebugPort.currentText()]
                        line = line.replace(setting_val, new_value)
    
                        try:
                            line = line.replace(comment, "//" + self.ui.IFCDebugPort.currentText())
                        except:
                            pass
    
                    elif setting_name == "IFC_COMMAND_PORT_NUMBER":
                        new_value = self.portComboBoxIndicesPortNum[self.ui.IFCCommandPort.currentText()]
                        line = line.replace(setting_val, new_value)
    
                        try:
                            line = line.replace(comment, "//" + self.ui.IFCCommandPort.currentText())
                        except:
                            pass
    
                    elif setting_name == "IFC_GPS_PORT_NUMBER":
                        new_value = self.portComboBoxIndicesPortNum[self.ui.IFCGPSPort.currentText()]
                        line = line.replace(setting_val, new_value)
    
                        try:
                            line = line.replace(comment, "//" + self.ui.IFCGPSPort.currentText())
                        except:
                            pass
    
                    elif setting_name == "IFC_TELEM_PORT_NUMBER":
                        new_value = self.portComboBoxIndicesPortNum[self.ui.IFCTelemetryPort.currentText()]
                        line = line.replace(setting_val, new_value)
    
                        try:
                            line = line.replace(comment, "//" + self.ui.IFCTelemetryPort.currentText())
                        except:
                            pass

            new_contents.append(line)

        new_contents = "".join(new_contents)

        with open(self.IFCSerialFilename, 'w') as outFile:
            outFile.write(new_contents)

    def close(self):
        exit()


def main():
    app = QApplication(sys.argv)
    w = AppWindow()
    w.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    try:
        main()
    except SystemExit:
        pass
