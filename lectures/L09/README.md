# L09 - SW/HW-integrationstestning (del II)

## Dagordning
* Automatisering av tester genom seriell kommunikation med hårdvaran.
* Demonstration av `serial::read()` för att ta emot kommandon från PC.
* Användning av Python-skript för att skicka kommandon och läsa svar från target.
* Implementera automatiserade tester med Python (för högre betyg).

## Mål med lektionen
* Förstå hur seriell kommunikation kan användas för testautomation.
* Kunna implementera kommandotolkning på embedded-sidan med `serial::read()`.
* Kunna skriva Python-skript för att kommunicera med hårdvara via seriell port.
* **Huvudmål**: Förstå grunderna i automatiserad testning av embedded-system och avsluta projektarbetet.

## Förutsättningar
* Genomförd L08 med fungerande manuell testning på target.
* Python 3 installerat på utvecklingsdatorn.
* Tillgång till Arduino/ATmega328P med fungerande firmware.

## Teori: Manuell vs automatiserad testning

### Manuell testning
* Kräver mänsklig interaktion (knapptryckningar, observation).
* Tidskrävande och repetitivt.
* Risk för mänskliga fel.
* Svårt att dokumentera och reproducera.

### Automatiserad testning
* Tester körs automatiskt via skript.
* Snabbare att köra upprepade tester.
* Konsistenta och reproducerbara resultat.
* Enklare att integrera i CI/CD-pipelines.
* **Nackdelar**: Kräver initial setup och underhåll av testskript.

## Instruktioner

### Förberedelser

#### 1. Installera Python och nödvändiga paket
Öppna Git Bash. Kolla om Python är installerat:

```bash
python --version
```

Om inte, installera Python via följande kommando, vilket öppnar Microsoft Store:

```bash
python
```

Installera nödvändiga paket:

```bash
python -m pip install pyserial pylint
```

#### 2. Hitta serieport
Identifiera vilken serieport Arduino är ansluten till (COM3, COM4, etc.), exempelvis via `Device Manager` (Enhetshanteraren).

### Implementera kommandotolkning på embedded-sidan

Anta att vi implementerar följande kommandon:
* `t`: Står för `toggle` och används för att toggla toggle-timern.
* `s`: Står för `status` och används för att läsa toggle-timerns tillstånd.
* `r`: Står för `read` och används för att läsa rumstemperaturen.

#### Lägg till kommandohantering i systemlogiken
**För högre betyg**: Implementera kommandohantering i er target-logik enligt exemplet nedan.
För att läsa data som skickas från er PC på target, använd `serial::read()` i 
[logic.cpp](../../library/source/logic/logic.cpp), exempelvis via en privat metod som anropas från
logik-loopen. Nedan visas ett exempel på en sådan metod:

```cpp
// -----------------------------------------------------------------------------
bool Logic::readSerialPort() noexcept
{
    // Set read timeout to 100 ms.
    constexpr uint16_t readTimeout_ms{100U};

    // Create a buffer with capacity for 10 bytes.
    constexpr uint16_t bufferSize{10U};
    uint8_t buffer[bufferSize]{};

    // Read the serial port, return false on failure.
    const int16_t bytesRead{mySerial.read(buffer, bufferSize, readTimeout_ms)};

    if (0 > bytesRead) 
    { 
        mySerial.printf("Failed to read serial data!\n"); 
        return false;
    }

    // Print received data if any.
    if (0 < bytesRead)
    {
        //! @todo Remove the next three lines in the real implementation!
        mySerial.printf("Received %d character(s) from the serial port: ", bytesRead);
        for (int16_t i{}; i < bytesRead; ++i) { mySerial.printf("%c", buffer[i]); }
        mySerial.printf("\n");

        // Check the first character to determine command:
        const char cmd{static_cast<char>(buffer[0U])};

        switch (cmd)
        {
            case 't':
                //! @todo Toggle the toggle timer here!
                break;
            case 's':
                //! @todo Print the state of the toggle timer here!
                break;
            case 'r':
                //! @todo Read and print the temperature here!
                break;
            default:
                // Return false to indicate invalid command.
                mySerial.printf("Unknown command %c!\n", cmd);
                return false;
        }
    }
    // Return true to indicate success.
    return true;
}
```

