EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
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
P 8250 2425
F 0 "U?" H 8250 3406 50  0000 C CNN
F 1 "MCP2515-xSO" H 8250 3315 50  0000 C CNN
F 2 "Package_SO:SOIC-18W_7.5x11.6mm_P1.27mm" H 8250 1525 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21801e.pdf" H 8350 1625 50  0001 C CNN
	1    8250 2425
	1    0    0    -1  
$EndComp
$Comp
L Interface_CAN_LIN:MCP2551-I-SN U?
U 1 1 60053ED2
P 9975 2025
F 0 "U?" H 9975 2606 50  0000 C CNN
F 1 "MCP2551-I-SN" H 9975 2515 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 9975 1525 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/devicedoc/21667d.pdf" H 9975 2025 50  0001 C CNN
	1    9975 2025
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 1825 8900 1825
Wire Wire Line
	9475 1925 9425 1925
Wire Wire Line
	9425 1825 9475 1825
Wire Wire Line
	8850 1925 8900 1925
Text Label 8900 1825 0    50   ~ 0
RXCAN
Text Label 8900 1925 0    50   ~ 0
TXCAN
Text Label 9425 1925 2    50   ~ 0
RXCAN
Text Label 9425 1825 2    50   ~ 0
TXCAN
Wire Wire Line
	8850 3025 9200 3025
$Comp
L Device:R_Small R?
U 1 1 60055B84
P 9200 2875
F 0 "R?" H 9259 2921 50  0000 L CNN
F 1 "10k" H 9259 2830 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 9200 2875 50  0001 C CNN
F 3 "~" H 9200 2875 50  0001 C CNN
	1    9200 2875
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 60056047
P 9200 2725
F 0 "#PWR?" H 9200 2575 50  0001 C CNN
F 1 "+5V" H 9215 2898 50  0000 C CNN
F 2 "" H 9200 2725 50  0001 C CNN
F 3 "" H 9200 2725 50  0001 C CNN
	1    9200 2725
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 2725 9200 2775
Wire Wire Line
	9200 2975 9200 3025
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
$EndSCHEMATC
