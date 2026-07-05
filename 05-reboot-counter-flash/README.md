# Step 5: Keep Data in the Emulated Flash

In this step you will use QEMU's `flash-file=...` machine option. This option
backs the Pico 1 external flash with a raw host file, so the emulated flash can
survive after QEMU exits.

The step has two parts:

- first, you will run the Step 4 multicore firmware from a persistent flash
  file and reboot it without passing `-kernel` again;
- then, you will build a small `reboot_counter` firmware that stores a counter
  in flash and shows the difference between reinstalling a firmware image and
  rebooting the existing flash contents.

## What This Step Teaches

You will learn how to:

- create a raw erased-flash image for QEMU;
- install an ELF or UF2 image into that flash with `-kernel`;
- reboot the same flash contents without `-kernel`;
- write data from Pico SDK firmware into the emulated flash;
- observe which data is persistent and which data is reset by reinstalling the
  firmware image.

## Assumptions

This step assumes that you completed:

- [Step 1](../01-install-qemu-pipico1/README.md), so QEMU is available at
  `$HOME/src/qemu-rp2040-pico/build/qemu-system-arm`;
- [Step 2](../02-install-pico-sdk/README.md), so `PICO_SDK_PATH` is set;
- [Step 4](../04-hello-world-core1/README.md), so the multicore example is
  available at `$HOME/src/hello-core1/build/hello_core1.elf`.

The tutorial repository is expected at:

```text
$HOME/src/qemu-rp2040-pico-tutorial
```

## Part 1: Install a Firmware into a Raw Flash File

Create a working directory for this step:

```sh
mkdir -p "$HOME/src/pico-flash-persistence"
cd "$HOME/src/pico-flash-persistence"
```

Create a 2 MiB raw flash file filled with the erased NOR value, `0xff`:

```sh
python3 - <<'PY'
from pathlib import Path
Path("flash.bin").write_bytes(b"\xff" * (2 * 1024 * 1024))
PY
```

Now run the Step 4 multicore program with both `flash-file=flash.bin` and
`-kernel`:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico,flash-file=flash.bin \
  -kernel "$HOME/src/hello-core1/build/hello_core1.elf" \
  -nographic
```

Expected output:

```text
hello from core 0
hello from core 1
```

Stop QEMU with:

```text
Ctrl-a x
```

When `flash-file=...` and `-kernel ...` are used together, QEMU first loads
`flash.bin`, then installs the kernel image into the emulated flash, then writes
the updated flash contents back to `flash.bin`.

Installing the kernel image does not erase the whole flash file. QEMU only
overwrites the flash bytes covered by the ELF segments or UF2 blocks. Bytes
outside the loaded image keep their previous value from `flash.bin`.

Run QEMU again with the same flash file, but without `-kernel`:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico,flash-file=flash.bin \
  -nographic
```

Expected output is again:

```text
hello from core 0
hello from core 1
```

This time no new firmware image was installed. QEMU simply booted from the
contents already present in `flash.bin`. This is similar to rebooting a physical
Pico without pressing the BOOTSEL button.

## Part 2: Build the Reboot Counter

Copy the counter example next to your other Pico projects:

```sh
cp -R "$HOME/src/qemu-rp2040-pico-tutorial/05-reboot-counter-flash/reboot-counter" \
  "$HOME/src/reboot-counter"
cd "$HOME/src/reboot-counter"
```

Build it:

```sh
cmake -G Ninja -DPICO_BOARD=pico -S . -B build
cmake --build build
```

Expected output files include:

```text
build/reboot_counter.elf
```

If your SDK environment has `picotool` available, the build may also produce
`build/reboot_counter.uf2`. This step uses the ELF image, so UF2 generation is
not required here.

If CMake tries to download `picotool` and your machine has no network access,
configure the build without the UF2 post-processing step:

```sh
cmake -G Ninja -DPICO_BOARD=pico -DPICO_NO_PICOTOOL=1 -S . -B build
cmake --build build
```

The program contains a dedicated flash sector with an initial counter value.
At runtime it:

- reads the counter from that flash sector;
- prints the next counter value;
- erases the sector;
- writes the updated value back to flash.

The firmware uses the Pico SDK `flash_range_erase()` and
`flash_range_program()` functions.

The counter sector is part of the firmware image on purpose. This makes the
effect of `-kernel` visible: reinstalling the ELF rewrites that sector to its
initial value, while rebooting without `-kernel` keeps the value written by the
previous run.

## Reinstalling the Firmware Resets the Counter

Create a fresh flash file:

```sh
cd "$HOME/src/pico-flash-persistence"
python3 - <<'PY'
from pathlib import Path
Path("counter-flash.bin").write_bytes(b"\xff" * (2 * 1024 * 1024))
PY
```

Install and run the counter firmware:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico,flash-file=counter-flash.bin \
  -kernel "$HOME/src/reboot-counter/build/reboot_counter.elf" \
  -nographic
```

Expected output:

```text
reboot counter: 1
```

Stop QEMU with `Ctrl-a x`, then run the same command again with `-kernel`:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico,flash-file=counter-flash.bin \
  -kernel "$HOME/src/reboot-counter/build/reboot_counter.elf" \
  -nographic
```

Expected output is again:

```text
reboot counter: 1
```

This happens because `-kernel` reinstalls the firmware image into the flash
file before the emulated Pico boots. QEMU does not erase the whole flash file:
it overwrites only the bytes covered by the ELF or UF2 image. In this example,
the firmware image deliberately contains the initial counter sector, so
reinstalling the image also rewrites that sector and resets the counter.

## Rebooting Without `-kernel` Preserves the Counter

Now install the firmware once:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico,flash-file=counter-flash.bin \
  -kernel "$HOME/src/reboot-counter/build/reboot_counter.elf" \
  -nographic
```

Stop QEMU with `Ctrl-a x`.

Reboot from the same flash file without `-kernel`:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico,flash-file=counter-flash.bin \
  -nographic
```

Expected output:

```text
reboot counter: 2
```

Stop QEMU, then run the same command once more, still without `-kernel`:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico,flash-file=counter-flash.bin \
  -nographic
```

Expected output:

```text
reboot counter: 3
```

You are now using the raw host file as the Pico's persistent external flash.
The firmware image is installed only when you pass `-kernel`; otherwise, QEMU
boots from whatever the emulated flash contained at the end of the previous
run.

Congratulations! You have now installed Pico firmware into an emulated flash
file and observed persistent flash writes across QEMU reboots.

## Troubleshooting

### QEMU Cannot Load `flash.bin`

The `flash-file=...` option expects an existing raw file. Create it first:

```sh
python3 - <<'PY'
from pathlib import Path
Path("flash.bin").write_bytes(b"\xff" * (2 * 1024 * 1024))
PY
```

### The Counter Always Prints `1`

Check whether you are still passing `-kernel`. Passing `-kernel` reinstalls the
firmware image before boot, so this example starts again from the initial
counter sector.

To test persistence, run with `flash-file=counter-flash.bin` but without
`-kernel`.

### QEMU Does Not Exit with `Ctrl-C`

With `-nographic`, `Ctrl-C` is sent to the guest. Use the QEMU escape sequence:

```text
Ctrl-a x
```