### Python-skript för testautomation

#### Python-implementation (för testning och debugging)
Skriptet [serial_demo.py](./serial_demo.py) låter er manuellt skicka kommandon och se svar i realtid, vilket är mycket användbart för att testa att er C++-implementation fungerar innan ni bygger automatiserade tester. Skriptet måste köras i Windows.

Kör skriptet med följande kommando i Git Bash:

```bash
python serial_demo.py
```

Använd `pylint` för att köra lint-analys på dina filer. Som exempel kan följande kommando användas
för att köra lint-analys på `serial_demo.py`:

```bash
python -m pylint serial_demo.py
```

Använd följande kommando för att lint-analysa alla filer i aktuell katalog:

```bash
python -m pylint .
```

***OBS!** Det går även att köra direkt i VS Code.*

### Uppgift: Implementera testautomation med klasser

**Mål**: Skapa ett objektorienterat Python-program som automatiserar testfallen från L08.

#### Krav för godkänd implementation (högre betyg):

1. **Separat katalog för seriella moduler**: Skapa en underkatalog döpt `uart` i katalogen
[test/scripts](../../library/test/scripts/) innehållande filerna `interface.py`, `driver.py`, `stub.py`, `factory.py` samt `__init__.py`.
2. **Interface**: Definiera ett interface döpt `IUart` i `interface.py` för seriell kommunikation med UART, innehållande gemensamma metoder oavsett operativsystem.
3. **Driver-klass för seriell kommunikation**: Implementera en driverklass döpt `UartDriver` i `driver.py`, som ärver från interfacet och hanterar seriell kommunikation när operativsystemet är Windows. Klassen ska automatiskt hitta rätt COM-port (där mikrokontrollern är ansluten).
4. **Stub-klass vid användning av Linux/Mac**: Implementera en stub-klass döpt `UartStub` i `stub.py`, som ärver från interfacet och simulerar seriell kommunikation när operativsystemet är Linux eller Mac.
5. **Factory-funktion**: Implementera en factory-funktion i `factory.py` som returnerar rätt implementation (`UartDriver` eller `UartStub`) baserat på parameter, se exempel nedan.
6. **Testimplementation**: Implementera era tester i [serial_test.py](../../library/test/scripts/serial_test.py). Använd factory-funktionen för att skapa seriell instans - använd `UartDriver` för Windows, `UartStub` för övriga operativsystem.
7. **Loggning till fil**: All seriell data (både kommandon och svar) ska loggas till en textfil för senare analys.
8. **Testmetoder**: Implementera metoder för varje testfall:
   - `test_toggle()` - Testa toggle-funktionalitet (toggling samt avläsning av toggle-timerns status).
   - `test_temperature()` - Testa temperaturavläsning.
   - `test_eeprom()` - Verifiera EEPROM-persistens.
9. **Automatisk verifiering**: Verifiera förväntade svar från target.
10. **Testrapport**: Generera en rapport med PASS/FAIL för varje test.
11. **Robust felhantering**: Hantera timeout, anslutningsfel, ogiltiga svar, etc. Systemet ska vara stabilt även vid fel.

#### Exempel

##### Factory-funktion

Använd gärna följande factory-implementation i `uart/factory.py`:

```python
"""Serial factory implementation."""
from .driver import UartDriver
from .interface import IUart
from .stub import UartStub

def create(baudrate: int = 9600, timeout_ms: int | float = 10, stub: bool = False) -> IUart:
    """Create serial instance.

    Args:
        baudrate: Baud rate in bps (default = 9600 bps).
        timeout_ms: Timeout in milliseconds (default = 10 ms).
        stub: True to create a stub (default = false).

    Returns:
        New serial instance.
    """
    if stub:
        return UartStub()
    return UartDriver(baudrate, timeout_ms)
```

I [serial_test.py](../../library/test/scripts/serial_test.py) kan factoryn användas såsom visas nedan. I detta exempel används `platform.system()` för att bestämma vilket operativsystem som används:

