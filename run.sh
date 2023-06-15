#!/bin/bash
meson compile -C build && cd build && ninja && ./pgl_demo
