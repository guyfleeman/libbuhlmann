# 

## Build Instructions

```
mkdir build
cd build
cmake ..
make <target>
```

## Build Targets

### DSO Core

`make libbuhlmann`

### Planning Testing

`make planning_testing`

### Additional Targets

You can make additional executable targets by coping the planning testing section, and changing the names.

## Compartment Data

## Background

## Format

`<time in seconds> <C0> ... <C15>`

Compartment loading values range from 0.0 to 2.0, where 0.0 represents an underloaded compartment, 1.0 represents a saturated compartment, and 2.0 represents a maximally overloaded compartment. We should visualize this with a column of squares per time step, colored based on compartment load. This can be done in 2D gl without perspective.

An example file is located in the data folder.

