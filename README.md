# PunkMuff
 [![PunkMuff multiplatform build](https://github.com/gmoican/PunkMuff/actions/workflows/main.yml/badge.svg)](https://github.com/gmoican/PunkMuff/actions/workflows/main.yml)

![DemoImage](docs/images/demo.png)

## Introduction
This is a simple vst3/au distortion plugin made with [JUCE](https://juce.com/) that I built to introduce myself in the JUCE framework. The functionality is somewhat inspired by the [Big Muff Pi](https://www.electrosmash.com/big-muff-pi-analysis), but I implemented my own processing without trying out the original pedal.

## Features
- **Mode**: Slightly adjusts the internal behaviour of the clipping processor:
    - Mode 1: Standard Big Muff Pi behaviour.
    - Mode 2: Hizumitas mod ? ? ?
    - Mode 3: ... ? ? ?
- **Sustain**: Sets the gain before the clipping processing.
- **Tone**: Adjust the frequency response after the clipping, check the referenced Big Muff analysis for a detailed graph.
- **Level**: Adjust the output gain.

## Interesting links

* [Big Muff Pi Analysis](https://www.electrosmash.com/big-muff-pi-analysis)