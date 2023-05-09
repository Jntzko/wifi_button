#!/bin/bash

# Is NetworkManager installed
if ! command -v nmcli &> /dev/null
then
    echo "NetworkManager is not installed."
    exit
fi

# Test if Hotspot is already active
if nmcli device status | grep "Hotspot" &> /dev/null; then
    # Deactivate Hotspots
    nmcli radio wifi off
    # Connect to wifi
    nmcli radio wifi on
    echo "Hotspot deactivated and wifi restored."
else
    # Activate Hotspot
    nmcli device wifi hotspot ssid "tableWifi" password "1234Blumenk0hl"
    echo "Hotspot is activated."
fi

