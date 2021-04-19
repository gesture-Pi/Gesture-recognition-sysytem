# Gesture-Pi

<p align="center">
  <a href="https://www.bilibili.com/video/BV1sA411V7Yw/">
    <img src="Images/Bilibili.png" alt="Bilibili"  width="64" height="64">
  </a>
  </a>
  <a href="https://twitter.com/Caiwentao12345">
    <img src="Images/twitter hover_pc.png" alt="Twitter"  width="64" height="64">
  </a>
  </a>
  <a href="https://www.youtube.com/watch?v=zLDupBEazR8">
    <img src="Images/youtube.png" alt="youtube"  width="64" height="64">
  </a>
  


## Table of Contents
- [Abstract]( #Abstract)
- [Introduction]( #Introduction)
- [Install](#install)
- [Usage](#usage)
	- [Generator](#generator)
- [Badge](#badge)
- [Example Readmes](#example-readmes)
- [Related Efforts](#related-efforts)
- [Maintainers](#maintainers)
- [Contributing](#contributing)
- [License](#license)

## Abstract

The gesture recognition system designed in this paper takes STM32 single chip microcomputer and raspberry pi as the core control unit, 8 copper clad plates as sensors to obtain the capacitance corresponding to the gesture, and uses TI company's sensor chip FDC2214 to detect the capacitance, and uses machine learning KNN (proximity) algorithm for data training and testing to finally recognize the specified gesture. Through the key module and OLED display to achieve human-computer interaction function, it can realize the function of key parameter adjustment and mode switching. The model of server and client based on TCP protocol enables users of raspberry pi to communicate with each other. The whole system involves gesture recognition test structure design, system circuit design, data transmission, data storage and KNN algorithm. Through a large number of tests, the advantages and disadvantages of different schemes are compared, and the existing appearance test structure and control system structure are finally determined. The experimental results show that the design of gesture recognition system is feasible.

## Introduction

Gestrue Pi uses capacitor boards as a sensor to recognize the voltage and current changes when users make different gestures, and then transmits the data to raspberry pi, which is used for gesture recognition and communication with other devices.
When we can achieve this, it will be able to do many things. For example, two people can play with Rock-Paper-Scissors remotely and enjoy an immersive experience through raspberry pi.

## Algorithm

KNN (K-Nearest Neighbor) method, originally proposed by Cover and Hart in 1968, is a relatively mature method in theory and one of the simplest machine learning algorithms.The idea of this method is very simple and intuitive: if most of the K most similar (that is, the closest in the feature space) samples of a sample belong to a certain category, then the sample also belongs to this category.In the classification decision, the method only determines the category of the samples to be divided according to the category of the nearest one or several samples.

## Hardware

FDC2214 sensor
STM32Fxx(we use STM32F103C8T6here)
an OLED screen
rocker botton or replaced by several bottons
raspberry Pi

## Prerequisites

This project consists of two parts: the STM32 and the raspberry pi program. The former uses Keil4 or 5 to run, which requires basic librabry, and it can be found in libraries file. Apart from that, the FDC sensor should be connected in the right pin, just as the program shown.超链接 

While the raspberry pi would need to fix the serial port in ttyAMA0 which would be relatively stable than the other.

This project uses pthread. Go check if you don't have them locally installed.

```sh
$ sudo apt-get install pthread
```

Besides, the chip should be connected in the right way of raspberry pi: GND - pin6, RXD - pin8, TXD - pin10.

## Usage

ALL the file have been integrated in Makefile, you can just make and then run in the raspberrypi.

```sh
$ make
$ ./Pisever
```
