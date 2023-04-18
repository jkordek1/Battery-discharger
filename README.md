# Battery discharger

[![Version](https://img.shields.io/github/v/release/jkordek1/Battery-discharger)](https://github.com/jkordek1/Man-Don-t-Get-Angry-Board-game/releases/tag/Initial)
[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Issues](https://img.shields.io/github/issues/jkordek1/Battery-discharger)](https://github.com/jkordek1/Battery-discharger/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/jkordek1/Battery-discharger)](https://github.com/jkordek1/Battery-discharger/pulls)
 
 
## Why do you need this?

If you have a lot of 18650 Li-Ion battery cells that you won't be using any time soon you need to discharge them. In order for cells to stay healthy and remain their capacirty, they need to be stored at 50% of capacity (3.7 - 3.8V).
This device discharges the batteries to the storage level voltage.

## What is it?

 Automated battery discharger for Li-Po and Li-Ion batteries powered by ATmega328p microcontroller.
 Used for safely discharging 4 single-cell 18650 batteries to a storage level voltage (3.7V).
 No need for external power, the circuit is powered from cell #1.
 
 First push button switches between voltage and current monitoring.
 Second push button is used for manual voltage level measurement and for deactivating the buzzer.
 
 The code is written in Arduino and is available to download.
 
 The device is functional but could be improved.
 
 ## TO-DO
 - implement voltage level selection via push buttons 
 
 
## Images
<p align="center">
  <img width="600" src="https://raw.githubusercontent.com/jkordek1/Battery-discharger/main/Images/Front.jpg">
</p>
