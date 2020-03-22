#!/usr/bin/env python3

import time
import pygame

pygame.joystick.init()

try:
    controller = pygame.joystick.Joystick(0)
    controller.init()
    print("Name of gamepad: ", controller.get_name())
    print("Number of buttons: ", controller.get_numbuttons())
    print("Number of axes: ", controller.get_numaxes())
except pygame.error:
    print("Gamepad not connected.")

while controller.get_init():
    print(controller.get_axis(0))

    time.sleep(0.05)