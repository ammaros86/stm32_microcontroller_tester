  ✔ Development of a Testing System for STM32 Microcontrollers ✔


During my master's thesis at the Berliner Hochschule für Technik, the focus was on developing a testing system for STM32 microcontrollers, particularly the STM32-Nucleo-Board F303RE.
The primary goal was to identify defects in these single-board computers, which are used as programmable control units in various fields such as control engineering and measurement technology.

Students utilize these microcontrollers for writing, testing, and developing new programs. However, their application and usage can lead to functional impairments and defects. Undetected defects can create uncertainties among students, questioning whether occurring errors are attributable to their custom software or potential hardware issues.

<p align="center">
  <img src="https://github.com/ammaros86/stm32_microcontroller_tester/assets/56800295/4d68d2fe-d07b-4d6e-a9be-8d518043daf1" />
</p>

To address these challenges, I developed an embedded testing system capable of quickly and reliably detecting defects on the STM32-F303RE board. The testing system allows for a comprehensive examination of pins, flash memory, power consumption, and the temperature of the microcontroller. Additionally, it can verify the continuity of GPIOs. The results are logged in files on the device and can also be stored in a database.
The testing device is designed to be user-friendly, featuring an LCD display and control buttons. To simplify handling and result evaluation, a web server, a database, and an Android app were developed. 
The hardware involves the creation of two boards. The main board integrates various components such as analog-to-digital converters, digital-to-analog converters, a constant current source, and multiple analog bidirectional multiplexers. The software was implemented in programming languages including C++, C, Java, and PHP, utilizing three interfaces (UART, SPI, I2C).

The developed testing device has been successfully implemented at the the Berliner Hochschule für Technik and is currently being utilized by laboratory engineers to conduct regular and thorough inspections of the Nucleo boards.

 
On the LCD screen, 15 entries are displayed. From these, the user can select and execute a specific function using either the buttons or the app. Let me elaborate on these commands:

- New Board: This command should be selected and executed before connecting a Nucleo board to the test device. It's designed to prevent short circuits when inserting the Nucleo board into the test device's connector. During this process, the interrupts of the four buttons on the test device and other device functions are disabled. Once the Nucleo board is inserted, the red button should be continuously pressed for at least three seconds to reactivate the interrupts and proceed with the tests. Following this, the test device conducts a check to ensure the proper connection of the Nucleo board. This includes verifying the USB connection and the physical connection. To accomplish this, the PCB incorporates pull-down resistors on the Nucleo board's supply pins and pull-up resistors on its GND pins. Voltage measurements on these pins can confirm the correct connection of the Nucleo board and verify interconnections of the GND pins.

- Complete Test: This allows the user to conduct a comprehensive test, where all available tests are performed sequentially.

- Continuity Test: Also known as contact testing, this test is used to identify connection issues between an ATE tester and a test object. It involves connecting all supply pins, the USB connection, and the ground of the test device. A small current, typically 100 μA, is then applied to the pin using a constant current source while measuring the voltage at the pin. In a fault-free condition, the voltage drop at the tested pin typically ranges between 550 and 750 mV. A short circuit between the tested pin and ground results in a measured voltage drop of 0 V, while an open circuit leads to a measured voltage close to the programmed terminal voltage of the power source.

- Power Pins Test: This test measures the voltage at the supply and ground pins of the Nucleo board using analog-to-digital converters.

- Flash Test: This test is conducted in two stages. Firstly, a test program is transferred to the flash memory of the Nucleo board, and the results are checked for any issues. This test program enables the test device to control the Nucleo board for the remaining tests. Subsequently, the Nucleo board receives a command via the UART interface to check the flash memory. During the flash memory check, a data pattern is written to the free cells of the flash memory, read, and then erased. The results of this process are then examined for any errors in writing, reading, or erasing. Finally, the Nucleo board sends a confirmation to the test device with the test results.

- Board Current Test: The current consumption of the Nucleo board is measured using a current sensor INA219, which is controlled via I2C. A two-pin connector on the test device is used to connect to the pins of the JP5 connector of the Nucleo board using a provided cable. This connection is utilized to measure the microcontroller's current consumption. In case of increased current consumption, the USB connection of the board is turned off to protect the test device from damage.

- Temperature: The Nucleo board can operate in a temperature range of -40°C to 85°C and features an integrated temperature sensor. It has been observed that boards with significant power consumption and defects exhibit increased temperature. Therefore, temperature measurement aids in detecting defective boards. The test device sends a command to measure the temperature to the Nucleo board. After measurement, the Nucleo board sends the measured value to the test device. In case of increased temperature, the USB connection of the board is turned off to protect the test device from damage.

- Digital Input Test: These three procedures validate the proper functioning of a digital input and the pull resistors. This test confirms whether a GPIO can be successfully initialized as a digital input and whether the level state behaves correctly with the applied voltages at the GPIO pin. The applied voltages at the inputs are generated using a digital-to-analog converter. These voltages correspond to the threshold voltages U_IH and U_IL of the GPIOs of the Nucleo board. While the threshold voltage U_IH represents the minimum voltage at a digital input that can be considered a high level, the threshold voltage U_IL represents the maximum voltage at a digital input that can be considered a low level. All digital inputs undergo three rounds of testing: once with the activation of the internal pull resistors, once with the activation of the internal pull-up resistors, and once with the activation of the pull-down resistors. Each time, the Nucleo Board receives a command to configure a digital input. After applying the voltage to the pin, the status of the digital input is read and sent to the test device.

- Digital Output Test: In this test, each GPIO undergoes 9 iterations of testing. Similar to the digital input test, each GPIO is examined with various pull resistor configurations. Furthermore, for each test, the output speed of the GPIO is adjusted differently. This entails configuring specific pull and speed settings for each test and measuring the voltage at the GPIOs using the digital-to-analog converter.

- Short Circuits Test: This test aims to detect any short circuits between two or more GPIOs. For each iteration of this test, a GPIO pin (designated as pin X) is selected, configured as an output, and set to a high level. Subsequently, the voltage on the remaining pins is measured. If another GPIO pin registers a high level, it is recorded as a potential indication of a short circuit. Following this, GPIO pin X is set to a low level, and the voltage on the remaining suspected pins is measured. If these pins also register a low level, it suggests a short circuit between pin X and the suspected GPIOs.

- Analog Pins Test: The Nucleo Board boasts 22 analog pins, each of which can be tested using digital-to-analog converters. For this test, all 22 pins are configured as analog inputs. The test device then sequentially applies five different voltages to each input pin. Subsequently, the measured values are transmitted to the test device for comparison with expected values, allowing for the identification of any discrepancies.

- Server IP Address: The IP address of the web server is essential for establishing a connection between the Android app and the test device. Upon executing this command, the IP address is promptly displayed on the LCD screen.

- Copy Log Files: Log files containing test results from both button and app-initiated tests can be transferred to a USB stick using this command.

- Turn USB ON: If the test device detects elevated temperature or power consumption from a Nucleo board, its USB ports are disabled to prevent potential damage. Users can reactivate the USB ports with this command.

- Power OFF: This command initiates the shutdown process for the Raspberry Pi operating system. However, to completely power off the test device, the physical switch must also be set to the 0 position.

