EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "OSKAR III Mainboard"
Date "2021-01-18"
Rev "a"
Comp "Science Centre AHHAA Foundation / SA Teaduskeskus AHHAA"
Comment1 "Rando Hinn"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATmega:ATmega2561-16AU U?
U 1 1 6004DA6C
P 5750 3625
F 0 "U?" H 6225 1675 50  0000 C CNN
F 1 "ATmega2561-16AU" H 6525 1575 50  0000 C CNN
F 2 "Package_QFP:TQFP-64_14x14mm_P0.8mm" H 5750 3625 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf" H 5750 3625 50  0001 C CNN
	1    5750 3625
	1    0    0    -1  
$EndComp
$Comp
L Interface_CAN_LIN:MCP2515-xSO U?
U 1 1 60052217
P 9925 5450
F 0 "U?" H 9925 6431 50  0000 C CNN
F 1 "MCP2515-xSO" H 9925 6340 50  0000 C CNN
F 2 "Package_SO:SOIC-18W_7.5x11.6mm_P1.27mm" H 9925 4550 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21801e.pdf" H 10025 4650 50  0001 C CNN
	1    9925 5450
	1    0    0    -1  
$EndComp
$Comp
L Interface_CAN_LIN:MCP2551-I-SN U?
U 1 1 60053ED2
P 9950 1450
F 0 "U?" H 9950 2031 50  0000 C CNN
F 1 "MCP2551-I-SN" H 9950 1940 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 9950 950 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/devicedoc/21667d.pdf" H 9950 1450 50  0001 C CNN
	1    9950 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	10525 4850 10575 4850
Wire Wire Line
	9450 1350 9400 1350
Wire Wire Line
	9400 1250 9450 1250
Wire Wire Line
	10525 4950 10575 4950
Text Label 10575 4850 0    50   ~ 0
RXCAN
Text Label 10575 4950 0    50   ~ 0
TXCAN
Text Label 9400 1350 2    50   ~ 0
RXCAN
Text Label 9400 1250 2    50   ~ 0
TXCAN
Wire Wire Line
	10525 6050 10875 6050
$Comp
L Device:R_Small R?
U 1 1 60055B84
P 10875 5900
F 0 "R?" H 10934 5946 50  0000 L CNN
F 1 "10k" H 10934 5855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 10875 5900 50  0001 C CNN
F 3 "~" H 10875 5900 50  0001 C CNN
	1    10875 5900
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 60056047
P 10875 5750
F 0 "#PWR?" H 10875 5600 50  0001 C CNN
F 1 "+5V" H 10890 5923 50  0000 C CNN
F 2 "" H 10875 5750 50  0001 C CNN
F 3 "" H 10875 5750 50  0001 C CNN
	1    10875 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	10875 5750 10875 5800
Wire Wire Line
	10875 6000 10875 6050
$Comp
L Device:R_Small R?
U 1 1 60056B65
P 5075 1775
F 0 "R?" H 5016 1729 50  0000 R CNN
F 1 "10k" H 5016 1820 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5075 1775 50  0001 C CNN
F 3 "~" H 5075 1775 50  0001 C CNN
	1    5075 1775
	1    0    0    1   
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 60056F51
P 5075 1625
F 0 "#PWR?" H 5075 1475 50  0001 C CNN
F 1 "+5V" H 5090 1798 50  0000 C CNN
F 2 "" H 5075 1625 50  0001 C CNN
F 3 "" H 5075 1625 50  0001 C CNN
	1    5075 1625
	1    0    0    -1  
$EndComp
Wire Wire Line
	5075 1925 5075 1875
Wire Wire Line
	5075 1625 5075 1675
Wire Wire Line
	5075 1925 5150 1925
Wire Wire Line
	5750 5625 5750 5675
$Comp
L power:GND #PWR?
U 1 1 600585F2
P 5750 5675
F 0 "#PWR?" H 5750 5425 50  0001 C CNN
F 1 "GND" H 5755 5502 50  0000 C CNN
F 2 "" H 5750 5675 50  0001 C CNN
F 3 "" H 5750 5675 50  0001 C CNN
	1    5750 5675
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal_Small Y?
U 1 1 60058D21
P 8700 6075
F 0 "Y?" V 8625 6175 50  0000 L CNN
F 1 "16MHz" V 8700 6175 50  0000 L CNN
F 2 "" H 8700 6075 50  0001 C CNN
F 3 "~" H 8700 6075 50  0001 C CNN
	1    8700 6075
	0    1    1    0   
$EndComp
Wire Wire Line
	9325 5650 9275 5650
Wire Wire Line
	9325 5750 9275 5750
Text Label 9275 5650 2    50   ~ 0
OSC2
Text Label 9275 5750 2    50   ~ 0
OSC1
Wire Wire Line
	8700 5925 8625 5925
Wire Wire Line
	8700 5925 8700 5975
Wire Wire Line
	8700 6175 8700 6225
Wire Wire Line
	8700 6225 8625 6225
Wire Wire Line
	8700 5925 8750 5925
Connection ~ 8700 5925
Wire Wire Line
	8700 6225 8750 6225
Connection ~ 8700 6225
Text Label 8750 5925 0    50   ~ 0
OSC2
Text Label 8750 6225 0    50   ~ 0
OSC1
$Comp
L Device:C_Small C?
U 1 1 6005B015
P 8525 5925
F 0 "C?" V 8296 5925 50  0000 C CNN
F 1 "C_Small" V 8387 5925 50  0000 C CNN
F 2 "" H 8525 5925 50  0001 C CNN
F 3 "~" H 8525 5925 50  0001 C CNN
	1    8525 5925
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 6005B0EB
P 8525 6225
F 0 "C?" V 8650 6225 50  0000 C CNN
F 1 "C_Small" V 8725 6125 50  0000 C CNN
F 2 "" H 8525 6225 50  0001 C CNN
F 3 "~" H 8525 6225 50  0001 C CNN
	1    8525 6225
	0    -1   1    0   
$EndComp
Wire Wire Line
	8425 5925 8375 5925
Wire Wire Line
	8375 5925 8375 6225
Wire Wire Line
	8375 6225 8425 6225
Wire Wire Line
	8375 6225 8375 6275
Connection ~ 8375 6225
$Comp
L power:GND #PWR?
U 1 1 6005C82D
P 8375 6275
F 0 "#PWR?" H 8375 6025 50  0001 C CNN
F 1 "GND" H 8380 6102 50  0000 C CNN
F 2 "" H 8375 6275 50  0001 C CNN
F 3 "" H 8375 6275 50  0001 C CNN
	1    8375 6275
	1    0    0    -1  
$EndComp
Wire Notes Line
	8175 6525 8175 4300
Wire Notes Line
	8175 4300 11200 4300
Text Notes 8225 4425 0    50   ~ 0
CAN Controller and associated circuitry
$EndSCHEMATC
