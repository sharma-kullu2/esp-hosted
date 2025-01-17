### Directory structure for Linux based host

```
├── linux
│   ├── host_control            (Contain necessary files for installation of control path)
│   │   ├── c_support           (Contain files to test basic control path commands and stress.c for stress testing)
│   │   ├── python_support      (Contain python scripts for Wi-Fi functionality,
│   │   │                           `test.py` to test basic control path commands, stress.py for stress testing, also
│   │   │                           `commands_lib.py` control path commands implementation using
│   │   │                           `ctypes` module converts command requests from python to c and maps command responses from c to python)
│   │   ├── rpi_init.sh         (Installation sequence for ESP-Hosted driver)
│   │   └── spidev_disabler.dts (dts file for SPI transport)
│   └── host_driver             (Contain ESP-Hosted kernel module files)
│       └── esp32               (ESP-hosted kernel module files)
│           ├── sdio            (Contain SDIO transport files used by kernel module to communicate
│           │                       with ESP peripheral)
│           └── spi             (Contain SPI transport files used by kernel module to communicate
│                                   with ESP peripheral)
```
