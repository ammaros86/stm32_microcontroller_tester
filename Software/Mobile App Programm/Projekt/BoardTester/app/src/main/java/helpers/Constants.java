package helpers;

public class Constants {
    public static final int BoardCompleteTestedValue = 511;
    public static final int BoardNotTestedValue = 0;
    public static final int ProcessDelay = 200;

    public static final int ReadAppCommandDelay = 100;
    public static final int ReadAppControllerDelay = 100;

    public static final int BoardGpioContinuityTestValuePosition = 0;
    public static final int BoardCurrentTestValuePosition = 1;
    public static final int BoardPowerGndPinsTestValuePosition = 2;
    public static final int BoardFlashTestValuePosition = 3;
    public static final int BoardTemperatureTestValuePosition = 4;
    public static final int BoardGpioInputTestValuePosition = 5;
    public static final int BoardGpioOutputTestValuePosition = 6;
    public static final int BoardAnalogInputTestValuePosition =7;
    public static final int BoardGpioShortCircuitTestValuePosition = 8;

    public static final int UploadTestProgramSuccess = 1;
    public static final int UploadTestProgramFail = 0;

    public static final int FlashTestDone = 1;
    public static final int FlashTestFail = 2;


    public static final String ExceptionNoData = "No Data";
    public static final String ExceptionValueNull = "Value null";
    public static final String PA2 = "PA2";
    public static final String PA3 = "PA3";
    public static final String PF0 = "PF0";
    public static final String PC14 = "PC14";
    public static final String PC15 = "PC15";
    public static final String PA13 = "PA13";
    public static final String PA14 = "PA14";
    public static final String PC13 = "PC13";

    public static final int GpioInputHighOk = 1;
    public static final int GpioInputLowOk = 0;
    public static final int GpioOutputSize = 51;

    public static final int BoardNotConnectedValue = 15;

    public static final int GpioOutputTestSpeedLow = 1;
    public static final int GpioOutputTestSpeedMedium = 2;
    public static final int GpioOutputTestSpeedHigh = 3;

    public static final int maxBoardTemperature = 86;
    public static final String newServerWaitText = "New Server Configuration \n IP-Address: ";
    public static final String waitText = "\n Please wait...";

    //LOG FILES LIST
    public static final String urlLogFilesSuffix = "/PhpFiles/Log/list_log_files.php";

    //TEST RESULTS
    public static final String urlBoardGpioContinuitySuffix = "/PhpFiles/TestResults/gpio_continuity.php";
    public static final String urlCurrentConsumptionSuffix = "/PhpFiles/TestResults/board_current.php";
    public static final String urlBoardPowerVoltageSuffix = "/PhpFiles/TestResults/board_power_voltage.php";
    public static final String urlBoardFlashSuffix = "/PhpFiles/TestResults/board_flash.php";
    public static final String urlBoardTemperatureSuffix = "/PhpFiles/TestResults/board_temperature.php";
    public static final String urlBoardGpioInputNoPullSuffix = "/PhpFiles/TestResults/gpio_input_no_pull.php";
    public static final String urlBoardGpioInputPullDownSuffix = "/PhpFiles/TestResults/gpio_input_pull_down.php";
    public static final String urlBoardGpioInputPullUpSuffix = "/PhpFiles/TestResults/gpio_input_pull_up.php";

    public static final String urlBoardGpioOutputNoPullSuffix = "/PhpFiles/TestResults/gpio_output_no_pull.php";
    public static final String urlBoardGpioOutputPullDownSuffix = "/PhpFiles/TestResults/gpio_output_pull_down.php";
    public static final String urlBoardGpioOutputPullUpSuffix = "/PhpFiles/TestResults/gpio_output_pull_up.php";
    public static final String urlBoardGpioShortCircuitSuffix = "/PhpFiles/TestResults/gpio_short_circuit.php";
    public static final String urlBoardAnalogInputSuffix = "/PhpFiles/TestResults/analog_input.php";

    public static final String urlBoardCommandStateSuffix = "/PhpFiles/readTestCommands.php";
    public static final String urlReadBoardController= "/PhpFiles/readControllerCommands.php";
    public static final String urlBoardCommandSuffix = "/PhpFiles/updateTestCommand.php";
    public static final String urlBoardControllerSuffix= "/PhpFiles/updateControllerCommand.php";

    public static final String urlAddNewBoardSuffix = "/PhpFiles/addNewBoard.php";



    public static final String addBoardTag = "AddBoardTag";
    public static final String removeBoardTag = "RemoveBoardTag";
    public static final String removeLogTag = "RemoveLogTag";

    public static final String urlRemoveBoardSuffix = "/PhpFiles/remove_board.php";

    public static final String urlRemoveLogSuffix = "/PhpFiles/Log/remove_log_file.php";

    //Flash Result
    public static final String flashKey = "FLASH";

    //Power Pins Result
    public static final String boardID = "BOARD_ID";
    public static final String BOARD_NAME_KEY = "BOARD_NAME";

    public static final int boardID_Default = -1;

    public static final String powerPinsVoltageKey = "POWER_PINS_VOLTAGE";

    // GPIO Continuity
    public static final String gpioContinuityKey = "GPIO_CONTINUITY";

    // GPIO Input
    public static final String gpioInputNoPullKey = "GPIO_INPUT_NO_PULL";
    public static final String gpioInputPullDownKey = "GPIO_INPUT_PULL_DOWN";
    public static final String gpioInputPullUpKey = "GPIO_INPUT_PULL_UP";

