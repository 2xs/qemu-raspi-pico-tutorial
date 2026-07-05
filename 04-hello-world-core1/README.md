# Step 4: Build and Run `hello_world` from Core 1

In this step you will build a small Pico SDK program that starts both RP2040
cores. Core 0 prints one message, core 1 prints another message, and both
messages are visible on the emulated UART0 console.

By the end of the step you will have checked that QEMU can run a simple
SDK-based multicore Pico 1 firmware.

## What This Step Teaches

You will learn how to:

- write a minimal SDK program using `pico_multicore`;
- start core 1 with `multicore_launch_core1()`;
- build the program as an ELF and UF2 image;
- run the multicore firmware on QEMU;
- observe output produced by both RP2040 cores.

## Assumptions

This step assumes that you completed:

- [Step 1](../01-install-qemu-pipico1/README.md), so QEMU is available at
  `$HOME/src/qemu-rp2040-pico/build/qemu-system-arm`;
- [Step 2](../02-install-pico-sdk/README.md), so `PICO_SDK_PATH` is set;
- [Step 3](../03-hello-world-uart0/README.md), so you already know how UART0
  output appears in the QEMU terminal.

The tutorial repository is expected at:

```text
$HOME/src/qemu-rp2040-pico-tutorial
```

## Inspect the Example

The example project lives in this step directory:

```text
04-hello-world-core1/hello-core1/
```

The important SDK call is:

```c
multicore_launch_core1(core1_entry);
```

The two core entry paths print:

```c
puts("hello from core 0");
puts("hello from core 1");
```

This intentionally stays small. `puts()` goes through the Pico SDK stdio path,
which is enough for this tutorial step. The goal here is not to benchmark
locking or scheduling; it is to see both emulated cores run SDK code.

## Copy the Example

Copy the project next to the SDK and examples:

```sh
cp -R "$HOME/src/qemu-rp2040-pico-tutorial/04-hello-world-core1/hello-core1" \
  "$HOME/src/hello-core1"
cd "$HOME/src/hello-core1"
```

## Build the Firmware

Configure and build the project:

```sh
cmake -G Ninja -DPICO_BOARD=pico -S . -B build
cmake --build build
```

Expected output files include:

```text
build/hello_core1.elf
build/hello_core1.uf2
```

## Run the ELF Image

Run the ELF firmware with QEMU:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico \
  -kernel "$HOME/src/hello-core1/build/hello_core1.elf" \
  -nographic
```

Expected output:

```text
hello from core 0
hello from core 1
```

The order of the two lines is not important. Once core 1 is launched, both
cores run independently, so either message may appear first.

Stop QEMU with:

```text
Ctrl-a x
```

## Run the UF2 Image

Run the same firmware as a UF2 image:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" \
  -machine raspi-pico \
  -kernel "$HOME/src/hello-core1/build/hello_core1.uf2" \
  -nographic
```

Expected output is again:

```text
hello from core 0
hello from core 1
```

Congratulations! You have now started both RP2040 cores under QEMU.

Next step: keep data in the emulated external flash across QEMU runs. Go to
[`05-reboot-counter-flash`](../05-reboot-counter-flash/README.md).

## Troubleshooting

### `pico/multicore.h` Is Not Found

Check that `PICO_SDK_PATH` is set:

```sh
echo "$PICO_SDK_PATH"
```

If it is empty, run:

```sh
export PICO_SDK_PATH="$HOME/src/pico-sdk"
```

Then rerun CMake from a clean build directory.

### Only One Line Appears

Make sure you are running the `hello_core1` firmware built in this step, not
the UART-only firmware from Step 3:

```sh
ls "$HOME/src/hello-core1/build/hello_core1.elf"
```

Also check that your QEMU branch contains RP2040 dual-core support:

```sh
"$HOME/src/qemu-rp2040-pico/build/qemu-system-arm" -machine help | grep raspi-pico
```

### QEMU Does Not Exit with `Ctrl-C`

With `-nographic`, `Ctrl-C` is sent to the guest. Use the QEMU escape sequence:

```text
Ctrl-a x
```
