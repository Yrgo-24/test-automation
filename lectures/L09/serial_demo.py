"""Serial communication with ATmega328p via UART."""
import time
from serial import Serial
from serial.tools import list_ports as serial_ports

def get_com_port() -> str | None:
    """Get COM port of Arduino Uno.
    
    Returns:
        COM port connected to Arduino Uno as a string, or none if not found.
    """
    # Iterate through all COM ports, return port name if Arduino Uno is found.
    for port in serial_ports.comports():
        # Check if port description contains "arduino" or "uno".
        desc = (port.description or "").lower()
        if "arduino" in desc or "uno" in desc:
            return port.device
        # If that didn't match, check if the hardware ID matches Arduino Uno.
        # Note: SB VID:PID for Arduino ID is 2341:0043.
        hw_id = (port.hwid or "").lower()
        if "vid:pid=2341:0043" in hw_id:
            return port.device
    # Return none if no Arduino Uno was found.
    return None

def main():
    """Communicate with ATmega328p via UART."""
    com_port = get_com_port()

    if com_port is None:
        raise RuntimeError("Arduino Uno not found on any COM port!")

    # Connect to Arduino Uno (adjust COM port as needed).
    with Serial(com_port, 9600, timeout=1) as ser:

        # Wait for the Arduino to start.
        time.sleep(2)

        # Read startup responses.
        print("Reading startup responses from Arduino:")
        while ser.in_waiting > 0:
            msg = ser.readline().decode("utf-8", errors="replace").strip()
            if len(msg) > 0:
                print(f"{msg}")
        print("\nArduino tester ready. Commands: t, s, r, q (quit)")

        # Continuously monitor user input.
        while True:
            # Read commands entered by the user, terminate the program if 'q' is received.
            cmd = input("Enter command: ").strip()
            if cmd == 'q':
                print("Terminating the program!")
                break

            # Send data to the Arduino, encoded as bytes.
            ser.write(cmd.encode())

            # Wait for response with timeout.
            timeout = 2.0
            start_time = time.time()

            # Read any messages from Arduino.
            while (time.time() - start_time) < timeout:
                if ser.in_waiting > 0:
                    msg = ser.readline().decode("utf-8", errors="replace").strip()
                    if len(msg) > 0:
                        print(f"Received from Arduino: {msg}")
                else:
                    # Generate short sleep to avoid busy-waiting.
                    time.sleep(0.1)

# Invoke the main function if this is the startup script.
if __name__ == "__main__":
    main()