    //GPIO Output
    public static final String gpioOutputNoPullKey = "GPIO_OUTPUT_NO_PULL";
    public static final String gpioOutputPullDownKey = "GPIO_OUTPUT_PULL_DOWN";
    public static final String gpioOutputPullUpKey = "GPIO_OUTPUT_PULL_UP";
    public static final String gpioShortCircuitKey = "GPIO_SHORT_CIRCUIT";

    //Analog Input
    public static final String analogInputKey = "ANALOG_INPUT";

    public static final String vinPin = "VIN";
    public static final String gnd1Pin = "GND1";
    public static final String aGndPin = "AGND";
    public static final String gnd2Pin = "GND2";
    public static final String gnd3Pin = "GND3";
    public static final String gnd4Pin = "GND4";
    public static final String gnd5Pin = "GND5";
    public static final String gnd6Pin = "GND6";
    public static final String aVddPin = "AVDD";
    public static final String vddPin = "VDD";
    public static final String vBatPin = "VBAT";
    public static final String iOrefPin = "IOREF";
    public static final String p3VPin = "3.3V";
    public static final String pU5VPin = "U5V";
    public static final String p5VPin = "5V";

    public static final String e5VPin = "E5V";


    public static final String SuccessKey = "success";
    public static final String BoardAdded = "done";
    public static final String BoardCommandsTable = "board_commands";
    public static final String BoardFlashTable = "board_flash";
    public static final String BoardTemperatureTable = "board_temperature";
    public static final String BoardInputNoPullTable = "gpio_input_no_pull";
    public static final String BoardInputPullDownTable = "gpio_input_pull_down";
    public static final String BoardInputPullUpTable = "gpio_input_pull_up";
    public static final String BoardOutputNoPullTable = "gpio_output_no_pull";
    public static final String BoardOutputPullDownTable = "gpio_output_pull_down";
    public static final String BoardOutputPullUpTable = "gpio_output_pull_up";
    public static final String BoardGpioShortCircuitTable = "gpio_short_circuit";
    public static final String BoardPowerGndPinsTable = "power_gnd_pins";
    public static final String AppControllerRequestTag = "AppControllerRequestTag";

    public static final String UrlPrefix = "http://";
    public static final String ControllerTag = "BoardControllerTag";
    public static final String LogFilesTag = "LogFilesTag";

    public static final String TemperatureTag = "TemperatureTag";
    public static final String CurrentMeasureTag = "CurrentMeasureTag";

    public static final String FlashTag = "FlashTag";
    public static final String PowerPinsVoltageTag = "PowerPinsVoltageTag";
    public static final String GpioContinuityTag = "GpioContinuityTag";

    public static final String GpioInputNoPullTag = "GpioInputNoPullTag";
    public static final String GpioInputPullDownTag = "GpioInputPullDownTag";
    public static final String GpioInputPullUpTag = "GpioInputPullUpTag";
    public static final String GpioOutputNoPullTag = "GpioOutputNoPullTag";
    public static final String GpioOutputPullDownTag = "GpioOutputPullDownTag";
    public static final String GpioOutputPullUpTag = "GpioOutputPullUpTag";
    public static final String AnalogInputTag = "AnalogInputTag";
    public static final String GpioShortCircuitTag = "GpioShortCircuitTag";

    public static final String BoardIdKey = "board_id";
    public static final String DateFormat = "yyyy-MM-dd HH:mm:ss";

    public static final String NotTestableGpioPins = "- PA2, PA3, PA13, PA14, PC14, PC15 and PF0.\n- PC13 is connected to Pull-Up resistance (User Button) and  tested only in No-Pull-Mode" ;
    //    public static final String NotTestableGpioOutputPins = "- PA2 and PA3 used for UART communication. \n- PA13 and PA14 share with SWD signals connected to ST-LINK/V2-1\n- PC14, PC15 and PF0 are not connected. \n- PC13 is connected to Pull-Up resistance (User Button) and tested only in No-Pull-Mode";
    public static final String NotTestableGpioShort = "- PA2, PA3, PA13, PA14, PC14, PC15 and PF0.";

    public static final String FailedTest = "Test Failed";

    public static final String GPIO_OUTPUT_OK = "OK";

    public static final String WAIT_TEXT = "please wait...";
    public static final String COMMUNICATION_FAILURE = "Communication Failure";
    public static final String EXPORT_SUCCESS = "Export success";

    public static final String USB_MEDIA_ERROR = "No USB-Media detected";
    public static final String USB_PORT_STATE_OFF = "OFF";
    public static final String USB_PORT_STATE_ON = "ON";
    public static final String FAILED_USB_PORT = "failed tro turn USB Port On";
    public static final String POWER_STATE_ON = "ON";
    public static final String POWER_STATE_OFF = "OFF";

    public static final String CONNECTED = "Connected";
    public static final String CONNECTION_FAILED = "Disconnected or connection failed";
    public static final String USB_CONNECTION_ERROR = "Check USB-Connection";
    public static final String PHYSICAL_CONNECTION_ERROR = "Check Physical Connection";

    public static final int GPIO_SHORT_MULTIPLEXERS_ERROR = -2 ;
    public static final int GPIO_SHORT_NOT_TESTABLE = -3 ;

    public static double outputMaxLowVoltageDefault = 1.3;
    public static double outputMinHighVoltageDefault = 2;

}