```python
import platform
from uart import factory

# Check OS, use stub on Linux/Mac, real driver on Windows.
use_stub = platform.system() != "Windows"

# Create UART instance, use default baud rate and timeout.
uart = factory.create(stub=use_stub)
```

#### Behandla katalogen `uart` som ett package

Lägg gärna till följande innehåll i filen `uart/__init__.py` efter att ha skapat samtliga moduler:

```python
"""Module initialization."""
from .driver import UartDriver
from .factory import create
from .interface import IUart
from .stub import UartStub

# Define public symbols exported by this package.
__all__ = ["UartDriver", "create", "IUart", "UartStub"]
```

#### Hard-reset för EEPROM test

För att starta om enheten för EEPROM-tester kan DTR-pinnen togglas, såsom visas nedan:

```python
import time
from serial import Serial

# Connect to Arduino Uno (adjust COM port as needed).
ser = Serial("COM3", 9600, timeout=1)

# Toggle DTR to trigger hardware reset (DTR is wired to RESET on Arduino Uno).
ser.dtr = False
time.sleep(0.1)
ser.dtr = True

# Give the Arduino Uno time to start up.
time.sleep(2.0)

# Close the connection.
ser.close()
```

### Tips för implementation

* **Interface**: Använd `abc.ABC` och `@abstractmethod` för att skapa ett interface i Python.
* **COM-port detektering**: Använd `serial.tools.list_ports` för att lista alla tillgängliga COM-portar.
* **Loggning**: Använd `open()` med mode `'a'` (append) för att skriva till en loggfil. Timestamp:a varje rad med `datetime.now()`.
* **Vänta på svar**: Använd `time.sleep()` eller `serial.in_waiting()` för att vänta på svar från target.
* **Parsing av svar**: Använd `strip()`, `split()` för att tolka svar från Arduino.
* **Timeout-hantering**: Använd attributet `timeout` i `Serial()`-konstruktorn.
* **Try-except**: Använd `try-except` för att hantera fel på ett robust sätt.
* **Debugging**: Skriv ut alla kommandon och svar för att underlätta felsökning.
* **Testsekvenser**: Testa en funktion i taget innan ni kombinerar tester.

### Exempel på testfall att automatisera

1. **Toggle-test**:
   - Skicka 't' för växla toggle-timern → verifiera att lysdioden börjar blinka samt att "Toggle timer enabled!" skrivs ut i terminalen.
   - Skicka 't' igen → verifiera att lysdioden släcks samt att "Toggle timer disabled!" skrivs ut i terminalen.

2. **Status-test**:
   - Skicka 's' → verifiera att toggle-timerns status skrivs ut i terminalen samt att den matchar aktuellt tillstånd på target.

3. **Temperatur-test**:
   - Skicka 'r' → verifiera att temperaturen skrivs ut i terminalen.

4. **EEPROM-test**:
   - Aktivera toggle → resetta Arduino → verifiera att toggle-timern är aktiv vid start.
   - Inaktivera toggle → resetta Arduino → verifiera att toggle-timern är inaktivt vid start.

5. **End-to-end scenario**:
   - Automatisera hela scenariot från L08 via Python-skript.

### Felsökning

#### Problem: Kan inte ansluta till seriell port
* **Lösning**: Kontrollera att rätt port hittas, exempelvis via en debug-utskrift. Stäng alla andra program som använder porten (seriell monitor, etc.).

#### Problem: Inget svar från Arduino
* **Lösning**: Verifiera att baud rate matchar (vanligen 9600 bps). Kontrollera att firmware är uppladdad och kör.

#### Problem: Trasiga tecken i svaret
* **Lösning**: Kontrollera baudrate och encoding (UTF-8).

#### Problem: Serial timeout
* **Lösning**: Öka timeout-värdet vid läsning.

## Utvärdering
* Lyckades ni automatisera testfallen med Python?
* Vilka fördelar och nackdelar upptäckte ni med automatiserad testning?
* Hur kan automatiserad testning förbättra utvecklingsprocessen?
* Vilka utmaningar fanns med implementationen?
* Hur skulle ni vidareutveckla testautomationen?
* Några övriga synpunkter på lektionen?

## Nästa lektion
* Slutförande av testsvit och kursavslutning.
