# Stackmat Timer for Arduino

A library for reading the RS-232 Serial output from a 
Stackmat Timer (also known as Speedcubing Timers)

## Features

* Gets the current state of the timer
* Gets the current time of the timer
* Can output interpolated time between timer updates, great for displays showing the current time!

## How to use

See the included example!

## States

`ST_Reset` = Timer was reset
`ST_Running` = Timer is currently running
`ST_LeftHandOnTimer` = Left hand is on timer (Not supported by all timers!)
`ST_RightHandOnTimer` = Right hand is on timer (Not supported by all timers!)
`ST_BothHandsOnTimer` = Both hand is on timer, preparing to start (Not supported by all timers!)
`ST_ReadyToStart` = Ready to start (Not supported by all timers!)
`ST_Stopped` = The timer has been stopped

## Known working timers

### Qiyi Speedcubing Timer 

Works well, but only reports `ST_Reset` and `ST_Running` (But also supports `ST_Stopped`)
Powered by 2 AAA batteries, signal voltage is around +3v
