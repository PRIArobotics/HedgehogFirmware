#!/bin/bash

# set STM NRST
raspi-gpio set 5 op dh
# set STM NRST enable
raspi-gpio set 26 op dh
# force enable power
raspi-gpio set 23 op dh
# force enable rpi regulator
raspi-gpio set 24 op dh

sudo openocd -f openocd/openocd_hedgehog_swd.cfg -c "program $1 0x8000000 verify; reset run; exit"

# reset STM NRST enable
raspi-gpio set 26 op dl

sleep 2

# reset STM NRST
raspi-gpio set 5 op dh
# force enable power
raspi-gpio set 23 op dl
# force enable rpi regulator
raspi-gpio set 24 op dl

